<?php

$SIAF_DIR_TXT_FILES = SIAF_DIR_TXT_FILES.'APOIO'.$ano_referencia.'/';

/**
 * Função Principal que reune os metodos para importação de um arquivo, retornando o script de execução
 *
 * @param unknown_type $nomearquivo
 * @param unknown_type $dataatualizacao
 */


function ImportacaoFR($nomearquivo,$dataatualizacao,$ano_referencia)
{
	global $db;
	global $objBar1;
	global $BancoImportacao;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_fr_'.$ano_referencia.'.sql','a');
		
	$arq_FR = new ImportacaoArquivo();
	
	$dados = $arq_FR->importarArquivo( $nomearquivo );
	$Navegador=0;
	$script_import = "";
	$script_final = "";
	
	
		
	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);
	
	
	foreach ($dados as $registro)
	{
		$frscod = "'".trim($registro["GR-FONTE"])."'";
		if (str_replace(" ","",$frscod)=="''")
						$frscod = 'NULL';
		
				
		$frsdsc = trim($registro["IT-NO-FONTE"]);
		$frsdsc = str_replace("\\","/",$frsdsc); // Substitue \ por \\  para interpertar no script
		$frsdsc = str_replace("'"," ",$frsdsc); // Substitue ' por \'  para interpertar no script
		$frsdsc = "'".$frsdsc."'";
		if (str_replace(" ","",$frsdsc)=="''")
						$frsdsc = 'NULL';
		
		$frssof = "'".trim($registro["IT-IN-TIPO-FONTE-SOF"])."'";
		if (str_replace(" ","",$frssof)=="''")
						$frssof = 'NULL';
	
		
		$frssofprog = "'".trim($registro["IT-IN-FONTE-SOF-PROGRAMACAO"])."'";
		if (str_replace(" ","",$frssofprog)=="''")$frssofprog = 'NULL';
		
		
		$frsaplicacao = "'".trim($registro["IT-IN-PERMITE-APLICACAO"])."'";
		if (str_replace(" ","",$frsaplicacao)=="''")
						$frsaplicacao = 'NULL';

		$frsexclusivastn = "'".trim($registro["IT-IN-EXCLUSIVA-STN"])."'";
		if (str_replace(" ","",$frsexclusivastn)=="''")
						$frsexclusivastn = 'NULL';
							
		$frspagto = "'".trim($registro["IT-IN-PERMITE-PAGAMENTO"])."'";
		if (str_replace(" ","",$frspagto)=="''")
						$frspagto = 'NULL';
							
		$esfcod = "'".trim($registro["IT-IN-ESFERA-ORCAMENTARIA"])."'";
		if (str_replace(" ","",$esfcod)=="''")
						$esfcod = 'NULL';
						
		$frsobs = "'".trim($registro["IT-TX-MOTIVO"])."'";
		$frsobs = str_replace("\\","/",$frsobs); // Substitue \ por \\  para interpertar no script
		$frsobs = str_replace("'"," ",$frsobs); // Substitue ' por \'  para interpertar no script
		$frsobs = "'".$frsobs."'";
		if (str_replace(" ","",$frsobs)=="''")
						$frsobs = 'NULL';				
		

		
		
		//verifica se ja foi feita a busca deste codigo
		if ($frscod != "''")
		{
			
			$testebusca = false;
			$busca_fr = "Select frscod from financeiro.fonterecursosiafi  where frscod = ".$frscod.";";
			if(($RS = pg_query($BancoImportacao,$busca_fr)) && (pg_num_rows($RS)>=1)) {
				//$testebusca = $db->pegaUm($busca_fr);
				$testebusca = true;
			}
			
			
		}else 
		{
			$testebusca = false;
		}
		
		
		
		if ($testebusca == false )
		{

			$sql_fr = " insert into financeiro.fonterecursosiafi ("
						."frscod"
						.",frsdsc"
						.",frssof"
						.",frssofprog"
						.",frsaplicacao"
						.",frsexclusivastn"
						.",frspagto"
						.",esfcod"
						.",frsobs"
						.") values(".
						 $frscod
						.",".$frsdsc
						.",".$frssof
						.",".$frssofprog
						.",".$frsaplicacao
						.",".$frsexclusivastn
						.",".$frspagto
						.",".$esfcod
						.",".$frsobs.");";
	
						$gravar = fwrite($arquivo2,$sql_fr."\n");		
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

function RodaFR()
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
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_fr_'.$ano_referencia.'.sql');
	
	
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
		      if ($file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && (substr($file,0,1)=='f')) //evita leitura de . e .. e pega somente os arquivos de conta
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
				   $matrix[$file]['logarquivotipo'] = "fr";
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
		
		
		
		$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_fr_'.$ano_referencia.'.sql','w+');
		$sql_fr_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo2,$sql_fr_cab);
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
				
				$val = ImportacaoFR($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
				
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
			$dados1['logarquivotipo'] = "fr";
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
				
			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_fr_'.$ano_referencia.'.sql','roda_fr_'.$ano_referencia.'.sql')){
					
						
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
				
				ImprimeStatus("formul.txtstatus",'gravando log');
				
			
				gravarLog($dados1,2);
				
				
				ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				//exec('vacuumdb -d simec -f -v -z -table financeiro.fonterecursosiafi') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				//exec('reindex simec financeiro.fonterecursosiafi') ;
				
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$SIAF_DIR_TXT_FILES.'f*.* ');
				exec('mv '.$SIAF_DIR_TXT_FILES.'f*.* '.$SIAF_DIR_TXT_FILES.'/REALIZADO');
						
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











