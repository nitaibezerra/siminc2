<?php
/**
 * Formulário de atualização de valores da captação.
 * @version $Id: formValores.inc 96412 2015-04-15 18:51:41Z werteralmeida $
 */
?>
<script type="text/javascript" lang="JavaScript">
jQuery(document).ready(function() {
	$('#capvalorsof').attr('onblur','this.value=mascaraglobal(\'###.###.###.###\',this.value);');
	$('#capvalorsof').blur();
	$('#capvaloruo').attr('onblur','this.value=mascaraglobal(\'###.###.###.###\',this.value);');
	$('#capvaloruo').blur();

    // -- Ações de modificação da previsao
    $('#form-valores').on('click', '.concordar-com-previsao', function(e){
        concordarBloquear();
        $('#formPrincipal').submit();
    }).on('click', '.discordar-da-previsao', function(e){
        descordarDesbloquear();
        $('#botoesformulario').empty();
        $('#botoes button.concordar-com-previsao').clone().css('margin-right', '15px').appendTo('#botoesformulario');
        $('#botoes button.salvar-alteracao-previsao').clone().appendTo('#botoesformulario');
    }).on('click', '.salvar-alteracao-previsao', function(e){
        validaCampo();
    });

    // -- Ajustando os botoes do formulario
    botoesFormulario();
});

function botoesFormulario()
{
    if ('C' == $('#captipo').val()) {
        concordarBloquear();
        $('#botoes button.discordar-da-previsao').clone().appendTo('#botoesformulario');
    } else if ('A' == $('#captipo').val()) {
        $('#botoes button.concordar-com-previsao').clone().css('margin-right', '15px').appendTo('#botoesformulario');
        $('#botoes button.salvar-alteracao-previsao').clone().appendTo('#botoesformulario');
    }
}

function validaCampo(e) {
    if ('C' != $('#captipo').val()) {
        if ("" == $('#capvaloruo').val()) {
            $('#modal-confirm .modal-body p').html('Favor preencher o campo Total UO.');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('OK');
            $('#modal-confirm').modal();
            return false;
        }
        if ("" == $('#justificativa').val()) {
            $('#modal-confirm .modal-body p').html('Favor preencher o campo justificativa.');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('OK');
            $('#modal-confirm').modal();
            return false;
        }
        if ("" == $('#metodologia').val()) {
            $('#modal-confirm .modal-body p').html('Favor preencher o campo metodologia.');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('OK');
            $('#modal-confirm').modal();
            return false;
        }
        if ("" == $('#memoriaCalculo').val()) {
            $('#modal-confirm .modal-body p').html('Favor preencher o campo Memória de Cálculo.');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('OK');
            $('#modal-confirm').modal();
            return false;
        }
    }

    $('#justificativa').prop('required', false);
    $('#metodologia').prop('required', false);
    $('#memoriaCalculo').prop('required', false);
    $('#capvalorsof').prop('required', false);
    $('#capvaloruo').prop('required', false);
    $('#formPrincipal').submit();
}

function concordarBloquear() {
    var valorSOF = $('#capvalorsof').val();
    $('#capvaloruo').val(valorSOF);
    $('#capvaloruo').attr('readonly', true);
    $('#justificativa')
            .attr('readonly', true)
            .attr('data-content', $('#justificativa').val())
            .val('')
            .attr('placeholder', 'De acordo');
    $('#metodologia')
            .attr('readonly', true)
            .attr('data-content', $('#metodologia').val())
            .val('')
            .attr('placeholder', 'De acordo');
    $('#memoriaCalculo')
            .attr('readonly', true)
            .attr('data-content', $('#memoriaCalculo').val())
            .val('')
            .attr('placeholder', 'De acordo');
    $('#usufoneddd').attr('readonly', true);
    $('#usufonenum').attr('readonly', true);
    $('#captipo').val('C');
}

function descordarDesbloquear() {
    //alert('O Total UO deve ser diferente do Total SOF');
    $('#capvaloruo').val('').focus();
    $('#capvaloruo').attr('readonly', false);
    $('#justificativa')
            .attr('readonly', false)
            .val($('#justificativa').attr('data-content'))
            .removeAttr('placeholder')
            .removeAttr('data-content');
    $('#metodologia')
            .attr('readonly', false)
            .val($('#metodologia').attr('data-content'))
            .removeAttr('placeholder')
            .removeAttr('data-content');
    $('#memoriaCalculo')
            .attr('readonly', false)
            .val($('#memoriaCalculo').attr('data-content'))
            .removeAttr('placeholder')
            .removeAttr('data-content');
    $('#usufoneddd').attr('readonly', false);
    $('#usufonenum').attr('readonly', false);
    $('#captipo').val('A');
}
</script>
<div id="botoes" style="display:none">
    <button class="btn btn-info concordar-com-previsao" type="button">
        <span class="glyphicon glyphicon-flag"></span> Concordar com a previsão
    </button>
    <button class="btn btn-warning discordar-da-previsao" type="button">
        <span class="glyphicon glyphicon-remove"></span> Discordar da previsão
    </button>
    <button class="btn btn-primary salvar-alteracao-previsao" id="atualizar" type="button">
        <span class="glyphicon glyphicon-ok"></span> Salvar alteração da previsão
    </button>
