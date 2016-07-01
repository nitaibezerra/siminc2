	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
        <?php 
        
		$param					= array();
		$param['not(obridpai)'] = true;
        $obras   				= new Obras();
        $arObrid 				= $obras->pegaIdObraPorEmpid( $empid, $param );

        foreach ( $arObrid as $obrid ):
        	$obra = new Obras( $obrid );
        	
			$supervisao 		= new Supervisao();
			$supid				= $supervisao->pegaSupidByObraAndSfndeid( $obrid, $sfndeid );
            // ver($supid,d);
			
        	$param				= array();
			$param['empid'] 	= $empid;
			$param['is(sfndeid)'] = true;
			$ultimoSupid 		= $supervisao->pegaUltSupidByObra( $obrid, $param );
			
			unset($total);
        ?>
        <tr id="tr9" >
            <td colspan="3">
                <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                    <thead>
                        <tr style="background-color: #CDCDCD;">
                            <td colspan="15" valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            	<input type="hidden" name="obrid[]" id="obrid_<?php echo $obrid ?>" value="<?php echo $obrid ?>">
                            	<input type="" name="supid[<?php echo $obrid ?>]" id="supid_<?php echo $obrid ?>" value="<?php echo $supid ?>">
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
					$dadoEtapa = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid));
                    // ver($dadoEtapa,d);
					$habil = (count($dadoEtapa) ? 'N' : 'S');
                    
					foreach ($dadoEtapa as $etapa){
						
                        $total['vlrProjeto'] = $etapa['ocrvalorexecucao'];

                        $total['vlrObra']  += $etapa['icovlritem'];
                        $total['percObra']  = ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100;

                        $total['vlrExecSobreObra']  += $etapa['spivlrfinanceiroinfsupervisor'];
                        $total['percExecSobreObra']  = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;

                        $total['vlrExecSobreObraUlt']  += $etapa['spivlrfinanceiroanterior'];
                        $total['percExecSobreObraUlt']  = ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100;
//                        $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];
						
                        
						$etapa['icodtinicioitem'] = formata_data( $etapa['icodtinicioitem'] );
					    $etapa['icodterminoitem'] = formata_data( $etapa['icodterminoitem'] );
					
					    $etapa['spipercsobreobrainfsupervisor'] = ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100;
					    
					    // Busca Filhos da Etapa
					    $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
                        $habil = (count($dadoDetalhe) ? 'N' : 'S');
?>
                            <tbody>
                                <tr bgcolor="#FDF8E7" id="<?="tr_item_etapa_" . $obrid . "_" . $etapa['icoid']?>">
                                    <td align="left">
                                    <input type="hidden" name="icoid[<?php echo $obrid?>][]" value="<?=$etapa['icoid']?>">
                                    <input type="hidden" name="ico_spiid[<?php echo $obrid?>][]" value="<?=$etapa['spiid'] ?>">
                                    &nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$etapa['itcdesc'] ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    <?=number_format($etapa['icovlritem'], 2, ',', '.') ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    <?=number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    -
                                    </td>
                                    <td align="center">
                                    -
                                    </td>
                                    <td align="center">
                                    <?=$etapa['icodtinicioitem']  ?>
                                    </td>
                                    <td align="center">
                                    <?=$etapa['icodterminoitem']  ?>
                                    </td>
                                    <td align="center">
                                    <input type="hidden" name="ico_spivlrfinanceiroanterior[]" value="<?=number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                    <?php echo campo_texto( 'ico_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                                    </td>
                                    <td align="center">
                                    <?php echo campo_texto( 'ico_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                    </td>
                                    <td align="center">
                                    <?php echo campo_texto( 'ico_spivlrinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, '. $obrid .' )', number_format($etapa['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                    </td>
                                    <td align="center">
                                    <?php echo campo_texto( 'ico_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, '. $obrid .', \'numerico\' )', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    <input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?=number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                    <span>
                                    <?=number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
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
// ver($dadoDetalhe,d);
                            foreach ($dadoDetalhe as $detalhe){
                                $ditid = $detalhe['ditid'];
                                $detalhe['ditdatainicio']  = formata_data( $detalhe['ditdatainicio'] );
                                $detalhe['ditdatatermino'] = formata_data( $detalhe['ditdatatermino'] );
                                $detalhe['spipercsobreobrainfsupervisor'] = ($detalhe['spivlrfinanceiroinfsupervisor'] / $detalhe['ocrvalorexecucao']) * 100;

                                //adicionando os filhos
                                $dadoDetalheFilho = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai ={$ditid}"));
                                $habil = (count($dadoDetalheFilho) ? 'N' : 'S');
?>
                                <tbody>
                                    <tr bgcolor="#DBF6D5" id="<?="tr_item_detalhamento_" . $obrid . "_" . $detalhe['icoid'] . "_" . $detalhe['ditid']?>">
                                        <td align="left">
                                        <input type="hidden" name="ditid[<?php echo $obrid?>][]" value="<?=$detalhe['ditid']?>">
                                        <input type="hidden" name="dit_spiid[<?php echo $obrid?>][]" value="<?=$detalhe['spiid'] ?>">
                                        &nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$detalhe['ditdsc'] ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=number_format($detalhe['ditvalor'], 2, ',', '.') ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=number_format($detalhe['ditpercsobreobra'], 2, ',', '.') ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=($detalhe['ditmetafisica'] ? number_format($detalhe['ditmetafisica'], 2, ',', '.') : '-') ?>
                                        </td>
                                        <td align="center">
                                        <?=($detalhe['umcdsc'] ? $detalhe['umcdsc'] : '-') ?>
                                        </td>
                                        <td align="center">
                                        <?=$detalhe['ditdatainicio']  ?>
                                        </td>
                                        <td align="center">
                                        <?=$detalhe['ditdatatermino']  ?>
                                        </td>
                                        <td align="center">
                                        <input type="hidden" name="dit_spivlrfinanceiroanterior[]" value="<?=number_format($detalhe['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                        <?php echo campo_texto( 'dit_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemexecanterior'], 2, ',', '.')); ?>
                                        </td>
                                        <td align="center">
                                        <?php echo campo_texto( 'dit_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                        </td>
                                        <td align="center">
                                        <?php echo campo_texto( 'dit_spivlrinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, '. $obrid .', '. $detalhe['icoid'] .' )', number_format($detalhe['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                        </td>
                                        <td align="center">
                                        <?php echo campo_texto( 'dit_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $habil, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, '. $obrid .', '. $detalhe['icoid'] .', \'numerico\' )', number_format($detalhe['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <input type="hidden" name="dit_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?=number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                        <span>
                                        <?=number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                        </span>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=($detalhe['umcdsc'] ?
                                                        campo_texto( 'dit_spivlrmetafisicasupervisor[' . $detalhe['ditid'] . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalhe['ditmetafisica'] . ' )', number_format($detalhe['spivlrmetafisicasupervisor'], 2, ',', '.'))
                                                               :
                                                        '&nbsp;') ?>
                                        </td>

                                    <td align="right" style="color:#336EFF">
	                                	<input type="hidden" value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dit_spidiferencavlr[]">
									<span> <?php echo number_format($detalhe['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
                                </td>
                                <td align="right" style="color:#336EFF">
                                	<input type="hidden" value="<?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dit_spidiferencaporcentagem[]">
									<span> <?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
	                                </td>
                                    </tr>
                                </tbody>
<?php
                                foreach ($dadoDetalheFilho as $detalheFilho){
                                    $detalheFilho['ditdatainicio']  = formata_data( $detalheFilho['ditdatainicio'] );
                                    $detalheFilho['ditdatatermino'] = formata_data( $detalheFilho['ditdatatermino'] );

                                    $detalheFilho['spipercsobreobrainfsupervisor'] = ($detalheFilho['spivlrfinanceiroinfsupervisor'] / $detalheFilho['ocrvalorexecucao']) * 100;
?>
                                    <tbody>
                                        <tr bgcolor="#D7EAF2" id="<?="tr_item_detalhamento_filho_" . $obrid . "_" . $detalheFilho['icoid'] . "_" . $detalheFilho['ditidpai'] . "_" . $detalheFilho['ditid']?>">
                                            <td align="left">
                                            <input type="hidden" name="ditid_filho[<?php echo $obrid?>][]" value="<?=$detalheFilho['ditid']?>">
                                            <input type="hidden" name="dtf_spiid[<?php echo $obrid?>][]" value="<?=$detalheFilho['spiid'] ?>">
                                            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$detalheFilho['ditdsc'] ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=number_format($detalheFilho['ditvalor'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=number_format($detalheFilho['ditpercsobreobra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=($detalheFilho['ditmetafisica'] ? number_format($detalheFilho['ditmetafisica'], 2, ',', '.') : '-') ?>
                                            </td>
                                            <td align="center">
                                            <?=($detalheFilho['umcdsc'] ? $detalheFilho['umcdsc'] : '-') ?>
                                            </td>
                                            <td align="center">
                                            <?=$detalheFilho['ditdatainicio']  ?>
                                            </td>
                                            <td align="center">
                                            <?=$detalheFilho['ditdatatermino']  ?>
                                            </td>
                                            <td align="center">
                                            <input type="hidden" name="dtf_spivlrfinanceiroanterior[]" value="<?=number_format($detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                            <?php echo campo_texto( 'dtf_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemexecanterior'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="center">
                                            <?php echo campo_texto( 'dtf_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="center">
                                            <?php echo campo_texto( 'dtf_spivlrinfsupervisor[' . $obrid . '][]', 'N', 'S', '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, '. $obrid .', '. $detalheFilho['icoid'] .', '. $detalheFilho['ditidpai'] .' )', number_format($detalheFilho['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                            </td>
                                            <td align="center">
                                            <?php echo campo_texto( 'dtf_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, '. $obrid .', '. $detalheFilho['icoid'] .', '. $detalheFilho['ditidpai'] .', \'numerico\'  )', number_format($detalheFilho['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <input type="hidden" name="dtf_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?=number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                            <span>
                                            <?=number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                            </span>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=($detalheFilho['umcdsc'] ?
                                                            campo_texto( 'dtf_spivlrmetafisicasupervisor[' . $obrid . '][' . $detalheFilho['ditid'] . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalheFilho['ditmetafisica'] . ' )', number_format($detalheFilho['spivlrmetafisicasupervisor'], 2, ',', '.'))
                                                                   :
                                                            '&nbsp;') ?>
                                            </td>

		                                   <td align="right" style="color:#336EFF">
			                                	<input type="hidden" value="<?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencavlr[]">
											<span> <?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
		                                </td>
		                                <td align="right" style="color:#336EFF">
		                                	<input type="hidden" value="<?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencaporcentagem[]">
											<span> <?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
			                                </td>

                                        </tr>
                                    </tbody>
<?php
                                }
                            }
                        }
?>
                    <tbody>
                        <tr id="tr_total_supervisao_<?php echo $obrid ?>" bgcolor="#E3E3E3">
                            <td align="right">
                            <b>Total</b>
                            <input type="hidden" name="vlrProjeto_<?php echo $obrid ?>" id="vlrProjeto_<?php echo $obrid ?>" value="<?=$total['vlrProjeto'] ?>">
                            </td>
                            <td align="right" style="color:#336EFF">
                            <?=number_format($total['vlrObra'], 2, ',', '.') ?>
                            </td>
                            <td align="right" style="color:#336EFF">
                            <?=number_format($total['percObra'], 2, ',', '.') ?>
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
                            <?php // echo campo_texto( 'total_spivlritemexecanterior', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00'); ?>
                            </td>
                            <td align="center">
                            <?php echo campo_texto( 'total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
                            </td>
                            <td align="right">
                            <?php // echo campo_texto( 'total_spivlrinfsupervisor', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00'); ?>
                            </td>
                            <td align="right">
                            &nbsp;
                            </td>
                            <td align="right" style="color:#336EFF">
                            <?=number_format($total['percExecSobreObra'], 2, ',', '.') ?>
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
                        </tr>
                    </tbody>
                </table>
                <br>
            </td>
        </tr>    
        <?php
		endforeach;
        ?>
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