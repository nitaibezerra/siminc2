<?
include APPRAIZ ."includes/workflow.php";

include "_funcoes_coordenadoradjuntoies.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

verificarValidacao(array('docid' => $_SESSION['sisindigena']['coordenadoradjuntoies']['docid']));
	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);
	
		
if($_SESSION['sisindigena']['coordenadoradjuntoies']['iuscpf']!=$_SESSION['usucpf']) {
	
	$sql = "SELECT uc.uncid,
				   un.unisigla || ' - ' || un.uninome  as descricao,
				   i.iusd 
			FROM sisindigena.usuarioresponsabilidade u 
			INNER JOIN sisindigena.identificacaousuario i ON i.iuscpf = u.usucpf 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = u.pflcod 
			LEFT JOIN sisindigena.universidadecadastro uc ON uc.uncid = u.uncid
			LEFT JOIN sisindigena.universidade un ON un.uniid  = uc.uniid
			WHERE u.rpustatus='A' AND u.pflcod='".PFL_COORDENADORADJUNTOIES."' AND u.usucpf='".$_SESSION['usucpf']."' AND uc.uncid IS NOT NULL";

	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	if($usuarioresponsabilidade) {
		carregarCoordenadorAdjuntoIES(array("uncid"=>$usuarioresponsabilidade['uncid'],"iusd"=>$usuarioresponsabilidade['iusd']));
	}
	
}

if($_SESSION['sisindigena']['coordenadoradjuntoies']['descricao']) {
	
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sisindigena']['coordenadoradjuntoies']['descricao']."</td>
				 </tr>
				 </table>";
			
	
		
	monta_titulo( "Coordenador Adjunto IES", $subtitulo);

}
	
if($_SESSION['sisindigena']['coordenadoradjuntoies']['iusd']) : 
	
	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "dados";
		
	$abaativa = "/sisindigena/sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoiesexecucao&acao=A&aba=".$_REQUEST['aba'];
	
	montaAbasSisIndigena('coordenadoradjuntoiesexecucao', $abaativa);
		
	include $_REQUEST['aba'].".inc";
		
else :
?>
<table align="center" width="95%" border="0" cellpadding="3" cellspacing="1" class="listagem">
	<tr>
	  <td align="center"><img src="../imagens/AtencaoP.gif" align="absmiddle"> Não foi encontrado associação do Coordenador Adjunto IES com Universidade.</td>
	</tr>
</table>
<?
endif;
?>