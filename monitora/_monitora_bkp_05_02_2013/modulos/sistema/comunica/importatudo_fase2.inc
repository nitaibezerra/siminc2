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
$sql="delete from dadofinanceiro where prgano='".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();
$adodbXML->ImportaXML(&$clientedb, "CargaDadoFinanceiro.xml", "monitora.dadofinanceiro",'FIN','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaDadoFinanceiroRAP.xml", "monitora.dadofinanceiro",'FIN','sigplan');
$sql='update monitora.dadofinanceiro set acaid=a.acaid from acao a where monitora.dadofinanceiro.acacod=a.acacod and monitora.dadofinanceiro.prgcod=a.prgcod and monitora.dadofinanceiro.prgano=a.prgano and monitora.dadofinanceiro.saccod=a.saccod and monitora.dadofinanceiro.acaid is null';
$saida = $db->executar($sql);
$db -> commit();



?>
