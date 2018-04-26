FROM culturagovbr/salic-db

COPY ./bkp_prod_dbsiminc.backup /var/www/

COPY ./1_create_db.sql /docker-entrypoint-initdb.d/
COPY ./2_create_users.sql /docker-entrypoint-initdb.d/
COPY ./3_restore_siminc2_treinamento.sh /docker-entrypoint-initdb.d/
COPY ./4_create_auditoria.sql /docker-entrypoint-initdb.d/
COPY ./5_grants.sql /docker-entrypoint-initdb.d/

