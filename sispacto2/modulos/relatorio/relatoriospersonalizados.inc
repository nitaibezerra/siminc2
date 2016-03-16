<?php

include_once "_funcoes.php";

$perfis = pegaPerfilGeral();

if ( isset( $_REQUEST['buscar'] ) ) {
	include $_REQUEST['relatorio'];
	exit;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Personalizados - SISPACTO", "" );


if(in_array(PFL_COORDENADORIES,$perfis)) {
	
	
	$relatorios = array(
			0=>array('codigo'=>'relatpersonal_turmasgerais.inc','descricao'=>'Informações sobre dados gerais das turmas'),
			1=>array('codigo'=>'relatpersonal_relatoexperiencia.inc','descricao'=>'Relato Experiência'),
			2=>array('codigo'=>'relatpersonal_aprendizagemmat.inc','descricao'=>'Informações sobre aprendizagem (Matemática)'),
			3=>array('codigo'=>'relatpersonal_aprendizagempor.inc','descricao'=>'Informações sobre aprendizagem (Português)'),
			4=>array('codigo'=>'relatpersonal_materiais.inc','descricao'=>'Informações a entrega dos materiais'),
			5=>array('codigo'=>'relatpersonal_aprendizagemdid.inc','descricao'=>'Informações sobre uso do materiais didáticos'),
			6=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do PACTO 2014'),
			7=>array('codigo'=>'relatpersonal_impressaoana.inc','descricao'=>'Relatório de impressão ANA'),
			8=>array('codigo'=>'relatpersonal_questoesatv8.inc','descricao'=>'Relatório das questões da atividade 8'),
			9=>array('codigo'=>'relatpersonal_quantitativos.inc','descricao'=>'Quantitativos do SISPACTO 2014')

	);

} else {

	
	$relatorios = array(//0=>array('codigo'=>'relatpersonal_erroscadastrossgb.inc','descricao'=>'Relatório de erros no cadastro de bolsistas no SGB'),
						//1=>array('codigo'=>'relatpersonal_aceitacaotermocompromisso.inc','descricao'=>'Relatório de adesão ao termo de compromisso'),
						//2=>array('codigo'=>'relatpersonal_errospagamentossgb.inc','descricao'=>'Relatório de erros no envio de pagamentos no SGB'),
						//3=>array('codigo'=>'relatpersonal_bolsistasparados.inc','descricao'=>'Relatório de Bolsistas sem tramitação do pagamento'),
						0=>array('codigo'=>'relatpersonal_relatorioabrangencia.inc','descricao'=>'Relatório de Abrangência Municípios/Universidades'),
						//6=>array('codigo'=>'relatpersonal_numerobolsistas.inc','descricao'=>'Número de bolsistas do PACTO'),
						1=>array('codigo'=>'relatpersonal_turmasgerais.inc','descricao'=>'Informações sobre dados gerais das turmas'),
						//8=>array('codigo'=>'relatpersonal_professorespacto.inc','descricao'=>'Professores participando do Pacto'),
						2=>array('codigo'=>'relatpersonal_aprendizagemmat.inc','descricao'=>'Informações sobre aprendizagem (Matemática)'),
					    3=>array('codigo'=>'relatpersonal_aprendizagempor.inc','descricao'=>'Informações sobre aprendizagem (Português)'),
						4=>array('codigo'=>'relatpersonal_materiais.inc','descricao'=>'Informações a entrega dos materiais'),
						5=>array('codigo'=>'relatpersonal_aprendizagemdid.inc','descricao'=>'Informações sobre uso do materiais didáticos'),
						//10=>array('codigo'=>'relatpersonal_turmasformadores.inc','descricao'=>'Turmas de formadores'),
						6=>array('codigo'=>'relatpersonal_dataplanoatv.inc','descricao'=>'Relatório das datas dos planos de atividades'),
						7=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do PACTO 2014'),
						8=>array('codigo'=>'relatpersonal_contatosbolsistas.inc','descricao'=>'Relatório de contato dos bolsistas'),
						9=>array('codigo'=>'relatpersonal_impressaoana.inc','descricao'=>'Relatório de impressão ANA'),
						10=>array('codigo'=>'relatpersonal_relatoexperiencia.inc','descricao'=>'Relato Experiência'),
						11=>array('codigo'=>'relatpersonal_questoesatv8.inc','descricao'=>'Relatório das questões da atividade 8'),
						12=>array('codigo'=>'relatpersonal_quantitativos.inc','descricao'=>'Quantitativos do SISPACTO 2014'),
						13=>array('codigo'=>'relatpersonal_adesao.inc','descricao'=>'Informações sobre adesões'),
						14=>array('codigo'=>'relatpersonal_instrumentos.inc','descricao'=>'Relatório dos instrumentos'),
						15=>array('codigo'=>'relatpersonal_previsaomateriais.inc','descricao'=>'Planilha de distribuição de material do Pacto da Alfabetização 2015 - Censo 2014 homologado'),
						16=>array('codigo'=>'relatpersonal_orcamento.inc','descricao'=>'Relatório de orçamento'),
						17=>array('codigo'=>'relatpersonal_atividadecontribuicao.inc','descricao'=>'Relatório de contribuição do PACTO'),
						18=>array('codigo'=>'relatpersonal_instrumentosabertos.inc','descricao'=>'Relatório dos instrumentos dissertativos'),
						19=>array('codigo'=>'relatpersonal_inep.inc','descricao'=>'Relatório de INEP')
						
						);
	
}


?>
<script type="text/javascript">
function exibirRelatorio() {
	if(document.getElementById('relatorio').value!='') {
		var formulario = document.formulario;
		// submete formulario
		formulario.target = 'relatoriopersonlizadossispacto';
		var janela = window.open( '', 'relatoriopersonlizadossispacto', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
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
			$db->monta_combo('relatorio', $relatorios, 'S', 'Selecione', '', '', '', '400', 'S', 'relatorio');
			?>
			</td>
		</tr>

	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
		</tr>
</table>