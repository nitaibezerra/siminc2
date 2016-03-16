<?php
include APPRAIZ."fabrica/classes/NotaFiscalMemorando.class.inc";

if($_REQUEST['requisicao'] && $_REQUEST['nfsid']){
	$nfsid = $_REQUEST['nfsid'];
	$nf = new NotaFiscalMemorando($nfsid);
	$nf->$_REQUEST['requisicao']();
	if($_REQUEST['reload']){
		Header("Location: fabrica.php?modulo=sistema/geral/notaFiscal/listarVinculo&acao=A");
	}
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Memorando / Notas Fiscais";
monta_titulo( $titulo, '&nbsp;' );

$nf = new NotaFiscalMemorando();

if($_REQUEST['requisicao'] && !$_REQUEST['nfsid']){
	$nf->$_REQUEST['requisicao']();
}


extract($_POST);

?>
<script>
	function alterarVinculo(nfsid)
	{
		window.location.href = 'fabrica.php?modulo=sistema/geral/notaFiscal/vincularNota&acao=A&nfsid=' + nfsid;
	}
	
	function excluirNota(nfsid)
	{
		if(confirm('Deseja excluir dos dados?')){
			alert('Dados excluído com Sucesso!');
			window.location.href = 'fabrica.php?modulo=sistema/geral/notaFiscal/listarVinculo&acao=A&requisicao=excluirNotaFiscal&reload=true&nfsid=' + nfsid;
		}
	}
	
	function novoVinculo()
	{
		window.location.href = 'fabrica.php?modulo=sistema/geral/notaFiscal/vincularNota&acao=A';
	}
	
	function pesquisarMemorando()
	{
		document.getElementById('form_listar_memorando').submit();
	}
	
</script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<form name="form_listar_sistema" id="form_listar_memorando" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Nota Fiscal:</td>
			<td>
				<input type="hidden" name="requisicao" value="pesquisarMemorando" />
				<?//$nfm->comboNotaFiscal($nfsid)?>
				<input type="text" name="nfsdsc" id="nfsdsc" value="<?=$_REQUEST['nfsdsc']?>" />
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Memorando:</td>
			<td><?$nf->comboMemorando($memoid, "N")?></td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="pesquisarMemorando()" value="Pesquisar" >
				<input type="button" onclick="novoVinculo()" value="Cadastrar Nota" >
			</td>
		</tr>
	</table>
</form>
<?php $nf->showListaNotaFiscalMemorando(); ?>