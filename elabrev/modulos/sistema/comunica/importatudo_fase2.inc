<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_fase2.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 2
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

print  'dadofinanceiro<br>';
$sql="delete from elabrev.dadofinanceiro where prsano='".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$adodbXML->ImportaXML(&$clientedb, "CargaPPADadoFinanceiro.xml", "elabrev.dadofinanceiro",'FIN','ppa');

$sql="update elabrev.dadofinanceiro set prsano = '".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$sql = "update elabrev.dadofinanceiro  set acaid=a.acaid from elabrev.ppaacao a where elabrev.dadofinanceiro.acacodppa=a.acacodppa and elabrev.dadofinanceiro.prgcod=a.prgcod and elabrev.dadofinanceiro.prsano=a.prsano and elabrev.dadofinanceiro.acaid is null";
$saida = $db->executar($sql);
$db -> commit();



/*


CargaPPAAcao.xml
CargaPPAPrograma.xml

sigplan

CargaRestricaoAcao.xml

print  $_SESSION['migração'];
*/
?>
