<?php
//header ('Content-type: text/html; charset=UTF-8');

$estado = wf_pegarEstadoAtual($this->entity['docid']['value']);
$demanda = new Controller_Documento();

if ($this->entity['dmdid']['value']) {
    $historico = $demanda->buscar_historico_documento($this->entity['dmdid']['value']);
}

$liberacao = $demanda->liberar_alteracao($this->entity['docid']['value']);
if ($historico['dmaid']) {
    $this->entity['dmdassunto']['value'] = $historico['dmaassunto'];
    $this->entity['dmdprazoemdias']['value'] = $historico['dmaprazoemdias'];
    $this->entity['dmdprazoemdata']['value'] = $historico['dmaprazoemdata'];
    $this->entity['docid']['value'] = $historico['docid'];
}

?>
<div class="modal-dialog-large" style="width: 800px">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
            <h3 class="modal-title">
                Salvar documento
                <!--                <small>na Instituição -->
                <?php //echo $_SESSION['instituicao']['intdscrazaosocial']  ?><!--</small>-->
            </h3>
            <!--            <h4 class="modal-title"></h4>-->
        </div>
        <div class="modal-body">
            <div class="row">
                <div class="col-md-12">
                    <form id="form_save" method="post" class="form-horizontal">
                        <input name="dmdid" value="<?php echo $this->entity['dmdid']['value'] ?>" type="hidden" />
                        <input name="dmaid" value="<?php echo $historico['dmaid'] ?>" type="hidden" />
                        <input name="docid" value="<?php echo $this->entity['docid']['value'] ?>" type="hidden" />
                        <div class="col-md-10">
                            <div class="well">
                                <fieldset>
                                    <!--                                        <legend>Pesquisa</legend>-->
                                    <!--                                        <div class="col-md-1"></div>-->
                                    <!--                                        <div class="col-md-10">-->
                                    <div class="form-group">
                                        <label for="tpdid" class="col-lg-2 col-md-2  control-label">Tipo</label>
                                        <div class="col-lg-10 col-md-10 ">
                                            <?php if ($estado['esdid'] == ESD_DEMANDA_EM_ATENDIMENTO || $estado['esdid'] == ESD_DEMANDA_EM_DILIGENCIA) { ?>
                                                <div class="btn-group" data-toggle="buttons">
                                                    <label
                                                        class="btn btn-default active">
                                                        <input id="tpdid" name="tpdid" type="radio" 
                                                               value="<?php echo $this->entity['tpdid']['value'] ?>" checked="checked">
                                                               <?php
                                                               if ($this->entity['tpdid']['value'] == '1') {
                                                                   echo "Ofício";
                                                               } else if ($this->entity['tpdid']['value'] == '2') {
                                                                   echo "Memo";
                                                               } else if ($this->entity['tpdid']['value'] == '3') {
                                                                   echo "Portaria";
                                                               } else if ($this->entity['tpdid']['value'] == '4') {
                                                                   echo "Despacho";
                                                               }
                                                               ?>
                                                    </label>
                                                </div>
                                                <?php } else { ?>
                                                <div class="btn-group" data-toggle="buttons">
    <?php foreach ($this->tipoDocumento as $tipoDocumento): ?>
                                                        <label
                                                            class="btn btn-default <?php if ($this->entity['tpdid']['value'] == $tipoDocumento['tpdid']) echo 'active' ?>">
                                                            <input id="tpdid" name="tpdid" type="radio" 
                                                                   value="<?php echo $tipoDocumento['tpdid'] ?>" <?php if ($this->entity['tpdid']['value'] == $tipoDocumento['tpdid']) echo 'checked="checked"' ?>>
                                                        <?php echo $tipoDocumento['tpddsc'] ?>
                                                        </label>
                                                <?php endforeach; ?>
                                                </div>
<?php } ?>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="" class="col-lg-2 col-md-2 control-label">Número</label>
                                        <div class="col-lg-3 col-md-3 ">
                                            <input <?= $liberacao['readonly'] ?>  id="dmdnumdocumento" name="dmdnumdocumento" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['dmdnumdocumento']['value'] ?>" maxlength="4">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmddb" class="col-lg-2 col-md-2  control-label">D/B</label>

                                        <div class="col-lg-10 col-md-10  ">
                                            <div class="btn-group" data-toggle="buttons">
                                                <label
                                                    class="btn btn-default <?php if ($this->entity['dmddb']['value'] == 'D') echo 'active' ?>">
                                                    <input id="dmddb" name="dmddb" type="radio"
                                                           value="D" <?php if ($this->entity['dmddb']['value'] == 'D') echo 'checked="checked"' ?>>
                                                    D
                                                </label>
                                                <label
                                                    class="btn btn-default <?php if ($this->entity['dmddb']['value'] == 'B') echo 'active' ?>">
                                                    <input id="dmddb" name="dmddb" type="radio"
                                                           value="B" <?php if ($this->entity['dmddb']['value'] == 'B') echo 'checked="checked"' ?>>
                                                    B
                                                </label>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmdassunto" class="col-lg-2 col-md-2  control-label">Assunto</label>

                                        <div class="col-lg-10 col-md-10  ">
                                            <?php
                                            echo inputTextArea('dmdassunto', $this->entity['dmdassunto']['value'], 'dmdassunto2', 500, array('cols' => 50, 'rows' => 4));
                                            ?>
                                            <!-- input id="dmdassunto" name="dmdassunto" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['dmdassunto']['value'] ?>"> -->
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="prcid_orig" class="col-lg-2 col-md-2  control-label">Interessado</label>

                                        <div class="col-lg-10 col-md-10  ">
                                            <select  <?= $liberacao['disabled'] ?> id="prcid_orig" name="prcid_orig" class="form-control" data-placeholder="Selecione">
                                                <option></option>
                                                <?php foreach ($this->procedencias as $procedencia): ?>
                                                    <option <?php if ($this->entity['prcid_orig']['value'] == $procedencia['prcid']) echo 'selected="selected"' ?> value="<?php echo $procedencia['prcid'] ?>"><?php echo $procedencia['prcdsc'] ?></option>
