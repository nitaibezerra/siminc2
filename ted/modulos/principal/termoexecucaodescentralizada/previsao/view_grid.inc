<?php
$dados = $prevOrcamentaria->getPrevisao();
//ver($dados);

if (is_array($dados)) {
    foreach ($dados as $k => $row) : ?>

        <?php if ($row['provalor'] < 1) continue; ?>

        <?php
        $notaCredito = '';
        if (!empty($row['codsigefnc'])) {
            $notaCredito = $row['codsigefnc'];
        } else if ($row['codncsiafi']) {
            $notaCredito = $row['codncsiafi'];
        }

        ?>

        <?php $cssStyle = ($row['creditoremanejado'] == 't') ? 'info' : ''; ?>

        <tr id="tr_<?=$row['proid'];?>" class="<?=$cssStyle;?>" data-lote-nc="<?=$row['lote'];?>">

            <td class="text-center">
                <?php if ($prevOrcamentaria->permiteExcluirND($row)) : ?>
                    <span class="glyphicon glyphicon-remove cursor-click remove-nc" data-remove-prev="<?=$row['proid'];?>"></span>
                <?php endif; ?>
                <?php if ($prevOrcamentaria->permiteRemanejamento($row['proid'])) : ?>
                    <span class="glyphicon glyphicon-refresh tranferir-credito" data-target-proid="<?=$row['proid'];?>" aria-hidden="true"></span>
                <?php endif; ?>
                <input type="hidden" class="transferir-celula" name="creditoremanejado[]" id="transfer-<?=$row['proid'];?>" value="<?=$row['creditoremanejado']?>" />
                <input type="hidden" name="notacredito[]" id="nota-credito-<?=$row['proid']?>" value="<?=$notaCredito;?>" />
            </td>
            <td class="text-center">
                <input type="hidden" name="proid[]" value="<?=$row['proid'];?>">
                <?php if ($prevOrcamentaria->permiteCadastroNC($row)) : ?>
                    <input type="checkbox" name="nc_proid[]" class="ckboxChild" id="nc_proid_<?=$row['proid'];?>" value="<?=$row['proid'];?>">
                <?php endif; ?>
            </td>
            <td class="text-center" id="td_anoref_<?=$row['proid'];?>" width="">
                <select  name="proanoreferencia[]" class="proanoreferencia" data-proanoreferencia-value="<?=$row['proanoreferencia'];?>" class="form-control chosen-select" id="proanoreferencia_<?=$row['proid'];?>">
                    <option value="" label="-Selecione-">-Selecione-</option>
                    <option value="2012" label="2012">2012</option>
                    <option value="2013" label="2013">2013</option>
                    <option value="2014" label="2014">2014</option>
                    <option value="2015" label="2015">2015</option>
                    <option value="2016" label="2016">2016</option>
                    <option value="2017" label="2017">2017</option>
                    <option value="2018" label="2018">2018</option>
                    <option value="2019" label="2019">2019</option>
                    <option value="2020" label="2020">2020</option>
                    <option value="2021" label="2021">2021</option>
                    <option value="2022" label="2022">2022</option>
                    <option value="2023" label="2023">2023</option>
                </select>
            </td>
            <td class="text-center" id="td_acao_<?=$row['proid'];?>"></td>
            <td class="text-center container-ptrid" id="td_prg_<?=$row['proid'];?>" width="10%">
                <?php if ((($situacao['esdid'] == EM_ANALISE_OU_PENDENTE) || $row['ptrid'])
                    || ($situacao['esdid'] == EM_DESCENTRALIZACAO && Ted_Utils_Model::possuiPerfil(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA)))): ?>
                <select  name="ptrid[]" class="ptrid" data-proid-value="<?=$row['proid'];?>" data-ptrid-value="<?=$row['ptrid'];?>" class="form-control chosen-select" id="ptrid_<?=$row['proid'];?>">
                </select>
                <?php else : ?>
                    -
                <?php endif; ?>
            </td>
            <td class="text-center container-pliid" id="td_pi_<?=$row['proid'];?>" width="10%">
                <?php if ((($situacao['esdid'] == EM_ANALISE_OU_PENDENTE) || $row['pliid'])
                        || ($situacao['esdid'] == EM_DESCENTRALIZACAO && Ted_Utils_Model::possuiPerfil(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA)))): ?>
                    <select  name="pliid[]" class="pliid" data-ptrid-value="<?=$row['ptrid'];?>" data-pliid-value="<?=$row['pliid'];?>" class="form-control chosen-select" id="pliid_<?=$row['proid'];?>">
                    </select>
                <?php else : ?>
                    -
                <?php endif; ?>
            </td>
            <td class="text-center" id="td_acaodsc_<?=$row['proid'];?>" width="5%" style="font-size: 9px;">
                <?php if ((($situacao['esdid'] == EM_ANALISE_OU_PENDENTE) || $row['acatitulo'])
                        || ($situacao['esdid'] == EM_DESCENTRALIZACAO && Ted_Utils_Model::possuiPerfil(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA)))): ?>
                <?=$row['acatitulo'];?>
                <?php else : ?>
                    -
                <?php endif; ?>
            </td>
            <td class="text-center container-ndpid" width="10%">
                <select  name="ndpid[]" class="ndpid" data-ndpid-value="<?=$row['ndpid'];?>" class="form-control chosen-select" id="ndpid_<?=$row['proid'];?>">
                </select>
            </td>
            <td class="text-center" width="10%">
                <input  type="text" name="provalor[]" maxlength="17" value="<?=$row['provalor'];?>" id="provalor_<?=$row['proid'];?>" class="form-control text-right" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);">
            </td>
            <td class="text-center" width="">
                <?php if ((($situacao['esdid'] == EM_ANALISE_OU_PENDENTE) || $row['crdmesliberacao'])
                         || ($situacao['esdid'] == EM_DESCENTRALIZACAO && Ted_Utils_Model::possuiPerfil(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA)))): ?>
                    <select  name="crdmesliberacao[]" class="crdmesliberacao" data-crdmesliberacao-value="<?=$row['crdmesliberacao'];?>" class="form-control chosen-select" id="crdmesliberacao_<?=$row['proid'];?>">
                        <option value="" label="-Selecione-">-Selecione-</option>
                        <option value="1" label="Janeiro">Janeiro</option>
                        <option value="2" label="Fevereiro">Fevereiro</option>
                        <option value="3" label="Março">Março</option>
                        <option value="4" label="Abril">Abril</option>
                        <option value="5" label="Maio">Maio</option>
                        <option value="6" label="Junho">Junho</option>
                        <option value="7" label="Julho">Julho</option>
                        <option value="8" label="Agosto">Agosto</option>
                        <option value="9" label="Setembro">Setembro</option>
                        <option value="10" label="Outubro">Outubro</option>
                        <option value="11" label="Novembro">Novembro</option>
                        <option value="12" label="Dezembro">Dezembro</option>
                    </select>
                <?php else : ?>
                    -
                <?php endif; ?>
            </td>
            <td class="text-center" width="">
                <span title="Favor, preencher o novo prazo total de execução do TED, contado da data de início da vigência até o prazo final, incluindo o termo aditivo."></span>
                <select name="crdmesexecucao[]" class="crdmesexecucao" data-crdmesexecucao-value="<?=$row['crdmesexecucao'];?>" class="form-control chosen-select" id="crdmesexecucao_<?=$row['proid'];?>">
                </select>
            </td>

        </tr>

    <? endforeach;
}
