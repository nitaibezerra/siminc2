<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

if($_REQUEST["excluir_especificação"]) {
	/*$sql = "SELECT DISTINCT
				esp.espid,
			    esp.espstatus,
			    ie.icestatus,
			    ptie.ptestatus
			FROM
				emenda.especificacao esp
			    inner join emenda.iniciativaespecificacao ie
			    	on ie.espid = esp.espid
			    inner join emenda.ptiniciativaespecificacao ptie
			    	on ptie.iceid = ie.iceid
			WHERE
				esp.espid = " . $_REQUEST["excluir_especificação"];*/
	
	$sql = "UPDATE emenda.especificacao SET espstatus = 'I' WHERE espid = " . $_REQUEST["excluir_especificação"];
	$db->executar($sql);
	
	$sql = "UPDATE emenda.especificacaoresponsavel SET erestatus = 'I' WHERE espid = " . $_REQUEST["excluir_especificação"];
	$db->executar($sql);
	
	$db->commit();
	$db->sucesso('principal/listaEspecificacoes');
}

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Especificação (Metas)', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

$resid = $_REQUEST["resid"];
$espnome = $_REQUEST["espnome"];
$espunidademedida = $_REQUEST["espunidademedida"];
$espkit = $_REQUEST["espkit"];

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

?>

<form id="formListaEspecificacao" name="formListaEspecificacao" action="" method="post" enctype="multipart/form-data">
<input type="hidden" value="" id="excluir_especificação" name="excluir_especificação" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<!-- <tr>
		<td class="SubTituloDireita" valign="top">Responsável:</td>
		<td id="td_emendas">
			<?=comboResponsavel($_SESSION['usucpf'], $resid);?>
		</td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Especificação:</td>
		<td>
			<?
				$espnome = $_REQUEST["espnome"];
				echo campo_texto( 'espnome', 'N', 'S', '', 50, 250, '', '','','','','id="espnome"'); 
			?>
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita">Unidade de Medida:</td>
		<td>
			<?
				$espunidademedida = $_REQUEST["espunidademedida"];
				echo campo_texto( 'espunidademedida', 'N', 'S', '', 50, 100, '', '','','','','id="espunidademedida"'); 
			?>
			<br /><br />
			<input type="checkbox" value="S" name="espkit" id="espkit" <?= ($_REQUEST["espkit"]) ? "checked=\"checked\"" : "" ?>> Kit ou Módulo
		</td>
	</tr>
	<tr bgcolor="#c0c0c0">
		<td>&nbsp;</td>
		<td>
			<?
			if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
				$retorno = '';
			} else {
				$retorno = 'disabled="disabled"';
			}
			?>
			<input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();">
			<input type="button" value="Nova Especificação" name="btnNovo" id="btnNovo" onclick="novaEspecificacao();" <?=$retorno;?> />
		</td>
	</tr>
</table>
</form>

<br />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista de Especificações Cadastradas</b></td>
	</tr>
</table>

<?

$where = "";

/*if($_REQUEST["resid"]) {
	$where .= " AND r.resid = ". $_REQUEST["resid"];
} else {
	// verifica os responsáveis associados ao perfil do usuário
	$responsaveis = recuperaResponsaveis($_SESSION["usucpf"]);
	// limita o resultado da pesquisa dependendo dos responsáveis associados
	if($responsaveis) {
		if(count($responsaveis) == 1)
			$where .= " AND r.resid = " . $responsaveis[0];
		else
			$where .= " AND r.resid in (" . implode(",", $responsaveis) . ")";
	}
}*/
	
$where .= ($_REQUEST["espnome"]) ? " AND UPPER(e.espnome) ilike '%".str_to_upper( trim($_REQUEST["espnome"]))."%'" : "";
$where .= ($_REQUEST["espunidademedida"]) ? " AND UPPER(e.espunidademedida) ilike '%".str_to_upper( trim($_REQUEST["espunidademedida"]) )."%'" : "";
$where .= ($_REQUEST["espkit"]) ? " AND e.espkit = true" : "";

if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
	$acao = "'<a href=\"#\" onclick=\"alterarEspecificacao(' || e.espid || ');\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			 <a href=\"#\" onclick=\"excluirEspecificacao(' || e.espid || ');\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
} else {
	 $acao = "'<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			   <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">'";
}

$sql = "SELECT
			".$acao." as comando,
			e.espid ||' - '||e.espnome as especificacao,
			CASE WHEN e.espkit = true THEN e.espunidademedida || ' (kit)' ELSE e.espunidademedida END AS unidade,
			(select * from emenda.agrupaAnoEspecificacao(e.espid)) as ano
		FROM
			emenda.especificacao e
		WHERE
			e.espstatus = 'A'
			".$where."
		ORDER BY
			e.espnome";

/*$arEspecif = $db->carregar( $sql );

foreach ($arEspecif as $chave => $val) {
	
	$sql = "SELECT 
				res.resdsc
			FROM 
			  emenda.responsavel res
			  inner join emenda.especificacaoresponsavel enr
			  	on res.resid = enr.resid
			WHERE
				enr.espid = ".$val['espid']."
				and enr.erestatus = 'A'";
	
	$arResp = $db->carregarColuna( $sql );
	
	$arDados[$chave] = array("comando" => $val['comando'],
		         		   //"responsavel" => implode(', ', $arResp),
		         		   "especificacao" => $val['especificacao'],
						   "unidade" => $val['unidade']);
}*/


$cabecalho 		= array( "Comando", "Especificação da Ação", "Unidade de Medida", 'Ano');
$tamanho		= array( '10%', '30%', '30%', '30%' );
$alinhamento	= array( 'center', 'left', 'center' );
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
//$db->monta_lista_array($arDados, $cabecalho, 5000, 20, '', 'center', '');

?>

<script type="text/javascript">

var form 			= document.getElementById("formListaEspecificacao");
var btnPesquisar 	= document.getElementById("btnPesquisar");
var btnNovo 		= document.getElementById("btnNovo");

function novaEspecificacao() {
	btnPesquisar.disabled 	= true;
	btnNovo.disabled 		= true;
	
	window.location.href = "emenda.php?modulo=principal/cadastroEspecificacoes&acao=A";
}
	
function pesquisar(){
	btnPesquisar.disabled 	= true;
	btnNovo.disabled 		= true;
	
	form.submit();
}

function alterarEspecificacao(espid) {
	window.location.href = "emenda.php?modulo=principal/cadastroEspecificacoes&acao=A&espid=" + espid;
}

function excluirEspecificacao(espid) {
	var hdExluirEspecificacao = document.getElementById("excluir_especificação");
	
	if(confirm("Deseja realmente excluir esta especificação ?")) {
		hdExluirEspecificacao.value = espid;
		form.submit();
	}
}

</script>