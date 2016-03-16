<?

if($_REQUEST['req'] == 'ACsidoc'){
	ob_clean();
	$sql = "SELECT prcnumsidoc FROM profeinep.processoprofeinep"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($key), $q) !== false) {
			$prcnumsidoc = trim($value['prcnumsidoc']);
			echo "{$prcnumsidoc}|{$prcnumsidoc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACprcnomeinteressado'){
	ob_clean();
	$sql = "SELECT prcnomeinteressado FROM profeinep.processoprofeinep"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['prcnomeinteressado']), $q) !== false) {
			$prcnomeinteressado = $value['prcnomeinteressado'];
			echo "{$prcnomeinteressado}|{$prcnomeinteressado}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACtasdsc'){
	ob_clean();
	$sql = "SELECT tasdsc FROM profeinep.tipoassunto WHERE tasstatus = 'A' ORDER BY tasdsc"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['tasdsc']), $q) !== false) {
			$tasdsc = $value['tasdsc'];
			echo "{$tasdsc}|{$tasdsc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACprodsc'){
	ob_clean();
	$sql = "SELECT prodsc FROM profeinep.procedencia WHERE unpid = '".$_REQUEST['unpid']."' AND prodtstatus = 'A' ORDER BY	prodsc"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['prodsc']), $q) !== false) {
			$prodsc = $value['prodsc'];
			echo "{$prodsc}|{$prodsc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['requisicaoAjax'] != '') {
//	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']($_REQUEST);
	die();
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}

include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

/* Gravando a variável de sessão */
if($_REQUEST['prcid'])
	$_SESSION['profeinep_var']['prcid'] = $_REQUEST['prcid'];
	
/* Tratamento dos documentos */
$docid = pegarDocid($_SESSION['profeinep_var']['prcid']);
if(!$docid) {
	$docid = criarDocumento($_SESSION['profeinep_var']['prcid']);
}

// verifica se muda a cor da aba de documento
$boMudaCorAba = verificaSeMudaCor($_SESSION['profeinep_var']['prcid']);


// Verificando se projeto está aguardando resposta externa e tem data de prazo prevista

/* Carregando o projeto PROFE/INEP, testando os possíveis erros */
if($_SESSION['profeinep_var']['prcid']) {
	$sql = "SELECT 
				proj.*, 
				had.advid as advogado,
				pro.unpid,
				pro.prodsc,
				esd.esdid,
				tas.tasdsc,
				espdtrespexterna as prazo,
				espnumdiasrespexterna as prazo_old,
				proj.tacid  
			FROM profeinep.processoprofeinep proj
			LEFT JOIN (SELECT max(hadid), advid, prcid, dtatribuicao 
					   FROM profeinep.historicoadvogados 
					   GROUP BY hadid,advid, prcid, dtatribuicao
					   ORDER BY hadid DESC ) had ON had.prcid = proj.prcid
			LEFT JOIN profeinep.tipoassunto 	   tas ON tas.tasid = proj.tasid
			LEFT JOIN profeinep.procedencia 	   pro ON pro.proid = proj.proid
			LEFT JOIN profeinep.estruturaprocesso esp ON esp.prcid = proj.prcid
			LEFT JOIN workflow.documento 	   doc ON doc.docid = esp.docid
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE proj.prcid='".$_SESSION['profeinep_var']['prcid']."'";
	$processoprofeinep = $db->pegaLinha($sql);
	
	if($processoprofeinep) {
		if( $processoprofeinep['esdid'] == WF_AGUARDANDO_RESPOSTA_EXTERNA ){
			if( $processoprofeinep['prazo'] == '' && $processoprofeinep['prazo_old'] == '' ){
				direcionar('?modulo=principal/prazoRespostaExterna&acao=C','Processo não possui prazo para resposta externa.');
			}
			$prazo = $processoprofeinep['prazo'];
		}
		
		$cooid = $processoprofeinep['cooid'];
		$advid = $processoprofeinep['advogado'];
		$prcnumsidoc = $processoprofeinep['prcnumsidoc'];
		$prcnumeroprocjudicial = $processoprofeinep['prcnumeroprocjudicial'];
		$prcnomeinteressado = $processoprofeinep['prcnomeinteressado'];
		$tprid = $processoprofeinep['tprid'];
		$_SESSION['profeinep_var']['tprid'] = $tprid;
		$prcdtentrada 	= $processoprofeinep['prcdtentrada'];
		$proid 		  	= $processoprofeinep['proid'];
		$prodsc 	  	= $processoprofeinep['prodsc'];
		$unpid 	      	= $processoprofeinep['unpid'];
		$tasid 		  	= $processoprofeinep['tasid'];
		$tasdsc			= $processoprofeinep['tasdsc'];
		$prcdesc 	  	= $processoprofeinep['prcdesc'];
		$tipid 		    = $processoprofeinep['tipid'];
		$tacid 		    = $processoprofeinep['tacid'];
		$prcprioritario = $processoprofeinep['prcprioritario'];
		$prcnumeroprocjudantigo = $processoprofeinep['prcnumeroprocjudantigo'];
	} else {
		direcionar('?modulo=inicio&acao=C','Não existe o projeto solicitado. Entre em contato com a equipe técnica.');
	}
} else {
	direcionar('?modulo=inicio&acao=C','Ocorreu um problema na variável de sessão, caso o erro persista entre em contato com a equipe técnica.');
}

