<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_supervisories.php";

if($_REQUEST['requisicao']) {
	inserirLogRequisicao($_REQUEST);
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

if($_SESSION['sispacto3']['supervisories']['iuscpf']!=$_SESSION['usucpf']) {
	
	$sql = "SELECT uc.uncid,
				   un.unisigla || ' - ' || un.uninome  as descricao,
				   i.iusd 
			FROM sispacto3.usuarioresponsabilidade u 
			INNER JOIN sispacto3.identificacaousuario i ON i.iuscpf = u.usucpf 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = u.pflcod 
			LEFT JOIN sispacto3.universidadecadastro uc ON uc.uncid = u.uncid
			LEFT JOIN sispacto3.universidade un ON un.uniid  = uc.uniid
			WHERE u.rpustatus='A' AND u.pflcod='".PFL_SUPERVISORIES."' AND u.usucpf='".$_SESSION['usucpf']."' AND uc.uncid IS NOT NULL";

	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	if($usuarioresponsabilidade) {
		carregarSupervisorIES(array("uncid"=>$usuarioresponsabilidade['uncid'],"iusd"=>$usuarioresponsabilidade['iusd']));
	}
	
}

if($_SESSION['sispacto3']['supervisories']['descricao']) {
	
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sispacto3']['supervisories']['descricao']."</td>
				 </tr>
				 </table>";
		
	
	monta_titulo( "Supervisor IES", $subtitulo);
	
}
	
if($_SESSION['sispacto3']['supervisories']['iusd']) : 

	if(!$_REQUEST['aba']) $_REQUEST['aba'] = "dados";
	
	$abaativa = "/sispacto3/sispacto3.php?modulo=principal/supervisories/supervisories&acao=A&aba=".$_REQUEST['aba'];

	montaAbasSispacto('supervisories', $abaativa);
	
	if(is_file(APPRAIZ_SISPACTO.'supervisories/'.$_REQUEST['aba'].".inc")) {
		include $_REQUEST['aba'].".inc"; 
	} else {
		$al = array("location"=>"sispacto3.php?modulo=principal/supervisories/supervisories&acao=A&aba=dados");
		alertlocation($al);
	}
	
else :
?>
<table align="center" width="95%" border="0" cellpadding="3" cellspacing="1" class="listagem">
	<tr>
	  <td align="center"><img src="../imagens/AtencaoP.gif" align="absmiddle"> Não foi encontrado associação do Supervisor IES com Universidade.</td>
	</tr>
</table>
<?
endif;
?>