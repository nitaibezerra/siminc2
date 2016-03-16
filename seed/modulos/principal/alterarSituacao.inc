<?php
$obSeed = new Seed();
$obProcesso = new Processos();


include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );

$prcid = $_SESSION['seed']['prcid'];
$obSeed->validaSession( $prcid );

monta_titulo('SEED', 'Alterar Situação');

$obSeed->montaCabecalhoProcesso( $prcid );

if($_POST["requisicao"] == 'salvar') {
	$obProcesso->cadastraSituacaoProcesso( $_POST );
}	
		
?>
<script language="javascript" type="text/javascript" src="js/processo.js"></script>
<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="prcid" id="prcid" value="<?=$prcid; ?>">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Situação:</td>
		<td><?
			$sql = "SELECT 
					  sitid as codigo,
					  sitdsc as descricao
					FROM 
					  seed.situacao
					WHERE
						sitstatus = 'A'";			
			$db->monta_combo("sitid", $sql, 'S', 'Selecione...', '', '', '', '265','S','sitid', '', '', 'Tipo Processo');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Justificativa:</td>
		<td><?=campo_textarea('hstdsc', 'S', 'S', 'Objeto', 80, 5, 4000, '', '', '', '', 'Justificativa' ); ?></td>
	</tr>   
    <tr>
    	<td class ="SubTituloDireita" align="right">Responsável: </td>
        <td><?=$_SESSION['usunome']; ?></td>
	</tr>
    <tr>
    	<td class ="SubTituloDireita" align="right">  </td>
        <td><input type="button" name="btSalvar" id="btSalvar" onclick="incluirSituacao();" value="Salvar"></td>
	</tr>
</table>
</form>