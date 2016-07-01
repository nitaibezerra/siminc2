<?php

include_once "_funcoes.php";
include_once "_constantes.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


$perfis = pegaPerfilGeral();

if ( isset( $_REQUEST['buscar'] ) ) {
	include $_REQUEST['relatorio'];
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Personalizados - SISFOR", "" );

$relatorios = array(//0=>array('codigo'=>'relatpersonal_erroscadastrossgb.inc','descricao'=>'Relatório de erros no cadastro de bolsistas no SGB'),
						//1=>array('codigo'=>'relatpersonal_aceitacaotermocompromisso.inc','descricao'=>'Relatório de adesão ao termo de compromisso'),
						//2=>array('codigo'=>'relatpersonal_errospagamentossgb.inc','descricao'=>'Relatório de erros no envio de pagamentos no SGB'),
						//3=>array('codigo'=>'relatpersonal_bolsistasparados.inc','descricao'=>'Relatório de Bolsistas sem tramitação do pagamento'),
						//0=>array('codigo'=>'relatpersonal_quantitativos.inc','descricao'=>'Relatório de Quantitativos'),
						//5=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do PACTO'),
						//6=>array('codigo'=>'relatpersonal_numerobolsistas.inc','descricao'=>'Número de bolsistas do PACTO'),
						//7=>array('codigo'=>'relatpersonal_turmasgerais.inc','descricao'=>'Informações sobre dados gerais das turmas'),
						//8=>array('codigo'=>'relatpersonal_professorespacto.inc','descricao'=>'Professores participando do Pacto'),
						//9=>array('codigo'=>'relatpersonal_aprendizagem.inc','descricao'=>'Informações sobre aprendizagem'),
						//10=>array('codigo'=>'relatpersonal_turmasformadores.inc','descricao'=>'Turmas de formadores'),
						//11=>array('codigo'=>'relatpersonal_quantitativos.inc','descricao'=>'Quantitativos do SISPACTO'),
						//1=>array('codigo'=>'relatpersonal_dataplanoatv.inc','descricao'=>'Relatório das datas dos planos de atividades'),
						0=>array('codigo'=>'relatpersonal_contatosbolsistas.inc','descricao'=>'Relatório de contato dos bolsistas'),
						1=>array('codigo'=>'relatpersonal_cursistas.inc','descricao'=>'Relatório dos cursistas do SISFOR'),
						2=>array('codigo'=>'relatpersonal_bolsasprojeto.inc','descricao'=>'Relatório qtds de bolsas no projeto'),
						3=>array('codigo'=>'relatpersonal_infogeralcursos.inc','descricao'=>'Informações gerais sobre os cursos'),
						4=>array('codigo'=>'relatpersonal_cursoandamento_cargahoraria.inc','descricao'=>'Informações sobre o curso em andamento: Carga horária'),
						5=>array('codigo'=>'relatpersonal_cursoandamento_vigencia.inc','descricao'=>'Informações sobre o curso em andamento: Vigência'),
						6=>array('codigo'=>'relatpersonal_cursoandamento_vagaspreenchidas.inc','descricao'=>'Informações sobre o curso em andamento: Vagas Preenchidas'),
						7=>array('codigo'=>'relatpersonal_formacao_quantitativo.inc','descricao'=>'Informações sobre os perfis atuantes na formação: Quantitativos'),
						8=>array('codigo'=>'relatpersonal_formacao_escolaridade.inc','descricao'=>'Informações sobre os perfis atuantes na formação: Escolaridade'),
						9=>array('codigo'=>'relatpersonal_formacao_sexo.inc','descricao'=>'Informações sobre os perfis atuantes na formação: Sexo'),
						10=>array('codigo'=>'relatpersonal_formacao_idade.inc','descricao'=>'Informações sobre os perfis atuantes na formação: Idade'),
						11=>array('codigo'=>'relatpersonal_bolsasformacao.inc','descricao'=>'Informações sobre as bolsas vinculadas/pagas aos perfis de formação'),
						12=>array('codigo'=>'relatpersonal_execucao2014.inc','descricao'=>'Relatório de execução 2014'),
						13=>array('codigo'=>'relatpersonal_valoresies.inc','descricao'=>'Relatório de valores IES'),
						14=>array('codigo'=>'relatpersonal_vigenciacursos.inc','descricao'=>'Vigência do curso e dos bolsistas'),
						15=>array('codigo'=>'relatpersonal_bolsistasemtermo.inc','descricao'=>'Lista de Bolsistas sem termo de compromisso'),
						16=>array('codigo'=>'relatpersonal_andamentocursos.inc','descricao'=>'Andamento dos cursos')
						//17=>array('codigo'=>'relatpersonal_relatoriofinal.inc','descricao'=>'Situação dos Relatórios Finais')
			
						
						);
	



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
			$db->monta_combo('relatorio', $relatorios, 'S', 'Selecione', '', '', '', '', 'S', 'relatorio');
			?>
			</td>
		</tr>

	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
		</tr>
</table>