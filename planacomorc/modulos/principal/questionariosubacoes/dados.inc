<?php
$monsubacaquestionario = $db->pegaLinha ( "SELECT *
                                     FROM planacomorc.monsubacaquestionario
                                     WHERE qstid='" . $_SESSION ['planacomorc'] ['qstid'] . "'" );
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script>
function atualizarQuestionario() {
    if(jQuery('#qstnome').val()=='') {
        alert('Nome do questionário em branco');
        return false;
    }
    if(jQuery('#percod').val()=='') {
        alert('Período em branco');
        return false;
    }
    $('#formquestionario').submit();
}
</script>
<section class="col-md-12">
    <section class="well">
        <form class="form-horizontal" method="post" name="formquestionario" id="formquestionario">
            <input type="hidden" name="requisicao" value="atualizarQuestionario">
            <section class="form-group">
                    <label class="control-label col-md-2" for="nome">Nome:</label>
                    <section class="col-md-10">
                            <?php inputTexto('qstnome', $monsubacaquestionario['qstnome'], 'nome', 150,false);?>
                    </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="subacoes">Subações:</label>
                <section class="col-md-10">
                <?php
                $sql = <<<DML
SELECT s.id_subacao AS codigo,
s.codigo || ' - ' || titulo AS descricao,
mqp.qstid
FROM planacomorc.subacao s
LEFT JOIN planacomorc.monqstsubacaperiodo mqp
ON (mqp.id_subacao = s.id_subacao AND mqp.qstid = {$_SESSION['planacomorc']['qstid']})
WHERE s.id_exercicio = {$_SESSION['exercicio']}
DML;
                $selectedItems = array ();
                if ($dados = $db->carregar ( $sql )) {
                    foreach ($dados as $item) {
                        if (! empty ($item['qstid'])) {
                            $selectedItems[] = $item['codigo'];
                        }
                    }
                }
                inputCombo('id_subacoes[]', $sql, $selectedItems, 'subacoes', array('multiple'=>'multiple','titulo'=>'Selecione'));
                ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2" for="percod">Períodos:</label>
                <section class="col-md-10">
                        <?
                        $sql = "
                                SELECT
                                        id_periodo_referencia as codigo,
                        titulo || ' (' || to_char(inicio_validade,'MM/YYYY') ||' até ' || to_char(fim_validade,'MM/YYYY') || ') / ' || descricao as descricao
                        FROM planacomorc.periodo_referencia
                        ORDER BY 1";
                        inputCombo('percod', $sql, $monsubacaquestionario ['id_periodo_referencia'], 'percod')
                        ?>
                </section>
            </section>
            <input type="button" class="btn btn-primary" name="questionario" value="Salvar"	onclick="atualizarQuestionario();">
        </form>
    </section>
</section>