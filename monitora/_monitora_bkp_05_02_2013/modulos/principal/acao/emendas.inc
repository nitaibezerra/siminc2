<?
 /*
   Sistema Simec
   Setor responsï¿½vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Adonias Malosso
   Programador: Adonias Malosso (malosso@gmail.com)
   Tï¿½tulo: emendas.inc
   Finalidade: apresentar as emendas e sua execuï¿½ï¿½o financeira
   */
require_once "config.inc";

//Recupera tudo que foi passado por REQUEST e instancia as variï¿½ï¿½eis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;

// filtros
if (!isset($gndcod)) $gndcod = array(3,4);

//Agrupamento da listagem
if (!$ordemgrupo) $ordemgrupo='A';
if ($ordemgrupo=='L') $agrupadopor = 'Localizador';
elseif ($ordemgrupo=='U') $agrupadopor = 'Unidade';
elseif ($ordemgrupo=='T') $agrupadopor = 'Autor';
else $agrupadopor = 'Ação';

//Sumï¿½rio
switch($sumariogrupo) {
	case "unicod": $sumariogrupo = 'unicod'; break;
	case "emdsglpartidoautor": $sumariogrupo = 'emdsglpartidoautor'; break;
	case "foncod": $sumariogrupo = 'foncod'; break;
	case "gndcod": $sumariogrupo = 'gndcod'; break;
	case "acaptres": $sumariogrupo = 'acaptres'; break;
	case "funcprog": default:
		$sumariogrupo = 'funcprog';
	break;
}
//dbg($sumariogrupo);

//Ordem da listagem
if (!$ordemlista) $ordemlista = 2;

//Direcionamento da ordenaï¿½ï¿½o ASC / DESC
if ($ordemlistadir <> 'DESC') {
	$ordemlistadir = 'ASC';
	$ordemlistadir2 = 'DESC';
}
else {
	$ordemlistadir2 = 'ASC';
}

//filtros de tipo de Aï¿½ï¿½o -- somente emendas
$wh .= " AND a.acasnemenda='t' ";
if (isset($acaptres) && $acaptres) {
	$wh .= " AND a.acaptres = '$acaptres'";
}

$whjoin = "";
if (isset($emdsglpartidoautor) && (@count($emdsglpartidoautor)>=1 && (bool)$emdsglpartidoautor[0]) ) {
	$buffer = implode("', '", $emdsglpartidoautor);
	$whjoin .= " AND e.emdsglpartidoautor IN ('$buffer')";
}
if (isset($emdufautor) && (@count($emdufautor)>=1 && (bool)$emdufautor[0]) ) {
	$buffer = implode("', '", $emdufautor);
	$whjoin .= " AND e.emdufautor IN ('$buffer')";
}
if (isset($emdcodtipoautor) && (@count($emdcodtipoautor)>=1 && (bool)$emdcodtipoautor[0]) ) {
	$buffer = implode("', '", $emdcodtipoautor);
	$whjoin .= " AND e.emdcodtipoautor IN ('$buffer')";
}
if (isset($emdnomeautor) && (@count($emdnomeautor)>=1 && (bool)$emdnomeautor[0]) ) {
	$buffer = implode("', '", $emdnomeautor);
	$whjoin .= " AND e.emdnomeautor IN ('$buffer')";
}
if (isset($gndcod) && (@count($gndcod)>=1 && (bool)$gndcod[0])) {
	$buffer = implode("', '", $gndcod);
	$whjoin .= " AND e.gndcod IN ('$buffer')";
}
if (isset($foncod) && $foncod) {
	$whjoin .= " AND e.foncod = '$foncod'";
}
if (isset($mapcod) && $mapcod) {
	$whjoin .= " AND e.mapcod = '$mapcod'";
}

