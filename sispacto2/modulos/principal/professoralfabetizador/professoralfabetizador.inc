<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_professoralfabetizador.php";

if($_REQUEST['requisicao']) {
	if(function_exists($_REQUEST['requisicao'])) {
		$_REQUEST['requisicao']($_REQUEST);
		$db->close();
	}
	exit;
}

	
include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

echo "<br>";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);


if($_SESSION['sispacto2']['professoralfabetizador']['iuscpf']!=$_SESSION['usucpf']) {
	
	$sql = "SELECT uc.uncid,
				   un.unisigla || ' - ' || un.uninome  as descricao,
				   i.iusd
			FROM sispacto2.usuarioresponsabilidade u 
			INNER JOIN sispacto2.identificacaousuario i ON i.iuscpf = u.usucpf 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = u.pflcod 
			LEFT JOIN sispacto2.universidadecadastro uc ON uc.uncid = u.uncid
			LEFT JOIN sispacto2.universidade un ON un.uniid  = uc.uniid
			WHERE u.rpustatus='A' AND u.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND u.usucpf='".$_SESSION['usucpf']."' AND uc.uncid IS NOT NULL 
			ORDER BY rpudata_inc DESC LIMIT 1";

	$usuarioresponsabilidade = $db->pegaLinha($sql);
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_PROFESSORALFABETIZADOR,$perfis)) {
		unset($_SESSION['sispacto2']['professoralfabetizador']);
	}
	
	if($usuarioresponsabilidade) {
	
		carregarProfessorAlfabetizador(array("uncid"=>$usuarioresponsabilidade['uncid'],
											 "iusd"=>$usuarioresponsabilidade['iusd']));
	}
	
}

if($_SESSION['sispacto2']['professoralfabetizador']['descricao']) {
	
	$subtitulo = "<table class=listagem width=100%>
				 <tr>
					<td class=SubTituloCentro>".$_SESSION['sispacto2']['professoralfabetizador']['descricao']."</td>
				 </tr>
				 </table>";
		
	
	monta_titulo( "Professor Alfabetizador", $subtitulo);
	
}
	
if($_SESSION['sispacto2']['professoralfabetizador']['iusd']) :

	if($_SESSION['sispacto2']['professoralfabetizador']['iusdesativado']=='t') {
		
		$_REQUEST['aba'] = "principal";
		
		$abaativa = "/sispacto2/sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=".$_REQUEST['aba'];
		
		$menu[] = array("id" => "1", "descricao" => "Principal", "link" => $abaativa);
		
		echo "<br>";
		
		echo montarAbasArray($menu, $abaativa);
		
		
	} else {

		if(!$_REQUEST['aba']) $_REQUEST['aba'] = "dados";
		
		$abaativa = "/sispacto2/sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=".$_REQUEST['aba'];
	
		montaAbasSispacto('professoralfabetizador', $abaativa);
	
	}
	
	if(is_file(APPRAIZ_SISPACTO.'professoralfabetizador/'.$_REQUEST['aba'].".inc")) {
		include $_REQUEST['aba'].".inc"; 
	} else {
		$al = array("location"=>"sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dados");
		alertlocation($al);
	}
	
else :
?>
<table align="center" width="95%" border="0" cellpadding="3" cellspacing="1" class="listagem">
	<tr>
	  <td align="center"><img src="../imagens/AtencaoP.gif" align="absmiddle"> Não foi encontrado associação do Professor Alfabetizador com Universidade.</td>
	</tr>
</table>
<?
endif;
?>