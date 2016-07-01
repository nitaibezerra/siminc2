#!/bin/bash

TAB='\t';
NULO='\\N';
LIDOS='lidos/';
LENDO='Lendo/';
SQLCOPY='sqlCopy/';
FILE=$1;

cp ${FILE} ${LENDO}${FILE};
cd ${LENDO}

echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.ORGAOGESTAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.UGSUBORGAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.DEPOSITOBANCARIO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.DESTINACAOGR.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.GRUPOFINANCEIRO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.RECOLHIMENTOUG.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.DOMICILIOBANCARIOCREDOR.sql;

echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.orgaogestao ( it_in_operacao, it_co_usuario, it_da_operacao, gr_orgao, it_co_gestao, it_in_cod_bb_gr ) FROM stdin;" >> ${FILE}.ORGAOGESTAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.ugsuborgao ( it_in_operacao, it_co_usuario, it_da_operacao, it_co_suborgao, it_in_unidade_gestora ) FROM stdin;" >> ${FILE}.UGSUBORGAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.depositobancario (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_co_ug_gestao_deposito_tipo_dbuq,${TAB}it_no_deposito,${TAB}it_tx_descricao_deposito,${TAB}it_in_depositante,${TAB}it_in_inclusao_codigo_stn,${TAB}it_in_alteracao_campo_1,${TAB}it_in_alteracao_campo_2,${TAB}it_in_alteracao_campo_3,${TAB}it_in_alteracao_campo_4,${TAB}it_in_alteracao_campo_5,${TAB}it_in_alteracao_campo_6,${TAB}it_in_alteracao_campo_7,${TAB}it_in_alteracao_campo_8,${TAB}it_in_alteracao_campo_9,${TAB}it_in_alteracao_campo_10,${TAB}it_in_alteracao_campo_11,${TAB}it_in_alteracao_campo_12,${TAB}gr_codigo_evento_1,${TAB}gr_codigo_evento_2,${TAB}gr_codigo_evento_3,${TAB}gr_codigo_evento_4,${TAB}gr_codigo_evento_5,${TAB}gr_codigo_evento_6,${TAB}gr_codigo_evento_7,${TAB}gr_codigo_evento_8,${TAB}gr_codigo_evento_9,${TAB}gr_codigo_evento_10,${TAB}gr_codigo_evento_11,${TAB}gr_codigo_evento_12,${TAB}it_co_inscricao1_1,${TAB}it_co_inscricao1_2,${TAB}it_co_inscricao1_3,${TAB}it_co_inscricao1_4,${TAB}it_co_inscricao1_5,${TAB}it_co_inscricao1_6,${TAB}it_co_inscricao1_7,${TAB}it_co_inscricao1_8,${TAB}it_co_inscricao1_9,${TAB}it_co_inscricao1_10,${TAB}it_co_inscricao1_11,${TAB}it_co_inscricao1_12,${TAB}it_co_inscricao2_1,${TAB}it_co_inscricao2_2,${TAB}it_co_inscricao2_3,${TAB}it_co_inscricao2_4,${TAB}it_co_inscricao2_5,${TAB}it_co_inscricao2_6,${TAB}it_co_inscricao2_7,${TAB}it_co_inscricao2_8,${TAB}it_co_inscricao2_9,${TAB}it_co_inscricao2_10,${TAB}it_co_inscricao2_11,${TAB}it_co_inscricao2_12,${TAB}gr_classificacao1_1,${TAB}gr_classificacao1_2,${TAB}gr_classificacao1_3,${TAB}gr_classificacao1_4,${TAB}gr_classificacao1_5,${TAB}gr_classificacao1_6,${TAB}gr_classificacao1_7,${TAB}gr_classificacao1_8,${TAB}gr_classificacao1_9,${TAB}gr_classificacao1_10,${TAB}gr_classificacao1_11,${TAB}gr_classificacao1_12,${TAB}gr_classificacao2_1,${TAB}gr_classificacao2_2,${TAB}gr_classificacao2_3,${TAB}gr_classificacao2_4,${TAB}gr_classificacao2_5,${TAB}gr_classificacao2_6,${TAB}gr_classificacao2_7,${TAB}gr_classificacao2_8,${TAB}gr_classificacao2_9,${TAB}gr_classificacao2_10,${TAB}gr_classificacao2_11,${TAB}gr_classificacao2_12,${TAB}it_in_favorecido_1,${TAB}it_in_favorecido_2,${TAB}it_in_favorecido_3,${TAB}it_in_favorecido_4,${TAB}it_in_favorecido_5,${TAB}it_in_favorecido_6,${TAB}it_in_favorecido_7,${TAB}it_in_favorecido_8,${TAB}it_in_favorecido_9,${TAB}it_in_favorecido_10,${TAB}it_in_favorecido_11,${TAB}it_in_favorecido_12,${TAB}it_co_favorecido_1,${TAB}it_co_favorecido_2,${TAB}it_co_favorecido_3,${TAB}it_co_favorecido_4,${TAB}it_co_favorecido_5,${TAB}it_co_favorecido_6,${TAB}it_co_favorecido_7,${TAB}it_co_favorecido_8,${TAB}it_co_favorecido_9,${TAB}it_co_favorecido_10,${TAB}it_co_favorecido_11,${TAB}it_co_favorecido_12,${TAB}it_in_fracionamento_1,${TAB}it_in_fracionamento_2,${TAB}it_in_fracionamento_3,${TAB}it_in_fracionamento_4,${TAB}it_in_fracionamento_5,${TAB}it_in_fracionamento_6,${TAB}it_in_fracionamento_7,${TAB}it_in_fracionamento_8,${TAB}it_in_fracionamento_9,${TAB}it_in_fracionamento_10,${TAB}it_in_fracionamento_11,${TAB}it_in_fracionamento_12,${TAB}it_va_fracionamento_1,${TAB}it_va_fracionamento_2,${TAB}it_va_fracionamento_3,${TAB}it_va_fracionamento_4,${TAB}it_va_fracionamento_5,${TAB}it_va_fracionamento_6,${TAB}it_va_fracionamento_7,${TAB}it_va_fracionamento_8,${TAB}it_va_fracionamento_9,${TAB}it_va_fracionamento_10,${TAB}it_va_fracionamento_11,${TAB}it_va_fracionamento_12,${TAB}it_in_limite_vinculacao_1,${TAB}it_in_limite_vinculacao_2,${TAB}it_in_limite_vinculacao_3,${TAB}it_in_limite_vinculacao_4,${TAB}it_in_limite_vinculacao_5,${TAB}it_in_limite_vinculacao_6,${TAB}it_in_limite_vinculacao_7,${TAB}it_in_limite_vinculacao_8,${TAB}it_in_limite_vinculacao_9,${TAB}it_in_limite_vinculacao_10,${TAB}it_in_limite_vinculacao_11,${TAB}it_in_limite_vinculacao_12,${TAB}it_co_inscricao_maquina_1,${TAB}it_co_inscricao_maquina_2,${TAB}it_co_inscricao_maquina_3,${TAB}it_co_inscricao_maquina_4,${TAB}it_co_inscricao_maquina_5,${TAB}it_co_inscricao_maquina_6,${TAB}it_co_inscricao_maquina_7,${TAB}it_co_inscricao_maquina_8,${TAB}it_co_inscricao_maquina_9,${TAB}it_co_inscricao_maquina_10,${TAB}it_co_inscricao_maquina_11,${TAB}it_co_inscricao_maquina_12 ) FROM stdin;" >> ${FILE}.DEPOSITOBANCARIO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.destinacaogr (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_co_destinacao_gr,${TAB}it_no_destinacao_gr,${TAB}it_no_reduzido_destinacao_gr,${TAB}it_in_tipo_destinacao,${TAB}it_co_grupo_destinacao,${TAB}gr_fonte_recurso,${TAB}it_in_tipo_beneficiario,${TAB}it_co_ug_beneficiario,${TAB}it_co_gestao_beneficiario,${TAB}gr_codigo_evento_resti_fonte,${TAB}gr_codigo_evento_resti_dest,${TAB}it_tx_motivo,${TAB}it_pe_destinacao_principal,${TAB}it_pe_destinacao_secundaria_1,${TAB}it_pe_destinacao_secundaria_2,${TAB}it_pe_destinacao_secundaria_3,${TAB}it_pe_destinacao_secundaria_4,${TAB}it_pe_destinacao_secundaria_5,${TAB}it_pe_destinacao_secundaria_6,${TAB}it_pe_destinacao_secundaria_7,${TAB}it_pe_destinacao_secundaria_8,${TAB}it_pe_destinacao_secundaria_9,${TAB}it_pe_destinacao_secundaria_10,${TAB}it_co_destinacao_secundaria_1,${TAB}it_co_destinacao_secundaria_2,${TAB}it_co_destinacao_secundaria_3,${TAB}it_co_destinacao_secundaria_4,${TAB}it_co_destinacao_secundaria_5,${TAB}it_co_destinacao_secundaria_6,${TAB}it_co_destinacao_secundaria_7,${TAB}it_co_destinacao_secundaria_8,${TAB}it_co_destinacao_secundaria_9,${TAB}it_co_destinacao_secundaria_10,${TAB}gr_codigo_evento_arrec_fonte_1,${TAB}gr_codigo_evento_arrec_fonte_2,${TAB}gr_codigo_evento_arrec_fonte_3,${TAB}gr_codigo_evento_arrec_fonte_4,${TAB}gr_codigo_evento_arrec_fonte_5,${TAB}gr_codigo_evento_arrec_fonte_6,${TAB}gr_codigo_evento_arrec_fonte_7,${TAB}gr_codigo_evento_arrec_fonte_8,${TAB}gr_codigo_evento_arrec_fonte_9,${TAB}gr_codigo_evento_arrec_fonte_10,${TAB}gr_codigo_evento_arrec_dest_1,${TAB}gr_codigo_evento_arrec_dest_2,${TAB}gr_codigo_evento_arrec_dest_3,${TAB}gr_codigo_evento_arrec_dest_4,${TAB}gr_codigo_evento_arrec_dest_5,${TAB}gr_codigo_evento_arrec_dest_6,${TAB}gr_codigo_evento_arrec_dest_7,${TAB}gr_codigo_evento_arrec_dest_8,${TAB}gr_codigo_evento_arrec_dest_9,${TAB}gr_codigo_evento_arrec_dest_10,${TAB}gr_codigo_evento_retif_fonte_1,${TAB}gr_codigo_evento_retif_fonte_2,${TAB}gr_codigo_evento_retif_fonte_3,${TAB}gr_codigo_evento_retif_fonte_4,${TAB}gr_codigo_evento_retif_fonte_5,${TAB}gr_codigo_evento_retif_fonte_6,${TAB}gr_codigo_evento_retif_fonte_7,${TAB}gr_codigo_evento_retif_fonte_8,${TAB}gr_codigo_evento_retif_fonte_9,${TAB}gr_codigo_evento_retif_fonte_10,${TAB}gr_codigo_evento_retif_dest_1,${TAB}gr_codigo_evento_retif_dest_2,${TAB}gr_codigo_evento_retif_dest_3,${TAB}gr_codigo_evento_retif_dest_4,${TAB}gr_codigo_evento_retif_dest_5,${TAB}gr_codigo_evento_retif_dest_6,${TAB}gr_codigo_evento_retif_dest_7,${TAB}gr_codigo_evento_retif_dest_8,${TAB}gr_codigo_evento_retif_dest_9,${TAB}gr_codigo_evento_retif_dest_10 ) FROM stdin;" >> ${FILE}.DESTINACAOGR.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.grupofinanceiro (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_co_grupo_financeiro,${TAB}gr_financeiro_grupo_1,${TAB}gr_financeiro_grupo_2,${TAB}gr_financeiro_grupo_3,${TAB}gr_financeiro_grupo_4,${TAB}gr_financeiro_grupo_5,${TAB}gr_financeiro_grupo_6,${TAB}gr_financeiro_grupo_7,${TAB}gr_financeiro_grupo_8,${TAB}gr_financeiro_grupo_9,${TAB}gr_financeiro_grupo_10,${TAB}gr_financeiro_grupo_11,${TAB}gr_financeiro_grupo_12,${TAB}gr_financeiro_grupo_13,${TAB}gr_financeiro_grupo_14,${TAB}gr_financeiro_grupo_15,${TAB}gr_financeiro_grupo_16,${TAB}gr_financeiro_grupo_17,${TAB}gr_financeiro_grupo_18,${TAB}gr_financeiro_grupo_19,${TAB}gr_financeiro_grupo_20,${TAB}gr_financeiro_grupo_21,${TAB}gr_financeiro_grupo_22,${TAB}gr_financeiro_grupo_23,${TAB}gr_financeiro_grupo_24,${TAB}gr_financeiro_grupo_25,${TAB}gr_financeiro_grupo_26,${TAB}gr_financeiro_grupo_27,${TAB}gr_financeiro_grupo_28,${TAB}gr_financeiro_grupo_29,${TAB}gr_financeiro_grupo_30,${TAB}gr_financeiro_grupo_31,${TAB}gr_financeiro_grupo_32,${TAB}gr_financeiro_grupo_33,${TAB}gr_financeiro_grupo_34,${TAB}gr_financeiro_grupo_35,${TAB}gr_financeiro_grupo_36,${TAB}gr_financeiro_grupo_37,${TAB}gr_financeiro_grupo_38,${TAB}gr_financeiro_grupo_39,${TAB}gr_financeiro_grupo_40,${TAB}gr_financeiro_grupo_41,${TAB}gr_financeiro_grupo_42,${TAB}gr_financeiro_grupo_43,${TAB}gr_financeiro_grupo_44,${TAB}gr_financeiro_grupo_45,${TAB}gr_financeiro_grupo_46,${TAB}gr_financeiro_grupo_47,${TAB}gr_financeiro_grupo_48,${TAB}gr_financeiro_grupo_49,${TAB}gr_financeiro_grupo_50 ) FROM stdin;" >> ${FILE}.GRUPOFINANCEIRO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.recolhimentoug (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_co_terminal_usuario,${TAB}gr_ug_gestao_recolhimento,${TAB}it_in_preenchimento_gr_eletr_1,${TAB}it_in_preenchimento_gr_eletr_2,${TAB}it_in_preenchimento_gr_eletr_3,${TAB}it_in_preenchimento_gr_eletr_4,${TAB}it_in_preenchimento_gr_eletr_5,${TAB}it_in_preenchimento_gr_eletr_6,${TAB}it_in_preenchimento_gr_eletr_7,${TAB}it_in_preenchimento_gr_eletr_8,${TAB}it_in_preenchimento_gr_eletr_9,${TAB}it_in_preenchimento_gr_eletr_10,${TAB}it_in_preenchimento_gr_eletr_11,${TAB}it_in_preenchimento_gr_eletr_12,${TAB}it_in_altera_gr_eletronica_1,${TAB}it_in_altera_gr_eletronica_2,${TAB}it_in_altera_gr_eletronica_3,${TAB}it_in_altera_gr_eletronica_4,${TAB}it_in_altera_gr_eletronica_5,${TAB}it_in_altera_gr_eletronica_6,${TAB}it_in_altera_gr_eletronica_7,${TAB}it_in_altera_gr_eletronica_8,${TAB}it_in_altera_gr_eletronica_9,${TAB}it_in_altera_gr_eletronica_10,${TAB}it_in_altera_gr_eletronica_11,${TAB}it_in_altera_gr_eletronica_12,${TAB}gr_evento_arrecadacao_1,${TAB}gr_evento_arrecadacao_2,${TAB}gr_evento_arrecadacao_3,${TAB}gr_evento_arrecadacao_4,${TAB}gr_evento_arrecadacao_5,${TAB}gr_evento_arrecadacao_6,${TAB}gr_evento_arrecadacao_7,${TAB}gr_evento_arrecadacao_8,${TAB}gr_evento_arrecadacao_9,${TAB}gr_evento_arrecadacao_10,${TAB}gr_evento_arrecadacao_11,${TAB}gr_evento_arrecadacao_12,${TAB}it_co_inscricao1_arrecadacao_1,${TAB}it_co_inscricao1_arrecadacao_2,${TAB}it_co_inscricao1_arrecadacao_3,${TAB}it_co_inscricao1_arrecadacao_4,${TAB}it_co_inscricao1_arrecadacao_5,${TAB}it_co_inscricao1_arrecadacao_6,${TAB}it_co_inscricao1_arrecadacao_7,${TAB}it_co_inscricao1_arrecadacao_8,${TAB}it_co_inscricao1_arrecadacao_9,${TAB}it_co_inscricao1_arrecadacao_10,${TAB}it_co_inscricao1_arrecadacao_11,${TAB}it_co_inscricao1_arrecadacao_12,${TAB}it_co_inscricao2_arrecadacao_1,${TAB}it_co_inscricao2_arrecadacao_2,${TAB}it_co_inscricao2_arrecadacao_3,${TAB}it_co_inscricao2_arrecadacao_4,${TAB}it_co_inscricao2_arrecadacao_5,${TAB}it_co_inscricao2_arrecadacao_6,${TAB}it_co_inscricao2_arrecadacao_7,${TAB}it_co_inscricao2_arrecadacao_8,${TAB}it_co_inscricao2_arrecadacao_9,${TAB}it_co_inscricao2_arrecadacao_10,${TAB}it_co_inscricao2_arrecadacao_11,${TAB}it_co_inscricao2_arrecadacao_12,${TAB}gr_classificacao1_arrecadacao_1,${TAB}gr_classificacao1_arrecadacao_2,${TAB}gr_classificacao1_arrecadacao_3,${TAB}gr_classificacao1_arrecadacao_4,${TAB}gr_classificacao1_arrecadacao_5,${TAB}gr_classificacao1_arrecadacao_6,${TAB}gr_classificacao1_arrecadacao_7,${TAB}gr_classificacao1_arrecadacao_8,${TAB}gr_classificacao1_arrecadacao_9,${TAB}gr_classificacao1_arrecadacao_10,${TAB}gr_classificacao1_arrecadacao_11,${TAB}gr_classificacao1_arrecadacao_12,${TAB}gr_classificacao2_arrecadacao_1,${TAB}gr_classificacao2_arrecadacao_2,${TAB}gr_classificacao2_arrecadacao_3,${TAB}gr_classificacao2_arrecadacao_4,${TAB}gr_classificacao2_arrecadacao_5,${TAB}gr_classificacao2_arrecadacao_6,${TAB}gr_classificacao2_arrecadacao_7,${TAB}gr_classificacao2_arrecadacao_8,${TAB}gr_classificacao2_arrecadacao_9,${TAB}gr_classificacao2_arrecadacao_10,${TAB}gr_classificacao2_arrecadacao_11,${TAB}gr_classificacao2_arrecadacao_12,${TAB}it_in_altera_arrecadacao_1,${TAB}it_in_altera_arrecadacao_2,${TAB}it_in_altera_arrecadacao_3,${TAB}it_in_altera_arrecadacao_4,${TAB}it_in_altera_arrecadacao_5,${TAB}it_in_altera_arrecadacao_6,${TAB}it_in_altera_arrecadacao_7,${TAB}it_in_altera_arrecadacao_8,${TAB}it_in_altera_arrecadacao_9,${TAB}it_in_altera_arrecadacao_10,${TAB}it_in_altera_arrecadacao_11,${TAB}it_in_altera_arrecadacao_12,${TAB}gr_evento_retificacao_1,${TAB}gr_evento_retificacao_2,${TAB}gr_evento_retificacao_3,${TAB}gr_evento_retificacao_4,${TAB}gr_evento_retificacao_5,${TAB}gr_evento_retificacao_6,${TAB}gr_evento_retificacao_7,${TAB}gr_evento_retificacao_8,${TAB}gr_evento_retificacao_9,${TAB}gr_evento_retificacao_10,${TAB}gr_evento_retificacao_11,${TAB}gr_evento_retificacao_12,${TAB}it_co_inscricao1_retificacao_1,${TAB}it_co_inscricao1_retificacao_2,${TAB}it_co_inscricao1_retificacao_3,${TAB}it_co_inscricao1_retificacao_4,${TAB}it_co_inscricao1_retificacao_5,${TAB}it_co_inscricao1_retificacao_6,${TAB}it_co_inscricao1_retificacao_7,${TAB}it_co_inscricao1_retificacao_8,${TAB}it_co_inscricao1_retificacao_9,${TAB}it_co_inscricao1_retificacao_10,${TAB}it_co_inscricao1_retificacao_11,${TAB}it_co_inscricao1_retificacao_12,${TAB}it_co_inscricao2_retificacao_1,${TAB}it_co_inscricao2_retificacao_2,${TAB}it_co_inscricao2_retificacao_3,${TAB}it_co_inscricao2_retificacao_4,${TAB}it_co_inscricao2_retificacao_5,${TAB}it_co_inscricao2_retificacao_6,${TAB}it_co_inscricao2_retificacao_7,${TAB}it_co_inscricao2_retificacao_8,${TAB}it_co_inscricao2_retificacao_9,${TAB}it_co_inscricao2_retificacao_10,${TAB}it_co_inscricao2_retificacao_11,${TAB}it_co_inscricao2_retificacao_12,${TAB}gr_classificacao1_retificacao_1,${TAB}gr_classificacao1_retificacao_2,${TAB}gr_classificacao1_retificacao_3,${TAB}gr_classificacao1_retificacao_4,${TAB}gr_classificacao1_retificacao_5,${TAB}gr_classificacao1_retificacao_6,${TAB}gr_classificacao1_retificacao_7,${TAB}gr_classificacao1_retificacao_8,${TAB}gr_classificacao1_retificacao_9,${TAB}gr_classificacao1_retificacao_10,${TAB}gr_classificacao1_retificacao_11,${TAB}gr_classificacao1_retificacao_12,${TAB}gr_classificacao2_retificacao_1,${TAB}gr_classificacao2_retificacao_2,${TAB}gr_classificacao2_retificacao_3,${TAB}gr_classificacao2_retificacao_4,${TAB}gr_classificacao2_retificacao_5,${TAB}gr_classificacao2_retificacao_6,${TAB}gr_classificacao2_retificacao_7,${TAB}gr_classificacao2_retificacao_8,${TAB}gr_classificacao2_retificacao_9,${TAB}gr_classificacao2_retificacao_10,${TAB}gr_classificacao2_retificacao_11,${TAB}gr_classificacao2_retificacao_12,${TAB}it_in_altera_retificacao_1,${TAB}it_in_altera_retificacao_2,${TAB}it_in_altera_retificacao_3,${TAB}it_in_altera_retificacao_4,${TAB}it_in_altera_retificacao_5,${TAB}it_in_altera_retificacao_6,${TAB}it_in_altera_retificacao_7,${TAB}it_in_altera_retificacao_8,${TAB}it_in_altera_retificacao_9,${TAB}it_in_altera_retificacao_10,${TAB}it_in_altera_retificacao_11,${TAB}it_in_altera_retificacao_12,${TAB}gr_evento_restituicao_1,${TAB}gr_evento_restituicao_2,${TAB}gr_evento_restituicao_3,${TAB}gr_evento_restituicao_4,${TAB}gr_evento_restituicao_5,${TAB}gr_evento_restituicao_6,${TAB}gr_evento_restituicao_7,${TAB}gr_evento_restituicao_8,${TAB}gr_evento_restituicao_9,${TAB}gr_evento_restituicao_10,${TAB}gr_evento_restituicao_11,${TAB}gr_evento_restituicao_12,${TAB}it_co_inscricao1_restituicao_1,${TAB}it_co_inscricao1_restituicao_2,${TAB}it_co_inscricao1_restituicao_3,${TAB}it_co_inscricao1_restituicao_4,${TAB}it_co_inscricao1_restituicao_5,${TAB}it_co_inscricao1_restituicao_6,${TAB}it_co_inscricao1_restituicao_7,${TAB}it_co_inscricao1_restituicao_8,${TAB}it_co_inscricao1_restituicao_9,${TAB}it_co_inscricao1_restituicao_10,${TAB}it_co_inscricao1_restituicao_11,${TAB}it_co_inscricao1_restituicao_12,${TAB}it_co_inscricao2_restituicao_1,${TAB}it_co_inscricao2_restituicao_2,${TAB}it_co_inscricao2_restituicao_3,${TAB}it_co_inscricao2_restituicao_4,${TAB}it_co_inscricao2_restituicao_5,${TAB}it_co_inscricao2_restituicao_6,${TAB}it_co_inscricao2_restituicao_7,${TAB}it_co_inscricao2_restituicao_8,${TAB}it_co_inscricao2_restituicao_9,${TAB}it_co_inscricao2_restituicao_10,${TAB}it_co_inscricao2_restituicao_11,${TAB}it_co_inscricao2_restituicao_12,${TAB}gr_classificacao1_restituicao_1,${TAB}gr_classificacao1_restituicao_2,${TAB}gr_classificacao1_restituicao_3,${TAB}gr_classificacao1_restituicao_4,${TAB}gr_classificacao1_restituicao_5,${TAB}gr_classificacao1_restituicao_6,${TAB}gr_classificacao1_restituicao_7,${TAB}gr_classificacao1_restituicao_8,${TAB}gr_classificacao1_restituicao_9,${TAB}gr_classificacao1_restituicao_10,${TAB}gr_classificacao1_restituicao_11,${TAB}gr_classificacao1_restituicao_12,${TAB}gr_classificacao2_restituicao_1,${TAB}gr_classificacao2_restituicao_2,${TAB}gr_classificacao2_restituicao_3,${TAB}gr_classificacao2_restituicao_4,${TAB}gr_classificacao2_restituicao_5,${TAB}gr_classificacao2_restituicao_6,${TAB}gr_classificacao2_restituicao_7,${TAB}gr_classificacao2_restituicao_8,${TAB}gr_classificacao2_restituicao_9,${TAB}gr_classificacao2_restituicao_10,${TAB}gr_classificacao2_restituicao_11,${TAB}gr_classificacao2_restituicao_12,${TAB}it_in_altera_restituicao_1,${TAB}it_in_altera_restituicao_2,${TAB}it_in_altera_restituicao_3,${TAB}it_in_altera_restituicao_4,${TAB}it_in_altera_restituicao_5,${TAB}it_in_altera_restituicao_6,${TAB}it_in_altera_restituicao_7,${TAB}it_in_altera_restituicao_8,${TAB}it_in_altera_restituicao_9,${TAB}it_in_altera_restituicao_10,${TAB}it_in_altera_restituicao_11,${TAB}it_in_altera_restituicao_12,${TAB}it_co_destinacao_1,${TAB}it_co_destinacao_2,${TAB}it_co_destinacao_3,${TAB}it_co_destinacao_4,${TAB}it_co_destinacao_5,${TAB}it_co_destinacao_6,${TAB}it_co_destinacao_7,${TAB}it_co_destinacao_8,${TAB}it_co_destinacao_9,${TAB}it_co_destinacao_10,${TAB}it_co_destinacao_11,${TAB}it_co_destinacao_12,${TAB}it_in_altera_destinacao_1,${TAB}it_in_altera_destinacao_2,${TAB}it_in_altera_destinacao_3,${TAB}it_in_altera_destinacao_4,${TAB}it_in_altera_destinacao_5,${TAB}it_in_altera_destinacao_6,${TAB}it_in_altera_destinacao_7,${TAB}it_in_altera_destinacao_8,${TAB}it_in_altera_destinacao_9,${TAB}it_in_altera_destinacao_10,${TAB}it_in_altera_destinacao_11,${TAB}it_in_altera_destinacao_12,${TAB}it_tx_motivo,${TAB}it_in_situacao_recolhimento) FROM stdin;" >> ${FILE}.RECOLHIMENTOUG.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.domiciliobancariocredor (it_co_usuario,${TAB}it_co_terminal_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_pagador,${TAB}it_in_operacao,${TAB}it_co_credor_domicilio,${TAB}it_co_banco,${TAB}it_co_agencia,${TAB}it_co_conta,${TAB}it_in_conta_conjunta,${TAB}it_in_tipo_conta,${TAB}it_co_ug_supridora,${TAB}it_co_gestao_supridora) FROM stdin;" >> ${FILE}.DOMICILIOBANCARIOCREDOR.sql;

