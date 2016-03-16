<?php
/*
 * Criado em 24/04/2007
 * Contem as rotinas de transação relacionadas a TB.
 */


/*
 * CONSTANTE	PARTE	TABELA	INICIO	FINAL	TAMANHO	FORMATO	CONTEUDO/OBSERVAÇÃO	OBSERVACOES	DIVISAO	TAMANHO
 * TB	TIPO	24 - TABELA PROGRAM TRABALHO RESUMIDO	1	2	2	NUM	TIPO DO REGISTRO	 CONSTANTE = 24
 * TB	TIPO	24 - TABELA PROGRAM TRABALHO RESUMIDO	3	3	1	ALFANUM	IT-IN-OPERACAO
 * TB	TIPO	24 - TABELA PROGRAM TRABALHO RESUMIDO	4	11	8	NUM	IT-DA-TRANSACAO	 (DDMMAAAA)
 * TB	TIPO	24 - TABELA PROGRAM TRABALHO RESUMIDO	12	16	5	NUM	GR-UNIDADE-ORCAMENTARIA
 * TB	TIPO	24 - TABELA PROGRAM TRABALHO RESUMIDO	17	33	17	ALFANUM	GR-PROGRAMA-TRABALHO-A
 * TB	TIPO	24 - TABELA PROGRAM TRABALHO RESUMIDO	34	39	6	NUM	IT-CO-PROGRAMA-TRABALHO-RESUMIDO
 * TB	TIPO	24 - TABELA PROGRAM TRABALHO RESUMIDO	40	5000	4961	ALFANUM	FILLER
*/


	//Layouts de Apoio trabalhados
	$Layout_Apoio = array();
	$Layout_Apoio['24'] = '24';


 	$Processo_Importacao[$script_nome]['TB'] = false;
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

	$teste = false;
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


		//pula linha vazia para nao ser contada
		if ($linha == "") continue;


		//Fecha Arquivo de log de acordo as condições quando encontra um trailer
		//Tamanho de 8 caracters + header CD
		if (($tamanho_reg_2 == 10)and (substr($linha,0,2)=='TB'))
		{
			//Fecha Arquivo
			fclose($arquivo00);

			$Processo_Importacao[$script_nome]['TB'] = true;

			//Verifica se a quantidade de registros bate
			if ($Contador_Registros != (int)substr($linha,2,8))
			{
				ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
				return $ErroImport[17];
			}

			break; //sai do laço e busca proximo registro

		}

		$Contador_Registros++;


		//Seleciona quais tabelas de apoio a importar.
		$tipo_apoio = substr($linha,0,2);
		switch ($tipo_apoio)
		{
			case '24':
				ImprimeStatus("formul.txtstatus",'Importando dados de Apoio - Ptres' ,Cor_Executando);

				$ptrcod = format_sql(substr($linha,33,6));
				$ptrano = format_sql(substr($linha,7,4));
				$unicod = format_sql(substr($linha,11,5));
				$ptrprgtrabalho = format_sql(substr($linha,16,17));
				$irpcod = format_sql('');
				$ptrtipocreditonum = format_sql('');
				$funcod = format_sql(substr($linha,16,2));
				$sfucod = format_sql(substr($linha,18,3));
				$prgcod = format_sql(substr($linha,21,4));
				$acacod = format_sql(substr($linha,25,4));
				$loccod = format_sql(substr($linha,29,4));

				//verifica se ja foi feita a busca deste codigo
				if (($ptrcod != "''")||($ptrcod != 'NULL'))
				{

					$testebusca = false;
					$busca_ptr = "Select ptrcod from financeiro.ptres  where ptrcod = ".$ptrcod." and ptrano = ".$ano_referencia.";";
					if(($RS = pg_query($BancoImportacao,$busca_ptr)) && (pg_num_rows($RS)>=1)) {
						//$testebusca = $db->pegaUm($busca_ptr);
						$testebusca = true;
					}
				}else
				{
					$testebusca = false;
				}

				if ($testebusca == false )
				{
					$sql_ptr = " insert into financeiro.ptres ("
						."ptrcod"
						.",ptrano"
						.",unicod"
						.",ptrprgtrabalho"
						.",irpcod"
						.",ptrtipocreditonum"
						.",funcod"
						.",sfucod"
						.",prgcod"
						.",acacod"
						.",loccod"
						.",ptrstatus"
						.") values(".
						 $ptrcod
						.",".$ptrano
						.",".$unicod
						.",".$ptrprgtrabalho
						.",".$irpcod
						.",".$ptrtipocreditonum
						.",".$funcod
						.",".$sfucod
						.",".$prgcod
						.",".$acacod
						.",".$loccod
						.",'N' );";

						$gravar = fwrite($arquivo00,$sql_ptr."\n");


				}

			break;
			default:
				continue;
				break;
		}


	}


	//Indica que o arquivo nao encontrou o trailer do arquivo
	if ($Processo_Importacao[$script_nome]['TB'] == false)
	{
		ImprimeStatus("formul.txtstatus",$ErroImport[17] ,Cor_Erro);
		return $ErroImport[17];
	}




?>
