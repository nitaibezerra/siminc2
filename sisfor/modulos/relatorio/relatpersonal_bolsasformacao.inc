<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Informações sobre as bolsas vinculadas/pagas aos perfis de formação</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

 


echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "select 
			s.unicod,
			uniabrev||' - '||unidsc as universidade, 
			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end as curso,
			case when s.ieoid is not null then cor.coordsigla 
			     when s.cnvid is not null then cor2.coordsigla 
			     when s.ocuid is not null then cor3.coordsigla 
			     when s.oatid is not null then cor4.coordsigla end as secretaria,
			'F'||s.siftipoplanejamento as fase,
			(select coalesce(sum(t.tpeqtdbolsa),0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and t.pflcod=1105) as coordenadorgeralv,
			(select coalesce(sum(t.tpeqtdbolsa)*pp.plpvalor,0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd inner join sisfor.pagamentoperfil pp on pp.pflcod=t.pflcod where t.sifid=s.sifid and t.pflcod=1105 group by pp.plpvalor) as coordenadorgeralvr,
			(select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1105) as coordenadorgeralp,
			(select sum(pbovlrpagamento) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1105) as coordenadorgeralpr,
			(select epiqtd from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1105) as coordenadorgeralc,
			(select epivalor from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1105) as coordenadorgeralcr,

			(select coalesce(sum(t.tpeqtdbolsa),0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and t.pflcod=1195) as coordenadoradjuntov,
			(select coalesce(sum(t.tpeqtdbolsa)*pp.plpvalor,0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd inner join sisfor.pagamentoperfil pp on pp.pflcod=t.pflcod where t.sifid=s.sifid and t.pflcod=1195 group by pp.plpvalor) as coordenadoradjuntovr,
			(select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1195) as coordenadoradjuntop,
			(select sum(pbovlrpagamento) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1195) as coordenadoradjuntopr,
			(select epiqtd from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1195) as coordenadoradjuntoc,
			(select epivalor from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1195) as coordenadoradjuntocr,

			(select sum(t.tpeqtdbolsa) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and t.pflcod=1197) as supervisoriesv,
			(select coalesce(sum(t.tpeqtdbolsa)*pp.plpvalor,0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd inner join sisfor.pagamentoperfil pp on pp.pflcod=t.pflcod where t.sifid=s.sifid and t.pflcod=1197 group by pp.plpvalor) as supervisoriesvr,
			(select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1197) as supervisoriesp,
			(select sum(pbovlrpagamento) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1197) as supervisoriespr,
			(select epiqtd from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1197) as supervisoriesc,
			(select epivalor from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1197) as supervisoriescr,

			(select sum(t.tpeqtdbolsa) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and t.pflcod=1198) as formadorv,
			(select coalesce(sum(t.tpeqtdbolsa)*pp.plpvalor,0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd inner join sisfor.pagamentoperfil pp on pp.pflcod=t.pflcod where t.sifid=s.sifid and t.pflcod=1198 group by pp.plpvalor) as formadorvr,
			(select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1198) as formadorp,
			(select sum(pbovlrpagamento) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1198) as formadorpr,
			(select epiqtd from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1198) as formadorc,
			(select epivalor from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1198) as formadorcr,

			(select sum(t.tpeqtdbolsa) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and t.pflcod=1196) as pesquisadorv,
			(select coalesce(sum(t.tpeqtdbolsa)*pp.plpvalor,0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd inner join sisfor.pagamentoperfil pp on pp.pflcod=t.pflcod where t.sifid=s.sifid and t.pflcod=1196 group by pp.plpvalor) as pesquisadorvr,
			(select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1196) as pesquisadorp,
			(select sum(pbovlrpagamento) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1196) as pesquisadorpr,
			(select epiqtd from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1196) as pesquisadorc,
			(select epivalor from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1196) as pesquisadorcr,

			(select sum(t.tpeqtdbolsa) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd where t.sifid=s.sifid and t.pflcod=1199) as tutorv,
			(select coalesce(sum(t.tpeqtdbolsa)*pp.plpvalor,0) from sisfor.identificacaousuario i inner join sisfor.tipoperfil t on t.iusd = i.iusd inner join sisfor.pagamentoperfil pp on pp.pflcod=t.pflcod where t.sifid=s.sifid and t.pflcod=1199 group by pp.plpvalor) as tutorvr,
			(select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1199) as tutorp,
			(select sum(pbovlrpagamento) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid=p.docid and d.esdid!=1254 inner join sisfor.tipoperfil t on t.tpeid = p.tpeid where t.sifid=s.sifid and t.pflcod=1199) as tutorpr,
			(select epiqtd from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1199) as tutorc,
			(select epivalor from sisfor.equipeies e where e.sifid=s.sifid and e.pflcod=1199) as tutorcr

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
		where sifstatus='A'  AND d.esdid='1187' ORDER BY 2,3";

$cabecalho = array("U.O.","IES","Curso","Secretaria","Fase","Coordenador Geral(vinculadas)","Coordenador Geral(vinculadas R$)","Coordenador Geral(pagas)","Coordenador Geral(pagas R$)","Coordenador Geral(pactuadas)","Coordenador Geral(pactuadas R$)","Coordenador Adjunto(vinculadas)","Coordenador Adjunto(vinculadas R$)","Coordenador Adjunto(pagas)","Coordenador Adjunto(pagas R$)","Coordenador Adjunto(pactuadas)","Coordenador Adjunto(pactuadas R$)","Supervisor IES(vinculadas)","Supervisor IES(vinculadas R$)","Supervisor IES(pagas)","Supervisor IES(pagas R$)","Supervisor IES(pactuadas)","Supervisor IES(pactuadas R$)","Formador IES(vinculadas)","Formador IES(vinculadas R$)","Formador IES(pagas)","Formador IES(pagas R$)","Formador IES(pactuadas)","Formador IES(pactuadas R$)","Pesquisador (vinculadas)","Pesquisador (vinculadas R$)","Pesquisador (pagas)","Pesquisador (pagas R$)","Pesquisador(pactuadas)","Pesquisador(pactuadas R$)","Tutor(vinculadas)","Tutor(vinculadas R$)","Tutor(pagas)","Tutor(pagas R$)","Tutor(pactuadas)","Tutor(pactuadas R$)");

if($_REQUEST['verxls']) {

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_xls_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_xls_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000000,5,'N','100%','');
	exit;
	
} else {

	$db->monta_lista_simples($sql,$cabecalho,100000000,5,5,'N','100%','N');

}

echo '<p align=center><input type="button" name="verxls" value="Ver XLS" onclick="window.location=\'sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_bolsasformacao.inc&verxls=1\';"></p>';



echo '</td>';
echo '</tr>';
echo '</table>';

 
?>