REG_ORGAOGESTAO=0;
REG_UGSUBORGAO=0;
REG_DEPOSITOBANCARIO=0;
REG_DESTINACAOGR=0;
REG_GRUPOFINANCEIRO=0;
REG_RECOLHIMENTOUG=0;
REG_DOMICILIOBANCARIOCREDOR=0;

contasNum() {
	operadorValor="";
	case $1 in
		"J") operadorValor=1; ;;
		"K") operadorValor=2; ;;
		"L") operadorValor=3; ;;
		"M") operadorValor=4; ;;
		"N") operadorValor=5; ;;
		"O") operadorValor=6; ;;
		"P") operadorValor=7; ;;
		"Q") operadorValor=8; ;;
		"R") operadorValor=9; ;;
		"}") operadorValor=0; ;;
		*) operadorValor=99; ;;
	esac
}

multiplicos(){
	qtCampos=$1;
	tamanho=$2;
	texto=$3;
	n=0;
	x=0;
	valores='';
	#echo 'texto:'$texto;
	#while [ $n -le $qtCampos ]; do
	while [ $n -lt $qtCampos ]; do
		valor=${texto:$x:$tamanho};
		if [ -z "$valor" ]; then
			valor=${NULO};
		fi
		#echo "Valor->"$valor"<-";
		x=$x+$tamanho;
		if [ $n -eq 0 ]; then
				valores=$valor;
			else
				valores=$valores${TAB}$valor;
		fi
		let n++;
	done
}
multiplicosFloat(){
	qtCampos=$1;
	qtInteiros=$2;
	qtCasasDecimais=$3;
	texto=$4;
	n=0;
	x=0;
	valores='';
	#echo 'texto:'$texto;
	while [ $n -lt $qtCampos ]; do
		valor=${texto:$x:$qtInteiros};
		if [ -z "$valor" ]; then
			valor=${NULO};
		else
			valor=${texto:$qtInteiros:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				valor="-"${stValor:0:15}"."${texto:$qtInteiros:1}$operadorValor
			else
				valor=${stValor:0:15}"."${texto:$qtInteiros:2}
			fi
		fi
		x=$x+$qtInteiros;
		if [ $n -eq 0 ]; then
				valores=$valor;
			else
				valores=$valores${TAB}$valor;
		fi
		let n++;
	done
}

cat ${FILE} | grep ^[^TA*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do 
    TIPOREGISTRO=${LINHA:0:2};
    case $TIPOREGISTRO in
    
 	# CONSTANTE 01 - TABELA DE ORGAO/GESTAO
	"01")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	
		# 00004 00011  NUM     IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
		# 00015 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
		# 00023 00005  NUM     GR-ORGAO
		CAMPO04=${LINHA:22:5}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
		# 00028 00005  NUM     IT-CO-GESTAO
		CAMPO05=${LINHA:27:5}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
		# 00033 00001  NUM     IT-IN-COD-BB-GR
		CAMPO06=${LINHA:32:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

	REG_ORGAOGESTAO=`expr $REG_ORGAOGESTAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.ORGAOGESTAO.sql;;

	# CONSTANTE 03 - TABELA DE UG/SUBORGAO
	"03")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		
		# 00004 00011  NUM     IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		
		# 00015 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		
		# 00023 00004  NUM     IT-CO-SUBORGAO
		CAMPO04=${LINHA:22:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		
		# 00027 00006  NUM     IT-CO-UNIDADE-GESTORA
		CAMPO05=${LINHA:26:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi

	REG_UGSUBORGAO=`expr $REG_UGSUBORGAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.UGSUBORGAO.sql;;

	# CONSTANTE 05 - TABELA DE DEPOSITO BANCARIO
	"05")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		# 00004 00011  NUM     IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		# 00015 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		# 00023 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
		CAMPO04=${LINHA:22:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		# 00027 00006  NUM     IT-CO-UG-OPERADOR
		CAMPO05=${LINHA:26:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
		
		# 00033 00015  ALFANUM GR-UG-GESTAO-DEPOSITO-TIPO-DBUQ
		CAMPO06=${LINHA:32:15}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
		
		# 00048 00045  ALFANUM IT-NO-DEPOSITO
		CAMPO07=${LINHA:47:45}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
		
		# 00093 00234  ALFANUM IT-TX-DESCRICAO-DEPOSITO
		CAMPO08=${LINHA:92:234}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		
		# 00327 00001  NUM     IT-IN-DEPOSITANTE
		CAMPO09=${LINHA:326:1}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
		
		# 00328 00001  NUM     IT-IN-INCLUSAO-CODIGO-STN
		CAMPO10=${LINHA:327:1}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		
		# 00329 00012  ALFANUM IT-IN-ALTERACAO-CAMPO (12 OCORRENCIAS FORMATO A1)
		multiplicos 12 1 ${LINHA:328:12}; CAMPO11=$valores;
		
		# 00341 00072  NUM     GR-CODIGO-EVENTO (12 OCORRENCIAS FORMATO N6)
		multiplicos 12 6 ${LINHA:340:72}; CAMPO12=$valores;
		
		# 00413 00168  ALFANUM IT-CO-INSCRICAO1 (12 OCORRENCIAS FORMATO A14)
		multiplicos 12 14 ${LINHA:412:168}; CAMPO13=$valores;
		
		# 00581 00168  ALFANUM IT-CO-INSCRICAO2 (12 OCORRENCIAS FORMATO A14)
		multiplicos 12 14 ${LINHA:580:168}; CAMPO14=$valores;
		
		# 00749 00108  NUM     GR-CLASSIFICACAO1 (12 OCORRENCIAS FORMATO N9)
		multiplicos 12 9 ${LINHA:748:108}; CAMPO15=$valores;
		
		# 00857 00108  NUM     GR-CLASSIFICACAO2 (12 OCORRENCIAS FORMATO N9)
		multiplicos 12 9 ${LINHA:856:108}; CAMPO16=$valores;
		
		# 00965 00012  NUM     IT-IN-FAVORECIDO (12 OCORRENCIAS FORMATO N1)
		multiplicos 12 1 ${LINHA:964:12}; CAMPO17=$valores;
		
		# 00977 00168  ALFANUM IT-CO-FAVORECIDO (12 OCORRENCIAS FORMATO A14)
		multiplicos 12 14 ${LINHA:976:168}; CAMPO18=$valores;
		
		# 01145 00012  ALFANUM IT-IN-FRACIONAMENTO (12 OCORRENCIAS FORMATO A1)
		multiplicos 12 1 ${LINHA:1144:12}; CAMPO19=$valores;
		
		# 01157 00204  NUM    IT-VA-FRACIONAMENTO (12 OCORRENCIAS FORMATO N15,2)
		multiplicosFloat 12 15 2 ${LINHA:1156:204}; CAMPO20=$valores;
		
		# 01361 00012  NUM     IT-IN-LIMITE-VINCULACAO (12 OCORRENCIAS FORMATO N1)
		multiplicos 12 1 ${LINHA:1360:12}; CAMPO21=$valores;
		
		# 01373 00168  ALFANUM IT-CO-INSCRICAO-MAQUINA (12 OCORRENCIAS FORMATO A14)
		multiplicos 12 14 ${LINHA:1372:168}; CAMPO22=$valores;

	REG_DEPOSITOBANCARIO=`expr $REG_DEPOSITOBANCARIO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22 >> ${FILE}.DEPOSITOBANCARIO.sql;;
	
	# CONSTANTE 06 - TABELA DE DESTINACAO GR
	"06")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		
		# 00004 00011  NUM     IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		
		# 00015 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		
		# 00023 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
		CAMPO04=${LINHA:22:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		
		# 00027 00006  NUM     IT-CO-UG-OPERADOR
		CAMPO05=${LINHA:26:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
		
		# 00033 00006  NUM     IT-CO-DESTINACAO-GR
		CAMPO06=${LINHA:32:6}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
		
		# 00039 00045  ALFANUM IT-NO-DESTINACAO-GR
		CAMPO07=${LINHA:38:45}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
		
		# 00084 00025  ALFANUM IT-NO-REDUZIDO-DESTINACAO-GR
		CAMPO08=${LINHA:83:25}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		
		# 00109 00001  NUM     IT-IN-TIPO-DESTINACAO
		CAMPO09=${LINHA:108:1}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
		
		# 00110 00002  NUM     IT-CO-GRUPO-DESTINACAO
		CAMPO10=${LINHA:109:2}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		
		# 00112 00010  ALFANUM GR-FONTE-RECURSO
		CAMPO11=${LINHA:111:10}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
		
		# 00122 00001  NUM     IT-IN-TIPO-BENEFICIARIO
		CAMPO12=${LINHA:121:1}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
		
		# 00123 00006  NUM     IT-CO-UG-BENEFICIARIO
		CAMPO13=${LINHA:122:6}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
		
		# 00129 00005  NUM     IT-CO-GESTAO-BENEFICIARIO
		CAMPO14=${LINHA:128:5}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
		
		# 00134 00006  NUM     GR-CODIGO-EVENTO-RESTI-FONTE
		CAMPO15=${LINHA:133:6}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
		
		# 00140 00006  NUM     GR-CODIGO-EVENTO-RESTI-DEST
		CAMPO16=${LINHA:139:6}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
		
		# 00146 00140  ALFANUM IT-TX-MOTIVO
		CAMPO17=${LINHA:145:140}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
		
		# 00286 00008  NUM     IT-PE-DESTINACAO-PRINCIPAL (FORMATO N1,7)
		multiplicosFloat 1 1 7 ${LINHA:287:8}; CAMPO18=$valores;
		
		# 00294 00080  NUM     IT-PE-DESTINACAO-SECUNDARIA (10 OCORRENCIAS FORMATO N1,7)
		multiplicosFloat 10 1 7 ${LINHA:293:80}; CAMPO19=$valores;
		
		# 00374 00060  NUM     IT-CO-DESTINACAO-SECUNDARIA (10 OCORRENCIAS FORMATO N6)
		multiplicos 10 6 ${LINHA:373:60}; CAMPO20=$valores;
		
		# 00434 00060  NUM     GR-CODIGO-EVENTO-ARREC-FONTE (10 OCORRENCIAS FORMATO N6)
		multiplicos 10 6 ${LINHA:433:60}; CAMPO21=$valores;
		
		# 00494 00060  NUM     GR-CODIGO-EVENTO-ARREC-DEST (10 OCORRENCIAS FORMATO N6)
		multiplicos 10 6 ${LINHA:493:60}; CAMPO22=$valores;
		
		# 00554 00060  NUM     GR-CODIGO-EVENTO-RETIF-FONTE (10 OCORRENCIAS FORMATO N6)
		multiplicos 10 6 ${LINHA:553:60}; CAMPO23=$valores;
		
		# 00614 00060  NUM     GR-CODIGO-EVENTO-RETIF-DEST  (10 OCORRENCIAS FORMATO N6)
		multiplicos 10 6 ${LINHA:613:60}; CAMPO24=$valores;
		
		REG_DESTINACAOGR=`expr $REG_DESTINACAOGR + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24 >> ${FILE}.DESTINACAOGR.sql;;
		
		
	# CONSTANTE 07 - TABELA DE GRUPO FINANCEIRO
	"07")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		
		# 00004 00011  NUM     IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		
		# 00015 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		
		# 00023 00002  NUM     IT-CO-GRUPO-FINANCEIRO
		CAMPO04=${LINHA:22:2}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		
		# 00025 00100  ALFANUM GR-FINANCEIRO-GRUPO (50 OCORRENCIAS FORMATO A2)
		multiplicos 50 2 ${LINHA:24:100}; CAMPO05=$valores;
		
		REG_GRUPOFINANCEIRO=`expr $REG_GRUPOFINANCEIRO + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.GRUPOFINANCEIRO.sql;;
		
	# CONSTANTE 11 - TABELA DE RECOLHIMENTO UG
	"11")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		
		# 00004 00011  NUM     IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		
		# 00015 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		
		# 00023 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
		CAMPO04=${LINHA:22:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		
		# 00027 00006  NUM     IT-CO-UG-OPERADOR
		CAMPO05=${LINHA:26:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
		
		# 00033 00008  ALFANUM IT-CO-TERMINAL-USUARIO
		CAMPO06=${LINHA:32:8}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
		
		# 00041 00016  ALFANUM GR-UG-GESTAO-RECOLHIMENTO
		CAMPO07=${LINHA:40:16}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
		
		# 00057 00012  NUM     IT-IN-PREENCHIMENTO-GR-ELETR (12 OCORR. FORMATOTO N1
		multiplicos 12 1 ${LINHA:56:12}; CAMPO08=$valores;
		
		# 00069 00012  ALFANUM IT-IN-ALTERA-GR-ELETRONICA (12 OCORR. FORMATO A1)
		multiplicos 12 1 ${LINHA:68:12}; CAMPO09=$valores;
		
		# 00081 00072  NUM     GR-EVENTO-ARRECADACAO (12 OCORRENCIAS FORMATO N6)
		multiplicos 12 6 ${LINHA:80:72}; CAMPO10=$valores;
		
		# 00153 00168  ALFANUM IT-CO-INSCRICAO1-ARRECADACAO (12 OCORR. FORMATO A14)
		multiplicos 12 14 ${LINHA:152:168}; CAMPO11=$valores;
		
		# 00321 00168  ALFANUM IT-CO-INSCRICAO2-ARRECADACAO (12 OCORR. FORMATO A14)
		multiplicos 12 14 ${LINHA:320:168}; CAMPO12=$valores;
		
		# 00489 00108  NUM     GR-CLASSIFICACAO1-ARRECADACAO (12 OCORR. FORMATO A9)
		multiplicos 12 9 ${LINHA:488:108}; CAMPO13=$valores;
		
		# 00597 00108  NUM     GR-CLASSIFICACAO2-ARRECADACAO (12 OCORR. FORMATO A9)
		multiplicos 12 9 ${LINHA:596:108}; CAMPO14=$valores;
		
		# 00705 00012  ALFANUM IT-IN-ALTERA-ARRECADACAO (12 OCORRENCIAS FORMATO A1)
		multiplicos 12 1 ${LINHA:704:12}; CAMPO15=$valores;
		
		# 00717 00072  NUM     GR-EVENTO-RETIFICACAO (12 OCORRENCIAS FORMATO N6)
		multiplicos 12 6 ${LINHA:716:72}; CAMPO16=$valores;
		
		# 00789 00168  ALFANUM IT-CO-INSCRICAO1-RETIFICACAO (12 OCORR. FORMATO A14)
		multiplicos 12 14 ${LINHA:788:168}; CAMPO17=$valores;
		
		# 00957 00168  ALFANUM IT-CO-INSCRICAO2-RETIFICACAO (12 OCORR. FORMATO A14)
		multiplicos 12 14 ${LINHA:956:168}; CAMPO18=$valores;
		
		# 01125 00108  NUM     GR-CLASSIFICACAO1-RETIFICACAO (12 OCORR. FORMATO A9)
		multiplicos 12 9 ${LINHA:1124:108}; CAMPO19=$valores;
		
		# 01233 00108  NUM     GR-CLASSIFICACAO2-RETIFICACAO (12 OCORR. FORMATO N9
		multiplicos 12 9 ${LINHA:1232:108}; CAMPO20=$valores;
		
		# 01341 00012  ALFANUM IT-IN-ALTERA-RETIFICACAO (12 OCORRENCIAS FORMATO A1)
		multiplicos 12 1 ${LINHA:1340:12}; CAMPO21=$valores;
		
		# 01353 00072  NUM     GR-EVENTO-RESTITUICAO (12 OCORRENCIAS FORMATO N6)
		multiplicos 12 6 ${LINHA:1352:72}; CAMPO22=$valores;
		
		# 01425 00168  ALFANUM IT-CO-INSCRICAO1-RESTITUICAO (12 OCORR. FORMATO A14)
		multiplicos 12 14 ${LINHA:1424:168}; CAMPO23=$valores;
		
		# 01593 00168  ALFANUM IT-CO-INSCRICAO2-RESTITUICAO (12 OCORR. FORMATO A14)
		multiplicos 12 14 ${LINHA:1592:168}; CAMPO24=$valores;
		
		# 01761 00108  NUM     GR-CLASSIFICACAO1-RESTITUICAO (12 OCORR. FORMATO N9)
		multiplicos 12 9 ${LINHA:1760:108}; CAMPO25=$valores;
		
		# 01869 00108  NUM     GR-CLASSIFICACAO2-RESTITUICAO (12 OCORR. FORMATO N9)
		multiplicos 12 9 ${LINHA:1868:108}; CAMPO26=$valores;
		
		# 01977 00012  NUM     IT-IN-ALTERA-RSTITUICAO (12 OCORRENCIAS FORMATO A1)
		multiplicos 12 1 ${LINHA:1976:12}; CAMPO27=$valores;
		
		# 01989 00072  NUM     IT-CO-DESTINACAO (12 OCORRENCIAS FORMATO N6)
		multiplicos 12 6 ${LINHA:1988:72}; CAMPO28=$valores;
		
		# 02061 00012  ALFANUM IT-IN-ALTERA-DESTINACAO (12 OCORRENCIAS FORMATO A1)
		multiplicos 12 1 ${LINHA:2060:12}; CAMPO29=$valores;
		
		# 02073 00140  ALFANUM IT-TX-MOTIVO
		CAMPO30=${LINHA:2072:140}; if [ -z "$CAMPO30" ]; then CAMPO30=${NULO}; fi
		
		# 02213 00001  NUM     IT-IN-SITUACAO-RECOLHIMENTO
		CAMPO31=${LINHA:2212:1}; if [ -z "$CAMPO31" ]; then CAMPO31=${NULO}; fi
		
		REG_RECOLHIMENTOUG=`expr $REG_RECOLHIMENTOUG + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27${TAB}$CAMPO28${TAB}$CAMPO29${TAB}$CAMPO30${TAB}$CAMPO31 >> ${FILE}.RECOLHIMENTOUG.sql;;
		
	# CONSTANTE 19 - TABELA DE CONTROLE DE CREDOR
	"19")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		
		# 00004 00011  ALFANUM IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		
		# 00015 00008  NUM     IT-DA-OPERACAO (FORMATO DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		
		# 00023 00002  NUM     IT-CO-PARAM-CONTROLE-CREDOR
		CAMPO04=${LINHA:22:2}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		
		# 00025 00045  ALFANUM IT-NO-PARAM-CONTROLE-CREDOR
		CAMPO05=${LINHA:24:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			
		REG_DOMICILIOBANCARIOCREDOR=`expr $REG_DOMICILIOBANCARIOCREDOR + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.RECOLHIMENTOUG.sql;;
		
	# CONSTANTE 20 - TABELA DE DARF SISTEMA
	"20")
		# 00003 00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		# 00004 00011  ALFANUM IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		# 00015 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		# 00023 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
		CAMPO04=${LINHA:22:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		# 00027 00006  NUM     IT-CO-UNIDADE-GESTORA-SALDO
		CAMPO05=${LINHA:26:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
		# 00033 00005  NUM     IT-CO-GESTAO-SALDO
		CAMPO06=${LINHA:32:5}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
		# 00038 00009  ALFANUM GR-CODIGO-CONTA
		CAMPO07=${LINHA:37:9}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
		# 00047 00043  ALFANUM IT-CO-CONTA-CORRENTE-CONTABIL
		CAMPO08=${LINHA:46:43}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		# 00090 00008  NUM     IT-DA-EXECUCAO (FORMATO DDMMAAAA)
		CAMPO09=${LINHA:89:8}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
		# 00098 00001  ALFANUM IT-IN-TIPO-EXECUCAO
		CAMPO10=${LINHA:97:1}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		# 00099 00003  NUM     IT-QT-DIAS-UTEIS
		CAMPO11=${LINHA:98:3}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
		# 00102 00006  NUM     IT-CO-UNIDADE-GESTORA-EMIT
		CAMPO12=${LINHA:101:6}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
		# 00108 00005  NUM     IT-CO-GESTAO-EMIT
		CAMPO13=${LINHA:107:5}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
		# 00113 00014  ALFANUM IT-CO-FAVORECIDO
		CAMPO14=${LINHA:112:14}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
		# 00127 00004  NUM     IT-CO-RECEITA
		CAMPO15=${LINHA:126:4}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
		# 00131 00001  NUM     IT-IN-TIPO-RECURSO
		CAMPO16=${LINHA:130:1}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
		# 00132 00006  NUM     IT-CO-UG-DOC-REFERENCIA
		CAMPO17=${LINHA:131:6}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
		# 00138 00005  NUM     IT-CO-GESTAO-DOC-REFERENCIA
		CAMPO18=${LINHA:137:5}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
		# 00143 00012  ALFANUM GR-AN-NU-DOCUMENTO-REFERENCIA
		CAMPO19=${LINHA:142:12}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
		# 00155 00010  ALFANUM GR-FONTE-RECURSO
		CAMPO20=${LINHA:154:10}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
		# 00165 00003  NUM     IT-CO-VINC-PAGAMENTO
		CAMPO21=${LINHA:162:3}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
		# 00168 00001  NUM     IT-CO-GRUPO-DESPESA
		CAMPO22=${LINHA:167:1}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
		# 00169 00017  ALFANUM IT-NU-PROCESSO
		CAMPO23=${LINHA:168:17}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
		# 00186 00017  ALFANUM IT-NU-REFERENCIA
		CAMPO24=${LINHA:185:17}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
		# 00203 00234  ALFANUM IT-TX-OBSERVACAO
		CAMPO25=${LINHA:202:234}; if [ -z "$CAMPO25" ]; then CAMPO25=${NULO}; fi
		
		REG_RECOLHIMENTOUG=`expr $REG_RECOLHIMENTOUG + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25 >> ${FILE}.RECOLHIMENTOUG.sql;;
		
		# CONSTANTE 23 - TABELA DE NATUREZA RESPONSABILIDADE
		"23")
			# 00003 00001  ALFANUM IT-IN-OPERACAO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00004 00011  ALFANUM IT-CO-USUARIO
			CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00015 00008  NUM     IT-DA-OPERACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00023 00004  NUM     IT-HO-OPERACAO (FORMATO HHMM)
			CAMPO04=${LINHA:22:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00027 00003  ALFANUM IT-CO-NATUREZA-RESPONSABILIDADE
			CAMPO05=${LINHA:26:3}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00030 00045  ALFANUM IT-NO-NATUREZA-RESPONSABILIDADE
			CAMPO06=${LINHA:29:45}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00075 00019  ALFANUM IT-NO-MNEMONICO-NATUREZA-RESP
			CAMPO07=${LINHA:74:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00094 00003  ALFANUM IT-CO-NAT-RESP-SUBSTITUTA
			CAMPO08=${LINHA:93:3}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		
		# CONSTANTE 26 - TABELA DE SETOR ATIVIDADE ECONOMICA
		"26")
			# 00003 00001  ALFANUM IT-IN-OPERACAO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00004 00011  ALFANUM IT-CO-USUARIO
			CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00015 00008  NUM     IT-DA-OPERACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00023 00002  NUM     IT-CO-SETOR-ATIV-ECONOMICA-VELHO
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00025 00045  ALFANUM IT-NO-SETOR-ATIV-ECONOMICA
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00070 00002  ALFANUM IT-CO-SETOR-ATIV-ECONOMICA
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
		
		# CONSTANTE 28 - TABELA DE CODIGO BB/GR
		"28")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00026 00001  ALFANUM IT-IN-OPERACAO
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00027 00005  NUM     IT-CO-BB-GR
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00032 00006  NUM     IT-CO-UNIDADE-GESTORA
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00038 00005  NUM     IT-CO-GESTAO
			CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00043 00001  NUM     IT-IN-COD-BB-GR
			CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		
		# CONSTANTE 29 - TABELA DE FPAS
		"29")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00026 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00032 00001  ALFANUM IT-IN-OPERACAO
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00033 00003  NUM     IT-CO-FPAS
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00036 00045  ALFANUM IT-NO-FPAS
			CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00081 00200  ALFANUM IT-CO-ENTIDADE (50 OCORRENCIAS FORMATO N4)
			multiplicos 50 4 ${LINHA:80:20}; CAMPO08=$valores;
			
			# 00281 00140  ALFANUM IT-TX-MOTIVO
			CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
		 
		 
		# CONSTANTE 31 - TABELA INDICE CORRECAO PROJUD
		"31")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00007  NUM     IT-IN-CORRECAO (FORMATO N2,5)
			CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00048 00002  NUM     IT-ME-CORRECAO
			CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00050 00004  NUM     IT-AN-CORRECAO
			CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 00054 00050  ALFANUM IT-NO-INDICE-CORRECAO
			CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		
		# CONSTANTE 33 - TABELA DE LIMITE EMPENHO
		"33")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00002  NUM     IT-IN-MODALIDADE-LICITACAO
			CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00043 00002  ALFANUM IT-CO-INCISO
			CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00045 00017  NUM     IT-VA-LIMITE (FORMATO N15,2)
			CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 00062 00008  NUM     IT-DA-INICIO-VIGENCIA (FORMATO DDMMAAAA)
			CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			# 00070 00001  NUM     IT-IN-LIMITE-ATUAL
			CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			# 00071 00017  NUM     IT-VA-LIMITE-2 (FORMATO DDMMAAAA)
			CAMPO12=${LINHA:153:8}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
		
		# CONSTANTE 34 - TABELA DE LRF
		"34")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00026 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00032 00001  ALFANUM IT-IN-OPERACAO
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00033 00003  NUM     IT-NU-LEI
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00036 00070  ALFANUM IT-NO-EXIGENCIA-LEGAL
			CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00106 02100  ALFANUM IT-TX-INSTRUCAO-NORMA (30 OCORRENCIAS FORMATO A70)
			CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 02206 00001  NUM     IT-IN-LEI
			CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 02207 00003  ALFANUM IT-NU-SUBGRUPO
			CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			# 02210 00001  ALFANUM IT-IN-VISIVEL
			CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
		
		# CONSTANTE 36 - TABELA DE MOTIVO INADIMPLENCIA
		"36")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00003  NUM     IT-CO-MOTIVO-INADIMPLENCIA
			CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00044 00045  ALFANUM IT-NO-MOTIVO-INADIMPLENCIA
			CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00089 00003  NUM     IT-CO-GRUPO-MOTIVO-INADIMPLENCIA
			CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 00092 01050  ALFANUM IT-TX-MOTIVO (15 OCORRENCIAS FORMATO A70)
			CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		
		# CONSTANTE 37 - TABELA DE ORIGEM PRECATORIO
		"37")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00005  NUM     IT-CO-ORGAO-CADASTRADOR
			CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00046 00006  NUM     IT-CO-ORIGEM-PRECATORIO
			CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00052 00005  NUM     IT-CO-ORGAO-PAGADOR
			CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 00057 00004  NUM     IT-CO-MUNICIPIO
			CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			# 00061 00002  ALFANUM IT-CO-UF
			CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			# 00063 00045  ALFANUM IT-NO-TITULO-ORIGEM
			CAMPO12=${LINHA:153:8}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
			# 00108 00019  ALFANUM IT-NO-MNEMONICO-TITULO-ORIGEM
			CAMPO13=${LINHA:153:8}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi

