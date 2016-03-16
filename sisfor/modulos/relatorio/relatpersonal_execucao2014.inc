<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Execução 2014</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

 


echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "
select 
			u.unicod,
			u.uniabrev||' - '||u.unidsc as universidade,
			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end as curso,
			case when s.ieoid is not null then cor.coordsigla 
			     when s.cnvid is not null then cor2.coordsigla 
			     when s.ocuid is not null then cor3.coordsigla 
			     when s.oatid is not null then cor4.coordsigla end as secretaria,
			(select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as valortotal,
			case when sifvalorempenhado > 0 then sifvalorempenhado else 0 end as empenhadoscurso,
			(select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A')-(case when sifvalorempenhado > 0 then sifvalorempenhado else 0 end) as saldonaoempenhado,
			case when planejamentofinalizado2014=true then 'Sim' else 'Não' end as finalizado
		from sisfor.sisfor s
			inner join workflow.documento d on d.docid = s.docidprojeto
			left join workflow.historicodocumento h on h.hstid = d.hstid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join public.unidade u on u.unicod = s.unicod
			left join sisfor.execucao2014 ex on ex.unicod = s.unicod 
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
		where sifstatus='A' and d.esdid=1187 
		order by u.unidsc";

$cabecalho = array("Cod. UO","IES","Curso","Secretaria","Valor total","Valor empenhado","Saldo","Finalizado?");

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

echo '<p align=center><input type="button" name="verxls" value="Ver XLS" onclick="window.location=\'sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_execucao2014.inc&verxls=1\';"></p>';


echo '</td>';
echo '</tr>';
echo '</table>';

 
?>