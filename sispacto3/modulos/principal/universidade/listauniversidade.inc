<?
include "_funcoes_universidade.php";

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
		if(!isset($_REQUEST['muncod_endereco'])) $_REQUEST['muncod_endereco'] = $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod'];
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['uf']."' ORDER BY mundescricao"; 
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	else: 
		echo "Selecione uma UF";
	endif; ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação Dados do projeto</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_PROJETOIES."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdiddadosprojeto', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdiddadosprojeto', '', $_REQUEST['esdiddadosprojeto']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação Estrutura da formação</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_PROJETOIES."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdidestruturaformacao', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdidestruturaformacao', '', $_REQUEST['esdidestruturaformacao']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação Orçamento</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_PROJETOIES."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdidorcamento', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdidorcamento', '', $_REQUEST['esdidorcamento']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação Equipe IES</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_PROJETOIES."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdidequipeies', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdidequipeies', '', $_REQUEST['esdidequipeies']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação Turmas</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_PROJETOIES."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdidturmas', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdidturmas', '', $_REQUEST['esdidturmas']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação Formação Inicial</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_FORMACAOINICIAL."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdidformacaoinicial', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdidformacaoinicial', '', $_REQUEST['esdidformacaoinicial']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Coordenador IES</td>
	<td><?=campo_texto('coordenadories', "S", "S", "Nome", 67, 150, "", "", '', '', 0, 'id="coordenadories"', '', $_REQUEST['coordenadories']); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/universidade/listauniversidade&acao=A';"></td>
</tr>
</table>
</form>

 <script>

function gerenciarCoordenadorIES(uncid) {
	window.open('sispacto3.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid='+uncid,'CoordenadorIES','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');	
}

</script>
<?

if($_REQUEST['uf']) {
	$f[] = "foo.estuf='".$_REQUEST['uf']."'";
}

if($_REQUEST['muncod_endereco']) {
	$f[] = "foo.muncod='".$_REQUEST['muncod_endereco']."'";
}

if($_REQUEST['esdiddadosprojeto']) {
	if($_REQUEST['esdiddadosprojeto']=='9999999') {
		$f[] = "foo.esdiddadosprojeto IS NULL";		
	} else {
		$f[] = "foo.esdiddadosprojeto='".$_REQUEST['esdiddadosprojeto']."'";		
	}
}

if($_REQUEST['esdidestruturaformacao']) {
	if($_REQUEST['esdidestruturaformacao']=='9999999') {
		$f[] = "foo.esdidestruturaformacao IS NULL";
	} else {
		$f[] = "foo.esdidestruturaformacao='".$_REQUEST['esdidestruturaformacao']."'";
	}
}

if($_REQUEST['esdidorcamento']) {
	if($_REQUEST['esdidorcamento']=='9999999') {
		$f[] = "foo.esdidorcamento IS NULL";
	} else {
		$f[] = "foo.esdidorcamento='".$_REQUEST['esdidorcamento']."'";
	}
}

if($_REQUEST['esdidequipeies']) {
	if($_REQUEST['esdidequipeies']=='9999999') {
		$f[] = "foo.esdidequipeies IS NULL";
	} else {
		$f[] = "foo.esdidequipeies='".$_REQUEST['esdidequipeies']."'";
	}
}

if($_REQUEST['esdidturmas']) {
	if($_REQUEST['esdidturmas']=='9999999') {
		$f[] = "foo.esdidturmas IS NULL";
	} else {
		$f[] = "foo.esdidturmas='".$_REQUEST['esdidturmas']."'";
	}
}

if($_REQUEST['esdidformacaoinicial']) {
	$f[] = "foo.esdidformacaoinicial='".$_REQUEST['esdidformacaoinicial']."'";		
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
			   foo.reinome,
			   foo.situacaodadosprojeto,
			   foo.situacaoestruturaformacao,
			   foo.situacaoorcamento,
			   foo.situacaoequipeies,
			   foo.situacaoturmas,
			   foo.situacaoformacaoinicial
		FROM(
		SELECT '<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"window.location=\'sispacto3.php?modulo=principal/universidade/universidade&acao=A&uncid='||u.uncid||'&requisicao=carregarCoordenadorIES&direcionar=true\';\"></center>' as acao1,
			   '<center><img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorIES(\''||u.uncid||'\');\" style=\"cursor:pointer;\"></center>' as acao2,
				re.reinome,
				su.uninome,
				COALESCE((SELECT iusnome FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd WHERE i.uncid=u.uncid AND t.pflcod=".PFL_COORDENADORIES."),'Coordenador IES não cadastrado') as coordenadories,
				COALESCE(esd.esddsc,'Não iniciou') as situacaodadosprojeto,
				COALESCE(esd2.esddsc,'Não iniciou') as situacaoestruturaformacao,
				COALESCE(esd3.esddsc,'Não iniciou') as situacaoorcamento,
				COALESCE(esd4.esddsc,'Não iniciou') as situacaoequipeies,
				COALESCE(esd5.esddsc,'Não iniciou') as situacaoturmas,
				COALESCE(esd6.esddsc,'Não iniciou') as situacaoformacaoinicial,
				su.uniuf as estuf,
				m.muncod,
				m.mundescricao,
				esd.esdid as esdiddadosprojeto,
				esd2.esdid as esdidestruturaformacao,
				esd3.esdid as esdidorcamento,
				esd4.esdid as esdidequipeies,
				esd5.esdid as esdidturmas,
				esd6.esdid as esdidformacaoinicial
		FROM sispacto3.universidadecadastro u 
		INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
		LEFT JOIN sispacto3.reitor re on re.uniid = su.uniid 
		LEFT JOIN workflow.documento doc on doc.docid = u.dociddadosprojeto 
		LEFT JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
		LEFT JOIN workflow.documento doc2 on doc2.docid = u.docidestruturaformacao 
		LEFT JOIN workflow.estadodocumento esd2 on esd2.esdid = doc2.esdid
		LEFT JOIN workflow.documento doc3 on doc3.docid = u.docidorcamento 
		LEFT JOIN workflow.estadodocumento esd3 on esd3.esdid = doc3.esdid
		LEFT JOIN workflow.documento doc4 on doc4.docid = u.docidequipeies 
		LEFT JOIN workflow.estadodocumento esd4 on esd4.esdid = doc4.esdid 
		LEFT JOIN workflow.documento doc5 on doc5.docid = u.docidturmas 
		LEFT JOIN workflow.estadodocumento esd5 on esd5.esdid = doc5.esdid
			
		LEFT JOIN workflow.documento doc6 on doc6.docid = u.docidformacaoinicial 
		LEFT JOIN workflow.estadodocumento esd6 on esd6.esdid = doc6.esdid
			 
		LEFT JOIN territorios.municipio m ON m.muncod = su.muncod 
		WHERE u.uncstatus='A'
		) foo
		".(($f)?" WHERE ".implode(" AND ",$f):"")." ORDER BY foo.uninome";

$cabecalho = array("&nbsp;","&nbsp;","Universidade","UF","Município","Coordenador IES","Dirigente","Dados do projeto","Estrutura da formação","Orçamento","Equipe IES","Turmas","Formação Inicial");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>