<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listaacaodespesa.php
   
   */
include "config.inc";
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Pragma: no-cache");
header('Content-Type: text/html; charset=iso-8859-1');

include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$ordem = $_REQUEST['ordem'];
$acaid = str_replace('|',chr(39),$_REQUEST['acaid']);
$tipoacao1 = $_REQUEST['tipoacao1'];
$tipoacao2 = $_REQUEST['tipoacao2'];
$tipoacao3 = $_REQUEST['tipoacao3'];
/*
if ($db->usuarioPossuiPermissaoTodasUnidades())
	$join_responsabilidade = "";
else
	$join_responsabilidade = " inner join elabrev.usuarioresponsabilidade r on r.unicod = u.unicod and r.rpustatus = 'A' and r.usucpf = '".$_SESSION['usucpf']."' inner join seguranca.perfil p ON p.pflcod = r.pflcod and p.sisid = ".$_SESSION['sisid'];
*/
$join_responsabilidade = $db->usuarioJoinUnidadesPermitidas();
$join_responsabilidade .= " and a.unicod = unijoin.unicod ";


//filtros de tipo de ação
$wh = " and a.acasnrap = 'f' ";

//Lista Ações
if ($_REQUEST['prgid'] and !$_REQUEST['codigo'] and !$acaid)
{
	$prgid = $_REQUEST['prgid'];
	$tit1 = 'Unidades:';
	$sql	=
		" select 
			 u.unicod as codigo, 
			 u.unidsc as descricao, 
			 count(*) as total, 
			 c.limite as limite, 
			 c.despesa as despesa, 
			 c.saldo as saldo 
		 from elabrev.ppaacao_orcamento a 
			 inner join unidade u on a.unicod = u.unicod 
			 " . $join_responsabilidade . "
			 left join ( 
				 select 
					 unicod, 
					 sum( coalesce ( vllimite, 0 ) ) as limite, 
					 sum( coalesce ( vldespesa, 0 ) ) as despesa, 
					 sum( coalesce ( saldo, 0 ) ) as saldo 
				 from elabrev.v_saldounidadefonte2 
				 where ppoanoexercicio = '" . $_SESSION['exercicio'] . "' 
				 group by unicod, unidsc 
				 ) as c on c.unicod = a.unicod 
		 where prgano = '" . $_SESSION['exercicio'] . "' 
		 and a.prgid in ( '" . $_REQUEST['prgid'] . "' ) and 
		 a.acastatus='A' " . $wh . "
		 group by u.unicod, u.unidsc, c.limite, c.despesa, c.saldo 
		 order by u.unicod ";
	?>
	<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="color:#003F7E;">
	<tr>
	      <td style="width:12px;" nowrap>&nbsp;</td>
		  <td width="100%"><?= $tit1 ?></td>
		  <td style="width:100px;" align="right" nowrap>Limite:</td>
		  <td style="width:101px;" align="right" nowrap>Despesa:</td>
		  <td style="width:97px;" align="right" nowrap>Saldo:</td>
	</tr>
	<?
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if ( $nlinhas  < 0)
	{
		print "</table><font color='red'>Não foram encontrados Registros</font>";
	}
	else
	{
		for ( $i = 0; $i <= $nlinhas; $i++ )
		{
			$res = $db->carrega_registro( $RS, $i );
			if( is_array( $res ) )
			{
				foreach( $res as $k => $v ) ${$k} = $v;
			}
			?>
			<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
				<td valign="top">
					<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
				</td>
				<td valign="top" style="border-top: 1px solid #cccccc; padding:2px;" onclick="abreconteudo('geral/listaacaodespesa.php?codigo=<?=$codigo?>&prgid=<?=$prgid?>&ordem=<?=$ordem?>&tipoacao=<?=$tipoacao?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>','<?=$prgid.$codigo?>')"><img src="../imagens/mais.gif" width="9" height="9" alt="" border="0" name="+" id="img<?=$prgid.$codigo?>">
					<?= $codigo ?> - <?= $descricao ?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; padding:2px;">
					<?= number_format( ( $limite ) , 0, ',', '.')?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; padding:2px;">
					<?= number_format( ( $despesa ) , 0, ',', '.')?>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; padding:2px;">
					<?= number_format( ( $saldo ) , 0, ',', '.')?>
				</td>
			</tr>
			<tr>
				<td colspan="5" id="td<?=$prgid.$codigo?>"></td>
			</tr>
			<?
		}
		print "</table>";
		$db -> close(); exit();
	}
}
?>