# CONSTANTE 38 - TABELA DE PROCESSO JUDICIAL
"38")
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	CAMPO12=${LINHA:153:8}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	CAMPO13=${LINHA:153:8}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	CAMPO14=${LINHA:153:8}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
	CAMPO15=${LINHA:153:8}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
	CAMPO16=${LINHA:153:8}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
	CAMPO17=${LINHA:153:8}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
	CAMPO18=${LINHA:153:8}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
	CAMPO19=${LINHA:153:8}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
	CAMPO20=${LINHA:153:8}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
	CAMPO21=${LINHA:153:8}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
	CAMPO22=${LINHA:153:8}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
	CAMPO23=${LINHA:153:8}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
	CAMPO24=${LINHA:153:8}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
	CAMPO25=${LINHA:153:8}; if [ -z "$CAMPO25" ]; then CAMPO25=${NULO}; fi
	CAMPO26=${LINHA:153:8}; if [ -z "$CAMPO26" ]; then CAMPO26=${NULO}; fi
	CAMPO27=${LINHA:153:8}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	CAMPO28=${LINHA:102:1}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi
	CAMPO29=${LINHA:103:50}; if [ -z "$CAMPO29" ]; then CAMPO29=${NULO}; fi
	CAMPO20=${LINHA:153:8}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
	CAMPO21=${LINHA:153:8}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
	CAMPO22=${LINHA:153:8}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
	CAMPO23=${LINHA:153:8}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
	CAMPO24=${LINHA:153:8}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
	CAMPO25=${LINHA:153:8}; if [ -z "$CAMPO25" ]; then CAMPO25=${NULO}; fi
	CAMPO26=${LINHA:153:8}; if [ -z "$CAMPO26" ]; then CAMPO26=${NULO}; fi
	CAMPO27=${LINHA:153:8}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	CAMPO28=${LINHA:153:8}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi
	CAMPO29=${LINHA:153:8}; if [ -z "$CAMPO29" ]; then CAMPO29=${NULO}; fi
	CAMPO30=${LINHA:153:8}; if [ -z "$CAMPO30" ]; then CAMPO30=${NULO}; fi
	CAMPO31=${LINHA:153:8}; if [ -z "$CAMPO31" ]; then CAMPO31=${NULO}; fi
	CAMPO32=${LINHA:153:8}; if [ -z "$CAMPO32" ]; then CAMPO32=${NULO}; fi
	CAMPO33=${LINHA:153:8}; if [ -z "$CAMPO33" ]; then CAMPO33=${NULO}; fi
	CAMPO34=${LINHA:153:8}; if [ -z "$CAMPO34" ]; then CAMPO34=${NULO}; fi
	CAMPO35=${LINHA:153:8}; if [ -z "$CAMPO35" ]; then CAMPO35=${NULO}; fi
	CAMPO36=${LINHA:153:8}; if [ -z "$CAMPO36" ]; then CAMPO36=${NULO}; fi
	CAMPO37=${LINHA:153:8}; if [ -z "$CAMPO37" ]; then CAMPO37=${NULO}; fi
	CAMPO38=${LINHA:102:1}; if [ -z "$CAMPO38" ]; then CAMPO38=${NULO}; fi
	CAMPO39=${LINHA:103:50}; if [ -z "$CAMPO39" ]; then CAMPO39=${NULO}; fi
	CAMPO30=${LINHA:153:8}; if [ -z "$CAMPO30" ]; then CAMPO30=${NULO}; fi
	CAMPO31=${LINHA:153:8}; if [ -z "$CAMPO31" ]; then CAMPO31=${NULO}; fi
	CAMPO32=${LINHA:153:8}; if [ -z "$CAMPO32" ]; then CAMPO32=${NULO}; fi
	CAMPO33=${LINHA:153:8}; if [ -z "$CAMPO33" ]; then CAMPO33=${NULO}; fi
	CAMPO34=${LINHA:153:8}; if [ -z "$CAMPO34" ]; then CAMPO34=${NULO}; fi
	CAMPO35=${LINHA:153:8}; if [ -z "$CAMPO35" ]; then CAMPO35=${NULO}; fi
	CAMPO36=${LINHA:153:8}; if [ -z "$CAMPO36" ]; then CAMPO36=${NULO}; fi
	CAMPO37=${LINHA:153:8}; if [ -z "$CAMPO37" ]; then CAMPO37=${NULO}; fi
	CAMPO38=${LINHA:153:8}; if [ -z "$CAMPO38" ]; then CAMPO38=${NULO}; fi
	CAMPO39=${LINHA:153:8}; if [ -z "$CAMPO39" ]; then CAMPO39=${NULO}; fi
	CAMPO40=${LINHA:153:8}; if [ -z "$CAMPO40" ]; then CAMPO40=${NULO}; fi
	CAMPO41=${LINHA:153:8}; if [ -z "$CAMPO41" ]; then CAMPO41=${NULO}; fi
	CAMPO42=${LINHA:153:8}; if [ -z "$CAMPO42" ]; then CAMPO42=${NULO}; fi
	CAMPO43=${LINHA:153:8}; if [ -z "$CAMPO43" ]; then CAMPO43=${NULO}; fi
	CAMPO44=${LINHA:153:8}; if [ -z "$CAMPO44" ]; then CAMPO44=${NULO}; fi
	CAMPO45=${LINHA:153:8}; if [ -z "$CAMPO45" ]; then CAMPO45=${NULO}; fi
	CAMPO46=${LINHA:153:8}; if [ -z "$CAMPO46" ]; then CAMPO46=${NULO}; fi
	CAMPO47=${LINHA:153:8}; if [ -z "$CAMPO47" ]; then CAMPO47=${NULO}; fi
	CAMPO48=${LINHA:102:1}; if [ -z "$CAMPO48" ]; then CAMPO48=${NULO}; fi
	CAMPO49=${LINHA:103:50}; if [ -z "$CAMPO49" ]; then CAMPO49=${NULO}; fi
	CAMPO40=${LINHA:153:8}; if [ -z "$CAMPO40" ]; then CAMPO40=${NULO}; fi
	CAMPO41=${LINHA:153:8}; if [ -z "$CAMPO41" ]; then CAMPO41=${NULO}; fi
	CAMPO42=${LINHA:153:8}; if [ -z "$CAMPO42" ]; then CAMPO42=${NULO}; fi
	CAMPO43=${LINHA:153:8}; if [ -z "$CAMPO43" ]; then CAMPO43=${NULO}; fi
	CAMPO44=${LINHA:153:8}; if [ -z "$CAMPO44" ]; then CAMPO44=${NULO}; fi
	CAMPO45=${LINHA:153:8}; if [ -z "$CAMPO45" ]; then CAMPO45=${NULO}; fi
	CAMPO46=${LINHA:153:8}; if [ -z "$CAMPO46" ]; then CAMPO46=${NULO}; fi
	CAMPO47=${LINHA:153:8}; if [ -z "$CAMPO47" ]; then CAMPO47=${NULO}; fi
	CAMPO48=${LINHA:153:8}; if [ -z "$CAMPO48" ]; then CAMPO48=${NULO}; fi
	CAMPO49=${LINHA:153:8}; if [ -z "$CAMPO49" ]; then CAMPO49=${NULO}; fi
	CAMPO50=${LINHA:153:8}; if [ -z "$CAMPO50" ]; then CAMPO50=${NULO}; fi
	CAMPO51=${LINHA:153:8}; if [ -z "$CAMPO51" ]; then CAMPO51=${NULO}; fi
	CAMPO52=${LINHA:153:8}; if [ -z "$CAMPO52" ]; then CAMPO52=${NULO}; fi
	CAMPO53=${LINHA:153:8}; if [ -z "$CAMPO53" ]; then CAMPO53=${NULO}; fi
	CAMPO54=${LINHA:153:8}; if [ -z "$CAMPO54" ]; then CAMPO54=${NULO}; fi
	CAMPO55=${LINHA:153:8}; if [ -z "$CAMPO55" ]; then CAMPO55=${NULO}; fi
	CAMPO56=${LINHA:153:8}; if [ -z "$CAMPO56" ]; then CAMPO56=${NULO}; fi
	CAMPO57=${LINHA:153:8}; if [ -z "$CAMPO57" ]; then CAMPO57=${NULO}; fi
	# 00003 00011  ALFANUM IT-CO-USUARIO
	# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
	# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
	# 00026 00006  NUM     IT-CO-UG-OPERADOR
	# 00032 00001  ALFANUM IT-IN-OPERACAO
	# 00033 00006  NUM     IT-CO-UG-CADASTRADORA
	# 00039 00005  NUM     IT-CO-GESTAO-CADASTRADORA
	# 00044 00030  ALFANUM IT-NU-PRECATORIO
	# 00074 00002  NUM     IT-IN-SENTENCA
	# 00076 00003  ALFANUM IT-IN-SITUACAO
	# 00079 00006  NUM     IT-CO-UNIDADE-GESTORA-PAGADORA
	# 00085 00005  NUM     IT-CO-GESTAO-PAGADORA
	# 00090 00100  ALFANUM IT-NO-REQUERENTE
	# 00190 00014  ALFANUM IT-CO-IDENTIFICACAO-REQUERENTE
	# 00204 00006  ALFANUM IT-CO-NATUREZA-DESPESA
	# 00210 00002  NUM     IT-IN-TIPO-DESPESA
	# 00212 00005  NUM     IT-CO-ORGAO-REU
	# 00217 00005  NUM     IT-CO-UNIDADE-ORCAMENTARIA
	# 00222 00234  ALFANUM IT-TX-DESCRICAO-PRECATOR
	# 00456 00006  NUM     IT-CO-VARA-ORIGEM
	# 00462 00008  NUM     IT-DA-AUTUACAO (FORMATO DDMMAAAA)
	# 00470 00002  NUM     IT-IN-TIPO-JUSTICA
	# 00472 00030  ALFANUM IT-CO-ACAO-ORIGINARIA
	# 00502 00002  NUM     IT-CO-VARA-COMARCA
	# 00504 00008  NUM     IT-DA-AJUIZAMENTO-ACAO (FORMATO DDMMAAAA)
	# 00512 00008  NUM     IT-CO-ASSUNTO
	# 00520 00004  ALFANUM IT-AN-PROPOSTA-ORCAMENTARIA
	# 00524 00001  NUM     IT-IN-NATUREZA-ALIMENTICIA
	# 00525 00001  ALFANUM IT-IN-BLOQUEADO
	# 00526 00017  NUM     IT-VA-PRECATORIO (FORMATO N15,2)
	# 00543 00008  NUM     IT-DA-VALOR (FORMATO DDMMAAAA)
	# 00551 00017  NUM     IT-VA-ATUAL (FORMATO N15,2)
	# 00568 00008  NUM     IT-DA-VALOR-ATUAL (FORMATO DDMMAAAA)
	# 00576 00005  NUM     IT-CO-ORGAO-CADASTRADOR
	# 00581 00012  ALFANUM IT-IN-NUMERO-SEQUENCIAL
	# 00593 00017  NUM     IT-VA-RETENCAO (FORMATO N15,2)
	# 00610 00001  NUM     IT-IN-SENTENCA-PEQUENO-VALOR
	# 00611 00006  NUM     IT-QT-BENEFICIARIO
	# 00617 00030  NUM     IT-CO-BANCO (10 OCORRENCIAS)
	# 00647 00040  NUM     IT-CO-AGENCIA (10 OCORRENCIAS)
	# 00687 00100  ALFANUM IT-NU-CONTA-CORRENTE (10 OCORRENCIAS)
	# 00787 00170  NUM     IT-VA-PARCELA (10 OCORRENCIAS FORMATO N15,2)
	# 00957 00170  NUM     IT-VA-PARCELA-RETENCAO(10 OCORRENCIAS FORMATO N15,2)
	# 01127 00170  NUM     IT-VA-PARCELA-LIQUIDA (10 OCORRENCIAS FORMATO N15,2)
	# 01297 00080  NUM     IT-DA-PREVISAO-VENCIMENTO (10 OCORRENCIAS FORMATO DDMMAAAA)
	# 01377 00170  NUM     IT-VA-PARCELA-A-PAGAR (10 OCORRENCIAS FORMATO N15,2)
	# 01547 00170  NUM     IT-VA-PARCELA-PAGO (10 OCORRENCIAS FORMATO N15,2)
	# 01717 00170  NUM     IT-VA-COMPLEMENTO-PAGO(10 OCORRENCIAS FORMATO N15,2)
	# 01887 00020  NUM     IT-NU-PARCELA-COMPLEMENTO(10 OCORRENCIAS FORMATO N02
	# 01907 00020  NUM     IT-SQ-COMPLEMENTO (10 OCORRENCIAS FORMATO N02)
	# 01927 00170  NUM     IT-VA-COMPLEMENTO (10 OCORRENCIAS FORMATO N15,2)
	# 02097 00170  NUM     IT-VA-COMPLEMENTO-RETENCAO (10 OCORRENCIAS FORMATO N15,2)
	# 02267 00080  NUM     IT-DA-COMPLEMENTO-VENCIMENTO (10 OCORRENCIAS FORMATO N15,2)
	# 02347 00170  NUM     IT-VA-COMPLEMENTO-A-PAGAR (10 OCORRENCIAS FORMATO 15,2)
	# 02517 00020  NUM     IT-NU-PARCELA-DEVOLUCAO (10 OCORRENCIAS FORMATO N02)
	# 02537 00020  NUM     IT-SQ-DEVOLUCAO (10 OCORRENCIAS FORMATO N02)
	# 02557 00170  NUM     IT-VA-DEVOLUCAO (10 OCORRENCIAS FORMATO N15,2)
	# 02727 00230  ALFANUM IT-NU-DOCUMENTO-DEVOLUCAO (10 OCORRENCIAS FORMATO A23)
	# 02957 00010  NUM     IT-IN-OPERACAO-DEVOLUCAO(10 OCORRENCIAS FORMATO N01)

