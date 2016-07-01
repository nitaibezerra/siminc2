<?php 

if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
}

if( $_SESSION['catalogo']['curid'] ){
	$permissoes = testaPermissao();
}else{
	$permissoes['gravar'] = true;
}

$dadosContato = recuperaContato($_REQUEST);
if(is_array($dadosContato)){
	foreach($dadosContato as $k => $dado){
		$$k = $dado;
	}
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

monta_abas();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_contato.js"></script>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr class="SubTituloEsquerda">
		<td style="FONT-SIZE: 12pt;">
			<center>
				Contato no MEC ou Capes
			</center>
		</td>
	</tr>
	<tr class="FundoTitulo">
		<td>
			<center><img border="0" align="top" src="../imagens/obrig.gif" >Indica campo obrigatório</center>
		</td>
	</tr>
</table>
<?php 
if( $_SESSION['catalogo']['curid'] ){
	montaCabecalho( $_SESSION['catalogo']['curid'] );
}
?>
<form method="post" name="frmContato" id="frmContato">
	<input type="hidden" value="" id="req" name="req"/>
	<input type="hidden" value="<?=$_REQUEST['curid'] ?>" id="curid" name="curid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">
				Telefone Principal&nbsp;
			</td>
			<td>
				<?=campo_texto('curconttel', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '15', '30','##-####-####|####-#######', '', '', '', '', 'id="curconttel"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Telefone Adicional&nbsp;
			</td>
			<td>
				<?=campo_texto('curconttel2', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '15', '30','##-####-####|####-#######', '', '', '', '', 'id="curconttel2"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Nome&nbsp;
			</td>
			<td>
				<?=campo_texto('curcontdesc', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '255','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				E-mail&nbsp;
			</td>
			<td>
				<?=campo_texto('curcontemail', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '255','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Site&nbsp;
			</td>
			<td>
				<?=campo_texto('curcontsite', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '255','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Outras Informações&nbsp;
			</td>
			<td>
				<?=campo_textarea('curcontinfo', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Ementa', '75', '4', '5000'); ?>
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<center>
					<input type="button" id="voltar" value="Voltar"/>
					<input type="button" id="salvar" value="Salvar" <?=(!$permissoes['gravar']  ? 'disabled' : '') ?>/>
				</center>
			</td>
		</tr>
	</table>
</form>