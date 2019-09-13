# pg_log_disconnections_during_tx
PostgreSQL extension to log disconnection during transaction.

pg_log_disconnections_during_tx is released under the [PostgreSQL License](https://opensource.org/licenses/postgresql), a liberal Open Source license, similar to the BSD or MIT licenses.

## Test Status
[![Build Status](https://travis-ci.org/MasaoFujii/pg_log_disconnections_during_tx.svg?branch=master)](https://travis-ci.org/MasaoFujii/pg_log_disconnections_during_tx)

## Install

Download the source archive of pg_log_disconnections_during_tx from
[here](https://github.com/MasaoFujii/pg_log_disconnections_during_tx),
and then build and install it.

    $ cd pg_log_disconnections_during_tx
    $ make USE_PGXS=1 PG_CONFIG=/opt/pgsql-X.Y.Z/bin/pg_config
    $ su
    # make USE_PGXS=1 PG_CONFIG=/opt/pgsql-X.Y.Z/bin/pg_config install
    # exit

USE_PGXS=1 must be always specified when building this extension.
The path to [pg_config](http://www.postgresql.org/docs/devel/static/app-pgconfig.html)
(which exists in the bin directory of PostgreSQL installation)
needs be specified in PG_CONFIG.
However, if the PATH environment variable contains the path to pg_config,
PG_CONFIG doesn't need to be specified.

## Configure

[shared_preload_libraries](http://www.postgresql.org/docs/devel/static/runtime-config-client.html#GUC-SHARED-PRELOAD-LIBRARIES)
or [session_preload_libraries](http://www.postgresql.org/docs/devel/static/runtime-config-client.html#GUC-SESSION-PRELOAD-LIBRARIES)
(available in PostgreSQL 9.4 or later) must be set to 'pg_log_disconnections_during_tx'
in postgresql.conf
if you want to use the configuration parameters which this extension provides.

## Configuration Parameters

### pg_log_disconnections_during_tx.enabled (boolean)
Causes session terminations to be logged **onlly if transaction is in progress**.
The log output provides same information as
[log_disconnections](https://www.postgresql.org/docs/devel/runtime-config-logging.html#GUC-LOG-DISCONNECTIONS).
Only superusers can change this parameter. The default is off.

## Copyright
Copyright (c) 2019, Fujii Masao
