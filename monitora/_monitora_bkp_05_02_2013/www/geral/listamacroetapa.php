<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listamacroetapa.php
   Finalidade: permitir a listagem inteligente das etapas e macro-etapas
   */

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$ptoid=$_SESSION['ptoid'];
$pjeid=$_SESSION['pjeid'];
$dadosRequest = $_SESSION[ 'request' ];
$erroData = $_SESSION[ 'erroData' ];
$arrCodigos = explode( ',', $dadosRequest[ 'arrCod' ] );

$coordpje = $_SESSION['coordpje'];
$intPixelPasso = 20;

/*
//$sql = "select p.ptoid,ptoid_pai, p.ptotipo,p.ptocod,p.ptoordem, 
case when p.ptotipo='M' then 'M' when p.ptotipo='P' then 'E' end as tipo, 
ptodsc,to_char(ptodata_ini,'dd/mm/yyyy') as inicio, 
to_char(ptodata_fim,'dd/mm/yyyy') as termino 
from 
monitora.planotrabalho p where p.ptostatus='A' and p.ptoid in 
(select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid'].") 
and ptoid_pai=".$_REQUEST['ptoid']." order by p.ptotipo desc,p.ptoid_pai, p.ptoordem,p.ptocod";
*/

$sql = "select p.ptoid,ptoid_pai, p.ptotipo,p.ptocod,p.ptoordem, 
case when p.ptotipo='M' then 'M' when p.ptotipo='P' then 'E' end as tipo, 
ptodsc,to_char(ptodata_ini,'dd/mm/yyyy') as inicio, 
to_char(ptodata_fim,'dd/mm/yyyy') as termino 
from 
monitora.planotrabalho p where p.ptostatus='A' and p.ptoid in 
(select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid'].") 
and ptoid_pai=".$_REQUEST['ptoid']." order by p.ptoordem";

if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) $coordpje = true;

