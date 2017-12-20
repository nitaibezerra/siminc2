<?php 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo.inc
   Finalidade: permitir importar os dados do SIGPLAN fase 1
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

/*print 'regiao<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaRegiao.xml", "regiao",'REG','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaRegiao.xml", "regiao",'REG','ppa');
print 'tipoacao<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaTipoAcao.xml", "tipoacao",'TAC','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaTipoAcao.xml", "tipoacao",'TAC','ppa');
print 'tipoorgao<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaTipoOrgao.xml", "tipoorgao",'TPO','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaTipoOrgao.xml", "tipoorgao",'TPO','ppa');
print 'tipoprograma<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaTipoPrograma.xml", "tipoprograma",'TPR','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaTipoPrograma.xml", "tipoprograma",'TPR','ppa');
print 'tiporestricao<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaTipoRestricao.xml", "tiporestricao",'TRS','sigplan');
print 'tiposituacao<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaTipoSituacao.xml", "vs_tiposituacao",'SIT','sigplan');
print 'periodicidade<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaPeriodicidade.xml", "periodicidade",'PER','sigplan');
print 'orgao<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaOrgao.xml", "orgao",'ORG','sigplan');
$adodbXML->ImportaXML(&$clientedb, "CargaOrgao.xml", "orgao",'ORG','ppa');
print 'natureza<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaNatureza.xml", "natureza",'NAT','ppa');
print 'municipio<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaMunicipio.xml", "municipio",'MUN','ppa');
print 'ppalocalizador<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaPPALocalizador.xml", "ppalocalizador",'LOC','ppa');
print 'unidademedida<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaUnidadeMedida.xml", "unidademedida",'UNM','sigplan')
;
print 'fonteppa<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaFontePPA.xml", "fonteppa",'FPP','sigplan');
print 'esfera<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaEsfera.xml", "esfera",'ESF','ppa');
print 'ppafuncao<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaPPAFuncao.xml", "ppafuncao",'FUN','ppa');
print 'ppasubfuncao<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaPPASubFuncao.xml", "ppasubfuncao",'SFU','ppa');
print 'tipoinclusao<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaTipoInclusaoAcao.xml", "tipoinclusaoacao",'TIN','ppa');
print 'tipoatualizacao<br>';
$adodbXML->ImportaXML(&$clientedb, "CargaTipoAtualizacao.xml", "tipoatualizacao",'TAT','ppa');
print 'orgaosiorg<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaOrgaoSIORG.xml", "orgaosiorg",'OSI','ppa');
print 'tipodespesa<br>';

//$sql='update orgaosiorg set orgid=o.orgid from orgao o where o.orgcod=orgaosiorg.orgcod and o.organo=orgaosiorg.organo and orgaosiorg.orgid is null';
////$saida = $db->executar($sql);
//$db -> commit();

$adodbXML->ImportaXML(&$clientedb, "CargaTipoDespesa.xml", "tipodespesa",'TDP','ppa');
print 'produto<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaProduto.xml", "produto",'PRO','sigplan');
print 'opcaoestrategica<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaOpcaoEstrategica.xml", "opcaoestrategica",'OES','sigplan');
print 'basegeografica<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaBaseGeografica.xml", "basegeografica",'BSG','sigplan');
print 'macroobjetivo<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaMacroObjetivo.xml", "macroobjetivo",'MOB','sigplan');
print 'unidade<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaUnidade.xml", "unidade",'UNI','sigplan');
print 'programa<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaPrograma.xml", "programa",'PRG','sigplan');
print 'indicador<br>';

$adodbXML->upgradedb_ppa_programa($dbconnection, "CargaPPAPrograma.xml", "programa",'PRG','ppa');
//$sql='update programa set orgid=o.orgid from orgao o where o.orgcod=programa.orgcod and o.organo=programa.prgano and programa.orgid is null';
//$saida = $db->executar($sql);
//$db -> commit();
*/

$adodbXML->ImportaXML($clientedb, "CargaPPAIndicador.xml", "elabrev.ppaindicador",'IND','ppa');
$sql='update elabrev.ppaindicador set prgid=p.prgid 
from elabrev.ppaprograma_proposta p  
where p.prgcod=elabrev.ppaindicador.prgcod 
and p.prsano=elabrev.ppaindicador.prgano 
and elabrev.ppaindicador.prgid is null';

//$saida = $db->executar($sql);
//$db -> commit();
//print 'restricaoprograma<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaRestricaoPrograma.xml", "restricaoprograma",'RSP','sigplan');
//print 'acao<br>';
//$adodbXML->ImportaXML(&$clientedb, "CargaAcao.xml", "acao",'ACA','sigplan');
//$sql='update acao set prgid=programa.prgid where programa.prgcod=acao.prgcod and programa.prgano=acao.prgano and acao.prgid is null';
//$saida = $db->executar($sql);
//$db -> commit();
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
          $sql = "insert into execucaopto (exprealizado, usucpf, acaid, refcod) values (".${'fisqtderealizado'.$j}.",'00000000191',".$acaid.",".$refcod.")";
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
//print 'atualiza ppa acao<br>';
//$adodbXML->upgradedb_ppa($dbconnection, "CargaPPAAcao.xml", "acao",'ACA','ppa');



//exit();
/*


CargaPPAAcao.xml
CargaPPAPrograma.xml

sigplan

CargaRestricaoAcao.xml

print  $_SESSION['migração'];
*/
?>
