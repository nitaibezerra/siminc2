<? 
include_once( APPRAIZ."cte/classes/FormaExecucao.class.inc" );

$obFormaExecucao = new FormaExecucao();

if( $_REQUEST['frmid'] ){
	$obFormaExecucao->carregarPorId( $_REQUEST['frmid'] );
}

if( $_REQUEST['act'] ){
    
	$arCampos = array( "frmid", "frmdsc", "frmtipo", "frmbrasilpro" );
	
	$obFormaExecucao->popularObjeto( $arCampos );
	$obFormaExecucao->frmbrasilpro = isset( $_REQUEST["frmbrasilpro"] ) ? $_REQUEST["frmbrasilpro"] : 'false';

	$obFormaExecucao->salvar();
		
    $db->commit();
    $db->sucesso( $modulo );

}

if( $_REQUEST['stAcao'] == 'excluir'){
	
	if( $obFormaExecucao->excluir() ){
		$obFormaExecucao->commit();
	    $obFormaExecucao->sucesso( $modulo );
	}
	else{
		echo "<script>location.href='cte.php?modulo=/sistema/formaExecucao&acao=C';</script>";				
	}
}

include_once( APPRAIZ."includes/cabecalho.inc" );
print "<br>";

$obFormaExecucao->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,'(Configuração da Forma de Execução)');

?>
<div align="center">
	<form method="POST"  name="formulario" action="#" >
		<input type=hidden name="frmid" value="<?= $obFormaExecucao->frmid; ?>" />
		<input type=hidden name="modulo" value="<?= $modulo ?>" />
		<input type=hidden name="act" value="true" />
		<center>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td colspan="2" class="SubTituloDireita" ></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" >Descrição: </td>
					<td><?=campo_texto('frmdsc','S','','',60,100,'','', '', '', '', '', '', $obFormaExecucao->frmdsc);?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" >Tipo: </td>
					<td><?php $obFormaExecucao->monta_combo( "frmtipo", $obFormaExecucao->recuperarTipos(), "S", "Selecione", "", "" ,"","","S", '', '', $obFormaExecucao->frmtipo ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" >Brasil Pró: </td>
					<td><input type="checkbox" name="frmbrasilpro" id="frmbrasilpro" value="true" <?= $obFormaExecucao->frmbrasilpro == 't' ? 'checked="checked"' : "" ?> /></td>
				</tr>
				<tr bgcolor="#C0C0C0" align="center">       
					<td colspan="2">
						<input type='button' class="botao" value='Gravar' onclick="gravar_forma_execucao()" />
						<? if( $obFormaExecucao->frmid ){ ?>
							<input type='button' class='botao' value='Cancelar' id='btcancelar' name='btcancelar' onclick='history.back();' />
						<? } ?>
					</td>
				</tr>
			</table>
		</center>
		<br><br>
	</form>
</div>
<?php

$cabecalho = array('Ações', 'Descrição', 'Tipo', 'Brasil Pró');
$obFormaExecucao->monta_lista( $obFormaExecucao->recuperarListagem(), $cabecalho, 100, 20, '' ,'' ,'' );

?>

<script>
    
	function gravar_forma_execucao(){
	
		if( !validaBranco(document.formulario.frmdsc, 'Descrição') ) return false;
		if( !validaBranco(document.formulario.frmtipo, 'Tipo') ) return false;
		      
		document.formulario.submit();
	
	}
   
	function altera_forma_execucao( cod ){ 
		window.location.href = window.location.href+'&stAcao=alterar&frmid=' + cod;
	}   

	function excluir_forma_execucao( cod ){ 
	    if( window.confirm( "Confirma a exclusão do programa?") ){
			window.location.href = window.location.href+'&stAcao=excluir&frmid=' + cod;
	    }
	    else return;
	}   

</script>