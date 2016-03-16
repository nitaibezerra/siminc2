<?php
unset($_SESSION['snf']['insid']);

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
$arrPerfil = pegaPerfilGeral();
?>
<br />
<?php 
	monta_titulo( "Apresentação", '&nbsp' ); 
?>
<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="tabela">
	<tr>
		<td align="center" >
			<div class="termoAdesao" >
				<h2 class="center" >Apresentação</h2>
				<h4 class="center" >Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica</h4>
				<p><b>Instituição:</b> <a target="_blank" title="Portaria 1.328"  href="../snf/geral/portaria_1328_2011.pdf"  >Portaria nº 1.328</a>, de 23 de setembro de 2011, publicada no Diário Oficial da União de 26 de setembro de 2011, seção 1, página 14.</p>
				<p><b>Composição:</b> é formada pelas Instituições de Educação Superior (IES), públicas e comunitárias sem fins lucrativos, e pelos Institutos Federais de Educação, Ciência e Tecnologia (IF).</p>
				<p><b>Objetivo:</b> apoiar as ações de formação continuada de profissionais do magistério da educação básica e em atendimento às demandas de formação continuada formuladas nos planos estratégicos de que tratam os artigos 4º, 5º, e 6º do <a target="_blank" title="Portaria 1.328"  href="../snf/geral/decreto_6755_2009.pdf"  >Decreto n° 6.755</a>, de 29 de janeiro de 2009.</p>
				<p><b>Coordenação e supervisão:</b> exercida pelo Comitê Gestor da Política Nacional de Formação Inicial e Continuada de Profissionais da Educação Básica, instituído pela <a target="_blank" title="Portaria 1.328"  href="../snf/geral/portaria_1087_2011.pdf"  >Portaria nº 1.087</a>, de 10 de agosto de 2011.</p>
				<p><b>Forma de atuação:</b> em articulação com os sistemas de ensino e com os Fóruns Estaduais Permanentes de Apoio à Formação Docente.</p>
				<p><b>Ingresso das IES e IF à Rede:</b> por meio do termo de adesão eletrônico disponível abaixo, tendo como anexo o ato constitutivo do Comitê Gestor Institucional de Formação de Profissionais do Magistério da Educação Básica, conforme disposto na <a target="_blank" title="Portaria 1.328"  href="../snf/geral/resolucao_1_2011.pdf"  >Resolução nº 1</a>, de 17 de agosto de 2011, do Comitê Gestor da Política Nacional de Formação Inicial e Continuada de Profissionais da Educação Básica.</p>
			</div>
		</td>
	</tr>
	<tr class="center SubtituloTabela" >
		<td>
			<input type="button" onclick="window.location.href='snf.php?modulo=principal/visualizarTermoAdesao&acao=A'" name="btn_visualizar_modelo" value="Visualizar Termo de Adesão"  />
			<?php if(!in_array(PERFIL_CONSULTA,$arrPerfil)): ?>
				<input type="button" onclick="window.location.href='snf.php?modulo=principal/aderirTermoAdesao&acao=A'" onclick="" name="btn_proceder_adesao" value="Proceder à Adesão"  />
			<?php endif; ?>
		</td>
	</tr>
</table>
