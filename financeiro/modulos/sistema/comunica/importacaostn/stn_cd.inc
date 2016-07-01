<?php
/*
 * Criado em 24/04/2007
 * Contem as rotinas de transação relacionadas a DC.
 */


	$Otimizacao_ptres = null;
	$Otimizacao_ptres = array();
	$Processo_Importacao[$script_nome]['CD'] = false;
	$Contador_Registros = 0;
	//Criar String Cabeçalho
	for ($i =0; $i<=12;$i++)
	{
		if ($i<10){ $val = '0'.$i.''; $arquivo = 'arquivo'.$val; } else {$val = $i;$arquivo = 'arquivo'.$val;};
		${$arquivo} = fopen(STN_DIR_TXT_FILES_DESTINO.str_replace('.txt','',$script_nome).'_CD_'.$val.'.sql','a');
		//$sql_cabecalho ="SET client_encoding TO 'LATIN5';\nCOPY particionamento.saldocontabil".$ano_referencia_stn.$val."(slddatatransacao,sldano,orgcod,ungcod,gstcod,sldcccnum,esfcod,unicod,funcod,sfucod,prgcod,acacod,loccod,ptrcod,iducod,grfcod,frscod,frsgrcod,ndpcod,ungcodresponsavel,plicod,concod,tcccod,sldcreditovlr,slddebitovlr) FROM stdin;\n";
		$sql_cabecalho ="SET client_encoding TO 'LATIN5';\nCOPY particionamento.saldocontabil".$ano_referencia_stn.$val."(slddatatransacao,sldano,orgcod,ungcod,gstcod,sldcccnum,esfcod,unicod,ptrcod,iducod,grfcod,frscod,frsgrcod,ndpcod,ungcodresponsavel,plicod,concod,tcccod,sldcreditovlr,slddebitovlr) FROM stdin;\n";
		$gravar = fwrite(${$arquivo},$sql_cabecalho);
	}

	$Contador_Armazenamento = 0;//Variavel para controle da quantidade de registros gravados
	$armazenar = false; //Indica que os arquivos ainda não foram gravados
	$sql_dc_linhas = null; //Variaveis que armazena os sqls
	$sql_dc_linhas =  array();
	$sql_dc_linhas[0] = "";
	$sql_dc_linhas[1] = "";
	$sql_dc_linhas[2] = "";
	$sql_dc_linhas[3] = "";
	$sql_dc_linhas[4] = "";
	$sql_dc_linhas[5] = "";
	$sql_dc_linhas[6] = "";
	$sql_dc_linhas[7] = "";
	$sql_dc_linhas[8] = "";
	$sql_dc_linhas[9] = "";
	$sql_dc_linhas[10] = "";
	$sql_dc_linhas[11] = "";
	$sql_dc_linhas[12] = "";


	$tamanho_atual = 0;
	$proximo =0;
	while ($Navegador_fp< $Total_fp)
	//while( !feof( $handle ) )
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
		if ($tamanho_reg_2 == 8)
		{

			if ($armazenar==false)
			{
				//gravar dados em arquivo especifico
				$gravar = fwrite($arquivo00,$sql_dc_linhas[0]);
				$gravar = fwrite($arquivo01,$sql_dc_linhas[1]);
				$gravar = fwrite($arquivo02,$sql_dc_linhas[2]);
				$gravar = fwrite($arquivo03,$sql_dc_linhas[3]);
				$gravar = fwrite($arquivo04,$sql_dc_linhas[4]);
				$gravar = fwrite($arquivo05,$sql_dc_linhas[5]);
				$gravar = fwrite($arquivo06,$sql_dc_linhas[6]);
				$gravar = fwrite($arquivo07,$sql_dc_linhas[7]);
				$gravar = fwrite($arquivo08,$sql_dc_linhas[8]);
				$gravar = fwrite($arquivo09,$sql_dc_linhas[9]);
				$gravar = fwrite($arquivo10,$sql_dc_linhas[10]);
				$gravar = fwrite($arquivo11,$sql_dc_linhas[11]);
				$gravar = fwrite($arquivo12,$sql_dc_linhas[12]);


				$sql_dc_linhas[0] = "";
				$sql_dc_linhas[1] = "";
				$sql_dc_linhas[2] = "";
				$sql_dc_linhas[3] = "";
				$sql_dc_linhas[4] = "";
				$sql_dc_linhas[5] = "";
				$sql_dc_linhas[6] = "";
				$sql_dc_linhas[7] = "";
				$sql_dc_linhas[8] = "";
				$sql_dc_linhas[9] = "";
				$sql_dc_linhas[10] = "";
				$sql_dc_linhas[11] = "";
				$sql_dc_linhas[12] = "";

				$Contador_Armazenamento = 0;
				$armazenar = true;
			}


			//Fecha arquivos abertos
			for ($i=0; $i<=12;$i++)
			{
				if ($i<10){ $val = '0'.$i.''; $arquivo = 'arquivo'.$val; } else {$val = $i;$arquivo = 'arquivo'.$val;};
				fwrite(${$arquivo},"\\.");
				fclose(${$arquivo});
			}
			$Processo_Importacao[$script_nome]['CD'] = true;


			//Verifica se a quantidade de registros bate
			if ($Contador_Registros != (int)$linha)
			{
				ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
				return $ErroImport[17];
			}
			break; //sai do laço e busca proximo registro


		}

		$Contador_Registros++;
	 	$slddatatransacao =$data_transacao_stn;
		$sldano = format_copy($ano_referencia_stn);
		$orgcod = format_copy(substr($linha,0,5));
		$ungcod = format_copy(substr($linha,10,6));
		$gstcod = format_copy(substr($linha,16,5));
		$sldcccnum = format_copy(substr($linha,21,58));

		//sub dados de sldcccnum
		$esfcod = format_copy(substr($sldcccnum,0,1));
		$unicod = format_copy(substr($sldcccnum,1,5));
		//Dados busca Ptres
		$funcod = format_copy(substr($sldcccnum,6,2));
		$sfucod = format_copy(substr($sldcccnum,8,3));
		$prgcod = format_copy(substr($sldcccnum,11,4));
		$acacod = format_copy(substr($sldcccnum,15,4));
		$loccod = format_copy(substr($sldcccnum,19,4));
		//Busca o ptrescod

		if (is_null($Otimizacao_ptres[$funcod][$sfucod][$prgcod][$acacod][$loccod]))
		{
			$sql = "SELECT ptrcod ".
				" FROM financeiro.ptres ".
				" WHERE funcod = '".$funcod."'".
				" AND sfucod = '".$sfucod."'".
				" AND prgcod = '".$prgcod."'".
				" AND acacod = '".$acacod."'".
				" AND loccod = '".$loccod."'";
				$Otimizacao_ptres[$funcod][$sfucod][$prgcod][$acacod][$loccod] = format_copy($db->pegaUm($sql));
		}

		$ptrcod = $Otimizacao_ptres[$funcod][$sfucod][$prgcod][$acacod][$loccod];
		//$ptrcod = 'busca com $sldcccnum;


		$iducod = format_copy(substr($sldcccnum,23,1));
		$grfcod  = format_copy(substr($sldcccnum,24,1));
		$frscod  = format_copy(substr($sldcccnum,25,2));
		$frsgrcod = format_copy(substr($sldcccnum,27,6));
		$ndpcod  = format_copy(substr($sldcccnum,33,8));
		$ungcodresponsavel  = format_copy(substr($sldcccnum,41,6));
		$plicod  = format_copy(substr($sldcccnum,47,11));

		$concod = format_copy(substr($linha,79,7));
		$tcccod = format_copy(substr($linha,86,2));



		$ini=88;


		for ($mes= 0; $mes<=12;$mes++)
		{
			if (substr($linha,$ini,1) == "-")
			{
				$sldcreditovlr[$mes]=format_copy(substr($linha,$ini+1,17),'DECIMAL');
				$slddebitovlr[$mes]=0;
			}else
			{
				$sldcreditovlr[$mes]=0;
				$slddebitovlr[$mes]=format_copy(substr($linha,$ini+1,17),'DECIMAL');
			}
			$ini = $ini+18;

			$sql_dc_linhas[$mes] = $sql_dc_linhas[$mes]. $slddatatransacao."\t".
									$sldano."\t".
									$orgcod."\t".
									$ungcod."\t".
									$gstcod."\t".
									$sldcccnum."\t".
									$esfcod."\t".
									$unicod."\t".
									/*$funcod."\t".
									$sfucod."\t".
									$prgcod."\t".
									$acacod."\t".
									$loccod."\t".*/
									$ptrcod."\t".
									$iducod."\t".
									$grfcod."\t".
									$frscod."\t".
									$frsgrcod."\t".
									$ndpcod ."\t".
									$ungcodresponsavel."\t".
									$plicod."\t".
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
				$gravar = fwrite($arquivo00,$sql_dc_linhas[0]);
				$gravar = fwrite($arquivo01,$sql_dc_linhas[1]);
				$gravar = fwrite($arquivo02,$sql_dc_linhas[2]);
				$gravar = fwrite($arquivo03,$sql_dc_linhas[3]);
				$gravar = fwrite($arquivo04,$sql_dc_linhas[4]);
				$gravar = fwrite($arquivo05,$sql_dc_linhas[5]);
				$gravar = fwrite($arquivo06,$sql_dc_linhas[6]);
				$gravar = fwrite($arquivo07,$sql_dc_linhas[7]);
				$gravar = fwrite($arquivo08,$sql_dc_linhas[8]);
				$gravar = fwrite($arquivo09,$sql_dc_linhas[9]);
				$gravar = fwrite($arquivo10,$sql_dc_linhas[10]);
				$gravar = fwrite($arquivo11,$sql_dc_linhas[11]);
				$gravar = fwrite($arquivo12,$sql_dc_linhas[12]);


				$sql_dc_linhas[0] = "";
				$sql_dc_linhas[1] = "";
				$sql_dc_linhas[2] = "";
				$sql_dc_linhas[3] = "";
				$sql_dc_linhas[4] = "";
				$sql_dc_linhas[5] = "";
				$sql_dc_linhas[6] = "";
				$sql_dc_linhas[7] = "";
				$sql_dc_linhas[8] = "";
				$sql_dc_linhas[9] = "";
				$sql_dc_linhas[10] = "";
				$sql_dc_linhas[11] = "";
				$sql_dc_linhas[12] = "";

				$Contador_Armazenamento = 0;
				$armazenar = true;

		}



	}


	//Indica que o arquivo nao encontrou o trailer do arquivo
	if ($Processo_Importacao[$script_nome]['CD'] == false)
	{
		ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
		return $ErroImport[17];
	}




?>
