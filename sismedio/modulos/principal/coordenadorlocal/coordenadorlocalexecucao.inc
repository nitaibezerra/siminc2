<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_coordenadorlocal.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela,$url,$parametros);
	
if($_SESSION['sispacto']['coordenadorlocal']['iuscpf']!=$_SESSION['usucpf']) {
	
	$sql = "SELECT u.muncod, 
				   CASE WHEN u.muncod IS NOT NULL THEN m.estuf ELSE u.estuf END as estuf
			FROM sispacto.usuarioresponsabilidade u 
			LEFT JOIN territorios.municipio m ON m.muncod = u.muncod  
			LEFT JOIN territorios.estado e 	  ON e.estuf  = u.estuf
			WHERE u.rpustatus='A' AND u.pflcod IN('".PFL_COORDENADORLOCAL."') AND u.usucpf='".$_SESSION['usucpf']."' AND (u.muncod IS NOT NULL OR u.estuf IS NOT NULL)";
	
	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	carregarCoordenadorLocal(array("estuf"=>$usuarioresponsabilidade['estuf'],"muncod"=>$usuarioresponsabilidade['muncod']));

}

if($_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docidturma']) {
	$sql = "SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docidturma']."'";
	$esdid_pic = $db->pegaUm($sql);
}

if($esdid_pic != ESD_FECHADO_TURMA) {
	$al = array("alert"=>"Cadastramento de Turmas do Professores Alfabetizadores não esta completo","location"=>"sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A");
	alertlocation($al);
}


$subtitulo = "<table class=listagem width=100%>
			 <tr>
				<td class=SubTituloCentro>".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['descricao']."</td>
			 </tr>
			 </table>";
	

monta_titulo( "Coordenador Local", $subtitulo);

if($_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']) :

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "principal";
	
	$abaativa = "/sispacto/sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSispacto('coordenadorlocalexecucao', $abaativa);
	
	include $_REQUEST['aba'].".inc";
	
else :

$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_EQUIPEMEC,$perfis) || in_array(PFL_COORDENADORIES,$perfis)) :
?>
<script>alert('É necessário selecionar um Município/Estado para consultar o cadastramento dos Orientadores de Estudo');window.location='sispacto.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A';</script>
<?
else :
?>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem">
	<tr>
	  <td class="SubTituloCentro">Perfil não associado com Municípios / UFs</td>
	</tr>
</table>
<?
endif;

endif;
?>