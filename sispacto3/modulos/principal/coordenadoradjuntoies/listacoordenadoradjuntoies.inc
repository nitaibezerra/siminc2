<?
include "_funcoes_coordenadoradjuntoies.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>';

echo "<br>";

monta_titulo( "Lista - Coordenador Adjunto IES", "Lista de Coordenadores Adjunto IES");

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
        	<td class="SubTituloDireita" width="25%">IES</td>
        	<td>
        	<?
        	$sql = "SELECT u.uncid as codigo, uu.unisigla||' - '||uu.uninome as descricao FROM sispacto3.universidadecadastro u  
					INNER JOIN sispacto3.universidade uu ON uu.uniid = u.uniid 
					ORDER BY uu.unisigla";
			$db->monta_combo('uncid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'uncid', false, $_REQUEST['uncid']);
        	
        	?>        	
        	</td>
        </tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/coordenadoradjuntoies/listacoordenadoradjuntoies&acao=A';"></td>
</tr>
</table>
</form>
<?

$f[] = "foo.status='A' AND foo.status IS NOT NULL AND foo.uncid IS NOT NULL AND foo.perfil IS NOT NULL";

if($_REQUEST['iusnome']) {
    $iusnomeTmp = removeAcentos(str_replace("-"," ",$_REQUEST['iusnome']));
	$f[] = " UPPER(public.removeacento(foo.iusnome)) ilike '%".$iusnomeTmp."%'";
}

if($_REQUEST['iuscpf']) {
	$f[] = "foo.iuscpf = '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
}

if($_REQUEST['uncid']) {
	$f[] = "foo.uncid = '".$_REQUEST['uncid']."'";
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto3']['universidade']['uncid']) {
	$f[] = "foo.uncid = '".$_SESSION['sispacto3']['universidade']['uncid']."'";
}

$sql = "SELECT 
			CASE WHEN foo.status IS NULL OR foo.uncid IS NULL THEN '' ELSE '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"window.location=\'sispacto3.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&iusd='||foo.iusd||'&uncid='||foo.uncid||'&requisicao=carregarCoordenadorAdjuntoIES&direcionar=true\';\">' END as acao,
			CASE WHEN foo.status='A' THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\">' END ||' '|| 
			CASE WHEN foo.status IS NULL THEN 'Não Cadastrado'
		   		 WHEN foo.status='A'		THEN 'Ativo'
				 WHEN foo.status='B'		THEN 'Bloqueado' 
				 WHEN foo.status='P'		THEN 'Pendente' END as situacao,
			foo.iuscpf,
			foo.iusnome,
			foo.iusemailprincipal,
			CASE WHEN foo.iustermocompromisso=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=color:red;>Não</font>' END as termo
		FROM (
		SELECT
		i.iusd, 
		i.uncid, 
		i.iuscpf,
		i.iusnome,
		i.iusemailprincipal,
		i.iustermocompromisso,
		(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
		(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_COORDENADORADJUNTOIES.") as perfil
		FROM sispacto3.identificacaousuario i 
		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
		WHERE i.iusstatus='A' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."') foo 
		".(($f)?"WHERE ".implode(" AND ",$f):"")."
		ORDER BY foo.iusnome";

$cabecalho = array("&nbsp;","Situação","CPF","Nome","E-mail","Termo aceito?");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>