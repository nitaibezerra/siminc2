<?php 

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
jQuery.noConflict();
function abrePopupRelatorio() {
 	var checkBt = document.getElementById("aceita_adesao");
	var url;

	if(checkBt.checked) {
		jQuery('#aceita_adesao').attr('disabled', true);		
		url = 'pdeescola.php?modulo=merelatorio/plano_atendimento_geral_consolidado&acao=A';
		params = 'width=780,height=560,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1';
		window.location.href = url;
	}
	
// 	var janela = window.open( url, 'relatorio', params );
// 	janela.focus();
}
</script>
<center>
<img src="../imagens/brasao.gif" height="80"/>
<h3>
MINISTÉRIO DA EDUCAÇÃO<br/>
SECRETARIA DE EDUCAÇÃO BÁSICA<br/>
DIRETORIA DE CURRÍCULOS E EDUCAÇÃO INTEGRAL<br/><br/><br/>
TERMO DE COMPROMISSO AO PROGRAMA MAIS EDUCAÇÃO<br/>
</h3>
</center>

<div style="text-align:justify;line-height:180%;padding:15px;">
A Entidade Executora (EEx) firma o compromisso de acatar, cumprir e fazer cumprir as disposições da Resolução do Conselho Deliberativo do FNDE, que dispõe, no corrente exercício, dos processos de adesão e habilitação, formas de execução e prestação de contas, referente ao Programa Dinheiro Direto na Escola/Educação Integral (PDDE/Integral).
<br/>
Este termo acatado formaliza a adesão ao Programa Mais Educação das escolas relacionadas no Plano Geral Consolidado de sua rede de ensino.
</div> 

<p>&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="aceita" class="aceitaAdesao" id="aceita_adesao" onclick="javascript:abrePopupRelatorio()" />&nbsp;Aceito</p>