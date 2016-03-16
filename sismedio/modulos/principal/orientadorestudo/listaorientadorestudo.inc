<?
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

monta_titulo( "Lista - Orientador Estudo", "Lista de Orientadores de Estudos");

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
</tr>
       <tr>
        	<td class="SubTituloDireita" width="25%">INEP</td>
        	<td>
        	<?
        	echo campo_texto('iuscodigoinep', "S", "S", "INEP", 20, 50, "", "", '', '', 0, '', '', $_REQUEST['iuscodigoinep'] );
        	?>        	
        	</td>
        </tr>
        
       <tr>
        	<td class="SubTituloDireita" width="25%">IES</td>
        	<td>
        	<?
        	$sql = "SELECT u.uncid as codigo, uu.unisigla||' - '||uu.uninome as descricao FROM sismedio.universidadecadastro u  
					INNER JOIN sismedio.universidade uu ON uu.uniid = u.uniid 
					ORDER BY uu.unisigla";
			$db->monta_combo('uncid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'uncid', false, $_REQUEST['uncid']);
        	
        	?>        	
        	</td>
        </tr>
        
<tr>
        	<td class="SubTituloDireita">UF</td>
        	<td>
        	<?
        	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
			$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '', 'N', 'estuf', false, $_REQUEST['estuf']);
        	?>        	
        	</td>
        </tr>
        <tr>
        	<td class="SubTituloDireita">Município</td>
        	<td id="td_municipio">
        	<?
        	if($_REQUEST['estuf']) {
				carregarMunicipiosPorUF(array('id'=>'muncod_sim','name'=>'muncod','estuf'=>$_REQUEST['estuf'],'valuecombo'=>$_REQUEST['muncod']));
			}
        	?>
        	</td>
        </tr>

<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sismedio.php?modulo=principal/orientadorestudo/listaorientadorestudo&acao=A';"> <input type="submit" name="gerarxls" value="Gerar XLS"></td>
</tr>
</table>
</form>
<?

if(!$_POST['gerarxls']) $f[] = "foo.status='A' AND foo.status IS NOT NULL AND foo.uncid IS NOT NULL AND foo.perfil IS NOT NULL";

if($_REQUEST['estuf']) {
	$f[] = "foo.lemuf='".$_REQUEST['estuf']."'";
}

if($_REQUEST['muncod']) {
	$f[] = "foo.muncod='".$_REQUEST['muncod']."'";
}

if($_REQUEST['iusnome']) {
	$f[] = "foo.iusnome ilike '%".$_REQUEST['iusnome']."%'";
}

if($_REQUEST['iuscpf']) {
	$f[] = "foo.iuscpf = '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
}

if($_REQUEST['iuscodigoinep']) {
	$f[] = "foo.iuscodigoinep='".$_REQUEST['iuscodigoinep']."'";
}

if($_REQUEST['uncid']) {
	$f[] = "foo.uncid='".$_REQUEST['uncid']."'";
}


$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	if($_SESSION['sismedio']['universidade']['uncid']) $f[] = "foo.uncid = '".$_SESSION['sismedio']['universidade']['uncid']."'";
	else $f[] = "1=2";
}

if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
	if($_SESSION['sismedio']['coordenadoradjuntoies']['uncid']) $f[] = "foo.uncid = '".$_SESSION['sismedio']['coordenadoradjuntoies']['uncid']."'";
	else $f[] = "1=2";
}

$sql = "SELECT 
			CASE WHEN foo.status IS NULL OR foo.uncid IS NULL OR foo.perfil IS NULL THEN '' ELSE '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"window.location=\'sismedio.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&iusd='||foo.iusd||'&uncid='||foo.uncid||'&requisicao=carregarOrientadorEstudo&direcionar=true\';\">' END as acao,
			CASE WHEN foo.status='A' AND foo.perfil IS NOT NULL THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\">' END ||' '|| 
			CASE WHEN foo.status IS NULL OR foo.perfil IS NULL THEN 'Não Cadastrado'
		   		 WHEN foo.status='A'		THEN 'Ativo' || CASE WHEN foo.uncid IS NULL THEN '( Turma não atribuída )' ELSE '' END
				 WHEN foo.status='B'		THEN 'Bloqueado' 
				 WHEN foo.status='P'		THEN 'Pendente' END as situacao,
			foo.iuscpf,
			foo.iusnome,
			foo.iuscodigoinep,
			foo.lemuf,
			foo.lemmundsc,
			foo.iusemailprincipal,
			CASE WHEN foo.iustermocompromisso=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=color:red;>Não</font>' END as termo,
			foo.formadornome
			
		FROM (
		SELECT
		i.iusd, 
		ll.lemuf,
		ll.lemmundsc,
		ll.muncod,
		i.uncid, 
		i.iuscpf,
		i.iusnome,
		i.iusemailprincipal,
		i.iustermocompromisso,
		i2.iusnome as formadornome,
		i.iuscodigoinep,
		(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
		(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil
		FROM sismedio.identificacaousuario i 
		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sismedio.listaescolasensinomedio ll ON ll.lemcodigoinep::bigint = i.iuscodigoinep
		LEFT JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
		LEFT JOIN sismedio.turmas tt ON tt.turid = ot.turid 
		LEFT JOIN sismedio.identificacaousuario i2 ON i2.iusd = tt.iusd 
		WHERE i.iusstatus='A' AND t.pflcod='".PFL_ORIENTADORESTUDO."') foo 
		".(($f)?"WHERE ".implode(" AND ",$f):"")."
		ORDER BY foo.iusnome";

$cabecalho = array("&nbsp;","Situação","CPF","Nome","INEP","UF","Município","E-mail","Termo aceito?","Formador Regional");


if($_POST['gerarxls']) {
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_professor_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_orientador_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%','');
	exit;

}


$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>