$estado_documento = wf_pegarEstadoAtual( $docid );
$dadosestruturaprocessoid = $db->pegaUm("SELECT espid FROM profeinep.estruturaprocesso WHERE prcid = '".$_SESSION['profeinep_var']['prcid']."'");
/* FIM - Tratamento dos documentos */

/* Tratamento de segurança */
$permissoes = verificaPerfilProfeinep($estado_documento['esdid'],$cooid);
$perfilid = $perfilid == '' ? $_SESSION['profeinep']['perfilid'] : $perfilid;
/* FIM - Tratamento de segurança */
$_SESSION['profeinep']['coiid'] = $cooid;
echo "<br/>";
echo montarAbasArray(monta_abas_processo($_SESSION['profeinep_var']['tprid']),$_SERVER['REQUEST_URI']);
monta_cabecalho_profeinep($_SESSION['profeinep_var']['prcid']);

// se não for administrador, não pode alterar alguns campos
$boAdmin = false;
if( possuiPerfil(PRF_ADMINISTRADOR) )
{
	$boAdmin = true;
}

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="./js/profeinep.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<script>

	jQuery.noConflict();

	jQuery(document).ready(function(){
		// Serve para mudar a cor da aba documentos, quando existe número gerado e não tem arquivo anexado
		var boMudaCorAba = '<?php echo $boMudaCorAba;?>';
		if(!boMudaCorAba){
			if(jQuery('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().text() != 'Documentos'){
				jQuery('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			} else {
				jQuery('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			}
			
		}

		jQuery('#prodsc').removeClass('normal ac_input');
		jQuery('#prodsc').addClass('disabled ac_input');
		
		jQuery('#prcnumsidoc').autocomplete("profeinep.php?modulo=principal/editarprocesso&acao=A&req=ACsidoc", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});
		
		jQuery('#prcnomeinteressado').autocomplete("profeinep.php?modulo=principal/editarprocesso&acao=A&req=ACprcnomeinteressado", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});
		
		jQuery('#tasdsc').autocomplete("profeinep.php?modulo=principal/editarprocesso&acao=A&req=ACtasdsc", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});

		jQuery('#prodsc').autocomplete("profeinep.php?modulo=principal/editarprocesso&acao=A&req=ACprodsc", {
			extraParams: 
			{
		       unpid: function() { return jQuery("#unpid").val(); }
			},
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});

		jQuery('#unpid').change(function(){
			if(jQuery(this).val()!='')
			{
				if(jQuery('#prodsc').hasClass('disabled'))
				{
					jQuery('#prodsc').removeClass('disabled ac_input');
					jQuery('#prodsc').addClass('normal ac_input');
				}
			}
			else
			{
				jQuery('#prodsc').removeClass('normal ac_input');
				jQuery('#prodsc').addClass('disabled ac_input');
				
			}
		});

//		jQuery('#advid').live('change',function(){
//			if( jQuery(this).val() == '' ){
//				return false;
//			}
//			if(confirm('Esta ação resultará no encaminhamento desse processo ao advogado escolhido.\nDeseja prosseguir?')){
//				tramitaAdvogado( jQuery(this).val() );
////				window.location = window.location;
//			}
//		});
	});

	function tramitaAdvogado(advid)
	{
		if( advid == '' ){
			return false;
		}
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=encaminhaParaAdvogado&advid=" + advid,
			success: function(data) {
//				alert(data);
				//window.location = window.location;
				location.href="profeinep.php?modulo=principal/editarprocesso&acao=A&prcid=<?=$_SESSION['profeinep_var']['prcid']?>";
		  	}
		});
	}
	

	function verprocesso( prcid ){
		window.location = '?modulo=principal/editarprocesso&acao=A&prcid=' + prcid;
	}
	function exibirOcultarNumeracaoUnica(value) {
		var isJudicial = value==2; // 2, conforme ID na base de dados 
		if( isJudicial ) {
			document.getElementById('linhaNumeracaoUnicaJudicial').style.visibility = "visible";
		} else {
			document.getElementById('linhaNumeracaoUnicaJudicial').style.visibility = "collapse";
			//document.formulario.prcnumeroprocjudicial.value = "";			
		}
	}	
	function profeinepJs_digitoVerificador() {
		var campoNumeroProcessoSidoc    = document.getElementById('prcnumsidoc');
		var campoNumeroProcessoSidocbkp = document.getElementById('prcnumsidoc2').value;
		
		if(!conferirDigitoVerificadorModulo11(campoNumeroProcessoSidoc)){
			document.getElementById('prcnumsidoc').value = campoNumeroProcessoSidocbkp;
			return false;
		} 
	}
	
	function exibeAdvogados(value){
		if(!value){
			jQuery("#advid").val("");
			jQuery("#advid").attr("disabled","disabled");
		}else{
			jQuery("#advid").attr("disabled","disabled");
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax=carregarAdvogados&cooid=" + value,
				success: function(data) {
//					alert(data);
					jQuery("#combo_advogados").html(data);
			  }
			});
		}
	}
	function atualizaAdvogado(advid)
	{
		if(!advid){
			return false;
		}
		<?php if($docid): ?>
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=carregarWorkflow&docid=<?=$docid?>&advid=" + advid,
			success: function(data) {
				jQuery("#barra_workflow").html(data);
			}
		});
		<?php endif; ?>
		window.location = window.location;
	}
	function atualizaCoordenacao(cooid)
	{
		if(!cooid){
			cooidv = "null";
		}
			<?php if($docid): ?>
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax=carregarWorkflow2&docid=<?=$docid?>&cooid=" + cooid,
				success: function(data) {
					jQuery("#barra_workflow").html(data);
				}
			});
			<?php endif; ?>
	}

