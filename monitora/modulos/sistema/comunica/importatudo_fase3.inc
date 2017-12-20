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

print 'dadofisico<br>';
$sql="delete from dadofisico where prgano='".$_SESSION['exercicio']."'";
$saida = $db->executar($sql);
$db -> commit();

$adodbXML->ImportaXML(&$clientedb, "CargaDadoFisico.xml", "monitora.dadofisico",'FIS','sigplan');

$adodbXML->ImportaXML(&$clientedb, "CargaDadoFisicoRAP.xml", "monitora.dadofisico",'FIS','sigplan');

$sql='update monitora.dadofisico set acaid=a.acaid from acao a where monitora.dadofisico.acacod=a.acacod and monitora.dadofisico.prgcod=a.prgcod and monitora.dadofisico.prgano=a.prgano and monitora.dadofisico.saccod=a.saccod and monitora.dadofisico.acaid is null';
$saida = $db->executar($sql);
$db -> commit();

// ajusta o execucaopto
//procura em execucaopto se existe no realizado um valor para a referencia

$sql = "select acaid,acacod,fisqtderealizado1,fisqtderealizado2,fisqtderealizado3,fisqtderealizado4 ,fisqtderealizado5,fisqtderealizado6,fisqtderealizado7,fisqtderealizado8,fisqtderealizado9,fisqtderealizado10,fisqtderealizado11,fisqtderealizado12 from monitora.dadofisico where prgano='".$_SESSION['exercicio']."'";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
//print $sql;
//exit();
for ($i=0;$i<=$nlinhas;$i++)
{
  $res =  $db->carrega_registro($RS,$i);
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  // com cada acaid e cada mes, procurar em execucaopto
  for ($j=1;$j<13;$j++)
  {
 
      if  (${'fisqtderealizado'.$j} > 0)
      {
        $sql="select refcod from referencia where refmes_ref::integer=$j and refdata_limite_parecer_aca is null and refano_ref::integer=".$_SESSION['exercicio'];

        $RSr = $db->record_set($sql);
        $res =  $db->carrega_registro($RSr,0);
        $refcod= $res[0];
        $sql="select expid, exprealizado from monitora.execucaopto where acaid=$acaid and refcod=$refcod";
        $RSr = $db->record_set($sql);
        $nlinhas2 = $db->conta_linhas($RSr);
        unset($sql);
        unset($res);
        if ($nlinhas2==0)
           $res =  $db->carrega_registro($RSr,0);
        
        if (! is_array($res))
        {
          // não existe então inclui
          $sql = "insert into monitora.execucaopto (exprealizado, usucpf, acaid, refcod) values (".${'fisqtderealizado'.$j}.",'00000000191',".$acaid.",".$refcod.")";
        }
        else if ($res['exprealizado']==0)
        {
          // então atualiza
       	   $sql = "update monitora.execucaopto set exprealizado=".${'fisqtderealizado'.$j}." where acaid=".$acaid." and refcod=".$refcod;
        }
        if (isset($sql))
        {
          $db->executar($sql);
          $db->commit();
        }
        
      }
  }
  
}


flush();

?>
