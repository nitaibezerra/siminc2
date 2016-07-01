<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Visualização do Plano de Trabalho</td>
	</tr>
	<tr>
		<td>
		
		<? exibirCadastramentoSupervisor(array('picid' => $_SESSION['sisindigena2']['supervisories']['picid'],'iusd' => $_SESSION['sisindigena2']['supervisories']['iusd'])); ?>
		
		</td>
		<td width="3%" valign="top">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sisindigena2']['supervisories']['docid'], array('picid' => $_SESSION['sisindigena2']['supervisories']['picid']) );
		?>
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center">
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/supervisories/supervisories&acao=A&aba=definirequipe';"> 
		</td>
	</tr>
</table>
	