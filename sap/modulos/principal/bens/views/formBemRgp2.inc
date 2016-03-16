<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Salvar o registro
	 * @name salvar
	 * @return boolean
	 */	
	function salvar(requisicao){
	
		if($('tesslg')){
			var tesslg = $('tesslg').getValue();
		}

		if($('bmtid').getValue() == ''){

			alert('Selecione um item do processo para ser tombado.');
			return false;
			
		}else if($('rgpnum').getValue() == ''){

			alert('Preencha o número do RGP.');
			return false;
						
		}
		else if(tesslg == 'SUBS' && $('rgpanterior').getValue() == ''){
			alert('Preencha o RGP Anterior.')
			return false;
		}
		else if($('ecoid').getValue() == ''){

			alert('Selecione um Estado de Conservação.');
			return false;

		}else if($('mecid').getValue() == ''){

			alert('Selecione um Motivo para o Estado de Conservação.');
			return false;
			
		}else if($('rgpjust') && $('rgpjust').getValue() == ''){
			alert('Informe a Justificativa');
			return false;
		}else{

			if($('benstatus').getValue() == 'A' && confirm('Confirma as alterações?')){
				$('requisicao').setValue(requisicao);
				$('formularioCadastro').submit();
			}
			else if($('benstatus').getValue() == 'I'){
				$('requisicao').setValue(requisicao);
				$('formularioCadastro').submit();
			}

			
			

		}
		
	}


	/**
	 * Cancelar edição
	 * @name cancelar
	 * @return void
	 */	
	function cancelar(benstatus){

		var benid = $('benid').getValue();
		var bmtid = $('bmtid').getValue();
		if(benstatus == 'A'){
			location.href='?modulo=principal/bens/adicionarBemRgpAtivo&acao=A&benid='+benid+'&bmtid='+bmtid;
		}
		else if(benstatus == 'I'){
			location.href='?modulo=principal/bens/adicionarBemRgp&acao=A&benid='+benid+'&bmtid='+bmtid;
		}
		
	}


	function concluirBaixaPopup(benid,bebid,termoBaixa,termoEntrada){
		alert('A operação de substituição de bens foi concluída com sucesso. O processo de baixa registrado deu origem ao termo '+termoBaixa+' e o processo de entrada deu origem ao termo '+termoEntrada+'. Ambos os termos serão abertos em novas janelas.');
		window.location='?modulo=principal/bens/pesquisarBem&acao=A&benid='+benid+'&bebid='+bebid;
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	
		<!--  Dados da primeira tela -->
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Documento: </td>
			<td class="campo">
				<div id='documento'>
				<?php echo campo_texto('bennumdoc','N','N','',24,24,'','','left','',0,'id="bennumdoc"');?>
				<?php echo campo_texto('dscdoc','N','N','',100,100,'','','left','',0,'id="dscdoc"');?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Documento: </td>
			<td class="campo">
				<?php echo campo_texto('bendtdoc','N','N','',12,12,'','','left','',0,'id="bendtdoc"','',$bendtdoc);?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor do Documento: </td>
			<td class="campo">
				<?php
				if(!$erro){
					echo campo_texto('benvlrdoc', 'N', 'N', '', 17, 17, '', '', 'left', '', 0, 'id="benvlrdoc"', '', formata_valor($benvlrdoc));
				}
				else{
					echo campo_texto('benvlrdoc', 'N', 'N', '', 17, 17, '', '', 'left', '', 0, 'id="benvlrdoc"', '', $benvlrdoc);
				}
				?>
            </td>
		</tr>
		<!--  fim Dados da primeira tela -->
		
		
		<!-- Dados do item -->
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> Dados do item <?php echo $matdsc; ?> </td>
		</tr>
		<tr>
			<td class="SubtituloDireita" valign='top'> Descri&ccedil;&atilde;o do Material: </td>
			<td class="campo">
            	<?php echo campo_textarea('bmtdscit', 'N', 'N', '', 125, 8, 500,'',0,'',false,null,$bmtdscit);?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Qtd. de Itens para Tombamento: </td>
			<td class="campo">
				<?php echo campo_texto('bmtitmat','N','N','',3,3,'','','left','',0,'id="bmtitmat"','',$bmtitmat);?>
            </td>
		</tr>
		<!-- fim Dados do item -->
		
		
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> <?php echo $subTituloMaterial; ?> </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Rgp: </td>
			<td class="campo">
				<?php echo campo_texto('rgpnum', 'S', $benstatus == 'A' ? 'N' : 'S'	, 'Registro Geral do Patrimônio', 6, 6, '######', '', 'left', '', 0, 'id="rgpnum"', '', null, "verificaSomenteNumeros(this,'O Número do Rgp deve ser numérico!');"); ?>
            </td>			
		</tr>
		<tr>
			<td class="SubtituloDireita"> Rgp Anterior: </td>
			<td class="campo">
				<?php 
				if($tesslg == 'SUBS'){
					echo campo_texto('rgpanterior', 'S', 'S', 'Registro Geral do Patrimônio Anterior', 6, 6, '######', '', 'left', '', 0, 'id="rgpanterior"'); 
				}
				else{
					echo campo_texto('rgpanterior', 'N', 'S', 'Registro Geral do Patrimônio Anterior', 6, 6, '######', '', 'left', '', 0, 'id="rgpanterior"'); 
				}
				?>
            </td>			
		</tr>
		<tr>
			<td class="SubtituloDireita"> Estado de Conserva&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php 
				if(empty($ecoid)){
					$ecoid = 1;
				}
				echo $oEstadoConservacao->montaComboEstadoConservacao($ecoid); 
				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Motivo para o Estado de Conserva&ccedil;&atilde;o: </td>
			<td class="campo">
				<div id='motivoestadoconservacao'>
				<?php 
				if(empty($mecid)){
					$mecid = 5;
				}
				echo $oMotivoEstadoConservacao->montaComboMotivoEstadoConservacao($ecoid, $mecid); 
				?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero de S&eacute;rie: </td>
			<td class="campo">
				<?php echo campo_texto('rgpnumserie', 'N', 'S', 'Número de Série', 50, 50, '', '', 'left', '', 0, 'id="rgpnumserie"'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero da Nota SIAFI: </td>
			<td class="campo">
				<?php echo campo_texto('rgpnumnotasiafi', 'N', 'S', 'Número da Nota SIAFI', 50, 50, '', '', 'left', '', 0, 'id="rgpnumnotasiafi"'); ?>
            </td>
		</tr>
				
		<?php if($benstatus == 'A'): ?>
			<tr>
				<td class="SubtituloDireita">Justificativa:</td>
				<td class="campo">
					<?php echo campo_textarea('rgpjust','S', 'S', '', 125, 8, 500); ?>
				</td>
			</tr>		
		<?php endif; ?>
		
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='benid' id='benid' value="<?php echo $benid; ?>" />
            	<input type='hidden' name='bmtid' id='bmtid' value="<?php echo $bmtid; ?>" />
            	<input type='hidden' name='tesslg' id='tesslg' value="<?php echo $tesslg; ?>" />
            	<input type='hidden' name='rgpid' id='rgpid' value="<?php echo $_GET['rgpid']; ?>" />
            	<input type='hidden' name='benstatus' id='benstatus' value="<?php echo $benstatus; ?>" />
                
                <input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelar('<?php echo $benstatus; ?>');" />
               	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar('salvar');" />

            </td>
        </tr>
	</table>
</form>