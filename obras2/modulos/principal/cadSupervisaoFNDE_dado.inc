<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
<!--
		<tr>
            <td class="SubTituloDireita" width="20%">OS</td>
            <td>
                <?php
//				$situacaoObra = new Supervisao_Os();
//                $sql   		  = $situacaoObra->listaComboFiltroSupervisaoEmpresa( $empid, $sosid );
//                $db->monta_combo("sosid", $sql, $sosidHabil, "Selecione...", "", '', '', '', 'S', 'sosid');
                ?>
            </td>
            <td rowspan="10" align="right" valign="top" width="1">
            <?php
            	// Barra de estado WORKFLOW
//            	if($docid){
//                	wf_desenhaBarraNavegacao($docid, array('sfndeid' =>  $sfndeid));
//            	}
            ?>
            </td>
        </tr>
-->        
		<tr>
            <td class="SubTituloDireita">Data da Supervisão</td>
            <td>
                <?php
                echo campo_data2( 'sfndedtsupervisao', 'S', $somenteLeitura, '', 'S','','validaDataVistoria();', null, 'validaDataVistoria();');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome do Responsável</td>
            <td>
                <?php
		$vistoriador 	    = new Entidade( $entidvistoriador );
                $entnomevistoriador = $vistoriador->entnome;
                $entidvistoriador   = $vistoriador->getPrimaryKey();
                ?>
                <span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
                <input type="hidden" name="entidvistoriador" id="entidvistoriador" value="<? echo $entidvistoriador; ?>">
                <?php if($somenteLeitura == 'S' && $entnomevistoriador == ''){?>
                <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
                <?php }?>
                <?php print obrigatorio(); ?>
            </td>
        </tr>
        <tr id="tr_cargo_vistoriador" style="display: <?=($entidvistoriador ? '' : 'none' ) ?>">
            <td class="SubTituloDireita">Cargo do Vistoriador</td>
            <td>
				<?=campo_texto('sfndecargorepresentante', 'S', $somenteLeitura, '', 30, 255, '', '', '', '', 0, 'id="sfndecargorepresentante"' ); ?>            
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação da Obra</td>
            <td>
                <?php
				$situacaoObra = new SituacaoObra();
                $sql   		  = $situacaoObra->listaCombo();
                $db->monta_combo("sobid", $sql, $somenteLeitura, "Selecione...", "ctrlSituacaoObra", '', '', '', 'S', 'sobid');
                ?>
            </td>
        </tr>
        <tr id="situacaoObraSubPergunta" style="display: <?php echo ( $sobid == SITUACAO_OBRA_EM_EXECUCAO ? '' : 'none' ) ?>;">
            <td class="SubTituloDireita">Em conformidade com o Projeto Básico aprovado e contrato?</td>
            <td>
            	<input type="radio" name="sfndeacordo" id="sfndeacordo_s" value="s" <?=($sfndeacordo == 's' ? 'checked="checked"' : '') ?>>
            	<label for="sfndeacordo_s">Sim</label>
            	&nbsp;&nbsp;
            	<input type="radio" name="sfndeacordo" id="sfndeacordo_n" value="n" <?=($sfndeacordo == 'n' ? 'checked="checked"' : '') ?>>
            	<label for="sfndeacordo_n">Não</label>
            	<?=obrigatorio(); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Unidade em Funcionamento?</td>
            <td>
            	<input type="radio" name="sfndefuncionamento" id="sfndefuncionamento_s" value="s" <?=($sfndefuncionamento == 's' ? 'checked="checked"' : '') ?>>
            	<label for="sfndefuncionamento_s">Sim</label>
            	&nbsp;&nbsp;
            	<input type="radio" name="sfndefuncionamento" id="sfndefuncionamento_n" value="n" <?=($sfndefuncionamento == 'n' ? 'checked="checked"' : '') ?>>
            	<label for="sfndefuncionamento_n">Não</label>
            	<?=obrigatorio(); ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" height="50">&nbsp;</td>
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