<?
/*
   Sistema Simec
   Setor responsvel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a pgina inicial so simec
    */
//recupera todas as variaveis que veio pelo post ou get
foreach($_REQUEST as $k=>$v) ${$k}=$v;
if (!$aca){
//Chamada de programa
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">

<?$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo="Relatório de Validação";
monta_titulo($titulo_modulo,'Programas e Ações do MEC incluídas no BGU');
?>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
					
							<tr bgcolor="#e7e7e7">
							<?
							print '<td style="width:250px"><img src="../../imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img1" onclick="abre_tudo();">&nbsp;&nbsp;<b>Programas</b></td>';
							$sql = "select * from referencia where refdata_limite_parecer_aca is not null and refstatus='A' and refano_ref='".$_SESSION['exercicio']."' order by refmes_ref";
							$RS = $db->record_set($sql);
							$nlinhas = $db->conta_linhas($RS);
							$colunasref = $nlinhas+3;
							if ($nlinhas >= 0) {
    							 for ($i=0; $i<=$nlinhas;$i++){
     								$res = $db->carrega_registro($RS,$i);
									if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
									print '<td  align="center" style="color:#696969;"><p>'.$refdsc.'</p></td>';
									}
							}
							?>
							 </tr>
						<tbody>
	<? 
	$sql = "select v.prgid, v.refcod, v.acatotal, v.faltap as cor0, v.total_cor1 as cor1, v.total_cor2 as cor2, v.total_cor3 as cor3, p.prgcod||' - '||p.prgdsc as prgdsc, p.prgcod
from validacaostat v inner join programa p on v.prgid=p.prgid inner join acao aca on p.prgid=aca.prgid  and aca.acasnbgu='t' inner join referencia r on v.refcod=r.refcod and r.refsnmonitoramento='t' and r.refano_ref='".$_SESSION['exercicio']."'   where p.prgstatus='A' and p.prgano='".$_SESSION['exercicio']."' group by v.prgid, v.refcod, v.acatotal, v.faltap, v.total_cor1, v.total_cor2, v.total_cor3, p.prgcod||' - '||p.prgdsc, p.prgcod order by p.prgcod, v.refcod";

	//print $sql;
	
	$j=0; 
	$RS = $db->record_set($sql); 
	$nlinhas = $db->conta_linhas($RS); 
	if ($nlinhas >= 0) {
		//$nlinhas = 10;
	for ($i=0; $i<=$nlinhas;$i++){
	 	 	  	$res = $db->carrega_registro($RS,$i); 		
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=trim($v); 		
				if ($v_prgid <> $prgid) { 			
					if (!$v_prgid) print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">'; else print '</tr><tr><td id="td'.$j.'-'.$v_prgid.'" colspan="'.$colunasref.'" style="padding:0px;"></td><script>document.getElementById(\'td'.$j.'-'.$v_prgid.'\').style.visibility = "hidden";document.getElementById(\'td'.$j.'-'.$v_prgid.'\').style.display = "none";</script></tr><tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
					$v_prgid = $prgid; 			
					$j = $j +1; 			
					if (fmod($j,2) == 0) $marcado = '' ; else $marcado='#F7F7F7'; 			
					$funcao_abretudo .= "abreconteudo('monitora.php?modulo=inicio_bgu&acao=C&prgid=$prgid&aca=t','$j-$prgid');\n";
					print '<td style="width:250px"><img src="../imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img'.$j.'-'.$prgid.'" name="+" onclick="abreconteudo(\'monitora.php?modulo=inicio_bgu&acao=C&prgid='.$prgid.'&aca=t\',\''.$j.'-'.$prgid.'\');">&nbsp;&nbsp;<font color="#003366"><a href="'.$_SESSION['sisdiretorio'].'.php?modulo=principal/programa/cadprograma&acao=C&prgid='.$prgid.'">'.$prgdsc.'</a></font></td>';
					print "\n";
			} 			
			print '<td align="center"><a href="'.$_SESSION['sisdiretorio'].'.php?modulo=principal/programa/monitoraprg&acao=A&refcod='.$refcod.'&prgid='.$prgid.'" border=0>'.monta_grafico_validacao($cor0,$cor1,$cor2,$cor3,$acatotal).'</a></td>'; 	
		}
}
print '</tr><tr><td id="td'.$j.'-'.$v_prgid.'" colspan="'.$colunasref.'" style="padding:0px;"></td><script>document.getElementById(\'td'.$j.'-'.$v_prgid.'\').style.visibility = "hidden";document.getElementById(\'td'.$j.'-'.$v_prgid.'\').style.display = "none";</script></tr></table>';
print "\n";
?>

<script language="JavaScript">
function abre_tudo()
{
<?=$funcao_abretudo;?>
}
</script>
<?
}

