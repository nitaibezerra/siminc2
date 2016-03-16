<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
// function gravarQualificacaoProfissional() {

// 	jQuery('#form').submit();
// }

// function visualizarFormula()
// {
// 	var janela = window.open( 'projovemcampo.php?modulo=principal/planoImplementacao&acao=A&requisicao=popUpFormula', 'formula', 'width=800,height=400,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
//     janela.focus();
// }

</script>
<form id="form" name="form" method="POST">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="20%">
			Enviar o plano de implementação
			</td>
			<td>
			<? 
			include APPRAIZ ."includes/workflow.php";
			/* Barra de estado atual e ações e Historico */
			wf_desenhaBarraNavegacao( $docid, array( 'entid' => $_SESSION['rehuf_var']['entid'] ) );			
			?>
			</td>
		</tr>
	</table>
</form>
<?registarUltimoAcesso();?>