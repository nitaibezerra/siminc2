<?
 /*
   Sistema Simec
   Setor responsï¿½vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Adonias Malosso
   Programador: Adonias Malosso (malosso@gmail.com)
   Tï¿½tulo: recursosemendas.inc
   Finalidade: emissÃ£o de chequinho para recursos de emendas liberados
   */
require_once "config.inc";

//Recupera tudo que foi passado por REQUEST e instancia as variï¿½ï¿½eis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;

//Cabeï¿½alho do sistema
include APPRAIZ . "includes/cabecalho.inc";

//
// RELATÓRIO DE LIBERAÇÃO DE RECURSOS
$libid="";
$whlib = array();
if(is_array($_REQUEST["libid"]) && @count($_REQUEST["libid"])>0) {
	$libid = implode(", ", $_REQUEST["libid"]);
}
elseif(isset($_REQUEST["libid"])) {
	$libid = (integer)$_REQUEST["libid"];
}

if($libid) {
	$sqlEmenda = <<<EOS
		SELECT 
			(((COALESCE(a.funcod::text || '.'::text, ''::text)
				|| COALESCE(a.sfucod::text || '.'::text, ''::text)) 
				|| COALESCE(a.prgcod::text || '.'::text, ''::text)) 
				|| COALESCE(a.acacod::text || '.'::text, ''::text)) 
				|| COALESCE(a.loccod, ''::bpchar)::text
			AS funcprog,
			a.acadsc,
			TO_CHAR(l.libdata, 'DD/MM/YYYY') AS libdata,
			a.acaptres,
			e.emdufautor,
			e.emdsglpartidoautor,
			e.emdnomeautor,
			e.foncod,
			e.gndcod,
			e.mapcod,
			l.libvalor,
			u.usunome,
			TO_CHAR(l.libdatainc, 'DD/MM/YYYY HH24:MI:SS') AS libdatainc
		FROM monitora.emendaliberacao l 
		INNER JOIN monitora.emenda e 
			USING (emdid)
		INNER JOIN monitora.acao a
			USING (acaid)
		INNER JOIN seguranca.usuario u
			ON u.usucpf = l.libusucpf
		WHERE l.libid IN ($libid) AND l.libremovido = 'f'
EOS;

$liberacoes = $db->carregar($sqlEmenda);
	if(@count($liberacoes)>0) {
		monta_titulo('Relatório de recursos liberados','Total de liberações ('.count($liberacoes).')');
?>

<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
  <thead>
  <tr>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Func. Prog.</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Descrição</b></td>	
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Ptres</b></td>	
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Data</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Partido</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Autor</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Fonte</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>GND</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Mod. Aplicação</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Valor</b></td>
  </tr>
  </thead>
  <tbody>
<?
$linhas = $db->carregar($sqlEmenda);
foreach($linhas as $key=>$linha) {
	$marcado = $key % 2 ? "#F7F7F7" : "";
	?>
  <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
    <td title="Funcional Programática"><?=$linha["funcprog"]?></td>
    <td title="Descrição"><?=$linha["acadsc"]?></td>
    <td title="Ptres"><?=$linha["acaptres"]?></td>
    <td title="Data"><?=$linha["libdata"]?></td>
    <td title="Partido"><?=$linha["emdsglpartidoautor"]?></td>
    <td title="Autor"><?=$linha["emdnomeautor"]?></td>
    <td align="right" style="color:#0066cc;" title="Fonte"><?=$linha["foncod"]?></td>
    <td align="right" style="color:#0066cc;" title="GND"><?=$linha["gndcod"]?></td>
    <td align="right" style="color:#0066cc;" title="Mod. Aplicação"><?=$linha["mapcod"]?></td>
    <td align="right" style="color:#0066cc;" title="Valor" nowrap="nowrap">R$ <?=number_format($linha["libvalor"], 2, ",", ".")?></td>
  </tr>
	<?
}
?>
  </tbody>
</table>
<?
		return;
	}

}

// filtros
if (!isset($gndcod)) $gndcod = array(3,4);

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

?>
<br>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<?

$whjoin = array();
if (isset($emdsglpartidoautor) && (@count($emdsglpartidoautor)>=1 && (bool)$emdsglpartidoautor[0]) ) {
	$buffer = implode("', '", $emdsglpartidoautor);
	$whjoin[] = " emdsglpartidoautor IN ('$buffer')";
}
if (isset($emdufautor) && (@count($emdufautor)>=1 && (bool)$emdufautor[0]) ) {
	$buffer = implode("', '", $emdufautor);
	$whjoin[] = " emdufautor IN ('$buffer')";
}
if (isset($emdcodtipoautor) && (@count($emdcodtipoautor)>=1 && (bool)$emdcodtipoautor[0]) ) {
	$buffer = implode("', '", $emdcodtipoautor);
	$whjoin[] = " emdcodtipoautor IN ('$buffer')";
}
if (isset($emdnomeautor) && (@count($emdnomeautor)>=1 && (bool)$emdnomeautor[0]) ) {
	$buffer = implode("', '", $emdnomeautor);
	$whjoin[] = " emdnomeautor IN ('$buffer')";
}
if (isset($gndcod) && (@count($gndcod)>=1 && (bool)$gndcod[0]) ) {
	$buffer = implode("', '", $gndcod);
	$whjoin[] = " gndcod IN ('$buffer')";
}
if (isset($foncod) && $foncod) {
	$whjoin[] = " foncod = '$foncod'";
}
if (isset($mapcod) && $mapcod) {
	$whjoin[] = " mapcod = '$mapcod'";
}
if (isset($acaptres) && $acaptres) {
	$whjoin[] = " a.acaptres = '$acaptres'";
}