<?
//Lista detalhamento das Ações
if ($_REQUEST['codigo'] or $acaid)
  {
  $codigo = $_REQUEST['codigo'];
  $prgid = $_REQUEST['prgid'];
  $sql = "select ppoid from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoano = '" . ( $_SESSION['exercicio'] + 1 ) . "'";
  $id_proposta_ativa = $db->pegaUm( $sql );
  if ($prgid<>'') $wh .= " and a.prgid in('".$prgid."')";
  	if ($acaid)
	{
		$wh .= " and a.acaid in('".$acaid."') ";
		$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Unidade'; $tit4 = 'Localizador'; $tit5 = 'Responsabilidade';
		$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.unicod as cod2,  a.acadsc as desc1, u.unidsc as desc2, a.loccod as cod3, a.sacdsc as desc3 from acao a left join unidade u on a.unicod = u.unicod where a.prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' ".$wh." order by a.acacod, u.unicod, a.loccod";
	}
	elseif ($ordem == 'L') 
	{
		$jn .= " and ac.ppoid = " . ( $id_proposta_ativa ? $id_proposta_ativa : 'null' );
		$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Vl. Distribuído:';
		$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.loccod as cod2,  a.acadsc as desc1, a.sacdsc as desc2, coalesce( SUM(ac.dpavalor+ac.dpavalorexpansao) , 0) as valor from elabrev.ppaacao_orcamento a inner join unidade u on a.unicod = u.unicod " . $join_responsabilidade . " left join elabrev.despesaacao ac ON ac.acaid = a.acaid " . $jn . " where a.prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' and a.loccod ='".$codigo."' ".$wh." group by a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod, a.loccod, a.acadsc, a.sacdsc order by a.acacod, a.unicod, a.loccod";
	}
	else if ($ordem == 'U')
	{	
		$jn .= " and ac.ppoid = " . ( $id_proposta_ativa ? $id_proposta_ativa : 'null' );
		$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Vl. Distribuído:';
		$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.loccod as cod2,  a.acadsc as desc1, a.sacdsc as desc2, coalesce( SUM(ac.dpavalor+ac.dpavalorexpansao) , 0) as valor from elabrev.ppaacao_orcamento a inner join unidade u on a.unicod = u.unicod " . $join_responsabilidade . " left join elabrev.despesaacao ac ON ac.acaid = a.acaid " . $jn . " where a.prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' and u.unicod ='".$codigo."' ".$wh." group by a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod, a.loccod, a.acadsc, a.sacdsc order by a.acacod, a.unicod, a.loccod";
	} 
	else
	{
		$jn .= " and ac.ppoid = " . ( $id_proposta_ativa ? $id_proposta_ativa : 'null' );
		$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Vl. Distribuído:';
		$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.loccod as cod2,  a.acadsc as desc1, a.sacdsc as desc2, coalesce( SUM(ac.dpavalor+ac.dpavalorexpansao) , 0) as valor from elabrev.ppaacao_orcamento a inner join unidade u on a.unicod = u.unicod " . $join_responsabilidade . " left join elabrev.despesaacao ac ON ac.acaid = a.acaid " . $jn . " where a.prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' and u.unicod ='".$codigo."' ".$wh." group by a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod, a.loccod, a.acadsc, a.sacdsc order by a.acacod, a.unicod, a.loccod";
	}

	?>
  
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="color:#006600;">
<tr style="color:#000000;">
      <td valign="top" style="width:24px;" nowrap>&nbsp;</td>
	  <td valign="top" style="width:132px;" nowrap><?=$tit1?></td>
	  <td valign="top"><?=$tit2?></td>
	  <td valign="top" style="width:98px;" align="right">&nbsp;</td>
	  <td valign="top" style="width:98px;" align="right"><?=$tit3?></td>
	  <td valign="top" align="right" style="width:95px;">&nbsp;</td>
	  <?if ($tit4){?><td valign="top"><?=$tit4?></td><?}?>
	  <?if ($tit5){?><td valign="top"><?=$tit5?></td><?}?>
    </tr>
  <?
	 //$sql= "select * from acao where acacod='".$acacod."'";
	 $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas<0) print "<tr><td width=\"12\"></td><td colspan=\"6\"><font color='red'>Não foram encontrados Registros</font></td></tr>";
	else {
			for ($i=0; $i<=$nlinhas;$i++){
			$res = $db->carrega_registro($RS,$i);
			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
	<td valign="top" style="padding:2px;">
		<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
	</td>
	<td valign="top" style="border-top: 1px solid #cccccc; padding:2px;">
		<?if($unicod == '26101' || 
			 $unicod == '26290' ||
			 $unicod == '26291' ||
			 $unicod == '26298'){?>
			<a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/propostaorcamentaria/despesadetalhamentosub&acao=A&acaid=<?=$acaid?>"><?=$prgcod?>.<?=$acacod?>.<?=$loccod?></a>
		<?}else{?>
			<a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/propostaorcamentaria/despesadetalhamento&acao=A&acaid=<?=$acaid?>"><?=$prgcod?>.<?=$acacod?>.<?=$loccod?></a>
		<?}?>
	</td>
	<td valign="top" style="border-top: 1px solid #cccccc; padding:2px;">
		<?=$cod1?> - <?=$desc1?>
	</td>
	<td valign="top" style="border-top: 1px solid #cccccc; padding:2px;">&nbsp;</td>
	<td valign="top" align="right" style="border-top: 1px solid #cccccc; padding:2px;">
		<?=number_format( ( $valor ) , 0, ',', '.')?>
	</td>
	<? if ( $tit4 ) : ?>
		<td valign="top" style="border-top: 1px solid #cccccc; padding:2px;">
			<?=$cod3?> - <?=$desc3?>
		</td>
	<? endif; ?>
	<?if ( $tit5 ) : ?>
		<td valign="top" style="border-top: 1px solid #cccccc; padding:2px;">
			<? $db->monta_lista_simples("select pfldsc from usuarioresponsabilidade u inner join seguranca.perfil p on u.pflcod=p.pflcod where usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and u.acaid='".$acaid."'","",100,20) ?>
		</td>
	<? endif; ?>
</tr>
<?
}
?>
<?if ($tit5){?><tr><td colspan="6" align="right" style="color:000000;border-top: 2px solid #000000;">Total de Ações: (<?=$nlinhas+1?>)</td></tr><?}?>
</table>
<?$db -> close(); exit();
}
}
?>



     
         
