<?php
/*
 * Criado em 24/04/2007
 * Contem as rotinas de transação relacionadas a SC.
 */

/*
 * CONSTANTE	PARTE	TABELA	INICIO	FINAL	TAMANHO	FORMATO	CONTEUDO/OBSERVAÇÃO	OBSERVACOES	DIVISOES	TAMANHOS
 * NE	HEADER	NE	1	2	2	ALFANUM	CONSTANTE	CONSTANTE = NE
 * NE	HEADER	NE	3	10	8	NUM	DATA INICIO DO MOVIMENTO	(DDMMAAAA)
 * NE	HEADER	NE	11	18	8	NUM	DATA FIM DO MOVIMENTO	(DDMMAAAA)
 * NE	HEADER	NE	19	1667	1649	ALFANUM	FILLER
 * NE	DETALHE	NE	1	11	11	NUM	CPF DO USUARIO
 * NE	DETALHE	NE	12	19	8	ALFANUM	TERMINAL DO USUARIO
 * NE	DETALHE	NE	20	27	8	NUM	DATA DE TRANSACAO	(DDMMAAAA)
 * NE	DETALHE	NE	28	31	4	NUM	HORA TRANSACAO	(HHMM)
 * NE	DETALHE	NE	32	37	6	NUM	CODIGO DA UG DO OPERADOR
 * NE	DETALHE	NE	38	60	23	ALFANUM	NUMERO DA NE
 * NE	DETALHE	NE	61	72	12	ALFANUM	NUMERO DO DOC REFERENCIA
 * NE	DETALHE	NE	73	80	8	NUM	DATA DE EMISSAO	(DDMMAAAA)
 * NE	DETALHE	NE	81	81	1	NUM	TIPO DO FAVORECIDO
 * NE	DETALHE	NE	82	95	14	ALFANUM	CODIGO DO FAVORECIDO
 * NE	DETALHE	NE	96	329	234	ALFANUM	OBSERVACAO
 * NE	DETALHE	NE	330	335	6	NUM	CODIGO DO EVENTO
 * NE	DETALHE	NE	336	336	1	NUM	ESFERA ORCAMENTARIA
 * NE	DETALHE	NE	337	342	6	NUM	PROGRAMA DE TRABALHO RESUMIDO - PTRES
 * NE	DETALHE	NE	343	352	10	NUM	FONTE DE RECURSO
 * NE	DETALHE	NE	353	358	6	NUM	NATUREZA DE DESPESA
 * NE	DETALHE	NE	359	364	6	NUM	CODIGO DA UG RESPONSAVEL
 * NE	DETALHE	NE	365	375	11	ALFANUM	PLANO INTERNO
 * NE	DETALHE	NE	376	392	17	NUM	VALOR DA TRANSACAO	(N15,2)
 * NE	DETALHE	NE	393	394	2	NUM	MODALIDADE DE LICITACAO
 * NE	DETALHE	NE	395	395	1	NUM	TIPO DE NE
 * NE	DETALHE	NE	396	415	20	ALFANUM	REFERENCIA DA DISPENSA
 * NE	DETALHE	NE	416	416	1	NUM	ORIGEM MATERIAL
 * NE	DETALHE	NE	417	436	20	ALFANUM	NUMERO DO PROCESSO
 * NE	DETALHE	NE	437	438	2	ALFANUM	UF BENEFICIADA
 * NE	DETALHE	NE	439	442	4	NUM	MUNICIPIO BENEFICIADO
 * NE	DETALHE	NE	443	444	2	ALFANUM	INCISO
 * NE	DETALHE	NE	445	452	8	ALFANUM	AMPARO LEGAL
 * NE	DETALHE	NE	453	458	6	NUM	CODIGO DA UG DO DOC. REFERENCIA
 * NE	DETALHE	NE	459	463	5	NUM	CODIGO DA GESTAO DO DOC. REFERENCIA
 * NE	DETALHE	NE	464	464	1	NUM	CONTROLE DE EMISSAO
 * NE	DETALHE	NE	465	466	2	NUM	MES DE LANCAMENTO
 * NE	DETALHE	NE	467	476	10	ALFANUM	SISTEMA DE ORIGEM
 * NE	DETALHE	NE	477	477	1	NUM	INDENTIFICADOR DE NE DE CONTRA ENTREGA
 * NE	DETALHE	NE	478	479	2	NUM	SITUACAO DO CREDOR NO SICAF
 * NE	DETALHE	NE	480	767	288	NUM	DATA DE VENCIMENTO	(DDMMAAAA) OCORRE 36 VEZES TAMANHO DE CADA CAMPO 8 BYTES	36	8
 * NE	DETALHE	NE	768	1055	288	NUM	DATA DE PAGAMENTO	(DDMMAAAA) OCORRE 36 VEZES TAMANHO DE CADA CAMPO 8 BYTES	36	8
 * NE	DETALHE	NE	1056	1667	612	NUM	VALOR CRONOGRAMADO	 OCORRE 36 VEZES TAMANHO DE CADA CAMPO 17 (N15,2)	36	17
 * NE	TRAILER	NE	1	2	2	ALFANUM	CONSTANTE	CONSTANTE = NE
 * NE	TRAILER	NE	3	10	8	NUM	QUANTIDADE DE REGISTRO
 * NE	TRAILER	NE	11	1667	1657	ALFANUM	FILLER
 *
 */
 	$Processo_Importacao[$script_nome]['NE'] = false;
	$Contador_Registros = 0;
	//Abre Arquivo de Script com Cabeçalho
	$arquivo00 = fopen(STN_DIR_TXT_FILES_DESTINO.str_replace('.txt','',$script_nome).'_NE_'.$val.'.sql','a');
	$sql_cabecalho ="SET client_encoding TO 'LATIN5';\n";
	$gravar = fwrite($arquivo00,$sql_cabecalho);



	$Contador_Armazenamento = 0;//Variavel para controle da quantidade de registros gravados
	$armazenar = false; //Indica que os arquivos ainda não foram gravados
	$sql_ne_linhas = null; //Variaveis que armazena os sqls
	$sql_ne_linhas = "";



	$tamanho_atual = 0;
	$proximo =0;
	//while( !feof( $handle ) )
	while ($Navegador_fp< $Total_fp)
	{

		//$linha = fgets($handle);
		$linha = $fp[$Navegador_fp];
		$tamanho_reg = strlen($linha);
		$tamanho_reg_2 = strlen(trim($linha));
		$tamanho_atual =$tamanho_atual + $tamanho_reg;
		$proximo = muda_barra ($tamanho_arquivo,$tamanho_atual,$proximo);
		$linha = trim($linha);
		$Navegador_fp++;


		if ($linha == "") continue;


		//Fecha Arquivo de log de acordo as condições quando encontra um trailer
		//Tamanho de 10 caracters + header NE
		if (($tamanho_reg_2 == 10)and (substr($linha,0,2)=='NE'))
		{



			if ($armazenar==false)
			{
				//gravar dados em arquivo especifico
				$gravar = fwrite($arquivo00,$sql_ne_linhas);
				$sql_ne_linhas = "";
				$Contador_Armazenamento = 0;
				$armazenar = true;
			}

			//Fecha Arquivo
			fclose($arquivo00);

			$Processo_Importacao[$script_nome]['NE'] = true;

			if ($Contador_Registros != ((int)substr($linha,2,8)))
			{

				ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
				return $ErroImport[17];

			}

			break; //sai do laço e busca proximo registro

		}

		$Contador_Registros++;
	 	$slddatatransacao =format_sql(substr($linha,19,4),'DATA');
		$ungcod = format_sql(substr($linha,31,6));
		$gstcod = format_sql(substr($linha,43,5));
		$noeano = format_sql(substr($linha,19,4));;
		$noenum = format_sql(substr($linha,54,6));
		$noedocreferenciaano = format_sql('');
		$noedocreferencia = format_sql(substr($linha,60,12));
		$noedataemissao = format_sql(substr($linha,72,8),'DATA');
		$idpnumfavorecido = format_sql(substr($linha,81,14));
		$noeobs = format_sql(substr($linha,95,234));
		$evecod = format_sql(substr($linha,329,6));
		$esfcod = format_sql(substr($linha,335,1));
		$ptrcod = format_sql(substr($linha,336,6));
		$iducod = format_sql(substr($linha,342,1));//1
		$grfcod = format_sql(substr($linha,343,1));//1
		$frscod = format_sql(substr($linha,344,2));//?2
		$frsgrcod = format_sql(substr($linha,346,6));//6
		$ndpcod = format_sql(substr($linha,352,6));
		$ungcodresponsavel = format_sql(substr($linha,358,6));
		$noetransacaovlr = format_sql(substr($linha,375,17),'DECIMAL');
		$muncod = format_sql(substr($linha,438,4));
		$noeincisonum = format_sql(substr($linha,442,2));
		$noeamparolegalnum = format_sql(substr($linha,444,8));
		$ungcoddocreferencia = format_sql(substr($linha,452,6));
		$gstcoddocumentoreferencia = format_sql(substr($linha,458,5));
		$noecronogramavlr = format_sql('');//?
		$noedatavencimento = format_sql('');//?
		$noedatapagamento = format_sql('');//?
		$noecronogramadovlr = format_sql('');//?
		$noeprecatorionum = format_sql('');//
		$noepagprecatoriosit = format_sql('');//
		$noenumoriginal = format_sql(substr($linha,37,23));//noenum
		$noedataatualizacao = format_sql('');//de
		$noeliquidacaosit = format_sql('');//de
		$noeoriginalnum = format_sql('');//apagar


			//verifica se ja foi feita a busca deste codigo
		if (($noenumoriginal != "''")||($noenumoriginal != 'NULL'))
		{

			$testebusca = false;
			$BancoImportacao = $db->link;
			$busca_noe = "Select noenumoriginal from financeiro.notaempenho  where noenumoriginal = ".$noenumoriginal.";";
			if(($RS = pg_query($BancoImportacao,$busca_noe)) && (pg_num_rows($RS)>=1)) {
						//$testebusca = $db->pegaUm($busca_noe);
				$testebusca = true;
			}

		}else
		{
			$testebusca = false;
		}

		if ($testebusca == false )
			{

				$sql_ne_linhas = $sql_ne_linhas." insert into financeiro.notaempenho ("
						."ungcod"
						.",gstcod"
						.",noeano"
						.",noenum"
						.",noedocreferenciaano"
						.",noedocreferencia"
						.",noedataemissao"
						.",idpnumfavorecido"
						.",noeobs"
						.",evecod"
						.",esfcod"
						.",ptrcod"
						.",iducod"
						.",grfcod"
						.",frscod"
						.",frsgrcod"
						.",ndpcod"
						.",ungcodresponsavel"
						.",noetransacaovlr"
						.",muncod"
						.",noeincisonum"
						.",noeamparolegalnum"
						.",ungcoddocreferencia"
						.",gstcoddocumentoreferencia"
						.",noecronogramavlr"
						.",noedatavencimento"
						.",noedatapagamento"
						.",noecronogramadovlr"
						.",noeprecatorionum"
						.",noepagprecatoriosit"
						.",noenumoriginal"
						.",noedataatualizacao"
						.",noeliquidacaosit"
						.",noeoriginalnum"
						.",noestatus"
						.") values(".
						 $ungcod
						.",".$gstcod
						.",".$noeano
						.",".$noenum
						.",".$noedocreferenciaano
						.",".$noedocreferencia
						.",".$noedataemissao
						.",".$idpnumfavorecido
						.",".$noeobs
						.",".$evecod
						.",".$esfcod
						.",".$ptrcod
						.",".$iducod
						.",".$grfcod
						.",".$frscod
						.",".$frsgrcod
						.",".$ndpcod
						.",".$ungcodresponsavel
						.",".$noetransacaovlr
						.",".$muncod
						.",".$noeincisonum
						.",".$noeamparolegalnum
						.",".$ungcoddocreferencia
						.",".$gstcoddocumentoreferencia
						.",".$noecronogramavlr
						.",".$noedatavencimento
						.",".$noedatapagamento
						.",".$noecronogramadovlr
						.",".$noeprecatorionum
						.",".$noepagprecatoriosit
						.",".$noenumoriginal
						.",".$noedataatualizacao
						.",".$noeliquidacaosit
						.",".$noeoriginalnum
						.",'N' );\n";

			}else{
				$sql_ne_linhas = $sql_ne_linhas."update financeiro.notaempenho set " .
								" ungcod = ". $ungcod
							.", gstcod = ".$gstcod
							.", noeano = ".$noeano
							.", noenum = ".$noenum
							.", noedocreferenciaano = ". $noedocreferenciaano
							.", noedocreferencia = ". $noedocreferencia
							.", noedataemissao = ". $noedataemissao
							.", idpnumfavorecido = ". $idpnumfavorecido
							.", noeobs = ". $noeobs
							.", evecod = ". $evecod
							.", esfcod = ". $esfcod
							.", ptrcod = ". $ptrcod
							.", iducod = ". $iducod
							.", grfcod = ". $grfcod
							.", frscod = ". $frscod
							.", frsgrcod = ". $frsgrcod
							.", ndpcod = ". $ndpcod
							.", ungcodresponsavel = ". $ungcodresponsavel
							.", noetransacaovlr = ". $noetransacaovlr
							.", muncod = ". $muncod
							.", noeincisonum = ". $noeincisonum
							.", noeamparolegalnum = ". $noeamparolegalnum
							.", ungcoddocreferencia = ". $ungcoddocreferencia
							.", gstcoddocumentoreferencia = ". $gstcoddocumentoreferencia
							.", noecronogramavlr = ". $noecronogramavlr
							.", noedatavencimento = ". $noedatavencimento
							.", noedatapagamento = ". $noedatapagamento
							.", noecronogramadovlr = ". $noecronogramadovlr
							.", noeprecatorionum = ". $noeprecatorionum
							.", noepagprecatoriosit = ". $noepagprecatoriosit
							.", noenumoriginal = ". $noenumoriginal
							.", noedataatualizacao = ". $noedataatualizacao
							.", noeliquidacaosit = ". $noeliquidacaosit
							.", noeoriginalnum = ". $noeoriginalnum
							.", noestatus = 'N' "
							." where noenumoriginal = ".$noenumoriginal.";\n";


			}

		$Contador_Armazenamento ++;
		$armazenar = false;

		if ($Contador_Armazenamento > 50)
		{
				//gravar dados em arquivo especifico
				$gravar = fwrite($arquivo00,$sql_ne_linhas);


				$sql_ne_linhas = "";


				$Contador_Armazenamento = 0;
				$armazenar = true;

		}



	}


	//Indica que o arquivo nao encontrou o trailer do arquivo
	if ($Processo_Importacao[$script_nome]['NE'] == false)
	{
		ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
		return $ErroImport[17];
	}




?>
