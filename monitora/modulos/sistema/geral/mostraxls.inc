<?
/*
Sistema Simec
Setor responsavel: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier
Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
Módulo:mostra.inc
Finalidade: este arquivo permite exportar o plano gerencial para xls
*/

header( 'Content-Type: application/vnd.ms-excel' );
header( 'Expires: 0' );
header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' ); 
header( 'Content-Disposition: attachment; filename="relatorio.xls"' );

 $pjeid = $_SESSION['pjeid'];
$ptoid =  $_REQUEST['cod'];
$nivel = $_REQUEST['nivel'];
if (! $nivel) $nivel=1000;
 if (! $_REQUEST['cod'] )
 /*
$sql= "select p.ptoordem as Ordem,case when p.ptotipo='P' then 'E' else 'M' end as Tipo, p.ptodsc as Titulo,p.ptodescricao as Descricao,to_char(p.ptodata_ini,'dd/mm/YYYY') as Inicio,to_char(p.ptodata_fim,'dd/mm/YYYY') as Termino  from monitora.planotrabalho p where pjeid=$pjeid and p.ptostatus='A' order by p.ptoordem";
else
$sql= "select p.ptoordem as Ordem,case when p.ptotipo='P' then 'E' else 'M' end as Tipo, p.ptodsc as Titulo,p.ptodescricao as Descricao,to_char(p.ptodata_ini,'dd/mm/YYYY') as Inicio,to_char(p.ptodata_fim,'dd/mm/YYYY') as Termino  from monitora.planotrabalho p where p.pjeid=$pjeid and p.ptostatus='A' and p.ptoid in (select ptoid from  SELECT_ALL_CHILDS_FROM_PTO( $ptoid ))  order by p.ptoordem";
*/

$sql="select p.ptoordem as ID, 
p.ptodsc as Nome,(p.ptodata_fim - ptodata_ini)  
as duracao,to_char(p.ptodata_ini,'dd/mm/YYYY') as Inicio,to_char(p.ptodata_fim,'dd/mm/YYYY') as Termino, 
ptoordem_antecessor as predecessoras,ptonivel as Nivel ,usucpf as Nomes_de_recursos,ptodescricao as anotacoes  
from monitora.planotrabalho p where pjeid=".$_SESSION['pjeid']." and p.ptostatus='A' and ptonivel <= $nivel order by p.ptoordem";
else 
"select p.ptoordem as ID, 
p.ptodsc as Nome,(p.ptodata_fim - ptodata_ini)
as duracao,to_char(p.ptodata_ini,'dd/mm/YYYY') as Inicio,to_char(p.ptodata_fim,'dd/mm/YYYY') as Termino, 
ptoordem_antecessor as predecessoras,ptonivel as Nivel ,usucpf as Nomes_de_recursos, ptodescricao as anotacoes  
from monitora.planotrabalho p where pjeid=".$_SESSION['pjeid']." and p where p.pjeid=$pjeid and p.ptostatus='A' and p.ptoid in (select ptoid from  SELECT_ALL_CHILDS_FROM_PTO( $ptoid ))  and ptonivel <= $nivel order by p.ptoordem";


$db->sql_to_excel( $sql, 'relatorio', '' );
exit();

?>