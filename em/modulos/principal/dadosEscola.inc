<!-- script type=\"text/javascript\" src=\"../includes/JQuery/jquery-1.4.2.js\"></script -->
<?php

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if(isset($_REQUEST['emiid']) && isset($_REQUEST['entid'])){
	$_SESSION['emiid'] = $_REQUEST['emiid'];
	$_SESSION['entid'] = $_REQUEST['entid'];
}else{
	$_REQUEST['emiid'] = $_SESSION['emiid'];
	$_REQUEST['entid'] = $_SESSION['entid'];
}

if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='em.php?modulo=principal/dadosEscola&acao=A&entid={$_REQUEST['entid']}&emiid={$_REQUEST['emiid']}';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

$arPerfilConsulta = array( PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO);
if(!empty($_REQUEST['entid'])){
	$boExisteEntidade = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entid = {$_REQUEST['entid']}");
}else{
	$boExisteEntidade = false;
}
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid']   );
$esdid = pegaEstadoAtual( $docid );

$ativo = 'N';
if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
	$ativo = 'S';	
}

if(!$_REQUEST['emiid'] || !$_REQUEST['entid']) {
	echo "<script type=\"text/javascript\">
			alert(\"Ocorreu um erro com a entidade selecionada. Contate o Administrador do sistema.\");
			location.href = \"em.php?modulo=inicio&acao=C\";
		  </script>";
	exit;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$abacod_tela = 57523;
$url = 'em.php?modulo=principal/dadosEscola&acao=A';
$parametros = null;
$arMnuid = array();

$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);
	
$titulo = "Ensino Médio Inovador";
$subtitulo = "Cadastro - Dados Escola";
echo monta_titulo($titulo, $subtitulo);	
echo cabecalho($_REQUEST['entid']);

/*
 * Código do componentes de entidade
 */
$entidade = new Entidades();
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);
echo $entidade->formEntidade("em.php?modulo=principal/dadosEscola&acao=A&opt=salvarRegistro&entid={$_REQUEST['entid']}&emiid={$_REQUEST['emiid']}",
							 array("funid" => ENTIDADE_FUNID_ESCOLA, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );

if( $ativo == 'N' ){
	echo "<script>
			document.getElementById('btngravar').disabled = 1;
		  </script>";							 
}
				
?>
<div style="position:absolute;right:80px;top:350px;">
	<?php wf_desenhaBarraNavegacao( $docid , array("emiid" => $_GET['emiid']) ); ?>
</div>
<script type="text/javascript">

<?
if( $_REQUEST['entid'] ) {
	$existeAnoAnterior = $db->pegaUm("SELECT count(*) FROM em.emiensinomedioinovador WHERE entid = ".$_REQUEST['entid']." AND emistatus = 'A' AND emianoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
	
	if((integer)$existeAnoAnterior > 0) { ?>
		var textSession='\nAs escolas que não iniciaram em 2009, poderão inserir somente o PST (Programa Segundo Tempo) para 2010.'; 
	<? } else { ?>
		var textSession = '';
	<? }
}
?>

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url){

	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	<? if((integer)$_SESSION["exercicio"] == 2009) { ?>
	messageObj.setSize(450,250);
	<? } else { ?>
	messageObj.setSize(450,200);
	<? } ?>
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function displayStaticMessage(messageContent,cssClass){
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(400,250);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
	
	
}

function closeMessage(){
	alert( messageObj );
	messageObj.close();	
}

document.getElementById('tr_entnuninsest').style.display = 'none';
document.getElementById('tr_entungcod').style.display = 'none';
document.getElementById('tr_tpctgid').style.display = 'none';
document.getElementById('tr_entunicod').style.display = 'none';
/*
 * DESABILITANDO O NOME DA ENTIDADE
 */
document.getElementById('entnome').readOnly = true;
document.getElementById('entnome').className = 'disabled';
document.getElementById('entnome').onfocus = "";
document.getElementById('entnome').onmouseout = "";
document.getElementById('entnome').onblur = "";
document.getElementById('entnome').onkeyup = "";
/*
 * DESABILITANDO O CÓDIGO DO INEP
 */
document.getElementById('entcodent').readOnly = true;
document.getElementById('entcodent').className = 'disabled';
document.getElementById('entcodent').onfocus = "";
document.getElementById('entcodent').onmouseout = "";
document.getElementById('entcodent').onblur = "";
document.getElementById('entcodent').onkeyup = "";
/*
 * DESABILITANDO O CÓDIGO DO INEP
 */
document.getElementById('entnumcpfcnpj').readOnly = true;
document.getElementById('entnumcpfcnpj').className = 'disabled';
document.getElementById('entnumcpfcnpj').onfocus = "";
document.getElementById('entnumcpfcnpj').onmouseout = "";
document.getElementById('entnumcpfcnpj').onblur = "";
document.getElementById('entnumcpfcnpj').onkeyup = "";

$('frmEntidade').onsubmit  = function(e) {
	if (trim($F('entnumcpfcnpj')) == '' && trim($F('entcodent')) == '' ) {
		alert('Informe o CNPJ ou Código da escola (INEP).');
    	return false;
	}
	if (trim($F('entnome')) == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	if (trim($F('endcep1')) == '') {
		alert('O CEP é obrigatório.');
		return false;
	}
	if (trim($F('endlog1')) == '') {
		alert('O Logradouro é obrigatório.');
		return false;
	}
	if (trim($F('endnum1')) == '') {
		alert('O Número é obrigatório.');
		return false;
	}
	if (trim($F('endbai1')) == '') {
		alert('O Bairro é obrigatório.');
		return false;
	}
	if (trim($F('estuf1')) == '') {
		alert('UF inválido. Digite novamente o CEP.');
		return false;
	}
	if (trim($F('mundescricao1')) == '') {
		alert('Munícipio inválido. Digite novamente o CEP.');
		return false;
	}
	
	return true;
}
</script>