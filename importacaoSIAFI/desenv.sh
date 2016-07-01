#!/bin/bash

TAB='\t';
NULO='\\N';
LIDOS='lidos/';
LENDO='Lendo/';
SQLCOPY='sqlCopy/';

REG_ORGAOGESTAO=0;
REG_UGSUBORGAO=0;
REG_DEPOSITOBANCARIO=0;
REG_DESTINACAOGR=0;
REG_GRUPOFINANCEIRO=0;
REG_RECOLHIMENTOUG=0;
REG_DOMICILIOBANCARIOCREDOR=0;
REG_TAXACONVERSAOMENSAL=0;
REG_CONVENIOPAGAMENTOFATURA=0;
REG_CONTROLEDECREDOR=0;
REG_DARFSISTEMA=0;
REG_GPSSISTEMA=0;
REG_INDITRANSFERENCIA=0;
REG_NATUREZARESPONSABILIDADE=0;
REG_SETORATIVIDADEECONIMICA=0;
REG_CODIGOBBGR=0;
REG_FPAS=0;
REG_INDICECORRECAOPROJUD=0;
REG_LIMITEEMPENHO=0;
REG_LRF=0;
REG_MOTIVOINADIPLENCIA=0;
REG_ORIGEMPRECATORIO=0;
REG_PROCESSOJUDICIAL=0;
REG_PREVISAO=0;
REG_RECOLHIMENTOGFIP=0;
REG_SALARIOEDUCACAO=0;
REG_DOMICILIOBANCARIO=0;

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
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.TAXACONVERSAOMENSAL.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.CONVENIOPAGAMENTOFATURA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.CONTROLEDECREDOR.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.DARFSISTEMA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.GPSSISTEMA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.INDITRANSFERENCIA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.NATUREZARESPONSABILIDADE.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.SETORATIVIDADEECONIMICA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.CODIGOBBGR.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.FPAS.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.INDICECORRECAOPROJUD.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.LIMITEEMPENHO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.LRF.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.MOTIVOINADIPLENCIA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.ORIGEMPRECATORIO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PROCESSOJUDICIAL.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PREVISAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.RECOLHIMENTOGFIP.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.SALARIOEDUCACAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.DOMICILIOBANCARIO.sql;

echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.orgaogestao ( it_in_operacao, it_co_usuario, it_da_operacao, gr_orgao, it_co_gestao, it_in_cod_bb_gr ) FROM stdin;" >> ${FILE}.ORGAOGESTAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.ugsuborgao ( it_in_operacao, it_co_usuario, it_da_operacao, it_co_suborgao, it_in_unidade_gestora ) FROM stdin;" >> ${FILE}.UGSUBORGAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.depositobancario (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_co_ug_gestao_deposito_tipo_dbuq,${TAB}it_no_deposito,${TAB}it_tx_descricao_deposito,${TAB}it_in_depositante,${TAB}it_in_inclusao_codigo_stn,${TAB}it_in_alteracao_campo_1,${TAB}it_in_alteracao_campo_2,${TAB}it_in_alteracao_campo_3,${TAB}it_in_alteracao_campo_4,${TAB}it_in_alteracao_campo_5,${TAB}it_in_alteracao_campo_6,${TAB}it_in_alteracao_campo_7,${TAB}it_in_alteracao_campo_8,${TAB}it_in_alteracao_campo_9,${TAB}it_in_alteracao_campo_10,${TAB}it_in_alteracao_campo_11,${TAB}it_in_alteracao_campo_12,${TAB}gr_codigo_evento_1,${TAB}gr_codigo_evento_2,${TAB}gr_codigo_evento_3,${TAB}gr_codigo_evento_4,${TAB}gr_codigo_evento_5,${TAB}gr_codigo_evento_6,${TAB}gr_codigo_evento_7,${TAB}gr_codigo_evento_8,${TAB}gr_codigo_evento_9,${TAB}gr_codigo_evento_10,${TAB}gr_codigo_evento_11,${TAB}gr_codigo_evento_12,${TAB}it_co_inscricao1_1,${TAB}it_co_inscricao1_2,${TAB}it_co_inscricao1_3,${TAB}it_co_inscricao1_4,${TAB}it_co_inscricao1_5,${TAB}it_co_inscricao1_6,${TAB}it_co_inscricao1_7,${TAB}it_co_inscricao1_8,${TAB}it_co_inscricao1_9,${TAB}it_co_inscricao1_10,${TAB}it_co_inscricao1_11,${TAB}it_co_inscricao1_12,${TAB}it_co_inscricao2_1,${TAB}it_co_inscricao2_2,${TAB}it_co_inscricao2_3,${TAB}it_co_inscricao2_4,${TAB}it_co_inscricao2_5,${TAB}it_co_inscricao2_6,${TAB}it_co_inscricao2_7,${TAB}it_co_inscricao2_8,${TAB}it_co_inscricao2_9,${TAB}it_co_inscricao2_10,${TAB}it_co_inscricao2_11,${TAB}it_co_inscricao2_12,${TAB}gr_classificacao1_1,${TAB}gr_classificacao1_2,${TAB}gr_classificacao1_3,${TAB}gr_classificacao1_4,${TAB}gr_classificacao1_5,${TAB}gr_classificacao1_6,${TAB}gr_classificacao1_7,${TAB}gr_classificacao1_8,${TAB}gr_classificacao1_9,${TAB}gr_classificacao1_10,${TAB}gr_classificacao1_11,${TAB}gr_classificacao1_12,${TAB}gr_classificacao2_1,${TAB}gr_classificacao2_2,${TAB}gr_classificacao2_3,${TAB}gr_classificacao2_4,${TAB}gr_classificacao2_5,${TAB}gr_classificacao2_6,${TAB}gr_classificacao2_7,${TAB}gr_classificacao2_8,${TAB}gr_classificacao2_9,${TAB}gr_classificacao2_10,${TAB}gr_classificacao2_11,${TAB}gr_classificacao2_12,${TAB}it_in_favorecido_1,${TAB}it_in_favorecido_2,${TAB}it_in_favorecido_3,${TAB}it_in_favorecido_4,${TAB}it_in_favorecido_5,${TAB}it_in_favorecido_6,${TAB}it_in_favorecido_7,${TAB}it_in_favorecido_8,${TAB}it_in_favorecido_9,${TAB}it_in_favorecido_10,${TAB}it_in_favorecido_11,${TAB}it_in_favorecido_12,${TAB}it_co_favorecido_1,${TAB}it_co_favorecido_2,${TAB}it_co_favorecido_3,${TAB}it_co_favorecido_4,${TAB}it_co_favorecido_5,${TAB}it_co_favorecido_6,${TAB}it_co_favorecido_7,${TAB}it_co_favorecido_8,${TAB}it_co_favorecido_9,${TAB}it_co_favorecido_10,${TAB}it_co_favorecido_11,${TAB}it_co_favorecido_12,${TAB}it_in_fracionamento_1,${TAB}it_in_fracionamento_2,${TAB}it_in_fracionamento_3,${TAB}it_in_fracionamento_4,${TAB}it_in_fracionamento_5,${TAB}it_in_fracionamento_6,${TAB}it_in_fracionamento_7,${TAB}it_in_fracionamento_8,${TAB}it_in_fracionamento_9,${TAB}it_in_fracionamento_10,${TAB}it_in_fracionamento_11,${TAB}it_in_fracionamento_12,${TAB}it_va_fracionamento_1,${TAB}it_va_fracionamento_2,${TAB}it_va_fracionamento_3,${TAB}it_va_fracionamento_4,${TAB}it_va_fracionamento_5,${TAB}it_va_fracionamento_6,${TAB}it_va_fracionamento_7,${TAB}it_va_fracionamento_8,${TAB}it_va_fracionamento_9,${TAB}it_va_fracionamento_10,${TAB}it_va_fracionamento_11,${TAB}it_va_fracionamento_12,${TAB}it_in_limite_vinculacao_1,${TAB}it_in_limite_vinculacao_2,${TAB}it_in_limite_vinculacao_3,${TAB}it_in_limite_vinculacao_4,${TAB}it_in_limite_vinculacao_5,${TAB}it_in_limite_vinculacao_6,${TAB}it_in_limite_vinculacao_7,${TAB}it_in_limite_vinculacao_8,${TAB}it_in_limite_vinculacao_9,${TAB}it_in_limite_vinculacao_10,${TAB}it_in_limite_vinculacao_11,${TAB}it_in_limite_vinculacao_12,${TAB}it_co_inscricao_maquina_1,${TAB}it_co_inscricao_maquina_2,${TAB}it_co_inscricao_maquina_3,${TAB}it_co_inscricao_maquina_4,${TAB}it_co_inscricao_maquina_5,${TAB}it_co_inscricao_maquina_6,${TAB}it_co_inscricao_maquina_7,${TAB}it_co_inscricao_maquina_8,${TAB}it_co_inscricao_maquina_9,${TAB}it_co_inscricao_maquina_10,${TAB}it_co_inscricao_maquina_11,${TAB}it_co_inscricao_maquina_12 ) FROM stdin;" >> ${FILE}.DEPOSITOBANCARIO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.destinacaogr (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_co_destinacao_gr,${TAB}it_no_destinacao_gr,${TAB}it_no_reduzido_destinacao_gr,${TAB}it_in_tipo_destinacao,${TAB}it_co_grupo_destinacao,${TAB}gr_fonte_recurso,${TAB}it_in_tipo_beneficiario,${TAB}it_co_ug_beneficiario,${TAB}it_co_gestao_beneficiario,${TAB}gr_codigo_evento_resti_fonte,${TAB}gr_codigo_evento_resti_dest,${TAB}it_tx_motivo,${TAB}it_pe_destinacao_principal,${TAB}it_pe_destinacao_secundaria_1,${TAB}it_pe_destinacao_secundaria_2,${TAB}it_pe_destinacao_secundaria_3,${TAB}it_pe_destinacao_secundaria_4,${TAB}it_pe_destinacao_secundaria_5,${TAB}it_pe_destinacao_secundaria_6,${TAB}it_pe_destinacao_secundaria_7,${TAB}it_pe_destinacao_secundaria_8,${TAB}it_pe_destinacao_secundaria_9,${TAB}it_pe_destinacao_secundaria_10,${TAB}it_co_destinacao_secundaria_1,${TAB}it_co_destinacao_secundaria_2,${TAB}it_co_destinacao_secundaria_3,${TAB}it_co_destinacao_secundaria_4,${TAB}it_co_destinacao_secundaria_5,${TAB}it_co_destinacao_secundaria_6,${TAB}it_co_destinacao_secundaria_7,${TAB}it_co_destinacao_secundaria_8,${TAB}it_co_destinacao_secundaria_9,${TAB}it_co_destinacao_secundaria_10,${TAB}gr_codigo_evento_arrec_fonte_1,${TAB}gr_codigo_evento_arrec_fonte_2,${TAB}gr_codigo_evento_arrec_fonte_3,${TAB}gr_codigo_evento_arrec_fonte_4,${TAB}gr_codigo_evento_arrec_fonte_5,${TAB}gr_codigo_evento_arrec_fonte_6,${TAB}gr_codigo_evento_arrec_fonte_7,${TAB}gr_codigo_evento_arrec_fonte_8,${TAB}gr_codigo_evento_arrec_fonte_9,${TAB}gr_codigo_evento_arrec_fonte_10,${TAB}gr_codigo_evento_arrec_dest_1,${TAB}gr_codigo_evento_arrec_dest_2,${TAB}gr_codigo_evento_arrec_dest_3,${TAB}gr_codigo_evento_arrec_dest_4,${TAB}gr_codigo_evento_arrec_dest_5,${TAB}gr_codigo_evento_arrec_dest_6,${TAB}gr_codigo_evento_arrec_dest_7,${TAB}gr_codigo_evento_arrec_dest_8,${TAB}gr_codigo_evento_arrec_dest_9,${TAB}gr_codigo_evento_arrec_dest_10,${TAB}gr_codigo_evento_retif_fonte_1,${TAB}gr_codigo_evento_retif_fonte_2,${TAB}gr_codigo_evento_retif_fonte_3,${TAB}gr_codigo_evento_retif_fonte_4,${TAB}gr_codigo_evento_retif_fonte_5,${TAB}gr_codigo_evento_retif_fonte_6,${TAB}gr_codigo_evento_retif_fonte_7,${TAB}gr_codigo_evento_retif_fonte_8,${TAB}gr_codigo_evento_retif_fonte_9,${TAB}gr_codigo_evento_retif_fonte_10,${TAB}gr_codigo_evento_retif_dest_1,${TAB}gr_codigo_evento_retif_dest_2,${TAB}gr_codigo_evento_retif_dest_3,${TAB}gr_codigo_evento_retif_dest_4,${TAB}gr_codigo_evento_retif_dest_5,${TAB}gr_codigo_evento_retif_dest_6,${TAB}gr_codigo_evento_retif_dest_7,${TAB}gr_codigo_evento_retif_dest_8,${TAB}gr_codigo_evento_retif_dest_9,${TAB}gr_codigo_evento_retif_dest_10 ) FROM stdin;" >> ${FILE}.DESTINACAOGR.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.grupofinanceiro (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_co_grupo_financeiro,${TAB}gr_financeiro_grupo_1,${TAB}gr_financeiro_grupo_2,${TAB}gr_financeiro_grupo_3,${TAB}gr_financeiro_grupo_4,${TAB}gr_financeiro_grupo_5,${TAB}gr_financeiro_grupo_6,${TAB}gr_financeiro_grupo_7,${TAB}gr_financeiro_grupo_8,${TAB}gr_financeiro_grupo_9,${TAB}gr_financeiro_grupo_10,${TAB}gr_financeiro_grupo_11,${TAB}gr_financeiro_grupo_12,${TAB}gr_financeiro_grupo_13,${TAB}gr_financeiro_grupo_14,${TAB}gr_financeiro_grupo_15,${TAB}gr_financeiro_grupo_16,${TAB}gr_financeiro_grupo_17,${TAB}gr_financeiro_grupo_18,${TAB}gr_financeiro_grupo_19,${TAB}gr_financeiro_grupo_20,${TAB}gr_financeiro_grupo_21,${TAB}gr_financeiro_grupo_22,${TAB}gr_financeiro_grupo_23,${TAB}gr_financeiro_grupo_24,${TAB}gr_financeiro_grupo_25,${TAB}gr_financeiro_grupo_26,${TAB}gr_financeiro_grupo_27,${TAB}gr_financeiro_grupo_28,${TAB}gr_financeiro_grupo_29,${TAB}gr_financeiro_grupo_30,${TAB}gr_financeiro_grupo_31,${TAB}gr_financeiro_grupo_32,${TAB}gr_financeiro_grupo_33,${TAB}gr_financeiro_grupo_34,${TAB}gr_financeiro_grupo_35,${TAB}gr_financeiro_grupo_36,${TAB}gr_financeiro_grupo_37,${TAB}gr_financeiro_grupo_38,${TAB}gr_financeiro_grupo_39,${TAB}gr_financeiro_grupo_40,${TAB}gr_financeiro_grupo_41,${TAB}gr_financeiro_grupo_42,${TAB}gr_financeiro_grupo_43,${TAB}gr_financeiro_grupo_44,${TAB}gr_financeiro_grupo_45,${TAB}gr_financeiro_grupo_46,${TAB}gr_financeiro_grupo_47,${TAB}gr_financeiro_grupo_48,${TAB}gr_financeiro_grupo_49,${TAB}gr_financeiro_grupo_50 ) FROM stdin;" >> ${FILE}.GRUPOFINANCEIRO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.recolhimentoug (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_co_terminal_usuario,${TAB}gr_ug_gestao_recolhimento,${TAB}it_in_preenchimento_gr_eletr_1,${TAB}it_in_preenchimento_gr_eletr_2,${TAB}it_in_preenchimento_gr_eletr_3,${TAB}it_in_preenchimento_gr_eletr_4,${TAB}it_in_preenchimento_gr_eletr_5,${TAB}it_in_preenchimento_gr_eletr_6,${TAB}it_in_preenchimento_gr_eletr_7,${TAB}it_in_preenchimento_gr_eletr_8,${TAB}it_in_preenchimento_gr_eletr_9,${TAB}it_in_preenchimento_gr_eletr_10,${TAB}it_in_preenchimento_gr_eletr_11,${TAB}it_in_preenchimento_gr_eletr_12,${TAB}it_in_altera_gr_eletronica_1,${TAB}it_in_altera_gr_eletronica_2,${TAB}it_in_altera_gr_eletronica_3,${TAB}it_in_altera_gr_eletronica_4,${TAB}it_in_altera_gr_eletronica_5,${TAB}it_in_altera_gr_eletronica_6,${TAB}it_in_altera_gr_eletronica_7,${TAB}it_in_altera_gr_eletronica_8,${TAB}it_in_altera_gr_eletronica_9,${TAB}it_in_altera_gr_eletronica_10,${TAB}it_in_altera_gr_eletronica_11,${TAB}it_in_altera_gr_eletronica_12,${TAB}gr_evento_arrecadacao_1,${TAB}gr_evento_arrecadacao_2,${TAB}gr_evento_arrecadacao_3,${TAB}gr_evento_arrecadacao_4,${TAB}gr_evento_arrecadacao_5,${TAB}gr_evento_arrecadacao_6,${TAB}gr_evento_arrecadacao_7,${TAB}gr_evento_arrecadacao_8,${TAB}gr_evento_arrecadacao_9,${TAB}gr_evento_arrecadacao_10,${TAB}gr_evento_arrecadacao_11,${TAB}gr_evento_arrecadacao_12,${TAB}it_co_inscricao1_arrecadacao_1,${TAB}it_co_inscricao1_arrecadacao_2,${TAB}it_co_inscricao1_arrecadacao_3,${TAB}it_co_inscricao1_arrecadacao_4,${TAB}it_co_inscricao1_arrecadacao_5,${TAB}it_co_inscricao1_arrecadacao_6,${TAB}it_co_inscricao1_arrecadacao_7,${TAB}it_co_inscricao1_arrecadacao_8,${TAB}it_co_inscricao1_arrecadacao_9,${TAB}it_co_inscricao1_arrecadacao_10,${TAB}it_co_inscricao1_arrecadacao_11,${TAB}it_co_inscricao1_arrecadacao_12,${TAB}it_co_inscricao2_arrecadacao_1,${TAB}it_co_inscricao2_arrecadacao_2,${TAB}it_co_inscricao2_arrecadacao_3,${TAB}it_co_inscricao2_arrecadacao_4,${TAB}it_co_inscricao2_arrecadacao_5,${TAB}it_co_inscricao2_arrecadacao_6,${TAB}it_co_inscricao2_arrecadacao_7,${TAB}it_co_inscricao2_arrecadacao_8,${TAB}it_co_inscricao2_arrecadacao_9,${TAB}it_co_inscricao2_arrecadacao_10,${TAB}it_co_inscricao2_arrecadacao_11,${TAB}it_co_inscricao2_arrecadacao_12,${TAB}gr_classificacao1_arrecadacao_1,${TAB}gr_classificacao1_arrecadacao_2,${TAB}gr_classificacao1_arrecadacao_3,${TAB}gr_classificacao1_arrecadacao_4,${TAB}gr_classificacao1_arrecadacao_5,${TAB}gr_classificacao1_arrecadacao_6,${TAB}gr_classificacao1_arrecadacao_7,${TAB}gr_classificacao1_arrecadacao_8,${TAB}gr_classificacao1_arrecadacao_9,${TAB}gr_classificacao1_arrecadacao_10,${TAB}gr_classificacao1_arrecadacao_11,${TAB}gr_classificacao1_arrecadacao_12,${TAB}gr_classificacao2_arrecadacao_1,${TAB}gr_classificacao2_arrecadacao_2,${TAB}gr_classificacao2_arrecadacao_3,${TAB}gr_classificacao2_arrecadacao_4,${TAB}gr_classificacao2_arrecadacao_5,${TAB}gr_classificacao2_arrecadacao_6,${TAB}gr_classificacao2_arrecadacao_7,${TAB}gr_classificacao2_arrecadacao_8,${TAB}gr_classificacao2_arrecadacao_9,${TAB}gr_classificacao2_arrecadacao_10,${TAB}gr_classificacao2_arrecadacao_11,${TAB}gr_classificacao2_arrecadacao_12,${TAB}it_in_altera_arrecadacao_1,${TAB}it_in_altera_arrecadacao_2,${TAB}it_in_altera_arrecadacao_3,${TAB}it_in_altera_arrecadacao_4,${TAB}it_in_altera_arrecadacao_5,${TAB}it_in_altera_arrecadacao_6,${TAB}it_in_altera_arrecadacao_7,${TAB}it_in_altera_arrecadacao_8,${TAB}it_in_altera_arrecadacao_9,${TAB}it_in_altera_arrecadacao_10,${TAB}it_in_altera_arrecadacao_11,${TAB}it_in_altera_arrecadacao_12,${TAB}gr_evento_retificacao_1,${TAB}gr_evento_retificacao_2,${TAB}gr_evento_retificacao_3,${TAB}gr_evento_retificacao_4,${TAB}gr_evento_retificacao_5,${TAB}gr_evento_retificacao_6,${TAB}gr_evento_retificacao_7,${TAB}gr_evento_retificacao_8,${TAB}gr_evento_retificacao_9,${TAB}gr_evento_retificacao_10,${TAB}gr_evento_retificacao_11,${TAB}gr_evento_retificacao_12,${TAB}it_co_inscricao1_retificacao_1,${TAB}it_co_inscricao1_retificacao_2,${TAB}it_co_inscricao1_retificacao_3,${TAB}it_co_inscricao1_retificacao_4,${TAB}it_co_inscricao1_retificacao_5,${TAB}it_co_inscricao1_retificacao_6,${TAB}it_co_inscricao1_retificacao_7,${TAB}it_co_inscricao1_retificacao_8,${TAB}it_co_inscricao1_retificacao_9,${TAB}it_co_inscricao1_retificacao_10,${TAB}it_co_inscricao1_retificacao_11,${TAB}it_co_inscricao1_retificacao_12,${TAB}it_co_inscricao2_retificacao_1,${TAB}it_co_inscricao2_retificacao_2,${TAB}it_co_inscricao2_retificacao_3,${TAB}it_co_inscricao2_retificacao_4,${TAB}it_co_inscricao2_retificacao_5,${TAB}it_co_inscricao2_retificacao_6,${TAB}it_co_inscricao2_retificacao_7,${TAB}it_co_inscricao2_retificacao_8,${TAB}it_co_inscricao2_retificacao_9,${TAB}it_co_inscricao2_retificacao_10,${TAB}it_co_inscricao2_retificacao_11,${TAB}it_co_inscricao2_retificacao_12,${TAB}gr_classificacao1_retificacao_1,${TAB}gr_classificacao1_retificacao_2,${TAB}gr_classificacao1_retificacao_3,${TAB}gr_classificacao1_retificacao_4,${TAB}gr_classificacao1_retificacao_5,${TAB}gr_classificacao1_retificacao_6,${TAB}gr_classificacao1_retificacao_7,${TAB}gr_classificacao1_retificacao_8,${TAB}gr_classificacao1_retificacao_9,${TAB}gr_classificacao1_retificacao_10,${TAB}gr_classificacao1_retificacao_11,${TAB}gr_classificacao1_retificacao_12,${TAB}gr_classificacao2_retificacao_1,${TAB}gr_classificacao2_retificacao_2,${TAB}gr_classificacao2_retificacao_3,${TAB}gr_classificacao2_retificacao_4,${TAB}gr_classificacao2_retificacao_5,${TAB}gr_classificacao2_retificacao_6,${TAB}gr_classificacao2_retificacao_7,${TAB}gr_classificacao2_retificacao_8,${TAB}gr_classificacao2_retificacao_9,${TAB}gr_classificacao2_retificacao_10,${TAB}gr_classificacao2_retificacao_11,${TAB}gr_classificacao2_retificacao_12,${TAB}it_in_altera_retificacao_1,${TAB}it_in_altera_retificacao_2,${TAB}it_in_altera_retificacao_3,${TAB}it_in_altera_retificacao_4,${TAB}it_in_altera_retificacao_5,${TAB}it_in_altera_retificacao_6,${TAB}it_in_altera_retificacao_7,${TAB}it_in_altera_retificacao_8,${TAB}it_in_altera_retificacao_9,${TAB}it_in_altera_retificacao_10,${TAB}it_in_altera_retificacao_11,${TAB}it_in_altera_retificacao_12,${TAB}gr_evento_restituicao_1,${TAB}gr_evento_restituicao_2,${TAB}gr_evento_restituicao_3,${TAB}gr_evento_restituicao_4,${TAB}gr_evento_restituicao_5,${TAB}gr_evento_restituicao_6,${TAB}gr_evento_restituicao_7,${TAB}gr_evento_restituicao_8,${TAB}gr_evento_restituicao_9,${TAB}gr_evento_restituicao_10,${TAB}gr_evento_restituicao_11,${TAB}gr_evento_restituicao_12,${TAB}it_co_inscricao1_restituicao_1,${TAB}it_co_inscricao1_restituicao_2,${TAB}it_co_inscricao1_restituicao_3,${TAB}it_co_inscricao1_restituicao_4,${TAB}it_co_inscricao1_restituicao_5,${TAB}it_co_inscricao1_restituicao_6,${TAB}it_co_inscricao1_restituicao_7,${TAB}it_co_inscricao1_restituicao_8,${TAB}it_co_inscricao1_restituicao_9,${TAB}it_co_inscricao1_restituicao_10,${TAB}it_co_inscricao1_restituicao_11,${TAB}it_co_inscricao1_restituicao_12,${TAB}it_co_inscricao2_restituicao_1,${TAB}it_co_inscricao2_restituicao_2,${TAB}it_co_inscricao2_restituicao_3,${TAB}it_co_inscricao2_restituicao_4,${TAB}it_co_inscricao2_restituicao_5,${TAB}it_co_inscricao2_restituicao_6,${TAB}it_co_inscricao2_restituicao_7,${TAB}it_co_inscricao2_restituicao_8,${TAB}it_co_inscricao2_restituicao_9,${TAB}it_co_inscricao2_restituicao_10,${TAB}it_co_inscricao2_restituicao_11,${TAB}it_co_inscricao2_restituicao_12,${TAB}gr_classificacao1_restituicao_1,${TAB}gr_classificacao1_restituicao_2,${TAB}gr_classificacao1_restituicao_3,${TAB}gr_classificacao1_restituicao_4,${TAB}gr_classificacao1_restituicao_5,${TAB}gr_classificacao1_restituicao_6,${TAB}gr_classificacao1_restituicao_7,${TAB}gr_classificacao1_restituicao_8,${TAB}gr_classificacao1_restituicao_9,${TAB}gr_classificacao1_restituicao_10,${TAB}gr_classificacao1_restituicao_11,${TAB}gr_classificacao1_restituicao_12,${TAB}gr_classificacao2_restituicao_1,${TAB}gr_classificacao2_restituicao_2,${TAB}gr_classificacao2_restituicao_3,${TAB}gr_classificacao2_restituicao_4,${TAB}gr_classificacao2_restituicao_5,${TAB}gr_classificacao2_restituicao_6,${TAB}gr_classificacao2_restituicao_7,${TAB}gr_classificacao2_restituicao_8,${TAB}gr_classificacao2_restituicao_9,${TAB}gr_classificacao2_restituicao_10,${TAB}gr_classificacao2_restituicao_11,${TAB}gr_classificacao2_restituicao_12,${TAB}it_in_altera_restituicao_1,${TAB}it_in_altera_restituicao_2,${TAB}it_in_altera_restituicao_3,${TAB}it_in_altera_restituicao_4,${TAB}it_in_altera_restituicao_5,${TAB}it_in_altera_restituicao_6,${TAB}it_in_altera_restituicao_7,${TAB}it_in_altera_restituicao_8,${TAB}it_in_altera_restituicao_9,${TAB}it_in_altera_restituicao_10,${TAB}it_in_altera_restituicao_11,${TAB}it_in_altera_restituicao_12,${TAB}it_co_destinacao_1,${TAB}it_co_destinacao_2,${TAB}it_co_destinacao_3,${TAB}it_co_destinacao_4,${TAB}it_co_destinacao_5,${TAB}it_co_destinacao_6,${TAB}it_co_destinacao_7,${TAB}it_co_destinacao_8,${TAB}it_co_destinacao_9,${TAB}it_co_destinacao_10,${TAB}it_co_destinacao_11,${TAB}it_co_destinacao_12,${TAB}it_in_altera_destinacao_1,${TAB}it_in_altera_destinacao_2,${TAB}it_in_altera_destinacao_3,${TAB}it_in_altera_destinacao_4,${TAB}it_in_altera_destinacao_5,${TAB}it_in_altera_destinacao_6,${TAB}it_in_altera_destinacao_7,${TAB}it_in_altera_destinacao_8,${TAB}it_in_altera_destinacao_9,${TAB}it_in_altera_destinacao_10,${TAB}it_in_altera_destinacao_11,${TAB}it_in_altera_destinacao_12,${TAB}it_tx_motivo,${TAB}it_in_situacao_recolhimento) FROM stdin;" >> ${FILE}.RECOLHIMENTOUG.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.domiciliobancariocredor (it_co_usuario,${TAB}it_co_terminal_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_pagador,${TAB}it_in_operacao,${TAB}it_co_credor_domicilio,${TAB}it_co_banco,${TAB}it_co_agencia,${TAB}it_co_conta,${TAB}it_in_conta_conjunta,${TAB}it_in_tipo_conta,${TAB}it_co_ug_supridora,${TAB}it_co_gestao_supridora) FROM stdin;" >> ${FILE}.DOMICILIOBANCARIOCREDOR.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.taxadeconversaomensal (it_co_gestao,${TAB}gr_orgao,${TAB}it_in_status,${TAB}it_in_operacao,${TAB}it_co_usuario,${TAB} it_da_operacao,${TAB} it_op_taxa_conversao_01,${TAB}it_op_taxa_conversao_02,${TAB}it_op_taxa_conversao_03,${TAB}it_op_taxa_conversao_04,${TAB} it_op_taxa_conversao_05,${TAB} it_op_taxa_conversao_06,${TAB} it_op_taxa_conversao_07,${TAB} it_op_taxa_conversao_08,${TAB} it_op_taxa_conversao_09,${TAB} it_op_taxa_conversao_10,${TAB}it_op_taxa_conversao_11,${TAB}it_op_taxa_conversao_12,${TAB}it_op_taxa_conversao_13,${TAB} it_it_da_atual_01,${TAB}it_it_da_atual_02,${TAB}it_it_da_atual_03,${TAB}it_it_da_atual_04,${TAB}it_it_da_atual_05,${TAB}it_it_da_atual_06,${TAB}it_it_da_atual_07,${TAB}it_it_da_atual_08,${TAB}it_it_da_atual_09,${TAB}it_it_da_atual_10,${TAB}it_it_da_atual_11,${TAB}it_it_da_atual_12,${TAB}it_it_da_atual_13,${TAB}it_op_taxa_media_01,${TAB}it_op_taxa_media_02,${TAB} it_op_taxa_media_03,${TAB} it_op_taxa_media_04,${TAB} it_op_taxa_media_05,${TAB} it_op_taxa_media_06,${TAB} it_op_taxa_media_07,${TAB} it_op_taxa_media_08,${TAB}it_op_taxa_media_09,${TAB}it_op_taxa_media_10,${TAB}it_op_taxa_media_11,${TAB}it_op_taxa_media_12,${TAB}it_op_taxa_media_13,${TAB}it_op_taxa_media_14,${TAB}it_co_moeda) FROM stdin;" >> ${FILE}.TAXACONVERSAOMENSAL.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.conveniopagamentofatura(it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_nu_convenio,${TAB}it_nu_cnpj,${TAB}it_no_empresa,${TAB}it_nu_segmento,${TAB}it_in_pagamento,${TAB}it_co_empresa,${TAB}it_tp_lista,${TAB}it_co_banco,${TAB}it_in_critica_vencimento) FROM stdin;" >> ${FILE}.CONVENIOPAGAMENTOFATURA.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.controledecredor(it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_operacao,${TAB}it_co_param_controle_credor,${TAB}it_no_param_controle_credor) FROM stdin;" >> ${FILE}.CONTROLEDECREDOR.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.darfsistema (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_unidade_gestora_saldo,${TAB}it_co_gestao_saldo,${TAB}gr_codigo_conta,${TAB}it_co_conta_corrente_contabil,${TAB}it_da_execucao,${TAB}it_in_tipo_execucao,${TAB}it_qt_dias_uteis,${TAB}it_co_unidade_gestora_emit,${TAB}it_co_gestao_emit,${TAB}it_co_favorecido,${TAB}it_co_receita,${TAB}it_in_tipo_recurso,${TAB}it_co_ug_doc_referencia,${TAB}it_co_gestao_doc_referencia,${TAB}gr_an_nu_documento_referencia,${TAB}gr_fonte_recurso,${TAB}it_co_vinc_pagamento,${TAB}it_co_grupo_despesa,${TAB}it_nu_processo,${TAB}it_nu_referencia,${TAB}it_tx_observacao ) FROM stdin;" >> ${FILE}.DARFSISTEMA.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.gpssistema ( it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_unidade_gestora_saldo,${TAB}it_co_gestao_saldo,${TAB}gr_codigo_conta,${TAB}it_co_conta_corrente_contabil,${TAB}it_da_execucao,${TAB}it_in_tipo_execucao,${TAB}it_qt_dias_uteis,${TAB}it_co_unidade_gestora_emit,${TAB}it_co_gestao_emit,${TAB}it_co_recolhedor,${TAB}it_co_pagamento,${TAB}it_in_tipo_recurso,${TAB}it_co_ug_doc_referencia,${TAB}it_co_gestao_doc_referencia,${TAB}gr_an_nu_documento_referencia,${TAB}gr_fonte_recurso,${TAB}it_co_vinc_pagamento,${TAB}it_co_grupo_despesa,${TAB}it_nu_processo,${TAB}it_tx_observacao ) FROM stdin;" >> ${FILE}.GPSSISTEMA.sql;

echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.inditransferencia (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_co_cit,${TAB}it_no_formula character(160),it_tx_descricao,${TAB}it_tx_motivo,${TAB}gr_orgao_vinculado_01,${TAB}gr_orgao_vinculado_02,${TAB} gr_orgao_vinculado_03,${TAB} gr_orgao_vinculado_04,${TAB} gr_orgao_vinculado_05,${TAB} gr_orgao_vinculado_06,${TAB} gr_orgao_vinculado_07,${TAB} gr_orgao_vinculado_08,${TAB} gr_orgao_vinculado_09,${TAB} gr_orgao_vinculado_10,${TAB}it_in_tipo_ob_01,${TAB} it_in_tipo_ob_02,${TAB} it_in_tipo_ob_03,${TAB}it_in_tipo_ob_04,${TAB}it_in_tipo_ob_05,${TAB} it_in_tipo_ob_06,${TAB}it_in_tipo_ob_07,${TAB} it_in_tipo_ob_08,${TAB} it_in_tipo_ob_09,${TAB} it_in_tipo_ob_10,${TAB}it_in_obrigatoriedade_lista_01,${TAB} it_in_obrigatoriedade_lista_02,${TAB} it_in_obrigatoriedade_lista_03,${TAB} it_in_obrigatoriedade_lista_04,${TAB} it_in_obrigatoriedade_lista_05,${TAB} it_in_obrigatoriedade_lista_06,${TAB} it_in_obrigatoriedade_lista_07,${TAB} it_in_obrigatoriedade_lista_08,${TAB} it_in_obrigatoriedade_lista_09,${TAB} it_in_obrigatoriedade_lista_10 ) FROM stdin;" >> ${FILE}.INDITRANSFERENCIA.sql;

# CONSTANTE 23 - TABELA DE NATUREZA RESPONSABILIDADE
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.naturezaresponsabilidade ( it_in_operacao,${TAB}it_co_usuario,${TAB} it_da_operacao numeric(8),it_ho_operacao,${TAB}it_co_natureza_responsabilidade,${TAB} it_no_natureza_responsabilidade,${TAB}it_no_mnemonico_natureza_resp,${TAB}it_co_nat_resp_substituta ) FROM stdin;" >> ${FILE}.NATUREZARESPONSABILIDADE.sql;

#CONSTANTE 26 - TABELA DE SETOR ATIVIDADE ECONOMICA
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.setoratividadeeconomica (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_operacao,${TAB} it_co_setor_ativ_economica_velho,${TAB} it_no_setor_ativ_economica,${TAB}it_co_setor_ativ_economica ) FROM stdin;" >> ${FILE}.SETORATIVIDADEECONIMICA.sql;

#CONSTANTE 28 - TABELA DE CODIGO BB/GR
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.codigobbgr(it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_in_operacao,${TAB} it_co_bb_gr,${TAB} it_co_unidade_gestora,${TAB}it_co_gestao,${TAB}it_in_cod_bb_gr ) FROM stdin;" >> ${FILE}.CODIGOBBGR.sql;

#CONSTANTE 29 - TABELA DE FPAS
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.fpas ( it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB} it_co_ug_operador,${TAB} it_in_operacao,${TAB} it_co_fpas,${TAB} it_no_fpas,${TAB} it_co_entidade_01,${TAB}it_co_entidade_02,${TAB}it_co_entidade_03,${TAB}it_co_entidade_04,${TAB}it_co_entidade_05,${TAB}it_co_entidade_06,${TAB}it_co_entidade_07,${TAB}it_co_entidade_08,${TAB}it_co_entidade_09,${TAB}it_co_entidade_10,${TAB}it_co_entidade_11,${TAB}it_co_entidade_12,${TAB}it_co_entidade_13,${TAB}it_co_entidade_14,${TAB}it_co_entidade_15,${TAB}it_co_entidade_16,${TAB}it_co_entidade_17,${TAB}it_co_entidade_18,${TAB}it_co_entidade_19,${TAB}it_co_entidade_20,${TAB}it_co_entidade_21,${TAB}it_co_entidade_22,${TAB}it_co_entidade_23,${TAB}it_co_entidade_24,${TAB}it_co_entidade_25,${TAB}it_co_entidade_26,${TAB}it_co_entidade_27,${TAB}it_co_entidade_28,${TAB}it_co_entidade_29,${TAB}it_co_entidade_30,${TAB}it_co_entidade_31,${TAB}it_co_entidade_32,${TAB}it_co_entidade_33,${TAB}it_co_entidade_34,${TAB}it_co_entidade_35,${TAB}it_co_entidade_36,${TAB}it_co_entidade_37,${TAB}it_co_entidade_38,${TAB}it_co_entidade_39,${TAB}it_co_entidade_40,${TAB}it_co_entidade_41,${TAB}it_co_entidade_42,${TAB}it_co_entidade_43,${TAB}it_co_entidade_44,${TAB}it_co_entidade_45,${TAB}it_co_entidade_46,${TAB}it_co_entidade_47,${TAB}it_co_entidade_48,${TAB}it_co_entidade_49,${TAB}it_co_entidade_50,${TAB} it_tx_mot ) FROM stdin;" >> ${FILE}.FPAS.sql;

# CONSTANTE 31 - TABELA INDICE CORRECAO PROJUD
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.indicecorrecaoprejud ( it_co_usuario,${TAB}it_da_transacao,${TAB} it_ho_transacao,${TAB}it_co_ug_operador,${TAB} it_in_operacao,${TAB} it_nu_lei,${TAB} it_no_exigencia_legal,${TAB} it_tx_instrucao_norma_01,${TAB}it_tx_instrucao_norma_02,${TAB} it_tx_instrucao_norma_03,${TAB} it_tx_instrucao_norma_04,${TAB} it_tx_instrucao_norma_05,${TAB}it_tx_instrucao_norma_06,${TAB}it_tx_instrucao_norma_07,${TAB}it_tx_instrucao_norma_08,${TAB}it_tx_instrucao_norma_09,${TAB}it_tx_instrucao_norma_10,${TAB}it_tx_instrucao_norma_11,${TAB}it_tx_instrucao_norma_12,${TAB}it_tx_instrucao_norma_13,${TAB}it_tx_instrucao_norma_14,${TAB}it_tx_instrucao_norma_15,${TAB}it_tx_instrucao_norma_16,${TAB}it_tx_instrucao_norma_17,${TAB}it_tx_instrucao_norma_18,${TAB}it_tx_instrucao_norma_19,${TAB}it_tx_instrucao_norma_20,${TAB}it_tx_instrucao_norma_21,${TAB}it_tx_instrucao_norma_22,${TAB}it_tx_instrucao_norma_23,${TAB}it_tx_instrucao_norma_24,${TAB}it_tx_instrucao_norma_25,${TAB}it_tx_instrucao_norma_26,${TAB}it_tx_instrucao_norma_27,${TAB}it_tx_instrucao_norma_28,${TAB}it_tx_instrucao_norma_29,${TAB}it_tx_instrucao_norma_30,${TAB}it_in_lei,${TAB}it_nu_subgrupo,${TAB}it_in_visivel) FROM stdin;" >> ${FILE}.INDICECORRECAOPREJUD.sql;

