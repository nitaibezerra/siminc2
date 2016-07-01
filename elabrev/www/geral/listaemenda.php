<?
include_once("config.inc");
header('Content-Type: text/html; charset=iso-8859-1');
include "includes/classes_simec_er.inc";
include "includes/funcoes.inc";
$db = new cls_banco();
$ordem = $_REQUEST['ordem'];
$acaid = str_replace('|',chr(39),$_REQUEST['acaid']);
$tipoacao1 = $_REQUEST['tipoacao1'];
$tipoacao2 = $_REQUEST['tipoacao2'];
$tipoacao3 = $_REQUEST['tipoacao3'];

//filtros de tipo de ação
if ($tipoacao1 and !$tipoacao2 and !$tipoacao3) $wh = " and a.acasnrap='f' and a.acasnemenda='f' ";
elseif ($tipoacao1 and $tipoacao2 and !$tipoacao3) $wh = " and a.acasnemenda='f' ";
elseif ($tipoacao1 and !$tipoacao2 and $tipoacao3) $wh = " and a.acasnrap='f'";
elseif (!$tipoacao1 and $tipoacao2 and $tipoacao3) $wh = " and a.acasnrap='t' and a.acasnemenda='t' ";
elseif (!$tipoacao1 and $tipoacao2 and !$tipoacao3) $wh = " and a.acasnrap='t' ";
elseif (!$tipoacao1 and !$tipoacao2 and $tipoacao3) $wh = " and a.acasnemenda='t' ";
else $wh = "";

$codigo = $_REQUEST['codigo'];

//$emdsglpartidoautor
//$emdufautor
//$emdcodtipoautor
//$emdnomeautor

//prepara os arrays para enviar para o abreconteudo
$emdsglpartidoautor = unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdsglpartidoautor"]) : $_REQUEST["emdsglpartidoautor"]);
$emdcodtipoautor	= unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdcodtipoautor"]) : $_REQUEST["emdsglpartidoautor"]);
$emdufautor			= unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdufautor"]) : $_REQUEST["emdsglpartidoautor"]);
$emdnomeautor		= unserialize(get_magic_quotes_gpc() ? stripslashes($_REQUEST["emdnomeautor"]) : $_REQUEST["emdsglpartidoautor"]);

//var_dump($emdsglpartidoautor);
//var_dump($emdcodtipoautor);
//var_dump($emdufautor);
//var_dump($emdnomeautor);

