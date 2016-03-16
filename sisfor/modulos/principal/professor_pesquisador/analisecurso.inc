<?php

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

</script>

<div id="modalDiv" style="display: none;"></div>

<form id="formulario" method="post" name="formulario" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="pesquisarUniversidade" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita">Secretaria:</td>
			<td>
			<?php 
			$sql = "SELECT coordid as codigo, coordsigla as descricao FROM catalogocurso2014.coordenacao ORDER BY coordid";
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
			<td class="subtituloDireita">Universidade:</td>
			<td>
			<?php
			
			$sql = "SELECT 	DISTINCT ent.entunicod as codigo, COALESCE(ent.entsig||' - ','')||ent.entnome as descricao
					FROM 		entidade.entidade ent
					INNER JOIN 	entidade.funcaoentidade fun ON fun.entid = ent.entid AND funid IN (12,11) 
					WHERE ent.entstatus='A' AND ent.entunicod IS NOT NULL";
			
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
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisarCursos();"/>
				<input type="button" value="Ver todos" id="btnTodos" onclick="window.location='sisfor.php?modulo=principal/coordenador_curso/analisecurso&acao=A';"/>
			</td>
		</tr>
	</table>
</form>

<?

$perfis = pegaPerfilGeral();

if(in_array(PFL_EQUIPE_MEC,$perfis)) {
	$coordids = $db->carregarColuna("SELECT coordid FROM sisfor.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PFL_EQUIPE_MEC."' ANd rpustatus='A'");
	
	if($coordids) 
		$wh[] = "(cur.coordid IN(".implode(",",$coordids).") OR cur2.coordid IN(".implode(",",$coordids).") OR oc.coordid IN(".implode(",",$coordids)."))";

}

if($_REQUEST['esdid']) {
	$wh[] = "d.esdid='".$_REQUEST['esdid']."'";
}

if($_REQUEST['coordid']) {
	$wh[] = "(cor.coordid='".$_REQUEST['coordid']."' OR cor2.coordid='".$_REQUEST['coordid']."' OR cor3.coordid='".$_REQUEST['coordid']."')";
}

if($_REQUEST['unicod']) {
	$wh[] = "u.unicod='".$_REQUEST['unicod']."'";
}

if($_REQUEST['coordenadorcurso']) {
	$wh[] = "usu.usunome ilike '%".$_REQUEST['coordenadorcurso']."%'";
}

if($_REQUEST['nomecurso']) {
	$wh[] = "(cur.curid||' '||cur.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR cur2.curid||' '||cur2.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR oc.ocudesc ilike '%".$_REQUEST['nomecurso']."%')";
}

$sql = "select 
			'<img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"verProjetoCurso('||s.sifid||');\">' as acao,
			COALESCE(usu.usunome,'Não cadastrado'),
			uniabrev||' - '||unidsc as universidade, 
			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome end as curso,
			e.esddsc,
			case when s.ieoid is not null then cor.coordsigla 
			     when s.cnvid is not null then cor2.coordsigla 
			     when s.ocuid is not null then cor3.coordsigla end as secretaria,
			case when s.ieoid is not null then case when s.sifopcao='1' then 'Curso Proposto pelo MEC – Aceito' 
						   							when s.sifopcao='2' then 'Curso Proposto pelo MEC – Rejeitado'
						   							when s.sifopcao='3' then 'Curso Proposto pelo MEC – Repactuado'
						   							else 'não definido' end
			     when s.cnvid is not null then 'Curso Proposto pela IES – Do Catálogo' 
			     when s.ocuid is not null then 'Curso Proposto pela IES – Fora do Catálogo' end as tipo,
			to_char(htddata, 'dd/mm/YYYY HH24:MI') as htddata
		from sisfor.sisfor s 
		inner join workflow.documento d on d.docid = s.docidprojeto 
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
		where sifstatus='A' ".(($wh)?" AND ".implode(" AND ", $wh):"")." 
		order by unidsc";

$cabecalho = array("&nbsp;","Coordenador do curso","Universidade","Curso","Situação","Secretaria","Tipo de curso","Data de tramitação");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

?>