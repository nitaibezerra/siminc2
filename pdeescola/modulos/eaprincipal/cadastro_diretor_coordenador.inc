<?php

eaVerificaSessao();

require_once APPRAIZ . 'includes/classes/entidades.class.inc';

if ($_REQUEST['opt'] == 'salvarRegistro') {
	/*
	 * REGRA DO SISTEMA, NÃO PERMITIR A MESMA PESSOA SER COORDENADOR E DIRETOR
	 */
	$tipo = array(FUN_DIRETOR_EA     => FUN_COORDENADOR_EA,
				  FUN_COORDENADOR_EA => FUN_DIRETOR_EA);
	
	if($tipo[$_REQUEST['funcoes']['funid']]) {
		if(eaExisteDiretorCoordenadorPorCpf($tipo[$_REQUEST['funcoes']['funid']]) == str_replace(array(".","-"),"",$_REQUEST['entnumcpfcnpj'])) {
			echo "<script>
				alert('CPF ja cadastrado no perfil de diretor ".$db->pegaUm("SELECT fundsc FROM entidade.funcao WHERE funid='".$tipo[$_REQUEST['funcoes']['funid']]."'")."');
				window.location='pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
			  </script>";
			exit;
		}
	} else {
		echo "<script>
				alert('Nenhuma função atribuida para este CPF');
				window.location='pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A';
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
			window.location='pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

# Verifica se existe entidade
$boExisteEntidade = boExisteEntidade( $_SESSION['entid'] );
if( !$boExisteEntidade ){
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
	$docid = eaCriarDocumento( $_SESSION['entid'], $_SESSION['eacid'] );
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
			$docid = eaCriarDocumento( $_SESSION['entid'], $_SESSION['eacid']   );
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

// Tipo determina se é cadastro/consulta de Diretor ou Coordenador
$tipo = $_GET["tipo"];

if($tipo == "diretor") {
	$subtitulo = "Cadastro - Diretor";
	$funid = FUN_DIRETOR_EA;
	$stTipo = "Diretor";
} else {
	$subtitulo = "Cadastro - Coordenador";
	$funid = FUN_COORDENADOR_EA;
	$stTipo = "Coordenador";
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(carregaAbasEscolaAcessivel(), "/pdeescola/pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=$tipo&acao=A");
	
$titulo = "Escola Acessível";
echo monta_titulo($titulo, $subtitulo);
echo cabecalhoEscolaAcessivel();
	
/*
 * CÓDIGO DO NOVO COMPONENTE 
 */
$entidade = new Entidades();
$entidade->carregarPorFuncaoEntAssociado($funid, $_SESSION['entid']);

echo $entidade->formEntidade("pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=$tipo&acao=A&opt=salvarRegistro",
							 array("funid" => $funid, "entidassociado" => $_SESSION['entid']),
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
<script type="text/javascript">
<?
if($somenteConsulta) {
	//echo "document.getElementById('btngravar').disabled=true;";
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
</script>