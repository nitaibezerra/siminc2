<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Andamento de cursos</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

 


echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "select foo.unicod,
			   foo.uninome,
			   foo.usunome,
			   foo.curso,
			   foo.secretaria,
			   foo.fase,
			   foo.tipocertificacao,
			   coalesce(foo.sifprofmagisterio,0) as sifprofmagisterio,
			   foo.qtdinserido, 
			   foo.qtdemcurso,
			   foo.qtdaprovados,
			   foo.qtdreprovados,
			   foo.qtdevadidos,
			   foo.qtdfalecidos,
			   foo.orcvlrunitario,
			   coalesce(foo.epivalor,0) as epivalor,
			   coalesce(foo.bolsaspagas,0) as bolsaspagas,
			   (coalesce(foo.epivalor,0)-coalesce(foo.valorvinculado,0)) as valornaovinculado,
			   case when foo.sifprofmagisterio> 0 then (coalesce(foo.orcvlrunitario,0)::numeric/foo.sifprofmagisterio::numeric)::text else 'Não possui meta' end as custoalunocusteio,
			   case when foo.sifprofmagisterio> 0 then (coalesce(foo.epivalor,0)::numeric/foo.sifprofmagisterio::numeric)::text else 'Não possui meta' end as custoalunobolsas,
			   case when foo.sifprofmagisterio> 0 then ((coalesce(foo.orcvlrunitario,0)+coalesce(foo.epivalor,0))::numeric/foo.sifprofmagisterio::numeric)::text else 'Não possui meta' end as custoalunototalprevisto,
			   case when foo.qtdaprovados> 0 then ((coalesce(foo.orcvalorexecutado,0)+coalesce(foo.bolsaspagas,0))::numeric/foo.qtdaprovados::numeric)::text else 'Não possui aprovados' end as custoalunototalfinal,
			   foo.situacaoav,
			   foo.sifvigenciadtini,
			   foo.sifvigenciadtfim,
 			   foo.refbolsistaini,
			   foo.refbolsistafim,
			   foo.refcursistaini,
			   foo.refcursistafim
		
		from (
		
		select  u.unicod,
				u.uniabrev ||' - '|| unidsc as uninome,
				usu.usunome,
				case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
				     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
				     when s.ocuid is not null then oc.ocunome 
				     when s.oatid is not null then oatnome end as curso,
				
				case when s.ieoid is not null then cor.coordsigla 
				     when s.cnvid is not null then cor2.coordsigla 
				     when s.ocuid is not null then cor3.coordsigla 
				     when s.oatid is not null then cor4.coordsigla end as secretaria,
				
				case when s.siftipoplanejamento='1' then 'Fase 1 - 2014'
					 when s.siftipoplanejamento='2' then 'Fase 2 - 2014'
					 when s.siftipoplanejamento='3' then '2015'
					 else s.siftipoplanejamento::text end as fase,
				
				nc.ncuid||' - '||nc.ncudesc as tipocertificacao,
				s.sifprofmagisterio,
				(select count(distinct c.curid) from sisfor.cursistacurso c inner join sisfor.cursistaavaliacoes ca on ca.cucid = c.cucid where c.sifid=s.sifid) as qtdinserido, 
				(select count(distinct c.curid) from sisfor.cursistacurso c inner join sisfor.cursistaavaliacoes ca on ca.cucid = c.cucid where c.sifid=s.sifid and cavsituacao in('c','a','b','d') and c.fpbid in(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)) as qtdemcurso,
				(select count(distinct c.curid) from sisfor.cursistacurso c inner join sisfor.cursistaavaliacoes ca on ca.cucid = c.cucid where c.sifid=s.sifid and cavsituacao='p' and c.fpbid in(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)) as qtdaprovados,
				(select count(distinct c.curid) from sisfor.cursistacurso c inner join sisfor.cursistaavaliacoes ca on ca.cucid = c.cucid where c.sifid=s.sifid and cavsituacao='r' and c.fpbid in(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)) as qtdreprovados,
				(select count(distinct c.curid) from sisfor.cursistacurso c inner join sisfor.cursistaavaliacoes ca on ca.cucid = c.cucid where c.sifid=s.sifid and cavsituacao='e' and c.fpbid in(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)) as qtdevadidos,
				(select count(distinct c.curid) from sisfor.cursistacurso c inner join sisfor.cursistaavaliacoes ca on ca.cucid = c.cucid where c.sifid=s.sifid and cavsituacao='f' and c.fpbid in(

							SELECT f.fpbid FROM sisfor.folhapagamentocursista f 
							INNER JOIN sisfor.folhapagamento fp ON fp.fpbid = f.fpbid 
							WHERE f.sifid=c.sifid AND to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd') 
							ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date DESC LIMIT 1
							
							)) as qtdfalecidos,
				
				(select sum(orcvlrunitario) from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as orcvlrunitario,
				(select sum(orcvlrexecutado) from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as orcvalorexecutado,
				(select sum(epivalor) from sisfor.equipeies where sifid=s.sifid) as epivalor,
				(select round(sum(pbovlrpagamento),2) as valor from sisfor.pagamentobolsista pp inner join sisfor.tipoperfil tt on tt.tpeid = pp.tpeid inner join workflow.documento d on d.docid = pp.docid where tt.sifid=s.sifid and d.esdid=".ESD_PAGAMENTO_EFETIVADO.") as bolsaspagas,
				(select sum((tpeqtdbolsa*plpvalor))  from sisfor.tipoperfil t inner join sisfor.pagamentoperfil p on p.pflcod = t.pflcod where sifid=s.sifid) as valorvinculado,
				coalesce(eav.esddsc,'Não iniciou') as situacaoav,
				
				to_char(s.sifvigenciadtini,'dd/mm/YYYY') as sifvigenciadtini,
				to_char(s.sifvigenciadtfim,'dd/mm/YYYY') as sifvigenciadtfim,
				
				to_char((SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini
		         FROM sisfor.folhapagamentoprojeto fu
		         INNER JOIN sisfor.folhapagamento f ON f.fpbid = fu.fpbid
		         WHERE fu.sifid=s.sifid),'mm/YYYY') as refbolsistaini,
				
				to_char((SELECT MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim
		         FROM sisfor.folhapagamentoprojeto fu
		         INNER JOIN sisfor.folhapagamento f ON f.fpbid = fu.fpbid
		         WHERE fu.sifid=s.sifid),'mm/YYYY') as refbolsistafim,
				
				to_char((SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini
		         FROM sisfor.folhapagamentocursista fu
		         INNER JOIN sisfor.folhapagamento f ON f.fpbid = fu.fpbid
		         WHERE fu.sifid=s.sifid),'mm/YYYY') as refcursistaini,
				
				to_char((SELECT MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim
		         FROM sisfor.folhapagamentocursista fu
		         INNER JOIN sisfor.folhapagamento f ON f.fpbid = fu.fpbid
		         WHERE fu.sifid=s.sifid),'mm/YYYY') as refcursistafim
		
		
 
        from sisfor.sisfor s
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod 
		inner join workflow.documento doc on doc.docid = s.docidprojeto and doc.esdid = '" . ESD_PROJETO_VALIDADO . "' 
		left join sisfor.avaliacaofinal av on av.sifid = s.sifid 
		left join workflow.documento dav on dav.docid = av.docid 
		left join workflow.estadodocumento eav on eav.esdid = dav.esdid 
		left join catalogocurso2014.nivelcurso nc on nc.ncuid = s.siftipocertificacao::integer
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
        order by uninome
		
		) foo";

$cabecalho = array("Cod. UO","IES","Coordenador do curso","Curso","Secretaria","Fase","Tipo de certificação","Meta de cursistas",
				   "Cursistas inseridos", "Cursistas matriculados, cursando, desvinculado e trancado", "Aprovados", "Reprovados", "Evadidos", "Falecidos",
				   "Valor do custeio", "Valor Bolsas previstas", "Valor Bolsas pagas", "Valor de bolsas não vinculadas", 
				   "Custo Aluno Custeio Previsto",	"Custo Aluno Bolsa Previsto", "Custo Aluno Total Previsto", "Custo Aluno Total Final", "Relatório final",
				   "Início da vigência", "Fim da vigência", "Início - PE bolsistas", 
				   "Fim - PE bolsistas","Início - PE cursistas","Fim - PE cursistas");

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

echo '<p align=center><input type="button" name="verxls" value="Ver XLS" onclick="window.location=\'sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_andamentocursos.inc&verxls=1\';"></p>';


echo '</td>';
echo '</tr>';
echo '</table>';

 
?>