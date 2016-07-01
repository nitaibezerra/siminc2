<?
include "_funcoes_formadorregional.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

monta_titulo( "Lista - Formador Regional", "Lista de Formadores Regional");

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
	<td class="SubTituloDireita">IES</td>
	<td><?
	
   	$sql = "SELECT uu.uncid as codigo, u.unisigla||' - '||u.uninome as descricao FROM sismedio.universidadecadastro uu 
			INNER JOIN sismedio.universidade u ON u.uniid = uu.uniid 
			ORDER BY 2";
   	
    $db->monta_combo('uncid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'uncid','', $_REQUEST['uncid']);
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sismedio.php?modulo=principal/formadories/listaformadories&acao=A';"></td>
</tr>
</table>
</form>
<?

$f[] = "foo.status='A' AND foo.status IS NOT NULL AND foo.uncid IS NOT NULL AND foo.perfil IS NOT NULL";

if($_REQUEST['cpf']) {
	$f[] = "foo.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['cpf'])."'";
}

if($_REQUEST['nome']) {
	$f[] = "foo.iusnome ilike '%".$_REQUEST['nome']."%'";
}

if($_REQUEST['uncid']) {
	$f[] = "foo.uncid='".$_REQUEST['uncid']."'";
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$f[] = "foo.uncid = '".$_SESSION['sismedio']['universidade']['uncid']."'";
}

if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
	$f[] = "foo.uncid = '".$_SESSION['sismedio']['coordenadoradjuntoies']['uncid']."'";
}


$sql = "SELECT 
			CASE WHEN foo.status IS NULL OR foo.uncid IS NULL OR foo.perfil IS NULL THEN '' ELSE '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"window.location=\'sismedio.php?modulo=principal/formadorregional/formadorregional&acao=A&iusd='||foo.iusd||'&uncid='||foo.uncid||'&requisicao=carregarFormadorRegional&direcionar=true\';\">' END as acao,
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
		(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
		(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_FORMADORREGIONAL.") as perfil
		FROM sismedio.identificacaousuario i 
		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sismedio.universidadecadastro un ON un.uncid = i.uncid  
		INNER JOIN sismedio.universidade uu ON uu.uniid = un.uniid
		WHERE i.iusstatus='A' AND t.pflcod='".PFL_FORMADORREGIONAL."') foo 
		".(($f)?"WHERE ".implode(" AND ",$f):"")."
		ORDER BY foo.iusnome";

$cabecalho = array("&nbsp;","Situação","CPF","Nome","E-mail","Termo aceito?","Universidade");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>