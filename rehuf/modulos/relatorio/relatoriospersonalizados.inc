<?php
if ( isset( $_REQUEST['buscar'] ) ) {
	include $_REQUEST['relatorio'];
	exit;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Personalizados - REHUF", "" );

?>
<script type="text/javascript">
function exibirRelatorio() {
	if(document.getElementById('relatorio').value!='') {
		var formulario = document.formulario;
		// submete formulario
		formulario.target = 'relatoriopersonlizadosrehuf';
		var janela = window.open( '', 'relatoriopersonlizadosrehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
		formulario.submit();
		janela.focus();
	} else {
		alert("Selecione um relatório");
		return false;
	}
}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td>Filtros</td></tr>		
		<tr>
			<td class="SubTituloDireita" valign="top">Relatórios :</td>
			<td>
			<?
			$relatorios = array(0=>array('codigo'=>'relatpersonal_sustentsus.inc','descricao'=>'Relatório de Sustentabilidade SUS (Ideal) (2004-2008)'),
								1=>array('codigo'=>'relatpersonal_leitos.inc','descricao'=>'Relatório de Leitos (2004-2008)'),
								2=>array('codigo'=>'relatpersonal_leitos2009.inc','descricao'=>'Relatório de Leitos (2009-2010)'),
								3=>array('codigo'=>'relatpersonal_recursosus.inc','descricao'=>'Relatório de Recursos do SUS (2004-2008)'),
								4=>array('codigo'=>'relatpersonal_tipocontratacao.inc','descricao'=>'Relatório de quantidade de pessoal (2004-2008)'),
								5=>array('codigo'=>'relatpersonal_sustentsusreal.inc','descricao'=>'Relatório de Sustentabilidade SUS (Real) (2004-2008)'),
								6=>array('codigo'=>'relatpersonal_ftestdes2008.inc','descricao'=>'Relatório de Fatores Estratégicos de Desempenho (2008)'),
								7=>array('codigo'=>'relatpersonal_altacomplexidade.inc','descricao'=>'Relatório de Procedimentos de Alta Complexidade (2008-2009)'),
								8=>array('codigo'=>'relatpersonal_reccorr.inc','descricao'=>'Relatório de Saldo Corrente (2004-2008)'),
								9=>array('codigo'=>'relatpersonal_plantoeshosp.inc','descricao'=>'Relatório de plantões hospitalares (2009)'),
								10=>array('codigo'=>'relatpersonal_plantoeshosp2010.inc','descricao'=>'Relatório de plantões hospitalares (2010)'),
								11=>array('codigo'=>'relatpersonal_plantoeshosp2011.inc','descricao'=>'Relatório de plantões hospitalares (2011)'),
								12=>array('codigo'=>'relatpersonal_plantoeshosp2012.inc','descricao'=>'Relatório de plantões hospitalares (2012)'),
								13=>array('codigo'=>'relatpersonal_plantoeshosp2013.inc','descricao'=>'Relatório de plantões hospitalares (2013)'),
								14=>array('codigo'=>'relatpersonal_matrizdistribuicao.inc','descricao'=>'Matriz de distribuição (2009)'),
								15=>array('codigo'=>'relatpersonal_matrizdistribuicao2010.inc','descricao'=>'Matriz de distribuição (2010)'),
								16=>array('codigo'=>'relatpersonal_matrizdistribuicao2011.inc','descricao'=>'Matriz de distribuição (2011)'),
								17=>array('codigo'=>'relatpersonal_matrizdistribuicao2012.inc','descricao'=>'Matriz de distribuição (2012) - 1º'),
								18=>array('codigo'=>'relatpersonal_matrizdistribuicao2013.inc','descricao'=>'Matriz de distribuição (2013)'),
								19=>array('codigo'=>'relatpersonal_matrizdistribuicao2014.inc','descricao'=>'Matriz de distribuição (2014)'),
								20=>array('codigo'=>'relatpersonal_faec.inc','descricao'=>'Relatório de Procedimentos de FAEC (2008-2009)'),
								21=>array('codigo'=>'relatpersonal_receitacorrente.inc','descricao'=>'Relatório de Receita Corrente (2009)'),
								22=>array('codigo'=>'relatpersonal_hospitalendereco.inc','descricao'=>'Relatório de dados dos Hospitais'),
								23=>array('codigo'=>'relatpersonal_taxaspartocesario.inc','descricao'=>'Taxas de Partos Cesáreos'));
			$db->monta_combo('relatorio', $relatorios, 'S', 'Selecione', '', '', '', '400', 'S', 'relatorio');
			?>
			</td>
		</tr>

	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
		</tr>
</table>