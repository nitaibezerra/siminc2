#!/bin/bash

#PGUSER='postgres'
#PGPASSWORD='simec'
#PGHOST='localhost'
#PGDB='simecfinanceiro'
#FILELOG=$LOGPG"log_"`date +%Y%m%d`;

TAB='\t';
NULO='\\N';
LIDOS='lidos/';
LENDO='Lendo/';
SQLCOPY='sqlCopy/';
REG=0;
REGT=0;

REGUG=0;
REGGESTAO=0;
REGORGAO=0;
REGUO=0;
REGBANCOORIGINAL=0;
REGAGENCIABB=0;
REGCATEGORIAGASTO=0;
REGPLANOCONTA=0;
REGEVENTO=0;
REGPROGRAMATRABALHO=0;
REGCREDOR=0;
REGVINCULAPAGAMENTO=0;
REGFONTE=0;
REGFUNCAO=0;
REGSUBFUNCAO=0;
REGRECEITAFEDERAL=0;
REGPLANOINTERNO=0;
REGPAIS=0;
REGMUNICIPIO=0;
REGUF=0;
REGPROGRAMA=0;
REGINSTITUICAOEXTERNA=0;
REGPTRES=0;
REGACAO=0;
REGSUBACAO=0;
REGPTRESTB=0;
REGINSCRICAOGENERICA=0;
REGDESTINACAORECEITA=0;
REGPROJATIVSUBTITULO=0;

FILE=$1;

echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.UG.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.GESTAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.ORGAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.UO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.BANCOORIGINAL.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.AGENCIABB.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.CATEGORIAGASTO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PLANOCONTA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.EVENTO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PROGRAMATRABALHO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.CREDOR.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.DESTINACAORECEITA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.VINCULAPAGAMENTO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.FONTERECURSO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.FUNCAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.SUBFUNCAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.RECEITAFEDERAL.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PLANOINTERNO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PAIS.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.MUNICIPIO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.UF.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PROGRAMA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.INSTITUICAOEXTERNA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.ACAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.SUBACAO.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PTRESTB.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.INSCRICAOGENERICA.sql;
echo -e "SET client_encoding TO 'LATIN5'; " > ${FILE}.PROJATIVSUBTITULO.sql;

echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.ug (it_in_operacao, it_da_operacao, it_co_unidade_gestora, it_no_unidade_gestora, it_no_mnemonico_unidade_gestora, it_in_situacao_unidade_gestora, it_nu_cgc, it_co_pais, it_co_uf, gr_orgao, it_co_unidade_gestora_seto_orca, it_co_unidade_gestora_seto_audi, it_co_unidade_gestora_seto_cont, it_co_unidade_gestora_polo, it_ed_endereco, it_nu_cpf_ordenador_ass, it_no_ordenador_ass, it_co_moeda, it_in_esfera_administrativa, it_co_cep_novo, it_nu_fone_ug, it_in_funcao_ug, it_in_estado_municipio_ug, it_co_municipio, it_co_ug_controle_interno, it_in_uso_cpr) FROM stdin;" >> ${FILE}.UG.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.gestao (it_in_operacao, it_da_operacao, it_in_ativo, it_co_gestao, it_no_gestao, it_no_mnemonico_gestao, it_in_pertence_gestao_10000, it_in_gere_fundo, it_in_inscreve_resto_pagar, it_in_recursos_diferidos) FROM stdin;" >> ${FILE}.GESTAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.orgao (it_in_operacao, it_da_operacao, gr_orgao, it_no_orgao, it_in_plano_interno, it_co_unid_gest_seto_fina_pais, it_co_orgao_vinculacao, it_in_tipo_administracao, it_no_mnemonico_orgao, it_in_recebe_cota_stn, it_in_utilizacao, it_co_gestao, it_in_orcamento_sof_seplan, it_in_poder, it_co_unidade_gestora_coorden, it_in_programacao_orcamentaria, it_nu_cgc_orgao, it_in_subacao, it_co_orgao_dou, it_co_unid_gest_seto_cont, it_in_acumula_balanco_uniao, it_nu_conta_institucional, it_co_fase_programacao, it_in_uso_cpr, it_co_unidade_gestora_incorporacao) FROM stdin;" >> ${FILE}.ORGAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.uo (it_in_operacao, it_da_operacao, gr_unidade_orcamentaria, gr_orgao, it_no_unidade_orcamentaria, it_co_gestao, it_co_unidade_gestora_resp) FROM stdin;" >> ${FILE}.UO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.bancooriginal (it_in_operacao, it_da_operacao, it_co_banco, it_no_domicilio_bancario, it_no_mnemonico_banco, it_in_instituicao_finaceira, it_in_banco_ctu, it_in_altera_agencia, it_nu_subitem) FROM stdin;" >> ${FILE}.BANCOORIGINAL.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.agenciabb (it_in_operacao, it_da_operacao, it_co_banco, it_co_agencia, it_no_agencia, it_no_praca_pagamento, it_no_mnemonico_agencia, it_in_agencia_exterior, it_tx_endereco, it_co_cep) FROM stdin;" >> ${FILE}.AGENCIABB.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.categoriagasto(it_in_operacao, it_da_transacao, gr_categoria_gasto, it_no_categoria_gasto, it_no_mnemonico_categoria_gasto) FROM stdin;" >> ${FILE}.CATEGORIAGASTO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.planoconta (it_in_operacao, it_da_transacao, gr_codigo_conta, it_no_conta, it_tx_funcao_conta, it_tx_circunstancia_debito_1, it_tx_circunstancia_debito_2, it_tx_circunstancia_debito_3, it_tx_circunstancia_debito_4, it_tx_circunstancia_debito_5, it_tx_circunstancia_debito_6, it_tx_circunstancia_debito_7, it_tx_circunstancia_credito_1, it_tx_circunstancia_credito_2, it_tx_circunstancia_credito_3, it_tx_circunstancia_credito_4, it_tx_circunstancia_credito_5, it_tx_circunstancia_credito_6, it_tx_circunstancia_credito_7, it_tx_significado_saldo, it_tx_observacao_conta, it_in_conta_corrente_contabil, it_in_encerramento, it_in_inversao_saldo, it_in_escrituracao, it_in_saldo_contabil, it_in_lancamento_orgao, it_in_integracao, it_in_sistema_contabil, it_in_utilizacao_safem, it_in_lancamento_nssaldo, it_tx_motivo, it_tx_funca_01, it_tx_funca_02, it_tx_funca_03, it_tx_funca_04, it_tx_funca_05, it_tx_funca_06, it_tx_funca_07, it_tx_funca_08, it_tx_funca_09, it_tx_funca_10, it_tx_funca_11, it_tx_funca_12, it_tx_funca_13, it_tx_funca_14, it_tx_funca_15, it_tx_funca_16, it_tx_funca_17, it_tx_funca_18, it_tx_funca_19, it_tx_funca_20, it_tx_funca_21, it_tx_funca_22, it_tx_funca_23, it_tx_funca_24, it_tx_funca_25, it_tx_funca_26, it_tx_funca_27, it_tx_funca_28, it_tx_funca_29, it_tx_funca_30) FROM stdin;" >> ${FILE}.PLANOCONTA.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.evento (it_in_operacao, it_da_transacao, gr_codigo_evento, it_no_evento, it_tx_descricao_evento, it_tx_observacao_evento, it_in_abertura_encerramento, it_in_estorno_evento, it_in_evento_provisorio, it_in_verifica_saldo, it_tx_motivo, it_in_ug_emitente, it_in_gestao_emitente, it_in_favorecido, it_in_ug_favorecido, it_in_gestao_favorecido, it_in_cambio, it_in_restricao_uo, it_in_exercicio_ne, it_in_verif_equilibrio_sist_cont, it_in_detalhamento_plano_interno, it_in_inscricao1_evento, it_in_inscricao2_evento, it_in_restricao1_fonte, it_in_restricao2_fonte, it_in_detalhamneto1_fonte, it_in_detalhamneto2_fonte) FROM stdin;" >> ${FILE}.EVENTO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.programatrabalho (it_in_operacao, it_da_transacao, gr_programa_trabalho, it_co_municipio_ibge, it_co_uf, it_co_regiao) FROM stdin;" >> ${FILE}.PROGRAMATRABALHO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.credor (it_in_operacao, it_da_transacao, it_co_credor, it_co_tipo_crdor, it_no_mnemonico_credor, it_no_credor, it_ed_credor, it_co_municipio_credor, it_co_cep_credor, it_co_uf_credor, it_nu_telefone_credor, it_tx_motivo_credor, it_in_situacao_credor_srf, it_da_situacao_credor_srf) FROM stdin;" >> ${FILE}.CREDOR.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.destinacaoreceita (it_in_operacao, it_da_operacao, it_co_destinacao_receita, it_no_destinacao_receita, it_no_mnemonico_destinacao_rec, it_co_tipo_beneficiario, it_co_tipo_dest_receita, it_co_fonte_recurso, it_pe_saldo_coeficiente, it_co_evento_arrecadacao_dest, it_co_evento_incetivo_dest, it_co_evento_retificacao_dest, it_co_evento_restituicao_dest, it_co_evento_arrecadacao_fonte, it_co_evento_incentivo_fonte, it_co_evento_retificacao_fonte, it_co_evento_restituicao_fonte) FROM stdin;" >> ${FILE}.DESTINACAORECEITA.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.vinculapagamento (it_in_operacao, it_da_transacao, it_co_vinculacao_pagamento, it_no_vinculacao_pagamento) FROM stdin;" >> ${FILE}.VINCULAPAGAMENTO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.fonterecurso (it_in_operacao, it_da_operacao, gr_fonte, it_no_fonte, it_in_tipo_fonte_sof, it_in_fonte_sof_programacao) FROM stdin;" >> ${FILE}.FONTERECURSO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.funcao(it_in_operacao, it_da_operacao, it_co_funcao, it_no_funcao, it_tx_descricao_pt_1, it_tx_descricao_pt_2) FROM stdin;" >> ${FILE}.FUNCAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.subfuncao(it_in_operacao, it_da_operacao, it_co_subfuncao, it_no_subfuncao, it_tx_descricao_pt_subfunc_1, it_tx_descricao_pt_subfunc_2) FROM stdin;" >> ${FILE}.SUBFUNCAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.receitafederal(it_in_operacao, it_da_operacao, it_co_receita, it_co_destinacao_receita, it_co_tipo_receita, it_no_receita, it_no_titulo_reduzido_receita, it_in_permite_darf, it_co_evento_arrecadacao, it_co_evento_incetivo, it_co_evento_retificacao, it_co_evento_restituicao, gr_fonte_recurso) FROM stdin;" >> ${FILE}.RECEITAFEDERAL.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.planointerno(it_in_operacao, it_da_operacao, gr_orgao, it_co_plano_interno, it_no_plano_interno, gr_unidade_orcamentaria, it_co_subacao, it_in_esfera_orcamentaria, it_in_desdobra_etapa, it_in_fisico_financeiro, it_tx_objetivo_plano_interno_1, it_tx_objetivo_plano_interno_2, it_tx_objetivo_plano_interno_3, it_tx_objetivo_plano_interno_4, it_co_acao, it_in_classificacao_economica, it_in_prioridade_ldo, it_in_tipo_correcao, it_in_acompanha_programacao) FROM stdin;" >> ${FILE}.PLANOINTERNO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.pais(it_in_operacao, it_da_transacao, it_co_pais, it_no_pais) FROM stdin;" >> ${FILE}.PAIS.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.municipio(it_in_operacao, it_da_transacao, it_co_municipio, it_no_municipio, it_co_uf) FROM stdin;" >> ${FILE}.MUNICIPIO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.uf(it_in_operacao, it_da_transacao, it_co_uf, it_no_uf, it_co_subitem_contabil, it_nu_cgc_uf, it_co_uf_bb, it_co_regiao_uf) FROM stdin;" >> ${FILE}.UF.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.programa(it_in_operacao, it_da_operacao, it_co_programa, it_no_programa, it_tx_descricao_pt_01, it_tx_descricao_pt_02) FROM stdin;" >> ${FILE}.PROGRAMA.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.instituicaoexterna(it_in_operacao, it_da_transacao, it_nu_instituicao_externa, it_no_mnemonico_inst_externa, it_no_instituicao_externa, it_co_pais) FROM stdin;" >> ${FILE}.INSTITUICAOEXTERNA.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.acao(it_in_operacao, it_da_transacao, gr_orgao_acao, it_no_acao, it_tx_descricao_acao_1, it_tx_descricao_acao_2, it_tx_descricao_acao_3, it_tx_descricao_acao_4, it_tx_descricao_acao_5, it_co_ug_coordena_acao) FROM stdin;" >> ${FILE}.ACAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.subacao(it_in_operacao, it_da_transacao, gr_orgao_subacao, it_no_subacao, it_tx_descricao_subacao_1, it_tx_descricao_subacao_2, it_tx_descricao_subacao_3, it_tx_descricao_subacao_4, it_tx_descricao_subacao_5, it_co_ug_coordena_subacao, it_co_acao_prop_subacao) FROM stdin;" >> ${FILE}.SUBACAO.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.ptrestb(it_in_operacao, it_da_operacao, gr_unidade_orcamentaria, gr_programa_trabalho_a, it_co_programa_trabalho_resumido) FROM stdin;" >> ${FILE}.PTRESTB.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.inscricaogenerica(it_in_operacao, it_da_transacao, it_co_inscricao_generica, it_nu_inscricao_generica, it_no_mnemonico_inscricao) FROM stdin;" >> ${FILE}.INSCRICAOGENERICA.sql;
echo -e "SET DATESTYLE TO ISO,DMY; \nCOPY siafi.projativsubtitulo(it_in_operacao, it_da_operacao, gr_projeto_atividade_subtitulo, it_in_proj_ativ_subtitulo, it_no_proj_ativ_subtitulo, it_tx_descricao_pt, it_in_meio_fim) FROM stdin;" >> ${FILE}.PROJATIVSUBTITULO.sql;

