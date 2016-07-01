<?php

$SIAF_DIR_TXT_FILES = SIAF_DIR_TXT_FILES.'APOIO'.$ano_referencia.'/' ;

/**
 * Função Principal que reune os metodos para importação de um arquivo, retornando o script de execução
 *
 * @param unknown_type $nomearquivo
 * @param unknown_type $dataatualizacao
 */


function ImportacaoPC($nomearquivo,$dataatualizacao,$ano_referencia)
{
	
	global $objBar1;
	global $BancoImportacao;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	
	$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_PC_'.$ano_referencia.'.sql','a');
		
	$DadosPC = new ImportacaoArquivo();
	
	$dados = $DadosPC->importarArquivo( $nomearquivo );
	$Navegador=0;
	$script_import = "";
	$script_final = "";
	
	
		
	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	$objBar1->setProgress($proximo);

	foreach ($dados as $registro)
	{
		
		

		
		$concod = "'".trim($registro["GR-CODIGO-CONTA"])."'";
		if (str_replace(" ","",$concod )=="''")
						$concod  = 'NULL';
		
		$condsc = trim($registro["IT-NO-CONTA"]);
		$condsc = str_replace("\\","/",$condsc); // Substitue \ por \\  para interpertar no script
		$condsc = str_replace("'"," ",$condsc); // Substitue ' por \'  para interpertar no script
		$condsc = "'".$condsc."'";
		if (str_replace(" ","",$condsc)=="''")
						$condsc = 'NULL';
		
		$confuncao = trim($registro["IT-TX-FUNCAO-CONTA"]);
		$confuncao = str_replace("\\","/",$confuncao); // Substitue \ por \\  para interpertar no script
		$confuncao = str_replace("'"," ",$confuncao); // Substitue ' por \'  para interpertar no script
		$confuncao = "'".$confuncao."'";
		if (str_replace(" ","",$confuncao)=="''")
						$confuncao = 'NULL';
		
	
		$condebito = trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(1)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(2)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(3)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(4)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(5)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(6)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(7)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(8)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(9)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(10)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(11)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(12)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(13)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(14)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(15)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(16)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(17)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(18)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(19)"]).trim($registro["IT-TX-CIRCUNSTANCIA-DEBITO(20)"]);
		
		$condebito = str_replace("\\","/",$condebito); // Substitue \ por \\  para interpertar no script
		$condebito = str_replace("'"," ",$condebito); // Substitue ' por \'  para interpertar no script
		$condebito ="'".$condebito."'";
		if (str_replace(" ","",$condebito)=="''")
						$condebito = 'NULL';
		

		$concredito = trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(1)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(2)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(3)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(4)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(5)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(6)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(7)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(8)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(9)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(10)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(11)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(12)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(13)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(14)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(15)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(16)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(17)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(18)"])
						.trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(19)"]).trim($registro["IT-TX-CIRCUNSTANCIA-CREDITO(20)"]);
		
						
		$concredito = str_replace("\\","/",$concredito); // Substitue \ por \\  para interpertar no script
		$concredito = str_replace("'"," ",$concredito); // Substitue ' por \'  para interpertar no script				
		$concredito = "'".$concredito."'";
		if (str_replace(" ","",$concredito)=="''")
						$concredito = 'NULL';

						
		$consigsaldo = trim($registro["IT-TX-SIGNIFICADO-SALDO"]);
		$consigsaldo = str_replace("\\","/",$consigsaldo); // Substitue \ por \\  para interpertar no script
		$consigsaldo = str_replace("'"," ",$consigsaldo); // Substitue ' por \'  para interpertar no script
		$consigsaldo = "'".$consigsaldo."'";
		if (str_replace(" ","",$consigsaldo)=="''")
						$consigsaldo = 'NULL';
							
		$conobs = trim($registro["IT-TX-OBSERVACAO-CONTA"]);
		$conobs = str_replace("\\","/",$conobs); // Substitue \ por \\  para interpertar no script
		$conobs = str_replace("'"," ",$conobs); // Substitue ' por \'  para interpertar no script
		$conobs = "'".$conobs."'";
		if (str_replace(" ","",$conobs)=="''")
						$conobs = 'NULL';
						
		$tcccod = "'".trim($registro["IT-IN-CONTA-CORRENTE-CONTABIL"])."'";
		if (str_replace(" ","",$tcccod)=="''")
						$tcccod = 'NULL';				
		
		$conencerramentosit = "'".trim($registro["IT-IN-ENCERRAMENTO"])."'";
		if (str_replace(" ","",$conencerramentosit)=="''")
						$conencerramentosit = 'NULL';

		$coninversaosaldosit = "'".trim($registro["IT-IN-INVERSAO-SALDO"])."'";
		if (str_replace(" ","",$coninversaosaldosit)=="''")
						$coninversaosaldosit = 'NULL';
						
		$conescrituracaosit = "'".trim($registro["IT-IN-ESCRITURACAO"])."'";
		if (str_replace(" ","",$conescrituracaosit)=="''")
						$conescrituracaosit = 'NULL';
						
		$consaldosit = "'".trim($registro["IT-IN-SALDO-CONTABIL"])."'";
		if (str_replace(" ","",$consaldosit )=="''")
						$consaldosit  = 'NULL';
		
		$consistemacontabilsit = "'".trim($registro["IT-IN-SISTEMA-CONTABIL"])."'";
		if (str_replace(" ","",$consistemacontabilsit)=="''")
						$consistemacontabilsit = 'NULL';

		$orgcod = "'".trim($registro["IT-CO-ORGAO-CONTA(1)"])."'";
		if (str_replace(" ","",$orgcod)=="''")
						$orgcod = 'NULL';				
		
		$conrestricaoconveniosit = "'".trim($registro["IT-IN-RESTRICAO-CONVENIO"])."'";
		if (str_replace(" ","",$conrestricaoconveniosit)=="''")
						$conrestricaoconveniosit = 'NULL';
		
		$conpagcontraentregasit = "'".trim($registro["IT-IN-PAG-CONTRA-ENTREGA"])."'";
		if (str_replace(" ","",$conpagcontraentregasit)=="''")
						$conpagcontraentregasit = 'NULL';
						
		$conindresultpreimario = "'".trim($registro["IT-IN-RESULTADO-PRIMARIO"])."'";
		if (str_replace(" ","",$conindresultpreimario)=="''")
						$conindresultpreimario = 'NULL';
		
		$condespesaespecialsit = "'".trim($registro["IT-IN-DESPESA-ESPECIAL"])."'";
		if (str_replace(" ","",$condespesaespecialsit)=="''")
						$condespesaespecialsit = 'NULL';
						
		$ndpcod = "'".trim($registro["SB-NATUREZA-DESPESA"])."'";
		if (str_replace(" ","",$ndpcod)=="''")
						$ndpcod = 'NULL';	

		$conano	= "'".$ano_referencia."'";
	
		//verifica se ja foi feita a busca deste codigo
		if (($concod != 'NULL')and ($conano!='NULL'))
		{
			
			$testebusca = false;
			$busca_ptr = "Select concod from financeiro.planoconta  where concod = ".$concod." and conano = ".$conano.";";
			if(($RS = pg_query($BancoImportacao,$busca_ptr)) && (pg_num_rows($RS)>=1)) {
				
				$testebusca = true;
			}
			
			
		}else 
		{
			$testebusca = false;
		}
		
		
		
		if ($testebusca == false )
		{
			
			

			$sql_ptr = " insert into financeiro.planoconta ("
						."conano"
						.",concod" 
						.",condsc"
						.",confuncao"
						.",condebito"
						.",concredito"
						.",consigsaldo"
						.",conobs"
						.",tcccod"
						.",conencerramentosit"
						.",coninversaosaldosit" 
						.",conescrituracaosit"
						.",consaldosit"
						.",consistemacontabilsit"
						.",orgcod"
						.",conrestricaoconveniosit"
						.",conpagcontraentregasit"
						.",conindresultpreimario"
						.",condespesaespecialsit"
						.",ndpcod"
						.") values(".
						 $conano
						.",".$concod
						.",".$condsc
						.",".$confuncao
						.",".$condebito
						.",".$concredito
						.",".$consigsaldo
						.",".$conobs
						.",".$tcccod
						.",".$conencerramentosit
						.",".$coninversaosaldosit
						.",".$conescrituracaosit
						.",".$consaldosit
						.",".$consistemacontabilsit
						.",".$orgcod
						.",".$conrestricaoconveniosit
						.",".$conpagcontraentregasit
						.",".$conindresultpreimario
						.",".$condespesaespecialsit
						.",".$ndpcod.");";
	
				
		}else 
		{
				$sql_ptr = "update financeiro.planoconta set " .
						  	"conano = ".$conano
							.",concod = " .$concod
							.",condsc = ".$condsc
							.",confuncao = ".$confuncao
							.",condebito = ".$condebito
							.",concredito = ".$concredito
							.",consigsaldo = ".$consigsaldo
							.",conobs = ".$conobs
							.",tcccod = ".$tcccod
							.",conencerramentosit = ".$conencerramentosit
							.",coninversaosaldosit = ".$coninversaosaldosit
							.",conescrituracaosit = ".$conescrituracaosit
							.",consaldosit = ".$consaldosit
							.",consistemacontabilsit = ".$consistemacontabilsit
							.",orgcod = ".$orgcod
							.",conrestricaoconveniosit = ".$conrestricaoconveniosit
							.",conpagcontraentregasit = ".$conpagcontraentregasit
							.",conindresultpreimario = ".$conindresultpreimario
							.",condespesaespecialsit = ".$condespesaespecialsit
							.",ndpcod = ".$ndpcod
							." where conano = ".$conano." and concod = ".$concod.";";
										
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

function RodaPC()
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
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_PC_'.$ano_referencia.'.sql');
	
	
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
		      if ($file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && (substr($file,0,2)=='pc')) //evita leitura de . e .. e pega somente os arquivos de conta
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
					   $matrix[$file]['logarquivotipo'] = "pc";
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
		
			
		$arquivo2 = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_PC_'.$ano_referencia.'.sql','w+');
		
		//coloca permisão de acesso full ao arquivo
		
		$sql_PC_cab = "SET client_encoding TO 'LATIN5';\n";
		$gravar = fwrite($arquivo2,$sql_PC_cab);
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
				
				$val = ImportacaoPC($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
				
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
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
			$dados1['logdataimport']="'".$DataInicioImporta."'";
			$dados1['logarquivotipo'] = "pc";
			
			
			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_PC_'.$ano_referencia.'.sql','roda_PC_'.$ano_referencia.'.sql')){
			
					
				
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
							
				ImprimeStatus("formul.txtstatus",'gravando log');
		
				
				gravarLog($dados1,2);
				
			
				ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				//exec('vacuumdb -d simec -f -v -z -table financeiro.planoconta') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				//exec('reindex simec financeiro.planoconta') ;
				
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$SIAF_DIR_TXT_FILES.'pc*.* ');
				exec('mv '.$SIAF_DIR_TXT_FILES.'pc*.* '.$SIAF_DIR_TXT_FILES.'/REALIZADO');
						
				ImprimeStatus("formul.txtstatus",'Importação Realizada com Sucesso');
				return ($sucessoImport);
					
			}else {
				
				gravarLog($dados1,3);
				
				ImprimeStatus("formul.txtstatus",$ErroImport[12]);
				return $ErroImport[12];
			}
	
		}
			
	}
}
?>











