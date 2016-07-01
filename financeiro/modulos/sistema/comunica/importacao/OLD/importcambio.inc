<?php

$SIAF_DIR_TXT_FILES=SIAF_DIR_TXT_FILES.'APOIO'.$ano_referencia.'/';

/**
 * Função Principal que reune os metodos para importação de um arquivo, retornando o script de execução
 *
 * @param unknown_type $nomearquivo
 * @param unknown_type $dataatualizacao
 */

function ImportacaoCambio($nomearquivo,$dataatualizacao,$ano_referencia)
{
	
	global $objBar1;
	global $BancoImportacao;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_cambio_'.$ano_referencia.'.sql','a');
		
	$DadosCambio = new ImportacaoArquivo();
	$dados = $DadosCambio->importarArquivo( $nomearquivo );
	$Navegador=0;
	$script_import = "";
	$script_final = "";
	
	

	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);

	foreach ($dados as $registro)
	{
		
		
		$txccod = "'".trim($registro["IT-IN-TAXA-CONVERSAO"])."'";
		if (str_replace(" ","",$txccod)=="''")
						$txccod = 'NULL';
		
		$moecodorigem = "'".trim($registro["IT-CO-MOEDA-ORIGEM"])."'";
		if (str_replace(" ","",$moecodorigem)=="''")
						$moecodorigem = 'NULL';
		
		$moecoddestino = "'".trim($registro["IT-CO-MOEDA-DESTINO"])."'";
		if (str_replace(" ","",$moecoddestino)=="''")
						$moecoddestino = 'NULL';
	
		
		$cmbdatavigencia = "'".trim(substr($registro["IT-DA-VIGENCIA"],0,4)."-".substr($registro["IT-DA-VIGENCIA"],4,2)."-".substr($registro["IT-DA-VIGENCIA"],6,2))."'";
		if (str_replace(" ","",$cmbdatavigencia)=="'--'")$cmbdatavigencia = 'NULL';
		
		
		$cmbvaloperacao = "'".trim($registro["IT-OP-CAMBIAL"])."'";
		if (str_replace(" ","",$cmbvaloperacao)=="''")
						$cmbvaloperacao = 'NULL';
		

		$cmbinteiro = "'".trim($registro["IT-OP-INTEIRO"])."'";
		if (str_replace(" ","",$cmbinteiro)=="''")
						$cmbinteiro = 'NULL';
						
		$cmbnumerador = "'".trim($registro["IT-OP-NUMERADOR"])."'";
		if (str_replace(" ","",$cmbnumerador)=="''")
						$cmbnumerador = 'NULL';
							
		$cmbdenominador = "'".trim($registro["IT-OP-DENOMINADOR"])."'";
		if (str_replace(" ","",$cmbdenominador)=="''")
						$cmbdenominador = 'NULL';
						
		$cmbvaloperacaoaer = "'".trim($registro["IT-OP-CAMBIAL-AER"])."'";
		if (str_replace(" ","",$cmbvaloperacaoaer)=="''")
						$cmbvaloperacaoaer = 'NULL';				
		
		$cmbvaloperacaofmi = "'".trim($registro["IT-OP-CAMBIAL-FMI"])."'";
		if (str_replace(" ","",$cmbvaloperacaofmi)=="''")
						$cmbvaloperacaofmi = 'NULL';

		$cmbvaloperacaominimo = "'".trim($registro["IT-OP-VALOR-MINIMO"])."'";
		if (str_replace(" ","",$cmbvaloperacaominimo)=="''")
						$cmbvaloperacaominimo = 'NULL';
						
		$cmbvaloperacaomaximo = "'".trim($registro["IT-OP-VALOR-MAXIMO"])."'";
		if (str_replace(" ","",$cmbvaloperacaomaximo)=="''")
						$cmbvaloperacaomaximo = 'NULL';
						
		$cmbvaloperacaocompra = "'".trim($registro["IT-OP-CAMBIAL-COMPRA"])."'";
		if (str_replace(" ","",$cmbvaloperacaocompra)=="''")
						$cmbvaloperacaocompra = 'NULL';
		
		$cmbvaloperacaoant = "'".trim($registro["IT-OP-CAMBIAL-ANT"])."'";
		if (str_replace(" ","",$cmbvaloperacaoant)=="''")
						$cmbvaloperacaoant = 'NULL';
							
						
	
		//verifica se ja foi feita a busca deste codigo
		if (($cmbdatavigencia != 'NULL')and ($txccod!='NULL'))
		{
			
			$testebusca = false;
			$busca_ptr = "Select cmbid from financeiro.cambio  where txccod = ".$txccod." and moecodorigem = ".$moecodorigem." and moecoddestino = ".$moecoddestino." and cmbdatavigencia = ".$cmbdatavigencia.";";
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
			
			

			$sql_ptr = " insert into financeiro.cambio ("
						."txccod"
						.",moecodorigem"
						.",moecoddestino"
						.",cmbdatavigencia"
						.",cmbvaloperacao"
						.",cmbinteiro"
						.",cmbnumerador"
						.",cmbdenominador"
						.",cmbvaloperacaoaer"
						.",cmbvaloperacaofmi"
						.",cmbvaloperacaominimo"
						.",cmbvaloperacaomaximo"
						.",cmbvaloperacaocompra"
						.",cmbvaloperacaoant"
						.") values(".
						 $txccod
						.",".$moecodorigem
						.",".$moecoddestino
						.",".$cmbdatavigencia
						.",".$cmbvaloperacao
						.",".$cmbinteiro
						.",".$cmbnumerador
						.",".$cmbdenominador
						.",".$cmbvaloperacaoaer
						.",".$cmbvaloperacaofmi
						.",".$cmbvaloperacaominimo
						.",".$cmbvaloperacaomaximo
						.",".$cmbvaloperacaocompra
						.",".$cmbvaloperacaoant.");";
	
				$gravar = fwrite($arquivo2,$sql_ptr."\n");		
				$Navegador_registros++; 
				
		}/*else 
		{
				$sql_ptr = "update public.cambio set " .
						   	"txccod = ".$txccod
							.",moecodorigem = ".$moecodorigem
							.",moecoddestino = ".$moecoddestino
							.",cmbdatavigencia = ".$cmbdatavigencia
							.",cmbvaloperacao = ".$cmbvaloperacao
							.",cmbinteiro = ".$cmbinteiro
							.",cmbnumerador = ".$cmbnumerador
							.",cmbdenominador = ".$cmbdenominador
							.",cmbvaloperacaoaer = ".$cmbvaloperacaoaer
							.",cmbvaloperacaofmi = ".$cmbvaloperacaofmi
							.",cmbvaloperacaominimo = ".$cmbvaloperacaominimo
							.",cmbvaloperacaomaximo = ".$cmbvaloperacaomaximo
							.",cmbvaloperacaocompra = ".$cmbvaloperacaocompra
							.",cmbvaloperacaoant = ".$cmbvaloperacaoant
							." where txccod = ".$txccod." and moecodorigem = ".$moecodorigem." and moecoddestino = ".$moecoddestino." and cmbdatavigencia = ".$cmbdatavigencia.";";
										
		}*/
					
		
	
		
		//Grafico dos Arquivos
		$Navegador++;
			
		if  (ceil(($Navegador) * 100 / $tamanho_array) > ceil($proximo))
		{	
			$proximo = (($Navegador * 100) / $tamanho_array);
			$objBar1->setProgress($proximo);
		}
			
		
	}
	
	$objBar1->setProgress(100);

	
	fclose($arquivo2);
	ImprimeStatus("formul.txtstatus",'Criado script de '. $Navegador . ' registros de '.$nomearquivo);
	
	return $Navegador_registros;	
}



