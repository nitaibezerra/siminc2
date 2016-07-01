<?php

$SIAF_DIR_TXT_FILES = SIAF_DIR_TXT_FILES.'APOIO'.$ano_referencia.'/';

/**
 * Função Principal que reune os metodos para importação de um arquivo, retornando o script de execução
 *
 * @param unknown_type $nomearquivo
 * @param unknown_type $dataatualizacao
 */


function ImportacaoVP($nomearquivo,$dataatualizacao,$ano_referencia)
{
	global $db;
	global $objBar1;
	global $BancoImportacao;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_vp_'.$ano_referencia.'.sql','a');
		
	$Vinculacao = new ImportacaoArquivo();
	
	$dados = $Vinculacao->importarArquivo( $nomearquivo );
	$Navegador=0;
	$script_import = "";
	$script_final = "";
		
	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);
	
	
	foreach ($dados as $registro)
	{

		
		$vincod = "'".trim($registro["IT-CO-VINCULACAO-PAGAMENTO"])."'";
		if (str_replace(" ","",$vincod)=="''")
						$vincod = 'NULL';
	
						
		$vindsc = "'".trim($registro["IT-NO-VINCULACAO-PAGAMENTO"])."'";
		$vindsc = str_replace("\\","/",$vindsc); // Substitue \ por \\  para interpertar no script
		$vindsc = str_replace("'"," ",$vindsc); // Substitue ' por \'  para interpertar no script
		$vindsc = "'".$vindsc."'";
		if (str_replace(" ","",$vindsc)=="''")
						$vindsc = 'NULL';				
		

		
		
		//verifica se ja foi feita a busca deste codigo
		if ($vincod != "''")
		{
			
			$testebusca = false;
			$busca_ptr = "Select vincod from public.vinculacaopagamento where vincod = ".$vincod.";";
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

			$sql_ptr = " insert into public.vinculacaopagamento ("
						."vincod"
						.",vindsc"
						.",vinstatus"
						.") values(".
						 $vincod
						.",".$vindsc
						.",'I');";
	
						$gravar = fwrite($arquivo2,$sql_ptr."\n");		
						$Navegador_registros++;			
		}
					
		
	
		
		//Grafico dos Arquivos
		$Navegador++;
			
		if  (ceil(($Navegador) * 100 / $tamanho_array) > ceil($proximo))
		{	
			$proximo = (($Navegador * 100) / $tamanho_array);
			$objBar1->setProgress($proximo);
		}
			
		
	}
	
	$objBar1->setProgress(100);


	//fclose($arquivo1);
	fclose($arquivo2);
	ImprimeStatus("formul.txtstatus",'Criado script de '. $Navegador . ' registros de '.$nomearquivo);
	
	return $Navegador_registros;	
}



/**
 * Inicio - Importação
 */

function RodaVP()
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
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_vp_'.$ano_referencia.'.sql');
	
	
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
		      if ($file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && (substr($file,0,2)=='vp')) //evita leitura de . e .. e pega somente os arquivos de conta
		      {              
		      		//dbg($SIAF_DIR_TXT_FILES.$file,1);
		      		//checar_arquivo_ref($SIAF_DIR_TXT_FILES.$file,1);
		      	 
		       	   $matrix[$file]['logarquivonome']=$file; //armazena nomes dos arquivos na matriz   
				   $matrix[$file]['logarquivodata'] ="'".date("Y-m-d H:i:s", filectime($SIAF_DIR_TXT_FILES.$file))."'";
				   			   
				   $data_atual = date("Y-m-d H:i:s");
				   $matrix[$file]['logdatafim'] = "Null";
				   $matrix[$file]['logdataini'] = "'".$data_atual."'";
				   $matrix[$file]['logimporterros'] = "TRUE";
				   $matrix[$file]['logerros'] = $ErroImport[8];
				   $matrix[$file]['loginformacoes'] ="Null";
				   $matrix[$file]['logarquivoano']= "'".$ano_referencia."'";
				   $matrix[$file]['logarquivotipo'] = "vp";
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
		
		
		
		$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_vp_'.$ano_referencia.'.sql','w+');
		$sql_vp_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo2,$sql_vp_cab);
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
				
				//dados para aqruivo de log
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "Null";
				$nomearquivo['logdataini'] = "'".$data_atual."'";
				$nomearquivo['logimporterros'] = "true";
				$nomearquivo['logerros'] = $ErroImport[17];
				
				ImprimeStatus("formul.txtquantidade",$cont.' de '.$x);
				ImprimeStatus("formul.txtarquivo", $nomearquivo['logarquivonome']);
				ImprimeStatus("formul.txtstatus", 'Criando script de '.$nomearquivo['logarquivonome']);
				
				$val = ImportacaoVP($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
				
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "'".$data_atual."'";
				$nomearquivo['logatdregistros'] = $val;
				
				//dbg($arquivo_i);
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
			$dados1['logarquivotipo'] = "vp";
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
				
			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_vp_'.$ano_referencia.'.sql','roda_vp_'.$ano_referencia.'.sql')){
					
						
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
				
				ImprimeStatus("formul.txtstatus",'gravando log');
				
			
				gravarLog($dados1,2);
				
				
				ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				//exec('vacuumdb -d simec -f -v -z -table financeiro.fonterecursosiafi') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				//exec('reindex simec financeiro.fonterecursosiafi') ;
				
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$SIAF_DIR_TXT_FILES.'vp*.* ');
				exec('mv '.$SIAF_DIR_TXT_FILES.'vp*.* '.$SIAF_DIR_TXT_FILES.'/REALIZADO');
						
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











