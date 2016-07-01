<?php 
function excluirAnexo($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	// ver($dados,d);
	if ($dados ['arquivo']) {
		$sql = "
				DELETE FROM projovemcampo.instrumentolegal
				WHERE 
					pimid = {$_SESSION['projovemcampo']['pimid']}
		    	AND angid = (SELECT angid
							    FROM projovemcampo.anexogeral
							    WHERE  
							    	arqid = {$dados['arquivo']})
		   ";
    $db->executar ( $sql );
    $sql = "DELETE FROM projovemcampo.anexogeral WHERE arqid = {$dados['arquivo']}";
    $db->executar ( $sql );
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid= {$dados['arquivo']}";
		$db->executar ( $sql );
		$db->commit ();

		// -- Excluíndo arquivo do fs
		$file = new FilesSimec ();
		$file->excluiArquivoFisico ( $_POST ['arquivo'] );
		echo '<script type="text/javascript">alert("Arquivo excluído com sucesso!");</script>';
	} else {
		echo '<script type="text/javascript">alert("Nenhum arquivo foi informado para exclusão.");</script>';
	}
	echo '<script type="text/javascript">
	              window.location.href="projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais";
	    	  </script>';
	die ();
}
function criaPlanodeImplementacao(){
	global $db;
	
	$sql = "SELECT pimid FROM projovemcampo.planodeimplementacao WHERE apcid='" . $_SESSION ['projovemcampo']['apcid'] . "'";
	$planodeimplementacao = $db->pegaUm ($sql);
	if (!$planodeimplementacao) {
		$sql = "INSERT INTO projovemcampo.planodeimplementacao(apcid)
    			VALUES ('".$_SESSION['projovemcampo']['apcid']."')
    			RETURNING pimid;";
	
		$_SESSION['projovemcampo']['pimid']  = $db->pegaUm( $sql );
		$db->commit();
	}else{
		$_SESSION['projovemcampo']['pimid'] = $planodeimplementacao;
	}
	return $_SESSION['projovemcampo']['pimid'];
}

function calcularMontante() {
	global $db;

	$meta = pegameta();

	return ((340 * $meta * 24));
	die;
}

function popUpFormula(){

	?>
<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="/emi/geral/js/emi.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<?php monta_titulo("Fórmula",""); ?>
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubtituloDireita" width="25%" >Legenda</td>
				<td>
				<p>VR1 = Valor da 1 parcela</p>
				<p>MP1 = Meta prevista para atendimento</p>
				<p>6 = Meses de curso</p>
				<p>87,5% = Soma dos percentuais referentes a: percentual para pagamento de pessoal, aquisiçao de gêneros alimentícios, qualificação profissional</p>
				<p>Vpc = Valor per capta</p>
				<p>24 = Meses de curso</p>
				<? if($_SESSION['projovemcampo']['estuf']) : ?>
					<p>1,5% = Percentual para transporte de material didático</p>
				<? endif; ?>
				<p>1% = percentual para pagamento de auxílio financeiro para formação</p>
				<p>12 = Meses de Formação</p>
				<p>10% = Percentual para custeio de formação continuada</p>
				<p>R$ 54,00 = Adicional para elaboração e aplicação das provas</p>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" >Fórmula</td>
				<td>
				<?
				$vlr = "340,00";
				?>
				VR1 = MP1 X [(87,5 X 6 X Vpc) + (24 X 1,5% X Vpc) + (24 X 1% X Vpc) + (12 X 10% X Vpc) + (12 X 10% X Vpc)] + (MP1 X R$ 54,00)
				</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align:center" class="SubtituloDireita"  ><input type="button" name="btn_fechar" value="Fechar" onclick="window.close()" /> </td>
			</tr>
		</table>
	</body>
</html>
<?php	
}