</script>
<?php 
$bloqPrazo = 'S';
if( $estado_documento['esdid'] == ESDID_AGUARDANDO_RESPOSTA_EXTERNA ){
	$bloqPrazo = 'N';
}
?>
<form name="formulario" id="formulario" method="post" action="">
<input type="hidden" name="requisicao" value="atualizarprocessoprofeinep">
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor="#CCCCCC" colspan="2"><b>Cadastro de Processo</b></td>
		<td bgcolor="#CCCCCC" valign="top" id="barra_workflow" >
			<?php 
			$arrEsdPrazo = Array(ESDID_AGUARDANDO_PROVIDENCIA,ESDID_AGUARDANDO_RESPOSTA_EXTERNA);
			if( in_array($estado_documento['esdid'], $arrEsdPrazo) ){
			?>
				<b>Prazo de Resposta Externa</b><br>
				<?=campo_texto('prazo', 'S',$bloqPrazo,'',5,5,'','','left','',0,' id="prazo"','',$processoprofeinep['prazo_old']); ?> dias.
			<?php 
			}
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:20%;"><b>Selecione a Unidade :</b></td>
		<td>
			<?
			$sqlCoordenacao = "SELECT coonid as codigo, coodsc as descricao 
							   FROM profeinep.coordenacao
							   ORDER BY	coodsc";
			$db->monta_combo('cooid', $sqlCoordenacao, 'N', 'Selecione...', '', '', '', 300, 'N','cooid');
			?>
		</td>
		<td rowspan="9">
		<?
		if($cooid) {
			$sqlAdvogado = "SELECT DISTINCT adv.advid as codigo, ent.entnome as descricao 
							FROM profeinep.advogados adv 
							INNER JOIN profeinep.advogadosxcoordenacao adc ON adc.advid = adv.advid
							INNER JOIN entidade.entidade ent ON ent.entid = adv.entid 
							WHERE 
								adc.coonid='".$cooid."' AND 
								ent.entstatus = 'A' AND adv.advstatus = 'A'
						   	ORDER BY ent.entnome";
			$avds = $db->carregar($sqlAdvogado);
			$avds = is_array($avds) ? $avds : array();
			$avdids = $db->carregarColuna($sqlAdvogado,0);
			if( !in_array($advid,$avdids) ){
				$sql = "SELECT
							adv.advid as codigo, 
							ent.entnome as descricao 
						FROM 
							profeinep.advogados adv 
						INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
						WHERE
							advid = (SELECT advid 
									 FROM profeinep.historicoadvogados 
									 WHERE hadid = (SELECT max(hadid) 
									 				FROM profeinep.historicoadvogados 
									 				WHERE prcid = ".$_SESSION['profeinep_var']['prcid'].") )";
				
				$advnovo = $db->pegaLinha($sql);
				array_push($avds,$advnovo);
			}
			
			$advexiste = false;
			
			if($avds[0]) {
				
				foreach($avds as $adv) {
					
					if($adv['codigo']==$advid) {
						$advexiste = true;
					}
					
					$dadosAdvogado[] = $adv;
				}
				
				if(!$advexiste) {
					$db->executar("UPDATE profeinep.processoprofeinep SET advid=NULL WHERE prcid='".$_SESSION['profeinep_var']['prcid']."'");
					$db->commit();
				}
				
			} else {
				
				$dadosAdvogado = array();
			}
			
		} else {
			$dadosAdvogado = array();
		}
		
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $docid, array( 'docid' => $docid, 'prcid' => $_SESSION['profeinep_var']['prcid'], 'cooid' => $cooid, 'usar_acaoPossivel2' => true ) );			
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:20%;"><b>Selecione advogado :</b></td>
		<td id="combo_advogados">
			<?php
				$esdid = pegaEstadoWorkFlow($docid);
				
				if( $permissoes['selecionaradvogado'] )
					$selAdv = 'S';
				else
					$selAdv = 'N';
					
				$db->monta_combo( 'advid', $dadosAdvogado, $selAdv, 'Selecione...', '', '', '', 300, 'N', 'advid', '', $advid );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de Processo :</td>
		<td><?
		$sqlTipoProcesso = "SELECT tprid as codigo, tprdsc as descricao 
							FROM profeinep.tipoprocesso 
							WHERE tprstatus = 'A'
							ORDER BY tprdsc";
		$db->monta_combo('tprid', $sqlTipoProcesso, (($permissoes['gravar'] && $boAdmin)?'S':'N'), 'Selecione...', 'exibirOcultarNumeracaoUnica', '', '', 150, 'S', 'tprid');
		?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" style="width:20%;">Número do Processo SIDOC :</td>
		<td><?
			$pcjnumerosidoc = $_REQUEST['prcnumsidoc'];
			echo campo_texto('prcnumsidoc', 'S', (($boAdmin)?'S':'N'), '', 35, 25, '', '', 'left', '', 0, 'id="prcnumsidoc"','','','');
			?>
			<input type="hidden" name="prcnumsidoc2" id="prcnumsidoc2" value="<?=$prcnumsidoc ?>"/>
		</td>		
		
	</tr>
	
	<tr id="linhaNumeracaoUnicaJudicial" style="visibility: <?= ($tprid=='2' ? 'visible' : 'collapse' ) ?>;"> 
		<td class="SubTituloDireita" style="width:20%;">Numeração Única Judicial :</td>
		<td><? echo campo_texto('prcnumeroprocjudicial', 'S', (($permissoes['gravar'] && $boAdmin)?'S':'N'), '', 30, 25, '#######-##.####.#.##.####', '', 'right', '', 0, ' style="text-align:right;" ','',$prcnumeroprocjudicial); ?></td>
	</tr>	
	
	<tr style="visibility: visible;">
		<td class="SubTituloDireita" style="width:20%;">Numeração Judicial Antiga :</td>
		<td><? echo campo_texto('prcnumeroprocjudantigo', 'S', (($boAdmin)?'S':'N'), '', 30, 25, '#####-####-###-##-##-##', '', 'right', '', 0, ' style="text-align:right;" '); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Interessado :</td>
		<td><? echo campo_texto('prcnomeinteressado', 'S', (($permissoes['gravar'] && $boAdmin)?'S':'N'), '', 50, 255, '', '', 'left', '', 0, 'prcnomeinteressado'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data de Entrada do Processo :</td>
		<td><? echo campo_data('prcdtentrada', 'S', (($permissoes['gravar'] && $boAdmin)?'S':'N'), '', 'S'); ?></td>
	</tr>	
	
	<tr>
		<td class="SubTituloDireita">Tipo de Procedência :</td>
		<td><?
		
			//$unpid = $_REQUEST['unpid'];
			$sqlTipoProcedencia = "SELECT unpid as codigo,	unpdsc as descricao 
						   FROM profeinep.unidadeprocedencia
						   WHERE undpstatus = 'A'
						   ORDER BY	unpdsc";
			$db->monta_combo('unpid', $sqlTipoProcedencia, (($permissoes['gravar'] && $boAdmin)?'S':'N'), 'Selecione...', '', '', '', 300, 'S','unpid','',$unpid );
			?></td>
	</tr>	
	
	<tr>
		<td align='right' class="SubTituloDireita">Procedência:</td>
		<td>
			<?
			echo campo_texto('prodsc', 'S', (($boAdmin)?'S':'N'), '', 50, 255, '', '', 'left', '', 0, 'id="prodsc"');
			?>			
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tema :</td>
		<td>
			<?
			echo campo_texto('tasdsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="tasdsc"');
			?>
		</td>
	</tr>
	<?php if( $tprid == 2 ): ?>
	<tr>
		<td class="SubTituloDireita">Tipo de Ação :</td>
		<td>
		<?
		$sqlTipoAcao = "SELECT tacid as codigo, tacdsc as descricao
					    FROM profeinep.tipoacao
					    ORDER BY tacdsc";
		$db->monta_combo('tacid', $sqlTipoAcao, (($permissoes['gravar'])?'S':'N'), 'Selecione...', '', '', '', 300, 'N', 'tacid');
		?>
		</td>
	</tr>
	<?php endif; ?>
	<tr>
		<td class="SubTituloDireita">Prioridade :</td>
		<td><?
		$sqlPrioridade = "SELECT tipid as codigo, tipdsc as descricao
					      FROM profeinep.tipoprioridade
					      ORDER BY	tipdsc";
		$db->monta_combo('tipid', $sqlPrioridade, (($permissoes['gravar'])?'S':'N'), 'Selecione...', '', '', '', 300, 'S', 'tipid');
		?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Relevante :</td>
		<td>
			<input type="radio" name="prcprioritario" id="prcprioritario" value="sim" <?php if($prcprioritario=='t'){ echo 'checked=\"checked\"'; } ?>/> Sim
			<input type="radio" name="prcprioritario" id="prcprioritario" value="nao" <?php if($prcprioritario=='f'){ echo 'checked=\"checked\"'; } ?>/> Não
		</td>
	</tr>		
	
	<tr>
		<td class="SubTituloDireita">Assunto :</td>
		<td><? echo campo_textarea('prcdesc', 'N', (($permissoes['gravar'] && possuiPerfil(Array(PRF_ADMINISTRADOR,PRF_APOIO_PROTOCOLO)) )?'S':'N'), '', 100, 7, 500); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="3"><b>Partes no Processo</b></td>
	</tr>
	<tr>
		<td colspan="3">
			<table id="tabela_interessado" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo Pessoal</strong></td>
			</tr>
			<?
			$sql = "SELECT * FROM profeinep.interessadosprocesso as i 
					LEFT JOIN entidade.entidade as e ON i.entid = e.entid  
					WHERE i.prcid = ".$_SESSION['profeinep_var']['prcid'];
			$interessados = $db->carregar($sql);
			if($interessados[0]) {
				foreach($interessados as $in) {
					if($permissoes['gravar']) {
						$acoes = "<input type=\"hidden\" value=\"".$in['entid']."\" id=\"entid_".$in['entid']."\" name=\"entid[]\"/>
								  <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" title=\"Editar\" onclick=\"atualizaInteressado('linha_" . $in['entid']."');\"/> 
								  <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\" onclick=\"deletarlinhainteressado(this);\"/>";
					} else {
						$acoes = "&nbsp;";
					}
					echo "<tr>";
					echo "<td><center>".$acoes."</center></td>";
					echo "<td>".$in['entnome']."</td>";
					echo "<td>";
					echo ((strlen($in['entnumcpfcnpj']) == 11)?"Física":"Jurídica");
					echo "</td>";
				}
			}
			?>
			</table>
		</td>
	</tr>
	<? if($permissoes['gravar']) {?>
	<tr>
		<td colspan="3">
			<input type="button" value="Inserir Partes" style="cursor:pointer" onclick="abreJanelaCadastroInteressados();">
		</td>
	</tr>
	<? } ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="3"><b>Expressão Chave</b></td>
	</tr>
	<tr>
		<td colspan="3">
			<table id="tabela_expressao" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Expressão Chave</strong></td>
			</tr>
			<? if($permissoes['gravar']) {?>
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><img style="cursor:pointer;" src="../imagens/gif_inclui.gif"  onclick="cadastrarExpressao();"></td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><input type="text" class="normal" id="expressao_chave" size="30" maxlength="20" value="<?=$_REQUEST["expressao_chave"]?>"></td>
			</tr>
			<? } ?>
			<?
			$sql = "SELECT * FROM profeinep.expressaochave as i 
 					WHERE i.prcid = ".$_SESSION['profeinep_var']['prcid'];
			$pchave = $db->carregar($sql);
			if($pchave[0]) {
				foreach($pchave as $in) {
					if($permissoes['gravar']) {
						$acoes = "<input type='hidden' name='expressaochave[]' value='".$in['excdsc']."'>
								  <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" title=\"Editar\" onclick='editarExpressao(this.parentNode.parentNode.parentNode.rowIndex);'/> 
								  <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\" onclick=\"deletarExpressao(this.parentNode.parentNode.parentNode.rowIndex);\"/>";
					} else {
						$acoes = "&nbsp;";
					}
					echo "<tr>";
					echo "<td><center>".$acoes."</center></td>";
					echo "<td>".$in['excdsc']."</td>";
				}
			}
			?>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="3">Processo(s) Anexado(s)</td>
	</tr>
	<tr>
		<td colspan="3">
			<table id="tabela_vincprocessos" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ações</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº do processo</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Interessado</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data Entrada</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Situação PROFE/INEP</strong></td>
			</tr>
			<?
			$excluir = "'<center><img onclick=\"alert(\'Processo físico não está na PROFE/INEP e/ou o usuário logado não está vinculado à unidade.\');\" src=\"/imagens/excluir_01.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'";
			$arEstados = Array(WF_ENCERRADO,
							   WF_AGUARDANDO_RESPOSTA_EXTERNA);
			$coordUsu = pegaCoordUsu();
			$cooid = pegaCoordProc( $_SESSION['profeinep_var']['prcid'] );
			
			if( !in_array($esdid, $arEstados)||$db->testa_superuser() ){
				$excluir = "'<center><img onclick=\"deletarlinha(this);\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'";
			}
			
			$sql = "SELECT 
						( CASE WHEN 
							( 	select 
									htddata
								from 
									profeinep.estruturaprocesso esppai
								LEFT JOIN
									workflow.documento docpai ON docpai.docid = esppai.docid 
								LEFT JOIN
									workflow.historicodocumento hsd ON docpai.docid = hsd.docid
								where
									esppai.prcid = pv.prcid
								order by
									htddata desc
								limit
									1   
							 ) > prvdtvinculacao
							 then '<center><img onclick=\"alert(\'O Processo não pode ser desvinculado devido a movimentação do Processo Anexador!\');\" src=\"/imagens/excluir_01.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'
							 else $excluir
						end)||
						(
						
						select 
									htddata
								from 
									profeinep.estruturaprocesso esppai
								LEFT JOIN
									workflow.documento docpai ON docpai.docid = esppai.docid 
								LEFT JOIN
									workflow.historicodocumento hsd ON docpai.docid = hsd.docid
								where
									esppai.prcid = pv.prcid
								order by
									htddata desc
								limit
									1  
						
						)||' <br> '||prvdtvinculacao as img,
						'<center><a style=\"cursor:pointer;\" onclick=\"verprocesso(' || prc.prcid || ');\">' || prcnumsidoc || '</a></center>' as numero,
						tprdsc,
						prcnomeinteressado,
						to_char(prcdtentrada, 'dd/mm/YYYY') as prcdtentrada,
						esd.esddsc
					FROM 
						profeinep.processosvinculados as pv
					LEFT JOIN 
						profeinep.estruturaprocesso esppai ON pv.prcid = esppai.prcid 
					LEFT JOIN 
						profeinep.processoprofeinep prc ON prc.prcid = pv.pro_prcid 
					LEFT JOIN 
						profeinep.tipoprocesso tpr ON tpr.tprid = prc.tprid 
					LEFT JOIN 
						profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid 
					LEFT JOIN 
						workflow.documento doc ON doc.docid = esp.docid 
					LEFT JOIN 
						workflow.estadodocumento esd ON esd.esdid = doc.esdid
					WHERE 
						pv.prcid = ".$_SESSION['profeinep_var']['prcid'];
			
			if( possuiPerfil(PRF_ADMINISTRADOR) ){
				$sql = "SELECT 
							'<center><img onclick=\"deletarlinha(this);\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'  as img,
							'<center><a style=\"cursor:pointer;\" onclick=\"verprocesso(' || prc.prcid || ');\">' || prcnumsidoc || '</a></center>' as numero,
							tprdsc,
							prcnomeinteressado,
							to_char(prcdtentrada, 'dd/mm/YYYY') as prcdtentrada,
							esd.esddsc
						FROM 
							profeinep.processosvinculados as pv
						LEFT JOIN 
							profeinep.estruturaprocesso esppai ON pv.prcid = esppai.prcid 
						LEFT JOIN 
							profeinep.processoprofeinep prc ON prc.prcid = pv.pro_prcid 
						LEFT JOIN 
							profeinep.tipoprocesso tpr ON tpr.tprid = prc.tprid 
						LEFT JOIN 
							profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid 
						LEFT JOIN 
							workflow.documento doc ON doc.docid = esp.docid 
						LEFT JOIN 
							workflow.estadodocumento esd ON esd.esdid = doc.esdid
						WHERE 
							pv.prcid = ".$_SESSION['profeinep_var']['prcid'];
			}
			
			$pvinculados = $db->carregar($sql);
			
			if($pvinculados[0]) {
				foreach($pvinculados as $in) {
					echo "<tr>";
					echo "<td><center>".(($permissoes['gravar'])?$in['img']:"&nbsp")."</center></td>";
					echo "<td>".$in['numero']."</td>";
					echo "<td>".$in['tprdsc']."</td>";
					echo "<td>".$in['prcnomeinteressado']."</td>";
					echo "<td>".$in['prcdtentrada']."</td>";
					echo "<td>".$in['esddsc']."</td>";
					echo "</tr>";
				}
			}
			?>
			</table>
		</td>
	</tr>
	<? 
		$estadosArquivar = Array(WF_ENCERRADO,
								 WF_AGUARDANDO_RESPOSTA_EXTERNA);
		if( $permissoes['gravar'] && !in_array($esdid,$estadosArquivar) || $db->testa_superuser() ) {
	?>
	<tr>
		<td colspan="3"><input type="button" value="Anexar processo" onclick="abreJanelaProcessosVinculados();"></td>
	</tr>
	<? } ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="3">Processo(s) Anexador(es)</td>
	</tr>
	<tr>
		<td colspan="3">  
			<?php
				$sql = "SELECT 
							'<center><a style=\"cursor:pointer;\" onclick=\"verprocesso(' || prc.prcid || ');\">' || prcnumsidoc || '</a></center>', 
							tprdsc, prcnomeinteressado, 
							to_char(prcdtentrada, 'dd/mm/YYYY') as prcdtentrada, esd.esddsc 
						FROM 
							profeinep.processosvinculados as pv 
						LEFT JOIN 
							profeinep.processoprofeinep prc ON prc.prcid = pv.prcid 
						LEFT JOIN 
							profeinep.tipoprocesso tpr ON tpr.tprid = prc.tprid 
						LEFT JOIN 
							profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid 
						LEFT JOIN 
							workflow.documento doc ON doc.docid = esp.docid 
						LEFT JOIN 
							workflow.estadodocumento esd ON esd.esdid = doc.esdid 
						WHERE pv.pro_prcid = {$_SESSION['profeinep_var']['prcid']}";
				
				$cabecalho = array("Nº do Processo", "Tipo", "Nome do Interessado", "Data de Entrada", "Situação PROFE/INEP");
				
				$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'S');
				
			?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#CCCCCC" colspan="3" align="center">
		&nbsp;
		<? if($permissoes['gravar']) {?>
		<input type="button" value="Salvar" style="cursor:pointer" id="botaosubmeter" onclick="submeteFormCadastroProcesso();">
		<? } ?>
		 <input type="button" value="Voltar" style="cursor:pointer" id="botaovoltar" onclick="window.location= 'profeinep.php?modulo=inicio&acao=C';">
		</td>
	</tr>
</table>
</form>