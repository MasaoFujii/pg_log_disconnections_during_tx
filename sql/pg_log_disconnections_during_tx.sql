\connect

BEGIN;
\connect

BEGIN;
SELECT 1;
\connect

BEGIN;
SELECT 1;
SELECT 1;
\connect

BEGIN;
SELECT 1;
SELECT 1;
COMMIT;
\connect

BEGIN;
SAVEPOINT xx;
\connect

BEGIN;
SAVEPOINT xx;
SELECT 1;
\connect

BEGIN;
SAVEPOINT xx;
SELECT 1;
COMMIT;
\connect
