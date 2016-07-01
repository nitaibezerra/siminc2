<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_fase3.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 3
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

print  'dadofisico<br>';
$sql="delete from elabrev.dadofisico where prsano='".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$adodbXML->ImportaXML(&$clientedb, "CargaPPADadoFisico.xml", "elabrev.dadofisico",'FIS','ppa');

$sql="update elabrev.dadofisico set prsano = '".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$sql = "update elabrev.dadofisico  set acaid=a.acaid from elabrev.ppaacao a where elabrev.dadofisico.acacodppa=a.acacodppa and elabrev.dadofisico.prgcod=a.prgcod and elabrev.dadofisico.prsano=a.prsano and elabrev.dadofisico.acaid is null";
$saida = $db->executar($sql);
$db -> commit();

flush();

/*


CargaPPAAcao.xml
CargaPPAPrograma.xml

sigplan

CargaRestricaoAcao.xml

print  $_SESSION['migração'];
*/
?>
