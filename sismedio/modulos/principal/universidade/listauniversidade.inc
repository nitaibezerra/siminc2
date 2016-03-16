<?
include "_funcoes_universidade.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

monta_titulo( "Lista - Universidades", "Lista das universidades participantes");

?>
 
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '200', 'N', 'uf', '', $_REQUEST['uf']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<? 
	if($_REQUEST['uf']) :
		if(!isset($_REQUEST['muncod_endereco'])) $_REQUEST['muncod_endereco'] = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'];
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['uf']."' ORDER BY mundescricao"; 
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	else: 
		echo "Selecione uma UF";
	endif; ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_COORDENADORIES."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdid', '', $_REQUEST['esdid']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação (FR)</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_COMPOSICAOTURMA."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdid2', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdid2', '', $_REQUEST['esdid2']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação (OE)</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_COMPOSICAOTURMA."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdid3', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdid3', '', $_REQUEST['esdid3']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Coordenador IES</td>
	<td><?=campo_texto('coordenadories', "S", "S", "Nome", 67, 150, "", "", '', '', 0, 'id="coordenadories"', '', $_REQUEST['coordenadories']); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/listauniversidade&acao=A';"></td>
</tr>
</table>
</form>

 <script>

function gerenciarCoordenadorIES(uncid) {
	window.open('sismedio.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid='+uncid,'CoordenadorIES','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');	
}

</script>
<?

if($_REQUEST['uf']) {
	$f[] = "foo.estuf='".$_REQUEST['uf']."'";
}

if($_REQUEST['muncod_endereco']) {
	$f[] = "foo.muncod='".$_REQUEST['muncod_endereco']."'";
}

if($_REQUEST['esdid']) {
	if($_REQUEST['esdid']=='9999999') {
		$f[] = "foo.esdid IS NULL";		
	} else {
		$f[] = "foo.esdid='".$_REQUEST['esdid']."'";		
	}
}

if($_REQUEST['esdid2']) {
	if($_REQUEST['esdid2']=='9999999') {
		$f[] = "foo.esdid2 IS NULL";
	} else {
		$f[] = "foo.esdid2='".$_REQUEST['esdid2']."'";
	}
}

if($_REQUEST['esdid3']) {
	if($_REQUEST['esdid3']=='9999999') {
		$f[] = "foo.esdid3 IS NULL";
	} else {
		$f[] = "foo.esdid3='".$_REQUEST['esdid3']."'";
	}
}

if($_REQUEST['coordenadories']) {
	$f[] = "removeacento(foo.coordenadories) ilike removeacento('%".$_REQUEST['coordenadories']."%')";
}

$sql = "SELECT CASE WHEN foo.coordenadories='Coordenador IES não cadastrado' THEN '&nbsp;' ELSE foo.acao1 END as acao1,
			   foo.acao2,
			   foo.uninome,
			   foo.estuf,
			   foo.mundescricao,
			   foo.coordenadories,
			   foo.situacao,
			   foo.esdturmaformadoresregionais,
			   foo.esdturmaorientadoresestudo,
			   CASE WHEN foo.qtdpr=0 THEN 'Não' ELSE 'Sim' END as prcadastrado

		FROM(
		SELECT '<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"window.location=\'sismedio.php?modulo=principal/universidade/universidade&acao=A&uncid='||u.uncid||'&requisicao=carregarCoordenadorIES&direcionar=true\';\"></center>' as acao1,
			   '<center><img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorIES(\''||u.uncid||'\');\" style=\"cursor:pointer;\"></center>' as acao2,
				su.uninome,
				COALESCE(array_to_string(array((SELECT iusnome FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES.")),'<br>'),'Coordenador IES não cadastrado') as coordenadories,
				COALESCE(esd.esddsc,'Não iniciou Elaboração') as situacao,
				su.uniuf as estuf,
				m.muncod,
				m.mundescricao,
				esd.esdid,
				esd2.esdid as esdid2,
				esd3.esdid as esdid3,
				COALESCE(esd2.esddsc,'Não iniciou Elaboração') as esdturmaformadoresregionais,
				COALESCE(esd3.esddsc,'Não iniciou Elaboração') as esdturmaorientadoresestudo,
				(SELECT count(*) FROM sismedio.folhapagamentouniversidade WHERE uncid=u.uncid AND pflcod IS NOT NULL) as qtdpr
		FROM sismedio.universidadecadastro u 
		INNER JOIN sismedio.universidade su ON su.uniid = u.uniid
		LEFT JOIN sismedio.reitor re on re.uniid = su.uniid 
		LEFT JOIN workflow.documento doc on doc.docid = u.docid 
		LEFT JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
		LEFT JOIN workflow.documento doc2 on doc2.docid = u.docidturmaformadoresregionais 
		LEFT JOIN workflow.estadodocumento esd2 on esd2.esdid = doc2.esdid
		LEFT JOIN workflow.documento doc3 on doc3.docid = u.docidturmaorientadoresestudo 
		LEFT JOIN workflow.estadodocumento esd3 on esd3.esdid = doc3.esdid 
				
		LEFT JOIN territorios.municipio m ON m.muncod = su.muncod 
		WHERE u.uncstatus='A'
		) foo
		".(($f)?" WHERE ".implode(" AND ",$f):"")." ORDER BY foo.uninome";

$cabecalho = array("&nbsp;","&nbsp;","Universidade","UF","Município","Coordenador IES","Situação","Situação (FR)","Situação (OE)","Período de referência");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>