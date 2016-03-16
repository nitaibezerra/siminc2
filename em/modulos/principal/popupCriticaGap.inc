<?php

if(empty($_REQUEST['papid']) || !isset($_REQUEST['papid']))
	$_REQUEST['papid'] = $_SESSION['papid'];
else
	$_SESSION['papid'] = $_REQUEST['papid'];

if( !$_REQUEST["papid"] ) {
	print "<script>
			  alert('Ocorreu um erro com a Ação/Atividade selecionada.');
			  self.close();
		  </script>";
	die;
}

$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid'] );
$esdid = pegaEstadoAtual( $docid );

if($_REQUEST["submetido"]) {
	
	$_REQUEST["crpdsccritica"] = substr($_REQUEST["crpdsccritica"], 0, 2000);
	
	// insert
	if($_REQUEST["crpid"] == "") {
		// observação
		if( $_REQUEST["crpvalidado"] == "sim" )
			$crpobs = ($_REQUEST["crpobs"] == "sim") ? "'t'" : "'f'";
		else
			$crpobs = "NULL";
		
		$sql = "INSERT INTO
						em.emicritricagap(papid,crpdsccritica,crpvalidado,crpobs)
				VALUES
						(".$_REQUEST["papid"].", '".pg_escape_string($_REQUEST["crpdsccritica"])."', '".(($_REQUEST["crpvalidado"] == "sim") ? "t" : "f")."', ".$crpobs.")";
		$db->executar($sql);
	}
	// update
	else {
		if( checkPerfil(PERFIL_CADASTRADOR) ) {
			$resposta = ($_REQUEST["crpobservacao"] && $_REQUEST["crpobservacao"] == "t") ? "crptextoobs = '".pg_escape_string($_REQUEST["crpdscresposta"])."'" : "crpdscresposta = '".pg_escape_string($_REQUEST["crpdscresposta"])."'";
			
			$sql = "UPDATE
						em.emicritricagap
					SET
						{$resposta},
						crpenviado = 't'
					WHERE
						crpid = ".$_REQUEST["crpid"]." AND
						papid = ".$_REQUEST["papid"];
		} else {
			$critica = ($_REQUEST["crpenviado"] != "t") ? "crpdsccritica = '".pg_escape_string($_REQUEST["crpdsccritica"])."'," : "";
			
			// observação
			if( $_REQUEST["crpvalidado"] == "sim" )
				$crpobs = ($_REQUEST["crpobs"] == "sim") ? "'t'" : "'f'";
			else
				$crpobs = "NULL";
			
			$sql = "UPDATE
						em.emicritricagap
					SET
						{$critica}
						crpvalidado = '".(($_REQUEST["crpvalidado"] == "sim") ? "t" : "f")."',
						crpobs = {$crpobs}
					WHERE
						crpid = ".$_REQUEST["crpid"]." AND
						papid = ".$_REQUEST["papid"];
		}
		$db->executar($sql);
	}	
	$db->commit();	
	$db->sucesso("principal/popupCriticaGap&acao=A&papid={$_REQUEST["papid"]}");
	die;
}

$dadosCritica = $db->pegaLinha("SELECT * FROM em.emicritricagap WHERE papid = ".$_REQUEST["papid"]);
$dadosCritica = ($dadosCritica) ? $dadosCritica : array();
extract($dadosCritica);

$sql = "SELECT trim(papcaoatividade) as atividade FROM emi.emgap WHERE papid = {$_REQUEST["papid"]}";
monta_titulo("Crítica da Ação/Atividade: ".$db->pegaUm($sql)."", "PRC 2012");
echo montaCabecalhoPRC($_SESSION['emiid'], $_SESSION['mcpid'], $_REQUEST['papid'], false, true);
echo "<br/>";

$abacod_tela = 57532;
$url 		 = 'em.php?modulo=principal/popupCriticaGap&acao=A';
$parametros  = null;
$arMnuid 	 = array();
$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);

$ativo = 'N';
if(checkPerfil(array(PERFIL_ANALISTA_SEDUC, PERFIL_SUPER_USUARIO)) && $esdid == WF_ESDID_EM_ANALISE_SEDUC){
	$ativo = 'S';
}

?>