</div>
<div class="col-lg-12" id="form-valores">
    <?php if (podeSalvarCaptacao($dadoscaptacao)): ?>
    <div class="alert alert-info col-lg-8 col-lg-offset-2 text-center">
        <b>Importante:</b> Para concordar com a previsão, você pode <u><span style="cursor:pointer" class="concordar-com-previsao">clicar aqui</span></u> ou no botão <i>Concordar com a previsão</i>.
    </div>
    <?php endif; ?>
    <br style="clear:both" />
    <div class="well">
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal" enctype="multipart/form-data">
            <input type="hidden" name="acao" id="acao" value="salvar" />
            <input type="hidden" name="usucpf" value="<?php echo $dadoscaptacao['usucpf']; ?>" id="usucpf" />
            <input type="hidden" name="capid" value="<?php echo $dadoscaptacao['capid']; ?>" />
            <input type="hidden" name="captipo" value="<?php echo $dadoscaptacao['captipo']; ?>" id="captipo" />
            <div class="form-group col-md-4">
                <label for="capvalorsof" class="col-lg-4 control-label">Total SOF:</label>
                <div class="col-lg-8 input-group">
                    <span class="input-group-addon">R$</span>
                    <?php
                    $options = array(
                        'habil' => 'N',
                        'complemento' => 'required="required"',
                        'masc' => '###.###.###.###',
						'evtblur' => "this.value=mascaraglobal(\'###.###.###.###\',this.value);"
                    );
                    inputTexto('capvalorsof', $previsoes['total'], 'capvalorsof', 15, false, $options);
                    ?>
                </div>
            </div>
            <div class="form-group col-md-4">
                <label for="capvaloruo" class="col-lg-4 control-label">Total UO:</label>
                <div class="col-lg-8 input-group">
                    <span class="input-group-addon">R$ </span>
                    <?php
                    $options['habil'] = 'S';
                    if ('C' == $dadoscaptacao['captipo']) {
                    	$options['habil'] = 'N';
                    }
                    inputTexto('capvaloruo', $dadoscaptacao['capvaloruo'], 'capvaloruo', 15, false, $options);
                    ?>
                </div>
            </div>
            <br style="clear:both" />
            <div class="form-group">
                <label for="inputJustificativa" class="col-lg-2 control-label">Justificativa:</label>
                <div class="col-lg-10">
                    <?php
                    $options = array(
                        'complemento' => array(
                            'required' => true
                        )
                    );
                    if ('C' == $dadoscaptacao['captipo']) {
                        $options['complemento']['readonly'] = true;
                        $options['complemento']['placeholder'] = 'De acordo';
                    }
                    inputTextArea('justificativa', $dadoscaptacao['justificativa'], 'justificativa', 500, $options);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <label for="inputMetofologia" class="col-lg-2 control-label">Metodologia:</label>
                <div class="col-lg-10">
                    <?php inputTextArea('metodologia', $dadoscaptacao['metodologia'], 'metodologia', 500, $options); ?>
                </div>
            </div>
            <div class="form-group">
                <label for="inputMemoria" class="col-lg-2 control-label">Memória de Cálculo:</label>
                <div class="col-lg-10">
                    <?php inputTextArea('memoriacalculo', $dadoscaptacao['memoriacalculo'], 'memoriaCalculo', 500, $options); ?>
                </div>
            </div>
            <div class="form-group">
                <label for="usunome" class="col-lg-2 control-label">Dados do
                    Usuário:</label>
                <div class="col-lg-10" style="margin-top:7px"><?php echo $dadoscaptacao['usunome']; ?></div>
            </div>
            <div class="form-group">
                <label for="usuemail" class="col-lg-2 control-label">Email:</label>
                <div class="col-lg-3">
                    <input name="usuemail" id="usuemail" class="form-control"
                           type="text" value="<?php echo $dadoscaptacao['usuemail']; ?>"
                           <?php
                           if ($dadoscaptacao['captipo'] == 'C') {
                               echo "readonly";
                           }
                           ?>
                           required="required" size="6" />
                </div>
                <label for="usufoneddd" class="col-lg-1 control-label">DDD:</label>
                <div class="col-lg-1">
                    <input name="usufoneddd" id="usufoneddd" class="form-control"
                           type="text" value="<?php echo $dadoscaptacao['usufoneddd'] ?>" maxlength="2"
                           <?php
                           if ($dadoscaptacao['captipo'] == 'C') {
                               echo "readonly";
                           }
                           ?>
                           required="required" size="6" />
                </div>
                <label for="usufonenum" class="col-lg-1 control-label">Telefone:</label>
                <div class="col-lg-3">
                    <input name="usufonenum" maxlength="10" id="usufonenum" class="form-control"
                           type="text" value="<?php echo $dadoscaptacao['usufonenum'] ?>"
                           <?php
                           if ($dadoscaptacao['captipo'] == 'C') {
                               echo "readonly";
                           }
                           ?>
                           required="required" size="6" />
                </div>
            </div>
            <br style="clear:both" />
            <?php
            if(podeSalvarCaptacao($dadoscaptacao)){
                echo "<div id=\"botoesformulario\"></div>";
            } else {
                echo gravacaoDesabilitada('');
            }
            ?>
        </form>
    </div>
</div>