<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Passa para a tela anterior do cadastro (registro de bem)
	 * @name voltar
	 * @return void
	 */
	function voltar(status){
		if(status == 'I'){
			location.href='?modulo=principal/bens/adicionarBemMaterial&acao=A&benid=<?php echo $benid;?>';
		}
		else if(status == 'A'){
			location.href='?modulo=principal/bens/adicionarBemMaterialAtivo&acao=A&benid=<?php echo $benid;?>';
		}
		
	}

	/**
     * Abre a tela de relatório de visualização de entrada de bens
     * @name visualizarEntrada
     * @return void
     */
    function visualizarEntrada(){
        var benid = $('benid').getValue();

		var width = screen.availWidth;
		var height = screen.availHeight;
		var params = 'scrollbars=yes,width='+width+',height='+height+',top=0,left=0';
        
        AbrirPopUp('?modulo=principal/bens/visualizarEntrada&acao=A&benid='+benid,'visualizaEntrada',params);
    }

	/**
	 * Cancelar edição
	 * @name cancelar
	 * @return void
	 */	
	function cancelar(){

		location.href='?modulo=principal/bens/pesquisarBem&acao=A';
		
	}


	function concluirBaixaPopup(benid,bebid,termoBaixa,termoEntrada){
		alert('A operação de substituição de bens foi concluída com sucesso. O processo de baixa registrado deu origem ao termo '+termoBaixa+' e o processo de entrada deu origem ao termo '+termoEntrada+'. Ambos os termos serão abertos em novas janelas.');
		window.location='?modulo=principal/bens/pesquisarBem&acao=A&benid='+benid+'&bebid='+bebid;
	}


  /**
   * Pesquisa os tombamentos dentro do item
   * @name pesquisar
   * @return void
   */
   function pesquisar(){
       $('requisicao').setValue('pesquisaritem');
       $('formularioCadastro').submit();
   }


   /**
    * Limpa o campo da pesquisa
    * @name limpar
    * @return void
    */	 
    function limpar(){
 	   var benid = $('benid').getValue();
 	   var bmtid = $('bmtid').getValue();
 	   var benstatus = $('benstatus').getValue();
 	   if(benstatus ==  'I'){
 	       location.href='?modulo=principal/bens/adicionarBemRgp&acao=A&benid='+benid+'&bmtid='+bmtid;
 	   }
 	   else if(benstatus == 'A'){
 		  location.href='?modulo=principal/bens/adicionarBemRgpAtivo&acao=A&benid='+benid+'&bmtid='+bmtid;	
 	   }
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
		
		<!-- Pesquisa de tombamento no item -->
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> Pesquisa de Tombamento no Item </td>
		</tr>
		<tr>
			<td class="campo" colspan='2' align='center'>
				<?php echo campo_texto('pesquisadsc','N','S','',100,200,'','','left','',0,'id="pesquisadsc"');?>
            </td>
		</tr>
		<tr>
        	<td class="campo" colspan='2' align='center'>
				<input type='button' class="botao" name='btnPesquisarTombamento' id='btnPesquisarTombamento' value='Pesquisar' onclick="javascript:pesquisar();" />
				<input type='button' class="botao" name='btnLimparTombamento' id='btnLimparTombamento' value='Limpar' onclick="javascript:limpar();" />
			</td>
		</tr>
		<!-- fim Pesquisa de tombamento no item -->
		
		<!-- Tombamento do item -->
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> 
				<?php echo $subTituloItens; ?>
				<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='benid' id='benid' value="<?php echo $benid; ?>" />
            	<input type='hidden' name='bmtid' id='bmtid' value="<?php echo $bmtid; ?>" />
            	<input type='hidden' name='tesslg' id='tesslg' value="<?php echo $tesslg; ?>" />
            	<input type='hidden' name='benstatus' id='benstatus' value="<?php echo $benstatus; ?>" />
			</td>
		</tr>
	</table>
</form>
		<?php 
		if($qtdResultadoPesquisa >= 1){ 
			include_once APPSAP . 'modulos/principal/bens/views/partialsViews/listaBemRgp.inc';
		}
		?>
		<!-- fim Tombamento do item -->
		
				
		
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr class="buttons">
        	<td colspan='2' align='center'>
                <?php if($benstatus == 'A'): ?>
                	<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="voltar('A')" />
                	<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='cancelar()' />
                	<input type='button' class="botao" name='btnVisualizarEntrada' id='btnVisualizarEntrada' value='Visualizar Entrada' onclick="visualizarEntrada()" />
                	<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Tombamento do Item' onclick="voltar('A')" />
                	
                <?php else: ?>
					<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="voltar('I')" />
	               	<input type='button' class="botao" name='btnVisualizarEntrada' id='btnVisualizarEntrada' value='Visualizar Entrada' onclick="visualizarEntrada()" />
	               	<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Tombamento do Item' onclick="voltar('I')" />
					                
                <?php endif; ?>  

            </td>
        </tr>
	</table>
