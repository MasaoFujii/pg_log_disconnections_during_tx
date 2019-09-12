MODULE_big = pg_log_disconnections_during_tx
OBJS = pg_log_disconnections_during_tx.o

PGFILEDESC = "pg_log_disconnections_during_tx - log normal disconnections during transaction"

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pg_log_disconnections_during_tx
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