function gravarProfissionais($dados){
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	// -- Processar upload do arquivo
	if ($_FILES ['arquivo']['name']) {
// 		$_POST['descricao'] = $_POST['descricao']?$_POST['descricao']:'Documento para cadastro de profissionais no plano de implementação do PJC';
		$campos = array (
				"angdsc" => "'" . $_POST ['descricao'] . "'",
				"angtip" => "'IL'"
		);
		
		$file = new FilesSimec ( "anexogeral", $campos, "projovemcampo" );
		
		$file->setUpload ( $_POST['descricao'], '', true, 'angid' );
		
		$angid = $file->getCampoRetorno();
		$db->commit();
		// -- Salvando a referência do anexo geral para o público do programa
		
		if ($dados ['inldatainstlegal']) {
			$inldatainstlegal = "'" . formata_data_sql ( $dados ['inldatainstlegal'] ) . "'";
		} else {
			$inldatainstlegal = 'NOW()';
		}
		
		$sql = "INSERT INTO projovemcampo.instrumentolegal(pimid, angid, inlnuminstlegal, inldatainstlegal, tpdid)
				VALUES({$_SESSION['projovemcampo']['pimid']},
					   {$angid}, 
					   {$dados['inlnuminstlegal']}, 
					   {$inldatainstlegal}, 
					   '1')";
		$db->executar ( $sql );
	}
	
	/*Coordenador geral*/
	$efetivorecprog = 3;
	$efeticocomp = 2;
	
	
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 1";
	$coordegera = $db->pegaUm($sql);
	if($coordegera){
		if($dados['coordgeral']['ocpid'] == $efetivorecprog){
			$sql = "UPDATE projovemcampo.planoprofissional
					   SET 
					   ocpid =  '{$dados['coordgeral']['ocpid']}',
					   valorbrutoremuneracao =".(($dados ['coordgeralrec']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeralrec']['valorbrutoremuneracao'] ) . "'" : "NULL") . ", 
					   qtdmeses=" . (($dados ['coordgeralrec']['qtdmeses']) ? "'" . $dados ['coordgeralrec']['qtdmeses'] . "'" : "NULL") . ",
					   encargossociais=" . (($dados ['coordgeralrec']['encargossociais']) ? "'" . $dados ['coordgeralrec']['encargossociais'] . "'" : "NULL") . ",
					   numerohoraspagas= NULL,
					   valorhora= NULL
					 WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 1
					";
			$db->executar ( $sql );
		}elseif($dados['coordgeral']['ocpid'] == $efeticocomp){	
			$sql = "UPDATE projovemcampo.planoprofissional
					   SET
						ocpid =  '{$dados['coordgeral']['ocpid']}',
					   valorbrutoremuneracao =".(($dados ['coordgeralccm']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeralccm']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
					   qtdmeses=" . (($dados ['coordgeralccm']['qtdmeses']) ? "'" . $dados ['coordgeralccm']['qtdmeses'] . "'" : "NULL") . ",
					   encargossociais=" . (($dados ['coordgeralccm']['encargossociais']) ? "'" . $dados ['coordgeralccm']['encargossociais'] . "'" : "NULL") . ",
					   numerohoraspagas=" . (($dados ['coordgeralccm']['numerohoraspagas']) ? "'" . $dados ['coordgeralccm']['numerohoraspagas'] . "'" : "NULL") . ",
					   valorhora=".(($dados ['coordgeralccm']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeralccm']['valorhora'] ) . "'" : "NULL") . ",
					   meses=" . (($dados ['coordgeralccm']['meses']) ? "'" . $dados ['coordgeralccm']['meses'] . "'" : "NULL") . "
				  	WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 1
							";
			$db->executar ( $sql );
		}else{
			$sql = "UPDATE projovemcampo.planoprofissional
					SET
						ocpid =  '{$dados['coordgeral']['ocpid']}',
						valorbrutoremuneracao = NULL,
						qtdmeses= NULL,
						encargossociais = NULL,
						numerohoraspagas = NULL,
					    valorhora = NULL
					WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 1
									";
			$db->executar ( $sql );
		}	
	}else{
		if($dados['coordgeral']['ocpid'] == $efetivorecprog){
			$sql = "INSERT INTO projovemcampo.planoprofissional(
		            	pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao, 
		            	qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
		    		VALUES (
		    				{$_SESSION['projovemcampo']['pimid']}, 
		    				1, 
		    				'{$dados['coordgeral']['ocpid']}', 
		    				'{$dados['coordgeral']['qtdcontratado']}',
		    				".(($dados ['coordgeralrec']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeralrec']['valorbrutoremuneracao'] ) . "'" : "NULL") . ", 
		    				" . (($dados ['coordgeralrec']['qtdmeses']) ? "'" . $dados ['coordgeralrec']['qtdmeses'] . "'" : "NULL") . ", 
		            		" . (($dados ['coordgeralrec']['encargossociais']) ? "'" . $dados ['coordgeralrec']['encargossociais'] . "'" : "NULL") . ", 
		            		NULL, 
		            		NULL, 
		            		NULL
		    				);
					";
		    $db->executar ( $sql );
	    }elseif($dados['coordgeral']['ocpid'] == $efeticocomp){
		    $sql = "INSERT INTO projovemcampo.planoprofissional(
		            	pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao, 
		            	qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
		    		VALUES (
		    				{$_SESSION['projovemcampo']['pimid']}, 
		    				1, 
		    				'{$dados ['coordgeral']['ocpid']}', 
		    				'{$dados ['coordgeral']['qtdcontratado']}',
		    				".(($dados ['coordgeralccm']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeralccm']['valorbrutoremuneracao'] ) . "'" : "NULL") . ", 
		    				" . (($dados ['coordgeralccm']['qtdmeses']) ? "'" . $dados ['coordgeralccm']['qtdmeses'] . "'" : "NULL") . ", 
		            		" . (($dados ['coordgeralccm']['encargossociais']) ? "'" . $dados ['coordgeralccm']['encargossociais'] . "'" : "NULL") . ", 
		            		" . (($dados ['coordgeralccm']['numerohoraspagas']) ? "'" . $dados ['coordgeralccm']['numerohoraspagas'] . "'" : "NULL") . ", 
		            		".(($dados ['coordgeralccm']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeralccm']['valorhora'] ) . "'" : "NULL") . ", 
		            		" . (($dados ['coordgeralccm']['meses']) ? "'" . $dados ['coordgeralccm']['meses'] . "'" : "NULL") . "
		    				);
					";
		    				
		    $db->executar ( $sql );
	    }else{
	    	$sql = "INSERT INTO projovemcampo.planoprofissional(
	    				pimid, profid, ocpid, qtdcontratado)
	    			VALUES (
				    	{$_SESSION['projovemcampo']['pimid']},
				    	1,
				    	'{$dados ['coordgeral']['ocpid']}',
				    	'{$dados ['coordgeral']['qtdcontratado']}'    				
	    				);
					";
	    											$db->executar ( $sql );
	    }
	}
	/*Fim coordenador geral*/
	/*Coordenador Turma*/
	
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 2";
	$coordturma = $db->pegaUm($sql);
	
	if($dados ['coordturma']['qtdcontratado']){
		if($coordturma){
			if($dados['coordturma']['ocpid'] == $efetivorecprog){
				$sql = "UPDATE projovemcampo.planoprofissional
							   SET
								ocpid =  '{$dados['coordturma']['ocpid']}',
							   valorbrutoremuneracao =".(($dados ['coordturmarec']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordturmarec']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
							   qtdmeses=" . (($dados ['coordturmarec']['qtdmeses']) ? "'" . $dados ['coordturmarec']['qtdmeses'] . "'" : "NULL") . ",
							   encargossociais=" . (($dados ['coordturmarec']['encargossociais']) ? "'" . $dados ['coordturmarec']['encargossociais'] . "'" : "NULL") . ",
							   numerohoraspagas= NULL,
							   valorhora= NULL
						WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 2
								   ";
			   $db->executar ( $sql );
			}elseif($dados['coordturma']['ocpid'] == $efeticocomp){
				
				$sql = "UPDATE projovemcampo.planoprofissional
						SET
							ocpid =  '{$dados['coordturma']['ocpid']}',
							valorbrutoremuneracao =".(($dados ['coordturmaccm']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordturmaccm']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
							qtdmeses=" . (($dados ['coordturmaccm']['qtdmeses']) ? "'" . $dados ['coordturmaccm']['qtdmeses'] . "'" : "NULL") . ",
							encargossociais=" . (($dados ['coordturmaccm']['encargossociais']) ? "'" . $dados ['coordturmaccm']['encargossociais'] . "'" : "NULL") . ",
							numerohoraspagas=" . (($dados ['coordturmaccm']['numerohoraspagas']) ? "'" . $dados ['coordturmaccm']['numerohoraspagas'] . "'" : "NULL") . ",
							valorhora=".(($dados ['coordturmaccm']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordturmaccm']['valorhora'] ) . "'" : "NULL") . "
						WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 2
				   		";
		   		$db->executar ( $sql );
			}else{
				$sql = "UPDATE projovemcampo.planoprofissional
						SET
							ocpid =  '{$dados['coordturma']['ocpid']}',
							valorbrutoremuneracao = NULL,
							qtdmeses= NULL,
							encargossociais = NULL,
							numerohoraspagas = NULL,
						    valorhora = NULL
						WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 2
										";
				$db->executar ( $sql );
			}		
	   	}else{
	   		if($dados['coordturma']['ocpid']){
		   		if($dados['coordturma']['ocpid'] == $efetivorecprog){
					$sql = "INSERT INTO projovemcampo.planoprofissional(
								pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
								qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
							VALUES (
								{$_SESSION['projovemcampo']['pimid']},
					    		2,
								'{$dados ['coordturma']['ocpid']}',
						    	'{$dados ['coordturma']['qtdcontratado']}',
						    	".(($dados ['coordturmarec']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordturmarec']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
						    	" . (($dados ['coordturmarec']['qtdmeses']) ? "'" . $dados ['coordturmarec']['qtdmeses'] . "'" : "NULL") . ",
						    	" . (($dados ['coordturmarec']['encargossociais']) ? "'" . $dados ['coordturmarec']['encargossociais'] . "'" : "NULL") . ",
						    	NULL,
					            NULL,
					            NULL
				    			);
							";
					$db->executar ( $sql );
				}elseif($dados['coordturma']['ocpid'] == $efeticocomp){
				    $sql = "INSERT INTO projovemcampo.planoprofissional(
							    pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
							    qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
					    	VALUES (
					    		{$_SESSION['projovemcampo']['pimid']},
					    		2,
					    		'{$dados ['coordturma']['ocpid']}',
					    		'{$dados ['coordturma']['qtdcontratado']}',
					    		".(($dados ['coordturmaccm']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordturmaccm']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
								" . (($dados ['coordturmaccm']['qtdmeses']) ? "'" . $dados ['coordturmaccm']['qtdmeses'] . "'" : "NULL") . ",
								" . (($dados ['coordturmaccm']['encargossociais']) ? "'" . $dados ['coordturmaccm']['encargossociais'] . "'" : "NULL") . ",
								" . (($dados ['coordturmaccm']['numerohoraspagas']) ? "'" . $dados ['coordturmaccm']['numerohoraspagas'] . "'" : "NULL") . ",
								".(($dados ['coordturmaccm']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordturmaccm']['valorhora'] ) . "'" : "NULL") . ",
								" . (($dados ['coordturmaccm']['meses']) ? "'" . $dados ['coordturmaccm']['meses'] . "'" : "NULL") . "
				    				)
					    								";
					$db->executar ( $sql );
				}else{
					$sql = "INSERT INTO projovemcampo.planoprofissional(
								pimid, profid, ocpid, qtdcontratado)
							VALUES (
								{$_SESSION['projovemcampo']['pimid']},
								2,
								'{$dados ['coordturma']['ocpid']}',
								'{$dados ['coordturma']['qtdcontratado']}'
								);
							";
					$db->executar ( $sql );
				}
	   		}
		}
	}else{
		if($coordturma){
			$sql = "DELETE FROM projovemcampo.planoprofissional
					WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 2
			";
			$db->executar ( $sql );
		}
	}
