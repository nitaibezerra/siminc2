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


monta_titulo( 'Relatório de Controle de Financiamento - EJA', '&nbsp;' );

if(!$_REQUEST['itrid']) $_REQUEST['itrid']=2;

?>
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
}
if($_REQUEST['estuf']) {
	$f[] = "foo.estado='".$_REQUEST['estuf']."'";
}

$sql = "SELECT 
			foo.estado,
			'<a style=cursor:pointer; onclick=direcionarQuest('||foo.inuid||','||foo.pfaid||')>'||foo.descricao||'</a>' as descricao,
			foo.nrespondidos,
			foo.data_inicio,
			foo.nescolas,
			CASE WHEN ( (to_number(foo.total_matriculas,'999999') != foo.total_soma_matriculas1) OR (to_number(foo.total_matriculas,'999999') < foo.total_soma_matriculas2) )  THEN '<div align=right><font color=FF0000>'|| foo.total_matriculas ||'</font></div>' ELSE ''|| foo.total_matriculas ||'' END,
			foo.total_soma_matriculas1,
			foo.total_soma_matriculas2,
			foo.vagas_ef_ai,
			foo.vagas_ef_af,
			foo.vagas_em,
			foo.eja_ef_ai,
			foo.eja_ef_af,
			foo.eja_em,
			foo.matriculas_prog_brasil_alfa,
			foo.matriculas_comunidade_rurais,
			foo.matriculas_quilombolas,
			foo.matriculas_indigenas,
			foo.matriculas_unidades_prisionais,
			foo.esddsc			
		FROM (
		SELECT 
		{$cols} 
       (SELECT COUNT(*) from questionario.resposta where qrpid=q.qrpid AND perid IN(SELECT perid from questionario.pergunta where queid=".QUE_FINANCIAMENTO_DE_NOVAS_TURMAS_DE_EJA.")) || ' / '|| (SELECT COUNT(*) from questionario.pergunta where queid=".QUE_FINANCIAMENTO_DE_NOVAS_TURMAS_DE_EJA.") as nrespondidos,
	   (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3001) as data_inicio,
       (SELECT COUNT(*) from par.pfescolaeja where adpid=p.adpid) nescolas,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3003) as total_matriculas,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3004) as vagas_ef_ai,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3005) as vagas_ef_af,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3006) as vagas_em,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3008) as eja_ef_ai,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3009) as eja_ef_af,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3010) as eja_em,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3011) as matriculas_prog_brasil_alfa,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3012) as matriculas_comunidade_rurais,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3013) as matriculas_quilombolas,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3014) as matriculas_indigenas,
       (SELECT resdsc from questionario.resposta where qrpid=q.qrpid AND perid=3015) as matriculas_unidades_prisionais,
       (SELECT SUM(to_number((CASE WHEN resdsc = '' OR resdsc IS NULL THEN '0' ELSE resdsc END),'999999')) FROM questionario.resposta where qrpid=q.qrpid AND perid in(3004,3005,3006,3008,3009,3010)) as total_soma_matriculas1,
       (SELECT SUM(to_number((CASE WHEN resdsc = '' OR resdsc IS NULL THEN '0' ELSE resdsc END),'999999')) FROM questionario.resposta where qrpid=q.qrpid AND perid in(3011,3012,3013,3014,3015)) as total_soma_matriculas2,
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
				   "Itens respondidos",
				   "Data de Ínicio",
				   "Total de Escolas Atendidas",
				   "Total de Matrículas",
				   "Total Etapas de Ensino",
				   "Total Público Prioritário",
				   "Quantidade de Vagas Ensino Fundamental - Anos Iniciais",
				   "Quantidade de Vagas Ensino Fundamental - Anos Finais",
				   "Quantidade de Vagas Ensino Médio",
				   "Quantidade EJA integrada a qualificação profissional Ensino Fundamental - Anos Iniciais",
				   "Quantidade EJA integrada a qualificação profissional Ensino Fundamental - Anos Finais",
				   "Quantidade EJA integrada a qualificação profissional Ensino Médio",
				   "Número de matrículas dos públicos prioritários Egressos do Programa Brasil Alfabetizado",
				   "Número de matrículas dos públicos prioritários Estudantes das Comunidades Rurais",
				   "Número de matrículas dos públicos prioritários Quilombolas",
				   "Número de matrículas dos públicos prioritários Indígenas",
				   "Número de matrículas dos públicos prioritários Pessoas que cumprem pena em unidades prisionais",
				   "Estado do Workflow");

$db->monta_lista($sql,$cabecalho,6000,5,'S','center',$par2);
?>