# CONSTANTE 39 - TABELA DE PREVISAO
"39")
	# 00003 00011  ALFANUM IT-CO-USUARIO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
	CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
	CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	# 00034 00006  NUM     IT-CO-UG-OPERADOR
	CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	# 00040 00001  ALFANUM IT-IN-OPERACAO
	CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	# 00041 00003  NUM     IT-CO-PREVISAO
	CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	# 00044 00045  ALFANUM IT-NO-PREVISAO
	CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	# 00089 00019  ALFANUM IT-NO-MNEMONICO-PREVISAO
	CAMPO09=${LINHA:102:1}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	# 00108 00006  NUM     IT-CO-EVENTO
	CAMPO10=${LINHA:102:1}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi

# CONSTANTE 40 - TABELA DE RECOLHIMENTO GFIP
"40")
	# 00003 00011  ALFANUM IT-CO-USUARIO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
	CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	# 00026 00006  NUM     IT-CO-UG-OPERADOR
	CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	# 00032 00001  ALFANUM IT-IN-OPERACAO
	CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	# 00033 00004  NUM     IT-CO-GFIP
	CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	# 00037 00045  ALFANUM IT-NO-GFIP
	CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	# 00082 00001  NUM     IT-IN-EXIGE-COMPETENCIA
	CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	# 00083 00002  NUM     IT-ME-INICIO
	CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	# 00085 00004  NUM     IT-AN-INICIO
	CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	# 00089 00002  NUM     IT-ME-TERMINO
	CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	# 00091 00004  NUM     IT-AN-TERMINO
	CAMPO12=${LINHA:153:8}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	# 00095 00140  ALFANUM IT-TX-MOTIVO
	CAMPO13=${LINHA:153:8}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	# 00235 00002  NUM     IT-CO-CORRELACAO-BARRA
	CAMPO14=${LINHA:153:8}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
	# 00237 00001  NUM     IT-IN-EXIGE-EMPENHO
	CAMPO15=${LINHA:153:8}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi

