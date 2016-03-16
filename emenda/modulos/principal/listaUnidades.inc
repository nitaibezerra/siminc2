<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

if($_REQUEST["excluir_unidade"]) {
	$sql = "UPDATE emenda.unidades SET unistatus = 'I' WHERE uniid = " . $_REQUEST["excluir_unidade"];
	$db->executar($sql);
	
	$sql = "UPDATE emenda.unidnomeresp SET unrstatus = 'I' WHERE uniid = " . $_REQUEST["excluir_unidade"];
	$db->executar($sql);
	
	$db->commit();
	$db->sucesso('principal/listaUnidades');
	
	/*echo "<script type=\"text/javascript\">
			alert(\"Especificação excluída com sucesso.\");
			document.window.location.href = 'emenda.php?modulo=principal/listaUnidades&acao=A';
		  </script>";*/
}

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Unidades', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

$unisigla = $_REQUEST["unisigla"];
$uninome = $_REQUEST["uninome"];

$perfil = pegaPerfilArray( $_SESSION['usucpf'] );

?>

<form id="formListaUnidade" name="formListaUnidade" action="" method="post" enctype="multipart/form-data">
<input type="hidden" value="" id="excluir_unidade" name="excluir_unidade" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Sigla:</td>
		<td>
			<?
				$unisigla = $_REQUEST["unisigla"];
				echo campo_texto( 'unisigla', 'N', 'S', '', 50, 250, '', '','','','','id="unisigla"'); 
			?>
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita">Unidade:</td>
		<td>
			<?
				$uninome = $_REQUEST["uninome"];
				echo campo_texto( 'uninome', 'N', 'S', '', 50, 250, '', '','','','','id="uninome"'); 
			?>
		</td>
	</tr>
	<tr bgcolor="#c0c0c0">
		<td>&nbsp;</td>
		<td>
			<?
			if( in_array( GESTOR_EMENDAS, $perfil ) || in_array( SUPER_USUARIO, $perfil ) ){
				$retorno = '';
			} else {
				$retorno = 'disabled="disabled"';
			}
			?>
			<input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();">
			<input type="button" value="Nova Unidade" name="btnNovo" id="btnNovo" onclick="novaUnidade();" <?=$retorno;?> />
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

$where .= ($_REQUEST["unisigla"]) ? " AND UPPER(u.unisigla) like '%".str_to_upper(trim($_REQUEST["unisigla"]))."%'" : "";
$where .= ($_REQUEST["uninome"]) ? " AND UPPER(u.uninome) like '%".str_to_upper(trim($_REQUEST["uninome"]))."%'" : "";

if( in_array( GESTOR_EMENDAS, $perfil ) || in_array( SUPER_USUARIO, $perfil ) ){
	$acao = "'<a href=\"#\" onclick=\"alterarUnidade(' || u.uniid || ');\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			 <a href=\"#\" onclick=\"excluirUnidade(' || u.uniid || ');\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
} else {
	 $acao = "'<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
			   <img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">'";
}

$sql = "SELECT
			".$acao." as comando, uniid, unisigla, uninome
		FROM
			emenda.unidades u			
		WHERE
			u.unistatus = 'A'
			".$where."
		ORDER BY
			u.uniid";

$arUnidades = $db->carregar( $sql );

if (!$arUnidades) echo "<script>alert('Nenhum registro encontrado.');</script>";
else
{
	foreach ($arUnidades as $chave => $val) {
		
		$sql = "SELECT 
					unrcargo || ' - ' || unrnome AS x
				FROM 
				  emenda.unidnomeresp
				WHERE
					uniid = ".$val['uniid']."
					and unrstatus = 'A'";
		
		$arResp = $db->carregarColuna( $sql );
		
		$arDados[$chave] = array("comando" => $val['comando'],
			         		   "Sigla" => $val['unisigla'],
							   "Unidade" => $val['uninome'],
			         		   "responsavel" => implode(', ', $arResp));
	}
	
	$cabecalho 		= array( "Comando", "Sigla", "Unidade", "Responsável");
	$tamanho		= array( '10%', '30%', '30%', '30%' );
	$alinhamento	= array( 'center', 'left', 'center' );
	//$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
	$db->monta_lista_array($arDados, $cabecalho, 5000, 20, '', 'center', '');

}
?>

<script type="text/javascript">

var form 			= document.getElementById("formListaUnidade");
var btnPesquisar 	= document.getElementById("btnPesquisar");
var btnNovo 		= document.getElementById("btnNovo");

function novaUnidade() {
	btnPesquisar.disabled 	= true;
	btnNovo.disabled 		= true;
	
	window.location.href = "emenda.php?modulo=principal/cadastroUnidades&acao=A";
}
	
function pesquisar(){
	btnPesquisar.disabled 	= true;
	btnNovo.disabled 		= true;
	
	form.submit();
}

function alterarUnidade(uniid) {
	window.location.href = "emenda.php?modulo=principal/cadastroUnidades&acao=A&uniid=" + uniid;
}

function excluirUnidade(uniid) {
	var hdExluirUnidade = document.getElementById("excluir_unidade");
	
	if(confirm("Deseja realmente excluir esta especificação ?")) {
		hdExluirUnidade.value = uniid;
		form.submit();
	}
}

</script>