// 	ver($sql,d);
	/*Fim coordenador Turma*/
	/*Fim Assistente Coordenador*/
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 3";
	$coordassistentes = $db->pegaUm($sql);
	
	if($coordassistentes){
		if($dados['coordassistentes']['ocpid']){
			if($dados['coordassistentes']['ocpid'] == $efetivorecprog){
				$sql = "UPDATE projovemcampo.planoprofissional
						   SET
								ocpid =  '{$dados['coordassistentes']['ocpid']}',
							   valorbrutoremuneracao =".(($dados ['coordassistentes']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordassistentes']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
							   qtdmeses=" . (($dados ['coordassistentes']['qtdmeses']) ? "'" . $dados ['coordassistentes']['qtdmeses'] . "'" : "NULL") . ",
							   qtdcontratado = '{$dados ['coordassistentes']['qtdcontratado']}',
							   encargossociais=" . (($dados ['coordassistentes']['encargossociais']) ? "'" . $dados ['coordassistentes']['encargossociais'] . "'" : "NULL") . ",
								numerohoraspagas= NULL,
						   		valorhora= NULL
					   WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 3
								   ";
				$db->executar ( $sql );
			}else{
				$sql = "UPDATE projovemcampo.planoprofissional
						SET
							ocpid =  '{$dados['coordassistentes']['ocpid']}',
							qtdcontratado = '{$dados ['coordassistentes']['qtdcontratado']}',
							valorbrutoremuneracao = NULL,
							qtdmeses= NULL,
							encargossociais = NULL,
							numerohoraspagas = NULL,
						    valorhora = NULL
						WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 3
										";
				$db->executar ( $sql );
			}
		}
	}else{
		if($dados['coordassistentes']['ocpid']){
			if($dados['coordassistentes']['ocpid'] == $efetivorecprog){
				$sql = "INSERT INTO projovemcampo.planoprofissional(
							pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
							qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
						VALUES (
							{$_SESSION['projovemcampo']['pimid']},
							3,
							'{$dados ['coordassistentes']['ocpid']}',
							'{$dados ['coordassistentes']['qtdcontratado']}',
							".(($dados ['coordassistentes']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordassistentes']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
							" . (($dados ['coordassistentes']['qtdmeses']) ? "'" . $dados ['coordassistentes']['qtdmeses'] . "'" : "NULL") . ",
							" . (($dados ['coordassistentes']['encargossociais']) ? "'" . $dados ['coordassistentes']['encargossociais'] . "'" : "NULL") . ",
							NULL,
							NULL,
							NULL
			    			);
						";
				$db->executar ( $sql );
			}else{
				$sql = "INSERT INTO projovemcampo.planoprofissional(
							pimid, profid, ocpid, qtdcontratado)
						VALUES (
						{$_SESSION['projovemcampo']['pimid']},
						3,
						'{$dados ['coordassistentes']['ocpid']}',
						'{$dados ['coordassistentes']['qtdcontratado']}'
										);
						";
				$db->executar ( $sql );
			}
		}
	}
	/*Fim Assistente Coordenador*/
	/*Educador EF*/
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 4";
	$educadorEF = $db->pegaUm($sql);
	if($educadorEF){
		
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   qtdcontratado=" . (($dados['educadores']['F']['qtdcontratadorecursoproprio']) ? "'" .$dados['educadores']['F']['qtdcontratadorecursoproprio'] . "'" : "NULL") . "
			    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 4 AND ocpid = 4
					   ";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   qtdcontratado=" . (($dados['educadores']['F']['eduefetivo30hr']) ? "'" .$dados['educadores']['F']['eduefetivo30hr'] . "'" : "NULL") . "
			    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 4 AND ocpid = 1
					   ";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
					qtdcontratado =" . (($dados['contratadorecurso']['EF']['qtdcontratado']) ? "'" . $dados['contratadorecurso']['EF']['qtdcontratado'] . "'" : "NULL") . ",
				   valorbrutoremuneracao =".(($dados['contratadorecurso']['EF']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadorecurso']['EF']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				   qtdmeses=" . (($dados['contratadorecurso']['EF']['qtdmeses']) ? "'" . $dados['contratadorecurso']['EF']['qtdmeses'] . "'" : "NULL") . ",
				   encargossociais=" . (($dados ['contratadorecurso']['EF']['encargossociais']) ? "'" . $dados ['contratadorecurso']['EF']['encargossociais'] . "'" : "NULL") . "
			    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 4 AND ocpid = 3
					   ";
		$db->executar ( $sql );
								   
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
					qtdcontratado =" . (($dados['contratadocomp']['EF']['qtdcontratado']) ? "'" . $dados['contratadocomp']['EF']['qtdcontratado'] . "'" : "NULL") . ",
				   valorbrutoremuneracao =".(($dados ['contratadocomp']['EF']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EF']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				   qtdmeses=" . (($dados['contratadocomp']['EF']['qtdmeses']) ? "'" . $dados['contratadocomp']['EF']['qtdmeses'] . "'" : "NULL") . ",
				   encargossociais=" . (($dados['contratadocomp']['EF']['encargossociais']) ? "'" . $dados['contratadocomp']['EF']['encargossociais'] . "'" : "NULL") . ",
				   numerohoraspagas=" . (($dados['contratadocomp']['EF']['numerohoraspagas']) ? "'" . $dados['contratadocomp']['EF']['numerohoraspagas'] . "'" : "NULL") . ",
				   valorhora=".(($dados['contratadocomp']['EF']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EF']['valorhora'] ) . "'" : "NULL") . ",
				   meses=" . (($dados['contratadocomp']['EF']['meses']) ? "'" . $dados['contratadocomp']['EF']['meses'] . "'" : "NULL") . "
				WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 4 AND ocpid = 2
				   		";
   		$db->executar ( $sql );
		
	}else{
		
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					4,
					1,
					" . (($dados['educadores']['F']['eduefetivo30hr']) ? "'" . $dados['educadores']['F']['eduefetivo30hr'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
	    		);
				";
		$db->executar ( $sql );
		
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					4,
					4,
					" . (($dados['educadores']['F']['qtdcontratadorecursoproprio']) ? "'" . $dados['educadores']['F']['qtdcontratadorecursoproprio'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
	    		);
				";
		$db->executar ( $sql );
		
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					4,
					3,
					" . (($dados['contratadorecurso']['EF']['qtdcontratado']) ? "'" . $dados['contratadorecurso']['EF']['qtdcontratado'] . "'" : "NULL") . ",
					".(($dados['contratadorecurso']['EF']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadorecurso']['EF']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
					" . (($dados['contratadorecurso']['EF']['qtdmeses']) ? "'" . $dados['contratadorecurso']['EF']['qtdmeses'] . "'" : "NULL") . ",
					" . (($dados['contratadorecurso']['EF']['encargossociais']) ? "'" . $dados['contratadorecurso']['EF']['encargossociais'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL
	    		);
				";
		$db->executar ( $sql );
		
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					4,
					2,
					" . (($dados['contratadocomp']['EF']['qtdcontratado']) ? "'" . $dados['contratadocomp']['EF']['qtdcontratado'] . "'" : "NULL") . ",
					".(($dados['contratadocomp']['EF']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EF']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EF']['qtdmeses']) ? "'" . $dados['contratadocomp']['EF']['qtdmeses'] . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EF']['encargossociais']) ? "'" . $dados['contratadocomp']['EF']['encargossociais'] . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EF']['numerohoraspagas']) ? "'" . $dados['contratadocomp']['EF']['numerohoraspagas'] . "'" : "NULL") . ",
					".(($dados['contratadocomp']['EF']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EF']['valorhora'] ) . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EF']['meses']) ? "'" . $dados['contratadocomp']['EF']['meses'] . "'" : "NULL") . "
				)
			";
		$db->executar ( $sql );
	}
	/*Fim Educador EF*/
	/*Educador EQ*/
	
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 5";
	$educadorEQ = $db->pegaUm($sql);
	if($educadorEQ){
		
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   qtdcontratado=" . (($dados['educadores']['Q']['qtdcontratadorecursoproprio']) ? "'" . $dados['educadores']['Q']['qtdcontratadorecursoproprio'] . "'" : "NULL") . "
			    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 5 AND ocpid = 4
				    ";
		$db->executar ( $sql );
	
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   qtdcontratado=" . (($dados['educadores']['Q']['eduefetivo30hr']) ? "'" . $dados['educadores']['Q']['eduefetivo30hr'] . "'" : "NULL") . "
			    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 5 AND ocpid = 1
				    ";
		$db->executar ( $sql );
	
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
					qtdcontratado =" . (($dados['contratadorecurso']['EQ']['qtdcontratado']) ? "'" . $dados['contratadorecurso']['EQ']['qtdcontratado'] . "'" : "NULL") . ",
				   	valorbrutoremuneracao =".(($dados['contratadorecurso']['EQ']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadorecurso']['EQ']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				   	qtdmeses=" . (($dados['contratadorecurso']['EQ']['qtdmeses']) ? "'" . $dados['contratadorecurso']['EQ']['qtdmeses'] . "'" : "NULL") . ",
				   	encargossociais=" . (($dados ['contratadorecurso']['EQ']['encargossociais']) ? "'" . $dados ['contratadorecurso']['EQ']['encargossociais'] . "'" : "NULL") . "
			    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 5 AND ocpid = 3
				    ";
		$db->executar ( $sql );
			
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
					qtdcontratado =" . (($dados['contratadocomp']['EQ']['qtdcontratado']) ? "'" . $dados['contratadocomp']['EQ']['qtdcontratado'] . "'" : "NULL") . ",
				   	valorbrutoremuneracao =".(($dados ['contratadocomp']['EQ']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EQ']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				   	qtdmeses=" . (($dados['contratadocomp']['EQ']['qtdmeses']) ? "'" . $dados['contratadocomp']['EQ']['qtdmeses'] . "'" : "NULL") . ",
				   	encargossociais=" . (($dados['contratadocomp']['EQ']['encargossociais']) ? "'" . $dados['contratadocomp']['EQ']['encargossociais'] . "'" : "NULL") . ",
				  	numerohoraspagas=" . (($dados['contratadocomp']['EQ']['numerohoraspagas']) ? "'" . $dados['contratadocomp']['EQ']['numerohoraspagas'] . "'" : "NULL") . ",
				   	valorhora=".(($dados['contratadocomp']['EQ']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EQ']['valorhora'] ) . "'" : "NULL") . ",
				   	meses=" . (($dados['contratadocomp']['EQ']['meses']) ? "'" . $dados['contratadocomp']['EQ']['meses'] . "'" : "NULL") . "
			   WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 5 AND ocpid = 2
					   ";
		$db->executar ( $sql );
	
	}else{
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					5,
					1,
					" . (($dados['educadores']['Q']['eduefetivo30hr']) ? "'" . $dados['educadores']['Q']['eduefetivo30hr'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
	    		);
				";
		$db->executar ( $sql );
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					5,
					4,
					" . (($dados['educadores']['Q']['qtdcontratadorecursoproprio']) ? "'" . $dados['educadores']['Q']['qtdcontratadorecursoproprio'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
	    		);
				";
		$db->executar ( $sql );
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					5,
					3,
					" . (($dados['contratadorecurso']['EQ']['qtdcontratado']) ? "'" . $dados['contratadorecurso']['EQ']['qtdcontratado'] . "'" : "NULL") . ",
					".(($dados['contratadorecurso']['EQ']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadorecurso']['EQ']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
					" . (($dados['contratadorecurso']['EQ']['qtdmeses']) ? "'" . $dados['contratadorecurso']['EQ']['qtdmeses'] . "'" : "NULL") . ",
					" . (($dados['contratadorecurso']['EQ']['encargossociais']) ? "'" . $dados['contratadorecurso']['EQ']['encargossociais'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL
						);
								";
		$db->executar ( $sql );
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
				{$_SESSION['projovemcampo']['pimid']},
				5,
				2,
				" . (($dados['contratadocomp']['EQ']['qtdcontratado']) ? "'" . $dados['contratadocomp']['EQ']['qtdcontratado'] . "'" : "NULL") . ",
				".(($dados['contratadocomp']['EQ']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EQ']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				" . (($dados['contratadocomp']['EQ']['qtdmeses']) ? "'" . $dados['contratadocomp']['EQ']['qtdmeses'] . "'" : "NULL") . ",
				" . (($dados['contratadocomp']['EQ']['encargossociais']) ? "'" . $dados['contratadocomp']['EQ']['encargossociais'] . "'" : "NULL") . ",
				" . (($dados['contratadocomp']['EQ']['numerohoraspagas']) ? "'" . $dados['contratadocomp']['EQ']['numerohoraspagas'] . "'" : "NULL") . ",
				".(($dados['contratadocomp']['EQ']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EQ']['valorhora'] ) . "'" : "NULL") . ",
				" . (($dados['contratadocomp']['EQ']['meses']) ? "'" . $dados['contratadocomp']['EQ']['meses'] . "'" : "NULL") . "
				)
									";
		$db->executar ( $sql );
	}
