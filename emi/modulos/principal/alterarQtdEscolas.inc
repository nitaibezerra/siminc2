<?php

if( $_REQUEST["requisicao"] == "salvar" ){
	$emi->alteraQtdEscolas( $_REQUEST );
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$arMnuid = array();

if( !emiPossuiPerfil(EMI_PERFIL_ADMINISTRADOR) ){
	$arMnuid = array(5887);
}

$db->cria_aba($abacod_tela,$url,$parametros,$arMnuid);
monta_titulo("Quantidade de Escolas", "");

if( $_SESSION["emi"]["emeidPai"] ){
	
	$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"] );
	
	$emeqtdescolas = emiBuscaQtdEscolas( $_SESSION["emi"]["emeidPai"] ); 
	
}

?>
<form method="post" action="" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="salvar">
	<input type="hidden" name="emeid" id="emeid" value="<?=$_SESSION["emi"]["emeidPai"];?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="subtitulodireita" width="190px">Quantidade de Escolas:</td>
			<td>
				<?php echo campo_texto( 'emeqtdescolas', 'S', $emiSomenteLeitura, '', 3, 3, '##', '', 'left', '', 0, "id='emeqtdescolas'"); ?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" value="Salvar" onclick="emiValidaQtdEscolas();" style="cursor: pointer;" <?=$emiDisabled; ?>/>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>