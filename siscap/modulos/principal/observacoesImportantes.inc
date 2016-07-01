<?php
validaSessao( $_SESSION['siscap']['turid'], 'principal/cronogramaCurso' );

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Observações Importantes', '' );

?>	
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td width="35%" align="center">
		
				<div style="width:40%; text-align:justify;">
				<p style="margin: 20px 0; font-size: 16px;"><b>ATENÇÃO</b></p>
	
				<p>- O simples preenchimento e envio do formulário não garante sua participação no evento
				pretendido. Aguarde sua confirmação por e-mail.</p>
				
				<p>- Ao enviar sua ficha de pré-inscrição, sua chefia imediata receberá, automaticamente,
				por e-mail, uma mensagem para autorizar sua participação.</p>
				
				<p>- A autorização de sua chefia imediata é condição para que sua pré-inscrição seja
				avaliada pela equipe técnica do CETREMEC.</p>
				
				<p>- Caso haja desistência, informar ao CETREMEC por meio do endereço eletrônico cetremeccursos@mec.gov.br,
				com antecedência de até 2(dois) dias úteis da data de início do curso.
				
				<p><a href="http://intramec.mec.gov.br/component/docman/doc_download/1525-2010bol42supl.html" class="doclink">
				- Acesse aqui a Portaria nº 1.507, de 28 de outubro de 2010, que regulamenta a Política
				Nacional de Desenvolvimento de Pessoal no âmbito da administração direta do Ministério da
				Educação.</a></p>
				</div>		
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="50%">
	<tr bgcolor="#D0D0D0">
		<td colspan="4" style="text-align: center">					
			<input type="button" id="btVoltar" value="Voltar" onclick="voltar();" />
			<input type="button" id="btImprimir" value="Visualizar Impressão" onclick="imprimir();" />
			<input type="button" id="btAvancar" value="Avançar" onclick="inscreverServidor();" />
		</td>
	</tr>
</table>
<script language="JavaScript">

/*var btImprimir	=	$("btImprimir");
var btAvancar	=	$("btAvancar");
var btVoltar	=	$("btVoltar");*/

function voltar(){
	window.location.href='/siscap/siscap.php?modulo=principal/cronogramaCurso&acao=A';
}
function inscreverServidor(){
	window.location.href='siscap.php?modulo=principal/dadosServidor&acao=A';
}
function imprimir(){
	window.open('siscap.php?modulo=principal/popupImprimirObservacoesImportantes&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
}
</script>