cat ${FILE} | grep ^[^TB*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do 
    
    TIPOREGISTRO=${LINHA:0:2};
    
    case $TIPOREGISTRO in

    
        # NOME/TIPO DO REGISTRO : TIPO 01 - TABELA DE UG
        "01") 
        CAMPO01=${LINHA:  2:  1};    #    00003   00001  ALFANUM IT-IN-OPERACAO
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

        CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                 
        	if [ -z "$CAMPO02" ]; then
        	CAMPO02=${NULO};
        	fi

        CAMPO03=${LINHA:11:6};    #    00012  00006  NUM     IT-CO-UNIDADE-GESTORA                    
        	if [ -z "$CAMPO03" ]; then
        	CAMPO03=${NULO};
        	fi

        CAMPO04=${LINHA:17:45};    #    00018  00045  ALFANUM IT-NO-UNIDADE-GESTORA             
        	if [ -z "$CAMPO04" ]; then 
        	CAMPO04=${NULO};
        	fi

        CAMPO05=${LINHA:62:19};    #    00063  00019  ALFANUM IT-NO-MNEMONICO-UNIDADE-GESTORA   
        	if [ -z "$CAMPO05" ]; then
        	CAMPO05=${NULO};
        	fi

        CAMPO06=${LINHA:81:1};    #    00082  00001  NUM     IT-IN-SITUACAO-UNIDADE-GESTORA           
        	if [ -z "$CAMPO06" ]; then
        	CAMPO06=${NULO};
        	fi

        CAMPO07=${LINHA:82:14};    #    00083  00014  NUM     IT-NU-CGC                               
        	if [ -z "$CAMPO07" ]; then
        	CAMPO07=${NULO};
        	fi

        CAMPO08=${LINHA:96:3};    #    00097  00003  NUM     IT-CO-PAIS                               
        	if [ -z "$CAMPO08" ]; then
        	CAMPO08=${NULO};
        	fi

        CAMPO09=${LINHA:99:2};    #    00100  00002  ALFANUM IT-CO-UF                           
        	if [ -z "$CAMPO09" ]; then
        	CAMPO09=${NULO};
        	fi

        CAMPO10=${LINHA:101:5};    #    00102  00005  NUM     GR-ORGAO                                
        	if [ -z "$CAMPO10" ]; then
        	CAMPO10=${NULO};
        	fi

        CAMPO11=${LINHA:106:6};    #    00107  00006  NUM     IT-CO-UNIDADE-GESTORA-SETO-ORCA         
        	if [ -z "$CAMPO11" ]; then
        	CAMPO11=${NULO};
        	fi

        CAMPO12=${LINHA:112:6};    #    00113  00006  NUM     IT-CO-UNIDADE-GESTORA-SETO-AUDI         
        	if [ -z "$CAMPO12" ]; then
        	CAMPO12=${NULO};
        	fi

        CAMPO13=${LINHA:118:6};    #    00119  00006  NUM     IT-CO-UNIDADE-GESTORA-SETO-CONT         
        	if [ -z "$CAMPO13" ]; then
        	CAMPO13=${NULO};
        	fi

        CAMPO14=${LINHA:124:6};    #    00125  00006  NUM     IT-CO-UNIDADE-GESTORA-POLO              
        	if [ -z "$CAMPO14" ]; then
        	CAMPO14=${NULO};
        	fi

        CAMPO15=${LINHA:130:65};    #    00131  00065  ALFANUM IT-ED-ENDERECO                   
        	if [ -z "$CAMPO15" ]; then
        	CAMPO15=${NULO};
        	fi

        CAMPO16=${LINHA:195:11};    #    00196  00011  NUM     IT-NU-CPF-ORDENADOR-ASS                
        	if [ -z "$CAMPO16" ]; then
        	CAMPO16=${NULO};
        	fi

        CAMPO17=${LINHA:206:25};    #    00207  00025  ALFANUM IT-NO-ORDENADOR-ASS              
        	if [ -z "$CAMPO17" ]; then
        	CAMPO17=${NULO};
        	fi

        CAMPO18=${LINHA:231:3};    #    00232  00003  NUM     IT-CO-MOEDA                             
        	if [ -z "$CAMPO18" ]; then
        	CAMPO18=${NULO};
        	fi

        CAMPO19=${LINHA:234:1};    #    00235  00001  ALFANUM IT-IN-ESFERA-ADMINISTRATIVA       
        	if [ -z "$CAMPO19" ]; then
        	CAMPO19=${NULO};
        	fi

        CAMPO20=${LINHA:235:8};    #    00236  00008  NUM     IT-CO-CEP-NOVO                          
        	if [ -z "$CAMPO20" ]; then
        	CAMPO20=${NULO};
        	fi

        CAMPO21=${LINHA:243:45};    #    00244  00045  ALFANUM IT-NU-FONE-UG                    
        	if [ -z "$CAMPO21" ]; then
        	CAMPO21=${NULO};
        	fi

        CAMPO22=${LINHA:288:1};    #    00289  00001  NUM     IT-IN-FUNCAO-UG                         
              if [ -z "$CAMPO22" ]; then
        	CAMPO22=${NULO};
        	fi

        CAMPO23=${LINHA:289:1};    #    00290  00001  NUM     IT-IN-ESTADO-MUNICIPIO-UG               
              if [ -z "$CAMPO23" ]; then
        	CAMPO23=${NULO};
        	fi

        CAMPO24=${LINHA:290:4};    #    00291  00004  NUM     IT-CO-MUNICIPIO                         
              if [ -z "$CAMPO24" ]; then
        	CAMPO24=${NULO};
        	fi

        CAMPO25=${LINHA:294:6};    #    00295  00006  NUM     IT-CO-UG-CONTROLE-INTERNO               
              if [ -z "$CAMPO25" ]; then
        	CAMPO25=${NULO};
        	fi

        CAMPO26=${LINHA:300:1};    #    00301  00001  NUM     IT-IN-USO-CPR                        
              if [ -z "$CAMPO26" ]; then
        	CAMPO26=${NULO};
        	fi

       REGUG=`expr $REGUG + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26 >> ${FILE}.UG.sql;
	;;

        # NOME/TIPO DO REGISTRO : TIPO 02 - TABELA GESTAO
        "02")
       CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                  
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)             
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:1};    #    00012  00001  ALFANUM IT-IN-ATIVO                    
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:12:5};    #    00013  00005  NUM     IT-CO-GESTAO                         
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:17:45};    #    00018  00045  ALFANUM IT-NO-GESTAO                  
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:62:19};    #    00063  00019  ALFANUM IT-NO-MNEMONICO-GESTAO        
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:81:1};    #    00082  00001  NUM     IT-IN-PERTENCE-GESTAO-10000          
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

	CAMPO08=${LINHA:82:1};    #    00083  00001  NUM     IT-IN-GERE-FUNDO                     
		if [ -z "$CAMPO08" ]; then
		CAMPO08=${NULO};
		fi

	CAMPO09=${LINHA:83:1};    #    00084  00001  NUM     IT-IN-INSCREVE-RESTO-PAGAR           
		if [ -z "$CAMPO09" ]; then
		CAMPO09=${NULO};
		fi

	CAMPO10=${LINHA:84:1};           #    00085  00001  NUM     IT-IN-RECURSOS-DIFERIDOS          
		if [ -z "$CAMPO10" ]; then
		CAMPO10=${NULO};
		fi

       REGGESTAO=`expr $REGGESTAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10  >> ${FILE}.GESTAO.sql;;
	

        # NOME/TIPO DO REGISTRO : TIPO 03 - TABELA ORGAO
        "03")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                        
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                   
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:5};    #    00012  00005  NUM     GR-ORGAO                             
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:16:45};    #    00017  00045  ALFANUM IT-NO-ORGAO                               
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:61:1};    #    00062  00001  NUM     IT-IN-PLANO-INTERNO                        
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:62:6};    #    00063  00006  NUM     IT-CO-UNID-GEST-SETO-FINA-PAIS             
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:68:6};    #    00069  00006  NUM     IT-CO-ORGAO-VINCULACAO                     
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

	CAMPO08=${LINHA:74:1};    #    00075  00001  NUM     IT-IN-TIPO-ADMINISTRACAO             
		if [ -z "$CAMPO08" ]; then
		CAMPO08=${NULO};
		fi

	CAMPO09=${LINHA:75:19};    #    00076  00019  ALFANUM IT-NO-MNEMONICO-ORGAO                     
		if [ -z "$CAMPO09" ]; then
		CAMPO09=${NULO};
		fi

	CAMPO10=${LINHA:94:1};    #    00095  00001  NUM     IT-IN-RECEBE-COTA-STN                      
		if [ -z "$CAMPO10" ]; then
		CAMPO10=${NULO};
		fi

	CAMPO11=${LINHA:95:1};    #    00096  00001  NUM     IT-IN-UTILIZACAO                           
		if [ -z "$CAMPO11" ]; then
		CAMPO11=${NULO};
		fi

	CAMPO12=${LINHA:96:5};    #    00097  00005  NUM     IT-CO-GESTAO                               
		if [ -z "$CAMPO12" ]; then
		CAMPO12=${NULO};
		fi

	CAMPO13=${LINHA:101:1};    #    00102  00001  NUM     IT-IN-ORCAMENTO-SOF-SEPLAN                
		if [ -z "$CAMPO13" ]; then
		CAMPO13=${NULO};
		fi

	CAMPO14=${LINHA:102:1};    #    00103  00001  NUM     IT-IN-PODER                               
		if [ -z "$CAMPO14" ]; then
		CAMPO14=${NULO};
		fi

	CAMPO15=${LINHA:103:6};    #    00104  00006  NUM     IT-CO-UNIDADE-GESTORA-COORDEN             
		if [ -z "$CAMPO15" ]; then
		CAMPO15=${NULO};
		fi

	CAMPO16=${LINHA:109:1};    #    00110  00001  NUM     IT-IN-PROGRAMACAO-ORCAMENTARIA            
		if [ -z "$CAMPO16" ]; then
		CAMPO16=${NULO};
		fi

	CAMPO17=${LINHA:110:14};    #    00111  00014  NUM     IT-NU-CGC-ORGAO                    
		if [ -z "$CAMPO17" ]; then
		CAMPO17=${NULO};
		fi

	CAMPO18=${LINHA:124:1};    #    00125  00001  ALFANUM IT-IN-SUBACAO                             
		if [ -z "$CAMPO18" ]; then
		CAMPO18=${NULO};
		fi

	CAMPO19=${LINHA:125:5};    #    00126  00005  NUM     IT-CO-ORGAO-DOU                           
		if [ -z "$CAMPO19" ]; then
		CAMPO19=${NULO};
		fi

	CAMPO20=${LINHA:130:6};    #    00131  00006  NUM     IT-CO-UNID-GEST-SETO-CONT                 
		if [ -z "$CAMPO20" ]; then
		CAMPO20=${NULO};
		fi

	CAMPO21=${LINHA:136:1};    #    00137  00001  NUM     IT-IN-ACUMULA-BALANCO-UNIAO         
		if [ -z "$CAMPO21" ]; then
		CAMPO21=${NULO};
		fi

	CAMPO22=${LINHA:137:10};    #    00138  00010  ALFANUM IT-NU-CONTA-INSTITUCIONAL          
		if [ -z "$CAMPO22" ]; then
		CAMPO22=${NULO};
		fi

	CAMPO23=${LINHA:147:2};    #    00148  00002  ALFANUM IT-CO-FASE-PROGRAMACAO                    
		if [ -z "$CAMPO23" ]; then
		CAMPO23=${NULO};
		fi

	CAMPO24=${LINHA:149:1};    #    00150  00001  NUM     IT-IN-USO-CPR                             
		if [ -z "$CAMPO24" ]; then
		CAMPO24=${NULO};
		fi

	CAMPO25=${LINHA:150:6};           #    00151  00006  NUM     IT-CO-UNIDADE-GESTORA-INCORPORACAO     
		if [ -z "$CAMPO25" ]; then
		CAMPO25=${NULO};
		fi

       REGORGAO=`expr $REGORGAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25 >> ${FILE}.ORGAO.sql;;


        # NOME/TIPO DO REGISTRO : TIPO 04 - TABELA UNIDADE ORCA
        "04")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                            
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                                                                       
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:5};    #    00012  00005  NUM     GR-UNIDADE-ORCAMENTARIA                                                                        
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:16:5};    #    00017  00005  NUM     GR-ORGAO                                                                                       
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:21:45};    #    00022  00045  ALFANUM IT-NO-UNIDADE-ORCAMENTARIA                                                              
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:66:5};    #    00067  00005  NUM     IT-CO-GESTAO                                                                                   
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:71:6};           #    00072  00006  NUM     IT-CO-UNIDADE-GESTORA-RESP                                                                  
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

       REGUO=`expr $REGUO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07 >> ${FILE}.UO.sql;;

	# NOME/TIPO DO REGISTRO : TIPO 05 - TABELA BANCO ORIGINAL
	"05")

	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi

	# 00012 00003  NUM     IT-CO-BANCO
	CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi

	# 00015 00045  ALFANUM IT-NO-DOMICILIO-BANCARIO
	CAMPO04=${LINHA:14:45}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi

	# 00060 00015  ALFANUM IT-NO-MNEMONICO-BANCO
	CAMPO05=${LINHA:59:15}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi

	# 00075 00001  NUM     IT-IN-INSTITUICAO-FINACEIRA
	CAMPO06=${LINHA:74:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

	# 00076 00001  NUM     IT-IN-BANCO-CTU
	CAMPO07=${LINHA:76:1}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi

	# 00077 00001  NUM     IT-IN-ALTERA-AGENCIA
	CAMPO08=${LINHA:76:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi

	# 00078 00002  NUM     IT-NU-SUBITEM
	CAMPO09=${LINHA:77:2}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi

	REGBANCOORIGINAL=`expr $REGBANCOORIGINAL + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09 >> ${FILE}.BANCOORIGINAL.sql;; 

	# NOME/TIPO DO REGISTRO : TIPO 06 - TABELA AGENCIA BB
	"06")

	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
	CAMPO02=${LINHA: 7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi

	# 00012 00003  NUM     IT-CO-BANCO
	CAMPO03=${LINHA:11:3}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi

	# 00015 00004  NUM     IT-CO-AGENCIA
	CAMPO04=${LINHA:14:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi

	# 00019 00045  ALFANUM IT-NO-AGENCIA
	CAMPO05=${LINHA:18:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi

	# 00064 00020  ALFANUM IT-NO-PRACA-PAGAMENTO
	CAMPO06=${LINHA:63:20}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

	# 00084 00019  ALFANUM IT-NO-MNEMONICO-AGENCIA
	CAMPO07=${LINHA:83:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi

	# 00103 00001  ALFANUM IT-IN-AGENCIA-EXTERIOR
	CAMPO08=${LINHA:102:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi

	# 00104 00050  ALFANUM IT-TX-ENDERECO
	CAMPO09=${LINHA:103:50}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi

	# 00154 00008  NUM     IT-CO-CEP
	CAMPO10=${LINHA:153:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
	REGAGENCIABB=`expr $REGBAGENCIABB + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10 >> ${FILE}.AGENCIABB.sql;;


        # NOME/TIPO DO REGISTRO : TIPO 07 - TABELA CATEGORIA GASTO
        "07")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                                                              
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)                                                                                                        
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:1};    #    00012  00001  ALFANUM GR-CATEGORIA-GASTO                                                                                                         
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:12:45};    #    00013  00045  ALFANUM IT-NO-CATEGORIA-GASTO                                                                                                     
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:57:19};        #    00058  00019  ALFANUM IT-NO-MNEMONICO-CATEGORIA-GASTO                                                                                           
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

       REGCATEGORIAGASTO=`expr $REGCATEGORIAGASTO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.CATEGORIAGASTO.sql;;  


        # NOME/TIPO DO REGISTRO : TIPO 08 - TABELA PLANO CONTA
        "08")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                                         		
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)                                          			
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:9};    #    00012  00009  NUM     GR-CODIGO-CONTA                                                     		
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:20:45};    #    00021  00045  ALFANUM IT-NO-CONTA                                                             
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:65:253};    #    00066  00253  ALFANUM IT-TX-FUNCAO-CONTA                                                  	
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:318:110};    #    00319  00770  ALFANUM IT-TX-CIRCUNSTANCIA-DEBITO CAMPO DE 110 BYTES QUE OCRRE 7 VEZES      	
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:428:110};
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

	CAMPO08=${LINHA:538:110};                    
		if [ -z "$CAMPO08" ]; then
		CAMPO08=${NULO};
		fi

	CAMPO09=${LINHA:648:110};                    
		if [ -z "$CAMPO09" ]; then
		CAMPO09=${NULO};
		fi

	CAMPO10=${LINHA:758:110};                  	
		if [ -z "$CAMPO10" ]; then
		CAMPO10=${NULO};
		fi

	CAMPO11=${LINHA:868:110};                  	
		if [ -z "$CAMPO11" ]; then
		CAMPO11=${NULO};
		fi

	CAMPO12=${LINHA:978:110};                  	
		if [ -z "$CAMPO12" ]; then
		CAMPO12=${NULO};
		fi

	CAMPO13=${LINHA:1088:110};    #    01089  00770  ALFANUM IT-TX-CIRCUNSTANCIA-CREDITO CAMPO DE 110 BYTES QUE OCORRE 7 VEZES   	                  	
		if [ -z "$CAMPO13" ]; then
		CAMPO13=${NULO};
		fi

	CAMPO14=${LINHA:1198:110};                  	
		if [ -z "$CAMPO14" ]; then
		CAMPO14=${NULO};
		fi

	CAMPO15=${LINHA:1308:110};                  	
		if [ -z "$CAMPO15" ]; then
		CAMPO15=${NULO};
		fi

	CAMPO16=${LINHA:1418:110};                  	
		if [ -z "$CAMPO16" ]; then
		CAMPO16=${NULO};
		fi

	CAMPO17=${LINHA:1528:110};                  	
		if [ -z "$CAMPO17" ]; then
		CAMPO17=${NULO};
		fi

	CAMPO18=${LINHA:1638:110};                  	
		if [ -z "$CAMPO18" ]; then
		CAMPO18=${NULO};
		fi

	CAMPO19=${LINHA:1748:110};                  	
		if [ -z "$CAMPO19" ]; then
		CAMPO19=${NULO};
		fi

	CAMPO20=${LINHA:1858:210};    #    01859  00210  ALFANUM IT-TX-SIGNIFICADO-SALDO                                             	
		if [ -z "$CAMPO20" ]; then
		CAMPO20=${NULO};
		fi

	CAMPO21=${LINHA:2068:210};    #    02069  00210  ALFANUM IT-TX-OBSERVACAO-CONTA                                              	
		if [ -z "$CAMPO21" ]; then
		CAMPO21=${NULO};
		fi

	CAMPO22=${LINHA:2278:2};    #    02279  00002  NUM     IT-IN-CONTA-CORRENTE-CONTABIL                     		
		if [ -z "$CAMPO22" ]; then
		CAMPO22=${NULO};
		fi

	CAMPO23=${LINHA:2280:1};    #    02281  00001  NUM     IT-IN-ENCERRAMENTO                                		
		if [ -z "$CAMPO23" ]; then
		CAMPO23=${NULO};
		fi

	CAMPO24=${LINHA:2281:1};    #    02282  00001  NUM     IT-IN-INVERSAO-SALDO                              		
		if [ -z "$CAMPO24" ]; then
		CAMPO24=${NULO};
		fi

	CAMPO25=${LINHA:2282:1};    #    02283  00001  NUM     IT-IN-ESCRITURACAO                                		
		if [ -z "$CAMPO25" ]; then
		CAMPO25=${NULO};
		fi

	CAMPO26=${LINHA:2283:1};    #    02284  00001  ALFANUM IT-IN-SALDO-CONTABIL                              	
		if [ -z "$CAMPO26" ]; then
		CAMPO26=${NULO};
		fi

	CAMPO27=${LINHA:2284:1};    #    02285  00001  NUM     IT-IN-LANCAMENTO-ORGAO                            		
		if [ -z "$CAMPO27" ]; then
		CAMPO27=${NULO};
		fi

	CAMPO28=${LINHA:2285:1};    #    02286  00001  NUM     IT-IN-INTEGRACAO                                  		
		if [ -z "$CAMPO28" ]; then
		CAMPO28=${NULO};
		fi

	CAMPO29=${LINHA:2286:1};    #    02287  00001  ALFANUM IT-IN-SISTEMA-CONTABIL                            	
		if [ -z "$CAMPO29" ]; then
		CAMPO29=${NULO};
		fi

	CAMPO30=${LINHA:2287:1};    #    02288  00001  NUM     IT-IN-UTILIZACAO-SAFEM                            		
		if [ -z "$CAMPO30" ]; then
		CAMPO30=${NULO};
		fi

	CAMPO31=${LINHA:2288:1};    #    02289  00001  NUM     IT-IN-LANCAMENTO-NSSALDO                          		
		if [ -z "$CAMPO31" ]; then
		CAMPO31=${NULO};
		fi

	CAMPO32=${LINHA:2289:140};    #    02290  00140  ALFANUM IT-TX-MOTIVO                                      	
		if [ -z "$CAMPO32" ]; then
		CAMPO32=${NULO};
		fi

	CAMPO33=${LINHA:2429:70};    #    02430  02100  ALFANUM IT-TX-FUNCA CAMPO DE 70 BYTES QUE OCORRE 30 VEZES 	
		if [ -z "$CAMPO33" ]; then
		CAMPO33=${NULO};
		fi

	CAMPO34=${LINHA:2499:70};	
		if [ -z "$CAMPO34" ]; then
		CAMPO34=${NULO};
		fi

	CAMPO35=${LINHA:2569:70};	
		if [ -z "$CAMPO35" ]; then
		CAMPO35=${NULO};
		fi

	CAMPO36=${LINHA:2639:70};	
		if [ -z "$CAMPO36" ]; then
		CAMPO36=${NULO};
		fi

	CAMPO37=${LINHA:2709:70};	
		if [ -z "$CAMPO37" ]; then
		CAMPO37=${NULO};
		fi

	CAMPO38=${LINHA:2779:70};	
		if [ -z "$CAMPO38" ]; then
		CAMPO38=${NULO};
		fi

	CAMPO39=${LINHA:2849:70};	
		if [ -z "$CAMPO39" ]; then
		CAMPO39=${NULO};
		fi

	CAMPO40=${LINHA:2919:70};	
		if [ -z "$CAMPO40" ]; then
		CAMPO40=${NULO};
		fi

	CAMPO41=${LINHA:2989:70};	
		if [ -z "$CAMPO41" ]; then
		CAMPO41=${NULO};
		fi

	CAMPO42=${LINHA:3059:70};	
		if [ -z "$CAMPO42" ]; then
		CAMPO42=${NULO};
		fi

	CAMPO43=${LINHA:3129:70};	
		if [ -z "$CAMPO43" ]; then
		CAMPO43=${NULO};
		fi

	CAMPO44=${LINHA:3199:70};	
		if [ -z "$CAMPO44" ]; then
		CAMPO44=${NULO};
		fi

	CAMPO45=${LINHA:3269:70};	
		if [ -z "$CAMPO45" ]; then
		CAMPO45=${NULO};
		fi

	CAMPO46=${LINHA:3339:70};	
		if [ -z "$CAMPO46" ]; then
		CAMPO46=${NULO};
		fi

	CAMPO47=${LINHA:3709:70};	
		if [ -z "$CAMPO47" ]; then
		CAMPO47=${NULO};
		fi

	CAMPO48=${LINHA:3779:70};	
		if [ -z "$CAMPO48" ]; then
		CAMPO48=${NULO};
		fi

	CAMPO49=${LINHA:3849:70};	
		if [ -z "$CAMPO49" ]; then
		CAMPO49=${NULO};
		fi

	CAMPO50=${LINHA:3919:70};	
		if [ -z "$CAMPO50" ]; then
		CAMPO50=${NULO};
		fi

	CAMPO51=${LINHA:3989:70};	
		if [ -z "$CAMPO51" ]; then
		CAMPO51=${NULO};
		fi

	CAMPO52=${LINHA:4059:70};	
		if [ -z "$CAMPO52" ]; then
		CAMPO52=${NULO};
		fi

	CAMPO53=${LINHA:4129:70};	
		if [ -z "$CAMPO53" ]; then
		CAMPO53=${NULO};
		fi

	CAMPO54=${LINHA:4199:70};	
		if [ -z "$CAMPO54" ]; then
		CAMPO54=${NULO};
		fi

	CAMPO55=${LINHA:4269:70};	
		if [ -z "$CAMPO55" ]; then
		CAMPO55=${NULO};
		fi

	CAMPO56=${LINHA:4339:70};	
		if [ -z "$CAMPO56" ]; then
		CAMPO56=${NULO};
		fi

	CAMPO57=${LINHA:4409:70};	
		if [ -z "$CAMPO57" ]; then
		CAMPO57=${NULO};
		fi

	CAMPO58=${LINHA:4479:70};	
		if [ -z "$CAMPO58" ]; then
		CAMPO58=${NULO};
		fi

	CAMPO59=${LINHA:4549:70};	
		if [ -z "$CAMPO59" ]; then
		CAMPO59=${NULO};
		fi

	CAMPO60=${LINHA:4619:70};	
		if [ -z "$CAMPO60" ]; then
		CAMPO60=${NULO};
		fi

	CAMPO61=${LINHA:4689:70};	
		if [ -z "$CAMPO61" ]; then
		CAMPO61=${NULO};
		fi

	CAMPO62=${LINHA:4759:70};	   
		if [ -z "$CAMPO62" ]; then
		CAMPO62=${NULO};
		fi

       REGPLANOCONTA=`expr $REGPLANOCONTA + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27${TAB}$CAMPO28${TAB}$CAMPO29${TAB}$CAMPO30${TAB}$CAMPO31${TAB}$CAMPO32${TAB}$CAMPO33${TAB}$CAMPO34${TAB}$CAMPO35${TAB}$CAMPO36${TAB}$CAMPO37${TAB}$CAMPO38${TAB}$CAMPO39${TAB}$CAMPO40${TAB}$CAMPO41${TAB}$CAMPO42${TAB}$CAMPO43${TAB}$CAMPO44${TAB}$CAMPO45${TAB}$CAMPO46${TAB}$CAMPO47${TAB}$CAMPO48${TAB}$CAMPO49${TAB}$CAMPO50${TAB}$CAMPO51${TAB}$CAMPO52${TAB}$CAMPO53${TAB}$CAMPO54${TAB}$CAMPO55${TAB}$CAMPO56${TAB}$CAMPO57${TAB}$CAMPO58${TAB}$CAMPO59${TAB}$CAMPO60${TAB}$CAMPO61${TAB}$CAMPO62 >> ${FILE}.PLANOCONTA.sql;;

	# NOME/TIPO DO REGISTRO : TIPO 09 - TABELA EVENTO
	"09")

	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
	CAMPO02=${LINHA: 7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi

	# 00012 00006  NUM     GR-CODIGO-EVENTO
	CAMPO03=${LINHA:11:6}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi

	# 00018 00030  ALFANUM IT-NO-EVENTO
	CAMPO04=${LINHA:17: 30}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi

	# 00048 00150  ALFANUM IT-TX-DESCRICAO-EVENTO
	CAMPO05=${LINHA:47:150}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi

	# 00198 00100  ALFANUM IT-TX-OBSERVACAO-EVENTO
	CAMPO06=${LINHA:197:100}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

	# 00298 00001  NUM     IT-IN-ABERTURA-ENCERRAMENTO
	CAMPO07=${LINHA:297:1}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi

	# 00299 00001  NUM     IT-IN-ESTORNO-EVENTO
	CAMPO08=${LINHA:299:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi

	# 00300 00001  NUM     IT-IN-EVENTO-PROVISORIO
	CAMPO09=${LINHA:299:1}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi

	# 00301 00001  NUM     IT-IN-VERIFICA-SALDO
	CAMPO10=${LINHA:300:1}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi

	# 00302 00140  ALFANUM IT-TX-MOTIVO
	CAMPO11=${LINHA:139:140}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi

	# 00442 00002  NUM     IT-IN-UG-EMITENTE
	CAMPO12=${LINHA:441:2}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi

	# 00444 00001  NUM     IT-IN-GESTAO-EMITENTE
	CAMPO13=${LINHA:443:1}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi

	# 00445 00001  NUM     IT-IN-FAVORECIDO
	CAMPO14=${LINHA:444:1}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi

	# 00446 00002  NUM     IT-IN-UG-FAVORECIDO
	CAMPO15=${LINHA:445:2}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi

	# 00448 00001  NUM     IT-IN-GESTAO-FAVORECIDO
	CAMPO16=${LINHA:447:1}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi

	# 00449 00001  NUM     IT-IN-CAMBIO
	CAMPO17=${LINHA:448:1}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi

	# 00450 00001  NUM     IT-IN-RESTRICAO-UO
	CAMPO18=${LINHA:449:1}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi

	# 00451 00001  NUM     IT-IN-EXERCICIO-NE
	CAMPO19=${LINHA:450:1}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi

	# 00452 00001  NUM     IT-IN-VERIF-EQUILIBRIO-SIST-CONT
	CAMPO20=${LINHA:451:1}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi

	# 00453 00001  ALFANUM IT-IN-DETALHAMENTO-PLANO-INTERNO
	CAMPO21=${LINHA:452:1}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi

	# 00454 00002  ALFANUM IT-IN-INSCRICAO1-EVENTO
	CAMPO22=${LINHA:453:2}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi

	# 00456 00002  ALFANUM IT-IN-INSCRICAO2-EVENTO
	CAMPO23=${LINHA:455:2}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi

	# 00458 00001  NUM     IT-IN-RESTRICAO1-FONTE
	CAMPO24=${LINHA:457:1}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi

	# 00459 00001  NUM     IT-IN-RESTRICAO2-FONTE
	CAMPO25=${LINHA:458:1}; if [ -z "$CAMPO25" ]; then CAMPO25=${NULO}; fi

	# 00460 00001  ALFANUM IT-IN-DETALHAMNETO1-FONTE
	CAMPO26=${LINHA:459:1}; if [ -z "$CAMPO26" ]; then CAMPO26=${NULO}; fi

	# 00461 00001  ALFANUM IT-IN-DETALHAMNETO2-FONTE
	CAMPO27=${LINHA:460:1}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	
	REGEVENTO=`expr $REGEVENTO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27 >> ${FILE}.EVENTO.sql;;


        # NOME/TIPO DO REGISTRO : TIPO 10 - TABELA PROGRAMA TRABALHO
        "10")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO        
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:17};    #    00012  00017  ALFANUM GR-PROGRAMA-TRABALHO 
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:28:7};    #    00029  00007  ALFANUM IT-CO-MUNICIPIO-IBGE 
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:35:2};    #    00036  00002  ALFANUM IT-CO-UF          
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:37:2};    #    00038  00002  ALFANUM IT-CO-REGIAO      
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

       REGPROGRAMATRABALHO=`expr $REGPROGRAMATRABALHO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.PROGRAMATRABALHO.sql;; 

	# NOME/TIPO DO REGISTRO : TIPO 11 - TABELA CREDOR
	"11")
	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi

	# 00012 00014  ALFANUM IT-CO-CREDOR
	CAMPO03=${LINHA:11:14}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi

	# 00026 00001  NUM     IT-CO-TIPO-CRDOR
	CAMPO04=${LINHA:25:1}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi

	# 00027 00019  ALFANUM IT-NO-MNEMONICO-CREDOR
	CAMPO05=${LINHA:26:19}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi

	# 00046 00055  ALFANUM IT-NO-CREDOR
	CAMPO06=${LINHA:46:55}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

	# 00101 00065  ALFANUM IT-ED-CREDOR
	CAMPO07=${LINHA:100:65}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi

	# 00166 00004  NUM     IT-CO-MUNICIPIO-CREDOR
	CAMPO08=${LINHA:165:4}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi

	# 00170 00008  NUM     IT-CO-CEP-CREDOR
	CAMPO09=${LINHA:169:8}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi

	# 00178 00002  ALFANUM IT-CO-UF-CREDOR
	CAMPO10=${LINHA:177:2}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi

	# 00180 00045  ALFANUM IT-NU-TELEFONE-CREDOR
	CAMPO11=${LINHA:179:45}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi

	# 00225 00140  ALFANUM IT-TX-MOTIVO-CREDOR
	CAMPO12=${LINHA:224:140}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi

	# 00365 00001  NUM     IT-IN-SITUACAO-CREDOR-SRF
	CAMPO13=${LINHA:264:1}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi

	# 00366 00008  NUM     IT-DA-SITUACAO-CREDOR-SRF
	CAMPO14=${LINHA:365:8}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi

	REGCREDOR=`expr $REGCREDOR + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14 >> ${FILE}.CREDOR.sql;;


        # TIPO 12 - TABELA DESTINACAO RECEITA
        "12")
		# 00003  00001  ALFANUM IT-IN-OPERACAO
		CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

		# 00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
		CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi

		# 00012  00005  NUM     IT-CO-DESTINACAO-RECEITA 
		CAMPO03=${LINHA:11:5}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi

		# 00017  00045  NUM     IT-NO-DESTINACAO-RECEITA
		CAMPO04=${LINHA:16:45}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi

		# 00062  00019  ALFANUM IT-NO-MNEMONICO-DESTINACAO-REC
		CAMPO05=${LINHA:61:19}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi

		# 00081  00001  NUM     IT-CO-TIPO-BENEFICIARIO
		CAMPO06=${LINHA:80:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

		# 00082  00002  NUM     IT-CO-TIPO-DEST-RECEITA
		CAMPO07=${LINHA:81:2}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi

		# 00084  00010  NUM     IT-CO-FONTE-RECURSO
		CAMPO08=${LINHA:83:10}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi

		# 00094  00008  NUM     IT-PE-SALDO-COEFICIENTE (N1,7)
		CAMPO09=${LINHA:93:8}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi

		# 00102  00006  NUM     IT-CO-EVENTO-ARRECADACAO-DEST
		CAMPO10=${LINHA:101:6}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi

		# 00108  00006  NUM     IT-CO-EVENTO-INCETIVO-DEST
		CAMPO11=${LINHA:107:6}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi

		# 00114  00006  NUM     IT-CO-EVENTO-RETIFICACAO-DEST
		CAMPO12=${LINHA:113:6}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi

		# 00120  00006  NUM     IT-CO-EVENTO-RESTITUICAO-DEST
		CAMPO13=${LINHA:119:6}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi

		# 00126  00006  NUM     IT-CO-EVENTO-ARRECADACAO-FONTE
		CAMPO14=${LINHA:125:6}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi

		# 00132  00006  NUM     IT-CO-EVENTO-INCENTIVO-FONTE
		CAMPO15=${LINHA:131:6}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi

		# 00138  00006  NUM     IT-CO-EVENTO-RETIFICACAO-FONTE
		CAMPO16=${LINHA:137:6}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi

		# 00144  00006  NUM     IT-CO-EVENTO-RESTITUICAO-FONTE
		CAMPO17=${LINHA:143:6}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi

	       REGDESTINACAORECEITA=`expr $REGDESTINACAORECEITA + 1`;
		echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17 >> ${FILE}.DESTINACAORECEITA.sql;; 




        # NOME/TIPO DO REGISTRO : TIPO 13 - TABELA VINCULA PAGAMENTO
        "13")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                   
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)                                                             
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:3};    #    00012  00003  NUM     IT-CO-VINCULACAO-PAGAMENTO                                                            
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:14:45};    #    00015  00045  NUM     IT-NO-VINCULACAO-PAGAMENTO                                                        
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

       REGVINCULAPAGAMENTO=`expr $REGVINCULAPAGAMENTO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04 >> ${FILE}.VINCULAPAGAMENTO.sql;;  


        # NOME/TIPO DO REGISTRO : TIPO 14 - TABELA FONTE RECURSO
        "14")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                               
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                                                                          
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:2};    #    00012  00002  ALFANUM GR-FONTE                                                                                          
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:13:45};    #    00014  00045  ALFANUM IT-NO-FONTE                                                                                      
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:58:1};    #    00059  00001  NUM     IT-IN-TIPO-FONTE-SOF                                                                              
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO06=${LINHA:59:1};           #    00060  00001  NUM     IT-IN-FONTE-SOF-PROGRAMACAO                                                                    
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

       REGFONTE=`expr $REGFONTE + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.FONTERECURSO.sql;; 


        # NOME/TIPO DO REGISTRO : TIPO 15 - TABELA FUNCAO PT
        "15")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                                                      
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                                                                                                 
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:2};    #    00012  00002  ALFANUM IT-CO-FUNCAO                                                                                                             
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:13:60};    #    00014  00060  ALFANUM IT-NO-FUNCAO                                                                                                            
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:73:55};    #    00074  00110  ALFANUM IT-TX-DESCRICAO-PT CAMPO COM 55 BYTES QUE OCORRE 2 VEZES                                                                
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:128:55};                                                                   
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

       REGFUNCAO=`expr $REGFUNCAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.FUNCAO.sql;; 


        # NOME/TIPO DO REGISTRO : TIPO 16 - TABELA SUBFUNCAO PT
        "16")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                                                                
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                                                                                                           
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:2};    #    00012  00003  ALFANUM IT-CO-SUBFUNCAO                                                                                                                    
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:13:60};    #    00015  00060  ALFANUM IT-NO-SUBFUNCAO                                                                                                                   
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:73:55};    #    00075  00110  ALFANUM IT-TX-DESCRICAO-PT-SUBFUNC CAMPO DE 55 BYTES QUE OCORRE 2 VEZES                                                                   
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:128:55};                                                                      
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

       REGSUBFUNCAO=`expr $REGSUBFUNCAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.SUBFUNCAO.sql;; 


	# NOME/TIPO DO REGISTRO : TIPO 17 - TABELA RECEITA FEDERAL
	"17")
	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	
	# 00004 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00012 00004  NUM     IT-CO-RECEITA
	CAMPO03=${LINHA:11:4}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00016 00005  NUM     IT-CO-DESTINACAO-RECEITA
	CAMPO04=${LINHA:15:5}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00021 00002  NUM     IT-CO-TIPO-RECEITA
	CAMPO05=${LINHA:20:2}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	# 00023 00045  NUM     IT-NO-RECEITA
	CAMPO06=${LINHA:22:45}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	# 00068 00019  ALFANUM IT-NO-TITULO-REDUZIDO-RECEITA
	CAMPO07=${LINHA:67:19}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	# 00087 00001  ALFANUM IT-IN-PERMITE-DARF
	CAMPO08=${LINHA:86:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
	# 00088 00006  NUM     IT-CO-EVENTO-ARRECADACAO
	CAMPO09=${LINHA:87:6}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
	# 00094 00006  NUM     IT-CO-EVENTO-INCETIVO
	CAMPO10=${LINHA:93:6}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
	# 00100 00006  NUM     IT-CO-EVENTO-RETIFICACAO
	CAMPO11=${LINHA:99:6}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	
	# 00106 00006  NUM     IT-CO-EVENTO-RESTITUICAO
	CAMPO12=${LINHA:105:6}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	
	# 00112 00010  NUM     GR-FONTE-RECURSO
	CAMPO13=${LINHA:111:10}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi

	REGRECEITAFEDERAL=`expr $REGRECEITAFEDERAL + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13 >> ${FILE}.RECEITAFEDERAL.sql;;	

        # NOME/TIPO DO REGISTRO : TIPO 18 - TABELA PLANO INTERNO
        "18")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                     
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                                                
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:5};    #    00012  00005  NUM     GR-ORGAO                                                                
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:16:11};    #    00017  00011  ALFANUM IT-CO-PLANO-INTERNO                                              
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:27:45};    #    00028  00045  NUM     IT-NO-PLANO-INTERNO                                              
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:72:5};    #    00073  00005  NUM     GR-UNIDADE-ORCAMENTARIA                                                 
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:77:5};    #    00078  00005  ALFANUM IT-CO-SUBACAO                                                     
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

	CAMPO08=${LINHA:82:2};    #    00083  00002  NUM     IT-IN-ESFERA-ORCAMENTARIA                                               
		if [ -z "$CAMPO08" ]; then
		CAMPO08=${NULO};
		fi

	CAMPO09=${LINHA:84:1};    #    00085  00001  ALFANUM IT-IN-DESDOBRA-ETAPA                                              
		if [ -z "$CAMPO09" ]; then
		CAMPO09=${NULO};
		fi

	CAMPO10=${LINHA:85:1};    #    00086  00001  ALFANUM IT-IN-FISICO-FINANCEIRO                                           
		if [ -z "$CAMPO10" ]; then
		CAMPO10=${NULO};
		fi

	CAMPO11=${LINHA:86:60};    #    00087  00240  ALFANUM IT-TX-OBJETIVO-PLANO-INTERNO CAMPO DE 60 BYTES QUE OCORRE 4 VEZES
		if [ -z "$CAMPO11" ]; then
		CAMPO11=${NULO};
		fi

	CAMPO12=${LINHA:146:60};                                     
		if [ -z "$CAMPO12" ]; then
		CAMPO12=${NULO};
		fi

	CAMPO13=${LINHA:206:60};                                     
		if [ -z "$CAMPO13" ]; then
		CAMPO13=${NULO};
		fi

	CAMPO14=${LINHA:266:60};                                     
		if [ -z "$CAMPO14" ]; then
		CAMPO14=${NULO};
		fi

	CAMPO15=${LINHA:326:5};    #    00327  00005  ALFANUM IT-CO-ACAO                                                       
		if [ -z "$CAMPO15" ]; then
		CAMPO15=${NULO};
		fi

	CAMPO16=${LINHA:331:1};    #    00332  00001  ALFANUM IT-IN-CLASSIFICACAO-ECONOMICA                                    
		if [ -z "$CAMPO16" ]; then
		CAMPO16=${NULO};
		fi

	CAMPO17=${LINHA:332:1};    #    00333  00001  ALFANUM IT-IN-PRIORIDADE-LDO          
		if [ -z "$CAMPO17" ]; then
		CAMPO17=${NULO};
		fi

	CAMPO18=${LINHA:333:2};    #    00334  00002  NUM     IT-IN-TIPO-CORRECAO                 
		if [ -z "$CAMPO18" ]; then
		CAMPO18=${NULO};
		fi

	CAMPO19=${LINHA:335:1};    #    00336  00001  NUM     IT-IN-ACOMPANHA-PROGRAMACAO      
		if [ -z "$CAMPO19" ]; then
		CAMPO19=${NULO};
		fi

       REGPLANOINTERNO=`expr $REGPLANOINTERNO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19 >> ${FILE}.PLANOINTERNO.sql;;


        # NOME/TIPO DO REGISTRO : TIPO 19 - TABELA PAIS
        "19")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                      
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)                                                
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:3};    #    00012  00003  NUM     IT-CO-PAIS                	                                       
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:14:45};           #    00015  00045  NUM     IT-NO-PAIS                                                           
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

       REGPAIS=`expr $REGPAIS + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04 >> ${FILE}.PAIS.sql;; 


        # NOME/TIPO DO REGISTRO : TIPO 20 - TABELA MUNICIPIO
        "20")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                      
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)                                                                
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:4};    #    00012  00004  NUM     IT-CO-MUNICIPIO              	                                                    
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:15:45};    #    00016  00045  ALFANUM IT-NO-MUNICIPIO                                                                         
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:60:2};	       #    00061  00002  ALFANUM IT-CO-UF                                                                              
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

       REGMUNICIPIO=`expr $REGMUNICIPIO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.MUNICIPIO.sql;; 


        # NOME/TIPO DO REGISTRO : TIPO 21 - TABELA UF
        "21")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO             	                                                                     
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA) 		                                                                     
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:2};    #    00012  00002  NUM     IT-CO-UF                   	                                                                     
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:13:20};    #    00014  00020  ALFANUM IT-NO-UF                                                                                         
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:33:2};    #    00034  00002  NUM     IT-CO-SUBITEM-CONTABIL     	                                                                     
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:35:14};    #    00036  00014  ALFANUM IT-NU-CGC-UF                                                                                     
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:49:2};    #    00050  00002  ALFANUM IT-CO-UF-BB                                                                                       
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

	CAMPO08=${LINHA:51:2};         #    00052  00002  ALFANUM IT-CO-REGIAO-UF                                                                                  
		if [ -z "$CAMPO08" ]; then
		CAMPO08=${NULO};
		fi

       REGUF=`expr $REGUF + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08 >> ${FILE}.UF.sql;; 


        # NOME/TIPO DO REGISTRO : TIPO 22 - TABELA PROGRAMA PT
        "22")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                                                       
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-OPERACAO (DDMMAAAA)                                                                                                  
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:4};    #    00012  00004  ALFANUM IT-CO-PROGRAMA                                                                                                      
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:15:60};    #    00016  00060  ALFANUM IT-NO-PROGRAMA                                                                                                     
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:75:55};    #    00076  00110  ALFANUM IT-TX-DESCRICAO-PT CAMPO DE 55 BYTES QUE OCORRE 2 VEZES                                                            
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:130:55};                                                               
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

       REGPROGRAMA=`expr $REGPROGRAMA + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.PROGRAMA.sql;; 

	# NOME/TIPO DO REGISTRO : TIPO 23 - TABELA INSTITUICAO EXTERNA
	"23")
	# 00003  00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00012 00005  NUM     IT-NU-INSTITUICAO-EXTERNA
	CAMPO03=${LINHA:11:5}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00017 00019  ALFANUM IT-NO-MNEMONICO-INST-EXTERNA
	CAMPO04=${LINHA:16:19}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00036 00045  ALFANUM IT-NO-INSTITUICAO-EXTERNA
	CAMPO05=${LINHA:35:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	# 00081 00003  NUM     IT-CO-PAIS
	CAMPO06=${LINHA:80:3}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	

	REGINSTITUICAOEXTERNA=`expr $REGINSTITUICAOEXTERNAL + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06 >> ${FILE}.INSTITUICAOEXTERNA.sql;;	


	# NOME/TIPO DO REGISTRO : TIPO 24 - TABELA PROGRAM TRABALHO RESUMIDO
	"24")
	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00012 00005  NUM     GR-UNIDADE-ORCAMENTARIA
	CAMPO03=${LINHA:11:5}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00017 00017  ALFANUM GR-PROGRAMA-TRABALHO-A
	CAMPO04=${LINHA:16:17}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00034 00006  NUM     IT-CO-PROGRAMA-TRABALHO-RESUMIDO
	CAMPO05=${LINHA:33:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	REGPTRESTB=`expr $REGPTRESTB + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.PTRESTB.sql;;
	
	# NOME/TIPO DO REGISTRO : TIPO 25 - TABELA INSCRICAO GENERICA
	"25")
	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00012 00002  ALFANUM IT-CO-INSCRICAO-GENERICA
	CAMPO03=${LINHA:11:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00014 00007  ALFANUM IT-NU-INSCRICAO-GENERICA
	CAMPO04=${LINHA:13:7}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00021 00045  ALFANUM IT-NO-MNEMONICO-INSCRICAO
	CAMPO05=${LINHA:20:45}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	REGINSCRICAOGENERICA=`expr $REGINSCRICAOGENERICA + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05 >> ${FILE}.INSCRICAOGENERICA.sql;;

	# NOME/TIPO DO REGISTRO : TIPO 26 - TABELA PROJ. ATIV. SUBTITULO
	"26")
	# 00003 00001  ALFANUM IT-IN-OPERACAO
	CAMPO01=${LINHA:2:1}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00004 00008  NUM     IT-DA-OPERACAO (DDMMAAAA)
	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00012 00008  ALFANUM GR-PROJETO-ATIVIDADE-SUBTITULO
	CAMPO03=${LINHA:11:8}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00020 00001  ALFANUM IT-IN-PROJ-ATIV-SUBTITULO
	CAMPO04=${LINHA:19:1}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00021 00060  ALFANUM IT-NO-PROJ-ATIV-SUBTITULO
	CAMPO05=${LINHA:20:60}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	# 00081 00110  ALFANUM IT-TX-DESCRICAO-PT CAMPO DE 55 BYTES QUE OCORRE 2 VEZES
	CAMPO06=${LINHA:80:110}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	# 00191 00001  ALFANUM IT-IN-MEIO-FIM
	CAMPO07=${LINHA:190:1}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	REGPROJATIVSUBTITULO=`expr $REGPROJATIVSUBTITULO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07 >> ${FILE}.PROJATIVSUBTITULO.sql;;


        # NOME/TIPO DO REGISTRO : TIPO 27 - TABELA ACAO
        "27")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                                                                                    
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)                                                                                                                              
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:10};    #    00012  00010  ALFANUM GR-ORGAO-ACAO                                                                                                                                   
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:21:45};    #    00022  00045  ALFANUM IT-NO-ACAO                                                                                                                                      
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:66:60};    #    00067  00300  ALFANUM IT-TX-DESCRICAO-ACAO CAMPO DE 60 BYTES QUE OCORRE 5 VEZES                                                                                       
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:126:60};    #
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:186:60};    #                                                                                   
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

	CAMPO08=${LINHA:246:60};    #                                                                                   
		if [ -z "$CAMPO08" ]; then
		CAMPO08=${NULO};
		fi

	CAMPO09=${LINHA:306:60};    #                                                                                   
		if [ -z "$CAMPO09" ]; then
		CAMPO09=${NULO};
		fi

	CAMPO10=${LINHA:366:6};    #    00367  00006  ALFANUM IT-CO-UG-COORDENA-ACAO                                                                                   
		if [ -z "$CAMPO10" ]; then
		CAMPO10=${NULO};
		fi

       REGACAO=`expr $REGACAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10 >> ${FILE}.ACAO.sql;;


        # NOME/TIPO DO REGISTRO : TIPO 28 - TABELA SUBACAO
        "28")
        CAMPO01=${LINHA:2:1};    #    00003  00001  ALFANUM IT-IN-OPERACAO                                                                                                                                             
		if [ -z "$CAMPO01" ]; then
		CAMPO01=${NULO};
		fi

	CAMPO02=${LINHA:  7: 4}${LINHA:  5: 2}${LINHA:  3: 2};    #    00004  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
		if [ -z "$CAMPO02" ]; then
		CAMPO02=${NULO};
		fi

	CAMPO03=${LINHA:11:10};    #    00012  00010  ALFANUM GR-ORGAO-SUBACAO                                                                                                                                         
		if [ -z "$CAMPO03" ]; then
		CAMPO03=${NULO};
		fi

	CAMPO04=${LINHA:21:45};    #    00022  00045  ALFANUM IT-NO-SUBACAO                                                                                                                                            
		if [ -z "$CAMPO04" ]; then
		CAMPO04=${NULO};
		fi

	CAMPO05=${LINHA:66:60};    #    00067  00300  ALFANUM IT-TX-DESCRICAO-SUBACAO CAMPO DE 60 BYTES QUE OCORRE 5 VEZES                                                                                             
		if [ -z "$CAMPO05" ]; then
		CAMPO05=${NULO};
		fi

	CAMPO06=${LINHA:126:60};    #                                                                                                                      
		if [ -z "$CAMPO06" ]; then
		CAMPO06=${NULO};
		fi

	CAMPO07=${LINHA:186:60};    #                                                                                                                       
		if [ -z "$CAMPO07" ]; then
		CAMPO07=${NULO};
		fi

	CAMPO08=${LINHA:246:60};    #                                                                                     
		if [ -z "$CAMPO08" ]; then
		CAMPO08=${NULO};
		fi

	CAMPO09=${LINHA:306:60};    #                                                                                     
		if [ -z "$CAMPO09" ]; then
		CAMPO09=${NULO};
		fi

	CAMPO10=${LINHA:366:6};    #    00367  00006  ALFANUM IT-CO-UG-COORDENA-SUBACAO                                                                                              
		if [ -z "$CAMPO10" ]; then
		CAMPO10=${NULO};
		fi

	CAMPO11=${LINHA:372:5};    #    00373  00005  ALFANUM IT-CO-ACAO-PROP-SUBACAO                                                                                           
		if [ -z "$CAMPO11" ]; then
		CAMPO11=${NULO};
		fi

       REGSUBACAO=`expr $REGSUBACAO + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11 >> ${FILE}.SUBACAO.sql;;

    esac
    
#    if [ ${AUXTIPOREGISTRO}==0 ]; then
#        AUXTIPOREGISTRO=$TIPOREGISTRO;
#    fi
#	if [ ${AUXTIPOREGISTRO}!=${TIPOREGISTRO} ]; then
#        case $AUXTIPOREGISTRO in
#        "01") echo $REGUG " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "02") echo $REGGESTAO " - GESTAO (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "03") echo $REGORGAO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "04") echo $REGUO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "07") echo $REGCATEGORIAGASTO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "08") echo $REGPLANOCONTA " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "10") echo $REGPROGRAMATRABALHO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "13") echo $REGVINCULAPAGAMENTO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "14") echo $REGFONTE " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "15") echo $REGFUNCAO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "16") echo $REGSUBFUNCAO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "18") echo $REGPLANOINTERNO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "19") echo $REGPAIS " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "20") echo $REGMUNICIPIO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "21") echo $REGUF " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "22") echo $REGPROGRAMA " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "24") echo $REGPTRES " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "27") echo $REGACAO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        "28") echo $REGSUBACAO " - UG (" $AUXTIPOREGISTRO ")" >> logTB.txt;;
#        esac
#        ARQUIVO="";
#        AUXTIPOREGISTRO=$TIPOREGISTRO;
#        REG=0;
#    fi
#    REGT=`expr $REGT + 1`;

done

tar -zcf ${LIDOS}${FILE}".tar.gz" ${FILE}

if [ `cat ${FILE}".UG.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".UG.sql";
	echo "Linhas Processadas para UG :"`cat ${FILE}".UG.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".UG.sql.tar.gz" ${FILE}".UG.sql"
	#rm ${FILE}".UG.sql"
fi

if [ `cat ${FILE}".GESTAO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".GESTAO.sql";
	echo "Linhas Processadas para GESTAO :"`cat ${FILE}".GESTAO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".GESTAO.sql.tar.gz" ${FILE}".GESTAO.sql"
	#rm ${FILE}".GESTAO.sql"
fi

if [ `cat ${FILE}".ORGAO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".ORGAO.sql";
	echo "Linhas Processadas para ORGAO :"`cat ${FILE}".ORGAO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".ORGAO.sql.tar.gz" ${FILE}".ORGAO.sql"
	#rm ${FILE}".ORGAO.sql"
fi

if [ `cat ${FILE}".UO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".UO.sql";
	echo "Linhas Processadas para UO :"`cat ${FILE}".UO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".UO.sql.tar.gz" ${FILE}".UO.sql"
	#rm ${FILE}".UO.sql"
fi

if [ `cat ${FILE}".BANCOORIGINAL.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".BANCOORIGINAL.sql";
	echo "Linhas Processadas para BANCOORIGINAL :"`cat ${FILE}".BANCOORIGINAL.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".BANCOORIGINAL.sql.tar.gz" ${FILE}".BANCOORIGINAL.sql"
	#rm ${FILE}".BANCOORIGINAL.sql"
fi

if [ `cat ${FILE}".AGENCIABB.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".AGENCIABB.sql";
	echo "Linhas Processadas para AGENCIABB :"`cat ${FILE}".AGENCIABB.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".AGENCIABB.sql.tar.gz" ${FILE}".AGENCIABB.sql"
	#rm ${FILE}".AGENCIABB.sql"
fi

if [ `cat ${FILE}".CATEGORIAGASTO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".CATEGORIAGASTO.sql";
	echo "Linhas Processadas para CATEGORIAGASTO :"`cat ${FILE}".CATEGORIAGASTO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".CATEGORIAGASTO.sql.tar.gz" ${FILE}".CATEGORIAGASTO.sql"
	#rm ${FILE}".CATEGORIAGASTO.sql"
fi

if [ `cat ${FILE}".PLANOCONTA.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".PLANOCONTA.sql";
	echo "Linhas Processadas para PLANOCONTA :"`cat ${FILE}".PLANOCONTA.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".PLANOCONTA.sql.tar.gz" ${FILE}".PLANOCONTA.sql"
	#rm ${FILE}".PLANOCONTA.sql"
fi

if [ `cat ${FILE}".EVENTO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".EVENTO.sql";
	echo "Linhas Processadas para EVENTO :"`cat ${FILE}".EVENTO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".EVENTO.sql.tar.gz" ${FILE}".EVENTO.sql"
	#rm ${FILE}".EVENTO.sql"
fi

if [ `cat ${FILE}".PROGRAMATRABALHO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".PROGRAMATRABALHO.sql";
	echo "Linhas Processadas para PROGRAMATRABALHO :"`cat ${FILE}".PROGRAMATRABALHO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".PROGRAMATRABALHO.sql.tar.gz" ${FILE}".PROGRAMATRABALHO.sql"
	#rm ${FILE}".PROGRAMATRABALHO.sql"
fi

if [ `cat ${FILE}".CREDOR.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".CREDOR.sql";
	echo "Linhas Processadas para CREDOR :"`cat ${FILE}".CREDOR.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".CREDOR.sql.tar.gz" ${FILE}".CREDOR.sql"
	#rm ${FILE}".CREDOR.sql"
fi

if [ `cat ${FILE}".DESTINACAORECEITA.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".DESTINACAORECEITA.sql";
	echo "Linhas Processadas para DESTINACAORECEITA :"`cat ${FILE}".DESTINACAORECEITA.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".DESTINACAORECEITA.sql.tar.gz" ${FILE}".DESTINACAORECEITA.sql"
	#rm ${FILE}".DESTINACAORECEITA.sql"
fi

if [ `cat ${FILE}".VINCULAPAGAMENTO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".VINCULAPAGAMENTO.sql";
	echo "Linhas Processadas para VINCULAPAGAMENTO :"`cat ${FILE}".VINCULAPAGAMENTO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".VINCULAPAGAMENTO.sql.tar.gz" ${FILE}".VINCULAPAGAMENTO.sql"
	#rm ${FILE}".VINCULAPAGAMENTO.sql"
fi

if [ `cat ${FILE}".FONTERECURSO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".FONTERECURSO.sql";
	echo "Linhas Processadas para FONTERECURSO :"`cat ${FILE}".FONTERECURSO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".FONTERECURSO.sql.tar.gz" ${FILE}".FONTERECURSO.sql"
	#rm ${FILE}".FONTERECURSO.sql"
fi

if [ `cat ${FILE}".FUNCAO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".FUNCAO.sql";
	echo "Linhas Processadas para FUNCAO:"`cat ${FILE}".FUNCAO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".FUNCAO.sql.tar.gz" ${FILE}".FUNCAO.sql"
	#rm ${FILE}".FUNCAO.sql"
fi

if [ `cat ${FILE}".SUBFUNCAO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".SUBFUNCAO.sql";
	echo "Linhas Processadas para SUBFUNCAO :"`cat ${FILE}".SUBFUNCAO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".SUBFUNCAO.sql.tar.gz" ${FILE}".SUBFUNCAO.sql"
	#rm ${FILE}".SUBFUNCAO.sql"
fi

if [ `cat ${FILE}".RECEITAFEDERAL.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".RECEITAFEDERAL.sql";
	echo "Linhas Processadas para RECEITAFEDERAL :"`cat ${FILE}".RECEITAFEDERAL.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".RECEITAFEDERAL.sql.tar.gz" ${FILE}".RECEITAFEDERAL.sql"
	#rm ${FILE}".RECEITAFEDERAL.sql"
fi

if [ `cat ${FILE}".PLANOINTERNO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".PLANOINTERNO.sql";
	echo "Linhas Processadas para PLANOINTERNO :"`cat ${FILE}".PLANOINTERNO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".PLANOINTERNO.sql.tar.gz" ${FILE}".PLANOINTERNO.sql"
	#rm ${FILE}".PLANOINTERNO.sql"
fi

if [ `cat ${FILE}".PAIS.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".PAIS.sql";
	echo "Linhas Processadas para PAIS :"`cat ${FILE}".PAIS.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".PAIS.sql.tar.gz" ${FILE}".PAIS.sql"
	#rm ${FILE}".PAIS.sql"
fi

if [ `cat ${FILE}".MUNICIPIO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".MUNICIPIO.sql";
	echo "Linhas Processadas para MUNICIPIO :"`cat ${FILE}".MUNICIPIO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".MUNICIPIO.sql.tar.gz" ${FILE}".MUNICIPIO.sql"
	#rm ${FILE}".MUNICIPIO.sql"
fi

if [ `cat ${FILE}".UF.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".UF.sql";
	echo "Linhas Processadas para UF :"`cat ${FILE}".UF.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".UF.sql.tar.gz" ${FILE}".UF.sql"
	#rm ${FILE}".UF.sql"
fi

if [ `cat ${FILE}".PROGRAMA.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".PROGRAMA.sql";
	echo "Linhas Processadas para PROGRAMA :"`cat ${FILE}".PROGRAMA.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".PROGRAMA.sql.tar.gz" ${FILE}".PROGRAMA.sql"
	#rm ${FILE}".PROGRAMA.sql"
fi

if [ `cat ${FILE}".INSTITUICAOEXTERNA.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".INSTITUICAOEXTERNA.sql";
	echo "Linhas Processadas para INSTITUICAOEXTERNA :"`cat ${FILE}".INSTITUICAOEXTERNA.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".INSTITUICAOEXTERNA.sql.tar.gz" ${FILE}".INSTITUICAOEXTERNA.sql"
	#rm ${FILE}".INSTITUICAOEXTERNA.sql"
fi

if [ `cat ${FILE}".PTRESTB.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".PTRESTB.sql";
	echo "Linhas Processadas para PTRESTB :"`cat ${FILE}".PTRESTB.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".PTRESTB.sql.tar.gz" ${FILE}".PTRESTB.sql"
	#rm ${FILE}".PTRESTB.sql"
fi

if [ `cat ${FILE}".INSCRICAOGENERICA.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".INSCRICAOGENERICA.sql";
	echo "Linhas Processadas para INSCRICAOGENERICA :"`cat ${FILE}".INSCRICAOGENERICA.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".INSCRICAOGENERICA.sql.tar.gz" ${FILE}".INSCRICAOGENERICA.sql"
	#rm ${FILE}".INSCRICAOGENERICA.sql"
fi

if [ `cat ${FILE}".PROJATIVSUBTITULO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".PROJATIVSUBTITULO.sql";
	echo "Linhas Processadas para PROJATIVSUBTITULO :"`cat ${FILE}".PROJATIVSUBTITULO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".PROJATIVSUBTITULO.sql.tar.gz" ${FILE}".PROJATIVSUBTITULO.sql"
	#rm ${FILE}".PROJATIVSUBTITULO.sql"
fi

if [ `cat ${FILE}".ACAO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".ACAO.sql"
	echo "Linhas Processadas para ACAO :"`cat ${FILE}".ACAO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".ACAO.sql.tar.gz" ${FILE}".ACAO.sql"
	#rm ${FILE}".ACAO.sql"
fi

if [ `cat ${FILE}".SUBACAO.sql" | wc -l`>4 ]; then
	psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".SUBACAO.sql";
	echo "Linhas Processadas para SUBACAO :"`cat ${FILE}".SUBACAO.sql" | wc -l` >> ${FILELOG};
	#tar -zcf ${SQLCOPY}${FILE}".SUBACAO.sql.tar.gz" ${FILE}".SUBACAO.sql"
	#rm ${FILE}".SUBACAO.sql"
fi