# CONSTANTE 41 - TABELA DE SALARIO EDUCACAO
"41")
	# 00003 00011  ALFANUM IT-CO-USUARIO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
	CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	# 00026 00006  NUM     IT-CO-UG-OPERADOR
	CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	# 00032 00001  ALFANUM IT-IN-OPERACAO
	CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	# 00033 00004  NUM     IT-CO-SALARIO-EDUCACAO
	CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	# 00037 00045  ALFANUM IT-NO-SALARIO-EDUCACAO
	CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	# 00082 00001  NUM     IT-IN-COMPETENCIA
	CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	# 00083 00004  NUM     IT-AN-INICIO
	CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	# 00087 00004  NUM     IT-AN-REFERENCIA
	CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	# 00091 00001  NUM     IT-IN-PARCELA
	CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	# 00092 00001  NUM     IT-IN-PROCESSO-EXEC-FISCAL
	CAMPO12=${LINHA:153:8}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	# 00093 00001  NUM     IT-IN-CALCULO-DV
	CAMPO13=${LINHA:153:8}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	# 00094 00001  NUM     IT-IN-BASE-CONTRIBUICAO
	CAMPO14=${LINHA:153:8}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
	# 00095 00001  NUM     IT-IN-SALARIO-EDUCACAO
	CAMPO15=${LINHA:153:8}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
	# 00096 00004  NUM     IT-PE-SALARIO-EDUCACAO (FORMATO N2,2)
	CAMPO16=${LINHA:153:8}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
	# 00100 00001  NUM     IT-IN-DEDUCAO-SME
	CAMPO17=${LINHA:153:8}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
	# 00101 00001  NUM     IT-IN-COMPENSACAO
	CAMPO18=${LINHA:153:8}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
	# 00102 00001  NUM     IT-IN-VALOR-ATUALIZADO
	CAMPO19=${LINHA:153:8}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
	# 00103 00001  NUM     IT-IN-MULTA-JUROS
	CAMPO20=${LINHA:153:8}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
	# 00104 00140  ALFANUM IT-TX-MOTIVO
	CAMPO21=${LINHA:103:140}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
	
	# 00244 00080  NUM     IT-AN-INICIO-BASE-FORMULA(20 OCORRENCIAS FORMATO N4)
	multiplicos 20 4 ${LINHA:243:80}; CAMPO22=$valores;
	
	# 00324 00020  NUM     IT-IN-USO-VALOR-SALARIO-EDUCACAO (20 OCORRENCIAS FORMATO A1)
	multiplicos 20 1 ${LINHA:323:20}; CAMPO23=$valores;
	
	# 00344 00020  NUM     IT-IN-USO-VALOR-ATUALIZADO (20 OCORRENCIAS FORMATO A1)
	multiplicos 20 1 ${LINHA:343:20}; CAMPO24=$valores;
	
	# 00364 00020  NUM     IT-IN-USO-VALOR-DEDUCAO-SME (20 OCORRENCIAS FORMATO A1)
	multiplicos 20 1 ${LINHA:363:20}; CAMPO25=$valores;
	
	# 00384 00020  NUM     IT-IN-USO-VALOR-COMPENSACAO (20 OCORRENCIAS FORMATO A1)
	multiplicos 20 1 ${LINHA:363:20}; CAMPO25=$valores;
	
	# 00404 00020  NUM     IT-IN-USO-VALOR-MULTA-JUROS (20 OCORRENCIAS FORMATO A1)
	multiplicos 20 1 ${LINHA:403:20}; CAMPO26=$valores;
	
	# 00424 00001  NUM     IT-IN-MES-REFERENCIA
	CAMPO27=${LINHA:423:1}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	
	# 00425 00080  NUM     IT-AN-FIM-BASE-FORMULA (20 OCORRENCIAS FORMATO A4)
	multiplicos 20 4 ${LINHA:424:80}; CAMPO28=$valores;
	
	# 00505 00001  NUM     IT-IN-EXIGE-EMPENHO
	CAMPO28=${LINHA:504:1}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi

