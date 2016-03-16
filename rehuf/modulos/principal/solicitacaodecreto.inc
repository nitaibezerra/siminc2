<?php
function filtraGND($tpaid = null) {
	global $db;
	$tpaid = !$tpaid ? $_POST['tpaid'] : $tpaid;
	switch($tpaid){
		case "1":
			$arrGND = array(3,4);
			break;
		case "2":
			$arrGND = array(3);
			break;
		case "3":
			$arrGND = array(4);
			break;
		case "4":
			$arrGND = array(4,5);
			break;
		case "5":
			$arrGND = array(3);
			break;
		case "6":
			$arrGND = array(3);
			break;
	}
	if(!$tpaid || !$arrGND){
		$sql = "SELECT gndcod as codigo, gndcod || ' - ' ||  gnddsc as descricao FROM public.gnd where gndstatus = 'A' and 1=2 order by gndcod";
		$permissao = "N";
	}else{
		$sql = "SELECT gndcod as codigo, gndcod || ' - ' ||  gnddsc as descricao FROM public.gnd where gndstatus = 'A' and gndcod in (".implode(",",$arrGND).") order by gndcod";
		$permissao = "S";	
	}
	$db->monta_combo('gndcod', $sql, $permissao, 'Selecione', '', '', '', '200', 'S', 'gndcod');
}

if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

function permissaoAlterar() {
	global $db;

	$arrPerfil = pegaPerfilGeral();
	if( $db->testa_superuser() || in_array(PRF_ADMREHUF,$arrPerfil) || in_array(PRF_EQAPOIOHU,$arrPerfil) || in_array(PRF_GESTORHU,$arrPerfil) ){
		return true;
	}else{
		return false;
	}
	
}

function inserirSolitacaoDecreto($dados) {
	global $db;
	
	$slfid = pegarMomento();
	
	if(!$slfid) {
		die("<script>
				alert('Acesso negado');
				window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A';
			 </script>");
	}
		
	$sql = "INSERT INTO academico.solicitacaodecreto(
            entid, tpaid, usucpf, sldobjeto, sldjustificativa, sldvlrestimado, 
            sldstatus, slddtinclusao,gndcod,slfid)
    		VALUES ('".$_SESSION['rehuf_var']['entid']."', '".$dados['tpaid']."', '".$_SESSION['usucpf']."', 
    				'".$dados['sldobjeto']."', '".$dados['sldjustificativa']."', '".str_replace(array(".",","),array("","."),$dados['sldvlrestimado'])."', 'A', NOW(),'".$dados['gndcod']."','".$slfid."');";
    				
    $db->executar($sql);
    $db->commit();
    
    echo "<script>alert('Inserido com sucesso');window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A&entid={$_SESSION['rehuf_var']['entid']}';</script>";
	
}

