<?php

$SIAF_DIR_TXT_FILES = SIAF_DIR_TXT_FILES.'PTRES'.$ano_referencia.'/';

/**
 * Função Principal que reune os metodos para importação de um arquivo, retornando o script de execução
 *
 * @param unknown_type $nomearquivo
 * @param unknown_type $dataatualizacao
 */


function ImportacaoPTRES($nomearquivo,$dataatualizacao,$ano_referencia)
{
	global $db;
	global $objBar1;
	global $BancoImportacao;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_ptres_'.$ano_referencia.'.sql','a');
		
	$NotasContabil = new ImportacaoArquivo();
	
	$dados = $NotasContabil->importarArquivo( $nomearquivo );
	$Navegador=0;
	$script_import = "";
	$script_final = "";
	
	
		
	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);

	foreach ($dados as $registro)
	{
		$ptrcod = "'".trim($registro["IT-CO-PROGRAMA-TRABALHO-RESUMIDO"])."'";
		if (str_replace(" ","",$ptrcod)=="''")
						$ptrcod = 'NULL';
		
		$ptrano = $ano_referencia;
		
		$unicod = "'".trim($registro["GR-UNIDADE-ORCAMENTARIA"])."'";
		if (str_replace(" ","",$unicod)=="''")
						$unicod = 'NULL';
		
		$ptrprgtrabalho = "'".trim($registro["GR-PROGRAMA-TRABALHO-A"])."'";
		if (str_replace(" ","",$ptrprgtrabalho)=="''")
						$ptrprgtrabalho = 'NULL';
	
		
		$irpcod = "'".trim($registro["IT-IN-RESULTADO-PRIMARIO"])."'";
		if (str_replace(" ","",$irpcod)=="''")$irpcod = 'NULL';
		
		
		$ptrtipocreditonum = "'".trim($registro["IT-IN-TIPO-CREDITO"])."'";
		if (str_replace(" ","",$ptrtipocreditonum)=="''")
						$ptrtipocreditonum = 'NULL';
		

		$funcod = "'".substr(trim($registro["GR-PROGRAMA-TRABALHO-A"]),0,2)."'";
		if (str_replace(" ","",$funcod)=="''")
						$funcod = 'NULL';
						
		$sfucod = "'".substr(trim($registro["GR-PROGRAMA-TRABALHO-A"]),2,3)."'";
		if (str_replace(" ","",$sfucod)=="''")
						$sfucod = 'NULL';
							
		$prgcod = "'".substr(trim($registro["GR-PROGRAMA-TRABALHO-A"]),5,4)."'";
		if (str_replace(" ","",$prgcod)=="''")
						$prgcod = 'NULL';
						
		$acacod = "'".substr(trim($registro["GR-PROGRAMA-TRABALHO-A"]),9,4)."'";
		if (str_replace(" ","",$acacod)=="''")
						$acacod = 'NULL';				
		
		$loccod = "'".substr(trim($registro["GR-PROGRAMA-TRABALHO-A"]),13,4)."'";
		if (str_replace(" ","",$loccod)=="''")
						$loccod = 'NULL';
						
						
		/*funcod char(2),
		sfucod char(3),
		prgcod char(4),
		acacod char(4),
		loccod char(4),*/
						
						
		//constroi sql
		
		
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
	
						$gravar = fwrite($arquivo2,$sql_ptr."\n");	
						$Navegador_registros++;
						
		}/*else 
		{
				$sql_ptr = "update public.ptres set " .
							 " ptrcod = ". $ptrcod
							.", ptrano = ".$ptrano
							.", unicod = ".$unicod
							.", ptrprgtrabalho = ". $ptrprgtrabalho
							.", irpcod = ". $irpcod//??? erro
							.", ptrtipocreditonum = ". $ptrtipocreditonum
							.",funcod = ". $funcod
							.",sfucod = ". $sfucod
							.",prgcod = ". $prgcod
							.",acacod = ". $acacod
							.",loccod = ". $loccod
							.", ptrstatus = 'N' "
							." where ptrcod = ".$ptrcod." and ptrano = ".$ano_referencia.";";
										
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


	//fclose($arquivo1);
	fclose($arquivo2);
	ImprimeStatus("formul.txtstatus",'Criado script de '. $Navegador . ' registros de '.$nomearquivo);
	
	return $Navegador_registros;
}



/**
 * Inicio - Importação
 */

function RodaPTRES()
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
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_ptres_'.$ano_referencia.'.sql');
	
	
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
		      if ($file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && (substr($file,0,5)=='ptres')) //evita leitura de . e .. e pega somente os arquivos de conta
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
				   $matrix[$file]['logarquivotipo'] = "ptres";
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
		
		
		
		$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_ptres_'.$ano_referencia.'.sql','w+');
		$sql_ptres_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo2,$sql_ptres_cab);
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
				
				$val = ImportacaoPTRES($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
				
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
			$dados1['logarquivotipo'] = "ptres";
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
			
		
			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_ptres_'.$ano_referencia.'.sql','roda_ptres_'.$ano_referencia.'.sql')){
					
				//ImprimeStatus("formul.txtstatus",'Otimizando BD.');
					
				//exec('vacuumdb -U postgres -f -v -z importdados');
				//exec('mv ptres*.* ./REALIZADO/');
				
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
				
				ImprimeStatus("formul.txtstatus",'gravando log');
				
				gravarLog($dados1,2);
				
				
				ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				//exec('vacuumdb -d simec -f -v -z -table financeiro.ptres') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				//exec('reindex simec financeiro.ptres') ;
				
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$SIAF_DIR_TXT_FILES.'ptres*.* ');
				exec('mv '.$SIAF_DIR_TXT_FILES.'ptres*.* '.$SIAF_DIR_TXT_FILES.'/REALIZADO');
						
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











