<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Salvar o registro
	 * @name salvar
	 * @param requisicao - Requisição que será executada
	 * @return boolean
	 */
	function salvar(requisicao){

		if($('matid').getValue() == ''){

			alert('Selecione o Material.');
			return false;

		}else if($('bmtdscit').getValue() == ''){

			alert('Preencha a descrição do material.');
			return false;

		}else if($('bmtitmat').getValue() == ''){

			alert('Preencha a quantidade de material.');
			return false;

		}else if($('bmtvlrunitario').getValue() == ''){

			alert('Preencha o valor unitário do material.');
			return false;

		}else if($('bmtitmat').getValue() < 1){

			alert('A quantidade de item deste Material não pode ser zero.');
			return false;

		}else if($('bmtvlrunitario').getValue() == '0,00'){

			alert('O valor Unitário do material não pode ser zero.');
			return false;

		}
		else if($('bmtjust') && $('bmtjust').getValue() == ''){
			alert('Preencha a justificativa.');
			return false;
		}	
		else{

			$('requisicao').setValue(requisicao);
			$('formularioCadastro').submit();

		}
	}

	/**
	 * Somar valor total do item
	 * @name calculaValorTotal
	 * @return void
	 */
	function calculaValorTotal(){

		var obCal = new Calculo();

		var bmtitmat = $('bmtitmat').getValue();
		var valoritem = $('bmtvlrunitario').getValue();

		var total = obCal.operacao(bmtitmat, valoritem, '*');

		//seta o total no campo correspondente
		$('valortotal').setValue(mascaraglobal('[###.]###,##', total));

	}


	/**
	 * Buscar Classe
	 * @name consultarMaterial
	 * @return void
	 */
	function consultarMaterial(){

		AbrirPopUp('?modulo=sistema/tabelasdeapoio/material/pesquisarMaterial&acao=A','buscarMaterial','scrollbars=yes,width=900,height=520');

	}

	/**
	 * Seta os campos de material do form com o material cadastrado
	 * @name retornaDadosMaterial
	 * @param matdsc - Descrição do material
	 * @param matid - Id do material
	 * @param ccbdsc - Descrição da conta contábil
	 * @param ccbid - Id da conta contábil
	 * @return void
	 */
	function retornaDadosMaterial(matdsc,matid,ccbdsc,ccbid){
		$('matdsc').setValue(matdsc);
		$('matid').setValue(matid);
		$('ccbdsc').setValue(ccbdsc);
		$('ccbid').setValue(ccbid);
	}

	/**
	 * Cancelar edição
	 * @name cancelar
	 * @return void
	 */
	function cancelar(benstatus){

		var benid = $('benid').getValue();
		if(benstatus == 'A'){
			location.href='?modulo=principal/bens/adicionarBemMaterialAtivo&acao=A&benid='+benid;
		}
		else if(benstatus == 'I'){
			location.href='?modulo=principal/bens/adicionarBemMaterial&acao=A&benid='+benid;
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
		
		

		<tr>
			<td bgcolor='#e9e9e9' align='center' colspan='2'> <?php echo $subTituloMaterial; ?> </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Material: </td>
			<td class="campo">
				<?php echo campo_texto('matdsc', 'S', 'N', '', 100, 100, '', '', 'left', '', 0, 'id="matdsc"'); ?>
				<input type="hidden" name="matid" id="matid" value="<?php echo $matid; ?>" />
				<?php if($benstatus == 'I'): ?>
					<input type='button' class="botao" name='btnConsultarMaterial' id='btnConsultarMaterial' value='Consultar Material' onclick="consultarMaterial()" />
				<?php endif; ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Conta Cont&aacute;bil: </td>
			<td class="campo">
				<?php echo campo_texto('ccbid', 'S', 'N', '', 12, 12, '', '', 'left', '', 0, 'id="ccbid"'); ?>
				<?php echo campo_texto('ccbdsc', 'N', 'N', '', 100, 100, '', '', 'left', '', 0, 'id="ccbdsc"'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita" valign='top'> Descri&ccedil;&atilde;o do Material: </td>
			<td class="campo">
            	<?php echo campo_textarea('bmtdscit', 'S', 'S', 'Informe a Descrição do Material', 125, 8, 500);?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Qtd. de Unidades deste Material: </td>
			<td class="campo">
				<?php
				if($benstatus == 'A'){
					echo campo_texto('bmtitmat', 'S', 'N', 'Informe a Quantidade de Unidades deste Material', 3, 3, '###', '', 'left', '', 0, 'id="bmtitmat" onchange="calculaValorTotal()"');
				}
				else{
					echo campo_texto('bmtitmat', 'S', 'S', 'Informe a Quantidade de Unidades deste Material', 3, 3, '###', '', 'left', '', 0, 'id="bmtitmat" onchange="calculaValorTotal()"');
				}
				?>

            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Unit&aacute;rio do Material: </td>
			<td class="campo">
				<?php
				if(!$erro){
					echo campo_texto('bmtvlrunitario', 'S', $benstatus == 'A' ? 'N' : 'S', 'Informe o Valor Unit&aacute;rio do Material', 14, 14, '[###.]###,##', '', 'left', '', 0, 'id="bmtvlrunitario"', '', formata_valor($bmtvlrunitario), 'calculaValorTotal()');
				}
				else{
					echo campo_texto('bmtvlrunitario', 'S', $benstatus == 'A' ? 'N' : 'S', 'Informe o Valor Unit&aacute;rio do Material', 14, 14, '[###.]###,##', '', 'left', '', 0, 'id="bmtvlrunitario"', '', $bmtvlrunitario, 'calculaValorTotal()');
				}

				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Total do Material: </td>
			<td class="campo">
				<?php
				if(!$erro){
					echo campo_texto('valortotal', 'S', 'N', '', 18, 18, '', '', 'left', '', 0, 'id="valortotal"', '', formata_valor($valortotal));
				}
				else{
					echo campo_texto('valortotal', 'S', 'N', '', 18, 18, '', '', 'left', '', 0, 'id="valortotal"', '', $valortotal);
				}

				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data de Garantia: </td>
			<td class="campo">
				<?php echo campo_data2('bmtgarantia', 'N', 'S', 'Informe a Data de Garantia', 'N', '', '', empty($bmtgarantia) ? '' : $bmtgarantia); ?> (dd/mm/aaaa)
            </td>
		</tr>

		<?php if($benstatus == 'A'): ?>
			<tr>
				<td class="SubtituloDireita">Justificativa:</td>
				<td class="campo">
					<?php echo campo_textarea('bmtjust','S', 'S', '', 125, 8, 500); ?>
				</td>
			</tr>
		<?php endif; ?>

		<tr class="buttons">
        	<td colspan='2' align='center'>
				<input type='hidden' name='qtdmaterialcadastrada' id='qtdmaterialcadastrada'>
				<input type='hidden' name='valortotalbate' id='valortotalbate'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='benid' id='benid' value="<?php echo $benid; ?>" />
            	<input type='hidden' name='benitdoc' id='benitdoc' value='<?php echo $benitdoc; ?>'>
            	<input type='hidden' name='bmtid' id='bmtid' value='<?php echo $_GET['bmtid']; ?>'>
                

                <input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelar('<?php echo $benstatus; ?>');" />
               	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar('salvar');" />

            </td>
        </tr>
	</table>
</form>

