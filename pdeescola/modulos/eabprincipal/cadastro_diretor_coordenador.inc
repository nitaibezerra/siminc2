<?php

eabVerificaSessao();

require_once APPRAIZ . 'includes/classes/entidades.class.inc';

if ($_REQUEST['opt'] == 'salvarRegistro') {
	/*
	 * REGRA DO SISTEMA, NÃO PERMITIR A MESMA PESSOA SER COORDENADOR E DIRETOR
	 */
	$tipo = array(FUN_DIRETOR_EAB     => FUN_COORDENADOR_EAB,
				  FUN_COORDENADOR_EAB => FUN_DIRETOR_EAB);
	
	if($tipo[$_REQUEST['funcoes']['funid']]) {
		if(eabExisteDiretorCoordenadorPorCpf($tipo[$_REQUEST['funcoes']['funid']]) == str_replace(array(".","-"),"",$_REQUEST['entnumcpfcnpj'])) {
			echo "<script>
				alert('CPF ja cadastrado no perfil de diretor ".$db->pegaUm("SELECT fundsc FROM entidade.funcao WHERE funid='".$tipo[$_REQUEST['funcoes']['funid']]."'")."');
				window.location='pdeescola.php?modulo=eabprincipal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
			  </script>";
			exit;
		}
	} else {
		echo "<script>
				alert('Nenhuma função atribuida para este CPF');
				window.location='pdeescola.php?modulo=eabprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A';
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
			window.location='pdeescola.php?modulo=eabprincipal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
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

$sql = "SELECT docid FROM pdeescola.eabescolaaberta WHERE eabid = ".$_SESSION["eabid"];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = eabCriarDocumento( $_SESSION['entid'], $_SESSION['eabid'] );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Escola Aberta');
				location.href = 'pdeescola.php?modulo=inicio&acao=C';
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_ESCOLA_ABERTA.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadEscolaAberta = $db->pegaUm($sql);

if((integer)$vPerfilCadEscolaAberta > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == eabMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = eabCriarDocumento( $_SESSION['entid'], $_SESSION['eabid']   );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Escola Aberta');
						location.href = 'pdeescola.php?modulo=inicio&acao=C';
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

	$esdid = eabPegarEstadoAtual( $docid );
	if((integer)$esdid == CADASTRAMENTO_EAB || (integer)$esdid == AGUARDANDO_CORRECAO_CADASTRAMENTO_EAB) { 
		$somenteConsulta = false;
	}
}
$boSalvar = (!$somenteConsulta) ? true : false;

// Tipo determina se é cadastro/consulta de Diretor ou Coordenador
$tipo = $_GET["tipo"];

if($tipo == "diretor") {
	$subtitulo = "Cadastro - Diretor";
	$funid = FUN_DIRETOR_EAB;
	$stTipo = "Diretor";
} else {
	$subtitulo = "Cadastro - Coordenador";
	$funid = FUN_COORDENADOR_EAB;
	$stTipo = "Coordenador";
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(carregaAbasEscolaAberta(), "/pdeescola/pdeescola.php?modulo=eabprincipal/cadastro_diretor_coordenador&tipo=$tipo&acao=A");
	
$titulo = "Escola Aberta";
echo monta_titulo($titulo, $subtitulo);
echo cabecalhoEscolaAberta();
	
/*
 * CÓDIGO DO NOVO COMPONENTE 
 */
$entidade = new Entidades();
$entidade->carregarPorFuncaoEntAssociado($funid, $_SESSION['entid']);

echo $entidade->formEntidade("pdeescola.php?modulo=eabprincipal/cadastro_diretor_coordenador&tipo=$tipo&acao=A&opt=salvarRegistro",
							 array("funid" => $funid, "entidassociado" => $_SESSION['entid']),
							 array("enderecos"=>array(1))
							 );
$perfis = arrayPerfil();
 
if( !in_array(PDEESC_PERFIL_CAD_ESCOLA_ABERTA, $perfis) && !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis) )
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
</script>