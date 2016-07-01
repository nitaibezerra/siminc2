<?

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$prgid = $_REQUEST['prgid'];

?>
<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center"  style="border: 0px; color:#003F7E;" >
  <!--<tr style="color:#000000;">-->
<?				
			// aqui testo se o programa foi expandido	
			if ($prgid)
			{
			   $sql = "select distinct acaid,acacod, acadsc,prgcod from elabrev.ppaacao_proposta where prsano='".$_SESSION['exercicio']."' and prgid=$prgid order by acacod";
}
			   else
			   $sql = "select distinct pa.acaid,pa.acacod, pa.acadsc,pa.prgcod from elabrev.ppaacao_proposta pa inner join elabrev.usuarioresponsabilidade ur on ur.acaid=pa.acaid and ur.usucpf='".$_SESSION['usucpf']."' where pa.prsano='".$_SESSION['exercicio']."' order by acacod";
			$RSa = $db->record_set($sql);
	        $nlinhasa = $db->conta_linhas($RSa);
	        if ($nlinhasa >= 0) {
	        	for ($ii=0; $ii<=$nlinhasa;$ii++){
	        		$res = $db->carrega_registro($RSa,$ii);
			        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
			        if (fmod($ii,2) == 0) $marcado = '' ; else $marcado='#F9F9F9';
			        // testa se existem propostas de exclusao

			        print 
			        '<tr style="color:#000000;">
			        <td  align="left" style="width:420px;"><img src="../imagens/seta_filho.gif"   align="absmiddle"><a href="elabrev.php?modulo=principal/ppafasequalitativa/acao/cadacao&acao=C&acaid='.$acaid.'&prgid='.$prgid.'">'.$acacod.'-'.$acadsc."</a></td>
			        <td style=\"width:5px;\"><h2></h2></td>
			        <td style=\"width:5px;\"></td>
			        <td style=\"width:5px;\"></td>
			        <td style=\"width:5px;\"></td>

			        </tr>";	
	        	}
	        }

?>
</table>



     
         