// filtros disponï¿½eis
$sqlFiltro = array(
	"partidoAutor"=>"SELECT DISTINCT emdsglpartidoautor AS descricao, emdsglpartidoautor AS codigo FROM monitora.emenda WHERE prgano = '".$_SESSION['exercicio']."'"
	,"ufAutor"=>"SELECT DISTINCT emdufautor AS descricao, emdufautor AS codigo FROM monitora.emenda WHERE prgano = '".$_SESSION['exercicio']."'"
	,"tipoAutor"=>"SELECT taucod AS codigo, taudescricao AS descricao FROM monitora.tipoautoremenda"
	,"nomeAutor"=>"SELECT DISTINCT emdnomeautor AS descricao, emdnomeautor AS codigo FROM monitora.emenda WHERE prgano = '".$_SESSION['exercicio']."'"
	,"codGnd"=>"SELECT DISTINCT gndcod AS codigo, gndcod AS descricao FROM monitora.emenda WHERE prgano = '".$_SESSION['exercicio']."'"
	,"lsPtres"=>"SELECT DISTINCT acaptres AS valor, acaptres AS descricao FROM monitora.acao WHERE acaptres LIKE '%s%' AND prgano = '".$_SESSION['exercicio']."' AND acasnemenda = 't' ORDER BY acaptres"
	,"lsFonte"=>"SELECT DISTINCT f.foncod AS valor, f.foncod || ' - ' || f.fondsc AS descricao FROM public.fonterecurso f INNER JOIN monitora.emenda e ON f.foncod = e.foncod AND e.prgano = '" . $_SESSION['exercicio'] . "' WHERE f.foncod LIKE '%s%' ORDER BY f.foncod"
	,"lsModAplicacao"=>"SELECT DISTINCT m.mapcod AS valor, m.mapcod || ' - ' || m.mapdsc AS descricao FROM public.modalidadeaplicacao m INNER JOIN monitora.emenda e ON m.mapcod = e.mapcod AND e.prgano = '" . $_SESSION['exercicio'] . "' WHERE m.mapcod LIKE '%s%' ORDER BY m.mapcod"	
);


