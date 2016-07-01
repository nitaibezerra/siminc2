<?php
if($_GET['painel']){
	$_REQUEST['entid'] = $_GET['entid'];
	$_SESSION['memid'] = $_GET['memid'];
}

require_once APPRAIZ . "includes/classes/entidades.class.inc";
define("FUNCAO_ESCOLA", 3);

if ($_REQUEST['opt'] == 'salvarRegistro') {
	/*
	 * CÓDIGO DO NOVO COMPONENTE 
	 */
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='pdeescola.php?modulo=meprincipal/dados_escola&acao=A';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

$arPerfilMaisEducacaoVeLista = array( PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO
						   		     ,PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO);

						   		     
if($_REQUEST['entid']){
	$_SESSION['meentid'] = $_REQUEST['entid'];
} else {
	$_REQUEST['entid'] = $_SESSION['meentid'];	
}

me_verificaSessao();

require_once APPRAIZ . "includes/cabecalho.inc";

$boExisteEntidade = boExisteEntidade( $_SESSION['meentid'] );
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

// Verifica se o 'memid' existe na sessão.
if(!$_SESSION['memid'] || !$_SESSION['meentid']) {
	echo "<script type=\"text/javascript\">
			alert(\"Ocorreu um erro com a entidade selecionada. Contate o Administrador do sistema.\");
		  </script>";
	echo "<span style=font-size:15px;>Ocorreu um erro com a entidade selecionada. Contate o Administrador do sistema.</span>";
	exit;
}
/*if(!$_SESSION["memid"]){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/pdeescola/pdeescola.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}*/

$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A'";
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Mais Educação');
				history.back(-1);
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);

if((integer)$vPerfilCadMaisEscola > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == meMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']  );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Mais Educação');
						history.back(-1);
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}
	
	$esdid = mePegarEstadoAtual( $docid );
	if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) { 
		$somenteConsulta = false;
	}
}

$boSalvar = (!$somenteConsulta) ? true : false;


echo '<br/>';
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/dados_escola&acao=A");
	
$titulo = "Mais Educação";
$subtitulo = "Cadastro - Dados Escola";
echo monta_titulo($titulo, $subtitulo);
	
echo cabecalhoMaisEducacao();
	
/*
 * Código do componentes de entidade
 */
$entidade = new Entidades();
if($_SESSION['meentid'])
	$entidade->carregarPorEntid($_SESSION['meentid']);
echo $entidade->formEntidade("pdeescola.php?modulo=meprincipal/dados_escola&acao=A&opt=salvarRegistro",
							 array("funid" => FUNCAO_ESCOLA, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );

$perfis = arrayPerfil();

if( (count( $perfis ) == 1 && in_array(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO, $perfis)) || !$boSalvar){
	echo "<script>
			document.getElementById('btngravar').disabled = 1;
		  </script>";							 
}

?>
 
<!-- 
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
 -->
 
<script type="text/javascript">
<?
if( $_SESSION["meentid"] ) {

	//U - URBANO, R - RURAL
	$classificacaoEscola = $db->pegaUm("SELECT memclassificacaoescola FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
	if(!$classificacaoEscola) $classificacaoEscola = "U";
	

	$existeAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION["meentid"]." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1));
	
	
	if((integer)$existeAnoAnterior > 0) { ?>
		var textSession='\nAs escolas que não iniciaram em 2009, poderão inserir somente o PST (Programa Segundo Tempo) para 2010.'; 
	<? } else { ?>
		var textSession = '';
	<? }
}
?>

//alert( 'Observações'+textSession+'\n\nAs escolas que escolherem as atividades, banda fanfara, hip- hop, rádio escolar, cine clube, vídeo e fotografia irão receber os kits ao longo do ano de 2010.' );

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url){

	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	<? if((integer)$_SESSION["exercicio"] == 2009) { ?>
	messageObj.setSize(450,250);
	<? } else { ?>
	messageObj.setSize(650,530);
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


function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}


//chama msg 'novidades importantes'
<?if($classificacaoEscola == "U" && (integer)$existeAnoAnterior > 0 && (integer)$_SESSION["exercicio"] == 2013){?>
	displayMessage('?modulo=meprincipal/alertDadosEscola&acao=A');
<?}?>


</script>

<?php 
/*
	$existeEscolaRural = $db->pegaUm("SELECT count(memid) FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A' AND memclassificacaoescola = 'R' AND memanoreferencia = ".((integer)$_SESSION["exercicio"]));
	
	if((integer)$existeEscolaRural > 0) { 


		$texto = "<div style=\"font-size:12px\" >
					<center><b><font color=red>AVISO</font></b></center>
					<br>
					Para as escolas do campo, o Plano de Atendimento deverá ser refeito, devido a atualização das regras e atividades. 
					<br><br> Atenciosamente,
					<br> Equipe Mais Educação.
		 		  </div>";
		popupAlertaGeral($texto,'450px',"150px");
	}
*/
?>