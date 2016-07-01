<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
    <?php 
        
        $param                  = array();
        $param['not(obridpai)'] = true;
        $obras                  = new Obras();
        $arObrid                = $obras->pegaIdObraPorEmpid( $empid, $param );
        
        foreach ( $arObrid as $obrid ):
            
            $obra               = new Obras( $obrid );
            $supervisao         = new Supervisao();
            $supid              = $supervisao->pegaSupidByObraAndSmiid( $obrid, $smiid );
            $param              = array();
            $param['empid'] 	= $empid;
            $param['is(smiid)'] = true;
            $ultimoSupid        = $supervisao->pegaUltSupidByObra( $obrid, $param );

            unset($total);
            
    ?>
    <tr id="tr9" >
        <td colspan="3">
            <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                <thead>
                    <tr style="background-color: #CDCDCD;">
                        <td colspan="15" valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            <input type="hidden" name="obrid[]" id="obrid_<?php echo $obrid ?>" value="<?php echo $obrid ?>">
                            <input type="hidden" name="supid[<?php echo $obrid ?>]" id="supid_<?php echo $obrid ?>" value="<?php echo $supid ?>">
                            <b><?php echo "($obrid) {$obra->obrnome}" ?></b>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Descrição</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre o Objeto</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Unidade de Medida</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
                        <td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Última Supervisão</b></td>
                        <td colspan="4" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
                        <td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Diferença para a Supervisão anterior</b></td>
                    </tr>
                    <tr>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre o Objeto</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado<br></b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre o Objeto após Supervisão</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade Executada<br></b></td>

                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado</b></td>
                        <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Executado<br></b></td>
                    </tr>
                </thead>

