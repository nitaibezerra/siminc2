<?php if(count($aSiconv)){ ?>

    <div class="col-md-6">
        <h5 class="text-center" style="color: #55b3a3; border-bottom: 1px #55b3a3 solid; padding-bottom: 5px;">Dados da Programa</h5>
        <?php
        global $simec;
        $simec->setPodeEditar(0);

        echo $simec->input('benpprogramanumero', 'Número', $aSiconv['programa'][0]['prgcodigo']);
        echo $simec->input('benprogramatitulo', 'Título', $aSiconv['programa'][0]['prgtitulo']);
        echo $simec->textarea('benprogramaobjeto', 'Objeto', $aSiconv['programa'][0]['benprogramatitulo']);
        ?>
    </div>
    <div class="col-md-6">
        <h5 class="text-center" style="color: #55b3a3; border-bottom: 1px #55b3a3 solid; padding-bottom: 5px;">Dados da Proposta</h5>
        <?php
        $sicsequencial = $aSiconv['siconv']['sicsequencial'] ? $aSiconv['siconv']['sicsequencial'] . '/' . $aSiconv['siconv']['prsano'] : null;
        echo $simec->input('benpropostanumero', 'Número', $sicsequencial, ['class'=>'inteiro']);
        echo '<span title="' . $aSiconv['siconv']['sicjustificativa'] . '">' . $simec->input('benpropostatitulo', 'Título', substr($aSiconv['siconv']['sicjustificativa'], 0, 300) . '...', ['maxlength' => 1000]) . '</span>';
        echo $simec->textarea('benpropostaobjeto', 'Objeto', $aSiconv['siconv']['sicobjeto']);
        echo $simec->select('sitid', 'Situação', $aSiconv['siconv']['sitid'], (new Emendas_Model_SiconvSituacao())->recuperarSqlCombo());
        $simec->setPodeEditar(1);
        ?>
    </div>

    <?php $aTipoParecer = [
        Emendas_Model_SiconvParecer::K_TIPO_PROPOSTA => 'Parecer',
        Emendas_Model_SiconvParecer::K_TIPO_PLANO_TRABALHO => 'Parecer - Plano de Trabalho',
    ];

    foreach($aTipoParecer as $tipo => $titulo){ ?>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5><?php echo $titulo; ?></h5>
                </div>
                <div class="ibox-content">

                    <?php if(isset($aSiconv['parecer'][$tipo]) && is_array($aSiconv['parecer'][$tipo])){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover bedidTables" >
                                <thead>
                                <tr class="text-center">
                                    <th>Data</th>
                                    <th>Parecer</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aSiconv['parecer'][$tipo] as $dados){ ?>
                                    <tr>
                                        <td><?php echo formata_data($dados['spadata'], "%d/%m/%Y %H:%M:%S"); ?></td>
                                        <td class="coluna-parecer" title="<?php echo $dados['spadsc']; ?>"><?php echo substr($dados['spadsc'], 0, 100); ?>...</td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro encontrado</div>';
                    }?>
                </div>
            </div>
        </div>
    <?php } ?>

    <div class="clearfix"></div>

<?php }