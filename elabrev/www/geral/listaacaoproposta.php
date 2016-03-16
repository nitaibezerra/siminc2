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
//Recupera tudo que foi passado por REQUEST e instancia as variáveis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;

//Lista Ações
if ( $_REQUEST['subnivel'] == '3' )
{
	$prgid = $_REQUEST['prgid'];

	$wh1 = '';
	$wh = '';

	if ( $inclusao ) $wh .= ' or tipo = \'I\'';
	if ( $alteracao ) $wh .= ' or tipo = \'A\'';
	if ( $exclusao ) $wh .= ' or tipo = \'E\'';
	if ( $wh != '' ) $wh1 = ' where 1 = 2 '.$wh; else $wh1 = ' where 1 = 1';
	
	$wh .= " and a.acaid in(".$_REQUEST['acaid'].")";
	$tit1 = 'Propostas:'; $tit2 = 'Tipo:'; 
	$sql = 'select acaid as codigo, acadsc as descricao, case tipo when \'I\' then \'Inclusão\' when \'A\' then \'Alteração\' when \'F\' then \'Fusão\' when \'M\' then \'Migração\' when \'E\' then \'Exclusão\' end as Tipo from elabrev.v_propostas_acoes a where 1 = 1 '.$wh; 
	?>
	<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="color:#003F7E;">
	<tr>
	  <td style="width:24px;" nowrap>&nbsp;</td>
	  <td valign="top">
		<?=$tit1?>
	  </td>
	  <td valign="top" align="right">
	  	<?=$tit2?></td>
	  <td valign="top" align="right" style="width:120px;">&nbsp;</td>
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
					&nbsp;
				</td>
				<td valign="top" style="border-top: 1px solid #cccccc; padding:2px; width:250px;" >
					<a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/acao/geraproposta&acao=A&acod=<?=$codigo?>" target="_blank"><?= $descricao ?></a></td>
				</td>
				<td align="right" style="border-top: 1px solid #cccccc; padding:2px; width:90px;">
					<a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/acao/geraproposta&acao=A&acod=<?=$codigo?>" target="_blank"><?= $tipo ?></a></td>
				</td>
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
if ($_REQUEST['tipo'])
  {
  	
  $tipo = $_REQUEST['tipo'];
  $unicod = $_REQUEST['unicod'];
  $prgid = $_REQUEST['prgid'];
  $acaid = $_REQUEST['acaid'];
//  if ($prgid<>'') $wh .= " and a.prgid in('".$prgid."')";
  	if ( $tipo == 'A' )
	{
		$wh .= " and p.prgid in(".$prgid.")";
		$tit1 = 'Ações:'; $tit2 = 'Quantidade:'; 
		$sql = 'select p.prgcod ||  \'.\' || a.acacod ||  \'.\' || a.unicod || \' - \' || a.acadsc as descricao, count(prop2.acaid) as qtd, p.prgdsc, a.acaid as Codigo, a.acadsc from elabrev.v_propostas_acoes prop inner join elabrev.v_propostas_acoes prop2 ON prop2.acaid = prop.acaid inner join elabrev.ppaacao a ON a.acaid = prop.acaid inner join elabrev.ppaprograma p ON p.prgid = prop.prgid where 1=1 ' .$wh.' group by p.prgcod ||  \'.\' || a.acacod ||  \'.\' || a.unicod, p.prgdsc, a.acaid, a.acadsc';
	}
	elseif ($tipo == 'U') 
	{
		$wh .= " and prop.unicod in(".$unicod.")";
		$tit1 = 'Ações:'; $tit2 = 'Quantidade:'; 
		$sql = 'select distinct prop.acaid as codigo, a.acadsc as descricao, count(prop.acaid) as qtd from elabrev.v_propostas_acoes prop inner join elabrev.ppaacao a ON a.acaid = prop.acaid where 1 = 1 ' .$wh.' group by prop.acaid, a.acadsc order by a.acadsc ';
	}
	else if ($tipo == 'I') 
	{
		$wh1 = '';
		$wh = '';

		if ( $inclusao ) $wh .= ' or tipo = \'I\'';
		if ( $alteracao ) $wh .= ' or tipo = \'A\'';
		if ( $exclusao ) $wh .= ' or tipo = \'E\'';
		if ( $wh != '' ) $wh1 = ' where 1 = 2 '.$wh; else $wh1 = ' where 1 = 1';

		$wh .= " and prgid in(".$prgid.")";
		$tit1 = 'Indicadores:'; $tit2 = 'Tipo:'; 
		$sql = 'select indnum as codigo, inddsc as descricao, case tipo when \'I\' then \'Inclusão\' when \'A\' then \'Alteração\' when \'F\' then \'Fusão\' when \'M\' then \'Migração\' when \'E\' then \'Exclusão\' end as qtd from elabrev.v_propostas_indicadores ' .$wh1.' order by 2 ';
	}
/*	else
	{
		$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Vl. Distribuído:';
		$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.loccod as cod2,  a.acadsc as desc1, a.sacdsc as desc2, coalesce( SUM(ac.dpavalor) , 0) as valor from elabrev.ppaacao_orcamento a inner join unidade u on a.unicod = u.unicod " . $join_responsabilidade . " left join elabrev.despesaacao ac ON ac.acaid = a.acaid where a.prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' and u.unicod ='".$codigo."' ".$wh." group by a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod, a.loccod, a.acadsc, a.sacdsc order by a.acacod, a.unicod, a.loccod";
	}
*/
	?>
  
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="color:#006600;">
<tr style="color:#000000;">
	<td valign="top" style="width:24px;" nowrap>&nbsp;</td>
	  <td valign="top" style="width:132px;" nowrap><?=$tit1?></td>
	  <td valign="top"><?=$tit2?></td>
	  <td valign="top" align="right" style="width:95px;">&nbsp;</td>
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
	<td valign="top" colspan="<?if ( $tit3 ) print ''; else '4';?>" style="border-top: 1px solid #cccccc; padding:2px;" <? if ( $tipo != 'I' ) { ?> onclick="abreconteudo('geral/listaacaoproposta.php?&subnivel=3&acaid=<?=$codigo?>&prgid=<?=$prgid?>&inclusao=<?=$inclusao?>&exclusao=<?=$exclusao?>&alteracao=<?=$alteracao?>&fusao=<?=$fusao?>','<?=$codigo?>')" <? } ?> >
		<? if ( $tipo != 'I' ) { ?>
		<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$codigo?>">
		<?=$descricao?>
		<? } 
			else
			{
?>
		<a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/relindicadornovo&acao=A&indnum=<?=$codigo?>" target="_blank"><?=$descricao?></a></td>
<?				
			}
		?>
		
	</td>
	<td valign="top" style="border-top: 1px solid #cccccc; padding:2px;">
	<?=$qtd?>
	</td>
</tr>
<? if ( $tipo != 'I' ) { ?>
<tr bgcolor="<?=$marcado?>">
	<td id="td<?=$codigo?>" colspan="<? if ($listaprg == 'S') { print '3'; } else { print '5'; }?>" style="padding-left:65px;"></td>
</tr>
<?						}
}
?>
<?if ($tit5){?><tr><td colspan="6" align="right" style="color:000000;border-top: 2px solid #000000;">Total de Ações: (<?=$nlinhas+1?>)</td></tr><?}?>
</table>
<?$db -> close(); exit();
}
}
?>



     
         
