<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>




	/**
	 * Abre a tela de pesquisa de endereço
	 * @name pesquisarEndereco
	 * @return void
	 */
	function pesquisarEndereco(from){
		$('unidadeAtualNova').setValue(from);
		AbrirPopUp('?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo=N&viaEntradaBens=S','buscarEndereco','scrollbars=yes, width=800, height=770');
	}


	/**
	 * Função responsável por setar os dados de endereço e unidade
	 * @name retornaEndereco
	 * @return void
	 */	
	function retornaEndereco(uendid){

		if(typeof(uendid) == 'undefined'){
			uendid = '';
		}
 		
		var atualNova = $('unidadeAtualNova').getValue();
		carregaDadosEnderecoMovimentacaoLocalidade(uendid,atualNova);
	}
	
	
	/**
	 * Conclui a movimentação
	 * @name concluirMovimentacao
	 * @return void
	 */
	function concluirMovimentacao(){
		$('btnConcluir').disable();
		if(validaFormulario()){
			if(confirm('Confirma a conclusão do processo de movimentação?')){
				$('requisicao').setValue('concluir');
				$('formularioCadastro').submit();
			}
			else{
				$('btnConcluir').enable();
			}
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
			$('btnConcluir').enable();
	        return false;
		}
	
		if($('uendidatual').getValue() == '') {
			alert('A "Localidade Atual" é obrigatória!');
			$('btnBuscaEnderecoAtual').focus();
			$('btnConcluir').enable();
	        return false;
		}
	
		if($('uendidnova').getValue() == '') {
			alert('A "Nova Localidade" é obrigatória!');
			$('btnBuscaEnderecoNovo').enable().focus();
			$('btnConcluir').enable();
	        return false;
		}
	
		if($('uendidatual').getValue() == $('uendidnova').getValue()){
			alert('As Localidades Estão Iguais');
			$('btnConcluir').enable();
	        return false;
		}

		if($('qtd_rgps').getValue() <= 0 || $('qtd_rgps').getValue() == ''){
			alert('A movimentação não pôde ser concluída pois não existem bens relacionados ao processo');
			$('btnConcluir').enable();
	        return false;
		}

		if($('bhsjustificativa') && $('bhsjustificativa').getValue() == ''){
            alert('O campo "Justificativa" é obrigatório!');
            $('bhsjustificativa').focus();
            $('btnConcluir').enable();
    		return false;
    	}
			
	
		return true;
	
	}


	/**
	 * Função responsável por cancelar o registro
	 * @name cancelar
	 * @return void
	 */	
	function cancelar(){

		if($('mvbid').getValue() != ''){
			if(confirm('Todos os dados do processo serão perdidos. Deseja realmente cancelar a movimentação?')){
				$('requisicao').setValue('cancelar');
				$('formularioCadastro').submit();
			}
		}
		else{
			window.location='?modulo=principal/bensmovimentacao/pesquisar&acao=A';
		}
		
		
	}


	/**
	 * Função responsável por visualizar a movimentação
	 * @name visualizarMovimentacao
	 * @return void
	 */
	function visualizarMovimentacao(){

		var tmvid = $('tmvid').getValue();
        var mvbid = $('mvbid').getValue();
        var mvbobservacao = $('mvbobservacao').getValue();

        var unidadeantes = $('unidadeatual').getValue();
        if(unidadeantes != ''){
			var enderecoantes = $F('endlogatual')+', '+$F('enadescricaoatual')+', Sala '+$F('easdescricaoatual');
        }
        else{
        	var enderecoantes = '';
        }
        
		var unidadeagora = $('unidadenova').getValue();
		if(unidadeagora != ''){
			var enderecoagora = $F('endlognova')+', '+$F('enadescricaonova')+', Sala '+$F('easdescricaonova');
		}
		else{
        	var enderecoagora = '';
        }

        var altura = screen.availHeight;
        var largura = screen.availWidth;
        var params = 'scrollbars=yes, width='+largura+', height='+altura+', top=0, left=0';
        
        if(mvbid != ''){
        	AbrirPopUp('?modulo=principal/bensmovimentacao/visualizarMovimentacao&acao=A&tmvid='+tmvid+'&mvbid='+mvbid+'&mvbtipo=L&unidadeantes='+unidadeantes+'&enderecoantes='+enderecoantes+'&unidadeagora='+unidadeagora+'&enderecoagora='+enderecoagora+'&mvbobservacao='+mvbobservacao,'visualizaMovimentacao',params);
        	//AbrirPopUp('?modulo=principal/bensmovimentacao/visualizarMovimentacao&acao=A&mvbid='+mvbid,'visualizaMovimentacao',params);
        }
        else{
        	alert('Os dados da movimentação são obrigatórios!');
		}
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Termo de Responsabilidade: </td>
			<td class="campo">
				<?=campo_texto('ternum','N','N','',15,10,'','','left','',0,'id="ternum"', '', $ternum);?>				
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Tipo de Movimenta&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php
				if(empty($tmvid)){
					$tmvid = 1;
				}
				$oTipoMovimentacao->montaComboTipoMovimentacao($tmvid,'S',false);
				?>
            </td>
		</tr>
		
		<tr>
			<td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Localidade Atual</b></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<?php echo campo_texto('unidadeatual','N','N','',100,150,'','','left','',0,'id="unidadeatual"');?>
				<input type='hidden' name='uendidatual' id='uendidatual' value='<?php echo $uendidatual; ?>'>
				<?php if($_GET['edicao'] != 'S'){ ?>
				<input type='button' name='btnBuscaEnderecoAtual' id='btnBuscaEnderecoAtual' value='Buscar Unidade e Endereço' onclick="javascript:pesquisarEndereco('atual');">
				<?php } ?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('endufatual','N','N','',2,2,'','','left','',0,'id="endufatual"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('endcidatual','N','N','',100,100,'','','left','',0,'id="endcidatual"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('endcepatual','N','N','',15,10,'','','left','',0,'id="endcepatual"');?>
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Endere&ccedil;o: </td>
			<td class="campo">
				<?php echo campo_texto('endlogatual','N','N','',100,200,'','','left','',0,'id="endlogatual"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('enadescricaoatual','N','N','',15,15,'','','left','',0,'id="enadescricaoatual"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('easdescricaoatual','N','N','',50,50,'','','left','',0,'id="easdescricaoatual"'); ?>
            </td>
		</tr>

		
		
		
		
		<tr>
			<td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Nova Localidade</b></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<?php echo campo_texto('unidadenova','N','N','',100,150,'','','left','',0,'id="unidadenova"');?>
				<input type='hidden' name='uendidnova' id='uendidnova' value='<?php echo $uendidnova; ?>'>
				<input type='button' name='btnBuscaEnderecoNovo' id='btnBuscaEnderecoNovo' value='Buscar Unidade e Endereço' onclick="javascript:pesquisarEndereco('nova');">
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('endufnova','N','N','',2,2,'','','left','',0,'id="endufnova"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('endcidnova','N','N','',100,100,'','','left','',0,'id="endcidnova"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('endcepnova','N','N','',15,10,'','','left','',0,'id="endcepnova"');?>
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Endere&ccedil;o: </td>
			<td class="campo">
				<?php echo campo_texto('endlognova','N','N','',100,200,'','','left','',0,'id="endlognova"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('enadescricaonova','N','N','',15,15,'','','left','',0,'id="enadescricaonova"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('easdescricaonova','N','N','',50,50,'','','left','',0,'id="easdescricaonova"'); ?>
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
            	<?php echo campo_textarea('bhsjustificativa','S', 'S', '', 54, 6, 5000,'','','','','',''); ?>
            </td>
        </tr>                
        <?php endif; ?>
		

		<tr class="buttons">
        	<td colspan='2' align='center'>
            	
            	<?php //if($mvbstatus != 'A'){ ?>
            	<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='javascript:cancelar();'>
            	<?php //} ?>
            	
            	<input type='button' class="botao" name='btnVisulizar' id='btnVisulizar' value='Visualizar Movimentação' onclick='javascript:visualizarMovimentacao();'>
            	
            	<?php if($mvbstatus != 'A'){ ?>
            	<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Movimentação' onclick='javascript:concluirMovimentacao();'>
            	<?php }else{ ?>
            	<input type='button' class="botao" name='btnConcluir' id='btnConcluir' value='Concluir Edição' onclick='javascript:concluirMovimentacao();'>
            	<?php } ?>
            	
            	<input type='hidden' name='requisicao' id='requisicao' value=''>
            	<input type='hidden' name='mvbid' id='mvbid' value='<?php echo $mvbid; ?>'>
            	<input type='hidden' name='unidadeAtualNova' id='unidadeAtualNova' value=''>
            	<input type='hidden' name='mvbtipo' id='mvbtipo' value='L'>
            	
            	<input type='hidden' name='mvbstatus' id='mvbstatus' value='<?php echo $mvbstatus; ?>'>
            	
            	
            	<input type='hidden' name='qtd_rgps' id='qtd_rgps' value='<?php echo $qtd_rgps; ?>'>
            </td>
        </tr>
	</table>
</form>
<script>
//trecho pois qdo seleciona o endereço de destino, sem concluir a movimentação
//e exclui um rgp da movimentação, qdo volta pra tela perde os dados
//do endereço de destino(endereço novo)
var uendidnova = $('uendidnova').getValue();
if(uendidnova == ''){
	$('unidadeAtualNova').setValue('nova');
	retornaEndereco(<?php echo $_GET['uendidnova']; ?>);
}
</script>
<?php include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/lista.inc'; ?>