<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if(!isset($_SESSION["iniid"])) {
	echo "<script type=\"text/javascript\">
			alert(\"Deve-se utilizar uma Iniciativa já existente para acessar esta tela.\");
			window.location = \"emenda.php?modulo=principal/listaAcao&acao=A\";
		  </script>";
	die;
}

if($_REQUEST["pesquisa"] && $_REQUEST["excluir_emenda"]) {
	$sql = "UPDATE emenda.iniciativaemendadetalhe SET iedstatus = 'I' WHERE iedid = ".$_REQUEST["excluir_emenda"];
	$db->executar($sql);
	$db->commit();
	
	echo "<script type=\"text/javascript\">
			alert(\"Emenda excluída com sucesso.\");
			window.location = \"emenda.php?modulo=principal/emendasIniciativa&acao=A\";
		  </script>";
}

// Recupera os dados do cabeçalho
$sql = "SELECT
			res.resdsc as responsavel,
			uni.unidsc as unidade,
			ini.ininome as iniciativa,
			gnd.gndcod || ' - ' || gnd.gnddsc as gnd
		FROM
			emenda.iniciativa ini
		INNER JOIN
			emenda.iniciativaresponsavel ir on ir.iniid = ini.iniid
		INNER JOIN
			emenda.responsavel res ON res.resid = ir.resid
		INNER JOIN
			public.unidade uni ON uni.unicod = res.unicod 
							  AND uni.unitpocod = res.unitpocod
		INNER JOIN
			public.gnd gnd ON gnd.gndcod = ini.gndcod
		WHERE
			ini.inistatus = 'A' AND 
			res.resstatus = 'A' AND 
			--uni.unistatus = 'A' AND 
			--gnd.gndstatus = 'A' AND
			ini.iniid = ".$_SESSION["iniid"];
$dadosCabecalho = $db->carregar($sql);

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

$db->cria_aba( $abacod_tela, $url, '' );

//echo montarAbasArray(carregaAbasEmendas('listaAcao'), "/emenda/emenda.php?modulo=principal/listaAcao&acao=A");
monta_titulo('Emendas da Iniciativa', '<b>Dados da Iniciativa</b>');

?>

<!-- cabeçalho -->
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Unidade Orçamentária:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dadosCabecalho[0]['unidade']?></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Iniciativa:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dadosCabecalho[0]['iniciativa']?></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">GND:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dadosCabecalho[0]['gnd']?></td>
	</tr>
 </tbody>
</table>

<form id="formPesquisaEmendaIniciativa" method="post" action="">
<input type="hidden" name="pesquisa" value="1" />
<input type="hidden" name="excluir_emenda" value="" />
<!-- filtro -->
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
		<td width="100%" colspan="2" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><b>Filtro de Pesquisa</b></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Exercício:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$_SESSION["exercicio"]?></td>
	</tr>
	<?
	$prgid = $_REQUEST["prgid"];
	
	$sql = "SELECT DISTINCT 
				vfun.prgid as codigo,
				vfun.prgcod || ' - ' || pro.prgdsc as descricao 
			FROM 
				emenda.v_funcionalprogramatica vfun 
			INNER JOIN 
				monitora.programa pro ON pro.prgid = vfun.prgid
			INNER JOIN
				emenda.iniciativaemendadetalhe ied ON ied.iniid = ".$_SESSION["iniid"]."
			INNER JOIN
				emenda.emendadetalhe emd ON emd.emdid = ied.emdid
			INNER JOIN
				emenda.emenda eme ON eme.emeid = emd.emeid 
								 AND eme.acaid = vfun.acaid
			WHERE
				pro.prgstatus = 'A' AND 
				ied.iedstatus = 'A' AND 
				vfun.acastatus = 'A' AND 
				vfun.prgano = '".$_SESSION["exercicio"]."'";
	?>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Programa:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?$db->monta_combo("prgid", $sql, 'S', "Selecione...", '', '', '', '300', 'N', '');?></td>
	</tr>
	<?
	$acaid = $_REQUEST["acaid"];
	
	$sql = "SELECT DISTINCT 
				vfun.acacod as codigo,
				vfun.acacod || ' - ' || vfun.acadsc as descricao 
			FROM 
				emenda.v_funcionalprogramatica vfun
			INNER JOIN
				emenda.iniciativaemendadetalhe ied ON ied.iniid = ".$_SESSION["iniid"]."
			INNER JOIN
				emenda.emendadetalhe emd ON emd.emdid = ied.emdid
			INNER JOIN
				emenda.emenda eme ON eme.emeid = emd.emeid 
								 AND eme.acaid = vfun.acaid
			WHERE 
				ied.iedstatus = 'A' AND 
				vfun.acastatus = 'A' AND 
				vfun.prgano = '".$_SESSION["exercicio"]."'";
	?>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Ação:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?$db->monta_combo("acaid", $sql, 'S', "Selecione...", '', '', '', '300', 'N', '');?></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda"></td>
		<td width="100" style="text-align: left" class="SubTituloEsquerda"><input type="button" value="Pesquisar" id="bt_pesquisar" onclick="realizarPesquisa();" /></td>
	</tr>
 </tbody>
</table>
</form>

<!-- popup -->
<table align="center" class="Tabela" border="0" cellpadding="0" cellspacing="0">
<tr><td>
<? if(possuiPermissao()) { ?>
<a href="#" onclick="adicionarEmendas();"><img src="../imagens/gif_inclui.gif" border="0" style="cursor:pointer;">&nbsp;Adicionar Emendas à Iniciativa</a>
<? } else { ?>
<img src="../imagens/gif_inclui_d.gif" border="0" style="cursor:pointer;">&nbsp;Adicionar Emendas à Iniciativa
<? } ?>
</td></tr>
</table>

<!-- lista das emendas -->
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
		<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><b>Emendas da Iniciativa</b></td>
	</tr>
 </tbody>
</table>
<?

$where = "";
if($_REQUEST["pesquisa"]) {
	$where .= ($_REQUEST["prgid"]) ? " AND vfun.prgid = ".$_REQUEST["prgid"]."" : "";
	$where .= ($_REQUEST["acacod"]) ? " AND vfun.acacod = '".$_REQUEST["acacod"]."'" : "";
}

if(possuiPermissao()) {
	$acao = "'<a href=\"#\" onclick=\"excluirEmendaIniciativa(' || ied.iedid || ');\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
} else {
	$acao = "'<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">'";
}

$sql = "SELECT
			".$acao." as acao,
			eme.emecod as numero,
			case when eme.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autor,
			uni.unidsc as uo,
			vfun.fupfuncionalprogramatica||' ' as funcional,
			vfun.fupdsc as subtitulo,
			emd.gndcod as gnd,
			emd.mapcod as mod,
			emd.foncod as fonte,
			to_char(emd.emdvalor,'R$ 999G999G990D00') as valor
		FROM
			emenda.iniciativaemendadetalhe ied
		INNER JOIN
			emenda.emendadetalhe emd ON emd.emdid = ied.emdid
		INNER JOIN
			emenda.emenda eme ON eme.emeid = emd.emeid
		INNER JOIN
			emenda.autor aut ON aut.autid = eme.autid
		INNER JOIN
			emenda.v_funcionalprogramatica vfun ON vfun.acaid = eme.acaid
		INNER JOIN
			public.unidade uni ON uni.unicod = vfun.unicod
		WHERE
			ied.iniid = ".$_SESSION["iniid"]." AND 
			ied.iedstatus = 'A' AND
			--aut.autstatus = 'A' AND
			vfun.acastatus = 'A' AND 
			vfun.prgano = '".$_SESSION["exercicio"]."'
			".$where."
		ORDER BY
			eme.emecod";


$cabecalho 		= array( "Ação", "Número", "Autor", "UO", "Funcional Prg", "Subtítulo", "GND", "Mod", "Fonte", "Valor");
$tamanho		= array( '5%', '5%', '15%', '5%', '15%', '30%', '5%', '5%', '5%', '10%' );
$alinhamento	= array( 'center', 'center', 'left', 'center', 'center', 'left', 'center', 'center', 'center', 'center' );
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
?>
<script type="text/javascript">

var btPesquisar = document.getElementById("bt_pesquisar");
var form 		= document.getElementById("formPesquisaEmendaIniciativa");

function realizarPesquisa() {
	btPesquisar.disabled = true;
	form.submit();
}

function adicionarEmendas() {
	var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereEmendaIniciativa&acao=A', 'emendainiciativa', 'width=800,height=600,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
	janela.focus();
}

function excluirEmendaIniciativa(iedid) {
	if(confirm("Deseja realmente excluir esta Emenda?")) {
		var excluirEmenda = document.getElementsByName("excluir_emenda")[0];
		excluirEmenda.value = iedid;
		form.submit();
	}
}

</script>