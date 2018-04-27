#!/bin/bash
pg_restore --host localhost --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" --disable-triggers -O -x --verbose /var/www/bkp_prod_dbsiminc.backup
