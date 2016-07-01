<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	// Evento de carregamento das telas
	Event.observe(window, 'load', function() {
		verificaTipoMovimentacao();
	});

	/**
	 * Limpa o campo de nome do responsável
	 * @name limparBuscaResponsavel
	 * @return void
	 */
	function limparBuscaResponsavel(){
		$('no_servidor').clear();
	}

	/**
	 * Abre a tela de pesquisa de endereço
	 * @name pesquisarEndereco
	 * @return void
	 */
	function pesquisarEndereco(){
		var uorco_uorg_lotacao_servidor = $('uorco_uorg_lotacao_servidor').getValue();
		AbrirPopUp('?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo=N&viaEntradaBens=S&uorco_uorg_lotacao_servidor='+uorco_uorg_lotacao_servidor,'buscarEndereco','scrollbars=yes, width=800, height=770');
	}

	/**
	 * Função responsável por setar os dados de endereço e unidade
	 * @name retornaEndereco
	 * @return void
	 */
	function retornaEndereco(uendid){
		carregaDadosEndereco(uendid);
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
			montaComboMotivoEstadoConservacao(resposta.ecoid, resposta.mecid, 'S');

			$('rgpnumserie').setValue(resposta.rgpnumserie);
			$('rgp_nu_matricula_siape').setValue(resposta.nu_matricula_siape);
			$('rgp_no_servidor').setValue(resposta.no_servidor);
			$('rgp_uorno').setValue(resposta.uorno);
			$('rgp_enduf').setValue(resposta.enduf);
			$('rgp_endcid').setValue(resposta.endcid);
			$('rgp_endcep').setValue(resposta.endcep);
			$('rgp_endlog').setValue(resposta.endlog);
			$('rgp_enadescricao').setValue(resposta.enadescricao);
			$('rgp_easdescricao').setValue(resposta.easdescricao);

		}else{

			limpaDadosRgp();
			alert('Dados do RGP não encontrado.');

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
	 * @param object request - Requisicao
	 * @return void
	 */
	function createCarregaDadosRGP(request){

		// desabilita botoes para fazer a requisicao
		enableButtons(false);

	}

	/**
	 * Callback de complete na busca de rgp
	 * @name completeCarregaDadosRGP
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

		$('rgpid').clear();
		$('matdsc').clear();
		$('sbmdsc').clear();
		$('ecoid').setValue(0);
		$('mecid').setValue(0);
		$('rgpnumserie').clear();
		$('rgp_nu_matricula_siape').clear();
		$('rgp_no_servidor').clear();
		$('rgp_uorno').clear();
		$('rgp_enduf').clear();
		$('rgp_endcid').clear();
		$('rgp_endcep').clear();
		$('rgp_endlog').clear();
		$('rgp_enadescricao').clear();
		$('rgp_easdescricao').clear();

		$('rgpnum').focus();

	}

	/**
	 * Callback de sucesso na pesquisa do responsavel da devolucao
	 * @name successCarregaResponsavelDevolucao
	 * @param object transport - Retorno
	 * @return void
	 */
	function successCarregaResponsavelDevolucao(transport){

		var resposta = transport.responseText.evalJSON();

		if(resposta.status == 'ok'){

			$('no_servidor_devolucao').setValue(resposta.no_servidor);

		}else{

			$('no_servidor_devolucao').clear();

			if(resposta.nu_matricula_siape != ''){
				alert('A matrícula do Responsável pela Devolução informada não foi encontrada.');
			}

		}

	}

	/**
	 * Callback de falha na pesquisa do novo responsavel
	 * @name failureCarregaResponsavelDevolucao
	 * @param object transport - Retorno
	 * @return void
	 */
	function failureCarregaResponsavelDevolucao(transport){

		$('no_servidor_devolucao').clear();
		alert('A matrícula do Responsável pela Devolução informada não foi encontrada.');

	}

	/**
	 * Callback de sucesso na pesquisa do responsavel da devolucao
	 * @name successCarregaResponsavelRecebimento
	 * @param object transport - Retorno
	 * @return void
	 */
	function successCarregaResponsavelRecebimento(transport){

		var resposta = transport.responseText.evalJSON();

		if(resposta.status == 'ok'){

			$('no_servidor_recebimento').setValue(resposta.no_servidor);

		}else{

			$('no_servidor_recebimento').clear();

			if(resposta.nu_matricula_siape != ''){
				alert('A matrícula do Responsável pelo Recebimento informada não foi encontrada.');
			}

		}

	}

	/**
	 * Callback de falha na pesquisa do novo responsavel
	 * @name failureCarregaResponsavelRecebimento
	 * @param object transport - Retorno
	 * @return void
	 */
	function failureCarregaResponsavelRecebimento(transport){

		$('no_servidor_recebimento').clear();
		alert('A matrícula do Responsável pelo Recebimento informada não foi encontrada.');

	}

	/**
	 * Callback de complete na pesquisa de responsavel
	 * @name completeCarregaResponsavel
	 * @return void
	 */
	function completeCarregaResponsavel(){

		enableButtons();

	}

	/**
	 * Callback de criacao na pesquisa de responsavel
	 * @name createCarregaResponsavel
	 * @return void
	 */
	function createCarregaResponsavel(){

		enableButtons(false);

	}

 	/**
	 * Valida os dados para salvar o rgp na movimentacao
	 * @name salvar
	 * @return void
	 */
	function salvar(){

		$('requisicao').setValue('salvar');
		enableButtons(false);

		if(validaFormulario())
			$('formularioCadastro').submit();
		else
			enableButtons();

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

		if($('no_servidor').getValue() == '') {
			alert('A "matrícula do responsável" é obrigatória!');
			$('nu_matricula_siape').focus();
	        return false;
		}

		if($('uendid').getValue() == '') {
			alert('A "Localidade" é obrigatória!');
			$('btnBuscaEndereco').enable().focus();
	        return false;
		}

		var textoTipoMovimentacao = $('tmvid').options[$('tmvid').selectedIndex].text;

		if(textoTipoMovimentacao.toLowerCase() == 'devolução'){

			if($('mvbresponsaveldevolucaonumatriculasiape').getValue() == '' || $('no_servidor_devolucao').getValue() == '') {
				alert('O campo "Responsável pela Devolução" é obrigatório!');
				$('mvbresponsaveldevolucaonumatriculasiape').focus();
		        return false;
			}

			if($('mvbresponsavelrecebimentonumatriculasiape').getValue() == '' || $('no_servidor_recebimento').getValue() == '') {
				alert('O campo "Responsável pelo Recebimento" é obrigatório!');
				$('mvbresponsavelrecebimentonumatriculasiape').focus();
		        return false;
			}

		}

		if($('rgpnum').getValue() == '' || $('rgpid').getValue() == '') {
			alert('O "Número do RGP" é obrigatório!');
			$('btnBuscaEndereco').enable().focus();
	        return false;
		}

		if($('ecoid').getValue() == '') {
			alert('O campo "Estado de Conservação" é obrigatório!');
			$('ecoid').focus();
	        return false;
		}

		if($('mecid').getValue() == '') {
			alert('O campo "Motivo para o Estado de Conservação" é obrigatório!');
			$('mecid').focus();
	        return false;
		}

		return true;

 	}

    /**
     * Valida os campos obrigatórios da conclusao
     * @name validaConcluir
     * @return bool
     */
    function validaConcluir(){

        if($('tmvid').getValue() == '') {
            alert('O campo "Tipo de Movimentação" é obrigatório!');
            $('tmvid').focus();
            return false;
        }

        if($('no_servidor').getValue() == '') {
            alert('A "matrícula do responsável" é obrigatória!');
            $('nu_matricula_siape').focus();
            return false;
        }

        if($('uendid').getValue() == '') {
            alert('A "Localidade" é obrigatória!');
            $('btnBuscaEndereco').enable().focus();
            return false;
        }

        if(($('requisicao')!='concluir') && ($('bhsjustificativa') && $('bhsjustificativa').getValue() == '')){
            alert('O campo "Justificativa" é obrigatório!');
            $('bhsjustificativa').focus();
            return false;
        }

        var textoTipoMovimentacao = $('tmvid').options[$('tmvid').selectedIndex].text;

        if(textoTipoMovimentacao.toLowerCase() == 'devolução'){

            if($('mvbresponsaveldevolucaonumatriculasiape').getValue() == '' || $('no_servidor_devolucao').getValue() == '') {
                alert('O campo "Responsável pela Devolução" é obrigatório!');
                $('mvbresponsaveldevolucaonumatriculasiape').focus();
                return false;
            }

            if($('mvbresponsavelrecebimentonumatriculasiape').getValue() == '' || $('no_servidor_recebimento').getValue() == '') {
                alert('O campo "Responsável pelo Recebimento" é obrigatório!');
                $('mvbresponsavelrecebimentonumatriculasiape').focus();
                return false;
            }

        }

        return true;

    }

	/**
	 * Função responsável por verificar o tipo de entrada e saída selecionado e bloqueia os campos de portaria
	 * @name verificaTipoEntradaSaida
	 * @return void
	 */
	function verificaTipoMovimentacao(){

		var textoTipoMovimentacao = $('tmvid').options[$('tmvid').selectedIndex].text;

		if(textoTipoMovimentacao.toLowerCase() == 'devolução'){

			$('linhatitulodadosdevolucao').show();
			$('linharesponsaveldevolucao').show();
			$('linharesponsavelrecebimento').show();

			$('mvbresponsaveldevolucaonumatriculasiape').enable().next().setStyle({display:'inline'});
			$('mvbresponsavelrecebimentonumatriculasiape').enable().next().setStyle({display:'inline'});

		}else{

			$('linhatitulodadosdevolucao').hide();
			$('linharesponsaveldevolucao').hide();
			$('linharesponsavelrecebimento').hide();
			$('mvbresponsaveldevolucaonumatriculasiape').clear();
			$('no_servidor_devolucao').clear();
			$('mvbresponsavelrecebimentonumatriculasiape').clear();
			$('no_servidor_recebimento').clear();

			$('mvbresponsaveldevolucaonumatriculasiape').clear().disable().next().setStyle({display:'none'});
			$('mvbresponsavelrecebimentonumatriculasiape').clear().disable().next().setStyle({display:'none'});

		}

	}

	/**
	 * Cancela a Movimentação
	 * @name cancelar
	 * @return void
	 */
	function cancelar(){

		if($('mvbstatus')){
			var mvbstatus = $('mvbstatus').getValue();
		}

		if($('mvbid').getValue() != ''){
			if(mvbstatus == 'I'){
				if(confirm('Todos os dados do processo serão perdidos. Deseja realmente cancelar a movimentação?')){
					$('requisicao').setValue('cancelar');
					$('formularioCadastro').submit();
				}
			}
			else{
				$('requisicao').setValue('cancelar');
				$('formularioCadastro').submit();
			}
		}
		else{
			window.location='?modulo=principal/bensmovimentacao/pesquisar&acao=A';
		}


	}

 	/**
	 * Valida os dados para concluir a movimentacao
	 * @name concluir
	 * @return void
	 */
	function concluir(){

 	    $('requisicao').setValue('concluir');
        enableButtons(false);

        if(validaConcluir()){
            if(confirm('Deseja realmente concluir a movimentação dos Bens Selecionados ?')){
                $('formularioCadastro').submit();
            }
        }else{
            enableButtons();
        }

	}

 	/**
	 * Visualização de bens
	 * @name visualizarMovimentacao
	 * @return void
	 */
 	function visualizarMovimentacao(){

 		var mvbid = $F('mvbid');
        var tmvid = $F('tmvid');
		var mvbobservacao = $F('mvbobservacao');

		var matricula = $F('nu_matricula_siape');
		var resp = $F('no_servidor');

		var unidade = $F('unidade');
		if(unidade != ''){
			var end = $F('endlog')+', '+$F('enadescricao')+', Sala '+$F('easdescricao');
		}
		else{
			var end = '';
		}

		var matriculadevolucao = $F('mvbresponsaveldevolucaonumatriculasiape');
		var respdevolucao = $F('no_servidor_devolucao');
		var matricularecebimento = $F('mvbresponsavelrecebimentonumatriculasiape');
		var resprecebimento = $F('no_servidor_recebimento');
		

        var altura = screen.availHeight;
        var largura = screen.availWidth;
        var params = 'scrollbars=yes, width='+largura+', height='+altura+', top=0, left=0';

        if(mvbid != ''){
        	AbrirPopUp('?modulo=principal/bensmovimentacao/visualizarMovimentacao&acao=A&tmvid='+tmvid+'&mvbid='+mvbid+'&mvbtipo=G&matricula='+matricula+'&resp='+resp+'&unidade='+unidade+'&end='+end+'&mvbobservacao='+mvbobservacao+'&matriculadevolucao='+matriculadevolucao+'&respdevolucao='+respdevolucao+'&matricularecebimento='+matricularecebimento+'&resprecebimento='+resprecebimento,'visualizaMovimentacao',params);
        	//AbrirPopUp('?modulo=principal/bensmovimentacao/visualizarMovimentacao&acao=A&mvbid='+mvbid,'visualizaMovimentacao',params);
        }else{
        	alert('Os dados da movimentação são obrigatórios!');
		}

	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">

		<!-- Dados da Movimentação -->
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Termo de Responsabilidade/Devolu&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('ternum','N','N','',15,10,'','','left','',0,'id="ternum"', '', $ternum);?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Tipo de Movimenta&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php
				$tmvid = empty($tmvid) ? TMV_INTERNA : $tmvid;
				$oTipoMovimentacao->montaComboTipoMovimentacao($tmvid, 'S', true, 'S');
				?>
            </td>
		</tr>

		<tr>
			<td class="SubtituloDireita"> Novo Respons&aacute;vel Pelo(s) RGP(s): </td>
			<td class="campo">
				<?php echo campo_texto('nu_matricula_siape','S','S','Informe a Matr&iacute;cula SIAPE do Respons&aacute;vel',20,20,'','','left','',0,'id="nu_matricula_siape" onkeypress="return somenteNumeros(event);" onchange="javascript:carregaResponsavel(this.value);"');?>
				<?php echo campo_texto('no_servidor','N','N','',50,50,'','','left','',0,'id="no_servidor"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<?php echo campo_texto('unidade','N','N','',100,150,'','','left','',0,'id="unidade"');?>
				<input type='hidden' name='uendid' id='uendid' value='<?php echo $uendid; ?>'>
				<input type='hidden' name='uorco_uorg_lotacao_servidor' id='uorco_uorg_lotacao_servidor' value='<?php echo $uorco_uorg_lotacao_servidor; ?>'>
				<input type='button' name='btnBuscaEndereco' id='btnBuscaEndereco' value='Buscar Unidade e Endereço' onclick="javascript:pesquisarEndereco();">
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
			<td class="SubtituloDireita"> Endere&ccedil;o: </td>
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
		<tr>
			<td class="SubtituloDireita"> Observa&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_textarea('mvbobservacao', 'N', 'S', '', 85, 5, 5000, '', '', '', '', 'Observação');?>
            </td>
		</tr>
		<?php if($mvbstatus == 'A'): ?>
			<tr>
				<td class="SubtituloDireita">Justificativa:</td>
				<td >
					<?php echo campo_textarea('bhsjustificativa','S', 'S', '', 125, 5, 5000); ?>
				</td>
			</tr>
		<?php endif; ?>
		<!-- fim Dados da Movimentação -->

		<!-- Devolução -->
		<tr id='linhatitulodadosdevolucao'>
			<th class="SubtituloCentro" colspan="2"> Dados da Devolu&ccedil;&atilde;o </th>
		</tr>
		<tr id='linharesponsaveldevolucao'>
			<td class="SubtituloDireita"> Respons&aacute;vel Pela Devolu&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('mvbresponsaveldevolucaonumatriculasiape','S','S','Informe a Matr&iacute;cula SIAPE do Respons&aacute;vel',20,20,'','','left','',0,'id="mvbresponsaveldevolucaonumatriculasiape" onkeypress="return somenteNumeros(event);" onchange="carregaResponsavelMovimentacao(this.value, \'successCarregaResponsavelDevolucao\', \'failureCarregaResponsavelDevolucao\', \'createCarregaResponsavel\', \'completeCarregaResponsavel\');"');?>
				<?php echo campo_texto('no_servidor_devolucao','N','N','',50,50,'','','left','',0,'id="no_servidor_devolucao"');?>
			</td>
		</tr>
		<tr id='linharesponsavelrecebimento'>
			<td class="SubtituloDireita"> Respons&aacute;vel Pelo Recebimento: </td>
			<td class="campo">
				<?php echo campo_texto('mvbresponsavelrecebimentonumatriculasiape','S','S','Informe a Matr&iacute;cula SIAPE do Respons&aacute;vel',20,20,'','','left','',0,'id="mvbresponsavelrecebimentonumatriculasiape" onkeypress="return somenteNumeros(event);" onchange="carregaResponsavelMovimentacao(this.value, \'successCarregaResponsavelRecebimento\', \'failureCarregaResponsavelRecebimento\', \'createCarregaResponsavel\', \'completeCarregaResponsavel\');"');?>
				<?php echo campo_texto('no_servidor_recebimento','N','N','',50,50,'','','left','',0,'id="no_servidor_recebimento"');?>
			</td>
		</tr>
		<!-- fim Devolução -->


		<!-- Dados Patrimonio  -->
		<tr>
			<th class="SubtituloCentro" colspan="2"> Dados dos Bens da Movimenta&ccedil;&atilde;o </th>
		</tr>
		<tr>
			<td class="SubtituloDireita"> RGP: </td>
			<td class="campo">
				<input type='hidden' name='rgpid' id='rgpid' value='<?php echo $rgpid; ?>'>
				<?php echo campo_texto('rgpnum', 'S', !empty($bbrid) || $bebsituacao == 'A' ? 'N' : 'S', 'Informe o RGP', 10, 6, '', '', 'left', '', 0, 'id="rgpnum" onchange="carregaDadosRGP(this.value, \'successCarregaDadosRGP\', \'failureCarregaDadosRGP\', \'createCarregaDadosRGP\', \'completeCarregaDadosRGP\');"', '', null, "verificaSomenteNumeros(this,'O Número do Rgp deve ser numérico!');"); ?>
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
				<?php echo $oEstadoConservacao->montaComboEstadoConservacao($ecoid, 'S'); ?>
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
				<?php echo campo_texto('rgpnumserie','N','N','Número de Série',60,50,'','','left','',0,'id="rgpnumserie"' );?>
            </td>
		</tr>
				<tr>
			<td class="SubtituloDireita"> Respons&aacute;vel Pelos RGP's: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_nu_matricula_siape','N','N','Matr&iacute;cula SIAPE do Respons&aacute;vel', 20, 20, '', '', 'left', '', 0, 'id="rgp_nu_matricula_siape"'); ?>
				<?php echo campo_texto('rgp_no_servidor', 'N', 'N', '', 50, 50, '', '', 'left', '', 0, 'id="rgp_no_servidor"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_uorno','N','N','',150,150,'','','left','',0,'id="rgp_uorno"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_enduf','N','N','',2,2,'','','left','',0,'id="rgp_enduf"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_endcid','N','N','',100,100,'','','left','',0,'id="rgp_endcid"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_endcep','N','N','',10,10,'','','left','',0,'id="rgp_endcep"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Endereço: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_endlog','N','N','',100,200,'','','left','',0,'id="rgp_endlog"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_enadescricao','N','N','',15,15,'','','left','',0,'id="rgp_enadescricao"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('rgp_easdescricao','N','N','',50,50,'','','left','',0,'id="rgp_easdescricao"'); ?>
            </td>
		</tr>
		<!-- fim Dados Patrimonio  -->

		<tr class="buttons">
        	<td colspan='2' align='center'>
           		<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='cancelar()' />
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick='salvar()' />
            	<?php if(!empty($mvbid)): ?>
            		<input type='button' class="botao" name='btnVisulizar' id='btnVisulizar' value='Visualizar Movimentação' onclick="visualizarMovimentacao()" />
            	<?php endif; ?>
            	<?php if($mvbstatus != 'A'): ?>
            		<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Movimentação' onclick='concluir();'>
            	<?php else: ?>
            		<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Edição' onclick='concluir();'>
            	<?php endif; ?>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar' />
            	<input type='hidden' name='mvbtipo' id='mvbtipo' value='G' />
            	<input type='hidden' name='mvbid' id='mvbid' value='<?php echo $mvbid; ?>' />
            	<input type='hidden' name='mvbstatus' id='mvbstatus' value='<?php echo $mvbstatus; ?>' />
            </td>
        </tr>

	</table>
</form>

<?php include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/lista.inc'; ?>