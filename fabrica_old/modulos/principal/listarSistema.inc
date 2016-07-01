<?php
include APPRAIZ."fabrica/classes/ContratoSistema.class.inc";


if($_REQUEST['requisicao'] && $_REQUEST['ctsid']){
	$ctsid = $_REQUEST['ctsid'];
	$cs = new ContratoSistema($ctsid);
	$cs->$_REQUEST['requisicao']();
	if($_REQUEST['reload']){
		Header("Location: fabrica.php?modulo=principal/listarSistema&acao=A");
	}
}


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Sistemas / Contratos";
monta_titulo( $titulo, '&nbsp;' );

$cs = new ContratoSistema();

if($_REQUEST['requisicao'] && !$_REQUEST['ctsid']){
	$cs->$_REQUEST['requisicao']();
}


extract($_POST);

?>
<script>
	function alterarSistema(ctsid)
	{
		window.location.href = 'fabrica.php?modulo=principal/cadSistema&acao=A&ctsid=' + ctsid;
	}
	
	function excluirSistema(ctsid)
	{
		if(confirm('Deseja realmente excluir o sistema?')){
			window.location.href = 'fabrica.php?modulo=principal/listarSistema&acao=A&requisicao=excluirContratoSistema&reload=true&ctsid=' + ctsid;
		}
	}
	
	function novoContratoSistema()
	{
		window.location.href = 'fabrica.php?modulo=principal/cadSistema&acao=A';
	}
	
	function pesquisarContratoSistema()
	{
		document.getElementById('form_listar_sistema').submit();
	}
	
</script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<form name="form_listar_sistema" id="form_listar_sistema" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Sistema:</td>
			<td>
				<input type="hidden" name="requisicao" value="pesquisarContratoSistema" />
				<?$cs->comboSistema($sidid)?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Contrato:</td>
			<td><?$cs->comboContrato($ctrid)?></td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="pesquisarContratoSistema()" value="Pesquisar" >
				<input type="button" onclick="novoContratoSistema()" value="Novo Sistema" >
			</td>
		</tr>
	</table>
</form>
<?php $cs->showListaContratoSistema(); ?>