$whjoin = "";
if (isset($emdsglpartidoautor) && (@count($emdsglpartidoautor)>=1 && (bool)$emdsglpartidoautor[0]) ) {
	$buffer = implode("', '", $emdsglpartidoautor);
	$whjoin .= " AND emd.emdsglpartidoautor IN ('$buffer')";
}
if (isset($emdufautor) && (@count($emdufautor)>=1 && (bool)$emdufautor[0]) ) {
	$buffer = implode("', '", $emdufautor);
	$whjoin .= " AND emd.emdufautor IN ('$buffer')";
}
if (isset($emdcodtipoautor) && (@count($emdcodtipoautor)>=1 && (bool)$emdcodtipoautor[0]) ) {
	$buffer = implode("', '", $emdcodtipoautor);
	$whjoin .= " AND emd.emdcodtipoautor IN ('$buffer')";
}
if (isset($emdnomeautor) && (@count($emdnomeautor)>=1 && (bool)$emdnomeautor[0]) ) {
	$buffer = implode("', '", $emdnomeautor);
	$whjoin .= " AND emd.emdnomeautor IN ('$buffer')";
}
if ($acaid)
{
$wh .= " and a.acaid in('".$acaid."')";
$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Unidade'; $tit4 = 'Localizador'; $tit5 = 'Responsabilidade';
$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.unicod as cod2,  a.acadsc as desc1, b.unidsc as desc2, a.loccod as cod3, a.sacdsc as desc3 from acao a INNER JOIN  dbemd.emenda emd ON emd.acaid = a.acaid " . $whjoin . " left join unidade b on a.unicod = b.unicod where a.prgano = '" . $_SESSION["exercicio"] . "' and a.acastatus='A' ".$wh." order by a.acacod, a.unicod, a.loccod";
}
elseif ($ordem == 'L') 
{
	$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Unidade'; $tit4 = 'Localizador';
	$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.unicod as cod2,  a.acadsc as desc1, b.unidsc as desc2, a.loccod as cod3, a.sacdsc as desc3 from acao a INNER JOIN dbemd.emenda emd ON emd.acaid = a.acaid " . $whjoin . " left join unidade b on a.unicod = b.unicod where a.prgano = '" . $_SESSION["exercicio"] . "' and a.acastatus='A' and a.regcod ='".$codigo."' ".$wh." order by a.acacod, a.unicod, a.loccod";
} 
elseif ($ordem == 'U') 
{	
	$tit1 = 'Código:'; $tit2 = 'Ação:'; $tit3 = 'Localizador:';
	$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.acacod as cod1, a.loccod as cod2,  a.acadsc as desc1, a.sacdsc as desc2 from acao a INNER JOIN  dbemd.emenda emd ON emd.acaid = a.acaid " . $whjoin . " inner join unidade b on a.unicod = b.unicod where a.prgano = '" . $_SESSION["exercicio"] . "' and a.acastatus='A' and a.unicod ='".$codigo."' ".$wh." order by a.acacod, a.unicod, a.loccod";
} 
else
{
	$tit1 = 'Código:'; $tit2 = 'Unidade:'; $tit3 = 'Localizador:';
	$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.sacdsc, b.unidsc, a.unicod as cod1, a.loccod as cod2,  b.unidsc as desc1, a.sacdsc as desc2 from acao a INNER JOIN  dbemd.emenda emd ON emd.acaid = a.acaid " . $whjoin . " left join unidade b on a.unicod = b.unicod where a.prgano = '" . $_SESSION["exercicio"] . "' and a.acastatus='A' and a.prgcod||'.'||a.acacod ='".$codigo."' ".$wh." order by a.acacod, a.unicod, a.loccod";
} 
//var_dump($sql);
?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
	<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top"><?=$tit1?></td>
	  <td valign="top"><?=$tit2?></td>
	  <td valign="top"><?=$tit3?></td>
	  <?if ($tit4){?><td valign="top"><?=$tit4?></td><?}?>
	  <?if ($tit5){?><td valign="top"><?=$tit5?></td><?}?>
    </tr>
<?
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

if ($nlinhas<0) {
	print "</table><font color='red'>Não foram encontrados Registros</font>";
}
else {
	for ($i=0; $i<=$nlinhas;$i++) {
		$res = $db->carrega_registro($RS,$i);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		
		//Relatorio Resumido
		$cabecalho = array('Func. Progr.','Partido','UF','Autor','Dotação Disponível','Valor Empenhado','Valor Liquidado','Valor Pago');
		$sqlEmenda = "SELECT funcprog, emdsglpartidoautor, emdufautor, emdnomeautor, exeautorizado, exeempenhado, exeliquidado, exepago FROM dbemd.vw_emenda_execucao emd WHERE acaid = $acaid $whjoin";
	?>
	<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
      <td valign="top" width="12" style="padding:2px;"><img src="imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><a href="simec_er.php?modulo=principal/acao/monitoraemenda&acao=C&acaid=<?=$acaid?>&prgid=<?=$prgid?>" style="color:#003366;"><?=$prgcod?>.<?=$acacod?>.<?=$unicod?></a></td>
	  <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$cod1?> - <?=$desc1?></td>
	  <td valign="top" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$cod2?> - <?=$desc2?></td>
	  <?if ($tit4){?><td valign="top" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$cod3?> - <?=$desc3?></td><?}?>
	  <?if ($tit5){?><td valign="top" style="border-top: 1px solid #cccccc; padding:2px; color:#006600"><?$db->monta_lista_simples("select pfldsc from usuarioresponsabilidade u inner join perfil p on u.pflcod=p.pflcod where usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and u.acaid='".$acaid."'","",100,20)?></td><?}?>
    </tr>
	<tr>
	<td valign="top" width="12">&nbsp;</td>
	<td colspan="6" style="border-bottom: 2px solid black;"><?
		$db->monta_lista_simples($sqlEmenda,$cabecalho,300,20,'S', '100%');
	?>
	</td></tr>
	<tr><td colspan="7">&nbsp;</td></tr>
	<?
	}
	?>
	<?if ($tit5){?><tr><td colspan="6" align="right" style="color:000000;border-top: 2px solid #000000;">Total de Ações: (<?=$nlinhas+1?>)</td></tr><?}?>
	</table>
	<?
	$db->close();
	exit();
}
?>