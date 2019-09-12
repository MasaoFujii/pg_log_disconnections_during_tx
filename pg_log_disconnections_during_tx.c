/*-------------------------------------------------------------------------
 *
 * pg_log_disconnections_during_tx.c
 *	  Enables PostgreSQL to log normal disconnections during transaction
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/xact.h"
#include "executor/executor.h"
#include "libpq/libpq-be.h"
#include "miscadmin.h"
#include "storage/ipc.h"
#include "utils/guc.h"

PG_MODULE_MAGIC;

/* GUC Variables */
static bool	log_disconnections_during_tx_enabled = false;

/* Saved hook values in case of unload */
static ExecutorStart_hook_type prev_ExecutorStart = NULL;

void		_PG_init(void);
void		_PG_fini(void);

static void	this_ExecutorStart(QueryDesc *queryDesc, int eflags);
static void	log_disconnections_during_tx(int code, Datum arg);


/*
 * Module load callback
 */
void
_PG_init(void)
{
	/* Define custom GUC variables. */
	DefineCustomBoolVariable("pg_log_disconnections_during_tx.enabled",
							 "Log normal disconnections during transaction.",
							 NULL,
							 &log_disconnections_during_tx_enabled,
							 false,
							 PGC_SUSET,
							 0,
							 NULL,
							 NULL,
							 NULL);

	EmitWarningsOnPlaceholders("pg_log_disconnections_during_tx");

	/* Install hooks. */
	prev_ExecutorStart = ExecutorStart_hook;
	ExecutorStart_hook = this_ExecutorStart;
}

/*
 * Module unload callback
 */
void
_PG_fini(void)
{
	/* Uninstall hooks. */
	ExecutorStart_hook = prev_ExecutorStart;
}

/*
 * ExecutorStart hook
 */
static void
this_ExecutorStart(QueryDesc *queryDesc, int eflags)
{
	if (prev_ExecutorStart)
		prev_ExecutorStart(queryDesc, eflags);
	else
		standard_ExecutorStart(queryDesc, eflags);

	before_shmem_exit(log_disconnections_during_tx, 0);
}

/*
 * Backend-shutdown callback to log end of session during transaction.
 */
static void
log_disconnections_during_tx(int code, Datum arg)
{
	Port	   *port = MyProcPort;
	long		secs;
	int			usecs;
	int			msecs;
	int			hours,
				minutes,
				seconds;

	if (!log_disconnections_during_tx_enabled)
		return;

	if (!IsTransactionBlock())
		return;

	TimestampDifference(MyStartTimestamp,
						GetCurrentTimestamp(),
						&secs, &usecs);
	msecs = usecs / 1000;

	hours = secs / SECS_PER_HOUR;
	secs %= SECS_PER_HOUR;
	minutes = secs / SECS_PER_MINUTE;
	seconds = secs % SECS_PER_MINUTE;

	ereport(LOG,
			(errmsg("disconnection with an open transaction: "
					"session time: %d:%02d:%02d.%03d "
					"user=%s database=%s host=%s%s%s",
					hours, minutes, seconds, msecs,
					port->user_name, port->database_name, port->remote_host,
					port->remote_port[0] ? " port=" : "", port->remote_port)));
}
