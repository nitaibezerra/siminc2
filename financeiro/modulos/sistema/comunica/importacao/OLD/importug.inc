<?php

$SIAF_DIR_TXT_FILES = SIAF_DIR_TXT_FILES.'APOIO'.$ano_referencia.'/' ;

/**
 * Função Principal que reune os metodos para importação de um arquivo, retornando o script de execução
 *
 * @param unknown_type $nomearquivo
 * @param unknown_type $dataatualizacao
 */


function ImportacaoUG($nomearquivo,$dataatualizacao,$ano_referencia)
{
	global $db;
	global $objBar1;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_ug_'.$ano_referencia.'.sql','a');
		
	$Dadosug = new ImportacaoArquivo();
	
	$dados = $Dadosug->importarArquivo( $nomearquivo );
	$Navegador=0;
	$script_import = "";
	$script_final = "";
	
	global $BancoImportacao;
		
	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);

	foreach ($dados as $registro)
	{
		
		$ungcod = "'".trim($registro["IT-CO-UNIDADE-GESTORA"])."'";
		if (str_replace(" ","",$ungcod )=="''")
						$ungcod  = 'NULL';
		
		$itnounidadegestora = trim($registro["IT-NO-UNIDADE-GESTORA"]);
		$itnounidadegestora = str_replace("\\","/",$itnounidadegestora); // Substitue \ por \\  para interpertar no script
		$itnounidadegestora = str_replace("'"," ",$itnounidadegestora); // Substitue ' por \'  para interpertar no script
		$itnounidadegestora = "'".$itnounidadegestora."'";
		if (str_replace(" ","",$itnounidadegestora)=="''")
						$itnounidadegestora = 'NULL';
		
		$itnomnemonicounidadegestora = trim($registro["IT-NO-MNEMONICO-UNIDADE-GESTORA"]);
		$itnomnemonicounidadegestora = str_replace("\\","/",$itnomnemonicounidadegestora); // Substitue \ por \\  para interpertar no script
		$itnomnemonicounidadegestora = str_replace("'"," ",$itnomnemonicounidadegestora); // Substitue ' por \'  para interpertar no script
		$itnomnemonicounidadegestora = "'".$itnomnemonicounidadegestora."'";
		if (str_replace(" ","",$itnomnemonicounidadegestora)=="''")
						$itnomnemonicounidadegestora = 'NULL';
		
		//$itinsituacaounidadegestora = "'".trim($registro["IT-IN-SITUACAO-UNIDADE-GESTORA"])."'";
		//if (str_replace(" ","",$itinsituacaounidadegestora)=="''")
		//				$itinsituacaounidadegestora = 'NULL';				
		
		$itnucgc = "'".trim($registro["IT-NU-CGC"])."'";
		if (str_replace(" ","",$itnucgc)=="''")
						$itnucgc = 'NULL';

		$itcopais = "'".trim($registro["IT-CO-PAIS"])."'";
		if (str_replace(" ","",$itcopais)=="''")
						$itcopais = 'NULL';
						
		$itcouf = "'".trim($registro["IT-CO-UF"])."'";
		if (str_replace(" ","",$itcouf)=="''")
						$itcouf = 'NULL';
						
		$orgcod = "'".trim($registro["GR-ORGAO"])."'";
		if (str_replace(" ","",$orgcod )=="''")
						$orgcod  = 'NULL';
		
		$itcounidadegestorasetoorca = "'".trim($registro["IT-CO-UNIDADE-GESTORA-SETO-ORCA"])."'";
		if (str_replace(" ","",$itcounidadegestorasetoorca)=="''")
						$itcounidadegestorasetoorca = 'NULL';

		$itcounidadegestorasetoaudi = "'".trim($registro["IT-CO-UNIDADE-GESTORA-SETO-AUDI"])."'";
		if (str_replace(" ","",$itcounidadegestorasetoaudi)=="''")
						$itcounidadegestorasetoaudi = 'NULL';				
		
		$itcounidadegestorasetocont = "'".trim($registro["IT-CO-UNIDADE-GESTORA-SETO-CONT"])."'";
		if (str_replace(" ","",$itcounidadegestorasetocont)=="''")
						$itcounidadegestorasetocont = 'NULL';
		
		$itcounidadegestorasetofina = "'".trim($registro["IT-CO-UNIDADE-GESTORA-SETO-FINA"])."'";
		if (str_replace(" ","",$itcounidadegestorasetofina)=="''")
						$itcounidadegestorasetofina = 'NULL';
						
		$itcounidadegestorapolo = "'".trim($registro["IT-CO-UNIDADE-GESTORA-POLO"])."'";
		if (str_replace(" ","",$itcounidadegestorapolo)=="''")
						$itcounidadegestorapolo = 'NULL';
		
						
		$itedendereco = trim($registro["IT-ED-ENDERECO"]);
		$itedendereco = str_replace("\\","/",$itedendereco); // Substitue \ por \\  para interpertar no script
		$itedendereco = str_replace("'"," ",$itedendereco); // Substitue ' por \'  para interpertar no script
		$itedendereco = "'".$itedendereco."'";
		if (str_replace(" ","",$itedendereco)=="''")
						$itedendereco = 'NULL';					
	
		$itinmudancaorgao = "'".trim($registro["IT-IN-MUDANCA-ORGAO"])."'";
		if (str_replace(" ","",$itinmudancaorgao)=="''")
						$itinmudancaorgao = 'NULL';
						
		$itnucpfordenadorass = "'".trim($registro["IT-NU-CPF-ORDENADOR-ASS"])."'";
		if (str_replace(" ","",$itnucpfordenadorass)=="''")
						$itnucpfordenadorass = 'NULL';	
						
		$itnucpfgestorfinanceiro = "'".trim($registro["IT-NU-CPF-GESTOR-FINANCEIRO"])."'";
		if (str_replace(" ","",$itnucpfgestorfinanceiro)=="''")
						$itnucpfgestorfinanceiro = 'NULL';	
		
		$itcoorgaosiorg = "'".trim($registro["IT-CO-ORGAO-SIORG"])."'";
		if (str_replace(" ","",$itcoorgaosiorg)=="''")
						$itcoorgaosiorg = 'NULL';	
						
		$moedacod = "'".trim($registro["IT-CO-MOEDA"])."'";
		if (str_replace(" ","",$moedacod)=="''")
						$moedacod = 'NULL';	
						
		$itcomunicipio = "'".trim($registro["IT-CO-MUNICIPIO"])."'";
		if (str_replace(" ","",$itcomunicipio)=="''")
						$itcomunicipio = 'NULL';	
						
		$itcougvinculacao = "'".trim($registro["IT-CO-UG-VINCULACAO"])."'";
		if (str_replace(" ","",$itcougvinculacao)=="''")
						$itcougvinculacao = 'NULL';	
						
						
		$itcougcontroleinterno = "'".trim($registro["IT-CO-UG-CONTROLE-INTERNO"])."'";
		if (str_replace(" ","",$itcougcontroleinterno)=="''")
						$itcougcontroleinterno = 'NULL';	
						
		$grunidadeorcamentaria = "'".trim($registro["GR-UNIDADE-ORCAMENTARIA"])."'";
		if (str_replace(" ","",$grunidadeorcamentaria)=="''")
						$grunidadeorcamentaria = 'NULL';	
						
			
		//verifica se ja foi feita a busca deste codigo
		if ($ungcod != 'NULL')
		{
			
			$testebusca = false;
			$busca_ptr = "Select ungcod from financeiro.unidadegestora where ungcod = ".$ungcod.";";
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
			
			

			$sql_ptr = "INSERT INTO financeiro.unidadegestora("
            			."ungcod,"
            			."itnounidadegestora,"
            			."itnomnemonicounidadegestora,"
            			//."itinsituacaounidadegestora,"
            			."itnucgc,"
            			."itcopais,"
            			."itcouf,"
            			."orgcod,"
            			."itcounidadegestorasetoorca,"
            			."itcounidadegestorasetoaudi,"
            			."itcounidadegestorasetocont,"
            			."itcounidadegestorasetofina,"
            			."itcounidadegestorapolo,"
            			."itedendereco,"
            			."itinmudancaorgao,"
            			."itnucpfordenadorass,"
            			."itnucpfgestorfinanceiro,"
            			."itcoorgaosiorg,"
            			."moedacod,"
            			."itcomunicipio,"
            			."itcougvinculacao,"
            			."itcougcontroleinterno,"
            			."grunidadeorcamentaria)
            			VALUES (".
						$ungcod
            			.",".$itnounidadegestora
            			.",".$itnomnemonicounidadegestora
            			.",".$itinsituacaounidadegestora
            			.",".$itnucgc
            			.",".$itcopais
            			.",".$itcouf
            			.",".$orgcod
            			.",".$itcounidadegestorasetoorca
            			.",".$itcounidadegestorasetoaudi
            			.",".$itcounidadegestorasetocont
            			.",".$itcounidadegestorasetofina
            			.",".$itcounidadegestorapolo
            			.",".$itedendereco
            			.",".$itinmudancaorgao
            			.",".$itnucpfordenadorass
            			.",".$itnucpfgestorfinanceiro
            			.",".$itcoorgaosiorg
            			.",".$moedacod
            			.",".$itcomunicipio
            			.",".$itcougvinculacao
            			.",".$itcougcontroleinterno
            			.",".$grunidadeorcamentaria.");";
	
				
		}else 
		{
				$sql_ptr = "UPDATE financeiro.unidadegestora SET ".
							"ungcod=".$ungcod
							.",itnounidadegestora=".$itnounidadegestora
							.",itnomnemonicounidadegestora=".$itnomnemonicounidadegestora
							//.",itinsituacaounidadegestora=".$itinsituacaounidadegestora
							.",itnucgc=".$itnucgc
							.",itcopais=".$itcopais
							.",itcouf=".$itcouf
       						.",orgcod=".$orgcod
       						.",itcounidadegestorasetoorca=". $itcounidadegestorasetoorca
       						.",itcounidadegestorasetoaudi=". $itcounidadegestorasetoaudi
       						.",itcounidadegestorasetocont=". $itcounidadegestorasetocont
       						.",itcounidadegestorasetofina=". $itcounidadegestorasetofina
       						.",itcounidadegestorapolo=". $itcounidadegestorapolo
       						.",itedendereco=". $itedendereco
       						.",itinmudancaorgao=". $itinmudancaorgao
       						.",itnucpfordenadorass=". $itnucpfordenadorass
       						.",itnucpfgestorfinanceiro=". $itnucpfgestorfinanceiro
       						.",itcoorgaosiorg=". $itcoorgaosiorg
       						.",moedacod=". $moedacod
       						.",itcomunicipio=". $itcomunicipio
       						.",itcougvinculacao=".$itcougvinculacao
       						.",itcougcontroleinterno=". $itcougcontroleinterno
       						.",grunidadeorcamentaria=" .$grunidadeorcamentaria 
       						."WHERE ungcod = ".$ungcod.";";
										
		}
					
		$gravar = fwrite($arquivo2,$sql_ptr."\n");
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


	fclose($arquivo2);
	ImprimeStatus("formul.txtstatus",'Criado script de '. $Navegador . ' registros de '.$nomearquivo);
	
	return $Navegador_registros;	
}



/**
 * Inicio - Importação
 */

function RodaUG()
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
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_ug_'.$ano_referencia.'.sql');
	
	
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
		      if ($file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && (substr($file,0,2)=='ug')) //evita leitura de . e .. e pega somente os arquivos de conta
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
					   $matrix[$file]['logarquivotipo'] = "ug";
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
		
			
		$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_ug_'.$ano_referencia.'.sql','w+');
		$sql_ug_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo2,$sql_ug_cab);
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
		
		
		//if (true)
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
				
				$val = ImportacaoUG($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
				
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
			$dados1['logarquivotipo'] = "ug";
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
					
			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_ug_'.$ano_referencia.'.sql','roda_ug_'.$ano_referencia.'.sql')){
					
				//ImprimeStatus("formul.txtstatus",'Otimizando BD.');
					
				//exec('vacuumdb -U postgres -f -v -z importdados');
				//exec('mv c*.* ./REALIZADO/');
				
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
				
				ImprimeStatus("formul.txtstatus",'gravando log');
		
				gravarLog($dados1,2);
							
				ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				//exec('vacuumdb -d simec -f -v -z -table financeiro.unidadegestora') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				//exec('reindex simec financeiro.unidadegestora') ;
				
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$SIAF_DIR_TXT_FILES.'ug*.* ');
				exec('mv '.$SIAF_DIR_TXT_FILES.'ug*.* '.$SIAF_DIR_TXT_FILES.'/REALIZADO');
						
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











