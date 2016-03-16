<?
include APPRAIZ ."includes/workflow.php";
include_once "_funcoes.php";
include_once "_funcoes_escola.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

monta_titulo( "Lista - Escolas", "Lista de Escolas do Ensino Médio");

?>
<script>

function gerenciarGestorEscola(lemcodigoinep) {
	window.open('sismedio.php?modulo=principal/escola/gerenciargestorescola&acao=A&lemcodigoinep='+lemcodigoinep,'Gestor','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');	
}

function gerenciarApEscola(inep, obj) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	
	if(obj.title=="mais") {
		obj.title      = "menos";
		obj.src        = "../imagens/menos.gif";
		var nlinha     = tabela.insertRow(linha.rowIndex);
		var ncol0       = nlinha.insertCell(0);
		ncol0.innerHTML = "&nbsp;";
		var ncol1       = nlinha.insertCell(1);
		ncol1.colSpan   = 11;
		ncol1.id      = 'esc_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarCadastramentoEscola&lemcodigoinep='+inep,ncol1.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
	
}

function validarCadastramentoEscola(docid) {
	var conf = confirm('Deseja realmente validar o cadastramento da escola?');

	if(conf) {
		ajaxatualizar('requisicao=validarCadastramentoEscola&docid='+docid,'');
		jQuery('#formulario').submit();
	}
}

