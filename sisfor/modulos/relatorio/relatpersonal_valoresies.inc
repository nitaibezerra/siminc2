<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Valores IES</b></p>
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
foo.unicod, foo.unidsc, foo.uniabrev, foo.valorloaplanejamento, foo.loa2014, foo.empenhadas as empenhadassiafi, foo.totalaprovado, foo.valorempenhado, (foo.totalaprovado-foo.valorempenhado) as saldo
from (

select 
u.unicod, 
u.unidsc, 
u.uniabrev, 
e.loa2014, 
e.empenhadas,
(
select sum(vppvalor) from sisfor.valorprevistoploa vpl where unicod=s.unicod) as valorloaplanejamento,
(
select sum(orcvlrunitario) as orcvlrunitario 
from sisfor.orcamento o 
inner join sisfor.sisfor ss on ss.sifid = o.sifid
inner join workflow.documento d on d.docid = ss.docidprojeto and d.esdid=1187
where s.unicod=ss.unicod and orcstatus='A' and ss.sifstatus='A') as totalaprovado,
(
select sum(sifvalorempenhado) as sifvalorempenhado 
from sisfor.sisfor ss 
inner join workflow.documento d on d.docid = ss.docidprojeto and d.esdid=1187
where s.unicod=ss.unicod and ss.sifstatus='A') as valorempenhado
--, s.* 

from sisfor.sisfories s 
inner join public.unidade u on u.unicod = s.unicod 
left join sisfor.execucao2014 e on e.unicod = u.unicod

) foo";

$cabecalho = array("Cod. UO","IES","Sigla","Valor LOA planejamento","LOA 2014(importação)","Valor empenhado SIAFI(importação)","Valor total Aprovado","Valor empenhado(informado pela IES)","Saldo[Valor total Aprovado - Valor empenhado(informado pela IES)]");

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

echo '<p align=center><input type="button" name="verxls" value="Ver XLS" onclick="window.location=\'sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_valoresies.inc&verxls=1\';"></p>';


echo '</td>';
echo '</tr>';
echo '</table>';

 
?>