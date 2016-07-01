<?php
include APPRAIZ . "includes/cabecalho.inc";

print '<br/>';
monta_titulo ( 'Importar Preenchimento de Crédito', '<form method="post" action="elabrev.php?modulo=principal/importacaocredito/upload&acao=A&execucao=upload"
	enctype="multipart/form-data">
	<label>Arquivo</label> <input type="file" name="arquivo" /> <input
		type="submit" value="Enviar Arquivo" />
</form>' );

$execucao = $_REQUEST ['execucao'];

// Se a execução for salvar os registros.
if ($execucao == 'salvar') {
	// Recupera os valores para acrescentar no insert e/ou update
	$cadastrar = $_POST ['cadastrar'];
	$alterar = $_POST ['alterar'];
	
	if ($cadastrar != null || $cadastrar != '') {
		// Percorre o array retornado.
		foreach ( $cadastrar as $dado ) {
			// Separa o array para preencher a query de cadastro
			$arr = explode ( "_", $dado );
			$queryCadastrar .= "INSERT INTO 
									(dpaid,
	    							iducod,
	    							foncod,
	    							ndpid,
	    							dpavalor,
	    							aca.acaid,
	    							idoid,
	    							tpdid,
	    							ppoid,
	    							tcrid,
	    							acaidloa,
	    							nrcid,
	    							mcrid,
	    							dpanumerodecontrole,
	    							sbaid,
	    							ungcod,
	    							dpatipovalor,
	    							dpaidentificadorunico,
	    							ploid,
	    							dpavalordotacao,
	    							dea.irpcod,
	    							plocod,
	    							ploidloa,
	    							dpavalorexpansao,
	    							aca.unicod,
	    							aca.acacod,
	    							'???' AS naosei)
								VALUES 
						($arr[0],$arr[1],$arr[2],$arr[3],$arr[4],$arr[5],$arr[6],$arr[7],$arr[8],$arr[9],$arr[10]);";
		}
		// if($db->executar( $queryCadastrar )){
		// $db->commit();
		// echo "<script>alert('Registros cadastrados com sucesso!');</script>";
		// }
	}
	
	if ($alterar != null || $alterar != '') {
		foreach ( $alterar as $dados ) {
			$array = explode ( "_", $dados );
			
			$queryAlterar .= "update 
		(dpaid,
		iducod,
		foncod,
		ndpid,
		dpavalor,
		aca.acaid,
		idoid,
		tpdid,
		ppoid,
		tcrid,
		acaidloa,
		nrcid,
		mcrid,
		dpanumerodecontrole,
		sbaid,
		ungcod,
		dpatipovalor,
		dpaidentificadorunico,
		ploid,
		dpavalordotacao,
		dea.irpcod,
		plocod,
		ploidloa,
		dpavalorexpansao,
		aca.unicod,
		aca.acacod,
		'???' AS naosei)
		VALUES
		($array[0],$array[1],$array[2],$array[3],$array[4],$array[5],$array[6],$array[7],$array[8],$array[9],$array[10])
		where dpaid = $array[27];";
		}
		// if($db->executar( $queryAlterar )){
		// $db->commit();
		// echo "<script>alert('Registros alterados com sucesso!');</script>";
		// }
	}
}

// Se a execução for upload do arquivo.
if ($execucao == 'upload') {
	
	// Pasta onde o arquivo vai ser salvo
	$_UP ['pasta'] = APPRAIZ . 'www/upload/';
	
	// Tamanho máximo do arquivo (em Bytes)
	$_UP ['tamanho'] = 1024 * 1024 * 2; // 2Mb
	                                    
	// Array com as extensões permitidas
	$_UP ['extensoes'] = array (
			'csv' 
	);
	
	// Renomeia o arquivo? (Se true, o arquivo será salvo com nome único)
	$_UP ['renomeia'] = true;
	
	// Array com os tipos de erros de upload do PHP
	$_UP ['erros'] [0] = 'Não houve erro';
	$_UP ['erros'] [1] = 'O arquivo no upload é maior do que o limite do PHP';
	$_UP ['erros'] [2] = 'O arquivo ultrapassa o limite de tamanho especifiado no HTML';
	$_UP ['erros'] [3] = 'O upload do arquivo foi feito parcialmente';
	$_UP ['erros'] [4] = 'Não foi feito o upload do arquivo';
	
	// Verifica se houve algum erro com o upload. Se sim, exibe a mensagem do erro
	if ($_FILES ['arquivo'] ['error'] != 0) {
		die ( "<script>alert('Não foi possível fazer o upload, erro:
				" . $_UP ['erros'] [$_FILES ['arquivo'] ['error']] . "');</script>" );
		exit (); // Para a execução do script
	}
	
	// Caso script chegue a esse ponto, não houve erro com o upload e o PHP pode continuar
	// Faz a verificação da extensão do arquivo
	$extensao = strtolower ( end ( explode ( '.', $_FILES ['arquivo'] ['name'] ) ) );
	if (array_search ( $extensao, $_UP ['extensoes'] ) === false) {
		echo "<script>alert('Por favor, envie arquivos com a extensão csv.');</script>";
	} 	

	// Faz a verificação do tamanho do arquivo
	else if ($_UP ['tamanho'] < $_FILES ['arquivo'] ['size']) {
		echo "<script>alert('O arquivo enviado é muito grande, envie arquivos de até 2Mb.');</script>";
	} 	

	// O arquivo passou em todas as verificações, movimentação do arquivo para a pasta
	else {
		// Primeiro verifica se deve trocar o nome do arquivo
		if ($_UP ['renomeia'] == true) {
			
			// Cria um nome com extensão .csv
			$nome_final = 'importacaocredito.csv';
		} else {
			
			// Mantém o nome original do arquivo
			$nome_final = $_FILES ['arquivo'] ['name'];
		}
		
		// Depois verifica se é possível mover o arquivo para a pasta escolhida
		if (move_uploaded_file ( $_FILES ['arquivo'] ['tmp_name'], $_UP ['pasta'] . $nome_final )) {
			
			// Upload efetuado com sucesso
			echo "<script>alert('Upload efetuado com sucesso!');</script>";
			
			// Abertura do arquivo
			$f = fopen ( APPRAIZ . "www/upload/importacaocredito.csv", "r" );
			
			echo "<form action='elabrev.php?modulo=principal/importacaocredito/upload&acao=A&execucao=salvar' method='post' name='cadastro'><br><table border='1'align='center'class='tabela' cellspacing='0' 
				cellpadding='10' bgcolor='#fbfbfb'><tr><td><table class='tabela' cellpadding='3' style='width:100%;'><tr text-align: center; vertical-align: top><td></td><td>Cod. Identificador do Uso</td>
				<td>Cod. da Fonte</td><td>Cod. Natureza Despesa</td><td>Valor</td><td>acaid</td><td>Cod. Id. Operação</td><td>Tipo Despesa</td><td>Cod. Proposta</td><td>Cod. Tipo Crédito</td>
				<td>Cod. Ação LOA</td><td>Cod. Nat. Despesa</td><td>Controle</td><td>Cod. Subação</td><td>Cod. Unidade Gestora</td><td>dpatipovalor</td><td>dpaidentificadorunico</td><td>ploid</td>
				<td>dpavalordotacao</td><td>irpcod</td><td>plocod</td><td>ploidloa</td><td>dpavalorexpansao</td></tr>";
			
			$queryA;
			// Lê todas as linhas do arquivo
			while ( ! feof ( $f ) ) {
				$ar = explode ( ";", fgets ( $f ) );
				if ($ar ['0'] == 'dpaid')
					continue;
					
					// Verificando nos campos se o valor é nulo para não colocar as aspas simples
				if ($ar [0] != 'null') {
					$dpaid = '= ' . $ar [0];
					// Verificando nos campos se o valor é nulo para colocar a palavra is para fazer a comparação.
				} else {
					$dpaid = 'is ' . $ar [0];
				}
				
				if ($ar ['1'] != 'null') {
					$iducod = "= '$ar[1]'";
				} else {
					$iducod = 'is ' . $ar [1];
				}
				
				if ($ar ['2'] != 'null') {
					$foncod = "= '$ar[2]'";
				} else {
					$foncod = 'is ' . $ar [2];
				}
				
				if ($ar ['3'] != 'null') {
					$ndpid = '= ' . $ar [3];
				} else {
					$ndpid = 'is ' . $ar [3];
				}
				
				if ($ar ['4'] != 'null') {
					$dpavalor = '= ' . $ar [4];
				} else {
					$dpavalor = 'is ' . $ar [4];
				}
				
				if ($ar ['5'] != 'null') {
					$acaid = '= ' . $ar [5];
				} else {
					$acaid = 'is ' . $ar [5];
				}
				
				if ($ar ['6'] != 'null') {
					$idoid = '= ' . $ar [6];
				} else {
					$idoid = 'is ' . $ar [6];
				}
				
				if ($ar ['7'] != 'null') {
					$tpdid = '= ' . $ar [7];
				} else {
					$tpdid = 'is ' . $ar [7];
				}
				
				if ($ar ['8'] != 'null') {
					$ppoid = '= ' . $ar [8];
				} else {
					$ppoid = 'is ' . $ar [8];
				}
				
				if ($ar ['9'] != 'null') {
					$tcrid = '= ' . $ar [9];
				} else {
					$tcrid = 'is ' . $ar [9];
				}
				
				if ($ar ['10'] != 'null') {
					$acaidloa = '= ' . $ar [10];
				} else {
					$acaidloa = 'is ' . $ar [10];
				}
				
				if ($ar ['11'] != 'null') {
					$nrcid = "= '$ar[11]'";
				} else {
					$nrcid = 'is ' . $ar [11];
				}
				
				if ($ar ['12'] != 'null') {
					$mcrid = "= '$ar[12]'";
				} else {
					$mcrid = 'is ' . $ar [12];
				}
				
				if ($ar ['13'] != 'null') {
					$dpanumerodecontrole = "= '$ar[13]'";
				} else {
					$dpanumerodecontrole = 'is ' . $ar [13];
				}
				
				if ($ar ['14'] != 'null') {
					$sbaid = "= '$ar[14]'";
				} else {
					$sbaid = 'is ' . $ar [14];
				}
				
				if ($ar ['15'] != 'null') {
					$ungcod = "= '$ar[15]'";
				} else {
					$ungcod = 'is ' . $ar [15];
				}
				
				if ($ar ['16'] != 'null') {
					$dpatipovalor = "= '$ar[16]'";
				} else {
					$dpatipovalor = 'is ' . $ar [16];
				}
				
				if ($ar ['17'] != 'null') {
					$dpaidentificadorunico = "= '$ar[17]'";
				} else {
					$dpaidentificadorunico = 'is ' . $ar [17];
				}
				
				if ($ar ['18'] != 'null') {
					$ploid = "'$ar[18]'";
				} else {
					$ploid = $ar [18];
				}
				
				if ($ar ['19'] != 'null') {
					$dpavalordotacao = "'$ar[19]'";
				} else {
					$dpavalordotacao = $ar [19];
				}
				
				if ($ar ['20'] != 'null') {
					$irpcod = "= '$ar[20]'";
				} else {
					$irpcod = 'is ' . $ar [20];
				}
				
				if ($ar ['21'] != 'null') {
					if ($ar ['21'] == 0) {
						$ar ['21'] = '0000';
					}
					$plocod = "= '$ar[21]'";
				} else {
					$plocod = 'is ' . $ar [21];
				}
				
				if ($ar ['22'] != 'null') {
					$ploidloa = "= '$ar[22]'";
				} else {
					$ploidloa = 'is ' . $ar [22];
				}
				
				if ($ar ['23'] != 'null') {
					$dpavalorexpansao = "= '$ar[23]'";
				} else {
					$dpavalorexpansao = 'is ' . $ar [23];
				}
				
				if ($ar ['24'] != 'null') {
					$uo = "= '$ar[24]'";
				} else {
					$uo = 'is ' . $ar [24];
				}
				
				if ($ar ['25'] != 'null') {
					$acao = "= '$ar[25]'";
				} else {
					$acao = 'is ' . $ar [25];
				}
				
				if ($ar ['26'] != 'null') {
					$marcar = "= '$ar[26]'";
				} else {
					$marcar = 'is ' . $ar [26];
				}
				
				$sql = " 	SELECT
    								dpaid, -- Apenas para fazer UPDATE
   									iducod, -- Cod. Identificador do Uso
    								foncod,-- Cod. da Fonte
    								fon.fondsc, -- Fonte
       								ndpid, -- Cod. Natureza Despesa
    								dpavalor,-- Valor
									--    aca.acaid, -- Cod. Ação
									--    aca.acacod,-- Ação
    								idoid, -- Cod. Id. Operação
    								tpdid, -- Tipo Despesa
    								ppoid,-- Cod. Proposta
   									tcrid,-- Cod. Tipo Crédito
    								acaidloa,-- Cod. Ação LOA
    								nrcid, -- Cod. Nat. Despesa
    								-- mcrid, RETIRADO
    								dpanumerodecontrole, -- Controle
    								sbaid, -- Cod. Subação
    								ungcod,-- Cod. Unidade Gestora
    								dpatipovalor,
    								dpaidentificadorunico,
    								ploid,
    								dpavalordotacao,
    								dea.irpcod,
    								plocod,
    								ploidloa,
    								dpavalorexpansao
    								--,aca.unicod 
    								-- aca.acacod MUDOU DE LUGAR
							 FROM 	elabrev.despesaacao dea
						LEFT JOIN  	public.identifuso idu USING  (iducod)
						LEFT JOIN 	public.fonterecurso fon USING  (foncod)
						LEFT JOIN  	public.naturezadespesa ndp USING  (ndpid)
						LEFT JOIN 	public.idoc ido USING  (idoid)
					  --LEFT JOIN   public.tipodespesa USING (tpdid)
						LEFT JOIN 	elabrev.propostaorcamento ppo USING(ppoid)
						LEFT JOIN 	elabrev.tipocredito tcr USING (tcrid)
						LEFT JOIN 	monitora.acao acaloa ON acaloa.acaid = dea.acaidloa
						LEFT JOIN 	public.naturezareceita nrc using(nrcid)
						LEFT JOIN 	monitora.pi_subacao sba using (sbaid)
						LEFT JOIN 	public.unidadegestora ung USING (ungcod)
							WHERE 	acaloa.prgano = '{$_SESSION['exercicio']}'
						   -- AND   dpaid  $dpaid
							  AND   iducod  $iducod
					          AND   foncod  $foncod
	    			          AND   ndpid  $ndpid
	    			       -- AND   dpavalor  $dpavalor
							  AND   acaloa.acaid $acaid
							  AND   idoid $idoid
							  AND   tpdid $tpdid
							  AND   ppoid $ppoid
							  AND   tcrid $tcrid 
							  AND   acaidloa $acaidloa  
							  AND   nrcid $nrcid  
							  AND   mcrid $mcrid  
							  AND   dpanumerodecontrole $dpanumerodecontrole  
							  AND   sbaid $sbaid   
							  AND   ungcod $ungcod  
						   -- AND   dpatipovalor $dpatipovalor  
							  AND   dpaidentificadorunico $dpaidentificadorunico 
							  AND   dea.irpcod $irpcod 
							  AND   plocod $plocod 
							  AND   ploidloa $ploidloa  
						   -- AND   dpavalorexpansao $dpavalorexpansao";
				
				$verifica = $db->pegaLinha ( $sql );
				
				$sql_iducod = "SELECT DISTINCT
      								  idu.idudsc -- Identificador do Uso
   								FROM  elabrev.despesaacao dea
						   LEFT JOIN  public.identifuso idu USING  (iducod)
							   WHERE  iducod $iducod";
				
				$verifica_iducod = $db->pegaLinha ( $sql_iducod );
				
				$sql_foncod = "SELECT DISTINCT
									  fon.fondsc -- Fonte
				    			 FROM elabrev.despesaacao dea
				            LEFT JOIN public.fonterecurso fon USING  (foncod)
								WHERE foncod $foncod";
				
				$verifica_foncod = $db->pegaLinha ( $sql_foncod );
				
				$sql_ndpid = "SELECT DISTINCT
									 ndp.ndpdsc-- Natureza Despesa
							   FROM  elabrev.despesaacao dea
						  LEFT JOIN  public.naturezadespesa ndp USING  (ndpid)
							  WHERE  ndpid $ndpid";
				
				$verifica_ndpid = $db->pegaLinha ( $sql_ndpid );
				
				$sql_idoid = "SELECT DISTINCT
									 ido.idodsc -- Id. Operação
							   FROM  elabrev.despesaacao dea
						  LEFT JOIN  public.idoc ido USING  (idoid)
							  WHERE  idoid $idoid";
				
				$verifica_idoid = $db->pegaLinha ( $sql_idoid );
				
				$sql_ppoid = "SELECT DISTINCT    
									 ppo.ppodsc -- Proposta
							   FROM  elabrev.despesaacao dea
						  LEFT JOIN  elabrev.propostaorcamento ppo USING(ppoid)
							  WHERE  ppoid $ppoid";
				
				$verifica_ppoid = $db->pegaLinha ( $sql_ppoid );
				
				$sql_tcrid = "SELECT DISTINCT
								     tcr.tcrcod -- Tipo Crédito
				      		   FROM  elabrev.despesaacao dea
						  LEFT JOIN  elabrev.tipocredito tcr USING (tcrid)
							  WHERE  tcrid  $tcrid";
				
				$verifica_tcrid = $db->pegaLinha ( $sql_tcrid );
				
				$sql_acaidloa = "SELECT DISTINCT
										acaloa.procod ||'.'|| acaloa.acacod || '.' || acaloa.unicod || '.' || acaloa.loccod as acacodloa
								  FROM  elabrev.despesaacao dea
							 LEFT JOIN  monitora.acao acaloa ON acaloa.acaid = dea.acaidloa
								 WHERE  acaidloa $acaidloa";
				
				$verifica_acaidloa = $db->pegaLinha ( $sql_acaidloa );
				
				$sql_nrcid = "SELECT DISTINCT
									 nrc.nrccod -- Nat. Despesa
							   FROM  elabrev.despesaacao dea
						  LEFT JOIN  public.naturezareceita nrc using(nrcid)
						      WHERE  nrcid $nrcid";
				
				$verifica_nrcid = $db->pegaLinha ( $sql_nrcid );
				
				$sql_sbaid = "SELECT DISTINCT
									 sba.sbacod -- Subação
							   FROM  elabrev.despesaacao dea
				          LEFT JOIN  monitora.pi_subacao sba using (sbaid)
							  WHERE  sbaid $sbaid";
				
				$verifica_sbaid = $db->pegaLinha ( $sql_sbaid );
				
				$sql_ungcod = "SELECT DISTINCT
									  ung.gescod -- Unidade Gestora
								FROM  elabrev.despesaacao dea
						   LEFT JOIN  public.unidadegestora ung USING (ungcod)
							   WHERE  ungcod $ungcod";
				
				$verifica_ungcod = $db->pegaLinha ( $sql_ungcod );
				
				// Se o registro retornar uma linha quer dizer que o registro já existe no banco, logo será alterado caso o usuario queira.
				if ($verifica != null || $var != '') {
					$cor = "#9ACD32";
					$cadastro = "<input type='checkbox' name='alterar[]' value='$ar[0]_$ar[1]_$ar[2]_$ar[3]_$ar[4]_$ar[5]_$ar[6]_$ar[7]_$ar[8]_$ar[9]_$ar[10]
					_$ar[11]_$ar[12]_$ar[13]_$ar[14]_$ar[15]_$ar[16]_$ar[17]_$ar[18]_$ar[19]_$ar[20]_$ar[21]_$ar[22]_$ar[23]_$ar[24]_$ar[25]_$ar[26]_$verifica[dpaid]'>";
					// Se o registro não retornar nenhuma linha quer dizer que o registro não existe no banco, logo será cadastrado caso o usuario queira.
				} else {
					$cor = "red";
					$cadastro = "<input type='checkbox' name='cadastrar[]' value='$ar[0]_$ar[1]_$ar[2]_$ar[3]_$ar[4]_$ar[5]_$ar[6]_$ar[7]_$ar[8]_$ar[9]_$ar[10]
					_$ar[11]_$ar[12]_$ar[13]_$ar[14]_$ar[15]_$ar[16]_$ar[17]_$ar[18]_$ar[19]_$ar[20]_$ar[21]_$ar[22]_$ar[23]_$ar[24]_$ar[25]_$ar[26]' checked>";
				}
				
				echo "<tr bgcolor='$cor'><td>$cadastro</td><td>$ar[1] - $verifica_iducod[idudsc]</td><td>$ar[2] - $verifica_foncod[fondsc]</td><td>$ar[3] - $verifica_ndpid[ndpdsc]</td><td>$ar[4]</td>
				<td>$ar[5]</td><td>$ar[6] - $verifica_idoid[idodsc]</td><td>$ar[7]</td><td>$ar[8] - $verifica_ppoid[ppodsc]</td><td>$ar[9] - $verifica_tcrid[tcrcod]</td>
				<td>$ar[10] - $verifica_acaidloa[acacodloa]</td><td>$ar[11] - $verifica_nrcid[nrccod]</td><td>$ar[13]</td><td>$ar[14] - $verifica_sbaid[sbacod]</td><td>$ar[15] - $verifica_ungcod[gescod]</td>
				<td>$ar[16]</td><td>$ar[17]</td><td>$ar[18]</td><td>$ar[19]</td><td>$ar[20]</td><td>$ar[21]</td><td>$ar[22]</td><td>$ar[23]</td></tr>";
			}
			echo "</table></td></tr><td align='center'><input type='submit' value='Carregar'></td></tr></table></form>";
			
			// Fechamento do arquivo.
			fclose ( $f );
			
			// Exclui o arquivo.
			unlink ( APPRAIZ . "www/upload/importacaocredito.csv" );
		} else {
			
			// Não foi possível fazer o upload, provavelmente a pasta está incorreta
			echo "<script>alert('Não foi possível enviar o arquivo, tente novamente');</script>";
		}
	}
}
?>