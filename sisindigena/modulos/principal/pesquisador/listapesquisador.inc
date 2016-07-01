<?
include "_funcoes_coordenadorlocal.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena.js"></script>';

echo "<br>";

monta_titulo( "Lista - Formador Pesquisador", "Lista de Formadores Pesquisadores");

?>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">CPF</td>
	<td><? echo campo_texto('iuscpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, '', '', $_REQUEST['iuscpf'] ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome</td>
	<td><? echo campo_texto('iusnome', "S", "S", "Nome", 67, 150, "", "", '', '', 0, '', '', $_REQUEST['iusnome'] ); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/pesquisador/listapesquisador&acao=A';"></td>
</tr>
</table>
</form>
<?

if($_REQUEST['iusnome']) {
	$f[] = "foo.iusnome ilike '%".$_REQUEST['iusnome']."%'";
}

if($_REQUEST['iuscpf']) {
	$f[] = "foo.iuscpf = '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$f[] = "foo.uncid = '".$_SESSION['sisindigena']['universidade']['uncid']."'";
}

$sql = "SELECT 
			'<img src=\"../imagens/alterar.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"window.location=\'sisindigena.php?modulo=principal/pesquisador/pesquisador&acao=A&iusd='||foo.iusd||'&requisicao=carregarPesquisador&direcionar=true\';\">' as acao,
			CASE WHEN foo.status='A' THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\">' END ||' '|| 
			CASE WHEN foo.status IS NULL THEN 'Não Cadastrado'
		   		 WHEN foo.status='A'		THEN 'Ativo'
				 WHEN foo.status='B'		THEN 'Bloqueado' 
				 WHEN foo.status='P'		THEN 'Pendente' END as situacao,
			foo.iuscpf,
			foo.iusnome,
			foo.iusemailprincipal,
			CASE WHEN foo.iustermocompromisso=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=color:red;>Não</font>' END as termo,
			foo.uninome
		FROM (
		SELECT
		i.iusd, 
		i.uncid, 
		i.iuscpf,
		i.iusnome,
		i.iusemailprincipal,
		i.iustermocompromisso,
		un.picid,
		(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
		(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_PESQUISADOR.") as perfil,
		uu.uninome
		FROM sisindigena.identificacaousuario i 
		INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
		LEFT JOIN sisindigena.nucleouniversidade un ON un.picid = i.picid 
		LEFT JOIN sisindigena.universidade uu ON uu.uniid = un.uniid 
		WHERE i.iusstatus='A' AND t.pflcod='".PFL_PESQUISADOR."') foo 
		".(($f)?"WHERE ".implode(" AND ",$f):"")."
		ORDER BY foo.iusnome";

$cabecalho = array("&nbsp;","Situação","CPF","Nome","E-mail","Termo aceito?","Núcleo");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>