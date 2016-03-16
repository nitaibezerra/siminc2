<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_orientadorestudo.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena2.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

if($_SESSION['sisindigena2']['orientadorestudo']['iuscpf']!=$_SESSION['usucpf']) {
	
	$sql = "SELECT 
				   i.iusd 
			FROM sisindigena2.usuarioresponsabilidade u 
			INNER JOIN sisindigena2.identificacaousuario i ON i.iuscpf = u.usucpf 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = u.pflcod 
			WHERE u.rpustatus='A' AND u.pflcod='".PFL_ORIENTADORESTUDO."' AND u.usucpf='".$_SESSION['usucpf']."'";

	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	if($usuarioresponsabilidade) {
		carregarOrientadorEstudo(array("iusd"=>$usuarioresponsabilidade['iusd']));
	}
	
}

if($_SESSION['sisindigena2']['orientadorestudo']['descricao']) {
	
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sisindigena2']['orientadorestudo']['descricao']."</td>
				 </tr>
				 </table>";
		
	
	monta_titulo( "Orientador de Estudo", $subtitulo);
	
}
	
if($_SESSION['sisindigena2']['orientadorestudo']['iusd']) : 

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "dados";
	
	$abaativa = "/sisindigena2/sisindigena2.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSisIndigena('orientadorestudo', $abaativa);
	
	if(is_file(APPRAIZ_SISINDIGENA.'orientadorestudo/'.$_REQUEST['aba'].".inc")) {
		include $_REQUEST['aba'].".inc"; 
	} else {
		$al = array("location"=>"sisindigena2.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=dados");
		alertlocation($al);
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