# CONSTANTE 33 - TABELA DE LIMITE EMPENHO
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.limiteempenho( it_co_usuario,${TAB}it_co_terminal_usuario,${TAB} it_da_transacao,${TAB}it_ho_transacao,${TAB} it_co_ug_operador,${TAB} it_in_operacao,${TAB} it_co_motivo_inadiplencia,${TAB} it_no_motivo_inadiplencia,${TAB}it_co_grupo_motivo_inadiplencia,${TAB} it_tx_motivo_01,${TAB}it_tx_motivo_02,${TAB} it_tx_motivo_03,${TAB}it_tx_motivo_04,${TAB}it_tx_motivo_05,${TAB}it_tx_motivo_06,${TAB}it_tx_motivo_07,${TAB}it_tx_motivo_08,${TAB}it_tx_motivo_09,${TAB}it_tx_motivo_10,${TAB}it_tx_motivo_11,${TAB}it_tx_motivo_12,${TAB}it_tx_motivo_13,${TAB}it_tx_motivo_14,${TAB}it_tx_motivo_15 ) FROM stdin;" >> ${FILE}.LIMITEEMPENHO.sql;

# CONSTANTE 34 - TABELA DE LRF
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.lrf(  it_co_usuario,${TAB} it_da_transacao,${TAB}  it_ho_transacao,${TAB}  it_co_ug_operador,${TAB}    it_in_operacao,${TAB} it_nu_lei,${TAB} it_no_exigencia_legal,${TAB} it_tx_instrucao_norma_01,${TAB} it_tx_instrucao_norma_02,${TAB} it_tx_instrucao_norma_03,${TAB}  it_tx_instrucao_norma_04,${TAB} it_tx_instrucao_norma_05,${TAB} it_tx_instrucao_norma_06,${TAB} it_tx_instrucao_norma_07,${TAB}it_tx_instrucao_norma_08,${TAB}it_tx_instrucao_norma_09,${TAB}it_tx_instrucao_norma_10,${TAB}it_tx_instrucao_norma_11,${TAB}it_tx_instrucao_norma_12,${TAB}it_tx_instrucao_norma_13,${TAB}it_tx_instrucao_norma_14,${TAB}it_tx_instrucao_norma_15,${TAB}it_tx_instrucao_norma_16,${TAB} it_tx_instrucao_norma_17,${TAB}it_tx_instrucao_norma_18,${TAB}it_tx_instrucao_norma_19,${TAB}it_tx_instrucao_norma_20,${TAB}it_tx_instrucao_norma_21,${TAB}it_tx_instrucao_norma_22,${TAB}it_tx_instrucao_norma_23,${TAB}it_tx_instrucao_norma_24,${TAB}it_tx_instrucao_norma_25,${TAB}it_tx_instrucao_norma_26,${TAB}it_tx_instrucao_norma_27,${TAB}it_tx_instrucao_norma_28,${TAB}it_tx_instrucao_norma_29,${TAB}it_tx_instrucao_norma_30,${TAB}it_in_lei,${TAB} it_nu_subgrupo,${TAB}it_in_visivel) FROM stdin;" >> ${FILE}.LRF.sql; 

# CONSTANTE 36 - TABELA DE MOTIVO INADIMPLENCIA
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.motivoinadiplencia ( it_co_usuario,${TAB}it_co_terminal_usuario,${TAB} it_da_transacao,${TAB}it_ho_transacao,${TAB}  it_co_ug_operador,${TAB}it_in_operacao,${TAB} it_co_motivo_inadiplencia,${TAB} it_no_motivo_inadiplencia,${TAB}it_co_grupo_motivo_inadiplencia,${TAB}it_tx_motivo_01,${TAB} it_tx_motivo_02,${TAB}it_tx_motivo_03,${TAB}it_tx_motivo_04,${TAB}it_tx_motivo_05,${TAB}it_tx_motivo_06,${TAB}it_tx_motivo_07,${TAB}it_tx_motivo_08,${TAB}it_tx_motivo_09,${TAB}it_tx_motivo_10,${TAB}it_tx_motivo_11,${TAB}it_tx_motivo_12,${TAB}it_tx_motivo_13,${TAB}it_tx_motivo_14,${TAB}it_tx_motivo_15) FROM stdin;" >> ${FILE}.MOTIVOINADIPLENCIA.sql; 

# CONSTANTE 37 - TABELA DE ORIGEM PRECATORIO
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.origemprecatorio ( it_co_usuario,${TAB}it_co_terminal_usuario,${TAB} it_da_transacao,${TAB}it_ho_transacao,${TAB}  it_co_ug_operador,${TAB}it_in_operacao,${TAB} it_co_orgao_cadastrador,${TAB} it_co_origem_precatorio,${TAB}it_co_orgao_pagador,${TAB}it_co_minicipio,${TAB} it_co_uf,${TAB} it_no_titulo_origem,${TAB}it_no_mnemonico_titulo_origem ) FROM stdin;" >> ${FILE}.ORIGEMPRECATORIO.sql; 

# CONSTANTE 38 - TABELA DE PROCESSO JUDICIAL
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.processojudicial(it_co_usuario,${TAB}it_da_transacao,${TAB} it_ho_transacao,${TAB}it_co_ug_operador,${TAB} it_in_operacao,${TAB}it_co_ug_cadastradora,${TAB}it_co_gestao_cadastradora,${TAB}it_nu_precatorio,${TAB}it_in_setenca,${TAB}it_in_situacao,${TAB}it_co_unidade_gestora_pagadora,${TAB}it_co_gestao_pagadora,${TAB}it_no_requerente character(100),it_co_identificacao_requerente,${TAB}it_co_natureza_despesa,${TAB} it_in_tipo_despesa,${TAB} it_co_orgao_reu,${TAB}it_co_unidade_orcamentaria,${TAB} it_tx_descricao_precator,${TAB} it_co_vara_origem,${TAB}it_da_atuacao,${TAB}it_in_tipo_justica,${TAB} it_co_acao_originaria,${TAB} it_co_vara_comarca,${TAB} it_da_ajuizamento_acao,${TAB} it_co_assunto numeric(8), it_an_proposta_orcamentaria,${TAB}it_in_natureza_alimenticia,${TAB} it_in_bloqueado,${TAB}it_va_precatorio,${TAB}it_da_valor,${TAB}it_va_atual,${TAB} it_co_valor_atual,${TAB}it_co_orgao_cadastrador,${TAB}it_in_numero_sequencial,${TAB} it_va_retencao,${TAB}it_in_setenca_pequeno_valor,${TAB}it_qt_beneficiario,${TAB} it_co_banco_01,${TAB} it_co_banco_02,${TAB} it_co_banco_03,${TAB} it_co_banco_04,${TAB} it_co_banco_05,${TAB} it_co_banco_06,${TAB} it_co_banco_07,${TAB} it_co_banco_08,${TAB} it_co_banco_09,${TAB} it_co_banco_10,${TAB} it_co_agencia_01,${TAB} it_co_agencia_02,${TAB} it_co_agencia_03,${TAB} it_co_agencia_04,${TAB} it_co_agencia_05,${TAB} it_co_agencia_06,${TAB} it_co_agencia_07,${TAB} it_co_agencia_08,${TAB} it_co_agencia_09,${TAB} it_co_agencia_10,${TAB} it_nu_conta_corrente_01,${TAB} it_nu_conta_corrente_02,${TAB} it_nu_conta_corrente_03,${TAB} it_nu_conta_corrente_04,${TAB} it_nu_conta_corrente_05,${TAB} it_nu_conta_corrente_06,${TAB} it_nu_conta_corrente_07,${TAB} it_nu_conta_corrente_08,${TAB} it_nu_conta_corrente_09,${TAB} it_nu_conta_corrente_10,${TAB} it_va_parcela_01,${TAB}it_va_parcela_02,${TAB}it_va_parcela_03,${TAB}it_va_parcela_04,${TAB}it_va_parcela_05,${TAB}it_va_parcela_06,${TAB}it_va_parcela_07,${TAB}it_va_parcela_08,${TAB}it_va_parcela_09,${TAB}it_va_parcela_10,${TAB}it_va_parcela_retencao_01,${TAB} it_va_parcela_retencao_02,${TAB} it_va_parcela_retencao_03,${TAB} it_va_parcela_retencao_04,${TAB} it_va_parcela_retencao_05,${TAB} it_va_parcela_retencao_06,${TAB} it_va_parcela_retencao_07,${TAB} it_va_parcela_retencao_08,${TAB} it_va_parcela_retencao_09,${TAB} it_va_parcela_retencao_10,${TAB} it_va_parcela_liquida_01,${TAB}it_va_parcela_liquida_02,${TAB} it_va_parcela_liquida_03,${TAB}it_va_parcela_liquida_04,${TAB}it_va_parcela_liquida_05,${TAB}it_va_parcela_liquida_06,${TAB}it_va_parcela_liquida_07,${TAB}it_va_parcela_liquida_08,${TAB}it_va_parcela_liquida_09,${TAB}it_va_parcela_liquida_10,${TAB}it_da_previsao_vencimento_01,${TAB} it_da_previsao_vencimento_02,${TAB} it_da_previsao_vencimento_03,${TAB} it_da_previsao_vencimento_04,${TAB} it_da_previsao_vencimento_05,${TAB} it_da_previsao_vencimento_06,${TAB}it_da_previsao_vencimento_07,${TAB} it_da_previsao_vencimento_08,${TAB} it_da_previsao_vencimento_09,${TAB} it_da_previsao_vencimento_10,${TAB} it_va_parcela_a_pagar_01,${TAB}it_va_parcela_a_pagar_02,${TAB}it_va_parcela_a_pagar_03,${TAB}it_va_parcela_a_pagar_04,${TAB}it_va_parcela_a_pagar_05,${TAB}it_va_parcela_a_pagar_06,${TAB}it_va_parcela_a_pagar_07,${TAB}it_va_parcela_a_pagar_08,${TAB}it_va_parcela_a_pagar_09,${TAB}it_va_parcela_a_pagar_10,${TAB}it_va_parcela_pago_01,${TAB} it_va_parcela_pago_02,${TAB} it_va_parcela_pago_03,${TAB} it_va_parcela_pago_04,${TAB} it_va_parcela_pago_05,${TAB} it_va_parcela_pago_06,${TAB} it_va_parcela_pago_07,${TAB} it_va_parcela_pago_08,${TAB} it_va_parcela_pago_09,${TAB} it_va_parcela_pago_10,${TAB} it_va_complemento_pago_01,${TAB} it_va_complemento_pago_02,${TAB} it_va_complemento_pago_03,${TAB} it_va_complemento_pago_04,${TAB} it_va_complemento_pago_05,${TAB} it_va_complemento_pago_06,${TAB} it_va_complemento_pago_07,${TAB} it_va_complemento_pago_08,${TAB} it_va_complemento_pago_09,${TAB} it_va_complemento_pago_10,${TAB} it_nu_parcela_complemento_01,${TAB} it_nu_parcela_complemento_02,${TAB} it_nu_parcela_complemento_03,${TAB} it_nu_parcela_complemento_04,${TAB} it_nu_parcela_complemento_05,${TAB} it_nu_parcela_complemento_06,${TAB} it_nu_parcela_complemento_07,${TAB} it_nu_parcela_complemento_08,${TAB} it_nu_parcela_complemento_09,${TAB} it_nu_parcela_complemento_10,${TAB} it_sq_complemento_01,${TAB} it_sq_complemento_02,${TAB} it_sq_complemento_03,${TAB} it_sq_complemento_04,${TAB} it_sq_complemento_05,${TAB} it_sq_complemento_06,${TAB} it_sq_complemento_07,${TAB} it_sq_complemento_08,${TAB} it_sq_complemento_09,${TAB} it_sq_complemento_10,${TAB} it_va_complemento_01,${TAB}it_va_complemento_02,${TAB}it_va_complemento_03,${TAB}it_va_complemento_04,${TAB}it_va_complemento_05,${TAB}it_va_complemento_06,${TAB}it_va_complemento_07,${TAB}it_va_complemento_08,${TAB}it_va_complemento_09,${TAB}it_va_complemento_10,${TAB}it_va_complemento_retencao_01,${TAB} it_va_complemento_retencao_02,${TAB} it_va_complemento_retencao_03,${TAB} it_va_complemento_retencao_04,${TAB} it_va_complemento_retencao_05,${TAB} it_va_complemento_retencao_06,${TAB} it_va_complemento_retencao_07,${TAB} it_va_complemento_retencao_08,${TAB} it_va_complemento_retencao_09,${TAB} it_va_complemento_retencao_10,${TAB} it_da_complemento_vencimento_01,${TAB}it_da_complemento_vencimento_02,${TAB}it_da_complemento_vencimento_03,${TAB}it_da_complemento_vencimento_04,${TAB}it_da_complemento_vencimento_05,${TAB}it_da_complemento_vencimento_06,${TAB}it_da_complemento_vencimento_07,${TAB}it_da_complemento_vencimento_08,${TAB}it_da_complemento_vencimento_09,${TAB}it_da_complemento_vencimento_10,${TAB}it_va_complemento_a_pagar_01,${TAB}it_va_complemento_a_pagar_02,${TAB}it_va_complemento_a_pagar_03,${TAB}it_va_complemento_a_pagar_04,${TAB}it_va_complemento_a_pagar_05,${TAB}it_va_complemento_a_pagar_06,${TAB}it_va_complemento_a_pagar_07,${TAB}it_va_complemento_a_pagar_08,${TAB}it_va_complemento_a_pagar_09,${TAB}it_va_complemento_a_pagar_10,${TAB}it_nu_parcela_devolucao_01,${TAB} it_nu_parcela_devolucao_02,${TAB} it_nu_parcela_devolucao_03,${TAB} it_nu_parcela_devolucao_04,${TAB} it_nu_parcela_devolucao_05,${TAB} it_nu_parcela_devolucao_06,${TAB}it_nu_parcela_devolucao_07,${TAB}it_nu_parcela_devolucao_08,${TAB}it_nu_parcela_devolucao_09,${TAB}it_nu_parcela_devolucao_10,${TAB}it_sq_devolucao_01,${TAB} it_sq_devolucao_02,${TAB} it_sq_devolucao_03,${TAB} it_sq_devolucao_04,${TAB} it_sq_devolucao_05,${TAB} it_sq_devolucao_06,${TAB}it_sq_devolucao_07,${TAB}it_sq_devolucao_08,${TAB}it_sq_devolucao_09,${TAB}it_sq_devolucao_10,${TAB} it_va_devolucao_01,${TAB}it_va_devolucao_02,${TAB}it_va_devolucao_03,${TAB}it_va_devolucao_04,${TAB}it_va_devolucao_05,${TAB}it_va_devolucao_06,${TAB}it_va_devolucao_07,${TAB}it_va_devolucao_08,${TAB}it_va_devolucao_09,${TAB}it_va_devolucao_10,${TAB}it_nu_documento_devolucao_01,${TAB}it_nu_documento_devolucao_02,${TAB}it_nu_documento_devolucao_03,${TAB}it_nu_documento_devolucao_04,${TAB}it_nu_documento_devolucao_05,${TAB}it_nu_documento_devolucao_06,${TAB}it_nu_documento_devolucao_07,${TAB}it_nu_documento_devolucao_08,${TAB}it_nu_documento_devolucao_09,${TAB}it_nu_documento_devolucao_10,${TAB}it_in_operacao_devolucao_01,${TAB}it_in_operacao_devolucao_02,${TAB}it_in_operacao_devolucao_03,${TAB}it_in_operacao_devolucao_04,${TAB}it_in_operacao_devolucao_05,${TAB}it_in_operacao_devolucao_06,${TAB}it_in_operacao_devolucao_07,${TAB}it_in_operacao_devolucao_08,${TAB}it_in_operacao_devolucao_09,${TAB}it_in_operacao_devolucao_10 ) FROM stdin;" >> ${FILE}.PROCESSOJUDICIAL.sql; 

