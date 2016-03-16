<?
include "_funcoes_supervisories.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

monta_titulo( "Lista - Supervisor IES", "Lista de Supervisores IES");

?>
<script>

function ativarSupervisores() {

	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "suscod");
	input.setAttribute("value", "A");
	document.getElementById("listasupervisores").appendChild(input);

	
	var input = document.createElement("input");
	input.setAttribute("type", "hidden");
	input.setAttribute("name", "requisicao");
	input.setAttribute("value", "ativarEquipe");
	document.getElementById("listasupervisores").appendChild(input);
	
	document.getElementById('listasupervisores').submit();

	
}

function marcarTodos(obj) {

	jQuery("[type=checkbox][name^='chk[']").attr('checked',obj.checked);
	
}

</script>
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
	<td class="SubTituloDireita">Universidade</td>
	<td><? 
	$sql = "SELECT uncid as codigo, unisigla||' - '||uninome as descricao 
			FROM sismedio.universidadecadastro u 
			INNER JOIN sismedio.universidade uu ON uu.uniid = u.uniid 
			ORDER BY 2";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'uncid', '', $_REQUEST['uncid']);
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sismedio.php?modulo=principal/supervisories/listasupervisories&acao=A';"></td>
</tr>
<tr>
	<td colspan="2"><input type="checkbox" name="marcartodos" onclick="marcarTodos(this);"> <b>Marcar todos</b></td>
</tr>
</table>
</form>
<?

//$f[] = "foo.status='A' AND foo.status IS NOT NULL AND foo.uncid IS NOT NULL AND foo.perfil IS NOT NULL";

if($_REQUEST['iusnome']) {
    $iusnomeTmp = removeAcentos(str_replace("-"," ",$_REQUEST['iusnome']));
	$f[] = "UPPER(public.removeacento(foo.iusnome)) ilike '%".$iusnomeTmp."%'";
}

if($_REQUEST['iuscpf']) {
	$f[] = "foo.iuscpf = '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
}

if($_REQUEST['uncid']) {
	$f[] = "foo.uncid = '".$_REQUEST['uncid']."'";
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$f[] = "foo.uncid = '".$_SESSION['sismedio']['universidade']['uncid']."'";
}

if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
	$f[] = "foo.uncid = '".$_SESSION['sismedio']['coordenadoradjuntoies']['uncid']."'";
}

$sql = "SELECT 
			'<input type=\"checkbox\" name=\"chk['||foo.pflcod||'][]\" value=\"'||foo.iuscpf||'\">' as chk,
			CASE WHEN foo.status IS NULL OR foo.uncid IS NULL OR foo.perfil IS NULL THEN '' ELSE '<img src=\"../imagens/alterar.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"window.location=\'sismedio.php?modulo=principal/supervisories/supervisories&acao=A&iusd='||foo.iusd||'&uncid='||foo.uncid||'&requisicao=carregarSupervisorIES&direcionar=true\';\">' END as acao,
			CASE WHEN foo.status='A' THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\">' END ||' '|| 
			CASE WHEN foo.status IS NULL THEN 'Não Cadastrado'
		   		 WHEN foo.status='A'		THEN 'Ativo'
				 WHEN foo.status='B'		THEN 'Bloqueado' 
				 WHEN foo.status='P'		THEN 'Pendente' END as situacao,
			foo.iuscpf,
			foo.iusnome,
			foo.iusemailprincipal,
			CASE WHEN foo.iustermocompromisso=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=color:red;>Não</font>' END as termo,
			foo.uniuf,
			foo.uninome
		FROM (
		SELECT
		i.iusd, 
		i.uncid, 
		i.iuscpf,
		i.iusnome,
		i.iusemailprincipal,
		i.iustermocompromisso,
		uu.uniuf,
		uu.uninome,
		(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
		(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_SUPERVISORIES.") as perfil,
		t.pflcod
		FROM sismedio.identificacaousuario i 
		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sismedio.universidadecadastro un ON un.uncid = i.uncid  
		INNER JOIN sismedio.universidade uu ON uu.uniid = un.uniid
		WHERE i.iusstatus='A' AND t.pflcod='".PFL_SUPERVISORIES."') foo 
		".(($f)?"WHERE ".implode(" AND ",$f):"")."
		ORDER BY foo.iusnome";

$cabecalho = array("&nbsp;","&nbsp;","Situação","CPF","Nome","E-mail","Termo aceito?","UF","Universidade");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2,"listasupervisores");
?>
<p align="center"><input type="button" name="ativar" value="Ativar Marcados" onclick="ativarSupervisores();"></p>