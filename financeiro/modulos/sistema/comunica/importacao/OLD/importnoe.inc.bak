<?php

$SIAF_DIR_TXT_FILES = SIAF_DIR_TXT_FILES.'NE'.$ano_referencia.'/';

/**
 * Função Principal que reune os metodos para importação de um arquivo, retornando o script de execução
 *
 * @param unknown_type $nomearquivo
 * @param unknown_type $dataatualizacao
 */


function ImportacaoNOE($nomearquivo,$dataatualizacao,$ano_referencia)
{
	global $db;
	global $objBar1;
	global $BancoImportacao;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	$arquivo1 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'apoio_noe_'.$ano_referencia.'.sql','a');
	$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_noe_'.$ano_referencia.'.sql','a');
		
	$NotasContabil = new ImportacaoArquivo();
	
	$dados = $NotasContabil->importarArquivo( $nomearquivo );
	$PlanosaInserir = array();
	$Navegador=0;
	$script_import = "";
	$script_final = "";
	
	
		
	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);

	foreach ($dados as $registro)
	{
		
		$ungcod ="'".trim(substr($registro["GR-UG-GESTAO-AN-NUMERO-NEUQ(1)"],0,6))."'";
		if (str_replace(" ","",$ungcod)=="''")$ungcod = 'NULL';
		
		$gstcod = "'".trim(substr($registro["GR-UG-GESTAO-AN-NUMERO-NEUQ(1)"],6,5))."'";
		if (str_replace(" ","",$gstcod)=="''")$gstcod = 'NULL';
				
		$noeano = "'".trim(substr($registro["GR-UG-GESTAO-AN-NUMERO-NEUQ(1)"],11,4))."'";
		if (str_replace(" ","",$noeano)=="''")$noeano = 'NULL';

		$noenum = "'".trim(substr($registro["GR-UG-GESTAO-AN-NUMERO-NEUQ(1)"],17))."'";
		if (str_replace(" ","",$noenum)=="''")$noenum = 'NULL';
			
		$noedocreferenciaano = "'".trim(substr($registro["GR-AN-NU-DOCUMENTO-REFERENCIA"],0,4))."'";
		if (str_replace(" ","",$noedocreferenciaano)=="''")$noedocreferenciaano = 'NULL';
				
		$noedocreferencia = "'".trim(substr($registro["GR-AN-NU-DOCUMENTO-REFERENCIA"],6,6))."'";
		if (str_replace(" ","",$noedocreferencia)=="''")$noedocreferencia = 'NULL';
		
		$noedataemissao = "'".trim(substr($registro["IT-DA-EMISSAO"],0,4)."-".substr($registro["IT-DA-EMISSAO"],4,2)."-".substr($registro["IT-DA-EMISSAO"],6,2))."'";
		if (str_replace(" ","",$noedataemissao)=="'--'") $noedataemissao = 'NULL';
		
		$idpnumfavorecido = "'".(trim($registro["ID-CO-FAVORECIDO"]))."'";
		if (str_replace(" ","",$idpnumfavorecido)=="''")$iidpnumfavorecido = 'NULL';
				
		$noeobs = trim($registro["IT-TX-OBSERVACAO"]);
		$noeobs = str_replace("\\","/",$noeobs); // Substitue \ por \\  para interpertar no script
		$noeobs = str_replace("'"," ",$noeobs); // Substitue ' por \'  para interpertar no script
		$noeobs = "'".$noeobs."'";
		if (str_replace(" ","",$noeobs)=="''")$noeobs = 'NULL';
		
		$evecod = "'".trim($registro["GR-CODIGO-EVENTO"])."'";
		if (str_replace(" ","",$evecod)=="''")$evecod = 'NULL';
				
		$esfcod = "'".trim($registro["IT-IN-ESFERA-ORCAMENTARIA"])."'";
		if (str_replace(" ","",$esfcod)=="''")$esfcod = 'NULL';
		
		$ptrcod = "'".trim($registro["IT-CO-PROGRAMA-TRABALHO-RESUMIDO"])."'";
		if (str_replace(" ","",$ptrcod)=="")$ptrcod = 'NULL';
		
		$iducod = "'".trim(substr($registro["GR-FONTE-RECURSO"],0,1))."'";
		if (str_replace(" ","",$iducod)=="''")$iducod = 'NULL';
				
		$grfcod = "'".trim(substr($registro["GR-FONTE-RECURSO"],1,1))."'";
		if (str_replace(" ","",$grfcod)=="''")$grfcod = 'NULL';

		$frscod = "'".trim(substr($registro["GR-FONTE-RECURSO"],2,2))."'";
		if (str_replace(" ","",$frscod)=="''")$frscod = 'NULL';
		
		$frsgrcod = "'".trim(substr($registro["GR-FONTE-RECURSO"],4,6))."'";
		if (str_replace(" ","",$frsgrcod)=="''")$frsgrcod = 'NULL';
		
		//$frscod_dlp = Busca_Apoio('fonterecursosiafi','frscod',"'".trim(substr($registro["GR-FONTE-RECURSO"],2,8))."'");
				
		$ndpcod = "'".trim($registro["GR-NATUREZA-DESPESA"])."'";
		if (str_replace(" ","",$ndpcod)=="")	$ndpcod = 'NULL';
				
		$ungcodresponsavel = "'".trim($registro["IT-CO-UG-RESPONSAVEL"])."'";
		if (str_replace(" ","",$ungcodresponsavel)=="''")$ungcodresponsavel = 'NULL';
				
		$noetransacaovlr = ((float)$registro["IT-VA-TRANSACAO"])/100;
		if (!is_double($noetransacaovlr)) $noetransacaovlr = 'NULL';
		
		$muncod = "'".trim($registro["IT-CO-MUNICIPIO-BENEFICIADO"])."'";
		if (str_replace(" ","",$muncod)=="''")$muncod = 'NULL';
				
		$noeincisonum = "'".trim($registro["IT-CO-INCISO"])."'";
		if (str_replace(" ","",$noeincisonum)=="''")$noeincisonum = 'NULL';
						
		$noeamparolegalnum = "'".trim($registro["IT-TX-AMPARO-LEGAL"])."'";
		if (str_replace(" ","",$noeamparolegalnum)=="''")$noeamparolegalnum = 'NULL';
		
		$ungcoddocreferencia = "'".trim($registro["IT-CO-UG-DOC-REFERENCIA"])."'";
		if (str_replace(" ","",$ungcoddocreferencia)=="''")$ungcoddocreferencia = 'NULL';
		
						
		$gstcoddocumentoreferencia = "'".trim($registro["IT-CO-GESTAO-DOC-REFERENCIA"])."'";
		if (str_replace(" ","",$gstcoddocumentoreferencia)=="''")$gstcoddocumentoreferencia = 'NULL';
				
		$noecronogramavlr = ((float)$registro["IT-VA-CRONOGRAMA(1)"])/100;
		if (!is_double($noecronogramavlr))$noecronogramavlr = 'NULL';
						
		$noedatavencimento = "'".substr($registro["IT-DA-VENCIMENTO(1)"],0,4)."-".substr($registro["IT-DA-VENCIMENTO(1)"],4,2)."-".substr($registro["IT-DA-VENCIMENTO(1)"],6,2)."'";
		if (str_replace(" ","",$noedatavencimento)=="'--'")$noedatavencimento = 'NULL';
	
		$noedatapagamento = "'".substr($registro["IT-DA-PAGAMENTO(1)"],0,4)."-".substr($registro["IT-DA-PAGAMENTO(1)"],4,2)."-".substr($registro["IT-DA-PAGAMENTO(1)"],6,2)."'";
		if (str_replace(" ","",$noedatapagamento)=="'--'")
						$noedatapagamento = 'NULL';
		
		$noecronogramadovlr = ((float)$registro["IT-VA-CRONOGRAMADO(1)"])/100;
		if (!is_double($noecronogramadovlr))$noecronogramadovlr = 'NULL';
							
		$noeprecatorionum = "'".trim($registro["IT-NU-PRECATORIO"])."'";
		if (str_replace(" ","",$noeprecatorionum)=="''")$noeprecatorionum = 'NULL';
						
		$noepagprecatoriosit = "'".trim($registro["IT-IN-PAGAMENTO-PRECATORIO"])."'";
		if (str_replace(" ","",$noepagprecatoriosit)=="''")$noepagprecatoriosit = 'NULL';

		$noenumoriginal = "'".trim(substr($registro["GR-UG-GESTAO-AN-NUMERO-NEUQ(1)"],11))."'";
		if (str_replace(" ","",$noenumoriginal)=="''")$noenumoriginal = 'NULL';
		
		//campo memo de preencimento facultativo
		$noeoriginalnum = "'".trim($registro["IT-NU-ORIGINAL"])."'";
		if (str_replace(" ","",$noeoriginalnum)=="''")$noeoriginalnum = 'NULL';
		
		$noedataatualizacao = "'".substr($registro["IT-DA-ATUALIZACAO"],0,4)."-".substr($registro["IT-DA-ATUALIZACAO"],4,2)."-".substr($registro["IT-DA-ATUALIZACAO"],6,2)."'";
		if (str_replace(" ","",$noedataatualizacao)=="'--'")$noedataatualizacao = 'NULL';
						
		$noeliquidacaosit = "'".trim($registro["IT-IN-LIQUIDACAO"])."'";
		if (str_replace(" ","",$noeliquidacaosit)=="''")$noeliquidacaosit = 'NULL';
		
		$idpnumfavorecido = "'".(trim($registro["ID-CO-FAVORECIDO"]))."'";
		if (str_replace(" ","",$idpnumfavorecido)=="''")$iidpnumfavorecido = 'NULL';
			

		//verifica se ja foi feita a busca deste codigo
		if (($noenumoriginal != "''")||($noenumoriginal != 'NULL'))
		{
			
			$testebusca = false;
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
				
				$sql_notae=" insert into financeiro.notaempenho ("
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
				$sql_notae = "update financeiro.notaempenho set " .
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
					
					
			$gravar = fwrite($arquivo2,$sql_notae."\n");
			$Navegador_registros++;
			//Grafico dos Arquivos	
			$Navegador++;
			
			if  (ceil(($Navegador) * 100 / $tamanho_array) > ceil($proximo))
			{	
				$proximo = (($Navegador * 100) / $tamanho_array);
				$objBar1->setProgress($proximo);
			}
	}
	
	$objBar1->setProgress(100);
	
	fclose($arquivo1);
	fclose($arquivo2);
	ImprimeStatus("formul.txtstatus",'Criado script de '. $Navegador . ' registros de '.$nomearquivo);
	return $Navegador_registros;	
}

