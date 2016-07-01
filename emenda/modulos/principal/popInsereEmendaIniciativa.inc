<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if($_REQUEST["salva"]) {
	$emendas = explode(",", $_REQUEST["emendas"]);
	
	for($i=0; $i<count($emendas); $i++) {
		$sql = "INSERT INTO emenda.iniciativaemendadetalhe(emdid, iniid, iedstatus)
						VALUES (".$emendas[$i].", ".$_SESSION["iniid"].", 'A')";
		$db->executar($sql);	
	}
	$db->commit();
	
	echo "<script type=\"text/javascript\">
			alert(\"Emenda(s) incluída(s) com sucesso.\");
			window.opener.location.reload();
			self.close();
		  </script>";
	exit;
}

$sql = "SELECT
			res.resid,
			res.resdsc as responsavel,
			uni.unicod,
			uni.unitpocod,
			uni.unidsc as unidade,
			ini.ininome as iniciativa,
			gnd.gndcod,
			gnd.gndcod || ' - ' || gnd.gnddsc as gnd
		FROM
			emenda.iniciativa ini
		INNER JOIN
			emenda.iniciativaresponsavel ir on ir.iniid = ini.iniid and irestatus = 'A'
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
			gnd.gndstatus = 'A' AND
			ini.iniid = ".$_SESSION["iniid"];
$dadosCabecalho = $db->carregar($sql);

?>

<html>
<head>
	<title>SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação</title>
	<script language="JavaScript" src="../../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<style>
		#div_rolagem table {
			width: 100%;		
		}
	</style>
</head>
<body>
<form id="formPesquisaEmendaIniciativa" method="post" action="">
<input type="hidden" name="pesquisa" value="1" />
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
		<td width="100" style="text-align: center;" colspan="2" class="SubTituloEsquerda"><b>Filtro</b></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Exercício:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$_SESSION['exercicio']?></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Responsável:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dadosCabecalho[0]['responsavel']?></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Unidade Orçamentária:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dadosCabecalho[0]['unidade']?></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">GND:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$dadosCabecalho[0]['gnd']?></td>
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
			WHERE
				pro.prgstatus = 'A' AND
				vfun.acastatus = 'A' AND 
				vfun.prgano = '".$_SESSION["exercicio"]."'";
	?>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Programa:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?$db->monta_combo("prgid", $sql, 'S', "Selecione...", '', '', '', '300', 'N', '');?></td>
	</tr>
	<?
	$acacod = $_REQUEST["acacod"];
	
	$sql = "SELECT DISTINCT 
				vfun.acacod as codigo,
				vfun.acacod || ' - ' || vfun.acadsc as descricao 
			FROM 
				emenda.v_funcionalprogramatica vfun
			WHERE
				vfun.acastatus = 'A' AND 
				vfun.prgano = '".$_SESSION["exercicio"]."'";
	?>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda">Ação:</td>
		<td width="80%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?$db->monta_combo("acacod", $sql, 'S', "Selecione...", '', '', '', '300', 'N', '');?></td>
	</tr>
	<tr>
		<td width="100" style="text-align: right;" class="SubTituloEsquerda"></td>
		<td width="100" style="text-align: left" class="SubTituloEsquerda"><input type="button" value="Pesquisar" id="bt_pesquisar" onclick="realizarPesquisa();" /></td>
	</tr>
 </tbody>
</table>
</form>
<br />
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
 <tbody>
 	<tr>
 		<td width="10%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><input type='checkbox' id='seleciona_todos' onclick='selecionaTodos();' /> <b>Selecionar Todos</b></td>
		<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><b>Lista de Emendas</b></td>
	</tr>
 </tbody>
</table>

<table align="center" class="Tabela" cellpadding="0" cellspacing="0">
 <tbody>
 	<tr>
 	<td>
<div id="div_rolagem" style="overflow-x: auto; overflow-y: auto; width:100%; height:300px;">
<?

$sql = "SELECT emdid FROM emenda.iniciativaemendadetalhe WHERE iniid = ".$_SESSION["iniid"]." AND iedstatus = 'A'";
$emdids = $db->carregarColuna($sql);

$emdids = ($emdids) ? ' AND emd.emdid not in ('.implode(",", $emdids).')' : '';