//Cabeï¿½alho do sistema
include APPRAIZ . "includes/cabecalho.inc";
?>
<br>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<?
if ($ordemgrupo=='A') {
	$subtit1 = 'de Ações'; 
	$subtit2 = 'da Ação'; 
	$sql = "select a.prgcod, a.acacod as codigo, a.acadsc as descricao, a.prgano, count(*) as numacoes from monitora.acao a INNER JOIN  monitora.emenda e ON e.acaid = a.acaid " . $whjoin . " where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."' ".$wh." group by a.prgcod, a.acacod , a.acadsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
}
elseif ($ordemgrupo=='U') {
	$subtit1 = 'de Unidades'; 
	$subtit2 = 'da Unidade'; 
	$sql = "select a.unicod, a.unicod as codigo, u.unidsc as descricao, a.prgano, count(*) as numacoes from monitora.acao a INNER JOIN  monitora.emenda e ON e.acaid = a.acaid " . $whjoin . " inner join unidade u on a.unicod=u.unicod where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."'  ".$wh." group by a.unicod, a.unicod , u.unidsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
}
elseif ($ordemgrupo=='L') {
	$subtit1 = 'de Localizadores'; 
	$subtit2 = 'do Localizador';
	$sql = "select a.loccod, a.regcod as codigo, r.regdsc as descricao, a.prgano, count(*) as numacoes from monitora.acao a INNER JOIN  monitora.emenda e ON e.acaid = a.acaid " . $whjoin . " left join regiao r on a.regcod=r.regcod where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."'  ".$wh." group by a.loccod, a.regcod , r.regdsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
}
elseif ($ordemgrupo=='T') {
	$subtit1 = 'de Autores';
	$subtit2 = 'do Autor';
	$sql = "select e.emdcodautor AS codigo, e.emdnomeautor as descricao, e.emdsglpartidoautor as partido, count(*) as numacoes from monitora.acao a INNER JOIN  monitora.emenda e ON e.acaid = a.acaid " . $whjoin . " where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."' ".$wh." group by e.emdsglpartidoautor, e.emdcodautor, e.emdnomeautor, a.prgano order by ".$ordemlista." ".$ordemlistadir;
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Consultar A&ccedil;&otilde;es e Emendas','Clique no CÃ³digo para ver detalhes - Total '.$subtit1.' com emendas encontradas ('.($nlinhas+1).')');?>
<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
    <form name="formulario" method="post">
	<input type="Hidden" name="ordemlista" value="<?=$ordemlista?>">
	<input type="Hidden" name="ordemlistadir" value="<?=$ordemlistadir?>">
	<tr>
		<td class="SubTituloDireita">Partido:</td>
		<td style="color:#000080;" align="center">
			<?=$db->monta_combo_multiplo("emdsglpartidoautor", $sqlFiltro["partidoAutor"], 'S', '-- Todos --', '', '', '', 5) ?>
		</td>
		<td class="SubTituloDireita">UF:</td>
		<td style="color:#000080;" align="center">
			<?=$db->monta_combo_multiplo("emdufautor", $sqlFiltro["ufAutor"], 'S', '-- Todos --', '', '', '', 5) ?>
		</td>
		<td class="SubTituloDireita">Tipo:</td>
		<td style="color:#000080;" align="center">
			<?=$db->monta_combo_multiplo("emdcodtipoautor", $sqlFiltro["tipoAutor"], 'S', '-- Todos --', '', '', '', 5) ?>
		</td>
		<td class="SubTituloDireita">Autor:</td>
		<td style="color:#000080;" align="center">
			<?=$db->monta_combo_multiplo("emdnomeautor", $sqlFiltro["nomeAutor"], 'S', '-- Todos --', '', '', '', 5) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">PTRES:</td>
		<td nowrap="nowrap"><?=campo_texto_ajax($sqlFiltro["lsPtres"], "acaptres", "acaptres", "Programa de trabalho resumido", $acaptres, 8, 5, '', 'left')?></td>
		<td class="SubTituloDireita">Fonte:</td>
		<td nowrap="nowrap"><?=campo_texto_ajax($sqlFiltro["lsFonte"], "foncod", "foncod", "Fonte de Recursos", $foncod, 5, 3, '', 'left')?></td>
		<td class="SubTituloDireita">Mod. Aplicação:</td>
		<td nowrap="nowrap"><?=campo_texto_ajax($sqlFiltro["lsModAplicacao"], "mapcod", "mapcod", "Modalidade de Aplicação", $mapcod, 5, 3, '', 'left')?></td>
		<td class="SubTituloDireita">GND:</td>
		<td nowrap="nowrap"><?=$db->monta_checkbox("gndcod[]", $sqlFiltro["codGnd"], $gndcod);?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Agrupar por:</td>		
		<td style="color:#800000;" nowrap="nowrap" colspan="6">
	    	<input type="radio" name="ordemgrupo" value="A" <?if ($ordemgrupo=='A') print 'checked';?>> A&ccedil;&atilde;o
		    <input type="radio" name="ordemgrupo" value="U" <?if ($ordemgrupo=='U') print 'checked';?>> Unidade
		    <input type="radio" name="ordemgrupo" value="L" <?if ($ordemgrupo=='L') print 'checked';?>> Localizador
	    	<input type="radio" name="ordemgrupo" value="T" <?if ($ordemgrupo=='T') print 'checked';?>> Autor	    
		</td>
		<td class="SubTituloDireita" style="text-align:center;" rowspan="2"><input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="filtra();"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sumarizar por:</td>		
		<td style="color:#800000;" nowrap="nowrap" colspan="6">
	    	<input type="radio" name="sumariogrupo" value="funcprogr" <?= ($sumariogrupo == 'funcprog' ? 'checked="checked"' : '') ?>> Funcional
	    	<input type="radio" name="sumariogrupo" value="acaptres" <?= ($sumariogrupo == 'acaptres' ? 'checked="checked"' : '') ?>> Ptres
		    <input type="radio" name="sumariogrupo" value="unicod" <?= ($sumariogrupo == 'unicod' ? 'checked="checked"' : '') ?>> Unidade
		    <input type="radio" name="sumariogrupo" value="emdsglpartidoautor" <?= ($sumariogrupo == 'emdsglpartidoautor' ? 'checked="checked"' : '') ?>> Partido
	    	<input type="radio" name="sumariogrupo" value="foncod" <?= ($sumariogrupo == 'foncod' ? 'checked="checked"' : '') ?>> Fonte
	    	<input type="radio" name="sumariogrupo" value="gndcod" <?= ($sumariogrupo == 'gndcod' ? 'checked="checked"' : '') ?>> GND
		</td>		
	</tr>
	</form>
</table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"  width="55"><?if ($ordemlista == '2') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('2','<?=$ordemlistadirnova?>');" title="Ordenar por Cï¿½digo"><strong>CÃ³digo</strong></label></td>
      <td valign="top" class="title"><?if ($ordemlista == '3') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('3','<?=$ordemlistadirnova?>');" title="Ordenar por TÃ­tulo <?=$subtit2?>"><strong>TÃ­tulo <?=$subtit2?></strong></label></td>
      <?if($ordemgrupo=='T') { ?><td valign="top" class="title" width="80"><?if ($ordemlista == '3') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('3','<?=$ordemlistadirnova?>');" title="Ordenar por Partido <?=$subtit2?>"><strong>Partido <?=$subtit2?></strong></label></td><? } ?>
      <td valign="top" class="title" width="45"><?if ($ordemlista == '5') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('5','<?=$ordemlistadirnova?>');"  title="Ordenar por Total"><strong>Total</strong></label></td>
    </tr>
</thead>
<tbody>
<?
$totalacoes = 0;

//prepara os arrays para enviar para o abreconteudo
$URLemdsglpartidoautor 	= urlencode(serialize($emdsglpartidoautor));
$URLemdcodtipoautor		= urlencode(serialize($emdcodtipoautor));
$URLemdufautor			= urlencode(serialize($emdufautor));
$URLemdnomeautor		= urlencode(serialize($emdnomeautor));
$URLgndcod				= urlencode(serialize($gndcod));

for ($i=0; $i<=$nlinhas;$i++){
	$res = $db->carrega_registro($RS,$i);

	// a linha abaixo transforma em variï¿½eis todos os campos do array
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

	if ($descricao=='')
		$descricao = '<font color="red">Não atribuído pelo SIGPLAN</font>';

	$totalacoes = $totalacoes + $numacoes;
	$marcado = $i % 2 ? '#F7F7F7' : '';
?>
	<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
		<td><?= ($ordemgrupo=='A' ? $prgcod.'.'.$codigo : $codigo) ?></td>
		<td onclick="abreconteudo('geral/listaemenda.php?codigo=<?=($ordemgrupo=='A' ? $prgcod.'.'.$codigo : ( $ordemgrupo=='L'? $loccod : $codigo))?>&ordem=<?=$ordemgrupo?>&sumariogrupo=<?=$sumariogrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>&emdsglpartidoautor=<?=$URLemdsglpartidoautor?>&emdcodtipoautor=<?=$URLemdcodtipoautor?>&emdufautor=<?=$URLemdufautor?>&emdnomeautor=<?=$URLemdnomeautor?>&gndcod=<?=$URLgndcod?>&foncod=<?=$foncod?>&mapcod=<?=$mapcod?>&acaptres=<?=$acaptres?>','<?=$i?>-<?=$codigo?>')" style="color:#003c7b">
			<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>"/> <?=$descricao?>
		</td>
        <? if($ordemgrupo=='T') { ?><td align="center" width="80"><?=$partido?></td><? } ?>
		<td align=right>(<?=$numacoes?>)</td>
	 </tr>
	 <tr bgcolor="<?=$marcado?>">
		 <td></td>
		 <td id="td<?=$i?>-<?=$codigo?>"></td>
		 <td></td>
	 </tr>
<?
}
$marcado = $i % 2 ? '#F7F7F7' : '';
?>
    <tr class="<?=$marcado?>">
	<td></td>
      <td valign="top" class="title"> 
	  <table width="100%"><tr><td><strong>Total <?=$subtit1?>  com emendas encontradas: <?=$i?></strong></td>
	  <td align="right"><strong>Total de emendas:</strong></td></tr></table>
	  </td>
	  <td align="right"><strong><?=$totalacoes?></strong></td>
   </tr>
</tbody>
</table>

      </td>
    </tr>
  </table>
  </center>
</div>
<script language="JavaScript">
function filtra()
{
document.formulario.submit();
}

function ordena(ordem, direcao)
{
document.formulario.ordemlista.value=ordem;
document.formulario.ordemlistadir.value=direcao;
document.formulario.submit();
}
</script>