<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>	    
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	    <script>
	    jQuery.noConflict();
	    function salvaCritica()
	    {	
	    	observacao = jQuery('textarea[name=crptextoobs]').val();  
	    	aprovacao = jQuery('input[name=crpvalidado]:checked').val();  	
	    	critica = jQuery('textarea[name=crpdsccritica]').val();	    	
	    	
	    	if(aprovacao == 'nao' && critica == ''){
	    		alert('O campo crítica é obrigatório!');
	    		jQuery('textarea[name=crpdsccritica]').focus();
	    		return false;
	    	}
	    	
	    	if(aprovacao == 'sim' && observacao == ''){
	    		alert('O campo observação é obrigatório!');
	    		jQuery('textarea[name=crptextoobs]').focus();
	    		return false;
	    	}
	    	
	    	document.formulario.submit();
	    }
	    jQuery(function(){
	    	jQuery('input[name=crpvalidado]').click(function(){
	    		if(this.value == 'nao'){
	    			jQuery('#tr_critica').show();
	    			jQuery('#tr_observacao').hide();
	    		}else{
	    			jQuery('#tr_critica').hide();
	    			jQuery('#tr_observacao').show();
	    		}
	    	});
	    });
	    </script>
	</head>
	<body>	
		<form id="formulario" name="formulario" method="post" action="">
		
			<input type="hidden" value="1" name="submetido" id="submetido" />
			<input type="hidden" value="<?=$crpid?>" name="crpid" id="crpid" />
			<input type="hidden" value="<?=$_REQUEST["papid"]?>" name="papid" id="papid" />
			<input type="hidden" value="<?=$crpenviado?>" name="crpenviado" id="crpenviado" />
			<input type="hidden" value="<?=$crpobs?>" name="crpobservacao" id="crpobservacao" />
			
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr id="tr_critica">
					<td class="subtitulodireita"><div id="label_critica_obs">Crítica</div></td>
					<td>
					<?php echo campo_textarea( 'crpdsccritica', '', $ativo, '', 100, 12, 2000, '', 0, '' ); ?>
					</td>
				</tr>
				<tr id="tr_observacao" style="display:none;">
					<td class="subtitulodireita"><div id="label_critica_obs">Observação</div></td>
					<td>
					<?php echo campo_textarea( 'crptextoobs', '', $ativo, '', 100, 12, 2000, '', 0, '' ); ?>
					</td>
				</tr>			
				<? if( checkPerfil(PERFIL_CADASTRADOR) || ( checkPerfil(EMI_PERFIL_ANALISTACOEM) && $crpenviado == "t" ) ) { ?>
				<tr>
					<td class="subtitulodireita">Resposta</td>
					<td>
					<?
						if($crpobs == "t") $crpdscresposta = $crptextoobs;
						
						echo campo_textarea( 'crpdscresposta', '', $ativo, '', 100, 12, 2000, '', 0, '' );
					?>
					</td>
				</tr>
				<? }?>			
				<tr>
					<td class="subtitulodireita">Aprovação</td>
					<td>				
						<input type="radio" name="crpvalidado" value="sim" <?=(($crpvalidado == "t") ? 'checked="checked"' : "")?>  onclick="habilitaObs();" <?php echo $ativo != 'S' ? 'disabled="disabled"' : '' ?>/> Sim
						<input type="radio" name="crpvalidado" value="nao" <?=((!$crpvalidado || $crpvalidado == "f") ? 'checked="checked"' : "")?>   onclick="habilitaObs();" <?php echo $ativo != 'S' ? 'disabled="disabled"' : '' ?>/> Não
					</td>
				</tr>			
				<tr>
					<td class="subtitulodireita">Observação</td>
					<td>				
						<input type="radio" name="crpobs" value="sim" <?=(($crpobs == "t") ? 'checked="checked"' : "")?> <?php echo $ativo != 'S' ? 'disabled="disabled"' : '' ?>/> Sim
						<input type="radio" name="crpobs" value="nao" <?=((!$crpobs || $crpobs == "f") ? 'checked="checked"' : "")?> <?php echo $ativo != 'S' ? 'disabled="disabled"' : '' ?>/> Não
					</td>
				</tr>			
				<tr bgcolor="#C0C0C0">
					<td colspan="2">					
						<input type="button" value="Salvar" onclick="salvaCritica();" style="cursor: pointer;" <?php echo $ativo != 'S' ? 'disabled="disabled"' : '' ?>/>					
						<input type="button" value="Voltar" onclick="document.location.href='em.php?modulo=principal/popupAcaoGap&acao=A'" style="cursor: pointer;" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>

<script>

habilitaObs();
function habilitaObs() {
	<? if( ! checkPerfil(PERFIL_CADASTRADOR) ) { ?>
	var validacao 	= 	document.getElementsByName('crpvalidado');
	var observacao 	= 	document.getElementsByName('crpobs');

	var criticaObs	=	document.getElementById('label_critica_obs');
	
	if(validacao[0].checked == true) {
		observacao[0].disabled 	=	false;
		observacao[1].disabled 	=	false;
		criticaObs.innerHTML	=	'Observação'; 
	} else {
		observacao[0].disabled	=	true;
		observacao[1].disabled	=	true;
		criticaObs.innerHTML	=	'Crítica';
	}
	<? } ?>
}

</script>