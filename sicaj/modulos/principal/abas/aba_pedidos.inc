<div class="row col-md-11">
    <div class="well">
        <form name="registrosicaj" class="form-horizontal" id="registrosicaj" method="POST" role="form">
            <input type="hidden" name="pdcid" id="pdcid" value="<?=$dados['pdcid']?>" />
            <input type="hidden" name="requisicao" id="requisicao" value="salvarPedidos" />
            <input type="hidden" name="unicod" id="unicod" value="<?= $unicod; ?>" />
            <input type="hidden" name="usucpf" id="usucpf" value="<?= $_SESSION['usucpf']; ?>" />
            <input type="hidden" name="pdctipo" id="pdctipo" value="<?php echo $pdctipo; ?>" />
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="numprocessoadm">Processo Administrativo:</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('numprocessoadm', $dados['numprocessoadm'], 'numprocessoadm', '', false,
                        array(
                            'limite' => '20',
                            'masc' => '#####.######/####-##',
                            'evtblur' => "this.value=mascaraglobal('#####.######/####-##', this.value);"
                        ));
                    ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdcnumprocessojudicial">Processo Judicial</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdcnumprocessojudicial', $dados['pdcnumprocessojudicial'], 'pdcnumprocessojudicial', '', false, array('limite' => '50')); ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdcjuizodacao">Juizo da Ação:</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdcjuizodacao', $dados['pdcjuizodacao'], 'pdcjuizodacao', '', false, array('limite' => '50')); ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdccodacaojudicial">Código da ação no SICAJ:</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdccodacaojudicial', $dados['pdccodacaojudicial'], 'pdccodacaojudicial', '', false, array('limite' => '6')); ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdccodobjeto">Código do objeto no SICAJ:</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdccodobjeto', $dados['pdccodobjeto'], 'pdccodobjeto', '', false, array('limite' => '50')); ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdcnumbeneficioacao">Número de beneficiários:</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdcnumbeneficioacao', $dados['pdcnumbeneficioacao'], 'pdcnumbeneficioacao', '', false, array('limite' => '50')); ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdcdatainicio">Início da eficácia temporal*:</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdcdatainicio', $dados['pdcdatainicio'], 'pdcdatainicio', '', false, $metaDataCombo); ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdcvalorexecatual">Valor no Exercício Atual (R$):</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdcvalorexecatual', $dados['pdcvalorexecatual'], 'pdcvalorexecatual', '', true); ?>
                </div>
            </div>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdcvalorexecpassado">Valor no Exercício Anterior (R$):</label>
                </div>
                <div class="col-md-9">
                    <?php inputTexto('pdcvalorexecpassado', $dados['pdcvalorexecpassado'], 'pdcvalorexecpassado', '', true); ?>
                </div>
            </div>
            <?php if (isset($_GET['id'])) : ?>
            <div class="form-group ">
                <div class="col-md-3">
                    <label class="control-label" for="pdcvalorcdo">Dados do usuário:</label>
                </div>
                <div class="col-md-9">
                    <p class="form-control-static">
                        <strong>
                    <?php
                        $dadosUser = getUsuarioPedido($_GET['id']);
                        echo $dadosUser['usunome'];
                    ?>
                        </strong>
                    </p>
                </div>
            </div>
            <div class="form-group ">
                <label class="col-lg-3 control-label" for="usuemail">Email:</label>
                <div class="col-lg-3">
                    <?php inputTexto('usuemail', $dadosUser['usuemail'], 'usuemail', '', false, array('limite' => '60')); ?>
                </div>
                <label class="col-lg-1 control-label" for="usufoneddd">DDD:</label>
                <div class="col-lg-1">
                    <?php inputTexto('usufoneddd', $dadosUser['usufoneddd'], 'usufoneddd', '', false, array('limite' => '2')); ?>
                </div>
                <label class="col-lg-1 control-label" for="usufonenum">Telefone:</label>
                <div class="col-lg-3">
                    <?php inputTexto('usufonenum', $dadosUser['usufonenum'], 'usufonenum', '', false, array('limite' => '15')); ?>
                </div>
            </div>
            <?php endif; ?>
            <br />
            <button type="button" class="btn btn-warning" id="limpar">Limpar</button>
            <button type="button" class="btn btn-primary" id="inserir">Salvar</button>
        </form>
    </div>
</div>
<div class="row col-md-1">
    <?php require dirname(__FILE__) . '/../tramitacao.inc';  ?>
</div>

<script type="text/javascript">
$(function(){
    /**
     * Ação para o botão inserir do formulário de pedidos
     * Faz validação de campos vazios
     */
    $("#inserir").on("click", function(){

        $("#registrosicaj .form-group").removeClass("has-error");

        var inputs = ['numprocessoadm', 'pdcnumprocessojudicial', 'pdccodacaojudicial', 'pdccodobjeto',
                      'pdcnumbeneficioacao', 'pdcdatainicio', 'pdcjuizodacao', 'pdcvalorexecatual', 'pdcvalorexecpassado']
          , errors = false;

        for (var i=0; inputs.length>i; i++) {
            if (!$("#"+inputs[i]).val()) {
                var formGroup = $("#"+inputs[i]).parent().parent();
                $(formGroup).addClass("has-error");
                errors = true;
            }
        }

        //console.log(errors);
        if (errors) return false;
        $("#registrosicaj").submit();
    });
    $("#pdcvalorexecatual").blur();
    $("#pdcvalorexecpassado").blur();
});
</script>