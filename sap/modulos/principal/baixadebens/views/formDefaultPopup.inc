<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>


	
	/**
	 * Submete o formulário
	 * @name concluirBensBaixa
	 * @return void
	 */	
	function concluirBensBaixa(){
		if(validaFormulario()){
			$('requisicao').setValue('concluir');
			$('formularioCadastro').submit();
		}
		
	}

	/**
	 * Valida os campos obrigatórios
	 * @name validaFormulario
	 * @return bool
	 */	
	function validaFormulario(){

		if($('bebnumprocesso').getValue() == ''){
			alert('O campo "Número do Processo" é obrigatório!');
			$('bebnumprocesso').focus();
			return false;
		}

		if($('bebdataprocesso').getValue() == ''){
			alert('O campo "Data do Processo" é obrigatório!');
			$('bebdataprocesso').focus();
			return false;
		}

		

		return true;

	}

	


	


	/**
	 * Abre a pesquisa
	 * @name cancelar
	 * @return void
	 */
	function cancelar(){
		var bebid = $('bebid').getValue();
		var bebsituacao = $('bebsituacao').getValue();

		if(bebid && bebsituacao == 'I'){
			window.location = '?modulo=principal/baixadebens/excluir&acao=A&bebid='+bebid;
		}
		else if(bebid && bebsituacao != 'I'){
			window.location = '?modulo=principal/baixadebens/editar&acao=A&bebid='+bebid;
		}
		else{
			window.location = '?modulo=principal/baixadebens/adicionar&acao=A';
		}
			
	}
	

	

	
	
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	
		<!-- Dados Baixa -->
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Processo: </td>
			<td class="campo">
				<?php echo campo_texto('bebnumprocesso','S', 'S','Informe o Número do Processo',30,20,'#####.######.####-##','','left','',0,'id="bebnumprocesso"','',null,'carregarProcesso(this.value);'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Processo: </td>
			<td class="campo">
				<?php echo campo_data2('bebdataprocesso', 'N', 'N', 'Data do Processo', 'N', '', '', $bebdataprocesso); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Motivo da Baixa: </td>
			<td class="campo">
				<?php $oTipoEntradaSaida->montaComboTipoEntradaSaida($_GET['tesid'],'','N','N','N'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Diploma da Baixa: </td>
			<td class="campo">
				<?php echo campo_texto('diplomabaixa','S','N','Informe o diploma da Baixa',15,12,'##.###','','left','',0,'id="diplomabaixa"', '', '99.658'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data da Baixa: </td>
			<td class="campo">
				<?php 
				if(empty($bebdata)){
					$bebdata = date('d/m/Y');
				}
				echo campo_texto('bebdata', 'N', 'N','',12,12,'','','left','',0,'id="bebdata"'); 
				?>
            </td>
		</tr>
		

		
	
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='bebsituacao' id='bebsituacao' value='<?php echo $bebsituacao; ?>'>
            	<input type='hidden' name='tesslg' id='tesslg' value=''>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>

				<input type='hidden' name='benid' id='benid' value='<?php echo $_GET['benid']; ?>'>
				<input type='hidden' name='tesid' id='tesid' value='<?php echo $_GET['tesid']; ?>'>  
                <input type='reset' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="cancelar()">
				<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Baixa' onclick="concluirBensBaixa();">
				            
				
            </td>
        </tr>
        
	</table>
</form>
