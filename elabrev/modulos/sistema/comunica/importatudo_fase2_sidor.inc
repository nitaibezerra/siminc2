<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_fase2_sidor.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 2 quantitativa
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

print  'dadofinanceiro<br>';
$sql="delete from elabrev.dadofinanceiro_orcamento where prgano='".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$adodbXML->ImportaXML($clientedb, "CargaPPADadoFinanceiro_SIDOR.xml", "elabrev.dadofinanceiro_orcamento",'FIN','ppa');

$sql="update elabrev.dadofinanceiro_orcamento set prgano = '".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$sql = "update elabrev.dadofinanceiro_orcamento  set acaid=a.acaid from elabrev.ppaacao_orcamento a where elabrev.dadofinanceiro_orcamento.acacodppa=a.acacodppa and elabrev.dadofinanceiro_orcamento.prgcod=a.prgcod and elabrev.dadofinanceiro_orcamento.prgano=a.prgano and elabrev.dadofinanceiro_orcamento.acaid is null";
$saida = $db->executar($sql);
$db -> commit();


?>
