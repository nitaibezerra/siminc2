<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Lista de Cursistas</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

?>
<form method="post" id="formulario" action="sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_cursistas.inc">
<input type="hidden" name="filtros" id="filtros" value="1">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Ver HTML"><input type="button" value="Ver XLS" onclick="jQuery('#filtros').val('2');jQuery('#formulario').submit();"></td>
</tr>
</table>
</form>



<? 
if($_REQUEST['filtros']) : 

echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "select distinct 
u.unicod as codigoies, 
u.uniabrev as siglaies, 
u.unidsc as ies, 
case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
     when s.ocuid is not null then oc.ocunome 
     when s.oatid is not null then oatnome end as curso,
case when s.ieoid is not null then cor.coordsigla 
     when s.cnvid is not null then cor2.coordsigla 
     when s.ocuid is not null then cor3.coordsigla 
     when s.oatid is not null then cor4.coordsigla end as secretaria,
curcpf, 
curnome, 
curemail,
m.estuf,
m.mundescricao 
from sisfor.cursista c 
inner join sisfor.cursistacurso cc on cc.curid = c.curid 
inner join sisfor.sisfor s on s.sifid = cc.sifid 
inner join public.unidade u on u.unicod = s.unicod 
left join territorios.municipio m on m.muncod = cc.muncod 
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
order by 2,4, 7";

$cabecalho = array("Código IES","Sigla IES","IES","Curso","Secretaria","CPF","Nome","E-mail","UF","Município");

if($_REQUEST['filtros']=='2') {

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_cursistas_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000000,5,'N','100%','');
	exit;

} else {
	$db->monta_lista_simples($sql,$cabecalho,100000000,5,5,'N','100%','N');
}

echo '</td>';
echo '</tr>';
echo '</table>';


endif; 
?>