/*Fim Educador EQ*/
/*Educador EP*/
	
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 6";
	$educadorEP = $db->pegaUm($sql);
	if($educadorEP){
		
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   qtdcontratado=" . (($dados['educadores']['P']['qtdcontratadorecursoproprio']) ? "'" .$dados['educadores']['P']['qtdcontratadorecursoproprio'] . "'" : "NULL") . "
				    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 6 AND ocpid = 4
				    ";
		$db->executar ( $sql );
	
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   qtdcontratado=" . (($dados['educadores']['P']['eduefetivo30hr']) ? "'" .$dados['educadores']['P']['eduefetivo30hr'] . "'" : "NULL") . "
				    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 6 AND ocpid = 1
				    ";
		$db->executar ( $sql );
	
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
					qtdcontratado=" . (($dados['contratadorecurso']['EP']['qtdcontratado']) ? "'" .$dados['contratadorecurso']['EP']['qtdcontratado'] . "'" : "NULL") . ",
				   	valorbrutoremuneracao =".(($dados['contratadorecurso']['EP']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadorecurso']['EP']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				   	qtdmeses=" . (($dados['contratadorecurso']['EP']['qtdmeses']) ? "'" . $dados['contratadorecurso']['EP']['qtdmeses'] . "'" : "NULL") . ",
				   	encargossociais=" . (($dados ['contratadorecurso']['EP']['encargossociais']) ? "'" . $dados ['contratadorecurso']['EP']['encargossociais'] . "'" : "NULL") . "
				    WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 6 AND ocpid = 3
				    ";
		$db->executar ( $sql );
			
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
					qtdcontratado=" . (($dados['contratadocomp']['EP']['qtdcontratado']) ? "'" .$dados['contratadocomp']['EP']['qtdcontratado'] . "'" : "NULL") . ",
				  	valorbrutoremuneracao =".(($dados ['contratadocomp']['EP']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EP']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				   	qtdmeses=" . (($dados['contratadocomp']['EP']['qtdmeses']) ? "'" . $dados['contratadocomp']['EP']['qtdmeses'] . "'" : "NULL") . ",
				  	encargossociais=" . (($dados['contratadocomp']['EP']['encargossociais']) ? "'" . $dados['contratadocomp']['EP']['encargossociais'] . "'" : "NULL") . ",
				   	numerohoraspagas=" . (($dados['contratadocomp']['EP']['numerohoraspagas']) ? "'" . $dados['contratadocomp']['EP']['numerohoraspagas'] . "'" : "NULL") . ",
				   	valorhora=".(($dados['contratadocomp']['EP']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EP']['valorhora'] ) . "'" : "NULL") . ",
				   	meses=" . (($dados['contratadocomp']['EP']['meses']) ? "'" . $dados['contratadocomp']['EP']['meses'] . "'" : "NULL") . "
					   WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 6 AND ocpid = 2
					   ";
		$db->executar ( $sql );
	}else{
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
				{$_SESSION['projovemcampo']['pimid']},
				6,
				1,
				" . (($dados['educadores']['P']['eduefetivo30hr']) ? "'" . $dados['educadores']['P']['eduefetivo30hr'] . "'" : "NULL") . ",
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
	    		);
				";
		$db->executar ( $sql );
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					6,
					4,
					" . (($dados['educadores']['P']['qtdcontratadorecursoproprio']) ? "'" . $dados['educadores']['P']['qtdcontratadorecursoproprio'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
    			);
			";
		$db->executar ( $sql );
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					6,
					3,
					" . (($dados['contratadorecurso']['EP']['qtdcontratado']) ? "'" . $dados['contratadorecurso']['EP']['qtdcontratado'] . "'" : "NULL") . ",
					".(($dados['contratadorecurso']['EP']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadorecurso']['EP']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
					" . (($dados['contratadorecurso']['EP']['qtdmeses']) ? "'" . $dados['contratadorecurso']['EP']['qtdmeses'] . "'" : "NULL") . ",
					" . (($dados['contratadorecurso']['EP']['encargossociais']) ? "'" . $dados['contratadorecurso']['EP']['encargossociais'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL
					);
								";
		$db->executar ( $sql );
	
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					6,
					2,
					" . (($dados['contratadocomp']['EP']['qtdcontratado']) ? "'" . $dados['contratadocomp']['EP']['qtdcontratado'] . "'" : "NULL") . ",
					".(($dados['contratadocomp']['EP']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EP']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EP']['qtdmeses']) ? "'" . $dados['contratadocomp']['EP']['qtdmeses'] . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EP']['encargossociais']) ? "'" . $dados['contratadocomp']['EP']['encargossociais'] . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EP']['numerohoraspagas']) ? "'" . $dados['contratadocomp']['EP']['numerohoraspagas'] . "'" : "NULL") . ",
					".(($dados['contratadocomp']['EP']['valorhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadocomp']['EP']['valorhora'] ) . "'" : "NULL") . ",
					" . (($dados['contratadocomp']['EP']['meses']) ? "'" . $dados['contratadocomp']['EP']['meses'] . "'" : "NULL") . "
					)
				";
					$db->executar ( $sql );
	}
	/*Fim Educador EP*/
	/*Educador ET*/
	
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 7";
	$educadorET = $db->pegaUm($sql);

	if($educadorET){
	
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
						qtdcontratado=" . (($dados['educadores']['T']['qtdcontratadorecursoproprio']) ? "'" .$dados['educadores']['T']['qtdcontratadorecursoproprio'] . "'" : "NULL") . "
			   WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 7 AND ocpid = 4
			   ";
	   $db->executar ( $sql );
	
	   $sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   		qtdcontratado=" . (($dados['educadores']['T']['eduefetivo30hr']) ? "'" .$dados['educadores']['T']['eduefetivo30hr'] . "'" : "NULL") . "
			 	WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 7 AND ocpid = 1
					   ";
		$db->executar ( $sql );
	
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
					qtdcontratado=" . (($dados['contratadorecurso']['ET']['qtdcontratado']) ? "'" .$dados['contratadorecurso']['ET']['qtdcontratado'] . "'" : "NULL") . ",
				   	valorbrutoremuneracao =".(($dados['contratadorecurso']['ET']['valorbrutoremuneracao']) ? "'" . str_replace ( array (".",","), array ("","."), $dados['contratadorecurso']['ET']['valorbrutoremuneracao'] ) . "'" : "NULL") . ",
				   	qtdmeses=" . (($dados['contratadorecurso']['ET']['qtdmeses']) ? "'" . $dados['contratadorecurso']['ET']['qtdmeses'] . "'" : "NULL") . ",
				   	encargossociais=" . (($dados ['contratadorecurso']['ET']['encargossociais']) ? "'" . $dados ['contratadorecurso']['ET']['encargossociais'] . "'" : "NULL") . "
			   WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 7 AND ocpid = 3
					   ";
		$db->executar ( $sql );
				   	
	}else{
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					7,
					1,
					" . (($dados['educadores']['T']['eduefetivo30hr']) ? "'" . $dados['educadores']['T']['eduefetivo30hr'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
				);
				";
		$db->executar ( $sql );
		
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					7,
					4,
					" . (($dados['educadores']['T']['qtdcontratadorecursoproprio']) ? "'" . $dados['educadores']['T']['qtdcontratadorecursoproprio'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
					);
					";
		$db->executar ( $sql );
		
		$sql = "INSERT INTO projovemcampo.planoprofissional(
					pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					7,
					3,
					" . (($dados['contratadorecurso']['ET']['qtdcontratado']) ? "'" . $dados['contratadorecurso']['ET']['qtdcontratado'] . "'" : "NULL") . ",
					".(($dados['contratadorecurso']['ET']['valorbrutoremuneracao'])?"'".str_replace( array (".",","),array ("","."),$dados['contratadorecurso']['EP']['valorbrutoremuneracao'])."'" : "NULL") . ",
					" . (($dados['contratadorecurso']['ET']['qtdmeses']) ? "'" . $dados['contratadorecurso']['ET']['qtdmeses'] . "'" : "NULL") . ",
					" . (($dados['contratadorecurso']['ET']['encargossociais']) ? "'" . $dados['contratadorecurso']['ET']['encargossociais'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL
					);
					";
		$db->executar ( $sql );
		
	}