$whjoin = implode(" AND ", $whjoin);
$sqlEmenda = <<<EOS
SELECT 
CASE WHEN l.libremovido = 'f' THEN
'<input type="checkbox" name="libid[]" value="' || l.libid || '">'
ELSE '' END AS checklib,
TO_CHAR(l.libdata, 'DD/MM/YYYY') AS libdata,
acaptres,
e.emdufautor,
e.emdsglpartidoautor,
e.emdnomeautor,
e.foncod,
e.gndcod,
e.mapcod,
l.libvalor,
u.usunome,
TO_CHAR(l.libdatainc, 'DD/MM/YYYY HH24:MI:SS') AS libdatainc
FROM monitora.emendaliberacao l 
INNER JOIN monitora.emenda e 
	USING (emdid)
INNER JOIN monitora.acao a
	USING (acaid)
INNER JOIN seguranca.usuario u
	ON u.usucpf = l.libusucpf
WHERE $whjoin
EOS;

//$cabecalho = array("&nbsp;", "Data", "UF", "Partido", "Autor", "Fonte", "GND", "Mod. Aplicação", "Valor", "Liberado por", "Liberado em");
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Emissão de chequinho para Emendas','Clique no Código para ver detalhes - Total de emendas encontradas ('.($nlinhas+1).')');?>
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
		<td nowrap="nowrap"><?=campo_texto_ajax($sqlFiltro["lsModAplicacao"], "mapcod", "mapcod", "Modalidade de AplicaÃ§Ã£o", $mapcod, 5, 3, '', 'left')?></td>
		<td class="SubTituloDireita">GND:</td>
		<td nowrap="nowrap"><?=$db->monta_checkbox("gndcod[]", $sqlFiltro["codGnd"], $gndcod);?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="8" style="text-align:center;"><input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="filtra();"></td>
	</tr>
	</form>
</table>
<br/>
<form method="post" name="formLiberaRecursos">
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
  <thead>
  <tr>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">&nbsp;</td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Data</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Ptres</b></td>	
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>UF</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Partido</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Autor</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Fonte</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>GND</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Mod. Aplicação</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Valor</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Liberado por</b></td>
	<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Liberado em</b></td>
  </tr>
  </thead>
  <tbody>
<?
$linhas = $db->carregar($sqlEmenda);
foreach($linhas as $key=>$linha) {
	$marcado = $key % 2 ? "#F7F7F7" : "";
	?>
  <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
    <td><?=$linha["checklib"]?></td>
    <td title="Data"><?=$linha["libdata"]?></td>
    <td title="Ptres"><?=$linha["acaptres"]?></td>
    <td title="UF"><?=$linha["emdufautor"]?></td>
    <td title="Partido"><?=$linha["emdsglpartidoautor"]?></td>
    <td title="Autor"><?=$linha["emdnomeautor"]?></td>
    <td align="right" style="color:#0066cc;" title="Fonte"><?=$linha["foncod"]?></td>
    <td align="right" style="color:#0066cc;" title="GND"><?=$linha["gndcod"]?></td>
    <td align="right" style="color:#0066cc;" title="Mod. Aplicação"><?=$linha["mapcod"]?></td>
    <td align="right" style="color:#0066cc;" title="Valor"><?=number_format($linha["libvalor"], 2, ",", ".")?></td>
    <td title="Liberado por"><?=$linha["usunome"]?></td>
    <td title="Liberado em"><?=$linha["libdatainc"]?></td>
  </tr>
	<?
}
?>
  </tbody>
  <tfoot>
  <tr>
    <td colspan="12" bgcolor="#FCFCFC"><b>Total de registros: <?=@count($linhas)?></b><br/><center><input type="button" name="imprimir" value="Imprimir" onClick="enviaFormRecursosEmendas()"/></center></td>
  </tr>
  </tfoot>
</table>
</form>
<script language="JavaScript">
function enviaFormRecursosEmendas() {
	document.formLiberaRecursos.action = "?modulo=principal/acao/recursosemenda&acao=C";
	document.formLiberaRecursos.submit();
}
</script>
