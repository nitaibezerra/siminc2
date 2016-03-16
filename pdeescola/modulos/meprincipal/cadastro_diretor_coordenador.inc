<?php
require_once APPRAIZ . 'includes/classes/entidades.class.inc';

if ($_REQUEST['opt'] == 'salvarRegistro') {
	/*
	 * REGRA DO SISTEMA, NÃO PERMITIR A MESMA PESSOA SER COORDENADOR E DIRETOR
	 */
	$tipo = array(FUN_DIRETOR_ME     => FUN_COORDENADOR_ME,
				  FUN_COORDENADOR_ME => FUN_DIRETOR_ME);
	
	if($tipo[$_REQUEST['funcoes']['funid']]) {
		if(existeDiretorCoordenadorPorCpf($tipo[$_REQUEST['funcoes']['funid']]) == str_replace(array(".","-"),"",$_REQUEST['entnumcpfcnpj'])) {
			echo "<script>
				alert('CPF ja cadastrado no perfil de diretor ".$db->pegaUm("SELECT fundsc FROM entidade.funcao WHERE funid='".$tipo[$_REQUEST['funcoes']['funid']]."'")."');
				window.location='pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
			  </script>";
			exit;
		}
	} else {
		echo "<script>
				alert('Nenhuma função atribuida para este CPF');
				window.location='pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A';
			  </script>";
		exit;
	}
	/*
	 * FIM
	 * REGRA DO SISTEMA, NÃO PERMITIR A MESMA PESSOA SER COORDENADOR E DIRETOR
	 */
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

me_verificaSessao();

# Verifica se existe entidade
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

if(!$_SESSION["memid"]){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/pdeescola/pdeescola.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}

$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']   );
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
			$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']   );
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


/*
$perfis = arrayPerfil();
if(in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis) || (in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $perfis) && $_GET['tipo'] == "diretor") || (in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $perfis) && $_GET['tipo'] == "coordenador")){
	$somenteConsulta = false;
}
*/
	
$boSalvar = (!$somenteConsulta) ? true : false;

// Tipo determina se é cadastro/consulta de Diretor ou Coordenador
$tipo = $_GET["tipo"];
if($tipo == "diretor"){
	$subtitulo = "Cadastro - Diretor";
	$funid = FUN_DIRETOR_ME;
	$stTipo = "Diretor";
} else {
	$subtitulo = "Cadastro - Professor Comunitário";
	$funid = FUN_COORDENADOR_ME;
	$stTipo = "Coordenador";
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=$tipo&acao=A");
	
$titulo = "Mais Educação";
echo monta_titulo($titulo, $subtitulo);
echo cabecalhoMaisEducacao();
	
/*
 * CÓDIGO DO NOVO COMPONENTE 
 */
$entidade = new Entidades();
$entidade->carregarPorFuncaoEntAssociado($funid,$_SESSION['meentid']);
echo $entidade->formEntidade("pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=$tipo&acao=A&opt=salvarRegistro",
							 array("funid" => $funid, "entidassociado" => $_SESSION['meentid']),
							 array("enderecos"=>array(1))
							 );


 
if( ( count( $perfis ) == 1 && in_array(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO, $perfis) ) || !$boSalvar ){
	echo "<script>
			document.getElementById('btngravar').disabled = 1;
		  </script> 
	";							 
}
?>
<script type="text/javascript">
<?
if($somenteConsulta) {
	echo "document.getElementById('btngravar').disabled=true;";
}
?>
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

function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}

</script>