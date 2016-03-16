<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:inicio.inc
   Finalidade: permitir abrir a página inicial so simec
    */
//recupera todas as variaveis que veio pelo post ou get
foreach($_REQUEST as $k=>$v) ${$k}=$v;
if (!$aca){
//Chamada de programa
include "includes/cabecalho.inc";
?>
<br>
<link rel="stylesheet" type="text/css" href="includes/listagem2.css">

<?$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo="Estatísticas de Avaliação das Ações do MEC";
monta_titulo($titulo_modulo,'<div style="margin:3px;" align="right">Legenda: |<span style="background-color:#FFF0EE;"> 0 - 49% </span>|<span style="background-color:#FEFFE6"> 50 - 74% </span>|<span style="background-color:#E9FFEC"> 75 - 99% </span>|<span style="background-color:#E6F7FF"> 100% </span>|</div>');
?>
<table align="center" width="95%" border="0" cellpadding="3" cellspacing="0" class="listagem" style="color:#333333;">
					
							<tr bgcolor="#e7e7e7">
							<?
							print '<td style="width:250px"><img src="imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img1" onclick="abre_tudo();">&nbsp;&nbsp;<b>Programas</b></td><td  style="width:40px"><b>Qtd. Ações</b></td>';
							$sql = "select * from referencia where refdata_limite_avaliacao_aca is not null and refstatus='A' and refano_ref='".$_SESSION['exercicio']."' order by refmes_ref";
							$RS = $db->record_set($sql);
							$nlinhas = $db->conta_linhas($RS);
							$colunasref = $nlinhas+4;
							if ($nlinhas >= 0) {
    							 for ($i=0; $i<=$nlinhas;$i++){
     								$res = $db->carrega_registro($RS,$i);
									if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
									$cab_mesref[$i] = substr($refdsc,0,3).'/'.$refano_ref;
									print '<td  align="center" style="color:#696969;font-size:8px;font-family:Verdana;">'.$cab_mesref[$i].'</td>';
									}
							}
							?>
							 </tr>
						<tbody>
	<? 
	$sql = "select v.prgid, p.prgcod||' - '||p.prgdsc as prgdsc, p.prgcod, v.refcod, r.refdsc,
v.acatotal, v.faltap as cor0, v.total_cor1 as cor1, v.total_cor2 as cor2, 
v.total_cor3 as cor3
from avaliacaostat v
inner join programa p on v.prgid=p.prgid
inner join referencia r on v.refcod=r.refcod    
where p.prgstatus='A' and v.refano_ref=p.prgano and p.prgano='".$_SESSION['exercicio']."' 
order by p.prgcod, r.refmes_ref";
	
	
	$j=0; 
	$RS = $db->record_set($sql); 
	$nlinhas = $db->conta_linhas($RS); 
	if ($nlinhas >= 0) {
		//$nlinhas = 10;
	for ($i=0; $i<=$nlinhas;$i++){
	 	 	  	$res = $db->carrega_registro($RS,$i); 		
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=trim($v); 		
				if ($v_prgid <> $prgid) { 
					$mes=0;
					if (!$v_prgid) print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">'; else print '</tr><tr><td id="td'.$j.'-'.$v_prgid.'" colspan="'.$colunasref.'" style="padding:0px;"></td><script>document.getElementById(\'td'.$j.'-'.$v_prgid.'\').style.visibility = "hidden";document.getElementById(\'td'.$j.'-'.$v_prgid.'\').style.display = "none";</script></tr><tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
					$v_prgid = $prgid; 			
					$j = $j +1; 			
					if (fmod($j,2) == 0) $marcado = '' ; else $marcado='#F7F7F7'; 			
					$funcao_abretudo .= "abreconteudo('simec.php?modulo=relatorio/geral/relatorio_evolucao&acao=C&prgid=$prgid&aca=t','$j-$prgid');\n";
					print '<td style="width:250px"><img src="imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img'.$j.'-'.$prgid.'" name="+" onclick="abreconteudo(\'simec.php?modulo=relatorio/geral/relatorio_evolucao&acao=C&prgid='.$prgid.'&aca=t\',\''.$j.'-'.$prgid.'\');">&nbsp;&nbsp;<font color="#003366"><a href="simec.php?modulo=principal/programa/cadprograma&acao=C&prgid='.$prgid.'">'.$prgdsc.'</a></font></td>';
					print "\n";
					print "<td align='right'>".$acatotal."&nbsp;</td>";
			}
			$porcentopreenchido = number_format(100-($cor0*100/$acatotal),0,',','.');
			if ($porcentopreenchido < 50) $cor = '#FFF0EE'; elseif ($porcentopreenchido < 75) $cor = '#FEFFE6'; elseif ($porcentopreenchido < 100) $cor = '#E9FFEC'; else $cor = '#E6F7FF';
			print '<td align="right" nowrap style="background-color:'.$cor.';" '.monta_grafico_avaliacao($cor0,$cor1,$cor2,$cor3,$acatotal,$cab_mesref[$mes]).'>'.$porcentopreenchido.'%</td>';
			$mes++;

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
//Chamada de Ação
header('Content-Type: text/html; charset=iso-8859-1');?>

<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="margin-left: 0px;" class="listagem">
					
							<tr bgcolor="#F5F5F5">
							<?
							print '<td style="width:200px;"><img src="imagens/seta_filho.gif" width="12" height="13" alt="" border="0" align="absmiddle"> <b>Ações</b></td><td style="width:100px;">Unidade</td>';
							$sql = "select * from referencia where refdata_limite_avaliacao_aca is not null and refstatus='A'  and refano_ref='".$_SESSION['exercicio']."' order by refmes_ref";
							$RS = $db->record_set($sql);
							$nlinhas = $db->conta_linhas($RS);
							$colunasref = $nlinhas+4;
							
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
	$sql .= " where a.acastatus='A' and acasnrap='f' and a.prgid=".$prgid;
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
			print '<td align="center"><a href="simec.php?modulo=principal/acao/monitoraacao&acao=A&refcod='.$refcod.'&acaid='.$acaid.'&prgid='.$prgid.'" border=0><img src="imagens/Sin_'.$avcordsc.'_'.$pacordsc.'.gif" border="0" title="'.str_replace('/','-',$cab_mesref[$mes]).': '.$avcorsignificado.'/'.$pacorsignificado.'"></a></td>';
			$mes++;
		}
}
//print '</tr><tr><td colspan="'.$colunasref.'" style="border-top: 2px solid #000000; height:3px;">&nbsp;</td></tr></table>'; 


}

function monta_grafico_avaliacao($vl0,$vl1,$vl2,$vl3,$vltotal,$mes)
{
	$vl0p = number_format($vl0*100/$vltotal,2,'.',',');
	$vl1p = number_format($vl1*100/$vltotal,2,'.',',');
	$vl2p = number_format($vl2*100/$vltotal,2,'.',',');
	$vl3p = number_format($vl3*100/$vltotal,2,'.',',');
	$txtgrafico = "Q<b>$mes</b> - Total: $vltotal Ações<br>$vl1 <img src=\'imagens/cor1.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Estável ($vl1p%)<br>$vl2 <img src=\'imagens/cor2.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Merece Atenção ($vl2p%)<br>$vl3 <img src=\'imagens/cor3.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Crítico ($vl3p%)<br>$vl0 <img src=\'imagens/cor0.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Não Avaliada ($vl0p%)";
	$imggrafico = "onmouseover=\"return escape('$txtgrafico')\"";
	return $imggrafico;
}


?>