<?php
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>
<br />
<?php $db->cria_aba($abacod_tela,$url,''); ?>
<?php monta_titulo( "Termo de Adesão", '&nbsp' ); ?>
<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="tabela">
	<tr>
		<td align="center" >
			<fieldset style="text-align:justify;width:60%;font-size:14px;margin:5px"  >
				<legend>Instruções</legend>
				<center><img src="../imagens/brasao.jpg" style="width:100px;height:100px"   /></center>
				<h3 class="center" >MINISTÉRIO DA EDUCAÇÃO</h3>
				<h3 class="center" >TERMO DE ADESÃO À REDE NACIONAL DE FORMAÇÃO CONTINUADA DOS PROFISSIONAIS DO MAGISTÉRIO DA EDUCAÇÃO BÁSICA PÚBLICA</h3>
				<p>O(A) [nome e sigla da Instituição], inscrita no CNPJ sob o nº _______________, neste ato representado(a) por seu(sua) dirigente máximo(a),  [nome e CPF], resolve formalizar sua adesão à Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica Pública.</p>
				<h4>DO OBJETIVO</h4>
				<p>CLÁUSULA PRIMEIRA – Participar como Instituição de Ensino Superior formadora da oferta de cursos e programas no âmbito da Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica Pública, nos termos da Portaria MEC n° 1.328, de 23 de setembro de 2011, publicada na página 14 da seção 01 do Diário Oficial da União no dia 26 de setembro de 2011 e das demais normas que venham a substituir ou complementar a legislação vigente, habilitando-se ao recebimento de recursos do MEC destinados a fomentar as ações da Rede e em atendimento às demandas de formação continuada formuladas nos planos estratégicos de que tratam os artigos 4º, 5º, e 6º do Decreto n° 6.755, de 29 de janeiro de 2009.</p>
				<h4>DA ADESÃO</h4>
				<p>CLÁUSULA SEGUNDA – Esta adesão, solicitada de forma eletronicamente pelo titular da Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia, junto com o ato de nomeação do signatário e do ato constitutivo do Comitê Gestor Institucional de Formação de Profissionais do Magistério da Educação Básica, tem eficácia após validação pelo Ministério da Educação.</p>
				<p>Parágrafo único: O apoio financeiro concedido à Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia será realizado a partir do próximo exercício fiscal, desde que a adesão ocorra até 31 de maio, ou somente a partir do exercício seguinte, se a adesão for posterior.</p>
				<h4>DA PARTICIPAÇÃO</h4>
				<p>CLÁUSULA TERCEIRA – A adesão abrange Instituições de Educação Superior (IES), públicas e comunitárias sem fins lucrativos, e Institutos Federais de Educação, Ciência e Tecnologia (IF) habilitados a ofertar cursos ou programas de formação continuada aos profissionais do magistério da educação básica de forma articulada com os sistemas de ensino e com os Fóruns Estaduais Permanentes de Apoio à Formação Docente.</p>
				<p>Parágrafo único: Os cursos e programas de formação continuada, após homologação no Comitê Gestor Institucional de Formação de Profissionais do Magistério da Educação Básica, deverão ser submetidos pelas Instituições de Ensino Superior e Institutos Federais de Educação, Ciência e Tecnologia, periodicamente, nos termos e prazos definidos pelos Fóruns Estaduais Permanentes de Apoio à Formação Docente, para posterior aprovação do fomento pelo MEC.</p> 
				<h4>DA VIGÊNCIA</h4>
				<p>CLÁUSULA QUARTA – Uma vez formalizada a adesão à Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica Pública, sua vigência é válida por tempo indeterminado, ou até que seja solicitado o seu cancelamento pela Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia, a qualquer tempo, mediante ofício assinado por seu titular ao Comitê Gestor da Política Nacional de Formação Inicial e Continuada de Profissionais da Educação Básica, implicando a interrupção definitiva do apoio financeiro aos cursos e programas fomentados pelo MEC.</p>
				<h4>DA ALTERAÇÃO OU DESISTÊNCIA</h4>
				<p>CLÁUSULA QUINTA – Fica a Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia obrigado a solicitar a alteração do Plano de Trabalho para fomento de cursos e programas no âmbito da Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica, sempre que caracterizada necessidade de alteração ou desistência de oferta, mediante envio de ofício do titular ao Comitê Gestor da Política Nacional de Formação Inicial e Continuada de Profissionais da Educação Básica, para interrupção do apoio financeiro, com duração sujeita aos mesmos prazos descritos no parágrafo segundo da Cláusula Segunda.</p>
				<h4>DA PUBLICIDADE</h4>
				<p>CLÁUSULA SEXTA – As opções por adesão, seu cancelamento, alteração ou desistência de oferta serão divulgadas em listas publicadas no Portal do Ministério da Educação na internet.</p>
				<p>E, por estar de acordo com todas as condições e cláusulas deste Termo de Adesão, firmo o presente instrumento.</p>
   				<p><input type="checkbox" /> Declaro que li e concordo com o Termo de Adesão.</p>
				<p class="center" >(Local e data)</p>
				<p class="center" >[Nome do dirigente máximo da Instituição]</p>
				<p class="center" >[CPF]</p>
			</fieldset>
		</td>
	</tr>
	<tr class="center SubtituloTabela" >
		<td>
			<input type="button" onclick="window.location.href='snf.php?modulo=inicio&acao=C'" name="btn_voltar" value="Voltar"  />
		</td>
	</tr>
</table>