/*Fim Educador ET*/
/*Educador E*/
	
	$sql = "SELECT true FROM projovemcampo.planoprofissional WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 8";
	$educadorE = $db->pegaUm($sql);
	if($educadorE){
		
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
						qtdcontratado=" . (($dados['educadores']['E']['qtdcontratadorecursoproprio']) ? "'" .$dados['educadores']['E']['qtdcontratadorecursoproprio'] . "'" : "NULL") . "
				   WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 8 AND ocpid = 4
				   ";
		$db->executar ( $sql );
	
		$sql = "UPDATE projovemcampo.planoprofissional
				   SET
				   		qtdcontratado=" . (($dados['educadores']['E']['eduefetivo30hr']) ? "'" .$dados['educadores']['E']['eduefetivo30hr'] . "'" : "NULL") . "
				WHERE pimid ={$_SESSION['projovemcampo']['pimid']} AND  profid = 8 AND ocpid = 1
					   		";
					   		$db->executar ( $sql );
	
	   }else{
	
			$sql = "INSERT INTO projovemcampo.planoprofissional(
					   pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					   qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
			   		VALUES (
					   {$_SESSION['projovemcampo']['pimid']},
					   8,
						1,
						" . (($dados['educadores']['E']['eduefetivo30hr']) ? "'" . $dados['educadores']['E']['eduefetivo30hr'] . "'" : "NULL") . ",
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
					   NULL
					   );
					   ";
					   $db->executar ( $sql );
	
		   $sql = "INSERT INTO projovemcampo.planoprofissional(
					   pimid, profid, ocpid, qtdcontratado, valorbrutoremuneracao,
					   qtdmeses, encargossociais, numerohoraspagas, valorhora, meses)
				  VALUES (
					{$_SESSION['projovemcampo']['pimid']},
					8,
					4,
					" . (($dados['educadores']['E']['qtdcontratadorecursoproprio']) ? "'" . $dados['educadores']['E']['qtdcontratadorecursoproprio'] . "'" : "NULL") . ",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
					);
				";
			$db->executar ( $sql );
	
		}
	$db->commit ();
/*Fim Educador E*/
	echo 
		"<script>
			alert('Gravado com sucesso');
			window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais';
		</script>";
	die;
}
function pegarocpid($profid){
	global $db;
	if($_SESSION['projovemcampo']['pimid']){
		$sql = "SELECT
					ocpid,
					qtdcontratado
				FROM
					projovemcampo.planoprofissional 
				WHERE
						pimid = {$_SESSION['projovemcampo']['pimid']}
				AND 	profid = {$profid}
				ORDER BY
					ocpid,profid";
		
		$dados = $db->pegaLinha($sql);
	}else{
		$dados = '';
	}
	return $dados;
	die;
}

function gravarFormacaoEducadores($dados){
	global $db;
	$sql = "UPDATE projovemcampo.planodeimplementacao
			   SET	qtdeduccontinuada= {$dados['qtdeduccontinuada']}, 
			       	auxfinanceiroeduc= ".(($dados['auxfinanceiroeduc']) ? "'".str_replace(array (".",","),array ("","."), $dados['auxfinanceiroeduc'])."'":"NULL") . "
			 WHERE 
			       	pimid = {$_SESSION['projovemcampo']['pimid']}
	";
	$db->executar ( $sql );
	
	if ($dados ['valor']) {
		foreach ( $dados ['valor'] as $tpfid=> $valor ) {
			$sql = "SELECT
						trpid
					FROM
						projovemcampo.tiporecursoplanoimplementacao
					WHERE
						pimid = {$_SESSION['projovemcampo']['pimid']}
					AND		tpfid = $tpfid";
			
			$teste = $db->pegaLinha($sql);
			if(!$teste){
				$sql = "INSERT INTO projovemcampo.tiporecursoplanoimplementacao(
			            	tpfid, pimid, valor)
			    		VALUES ( $tpfid, {$_SESSION['projovemcampo']['pimid']}, ".(($valor) ? "'".str_replace(array (".",","),array ("","."), $valor)."'":"NULL") . ")";
			}else{
				$sql ="UPDATE projovemcampo.tiporecursoplanoimplementacao
						   SET valor= ".(($valor) ? "'".str_replace(array (".",","),array ("","."), $valor)."'":"NULL") . "
						 WHERE 
								pimid = {$_SESSION['projovemcampo']['pimid']}
						AND		tpfid = $tpfid";
			}
			$db->executar ( $sql );
		}
	}
	$db->commit ();
	echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores';
		  </script>";
	die;
}


function pegarprofissionais($profid,$ocpid){
	global $db;
	
	$efetivorecprog = 3;
	$efeticocomp = 2;
	
	if($ocpid == $efeticocomp || $ocpid == $efetivorecprog){
		$valor = ",CASE 
						WHEN valorbrutoremuneracao is not null
						THEN ((valorbrutoremuneracao*qtdcontratado*qtdmeses)+((valorbrutoremuneracao*qtdcontratado*qtdmeses)*encargossociais/100))
						ELSE 0
					END as vlrtotal ";
	}
	
	$sql = "SELECT
				*
				$valor
			FROM
				projovemcampo.planoprofissional 
			WHERE
					pimid = {$_SESSION['projovemcampo']['pimid']}
			AND 	profid = {$profid}
			AND 	ocpid = {$ocpid}
			ORDER BY
				ocpid,profid";
	
	$prof = $db->pegaLinha($sql);
	
	return $prof;
	die;
}

