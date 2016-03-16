<?php
$Origem_Arquivos = SIAF_DIR_TXT_FILES.$ano_referencia."/" ;




/**
 * Variavel Global para fins de otimização, evitando 
 * consultas na tabela de referencia.
 * 
 *  */
$Otimizacao_1 = array();


function ImportacaoSaldo($nomearquivo,$dataatualizacao,$ano_referencia)
{

	global $Otimizacao_1;
	$Navegador_registros = 0;//Grava a quantidade de registros processados
	//abre aquivos de importacao
	
	for ($i =0; $i<=13;$i++)
	{
		if ($i<10){ $val = '0'.$i.''; $arquivo = 'arquivo'.$val; } else {$val = $i;$arquivo = 'arquivo'.$val;};
		${$arquivo} = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_saldo_diario_'.$ano_referencia.'_'.$val.'.sql','a');
	}
	
		
	$NotasContabil = new ImportacaoArquivo();
	$dados = $NotasContabil->importarArquivo( $nomearquivo );
	$script_import = "";
	$script_final = "";

	
	$tamanho_array = sizeof($dados);
	$proximo = 0;
	//$objBar1->setProgress($proximo);
	
	
	
	$armazenar = false;//controle se os dados da memoria foram salvos
	$sql_saldocontabil = array();//armazena as linhas a serem inseridas na memoria
	$Contador_Armazenamento = 0; //Flag para controlar a quantidade de registros na memoria
	
	foreach ($dados as $registro)
	{
					
		$armazenar = false;
				
		if (is_null($Otimizacao_1[$registro["GR-CODIGO-CONTA"]]))
		{
			
			
			$sql = "SELECT
				financeiro.planoconta.concod, 
				importacao.tipocontacorrcontabdetalhe.tclid, 
				importacao.tipocontacorrcontabdetalhe.tcccod, 
				importacao.tipocontacorrcontabdetalhe.tcdid, 
				importacao.tipocontacorrcontabdetalhe.tclposini as inicio, 
				importacao.tipocontacorrcontabdetalhe.tcltamanho as tamanho, 
				importacao.tabelacampodestino.tcdtabela as tabela_d, 
				importacao.tabelacampodestino.tcdcampo as campo_d,  
				importacao.tabelacampodestino.tcdtipocampo 
				FROM 
				financeiro.planoconta 
				Inner Join importacao.tipocontacorrentecontabil ON importacao.tipocontacorrentecontabil.tcccod = financeiro.planoconta.tcccod 
				Inner Join importacao.tipocontacorrcontabdetalhe ON importacao.tipocontacorrcontabdetalhe.tcccod = importacao.tipocontacorrentecontabil.tcccod 
				Inner Join importacao.tabelacampodestino ON importacao.tabelacampodestino.tcdid = importacao.tipocontacorrcontabdetalhe.tcdid 
				where 
				financeiro.planoconta.concod ='".$registro["GR-CODIGO-CONTA"]."' 
				Order by 
				importacao.tipocontacorrcontabdetalhe.tclposini ASC, 
				importacao.tabelacampodestino.tcdcampo ASC";
			
		
			$DadosDestino= carregar($sql);
			$Otimizacao_1[$registro["GR-CODIGO-CONTA"]] =$DadosDestino;
		}else 
		{
			$DadosDestino=$Otimizacao_1[$registro["GR-CODIGO-CONTA"]];
		}
		
		$ccc=array();
		//variaveis para construção.
		$ccc['s_tcccod'] = "\\N";	
		$ccc['s_cagcod'] = "\\N";
  		$ccc['s_iducod'] = "\\N";
  		$ccc['s_grfcod'] = "\\N";
  		$ccc['s_frscod'] = "\\N";
  		$ccc['s_frsgrcod'] = "\\N";
  		$ccc['s_vincod'] = "\\N";
  		$ccc['s_ntrcod'] = "\\N";
  		$ccc['s_esfcod'] = "\\N";
  		$ccc['s_unicod'] = "\\N";
  		$ccc['s_irpcod'] = "\\N";
    	$ccc['s_tarcod'] = "\\N";
    	$ccc['s_ptrcod'] = "\\N";
    	$ccc['s_ndpcod'] = "\\N";
    	$ccc['s_trrcod'] = "\\N";
    	$ccc['s_cccmes'] = "\\N";
    	$ccc['s_noenumoriginal']= "\\N";
    	$ccc['s_orgcod']= "\\N";
    	$ccc['s_npenumoriginal']= "\\N";
    	$ccc['s_ungcodresponsavel']= "\\N";
    	$ccc['s_ungcoddocumento']= "\\N";
    	$ccc['s_gstcoddocumento']= "\\N";
		
    	        	
		if ($DadosDestino != false)
		{
							
			if ((str_replace(" ","",$DadosDestino[0]['tcccod']) == '') || is_null($DadosDestino[0]['tcccod']))
			{
				$ccc['s_tcccod'] = "\\N";
			}else{
				$ccc['s_tcccod'] = $DadosDestino[0]['tcccod'];
			}
			
			foreach ($DadosDestino as $registro2){ 
				
				$ccc['s_'.$registro2['campo_d']] = trim(substr($registro["IT-CO-CONTA-CORRENTE-CONTABIL"],$registro2["inicio"],$registro2["tamanho"]));
				
				//trata exceção do campo ndpcod, cujo o tamanho pode ser 6 ou 8
				if (($registro2['campo_d'] == 'ndpcod') && ($registro2["tamanho"])==6)
				{
					$ccc['s_'.$registro2['campo_d']] = '00'.trim(substr($registro["IT-CO-CONTA-CORRENTE-CONTABIL"],$registro2["inicio"],$registro2["tamanho"]));
				}
							
				//trata campos vazios para nulo, caso for vazio
				$x3 = str_replace(' ','',$ccc['s_'.$registro2['campo_d']]);
				if(sizeof($x3)==0) $ccc['s_'.$registro2['campo_d']] ='\\N';
				
				
			}
			
		}
		

		
		
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
								
			//conversão de data
		    if(!is_null(trim($registro["IT-DA-TRANSACAO"])))
			{
				$anoc = substr($registro["IT-DA-TRANSACAO"],0,4);
				$mesc = substr($registro["IT-DA-TRANSACAO"],4,2);
				$diac = substr($registro["IT-DA-TRANSACAO"],6,2);
			}else{
				$anoc = $ano_referencia;
				$mesc = '12';
				$diac = '31';
			}
			
			if(!is_null(trim($registro["IT-HO-TRANSACAO"])))
			{
				$horc = substr($registro["IT-HO-TRANSACAO"],0,2);
				$minc = substr($registro["IT-HO-TRANSACAO"],2,2);
				$segc = substr($registro["IT-HO-TRANSACAO"],4,2);
			}else{
				$horc = '00';
				$minc = '00';
				$segc = '00';
			}
			
			
			$DataTransacao = $anoc."-".$mesc."-".$diac." ".$horc.":".$minc.":".$segc;
			
			
			//Coloca '0' na frente dos meses menores que 10	
			if ($mes < 10) {$mes_0= '0'.$mes;} else{$mes_0=$mes;}
			
				if (!(((((float) $registro[$campo1])/100 ) == 0) && ((((float) $registro[$campo2])/100 )==0)))
				{
			
					$sql_saldocontabil[$mes] = $sql_saldocontabil[$mes].trim($registro["IT-CO-UNIDADE-GESTORA"])."\t".
								trim($registro["GR-CODIGO-CONTA"])."\t".
								trim($registro["IT-CO-CONTA-CORRENTE-CONTABIL"])."\t".
								trim($registro["IT-CO-GESTAO"])."\t".
								$ano_referencia."\t".
								$mes_0."\t".
								((float) $registro[$campo1])/100 ."\t".
								((float) $registro[$campo2])/100 ."\t".
								str_replace("'","",$dataatualizacao)."\t".
								$DataTransacao."\t".
								$ccc['s_tcccod']."\t".
								$ccc['s_cagcod']."\t".
  								$ccc['s_iducod']."\t".
  								$ccc['s_grfcod']."\t".
  								$ccc['s_frscod']."\t".
  								$ccc['s_frsgrcod']."\t".
  								$ccc['s_vincod']."\t".
  								$ccc['s_ntrcod']."\t".
  								$ccc['s_esfcod']."\t".
  								$ccc['s_unicod']."\t".
  								$ccc['s_irpcod']."\t".
    							$ccc['s_tarcod']."\t".
    							$ccc['s_ptrcod']."\t".
    							$ccc['s_ndpcod']."\t".
    							$ccc['s_trrcod']."\t".
    							$ccc['s_cccmes']."\t".
    							$ccc['s_noenumoriginal']."\t".
    							$ccc['s_orgcod']."\t".
    							$ccc['s_npenumoriginal']."\t".
    							$ccc['s_ungcodresponsavel']."\t".
    							$ccc['s_ungcoddocumento']."\t".
    							$ccc['s_gstcoddocumento']."\n";
				}
		    					
				$Navegador_registros++;
							
		}
		

		
		
		if ($Contador_Armazenamento >= 50 )
		{
			
			//gravar dados em arquivo especifico
			$gravar = fwrite($arquivo00,$sql_saldocontabil[0]);
			$gravar = fwrite($arquivo01,$sql_saldocontabil[1]);
			$gravar = fwrite($arquivo02,$sql_saldocontabil[2]);
			$gravar = fwrite($arquivo03,$sql_saldocontabil[3]);
			$gravar = fwrite($arquivo04,$sql_saldocontabil[4]);
			$gravar = fwrite($arquivo05,$sql_saldocontabil[5]);
			$gravar = fwrite($arquivo06,$sql_saldocontabil[6]);
			$gravar = fwrite($arquivo07,$sql_saldocontabil[7]);
			$gravar = fwrite($arquivo08,$sql_saldocontabil[8]);
			$gravar = fwrite($arquivo09,$sql_saldocontabil[9]);
			$gravar = fwrite($arquivo10,$sql_saldocontabil[10]);
			$gravar = fwrite($arquivo11,$sql_saldocontabil[11]);
			$gravar = fwrite($arquivo12,$sql_saldocontabil[12]);
			$gravar = fwrite($arquivo13,$sql_saldocontabil[13]);
			
			$sql_saldocontabil[0] = "";
			$sql_saldocontabil[1] = "";
			$sql_saldocontabil[2] = "";
			$sql_saldocontabil[3] = "";
			$sql_saldocontabil[4] = "";
			$sql_saldocontabil[5] = "";
			$sql_saldocontabil[6] = "";
			$sql_saldocontabil[7] = "";
			$sql_saldocontabil[8] = "";
			$sql_saldocontabil[9] = "";
			$sql_saldocontabil[10] = "";
			$sql_saldocontabil[11] = "";
			$sql_saldocontabil[12] = "";
			$sql_saldocontabil[13] = "";
						
			$Contador_Armazenamento = 0;
			$armazenar = true;
		}
				
		$Contador_Armazenamento++;
				
		//Grafico dos Arquivos	
		$Navegador++;
		
			
		if  (ceil(($Navegador) * 100 / $tamanho_array) > ceil($proximo))
		{	
			$proximo = (($Navegador * 100) / $tamanho_array);
			//$objBar1->setProgress($proximo);
		}
		
			
	}
	
	
	//grava dados caso os ultimos registros não estejam armazenados
	if ($armazenar==false)
	{
			$gravar = fwrite($arquivo00,$sql_saldocontabil[0]);
			$gravar = fwrite($arquivo01,$sql_saldocontabil[1]);
			$gravar = fwrite($arquivo02,$sql_saldocontabil[2]);
			$gravar = fwrite($arquivo03,$sql_saldocontabil[3]);
			$gravar = fwrite($arquivo04,$sql_saldocontabil[4]);
			$gravar = fwrite($arquivo05,$sql_saldocontabil[5]);
			$gravar = fwrite($arquivo06,$sql_saldocontabil[6]);
			$gravar = fwrite($arquivo07,$sql_saldocontabil[7]);
			$gravar = fwrite($arquivo08,$sql_saldocontabil[8]);
			$gravar = fwrite($arquivo09,$sql_saldocontabil[9]);
			$gravar = fwrite($arquivo10,$sql_saldocontabil[10]);
			$gravar = fwrite($arquivo11,$sql_saldocontabil[11]);
			$gravar = fwrite($arquivo12,$sql_saldocontabil[12]);
			$gravar = fwrite($arquivo13,$sql_saldocontabil[13]);
			
			$sql_saldocontabil[0] = "";
			$sql_saldocontabil[1] = "";
			$sql_saldocontabil[2] = "";
			$sql_saldocontabil[3] = "";
			$sql_saldocontabil[4] = "";
			$sql_saldocontabil[5] = "";
			$sql_saldocontabil[6] = "";
			$sql_saldocontabil[7] = "";
			$sql_saldocontabil[8] = "";
			$sql_saldocontabil[9] = "";
			$sql_saldocontabil[10] = "";
			$sql_saldocontabil[11] = "";
			$sql_saldocontabil[12] = "";
			$sql_saldocontabil[13] = "";
			
			$Contador_Armazenamento = 0;
			$armazenar = true;	
	}
	
	
	//$objBar1->setProgress(100);
	ImprimeStatus("formul.txtstatus",'Criado script de '. $Navegador*14 . ' registros de '.$nomearquivo);
	ob_flush();
	flush();
	
		
	//fecha todos arquivos abertos
	
	for ($i=0; $i<=13;$i++)
	{
		if ($i<10){ $val = '0'.$i.''; $arquivo = 'arquivo'.$val; } else {$val = $i;$arquivo = 'arquivo'.$val;};
		fclose(${$arquivo});
	}
	
	
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
	global $Origem_Arquivos;
	global $ano_referencia;
	

	
	//$nomearquivo ='sc111355.txt';//'sc111371.txt';//'sc111321.txt';//'SCDIARIO_Saldo_Contabil_20061018.txt';//'sc111371.txt';
	$data = getdate();
	$dataatualizacao = "'".$data[year]."-".$data[mon]."-".$data[mday]."'";
	$Erro_Integridade = false;
	$VerificaDuplo ='';
	$VerificaDuplo = array();//Verifica se arquivo ja foi importado ou não
	$arquivo_i = array();
	$tamanho_arquivos = array();
	$DataInicioImporta = date("Y-m-d H:i:s"); //Chave para tabela de log
	$Matriz_Arquivos = null;
	
	//limpar todos os scripts previos
	rfr(SIAF_DIR_TXT_FILES_DESTINO,'*_saldo_diario'.$ano_referencia.'_*.sql');
	
	set_time_limit(0);
	
	
	
	
	if (!is_dir($Origem_Arquivos))
	{
		
		
		//ImprimeStatus("formul.txtstatus",$ErroImport[16]);
		return ($ErroImport[16]);
	}else 
	{
	 
		
		if ($handle=opendir($Origem_Arquivos))  //abre diretório para leitura
		{
		$x=0;
		
		
		   while (false!==($file=readdir($handle)))//lê arquivos do diretório
		   {
		   	
		   	    
		   	  if ( $file!="." && $file!=".." && substr($file,strlen($file)-4)=='.txt' && strtoupper(substr($file,0,1))=='S') //evita leitura de . e .. e pega somente os arquivos de conta
		      {          
		      	   
		      	   $VerificaDuplo[$file] = verificaLog($Origem_Arquivos,$file,$ano_referencia,'s');
		      					   
				   if ($VerificaDuplo[$file]!=1) 
				   {

					   $Matriz_Arquivos[$file]['logarquivonome']=$file; //armazena nomes dos arquivos na matriz   
					   $Matriz_Arquivos[$file]['logarquivodata'] ="'".date("Y-m-d H:i:s", filectime($Origem_Arquivos.$file))."'";
					   			   
					   $data_atual = date("Y-m-d H:i:s");
					   $Matriz_Arquivos[$file]['logdatafim'] = "Null";
					   $Matriz_Arquivos[$file]['logdataini'] = "'".$data_atual."'";
					   $Matriz_Arquivos[$file]['logimporterros'] = "TRUE";
					   $Matriz_Arquivos[$file]['logerros'] = $ErroImport[8];
					   $Matriz_Arquivos[$file]['loginformacoes'] ="Null";
					   $Matriz_Arquivos[$file]['logarquivoano']= "'".$ano_referencia."'";
					   $Matriz_Arquivos[$file]['logarquivotipo'] = "s";
					   $Matriz_Arquivos[$file]['logatdregistros'] = 0;
					   $Matriz_Arquivos[$file]['caminho']="'".$Origem_Arquivos."'";
					   $Matriz_Arquivos[$file]['logdataimport']="'".$DataInicioImporta."'";
					   $Matriz_Arquivos[$file]['logdatainiscript'] ='Null';
					   $Matriz_Arquivos[$file]['logdatafimscript'] ='Null';
					    
					   $x++;
				   }   
		      } 
		      
		   } 
		   
		     
		  closedir($handle);                              //fecha o diretório
		}
		
	
				
		
		if (!is_null($Matriz_Arquivos))
		{
			$cont = 1;
			//$Matriz_Arquivos =array('sc2005c9.txt','sc2005g9.txt');
			//$DivArquivo = array();//Define pontos de divisão
			//$tamanhototal = 0;  //Variavel calcula o tamanho do arquivo e divide
			
			ImprimeStatus("formul.txtstatus", "Testando integridade dos arquivos.");
			$Erro_Integridade = false;
			
			
			foreach ($Matriz_Arquivos as $nomearquivo)
			{
			
						
				//checar integridade do arquivo "caracter especial			
				if (checar_eof_file($Origem_Arquivos.$nomearquivo['logarquivonome'])){
	
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
				if (!checar_arquivo_ref($Origem_Arquivos.$nomearquivo['logarquivonome'],$matrix[$file]['logarquivotipo']))
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
			
			
			$tamanho_array = sizeof($Matriz_Arquivos);
		
			
			
			foreach ($Matriz_Arquivos as $nomearquivo)
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
				
				
				
				//dbg($Matriz_Arquivos);	
				$val = ImportacaoSaldo($nomearquivo['logarquivonome'],$dataatualizacao,$ano_referencia);	
			 			
				$data_atual = date("Y-m-d H:i:s");
				$nomearquivo['logdatafim'] = "'".$data_atual."'";
				$nomearquivo['logatdregistros'] = $val;
				
			
				gravarLog($nomearquivo);
				ImprimeStatus("formul.txtstatus", 'concluida criação script de '.$nomearquivo['logarquivonome']);
				
				
						
				if  (ceil(($cont) * 100 / $tamanho_array) > ceil($proximo))
				{	
					$proximo = (($cont * 100) / $tamanho_array);
				//	$objBar0->setProgress($proximo);
				}
				
				$cont++;
			}
			
			
			
			//Coloca os rodapes nos arquivos, para fechar o comando copy
			$val = '';
			for ($i = 0; $i<=13;$i++)
			{
				
				if ($i<10){ $val = '0'.$i.'';} else {$val = $i;};
				$arquivo_rod = fopen(SIAF_DIR_TXT_FILES_DESTINO.'dados_saldo_diario_'.$ano_referencia.'_'.$val.'.sql','a');
				$sql_saldocontabil_rod = "\.\n";
				$gravar = fwrite($arquivo_rod,$sql_saldocontabil_rod);
				fclose($arquivo_rod);
			}
			
				
			
			ImprimeStatus("formul.txtstatus", "Fim do processo de Criação de Script");
			
			
			ImprimeStatus("formul.txtstatus",'Rodando Scripts de Carga');
		
			
			$dados1 = array();
			$dados1['logdataimport']="'".$DataInicioImporta."'";
			$dados1['logarquivotipo'] = "s";
			$dados1['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
			

			if (RodarQuery(SIAF_DIR_TXT_FILES_DESTINO,'dados_saldo_diario_'.$ano_referencia.'_*.sql')){
				
				//ImprimeStatus("formul.txtstatus",'Otimizando BD.');
				
				//exec('vacuumdb -U postgres -f -v -z importdados');
				
				$dados1['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($dados1,4);
				
				ImprimeStatus("formul.txtstatus",'gravando log');
		
				gravarLog($dados1,2);
				
				//exec('mv s*.* ./REALIZADO/');
				
				
				
				/*ImprimeStatus("formul.txtstatus",'Aplicando Vacuum');
				
				for ($i = 0; $i<=13;$i++)
				{
					if ($i<10){ $val = '0'.$i;} else {$val = $i;};
					//exec("vacuumdb -d simec -f -v -z -table particionamento.saldocontabil".$ano_referencia.$val);
				}
				
				//exec('vacuumdb -d simec -f -v -z -table financeiro.planoconta') ;
				
				ImprimeStatus("formul.txtstatus",'Otimizando Indices');
				
				for ($i = 0; $i<=13;$i++)
				{
					if ($i<10){ $val = '0'.$i;} else {$val = $i;};
					//exec("reindex simec particionamento.saldocontabil".$ano_referencia.$val);
				}
				
				//exec('reindex simec financeiro.planoconta') ;
				*/
				ImprimeStatus("formul.txtstatus",'Movendo Arquivos ja realizados');
				exec('chmod 777 '.$Origem_Arquivos.'s*.* ');
				exec('mv '.$Origem_Arquivos.'s*.* '.$Origem_Arquivos.'/REALIZADO');
						
				
				//$objBar0->setProgress(100);
				
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