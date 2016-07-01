<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Callback de sucesso na pesquisa do novo responsavel
	 * @name successBuscaAtualResponsavel
	 * @param object transport - Retorno
	 * @return void
	 */
	function successBuscaAtualResponsavel(transport){
			
		var resposta = transport.responseText.evalJSON();
		
		if(resposta.status == 'ok'){
			
			$('no_servidor_atual').setValue(resposta.no_servidor);
			
		}else{
			
			$('no_servidor_atual').clear();
			alert('A matrícula do atual responsável informada não foi encontrada.');
				
		}
			
	}

	/**
	 * Callback de falha na pesquisa do novo responsavel
	 * @name failureBuscaAtualResponsavel
	 * @param object transport - Retorno
	 * @return void
	 */
	function failureBuscaAtualResponsavel(transport){

		$('no_servidor_atual').clear();
		alert('A matrícula do atual responsável informada não foi encontrada.');
		
	}

	/**
	 * Callback de complete na pesquisa de responsavel
	 * @name completeBuscaResponsavel
	 * @return void
	 */
	function completeBuscaResponsavel(){

		enableButtons();		
		 
	}

	/**
	 * Callback de criacao na pesquisa de responsavel
	 * @name createBuscaResponsavel
	 * @return void
	 */
	function createBuscaResponsavel(){

		enableButtons(false);
		 
	}

	/**
	 * Callback de sucesso na pesquisa do novo responsavel
	 * @name successBuscaNovoResponsavel
	 * @param object transport - Retorno
	 * @return void
	 */
	function successBuscaNovoResponsavel(transport){
			
		var resposta = transport.responseText.evalJSON();
		
		if(resposta.status == 'ok'){
			
			$('no_servidor_novo').setValue(resposta.no_servidor);
			
		}else{
			
			$('no_servidor_novo').clear();
			alert('A matrícula do novo responsável informada não foi encontrada.');
				
		}
			
	}

	/**
	 * Callback de falha na pesquisa do novo responsavel
	 * @name failureBuscaNovoResponsavel
	 * @param object transport - Retorno
	 * @return void
	 */
	function failureBuscaNovoResponsavel(transport){

		$('no_servidor_novo').clear();
		alert('A matrícula do novo responsável informada não foi encontrada.');
		
	}

 	/**
	 * Inclui os rgps na lista
	 * @name buscarRgps
	 * @return void
	 */
	function buscarRgps(){

		$('requisicao').setValue('buscarRgps');
		enableButtons(false);

		if($('no_servidor_atual').getValue() == '') {
			alert('A "matrícula do atual responsável" é obrigatória!');
			enableButtons();
	        return false;
		}

		$('formularioCadastro').submit();

	}

	 
 	/**
	 * Valida os dados para concluir a movimentacao
	 * @name concluirMovimentacao
	 * @return void
	 */
	function concluirMovimentacao(){

		$('requisicao').setValue('concluir');
		$('btnConcluir').disable();
		if(validaFormulario()){
			$('formularioCadastro').submit();
		}else{
			enableButtons();
		}
		
	}
	 
 	/**
	 * Valida os campos obrigatórios
	 * @name validaFormulario
	 * @return bool
	 */
	function validaFormulario(){
	
		if($('tmvid').getValue() == '') {
			alert('O campo "Tipo de Movimentação" é obrigatório!');
			$('tmvid').focus();
	        return false;
		}
	
		if($('no_servidor_atual').getValue() == '') {
			alert('A "matrícula do atual responsável" é obrigatória!');
			$('nu_matricula_siape_atual').focus();
	        return false;
		}
	
		if($('no_servidor_novo').getValue() == '') {
			alert('A "matrícula do novo responsável" é obrigatória!');
			$('nu_matricula_siape_novo').focus();
	        return false;
		}
	
		if($('nu_matricula_siape_novo').getValue() == $('nu_matricula_siape_atual').getValue()){
			alert('A "matrícula do novo responsável" e a "matrícula do atual responsável" não podem ser iguais!');
	        return false;
		}

		if($('bhsjustificativa') && $('bhsjustificativa').getValue() == ''){
			alert('O campo "Justificativa" é obrigatório!');
			$('bhsjustificativa').focus();
	        return false;
		}
	
		return true;
	
	}

 	/**
	 * Valida os campos obrigatórios
	 * @name validaFormulario
	 * @return bool
	 */
	function cancelar(){

		if($('mvbid').getValue() != ''){
	 		if(confirm('Todos os dados do processo serão perdidos. Deseja realmente cancelar a movimentação ?')){
				$('requisicao').setValue('cancelar');
				$('formularioCadastro').submit();
	 		}
		}
		else{
			window.location='?modulo=principal/bensmovimentacao/pesquisar&acao=A';
		}
	}

 	/**
	 * Visualização de bens
	 * @name visualizarMovimentacao
	 * @return bool
	 */
 	function visualizarMovimentacao(){
        var mvbid = $F('mvbid');
        var tmvid = $F('tmvid');
		var mvbobservacao = $F('mvbobservacao');

		var matriculaantes = $F('nu_matricula_siape_atual');
		var respantes = $F('no_servidor_atual');
		var matriculaagora = $F('nu_matricula_siape_novo');
		var respagora = $F('no_servidor_novo');

        var altura = screen.availHeight;
        var largura = screen.availWidth;
        var params = 'scrollbars=yes, width='+largura+', height='+altura+', top=0, left=0';
        
        if(mvbid != ''){
        	AbrirPopUp('?modulo=principal/bensmovimentacao/visualizarMovimentacao&acao=A&tmvid='+tmvid+'&mvbid='+mvbid+'&mvbtipo=S&matriculaantes='+matriculaantes+'&respantes='+respantes+'&matriculaagora='+matriculaagora+'&respagora='+respagora+'&mvbobservacao='+mvbobservacao,'visualizaMovimentacao',params);
        	//AbrirPopUp('?modulo=principal/bensmovimentacao/visualizarMovimentacao&acao=A&mvbid='+mvbid,'visualizaMovimentacao',params);
        }else{
        	alert('Os dados da movimentação são obrigatórios!');
		}
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Termo de Responsabilidade: </td>
			<td class="campo">
				<?php echo campo_texto('ternum','N','N','',15,10,'','','left','',0,'id="ternum"', '', $ternum); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Tipo de Movimenta&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php 
				$tmvid = empty($tmvid) ? TMV_INTERNA : $tmvid;
				$oTipoMovimentacao->montaComboTipoMovimentacao($tmvid, 'S', false); 
				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Atual Respons&aacute;vel: </td>
			<td class="campo">
				<?php echo campo_texto('nu_matricula_siape_atual','S','S','Informe a Matr&iacute;cula SIAPE do atual Respons&aacute;vel',20,20,'','','left','',0,'id="nu_matricula_siape_atual" onkeypress="return somenteNumeros(event);" onchange="carregaResponsavelMovimentacao(this.value, \'successBuscaAtualResponsavel\', \'failureBuscaAtualResponsavel\', \'createBuscaResponsavel\', \'completeBuscaResponsavel\');"'); ?>
				<?php echo campo_texto('no_servidor_atual', 'N', 'N', '', 50, 50, '', '', 'left', '', 0, 'id="no_servidor_atual"'); ?>
				<input type='button' class="botao" name='btnBuscarRgps' id='btnBuscarRgps' value="Buscar RGPs" onclick='javascript:buscarRgps();' />
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Novo Respons&aacute;vel: </td>
			<td class="campo">
				<?php echo campo_texto('nu_matricula_siape_novo','S','S','Informe a Matr&iacute;cula SIAPE do novo Respons&aacute;vel',20,20,'','','left','',0,'id="nu_matricula_siape_novo" onkeypress="return somenteNumeros(event);" onchange="carregaResponsavelMovimentacao(this.value, \'successBuscaNovoResponsavel\', \'failureBuscaNovoResponsavel\', \'createBuscaResponsavel\', \'completeBuscaResponsavel\');"'); ?>
				<?php echo campo_texto('no_servidor_novo', 'N', 'N', '', 50, 50, '', '', 'left', '', 0, 'id="no_servidor_novo"'); ?>				
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Observa&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_textarea('mvbobservacao', 'N', 'S', '', 85, 5, 5000, '', '', '', '', 'Observação'); ?>
            </td>
		</tr>
		
		<?php if($mvbstatus == 'A'): ?>
		<tr>
			<td class="SubtituloDireita">Justificativa:</td>
			<td >
				<?php echo campo_textarea('bhsjustificativa','S', 'S', '', 54, 6, 5000,'','','','','',''); ?>
			</td>
		</tr>		
		<?php endif; ?>
		
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<?php //if($mvbstatus <> 'A'): ?>
					<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='cancelar();'>
				<?php //endif; ?>
            	<input type='button' class="botao" name='btnVisulizar' id='btnVisulizar' value='Visualizar Movimentação' onclick='visualizarMovimentacao();'>
            	<?php if($mvbstatus != 'A'): ?>
            		<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Movimentação' onclick='concluirMovimentacao();'>
            	<?php else: ?>
            		<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Edição' onclick='concluirMovimentacao();'>
            	<?php endif; ?>
            	<input type='hidden' name='requisicao' id='requisicao' value='concluir'>
            	<input type='hidden' name='mvbid' id='mvbid' value='<?php echo $mvbid; ?>'>
            	<input type='hidden' name='mvbtipo' id='mvbtipo' value='S'>
            </td>
        </tr>						
	</table>
</form>

<?php include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/lista.inc'; ?>