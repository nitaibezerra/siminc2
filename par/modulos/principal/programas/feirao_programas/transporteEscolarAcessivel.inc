<?php

$qtdTotal = 0;
$qntPermitida = 0;

if( $_SESSION['par']['itrid'] == 1 ){ //estadual
	$w = " AND ppaid in (405, 413, 414, 415)";
	$crtid = "(450, 451, 452, 453)";
	$ppsid = 1075;
	if( $_SESSION['par']['estuf'] == 'DF' ){
		$sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.mun_estuf = map.estuf WHERE iu.mun_estuf = '".$_SESSION['par']['estuf']."'";
	} else {
		$sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.estuf = map.estuf WHERE iu.estuf = '".$_SESSION['par']['estuf']."' AND iu.itrid = 1";
	}
	$qntPermitida = $db->pegaUm( $sql );
} elseif( $_SESSION['par']['itrid'] == 2 ){ //municipal
	$w = " AND ppaid in (379, 380, 381, 382)";
	$crtid = "(381, 382, 383, 384)";
	$ppsid = 1074;
	$sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.muncod = map.muncod WHERE iu.muncod = '".$_SESSION['par']['muncod']."' AND iu.itrid = 2";
	$qntPermitida = $db->pegaUm( $sql );
}


$sql = "SELECT s.sbaid 
		FROM par.subacao s 
		INNER JOIN par.acao a ON a.aciid = s.aciid
		INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.inuid = {$_SESSION['par']['inuid']} AND s.sbastatus = 'A'
		WHERE s.ppsid = {$ppsid}";

$sbaid = $db->pegaUm($sql);

if(!empty($sbaid)){
	$sql_cotaaderido = "SELECT  	CASE WHEN f.sbaid IS NULL THEN 0 ELSE (COALESCE(SUM(icoquantidadetecnico),0)) END AS qtdAderido
						FROM 		par.subacaoitenscomposicao i
						LEFT JOIN 	( SELECT 		DISTINCT sbaid 
									  FROM 			par.empenhosubacao es
                               		  INNER JOIN 	par.empenho e ON e.empid = es.empid  and empstatus = 'A' and eobstatus = 'A'
                               		  WHERE 		eobstatus = 'A' AND e.empsituacao = '2 - EFETIVADO' AND eobano = '2012') AS f ON f.sbaid = i.sbaid						
						WHERE 		i.sbaid = {$sbaid} AND i.icoano = 2012 AND i.icovalidatecnico = 'S'
						GROUP BY  	f.sbaid";
	$cota_aderido = $db->pegaUm($sql_cotaaderido);
	
	$sql_cotaaderido2013 = "SELECT  	CASE WHEN f.sbaid IS NULL THEN 0 ELSE (COALESCE(SUM(icoquantidadetecnico),0)) END AS qtdAderido
							FROM 		par.subacaoitenscomposicao i
							LEFT JOIN 	( SELECT 		DISTINCT sbaid 
										  FROM 			par.empenhosubacao es
	                               		  INNER JOIN 	par.empenho e ON e.empid = es.empid and empstatus = 'A' and eobstatus = 'A'
	                               		  WHERE 		eobstatus = 'A' AND e.empsituacao = '2 - EFETIVADO' AND eobano = '2013') AS f ON f.sbaid = i.sbaid						
							WHERE 		i.sbaid = {$sbaid} AND i.icoano = 2013 AND i.icovalidatecnico = 'S'
							GROUP BY  	f.sbaid";
	$cota_aderido_2013 = $db->pegaUm($sql_cotaaderido2013);
} else {
	$cota_aderido = 0;
	$cota_aderido_2013 = 0;
}

$nova_cota = $qntPermitida - $cota_aderido;
$nova_cota_2013 = $qntPermitida - $cota_aderido_2013;

