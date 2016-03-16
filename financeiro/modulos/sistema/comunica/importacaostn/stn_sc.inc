<?php
/*
 * Criado em 24/04/2007
 * Contem as rotinas de transação relacionadas a SC.
 */


 	//Contas atualmente trabalhadas e seu tipo de conta

 	$contas['112160400']=45;
	$contas['112161400']=12;
	$contas['191140000']=72;
	$contas['192110101']=16;
	$contas['192110201']=16;
	$contas['192110209']=16;
	$contas['192110301']=16;
	$contas['192110303']=16;
	$contas['192120100']=16;
	$contas['192120200']=16;
	$contas['192120300']=16;
	$contas['192130100']=16;
	$contas['192130101']=16;
	$contas['192130102']=16;
	$contas['192130200']=16;
	$contas['192130201']=16;
	$contas['192140100']=16;
	$contas['192140200']=16;
	$contas['192190101']=16;
	$contas['192190109']=16;
	$contas['192190201']=16;
	$contas['192190209']=16;
	$contas['192190301']=16;
	$contas['192190302']=16;
	$contas['192210101']=16;
	$contas['192210102']=16;
	$contas['192210201']=16;
	$contas['192210202']=16;
	$contas['192210901']=16;
	$contas['192210909']=16;
	$contas['192220100']=16;
	$contas['192220200']=16;
	$contas['192220300']=16;
	$contas['192220901']=16;
	$contas['192220909']=16;
	$contas['192230000']=16;
	$contas['192240000']=16;
	$contas['192290100']=16;
	$contas['192290200']=16;
	$contas['192410101']=26;
	$contas['192410102']=26;
	$contas['192410103']=26;
	$contas['192410106']=16;
	$contas['192410109']=26;
	$contas['193110303']=50;
	$contas['193110304']=50;
	$contas['193110306']=50;
	$contas['193110307']=50;
	$contas['193110308']=50;
	$contas['193110316']=50;
	$contas['193110317']=50;
	$contas['193110318']=50;
	$contas['193210701']=52;
	$contas['195100000']=26;
	$contas['195300000']=26;
	$contas['195400000']=26;
	$contas['195910000']=26;
	$contas['195920000']=26;
	$contas['292110000']=16;
	$contas['292120101']=16;
	$contas['292120102']=16;
	$contas['292120103']=16;
	$contas['292120104']=16;
	$contas['292120201']=16;
	$contas['292120202']=16;
	$contas['292120500']=16;
	$contas['292129900']=16;
	$contas['292130100']=16;
	$contas['292130201']=31;
	$contas['292130209']=31;
	$contas['292140200']=16;
	$contas['292140300']=16;
	$contas['292210101']=16;
	$contas['292210201']=16;
	$contas['292210901']=16;
	$contas['292210909']=16;
	$contas['292220100']=16;
	$contas['292220200']=16;
	$contas['292220300']=16;
	$contas['292220901']=16;
	$contas['292220909']=16;
	$contas['292230000']=16;
	$contas['292240000']=16;
	$contas['292290100']=16;
	$contas['292290200']=16;
	$contas['292410101']=26;
	$contas['292410102']=26;
	$contas['292410402']=26;
	$contas['292410403']=26;
	$contas['292410508']=26;
	$contas['292410510']=26;
	$contas['292410590']=26;
	$contas['292410591']=26;
	$contas['292440100']=40;
	$contas['292440200']=40;
	$contas['293110104']=43;
	$contas['293110117']=43;
	$contas['293110118']=43;
	$contas['293110119']=43;
	$contas['293110303']=50;
	$contas['293110304']=50;
	$contas['293110306']=50;
	$contas['293110307']=50;
	$contas['293110308']=50;
	$contas['293110309']=50;
	$contas['293110601']=17;
	$contas['293110699']=17;
	$contas['295100000']=26;
	$contas['295200000']=26;
	$contas['295300000']=26;
	$contas['295400000']=26;
	$contas['295600000']=26;
	$contas['512120000']=12;
	$contas['612120000']=12;
	$contas['293110317']=50;
	$contas['293110603']=17;
	$contas['293110602']=17;
	$contas['293110216']=43;
	$contas['293110316']=50;
	$contas['293110214']=43;
	$contas['293110204']=43;
	$contas['293110207']=43;
	$contas['292120105']=16;
	$contas['191210100']=28;
	$contas['293110604']=17;
	$contas['293110605']=17;
	$contas['293110606']=17;
	$contas['293110607']=17;
	$contas['293110608']=17;
	$contas['293110609']=17;


	$Processo_Importacao[$script_nome]['SC'] = false;
	$Contador_Registros = 0;
	//Criar String Cabeçalho
	for ($i =0; $i<=13;$i++)
	{
		if ($i<10){ $val = '0'.$i.''; $arquivo = 'arquivo'.$val; } else {$val = $i;$arquivo = 'arquivo'.$val;};
		${$arquivo} = fopen(STN_DIR_TXT_FILES_DESTINO.str_replace('.txt','',$script_nome).'_SC_'.$val.'.sql','a');
		$sql_cabecalho ="SET client_encoding TO 'LATIN5';\nCOPY particionamento.saldocontabil".$ano_referencia_stn.$val."(slddatatransacao,sldmes,sldano,orgcod,ungcod,gstcod,sldcccnum,esfcod,unicod,ptrcod,iducod,grfcod,frscod,frsgrcod,ndpcod,ungcodresponsavel,plicod,noenumoriginal,npenumoriginal,vincod,irpcod,tarcod,concod,tcccod,sldcreditovlr,slddebitovlr) FROM stdin;\n";
		$gravar = fwrite(${$arquivo},$sql_cabecalho);
	}
	/*//Arquivo de Saldo
	$arquivo_cambio = fopen(STN_DIR_TXT_FILES_DESTINO.str_replace('.txt','',$script_nome).'_CAMBIO_'.$val.'.sql','a');
	$sql_cabecalho ="SET client_encoding TO 'LATIN5';\nCOPY financeiro.cambio(slddatatransacao,sldano,orgcod,ungcod,gstcod,sldcccnum,esfcod,unicod,funcod,sfucod,prgcod,acacod,loccod,ptrcod,iducod,grfcod,frscod,frsgrcod,ndpcod,ungcodresponsavel,plicod,noenumoriginal,npenumoriginal,vincod,irpcod,tarcod,concod,tcccod,sldcreditovlr,slddebitovlr) FROM stdin;\n";
	$gravar = fwrite(${$arquivo},$sql_cabecalho);*/


	$Contador_Armazenamento = 0;//Variavel para controle da quantidade de registros gravados
	$armazenar = false; //Indica que os arquivos ainda não foram gravados
	$sql_sc_linhas = null; //Variaveis que armazena os sqls
	$sql_sc_linhas =  array();
	$sql_sc_linhas[0] = "";
	$sql_sc_linhas[1] = "";
	$sql_sc_linhas[2] = "";
	$sql_sc_linhas[3] = "";
	$sql_sc_linhas[4] = "";
	$sql_sc_linhas[5] = "";
	$sql_sc_linhas[6] = "";
	$sql_sc_linhas[7] = "";
	$sql_sc_linhas[8] = "";
	$sql_sc_linhas[9] = "";
	$sql_sc_linhas[10] = "";
	$sql_sc_linhas[11] = "";
	$sql_sc_linhas[12] = "";
	$sql_sc_linhas[13] = "";


	$tamanho_atual = 0;
	$proximo =0;
	//while( !feof( $handle ) )
	while ($Navegador_fp< $Total_fp)
	//while ($Navegador_fp <=5)
	{

		//$linha = fgets($handle);
		$linha = $fp[$Navegador_fp];
		$tamanho_reg = strlen($linha);
		$tamanho_reg_2 = strlen(trim($linha));
		$tamanho_atual =$tamanho_atual + $tamanho_reg;
		$proximo = muda_barra ($tamanho_arquivo,$tamanho_atual,$proximo);
		$linha = trim($linha);
		$Navegador_fp++;
		
		//Fecha Arquivo de log de acordo as condições quando encontra um trailer
		//Tamanho de 8 caracters + header CD
		if (($tamanho_reg_2 == 10)and (substr($linha,0,2)=='SC'))
		{

			if ($armazenar==false)
			{
				//gravar dados em arquivo especifico
				$gravar = fwrite($arquivo00,$sql_sc_linhas[0]);
				$gravar = fwrite($arquivo01,$sql_sc_linhas[1]);
				$gravar = fwrite($arquivo02,$sql_sc_linhas[2]);
				$gravar = fwrite($arquivo03,$sql_sc_linhas[3]);
				$gravar = fwrite($arquivo04,$sql_sc_linhas[4]);
				$gravar = fwrite($arquivo05,$sql_sc_linhas[5]);
				$gravar = fwrite($arquivo06,$sql_sc_linhas[6]);
				$gravar = fwrite($arquivo07,$sql_sc_linhas[7]);
				$gravar = fwrite($arquivo08,$sql_sc_linhas[8]);
				$gravar = fwrite($arquivo09,$sql_sc_linhas[9]);
				$gravar = fwrite($arquivo10,$sql_sc_linhas[10]);
				$gravar = fwrite($arquivo11,$sql_sc_linhas[11]);
				$gravar = fwrite($arquivo12,$sql_sc_linhas[12]);
				$gravar = fwrite($arquivo13,$sql_sc_linhas[13]);


				$sql_sc_linhas[0] = "";
				$sql_sc_linhas[1] = "";
				$sql_sc_linhas[2] = "";
				$sql_sc_linhas[3] = "";
				$sql_sc_linhas[4] = "";
				$sql_sc_linhas[5] = "";
				$sql_sc_linhas[6] = "";
				$sql_sc_linhas[7] = "";
				$sql_sc_linhas[8] = "";
				$sql_sc_linhas[9] = "";
				$sql_sc_linhas[10] = "";
				$sql_sc_linhas[11] = "";
				$sql_sc_linhas[12] = "";
				$sql_sc_linhas[13] = "";

				$Contador_Armazenamento = 0;
				$armazenar = true;
			}


			//Fecha arquivos abertos
			for ($i=0; $i<=13;$i++)
			{
				if ($i<10){ $val = '0'.$i.''; $arquivo = 'arquivo'.$val; } else {$val = $i;$arquivo = 'arquivo'.$val;};
				//fwrite(${$arquivo},"\\.");
				fwrite(${$arquivo},"");
				fclose(${$arquivo});
			}
			$Processo_Importacao[$script_nome]['SC'] = true;

			//Verifica se a quantidade de registros bate
			if ($Contador_Registros != (int)substr($linha,2,8))
			{
				ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
				return $ErroImport[17];
			}

			break; //sai do laço e busca proximo registro

		}

		$Contador_Registros++;
		$GR_CODIGO_CONTA = format_copy(substr($linha,13,9));
		//if (is_null($contas[$GR_CODIGO_CONTA])) continue; //Sai do loop caso a conta não for trabalhada


	 	$slddatatransacao = format_copy(substr($linha,605,8),'DATA',substr($linha,613,8));
		$sldmes = format_copy(substr($linha,607,2));
	 	$sldano = format_copy(substr($linha,609,4));
		$ungcod = format_copy(substr($linha,2,6));
		$gstcod = format_copy(substr($linha,8,5));
		$concod = format_copy(substr($linha,13,7));
		$tcccod = format_copy(substr($linha,20,2));
		//$sldcccnum = format_copy(substr($linha,13,43));
		$sldcccnum = format_copy(substr($linha,22,43));


		switch ($contas[$GR_CODIGO_CONTA])
		{
			case 12:
				/*
				 * FONTE DE RECURSOS
				 */
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy($sldcccnum);
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;
			case 16:
				/*
				 *INDICA A ESTRUTURA COMBINADA DOS CODIGOS DE ESFERA   ORCAMENTA-
				 *RIA (EO), PROGRAMA DE TRABALHO RESUMIDO (PTRES), FONTE DE RECURSOS (FR), NATU-
				 *REZA DA DESPESA (ND), UNIDADE GESTORA RESPONSAVEL (UGR)  E PLANO INTERNO (PI),
				 *NO FORMATO X+XXXXXX+XXXXXXXXXX+XXXXXX+XXXXXX+XXXXXXXXXXX, ONDE:
				 *- X: 1 DIGITO NUMERICO QUE IDENTIFICA A ESFERA ORCAMENTARIA;
				 *- XXXXXX: 6 DIGITOS NUMERICOS QUE IDENTIFICAM O PROGRAMA DE TRABALHO RESUMIDO;
				 *- XXXXXXXXXX: 10 DIGITOS QUE INDIVIDUALIZAM A FONTE DE RECURSOS;
				 *- XXXXXX: 6 DIGITOS NUMERICOS QUE IDENTIFICAM A NATUREZA DA DESPESA;
				 *- XXXXXX: 6 DIGITOS NUMERICOS QUE IDENTIFICAM  A  UNIDADE GESTORA RESPONSAVEL;
				 *- XXXXXXXXXXX: ATE 11 DIGITOS ALFANUMERICOS  QUE IDENTIFICAM  O PLANO INTERNO,
				 *SO COMPONDO A ESTRUTURA SE O ORGAO UTILIZA-LO.
				 */
				$orgcod = format_copy('');
				$esfcod = format_copy(substr($sldcccnum,0,1));
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy(substr($sldcccnum,1,6));
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy(substr($sldcccnum,7,10));
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy(substr($sldcccnum,17,6));
				$ungcodresponsavel  = format_copy(substr($sldcccnum,23,6));
				$plicod  = format_copy(substr($sldcccnum,29,11));
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 17:
				/*
				 * PERMITIR A ADMINISTRACAO DE LIMITE FINANCEIRO PARA  A  EXECUCAO
				 *ORCAMENTARIA PELAS UNIDADES GESTORAS EXECUTORAS, VINCULADA A EVENTOS  DE  INS-
				 *CRICAO DO DETALHAMENTO 11, 21, 22 E 33.  SERA COMPOSTA  DE  DOIS DIGITOS  ALFA
				 *SENDO:
				 *1 - PRIMEIRA POSICAO - IDENTIFICA O TIPO DE DESPESA, ONDE:
				 *    C = CORRENTE (GD = 3)
				 *    I = INVESTIMENTO/INVERSOES (GD = 4 E 5)
				 *    R = PROCESSO DE INSCRICAO DE EMPENHOS EM RESTOS A PAGAR
				 *    P = PROJETO PILOTO (INDICADOR DE RESULTADO = 3)
				 *    O = DESPESAS OBRIGATORIAIS (INDICADOR DE RESULTADO =1 E INDICADOR DE EXCE-
				 *        CAO AO DECRETO = N)
				 *    E = PROJETO ESTRATEGICO (INDICADOR DE PROJETO ESTRATEGICO = S)
				 *2 ­ SEGUNDA POSICAO  ­ REPRESENTADA PELAS LETRAS A, B OU C NA RESPECTIVA TRAN­
				 *    SACAO DE GRUPO DE FONTE (CONGRFONTE)
				 */
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy('');
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 26:
			/*
			 * NE
			 */
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy('');
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy($sldcccnum);
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 28:
				/*
				 * INDICA O CODIGO CORRESPONDENTE A FONTE DE RECURSOS    INFORMADA
				 * PELA SECRETARIA DE ORCAMENTOS FEDERAIS, NO FORMATO XXXX, ONDE:
				 * - X: INDICADOR DE USO (IDUSO);
				 * - X: ORIGEM;
				 * - XX: FONTE SOF: IDENTIFICA A FONTE DE RECURSOS.
				 * OBS: A FONTE SOF E CORRESPONDENTE AOS QUATRO PRIMEIROS ALGARISMOS DA FONTE  DE
				 * RECURSOS.
				 */
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy(substr($sldcccnum,0,1));
				$grfcod  = format_copy('');
				$frscod  = format_copy(substr($sldcccnum,2,10));
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 31:
			/*
			 * INDICA A ESTRUTURA COMBINADA DOS CODIGOS DE ESFERA ORCAMENTARIA
			 * (EO), PROGRAMA DE TRABALHO RESUMIDO (PTRES), FONTE DE RECURSOS (FR),  NATUREZA
			 * DA DESPESA DETALHADA (ND), UNIDADE GESTORA RESPONSAVEL (UGR) E PLANO   INTERNO
			 * (PI), NO FORMATO X+XXXXXX+XXXXXXXXXX+XXXXXXXX+XXXXXX+XXXXXXXXXXX, ONDE:
			 * - X: 1 DIGITO NUMERICO QUE IDENTIFICA A ESFERA ORCAMENTARIA;
			 * - XXXXXX: 6 DIGITOS NUMERICOS QUE INDIVIDUALIZAM O PROGRAMA DE TRABALHO  RESU-
			 * MIDO;
			 * - XXXXXXXXXX: 10 DIGITOS NUMERICOS QUE IDENTIFICAM A FONTE DE RECURSOS;
			 * - XXXXXXXX: 8 DIGITOS NUMERICOS QUE IDENTIFICAM A NATUREZA DA DESPESA DETALHA-
			 * DA;
			 * - XXXXXX: 6 DIGITOS NUMERICOS QUE IDENTIFICAM  A  UNIDADE GESTORA RESPONSAVEL;
			 * - XXXXXXXXXXX: ATE 11 DIGITOS ALFA OU NUMERICOS QUE IDENTIFICAM O PLANO INTER-
			 * NO, SO COMPOE A ESTRUTURA SE O ORGAO UTILIZA-LO.
			 */
				$orgcod = format_copy('');
				$esfcod = format_copy(substr($sldcccnum,0,1));
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy(substr($sldcccnum,1,6));
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy(substr($sldcccnum,7,10));
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy(substr($sldcccnum,17,8));
				$ungcodresponsavel  = format_copy(substr($sldcccnum,25,6));
				$plicod  = format_copy(substr($sldcccnum,31,11));
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 40:
			/*
			 * INDICA  A ESTRUTURA COMBINADA DOS CODIGOS  DE  UNIDADE GESTORA,
			 * GESTAO  E  NUMERO  DO  PRE-EMPENHO,  NO  FORMATO XXXXXX+XXXXX+XXPEXXXXX, ONDE:
			 * - XXXXXX: 6 DIGITOS NUMERICOS QUE IDENTIFICAM A UNIDADE GESTORA;
			 * - XXXXX: 5 DIGITOS NUMERICOS QUE INDIVIDUALIZAM A GESTAO; E
			 * - XXPEXXXXX: ESTRUTURA COMBINADA DE ANO  DE  COMPETENCIA (XX), CONSTANTE  ALFA
			 * QUE IDENTIFICA O DOCUMENTO PRE-EMPENHO (PE) E NUMERO SEQUENCIAL DE EMISSAO  DO
			 * MESMO (XXXXX).
			 */

				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy('');
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy(substr($sldcccnum,9,10));
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 43:
				/*
				 * INDICA A ESTRUTURA COMBINADA DOS CODIGOS DE ORGAO, FONTE SOF  E
				 * CATEGORIA DE GASTOS, NO FORMATO XXXXX+XXXX+X, ONDE:
				 * - XXXXX: 5 DIGITOS NUMERICOS QUE IDENTIFICAM O ORGAO;
				 * - XXXX: 4 DIGITOS NUMERICOS CORRESPONDENTES A FONTE DE RECURSOS INFORMADA PELA
				 * SOF; E
				 * - X: 1 DIGITO ALFANUMERICO CORRESPONDENTE A CATEGORIA DE GASTOS, CONFORME DIS-
				 * CRIMINADO ABAIXO:
				 * A - PESSOAL;
				 * B - DIVIDA;
				 * C - OUTRAS DESPESAS;
				 * D - OUTROS GASTOS;
				 */
				$orgcod = format_copy(substr($sldcccnum,0,5));
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy('');
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 45:
			/*
			 * Fonte + Vinculação
			 */
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy(substr($sldcccnum,0,10));
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod= format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 50:
			/*
			 *INDICA A ESTRUTURA COMBINADA DOS CODIGOS DE UNIDADE GESTORA(UG)
			 *FONTE DE RECURSO (FR), CATEGORIA DE GASTO, RECURSO, VINCULACAO E MES, NO  FOR-
			 *MATO XXXXXX + XXXXXXXXXX + X + X + XXX + XX, SENDO:
			 *XXXXXX: 6 DIGITOS NUMERICOS QUE IDENTIFICAM A UNIDADE GESTORA;
			 *XXXXXXXXXX: 10 DIGITOS NUMERICOS QUE IDENTIFICAM A FONTE DE RECURSO;
			 *X: 1 DIGITO ALFABETICO QUE IDENTIFICA A CATEGORIA DE GASTO;
			 *X: 1 DIGITO NUMERICO QUE IDENTIFICA O TIPO DE RECURSO;
			 *XXX: 3 DIGITOS NUMERICOS QUE IDENTIFICAM O TIPO DE VINCULACAO; E
			 *XX: 2 DIGITOS NUMERICOS QUE IDENTIFICAM O MES.
			 */
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy(substr($sldcccnum,6,10));
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod = format_copy(substr($sldcccnum,18,3));
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 52:
			/*
			 * conta-corrente tratada pela inscricao 45 de eventos, sendo:
			 * FTE RECURSOS (10) + VINC. PAGAMENTO (3) + TIPO DOCUMENTO (2) + MES SAQUE (2):
			 * TIPO DOCUMENTO - 2 POSICOES NUMERICAS, SENDO:
			 * DOCUMENTO OB ­ BUSCAR 'TIPO DE OB'
			 * DOCUMENTO GR - FORMATAR (91) ; DOCUMENTO NL - FORMATAR (96)
			 * DOCUMENTO NS - FORMATAR (92) ; DOCUMENTO GP - FORMATAR (97)
			 * DOCUMENTO GFIP-FORMATAR (93) ; DOCUMENTO DR - FORMATAR (98)
			 * DOCUMENTO GE - FORMATAR (94) ; OUTROS DOCUMENTOS - FORMATAR (99)
			 * DOCUMENTO DF - FORMATAR (95) ;
			 * MES SAQUE -  POSICOES NUMERICAS, DE ACORDO COM A REGRA:
			 * TP DOC= 10,15,22,24,29,51,52,91,92,95,96,97,98->DATA SAQUE = DATA DE TRANSACAO
			 * TP DOC= 11,12,13,14,16,25,27,53,56 OU 83      ->DATA SAQUE = DIA UTIL SEGUINTE
                                              A DATA DE TRANSACAO;
			 * TP DOC= 55 ->DATA SAQUE = DIA UTIL ANTERIOR A DATA DE TRANSACAO.
			 * OBS: SE DOC. EMITIDO DIA NAO UTIL, DATA TRANSACAO = PRIMEIRO DIA UTIL SEGUINTE
			 */
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy(substr($sldcccnum,0,10));
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod = format_copy(substr($sldcccnum,10,3));
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;

			case 72:
			/*
			 * NATUREZA RECEITA (8) + FONTE RECURSOS (10)+ ESFERA (1) + UO (5) + INDICADOR DE
			 * RESULTADO (1) + TIPO DE ARRECADACAO (1), SENDO:
			 * - NATUREZA RECEITA - BUSCAR DO CAMPO CLASSIFICACAO DO DOCUMENTO;
			 * - FONTE DE RECURSOS - BUSCAR DO CAMPO INSCRICAO DO DOCUMENTO;
			 * - ESFERA ORCAMENTARIA - BUSCAR DA TABELA DE FONTE, A PARTIR DO CODIGO DA FONTE
			 * - UNIDADE ORCAMENTARIA - DA TABELA DE UO OU TABELA DE UG.
			 * - INDICADOR DE RESULTADO - DA TRANSACAO CONRECSOF, A PARTIR DA CLASSIFICACAO
  			 * DA RECEITA.
			 * - TIPO DE ARRECADACAO:
   			 * A) SE DOCUMENTO DARF = 8
   			 * B) SE DOCUMENTO GRU OU RA = 7
   			 * C) DEMAIS DOCUMENTOS ONDE SEJA CONTABILIZADO O C/C 72 = 9
			 */
				$orgcod = format_copy('');
				$esfcod = format_copy(substr($sldcccnum,18,1));
				$unicod = format_copy(substr($sldcccnum,19,5));
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy(substr($sldcccnum,8,10));
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy(substr($sldcccnum,0,8));
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod = format_copy('');
				$irpcod = format_copy(substr($sldcccnum,24,1));
				$tarcod = format_copy(substr($sldcccnum,25,1));
				break;
			default:
				$orgcod = format_copy('');
				$esfcod = format_copy('');
				$unicod = format_copy('');
				/*$funcod = format_copy('');
				$sfucod = format_copy('');
				$prgcod = format_copy('');
				$acacod = format_copy('');
				$loccod = format_copy('');*/
				$ptrcod = format_copy('');
				$iducod = format_copy('');
				$grfcod  = format_copy('');
				$frscod  = format_copy('');
				$frsgrcod = format_copy('');
				$ndpcod  = format_copy('');
				$ungcodresponsavel  = format_copy('');
				$plicod  = format_copy('');
				$noenumoriginal = format_copy('');
				$npenumoriginal = format_copy('');
				$vincod = format_copy('');
				$irpcod = format_copy('');
				$tarcod = format_copy('');
				break;
		}



		$ini=101;


		for ($mes= 0; $mes<=13;$mes++)
		{
			$slddebitovlr[$mes]=format_copy(substr($linha,$ini,18),'DECIMAL');
			$sldcreditovlr[$mes]=format_copy(substr($linha,$ini+252,18),'DECIMAL');
			if ($mes<10) {$sldmes='0'.$mes;} else {$sldmes=''.$mes;};

			$ini = $ini+18;

			$sql_sc_linhas[$mes] = $sql_sc_linhas[$mes].
									$slddatatransacao."\t".
									$sldmes."\t".
									$sldano."\t".
									$orgcod."\t".
									$ungcod."\t".
									$gstcod."\t".
									$sldcccnum."\t".
									$esfcod."\t".
									$unicod."\t".
									$ptrcod."\t".
									$iducod."\t".
									$grfcod."\t".
									$frscod."\t".
									$frsgrcod."\t".
									$ndpcod ."\t".
									$ungcodresponsavel."\t".
									$plicod."\t".
									$noenumoriginal."\t".
									$npenumoriginal."\t".
									$vincod."\t".
									$irpcod."\t".
									$tarcod."\t".
									$concod."\t".
									$tcccod."\t".
									$sldcreditovlr[$mes]."\t".
									$slddebitovlr[$mes]."\n";

		}

		$Contador_Armazenamento ++;
		$armazenar = false;

		if ($Contador_Armazenamento > 50)
		{
		
				//gravar dados em arquivo especifico
				$gravar = fwrite($arquivo00,$sql_sc_linhas[0]);
				$gravar = fwrite($arquivo01,$sql_sc_linhas[1]);
				$gravar = fwrite($arquivo02,$sql_sc_linhas[2]);
				$gravar = fwrite($arquivo03,$sql_sc_linhas[3]);
				$gravar = fwrite($arquivo04,$sql_sc_linhas[4]);
				$gravar = fwrite($arquivo05,$sql_sc_linhas[5]);
				$gravar = fwrite($arquivo06,$sql_sc_linhas[6]);
				$gravar = fwrite($arquivo07,$sql_sc_linhas[7]);
				$gravar = fwrite($arquivo08,$sql_sc_linhas[8]);
				$gravar = fwrite($arquivo09,$sql_sc_linhas[9]);
				$gravar = fwrite($arquivo10,$sql_sc_linhas[10]);
				$gravar = fwrite($arquivo11,$sql_sc_linhas[11]);
				$gravar = fwrite($arquivo12,$sql_sc_linhas[12]);
				$gravar = fwrite($arquivo12,$sql_sc_linhas[13]);


				$sql_sc_linhas[0] = "";
				$sql_sc_linhas[1] = "";
				$sql_sc_linhas[2] = "";
				$sql_sc_linhas[3] = "";
				$sql_sc_linhas[4] = "";
				$sql_sc_linhas[5] = "";
				$sql_sc_linhas[6] = "";
				$sql_sc_linhas[7] = "";
				$sql_sc_linhas[8] = "";
				$sql_sc_linhas[9] = "";
				$sql_sc_linhas[10] = "";
				$sql_sc_linhas[11] = "";
				$sql_sc_linhas[12] = "";
				$sql_sc_linhas[13] = "";

				$Contador_Armazenamento = 0;
				$armazenar = true;

		}



	}


	//Indica que o arquivo nao encontrou o trailer do arquivo
	if ($Processo_Importacao[$script_nome]['SC'] == false)
	{
		ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
		return $ErroImport[17];
	}




?>