function gravarGeneroAlimenticio($dados){
	global $db;
// 	ver($dados,d);
	$sql = "UPDATE projovemcampo.planodeimplementacao
			SET
				gaqtdcriancas= ".($dados['gaqtdcriancas']?$dados['gaqtdcriancas']:"NULL").",
				gaqtdmeses=".($dados['gaqtdmeses']?$dados['gaqtdmeses']:24).",
				gavalormensal=".(($dados['gavalormensal']) ? "'".str_replace(array (".",","),array ("","."), $dados['gavalormensal'])."'":"NULL") . "
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']} ;
	";
	$db->executar($sql);
	
	$db->commit();
	echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios';
		  </script>";
	die;

}

function gravarQualificacaoProfissional($dados){
	global $db;
// 	ver($dados);
	$dados['qualificacaoarco'] = $dados['qualificacaoarco'] ? $dados['qualificacaoarco']: 'f';
	$dados['qualificacaopronatec'] = $dados['qualificacaopronatec'] ? $dados['qualificacaopronatec']: 'f';
	
	$sql = "UPDATE projovemcampo.planodeimplementacao
			   SET 
					qualificacaoarco='{$dados['qualificacaoarco']}',
					qualificacaopronatec='{$dados['qualificacaopronatec']}'
			 WHERE 
			 		pimid = {$_SESSION['projovemcampo']['pimid']}";
	
	$db->executar ( $sql );
	
	if ($dados ['valor']) {
		$sql ="SELECT DISTINCT
					true
				FROM
					projovemcampo.despesaqualificacaoplano
				WHERE 
			 		pimid = {$_SESSION['projovemcampo']['pimid']}";
		$teste = $db->pegaUm($sql);
		if(!$teste){
			foreach ( $dados ['valor'] as $qpdid=> $valor ) {
				$sql = "INSERT INTO projovemcampo.despesaqualificacaoplano(
					            qpdid, pimid, valor, qtdmeses)
					    VALUES ($qpdid,{$_SESSION['projovemcampo']['pimid']} ,".(($valor) ? "'".str_replace(array (".",","),array ("","."), $valor)."'":"NULL") . ", {$dados['qtdmeses'][$qpdid]});
							";
				$db->executar ( $sql );
			}
		}else{
			foreach ( $dados ['valor'] as $qpdid=> $valor ) {
				$sql = "UPDATE projovemcampo.despesaqualificacaoplano
					   	SET 
							valor=".(($valor) ? "'".str_replace(array (".",","),array ("","."), $valor)."'":"NULL") . ", 
							qtdmeses={$dados['qtdmeses'][$qpdid]}
					 	WHERE 
							pimid = {$_SESSION['projovemcampo']['pimid']}
					 	AND qpdid = $qpdid 
					";
				$db->executar ( $sql );
// 			ver($sql);
			}
// 			die;
		}
	}
	$db->commit ();
	
	echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissional';
		  </script>";
	die;
}

function gravarTransporteDidatico($dados){
	global $db;
	
	$sql = "UPDATE projovemcampo.planodeimplementacao
			   SET 
			   	valormaterialdidatico=".(($dados['valormaterialdidatico']) ? "'".str_replace(array (".",","),array ("","."), $dados['valormaterialdidatico'])."'":"NULL") . "
 			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}
	";
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico';
		  </script>";
	die;
}
function gravarDemaisAcoes($dados){
	global $db;
	$sql = "SELECT DISTINCT
				true
			FROM 
				projovemcampo.tipodemaisacoesplano
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']} ";
	$teste = $db->pegaUm($sql);
	if(!$teste){
		foreach ( $dados['valorpormes'] as $tdaid=> $valor ) {
			$sqldemais .= "INSERT INTO projovemcampo.tipodemaisacoesplano(
				            tdaid, pimid, valorpormes, qtdmeses,valortotal)
				    VALUES ($tdaid, {$_SESSION['projovemcampo']['pimid']},".(($valor) ? "'".str_replace(array (".",","),array ("","."), $valor)."'":"NULL") . ",{$dados['qtdmeses'][$tdaid]},".(($dados['valortotal'][$tdaid]) ? "'".str_replace(array (".",","),array ("","."), $dados['valortotal'][$tdaid])."'":"NULL") . ");
						 ;";
		}
	}else{
// 		ver($dados,d);
		foreach ( $dados['valorpormes'] as $tdaid=> $valor ) {
			$sqldemais .= "UPDATE projovemcampo.tipodemaisacoesplano
  					 SET 
  					 	valorpormes=".(($valor) ? "'".str_replace(array (".",","),array ("","."), $valor)."'":"NULL") . ", 
  					 	qtdmeses={$dados['qtdmeses'][$tdaid]},
  					 	valortotal = ".(($dados['valortotal'][$tdaid]) ? "'".str_replace(array (".",","),array ("","."), $dados['valortotal'][$tdaid])."'":"NULL") . "
				 	WHERE
							pimid = {$_SESSION['projovemcampo']['pimid']}
					AND tdaid = $tdaid
								;";
		}
		
	}
	$db->executar ($sqldemais);
	$db->commit ();
	
	echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes';
		  </script>";
	die;
}
function pegarUsuarioMaterial() {
	global $db;

		$sql = "
            SELECT * FROM projovemcampo.planodeimplementacao pim 
            WHERE pim.pimid='" . $_SESSION['projovemcampo']['pimid'] . "'
		";
		return $db->pegaLinha ( $sql );
	return array ();
	die;
}

