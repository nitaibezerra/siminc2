<?php
monta_titulo( 'Observações Importantes', '' );

?>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td width="35%" align="center">
		
				<div style="width:40%; text-align:left;">
				<p style="margin: 20px 0; font-size: 16px;"><b>ATENÇÃO</b></p>
	
				<p>- O simples preenchimento e envio do formulário não garante sua participação no evento
				pretendido. Aguarde sua confirmação por e-mail.</p>
				
				<p>- Ao enviar sua ficha de pré-inscrição, sua chefia imediata receberá, automaticamente,
				por e-mail, uma mensagem para autorizar sua participação.</p>
				
				<p>- A autorização de sua chefia imediata é condição para que sua pré-inscrição seja
				avaliada pela equipe técnica do CETREMEC.</p>
				
				<p>- Caso haja desistência, entre em contato nos ramais do CETREMEC 7393, 7369, 7377, 7371, 
				com antecedência mínima de 48 horas.</p>
				
				<p><a href="http://intramec.mec.gov.br/component/docman/doc_download/1525-2010bol42supl.html" class="doclink">
				- Acesse aqui a portaria nº 1.507, de 28 de Outubro de 2010, que expede instruções
				específicas para a implementação da Política Nacional de Desenvolvimento de Pessoal,
				no âmbito da administração direta do Ministério da Educação.</a></p>
				</div>	
				
		
		</td>
	</tr>
</table>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="6" style="text-align: center" class="div_rolagem">
			<input type="button" id="btImprimir" value="Imprimir" onclick="javascript: window.print();" />
			<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
		</td>
	</tr>
</table>