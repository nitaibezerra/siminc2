<?php

eaVerificaSessao();

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
			window.location='pdeescola.php?modulo=eaprincipal/dados_escola&acao=A';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

$arPerfilMaisEducacaoVeLista = array( PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL
						   		     ,PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL);

						   		     
/*if($_REQUEST['entid']){
	$_SESSION['meentid'] = $_REQUEST['entid'];
} else {
	$_REQUEST['entid'] = $_SESSION['meentid'];	
}*/




if( !boExisteEntidade($_SESSION['entid']) ) {
	echo "<script>
			alert('A entidade informada não existe.');
			location.href = 'pdeescola.php?modulo=inicio&acao=C';
		  </script>";
	die;
}


// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;


$sql = "SELECT docid FROM pdeescola.eacescolaacessivel WHERE eacid = ".$_SESSION["eacid"];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = eaCriarDocumento( $_SESSION['entid'], $_SESSION['eacid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Escola Acessível');
				location.href = 'pdeescola.php?modulo=inicio&acao=C';
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadEscolaAcessivel = $db->pegaUm($sql);

if((integer)$vPerfilCadEscolaAcessivel > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == eaMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = eaCriarDocumento( $_SESSION['entid'], $_SESSION['eacid']  );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Escola Acessível');
						location.href = 'pdeescola.php?modulo=inicio&acao=C';
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}
	
	$esdid = eaPegarEstadoAtual( $docid );
		
	if((integer)$esdid == CADASTRAMENTO_EA || (integer)$esdid == CORRECAO_CADASTRAMENTO_EA) { 
		$somenteConsulta = false;
	}
}
$boSalvar = (!$somenteConsulta) ? true : false;
	
require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(carregaAbasEscolaAcessivel(), "/pdeescola/pdeescola.php?modulo=eaprincipal/dados_escola&acao=A");
	
$titulo = "Escola Acessível";
$subtitulo = "Cadastro - Dados Escola";
echo monta_titulo($titulo, $subtitulo);
	
echo cabecalhoEscolaAcessivel();
	
/*
 * Código do componentes de entidade
 */
$entidade = new Entidades();
if($_SESSION['entid'])
	$entidade->carregarPorEntid($_SESSION['entid']);
echo $entidade->formEntidade("pdeescola.php?modulo=eaprincipal/dados_escola&acao=A&opt=salvarRegistro",
							 array("funid" => FUNCAO_ESCOLA, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );

$perfis = arrayPerfil();
 
if( !in_array(PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL, $perfis) && !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis) )
{
	echo "<script>
			document.getElementById('btngravar').disabled = 1;
		  </script> 
	";							 
}

?>
 
<!-- 
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
 -->
 
<script type="text/javascript">

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
