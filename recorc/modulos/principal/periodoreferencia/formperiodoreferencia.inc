<?php
/**
 * Formulário de cadastro e edição de períodos de referência.
 * $Id: formperiodoreferencia.inc 98390 2015-06-09 18:48:49Z maykelbraz $
 */
?>
<style>
.pad-12{padding-top:12px!important}
.col-lg-10 img{display:none}
</style>
<script type="text/javascript" language="javascript">
function MouseOver(b){}
function MouseOut(b){}
function MouseClick(b){}
function MouseBlur(b){}
function textCounter(b){}
$(document).ready(function(){
    $('#formSavePerReferencia input').addClass('form-control');
    $('#formSavePerReferencia textarea').addClass('form-control');
    $('input[name=no_perdescricao]').css('width', '100px');

    $('#prfdatainicio').mask('99/99/9999').datepicker({});
    $('#prfdatafim').mask('99/99/9999').datepicker({});
    $('#prfpreenchimentoinicio').mask('99/99/9999').datepicker({});
    $('#prfpreenchimentofim').mask('99/99/9999').datepicker({});

    // -- Ação do botão buscar
    $('#buttonSearch').click(function(){
        $('input[required]').removeProp('required');
        $('#action').val('search');
        $('#formSavePerReferencia').submit();
    });

    // -- Ação de inserir
    $('#buttonSave').click(function(e){
        var enviar = true;
        // -- Limpando todos os erros anteriores
        $('#formSavePerReferencia .has-error').removeClass('has-error');
        var enviar = true;
        var msg = '<div class="alert alert-danger text-center">';
        $('input[required]').each(function(){
            if ('' == $(this).val()) {
                enviar = false;
                var label = $(this).parent().prev().text();
                $(this).closest('.form-group').addClass('has-error');
                msg += '<p>O campo <strong>' + label.replace(':', '') + '</strong> é obrigatório e não pode ser deixado em branco.</p>';
            }
        });
        msg += '</div>';
        if (enviar) {
            $('#action').attr('value', 'insert');
            $('#formSavePerReferencia').submit();
        } else {
            $('.modal-body').html(msg);
            $('#modal-alert').modal();
            return false;
        }
    });

    // -- Caixa de confirmação de exclusão de registros (botão sim)
    $('#modal-confirm button.btn-primary').click(function(){
        $('input[required]').removeProp('required');
        $('#action').val('delete');
        $('#formSavePerReferencia').submit();
    });
});

function deletePeriodoReferencia(id)
{
    $('.modal-body').html('<p>Deseja apagar o período de referência selecionado?</p>');
    $('#prfid').val(id);
    $('#modal-confirm').modal({'backdrop': 'static'});
}

function editPeriodoReferencia(id)
{
    window.location = window.location.href + '&action=load&data[prfid]=' + id;
}

</script>
<div class="well">
    <form id="formSavePerReferencia" name="formSave" method="POST" class="form-horizontal" novalidate>
        <fieldset>
            <input type="hidden" name="action" id="action" />
            <input type="hidden" name="data[prfid]" id="prfid" value="<?php echo $formData['prfid']; ?>" />
            <div class="form-group control-group">
                <label for="prfdsc" class="col-lg-2 control-label pad-12">Descrição:</label>
                <div class="col-lg-10">
                    <?php
                    echo campo_texto('data[prfdsc]', "S", "S", "Nome", 12, 50, "", "", '', '', 0, 'id="prfdsc" required', '', $formData['prfdsc']);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <label for="prfdatainicio" class="col-lg-2 control-label pad-12">Período de validade:</label>
                <div class="col-lg-2">
                    <input name="data[prfdatainicio]" type="text" class="form-control" id="prfdatainicio"
                           placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                           value="<?php echo $formData['prfdatainicio']; ?>" required />
                </div>
                <label class="col-lg-1 control-label pad-12" for="prfdatafim" style="text-align:center"> até </label>
                <div class="col-lg-2">
                    <input name="data[prfdatafim]" type="text" class="form-control" id="prfdatafim"
                           placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                           value="<?php echo $formData['prfdatafim']; ?>" required />
                </div>
            </div>
            <div class="form-group">
                <label for="prfpreenchimentoinicio" class="col-lg-2 control-label pad-12">Período de preenchimento:</label>
                <div class="col-lg-2">
                    <input name="data[prfpreenchimentoinicio]" type="text" class="form-control" id="prfpreenchimentoinicio"
                           placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                           value="<?php echo $formData['prfpreenchimentoinicio']; ?>" required />
                </div>
                <label class="col-lg-1 control-label pad-12" for="prfpreenchimentofim" style="text-align:center"> até </label>
                <div class="col-lg-2">
                    <input name="data[prfpreenchimentofim]" type="text" class="form-control" id="prfpreenchimentofim"
                           placeholder="00/00/0000" maxlength="10" data-format="dd/MM/yyyy hh:mm:ss"
                           value="<?php echo $formData['prfpreenchimentofim']; ?>" required />
                </div>
            </div>
            <?php
            $opcoes = array('flabel' => 'Cód. Captação', 'masc' => '###');
            inputTexto('data[codcaptacaosiop]', $formData['codcaptacaosiop'], 'codcaptacaosiop', 3, false, $opcoes);
            $opcoes = array('flabel' => 'Exercício SIOP', 'masc' => '####');
            inputTexto('data[exerciciosiop]', $formData['exerciciosiop'], 'exerciciosiop', 4, false, $opcoes);
            ?>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <?php if($formData['prfid']) : ?>
                    <button class="btn btn-danger" id="buttonCancel" >Cancelar</button>
                    <button class="btn btn-info" id="buttonSave" >Atualizar</button>
                    <?php else: ?>
                    <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
                    <button class="btn btn-success" id="buttonSave">Inserir</button>
                    <button class="btn btn-primary" id="buttonSearch" type="button">Buscar</button>
                    <?php endif ?>
                </div>
            </div>
        </fieldset>
    </form>
</div>
<?php echo getFlashMessage(); ?>