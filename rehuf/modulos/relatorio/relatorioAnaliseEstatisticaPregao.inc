<?php
require APPRAIZ . 'rehuf/classes/Pregao.class.inc';

if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}


if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	exit;
}

switch($_REQUEST['buscar']) {
	case '1':
		include "relatorioAnaliseEstatisticaPregao_resultado.inc";
		exit;
	case '2':
		include "relatorioAnaliseEstatisticaPregao_resultado.inc";
		exit;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório de Análise Estatística por Pregão - REHUF", "" );

//$permissoes = verificaPerfilRehuf();

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	formulario.buscar.value='1';
	if(formulario.preid.value=="") {
		alert("Selecione o Pregão.");
		return false;
	}

	if(parseInt(formulario.inferioritem.value) < 0 || parseInt(formulario.superioritem.value) < 0){
		alert("As porcentagens de preenchimento dos Itens não podem ser negativas");
		return false;
	}

	if(parseInt(formulario.inferiorHU.value) < 0 || parseInt(formulario.superiorHU.value) < 0){
		alert("As porcentagens de participação dos HU's não podem ser negativas");
		return false;
	}

	if(formulario.inferioritem.value != "" && formulario.superioritem.value != "" && parseInt(formulario.inferioritem.value) > parseInt(formulario.superioritem.value)){
		alert("A porcentagem de preenchimento do Item superior deve ser maior que a inferior");
		return false;
	}

	if(formulario.inferiorHU.value != "" && formulario.superiorHU.value != "" && parseInt(formulario.inferiorHU.value) > parseInt(formulario.superiorHU.value)){
		alert("A porcentagem de participação do HU superior deve ser maior que a inferior");
		return false;
	}
	
	// submete formulario
	formulario.target = 'relatoriopregaorehuf';
	var janela = window.open( '', 'relatoriopregaorehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}

function exibirRelatorioXLS(){
	var formulario = document.formulario;
	formulario.buscar.value='2';

	
	if(formulario.preid.value=="") {
		alert("Selecione o Pregão.");
		return false;
	}

	if(parseInt(formulario.inferioritem.value) < 0 || parseInt(formulario.superioritem.value) < 0){
		alert("As porcentagens de preenchimento dos Itens não podem ser negativas");
		return false;
	}

	if(parseInt(formulario.inferiorHU.value) < 0 || parseInt(formulario.superiorHU.value) < 0){
		alert("As porcentagens de participação dos HU's não podem ser negativas");
		return false;
	}

	if(formulario.inferioritem.value != "" && formulario.superioritem.value != "" && parseInt(formulario.inferioritem.value) > parseInt(formulario.superioritem.value)){
		alert("A porcentagem de preenchimento do Item superior deve ser maior que a inferior");
		return false;
	}

	if(formulario.inferiorHU.value != "" && formulario.superiorHU.value != "" && parseInt(formulario.inferiorHU.value) > parseInt(formulario.superiorHU.value)){
		alert("A porcentagem de participação do HU superior deve ser maior que a inferior");
		return false;
	}
	
	formulario.target = 'relatoriopregaorehuf';
	var janela = window.open( '', 'relatoriopregaorehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();	
}

function filtraPregao()
{
	ano = $("[name='ano']").val();
	$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=filtraPregao&classe=Pregao&ano="+ano,
		   success: function(msg){
			   console.log(msg);
		   		$('#td_pregao').html( msg );
		   }
		 });

}
$(document).ready(function(){
    $('#ano').attr('title','Permite listar os pregões de acordo com o ano informado. Caso não seja informado serão listados todos os pregões.');
    $('#preid_dsc').attr('title','Obrigatório informar o pregão.');
    $('#inferioritem').attr('title','Permite destacar, na cor amarela, os valores iguais ou inferiores ao informado.');
    $('#superioritem').attr('title','Permite destacar, na cor vermelha, os valores superiores ao informado.');
    $('#inferiorHU').attr('title','Permite destacar, na cor laranja, os valores iguais ou inferiores ao informado.');
    $('#superiorHU').attr('title','Permite destacar, na cor verde, os valores superiores ao informado.');
    
});
</script>
<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td>Filtros</td></tr>
        <tr>
        	<td class="SubTituloDireita" valign="top">Ano:</td>
        	<td>
        		<?php
        			$anos = array();
        			$j = 0;
        			for($i = 2009; $i <= date("Y"); $i++){
						$anos[$j] = array("codigo" => $i,"descricao" => $i);
						$j++;
					}
					$db->monta_combo("ano", $anos, 'S', "Selecione...", "filtraPregao()", '', '', '', 'N', 'ano');
        		?>	
        	</td>
        </tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Pregão :</td>
			<td id="td_pregao">
			<?
				$sql = "SELECT preid as codigo, preobjeto||' ('||COALESCE(precodigo,'-')||')' as descricao FROM rehuf.pregao WHERE prestatus='A'";
				//combo_popup( "preid", $sql, "Pregões", "292x650", 0, array(), "", "S", false, false, 5, 400 );
				campo_popup('preid',$sql,'Selecione..','','400x400',60);
			?>
			<img alt="obrigatorio" src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">% Preenchimento Item:</td>
			<td>
				&nbsp;&nbsp;Inferior: <?php echo campo_texto('inferioritem', 'N', "S", '', 5, 5, '', '', 'left', '', 0, 'id="inferioritem"', ''); ?> 
							Superior: <?php echo campo_texto('superioritem', 'N', "S", '', 5, 5, '', '', 'left', '', 0, 'id="superioritem"', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">% Participação HU:</td>
			<td>
				&nbsp;&nbsp;Inferior: <?php echo campo_texto('inferiorHU', 'N', "S", '', 5, 5, '', '', 'left', '', 0, 'id="inferiorHU"', ''); ?> 
							Superior: <?php echo campo_texto('superiorHU', 'N', "S", '', 5, 5, '', '', 'left', '', 0, 'id="superiorHU"', ''); ?>
			</td>
		</tr>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;">
			<input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/>
			<input type="button" name="filtrarXLS" value="Visualizar XLS" onclick="exibirRelatorioXLS();"/>										
		</td>
	</tr>
</table>