function gravarresponsavelmaterial($dados){
	global $db;
// 	ver($dados,d);
	$sql = "UPDATE projovemcampo.planodeimplementacao
			   SET
			   	cpfmaterialdidatico= '{$dados['cpfmaterialdidatico']}',
  				nomematerialdidatico = '{$dados['nomematerialdidatico']}',
  				cepmaterialdiddatico = ".str_replace(array ("-"),array (""), $dados['cepmaterialdiddatico']).",
  				enderecomaterialdidatico= '{$dados['enderecomaterialdidatico']}',
  				bairromaterialdidatico= '{$dados['bairromaterialdidatico']}',
  				complementomaterialdidatico = '{$dados['complementomaterialdidatico']}',
  				numeromaterialdidatico= '{$dados['numeromaterialdidatico']}',
  				muncodmaterialdidatico = '{$dados['muncodmaterialdidatico']}'
		   	WHERE
			   	pimid = {$_SESSION['projovemcampo']['pimid']}
	";
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=enderecoEntrega';
		  </script>";
	die;

}
function planodeimplementacao(){
	global $db;
	$sql = "SELECT
			*
			FROM
				projovemcampo.planodeimplementacao
			WHERE pimid={$_SESSION['projovemcampo']['pimid']}";
	$planodeimplementacao = $db->pegaLinha($sql);
	
	return $planodeimplementacao;
	return array ();
	die;
}
function validacaoCompletaPlanoImplementacao($retornarTotalMaximoDemaisAcoes = false) {
	global $db,$totalmaxdemaisacoes;
	
	$msg = '';

	$meta = pegameta();
	
	if ($meta < 1) {
		continue;
	}
/*
* VALIDANDO NÚMERO DE Turmas.Onúmero máximo de turma é a meta dividida por 15 que é o número mínimo de alunos por turma. O número máximo de alunos por turma é de 30. A somatório dos alunos nas turmas não pode ser maior que a meta.
*/

	$sql = "SELECT
				mun.muncod,
				t.turid,
				t.turqtdalunosprevistos
			FROM
					projovemcampo.turma t
			INNER JOIN 	projovemcampo.adesaoprojovemcampo a ON a.secaid = t.secaid
			INNER JOIN 	entidade.entidade e ON e.entid = t.entid AND t.turstatus = 'A'
			INNER JOIN	entidade.endereco ende ON ende.entid = e.entid
			INNER JOIN 	territorios.municipio mun ON mun.muncod = ende.muncod
			WHERE
				a.apcid = {$_SESSION['projovemcampo']['apcid']}
			ORDER BY 
				turid";
	$turmas = $db->Carregar($sql);
	
	$totalestudantes = 0;
	if ($turmas[0]) {
		$_CHK = Array ();
		foreach ( $turmas as $turma ) {
			$_CHK [$turma ['muncod']] [$turma ['turid']] = $turma['turqtdalunosprevistos'];
			$totalestudantes += $turma['turqtdalunosprevistos'];
		}
	} else {
		$msg [] = "<br> - Não existe turma não cadastrada.";
	}
	
	if ($totalestudantes != $meta) {
		$msg [] = "<br> - Quantidade de estudantes/Turma diferente da meta (Meta:" . $meta . ",Utilizado:" . $totalestudantes . ").";
	}

	if ($_CHK) {
	foreach ( array_keys ( $_CHK ) as $muncod ) {
		if (count ( $_CHK [$muncod] ) == 1) {
		$qtde = current ( $_CHK [$muncod] );
				if ($qtde< 15 || $qtde> 30) {
					$msg [] = "<br> - Se a turma for igual a 1 (um), o nº de alunos deve ser necessariamente entre 15 e 30.";
				}
			}
		}
	}
/* FIM validando número de Turmas */
	$montante = calcularMontante();

	$planodeimplementacao = planodeimplementacao();
	/*
 * VALIDANDO PROFISSIONAIS - verifica se a aba profissionais foi gravado pelo menos uma vez; - verifica se o valor total de profissionais é maior que o percentual previsto; - atualiza o percentual utilizado (caso tenha ocorrido alguma falha)
 */
	$profpercmax = '75.5';
	
	$efetivorecprog = 3;
	$efeticocomp = 2;
	$eftivo30 = 1;
	$efetivorecprop = 4;
	
	$coordenadorgeral = pegarocpid('1');
	$coordenadorgeralrec = pegarprofissionais('1',$efetivorecprog);
	$coordenadorgeralccm = pegarprofissionais('1',$efeticocomp);
	
	$coordturma = pegarocpid('2');
	$coordturmarec = pegarprofissionais('2',$efetivorecprog);
	$coordturmaccm = pegarprofissionais('2',$efeticocomp);
	
	$assistente = pegarocpid('3');
	$assistenterec = pegarprofissionais('3',$efetivorecprog);

	$educadores_EF_eft= pegarprofissionais('4',$eftivo30);
	$educadores_EF_recprop= pegarprofissionais('4',$efetivorecprop);
	$educadores_EF_rec = pegarprofissionais('4',$efetivorecprog);
	$educadores_EF_ccm = pegarprofissionais('4',$efeticocomp);
	
	$educadores_EQ_eft= pegarprofissionais('5',$eftivo30);
	$educadores_EQ_recprop= pegarprofissionais('5',$efetivorecprop);
	$educadores_EQ_rec = pegarprofissionais('5',$efetivorecprog);
	$educadores_EQ_ccm = pegarprofissionais('5',$efeticocomp);
	
	$educadores_EP_eft= pegarprofissionais('6',$eftivo30);
	$educadores_EP_recprop= pegarprofissionais('6',$efetivorecprop);
	$educadores_EP_rec = pegarprofissionais('6',$efetivorecprog);
	$educadores_EP_ccm = pegarprofissionais('6',$efeticocomp);
	
	$educadores_ET_eft= pegarprofissionais('7',$eftivo30);
	$educadores_ET_recprop= pegarprofissionais('7',$efetivorecprop);
	$educadores_ET_rec = pegarprofissionais('7',$efetivorecprog);
	
	$educadores_EE_eft= pegarprofissionais('8',$eftivo30);
	$educadores_EE_recprop= pegarprofissionais('8',$efetivorecprop);
	$educadores_E = $educadores_EE_eft['qtdcontratado'] + $educadores_EE_recprop['qtdcontratado'];
	
	$totalUtilizado_profissionais = 	$coordenadorgeralrec['vlrtotal'] + $coordenadorgeralccm['vlrtotal']
										+ $coordturmarec['vlrtotal'] + $coordturmaccm['vlrtotal']
										+ $assistenterec['vlrtotal']
										+ $educadores_EF_rec['vlrtotal'] + $educadores_EF_ccm['vlrtotal']
										+ $educadores_EQ_rec['vlrtotal'] + $educadores_EQ_ccm['vlrtotal']
										+ $educadores_EP_rec['vlrtotal'] + $educadores_EP_ccm['vlrtotal']
										+ $educadores_ET_rec['vlrtotal']
	;
	$totalUtilizado_profissionais = round($totalUtilizado_profissionais,2);
	
	$percutilizado = ($totalUtilizado_profissionais*100)/$montante;

	if (!$coordenadorgeral) {
		$msg [] = "<br> - A Tela de Profissionais não foi gravada.";
	}
	if (round ( $totalUtilizado_profissionais ) > (round($montante*$profpercmax)/100)) {
		$msg [] = "<br> - O valor utilizado em profissionais é maior que a percentagem prevista.";
	}

/* FIM validando profissionais */

/*
 * VALIDANDO FORMAÇÃO
*
* - verifica se a aba formação de educadores foi gravado pelo menos uma vez;
* - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
* - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)
* - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
* - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)
*
*/
	$formpercmax = '10';

	$sql = "SELECT
				SUM(valor)
			FROM
				projovemcampo.tiposrecursoformacao tpf
			LEFT JOIN projovemcampo.tiporecursoplanoimplementacao trp ON trp.tpfid=tpf.tpfid
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}";
	$totalutilizado_formacao = $db->pegaUm($sql);
	
	if (!$totalutilizado_formacao) {
		$msg [] = "<br> - A Tela de Formação de Educadores não foi gravada.";
	}
	
	if ($totalutilizado_formacao > ($montante*$formpercmax/100)) {
		$msg [] = "<br> - Recursos gastos com a formação é maior que a percentagem prevista.";
	}
	
	$auxpermax = '1';
	
// 	$sql = "SELECT
// 				count(pprid)
// 			FROM
// 				projovemcampo.planoprofissional
// 			WHERE
// 				pimid = {$_SESSION['projovemcampo']['pimid']}
// 			AND 	valorbrutoremuneracao is not null
// 			AND 	profid in(4,5,6)";
	
// 	$tiposprof = $db->pegaUm($sql);
	
// 	if(!$tiposprof){
// 		$tiposprof = '1';
// 	}
	
	$totalrec   = array(	"vlrtotal" 	 => 	$educadores_EF_rec['valorbrutoremuneracao']+
												$educadores_EQ_rec['valorbrutoremuneracao'],
							"qtdcontratado" => 		$educadores_EF_rec['qtdcontratado']+
												$educadores_EQ_rec['qtdcontratado']);
	
	$totalccm =  array(	"vlrtotal" 	 => 		$educadores_EF_ccm['valorbrutoremuneracao']+
												$educadores_EQ_ccm['valorbrutoremuneracao'],
						"qtdcontratado" => 			$educadores_EF_ccm['qtdcontratado']+
												$educadores_EQ_ccm['qtdcontratado']);
	$total = array("valor"=> $totalccm['vlrtotal']+ $totalrec['vlrtotal'],
					"qtd"=>$totalccm['qtdcontratado']+ $totalrec['qtdcontratado']);
	
	$valormax = ($montante*$auxpermax)/100;
	
	$totalutilizado_auxiliofinanceiro = $planodeimplementacao['qtdeduccontinuada']*$planodeimplementacao['auxfinanceiroeduc'];
	
	$sql = "SELECT
				count(pprid)
			FROM
				projovemcampo.planoprofissional
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}
			AND 	valorbrutoremuneracao is not null
			AND 	profid in(4,5)";
	
	$tiposprof = $db->pegaUm($sql);
	
	if(!$tiposprof){
		$tiposprof = '1';
	}
	if($total['valor']){
		if (round ( $planodeimplementacao['auxfinanceiroeduc'], 2 ) > round ( (($total['valor']/$tiposprof)*0.3), 2 )) {
			$msg [] = "<br> - Auxílio financeiro a ser pago(R$) maior do que o permitido.";
		}
	}
	if ($totalutilizado_auxiliofinanceiro > $valormax) {
		$msg [] = "<br> - Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação é maior que a percentagem prevista.";
	}
/* FIM validando formação */

/*
 * VALIDANDO GENÊRO ALIMENTICIOS
*
* - verifica se a aba genero alimenticios foi gravado pelo menos uma vez;
* - atualiza o percentual utilizado dos generos alimenticios (caso tenha ocorrido alguma falha)
*
*/
	$alimpermax = '5';
	
	$alimvalormax = ($montante*$alimpermax)/100;
	
	$totalutilizado_generoalimenticio = ($planodeimplementacao['gaqtdcriancas']+$meta)*$planodeimplementacao['gaqtdmeses']*$planodeimplementacao['gavalormensal'];
	
	if (!$planodeimplementacao['gavalormensal']) {
		$msg [] = "<br> - A Tela de Gêneros Alimenticios não foi gravada.";
	}
	
	if ($totalutilizado_generoalimenticio > $alimvalormax) {
		$msg [] = "<br> - Valor do Lanche ou Refeição é maior que a percentagem prevista.";
	}
/* FIM validando genêro alimenticios */

