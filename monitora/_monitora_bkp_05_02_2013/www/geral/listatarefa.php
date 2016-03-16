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

function projetoaberto()
 {
 	 global $db;
	 
 	// verifica se o projeto esta aberto para ser acompanhado,	//
 	// ou seja, se ele nao esta concluido, cancelado etc.		//
 	
	/*
	1;"Atrasado";"A";"red            "
2;"Cancelado";"A";"red            "
3;"Concluído";"A";"blue           "
4;"Em dia";"A";"green          "
5;"Não iniciado";"A";"yellow         "
6;"Paralisado";"I";"red            "
7;"Suspenso";"A";"black          "
8;"Sem andamento";"A";"black          "
9;"Iniciado";"A";"blue           "
10;"Fase de planejamento";"A";"yellow         "
11;"Fase de Execução";"A";"black          "
12;"Fase de Encerramento";"A";"black          "

	*/
	
	$sql = ' SELECT ' .
				'tpscod' .
			' FROM ' .
				'monitora.projetoespecial' .
			' WHERE ' .
				'pjeid' . ' = ' . $_SESSION['pjeid'];
	$sit=$db->pegaUm($sql);
	if ($sit=='11' or $sit=='10')
	{
		// se o projeto estiver Fase de planejamento ou Fase de Execução então pode acompanhar
		return true;
	}
//	else
	{
		return false;
	}
}

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



$statusprojeto= projetoaberto();
$sql = "select ptonivel from monitora.planotrabalho where pjeid=$pjeid and ptostatus='A' order by ptonivel desc limit 1";
$maiornivel=$db->pegaum($sql);

$sql = "select p.ptoid,ptoid_pai, p.ptotipo,p.ptocod,p.ptoordem, 
case when p.ptotipo='M' then 'M' when p.ptotipo='P' then 'E' end as tipo, 
ptodsc,to_char(ptodata_ini,'dd/mm/yyyy') as inicio, 
to_char(ptodata_fim,'dd/mm/yyyy') as termino 
from 
monitora.planotrabalho p where p.ptostatus='A' and p.ptoid in 
(select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid'].") 
and ptoid_pai=".$_REQUEST['ptoid']." order by ptocod::integer * 10 ^($maiornivel-ptonivel+1)";

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
<!--<table cellspacing="0" cellpadding="0" border="0"  style="width:770px; color:#003F7E;" id="tblListaMacroEtapa">-->
	 <?
  $rs = @$db->carregar( $sql );
  if (  $rs && count($rs) > 0 )
  {
	 $i=0;
	 foreach ( $rs as $linha )
		{
						?>
			 <table cellspacing="0" cellpadding="0" border="0"  style="width:700px;color:#003F7E;" id="tblListaMacroEtapa"> 
			 <?
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
			$sqlStatus = "select t.tpsdsc as status, t.tpscor as cor from public.tiposituacao t inner join monitora.execucaopto e on e.tpscod = t.tpscod where e.ptoid=".$ptoid." order by e.expdata desc limit 1";
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
					" inner join monitora.execucaopto ep using ( ptoid ) " .
					" left join ( " .
						" select expobs as observacao, ptoid from monitora.execucaopto where ptoid = '12' order by expdata desc limit 1 " .
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
			
			if ($filhos==1)
			{
			?>
<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
			<td style="width:60px; text-align:left; padding:3px;">
            <img border="0" src="../imagens/alterar.gif" title="Acompanhar a Tarefa." onclick="editartarefa('<?=$ptoid?>')">            
          </td>
			<td style="width:40px; text-align:right; padding:3px;"><?=$ppa.'&nbsp;&nbsp;'.$ptoordem ?></td>
			<td style="width:500px; text-align:left; padding:3px;" onclick="abreconteudo('geral/listatarefa.php?nivel=<?=$nivel + 1 ?>&ptoid=<?=$ptoid?>','<?=$ptoid?>')">
				<span style="padding-left:<?=$intPixelPasso*$nivel?>px;">
				<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
				<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$ptoid?>">
				<b>	<?=mostracod($ptoid).'-'.$ptodsc?></b>
				</span>
			</td>
		</tr>
			<? } else { ?>
<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
				<td style="width:60px; text-align:left; padding:3px;">
            <img border="0" src="../imagens/alterar.gif" title="Acompanhar a Tarefa." onclick="editartarefa('<?=$ptoid?>')">            
          </td>
				<td style="width:40px; text-align:right; padding:3px;"><?=$ppa.'&nbsp;&nbsp;'.$ptoordem ?></td>
   				<td style="width:500px; text-align:left; padding:3px;">
				<span style="padding-left:<?=$intPixelPasso*$nivel?>px;">
				<img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0">
				<i><?=mostracod($ptoid).'-'.$ptodsc?></i>
				</span>
			</td>
		</tr>
			<?	}	?>
			<tr>
				<td style="width:700px; " colspan="3" id="td<?=$ptoid?>"></td>
			</tr>
		<?
		$i++;
	}
}
//else print "</table><font color='red'>Não foram encontrados Registros</font>";
else print "<font color='red'>Não foram encontrados Registros</font>";
?>
<!--</table>-->
<JSCode>
	<?=$jsCode?>
</JSCode>
<? $db -> close(); exit(); ?>