<?php endforeach; ?>
                                            </select>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="prcid_dest" class="col-lg-2 col-md-2  control-label">Destino</label>

                                        <div class="col-lg-10 col-md-10  ">
                                            <select <?= $liberacao['disabled'] ?> id="prcid_dest" name="prcid_dest" class="form-control" data-placeholder="Selecione" >
                                                <option></option>
                                                <?php foreach ($this->procedencias as $procedencia): ?>
                                                    <option <?php if ($this->entity['prcid_dest']['value'] == $procedencia['prcid']) echo 'selected="selected"' ?> value="<?php echo $procedencia['prcid'] ?>"><?php echo $procedencia['prcdsc'] ?></option>
<?php endforeach; ?>
                                            </select>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmdreferencia" class="col-lg-2 col-md-2  control-label">Referência</label>
                                        <div class="col-lg-10 col-md-10  ">
                                            <input id="dmdreferencia" <?= $liberacao['readonly'] ?>  name="dmdreferencia" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['dmdreferencia']['value'] ?>">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-2 col-md-2 control-label">Reiteração</label>

                                        <div class="col-lg-2 col-md-2">
                                            <input type="hidden" name="dmdreiteracao" id="dmdreiteracao" value="f"/>
                                            <input <?= $liberacao['disabled'] ?> type="checkbox" name="chkreiteracao" id="chkreiteracao" onclick="checaReiteracao(this)" <?= $this->entity['dmdreiteracao']['value'] == 't' ? 'checked' : '' ?> />
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmdnumsidoc" class="col-lg-2 col-md-2  control-label">Sidoc</label>
                                        <div class="col-lg-10 col-md-10  ">
                                            <input <?= $liberacao['readonly'] ?>  id="dmdnumsidoc" name="dmdnumsidoc" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['dmdnumsidoc']['value'] ?>">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmddtentdocumento" class="col-lg-2 col-md-2  control-label">Data do documento</label>

                                        <div class="col-lg-3 col-md-3  ">
                                            <input id="dmddtentdocumento" <?= $liberacao['readonly'] ?> name="dmddtentdocumento" type="text" class="form-control" placeholder="dd/mm/aaaa" value="<?php echo $this->entity['dmddtentdocumento']['value'] ?>">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmddtemidocumento" class="col-lg-2 col-md-2  control-label">Data de publicação</label>

                                        <div class="col-lg-3 col-md-3  ">
                                            <input id="dmddtemidocumento"  name="dmddtemidocumento" type="text" class="form-control" placeholder="dd/mm/aaaa" value="<?php echo $this->entity['dmddtemidocumento']['value'] ?>">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmdprazoemdias" class="col-lg-2 col-md-2  control-label">Prazo em dias</label>
                                        <div class="col-lg-3 col-md-3  ">
                                            <input id="dmdprazoemdias" name="dmdprazoemdias" type="number" class="form-control" placeholder="" value="<?php echo $this->entity['dmdprazoemdias']['value'] ?>">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="dmdprazoemdata" class="col-lg-2 col-md-2  control-label">Prazo em data</label>
                                        <div class="col-lg-3 col-md-3">
                                            <input id="dmdprazoemdata" name="dmdprazoemdata" type="text" class="form-control" placeholder="dd/mm/aaaa" value="<?php echo $this->entity['dmdprazoemdata']['value'] ?>">
                                        </div>
                                    </div>
                                    <!--                    </div>-->
                                    <!--                                        <div class="col-md-1"></div>-->
                                </fieldset>
                            </div>
                        </div>

                        <div class="col-md-2 barraWorkflowDocumento" >
                            <?php
                            wf_desenhaBarraNavegacao($this->entity['docid']['value'], array('cooid' => $this->entity['docid']['value']));
                            ?>
                        </div>

                    </form>

                    <div class="clearfix"></div>
                    <!--        --><?php //$modelDemanda->recuperarListagem();    ?>
                </div>
            </div>
            <?PHP
            //if ($estado['esdid'] == ESD_DEMANDA_EM_CADASTRAMENTO || $estado['esdid'] == '') {
            if ($estado['esdid'] == ESD_DEMANDA_EM_CADASTRAMENTO || $estado['esdid'] == '') {
                ?>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    <button id="bt-salvar" type="button" class="btn btn-success">Salvar</button>
                </div>
                <?PHP
            } else if ($estado['esdid'] == ESD_DEMANDA_EM_ATENDIMENTO || $estado['esdid'] == ESD_DEMANDA_EM_DILIGENCIA) {
                ?> <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    <button id="bt-alterar" type="button" class="btn btn-success">Alterar</button>
                </div>
            <?php }
            ?>
        </div>
    </div>


