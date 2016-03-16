	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
        <tr>
            <td class="SubTituloDireita" width="20%">Empresa</td>
            <td>
                <?php
                    $empresami = new EmpresaMi();
                    $sql       = $empresami->listaCombo( $endereco->estuf );
                    $db->monta_combo("emiid", $sql, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'emiid');
                ?>
            </td>
            <td rowspan="10" align="right" valign="top" width="1">
            <?php
            	// Barra de estado WORKFLOW
                if( possui_perfil(PFLCOD_CALL_CENTER)){
                    wf_desenhaBarraNavegacao( $docid, array('smiid' =>  $smiid), array('acoes' => true));
                }else if($docid){
                	wf_desenhaBarraNavegacao($docid, array('smiid' =>  $smiid));
            	}
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data da Supervisão</td>
            <td>
                <?php
                    echo campo_data2( 'smidtsupervisao', 'S', $somenteLeitura, '', 'S','','//validaDataVistoria();' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome do Responsável</td>
            <td>
                <?php
                    $vistoriador        = new Entidade( $entidvistoriador );
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
		<?= campo_texto('smicargorepresentante', 'S', $somenteLeitura, '', 30, 255, '', '', '', '', 0, 'id="smicargorepresentante"' ); ?>            
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação da Obra</td>
            <td>
                <?php
                    $situacaoObra = new SituacaoObra();
                    $sql          = $situacaoObra->listaCombo();
                    $db->monta_combo("sobid", $sql, $somenteLeitura, "Selecione...", "ctrlSituacaoObra", '', '', '', 'S', 'sobid');
                ?>
            </td>
        </tr>
        <tr id="situacaoObraSubPergunta" style="display: <?php echo ( $sobid == SITUACAO_OBRA_EM_EXECUCAO ? '' : 'none' ) ?>;">
            <td class="SubTituloDireita">Em conformidade com o Projeto Básico aprovado e contrato?</td>
            <td>
            	<input type="radio" name="smiacordo" id="smiacordo_s" value="s" <?=($smiacordo == 's' ? 'checked="checked"' : '') ?>>
            	<label for="smiacordo_s">Sim</label>
            	&nbsp;&nbsp;
            	<input type="radio" name="smiacordo" id="smiacordo_n" value="n" <?=($smiacordo == 'n' ? 'checked="checked"' : '') ?>>
            	<label for="smiacordo_n">Não</label>
            	<?=obrigatorio(); ?>
            </td>
        </tr>

        <tr>
            <td colspan="2" height="50">&nbsp;</td>
        </tr>
	</table>