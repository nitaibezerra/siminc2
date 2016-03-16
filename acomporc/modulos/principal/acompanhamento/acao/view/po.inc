<?php if (!$dadosPO) : ?>
    <section class="alert alert-info text-center col-md-4 col-md-offset-4">Nenhum registro encontrado.</section>
<?php else: ?>
<!-- Acompanhamento Físico dos Planos Orçamentários -->
<section class="row">
    <? foreach ($dadosPO as $po): ?>
    <section class="col-md-6">
        <section class="panel panel-default" style="box-shadow: 0 0 4px #ccc; ">
            <section class="panel-heading" style="line-height: 150%;">
                <strong>Plano Orçamentário:</strong> <?= $po['plocod'] .' - '. $po['plodsc'] ?>
            </section>
            <table class="table">
                <thead>
                    <tr>
                        <th>Produto</th>
                        <th>Unidade de Medida</th>
                    </tr>
                </thead>
                <tbody>
                    <tr >
                        <td class="text-center"><?= $po['prddescricao'] ?></td>
                        <td class="text-center"><?= $po['unmdescricao'] ?></td>
                    </tr>
                </tbody>
            </table>
            <section class="panel-body">
                    <section class="form-group">
                        <label class="col-md-12">Financeiro acumulado (<?=$detalhePeriodo?>)</label>
                    </section>
                    <section class="form-group">
                        <section class="col-md-6">
                            <div class="input-group">
                                <span class="input-group-addon" id="basic-addon1"><strong>Dotação Atual</strong></span>
                                <input type="text" class="form-control" disabled="disabled" value="R$ <?=simec_number_format($po['dotacaoatual'], 2, ',', '.')?>">
                            </div>
                        </section>
                        <section class="col-md-6">
                            <div class="input-group">
                                <span class="input-group-addon" id="basic-addon1"><strong>Empenhado</strong></span>
                                <input type="text" class="form-control" disabled="disabled" value="R$ <?=simec_number_format($po['empenhado'], 2, ',', '.')?>">
                            </div>
                        </section>
                    </section>
                    <section class="form-group">
                        <section class="col-md-6">
                            <div class="input-group">
                                <span class="input-group-addon" id="basic-addon1"><strong>Liquidado</strong></span>
                                <input type="text" class="form-control" disabled="disabled" value="R$ <?=simec_number_format($po['liquidado'], 2, ',', '.')?>">
                            </div>
                        </section>
                        <section class="col-md-6">
                            <div class="input-group">
                                <span class="input-group-addon" id="basic-addon1"><strong>Pago</strong></span>
                                <input type="text" class="form-control" disabled="disabled" value="R$ <?=simec_number_format($po['pago'], 2, ',', '.')?>">
                            </div>
                        </section>
                    </section>
                <section class="form-group">
                    <label class="col-md-12">Físico acumulado (<?=$detalhePeriodo?>)</label>
                </section>
                <section class="form-group">
                    <section class="col-md-6">
                        <div class="input-group">
                            <span class="input-group-addon" id="basic-addon1"><strong>Meta</strong></span>
                            <input type="text" class="form-control" disabled="disabled" value="<?=$po['metafisica']?>">
                        </div>
                    </section>
                    <section class="col-md-6">
                        <div class="input-group">
                            <input type="hidden" name="acpidHidden[<?=$po['sspid']?>]" value="<?=$po['acpid']?>">
                            <? if (!$apenasListar) : ?>
                            <span class="input-group-addon" id="basic-addon1"><strong>Realizado</strong></span>
                            <input type="text" name="realizadoloa[<?=$po['sspid'] ?>]" id="realizadoloa<?= $po['plocod'] ?>" class="form-control" maxlength="11" value="<?=$po['metafisicarealizada']?>" onkeyup="this.value = mascaraglobal('###.###.###', this.value);" onblur="MouseBlur(this);mascaraglobal('###.###.###', this.value);">
                            <? else: ?>
                                <?if ($po['metafisicarealizada'] != ''): ?>
                                <span class="input-group-addon" id="basic-addon1"><strong>Realizado</strong></span>
                                <input type="text" name="realizadoloa[<?=$po['sspid'] ?>]" id="realizadoloa<?= $po['plocod'] ?>" class="form-control" maxlength="11" value="<?=$po['metafisicarealizada']?>" onkeyup="this.value = mascaraglobal('###.###.###', this.value);" onblur="MouseBlur(this);mascaraglobal('###.###.###', this.value);">
                                <? else: ?>
                                <label class="control-label">Realizado:</label>
                                <p class="form-control-static">Sem meta para o acompanhamento.</p>
                                <? endif; ?>
                            <? endif; ?>
                        </div>
                    </section>
                </section>
            </section>

        </section>
    </section>
    <? endforeach; ?>
</section>
<?php endif; ?>