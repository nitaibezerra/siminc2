<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}

if(!$_SESSION['sismedio']['universidade']['docid']) {
	$al = array("alert"=>"Problemas na navegação, selecione uma univerisade na lista inicial.","location"=>"sismedio.php?modulo=principal/universidade/listauniversidade&acao=A");
	alertlocation($al);
}

$sql = "SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sismedio']['universidade']['docid']."'";
$esdid_unc = $db->pegaUm($sql);

/*
if($esdid_unc != ESD_VALIDADO_COORDENADOR_IES) {
	$al = array("alert"=>"Projeto da Universidade não foi validado pelo MEC","location"=>"sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=principal");
	alertlocation($al);
}
*/

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);
	
		
if($_SESSION['sismedio']['universidade']['iuscpf']!=$_SESSION['usucpf']) {

	$sql = "SELECT uc.uncid,
				   un.unisigla || ' - ' || un.uninome  as descricao 
			FROM sismedio.usuarioresponsabilidade u 
			LEFT JOIN sismedio.universidadecadastro uc ON uc.uncid = u.uncid
			LEFT JOIN sismedio.universidade un ON un.uniid  = uc.uniid
			WHERE u.rpustatus='A' AND u.pflcod='".PFL_COORDENADORIES."' AND u.usucpf='".$_SESSION['usucpf']."'";

	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	if($usuarioresponsabilidade['uncid']) {
		carregarCoordenadorIES(array("uncid"=>$usuarioresponsabilidade['uncid']));
	}

}

if($_SESSION['sismedio']['universidade']['descricao']) {
	
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sismedio']['universidade']['descricao']."</td>
				 </tr>
				 </table>";
		
	
	monta_titulo( "Universidade - Execução", $subtitulo);
}

	
if($_SESSION['sismedio']['universidade']['uncid']) : 
	
	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "dados";
	
	$abaativa = "/sismedio/sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSismedio('universidadeexecucao', $abaativa);
	
	include $_REQUEST['aba'].".inc";
		
else :

$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_EQUIPEMEC,$perfis)) :
?>
<script>
	alert('É necessário selecionar uma Universidade para acompanhar o Projeto/Execução');
	window.location='/sismedio/sismedio.php?modulo=principal/universidade/listauniversidade&acao=A';
</script>
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