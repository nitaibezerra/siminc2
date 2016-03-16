<?php


$Otimizacao_1 = null;
$Otimizacao_1 = array();//campos de campo destino detalhe



$SIAF_DIR_TXT_FILES = SIAF_DIR_TXT_FILES.'SC'.$ano_referencia.'/' ;
/**
 * Variavel Global para fins de otimização, evitando
 * consultas na tabela de referencia.
 *
 *  */

 $Somatorio = array();

function ImportacaoSaldo($nomearquivo,$dataatualizacao,$ano_referencia)
{
	global $Somatorio;
	global $db;
	global $Otimizacao_1;
	global $objBar1;
	global $VerificaDuplo;
	global $testar_duplo;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	//abre aquivos de importacao

	for ($i =0; $i<=13;$i++)
	{
		if ($i<10){ $val = '0'.$i.''; $arquivo = 'arquivo'.$val; } else {$val = $i;$arquivo = 'arquivo'.$val;};
		${$arquivo} = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_saldo_diario_'.$ano_referencia.'_'.$val.'.sql','a');
	}


	$NotasContabil = new ImportacaoArquivo();
	$dados = $NotasContabil->importarArquivo( $nomearquivo );
	$PlanosaInserir = array();
	$Navegador=0;
	$script_import = "";
	$script_final = "";


	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);



	$armazenar = false;//controle se os dados da memoria foram salvos
	$sql_saldocontabil = array();//armazena as linhas a serem inseridas na memoria
	$Contador_Armazenamento = 0; //Flag para controlar a quantidade de registros na memoria

	foreach ($dados as $registro)
	{
		if ((trim($registro['GR-CODIGO-CONTA'])=='292410403')and ($ano_referencia == '2006'))
		{

			for ($mes= 0; $mes<=13;$mes++)
			{

				if ($mes == 0)
				{
					$campo1 = "IT-VA-DEBITO-INICIAL";
					$campo2 = "IT-VA-CREDITO-INICIAL";
				}else
				{
					$campo1 = "IT-VA-DEBITO-MENSAL(".$mes.")";
					$campo2 = "IT-VA-CREDITO-MENSAL(".$mes.")";
				}

				if (($registro[$campo1] == null)or($registro[$campo2] == null))
				{
					continue;
				}

				$Somatorio ['292410403'][$mes]=$Somatorio ['292410403'][$mes]+(((float) $registro[$campo2])/100)-(((float) $registro[$campo1])/100);

			}


		}

		//Grafico dos Arquivos
		$Navegador++;


		if  (ceil(($Navegador) * 100 / $tamanho_array) > ceil($proximo))
		{
			$proximo = (($Navegador * 100) / $tamanho_array);
			$objBar1->setProgress($proximo);
		}

		ob_flush();
		flush();


	}

	$objBar1->setProgress(100);
	ImprimeStatus("formul.txtstatus",'Criado script de '. $Navegador*14 . ' registros de '.$nomearquivo);
	ob_flush();
	flush();

	dbg($ano_referencia.'-'.$nomearquivo);
	dbg($Somatorio);
	return $Navegador_registros;
}


/**
 * Inicio - Importação
 */