<?php
    $supervisaoItem = new SupervisaoItem();

    $total['vlrProjeto']           = 0;
    $total['vlrObra']              = 0;
    $total['vlrExecSobreObra']     = 0;
    $total['vlrExecSobreObraUlt']  = 0;
    $total['percExecSobreObraUlt'] = 0;

    // Busca filhos da Edificação
    $dadoEtapa  = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid));
    $dadoEtapaF = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid), 'F');

    foreach ($dadoEtapa as $etapa){

        $total['vlrProjeto']                    = $etapa['ocrvalorexecucao'];
        $total['vlrObra']                      += $etapa['icovlritem'];
        $total['percObra']                      = ($etapa['ocrvalorexecucao'] > 0) ? ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100 : 0;
        $total['vlrExecSobreObra']             += $etapa['spivlrfinanceiroinfsupervisor'];
        $total['percExecSobreObra']             = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;
        $total['vlrExecSobreObraUlt']          += $etapa['spivlrfinanceiroanterior'];
        $total['percExecSobreObraUlt']          = ($etapa['ocrvalorexecucao'] > 0)?($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100 : 0;
        $etapa['icodtinicioitem']               = formata_data( $etapa['icodtinicioitem'] );
        $etapa['icodterminoitem']               = formata_data( $etapa['icodterminoitem'] );
        $etapa['spipercsobreobrainfsupervisor'] = ($etapa['ocrvalorexecucao'] > 0)?($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100:0;

?>
            <tbody>
                <tr bgcolor="#FDF8E7" id="<?php echo "tr_item_etapa_" . $obrid . "_" . $etapa['icoid']?>">
                    <td align="left">
                        <input type="hidden" name="icoid[<?php echo $obrid?>][]"                    value="<?php echo $etapa['icoid']?>">
                        <input type="hidden" name="ico_spiid[<?php echo $obrid?>][]"                value="<?php echo $etapa['spiid'] ?>">
                        <input type="hidden" name="ico_spitotitemsupervisor[<?php echo $obrid?>][]" value="">
                        &nbsp;
                        <img src="/imagens/seta_filho.gif" border="0">
                        <?php echo $etapa['itcdesc'] ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <?php echo number_format($etapa['icovlritem'], 2, ',', '.') ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <?php echo number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                    -
                    </td>
                    <td align="center">
                    -
                    </td>
                    <td align="center">
                        <?php echo $etapa['icodtinicioitem']  ?>
                    </td>
                    <td align="center">
                        <?php echo $etapa['icodterminoitem']  ?>
                    </td>
                    <td align="center">
                    <input type="hidden" name="ico_spivlrfinanceiroanterior[]" value="<?php echo number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                        <?php echo campo_texto('ico_spivlritemexecanterior['.$obrid.'][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                    </td>
                    <td align="center">
                        <?php echo campo_texto('ico_spivlritemsobreobraexecanterior['.$obrid.'][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                    </td>
                    <td align="center">
                        <?php echo campo_texto('ico_spivlrinfsupervisor['.$obrid.'][]', 'N', $habil, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, '. $obrid .' )', number_format($etapa['spivlrinfsupervisor'], 2, ',', '.')); ?>
                    </td>
                    <td align="center">
                        <?php echo campo_texto('ico_spivlrfinanceiroinfsupervisor['.$obrid.'][]', 'N', $habil, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, '. $obrid .', \'numerico\' )', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?php echo number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                        <span>
                            <?php echo number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                        </span>
                    </td>
                    <td align="right" style="color:#336EFF">
                    &nbsp;
                    </td>
                        <td align="right" style="color:#336EFF">
                            <input type="hidden" value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="ico_spidiferencavlr[]">
                            <span> <?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                    </td>
                    <td align="right" style="color:#336EFF">
                        <input type="hidden" value="<?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?>" name="ico_spidiferencaporcentagem[]">
                        <span> <?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
                    </td>
                </tr>
            </tbody>
<?php
    }
?>
                    <tr id="tr_total_supervisao_<?php echo $obrid ?>" bgcolor="#E3E3E3">
                        <td align="right">
                        <b>Total</b>
                        <input type="hidden" name="vlrProjeto_<?php echo $obrid ?>" id="vlrProjeto_<?php echo $obrid ?>" value="<?php echo $total['vlrProjeto'] ?>">
                        </td>
                        <td align="right" style="color:#336EFF">
                            <?php echo number_format($total['vlrObra'], 2, ',', '.') ?>
                        </td>
                        <td align="right" style="color:#336EFF">
                            <?php echo number_format($total['percObra'], 2, ',', '.') ?>
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                        <td align="center">
                            <?php echo campo_texto( 'total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                        <td align="right" style="color:#336EFF">
                            <?php echo number_format($total['percExecSobreObra'], 2, ',', '.') ?>
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                        <td align="right">
                            <?php echo number_format($total['vlrExecSobreObraUlt'], 2, ',', '.') ?>
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                    </tr>
                    </tbody>
                </table>
                <br>
            </td>
        </tr>
            <tr>
                <td colspan="3" bgcolor="#DCDCDC" style="text-align: center; font-weight: bold">
                    Serviços externos
                </td>
            </tr>
            <tr>
                <td colspan="3">
                    <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                        <thead>
                        <tr>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Descrição</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre a Obra</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Unidade de Medida</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
                            <td colspan="3" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Última Supervisão</b></td>
                            <td colspan="5" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
                        </tr>
                        <tr>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade já Executada</b></td>

                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade já Executada</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado<br></b></td>
                            <td colspan="1" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra após Supervisão</b></td>
                        </tr>


                        </thead>

<?php
                        
                        $total['vlrProjeto']           = 0;
                        $total['vlrObra']              = 0;
                        $total['vlrExecSobreObra']     = 0;
                        $total['vlrExecSobreObraUlt']  = 0;
                        $total['percExecSobreObraUlt'] = 0;
                        foreach ($dadoEtapaF as $etapa){
                            $total['vlrProjeto'] = 0;

                            $total['vlrObra'] += ($etapa['icovlritem'] * $etapa['itcquantidade']);

                    if ($etapa['ocrvalorexecucao'] > 0) {
                        $total['percObra'] = ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100;
                    } else {
                        $total['percObra'] = 0;
                    }
                    $total['vlrExecSobreObra'] += $etapa['spivlrfinanceiroinfsupervisor'];

                    if ($etapa['ocrvalorexecucao'] > 0) {
                        $total['percExecSobreObra'] = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;
                    } else {
                        $total['percExecSobreObra'] = 0;
                    }

                    $total['vlrExecSobreObraUlt'] += $etapa['spivlrfinanceiroanterior'];

                    if ($etapa['ocrvalorexecucao'] > 0) {
                        $total['percExecSobreObraUlt']  = ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100;
                    } else {
                        $total['percExecSobreObraUlt'] = 0;
                    }
                    //$total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];
                    $etapa['icodtinicioitem'] = formata_data($etapa['icodtinicioitem']);
                    $etapa['icodterminoitem'] = formata_data($etapa['icodterminoitem']);

                    if ($etapa['ocrvalorexecucao'] > 0) {
                    //$etapa['spipercsobreobrainfsupervisor'] = ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100;
                    } else {
                        $etapa['spipercsobreobrainfsupervisor'] = 0;
                    }

                    ?>
                    <tbody>
                        <tr bgcolor="#FDF8E7" id="<?php echo "tr_item_etapa_" . $obrid . "_" . $etapa['icoid']?>">
                            <td align="left">
                                <input type="hidden" name="icoid[<?php echo $obrid?>][]" value="<?php echo $etapa['icoid']?>">
                                <input type="hidden" name="ico_spiid[<?php echo $obrid?>][]" value="<?php echo $etapa['spiid'] ?>">
                                &nbsp;&nbsp;&nbsp;
                                <img src="/imagens/seta_filho.gif" border="0">
                                <?php echo $etapa['itcdesc'] ?>
                            </td>
                            <td align="right" style="color:#336EFF">
                                <?php echo number_format($etapa['icovlritem'], 2, ',', '.') ?>
                            </td>
                            <td align="right" style="color:#336EFF">
                                <?php echo number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                            </td>
                            <td align="right" style="color:#336EFF">
                                <?php echo $etapa['itcquantidade']?>
                            </td>
                            <td align="center">
                                <?php echo $etapa['umdeesc']?>
                            </td>
                            <td align="center">
                                <?php echo $etapa['icodtinicioitem']  ?>
                            </td>
                            <td align="center">
                                <?php echo $etapa['icodterminoitem']  ?>
                            </td>
                            <td align="center">
                                <input type="hidden" name="ico_spivlrfinanceiroanterior[]" value="<?php echo number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                <?php echo campo_texto( 'ico_spivlritemexecanterior[]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                            </td>
                            <td align="center">
                                <?php echo campo_texto( 'ico_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                            </td>
                            <td align="center">
                                <?php echo $etapa['spitotitemsupervisor']?>
                            </td>
                            <td align="center" class="ico_spitotitemsupervisor">
                                <?php echo campo_texto( 'ico_spitotitemsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 7, 6, '#######,##', '', 'right', '', 0, '', '', number_format($etapa['spitotitemsupervisor'], 2, ',', '.')); ?>
                            </td>
                            <td align="center">
                                <?php echo campo_texto( 'ico_spivlrinfsupervisor[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                            </td>
                            <td align="center" class="superviao_valor_externos">
                                <?php echo campo_texto( 'ico_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', 'N', '', 11, 15, '', '', 'right', '', 0, '', '', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                            </td>
                            <td align="right" class="totpercentitem" style="color:#336EFF">
                                <input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?php echo number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.') ?>">
                                <span>
                                <?php echo number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.') ?>
                                </span>
                            </td>

                        </tr>
                    </tbody>
                <?php
                }
                ?>
                <tbody>
                    <tr id="tr_total_supervisao_d" bgcolor="#E3E3E3">
                        <td align="right">
                            <b>Total</b>
                            <input type="hidden" name="vlrProjeto" id="vlrProjeto" value="<?php echo  $total['vlrProjeto'] ?>">
                        </td>
                        <td align="right" style="color:#336EFF">
                            <?php echo  number_format($total['vlrObra'], 2, ',', '.') ?>
                        </td>
                        <td align="right" style="color:#336EFF">
                            <?php echo  number_format($total['percObra'], 2, ',', '.') ?>
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            &nbsp;
                        </td>
                        <td align="center">
                            <?php echo campo_texto('total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
                        </td>
                        <td align="right">
                            &nbsp; 
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                        <td align="right">
                            &nbsp;
                        </td>
                        <td align="right" style="color:#336EFF" id="td_exec_obra_d">
                            &nbsp;
                        </td>
                    </tr>
                </tbody>
            </table>
        </td>
    </tr>
    <?php
        endforeach;
    ?>
    </table>

<script type="text/javascript">

    jQuery(document).ready(function () {
        jQuery(".ico_spitotitemsupervisor input:eq(0)").blur();
        jQuery(".ico_spitotitemsupervisor input").blur(function () {

            var obCalc = new Calculo();
            var qtd = jQuery(this).val().replace(',', '.');
            var maxQtd = jQuery(this).parents('tr:eq(0)').find('td:eq(3)').text().trim().replace(',', '.');
            var total = jQuery('#tr_total_supervisao_d td:eq(1)').text().trim();
            var valorItem = jQuery(this).parents('tr:eq(0)').find('td:eq(1)').text().trim();

            if (qtd > maxQtd) {
                qtd = maxQtd;
                jQuery(this).val(maxQtd);
                alert('A quantidade não pode ultrapassar a quantidade definida.');
                jQuery(".ico_spitotitemsupervisor input").keyup();
            }
        });
        
        jQuery(".ico_spitotitemsupervisor input").keyup(function () {

            var obCalc = new Calculo();

            var valor = new String( obCalc.retiraZeroEsquerda( new String(jQuery(this).val()) ) );

            if ( valor.length == 2 ){
                valor = '0' + valor;
            }else if ( valor.length == 1 ){
                valor = '00' + valor;
            }else if ( valor.length == 0 ){
                valor = '000';
            }

            jQuery(this).val(mascaraglobal("[###.]###,##", valor));

            var qtd = jQuery(this).val().replace(',', '.');
            var maxQtd = jQuery(this).parents('tr:eq(0)').find('td:eq(3)').text().trim().replace(',', '.');
            var total = jQuery('#tr_total_supervisao_d td:eq(1)').text().trim();
            var valorItem = jQuery(this).parents('tr:eq(0)').find('td:eq(1)').text().trim();

            // Total de acordo com o total
            valor = obCalc.operacao(valorItem, qtd, '*', 20);
            valor = obCalc.operacao(valor, 100, '*', 20);
            valor = obCalc.operacao(valor, total, '/', 20);
            valor = calculoSupervisao.formatarNumeroMonetario({value: new Number(valor).toFixed(2)});
            valor = valor + '<input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid ?>][]" value="' + valor + '">';

            jQuery(this).parents('td:eq(0)').next().next().next().html(valor);

            // Total do item
            valor = (qtd * 100) / maxQtd;
            jQuery(this).parents('td:eq(0)').next().find('input').val(calculoSupervisao.formatarNumeroMonetario({value: new Number(valor).toFixed(2)}));

            valor = obCalc.operacao(qtd, valorItem, '*', 2);
            jQuery(this).parents('td:eq(0)').next().next().find('input').val(calculoSupervisao.formatarNumeroMonetario({value: new Number(valor).toFixed(2)}));

            valor = 0;
            jQuery('.totpercentitem').each(function (i) {
                valor = obCalc.operacao(valor, jQuery(this).text().trim(), '+', 2);
            });
            jQuery('#td_exec_obra_d').html(valor);

            valorTotal = 0
            jQuery('.superviao_valor_externos input').each(function(){
                valorTotal = obCalc.operacao(valorTotal, jQuery(this).val(), '+', 20);
            });
            jQuery('#tr_total_supervisao_d td:eq(12)').html(calculoSupervisao.formatarNumeroMonetario( {value : new Number( valorTotal ).toFixed(2)} ));

        });
    });
</script>