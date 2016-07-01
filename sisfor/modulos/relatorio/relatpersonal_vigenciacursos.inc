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
select  u.unicod,
		u.uniabrev ||' - '|| unidsc as uninome,
		case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
		     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
		     when s.ocuid is not null then oc.ocunome 
		     when s.oatid is not null then oatnome end as curso,
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
        order by uninome";

$cabecalho = array("Cod. UO","IES","Curso","Início da vigência","Fim da vigência","Início - PE bolsistas","Fim - PE bolsistas","Início - PE cursistas","Fim - PE cursistas");

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

echo '<p align=center><input type="button" name="verxls" value="Ver XLS" onclick="window.location=\'sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_vigenciacursos.inc&verxls=1\';"></p>';


echo '</td>';
echo '</tr>';
echo '</table>';

 
?>