function existe_no_array( $array, $valor )
{
	for( $i = 0 ; $i < count( $array ) ; $i++ )
	{
		if( $valor == $array[ $i ] )
			return TRUE;
	}
	
	return FALSE;
}
?>
<? // <table cellspacing="0" cellpadding="0" border="0"  style="width:754px;color:#003F7E;" id="tblListaMacroEtapa"> ?>
<table cellspacing="0" cellpadding="0" border="0"  style="width:770px; color:#003F7E;" id="tblListaMacroEtapa">
	 <?
  $rs = @$db->carregar( $sql );
  if (  $rs && count($rs) > 0 )
  {
	 $i=0;
	 foreach ( $rs as $linha )
		{
			foreach($linha as $k=>$v) ${$k}=$v;
			switch( $linha[ 'ptotipo' ] )
			{
				case 'P':
			 	{
			 		$strTipo = 'E';
			 		break;
			 	}
			 	case 'M':
			 	{
			 		$strTipo = 'M';	
			 	}
			 }
		 
			$sql = "select ptoid from monitora.planotrabalho where ptostatus='A' and ptoid_pai=".$ptoid;
			$rs2 = @$db->carregar( $sql );
			if (  $rs2 && count($rs2) > 0 ) $filhos=1; else $filhos=0;
		    // exibe status
			$sqlStatus = "select t.tpsdsc as status, t.tpscor as cor from public.tiposituacao t inner join monitora.execucaopje e on e.tpscod = t.tpscod where e.ptoid=".$ptoid." order by e.expdata desc limit 1";
			$rsStatus = @$db->recuperar( $sqlStatus );
			$status = $rsStatus[ "status" ] ? $rsStatus[ "status" ] : "S/ avaliação";
			$cor = $rsStatus[ "cor" ] ? $rsStatus[ "cor" ] : "black";
			$sqlAlt =
				" select " .
					" pt.ptodsc, " .
					" pt.ptoid, " .
					" epobs.observacao, " .
					" pt.ptoprevistoexercicio as previsto, " .
					" sum( ep.exprealizado ) as realizado, " .
					" sum( ep.expfinanceiro ) as gasto, " .
					" ( ( sum( ep.exprealizado ) / pt.ptoprevistoexercicio ) * 100 ) as porcentagem " .
				" from monitora.planotrabalho pt " .
					" inner join monitora.execucaopje ep using ( ptoid ) " .
					" left join ( " .
						" select expobs as observacao, ptoid from monitora.execucaopje where ptoid = '12' order by expdata desc limit 1 " .
					" ) epobs using ( ptoid ) " .
				" group by pt.ptodsc, pt.ptoid, epobs.observacao, pt.ptoprevistoexercicio";
			$dadosAlt = $db->recuperar( $sqlAlt );
			$txtAlt =
				"Previsto: " . formata_valor( $dadosAlt['previsto'], 2 ) . "<br/>" .
				"Executado: " . formata_valor( $dadosAlt['realizado'], 2 ) . "<br/>" .
				"Gasto: " . formata_valor( $dadosAlt['gasto'], 2 ) . "<br/>" .
				"Percentual: " . formata_valor( $dadosAlt['porcentagem'], 0 ) . "%";
			if ( $dadosAlt['observacao'] )
			{
				$txtAlt .= "<br/><br/>" . $dadosAlt['observacao'];
			}
			$status = '<font color="'. $cor . '">' . $status . '</font>';
//			$status = '<span onmouseover="return escape(\'' . htmlentities( $txtAlt )  .'\')">' . $status . '</span>';
			$status = '<span onmouseover="SuperTitleOn(this,\'' . htmlentities( $txtAlt ) . '\')" onmouseout="SuperTitleOff(this)" >' . $status . '</span>';
			// FIM exibe status	
			
			$nivel = (integer) ($_REQUEST['nivel']); 
			
			if ($ptotipo=='M' and $filhos==1)
			{
			?>
			  <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';" style="border:none;" >
				<td style="width:90px; text-align:left; padding:3px;">
				<img border="0" src="../imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=$ptoid?>')">&nbsp;&nbsp;<img border="0" src="../imagens/incluimacroetapa.gif" title="Incluir Macro-Etapa dentro desta atividade." onclick="incluirmacroetapa('<?=$ptoid?>')">&nbsp;&nbsp;<img border="0" src="../imagens/incluietapa.gif" title="Incluir Etapa dentro desta atividade." onclick="incluiretapa('<?=$ptoid?>')">
				</td>
				<td style=" width:25px;text-align:right; padding:3px;"><?=$ptoordem?> </td>
				<td valign="top" style="width:30px; text-align:left; padding:3px;"><?=$tipo?></td>
			    <td style="width:395px; text-align:left; padding:3px;" onclick="abreconteudo('geral/listamacroetapa.php?nivel=<?=$nivel + 1 ?>&ptoid=<?=$ptoid?>','<?=$ptoid?>')">
						<span style="padding-left:<?=$intPixelPasso*$nivel?>px;">
							<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
							<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$ptoid?>">
							<b>
								<?=$ptodsc?>
							</b>
						</span>
				</td>
				<td style="width:70px; text-align:center; padding:3px;"
					onmouseover="this.childNodes[0].onmouseover()" onmouseout="this.childNodes[0].onmouseout()"
					>
						<?=$status?>
				</td>
				<?
					if( !$_SESSION[ 'showForm' ] && $_SESSION[ 'coordpje' ] )
					{
				?>
				<td style="width:60px; padding:3px;color:#003F7E;" onclick="altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#003F7E')">
							<?//verifica a existencia de erro na alteração da data
					   		if( $erroData  && existe_no_array( $arrCodigos, $ptoid ) )
							{
					      	?>
						      	<span id="dt_ini<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>><?= $dadosRequest[ 'dt_ini'.$ptoid ] ?></span>
						      	<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $dadosRequest[ 'dt_ini'.$ptoid ] ?>" />
						      	<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#000000', 1 );</script>
					      	<? 
							}
							else 
							{ 
							?>
								<span id="dt_ini<?=$ptoid?>"><?= $inicio ?></span>
								<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $inicio?>" />
						<?	} //Fim da verificação de erro na alteração da data	?>	
				   </td>
				   <td style="width:60px; padding:3px; color:#003F7E;" onclick="altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#003F7E')">
				        	<?//verifica a existencia de erro na alteração da data
						    if( $erroData  && existe_no_array( $arrCodigos, $ptoid ) )
							{
					      	?>
								<span id="dt_fim<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>><?= $dadosRequest[ 'dt_fim'.$ptoid ] ?></span>
								<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $dadosRequest[ 'dt_fim'.$ptoid ] ?>" />
								<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#000000', 1 );</script>
							<?
							}
							else 
							{
							?>
								<span id="dt_fim<?=$ptoid?>"><?= $termino ?></span>
								<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $termino?>" />
							<?
							}//Fim da verificação de erro na alteração da data 
							?>
				     </td>
					<? 
     				 $ok=0;
		          $sql = "select ptosnaprovado from monitora.planotrabalho where ptoid='$ptoid' and pjeid=$pjeid ";
       				$ok=$db->pegaUm($sql);
      				print '<td style="width:40px; padding:3px;" ><input type="checkbox" name="aprovpto[]" value="'.$ptoid.'"';
	  				if ($ok=='t') {print " checked";}; print "></td>";
				} 
				else 
				{
				?>
			     <td style="width:60px; text-align:center; padding:3px;"><?=$inicio?> </td>
			     <td style="width:60px; text-align:center; padding:3px;" ><?=$termino?> </td>  
			     <td style="width:40px; text-align:center; padding:3px;" ></td> 
				<?
				 }	?>
				</tr>
			<? } else { ?>
				<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
					<td style=" width:90px; text-align:left; padding:3px;">
					<img border="0" src="../imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=$ptoid?>')">&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir a atividade." onclick="excluir_pto('<?=$ptoid?>','<?=$ptocod?>')">
						<?
if ($ptotipo=='M') { ?>
						&nbsp;&nbsp;<img border="0" src="../imagens/incluimacroetapa.gif" title="Incluir Macro-Etapa dentro desta atividade." onclick="incluirmacroetapa('<?=$ptoid?>')">&nbsp;&nbsp;<img border="0" src="../imagens/incluietapa.gif" title="Incluir Etapa dentro desta atividade." onclick="incluiretapa('<?=$ptoid?>')">
						<?}?>
						
					</td>
					<td style="width:25px; text-align:right; padding:3px;"><?=$ptoordem?></td>
					<td style="width:30px; text-align:left; padding:3px;"><?= $strTipo ?></td>
					<td style="width:395px; text-align:left; padding:3px;">
						<span style="padding-left:<?=$intPixelPasso*$nivel?>px;">
							<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
							<i><?=$ptodsc?></i>
						</span>
					</td>
					<td style="width:70px; text-align:center; padding:3px;"><?=$status?></td>
						<?
					if( !$_SESSION[ 'showForm' ] && $_SESSION[ 'coordpje' ] )
					{
					?>
						<td style="width:60px; padding:3px;color:#003F7E;" onclick="altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#003F7E')">
								<?//verifica a existencia de erro na alteração da data
					      		if( $erroData  && existe_no_array( $arrCodigos, $ptoid ) )
					      		{
								?>
									<span id="dt_ini<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>>
										<?= $dadosRequest[ 'dt_ini'.$ptoid ] ?>
									</span>
									<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $dadosRequest[ 'dt_ini'.$ptoid ] ?>" />
									<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_ini', '<?=$ptoordem ?>', '#000000', 1 );</script>
								<? 
					      		}
					      		else 
					      		{
					      		?>
									<span id="dt_ini<?=$ptoid?>"><?= $inicio ?></span>
					      			<input type="hidden" name="dt_ini<?=$ptoid?>" value="<?= $inicio?>" />
					      	<?	} //Fim da verificação de erro na alteração da data	?>	
							</td>
							<td style="width:60px; padding:3px;color:#003F7E;" onclick="altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#003F7E')">
								<?//verifica a existencia de erro na alteração da data
								if( $erroData  && existe_no_array( $arrCodigos, $ptoid ) )
								{
									$jsCode .= "altera_data('$ptoid', 'dt_fim', '$ptoordem', '#000000', 1 );";
								?>
									<span id="dt_fim<?=$ptoid?>" <? if( $erroData[ 'ptoid' ] == $ptoid )echo 'style="color:#ff0000;"' ?>>
										<?= $dadosRequest[ 'dt_fim'.$ptoid ] ?>
									</span>
									<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $dadosRequest[ 'dt_fim'.$ptoid ] ?>" />
									<script type="text/javascript">altera_data('<?=$ptoid?>', 'dt_fim', '<?=$ptoordem ?>', '#000000', 1 );</script>
								<?
								}
								else
								{
								?>
									<span id="dt_fim<?=$ptoid?>"><?= $termino ?></span>
									<input type="hidden" name="dt_fim<?=$ptoid?>" value="<?= $termino?>" />
							<?	} //Fim da verificação de erro na alteração da data	?>
			        		</td>
							<? 
     				 	$ok=0;
		          	$sql = "select ptosnaprovado from monitora.planotrabalho where ptoid='$ptoid' and pjeid=$pjeid ";
       				$ok=$db->pegaUm($sql);
      				print '<td style="width:40px; padding:3px;" ><input type="checkbox" name="aprovpto[]" value="'.$ptoid.'"';
	  				if ($ok=='t') {print " checked";}; print "></td>";
					} 
			       	else 
			       	{
			       	?>
					<td style="width:60px; text-align:center; padding:3px;"><?=$inicio?> </td>
					<td style="width:60px; text-align:center; padding:3px;"><?=$termino?> </td> 
					<td style="width:40px; text-align:center; padding:3px;"></td> 
							<? 
	
					}	?>
				</tr>
			<?	}	?>
			<tr>
				<td style="width:770px; padding:3px;" colspan="8" id="td<?=$ptoid?>"></td>
			</tr>
		<?
		$i++;
	}
}
else print "</table><font color='red'>Não foram encontrados Registros</font>";
?>
</table>
<JSCode>
	<?=$jsCode?>
</JSCode>
<? $db -> close(); exit(); ?>


