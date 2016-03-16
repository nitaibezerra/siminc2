<?
include "_funcoes_coordenadorlocal.php";


if($_REQUEST['requisicao']) {
	inserirLogRequisicao($_REQUEST);
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>';

echo "<br>";

$perfis = pegaPerfilGeral();
if(!$perfis) $perfis = array();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_EQUIPEMEC,$perfis) || in_array(PFL_CONSULTAMEC,$perfis)) {
	
	$menu[] = array("id" => 1, "descricao" => "Municipal", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Municipal");
	$menu[] = array("id" => 2, "descricao" => "Estadual", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Estadual");
	
} elseif(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto3']['universidade']['uncid']) {
	
	$sql = "SELECT a.muncod FROM sispacto3.abrangencia a INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid WHERE e.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND a.esfera='M'";
	$in_muncod = $db->carregarColuna($sql);
	$sql = "SELECT DISTINCT m.estuf FROM sispacto3.abrangencia a INNER JOIN territorios.municipio m ON m.muncod = a.muncod INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid WHERE e.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND a.esfera='E'";
	$in_estuf = $db->carregarColuna($sql);
	if($in_muncod) $menu[] = array("id" => 1, "descricao" => "Municipal", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Municipal");
	if($in_estuf) $menu[] = array("id" => 2, "descricao" => "Estadual", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Estadual");
	
} elseif(in_array(PFL_COORDENADORADJUNTOIES,$perfis) && $_SESSION['sispacto3']['coordenadoradjuntoies']['uncid']) {
	
	$sql = "SELECT a.muncod FROM sispacto3.abrangencia a INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid WHERE e.uncid='".$_SESSION['sispacto3']['coordenadoradjuntoies']['uncid']."' AND a.esfera='M'";
	$in_muncod = $db->carregarColuna($sql);
	$sql = "SELECT DISTINCT m.estuf FROM sispacto3.abrangencia a INNER JOIN territorios.municipio m ON m.muncod = a.muncod INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid WHERE e.uncid='".$_SESSION['sispacto3']['coordenadoradjuntoies']['uncid']."' AND a.esfera='E'";
	$in_estuf = $db->carregarColuna($sql);
	if($in_muncod) $menu[] = array("id" => 1, "descricao" => "Municipal", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Municipal");
	if($in_estuf) $menu[] = array("id" => 2, "descricao" => "Estadual", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Estadual");
	
} else {
	
	$sql = "SELECT muncod, estuf FROM sispacto3.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$usuarioresponsabilidade = $db->carregar($sql);
	$menu = array();
	if($usuarioresponsabilidade[0]) {
		foreach($usuarioresponsabilidade as $ur) {
			if($ur['muncod']) {
				$in_muncod[] = $ur['muncod']; 
			} elseif($ur['estuf']) {
				$in_estuf[] = $ur['estuf'];
			}
		}
		if($in_muncod) $menu[] = array("id" => 1, "descricao" => "Municipal", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Municipal");
		if($in_estuf) $menu[] = array("id" => 2, "descricao" => "Estadual", "link" => "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=Estadual");
	}
}

if(!$_REQUEST['esfera']) {
	$_REQUEST['esfera'] = "Municipal";
	if($in_estuf)  $_REQUEST['esfera'] = "Estadual";
	if($in_muncod) $_REQUEST['esfera'] = "Municipal";
}

if(!$menu) {
	$menu = array();
	$f[] = "1=2";
}

$abaativa = "sispacto3.php?modulo=principal/coordenadorlocal/listacoordenadorlocal&acao=A&esfera=".$_REQUEST['esfera'];

echo montarAbasArray($menu, $abaativa);

monta_titulo( "Lista - Coordenador Local", "Lista ".$_REQUEST['esfera']." de Coordenadores Locais participantes");

?>
<script>

function gerenciarCoordenadorLocal(picid) {
	window.open('sispacto3.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid='+picid,'Anexos','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');	
}

function acessarCoordenadorLocal(estuf,muncod,obj) {
	var linha = obj.parentNode.parentNode.parentNode;
	var combo = linha.cells[5].childNodes[0];
	var iusd = combo.value;

	window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&estuf='+estuf+'&muncod='+muncod+'&iusd='+iusd+'&requisicao=carregarCoordenadorLocal&direcionar=true';
}

</script>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uf', $sql, 'S', 'Selecione', (($_REQUEST['esfera'] == "Municipal")?'carregarMunicipiosPorUF3':''), '', '', '200', 'N', 'uf', '', $_REQUEST['uf']);
	?></td>
</tr>
<? if($_REQUEST['esfera'] == "Municipal") : ?>
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
<? endif; ?>
<tr>
	<td class="SubTituloDireita">Situação</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_ORIENTADORESTUDO."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Elaboração' as descricao)";
	$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'uf', '', $_REQUEST['esdid']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação(Composição Turmas)</td>
	<td><?
	$sql = "(SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_FLUXOTURMA."' ORDER BY esdordem) 
			UNION ALL(SELECT 9999999 as codigo, 'Não iniciou Composição' as descricao)";
	$db->monta_combo('esdidcomposicaoturmas', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdidcomposicaoturmas', '', $_REQUEST['esdidcomposicaoturmas']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">CPF Coordenador Local</td>
	<td><?=campo_texto('cpfcoordenadorlocal', "N", "S", "CPF", 15, 15, "###.###.###-##", "", '', '', 0, 'id="cpfcoordenadorlocal"', '', $_REQUEST['cpfcoordenadorlocal']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome Coordenador Local</td>
	<td><?=campo_texto('coordenadorlocal', "N", "S", "Nome", 67, 150, "", "", '', '', 0, 'id="coordenadorlocal"', '', $_REQUEST['coordenadorlocal']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Exibir somente Coordenador Local não cadastrado</td>
	<td><input type="checkbox" name="somentecoordenadorlocal" value="TRUE" <?=(($_REQUEST['somentecoordenadorlocal']=="TRUE")?"checked":"") ?>></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location=window.location;"></td>
</tr>
</table>
</form>
<?

if($_REQUEST['uf']) {
	$f[] = "foo.estuf='".$_REQUEST['uf']."'";
}
if($_REQUEST['muncod_endereco']) {
	$f[] = "foo.muncod='".$_REQUEST['muncod_endereco']."'";
}
if($_REQUEST['coordenadorlocal']) {
	$f[] = "removeacento(foo.coordenadorlocal) ilike removeacento('%".$_REQUEST['coordenadorlocal']."%')";
}

if($_REQUEST['cpfcoordenadorlocal']) {
	$f[] = "removeacento(foo.coordenadorlocal) ilike '%".str_replace(array(".","-"),array("",""),$_REQUEST['cpfcoordenadorlocal'])."%'";
}

if($_REQUEST['esdid']) {
	if($_REQUEST['esdid']=='9999999') {
		$f[] = "foo.esdid IS NULL";		
	} else {
		$f[] = "foo.esdid='".$_REQUEST['esdid']."'";		
	}
}

if($_REQUEST['esdidcomposicaoturmas']) {
	if($_REQUEST['esdidcomposicaoturmas']=='9999999') {
		$f[] = "foo.esdidturmas IS NULL";		
	} else {
		$f[] = "foo.esdidturmas='".$_REQUEST['esdidcomposicaoturmas']."'";		
	}
}

if($_REQUEST['somentecoordenadorlocal']=="TRUE") {
	$f[] = "foo.coordenadorlocal ilike '%Coordenador Local não cadastrado%'";
}


if($_REQUEST['esfera'] == "Estadual") {
	$inn = "INNER JOIN territorios.estado m ON m.estuf = p.estuf".(($in_estuf)?" AND p.estuf IN('".implode("','",$in_estuf)."')":"");
	$col = "'<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"acessarCoordenadorLocal(\''||m.estuf||'\',\'\',this);\"></center>' as acao1,'<center><img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorLocal(\''||p.picid||'\');\" style=\"cursor:pointer;\"></center>' as acao2, m.estuf as estado, m.estdescricao as descricao,";
	$id_select = "estadual_'||m.estuf||'";
} else {
	$aa = 1;
	$inn = "INNER JOIN territorios.municipio m ON m.muncod = p.muncod".(($in_muncod)?" AND p.muncod IN('".implode("','",$in_muncod)."')":"");;
	$col = "'<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"acessarCoordenadorLocal(\''||m.estuf||'\',\''||m.muncod||'\',this);\">' as acao1, '<center><img   src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorLocal(\''||p.picid||'\');\" style=\"cursor:pointer;\"></center>' as acao2, m.estuf as estado, m.mundescricao as descricao,";
	$id_select = "municipal_'||m.estuf||'_'||m.muncod||'";
}


$sql = "SELECT CASE WHEN foo.coordenadorlocal='<select style=\"width:200px;\" id=\"".str_replace("m.","foo.",$id_select)."\"></select>' THEN '&nbsp;' ELSE foo.acao1::text END, foo.acao2::text, foo.estado, foo.descricao, foo.situacao, foo.coordenadorlocal, foo.situacaoturmas FROM (
		SELECT {$col} COALESCE(e.esddsc,'Não iniciou Elaboração') as situacao,
		COALESCE('<select style=\"width:200px;\" id=\"{$id_select}\">'||array_to_string(array(SELECT '<option value='||i.iusd||'>'||i.iusnome||' ('||i.iuscpf||' - '||COALESCE(uu.unisigla,'Sem IES')||')</option>' FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd LEFT JOIN sispacto3.universidadecadastro uc ON uc.uncid = i.uncid LEFT JOIN sispacto3.universidade uu ON uu.uniid = uc.uniid WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), '')||'</select>','Coordenador Local não cadastrado') as coordenadorlocal,
		p.picstatus,
		m.estuf,
		p.muncod,
		e.esdid,
		COALESCE(e2.esddsc,'Não iniciou Composição') as situacaoturmas,
		e2.esdid as esdidturmas
		FROM sispacto3.pactoidadecerta p 
		{$inn} 
		LEFT JOIN workflow.documento d ON d.docid = p.docid  
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
		LEFT JOIN workflow.documento d2 ON d2.docid = p.docidturma  
		LEFT JOIN workflow.estadodocumento e2 ON e2.esdid = d2.esdid 
		WHERE p.picstatus='A') foo 
		".(($f)?" WHERE ".implode(" AND ",$f):"")." ORDER BY foo.estuf, foo.descricao";

		
$cabecalho = array("&nbsp;","&nbsp;","UF","Descrição","Situação","Coordenador Local","Situação(Composição Turmas)");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>