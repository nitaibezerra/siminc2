<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
    <h4 class="modal-title">Adicionar Executor</h4>
</div>

<div class="modal-body">
    <form id="form-usuario-pessoa-fisica" class="form-horizontal" action="">
        <fieldset>

            <legend class="text-center">Selecione um</legend>
            <div class="row">
                <div class="col-lg-offset-3 col-lg-6">
                    <div class="form-group">
                        <div class="col-lg-12">
                            <select name="executor_cadastrado" id="executor_cadastrado" class="form-control">
                                <option value="">Selecione ...</option>
                                <?= $this->responsavelSolucao->getOptionsResponsavelSe(); ?>
                            </select>
                        </div>
                    </div>
                </div>
            </div>
            <h4 class="text-center">ou</h4>
            <br>
            <legend class="text-center">Cadastre um novo</legend>
            <div class="row">
                <div class="col-lg-6">

                    <div class="form-group">
                        <label for="usucpf" class="col-lg-3 control-label"><?= $this->usuario->getAttributeLabel('usucpf'); ?></label>

                        <div class="col-lg-3">
                            <input type="text" id="usucpf" name="usucpf" class="form-control" maxlength="15" placeholder="___-___-___-__">
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="usunome" class="col-lg-3 control-label"><?= $this->usuario->getAttributeLabel('usunome'); ?></label>

                        <div class="col-lg-7">
                            <input type="text" id="usunome" name="usunome" readonly="readonly" class="form-control" maxlength="80">
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="usuemail" class="col-lg-3 control-label"><?= $this->usuario->getAttributeLabel('usuemail'); ?></label>

                        <div class="col-lg-7">
                            <input type="text" id="usuemail" name="usuemail" class="form-control">
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="usufonenum" class="col-lg-3 control-label"><?= $this->usuario->getAttributeLabel('usufonenum'); ?></label>

                        <div class="col-lg-2">
                            <input type="text" id="usufoneddd" name="usufoneddd" class="form-control" placeholder="__" maxlength="2">
                        </div>
                        <div class="col-lg-3">
                            <input type="text" id="usufonenum" name="usufonenum" class="form-control" placeholder="____-____">
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="usuobs" class="col-lg-3 control-label"><?= $this->usuario->getAttributeLabel('usuobs'); ?></label>

                        <div class="col-lg-7">
                            <textarea rows="5" id="usuobs" name="usuobs" class="form-control"></textarea>
                        </div>
                    </div>
                </div>

                <div class="col-lg-6">
                    <div class="form-group">
                        <label class="col-lg-3 control-label"
                               for="regcod"><?= $this->usuario->getAttributeLabel('regcod'); ?></label>

                        <div class="col-lg-8">
                            <select id="regcod" name="regcod" class="form-control">
                                <?php echo $this->usuario->getComboUfs(); ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" dislplay="none">
                        <label class="col-lg-3 control-label" for="muncod"><?= $this->usuario->getAttributeLabel('muncod'); ?></label>

                        <div class="col-lg-8">
                            <select id="muncod" name="muncod" class="form-control">
                                <option value=""> Selecione uma UF</option>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="div_tpocod" style="display: none;">
                        <label class="col-lg-3 control-label" for="tpocod"><?= $this->usuario->getAttributeLabel('tpocod'); ?></label>

                        <div class="col-lg-8">
                            <select id="tpocod" name="tpocod" class="form-control">
                                <?php echo $this->usuario->getComboTipoOrgao(); ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="div_entidade" style="display: none;">
                        <label class="col-lg-3 control-label" for="entid"><?= $this->usuario->getAttributeLabel('entid'); ?></label>

                        <div class="col-lg-8">
                            <select id="entid" name="entid" class="form-control">
                                <option value=""> Selecione um Tipo de Órgão</option>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="ususexo" class="col-lg-3 control-label"><?= $this->usuario->getAttributeLabel('ususexo'); ?></label>

                        <div class="col-lg-5 btn-group bts_etapas_de_controle" data-toggle="buttons">
                            <label class="btn btn-default active label_sexo">
                                <input type="radio" value="M" name="ususexo">Masculino
                            </label>
                            <label class="btn btn-default label_sexo">
                                <input type="radio" value="F" name="ususexo">Feminino
                            </label>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="usudatanascimento" class="col-lg-3 control-label"><?= $this->usuario->getAttributeLabel('usudatanascimento'); ?></label>

                        <div class="col-lg-3">
                            <input id="usudatanascimento" name="usudatanascimento" type="text" class="form-control data" maxlength="10" placeholder="dd/mm/aaaa">
                        </div>
                    </div>
                </div>
            </div>
        </fieldset>
    </form>
</div>

<div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal"><span class="glyphicon glyphicon-remove"></span> Fechar </button>
    <button id="bt_salvar_executor" type="button" class="btn btn-success"><span class="glyphicon glyphicon-floppy-disk"></span> Salvar </button>
</div>

<script>
    $(document).ready(function () {
        $(".data").mask("99/99/9999");
        $("#usucpf").mask("999.999.999-99");
        $("#usufoneddd").mask("999");
        $("#usufonenum").mask("9999-9999");

        $(".data").datepicker({
            defaultDate: "-25y +1w",
            changeMonth: true,
            changeYear: true,
            numberOfMonths: 1,
            showAnim: 'fadeIn'
        });

        $('#usucpf, #executor_cadastrado ').on('blur', function (e) {
            getCpf($(this).val());
        });

        $('#regcod').on('change', function (e) {
            $.post(window.location.href, {controller: 'usuario', action: 'getMunicipios', regcod: $(this).val()}, function (data) {
                $('#muncod').html(data);
                $('#div_tpocod').show();
                $('#div_entidade').show();
            });
        });

        $('#tpocod').on('change', function (e) {
            $.post(window.location.href, {controller: 'usuario', action: 'getOrgaos', tpocod: $('#tpocod').val(), regcod: $('#regcod').val(), muncod: $('#muncod').val()}, function (data) {
                $('#entid').html(data);
            });
        });

        /*** SALVA O ETAPA CONTROLE***/
        $('#bt_salvar_executor').on('click', function () {
            if ( $('#executor_cadastrado').val() == '' && $('#usucpf').val().length == 0 ) {
                alert('Selecione um Executor!');
            }
            if ( $('#usucpf').val() ) {
//                var resp = confirm("Este usu\u00e1rio n\u00e3o existe no sistema SIMEC, deseja cri\u00e1-lo?");
//                if (resp) {
                    $('#form-usuario-pessoa-fisica').saveAjax({
                        controller: 'usuario',
                        action: 'salvarUsuario',
                        retorno: true,
                        displayErrorsInput: true,
                        functionSucsess: 'retornoCadastroExecutor'
                    });
//                } else {
//                    return false;
//                }
            }
// else if( $('#executor_cadastrado').val() != '' ){
//                $('.usucpf_atividade').val(  $('#executor_cadastrado').val() );
//                $('#nome_executor').html(' -- <b>Selecionado:</b> ' + $('#executor_cadastrado option:selected').text() );
//                $('#span_nome_executor').show();
//                $('#dialogo_formulario_executor').modal('hide');
//            }else{
//                alert('Selecione um Executor!');
//            }
        });
    });
</script>