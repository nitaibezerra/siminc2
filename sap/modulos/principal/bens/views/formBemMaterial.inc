<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Passa para a tela anterior do cadastro 
	 * @name voltar
	 * @return void
	 */
	function voltar(status){
		if(status == 'A'){
			location.href='?modulo=principal/bens/editarBem&acao=A&benid=<?php echo $benid;?>';
		}
		else if(status == 'I'){
			location.href='?modulo=principal/bens/editarBemAberto&acao=A&benid=<?php echo $benid;?>';
		}

	}

	/**
	 * Cancelar edição
	 * @name cancelar
	 * @return void
	 */
	function cancelar(){

		location.href='?modulo=principal/bens/pesquisarBem&acao=A';

	}

  /**
   * Abre a tela de relatório de visualização de entrada de bens
   * @name visualizarEntrada
   * @return void
   */
   function visualizarEntrada(){
   	   var benid = $('benid').getValue();

		var height = screen.availHeight;
		var width = screen.availWidth;
		var params = 'scrollbars=yes,width='+width+',height='+height+',top=0,left=0';
   	   
       AbrirPopUp('?modulo=principal/bens/visualizarEntrada&acao=A&benid='+benid,'visualizaEntrada',params);
   }


  /**
   * Pesquisa os itens dentro do processo
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
	   var benstatus = $('benstatus').getValue();
	   if(benstatus == 'I'){
		   location.href='?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='+benid;		
	   }
	   else if(benstatus == 'A'){
		   location.href='?modulo=principal/bens/adicionarBemMaterialAtivo&acao=A&benid='+benid;
	   }
	   
   }


   function incluir(){
	   var benid = $('benid').getValue();
	   location.href='?modulo=principal/bens/adicionarBemMaterial2&acao=A&benid='+benid;
   }	

   function concluirBaixaPopup(benid,bebid,termoBaixa,termoEntrada){
		alert('A operação de substituição de bens foi concluída com sucesso. O processo de baixa registrado deu origem ao termo '+termoBaixa+' e o processo de entrada deu origem ao termo '+termoEntrada+'. Ambos os termos serão abertos em novas janelas.');
		window.location='?modulo=principal/bens/pesquisarBem&acao=A&benid='+benid+'&bebid='+bebid;
   }

   function concluir(){
	   $('requisicao').setValue('concluir');
       $('formularioCadastro').submit();		
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
		
		
		
		<!-- Pesquisa de itens no processo -->
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> Pesquisa de itens no processo </td>
		</tr>
		<tr>
			<td class="campo" colspan='2' align='center'>
				<?php echo campo_texto('pesquisadsc','N','S','',100,200,'','','left','',0,'id="pesquisadsc"');?>
            </td>
		</tr>
		<tr>
        	<td class="campo" colspan='2' align='center'>
				<input type='button' class="botao" name='btnPesquisarItem' id='btnPesquisarItem' value='Pesquisar' onclick="javascript: pesquisar();" />
				<input type='button' class="botao" name='btnLimparItem' id='btnLimparItem' value='Limpar' onclick="javascript:limpar();" />
			</td>
		</tr>
		<!-- fim Pesquisa de itens no processo -->
		
		
		
		
		
		<!-- Itens do processo -->
		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> 
				<?php echo $subTituloItens; ?>
				<input type='hidden' name='qtdmaterialcadastrada' id='qtdmaterialcadastrada'>
				<input type='hidden' name='valortotalbate' id='valortotalbate'>
           	 	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='benid' id='benid' value="<?php echo $benid; ?>" />
            	<input type='hidden' name='benstatus' id='benstatus' value="<?php echo $benstatus; ?>" />  
			</td>
		</tr>
	</table>
</form>
		<?php 
		if($qtdItensProcesso >= 1 || $qtdResultadoPesquisa >= 1){ 
			include_once APPSAP . 'modulos/principal/bens/views/partialsViews/listaBemMaterial.inc';
		}
		?>
		
		<!-- fim Itens do processo -->

		
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr class="buttons">
        	<td colspan='2' align='center'>
                <?php if($benstatus == 'A'): ?>
					<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar('A');" />
                	<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='javascript:cancelar();' />
                	<input type='button' class="botao" name='btnVisualizarEntrada' id='btnVisualizarEntrada' value='Visualizar Entrada' onclick="javascript:visualizarEntrada();" />
                	<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Edição' onclick="javascript:concluir();" />

                <?php else: ?>
					<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar('I');" />
					<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Incluir Item' onclick="javascript:incluir();" />
					<input type='button' class="botao" name='btnVisualizarEntrada' id='btnVisualizarEntrada' value='Visualizar Entrada' onclick="javascript:visualizarEntrada();" />
					<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Cadastro'  onclick="javascript:concluir();" />
                <?php endif; ?>
            </td>
        </tr>
	</table>