function RodaSALDO()
{
	global $ErroImport;
	global $sucessoImport;
	global $objBar0;
	global $SIAF_DIR_TXT_FILES;
	global $ano_referencia;
	global $Somatorio;


	//$nomearquivo ='sc111355.txt';//'sc111371.txt';//'sc111321.txt';//'SCDIARIO_Saldo_Contabil_20061018.txt';//'sc111371.txt';
	$data = getdate();
	$dataatualizacao = "'".$data[mday]."-".$data[mon]."-".$data[year]."'";
	$Erro_Integridade = false;
	$VerificaDuplo ='';
	$VerificaDuplo = array();//Verifica se arquivo ja foi importado ou não
	$arquivo_i = array();
	$tamanho_arquivos = array();
	$DataInicioImporta = date("Y-m-d H:i:s"); //Chave para tabela de log
	$matrix = null;

	//limpar todos os scripts previos
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_saldo_diario'.$ano_referencia.'_*.sql');

	set_time_limit(0);

	if (!is_dir($SIAF_DIR_TXT_FILES))
	{
		ImprimeStatus("formul.txtstatus",$ErroImport[16]);
		return ($ErroImport[16]);
	}else
	{


		if ($handle=opendir($SIAF_DIR_TXT_FILES))  //abre diretório para leitura
		{
			$x=0;
		   	while (false!==($file=readdir($handle)))//lê arquivos do diretório
		   	{


		   	  if ( $file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && substr($file,0,1)=='s') //evita leitura de . e .. e pega somente os arquivos de conta
		      {

				   $VerificaDuplo[$file] = verificaLog($SIAF_DIR_TXT_FILES,$file,$ano_referencia,'sc');



				   if ($VerificaDuplo[$file]!=1)
				   {
					   $matrix[$file]['logarquivonome']=$file; //armazena nomes dos arquivos na matriz
					   $matrix[$file]['logarquivodata'] ="'".date("Y-m-d H:i:s", filectime($SIAF_DIR_TXT_FILES.$file))."'";

					   $data_atual = date("Y-m-d H:i:s");
					   $matrix[$file]['logdatafim'] = "Null";
					   $matrix[$file]['logdataini'] = "'".$data_atual."'";
					   $matrix[$file]['logimporterros'] = "TRUE";
					   $matrix[$file]['logerros'] = $ErroImport[8];
					   $matrix[$file]['loginformacoes'] ="Null";
					   $matrix[$file]['logarquivoano']= "'".$ano_referencia."'";
					   $matrix[$file]['logarquivotipo'] = "s";
					   $matrix[$file]['logatdregistros'] = 0;
					   $matrix[$file]['caminho']="'".$SIAF_DIR_TXT_FILES."'";
					   $matrix[$file]['logdataimport']="'".$DataInicioImporta."'";
					   $matrix[$file]['logdatainiscript'] ='Null';
					   $matrix[$file]['logdatafimscript'] ='Null';

					   $x++;
				   }
		      }

		   	}


		  	closedir($handle);                              //fecha o diretório
		}

		//dbg($matrix,1);
		//dbg($VerificaDuplo,1);
		//fecha ok


		if (!is_null($matrix))
		{
			$cont = 1;
			//$matrix =array('sc2005c9.txt','sc2005g9.txt');
			//$DivArquivo = array();//Define pontos de divisão
			//$tamanhototal = 0;  //Variavel calcula o tamanho do arquivo e divide

			ImprimeStatus("formul.txtstatus", "Testando integridade dos arquivos.");
			$Erro_Integridade = false;


			foreach ($matrix as $nomearquivo)
			{


				//checar integridade do arquivo "caracter especial
				if (checar_eof_file($SIAF_DIR_TXT_FILES.$nomearquivo['logarquivonome']))
				{

						ImprimeStatus("formul.txtstatus",$ErroImport[2].'('.$nomearquivo['logarquivonome'].')');
						$Erro_Integridade = true;
						$data_atual = date("Y-m-d H:i:s");
						$nomearquivo['logdataini'] = "'".$data_atual."'";
						$nomearquivo['logdatafim'] = "'".$data_atual."'";
						$nomearquivo['logimporterros'] = "TRUE";
						$nomearquivo['logerros'] = $ErroImport[2];
						gravarLog($nomearquivo);
						return $ErroImport[2].'('.$nomearquivo['logarquivonome'].')';//Erro por falta do caractere final de arquivo
					break;
				}

				//checar ero no arquivo de referencia
				if (!checar_arquivo_ref($SIAF_DIR_TXT_FILES.$nomearquivo['logarquivonome'],$matrix[$file]['logarquivotipo']))
				{

						ImprimeStatus("formul.txtstatus",$ErroImport[1].'('.$nomearquivo['logarquivonome'].')');
						$Erro_Integridade = true;
						$data_atual = date("Y-m-d H:i:s");
						$nomearquivo['logdataini'] = "'".$data_atual."'";
						$nomearquivo['logdatafim'] = "'".$data_atual."'";
						$nomearquivo['logimporterros'] = "TRUE";
						$nomearquivo['logerros'] = $ErroImport[1];
						gravarLog($nomearquivo);
						return $ErroImport[1].'('.$nomearquivo['logarquivonome'].')';
						break;
				}




			}

		}else
		{
			ImprimeStatus("formul.txtstatus",$ErroImport[16]);
			$Erro_Integridade = true;
			return ($ErroImport[16]);
		}

		//ok
		//dbg($Erro_Integridade,1);

		if ($Erro_Integridade != true)
		{


			//Coloca cabeçarios nos arquivos
			$val = '';
			for ($i = 0; $i<=13;$i++)
			{

				if ($i<10){ $val = '0'.$i;} else {$val = $i;};
				$arquivo_cab = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_saldo_diario_'.$ano_referencia.'_'.$val.'.sql','w+');
				$sql_saldocontabil_cab = "SET client_encoding TO 'LATIN5';\nCOPY particionamento.saldocontabil".$ano_referencia.$val."(ungcod,concod,sldcccnum,gstcod,sldano,sldmes,slddebitovlr,sldcreditovlr,slddataatualizacao,slddatatransacao,tcccod,cagcod,iducod,grfcod,frscod,frsgrcod, vincod,ntrcod,esfcod,unicod,irpcod,tarcod,ptrcod,ndpcod,trrcod,cccmes,noenumoriginal,orgcod,npenumoriginal,ungcodresponsavel,ungcoddocumento,gstcoddocumento) FROM stdin;\n";
				$gravar = fwrite($arquivo_cab,$sql_saldocontabil_cab);
				fclose($arquivo_cab);
			}


			$tamanho_array = sizeof($matrix);



			foreach ($matrix as $nomearquivo)
			{

				//dados para aqruivo de log
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "Null";
				$nomearquivo['logdataini'] = "'".$data_atual."'";
				$nomearquivo['logimporterros'] = "true";
				$nomearquivo['logerros'] = $ErroImport[17];
				ImprimeStatus("formul.txtquantidade",$cont.' de '.$x);
				ImprimeStatus("formul.txtarquivo", $nomearquivo['logarquivonome']);
				ImprimeStatus("formul.txtstatus", 'Criando script de '.$nomearquivo['logarquivonome']);



				//dbg($matrix);
				$val = ImportacaoSaldo($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);

				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "'".$data_atual."'";
				$nomearquivo['logatdregistros'] = $val;

				//dbg($arquivo_i);
				gravarLog($nomearquivo);

				ImprimeStatus("formul.txtstatus", 'concluida criação script de '.$nomearquivo['logarquivonome']);


				if  (ceil(($cont) * 100 / $tamanho_array) > ceil($proximo))
				{
					$proximo = (($cont * 100) / $tamanho_array);
					$objBar0->setProgress($proximo);
				}

				$cont++;
			}



			dbg($Somatorio,1);
			dbg('Fim');
			return 1;

		}
	}
}


?>
