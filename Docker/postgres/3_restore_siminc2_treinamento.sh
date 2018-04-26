#!/bin/bash
pg_restore --host localhost --port 5432 --username "postgres" --dbname "siminc2_desenvolvimento" --disable-triggers -O -x --verbose /var/www/bkp_prod_dbsiminc.backup

# Copia o arquivo do servidor para a pasta local
# scp adm_rafael@10.0.0.185:/home/bkp_siminc/bkp_prod_dbsiminc.backup /home/minc/dumps/restore-siminc2-tr

# # Criar banco siminc2_new
# psql --host 192.168.15.12 --port 5432 --username "postgres" -c "CREATE DATABASE siminc2_new;"

# psql --host 192.168.15.12 --port 5432 --username "postgres" -c "ALTER DATABASE siminc2_new SET datestyle TO European;"


# Executar criação de auditoria e mudança de senhas, emails
# psql --host 192.168.15.12 --port 5432 --username "postgres" --dbname "siminc2_new" -f /home/minc/dumps/restore-siminc2-tr/create_auditoria.sql

# Executar permissões nas tabelas
# psql --host 192.168.15.12 --port 5432 --username "postgres" --dbname "siminc2_new" -f /home/minc/dumps/restore-siminc2-tr/grants.sql

# Finalizar processos de banco e Mudar nome de bases
# psql --host 192.168.15.12 --port 5432 --username "postgres" -c "
# SELECT
#     pg_terminate_backend(pid)
# FROM
#     pg_stat_activity
# WHERE
#     datname = 'siminc2_treinamento';

# SELECT
#     pg_terminate_backend(pid)
# FROM
#     pg_stat_activity
# WHERE
#     datname = 'siminc2_new';

# ALTER DATABASE siminc2_treinamento RENAME TO siminc2_treinamento_bkp;
# ALTER DATABASE siminc2_new RENAME TO siminc2_treinamento;
# "

# Deleta base de dados de backup
# psql --host 192.168.15.12 --port 5432 --username "postgres" -c "DROP DATABASE siminc2_treinamento_bkp;"