<?
include APPRAIZ ."includes/workflow.php";
include_once "_funcoes_universidade.php";
include_once "_funcoes_centralacompanhamento.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}

if(!$_SESSION['sispacto2']['universidade']['docid']) {
	$al = array("alert"=>"Problemas na navegação, selecione uma univerisade na lista inicial.","location"=>"sispacto2.php?modulo=principal/universidade/listauniversidade&acao=A");
	alertlocation($al);
}

$sql = "SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sispacto2']['universidade']['docid']."'";
$esdid_unc = $db->pegaUm($sql);

if($esdid_unc != ESD_VALIDADO_COORDENADOR_IES) {
	$al = array("alert"=>"Projeto da Universidade não foi validado pelo MEC","location"=>"sispacto2.php?modulo=principal/universidade/universidade&acao=A&aba=principal");
	alertlocation($al);
}

$sql = "SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sispacto2']['universidade']['docidformacaoinicial']."'";
$esdid_for = $db->pegaUm($sql);

if($esdid_for != ESD_FECHADO_FORMACAOINICIAL) {
	$al = array("alert"=>"Formação inicial não foi finalizada","location"=>"sispacto2.php?modulo=principal/universidade/universidade&acao=A&aba=formacaoinicial");
	alertlocation($al);
}

$sql = "SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sispacto2']['universidade']['docidturma']."'";
$esdid_tur = $db->pegaUm($sql);

if($esdid_tur != ESD_FECHADO_TURMA) {
	$al = array("alert"=>"Fechamento da Turma (Outros) não foi finalizada","location"=>"sispacto2.php?modulo=principal/universidade/universidade&acao=A&aba=turmasoutros");
	alertlocation($al);
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);
	
		
if($_SESSION['sispacto2']['universidade']['iuscpf']!=$_SESSION['usucpf']) {

	$sql = "SELECT uc.uncid,
				   un.unisigla || ' - ' || un.uninome  as descricao 
			FROM sispacto2.usuarioresponsabilidade u 
			LEFT JOIN sispacto2.universidadecadastro uc ON uc.uncid = u.uncid
			LEFT JOIN sispacto2.universidade un ON un.uniid  = uc.uniid
			WHERE u.rpustatus='A' AND u.pflcod='".PFL_COORDENADORIES."' AND u.usucpf='".$_SESSION['usucpf']."' ORDER BY rpudata_inc DESC LIMIT 1";

	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	if($usuarioresponsabilidade['uncid']) {
		carregarCoordenadorIES(array("uncid"=>$usuarioresponsabilidade['uncid']));
	}
	
}

if($_SESSION['sispacto2']['universidade']['descricao']) {
	
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sispacto2']['universidade']['descricao']."</td>
				 </tr>
				 </table>";
		
	
	monta_titulo( "Universidade - Execução", $subtitulo);
}
	
if($_SESSION['sispacto2']['universidade']['uncid']) : 

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "principal";
	
	$abaativa = "/sispacto2/sispacto2.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSispacto('universidadeexecucao', $abaativa);
	
	include $_REQUEST['aba'].".inc";
	
else :
	$perfis = pegaPerfilGeral();
	
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_EQUIPEMEC,$perfis)) :
	?>
	<script>alert('É necessário selecionar uma Universidade para acompanhar o Projeto/Execução');window.location='sispacto2.php?modulo=principal/universidade/listauniversidade&acao=A';</script>
	<?
	else :
	?>
	<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem">
		<tr>
		  <td class="SubTituloCentro">Perfil não associado com Universidades</td>
		</tr>
	</table>
	<?
	endif;

endif;
?>