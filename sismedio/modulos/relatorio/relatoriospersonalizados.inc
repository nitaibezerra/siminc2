<?php

include_once "_funcoes.php";

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

monta_titulo( "Relatório Personalizados - SISMédio", "" );


if(in_array(PFL_COORDENADORIES,$perfis)) {

	$relatorios = array(
			0=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do SISMédio'),
			1=>array('codigo'=>'relatpersonal_notacomplementar.inc','descricao'=>'Relatório de avaliação complementar'),
			2=>array('codigo'=>'relatpersonal_atividadesobr.inc','descricao'=>'Relatório de Atividades obrigatórias'),
			3=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do SISMédio'),
			4=>array('codigo'=>'relatpersonal_notacomplementar.inc','descricao'=>'Relatório de avaliação complementar'),
			5=>array('codigo'=>'relatpersonal_atividadesobr.inc','descricao'=>'Relatório de Atividades obrigatórias')
			//1=>array('codigo'=>'relatpersonal_aprendizagemmat.inc','descricao'=>'Informações sobre aprendizagem (Matemática)'),
			//2=>array('codigo'=>'relatpersonal_aprendizagempor.inc','descricao'=>'Informações sobre aprendizagem (Português)'),
			//3=>array('codigo'=>'relatpersonal_materiais.inc','descricao'=>'Informações a entrega dos materiais'),
			//4=>array('codigo'=>'relatpersonal_aprendizagemdid.inc','descricao'=>'Informações sobre uso do materiais didáticos'),
			//5=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do PACTO 2014'),
			//6=>array('codigo'=>'relatpersonal_impressaoana.inc','descricao'=>'Relatório de impressão ANA')
	);

} else {

	$relatorios = array(//0=>array('codigo'=>'relatpersonal_erroscadastrossgb.inc','descricao'=>'Relatório de erros no cadastro de bolsistas no SGB'),
							//1=>array('codigo'=>'relatpersonal_aceitacaotermocompromisso.inc','descricao'=>'Relatório de adesão ao termo de compromisso'),
							//2=>array('codigo'=>'relatpersonal_errospagamentossgb.inc','descricao'=>'Relatório de erros no envio de pagamentos no SGB'),
							//3=>array('codigo'=>'relatpersonal_bolsistasparados.inc','descricao'=>'Relatório de Bolsistas sem tramitação do pagamento'),
							0=>array('codigo'=>'relatpersonal_quantitativos.inc','descricao'=>'Relatório de Quantitativos'),
							//5=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do PACTO'),
							//6=>array('codigo'=>'relatpersonal_numerobolsistas.inc','descricao'=>'Número de bolsistas do PACTO'),
							//7=>array('codigo'=>'relatpersonal_turmasgerais.inc','descricao'=>'Informações sobre dados gerais das turmas'),
							//8=>array('codigo'=>'relatpersonal_professorespacto.inc','descricao'=>'Professores participando do Pacto'),
							//9=>array('codigo'=>'relatpersonal_aprendizagem.inc','descricao'=>'Informações sobre aprendizagem'),
							//10=>array('codigo'=>'relatpersonal_turmasformadores.inc','descricao'=>'Turmas de formadores'),
							1=>array('codigo'=>'relatpersonal_dataplanoatv.inc','descricao'=>'Relatório das datas dos planos de atividades'),
							2=>array('codigo'=>'relatpersonal_contatosbolsistas.inc','descricao'=>'Relatório de contato dos bolsistas'),
							3=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do SISMédio'),
							//13=>array('codigo'=>'relatpersonal_gestaomobilizacao.inc','descricao'=>'Relatório personalizados Gestão e Mobilização'),
							//14=>array('codigo'=>'relatpersonal_evasao.inc','descricao'=>'Relatório de evasão do perfis'),
							4=>array('codigo'=>'relatpersonal_notacomplementar.inc','descricao'=>'Relatório de avaliação complementar'),
							5=>array('codigo'=>'relatpersonal_atividadesobr.inc','descricao'=>'Relatório de Atividades obrigatórias'),
							6=>array('codigo'=>'relatpersonal_aval_oe.inc','descricao'=>'Relatório Avaliação final da Formação - OE'),
							7=>array('codigo'=>'relatpersonal_aval_fr.inc','descricao'=>'Relatório Avaliação final da Formação - FR'),
							8=>array('codigo'=>'relatpersonal_adesao.inc','descricao'=>'Informações sobre adesões'),
							9=>array('codigo'=>'relatpersonal_acessoparticipacao.inc','descricao'=>'Relatório de acesso e participação (UF)'),
							10=>array('codigo'=>'relatpersonal_eficienciarendimento.inc','descricao'=>'Relatório de Eficiência/Rendimento'),
							11=>array('codigo'=>'relatpersonal_bolsaspagasperfil.inc','descricao'=>'Número de Bolsas pagas por Perfil')
							//17=>array('codigo'=>'relatpersonal_relatoriofinal.inc','descricao'=>'Situação dos Relatórios Finais')
				
							
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