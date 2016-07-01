<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 

<script>

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


</script>

<div id="modalDv" style="display:none;"></div>

<?

if($_REQUEST['vertodos'] || $_REQUEST['salvar']) unset($_SESSION['gravarfiltro'][$_SESSION['mnuid']]);

if($_SESSION['gravarfiltro'][$_SESSION['mnuid']]['esdid']) $_REQUEST['esdid'] = $_SESSION['gravarfiltro'][$_SESSION['mnuid']]['esdid'];
if($_SESSION['gravarfiltro'][$_SESSION['mnuid']]['coordid']) $_REQUEST['coordid'] = $_SESSION['gravarfiltro'][$_SESSION['mnuid']]['coordid'];
if($_SESSION['gravarfiltro'][$_SESSION['mnuid']]['unicod']) $_REQUEST['unicod'] = $_SESSION['gravarfiltro'][$_SESSION['mnuid']]['unicod'];
if($_SESSION['gravarfiltro'][$_SESSION['mnuid']]['coordenadorcurso']) $_REQUEST['coordenadorcurso'] = $_SESSION['gravarfiltro'][$_SESSION['mnuid']]['coordenadorcurso'];
if($_SESSION['gravarfiltro'][$_SESSION['mnuid']]['fpbid']) $_REQUEST['fpbid'] = $_SESSION['gravarfiltro'][$_SESSION['mnuid']]['fpbid'];
if($_SESSION['gravarfiltro'][$_SESSION['mnuid']]['nomecurso']) $_REQUEST['nomecurso'] = $_SESSION['gravarfiltro'][$_SESSION['mnuid']]['nomecurso'];
?>


<form method=post name="filtroscursos" id="filtroscursos">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Avaliações</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td>Para analisar a solicitação de pagamento de bolsas, clique no ícone <img src="../imagens/alterar.gif">  ou utilize os filtros para localizar o curso desejado.</td>
</tr>

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
	<td class="SubTituloDireita" width="20%">IES</td>
	<td>
	<?

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
	<td class="SubTituloDireita" width="20%">Curso</td>
	<td>
	<? 
	$nomecurso = $_REQUEST['nomecurso'];
	echo campo_texto('nomecurso', 'N', 'S', 'Nome do curso', '50', '', '', '','','');
	?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">Período</td>
	<td>
	<?
	$sql = "SELECT f.fpbid as codigo, m.mesdsc || ' / ' || fpbanoreferencia as descricao
			FROM sisfor.folhapagamento f
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE to_char(NOW(),'YYYY-mm-dd')::date>=(fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-01')::date ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-01')::date";
	
	$db->monta_combo('fpbid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbid','', $_REQUEST['fpbid']);
	
	?></td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">Situação</td>
	<td>
	<?
	$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".WF_TPDID_SISFOR_AVALIACAO."' ORDER BY esdid";
	$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'esdid', '', $_REQUEST['esdid']);
	?>
	</td>
</tr>

        <tr>
            <td align="center" bgcolor="#CCCCCC" colspan="2">
                <input type="submit" class="botao_enviar"  name="salvar" value="Pesquisar"> 
                <input type="button" class="botao_enviar"  name="vertodos" value="Ver todos" onclick="window.location='sisfor.php?modulo=principal/mec/mec&acao=A&aba=analise&vertodos=1';">
                <input type="submit" class="botao_enviar"  name="gerarxls" value="Gerar XLS">
            </td>
        </tr>
</table>
</form>
<?php 


if($_REQUEST['esdid']) {
	$_SESSION['gravarfiltro'][$_SESSION['mnuid']]['esdid'] = $_REQUEST['esdid']; 
	$wh[] = "doc.esdid='".$_REQUEST['esdid']."'";
}

if($_REQUEST['coordid']) {
	$_SESSION['gravarfiltro'][$_SESSION['mnuid']]['coordid'] = $_REQUEST['coordid'];
	$wh[] = "(cor.coordid='".$_REQUEST['coordid']."' OR cor2.coordid='".$_REQUEST['coordid']."' OR cor3.coordid='".$_REQUEST['coordid']."')";
}

if($_REQUEST['unicod']) {
	$_SESSION['gravarfiltro'][$_SESSION['mnuid']]['unicod'] = $_REQUEST['unicod'];
	$wh[] = "s.unicod='".$_REQUEST['unicod']."'";
}

if($_REQUEST['coordenadorcurso']) {
	$_SESSION['gravarfiltro'][$_SESSION['mnuid']]['coordenadorcurso'] = $_REQUEST['coordenadorcurso'];
	$wh[] = "usu.usunome ilike '%".$_REQUEST['coordenadorcurso']."%'";
}

if($_REQUEST['fpbid']) {
	$_SESSION['gravarfiltro'][$_SESSION['mnuid']]['fpbid'] = $_REQUEST['fpbid'];
	$wh[] = "f.fpbid='".$_REQUEST['fpbid']."'";
}

if($_REQUEST['nomecurso']) {
	$_SESSION['gravarfiltro'][$_SESSION['mnuid']]['nomecurso'] = $_REQUEST['nomecurso'];
	$wh[] = "(cur.curid||' '||cur.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR cur2.curid||' '||cur2.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR oc.ocunome ilike '%".$_REQUEST['nomecurso']."%' OR oat.oatnome ilike '%".$_REQUEST['nomecurso']."%')";
}


$perfis = pegaPerfilGeral();

