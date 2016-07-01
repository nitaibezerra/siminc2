<div class="row">
     <div class="col-lg-12">
        <table class="table table-bordered table-condensed  tbl_verde tbl_plano_implementacao">
            <thead>
            <tr>
                <th>ETAPAS DE IMPLEMENTAÇÃO</th>
                <th>ATIVIDADES</th>
                <th>STATUS</th>
                <th>RESPONSÁVEIS</th>
                <th>PRAZO</th>
            </tr>
            </thead>
            <tbody>

            <?php
            $etpid_old = array();
            if (!empty($this->dado['etapas'])): ?>

                <?php foreach ($this->dado['etapas'] as $etapa): ?>
                    <?php
                    $dadosAtividade = $this->atividade->getDados((int)$etapa['etpid'], true);
                    $count = count($dadosAtividade);
                    if ( !in_array($etapa['etpid'], $etpid_old )):
                        ?>
                        <tr>
                        <td rowspan="<?= $count; ?>" class="etapa_impl"><?= $etapa['etpdsc']; ?></td>

                        <?php if (!empty($dadosAtividade)):  ?>
                        <?php foreach ($dadosAtividade as $keyAtv => $atividade): ?>
                            <?php if ($keyAtv != 0): ?>
                                <tr>
                            <?php endif;
                            ?>
                            <td><?= $atividade['atvdsc']; ?></td>
                            <td align="center"><?= ($atividade['esddsc']=='Finalizado' ? '<img src="../imagens/check_checklist.png" title="'.$atividade['esddsc'].'" class="img_middle link">' : '<img src="../imagens/check_checklist_vermelho.png" title="'.$atividade['esddsc'].'" class="img_middle link">'); ?></td>
                            <td><?= $atividade['usucpf']; ?></td>
                            <td><?= $this->view->atividade->getDataPrazoComCor( $atividade['atvprazo'] ); ?></td>
                            </tr>
                        <?php endforeach; ?>

                    <?php else: ?>
                        <td colspan="4"></td>
                        </tr>
                    <?php
                    endif;
                        $etpid_old[] = $etapa['etpid'];
                        ?>
                    <?php endif; ?>

                <?php endforeach; ?>
            <?php endif; ?>
            </tbody>

        </table>
    </div>
</div>