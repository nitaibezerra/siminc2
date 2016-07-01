<?php

function redirecionarQuestionario($dados) {
	global $db;
	
	$sql = "SELECT * FROM par.instrumentounidade WHERE inuid='".$dados['inuid']."'";
	$arr = $db->pegaLinha($sql);
	
	if($arr) {
		$_SESSION['par']['itrid'] = $arr['itrid'];
		$_SESSION['par']['inuid'] = $arr['inuid'];
		$_SESSION['par']['estuf'] = $arr['estuf'];
		$_SESSION['par']['muncod'] = $arr['muncod'];
	}
	
	$sql = "SELECT * FROM par.pfadesao WHERE pfaid='".$dados['pfaid']."'";
	$arr = $db->pegaLinha($sql);
	
	if($arr) {
		$_SESSION['par']['pfaid'] = $arr['pfaid'];
		$_SESSION['par']['prgid'] = $arr['prgid'];
		$_SESSION['par']['pfaano'] = $arr['pfaano'];
	}

	echo "<script>window.location='par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=".$dados['pfaid']."';</script>";
	
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

define("QUE_FINANCIAMENTO_DE_NOVAS_TURMAS_DE_EJA",78);
define("PFA_EJA",10);
define("TPD_QUESTIONARIO_EJA", 84);


monta_titulo( 'Relatório de Financiamento - EJA - Simplificado', '&nbsp;' );

if(!$_REQUEST['itrid']) $_REQUEST['itrid']=2;

?>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>
function direcionarQuest(inuid,pfaid) {
	window.location='par.php?modulo=relatorio/financiamento_eja/controleFinanciamento&acao=A&requisicao=redirecionarQuestionario&inuid='+inuid+'&pfaid='+pfaid;
}

</script>
<form name="formulario" id="formulario" action="" method="post">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top">Esfera:</td>	
			<td>
				<label for="estadual">
					<input id="estadual" name="itrid" type="radio" value="1" <?=(($_REQUEST['itrid']==1)?"checked":"") ?>>
					Estadual
				</label>
				<label for="municipal">
					<input id="municipal" name="itrid" type="radio" value="2" <?=(($_REQUEST['itrid']==2)?"checked":"") ?>>
					Municipal
				</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Preenchimento do Questionário:</td>	
			<td>
			<?
			$arr = array(0 => array("codigo"=>"I","descricao"=>"Incompleto"),1 => array("codigo"=>"C","descricao"=>"Completo"));
			$db->monta_combo('situacao', $arr, 'S', "Todos", '', '', '', '', 'S', 'situacao', '', $_REQUEST['situacao']);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Estado do workflow:</td>	
			<td>
			<?
			$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid=".TPD_QUESTIONARIO_EJA." order by esdordem";
			$db->monta_combo('esdid', $sql, 'S', "Todos", '', '', '', '', 'S', 'esdid', '', $_REQUEST['esdid']);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">UF:</td>	
			<td>
			<?
			$sql = "select estuf as codigo, estuf as descricao from territorios.estado order by estuf";
			$db->monta_combo('estuf', $sql, 'S', "Todos", '', '', '', '', 'S', 'estuf', '', $_REQUEST['estuf']);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Nome do Estado/Município:</td>	
			<td><?=campo_texto( 'descricao', 'S', 'S', '', 40, 250, '', '','','','','id="descricao"','',$_REQUEST['descricao']); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Período:</td>	
			<td>
				<?=campo_data2("dtini","N","S","Data Início","##/##/####","","",$_REQUEST['dtini'],"","","dtini");?>
				&nbsp;&nbsp; à &nbsp;&nbsp;
				<?=campo_data2("dtfim","N","S","Data Fim","##/##/####","","",$_REQUEST['dtfim'],"","","dtfim");?>
			</td>
		</tr>
		<tr bgcolor="#CCCCCC">
			<td>&nbsp;</td>
			<td>
				<input type="submit" class="pesquisar" value="Pesquisar"/>
			</td>
		</tr>
		
	</table>
</form>
<?php 

if($_REQUEST['itrid']==1) {
	$join = "INNER JOIN territorios.estado e ON e.estuf=i.estuf";
	$cols = "e.estuf as estado, e.estdescricao as descricao,";
} elseif($_REQUEST['itrid']==2) {
	$join = "INNER JOIN territorios.municipio m ON m.muncod=i.muncod";
	$cols = "m.estuf as estado, m.mundescricao || '<br>IBGE -' || m.muncod as descricao,";
}

if($_REQUEST['situacao']) {
	if($_REQUEST['situacao']=="C") $f[] = "foo.nrespondidos='12 / 12'";
	if($_REQUEST['situacao']=="I") $f[] = "foo.nrespondidos!='12 / 12'";
}

if($_REQUEST['descricao']) {
	$f[] = "removeacento(foo.descricao) ilike removeacento('%".$_REQUEST['descricao']."%')";
}

if($_REQUEST['esdid']) {
	$f[] = "foo.esdid=".$_REQUEST['esdid']."";
}else{
	//$f[] = "foo.esdid in (566,567,568,572)";
}

if($_REQUEST['estuf']) {
	$f[] = "foo.estado='".$_REQUEST['estuf']."'";
}

if($_REQUEST['dtini']) {
	$f[] = "foo.data_envio >= '".formata_data_sql($_REQUEST['dtini'])." 00:00:00'";
}

if($_REQUEST['dtfim']) {
	$f[] = "foo.data_envio <= '".formata_data_sql($_REQUEST['dtfim'])." 23:59:59'";
}

$sql = "SELECT 
			'<center>'||foo.estado||'</center>' as estado,
			'<a style=cursor:pointer; onclick=direcionarQuest('||foo.inuid||','||foo.pfaid||')>'||foo.descricao||'</a>' as descricao,
			'<center>'||to_char(foo.data_envio, 'DD/MM/YYYY')||'</center>',
			foo.esddsc			
		FROM (
		SELECT 
		{$cols} 
       (SELECT COUNT(*) from questionario.resposta where qrpid=q.qrpid AND perid IN(SELECT perid from questionario.pergunta where queid=".QUE_FINANCIAMENTO_DE_NOVAS_TURMAS_DE_EJA.")) || ' / '|| (SELECT COUNT(*) from questionario.pergunta where queid=".QUE_FINANCIAMENTO_DE_NOVAS_TURMAS_DE_EJA.") as nrespondidos,
       (select min(htddata) from workflow.historicodocumento where docid=d.docid) as data_envio,
       ee.esddsc,
       ee.esdid,
       i.inuid,
       p.pfaid
		FROM par.instrumentounidade i 
		{$join}
		INNER JOIN par.pfadesaoprograma p ON i.inuid=p.inuid
		INNER JOIN par.pfquestionario q ON q.adpid=p.adpid 
		INNER JOIN workflow.documento d ON d.docid=p.docid 
		INNER JOIN workflow.estadodocumento ee ON ee.esdid=d.esdid 
		WHERE i.itrid=".$_REQUEST['itrid']." and p.pfaid=".PFA_EJA." ORDER BY 1,2) foo 
		".(($f)?" WHERE ".implode(" AND ",$f):"");

		//ver($sql,d);

$cabecalho = array("UF",
				   "Município",
				   "Data de Envio",
				   "Estado do Workflow");

$db->monta_lista($sql,$cabecalho,6000,5,'S','center',$par2);
?>