if( $_POST['requisicao'] == 'salvar' ){
	if(!empty($sbaid)){ //Edição
		if( is_array($_POST['qtd']) ){
			$sqlInsert = "";
			$sql = "DELETE FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} AND icoano = {$_POST['ano']}";
			$db->executar($sql);
			
			foreach( $_POST['qtd'] as $picid => $qt ){
				if($qt){
					//Pega os dados
					$sql = "SELECT pic.picdescricao, pic.picdetalhe, pic.umiid, dic.dicvalor 
							FROM par.propostaitemcomposicao pic 
							INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid 
							WHERE pic.picid = {$picid}";
					
					$dados = $db->pegaLinha( $sql );
					
					(float)$valorTotal = ((float)$qt * (float)$dados['dicvalor']);
					$qtdTotal = $qtdTotal + $qt;
					
					//Deleta os dados
					$sql = "DELETE FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} AND picid = {$picid} AND icoano = {$_POST['ano']}";
					$db->executar( $sql );

					//Insere os dados dos itens de composição
					$sqlInsert .= "INSERT INTO par.subacaoitenscomposicao
								( icoano, icodescricao, icoquantidade, icostatus, icovalor, icovalortotal, sbaid, unddid, icodetalhe, usucpf, dtatualizacao, picid ) 
							VALUES 
								({$_POST['ano']},'{$dados['picdescricao']}', {$qt}, 'A', {$dados['dicvalor']}, {$valorTotal}, {$sbaid}, {$dados['umiid']}, '{$dados['picdetalhe']}', '{$_SESSION['usucpf']}', 'NOW()', {$picid}); ";
				}
			}
		}
		
		if( $qtdTotal > $nova_cota){
			$db->rollback();
			echo '<script>alert("A cota máxima para 2013 é de '.$nova_cota.'!"); history.back(-1);</script>';
			return false;
		} else {
			$db->executar($sqlInsert);
		}
		
		$sql = "UPDATE par.subacaodetalhe SET sbdquantidade = {$qtdTotal} WHERE sbdano = {$_POST['ano']} AND sbaid = {$sbaid}";
		$db->executar( $sql );
		
	} else { //Inclusão
	
		$aciid = $db->pegaUm( "SELECT a.aciid from par.acao a
								inner join par.pontuacao p on p.ptoid = a.ptoid 
								WHERE inuid = ".$_SESSION['par']['inuid']." ".$w );

		if( !$aciid ){
			// Verifico se tem pontuação
			$dados = $db->pegaLinha( "select p.ptoid, p.crtid FROM 
									par.pontuacao p
									inner join par.criterio c on c.crtid = p.crtid AND c.crtid in {$crtid}
									WHERE p.inuid = ".$_SESSION['par']['inuid']." AND p.ptostatus = 'A'" );
			
			if( !$dados['ptoid'] ){
				
				$sqlInsertdados = "
					INSERT INTO par.pontuacao
						(crtid, ptodata, usucpf, inuid, ptostatus, ptojustificativa, ptodemandamunicipal)
					VALUES
						(382, now(), '{$_SESSION['usucpf']}', {$_SESSION['par']['inuid']}, 'A', 'A ação de disponibilização de transporte escolar acessível está inserida no Plano Nacional dos Direitos da Pessoa com Deficiência – Viver sem Limite. São 1530 municípios com o maior número de beneficiários do BPC, em idade escolar, fora da escola.', '1530 municípios com o maior número de beneficiários do BPC, em idade escolar, fora da escola.')
					
					RETURNING ptoid, crtid;
				";
				
				$dados = $db->pegaLinha($sqlInsertdados);
				
			} 
			//Insere a Ação!
			$oPontuacaoControle = new PontuacaoControle();
			$arPropostaAcao = $oPontuacaoControle->recuperarPropostaAcoesPorCriterio($dados['crtid']);
			$arPropostaAcao = ($arPropostaAcao) ? $arPropostaAcao : array();
			
			if($arPropostaAcao){
				$oAcao = new Acao();
				$oAcao->ppaid  	  = $arPropostaAcao['ppaid'];
				$oAcao->ptoid  	  = $dados['ptoid'];
				$oAcao->acistatus = 'A';
				$oAcao->acidsc 	  = $arPropostaAcao['ppadsc'];
				$aciid 		   	  = $oAcao->salvar();
				$oAcao->commit();
			}
				
			
		}
		
		$propostaSubacao = $db->pegaLinha( "SELECT * FROM par.propostasubacao WHERE ppsid = ".$ppsid );
		
		$estado = $db->pegaUm("SELECT d.esdid FROM par.instrumentounidade iu
						INNER JOIN workflow.documento d ON d.docid = iu.docid
						WHERE iu.inuid = ".$_SESSION['par']['inuid']);
		
		if( $estado == WF_ANALISE ){
			$desc = 'Em Análise';
			$esdid = WF_SUBACAO_ANALISE;
		} else {
			$desc = 'Em Elaboração';
			$esdid = WF_SUBACAO_ELABORACAO;
		}
		
		$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
				VALUES (62, ".$esdid.", '".$desc."', now()) returning docid ";
		$docid = $db->pegaUm($sql);
		
		//Insere a subação na árvore
		$oSubacao = new Subacao();		
		
		$oSubacao->sbaid 					  = null;
		$oSubacao->sbadsc 					  = $propostaSubacao['ppsdsc'];
		$oSubacao->sbaordem 				  = $propostaSubacao['ppsordem'];
		$oSubacao->sbaobra 					  = $propostaSubacao['sbaobra'];
		$oSubacao->sbaestrategiaimplementacao = $propostaSubacao['ppsestrategiaimplementacao'];
		$oSubacao->sbaptres 				  = $propostaSubacao['ppsptres'];
		$oSubacao->sbanaturezadespesa 		  = $propostaSubacao['ppsnaturezadespesa'];
		$oSubacao->sbamonitoratecnico 		  = $propostaSubacao['ppsmonitora'];
		$oSubacao->sbastatus				  = 'A';
		$oSubacao->frmid 					  = $propostaSubacao['frmid'];
		$oSubacao->ptsid 					  = $propostaSubacao['ptsid'];
		$oSubacao->indid 					  = $propostaSubacao['indid'];
		$oSubacao->foaid 					  = $propostaSubacao['foaid'];
		$oSubacao->undid 					  = $propostaSubacao['undid'];
		$oSubacao->ppsid 					  = $ppsid;
		$oSubacao->sbacronograma 			  = $propostaSubacao['ppscronograma'];
		$oSubacao->prgid					  = $propostaSubacao['prgid'];
		$oSubacao->sbaextraordinaria		  = 7;
		$oSubacao->aciid 					  = $aciid;
		$oSubacao->docid 					  = $docid;
		$sbaid = $oSubacao->salvar();
		
		if( $sbaid ){
			if( is_array($_POST['qtd']) ){
				$sqlInsert = "";
				foreach( $_POST['qtd'] as $picid => $qt ){
					if( $qt ){
					
						//Pega os dados
						$sql = "SELECT pic.picdescricao, pic.picdetalhe, pic.umiid, dic.dicvalor 
								FROM par.propostaitemcomposicao pic 
								INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid 
								WHERE pic.picid = {$picid}";
						$dados = $db->pegaLinha( $sql );
						
						(float)$valorTotal = ((float)$qt * (float)$dados['dicvalor']);
	
						$qtdTotal = $qtdTotal + $qt;
						
						//Insere os dados dos itens de composição
						$sqlInsert .= "INSERT INTO par.subacaoitenscomposicao
									( icoano, icodescricao, icoquantidade, icostatus, icovalor, icovalortotal, sbaid, unddid, icodetalhe, usucpf, dtatualizacao, picid ) 
								VALUES 
									( ".$_POST['ano'].", '".$dados['picdescricao']."', ".$qt.", 'A', ".$dados['dicvalor'].", ".$valorTotal.", ".$sbaid.", ".$dados['umiid'].", '".$dados['picdetalhe']."', '".$_SESSION['usucpf']."', 'NOW()', ".$picid." ); ";
					}
				}
			}
			
			if( $qtdTotal > $nova_cota ){
				$db->rollback();
				echo '<script>alert("A maior quantidade permitida é de '.$nova_cota.'!"); history.back(-1);</script>';
				return false;
			} else {
				$db->executar( $sqlInsert );
			}

			$sql = "INSERT INTO par.subacaodetalhe(
				            sbaid, sbdquantidade, sbdano, sbdinicio, sbdfim, sbdanotermino)
				    VALUES
				    		(".$sbaid.", ".$qtdTotal.", ".$_POST['ano'].", 1, 12, 2014)";
			$db->executar( $sql );
			
		}
	}
	$db->commit();
	echo '<script>alert("Operação realizada com sucesso!");</script>';
}

ini_set( "memory_limit", "1024M" );

include_once "termoprofuncionario.inc";

$aderiu = testaAdesaoProFuncionario(); ?>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" onclick="window.location.href='?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" id="btnVoltar" />
		</td>
	</tr>
</table>

<?php 
if( $aderiu == 'S' ){
	include_once APPRAIZ . 'includes/cabecalho.inc';
	
	echo '<br>';
	echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/transporteEscolarAcessivel&acao=A');
	monta_titulo( $titulo_modulo, recuperaMunicipioEstado() );

	if( $_SESSION['par']['itrid'] == 1 ){ //estadual
		$ppsid = 1075;
	} elseif( $_SESSION['par']['itrid'] == 2 ){ //municipal
		$ppsid = 1074;
	}	
	
	$sql = "SELECT 	pic.picid 			AS codigo, 
					pic.picdescricao 	AS descricao, 
					'' 					AS qtd, 
					pic.picdetalhe 		AS detalhe
			FROM par.propostaitemcomposicao 	pic 
			INNER JOIN par.propostasubacaoitem  psi ON psi.picid = pic.picid
			INNER JOIN par.propostasubacao 		ps  ON ps.ppsid  = psi.ppsid
			WHERE ps.ppsid ={$ppsid}";

	$itens = $db->carregar( $sql );
?>

<script type="text/javascript" src="../includes/highslide/highslide-with-gallery.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/highslide/highslide.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script>
	hs.graphicsDir = '../includes/highslide/graphics/';
	hs.align = 'center';
	hs.transitions = ['expand', 'crossfade'];
	hs.wrapperClassName = 'dark borderless floating-caption';
	hs.fadeInOut = true;
	hs.dimmingOpacity = .75;
	
	// Add the controlbar
	if (hs.addSlideshow) hs.addSlideshow({
		//slideshowGroup: 'group1',
		interval: 5000,
		repeat: false,
		useControls: false,
		fixedControls: 'fit',
		overlayOptions: {
			opacity: .6,
			position: 'bottom center',
			hideOnMouseOut: true
		}
	});
	
	function salvar(){
		form = document.formulario;
		erro = true;
		
		divCarregando();

		$('[name^=qtd]').each(function() {
			if( this.value ){
				erro = false; 
			}
		});
		
		if( erro == true ){
			alert("Você precisa preencher uma quantidade!");
			divCarregado();
			return false;
		}
		
		
		form.requisicao.value = 'salvar';
		form.submit();
	}
</script>

<?php
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
$programa = pegarProgramaDisponivel();
$programa = $programa ? $programa : array();
$habilitaSalvar = true;

$docid = pgPegarDocid( $_SESSION['par']['adpid'] );
$esdid = prePegarEstadoAtual( $docid );

if( (
	(in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
	(in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ||
	(in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrayPerfil) && in_array(PROGRAMA_ONIBUS_ACESSIVEL, $programa)) ||
	(in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrayPerfil) && in_array(PROGRAMA_ONIBUS_ACESSIVEL, $programa))||
	(in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) && in_array(PROGRAMA_ONIBUS_ACESSIVEL, $programa)) ||
	(in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil) && in_array(PROGRAMA_ONIBUS_ACESSIVEL, $programa)) ||
	(in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) && in_array(PROGRAMA_ONIBUS_ACESSIVEL, $programa))
	) && WF_PRO_TRANSPORTE_EM_CADASTRAMENTO == $esdid))){
	$habilitaSalvar = true;
}else{
	$habilitaSalvar = false;
}

