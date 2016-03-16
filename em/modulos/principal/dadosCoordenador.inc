<?php
require_once APPRAIZ . 'includes/classes/entidades.class.inc';

if ($_REQUEST['opt'] == 'salvarRegistro') {
	/*
	 * REGRA DO SISTEMA, NÃO PERMITIR A MESMA PESSOA SER COORDENADOR E DIRETOR
	 */
	$tipo = array(ENTIDADE_FUNID_DIRETOR  	 => ENTIDADE_FUNID_COORDENADOR,
				  ENTIDADE_FUNID_COORDENADOR => ENTIDADE_FUNID_DIRETOR);
	
	if($tipo[$_REQUEST['funcoes']['funid']]) {
		if(existeAssociacaoEntidde($_SESSION['entid'], $tipo[$_REQUEST['funcoes']['funid']]) == str_replace(array(".","-"),"",$_REQUEST['entnumcpfcnpj'])) {
			echo "<script>
				alert('CPF ja cadastrado no perfil de ".$db->pegaUm("SELECT fundsc FROM entidade.funcao WHERE funid='".$tipo[$_REQUEST['funcoes']['funid']]."'")."');
				window.location='em.php?modulo=principal/dadosCoordenador&acao=A';
			  </script>";
			exit;
		}
	} else {
		echo "<script>
				alert('Nenhuma função atribuida para este CPF');
				window.location='em.php?modulo=principal/dadosCoordenador&acao=A';
			  </script>";
		exit;
	}
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='em.php?modulo=principal/dadosCoordenador&acao=A';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid']   );
$esdid = pegaEstadoAtual( $docid );

$ativo = 'N';
if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
	$ativo = 'S';	
}

if(!$_SESSION['emiid']){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/em/em.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$abacod_tela = 57523;
$url = 'em.php?modulo=principal/dadosCoordenador&acao=A';
$parametros = null;
$arMnuid = array();

$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);
	
$titulo = "Ensino Médio Inovador";
$subtitulo = "Cadastro - Professor Articulador";
echo monta_titulo($titulo, $subtitulo);
echo cabecalho($_SESSION['entid']);

$entidade = new Entidades();
$entidade->carregarPorFuncaoEntAssociado(ENTIDADE_FUNID_COORDENADOR,$_SESSION['entid']);
echo $entidade->formEntidade("em.php?modulo=principal/dadosCoordenador&acao=A&opt=salvarRegistro",
							 array("funid" => ENTIDADE_FUNID_COORDENADOR, "entidassociado" => $_SESSION['entid']),
							 array("enderecos"=>array(1))
							 );	

if( $ativo == 'N' ){
	echo "<script>			
			document.getElementById('btngravar').disabled = 1;
		  </script>";							 
}							 
?>
<div style="position:absolute;right:80px;top:350px;">
	<?php wf_desenhaBarraNavegacao( $docid , array("emiid" => $_SESSION['emiid']) ); ?>
</div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

$('frmEntidade').onsubmit  = function(e) {
	if (trim($F('entnumcpfcnpj')) == '') {
		alert('CPF é obrigatório.');
    	return false;
	}
	if (trim($F('entnome')) == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	if (trim($F('entdatanasc')) != '') {
		if(!validaData(document.getElementById('entdatanasc'))) {
			alert("Data de nascimento é inválida.");return false;
		}
	}
	return true;
}

jQuery(function(){

	var titulo = jQuery('#tr_titulo').children()	
	var texto = titulo.html().replace('COORDENADOR','PROFESSOR').replace('MUNICIPAL','ARTICULADOR');
	
	titulo.html(texto);
	
});


function popupMapa(entid){
	window.open('em.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}
</script>