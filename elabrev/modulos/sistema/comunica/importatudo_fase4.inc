<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_fase4.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 4
   */
include 'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");


print 'acao<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaAcao.xml", "acao",'ACA','sigplan');
$sql='update acao set prgid=p.prgid from programa p where p.prgcod=acao.prgcod and p.prgano=acao.prgano and acao.prgid is null';
$saida = $db->executar($sql);
$db -> commit();
/*
print 'dadofinanceiro<br>';
$sql='delete from dadofinanceiro';
$saida = $db->executar($sql);
$db -> commit();
$adodbXML->ImportaXML(&$clientedb, "CargaDadoFinanceiro.xml", "dadofinanceiro",'FIN','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaDadoFinanceiroRAP.xml", "dadofinanceiro",'FIN','sigplan');
$sql='update dadofinanceiro set acaid=acao.acaid where dadofinanceiro.acacod=acao.acacod and dadofinanceiro.prgcod=acao.prgcod and dadofinanceiro.prgano=acao.prgano and dadofinanceiro.saccod=acao.saccod and dadofinanceiro.acaid is null';
$saida = $db->executar($sql);
$db -> commit();
flush();
exit();
print 'dadofisico<br>';
$sql='delete from dadofisico';
$saida = $db->executar($sql);
$db -> commit();
$adodbXML->ImportaXML(&$clientedb, "CargaDadoFisico.xml", "dadofisico",'FIS','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaDadoFisicoRAP.xml", "dadofisico",'FIS','sigplan');
$sql='update dadofisico set acaid=acao.acaid where dadofisico.acacod=acao.acacod and dadofisico.prgcod=acao.prgcod and dadofisico.prgano=acao.prgano and dadofisico.saccod=acao.saccod and dadofisico.acaid is null';
$saida = $db->executar($sql);
$db -> commit();

// ajusta o execucaopto
//procura em execucaopto se existe no realizado um valor para a referencia

$sql = 'select acaid,acacod,fisqtderealizado1,fisqtderealizado2,fisqtderealizado3,fisqtderealizado4 ,fisqtderealizado5,fisqtderealizado6,fisqtderealizado7,fisqtderealizado8,fisqtderealizado9,fisqtderealizado10,fisqtderealizado11,fisqtderealizado12 from dadofisico ';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
for ($i=0;$i<=$nlinhas;$i++)
{
  $res =  $db->carrega_registro($RS,$i);
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  // com cada acaid e cada mes, procurar em execucaopto
  for ($j=1;$j<13;$j++)
  {

      if  (${'fisqtderealizado'.$j} > 0)
      {
        $sql="select refcod from referencia where refmes_ref::integer=$j and refano_ref::integer=".$_SESSION['exercicio'];

        $RSr = $db->record_set($sql);
        $res =  $db->carrega_registro($RSr,0);
        $refcod= $res[0];
        $sql="select expid, exprealizado from execucaopto where acaid=$acaid and refcod=$refcod";
        $RSr = $db->record_set($sql);
        $nlinhas2 = $db->conta_linhas($RSr);
        unset($sql);
        unset($res);
        if ($nlinhas2==0)
           $res =  $db->carrega_registro($RSr,0);

        if (! is_array($res))
        {
          // não existe então inclui
          $sql = "insert into execucaopto (exprealizado, usucpf, acaid, refcod) values (".${'fisqtderealizado'.$j}.",'',".$acaid.",".$refcod.")";
        }
        else if ($res['exprealizado']==0)
        {
          // então atualiza
       	   $sql = "update execucaopto set exprealizado=".${'fisqtderealizado'.$j}." where acaid=".$acaid." and refcod=".$refcod;
        }
        if (isset($sql))
        {
          $db->executar($sql);
          $db->commit();
        }

      }
  }

}
*/
print 'atualiza ppa acao<br>';
$adodbXML->upgradedb_ppa($dbconnection, "CargaPPAAcao.xml", "acao",'ACA','ppa');



//exit();
/*


CargaPPAAcao.xml
CargaPPAPrograma.xml

sigplan

CargaRestricaoAcao.xml

print  $_SESSION['migração'];
*/
?>