if(in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis)) {
	
	$coordids = $db->carregarColuna("SELECT DISTINCT coordid FROM sisfor.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod IN('".PFL_EQUIPE_MEC."','".PFL_COORDENADOR_MEC."','".PFL_DIRETOR_MEC."') ANd rpustatus='A'");
	
	if($coordids) 
		$wh[] = "(cur.coordid IN(".implode(",",$coordids).") OR cur2.coordid IN(".implode(",",$coordids).") OR cor3.coordid IN(".implode(",",$coordids).") OR cor4.coordid IN(".implode(",",$coordids)."))";

}

if(in_array(PFL_COORDENADOR_INST,$perfis)) {

	$wh[] = "u.unicod in (select si.unicod from sisfor.sisfories si 
						inner join sisfor.tipoperfil tp on tp.tpeid = si.tpeid 
						inner join sisfor.identificacaousuario i on i.iusd = tp.iusd 
						where i.iuscpf='".$_SESSION['usucpf']."')";
	
}

$sql = "select '<span style=\"white-space: nowrap;\"><a href=\"sisfor.php?modulo={$_REQUEST['modulo']}&acao={$_REQUEST['acao']}&aba=analisedetalhada&sifid='||s.sifid||'&fpbid='||f.fpbid||'\"><img src=\"../imagens/alterar.gif\"></a> <img style=\"cursor: pointer;\" src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( '||doc.docid||' );\"/></span>' as acao,
				'<span style=font-size:x-small>'||u.uniabrev ||' - '|| unidsc||'</span>' as uninome,
            	'<span style=font-size:x-small>'||case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end||'</span>' as curdesc, 
			    '<span style=font-size:x-small>'||iusnome||'</span>' as iusnome, 
                '<span style=font-size:x-small>'||to_char((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date,'mm/YYYY')||'</span>' as mesanoini,
			  	'<span style=font-size:x-small>'||esd.esddsc||'</span>' as esddsc,
				(select count(*) as total from sisfor.mensario m 
				 inner join sisfor.tipoperfil t on t.tpeid = m.tpeid 
				 inner join sisfor.mensarioavaliacoes ma on ma.menid = m.menid 
				 where t.sifid = s.sifid and m.fpbid = f.fpbid and mavatividadesrealizadas='A') as qtdbolsasaptas, 
				'<span style=font-size:x-small>'||case when s.ieoid is not null then cor.coordsigla 
			     when s.cnvid is not null then cor2.coordsigla 
			     when s.ocuid is not null then cor3.coordsigla 
			     when s.oatid is not null then cor4.coordsigla end||'</span>' as secretaria,
				'<span style=font-size:x-small>'||to_char(hst.htddata, 'dd/mm/YYYY HH24:MI')||'</span>' as hstdata
        from sisfor.sisfor s 
        		inner join workflow.documento dprojeto on dprojeto.docid = s.docidprojeto and dprojeto.esdid=".ESD_PROJETO_VALIDADO."
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod 
                left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
                left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
                left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
                left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A' 
                left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid 
                left join seguranca.usuario usu on usu.usucpf = s.usucpf 
                left join sisfor.outrocurso oc on oc.ocuid = s.ocuid 
                left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
				left join sisfor.outraatividade oat on oat.oatid = s.oatid 
				left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
                inner join sisfor.folhapagamentoprojeto fpp on s.sifid = fpp.sifid
                INNER JOIN sisfor.folhapagamento f ON f.fpbid = fpp.fpbid
                inner join workflow.documento doc on doc.docid = fpp.docid 
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid 
				left join workflow.historicodocumento hst on hst.hstid = doc.hstid 
				left join seguranca.usuario uu on uu.usucpf = hst.usucpf 
                INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf
            where doc.esdid in( ".ESD_ANALISE_MEC.", ".ESD_ENVIADO_PAGAMENTO.",".ESD_ANALISE_COORDENADORINSTITUCIONAL.",".ESD_AVALIACAO.") ".(($wh)?"AND ".implode(" AND ", $wh):"")." 
			order by uninome, curdesc, (fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date";


include_once APPRAIZ . "includes/library/simec/Grafico.php";

$grafico = new Grafico();

$sql2 = "select
			foo.esddsc as descricao,
			foo.secretaria as categoria,
			count(*) as valor
			from (
			
			{$sql}
			
			) foo group by foo.esddsc, foo.secretaria";

echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
echo '<tr>';
echo '<td>';
$grafico->setTitulo('Situação das avaliações')->setTipo(Grafico::K_TIPO_COLUNA)->setWidth('100%')->gerarGrafico($sql2);
echo '</td>';
echo '</tr>';
echo '</table>';




if($_REQUEST['gerarxls']) {

	ob_clean();
	
	$arr = $db->carregar($sql);
	
	if($arr[0]) {
		foreach($arr as $key => $ar) {
			unset($arr[$key]['acao']);
		}
	}
	
	header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
	header("Pragma: no-cache");
	header("Content-type: application/xls; name=SIMEC_Cursos_".date("Ymdhis").".xls");
	header("Content-Disposition: attachment; filename=SIMEC_Analise_avaliacoes_".date("Ymdhis").".xls");
	header("Content-Description: MID Gera excel");
	
	$cabecalho = array("Universidade","Curso","Coordenador do curso","Período","Situação","Qtd. Bolsas Aptas","Secretaria","Última tramitação");
	$db->monta_lista_tabulado($arr, $cabecalho, 10000000, 5, 'N', '100%', '');
	exit;

} else {

	$cabecalho = array("&nbsp;","Universidade","Curso","Coordenador do curso","Período","Situação","Qtd. Bolsas Aptas","Secretaria","Última tramitação");
	$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));

}

 ?>