# CONSTANTE 39 - TABELA DE PREVISAO
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.previsao ( it_co_usuario,${TAB}it_co_terminal_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB} it_co_ug_operador,${TAB} it_in_operacao,${TAB} it_co_previsao,${TAB} it_no_previsao,${TAB} it_no_mnemonico_previsao,${TAB} it_co_evento ) FROM stdin;" >> ${FILE}.PREVISAO.sql;

# CONSTANTE 40 - TABELA DE RECOLHIMENTO GFIP
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.recolhimentogfip(it_co_usuario,${TAB}it_da_transacao,${TAB}it_ho_transacao,${TAB}it_co_ug_operador,${TAB}it_in_operacao,${TAB} it_co_gfip,${TAB}it_no_gfip,${TAB}it_in_exige_competencia,${TAB} it_me_inicio,${TAB} it_an_inicio,${TAB} it_me_termino,${TAB}it_an_termino,${TAB}it_tx_motivo,${TAB} it_co_correlacao_barra,${TAB} it_in_exige_empenho ) FROM stdin;" >> ${FILE}.RECOLHIMENTOGFIP.sql; 

# CONSTANTE 41 - TABELA DE SALARIO EDUCACAO
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.salarioeducacao( it_co_usuario,${TAB} it_da_transacao,${TAB}it_ho_transacao,${TAB} it_co_ug_operador,${TAB} it_in_operacao,${TAB} it_co_salario_educacao,${TAB}it_no_salario_educacao,${TAB} it_in_competencia,${TAB}it_an_inicio,${TAB}it_an_referencia,${TAB}it_in_parcela,${TAB} it_in_processo_exec_fiscal,${TAB}it_in_calculo_dv,${TAB}it_in_base_contribuicao,${TAB} it_in_salario_educacao,${TAB}it_pe_salario_educacao_1,${TAB}it_pe_salario_educacao_2,${TAB}it_in_deducao_sme,${TAB} it_in_compensacao,${TAB} it_in_valor_atualizado,${TAB}it_in_multa_juros,${TAB} it_tx_motivo,${TAB}it_an_inicio_base_formula_01,${TAB}it_an_inicio_base_formula_02,${TAB}it_an_inicio_base_formula_03,${TAB}it_an_inicio_base_formula_04,${TAB}it_an_inicio_base_formula_05,${TAB}it_an_inicio_base_formula_06,${TAB}it_an_inicio_base_formula_07,${TAB}it_an_inicio_base_formula_08,${TAB}it_an_inicio_base_formula_09,${TAB}it_an_inicio_base_formula_10,${TAB}it_an_inicio_base_formula_11,${TAB}it_an_inicio_base_formula_12,${TAB}it_an_inicio_base_formula_13,${TAB} it_an_inicio_base_formula_14,${TAB}it_an_inicio_base_formula_15,${TAB}it_an_inicio_base_formula_16,${TAB}it_an_inicio_base_formula_17,${TAB}it_an_inicio_base_formula_18,${TAB}it_an_inicio_base_formula_19,${TAB}it_an_inicio_base_formula_20,${TAB}it_in_uso_valor_salario_educacao_01,${TAB} it_in_uso_valor_salario_educacao_02,${TAB} it_in_uso_valor_salario_educacao_03,${TAB} it_in_uso_valor_salario_educacao_04,${TAB} it_in_uso_valor_salario_educacao_05,${TAB} it_in_uso_valor_salario_educacao_06,${TAB} it_in_uso_valor_salario_educacao_07,${TAB} it_in_uso_valor_salario_educacao_08,${TAB} it_in_uso_valor_salario_educacao_09,${TAB}it_in_uso_valor_salario_educacao_10,${TAB}it_in_uso_valor_salario_educacao_11,${TAB}it_in_uso_valor_salario_educacao_12,${TAB}it_in_uso_valor_salario_educacao_13,${TAB} it_in_uso_valor_salario_educacao_14,${TAB}it_in_uso_valor_salario_educacao_15,${TAB}it_in_uso_valor_salario_educacao_16,${TAB}it_in_uso_valor_salario_educacao_17,${TAB}it_in_uso_valor_salario_educacao_18,${TAB}it_in_uso_valor_salario_educacao_19,${TAB}it_in_uso_valor_salario_educacao_20,${TAB}it_in_uso_valor_atualizado_01,${TAB} it_in_uso_valor_atualizado_02,${TAB} it_in_uso_valor_atualizado_03,${TAB} it_in_uso_valor_atualizado_04,${TAB} it_in_uso_valor_atualizado_05,${TAB} it_in_uso_valor_atualizado_06,${TAB} it_in_uso_valor_atualizado_07,${TAB} it_in_uso_valor_atualizado_08,${TAB}it_in_uso_valor_atualizado_09,${TAB}it_in_uso_valor_atualizado_10,${TAB}it_in_uso_valor_atualizado_11,${TAB}it_in_uso_valor_atualizado_12,${TAB}it_in_uso_valor_atualizado_13,${TAB} it_in_uso_valor_atualizado_14,${TAB}it_in_uso_valor_atualizado_15,${TAB}it_in_uso_valor_atualizado_16,${TAB}it_in_uso_valor_atualizado_17,${TAB}it_in_uso_valor_atualizado_18,${TAB}it_in_uso_valor_atualizado_19,${TAB}it_in_uso_valor_atualizado_20,${TAB}it_in_uso_valor_deducao_sme_01,${TAB} it_in_uso_valor_deducao_sme_02,${TAB}it_in_uso_valor_deducao_sme_03,${TAB}it_in_uso_valor_deducao_sme_04,${TAB}it_in_uso_valor_deducao_sme_05,${TAB}it_in_uso_valor_deducao_sme_06,${TAB}it_in_uso_valor_deducao_sme_07,${TAB} it_in_uso_valor_deducao_sme_08,${TAB}it_in_uso_valor_deducao_sme_09,${TAB}it_in_uso_valor_deducao_sme_10,${TAB}it_in_uso_valor_deducao_sme_11,${TAB}it_in_uso_valor_deducao_sme_12,${TAB}it_in_uso_valor_deducao_sme_13,${TAB} it_in_uso_valor_deducao_sme_14,${TAB}it_in_uso_valor_deducao_sme_15,${TAB}it_in_uso_valor_deducao_sme_16,${TAB}it_in_uso_valor_deducao_sme_17,${TAB}it_in_uso_valor_deducao_sme_18,${TAB}it_in_uso_valor_deducao_sme_19,${TAB}it_in_uso_valor_deducao_sme_20,${TAB}it_in_uso_valor_compensacao_01,${TAB}it_in_uso_valor_compensacao_02,${TAB}it_in_uso_valor_compensacao_03,${TAB}it_in_uso_valor_compensacao_04,${TAB}it_in_uso_valor_compensacao_05,${TAB}it_in_uso_valor_compensacao_06,${TAB}it_in_uso_valor_compensacao_07,${TAB} it_in_uso_valor_compensacao_08,${TAB}it_in_uso_valor_compensacao_09,${TAB}it_in_uso_valor_compensacao_10,${TAB}it_in_uso_valor_compensacao_11,${TAB}it_in_uso_valor_compensacao_12,${TAB}it_in_uso_valor_compensacao_13,${TAB} it_in_uso_valor_compensacao_14,${TAB}it_in_uso_valor_compensacao_15,${TAB}it_in_uso_valor_compensacao_16,${TAB}it_in_uso_valor_compensacao_17,${TAB}it_in_uso_valor_compensacao_18,${TAB}it_in_uso_valor_compensacao_19,${TAB}it_in_uso_valor_compensacao_20,${TAB}it_in_uso_valor_multa_juros_01,${TAB}it_in_uso_valor_multa_juros_02,${TAB}it_in_uso_valor_multa_juros_03,${TAB}it_in_uso_valor_multa_juros_04,${TAB}it_in_uso_valor_multa_juros_05,${TAB}it_in_uso_valor_multa_juros_06,${TAB}it_in_uso_valor_multa_juros_07,${TAB} it_in_uso_valor_multa_juros_08,${TAB}it_in_uso_valor_multa_juros_09,${TAB}it_in_uso_valor_multa_juros_10,${TAB}it_in_uso_valor_multa_juros_11,${TAB}it_in_uso_valor_multa_juros_12,${TAB}it_in_uso_valor_multa_juros_13,${TAB} it_in_uso_valor_multa_juros_14,${TAB}it_in_uso_valor_multa_juros_15,${TAB}it_in_uso_valor_multa_juros_16,${TAB}it_in_uso_valor_multa_juros_17,${TAB}it_in_uso_valor_multa_juros_18,${TAB}it_in_uso_valor_multa_juros_19,${TAB}it_in_uso_valor_multa_juros_20,${TAB}it_in_mes_referencia,${TAB}it_an_fim_base_formula_01,${TAB} it_an_fim_base_formula_02,${TAB} it_an_fim_base_formula_03,${TAB}it_an_fim_base_formula_04,${TAB}it_an_fim_base_formula_05,${TAB}it_an_fim_base_formula_06,${TAB}it_an_fim_base_formula_07,${TAB} it_an_fim_base_formula_08,${TAB}it_an_fim_base_formula_09,${TAB}it_an_fim_base_formula_10,${TAB}it_an_fim_base_formula_11,${TAB}it_an_fim_base_formula_12,${TAB}it_an_fim_base_formula_13,${TAB} it_an_fim_base_formula_14,${TAB}it_an_fim_base_formula_15,${TAB}it_an_fim_base_formula_16,${TAB}it_an_fim_base_formula_17,${TAB}it_an_fim_base_formula_18,${TAB}it_an_fim_base_formula_19,${TAB}it_an_fim_base_formula_20,${TAB}it_in_exige_empenho ) FROM stdin;" >> ${FILE}.SALARIOEDUCACAO.sql;

