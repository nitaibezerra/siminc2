<?php
$strSQL = "SELECT * FROM planacomorc.monquestionario WHERE qstid = '%s'";
$stmt = sprintf($strSQL, $_SESSION['planacomorc']['qstid']);
$monquestionario = $db->pegaLinha($stmt);
?>

<section class="col-md-12">
    <section class="well">
        <form class="form-horizontal" name="formquestionario" id="formquestionario" action="" method="POST" role="form">
            <input type="hidden" name="requisicao" value="atualizarQuestionario">
            <input type="hidden" name="qstid" id="qstid" value="<?= $monquestionario['qstid'] ?>" />
            <div class="form-group">
                <label class="control-label col-md-2" for="qstnome">Nome:</label>
                <div class="col-md-10">
                    <?= inputTexto('qstnome', $monquestionario['qstnome'], 'qstnome', '62',false); ?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-2" for="id_acao_programatica">Ações:</label>
                <div class="col-md-10">
                    <?php
                    $sql = "SELECT distinct
                        id_acao_programatica as codigo,
                        a.codigo || '.' || org.codigo as descricao
                    FROM planacomorc.acao a
                        join planacomorc.acao_programatica apr on apr.id_acao = a.id_acao
                        join planacomorc.dados_acao_exercicio dae on dae.id_acao = a.id_acao and dae.id_exercicio=apr.id_exercicio
                        join planacomorc.programa pro on pro.id_programa = a.id_programa
                        join planacomorc.orgao org on org.id_orgao = apr.id_orgao
                    WHERE apr.id_exercicio = '".$_SESSION['exercicio']."'
                    ORDER BY descricao";

                    $selectedItems = array();
                    $strSQL = "
                        SELECT
                          id_acao_programatica
                        FROM
                          planacomorc.monqstacaperiodo
                        WHERE
                          qstid = {$monquestionario['qstid']}
                    ";

                    $itens = $db->carregar($strSQL);
                    if (is_array($itens)) {
                        foreach ($itens as $item) {
                            if (!empty($item['id_acao_programatica'])) {
                                $selectedItems[] = $item['id_acao_programatica'];
                            }
                        }
                    }

                    echo inputCombo('id_acao_programatica[]', $sql, $selectedItems, 'id_acao_programatica', array('multiple'=> 'multiple','titulo'=>'Selecione ao menos uma opção'));
                    ?>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label col-md-2" for="id_periodo_referencia">Periodos:</label>
                <div class="col-md-10">
                    <?php
                    $strSQL = "
                        SELECT
                          id_periodo_referencia as codigo,
                          titulo || ' (' || to_char(inicio_validade,'MM/YYYY') ||' até ' || to_char(fim_validade,'MM/YYYY') || ') / ' || id_exercicio as descricao
                        FROM planacomorc.periodo_referencia
                        ORDER BY 1
                    ";
                    echo inputCombo('id_periodo_referencia', $strSQL, $monquestionario['id_periodo_referencia'], 'id_periodo_referencia');
                    ?>
                </div>
            </div>
            <div class="form-group">                
                <div class="col-md-12">
                    <button type="submit" class="btn btn-primary" id="btn-Salva-Questionario">Salvar</button>
                </div>
            </div>
        </form>
    </section>
</section>