function atualizarSolitacaoDecreto($dados) {
	global $db;
	
	$sql = "SELECT * FROM academico.solicitacaodecreto WHERE sldid='".$dados['sldid']."'";
	$solicitacaodecreto = $db->pegaLinha($sql);
	$slfid = $solicitacaodecreto['slfid'];
	$slfid_ = pegarMomento();
	
	if($slfid != $slfid_) {
		die("<script>
				alert('Acesso negado');
				window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A';
			 </script>");
	}
	
	$sql = "UPDATE academico.solicitacaodecreto
		   	SET tpaid='".$dados['tpaid']."', gndcod='".$dados['gndcod']."', sldobjeto='".$dados['sldobjeto']."', sldjustificativa='".$dados['sldjustificativa']."', 
		       sldvlrestimado='".str_replace(array(".",","),array("","."),$dados['sldvlrestimado'])."'
		 	WHERE sldid='".$dados['sldid']."';";
	
    $db->executar($sql);
    $db->commit();
    
    echo "<script>alert('Atualizado com sucesso');window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A&entid={$_SESSION['rehuf_var']['entid']}';</script>";
	
}

function excluirSolicitacaoDecreto($dados) {
	global $db;
	
	$sql = "SELECT * FROM academico.solicitacaodecreto WHERE sldid='".$dados['sldid']."'";
	$solicitacaodecreto = $db->pegaLinha($sql);
	$slfid = $solicitacaodecreto['slfid'];
	$slfid_ = pegarMomento();
	
	if($slfid != $slfid_) {
		die("<script>
				alert('Acesso negado');
				window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A';
			 </script>");
	}
	
	$sql = "UPDATE academico.solicitacaodecreto
		   	SET sldstatus='I' 
		   	WHERE sldid='".$dados['sldid']."'";
	
    $db->executar($sql);
    $db->commit();
    
    echo "<script>alert('Removido com sucesso');window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A&entid={$_SESSION['rehuf_var']['entid']}';</script>";
	
}
$_SESSION['rehuf_var']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['rehuf_var']['entid'];
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

function pegarMomento() {
	global $db;
	$sql = "SELECT slfid FROM academico.sldfase WHERE slfdatainicio<='".date("Y-m-d")."' AND slfdatafim>='".date("Y-m-d")."'";
	$slfid = $db->pegaUm($sql);
	return $slfid;
}

$permissoes = verificaPerfilRehuf();
if( $permissoes['verhospitais'] && is_array($permissoes['verhospitais']) ) {
	if(!in_array($_SESSION['rehuf_var']['entid'],$permissoes['verhospitais'])){
		header("Location: rehuf.php?modulo=inicio&acao=C");
	}
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

echo montarAbasArray(carregardadosmenurehuf(), $_SERVER['REQUEST_URI']);

monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);

$permissaoAlterar = permissaoAlterar();

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function validarFormulario() {
 	if(document.getElementById('tpaid').value=='') {
 		alert('Tipo de solicitação obrigatório');
 		return false;
 	}
 	if(document.getElementById('sldobjeto').value=='') {
 		alert('Objeto obrigatório');
 		return false;
 	}
 	if(document.getElementById('sldjustificativa').value=='') {
 		alert('Justificativa obrigatório');
 		return false;
 	}
 	if(document.getElementById('gndcod').value=='') {
 		alert('GND obrigatório');
 		return false;
 	}
 	if(document.getElementById('sldvlrestimado').value=='') {
 		alert('Valor estimado obrigatório');
 		return false;
 	}
 	
 	document.getElementById('sldvlrestimado').onkeyup();
	document.getElementById('formulario').submit();
}

function excluir(sldid) {
	var conf = confirm('Deseja realmente excluir?');
	if(conf) {
		document.getElementById('requisicao').value="excluirSolicitacaoDecreto";
		document.getElementById('sldid').value=sldid;
		document.getElementById('formulario').submit();
	}

}

function filtraGND(tpaid)
{
	$.ajax({
	   type: "POST",
	   url: window.location,
	   data: "requisicaoAjax=filtraGND&tpaid=" + tpaid,
	   success: function(msg){
	   		$('#td_gnd').html( msg );
	   	}
	 });
}

</script>
<form id="formulario" name="formulario" method="post" action="rehuf.php?modulo=principal/solicitacaodecreto&acao=A">
<?
$req = "inserirSolitacaoDecreto";
if($_REQUEST['sldid']) {
	$sql = "SELECT * FROM academico.solicitacaodecreto WHERE sldid='".$_REQUEST['sldid']."'";
	$solicitacaodecreto = $db->pegaLinha($sql);
	$slfid = $solicitacaodecreto['slfid'];
	$slfid_ = pegarMomento();
	
	if($slfid != $slfid_) {
		die("<script>
				alert('Acesso negado');
				window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A';
			 </script>");
	}
	
	
	$sldid = $solicitacaodecreto['sldid'];
	$tpaid = $solicitacaodecreto['tpaid'];
	$gndcod = $solicitacaodecreto['gndcod'];
	$sldobjeto = $solicitacaodecreto['sldobjeto'];
	$sldjustificativa = $solicitacaodecreto['sldjustificativa'];
	$sldvlrestimado = number_format($solicitacaodecreto['sldvlrestimado'],2,",",".");
	$req = "atualizarSolitacaoDecreto";
}
?>
<?php if($permissaoAlterar): ?>
	<input type="hidden" name="requisicao" id="requisicao" value="<? echo $req; ?>">
	<input type="hidden" name="sldid" id="sldid" value="<? echo $sldid; ?>">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
	<td class="SubTituloDireita">Tipo:</td>
	<td>
	<?
	$sql = "SELECT tpaid as codigo, tpadsc as descricao FROM academico.tiposolicitacao";
	$db->monta_combo('tpaid', $sql, 'S', 'Selecione', 'filtraGND', '', '', '200', 'S', 'tpaid');
	?>
	</td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Objeto:</td>
	<td><?
	echo campo_texto('sldobjeto', "S", "S", "Objeto", 67, 150, "", "", '', '', 0, 'id="sldobjeto"' );
	?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Justificativa:</td>
	<td><?
	echo campo_textarea('sldjustificativa', 'S', 'S', '', '70', '4', '400');
	?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">GND:</td>
	<td id="td_gnd" >
	<?
	if($tpaid){
		filtraGND($tpaid);
	}else{
		$sql = "SELECT gndcod as codigo, gndcod || ' - ' ||  gnddsc as descricao FROM public.gnd where gndstatus = 'A' and gndcod in (3,4) order by gndcod";
		$db->monta_combo('gndcod', $sql, 'N', 'Selecione', '', '', '', '200', 'S', 'gndcod');	
	}
	?>
	</td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Valor:</td>
	<td><?
	echo campo_texto('sldvlrestimado', "S", "S", "Valor", 30, 14, "###.###.###,##", "", '', '', 0, 'id="sldvlrestimado"' );
	?>
	</td>
	</tr>
	<tr>
	<td align="center" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="validarFormulario();"> <input type="button" name="novo" value="Novo" onclick="window.location='rehuf.php?modulo=principal/solicitacaodecreto&acao=A';"></td>
	</tr>
	</table>
	</form>
<?php endif; ?>
<?

$slfid = pegarMomento();

if(!$slfid) {
	$permissaoAlterar = false;
}

$sql = "SELECT 
			".($permissaoAlterar ? " CASE WHEN s.slfid=2 THEN '<span style=\"white-space:nowrap;\" ><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=\'rehuf.php?modulo=principal/solicitacaodecreto&acao=A&sldid='||s.sldid||'\';\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluir(\''||s.sldid||'\');\"></span>' ELSE '&nbsp;' END as acao," : "")."
			s.sldid,
			sl.slfdsc,
			e.entnome, tpadsc, sldobjeto, sldjustificativa, g.gndcod || ' - ' || COALESCE(gnddsc,'N/A') AS gnddsc, sldvlrestimado, to_char(slddtinclusao, 'dd/mm/YYYY HH24:MI') as slddtinclusao 
		FROM academico.solicitacaodecreto s 
		LEFT JOIN entidade.entidade e ON e.entid = s.entid 
		LEFT JOIN academico.tiposolicitacao t ON t.tpaid = s.tpaid 
		LEFT JOIN public.gnd g ON g.gndcod = s.gndcod 
		INNER JOIN academico.sldfase sl ON sl.slfid = s.slfid 
		WHERE sldstatus='A' AND s.entid='".$_SESSION['rehuf_var']['entid']."' ORDER BY sldid";

if($permissaoAlterar){
	$cabecalho = array("&nbsp","Cód.","Momento","Nome da instituição","Tipo Solicitação","Objeto","Justificativa","GND","Valor Estimado","Data Inclusão");
}else{
	$cabecalho = array("Cód.","Momento","Nome da instituição","Tipo Solicitação","Objeto","Justificativa","GND","Valor Estimado","Data Inclusão");	
}
$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);
?>