/**
 * Inicio - Importação
 */

function RodaCambio()
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
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_cambio_'.$ano_referencia.'.sql');
	
	
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
		      if ($file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && (substr($file,0,1)=='c')) //evita leitura de . e .. e pega somente os arquivos de conta
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
					   $matrix[$file]['logarquivotipo'] = "c";
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
		
			
		$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_cambio_'.$ano_referencia.'.sql','w+');
		$sql_cambio_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo2,$sql_cambio_cab);
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
				
				//checar erro no arquivo de referencia
				if (!checar_arquivo_ref($SIAF_DIR_TXT_FILES.$nomearquivo['logarquivonome'],$nomearquivo['logarquivotipo']))
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
			
				//dados para aqruivo de log
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "Null";
				$nomearquivo['logdataini'] = "'".$data_atual."'";
				$nomearquivo['logimporterros'] = "true";
				$nomearquivo['logerros'] = $ErroImport[17];
				ImprimeStatus("formul.txtquantidade",$cont.' de '.$x);
				ImprimeStatus("formul.txtarquivo", $nomearquivo['logarquivonome']);
				ImprimeStatus("formul.txtstatus", 'Criando script de '.$nomearquivo['logarquivonome']);
				
				$val = ImportacaoCambio($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
				
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "'".$data_atual."'";
				$nomearquivo['logatdregistros'] = $val;
				
				gravarLog($nomearquivo);	
				
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
			$dados1['logarquivotipo'] = "c";
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
		
		
			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_cambio_'.$ano_referencia.'.sql','roda_cambio_'.$ano_referencia.'.sql')){
	
				
				
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
						
				ImprimeStatus("formul.txtstatus",'gravando log');
		
				
				gravarLog($dados1,2);
				
				
				ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				//exec('vacuumdb -d simec -f -v -z -table financeiro.cambio') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				//exec('reindex simec financeiro.cambio') ;
				
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$SIAF_DIR_TXT_FILES.'c*.* ');
				exec('mv '.$SIAF_DIR_TXT_FILES.'c*.* '.$SIAF_DIR_TXT_FILES.'/REALIZADO');
						
				ImprimeStatus("formul.txtstatus",'Importação Realizada com Sucesso');
				return($sucessoImport);
					
			}else {
				
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