if( $_SESSION['par']['itrid'] == 1 ){ //Estadual
	$ppsid = 1075;
	if( $_SESSION['par']['estuf'] == 'DF' ){
		$sql = "SELECT map.mprqtd 
				FROM par.municipioadesaoprograma map 
				INNER JOIN par.instrumentounidade iu ON iu.mun_estuf = map.estuf 
				WHERE iu.mun_estuf = '{$_SESSION['par']['estuf']}'";
	} else {
		$sql = "SELECT map.mprqtd 
				FROM par.municipioadesaoprograma map 
				INNER JOIN par.instrumentounidade iu ON iu.estuf = map.estuf 
				WHERE iu.estuf = '{$_SESSION['par']['estuf']}' AND iu.itrid = 1";
	}
	$qntPermitida = $db->pegaUm($sql);
} elseif( $_SESSION['par']['itrid'] == 2 ){ //Municipal
	$ppsid = 1074;
	$sql = "SELECT map.mprqtd 
			FROM par.municipioadesaoprograma map 
			INNER JOIN par.instrumentounidade iu ON iu.muncod = map.muncod 
			WHERE iu.muncod = '{$_SESSION['par']['muncod']}' AND iu.itrid = 2";
	$qntPermitida = $db->pegaUm($sql);
}
?>