# CONSTANTE 42 - TABELA DOMICILIO BANCARIO
"42")
	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	# 00004 00011  NUM     IT-CO-USUARIO
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	# 00015 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
	CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	# 00023 00006  NUM     IT-CO-UNIDADE-GESTORA
	CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	# 00029 00005  NUM     IT-CO-GESTAO
	CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	# 00034 00003  NUM     IT-CO-BANCO-PRINCIPAL
	CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	# 00037 00004  NUM     IT-CO-AGENCIA-PRINCIPAL
	CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	# 00041 00010  ALFANUM IT-NU-CONTA-CORRENTE-PRINCIPAL
	CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	# 00051 00004  NUM     IT-CO-AGEN-UNICA-INSTITUCIONAL
	CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	# 00055 00003  NUM     IT-CO-BANCO-ALTERN
	CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	# 00058 00004  NUM     IT-CO-AGENCIA-ALTERN
	CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	# 00062 00060  NUM     IT-CO-BANCO - 20 VEZES DE 3 BYTES
	CAMPO12=${LINHA:153:8}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	# 00122 00080  NUM     IT-CO-AGENCIA - 20 VEZES DE 4 BYTES
	CAMPO13=${LINHA:153:8}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	# 00202 00200  ALFANUM IT-NU-CONTA-CORRENTE - 20 DE 10 BYTES
	CAMPO14=${LINHA:153:8}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
