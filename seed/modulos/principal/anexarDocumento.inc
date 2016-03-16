<?php
$obSeed = new Seed();
$obProcesso = new Processos();

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_POST['requisicao'] == 'salvar' ){
	$obProcesso->salvarAnexos( $_POST );
}

if( $_POST['requisicao'] == 'excluir' ){
	$obProcesso->excluirAnexos( $_POST['arqid'] );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );

$prcid = $_SESSION['seed']['prcid'];
$obSeed->validaSession( $prcid );

monta_titulo('SEED', 'Anexar Documentos');

$obSeed->montaCabecalhoProcesso( $prcid );
?>
<script language="javascript" type="text/javascript" src="js/processo.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="arqid" id="arqid" value="<?=$arqid; ?>">
<input type="hidden" name="prcid" id="prcid" value="<?=$prcid; ?>">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Arquivo:</td>
		<td><input type="file" name="arquivo" id="arquivo" size="70" /></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo:</td>
		<td><?
			$sql = "SELECT 
					  	taxid as codigo,
					  	taxdsc as descricao
					FROM 
					  	seed.tipoanexo
					WHERE
						taxstatus = 'A'";
			$db->monta_combo("taxid", $sql, 'S', 'Selecione...', '', '', '', '265','S','taxid', '', '', 'Tipo Anexo');
			?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td><?=campo_textarea( 'anxdsc', 'N', 'S', 'Descrição', 80, 5, 200, '', '', '', '', 'anxdsc' ); ?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Salvar" onclick="salvarAnexo();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
<?
$obProcesso->listaAnexos( $prcid );
?>