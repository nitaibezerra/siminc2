<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

//include_once APPRAIZ . 'includes/workflow.php';
$sql = "
select
d.dimcod as cod,
d.dimdsc as dsc,
p.prgcod as prgcod,
p.prgdsc as prgdsc,
m.estuf as uf, 
m.muncodcapital as IBGE, 
m.estdescricao as estdsc, 
r.rspdsc as rspdsc,
to_char(r.rspdata , 'DD/MM/YYYY') as rspdata
from
cte.dimensao d
inner join cte.pergunta p ON p.dimid = d.dimid
inner join cte.resposta r ON r.prgid = p.prgid
inner join cte.instrumentounidade iu ON iu.inuid = r.inuid
inner join territorios.estado m ON m.estuf = iu.estuf
where d.itrid = 1 and p.prgstatus = 'A' and m.estuf in ('RR', 'SC', 'RJ', 'DF')
order by 	m.estuf, m.estdescricao, 
		d.dimcod ,
		p.prgcod 	
";

if($_REQUEST['tipo'] == 's'){

	$sql = "
select
d.dimcod as cod,
d.dimdsc as dsc,
p.prgcod as prgcod,
p.prgdsc as prgdsc,
m.estuf as uf, 
m.muncodcapital as ibge, 
m.estdescricao as estdsc, 
r.rspdsc as rspdsc,
to_char(r.rspdata , 'DD/MM/YYYY') as rspdata
from
cte.dimensao d
inner join cte.pergunta p ON p.dimid = d.dimid
inner join cte.resposta r ON r.prgid = p.prgid
inner join cte.instrumentounidade iu ON iu.inuid = r.inuid
inner join territorios.estado m ON m.estuf = iu.estuf
where d.itrid = 1 and p.prgstatus = 'A' and m.estuf not in ('RR', 'SC', 'RJ', 'DF')
order by 	m.estuf, m.estdescricao, 
		d.dimcod ,
		p.prgcod 	
";


}

$db = new cls_banco();
$dados = $db->carregar( $sql );


$saida = "<table border=1>";

foreach ($dados as $row) {
	$saida .= "<tr>";

	$saida .= "<td>" . $row['cod'] . "</td>";
	$saida .= "<td>" . $row['dsc'] . "</td>";
	$saida .= "<td>" . $row['prgcod'] . "</td>";
	$saida .= "<td>" . $row['prgdsc'] . "</td>";
	$saida .= "<td>" . $row['uf'] . "</td>";
	$saida .= "<td>" . $row['ibge'] . "</td>";
	$saida .= "<td>" . $row['estdsc'] . "</td>";
	$saida .= "<td>" . $row['rspdsc'] . "</td>";
	$saida .= "<td>" . $row['rspdata'] . "</td>";
	$saida .= "</tr>";

}
$saida .= "</table>";

echo $saida;

?>