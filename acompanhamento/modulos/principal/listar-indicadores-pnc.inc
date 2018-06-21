<?php
    
    # Busca a lista de indicadores do ano selecionado
    $acompanhamento = new Acompanhamento_Model_Acompanhamento();
    $listaIndicadorPnc = $acompanhamento->getIndicadoresPnc((object) array(
        'suoid' => $suoid,
        'mpnid' => $mpnid,
        'exercicio' => $exercicio));
    
    # Busca informações da Unidade e Subunidade pra exibir informações extras pra o usuário
    $subunidade = new Public_Model_SubUnidadeOrcamentaria($suoid);
    if($subunidade){
        $unidadeOrcamentaria = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
    }
?>

<!--Informações de origem da tela que não precisam estar no formulário de alteração-->
<input id="lista_janid" type="hidden" value="<?= $janid; ?>">
<input id="lista_suoid" type="hidden" value="<?= $suoid; ?>">
<input id="lista_mpnid" type="hidden" value="<?= $mpnid; ?>">
<input id="lista_exercicio" type="hidden" value="<?= $exercicio; ?>">

<table class="table table-bordered">
    <thead>
        <tr>
            <th>DESCRIÇÃO DO INDICADOR</th>
            <?php for($ano = 2010; $ano <= ((int)$exercicio); $ano++): ?>
                <th><?php echo $ano; ?></th>
            <?php endfor; ?>
        </tr>
    </thead>
    <?php foreach($listaIndicadorPnc as $indicador): ?>
        <tbody>
            <tr>
                <td><?php echo $indicador['ipncod']. ' - '. $indicador['ipndsc']; ?></td>
                <?php
                    for($ano = 2010; $ano <= ((int)$_SESSION['exercicio']); $ano++):
                        $acompanhamentoIndicadorPnc = (new Acompanhamento_Model_Acompanhamento())->buscarIndicadorPnc((object) array(
                            'janid' => $janid,
                            'unocod' => $unidadeOrcamentaria->unocod,
                            'suocod' => $subunidade->suocod,
                            'ipncod' => $indicador['ipncod'],
                            'prsano' => $ano
                        ));
                ?>
                    <td>
                        <?php if($acompanhamentoIndicadorPnc->acoquantidade): ?>
                            <a href="#" class="editar_indicador link" data-acoid="<?php echo $acompanhamentoIndicadorPnc->acoid; ?>" title="Editar Indicador">
                                <?= $acompanhamentoIndicadorPnc->acoquantidade; ?>
                            </a>
                        <?php else: ?>
                            <span class="editar_indicador fa fa-pencil link" data-acoid="" data-janid="<?php echo $janid; ?>" data-unocod="<?php echo $unidadeOrcamentaria->unocod; ?>" data-suocod="<?php echo $subunidade->suocod; ?>" data-ipncod="<?php echo trim($indicador['ipncod']); ?>" data-prsano="<?php echo $ano; ?>" title="Editar Indicador"></span>
                        <?php endif; ?>
                    </td>
                <?php endfor; ?>
            </tr>
        </tbody>
    <?php endforeach; ?>
</table>
