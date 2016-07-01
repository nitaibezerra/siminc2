<?php

if($_REQUEST['requisicao']=='atualizarFlagsProjeto') {
	
	$sql = "UPDATE sisfor.sisfor SET 
			".(($_REQUEST['sifsomenteorcamentoobr'])?"sifsomenteorcamentoobr=".$_REQUEST['sifsomenteorcamentoobr']:"")." 
			".(($_REQUEST['sifexecucaosisfor'])?"sifexecucaosisfor=".$_REQUEST['sifexecucaosisfor']:"")."  
			WHERE sifid='".$_REQUEST['sifid']."'";
	$db->executar($sql);
	$db->commit();
	exit;
	
}

if($_REQUEST['requisicao']=='verProjetoCurso') {
	
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

	
	if($_REQUEST['sifid']) $_SESSION['sisfor']['sifid'] = $_REQUEST['sifid'];
	
	$arr = $db->pegaLinha("SELECT s.unicod, 
								  CASE WHEN s.ieoid IS NOT NULL THEN ieo.curid
									   WHEN s.cnvid IS NOT NULL THEN cnv.curid END as curid, 
								  s.docidprojeto 
						   FROM sisfor.sisfor s
						   LEFT JOIN catalogocurso2014.iesofertante ieo ON ieo.ieoid = s.ieoid 
						   LEFT JOIN sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
						   WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");
	
	$_SESSION['sisfor']['unicod']       = $arr['unicod'];
	$_SESSION['sisfor']['docidprojeto'] = $arr['docidprojeto'];
	
	if($arr['curid']) $_SESSION['sisfor']['curid'] = $arr['curid'];
	else unset($_SESSION['sisfor']['curid']);
	
	include APPRAIZ ."includes/workflow.php";
	
	include_once APPRAIZ_SISFOR.'coordenador_curso/visualizacao_projeto.inc';
	
	exit;
	
}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

monta_titulo( "Análise dos cursos - Projeto", "Analisar os projetos propostos pelos coordenadores de curso");

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>
<script>

function verProjetoCurso(sifid) {

	ajaxatualizar('requisicao=verProjetoCurso&sifid='+sifid,'modalDiv');

	jQuery("#modalDiv").dialog({
        draggable:true,
        resizable:true,
        width: 1280,
        height: 600,
        modal: true,
     	close: function(){} 
    });

	
}

function pesquisarCursos() {
	
	jQuery('#formulario').submit();
	
}

function wf_exibirHistorico( docid )
{
	var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

function acessarCursoDireto(sifid) {
	window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&sifid='+sifid;
}

function somenteOrcamento(sifid, sit) {
	
	if(sit=='TRUE') {
		var conf = confirm('Deseja realmente considerar obrigatório somente o Orçamento?');
		
		if(conf) {
			ajaxatualizar('requisicao=atualizarFlagsProjeto&sifid='+sifid+'&sifsomenteorcamentoobr='+sit,'');
			jQuery('#somenteOrcamento_'+sifid).attr('src','../imagens/money_g.gif');
		}
		
	} else {

		var conf = confirm('Deseja realmente considerar obrigatório todas as informações padrões?');
		
		if(conf) {
			ajaxatualizar('requisicao=atualizarFlagsProjeto&sifid='+sifid+'&sifsomenteorcamentoobr='+sit,'');
			jQuery('#somenteOrcamento_'+sifid).attr('src','../imagens/money_01.gif');
		}
	
	}

	
	
}


function execucaoSisfor(sifid, sit) {
	
	if(sit=='TRUE') {
		var conf = confirm('Confirma que o curso será executado pelo SISFOR?');
		
		if(conf) {
			ajaxatualizar('requisicao=atualizarFlagsProjeto&sifid='+sifid+'&sifexecucaosisfor='+sit,'');
			jQuery('#execucaoSisfor_'+sifid).attr('src','../imagens/valida1.gif');
		}
		
	} else {

		var conf = confirm('Confirma que o curso não será executado pelo SISFOR?');
		
		if(conf) {
			ajaxatualizar('requisicao=atualizarFlagsProjeto&sifid='+sifid+'&sifexecucaosisfor='+sit,'');
			jQuery('#execucaoSisfor_'+sifid).attr('src','../imagens/valida3.gif');
		}
	
	}

	
	
}


</script>

<div id="modalDiv" style="display: none;"></div>

<?
$menu[] = array("id" => 1, "descricao" => "Fase 1 - 2014", "link" => "sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A&siftipoplanejamento=1");
$menu[] = array("id" => 2, "descricao" => "Fase 2 - 2014", "link" => "sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A&siftipoplanejamento=2");
$menu[] = array("id" => 2, "descricao" => "2015", "link" => "sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A&siftipoplanejamento=3");

if(!$_REQUEST['siftipoplanejamento']) $_REQUEST['siftipoplanejamento']='1';
$abaAtiva = "sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A&siftipoplanejamento=".$_REQUEST['siftipoplanejamento'];

echo montarAbasArray($menu, $abaAtiva);  
?>



<form id="formulario" method="post" name="formulario" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="pesquisarUniversidade" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita">Secretaria:</td>
			<td>
			<?php 
			$sql = "SELECT coordid as codigo, coordsigla as descricao FROM catalogocurso2014.coordenacao WHERE coorano='".$_SESSION['exercicio']."' ORDER BY coordid";
			$db->monta_combo('coordid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'coordid', '', $_REQUEST['coordid']);
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Situação:</td>
			<td>
			<?php 
			$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".WF_TPDID_PROJETO."' ORDER BY esdid";
			$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'esdid', '', $_REQUEST['esdid']);
			?>
			</td>
		</tr>				
		<tr>
			<td class="subtituloDireita">Composição da equipe:</td>
			<td>
			<?php 
			$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".WF_TPDID_SISFOR_CADASTRAMENTO."' ORDER BY esdid";
			$db->monta_combo('esdidcomporequipe', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'esdidcomporequipe', '', $_REQUEST['esdidcomporequipe']);
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Universidade:</td>
			<td>
			<?php
			
			$sql = "SELECT 	DISTINCT ent.entunicod as codigo, COALESCE(ent.entsig||' - ','')||ent.entnome as descricao
					FROM 		entidade.entidade ent
					INNER JOIN 	entidade.funcaoentidade fun ON fun.entid = ent.entid AND funid IN (12,11) 
					INNER JOIN sisfor.sisfories si ON si.unicod = ent.entunicod 
					WHERE ent.entstatus='A' AND ent.entunicod IS NOT NULL 
					ORDER BY 2";
			
			$db->monta_combo('unicod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'unicod', '', $_REQUEST['unicod']); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Coordenador do curso:</td>
			<td>
			<?php
			$coordenadorcurso = $_REQUEST['coordenadorcurso'];
			echo campo_texto('coordenadorcurso', 'N', 'S', 'Coordenador do curso', '50', '', '', '','',''); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome do curso:</td>
			<td>
			<?php 
			$nomecurso = $_REQUEST['nomecurso'];
			echo campo_texto('nomecurso', 'N', 'S', 'Nome do curso', '50', '', '', '','',''); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Execução no SISFOR:</td>
			<td>
			<input type="radio" name="sifexecucaosisfor" value="" <?=((!$_REQUEST['sifexecucaosisfor'])?"checked":"")?>> TODOS
			<input type="radio" name="sifexecucaosisfor" value="1" <?=(($_REQUEST['sifexecucaosisfor']=='1')?"checked":"")?>> SIM 
			<input type="radio" name="sifexecucaosisfor" value="2" <?=(($_REQUEST['sifexecucaosisfor']=='1')?"checked":"")?>> NÃO
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisarCursos();"/>
				<input type="button" value="Ver todos" id="btnTodos" onclick="window.location='sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A&siftipoplanejamento=<?=$_REQUEST['siftipoplanejamento'] ?>';"/>
				<input type="button" value="Gerar XLS" id="btnXLS" onclick="window.location='sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A&siftipoplanejamento=<?=$_REQUEST['siftipoplanejamento'] ?>&visualizacao=xls';"/>
			</td>
		</tr>
	</table>
</form>

<?

$perfis = pegaPerfilGeral();

if(in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis)) {
	$coordids = $db->carregarColuna("SELECT DISTINCT coordid FROM sisfor.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod IN('".PFL_EQUIPE_MEC."','".PFL_COORDENADOR_MEC."','".PFL_DIRETOR_MEC."') ANd rpustatus='A'");
	
	if($coordids) 
		$wh[] = "(cur.coordid IN(".implode(",",$coordids).") OR cur2.coordid IN(".implode(",",$coordids).") OR oc.coordid IN(".implode(",",$coordids).") OR cor4.coordid IN(".implode(",",$coordids)."))";

}

if(in_array(PFL_COORDENADOR_INST,$perfis)) {
	
	$unicods = $db->carregarColuna("SELECT unicod FROM sisfor.sisfories s 
									  INNER JOIN sisfor.tipoperfil t ON t.tpeid = s.tpeid 
									  INNER JOIN sisfor.identificacaousuario i ON i.iusd = t.iusd  
									  WHERE i.iuscpf='".$_SESSION['usucpf']."'");
	

	if($unicods)
		$wh[] = "s.unicod IN('".implode("','",$unicods)."')";

}

if($_REQUEST['esdid']) {
	$wh[] = "d.esdid='".$_REQUEST['esdid']."'";
}

if($_REQUEST['coordid']) {
	$wh[] = "(cor.coordid='".$_REQUEST['coordid']."' OR cor2.coordid='".$_REQUEST['coordid']."' OR cor3.coordid='".$_REQUEST['coordid']."' OR cor4.coordid='".$_REQUEST['coordid']."')";
}

if($_REQUEST['unicod']) {
	$wh[] = "u.unicod='".$_REQUEST['unicod']."'";
}

if($_REQUEST['esdidcomporequipe']) {
	$wh[] = "ectur.esdid='".$_REQUEST['esdidcomporequipe']."'";
}

if($_REQUEST['coordenadorcurso']) {
	$wh[] = "usu.usunome ilike '%".$_REQUEST['coordenadorcurso']."%'";
}

if($_REQUEST['nomecurso']) {
	$wh[] = "(cur.curid||' '||cur.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR cur2.curid||' '||cur2.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR oc.ocunome ilike '%".$_REQUEST['nomecurso']."%' OR oat.oatnome ilike '%".$_REQUEST['nomecurso']."%')";
}

if($_REQUEST['siftipoplanejamento']) {
	$wh[] = "s.siftipoplanejamento='".$_REQUEST['siftipoplanejamento']."'";
}

if($_REQUEST['sifexecucaosisfor']) {
	$wh[] = "s.sifexecucaosisfor=TRUE";
}

if(in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_INST,$perfis)) { 
	$acao = "'<span style=white-space:nowrap;><img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"verProjetoCurso('||s.sifid||');\"> <img style=\"cursor: pointer;\" src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \''||s.docidprojeto||'\' );\"> <img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||s.sifid||')\"></span>'";
} else {
	$acao = "'<span style=white-space:nowrap;><img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"verProjetoCurso('||s.sifid||');\"> <img style=\"cursor: pointer;\" src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \''||s.docidprojeto||'\' );\"> <img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||s.sifid||')\"> <img '||CASE WHEN s.sifsomenteorcamentoobr=TRUE THEN 'src=\"../imagens/money_g.gif\" onmouseover=\"return escape(\'Somente o preenchimento do orçamento é obrigatório\');\" onclick=\"somenteOrcamento('||s.sifid||',\'FALSE\')\"' ELSE 'src=\"../imagens/money_01.gif\" onmouseover=\"return escape(\'Todas as informações são obrigatórias\');\" onclick=\"somenteOrcamento('||s.sifid||',\'TRUE\')\"' END||' id=\"somenteOrcamento_'||s.sifid||'\" style=\"cursor:pointer;\" align=\"absmiddle\" > <img '||CASE WHEN s.sifexecucaosisfor=TRUE THEN 'src=\"../imagens/valida1.gif\"  onmouseover=\"return escape(\'Curso será executado pelo SISFOR\');\" onclick=\"execucaoSisfor('||s.sifid||',\'FALSE\')\"' ELSE 'src=\"../imagens/valida3.gif\"  onmouseover=\"return escape(\'Curso não será executado pelo SISFOR\');\" onclick=\"execucaoSisfor('||s.sifid||',\'TRUE\')\"' END||' id=\"execucaoSisfor_'||s.sifid||'\" style=\"cursor:pointer;\" align=\"absmiddle\" ></span>'";
}

include_once APPRAIZ . "includes/library/simec/Grafico.php";

$grafico = new Grafico();

$sql = "
select foo.categoria, foo.descricao, count(*) as valor from (					
select
case when s.ieoid is not null then cor.coordsigla 
     when s.cnvid is not null then cor2.coordsigla 
     when s.ocuid is not null then cor3.coordsigla 
     when s.oatid is not null then cor4.coordsigla end as categoria,
e.esddsc as descricao
from sisfor.sisfor s
inner join workflow.documento d on d.docid = s.docidprojeto 
left join workflow.documento ectur on ectur.docid = s.docidcomposicaoequipe 
left join workflow.historicodocumento h on h.hstid = d.hstid
inner join workflow.estadodocumento e on e.esdid = d.esdid
inner join public.unidade u on u.unicod = s.unicod
left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
left join catalogocurso2014.curso cur on cur.curid = ieo.curid
left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
left join seguranca.usuario usu on usu.usucpf = s.usucpf
left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
left join sisfor.outraatividade oat on oat.oatid = s.oatid 
left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
where sifstatus='A' ".(($wh)?" AND ".implode(" AND ", $wh):"")."
) foo 
group by foo.categoria, foo.descricao 
";

echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';
$grafico->setTitulo('Situação dos cursos')->setTipo(Grafico::K_TIPO_COLUNA)->setWidth('100%')->gerarGrafico($sql);
echo '</td>';
echo '</tr>';
echo '</table>';

$sql = "select 
			{$acao} as acao,
			COALESCE(usu.usunome,'Não cadastrado'),
			uniabrev||' - '||unidsc as universidade, 
			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end as curso,
			e.esddsc,
			coalesce(ectur.esddsc,'Não iniciou') as esddsc_comporturma,
			s.sifprofmagisterio,
			(select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as valortotal,
			case when s.ieoid is not null then cor.coordsigla 
			     when s.cnvid is not null then cor2.coordsigla 
			     when s.ocuid is not null then cor3.coordsigla 
			     when s.oatid is not null then cor4.coordsigla end as secretaria,
			case when s.ieoid is not null then case when s.sifopcao='1' then 'Curso Proposto pelo MEC – Aceito' 
						   							when s.sifopcao='2' then 'Curso Proposto pelo MEC – Rejeitado'
						   							when s.sifopcao='3' then 'Curso Proposto pelo MEC – Repactuado'
						   							else 'não definido' end
			     when s.cnvid is not null then 'Curso Proposto pela IES – Do Catálogo' 
			     when s.ocuid is not null then 'Curso Proposto pela IES – Fora do Catálogo' 
			     when s.oatid is not null then 'Outras Atividades' end as tipo,
			to_char(htddata, 'dd/mm/YYYY HH24:MI') as htddata,
			case when sifexecucaosisfor=true then (select case when count(*)>0 then 'Sim' else 'Não' end as possuiperiodo from sisfor.folhapagamentocursista where sifid=s.sifid) else 'Não se aplica' end as periodo
		from sisfor.sisfor s
			inner join workflow.documento d on d.docid = s.docidprojeto
			left join workflow.historicodocumento h on h.hstid = d.hstid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join public.unidade u on u.unicod = s.unicod
			left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
			left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid  
			left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
			left join catalogocurso2014.curso cur on cur.curid = ieo.curid
			left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
			left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
			left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
			left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
			left join seguranca.usuario usu on usu.usucpf = s.usucpf
			left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
			left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
			left join sisfor.outraatividade oat on oat.oatid = s.oatid 
			left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
		where sifstatus='A' ".(($wh)?" AND ".implode(" AND ", $wh):"")." 
		order by unidsc";

$cabecalho = array("&nbsp;","Coordenador do curso","Universidade","Curso","Situação","Equipe","Vagas previstas (meta)","Valor do projeto","Secretaria","Tipo de curso","Data de tramitação","Possui período de referência?");

if($_REQUEST['visualizacao']=='xls') {

	ob_clean();
	
	$sql = str_replace(array($acao),array("s.sifid"),$sql);
	
	$arr = $db->carregar($sql);
	
	if($arr[0]) {
		$cabecalho[] = "Aluno/Custeio";
		$cabecalho[] = "Aluno/Bolsa";
		foreach($arr as $key => $ar) {
			unset($arr[$key]['sifid']);	
			$arr[$key]['alunocusteio'] = calcularCustoAlunoCusteio(array('sifid' => $ar['acao'],'retorna' => true));
			$arr[$key]['alunobolsa'] = calcularCustoAlunoBolsa(array('sifid' => $ar['acao'],'retorna' => true));
		}
	}
	
	header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
	header("Pragma: no-cache");
	header("Content-type: application/xls; name=SIMEC_Cursos_".date("Ymdhis").".xls");
	header("Content-Disposition: attachment; filename=SIMEC_Cursos_".date("Ymdhis").".xls");
	header("Content-Description: MID Gera excel");
	
	$db->monta_lista_tabulado($arr, $cabecalho, 10000000, 5, 'N', '100%', '');
	exit;

} else {

	$param['managerOrder'][8]['campo'] = 'htddata';
	$param['managerOrder'][8]['alias'] = 'htddata';


	$db->monta_lista($sql,$cabecalho,50,10,'N','center','','','','','',$param);
	
}

?>