else{
//Chamada de Ao
header('Content-Type: text/html; charset=iso-8859-1');?>

<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="margin-left: 0px;" class="listagem">
					
							<tr bgcolor="#F5F5F5">
							<?
							print '<td style="width:200px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0" align="absmiddle"> <b>Aes</b></td><td style="width:100px;">Unidade</td>';
							$sql = "select * from referencia where refdata_limite_avaliacao_aca is not null and refstatus='A'  and refano_ref='".$_SESSION['exercicio']."' order by refmes_ref";
							$RS = $db->record_set($sql);
							$nlinhas = $db->conta_linhas($RS);
							$colunasref = $nlinhas+3;
							
							if ($nlinhas >= 0) {
    							 for ($i=0; $i<=$nlinhas;$i++){
     								$res = $db->carrega_registro($RS,$i);
									if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
									$cab_mesref[$i] = substr($refdsc,0,3).'/'.$refano_ref;
									print '<td style="color:#333333;font-size:8px;font-family:Verdana;">'.substr($refdsc,0,3).'/'.$refano_ref.'</td>';
									}
							}
							?>
							 </tr>
						<tbody>
	<? 
	$sql = "select a.acaid, a.acacod||'.'||a.unicod||'.'||a.loccod||'-'||a.acadsc as acadsc, uni.unidsc, trim(cav.cordsc) as avcordsc, trim(cpa.cordsc) as pacordsc , case when cav.corsignificado is null then 'NI' else cav.corsignificado end as avcorsignificado, case when cpa.corsignificado is null then 'NI' else cpa.corsignificado end as pacorsignificado, ref.refmes_ref, ref.refano_ref, ref.refcod";
	$sql .= " from acao a"; 
	$sql .= " left join unidade uni on a.unicod=uni.unicod "; 
	$sql .= " left join referencia ref on ref.refdata_limite_avaliacao_aca is not null and ref.refstatus='A'  and ref.refano_ref='".$_SESSION['exercicio']."'"; 
	$sql .= " left join avaliacaoparecer av on a.acaid=av.acaid and av.tpaid=1 and av.refcod=ref.refcod and av.avpstatus='A' and av.avpliberada='t'"; 
	$sql .= " left join avaliacaoparecer pa on a.acaid=pa.acaid and pa.tpaid=2 and pa.refcod=ref.refcod and pa.avpstatus='A' and pa.avpliberada='t'"; 
	$sql .= " left join cor cav on av.corcod=cav.corcod"; 
	$sql .= " left join cor cpa on pa.corcod=cpa.corcod"; 
	$sql .= " where a.acastatus='A' and acasnrap='f' and acasnbgu='t' and a.prgid=".$prgid;
	$sql .= " order by a.acaid, ref.refmes_ref"; 
	$j=0; 
	
	$RS = $db->record_set($sql); 
	$nlinhas = $db->conta_linhas($RS); 
	if ($nlinhas >= 0) {
	for ($i=0; $i<=$nlinhas;$i++){
	 	 	  	$res = $db->carrega_registro($RS,$i); 		
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=trim($v); 		
				if ($v_acaid <> $acaid) {
					$mes=0;
					if (!$v_acaid) print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">'; else print '</tr><tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
					$v_acaid = $acaid; 			
					$j = $j +1; 			
					if (fmod($j,2) == 0) $marcado = '' ; else $marcado='#F7F7F7'; 			
					print '<td style="width:200px;">&nbsp;&nbsp;<font color="#006633">'.$acadsc.'</font></td><td style="width:100px;">'.$unidsc.'</td>';
			} 			
			print '<td align="center"><a href="'.$_SESSION['sisdiretorio'].'.php?modulo=principal/acao/monitoraacao&acao=A&refcod='.$refcod.'&acaid='.$acaid.'&prgid='.$prgid.'" border=0><img src="../imagens/Sin_'.$avcordsc.'_'.$pacordsc.'.gif" border="0" title="'.str_replace('/','-',$cab_mesref[$mes]).': '.$avcorsignificado.'/'.$pacorsignificado.'"></a></td>';
			$mes++;
		}
}
print '</tr><tr><td colspan="'.$colunasref.'" style="border-top: 2px solid #000000; height:3px;">&nbsp;</td></tr></table>'; 


}?>