/*
 * VALIDANDO QUALIFICAÇÃO PROFISSIONAL
*
* - verifica se a aba qualificação profissional foi gravado pelo menos uma vez;
* - atualiza o percentual utilizado na qualificação profissional (caso tenha ocorrido alguma falha)
*
*/
	$qualipercmax = '7';
	
	$sql = "SELECT
				qtdmeses*valor as valortotal
			FROM
				projovemcampo.qualificacaoprofissionaldespesa qpd
			LEFT JOIN projovemcampo.despesaqualificacaoplano dqp ON dqp.qpdid = qpd.qpdid
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}";
	$qualificacaoprofissional = $db->Carregar($sql);
	
	$arcopronatec = $db->pegaLinha("SELECT qualificacaoarco, qualificacaopronatec FROM projovemcampo.planodeimplementacao WHERE pimid='".$_SESSION['projovemcampo']['pimid']."'");
	
	$totalutilizado_qualificacaoprofissional = $qualificacaoprofissional['0']['valortotal']+$qualificacaoprofissional['1']['valortotal']+$qualificacaoprofissional['2']['valortotal'];
	
	$totalmaxquali = ($qualipercmax*$montante)/100;
	
	if (!is_array($qualificacaoprofissional)) {
		$msg [] = "<br> - A Tela de Qualificação profissional não foi gravada.";
	}
	
	if ($totalutilizado_qualificacaoprofissional > $totalmaxquali) {
		$msg [] = "<br> - Despesas com qualificação profissional é maior que a percentagem prevista.";
	}
	
	if(!$arcopronatec['qualificacaoarco'] && !$arcopronatec['qualificacaopronatec']){
		$msg [] = "<br> - É necessário selecionar a oferta da despesa, se é por meio dos ARCOS  ou PRONATEC.";
	}

/* FIM validando qualificação profissional */
// 	if ($_SESSION ['projovemcampo'] ['estuf']){
	
		if (!$planodeimplementacao['cepmaterialdiddatico']) {
			$msg [] = "<br> - A Tela de Endereço Entrega Mat. Didático não foi gravada.";
		}
	/* FIM validando Endereço de Entrega */
	/*
	 * VALIDANDO TRANSPORTE DIDATICO - verifica se a aba transporte didatico foi gravado pelo menos uma vez; - atualiza o percentual utilizado em transporte didatico (caso tenha ocorrido alguma falha)
	*/
		
		$matepercmax = '1.5';
			
		$tmavlrmaximo = round( ( ($matepercmax*$montante)/100 ), 2 );
		
		$totalutilizado_transportematerial = $planodeimplementacao['valormaterialdidatico'];
		
		if (!$totalutilizado_transportematerial) {
			$msg [] = "<br> - A Tela de Transporte Didático não foi gravada.";
		}
			
		if ($totalutilizado_transportematerial > $tmavlrmaximo) {
			$msg [] = "<br> - Recursos Utilizados em transporte didático é maior que a percentagem prevista.";
		}
// 	}
	/* FIM validando transporte didatico */
/*
 * VALIDANDO DEMAIS AÇÕES - verifica se a aba demais ações foi gravado pelo menos uma vez; - atualiza o percentual utilizado em demais ações (caso tenha ocorrido alguma falha)
*/
	$totalUtilizado_profissionais = $totalUtilizado_profissionais > 0 ? $totalUtilizado_profissionais : '0';
	$totalutilizado_formacao = $totalutilizado_formacao > 0 ? $totalutilizado_formacao : '0';
	$totalutilizado_auxiliofinanceiro = $totalutilizado_auxiliofinanceiro > 0 ? $totalutilizado_auxiliofinanceiro : '0';
	$totalutilizado_generoalimenticio = $totalutilizado_generoalimenticio > 0 ? $totalutilizado_generoalimenticio : '0';
	$totalutilizado_qualificacaoprofissional = $totalutilizado_qualificacaoprofissional > 0 ? $totalutilizado_qualificacaoprofissional : '0';
	$totalutilizado_transportematerial = $totalutilizado_transportematerial > 0 ? $totalutilizado_transportematerial : '0';

	$totalmaxdemaisacoes = $montante - ($totalUtilizado_profissionais + $totalutilizado_formacao + $totalutilizado_auxiliofinanceiro + $totalutilizado_generoalimenticio + $totalutilizado_qualificacaoprofissional + $totalutilizado_transportematerial);

	// Gatilho pra não precisar refazer todos os calculos, reaproveitando o código
	if ($retornarTotalMaximoDemaisAcoes){
		return $totalmaxdemaisacoes;
	}
	$totalutilizado_demaisacoes = $db->pegaUm ( "SELECT SUM(valortotal) as x FROM projovemcampo.tipodemaisacoesplano WHERE pimid='" . $_SESSION ['projovemcampo'] ['pimid'] . "'" );
	
	$totalUtil_demaisacoes = str_replace ( '.', '', number_format ( $totalutilizado_demaisacoes, 2, '', '.' ) );
	$totalMax_demaisacoes = str_replace ( '.', '', number_format ( $totalmaxdemaisacoes, 2, '', '.' ) );
	
	if ($totalUtil_demaisacoes > $totalMax_demaisacoes) {
		$msg [] = "<br> - Gastos com Demais ações é maior que a percentagem prevista.";
	}
	
	/* FIM validando demais ações */


// return "Validação completa ainda não esta disponivel. Em breve poderá enviar o Plano de Implementação para analise do MEC";

// if(date("Y-m-d")>"2012-01-23") return "Prazo para envio do Plano de implementação terminou. Obrigado!";

/* VALIDANDO Endereço de Entrega */

	if(!is_array($msg)){
		return true;
	}else{
		return implode('',$msg);
	}
	die;
}
function liberademaisacoes() {
	global $db;
	
	$teste = true;
	/*
	* VALIDANDO PROFISSIONAIS - verifica se a aba profissionais foi gravado pelo menos uma vez; - verifica se o valor total de profissionais é maior que o percentual previsto; - atualiza o percentual utilizado (caso tenha ocorrido alguma falha)
	*/
	$coordenadorgeral = pegarocpid('1');

	if (!$coordenadorgeral) {
		$teste = false;
	}
/* FIM validando profissionais */

/*
 * VALIDANDO FORMAÇÃO
*/

	$sql = "SELECT
				SUM(valor)
			FROM
				projovemcampo.tiposrecursoformacao tpf
			LEFT JOIN projovemcampo.tiporecursoplanoimplementacao trp ON trp.tpfid=tpf.tpfid
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}";
	$totalutilizado_formacao = $db->pegaUm($sql);

	if (!$totalutilizado_formacao) {
		$teste = false;
	}

/* FIM validando formação */

/*
 * VALIDANDO GENÊRO ALIMENTICIOS
*
* - verifica se a aba genero alimenticios foi gravado pelo menos uma vez;
* - atualiza o percentual utilizado dos generos alimenticios (caso tenha ocorrido alguma falha)
			*
			*/

	$sql = "SELECT
				gaqtdcriancas,
				gaqtdmeses,
				gavalormensal
			FROM
				projovemcampo.planodeimplementacao
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']} ";

	$generoalimenticio = $db->pegaLinha($sql);

	if (!$generoalimenticio['gaqtdcriancas']) {
		$teste = false;
	}

/* FIM validando genêro alimenticios */

/*
* VALIDANDO QUALIFICAÇÃO PROFISSIONAL
*/

	$sql = "SELECT
				qtdmeses*valor as valortotal
			FROM
				projovemcampo.qualificacaoprofissionaldespesa qpd
			LEFT JOIN projovemcampo.despesaqualificacaoplano dqp ON dqp.qpdid = qpd.qpdid
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}";
	$qualificacaoprofissional = $db->Carregar($sql);

	if (!is_array($qualificacaoprofissional)) {
		$teste = false;
	}

/* FIM validando qualificação profissional */
	if ($_SESSION ['projovemcampo'] ['estuf']){
		/*
	 * VALIDANDO TRANSPORTE DIDATICO - verifica se a aba transporte didatico foi gravado pelo menos uma vez; - atualiza o percentual utilizado em transporte didatico (caso tenha ocorrido alguma falha)
		*/

		 $sql = "SELECT
				 	valormaterialdidatico
				 FROM
				 	projovemcampo.planodeimplementacao 
				 WHERE 
		 			pimid={$_SESSION['projovemcampo']['pimid']}";

		 $valormaterialdidatico = $db->pegaUm($sql);
		 	
		if (!$valormaterialdidatico) {
			$teste = false;
		}
	}
	$teste = true;
/* FIM validando transporte didatico */
	return $teste;
	die;
}
?>