<?php

if( $_REQUEST['req'] == 'resultado' ){
	include 'relatorioEmpenhoDeObras_resultado.inc';
	die();
}

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

monta_titulo( $titulo_modulo, '' );

?>
<div id="temporizador1" style="background: none repeat 0% 0% rgb(255, 255, 255); opacity: 0.65;
	 text-align: center; position: absolute; top: 0px; left: 0px; width: 1440px; 
	 height: 900px; z-index: 1000; display:none;">
	 <span id="spanCarregando" style="position: relative; top: 225px;">
		 <img src="/imagens/carregando.gif">
		 <center>Carregando...</center>
	 </span>
</div>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
	$(document).ready(function(){

		function carregando(){
			if( $('#temporizador1').css('display') == 'none' ){
				$('#temporizador1').show();
			}else{
				$('#temporizador1').hide();
			}
		}
	
		$('.visualizar').click(function(){

			selectAllOptions( document.filtro.colunas );
			
			var tipo = $(this).attr('id');
			
			if ( $('#colunas[value!=""]').length < 1 ){
				alert( 'Favor selecionar ao menos uma agrupador!' );
				return false;
			}

			$('#req').val('resultado');
			
			if( tipo == 'lista' ){
				carregando();
				$('#xls').val(' ');
				$.ajax({
			   		type: "POST",
			   		url: '/par/par.php?modulo=relatorio/relatorioEmpenhoDeObras&acao=A',
			   		data: $('#filtro').serialize(),
			   		async: false,
			   		success: function(msg){
			   			$('#div_resposta').html(msg);
			   			carregando();
			   		}
		 		});
			}
			
			if( tipo == 'listaxls' ){

				$('#xls').val('1');
				$('#filtro').attr('target','RelatorioEmpenho');
				var janela = window.open( 'par.php?modulo=relatorio/relatorioEmpenhoDeObras&acao=A', 'RelatorioEmpenho', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=no' );
				janela.focus();
				$('#filtro').submit();
			}
	
		});
		
	});	
</script>
<form action="" method="post" name="filtro" id="filtro"> 
	<input type="hidden" id="req" name="req" value="" >
	<input type="hidden" id="xls" name="xls" value="" >
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Agrupadores</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'numero_termo' => array(
													'codigo'    => 'numero_termo',
													'descricao' => '01. Nº do termo de compromisso'
						),
						'id_preobra' => array(
													'codigo'    => 'id_preobra',
													'descricao' => '02. ID da Pré-Obra'
						)
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoColunas', null, $origem );
					$agrupador->setDestino( 'colunas', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">N° do Termo de Compomisso:</td>
			<td><? echo campo_texto('numero_termo', "N", "S", "N° do Termo de Compomisso", 12, 8, "", "", '', '', 0, 'id="numero_termo"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Fonte da Pré-Obra:</td>
			<td>
				<input type="radio" name="fonte_preobra" value="PAC" <?=($_POST['fonte_preobra'] == 'PAC' ? 'checked="checked"' : '') ?>/> PAC 
				<input type="radio" name="fonte_preobra" value="PAR" <?=($_POST['fonte_preobra'] == 'PAR' ? 'checked="checked"' : '') ?>/> PAR
				<input type="radio" name="fonte_preobra" value="TDO" <?=($_POST['fonte_preobra'] != 'PAC' && $_POST['fonte_obra'] != 'PAR' ? 'checked="checked"' : '') ?>/> Todos 
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">ID da Pré-Obra:</td>
			<td><? echo campo_texto('preid', "N", "S", "ID da obra", 12, 8, "[#]", "", '', '', 0, 'id="preid"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">ID da Obra(Obras 2.0):</td>
			<td><? echo campo_texto('obrid', "N", "S", "ID da obra", 12, 8, "[#]", "", '', '', 0, 'id="obrid"' ); ?></td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC" width="10%"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" id="lista" value="Visualizar" class="visualizar" style="cursor: pointer;"/>
				<input type="button" id="listaxls" value="Visualizar XLS" class="visualizar" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr id="tr_resposta" >
			<td colspan="2" style="background-color: white;">
				<div id="div_resposta">
				</div>
			</td>
		</tr>
	</table>
</form>