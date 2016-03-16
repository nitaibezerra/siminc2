<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_orientadorestudo.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);


if($_SESSION['sismedio']['orientadorestudo']['iuscpf']!=$_SESSION['usucpf']) {
	
	$sql = "SELECT uc.uncid,
				   un.unisigla || ' - ' || un.uninome  as descricao,
				   i.iusd
			FROM sismedio.usuarioresponsabilidade u 
			INNER JOIN sismedio.identificacaousuario i ON i.iuscpf = u.usucpf 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = u.pflcod 
			LEFT JOIN sismedio.universidadecadastro uc ON uc.uncid = u.uncid
			LEFT JOIN sismedio.universidade un ON un.uniid  = uc.uniid
			WHERE u.rpustatus='A' AND u.pflcod='".PFL_ORIENTADORESTUDO."' AND u.usucpf='".$_SESSION['usucpf']."' AND uc.uncid IS NOT NULL";

	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	if($usuarioresponsabilidade) {
	
		carregarOrientadorEstudo(array("uncid"=>$usuarioresponsabilidade['uncid'],"iusd"=>$usuarioresponsabilidade['iusd']));
	
	}
	
}

if($_SESSION['sismedio']['orientadorestudo']['descricao']) {
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sismedio']['orientadorestudo']['descricao']."</td>
				 </tr>
				 </table>";
		
	
	monta_titulo( "Orientador Estudo", $subtitulo);
}
	
if($_SESSION['sismedio']['orientadorestudo']['iusd']) : 

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "dados";
	
	$abaativa = "/sismedio/sismedio.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSismedio('orientadorestudo', $abaativa);
	
	if(is_file(APPRAIZ_SISMEDIO.'orientadorestudo/'.$_REQUEST['aba'].'.inc')) {
		include $_REQUEST['aba'].".inc";
	} else {
		echo '<p align=center><b>Página não encontrada : '.$_REQUEST['aba'].'.inc</b></p>';
	}
	
else :
?>
<table align="center" width="95%" border="0" cellpadding="3" cellspacing="1" class="listagem">
	<tr>
	  <td align="center"><img src="../imagens/AtencaoP.gif" align="absmiddle"> Não foi encontrado associação do Orientador de Estudo com Universidade.</td>
	</tr>
</table>
<?
endif;
?>