<?
 /*
   Sistema Sistema Simec
   Setor responsável: SPO/MEC
   Desenvolvedor: Desenvolvedores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo: recuperar_senha.php
   Finalidade: Permitir recuperar a senha
   */
  include "includes/classes_simec.inc";
  include "includes/funcoes.inc";
  $db = new cls_banco();
   $sql = "select acaid, acacod, unicod, loccod, prgcod from acao";
   $RS = $db->record_set($sql);
   $nlinhas = $db->conta_linhas($RS);
   for ($i=0;$i<=$nlinhas;$i++)
   {
     $res =  $db->carrega_registro($RS,$i);
     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     $sqle = "select cod from emenda e where e.unicod='$unicod' and substr(e.funcprog,8,4)='$prgcod' and substr(e.funcprog,13,4)='$acacod' and substr(e.funcprog,18,4)='$loccod' ";
   $RSe = $db->record_set($sqle);
   $nlinhase = $db->conta_linhas($RSe);
   if ($nlinhase >= 0) {
print $acacod.'<br>';
      $sql = "update acao set acasnemenda ='t' where acaid=$acaid";
      $_SESSION['usucpf']='';
      $saida = $db->executar($sql);
      $db->commit();
   }
   }
print 'FIM';
?>



