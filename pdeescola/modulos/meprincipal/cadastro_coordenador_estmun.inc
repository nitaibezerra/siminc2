<?php
require_once APPRAIZ . 'includes/classes/entidades.class.inc';

$sql = "select muncod, regcod from seguranca.usuario where usucpf = '".$_SESSION['usucpf']."'";
$dados = $db->pegaLinha($sql);

$muncod = $dados['muncod'];
$estuf = $dados['regcod'];

// Tipo determina se é cadastro/consulta de Coordenador
$tipo = $_GET["tipo"];
if($tipo == "E"){
	$subtitulo = "Cadastro - Coordenador Estadual";
	$funid = FUN_COORDENADOR_ME_ESTADUAL;
	$stTipo = "Coordenador";
	$andSql = " and ed.estuf = '".$estuf."' ";
} else {
	$subtitulo = "Cadastro - Coordenador Municipal";
	$funid = FUN_COORDENADOR_ME_MUNICIPAL;
	$stTipo = "Coordenador";
	$andSql = " and ed.muncod = '".$muncod."' ";
}

if ($_REQUEST['opt'] == 'salvarRegistro') {
		
	
	
	if($_REQUEST['endereco'][1]['muncod'] == $muncod || $_REQUEST['endereco'][1]['estuf'] == $estuf){
		
		//desativa funid dos antigos coordenadores
		$sql = "UPDATE 
					entidade.funcaoentidade 
				SET 
					fuestatus='I' 
				WHERE 
					entid in (
								SELECT e.entid FROM entidade.entidade e  
								INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
								LEFT JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
								LEFT JOIN entidade.endereco ed on ed.entid = e.entid
								WHERE fe.fuestatus = 'A'
								and fe.funid = ".$funid."
								$andSql
							  )
				";
		$db->carregar($sql);
		$db->commit();
		
		//insere novo coordenador
		$entidade = new Entidades();
		$entidade->carregarEntidade($_REQUEST);
		$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
		$entidade->salvar();
		
		echo "<script>
				alert('Dados gravados com sucesso');
				window.location='pdeescola.php?modulo=meprincipal/cadastro_coordenador_estmun&tipo=".$_REQUEST['tipo']."&acao=A';
			  </script>";
		exit;
		
	}
	else{
		
		echo "<script>
				alert('Erro: Estado ou Município diferente da(s) escola(s).');
				window.location='pdeescola.php?modulo=meprincipal/cadastro_coordenador_estmun&tipo=".$_REQUEST['tipo']."&acao=A';
			  </script>";
		exit;	
		
	}
}



include_once APPRAIZ . 'includes/workflow.php';

//me_verificaSessao();

# Verifica se existe entidade
/*
$boExisteEntidade = boExisteEntidade( $_SESSION['meentid'] );
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}
*/

// Flag para determinar se a tela será de cadastro ou de consulta
/*
$somenteConsulta = true;

if(!$_SESSION["memid"]){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/pdeescola/pdeescola.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}
*/

/*
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
*/
/*
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

$boSalvar = (!$somenteConsulta) ? true : false;
*/





//recupera o entid
$sql = "SELECT e.entid FROM entidade.entidade e  
			INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
			LEFT JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
			LEFT JOIN entidade.endereco ed on ed.entid = e.entid
			WHERE fe.fuestatus = 'A'
			and fe.funid = ".$funid."
			$andSql
			";
			
$entid = $db->pegaUm($sql);

ver('', '', $entid, $sql);


require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/cadastro_coordenador_estmun&tipo=$tipo&acao=A");
	
$titulo = "Mais Educação";
echo monta_titulo($titulo, $subtitulo);
//echo cabecalhoMaisEducacao();
	
/*
 * CÓDIGO DO NOVO COMPONENTE 
 */
$entidade = new Entidades();

if($entid){
	$entidade->carregarPorEntid($entid);
}
echo $entidade->formEntidade("pdeescola.php?modulo=meprincipal/cadastro_coordenador_estmun&tipo=$tipo&acao=A&opt=salvarRegistro",
							 array("funid" => $funid, "entidassociado" => $entid),
							 array("enderecos"=>array(1))
							 );
 

$perfis = arrayPerfil();
 
if( (count( $perfis ) == 1 && pdeescola_possui_perfil(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO)) || !$boSalvar ){
	echo "<script>
			//document.getElementById('btngravar').disabled = 1;
		  </script> 
	";							 
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<?
if($somenteConsulta) {
	//echo "document.getElementById('btngravar').disabled=true;";
}
?>

jQuery.noConflict();

jQuery(document).ready(function(){
	jQuery('#tr_funid').hide();
	jQuery('#tr_entobs').hide();
	jQuery('#tr_funcoescadastradas').hide();
});

$('frmEntidade').onsubmit  = function(e) {
	if (trim($F('entnumcpfcnpj')) == '') {
		alert('CPF é obrigatório.');
    	return false;
	}
	if (trim($F('entnome')) == '') {
		alert('O nome é obrigatório.');
		return false;
	}
	if (trim($F('entdatanasc')) != '') {
		if(!validaData(document.getElementById('entdatanasc'))) {
			alert("Data de nascimento é inválida.");return false;
		}
	}
	if (trim($F('endcep1')) == '') {
		alert('O CEP é obrigatório.');
		return false;
	}
	if (trim($F('estuf1')) == '' && trim($F('muncod1')) == '') {
		alert('A UF e Munícipio são obrigatórios. Digite o CEP novamente!');
		return false;
	}

	
	return true;
}

function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}

</script>