/**
* Inicio - Importação
*/

function RodaNOE()
{

	global $ErroImport;
	global $sucessoImport;
	global $objBar0;
	global $SIAF_DIR_TXT_FILES;
	global $ano_referencia;
	$data = getdate();
	$dataatualizacao = "'".$data[mday]."-".$data[mon]."-".$data[year]."'";
	$Erro_Integridade = false;
	$DataInicioImporta = date("Y-m-d H:i:s"); //Chave para tabela de log
	$matrix = null;
	//limpar todos os scripts previos
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_noe_'.$ano_referencia.'.sql');
	
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
		      if ($file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && (substr($file,0,1)=='n' or substr($file,0,2)=='nf')) //evita leitura de . e .. e pega somente os arquivos de conta
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
				   $matrix[$file]['logarquivotipo'] = "n";
				   $matrix[$file]['logatdregistros'] = 0;
				   $matrix[$file]['caminho']="'".$SIAF_DIR_TXT_FILES."'";
				   $matrix[$file]['logdataimport']="'".$DataInicioImporta."'";
		            $matrix[$file]['logdatainiscript'] ='Null';
					$matrix[$file]['logdatafimscript'] ='Null';
		           
		           $x++;
		      } 
		      
		   } 
		  closedir($handle);                              //fecha o diretório
		}
		
		
		$arquivo1 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'apoio_noe_'.$ano_referencia.'.sql','w+');
		$sql_apoio_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo1,$sql_apoio_cab);
		fclose($arquivo1);
		
		$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_noe_'.$ano_referencia.'.sql','w+');
		$sql_saldocontabil_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo2,$sql_saldocontabil_cab);
		fclose($arquivo2);
		
		
		if (!is_null($matrix))
		{
			
			$cont = 1;
			$Erro_Integridade = false;
			
			
			foreach ($matrix as $nomearquivo)
			{
			
			
				//checar integridade do arquivo "caracter especial			
				if (checar_eof_file($SIAF_DIR_TXT_FILES.$nomearquivo['logarquivonome'])){
	
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
		
		
		if ($Erro_Integridade != true)
		{
			
			$tamanho_array = sizeof($matrix);
			
			foreach ($matrix as $nomearquivo)
			{
			
				//}
				
					//dados para aqruivo de log
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "Null";
				$nomearquivo['logdataini'] = "'".$data_atual."'";
				$nomearquivo['logimporterros'] = "true";
				$nomearquivo['logerros'] = $ErroImport[17];
				ImprimeStatus("formul.txtquantidade",$cont.' de '.$x);
				ImprimeStatus("formul.txtarquivo", $nomearquivo['logarquivonome']);
				ImprimeStatus("formul.txtstatus", 'Criando script de '.$nomearquivo['logarquivonome']);
				
				$val = ImportacaoNOE($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
				
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "'".$data_atual."'";
				$nomearquivo['logatdregistros'] = $val;
				
				gravarLog($nomearquivo);	
				
				
				ImprimeStatus("formul.txtstatus", 'concluida criação script de '.$nomearquivo['logarquivonome']);
							
				if  (ceil(($cont) * 100 / $tamanho_array) > ceil($proximo))
				{	
					$proximo = (($cont * 100) / $tamanho_array);
					$objBar0->setProgress($proximo);
				}
				$cont++;
				
			}
			
		
			ImprimeStatus("formul.txtstatus",'Rodando Scripts de Carga');
			
			$dados1 = array();
			$dados1['logdataimport']="'".$DataInicioImporta."'";
			$dados1['logarquivotipo'] = "n";
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
			
		
			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_noe_'.$ano_referencia.'.sql','roda_noe_'.$ano_referencia.'.sql')){
					
					
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
				
				ImprimeStatus("formul.txtstatus",'gravando log');
				
				gravarLog($dados1,2);
				
				
				ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				//exec('vacuumdb -d simec -f -v -z -table financeiro.notaempenho') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				//exec('reindex simec financeiro.notaempenho') ;
				
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$SIAF_DIR_TXT_FILES.'n*.* ');
				exec('mv '.$SIAF_DIR_TXT_FILES.'n*.* '.$SIAF_DIR_TXT_FILES.'/REALIZADO');
						
				ImprimeStatus("formul.txtstatus",'Importação Realizada com Sucesso');
				return($sucessoImport);
					
			}else{
				
				gravarLog($dados1,3);
				ImprimeStatus("formul.txtstatus",$ErroImport[12]);
				return $ErroImport[12];
			}
		}
		
	}
}
?>
<html>



</html>











