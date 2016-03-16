<?
include "_funcoes_formadoriesp.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

echo "<br>";

monta_titulo( "Lista - Formador IES", "Lista de Formadores IES");

?>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">CPF</td>
	<td><?
	echo campo_texto('cpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, '' );
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome</td>
	<td><?
	echo campo_texto('nome', "S", "S", "Nome", 50, 100, "", "", '', '', 0, '' );
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/formadoriesp/listaformadoriesp&acao=A';"></td>
</tr>
</table>
</form>
<?

$f[] = "foo.status='A' AND foo.status IS NOT NULL AND foo.uncid IS NOT NULL AND foo.perfil IS NOT NULL";

if($_REQUEST['cpf']) {
	$f[] = "foo.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['cpf'])."'";
}

if($_REQUEST['nome']) {
    $nomeTmp = removeAcentos(str_replace("-"," ",$_REQUEST['nome']));
	$f[] = "UPPER(public.removeacento(foo.iusnome)) ilike '%".$nomeTmp."%'";
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	if($_SESSION['sispacto2']['universidade']['uncid']) {
		$f[] = "foo.uncid = '".$_SESSION['sispacto2']['universidade']['uncid']."'";
	} else {
		$f[] = "1=2";
	}
}

if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
	if($_SESSION['sispacto2']['coordenadoradjuntoies']['uncid']) {
		$f[] = "foo.uncid = '".$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid']."'";
	} else {
		$f[] = "1=2";
	}
}

if(in_array(PFL_SUPERVISORIES,$perfis)) {
	if($_SESSION['sispacto2']['supervisories']['uncid']) {
		$f[] = "foo.uncid = '".$_SESSION['sispacto2']['supervisories']['uncid']."'";
	} else {
		$f[] = "1=2";
	}
}


$sql = "SELECT 
			CASE WHEN foo.status IS NULL OR foo.uncid IS NULL OR foo.perfil IS NULL THEN '' ELSE '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"window.location=\'sispacto2.php?modulo=principal/formadoriesp/formadoriesp&acao=A&iusd='||foo.iusd||'&uncid='||foo.uncid||'&requisicao=carregarFormadorIES&direcionar=true\';\">' END as acao,
			CASE WHEN foo.status='A' AND foo.perfil IS NOT NULL THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\">' END ||' '|| 
			CASE WHEN foo.status IS NULL OR foo.perfil IS NULL THEN 'Não Cadastrado'
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
		uu.uninome,
		(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
		(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_FORMADORIESP.") as perfil
		FROM sispacto2.identificacaousuario i 
		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sispacto2.universidadecadastro un ON un.uncid = i.uncid  
		INNER JOIN sispacto2.universidade uu ON uu.uniid = un.uniid
		WHERE i.iusstatus='A' AND t.pflcod='".PFL_FORMADORIESP."') foo 
		".(($f)?"WHERE ".implode(" AND ",$f):"")."
		ORDER BY foo.iusnome";

$cabecalho = array("&nbsp;","Situação","CPF","Nome","E-mail","Termo aceito?","Universidade");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>