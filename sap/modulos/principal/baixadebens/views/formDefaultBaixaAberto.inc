<?php 
require_once APPRAIZ . "www/includes/webservice/pj.php";
?>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarBensBaixa
	 * @return void
	 */	
	function gravarBensBaixa(){
		$('btnSalvar').disable();
		if(validaFormulario()){
			$('formularioCadastro').submit();
		}
	}
	
	/**
	 * Submete o formulário
	 * @name concluirBensBaixa
	 * @return void
	 */	
	function concluirBensBaixa(){
		var bebid = $('bebid').getValue();
		if(bebid){

			if($('bhsjustificativa') && $('bhsjustificativa').getValue() == ''){
				alert('O campo "Justificativa" é obrigatório!');
				$('bhsjustificativa').focus();
				$('btnSalvar').enable();
		        return false;
			}

			$('requisicao').setValue('concluir');
			$('formularioCadastro').submit();
		}else{
			alert('O desfazimento não pôde ser concluído pois não existem RGPs associados ao processo.')
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
			$('btnSalvar').enable();
			return false;
		}

		if($('tesid').getValue() == '') {
			alert('O campo "Motivo da Baixa" é obrigatório!');
			$('tesid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('diplomabaixa').getValue() == '') {
			alert('O campo "Diploma da Baixa" é obrigatório!');
			$('diplomabaixa').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('bebcnpjrecebedor').getValue() == '') {
			alert('O campo "CNPJ do Recebedor" é obrigatório!');
			$('bebcnpjrecebedor').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('ecoid').getValue() == '') {
			alert('O campo "Estado de Conservação" é obrigatório!');
			$('ecoid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('mecid').getValue() == '') {
			alert('O campo "Motivo para o Estado de Conservação" é obrigatório!');
			$('mecid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		//pega a sigla que o ajax recuperou e setou no campo oculto
		var sigla = $('tesslg').getValue();
		
		if(sigla == 'DOAC'){

			if($('bebnumportariacomissao').getValue() == '') {
				alert('O campo "Número da Portaria" é obrigatório!');
				$('bebnumportariacomissao').focus();
				$('btnSalvar').enable();
		        return false;
			}

			if($('bebdtportariacomissao').getValue() == '') {
				alert('O campo "Data da Portaria" é obrigatório!');
				$('bebdtportariacomissao').focus();
				$('btnSalvar').enable();
		        return false;
			}

		}

		/*if($('bhsjustificativa') && $('bhsjustificativa').getValue() == ''){
			alert('O campo "Justificativa" é obrigatório!');
			$('bhsjustificativa').focus();
			$('btnSalvar').enable();
	        return false;
		}*/
		
		if(!$('bhsjustificativa') && $('rgpnum').getValue() == '') {
			alert('O campo "RGP" é obrigatório!');
			$('rgpnum').focus();
			$('btnSalvar').enable();
	        return false;
		}

		return true;

	}

	/**
	 * Função responsável por verificar o tipo de entrada e saída selecionado e bloqueia os campos de portaria
	 * @name verificaTipoEntradaSaida
	 * @param tipo - Tipo de entrada e saída
	 * @return void
	 */
	function verificaTipoEntradaSaida(tipo){

		//chama o ajax que recupera a sigla do tipo de entrada pelo id
		recuperaSiglaTipoEntrada(tipo);

		//pega a sigla que o ajax recuperou e setou no campo oculto
		var sigla = $('tesslg').getValue();

		if(sigla == 'DOAC'){

			//mostra a parte de Dados da Comissão de Avaliação de Bens Permanentes 	
			$('linhadadoscomissao').show();
			$('linhanumeroportaria').show();
			$('linhadataportaria').show();
			
		}else{
			
			//esconde a parte de Dados da Comissão de Avaliação de Bens Permanentes 
			$('linhadadoscomissao').hide();
			$('linhanumeroportaria').hide();
			$('linhadataportaria').hide();
			$('bebnumportariacomissao').clear();
			$('bebdtportariacomissao').clear();

		}
		
	}


	/**
	 * Redireciona para a edição
	 * @name detalharBensBaixa
	 * @param bbrid - Id da tabela bens_baixa_rgp
	 * @return void
	 */	
	function detalharBensBaixa(bbrid){
		window.location = "?modulo=principal/baixadebens/editarBaixaAberto&acao=A&bbrid="+bbrid;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerBensBaixaRGP
	 * @param bbrid - Id da tabela bens_baixa_rgp
	 * @return void
	 */	
	function removerBensBaixaRGP(bbrid){
		window.location = "?modulo=principal/baixadebens/cancelarBaixaRgp&acao=A&origem=procab&bbrid=" + bbrid;
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
	

	/**
	 * Abre a visualizar baixa de bens
	 * @name visualizarBaixaBens
	 * @return void
	 */
	function visualizarBaixaBens(){
		var bebid = $('bebid').getValue();

		var width = screen.availWidth;
		var height = screen.availHeight;
		var params = 'scrollbars=yes,width='+width+',height='+height+',top=0,left=0';
		
		AbrirPopUp('?modulo=principal/baixadebens/visualizarBaixa&acao=A&bebid='+bebid,'visualizaBaixa',params);
	}

	/**
	 * Função responsável para buscar os dados do cnpj
	 * @name buscaCnpj
	 * @param obj - Campo com o valor a ser pesquisado
	 * @return void
	 */
	function buscaCnpj(obj){
		 $('bebnomerecebedor').setValue('');
		if( $('bebcnpjrecebedor').getValue() != '' ){
			if( !validarCnpj( obj.value  ) ){
				alert( "O CNPJ Informado é Inválido." );
				obj.value = "";
				return false;
			} else {
				var comp     = new dCNPJ();
				comp.buscarDados( obj.value );
				if(comp.dados.no_empresarial_rf == ''){
					alert('O CNPJ não Foi Encontrado.');
				}
				else{
					$('bebnomerecebedor').setValue( comp.dados.no_empresarial_rf );
				}
			}
		}
	}

	/**
	 * Callback de success na busca de rgp
	 * @name successCarregaDadosRGP
	 * @param object transport - Retorno
	 * @return void
	 */
	function successCarregaDadosRGP(transport){

		var resposta = transport.responseText.evalJSON();
		
		if(resposta.status == 'ok'){
			
			$('rgpid').setValue(resposta.rgpid);
			$('matdsc').setValue(resposta.matdsc);
			$('sbmdsc').setValue(resposta.sbmdsc);
			
			// marcando estado de conservacao
			$$('#ecoid option').find(function(ele){
			    if(ele.value == resposta.ecoid)
			        ele.selected = true
			})
			// marcando motivo de estado de conservacao
			montaComboMotivoEstadoConservacao(resposta.ecoid, resposta.mecid);
			
			$('rgpnumserie').setValue(resposta.rgpnumserie);
			$('nu_matricula_siape').setValue(resposta.nu_matricula_siape);
			$('no_servidor').setValue(resposta.no_servidor);
			$('uorno').setValue(resposta.uorno);
			$('enduf').setValue(resposta.enduf);
			$('endcid').setValue(resposta.endcid);
			$('endcep').setValue(resposta.endcep);
			$('endlog').setValue(resposta.endlog);
			$('enadescricao').setValue(resposta.enadescricao);
			$('easdescricao').setValue(resposta.easdescricao);
			
		}else{
			
			limpaDadosRgp();			
			alert('O RGP informado não foi encontrado.');
			
		}

			
	}

	/**
	 * Callback de failure na busca de rgp
	 * @name failureCarregaDadosRGP
	 * @param object transport - Retorno
	 * @return void
	 */
	function failureCarregaDadosRGP(transport){

		// limpa os campos
		limpaDadosRgp();
		// mensagem de erro
		alert('Ocorreu um erro ao buscar os dados do RGP.');
		
	}

	/**
	 * Callback de create na busca de rgp
	 * @name createCarregaDadosRGP
	 * @param object transport - Retorno
	 * @return void
	 */
	function createCarregaDadosRGP(request){
		
		// desabilita botoes para fazer a requisicao
		enableButtons(false);
			
	}

	/**
	 * Callback de complete na busca de rgp
	 * @name successBuscaAtualResponsavel
	 * @param object transport - Retorno
	 * @return void
	 */
	function completeCarregaDadosRGP(transport){
		
		enableButtons();
				
	}

	/**
	 * Limpa campos da busca de Rgp
	 * @name limpaDadosRgp
	 * @return void
	 */
	function limpaDadosRgp(){

		$('rgpnum').clear();
		$('rgpid').clear();
		$('matdsc').clear();
		$('sbmdsc').clear();
		$('ecoid').setValue(0);
		$('mecid').setValue(0);
		$('rgpnumserie').clear();
		$('nu_matricula_siape').clear();
		$('no_servidor').clear();
		$('uorno').clear();
		$('enduf').clear();
		$('endcid').clear();
		$('endcep').clear();
		$('endlog').clear();
		$('enadescricao').clear();
		$('easdescricao').clear();
		
	}
	
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	
		<!-- Dados Baixa -->
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Processo: </td>
			<td class="campo">
				<?php echo campo_texto('bebnumprocesso','S', $bebsituacao == 'A' ? 'N' : 'S','Informe o Número do Processo',30,20,'#####.######.####-##','','left','',0,'id="bebnumprocesso"','',null,'carregarProcesso(this.value);'); ?>
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
				<?php $oTipoEntradaSaida->montaComboTipoEntradaSaida($tesid,'S','S','S', $bebsituacao == 'A' ? 'N' : 'S'); ?>
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
		<tr>
			<td class="SubtituloDireita"> CNPJ do Recebedor: </td>
			<td class="campo">
				<?php echo campo_texto('bebcnpjrecebedor','S','S','Informe o CNPJ do Recebedor',30,18,'##.###.###/####-##','','left','',0,'id="bebcnpjrecebedor"', '', '', 'buscaCnpj(this);'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Recebedor: </td>
			<td class="campo">
				<?php echo campo_texto( 'bebnomerecebedor','N','N','Recebedor',82, 100,'','','left','',0,'id="bebnomerecebedor"' );?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero da Nota SIAFI: </td>
			<td class="campo">
				<?php echo campo_texto('bebnumnotasiafi','N','S','Informe o Número da Nota SIAFI',50,50,'','','left','',0,'id="bebnumnotasiafi"'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Observa&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_textarea('bebobs', 'N', 'S', '', 125, 5, 5000, '', '', '', '', 'Observações');?>
            </td>
		</tr>
		<?php if($bebsituacao == 'A'): ?>
			<tr>
				<td class="SubtituloDireita">Justificativa:</td>
				<td class="campo">
					<?php echo campo_textarea('bhsjustificativa','S', 'S', '', 125, 5, 800); ?>
				</td>
			</tr>		
		<?php endif; ?>
		<!-- fim Dados Baixa -->
		
		<!-- Dados Comissão -->
		<tr id='linhadadoscomissao'>
			<th class="SubtituloCentro" colspan="2"> Dados da Comiss&atilde;o de Avalia&ccedil;&atilde;o de Bens Permanentes </th>
		</tr>
		<tr id='linhanumeroportaria'>
			<td class="SubtituloDireita"> N&uacute;mero da Portaria: </td>
			<td class="campo">
				<?php echo campo_texto('bebnumportariacomissao','S','S','Informe o N&uacute;mero da Portaria', 50, 50, '', '', 'left','',0,'id="bebnumportariacomissao"'); ?>
            </td>
		</tr>
		<tr id='linhadataportaria'>
			<td class="SubtituloDireita"> Data da Portaria: </td>
			<td class="campo">

				<?php echo campo_data2('bebdtportariacomissao', 'S', 'S', 'Informe a Data da Portaria', 'N', '', '', formata_data($bebdtportariacomissao)); ?>
            </td>
		</tr>
		<!-- fim Dados Comissão -->
				
				
		<!-- Dados Patrimonio  -->
		<tr>
			<th class="SubtituloCentro" colspan="2"> Dados do Patrim&ocirc;nio </th>
		</tr>		
		<tr>
			<td class="SubtituloDireita"> RGP: </td>
			<td class="campo">
				<input type='hidden' name='rgpid' id='rgpid' value='<?php echo $rgpid; ?>'>
				<?php echo campo_texto('rgpnum', 'S', 'S', 'Informe o RGP', 10, 6, '', '', 'left', '', 0, 'id="rgpnum" onkeypress="return somenteNumeros(event);" onchange="carregaDadosRGP(this.value, \'successCarregaDadosRGP\', \'failureCarregaDadosRGP\', \'createCarregaDadosRGP\', \'completeCarregaDadosRGP\')"', '', null, "verificaSomenteNumeros(this,'O Número do Rgp deve ser numérico!');"); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('matdsc', 'N', 'N', 'Descrição do Material', 100, 100, '', '', 'left', '', 0, 'id="matdsc"' );?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Situa&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('sbmdsc', 'N', 'N', 'Situa&ccedil;&atilde;o do RGP', 100, 100, '', '', 'left', '', 0, 'id="sbmdsc"' );?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Estado de Conserva&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo $oEstadoConservacao->montaComboEstadoConservacao($ecoid); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Motivo para o Estado de Conserva&ccedil;&atilde;o: </td>
			<td class="campo">
				<div id='motivoestadoconservacao'>
				<?php echo $oMotivoEstadoConservacao->montaComboMotivoEstadoConservacao($ecoid, $mecid); ?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Número de S&eacute;rie: </td>
			<td class="campo">
				<?php echo campo_texto( 'rgpnumserie','N','N','Número de Série',60,50,'','','left','',0,'id="rgpnumserie"' );?>
            </td>
		</tr>
				<tr>
			<td class="SubtituloDireita"> Respons&aacute;vel Pelos RGP's: </td>
			<td class="campo">
				<?php echo campo_texto('nu_matricula_siape','N','N','Matr&iacute;cula SIAPE do Respons&aacute;vel', 20, 20, '', '', 'left', '', 0, 'id="nu_matricula_siape"'); ?>
				<?php echo campo_texto('no_servidor', 'N', 'N', '', 50, 50, '', '', 'left', '', 0, 'id="no_servidor"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<?php echo campo_texto('uorno','N','N','',150,150,'','','left','',0,'id="uorno"');?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('enduf','N','N','',2,2,'','','left','',0,'id="enduf"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('endcid','N','N','',100,100,'','','left','',0,'id="endcid"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('endcep','N','N','',10,10,'','','left','',0,'id="endcep"');?>
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Endereço: </td>
			<td class="campo">
				<?php echo campo_texto('endlog','N','N','',100,200,'','','left','',0,'id="endlog"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('enadescricao','N','N','',15,15,'','','left','',0,'id="enadescricao"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('easdescricao','N','N','',50,50,'','','left','',0,'id="easdescricao"'); ?>
            </td>
		</tr>
		<!-- fim Dados Patrimonio  -->
		
	
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='bebsituacao' id='bebsituacao' value='<?php echo $bebsituacao; ?>'>
            	<input type='hidden' name='tesslg' id='tesslg' value=''>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='bebid' id='bebid' value='<?php echo $bebid; ?>'>
            	<input type='hidden' name='bbrid' id='bbrid' value='<?php echo $bbrid; ?>'>            	
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="gravarBensBaixa();">
            	<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="limpaDadosRgp();">
                <input type='reset' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="cancelar()">
                <?php if($bebid || $bbrid): ?>
                	<input type='button' class="botao" name='btnVisulizar' id='btnVisulizar' value='Visualizar Baixa' onclick="visualizarBaixaBens();">
                <?php endif; ?>
				<?php 
				if(empty($bebid) || $bebsituacao == 'I'){
				?>
				<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Baixa' onclick="concluirBensBaixa();">
				<?php 
				}
				else{
				?>   
				<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Edição' onclick="concluirBensBaixa();">
				<?php 
				}
				?>             
				
            </td>
        </tr>
        
	</table>
</form>

<?php if(!empty($_GET['bbrid']) || !empty($_GET['bebid'])): ?>
	<script type="text/javascript"> verificaTipoEntradaSaida(<?php echo $tesid; ?>); </script>	
<?php endif; ?>

<?php include_once APPSAP . 'modulos/principal/baixadebens/views/partialsViews/listaBaixaRGP.inc'; ?>