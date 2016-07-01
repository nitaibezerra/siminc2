<?
  // verifica se é coordenador de ação
  if ($db->testa_coordenador($_SESSION['acaid'],'A')) {
  $coordaca = true;
  $status='coordaca';
}
if ($db->testa_digitador($_SESSION['acaid'],'A')) {
    $digit = true;
    $status='digit';
    }
  // caso a autorização não tenha sido feita, testo se é super usuário
if (! $coordaca and ! $gerex and ! $digit)
{
if ($db->testa_superuser()) {
     $coordaca = true;
     $status='coordaca';
  }
}
unset($referencias);
$referencias = array();
if ($coordaca or $digit){
	$sql = "select refcod from referencia where cast(now() as date)>=refdata_inicio and cast(now() as date)<=refdata_limite_avaliacao_aca";
	$RSref = $db->record_set($sql);
	$nlinhasref = $db->conta_linhas($RSref);
	if ($nlinhasref >= 0) {
		for ($ii=0; $ii<=$nlinhasref;$ii++){
		  $res = @$db->carrega_registro($RSref,$ii);
		  foreach($res as $k=>$v) ${$k}=$v;
		  array_push($referencias, $v);
		  }
	}
}
$sql = "select p.ptoid, p.ptoid_pai,p.ptotipo,p.ptocod, case when p.ptotipo='S' then 'Subação' when p.ptotipo='E' then 'Etapa' else 'Fase' end as tipo,p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptostatus='A' and p.ptoid in (select ptoid from monitora.plantrabacao where acaid=".$_SESSION['acaid'].") and p.ptoid=".$ptoid3."  group by p.ptoid,p.ptoid_pai,p.ptotipo, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma,p.ptoordem, u.unmdsc order by p.ptoordem,p.ptotipo desc,p.ptoid_pai, p.ptocod";


 $RSef = $db->record_set($sql);
$resef = @$db->carrega_registro($RSef,0);
 	if (is_array($resef)) {foreach($resef as $k=>$v) ${$k}=$v;}
	if ($previsto) $porcentorealizado = $totalrealizado*100/$previsto;
	if ($porcentorealizado > 100) $mostraporcentorealizado = 100; else $mostraporcentorealizado = $porcentorealizado;
	$porcentoexecutado = 100 - $mostraporcentorealizado;?>
    <THEAD bgcolor="#f5f5f5">
		<tr style="background-color:#ececec;">
			<TD colspan="12" align="left" style="color:#000099;"><img src="../imagens/seta_filho.gif" width="13" height="13" alt="" border="0">Cód.: <strong><?=$ptocod?> - <?=$ptodsc?></strong></TD>
			<TD colspan="2" align="right">Unid. Medida:<strong><?=$unmdsc?></strong><br>
			0% <label style="border: 1px solid #000000; font-size: 8px; border-top: 1px solid #c0c0c0; border-right: 1px solid #c0c0c0; border-left: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0;background-color:#ffffff;" title="Realizado <?=number_format($porcentorealizado, 0, '.', '')?>%"><span style="background-color:#33cc00;font-size: 8px;"><?for ($k=0; $k<$mostraporcentorealizado/2;$k++) print "&nbsp;";?></span><?for ($k=0; $k<$porcentoexecutado/2;$k++) print "&nbsp;";?></label>
      <?
      if ($porcentorealizado < 100) print number_format($porcentorealizado, 0, '.', ''); else print '100';
      ?>%</TD>
		</tr>
   </THEAD>
   <TBODY>
     <TR style="color:#808080;" bgcolor="#f6f6f6">
			<?$sql = "select refmes_ref, refano_ref from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
				$RS2 = $db->record_set($sql);
				$nlinhas2 = $db->conta_linhas($RS2);
				if ($nlinhas2 >= 0) {
					for ($j=0; $j<=$nlinhas2;$j++){
						$res2 = @$db->carrega_registro($RS2,$j);
					  	foreach($res2 as $k=>$v) ${$k}=$v;?>
			<TD align="right"><?=$refmes_ref.'/'.$refano_ref?></TD>
			<?		}
				}
				$colspan = 12 - $j;
				if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
			?>

		    <TD align="right">Realizado</TD>
		    <TD align="right">Previsto</TD>
		   </TR>
		    <TR style="height:30px;">
			<?
            $sql = "select r.refcod, case when e.exprealizado is null then -1 else e.exprealizado end as exprealizado,  e.expobs, e.tpscod, t.tpscod, t.tpsdsc from referencia r left join execucaopto e on r.refcod=e.refcod and e.ptoid=".$ptoid." left join tiposituacao t on e.tpscod=t.tpscod where r.refdata_limite_avaliacao_aca is not null and r.refsnmonitoramento='t' and r.refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
				$RS2 = $db->record_set($sql);
				$nlinhas2 = $db->conta_linhas($RS2);
				$totalrealizado = 0;
				if ($nlinhas2 >= 0) {
					for ($j=0; $j<=$nlinhas2;$j++){
						$res2 = @$db->carrega_registro($RS2,$j);
					  	foreach($res2 as $k=>$v) ${$k}=$v;
						if ($exprealizado == -1) $v_exprealizado=$exprealizado+1; else $v_exprealizado=$exprealizado;
						if (in($refcod,$referencias) and ($coordaca or $digit)) $txtexprealizado = '<input type="Text" size="5" value="'.$v_exprealizado.'" class="CampoEstilo" style="text-align : right; color:#808080;"  readonly onclick="edita(\''.$refcod.'\',\''.$ptoid.'\');" onmouseover="MouseOver(this);" onmouseout="MouseOut(this);">'; else $txtexprealizado = $v_exprealizado;?>
			<TD align="right" style="color:#3366ff;"><?=$txtexprealizado?></TD>
			<?	if ($ptosnsoma=='r') { if($exprealizado>-1) $totalrealizado = $exprealizado;}
				else{
					if ($exprealizado == -1) $exprealizado=$exprealizado+1;
					$totalrealizado = $totalrealizado + $exprealizado;	}

                    }
				}
				$colspan = 12 - $j;
				if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
			?>
		    <TD align="right" style="color:#cc0000;"><?=number_format($totalrealizado, 0, '.', '');?></TD>
		    <TD align="right" style="color:#0000CC;"><?=$previsto?></TD>
		   </TR>
   </TBODY>

			<?

?>
