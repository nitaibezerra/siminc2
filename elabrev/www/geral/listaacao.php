<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listaacao.php
   
   */

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$prgid = $_REQUEST['prgid'];
?>
<table border="0" cellspacing="0" cellpadding="0" align="center"  style="border: 0px; color:#003F7E;width:750px;" >
  <!--<tr style="color:#000000;">-->
<?				
			// aqui testo se o programa foi expandido	
			$sql = "select distinct acaid,acacod, acadsc from elabrev.ppaacao_proposta where prsano='".$_SESSION['exercicio']."' and prgid=$prgid order by acacod";

			$RSa = $db->record_set($sql);
	        $nlinhasa = $db->conta_linhas($RSa);
	        if ($nlinhasa >= 0) {
	        	for ($ii=0; $ii<=$nlinhasa;$ii++){
	        		$res = $db->carrega_registro($RSa,$ii);
			        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
			        if (fmod($ii,2) == 0) $marcado = '' ; else $marcado='#F9F9F9';
			        // testa se existem propostas de exclusao
   			        $sql="select count(peaid) from elabrev.proposta_exclusao_acao where acaid=$acaid";        
   			        $exclusao=$db->pegaum($sql);		           			           			        			        $sql="select count(eraid) from elabrev.elaboracaorevisao where eracod=$acaid and eratabela='ppaacao_proposta'";
   			       $alteracao= $db->pegaum($sql); 			        
   			        $sql="select count(pfaid) from elabrev.proposta_fusao_acao where acaid=$acaid";
   			        $fusao= $db->pegaum($sql);			        
   			        $sql="select count(pmaid) from elabrev.proposta_migracao_acao where acaid=$acaid";
  			        $migracao=$db->pegaum($sql);
   			    $sql="select count(acaid) from elabrev.ppaacao_proposta where acaid=$acaid and acastatus='N' ";        
   			        $inclusao=$db->pegaum($sql);  			        
			        print 
			        '<tr style="color:#000000;">
			        <td style="width:50px;"></td>
			        <td  align="left" style="width:420px;"><img src="../imagens/seta_filho.gif"   align="absmiddle">'.$acacod.'-'.$acadsc."</td>
			        <td style=\"width:5px;\"><h3>$migracao</h3></td>
			        <td style=\"width:5px;\"><h3><font color='red'>$exclusao</font></h3></td>
			        <td style=\"width:5px;\"><h3><font color='blue'>$alteracao</font></h3></td>
			        <td style=\"width:5px;\"><h3><font color='green'>$fusao</font></h3></td>
					<td style=\"width:5px;\"><h3><font color='maroon'>$inclusao</font></h3></td>			        
			        </tr>";	
	        	}
	        }

?>
</table>



     
         
