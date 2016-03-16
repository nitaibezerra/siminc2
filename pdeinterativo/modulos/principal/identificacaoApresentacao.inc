<?php
monta_titulo( "Apresentação", '&nbsp' );
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
	<tr>
		<td>
			<img border="0" src="/imagens/bussola.png" />
		</td>
		<td>
		<font style="color:#1719fd;">
		<p>Seja bem vindo(a) ao PDE Interativo!</p>
		<p>O PDE Interativo é uma ferramenta de apoio à gestão escolar desenvolvida pelo Ministério da Educação, em parceria com as Secretarias de Educação. Ele foi concebido a partir da metodologia de planejamento estratégico utilizada pelo PDE Escola e sua formulação teve como principal objetivo universalizar a metodologia, tornando-a acessível a todas as escolas e secretarias interessadas.</p> 
		<p>Uma inovação importante desta ferramenta é o seu caráter auto-instrucional, o que significa que não é necessário realizar uma formação específica para conhecer a metodologia. Basta ler as orietações disponíveis em cada tela e refletir coletivamente sobre os problemas e desafios identificados. O PDE Interativo também possibilita que as Secretarias e o Ministério da Educação conheçam melhor as escolas e, a partir daí, proponham políticas públicas cada vez mais aderentes às necessidades delas.</p>
		<p>Antes de iniciar a navegação e elaboração do plano, é importante entender como o sistema está organizado:</p>
		<p>
		1) A metodologia do PDE Interativo envolve quatro grandes etapas: Identificação, Primeiros Passos, Diagnóstico e Plano Geral.<br/>
		2) O Plano Geral está dividido em planos específicos e este ano a escola poderá visualizar o Plano de Formação (disponível para todas as escolas) e o PDE Escola (disponível apenas para as escolas priorizada por esse programa).<br/>
		3) Esses dois planos (PDE Escola e Plano de Formação) possuem fluxos de análise independentes. Ou seja, não é obrigatório concluir o PDE Escola para enviar o Plano de Formação e vice-versa.<br/>
		</p>
		<p>Em caso de dúvida, ligue para 0800 616161 ou envie um e-mail para pdeinterativo@mec.gov.br.</p>
		</font>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center" style="border-top:1px solid black;">
	<tr>
		<td align="right" class="SubTituloDireita" width="350px">
			O que deseja fazer agora?
		</td>
		<td>
			<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/principal&acao=A'" >
			<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Diretor'" >
		</td>
	</tr>
</table>