# CONSTANTE 42 - TABELA DOMICILIO BANCARIO 
echo -e "SET DATESTYLE TO ISO,DMY; \n COPY siafi.domiciliobancario (it_in_operacao,${TAB}it_co_usuario,${TAB}it_da_operacao,${TAB}it_co_unidade_gestora,${TAB}it_co_gestao,${TAB} it_co_banco_principal,${TAB}it_co_agencia_principal,${TAB}it_nu_conta_corrente_principal,${TAB}it_co_agen_unica_institucional,${TAB} it_co_banco_altern,${TAB} it_co_agencia_altern,${TAB} it_co_banco_01,${TAB} it_co_banco_02,${TAB} it_co_banco_03,${TAB} it_co_banco_04,${TAB}it_co_banco_05,${TAB}it_co_banco_06,${TAB}it_co_banco_07,${TAB}it_co_banco_08,${TAB}it_co_banco_09,${TAB}it_co_banco_10,${TAB}it_co_banco_11,${TAB}it_co_banco_12,${TAB}it_co_banco_13,${TAB} it_co_banco_14,${TAB}it_co_banco_15,${TAB}it_co_banco_16,${TAB}it_co_banco_17,${TAB}it_co_banco_18,${TAB}it_co_banco_19,${TAB}it_co_banco_20,${TAB}it_co_agencia_01,${TAB} it_co_agencia_02,${TAB}it_co_agencia_03,${TAB}it_co_agencia_04,${TAB}it_co_agencia_05,${TAB}it_co_agencia_06,${TAB}it_co_agencia_07,${TAB} it_co_agencia_08,${TAB}it_co_agencia_09,${TAB}it_co_agencia_10,${TAB}it_co_agencia_11,${TAB}it_co_agencia_12,${TAB}it_co_agencia_13,${TAB} it_co_agencia_14,${TAB}it_co_agencia_15,${TAB}it_co_agencia_16,${TAB}it_co_agencia_17,${TAB}it_co_agencia_18,${TAB}it_co_agencia_19,${TAB}it_co_agencia_20,${TAB}it_nu_conta_corrente_01,${TAB} it_nu_conta_corrente_02,${TAB}it_nu_conta_corrente_03,${TAB}it_nu_conta_corrente_04,${TAB}it_nu_conta_corrente_05,${TAB}it_nu_conta_corrente_06,${TAB}it_nu_conta_corrente_07,${TAB}it_nu_conta_corrente_08,${TAB}it_nu_conta_corrente_09,${TAB}it_nu_conta_corrente_10,${TAB}it_nu_conta_corrente_11,${TAB}it_nu_conta_corrente_12,${TAB}it_nu_conta_corrente_13,${TAB}it_nu_conta_corrente_14,${TAB}it_nu_conta_corrente_15,${TAB}it_nu_conta_corrente_16,${TAB}it_nu_conta_corrente_17,${TAB}it_nu_conta_corrente_18,${TAB}it_nu_conta_corrente_19,${TAB}it_nu_conta_corrente_20 ) FROM stdin;" >> ${FILE}.DOMICILIOBANCARIO.sql;




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
multiplicosData(){
	qtCampos=$1;
	texto=$2;
	tamanho=8;
	n=0;
	x=0;
	valores='';
	#echo 'texto:'$texto;
	while [ $n -lt $qtCampos ]; do
		valor=${texto:$x:$tamanho};
		if [ -z "$valor" ]; then
			valor=${NULO};
		fi
		valor=${valor:4:4}${valor:2:2}${valor:0:2};
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
		multiplicos 12 1 ${LINHA:56:12}; CREG_DARFSISTEMAAMPO08=$valores;
		
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
		REG_DARFSISTEMA
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
		
	"15") # CONSTANTE 15 - TABELA DE DOMICILIO BANCARIO CREDOR
		# 00003 00011  NUM     IT-CO-USUARIO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		
		# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
		CAMPO02=${LINHA:13:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		
		# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
		CAMPO03=${LINHA:25:4}${LINHA:23:2}${LINHA:21:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		
		# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
		CAMPO04=${LINHA:29:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		
		# 00034 00006  NUM     IT-CO-UG-REG_DARFSISTEMAPAGADOR
		CAMPO05=${LINHA:33:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
		
		# 00040 00001  ALFANUM IT-IN-OPERACAO
		CAMPO06=${LINHA:39:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
		
		# 00041 00014  ALFANUM IT-CO-CREDOR-DOMICILIO
		CAMPO07=${LINHA:40:14}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
		
		# 00055 00003  NUM     IT-CO-BANCO
		CAMPO08=${LINHA:54:3}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		
		# 00058 00004  NUM     IT-CO-AGENCIA
		CAMPO09=${LINHA:57:4}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
		
		# 00062 00010  ALFANUM IT-CO-CONTA
		CAMPO10=${LINHA:61:10}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		
		# 00072 00001  NUM     IT-IN-CONTA-CONJUNTA
		CAMPO11=${LINHA:71:1}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
		
		# 00073 00001  NUM     IT-IN-TIPO-CONTA
		CAMPO12=${LINHA:72:1}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
		
		# 00074 00006  NUM     IT-CO-UG-SUPRIDORA
		CAMPO13=${LINHA:73:6}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
		
		# 00080 00005  NUM     IT-CO-GESTAO-SUPRIDORA
		CAMPO14=${LINHA:79:5}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
 
		REG_DOMICILIOBANCARIOCREDOR=`expr $REG_DOMICILIOBANCARIOCREDOR + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14 >> ${FILE}.DOMICILIOBANCARIOCREDOR.sql;;

	"16") # CONSTANTE 16 - TABELA DE TAXA DE CONVERSAO MENSAL
		# 00003  00005  NUM     IT-CO-GESTAO
		CAMPO01=${LINHA:2:5}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		
		# 00008  00005  NUM     GR-ORGAO
		CAMPO02=${LINHA:7:5}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		
		# 00013  00001  ALFANUM IT-IN-STATUS
		CAMPO03=${LINHA:12:1}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		
		# 00014  00001  ALFANUM IT-IN-OPERACAO
		CAMPO04=${LINHA:14:1}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		
		# 00015  00011  ALFANUM IT-CO-USUARIO
		CAMPO05=${LINHA:14:11}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
		
		# 00026  00008  NUM     IT-DA-OPERACAO (FORMATO DDMMAAAA)
		CAMPO06=${LINHA:29:4}${LINHA:27:2}${LINHA:25:2}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi 
		
		# 00034  00156  NUM     IT-OP-TAXA-CONVERSAO (13 OCORRENCIAS FORMATO N5,7)
		multiplicosFloat 13 5 7 ${LINHA:34:156}; CAMPO07=$valores;
		
		# 00190  00104  NUM     IT-DA-ATUAL (FORMATO DDMMAAAA) (13 OCORRENCIAS FORMATO N8)
		multiplicosFloat 13 8 ${LINHA:189:104}; CAMPO08=$valores;
		
		# 00294  00156  NUM     IT-OP-TAXA-MEDIA (14 OCORRENCIAS FORMATO N5,7)
		multiplicosFloat 14 5 7 ${LINHA:293:156}; CAMPO09=$valores;
		
		# 00450  00003  NUM     IT-CO-MOEDA
		CAMPO10=${LINHA:449:3}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		
		REG_TAXACONVERSAOMENSAL=`expr $REG_TAXACONVERSAOMENSAL + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10 >> ${FILE}.TAXACONVERSAOMENSAL.sql;;
	 
	"18") # CONSTANTE 18 - TABELA DE CONVENIO PAGAMENTO FATURA
		# 00003  00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
		# 00004  00011  ALFANUM IT-CO-USUARIO
		CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
		# 00015  00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
		CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
		# 00023  00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
		CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
		# 00027  00006  NUM     IT-CO-UG-OPERADOR
		CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
		# 00033  00008  NUM     IT-NU-CONVENIO
		CAMPO06=${LINHA:65:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
		# 00041  00014  ALFANUM IT-NU-CNPJ
		CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
		# 00055  00060  ALFANUM IT-NO-EMPRESA
		CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		# 00115  00002  NUM     IT-NU-SEGMENTO
		CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
		# 00117  00001  NUM     IT-IN-PAGAMENTO
		CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
		# 00118  00008  NUM     IT-CO-EMPRESA
		CAMPO11=${LINHA:153:8}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
		# 00126  00001  ALFANUM IT-TP-LISTA
		CAMPO12=${LINHA:153:8}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
		# 00127  00003  NUM     IT-CO-BANCO
		CAMPO13=${LINHA:153:8}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
		# 00130  00001  NUM     IT-IN-CRITICA-VENCIMENTO	
		CAMPO14=${LINHA:153:8}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
		
		REG_CONVENIOPAGAMENTOFATURA=`expr $REG_CONVENIOPAGAMENTOFATURA + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14 >> ${FILE}.CONVENIOPAGAMENTOFATURA.sql;;
	
	"19") # CONSTANTE 19 - TABELA DE CONTROLE DE CREDOR
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
			
		REG_CONTROLEDECREDOR=`expr $REG_CONTROLEDECREDOR + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.CONTROLEDECREDOR.sql;;
		
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
		CAMPO21=${LINHA:164:3}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
		# 00168 00001  NUM     IT-CO-GRUPO-DESPESA
		CAMPO22=${LINHA:167:1}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
		# 00169 00017  ALFANUM IT-NU-PROCESSO
		CAMPO23=${LINHA:168:17}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
		# 00186 00017  ALFANUM IT-NU-REFERENCIA
		CAMPO24=${LINHA:185:17}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
		# 00203 00234  ALFANUM IT-TX-OBSERVACAO
		CAMPO25=${LINHA:202:234}; if [ -z "$CAMPO25" ]; then CAMPO25=${NULO}; fi
		
		REG_DARFSISTEMA=`expr $REG_DARFSISTEMA + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25 >> ${FILE}.DARFSISTEMA.sql;;
		
		"21") # CONSTANTE 21 - TABELA DE GPS SISTEMA
			# 00003  00001  ALFANUM IT-IN-OPERACAO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00004  00011  ALFANUM IT-CO-USUARIO
			CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00015  00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00023  00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:22:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00027  00006  NUM     IT-CO-UNIDADE-GESTORA-SALDO
			CAMPO05=${LINHA:26:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00033  00005  NUM     IT-CO-GESTAO-SALDO
			CAMPO06=${LINHA:32:5}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00038  00009  ALFANUM GR-CODIGO-CONTA
			CAMPO07=${LINHA:37:9}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00047  00043  ALFANUM IT-CO-CONTA-CORRENTE-CONTABIL
			CAMPO08=${LINHA:46:43}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00090  00008  NUM     IT-DA-EXECUCAO (FORMATO DDMMAAAA)
			CAMPO09=${LINHA:93:4}${LINHA:91:2}${LINHA:89:2}; if [ -z $CAMPO09 ]; then CAMPO09=${NULO}; fi
			# 00098  00001  ALFANUM IT-IN-TIPO-EXECUCAO
			CAMPO10=${LINHA:97:1}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			# 00099  00003  NUM     IT-QT-DIAS-UTEIS
			CAMPO11=${LINHA:98:3}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			# 00102  00006  NUM     IT-CO-UNIDADE-GESTORA-EMIT
			CAMPO12=${LINHA:101:6}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
			# 00108  00005  NUM     IT-CO-GESTAO-EMIT
			CAMPO13=${LINHA:107:5}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
			# 00113  00014  ALFANUM IT-CO-RECOLHEDOR
			CAMPO14=${LINHA:112:14}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
			# 00127  00004  NUM     IT-CO-PAGAMENTO
			CAMPO15=${LINHA:126:4}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
			# 00131  00001  NUM     IT-IN-TIPO-RECURSO
			CAMPO16=${LINHA:130:1}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
			# 00132  00006  NUM     IT-CO-UG-DOC-REFERENCIA
			CAMPO17=${LINHA:131:6}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
			# 00138  00005  NUM     IT-CO-GESTAO-DOC-REFERENCIA
			CAMPO18=${LINHA:137:5}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
			# 00143  00012  ALFANUM GR-AN-NU-DOCUMENTO-REFERENCIA
			CAMPO19=${LINHA:142:12}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
			# 00155  00010  ALFANUM GR-FONTE-RECURSO
			CAMPO20=${LINHA:154:10}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
			# 00165  00003  NUM     IT-CO-VINC-PAGAMENTO
			CAMPO21=${LINHA:164:3}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
			# 00168  00001  NUM     IT-CO-GRUPO-DESPESA
			CAMPO22=${LINHA:167:1}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
			# 00169  00020  ALFANUM IT-NU-PROCESSO
			CAMPO23=${LINHA:168:20}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
			# 00189  00234  ALFANUM IT-TX-OBSERVACAO
			CAMPO24=${LINHA:188:234}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
			
			REG_GPSSISTEMA=`expr $REG_GPSSISTEMA + 1`;
			echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24 >> ${FILE}.GPSSISTEMA.sql;;
		
		
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
			
			REG_NATUREZARESPONSABILIDADE=`expr $REG_NATUREZARESPONSABILIDADE + 1`;
			echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08 >> ${FILE}.NATUREZARESPONSABILIDADE.sql;;
		
		# CONSTANTE 26 - TABELA DE SETOR ATIVIDADE ECONOMICA
		"26")
			# 00003 00001  ALFANUM IT-IN-OPERACAO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00004 00011  ALFANUM IT-CO-USUARIO
			CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00015 00008  NUM     IT-DA-OPERACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00023 00002  NUM     IT-CO-SETOR-ATIV-ECONOMICA-VELHO
			CAMPO04=${LINHA:22:2}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00025 00045  ALFANUM IT-NO-SETOR-ATIV-ECONOMICA
			CAMPO05=${LINHA:24:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00070 00002  ALFANUM IT-CO-SETOR-ATIV-ECONOMICA
			CAMPO06=${LINHA:69:2}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

			REG_SETORATIVIDADEECONOMICA=`expr $REG_SETORATIVIDADEECONOMICA + 1`;
        	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.SETORATIVIDADEECONOMICA.sql;;
		
		# CONSTANTE 28 - TABELA DE CODIGO BB/GR
		"28")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA: 17: 4}${LINHA:  15: 2}${LINHA:  13: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:21:4}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00026 00001  ALFANUM IT-IN-OPERACAO
			CAMPO04=${LINHA:25:1}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00027 00005  NUM     IT-CO-BB-GR
			CAMPO05=${LINHA:26:5}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00032 00006  NUM     IT-CO-UNIDADE-GESTORA
			CAMPO06=${LINHA:31:6}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00038 00005  NUM     IT-CO-GESTAO
			CAMPO07=${LINHA:37:5}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00043 00001  NUM     IT-IN-COD-BB-GR
			CAMPO08=${LINHA:142:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
		
			REG_CODIGOBBGR=`expr $REG_CODIGOBBGR + 1`;
			echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08 >> ${FILE}.CODIGOBBGR.sql;;

		# CONSTANTE 29 - TABELA DE FPAS
		"29")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  17: 4}${LINHA:  15: 2}${LINHA:  13: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:21:4}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00026 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO04=${LINHA:25:6}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00032 00001  ALFANUM IT-IN-OPERACAO
			CAMPO05=${LINHA:31:1}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00033 00003  NUM     IT-CO-FPAS
			CAMPO06=${LINHA:32:3}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00036 00045  ALFANUM IT-NO-FPAS
			CAMPO07=${LINHA:35:45}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00081 00200  ALFANUM IT-CO-ENTIDADE (50 OCORRENCIAS FORMATO N4)
			multiplicos 50 4 ${LINHA:80:200}; CAMPO08=$valores;
			
			# 00281 00140  ALFANUM IT-TX-MOTIVO
			CAMPO09=${LINHA:280:140}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			
			REG_FPAS=`expr $REG_FPAS + 1`;	
			echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09 >> ${FILE}.FPAS.sql;;
		 
		# CONSTANTE 31 - TABELA INDICE CORRECAO PROJUD
		"31")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:13:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA: 25: 4}${LINHA: 23: 2}${LINHA:  21: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:29:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:33:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:39:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00007  NUM     IT-IN-CORRECAO (FORMATO N2,5)
			CAMPO07=${LINHA:40:7}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00048 00002  NUM     IT-ME-CORRECAO
			CAMPO08=${LINHA:47:2}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00050 00004  NUM     IT-AN-CORRECAO
			CAMPO09=${LINHA:49:4}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 00054 00050  ALFANUM IT-NO-INDICE-CORRECAO
			CAMPO10=${LINHA:53:50}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			
			REG_INDICECORRECAOPROJUD=`expr $REG_INDICECORRECAOPROJUD + 1`;
			echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10 >> ${FILE}.INDICECORRECAOPROJUD.sql;;
		
		# CONSTANTE 33 - TABELA DE LIMITE EMPENHO
		"33")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:11:3}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA: 25: 4}${LINHA: 23: 2}${LINHA:  21: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:29:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:33:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:39:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00002  NUM     IT-IN-MODALIDADE-LICITACAO
			CAMPO07=${LINHA:40:2}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00043 00002  ALFANUM IT-CO-INCISO
			CAMPO08=${LINHA:42:2}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			
			# 00045 00017  NUM     IT-VA-LIMITE (FORMATO N15,2)
			multiplicosFloat 15 2 ${LINHA:44:17}; CAMPO09=$valores;
			
			# 00062 00008  NUM     IT-DA-INICIO-VIGENCIA (FORMATO DDMMAAAA)
			CAMPO10=${LINHA: 65: 4}${LINHA: 63: 2}${LINHA:  61: 2}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			
			# 00070 00001  NUM     IT-IN-LIMITE-ATUAL
			CAMPO11=${LINHA:69:1}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			# 00071 00017  NUM     IT-VA-LIMITE-2 (FORMATO DDMMAAAA)
			CAMPO12=${LINHA: 74: 4}${LINHA: 72: 2}${LINHA:  70: 2}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
			
		
			REG_LIMITEEMPENHO=`expr $REG_LIMITEEMPENHO + 1`;
			echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12 >> ${FILE}.LIMITEEMPENHO.sql;;
		
		# CONSTANTE 34 - TABELA DE LRF
		"34")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  17: 4}${LINHA:  15: 2}${LINHA:  13: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:21:4}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00026 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO04=${LINHA:25:6}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00032 00001  ALFANUM IT-IN-OPERACAO
			CAMPO05=${LINHA:31:1}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00033 00003  NUM     IT-NU-LEI
			CAMPO06=${LINHA:32:3}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00036 00070  ALFANUM IT-NO-EXIGENCIA-LEGAL
			CAMPO07=${LINHA:35:70}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00106 02100  ALFANUM IT-TX-INSTRUCAO-NORMA (30 OCORRENCIAS FORMATO A70)
			multiplicos 30 70 ${LINHA:105:2100}; CAMPO08=$valores;

			# 02206 00001  NUM     IT-IN-LEI
			CAMPO09=${LINHA:2205:1}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 02207 00003  ALFANUM IT-NU-SUBGRUPO
			CAMPO10=${LINHA:2206:3}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			# 02210 00001  ALFANUM IT-IN-VISIVEL
			CAMPO11=${LINHA:2209:1}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi

		REG_LRF=`expr $REG_LRF + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11 >> ${FILE}.LRF.sql;;
		
		# CONSTANTE 36 - TABELA DE MOTIVO INADIMPLENCIA
		"36")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:13:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:  25: 4}${LINHA:  23: 2}${LINHA:  21: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:29:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:33:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:39:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00003  NUM     IT-CO-MOTIVO-INADIMPLENCIA
			CAMPO07=${LINHA:40:3}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00044 00045  ALFANUM IT-NO-MOTIVO-INADIMPLENCIA
			CAMPO08=${LINHA:43:45}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00089 00003  NUM     IT-CO-GRUPO-MOTIVO-INADIMPLENCIA
			CAMPO09=${LINHA:88:3}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 00092 01050  ALFANUM IT-TX-MOTIVO (15 OCORRENCIAS FORMATO A70)
			multiplicos 15 70 ${LINHA:91:1050}; CAMPO10=$valores;
		
			REG_MOTIVOINADIMPLENCIA=`expr $REG_MOTIVOINADIMPLENCIA + 1`;
			echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10 >> ${FILE}.MOTIVOINADIMPLENCIA.sql;;
		
		# CONSTANTE 37 - TABELA DE ORIGEM PRECATORIO
		"37")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:13:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:  25: 4}${LINHA:  23: 2}${LINHA:  21: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:29:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:33:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:39:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			# 00041 00005  NUM     IT-CO-ORGAO-CADASTRADOR
			CAMPO07=${LINHA:40:5}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			# 00046 00006  NUM     IT-CO-ORIGEM-PRECATORIO
			CAMPO08=${LINHA:45:6}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			# 00052 00005  NUM     IT-CO-ORGAO-PAGADOR
			CAMPO09=${LINHA:51:5}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			# 00057 00004  NUM     IT-CO-MUNICIPIO
			CAMPO10=${LINHA:56:4}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			# 00061 00002  ALFANUM IT-CO-UF
			CAMPO11=${LINHA:60:2}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			# 00063 00045  ALFANUM IT-NO-TITULO-ORIGEM
			CAMPO12=${LINHA:62:45}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
			# 00108 00019  ALFANUM IT-NO-MNEMONICO-TITULO-ORIGEM
			CAMPO13=${LINHA:107:19}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	
		REG_ORIGEMPRECATORIO=`expr $REG_INDICECORRECAOPROJUD + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13 >> ${FILE}.ORIGEMPRECATORIO.sql;;
		
		# CONSTANTE 38 - TABELA DE PROCESSO JUDICIAL
		"38")
				
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  17: 4}${LINHA:  15: 2}${LINHA:  13: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:21:4}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			
			# 00026 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO04=${LINHA:25:6}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			
			# 00032 00001  ALFANUM IT-IN-OPERACAO
			CAMPO05=${LINHA:31:1}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			
			# 00033 00006  NUM     IT-CO-UG-CADASTRADORA
			CAMPO06=${LINHA:32:6}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			
			# 00039 00005  NUM     IT-CO-GESTAO-CADASTRADORA
			CAMPO07=${LINHA:38:5}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00044 00030  ALFANUM IT-NU-PRECATORIO
			CAMPO08=${LINHA:43:30}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			
			# 00074 00002  NUM     IT-IN-SENTENCA
			CAMPO09=${LINHA:73:2}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			
			# 00076 00003  ALFANUM IT-IN-SITUACAO
			CAMPO10=${LINHA:75:3}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			
			# 00079 00006  NUM     IT-CO-UNIDADE-GESTORA-PAGADORA
			CAMPO11=${LINHA:78:6}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			
			# 00085 00005  NUM     IT-CO-GESTAO-PAGADORA
			CAMPO12=${LINHA:84:5}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
			
			# 00090 00100  ALFANUM IT-NO-REQUERENTE
			CAMPO13=${LINHA:89:100}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
			
			# 00190 00014  ALFANUM IT-CO-IDENTIFICACAO-REQUERENTE
			CAMPO14=${LINHA:189:14}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
			
			# 00204 00006  ALFANUM IT-CO-NATUREZA-DESPESA
			CAMPO15=${LINHA:203:6}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
			
			# 00210 00002  NUM     IT-IN-TIPO-DESPESA
			CAMPO16=${LINHA:209:2}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
			
			# 00212 00005  NUM     IT-CO-ORGAO-REU
			CAMPO17=${LINHA:211:5}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
			
			# 00217 00005  NUM     IT-CO-UNIDADE-ORCAMENTARIA
			CAMPO18=${LINHA:216:5}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
			
			# 00222 00234  ALFANUM IT-TX-DESCRICAO-PRECATOR
			CAMPO19=${LINHA:221:234}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
			
			# 00456 00006  NUM     IT-CO-VARA-ORIGEM
			CAMPO20=${LINHA:455:6}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
			
			# 00462 00008  NUM     IT-DA-AUTUACAO (FORMATO DDMMAAAA)
			CAMPO21=${LINHA:  465: 4}${LINHA:  463: 2}${LINHA:  461: 2}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
			
			# 00470 00002  NUM     IT-IN-TIPO-JUSTICA
			CAMPO22=${LINHA:469:2}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
			
			# 00472 00030  ALFANUM IT-CO-ACAO-ORIGINARIA
			CAMPO23=${LINHA:471:30}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
			
			# 00502 00002  NUM     IT-CO-VARA-COMARCA
			CAMPO24=${LINHA:501:2}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
			
			# 00504 00008  NUM     IT-DA-AJUIZAMENTO-ACAO (FORMATO DDMMAAAA)
			CAMPO25=${LINHA:503:8}; if [ -z "$CAMPO25" ]; then CAMPO25=${NULO}; fi
			
			# 00512 00008  NUM     IT-CO-ASSUNTO
			CAMPO26=${LINHA:511:8}; if [ -z "$CAMPO26" ]; then CAMPO26=${NULO}; fi
			
			# 00520 00004  ALFANUM IT-AN-PROPOSTA-ORCAMENTARIA
			CAMPO27=${LINHA:519:4}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
			
			# 00524 00001  NUM     IT-IN-NATUREZA-ALIMENTICIA
			CAMPO28=${LINHA:523:1}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi
			
			# 00525 00001  ALFANUM IT-IN-BLOQUEADO
			CAMPO29=${LINHA:524:1}; if [ -z "$CAMPO29" ]; then CAMPO29=${NULO}; fi
			
			# 00526 00017  NUM     IT-VA-PRECATORIO (FORMATO N15,2)
			CAMPO30=${LINHA:525:17}; if [ -z "$CAMPO30" ]; then CAMPO30=${NULO}; fi
			
			# 00543 00008  NUM     IT-DA-VALOR (FORMATO DDMMAAAA)
			CAMPO31=${LINHA:  546: 4}${LINHA:  544: 2}${LINHA:  542: 2}; if [ -z "$CAMPO31" ]; then CAMPO31=${NULO}; fi
				
			# 00551 00017  NUM     IT-VA-ATUAL (FORMATO N15,2)
			CAMPO32=${LINHA:550:17}; if [ -z "$CAMPO32" ]; then CAMPO32=${NULO}; fi
			
			# 00568 00008  NUM     IT-DA-VALOR-ATUAL (FORMATO DDMMAAAA)
			CAMPO33=${LINHA:  571: 4}${LINHA:  569: 2}${LINHA:  567: 2}; if [ -z "$CAMPO33" ]; then CAMPO33=${NULO}; fi
			
			# 00576 00005  NUM     IT-CO-ORGAO-CADASTRADOR
			CAMPO34=${LINHA:575:5}; if [ -z "$CAMPO34" ]; then CAMPO34=${NULO}; fi
			
			# 00581 00012  ALFANUM IT-IN-NUMERO-SEQUENCIAL
			CAMPO35=${LINHA:580:12}; if [ -z "$CAMPO35" ]; then CAMPO35=${NULO}; fi
			
			# 00593 00017  NUM     IT-VA-RETENCAO (FORMATO N15,2)
			CAMPO36=${LINHA:592:17}; if [ -z "$CAMPO36" ]; then CAMPO36=${NULO}; fi
			
			# 00610 00001  NUM     IT-IN-SENTENCA-PEQUENO-VALOR
			CAMPO37=${LINHA:609:1}; if [ -z "$CAMPO37" ]; then CAMPO37=${NULO}; fi
			
			# 00611 00006  NUM     IT-QT-BENEFICIARIO
			CAMPO38=${LINHA:610:6}; if [ -z "$CAMPO38" ]; then CAMPO38=${NULO}; fi
			
			# 00617 00030  NUM     IT-CO-BANCO (10 OCORRENCIAS)
			multiplicos 10 3 ${LINHA:616:30}; CAMPO39=$valores;
			
			# 00647 00040  NUM     IT-CO-AGENCIA (10 OCORRENCIAS)
			multiplicos 10 4 ${LINHA:646:40}; CAMPO40=$valores;
			
			# 00687 00100  ALFANUM IT-NU-CONTA-CORRENTE (10 OCORRENCIAS)
			multiplicos 10 10 ${LINHA:686:100}; CAMPO41=$valores;
			
			# 00787 00170  NUM     IT-VA-PARCELA (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:786:170}; CAMPO42=$valores;
			
			# 00957 00170  NUM     IT-VA-PARCELA-RETENCAO(10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:956:170}; CAMPO43=$valores;
			
			# 01127 00170  NUM     IT-VA-PARCELA-LIQUIDA (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:1126:170}; CAMPO44=$valores;
			
			# 01297 00080  NUM     IT-DA-PREVISAO-VENCIMENTO (10 OCORRENCIAS FORMATO DDMMAAAA)
			multiplicosData 10 ${LINHA:1296:80}; CAMPO45=$valores;
			
			# 01377 00170  NUM     IT-VA-PARCELA-A-PAGAR (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:1376:170}; CAMPO46=$valores;
			
			# 01547 00170  NUM     IT-VA-PARCELA-PAGO (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:1546:170}; CAMPO47=$valores;
			
			# 01717 00170  NUM     IT-VA-COMPLEMENTO-PAGO(10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:1716:170}; CAMPO48=$valores;
				
			# 01887 00020  NUM     IT-NU-PARCELA-COMPLEMENTO(10 OCORRENCIAS FORMATO N02
			multiplicos 10 2 ${LINHA:1886:20}; CAMPO49=$valores;	
			
			# 01907 00020  NUM     IT-SQ-COMPLEMENTO (10 OCORRENCIAS FORMATO N02)
			multiplicos 10 2 ${LINHA:1906:20}; CAMPO50=$valores;
			
			# 01927 00170  NUM     IT-VA-COMPLEMENTO (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:1926:170}; CAMPO51=$valores;
			
			# 02097 00170  NUM     IT-VA-COMPLEMENTO-RETENCAO (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:2096:170}; CAMPO52=$valores;	
			
			# 02267 00080  NUM     IT-DA-COMPLEMENTO-VENCIMENTO (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:2266:80}; CAMPO53=$valores;	
			
			# 02347 00170  NUM     IT-VA-COMPLEMENTO-A-PAGAR (10 OCORRENCIAS FORMATO 15,2)
			multiplicos 10 17 ${LINHA:2346:170}; CAMPO54=$valores;	
			
			# 02517 00020  NUM     IT-NU-PARCELA-DEVOLUCAO (10 OCORRENCIAS FORMATO N02)
			multiplicos 10 2 ${LINHA:2516:20}; CAMPO55=$valores;	
			
			# 02537 00020  NUM     IT-SQ-DEVOLUCAO (10 OCORRENCIAS FORMATO N02)
			multiplicos 10 2 ${LINHA:2536:20}; CAMPO56=$valores;	
			
			# 02557 00170  NUM     IT-VA-DEVOLUCAO (10 OCORRENCIAS FORMATO N15,2)
			multiplicos 10 17 ${LINHA:2556:170}; CAMPO57=$valores;	
			
			# 02727 00230  ALFANUM IT-NU-DOCUMENTO-DEVOLUCAO (10 OCORRENCIAS FORMATO A23)
			multiplicos 10 23 ${LINHA:2726:230}; CAMPO58=$valores;
				
			# 02957 00010  NUM     IT-IN-OPERACAO-DEVOLUCAO(10 OCORRENCIAS FORMATO N01)
			multiplicos 10 1 ${LINHA:2956:10}; CAMPO59=$valores;

		REG_PROCESSOJUDICIAL=`expr $REG_PROCESSOJUDICIAL + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27${TAB}$CAMPO28${TAB}$CAMPO29${TAB}$CAMPO30${TAB}$CAMPO31${TAB}$CAMPO32${TAB}$CAMPO33${TAB}$CAMPO34${TAB}$CAMPO35${TAB}$CAMPO36${TAB}$CAMPO37${TAB}$CAMPO38${TAB}$CAMPO39${TAB}$CAMPO40${TAB}$CAMPO41${TAB}$CAMPO42${TAB}$CAMPO43${TAB}$CAMPO44${TAB}$CAMPO45${TAB}$CAMPO46${TAB}$CAMPO47${TAB}$CAMPO48${TAB}$CAMPO49${TAB}$CAMPO50${TAB}$CAMPO51${TAB}$CAMPO52${TAB}$CAMPO53${TAB}$CAMPO54${TAB}$CAMPO55${TAB}$CAMPO56${TAB}$CAMPO57${TAB}$CAMPO58${TAB}$CAMPO59 >> ${FILE}.PROCESSOJUDICIAL.sql;

		# CONSTANTE 39 - TABELA DE PREVISAO
		"39")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			
			# 00014 00008  ALFANUM IT-CO-TERMINAL-USUARIO
			CAMPO02=${LINHA:21:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi			
			
			# 00022 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO03=${LINHA:  17: 4}${LINHA:  15: 2}${LINHA: 13: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi

			# 00030 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO04=${LINHA:29:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			
			# 00034 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO05=${LINHA:33:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			
			# 00040 00001  ALFANUM IT-IN-OPERACAO
			CAMPO06=${LINHA:39:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			
			# 00041 00003  NUM     IT-CO-PREVISAO
			CAMPO07=${LINHA:40:3}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00044 00045  ALFANUM IT-NO-PREVISAO
			CAMPO08=${LINHA:43:45}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			
			# 00089 00019  ALFANUM IT-NO-MNEMONICO-PREVISAO
			CAMPO09=${LINHA:88:19}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			
			# 00108 00006  NUM     IT-CO-EVENTO
			CAMPO10=${LINHA:107:6}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
		REG_PREVISAO=`expr $REG_PREVISAO + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10 >> ${FILE}.PREVISAO.sql;;

		# CONSTANTE 40 - TABELA DE RECOLHIMENTO GFIP
		"40")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  17: 4}${LINHA:  15: 2}${LINHA:  13: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:21:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			
			# 00026 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO04=${LINHA:25:6}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			
			# 00032 00001  ALFANUM IT-IN-OPERACAO
			CAMPO05=${LINHA:31:1}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			
			# 00033 00004  NUM     IT-CO-GFIP
			CAMPO06=${LINHA:32:4}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			
			# 00037 00045  ALFANUM IT-NO-GFIP
			CAMPO07=${LINHA:36:45}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00082 00001  NUM     IT-IN-EXIGE-COMPETENCIA
			CAMPO08=${LINHA:81:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			
			# 00083 00002  NUM     IT-ME-INICIO
			CAMPO09=${LINHA:82:2}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			
			# 00085 00004  NUM     IT-AN-INICIO
			CAMPO10=${LINHA:84:4}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			
			# 00089 00002  NUM     IT-ME-TERMINO
			CAMPO11=${LINHA:88:2}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			
			# 00091 00004  NUM     IT-AN-TERMINO
			CAMPO12=${LINHA:90:4}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
			
			# 00095 00140  ALFANUM IT-TX-MOTIVO
			CAMPO13=${LINHA:94:140}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
			
			# 00235 00002  NUM     IT-CO-CORRELACAO-BARRA
			CAMPO14=${LINHA:234:2}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
			
			# 00237 00001  NUM     IT-IN-EXIGE-EMPENHO
			CAMPO15=${LINHA:236:1}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
	
		REG_RECOLHIMENTOGFIP=`expr $REG_RECOLHIMENTOGFIP + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25 >> ${FILE}.RECOLHIMENTOGFIP.sql;;

		# CONSTANTE 41 - TABELA DE SALARIO EDUCACAO
		"41")
			# 00003 00011  ALFANUM IT-CO-USUARIO
			CAMPO01=${LINHA:2:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			
			# 00014 00008  NUM     IT-DA-TRANSACAO (FORMATO DDMMAAAA)
			CAMPO02=${LINHA:  17: 4}${LINHA:  15: 2}${LINHA:  13: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			
			# 00022 00004  NUM     IT-HO-TRANSACAO (FORMATO HHMM)
			CAMPO03=${LINHA:21:4}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
			
			# 00026 00006  NUM     IT-CO-UG-OPERADOR
			CAMPO04=${LINHA:25:6}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			
			# 00032 00001  ALFANUM IT-IN-OPERACAO
			CAMPO05=${LINHA:31:1}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			
			# 00033 00004  NUM     IT-CO-SALARIO-EDUCACAO
			CAMPO06=${LINHA:32:4}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			
			# 00037 00045  ALFANUM IT-NO-SALARIO-EDUCACAO
			CAMPO07=${LINHA:36:44}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00082 00001  NUM     IT-IN-COMPETENCIA
			CAMPO08=${LINHA:81:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			
			# 00083 00004  NUM     IT-AN-INICIO
			CAMPO09=${LINHA:82:4}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			
			# 00087 00004  NUM     IT-AN-REFERENCIA
			CAMPO10=${LINHA:86:4}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			
			# 00091 00001  NUM     IT-IN-PARCELA
			CAMPO11=${LINHA:90:1}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			
			# 00092 00001  NUM     IT-IN-PROCESSO-EXEC-FISCAL
			CAMPO12=${LINHA:91:1}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
			
			# 00093 00001  NUM     IT-IN-CALCULO-DV
			CAMPO13=${LINHA:92:1}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
			
			# 00094 00001  NUM     IT-IN-BASE-CONTRIBUICAO
			CAMPO14=${LINHA:93:1}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
			
			# 00095 00001  NUM     IT-IN-SALARIO-EDUCACAO
			CAMPO15=${LINHA:94:1}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
			
			# 00096 00004  NUM     IT-PE-SALARIO-EDUCACAO (FORMATO N2,2)
			CAMPO16=${LINHA:95:4}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
			
			# 00100 00001  NUM     IT-IN-DEDUCAO-SME
			CAMPO17=${LINHA:99:1}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
			
			# 00101 00001  NUM     IT-IN-COMPENSACAO
			CAMPO18=${LINHA:100:1}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
			
			# 00102 00001  NUM     IT-IN-VALOR-ATUALIZADO
			CAMPO19=${LINHA:101:1}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
			
			# 00103 00001  NUM     IT-IN-MULTA-JUROS
			CAMPO20=${LINHA:102:1}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
			
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
			multiplicos 20 1 ${LINHA:383:20}; CAMPO25=$valores;
			
			# 00404 00020  NUM     IT-IN-USO-VALOR-MULTA-JUROS (20 OCORRENCIAS FORMATO A1)
			multiplicos 20 1 ${LINHA:403:20}; CAMPO26=$valores;
			
			# 00424 00001  NUM     IT-IN-MES-REFERENCIA
			CAMPO27=${LINHA:423:1}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
			
			# 00425 00080  NUM     IT-AN-FIM-BASE-FORMULA (20 OCORRENCIAS FORMATO A4)
			multiplicos 20 4 ${LINHA:424:80}; CAMPO28=$valores;
			
			# 00505 00001  NUM     IT-IN-EXIGE-EMPENHO
			CAMPO28=${LINHA:504:1}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi

		REG_SALARIOEDUCACAO=`expr $REG_SALARIOEDUCACAO + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27${TAB}$CAMPO28 >> ${FILE}.SALARIOEDUCACAO.sql;;
		
		# CONSTANTE 42 - TABELA DOMICILIO BANCARIO
		"42")
			# 00003 00001  ALFANUM IT-IN-OPERACAO
			CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
			
			# 00004 00011  NUM     IT-CO-USUARIO
			CAMPO02=${LINHA:3:11}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
			
			# 00015 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
			CAMPO03=${LINHA:18:4}${LINHA:16:2}${LINHA:14:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
					
			# 00023 00006  NUM     IT-CO-UNIDADE-GESTORA
			CAMPO04=${LINHA:22:6}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
			
			# 00029 00005  NUM     IT-CO-GESTAO
			CAMPO05=${LINHA:28:5}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
			
			# 00034 00003  NUM     IT-CO-BANCO-PRINCIPAL
			CAMPO06=${LINHA:33:3}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
			
			# 00037 00004  NUM     IT-CO-AGENCIA-PRINCIPAL
			CAMPO07=${LINHA:36:4}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
			
			# 00041 00010  ALFANUM IT-NU-CONTA-CORRENTE-PRINCIPAL
			CAMPO08=${LINHA:40:10}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
			
			# 00051 00004  NUM     IT-CO-AGEN-UNICA-INSTITUCIONAL
			CAMPO09=${LINHA:50:4}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
			
			# 00055 00003  NUM     IT-CO-BANCO-ALTERN
			CAMPO10=${LINHA:54:3}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
			
			# 00058 00004  NUM     IT-CO-AGENCIA-ALTERN
			CAMPO11=${LINHA:57:4}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
			
			# 00062 00060  NUM     IT-CO-BANCO - 20 VEZES DE 3 BYTES
			multiplicos 20 3 ${LINHA:61:60}; CAMPO12=$valores;
			
			# 00122 00080  NUM     IT-CO-AGENCIA - 20 VEZES DE 4 BYTES
			multiplicos 20 4 ${LINHA:121:80}; CAMPO13=$valores;
			
			# 00202 00200  ALFANUM IT-NU-CONTA-CORRENTE - 20 DE 10 BYTES
			multiplicos 20 10 ${LINHA:201:200}; CAMPO14=$valores;
				
		REG_DOMICILIOBANCARIO=`expr $REG_DOMICILIOBANCARIO + 1`;	
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14 >> ${FILE}.DOMICILIOBANCARIO.sql;;

esac

done