</script>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><? 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('lemuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUFGestor', '', '', '', 'S', 'lemuf', '', $_REQUEST['lemuf']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio"><?
	
	if($_REQUEST['lemuf']) {

		carregarMunicipiosPorUF(array('id' => 'muncod','name' => 'muncod', 'estuf' => $_REQUEST['lemuf'],'valuecombo' => $_REQUEST['muncod']));
		
	} else {

		echo "Selecione uma UF";
		
	} 

	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação</td>
	<td><? 
	$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_FLUXOESCOLA."' ORDER BY esdordem";
	$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'esdid', '', $_REQUEST['esdid']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">CPF</td>
	<td><? echo campo_texto('lemcpfgestor', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, '', '', $_REQUEST['lemcpfgestor'] ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome do diretor</td>
	<td><? echo campo_texto('lemnomegestor', "S", "S", "Nome do diretor", 67, 150, "", "", '', '', 0, '', '', $_REQUEST['lemnomegestor'] ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">INEP</td>
	<td><? echo campo_texto('lemcodigoinep', "S", "S", "CPF", 15, 14, "###########", "", '', '', 0, '', '', $_REQUEST['lemcodigoinep'] ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome da escola</td>
	<td><? echo campo_texto('lemnomeescola', "S", "S", "Nome da escola", 67, 150, "", "", '', '', 0, '', '', $_REQUEST['lemnomeescola'] ); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sismedio.php?modulo=principal/escola/listaescola&acao=A';"></td>
</tr>
</table>
</form>
<?
if($_REQUEST['esdid']) {
	if($_REQUEST['esdid']=='9999999') $f[] = "e.esdid IS NULL";
	else $f[] = "e.esdid='".$_REQUEST['esdid']."'";
}

if($_REQUEST['lemuf']) {
	$f[] = "l.lemuf='".$_REQUEST['lemuf']."'";
}

if($_REQUEST['muncod']) {
	$f[] = "l.muncod='".$_REQUEST['muncod']."'";
}

if($_REQUEST['lemnomegestor']) {
    $lemnomegestorTmp = removeAcentos(str_replace("-"," ",$_REQUEST['lemnomegestor']));
	$f[] = " UPPER(public.removeacento(l.lemnomegestor)) ilike '%".$lemnomegestorTmp."%'";
}

if($_REQUEST['lemcpfgestor']) {
    $lemcpfgestor = removeAcentos($_REQUEST['lemcpfgestor']);
	$f[] = "UPPER(public.removeacento(l.lemcpfgestor)) = '".str_replace(array(".","-"),array("",""),$lemcpfgestor)."'";
}

if($_REQUEST['lemnomeescola']) {
    $lemnomeescolaTmp = removeAcentos(str_replace("-"," ",$_REQUEST['lemnomeescola']));
	$f[] = "UPPER(public.removeacento(l.lemnomeescola)) ilike '%".$lemnomeescolaTmp."%'";
}

if($_REQUEST['lemcodigoinep']) {
	$f[] = "l.lemcodigoinep='".$_REQUEST['lemcodigoinep']."'";
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_DIRIGENTEESTADUAL,$perfis)) {
	$f[] = "l.lemuf IN( SELECT estuf FROM sismedio.usuarioresponsabilidade WHERE rpustatus='A' AND pflcod='".PFL_DIRIGENTEESTADUAL."' AND usucpf='".$_SESSION['usucpf']."' )";
}

if(in_array(PFL_SUPERVISORIES,$perfis)) {
	if($_SESSION['sismedio']['supervisories']['uncid']) $f[] = "(l.lemuf='".$_SESSION['sismedio']['supervisories']['estuf']."' OR l.lemuf IN(select distinct lemuf from sismedio.abrangencia a inner join sismedio.listaescolasensinomedio l on l.lemcodigoinep = a.lemcodigoinep inner join sismedio.estruturacurso e on e.ecuid = a.ecuid where e.uncid='".$_SESSION['sismedio']['supervisories']['uncid']."'))";
	else $f[] = "1=2";
}

if(in_array(PFL_GESTORESCOLA,$perfis)) {
	$f[] = "l.lemcodigoinep::bigint IN( SELECT iuscodigoinep FROM sismedio.identificacaousuario WHERE iuscpf='".$_SESSION['usucpf']."' )";
}


$sql = "SELECT foo.acao, foo.consultar, foo.lemuf, foo.lemmundsc, foo.lemlocalizacao, foo.lemcodigoinep, foo.lemnomeescola, foo.gestor, CASE WHEN sistema IS NOT NULL AND perfil IS NOT NULL THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\">' END acesso, COALESCE(foo.esddsc,'Não iniciado') as esddsc, foo.nprof, foo.norie 
		FROM ( 
		SELECT CASE WHEN d.esdid=".ESD_ESCOLA_EM_ANALISE." THEN '<img src=\"../imagens/mais.gif\" title=mais align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"gerenciarApEscola('|| lemcodigoinep ||', this);\"> ' ELSE '' END || '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"acessarDiretor(\''||lemcpfgestor||'\','||lemcodigoinep||')\">' as acao, 
			   '<img src=\"../imagens/usuario.gif\" style=\"cursor:pointer;\" onclick=\"gerenciarGestorEscola('|| lemcodigoinep ||');\">' as consultar,
			   lemuf,
			   lemmundsc,
			   lemlocalizacao,
			   lemcodigoinep,
			   lemnomeescola, 
			   (SELECT count(*) FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd WHERE i.iusstatus='A' AND t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."') AND i.iuscodigoinep=l.lemcodigoinep::bigint) as nprof,
			   (SELECT count(*) FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd INNER JOIN sismedio.definicaoorientadoresestudo d ON d.iusd = i.iusd AND d.doecodigoinep = i.iuscodigoinep WHERE i.iusstatus='A' AND i.iuscodigoinep=l.lemcodigoinep::bigint) as norie,
			   e.esddsc,
			   CASE WHEN lemcpfgestor IS NOT NULL THEN replace(to_char(lemcpfgestor::numeric, '000:000:000-00'), ':', '.') || ' - ' || lemnomegestor 
					ELSE 'Gestor não cadastrado' END as gestor,
				(SELECT usucpf FROM seguranca.usuario_sistema WHERE usucpf=l.lemcpfgestor AND sisid=".SIS_MEDIO." AND suscod='A') as sistema,
				(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=l.lemcpfgestor AND pflcod=".PFL_GESTORESCOLA.") as perfil
		FROM sismedio.listaescolasensinomedio l 
		LEFT JOIN workflow.documento d ON d.docid = l.docid 
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
		".(($f)?"WHERE ".implode(" AND ",$f):"")."
		) foo 
		ORDER BY foo.lemuf, foo.lemmundsc";

$cabecalho = array("&nbsp;","&nbsp;","UF","Município","Localização","INEP","Escola","Nome do diretor","Acesso","Situação cadastramento","Qtd Professores","Qtd Orientadores");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>