$where = "";
if($_REQUEST["pesquisa"]) {
	$where .= ($_REQUEST["prgid"]) ? " AND vfun.prgid = ".$_REQUEST["prgid"]."" : "";
	$where .= ($_REQUEST["acacod"]) ? " AND vfun.acacod = '".$_REQUEST["acacod"]."'" : "";
}

$sql = "SELECT
			'<input type=\'checkbox\' name=\'emdid[]\' value=\'' || emd.emdid || '\' />' as acao,
			eme.emecod as numero,
			case when eme.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autor,
			vfun.fupfuncionalprogramatica||' ' as funcional,
			vfun.fupdsc as subtitulo,
			emd.mapcod as mod,
			emd.foncod as fonte,
			to_char(emd.emdvalor,'R$ 999G999G990D00') as valor
		FROM
			emenda.emendadetalhe emd
			inner join emenda.emenda eme 
            	on eme.emeid = emd.emeid
            inner join emenda.autor aut 
            	on aut.autid = eme.autid
            left join emenda.v_funcionalprogramatica vfun 
            	on vfun.acaid = eme.acaid
                and vfun.acastatus = 'A'
                and vfun.prgano = '".$_SESSION["exercicio"]."'
                and vfun.unicod = '".$dadosCabecalho[0]['unicod']."'
                and vfun.unitpocod = '".$dadosCabecalho[0]['unitpocod']."'
                ".$where."
		WHERE
			eme.resid = ".$dadosCabecalho[0]['resid']." AND 
			emd.gndcod = ".$dadosCabecalho[0]['gndcod']."
			".$emdids."
		ORDER BY
			eme.emecod";

$cabecalho 		= array( "Selecionar", "Número", "Autor", "Funcional Prg", "Subtítulo", "Mod", "Fonte", "Valor");
$tamanho		= array( '5%', '5%', '20%', '20%', '30%', '5%', '5%', '10%' );
$alinhamento	= array( 'center', 'center', 'left', 'center', 'left', 'center', 'center', 'center' );
$db->monta_lista( $sql, $cabecalho, 9999999999, 10, 'N', 'center', '', '', $tamanho, $alinhamento);

?>

</div>
</td></tr>
</tbody></table>
<table align="center" class="Tabela" border="0" cellpadding="0" cellspacing="0">
<tr><td width="100" style="text-align: center" class="SubTituloEsquerda">
<input type="button" id="bt_salvar" value="Salvar Dados" onclick="salvarEmendas();" />
&nbsp;
<input type="button" id="bt_fechar" value="Fechar Janela" onclick="self.close();" />
</td></tr>
</table>
<form id="formSalvaEmendas" method="post" action="">
<input type="hidden" name="salva" value="1" />
<input type="hidden" name="emendas" value="" />
</form>
</body>
</html>

<script type="text/javascript">

function salvarEmendas() {
	var btSalvar	= document.getElementById("bt_salvar");
	var btFechar	= document.getElementById("bt_fechar");
	var form		= document.getElementById("formSalvaEmendas");
	var emeid 		= document.getElementsByName("emdid[]");
	var emendas 	= document.getElementsByName("emendas")[0];
	var vAux 		= false;
	var arrEmendas 	= new Array();
	
	btSalvar.disabled = true;
	btFechar.disabled = true;
	
	for(var i=0; i<emeid.length; i++) {
		if(emeid[i].checked) {
			vAux = true;
			arrEmendas.push(emeid[i].value);
		}
	}
	
	if(vAux) {
		emendas.value = arrEmendas.join(",");
		form.submit();
	} else {
		alert('Pelo menos uma demanda deve ser selecionada.');
		btSalvar.disabled = false;
		btFechar.disabled = false;
		return;
	}
}

function selecionaTodos() {
	var selecionaTodos 	= document.getElementById("seleciona_todos");
	var emeid 			= document.getElementsByName("emdid[]");
	
	for(var i=0; i<emeid.length; i++) {
		if(selecionaTodos.checked) {
			if(!emeid[i].checked) emeid[i].checked = true;
		} else {
			if(emeid[i].checked) emeid[i].checked = false;
		}
	}
}

function realizarPesquisa() {
	var form		= document.getElementById("formPesquisaEmendaIniciativa");
	var btPesquisar	= document.getElementById("bt_pesquisar");
	
	btPesquisar.disabled = true;
	
	form.submit();
}

</script>