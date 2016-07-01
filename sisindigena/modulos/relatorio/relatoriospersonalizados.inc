<?php

include_once "_funcoes.php";

$perfis = pegaPerfilGeral();

if ( isset( $_REQUEST['buscar'] ) ) {
	include $_REQUEST['relatorio'];
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Personalizados - SISIndígena", "" );

$relatorios = array(//0=>array('codigo'=>'relatpersonal_erroscadastrossgb.inc','descricao'=>'Relatório de erros no cadastro de bolsistas no SGB'),
					//1=>array('codigo'=>'relatpersonal_aceitacaotermocompromisso.inc','descricao'=>'Relatório de adesão ao termo de compromisso'),
					//2=>array('codigo'=>'relatpersonal_errospagamentossgb.inc','descricao'=>'Relatório de erros no envio de pagamentos no SGB'),
					//3=>array('codigo'=>'relatpersonal_bolsistasparados.inc','descricao'=>'Relatório de Bolsistas sem tramitação do pagamento'),
					0=>array('codigo'=>'relatpersonal_perfilparticipantes.inc','descricao'=>'Perfil dos Participantes do SISIndígena'),
					1=>array('codigo'=>'relatpersonal_relatorioabrangencia.inc','descricao'=>'Relatório de abrangência nos Municípios'),
					2=>array('codigo'=>'relatpersonal_relatorioabrangencia2.inc','descricao'=>'Relatório de abrangência geral'),
					3=>array('codigo'=>'relatpersonal_certificados.inc','descricao'=>'Relatório de certificação')
					//9=>array('codigo'=>'relatpersonal_aprendizagem.inc','descricao'=>'Informações sobre aprendizagem'),
					//10=>array('codigo'=>'relatpersonal_turmasformadores.inc','descricao'=>'Turmas de formadores'),
					//11=>array('codigo'=>'relatpersonal_quantitativos.inc','descricao'=>'Quantitativos do SISPACTO'),
					//2=>array('codigo'=>'relatpersonal_dataplanoatv.inc','descricao'=>'Relatório das datas dos planos de atividades'),
					//13=>array('codigo'=>'relatpersonal_gestaomobilizacao.inc','descricao'=>'Relatório personalizados Gestão e Mobilização'),
					//14=>array('codigo'=>'relatpersonal_evasao.inc','descricao'=>'Relatório de evasão do perfis'),
					//15=>array('codigo'=>'relatpersonal_notacomplementar.inc','descricao'=>'Relatório de avaliação complementar'),
					//16=>array('codigo'=>'relatpersonal_acompanhamentogeral.inc','descricao'=>'Relatório de Acompanhamento Geral'),
					//17=>array('codigo'=>'relatpersonal_relatoriofinal.inc','descricao'=>'Situação dos Relatórios Finais')
		
					
					);
	


?>
<script type="text/javascript">
function exibirRelatorio() {
	if(document.getElementById('relatorio').value!='') {
		var formulario = document.formulario;
		// submete formulario
		formulario.target = 'relatoriopersonlizadossisindigena';
		var janela = window.open( '', 'relatoriopersonlizadossisindigena', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
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