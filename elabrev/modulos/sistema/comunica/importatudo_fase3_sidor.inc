<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_fase3_sidor.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 3
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

print  'dadofisico<br>';
$sql="delete from elabrev.dadofisico_orcamento where prgano='".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$adodbXML->ImportaXML($clientedb, "CargaPPADadoFisico_SIDOR.xml", "elabrev.dadofisico_orcamento",'FIS','ppa');

$sql="update elabrev.dadofisico_orcamento set prgano = '".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$sql = "update elabrev.dadofisico_orcamento  set acaid=a.acaid from elabrev.ppaacao_orcamento a where elabrev.dadofisico_orcamento.acacodppa=a.acacodppa and elabrev.dadofisico_orcamento.prgcod=a.prgcod and elabrev.dadofisico_orcamento.prgano=a.prgano and elabrev.dadofisico_orcamento.acaid is null";
$saida = $db->executar($sql);
$db -> commit();


?>
