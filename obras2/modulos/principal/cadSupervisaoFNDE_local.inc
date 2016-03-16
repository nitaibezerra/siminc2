<?php 
// dbg($sfndeendcorreto,d);
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
	<?php 
	if ( $sfndeendcorreto == 's' || empty($sfndeendcorreto) || empty($sfndefuncionamento) ){
		$campoEndHabil = 'N';
		$campoEndClass = 'disabled';
	}else{
		$campoEndHabil = 'S';
		$campoEndClass = '';
	}
	?>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>No cadastro da obra o endereço está correto?</label></td>
		<td colspan="1">
        	<input type="radio" onclick="verificaEnd('S')" name="sfndeendcorreto" id="sfndeendcorreto_s" value="s" <?=($sfndeendcorreto == 's' || empty($sfndeendcorreto) ? 'checked="checked"' : '') ?>>
        	<label for="sfndeendcorreto_s">Sim</label>
        	&nbsp;&nbsp;
        	<input type="radio" onclick="verificaEnd('N')" name="sfndeendcorreto" id="sfndeendcorreto_n" value="n" <?=($sfndeendcorreto == 'n' ? 'checked="checked"' : '') ?>>
        	<label for="sfndeendcorreto_n">Não</label>
        	<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP</label></td>
		<td colspan="1">
			<input type="hidden" name="emp_endid" id="emp_endid" value="<?=$empreendimento->endid ?>">
			<input type="hidden" name="endid" id="endid" value="<?=$endereco->endid ?>">
			<input type="text" name="endereco[endcep]" 
				   onkeyup="this.value=mascaraglobal('##.###-###', this.value);" 
				   onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo <?=$campoEndClass?>" id="endcep" 
				   value="<?=$endereco->endcep ?>" size="13" maxlength="10" 
				   <?=$campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?>/>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
		</td>
	</tr>
	<tr id="escolha_logradouro_id" style="display:none">
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>
		<td colspan="1">
			<input type="text" name="endlog" class="CampoEstilo" id="endlog" value="<?=$endereco->endlog ?>" size="48" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
            	<label id="lbLogadouro"> Logradouro </label>
            </td>
            <td colspan="1">
           		<input type="text" name="endereco[endlog]" class="CampoEstilo <?=$campoEndClass?>" id="endlogradouro" value="<?=$endereco->endlog ?>" size="65" <?=$campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
      		</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número</label></td>
      		<td colspan="2">
  				<input type="text" name="endereco[endnum]" class="CampoEstilo <?=$campoEndClass?>" id="endnum" value="<?=$endereco->endnum ?>" size="13" maxlength="8" <?=$campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
            </td>
      	</tr>
		<tr>								
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento</label></td>
      		<td colspan="2">
           		<input type="text" name="endereco[endcom]" class="CampoEstilo <?=$campoEndClass?>" id="endcom" value="<?=$endereco->endcom ?>" size="65" maxlength="100" <?=$campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
          	</td>
    	</tr>
		<tr> 
         	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro</label></td>
     		<td colspan="2">
    			<input type="text" name="endereco[endbai]" class="CampoEstilo <?=$campoEndClass?>" id="endbai" value="<?=$endereco->endbai ?>" size="20" <?=$campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
        	</td>
    	</tr>
    	<tr>
          	<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF </label></td>
          	<td colspan="2">
              	<input type="text" name="endereco[mundescricao]"  class="CampoEstilo <?=$campoEndClass?>" id="mundescricao" value="<?=$endereco->getMunDescricao()?>" size="20" <?=$campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
              	<input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="<?=$endereco->muncod ?>" />
             	<input type="text" name="endereco[estuf]" class="CampoEstilo <?=$campoEndClass?>" id="estuf" value="<?=$endereco->estuf ?>" style="width: 5ex; padding-left: 2px" <?=$campoEndHabil == 'N' ? 'readonly="readonly"' : '' ?> />
         	</td>
      	</tr>  	
  	</table> 

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario_supervisao_empresa'), 1500);
                   function bloqueiaForm(idForm){
                      jQuery(document).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#gerarXls').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>