<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="requisicao" value="">
	<input type="hidden" name="ano" value="2014"><!-- Ano Exercicio -->
	<table border="0" bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td width="90%" valign="top">
				<table border="0" style="border-bottom: 0px;" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td class="SubTituloDireita" width="10%">Cota em 2012:</td>
						<td colspan="3">&nbsp;<?php echo $qntPermitida; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="10%">Cota máxima permitida em 2013:</td>
						<td colspan="3">&nbsp;<?php echo $nova_cota; ?></td>
					</tr>					
					<tr>
						<td align="center" style="background: rgb(238, 238, 238)"><b>Ônibus</b></td>
						<td align="center" style="background: rgb(238, 238, 238)" width="10%"><b>Qtd. Aderido no ano 2012</b></td>
						<td align="center" style="background: rgb(238, 238, 238)" width="10%"><b>Qtd. a Aderir no ano 2013</b></td>
						<td align="center" style="background: rgb(238, 238, 238)" width="10%"><b>Qtd. a Aderir no ano 2014</b></td>
						<td align="center" style="background: rgb(238, 238, 238)" width="20%"><b>Descrição dos Ônibus</b></td>
					</tr>
					<?php
					if( is_array($itens) ){
						foreach( $itens as $item ){
							if(!empty($sbaid)){
								$sql_12 = "SELECT icoquantidade
										FROM par.subacaoitenscomposicao
										WHERE icoano = 2012 AND sbaid = {$sbaid} AND picid = {$item['codigo']}
										ORDER BY icoano";
								$itenssub12 = $db->pegaLinha($sql_12);
								
								$sql_13 = "SELECT icoquantidade
										FROM par.subacaoitenscomposicao
										WHERE icoano = 2013 AND sbaid = {$sbaid} AND picid = {$item['codigo']}
										ORDER BY icoano";

								$itenssub13 = $db->pegaLinha($sql_13);
														
								$sql_14 = "SELECT icoquantidade
										FROM par.subacaoitenscomposicao
										WHERE icoano = 2014 AND sbaid = {$sbaid} AND picid = {$item['codigo']}
										ORDER BY icoano";

								$itenssub14 = $db->pegaLinha($sql_14);						
							} ?>
					<tr>
						<td><?php echo $item['descricao']; ?></td>
						<td align="center"><?php echo empty($itenssub12['icoquantidade']) ? '0' : $itenssub12['icoquantidade']; ?></td>
						<td align="center"><?php echo empty($itenssub13['icoquantidade']) ? '0' : $itenssub13['icoquantidade']; ?></td>
						<td align="center"><input class="normal" maxlength="6" type="text" value="<?php echo $itenssub14['icoquantidade'];?>" name="qtd[<?php echo $item['codigo']; ?>]" size="10" id="qtd[<?php echo $item['codigo']; ?>]" style="text-align:center" onkeypress="return somenteNumeros(event);"></td>
						<td>
							<a href="../imagens/par/onibus<?php echo $item['codigo']; ?>.jpg" class="highslide" onclick="return hs.expand(this);"><img src="../imagens/par/onibus<?php echo $item['codigo']; ?>-min.jpg" title="Clique para ver em tamanho maior"></a><br><?php echo $item['detalhe']; ?>
							<div class="highslide-caption"><?php echo $item['descricao']; ?></div>
						</td>
					</tr>
					<?php }} else { ?>
					<tr>
						<td colspan="4">Sem itens cadastrados</td>
					</tr>
					<?php } } else {
						echo '<script>history.back(-1);</script>';
					} ?>
				</table>
			</td>
			<td align="center" valign="top">
				<?php $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
				wf_desenhaBarraNavegacao(pgTransporteCriarDocumento( $_SESSION['par']['adpid'] ), $dados_wf); ?>
			</td>
		</tr>
		<tr style='background-color: #DCDCDC'>
			<td colspan="2" align="center">
				<?php if($habilitaSalvar){ ?>
				<div style="float:center;"><input type="button" value="Salvar" onclick="salvar();"></div>
				<?php } ?>
			</td>
		</tr>
	</table>
	<br>
	<?php
		$docid = pgPegarDocid( $_SESSION['par']['adpid'] );
		$esdid = prePegarEstadoAtual( $docid );

		if( $esdid == WF_PRO_TRANSPORTE_EM_DILIGENCIA ){
			$sql = "SELECT * FROM workflow.comentariodocumento 
					WHERE cmdid = (SELECT MAX( cmdid ) FROM workflow.comentariodocumento WHERE docid = {$docid} AND cmdstatus = 'A' )";
			$dados = $db->pegaLinha( $sql );
			
		monta_titulo( 'Diligência', '' ); ?>
	<table style="border-bottom: 0px;" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" width="10%">Diligência:</td>
			<td colspan="2"><?php echo $dados['cmddsc']; ?></td>
		</tr>
	</table>
	<?php } ?>
</form>

<?php if($esdid != WF_PRO_TRANSPORTE_EM_CADASTRAMENTO){ ?>
	<script>
		jQuery('input').attr('disabled', true);
		jQuery('#btnVoltar').attr('disabled', false);
	</script>
<?php } ?>
