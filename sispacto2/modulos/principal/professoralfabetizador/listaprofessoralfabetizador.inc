<?
include "_funcoes_professoralfabetizador.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto2.js"></script>';

echo "<br>";

monta_titulo( "Lista - Professor Alfabetizador", "Lista de Professor Alfabetizador");

?>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" width="25%">CPF</td>
	<td><?
	echo campo_texto('cpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, '', '', $_REQUEST['cpf'] );
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome</td>
	<td><?
	echo campo_texto('nome', "S", "S", "Nome", 50, 100, "", "", '', '', 0, '', '', $_REQUEST['nome'] );
	?></td>
</tr>
       <tr>
        	<td class="SubTituloDireita" width="25%">Esfera</td>
        	<td>
        	<?
        	$esfera = array(0 => array('codigo' => 'E','descricao' => 'Estadual'),1 => array('codigo' => 'M','descricao' => 'Municipal'));
			$db->monta_combo('esfera', $esfera, 'S', 'TODOS', '', '', '', '', 'N', 'esfera', false, $_REQUEST['esfera']);
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
        		carregarMunicipiosPorUF(array("id"=>"muncod_sim","name"=>"muncod","estuf"=>$_REQUEST['estuf'],"valuecombo"=>$_REQUEST['muncod']));	
        	}
        	
        	?>
        	</td>
        </tr>

<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" name="filtrar" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/professoralfabetizador/listaprofessoralfabetizador&acao=A';"></td>
</tr>
</table>
</form>
<?

if($_REQUEST['filtrar']) {

	$f[] = "foo.status='A' AND foo.status IS NOT NULL AND foo.uncid IS NOT NULL AND foo.perfil IS NOT NULL";
	
	if($_REQUEST['esfera']=='E') {
		$f2[] = "p.estuf IS NOT NULL";
	} elseif($_REQUEST['esfera']=='M') {
		$f2[] = "p.muncod IS NOT NULL";
	}
	
	if($_REQUEST['muncod']) {
		$f2[] = "(p.muncod='".$_REQUEST['muncod']."' OR i.muncodatuacao='".$_REQUEST['muncod']."')";
	}
	
	if($_REQUEST['cpf']) {
		$f[] = "foo.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['cpf'])."'";
	}
	
	if($_REQUEST['nome']) {
        $nomeTmp = removeAcentos(str_replace("-"," ",$_REQUEST['nome']));
		$f[] = "UPPER(public.removeacento(foo.iusnome)) ilike '%".$nomeTmp."%'";
	}
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADORIES,$perfis)) {
		if($_SESSION['sispacto2']['universidade']['uncid']) $f[] = "foo.uncid = '".$_SESSION['sispacto2']['universidade']['uncid']."'";
		else $f[] = "1=2";
	}
	
	if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
		if($_SESSION['sispacto2']['coordenadoradjuntoies']['uncid']) $f[] = "foo.uncid = '".$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid']."'";
		else $f[] = "1=2";
	}
	
	if(in_array(PFL_SUPERVISORIES,$perfis)) {
		if($_SESSION['sispacto2']['supervisories']['uncid']) $f[] = "foo.uncid = '".$_SESSION['sispacto2']['supervisories']['uncid']."'";
		else $f[] = "1=2";
	}
	
	if(in_array(PFL_COORDENADORLOCAL,$perfis)) {
		if($_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']) $f[] = "foo.picid = '".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";
		else $f[] = "1=2";
	}
	
	
	$sql = "SELECT 
				'<img src=\"../imagens/alterar.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"window.location=\'sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&iusd='||foo.iusd||'&uncid='||foo.uncid||'&requisicao=carregarProfessorAlfabetizador&direcionar=true\';\">' as acao,
				foo.iuscpf,
				foo.iusnome,
				foo.iusemailprincipal,
				CASE WHEN foo.iustermocompromisso=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=color:red;>Não</font>' END as termo,
				foo.orientadornome
			FROM (
			SELECT
			i.iusd,
			i.picid, 
			i.uncid, 
			i.iuscpf,
			i.iusnome,
			i.iusemailprincipal,
			i.iustermocompromisso, 
			i2.iusnome as orientadornome,
			(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
			(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_PROFESSORALFABETIZADOR.") as perfil,
			CASE WHEN p.muncod IS NOT NULL THEN p.muncod 
			     WHEN p.estuf IS NOT NULL THEN i.muncodatuacao END as muncod,
			CASE WHEN p.muncod IS NOT NULL THEN 'M' 
			     WHEN p.estuf IS NOT NULL THEN 'E' END as esfera
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid
			LEFT JOIN sispacto2.orientadorturma ot ON ot.iusd = i.iusd 
			LEFT JOIN sispacto2.turmas tt ON tt.turid = ot.turid 
			LEFT JOIN sispacto2.identificacaousuario i2 ON i2.iusd = tt.iusd 
			WHERE i.iusstatus='A' AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."' ".(($f2)?"AND ".implode(" AND ",$f2):"").") foo
			".(($f)?"WHERE ".implode(" AND ",$f):"")."
			ORDER BY foo.iusnome";
	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Termo aceito?","Orientador de Estudo");
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

}
?>