</div>
<script type="text/javascript">

    setTimeout(function() {
//        $('#form_save #tpdid').chosen();
        $('#form_save #prcid_orig').chosen();
        $('#form_save #prcid_dest').chosen();
        $('#form_save #dmddtentdocumento').datepicker();
        $('#form_save #dmddtemidocumento').datepicker();
        $('#form_save #dmdprazoemdata').datepicker();
        $('#form_save #dmdnumdocumento').mask('9999');
        mudarFormulario();
    }, 300);

    var portaria = '<?php echo K_TIPO_DOCUMENTO_PORTARIA ?>';

    $('#form_save #tpdid').change(function() {
        mudarFormulario();
    });

    function checaReiteracao($chk) {
        if ($chk.checked) {
            $("#dmdreiteracao").val('t');
        } else {
            $("#dmdreiteracao").val('f');
        }
    }


    $('#form_save #dmdprazoemdias').change(function() {
        date = $('#form_save #dmddtentdocumento').val();
        day = parseInt($(this).val());

        if (date != '' && day != '') {
            countDate(date, day);
        }
    });

//            var dateTimeForm = '21/12/2013';
//            var day = 4;
    function countDate(dateForm, dayForm)
    {
        var arrDate = dateForm.split('/');

        var dd = parseInt(arrDate[0]);
        var MM = parseInt(arrDate[1]);
        var yyyy = parseInt(arrDate[2]);

        var date = new Date();

        date.setDate(dd);
        date.setMonth(MM - 1);
        date.setFullYear(yyyy);

        //somando
        var dd = dd + parseInt(dayForm);
        date.setDate(dd);

        // Verificando final de semana.
        // Se for sabado adiciona 2 dias se for domingo adiciona 1 dia.
//        var day = date.getDay();
//        if (day > 5) {
//            dd = dd + 2;
//            date.setDate( dd );
//                console.info('sabado');
//        } else if (day < 1) {
//            dd = dd + 1;
//            date.setDate( dd );
//                console.info('domingo');
//        }

        var month = date.getMonth() + 1;

        $('#form_save #dmdprazoemdata').val(date.getDate() + '/' + month + '/' + date.getFullYear());
    }

    function mudarFormulario()
    {
        var value = $('#form_save #tpdid:checked').val();

        if (value == portaria) {
            $('#form_save #prcid_dest').closest('.form-group').hide();
            $('#form_save #dmdreferencia').closest('.form-group').hide();
            $('#form_save #dmdnumsidoc').closest('.form-group').hide();
            $('#form_save #dmdprazoemdias').closest('.form-group').hide();
            $('#form_save #dmdprazoemdata_1').closest('.form-group').hide();

            $('#form_save #dmddb').closest('.form-group').fadeIn();
            $('#form_save #dmddtemidocumento').closest('.form-group').fadeIn();
        } else {
            $('#form_save #prcid_dest').closest('.form-group').fadeIn();
            $('#form_save #dmdreferencia').closest('.form-group').fadeIn();
            $('#form_save #dmdnumsidoc').closest('.form-group').fadeIn();
            $('#form_save #dmdprazoemdias').closest('.form-group').fadeIn();
            $('#form_save #dmdprazoemdata_1').closest('.form-group').fadeIn();

            $('#form_save #dmddb').closest('.form-group').hide();
            $('#form_save #dmddtemidocumento').closest('.form-group').hide();
        }
    }


    $('#bt-salvar').click(function() {
        //chamando function que renderiza somente o workflow atraves da function atualizarWorkflowAction()
        $('#form_save').saveAjax({controller: 'documento', action: 'salvar', clearForm: false, retorno: true, displayErrorsInput: true, functionSucsess: 'recuperaWorkflow'});
    });

    $('#bt-alterar').click(function() {
        //chamando function que renderiza somente o workflow atraves da function atualizarWorkflowAction()
        $('#form_save').saveAjax({controller: 'documento', action: 'alterar', clearForm: false, retorno: true, displayErrorsInput: true, functionSucsess: 'recuperaWorkflow'});
    });

    function recuperaWorkflow(dados) {
        //passando via ajax valores renderizados  para atualizar html da div que está o workflow
        $.post(window.location.href, {controller: 'documento', action: 'atualizarworkflow', docid: dados['docid']}, function(data) {
            $('.barraWorkflowDocumento').html(data);
        });
    }
</script>