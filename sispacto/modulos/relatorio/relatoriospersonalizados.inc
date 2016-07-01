<?php

include_once "_funcoes.php";

$perfis = pegaPerfilGeral();

if ( isset( $_REQUEST['buscar'] ) ) {
	include $_REQUEST['relatorio'];
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Personalizados - SISPACTO", "" );

if(in_array(PFL_COORDENADORIES,$perfis)) {
	
	$relatorios = array(0=>array('codigo'=>'relatpersonal_relatorioabrangencia.inc','descricao'=>'Relatório de Abrangência Municípios/Universidades'),
						1=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do PACTO'),
						2=>array('codigo'=>'relatpersonal_turmasgerais.inc','descricao'=>'Informações sobre dados gerais das turmas'),
						3=>array('codigo'=>'relatpersonal_aprendizagem.inc','descricao'=>'Informações sobre aprendizagem'),
						4=>array('codigo'=>'relatpersonal_evasao.inc','descricao'=>'Relatório de evasão do perfis'),
						5=>array('codigo'=>'relatpersonal_notacomplementar.inc','descricao'=>'Relatório de avaliação complementar'),
						6=>array('codigo'=>'relatpersonal_acompanhamentogeral.inc','descricao'=>'Relatório de Acompanhamento Geral')
						
						);
	
} else {
	
	$relatorios = array(0=>array('codigo'=>'relatpersonal_erroscadastrossgb.inc','descricao'=>'Relatório de erros no cadastro de bolsistas no SGB'),
						1=>array('codigo'=>'relatpersonal_aceitacaotermocompromisso.inc','descricao'=>'Relatório de adesão ao termo de compromisso'),
						2=>array('codigo'=>'relatpersonal_errospagamentossgb.inc','descricao'=>'Relatório de erros no envio de pagamentos no SGB'),
						3=>array('codigo'=>'relatpersonal_bolsistasparados.inc','descricao'=>'Relatório de Bolsistas sem tramitação do pagamento'),
						4=>array('codigo'=>'relatpersonal_relatorioabrangencia.inc','descricao'=>'Relatório de Abrangência Municípios/Universidades'),
						5=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do PACTO'),
						6=>array('codigo'=>'relatpersonal_numerobolsistas.inc','descricao'=>'Número de bolsistas do PACTO'),
						7=>array('codigo'=>'relatpersonal_turmasgerais.inc','descricao'=>'Informações sobre dados gerais das turmas'),
						8=>array('codigo'=>'relatpersonal_professorespacto.inc','descricao'=>'Professores participando do Pacto'),
						9=>array('codigo'=>'relatpersonal_aprendizagem.inc','descricao'=>'Informações sobre aprendizagem'),
						10=>array('codigo'=>'relatpersonal_turmasformadores.inc','descricao'=>'Turmas de formadores'),
						11=>array('codigo'=>'relatpersonal_quantitativos.inc','descricao'=>'Quantitativos do SISPACTO'),
						12=>array('codigo'=>'relatpersonal_dataplanoatv.inc','descricao'=>'Relatório das datas dos planos de atividades'),
						13=>array('codigo'=>'relatpersonal_gestaomobilizacao.inc','descricao'=>'Relatório personalizados Gestão e Mobilização'),
						14=>array('codigo'=>'relatpersonal_evasao.inc','descricao'=>'Relatório de evasão do perfis'),
						15=>array('codigo'=>'relatpersonal_notacomplementar.inc','descricao'=>'Relatório de avaliação complementar'),
						16=>array('codigo'=>'relatpersonal_acompanhamentogeral.inc','descricao'=>'Relatório de Acompanhamento Geral'),
						17=>array('codigo'=>'relatpersonal_relatoriofinal.inc','descricao'=>'Situação dos Relatórios Finais'),
						18=>array('codigo'=>'relatpersonal_desempenho.inc','descricao'=>'Desempenho das redes estaduais e municipais (físico e financeiro)'),
						19=>array('codigo'=>'relatpersonal_analiserelatoriofinal.inc','descricao'=>'Análise do relatório final'),
						20=>array('codigo'=>'relatpersonal_orcamento.inc','descricao'=>'Análise de orçamento'),
						21=>array('codigo'=>'relatpersonal_inep.inc','descricao'=>'Relatório de INEP'),
			
						
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