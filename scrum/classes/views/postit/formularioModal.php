<div class="modal-dialog-large">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
            <h4 class="modal-title">Entregavel / Demanda</h4>
        </div>
        <div class="modal-body">
            <div class="row">
                <div class="col-lg-6">
                    <div class="well">
                        <form id="form-entregavel" name="form-entregavel" method="post" class="form-horizontal">
                            <!--<input type="hidden" name="action" value="save"/>-->
                            <input type="hidden" name="entid" value="<?php echo $this->entity['entid']['value'] ?>"/>
                            <input type="hidden" name="qtdMaxHoraSprint" value="<?php echo $this->qtdMaxHoraSprint ?>"/>
                            <fieldset>
                                <div class="form-group text-center">
                                    <legend>Entregável</legend>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Data do cadástro</label>
                                    <div class="col-lg-8">
                                        <input name="entdtcad" id="entdtcad" type="text" class="form-control" disabled="disabled" value="<?php echo $this->entity['entdtcad']['value'] ?>">
                                        <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <!--                                <div class="form-group">
                                                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Sprint</label>
                                                                    <div class="col-lg-8">
                                                                        <input name="sptid" id="sptid" type="text" class="form-control" disabled="disabled" value="<?php if ($this->sprint['sptinicio']) echo $this->sprint['sptinicio'] . ' - ' . $this->sprint['sptfim'] ?>">
                                <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                                                    </div>
                                                                    <p class="help-block">Selecione um programa.</p>
                                                                </div>-->
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Solicitante</label>
                                    <div class="col-lg-8">
                                        <input name="usucpfsol" id="usucpfsol" type="text" min="1" max="100" disabled="disabled"  class="form-control" value="<?php echo $this->solicitante['usunome'] ?>">
                                        <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Estado</label>
                                    <div class="col-lg-8">
                                        <select name="entstid" id="entstid" class="chosen-select form-control" disabled="disabled">
                                            <option value=""> Selecione </option>
                                            <?php foreach ($this->status as $status): ?>
                                                <option <?php if ($this->entity['entstid']['value'] == $status['entstid']) echo 'selected="selected"' ?> value="<?php echo $status['entstid'] ?>"><?php echo $status['entstdsc'] ?></option>
                                            <?php endforeach; ?>
                                        </select>
                                        <!--<input name="entstid" id="entstid" type="text" min="1" max="100" disabled="disabled" class="form-control" value="<?php echo $this->entity['entstid']['value'] ?>">-->
                                        <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group has-warning">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Estória</label>
                                    <div class="col-lg-8">
                                        <select name="estid" id="estid" class="chosen-select form-control">
                                            <option value=""> Selecione </option>
                                            <?php foreach ($this->estoria as $estoria): ?>
                                                <option <?php if ($this->entity['estid']['value'] == $estoria['estid']) echo 'selected="selected"' ?> value="<?php echo $estoria['estid'] ?>"><?php echo $estoria['esttitulo'] ?></option>
                                            <?php endforeach; ?>
                                        </select>
                                        <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Responsável</label>
                                    <div class="col-lg-8">
                                        <select name="usucpfresp" id="usucpfresp" class="chosen-select form-control" data-placeholder="Selecione">
                                            <option value=""></option>
                                            <?php foreach ($this->equipe as $responsavel):
                                                if ($this->entity['usucpfresp']['value'] == $responsavel['usucpf']) $programador = $responsavel['usunome'];
                                                ?>
                                                <option <?php if ($this->entity['usucpfresp']['value'] == $responsavel['usucpf']) echo 'selected="selected"' ?> value="<?php echo $responsavel['usucpf'] ?>"><?php echo $responsavel['usunome'] ?></option>
                                            <?php endforeach ?>
                                        </select>
                                        <input name="programador" type="hidden" id="programador" value="<?php echo $programador ?>">
                                        <!--<input name="usucpfresp" id="usucpfresp" type="text" min="1" max="100" class="form-control" value="<?php echo $this->entity['usucpfresp']['value'] ?>">-->
                                        <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group has-warning">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Horas de execução</label>
                                    <div class="col-lg-8">
                                        <input name="enthrsexec" id="enthrsexec" type="number" min="1" max="100" required="required" placeholder="000" class="form-control" value="<?php echo $this->entity['enthrsexec']['value'] ?>">
                                        <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                        <!--<p class="help-block">Quantidade de horas para este entregável.</p>-->
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Descrição</label>
                                    <div class="col-lg-8">
                                        <textarea name="entdsc" id="entdsc" class="form-control" rows="2"><?php echo $this->entity['entdsc']['value'] ?></textarea>
                                        <?php // echo campo_texto('enthrsexec', 'N' , 'S', 'Descrição' , 25, '30', '###' , '' , '', '', '', 'id="enthrsexec" required="required" class="form-control" style="width=100%;"')  ?>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                            </fieldset>
                        </form>
                    </div>
                </div>
                <div class="col-lg-6">
                    <div class="well">
                        <form id="form-demanda" name="form-demanda" method="post" class="form-horizontal">
                            <fieldset>
                                <div class="form-group text-center">
                                    <legend>Demanda<?php if($this->entityDemanda['dmdid']['value']) echo ' - ' . $this->entityDemanda['dmdid']['value']; ?></legend>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Demandante</label>
                                    <div class="col-lg-8">
                                        <input type="text" class="form-control" id="demandante" placeholder="Demandante" value="<?php echo $this->solicitante['usunome'] ?>" disabled="disabled">
                                        <input name="analista" type="hidden" id="analista" value="<?php echo $this->solicitante['usunome'] ?>">
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Responsável</label>
                                    <div class="col-lg-8">
                                        <!--<input name="usunomeexecutor" id="usunomeexecutor" type="text" class="form-control" placeholder="Demandante" value="<?php echo $this->solicitante['usunome'] ?>" disabled="disabled">-->
                                        <select name="usucpfexecutor" id="usucpfexecutor" class="form-control" disabled="disabled">
                                            <option value=""> Selecione </option>
                                            <?php foreach ($this->equipe as $responsavel): ?>
                                                <option <?php if ($this->entity['usucpfresp']['value'] == $responsavel['usucpf']) echo 'selected="selected"' ?> value="<?php echo $responsavel['usucpf'] ?>"><?php echo $responsavel['usunome'] ?></option>
                                            <?php endforeach ?>
                                        </select>
                                        <p class="help-block"></p>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Prioridade da demanda</label>
                                    <div class="col-lg-8">
                                        <select name="priid" id="priid" class="form-control">
                                            <option value=""> Selecione </option>
                                            <?php foreach ($this->prioridades as $prioridade): ?>
                                                <option <?php if ($this->entityDemanda['priid']['value'] == $prioridade['priid']) echo 'selected="selected"' ?> value="<?php echo $prioridade['priid'] ?>"><?php echo $prioridade['pridsc'] ?></option>
                                            <?php endforeach ?>
                                        </select>
                                        <!--<input type="text" class="form-control" id="demandante" placeholder="Demandante" value="<?php echo $this->solicitante['usunome'] ?>" disabled="disabled">-->
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="dmddatainiprevatendimento">Previsão de início do atendimento</label>
                                    <div class="col-lg-4">
                                        <input name="dmddatainiprevatendimento" type="text" class="form-control" id="dmddatainiprevatendimento" placeholder="00/00/0000" maxlength="10" value="<?php echo $this->entityDemanda['dmddatainiprevatendimento']['value'] ?>" data-format="dd/MM/yyyy hh:mm:ss" >
                                        <input name="dmddatainiprevatendimento_old" type="hidden" id="dmddatainiprevatendimento_old" value="<?php echo $this->entityDemanda['dmddatainiprevatendimento']['value'] ?>">
                                    </div>
                                    <div class="col-lg-4">
                                        <input name="hiniatendimento" type="time" class="form-control" id="hiniatendimento" placeholder="00:00" value="<?php echo $this->entityDemanda['hiniatendimento']['value'] ?>" >
                                        <input name="hiniatendimento_old" type="hidden" id="hiniatendimento_old" value="<?php echo $this->entityDemanda['hiniatendimento']['value'] ?>">
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Previsão de término do atendimento</label>
                                    <div class="col-lg-4">
                                        <input name="dmddatafimprevatendimento" type="text" class="form-control" id="dmddatafimprevatendimento" placeholder="00/00/0000" maxlength="10" value="<?php echo $this->entityDemanda['dmddatafimprevatendimento']['value'] ?>" data-format="dd/MM/yyyy hh:mm:ss">
                                        <input name="dmddatafimprevatendimento_old" type="hidden" id="dmddatafimprevatendimento_old" value="<?php echo $this->entityDemanda['dmddatafimprevatendimento']['value'] ?>">
                                    </div>
                                    <div class="col-lg-4">
                                        <input name="hfimatendimento" type="time" class="form-control" id="hfimatendimento" placeholder="00:00" value="<?php echo $this->entityDemanda['hfimatendimento']['value'] ?>" >
                                        <input name="hfimatendimento_old" type="hidden" id="hfimatendimento_old" value="<?php echo $this->entityDemanda['hfimatendimento']['value'] ?>">
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Classificação da demanda</label>
                                    <div class="col-lg-8">
                                        <select name="dmdclassificacao" id="dmdclassificacao" class="form-control">
                                            <option value=""> Selecione </option>
                                            <?php foreach ($this->classificacao as $classificacao): ?>
                                                <option <?php if ($this->entityDemanda['dmdclassificacao']['value'] == $classificacao['codigo']) echo 'selected="selected"' ?> value="<?php echo $classificacao['codigo'] ?>"><?php echo $classificacao['descricao'] ?></option>
                                            <?php endforeach ?>
                                        </select>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-4 control-label" for="Programa">Tipo da demanda para Sistemas de Informação</label>
                                    <div class="col-lg-8">
                                        <select name="dmdclassificacaosistema" id="dmdclassificacaosistema" class="form-control">
                                            <option value=""> Selecione </option>
                                            <?php foreach ($this->tipoDemanda as $tipoDemanda): ?>
                                                <option <?php if ($this->entityDemanda['dmdclassificacaosistema']['value'] == $tipoDemanda['codigo']) echo 'selected="selected"' ?> value="<?php echo $tipoDemanda['codigo'] ?>"><?php echo $tipoDemanda['descricao'] ?></option>
                                            <?php endforeach ?>
                                        </select>
                                    </div>
                                    <!--<p class="help-block">Selecione um programa.</p>-->
                                </div>
                            </fieldset>
                        </form>
                    </div>
                </div>
            </div>
                <div class="row">
                    <div class="col-lg-12">
<!--                    <div class="well">-->
                        <br>
                        <!-- The file upload form used as target for the file upload widget -->
                        <form id="fileupload" action="" method="POST" enctype="multipart/form-data">
                            <input type="hidden" name="controller" value="postit" />
                            <input type="hidden" name="action" value="default" />
                            <!-- Redirect browsers with JavaScript disabled to the origin page -->
                            <noscript><input type="hidden" name="redirect" value="http://blueimp.github.io/jQuery-File-Upload/"></noscript>
                            <!-- The fileupload-buttonbar contains buttons to add/delete files and start/cancel the upload -->
                            <div class="row fileupload-buttonbar">
                                <div class="col-lg-7">
                                    <!-- The fileinput-button span is used to style the file input field as button -->
                <span class="btn btn-success fileinput-button">
                    <i class="glyphicon glyphicon-plus"></i>
                    <span>Adicionar arquivos...</span>
                    <input type="file" name="files[]" multiple>
                </span>
                                    <button type="submit" class="btn btn-primary start">
                                        <i class="glyphicon glyphicon-upload"></i>
                                        <span>Iniciar upload</span>
                                    </button>
                                    <button type="reset" class="btn btn-warning cancel">
                                        <i class="glyphicon glyphicon-ban-circle"></i>
                                        <span>Cancelar upload</span>
                                    </button>
                                    <button type="button" class="btn btn-danger delete">
                                        <i class="glyphicon glyphicon-trash"></i>
                                        <span>Remover</span>
                                    </button>
                                    <input type="checkbox" class="toggle">
                                    <!-- The global file processing state -->
                                    <span class="fileupload-process"></span>
                                </div>
                                <!-- The global progress state -->
                                <div class="col-lg-5 fileupload-progress fade">
                                    <!-- The global progress bar -->
                                    <div class="progress progress-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="100">
                                        <div class="progress-bar progress-bar-success" style="width:0%;"></div>
                                    </div>
                                    <!-- The extended global progress state -->
                                    <div class="progress-extended">&nbsp;</div>
                                </div>
                            </div>
                            <!-- The table listing the files available for upload/download -->
                            <table role="presentation" class="table table-striped"><tbody class="files"></tbody></table>
                        </form>
                        <br>
                    </div>

<!--                </div>-->
                </div>
            <!-- The main application script -->
            <script src="../library/bootstrap-file-upload-9.5.1/js/main.js"></script>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button id="button-save" type="button" class="btn btn-primary">Salvar</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
    <script lang="javascript">

        $('#enthrsexec').mask('999');
        $('#dmddatainiprevatendimento').mask('99/99/9999');
        $('#dmddatafimprevatendimento').mask('99/99/9999');
        $('#hiniatendimento').mask('99:99');
        $('#hfimatendimento').mask('99:99');

        $("#dmddatainiprevatendimento").datepicker({
//            defaultDate: "+1w",
//            changeMonth: true,
            minDate: 0,
            numberOfMonths: 1,
            showWeek: true
//            , onClose: function(selectedDate) {
//                $("#dmddatafimprevatendimento").datepicker("option", "minDate", selectedDate);
//            }
            , 'showAnim':'drop'
        });
        $("#dmddatafimprevatendimento").datepicker({
//            defaultDate: "+1w",
//            changeMonth: true,
            minDate: 0,
            numberOfMonths: 1,
            showWeek: true
            , onClose: function(selectedDate) {
                $("#dmddatainiprevatendimento").datepicker("option", "maxDate", selectedDate);
            }
            , 'showAnim':'drop'
        });

//        $("#dmddatainiprevatendimento").datepicker("option", "showAnim", 'drop');
//        $("#dmddatafimprevatendimento").datepicker("option", "showAnim", 'drop');



        $('#button-save').click(function() {
            data = $('#form-entregavel').serialize() + '&' + $('#form-demanda').serialize() + '&controller=postit&action=salvarFormularioModal';

            if (isWeekend($('#dmddatainiprevatendimento').val()) || isWeekend($('#dmddatafimprevatendimento').val())) {
                if (confirm('A demanda possui data no final de semana, deseja continuar?')) {
                } else {
                    $('#dmddatainiprevatendimento').val('');
                    $('#dmddatafimprevatendimento').val('');
                    return false;
                }
            }

            $.post(window.location.href, data, function(result) {

                if (result['status'] == true) {
                    $('.has-error').removeClass('has-error');
                    var html = '<div class="col-lg-12"><div class="alert alert-dismissable alert-success"><strong>Sucesso! </strong>' + result['msg'] + '<a class="alert-link" href="#"></a></div></div>'
//                    $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
                } else {
//                        var html = '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger"><strong>Erro! </strong>' + result['msg'] + '<a class="alert-link" href="#"></a></div></div>'
                    var html = '';
                    element = '';

                    $('.has-error').removeClass('has-error');
                    $(result['result']).each(function() {
                        element = $('#' + this.name);
                        label = element.parent().parent('.form-group').children('label').text();
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> ' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
                        element.parent().parent('.form-group').addClass('has-error');
                    });



                }

                $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);

//                element.focus();

            }, 'json');


//            $.post( window.location.href, {'controller': 'postit' , 'action' : 'formularioModal', 'entid' : $(this).attr('sprintPostitId')},  function(html) {
//                $('#modal').html(html).modal('show');
//            });
        });

        $('#usucpfresp').change(function() {
            element = $(this);
            $('form #usucpfexecutor').val(element.val());

            $.post(window.location.href, {controller: 'postit', action: 'ultimaDemanda', cpf: $(this).val()}, function(result) {
//                console.info(result);
//                alert(result.dmddatafimprevatendimento);
//                console.info(element);
//                element.parent().children('.help-block').html(result.dmddatafimprevatendimento);
//                $('#usucpfexecutor').next().text(result.dmddatafimprevatendimento)
                if (result.dmddatafimprevatendimento) {
                    $('#usucpfexecutor').next().html('Data e hora de finalização da ultima demanda: ' + result.dmddatafimprevatendimento);
//                    $('#dataUltimaDemanda').val(result.dmddatafimprevatendimento);

                    var arrDateTimeForm = result.dmddatafimprevatendimento.split(' ');

                    var dateForm = arrDateTimeForm[0];
                    var timeForm = arrDateTimeForm[1];

                    $('#dmddatainiprevatendimento').val(dateForm);
                    $('#hiniatendimento').val(timeForm);

                    horaExecucao = $('#enthrsexec').val();
                    if(horaExecucao != ''){
                        countDateTime(result.dmddatafimprevatendimento, horaExecucao)
                    }

                }
//                console.info(result);
            }, 'json');

//            if(!element.val()){
//                $('#usucpfexecutor').next().text(result.dmddatafimprevatendimento);
//            }
        });


        $('#enthrsexec').change(function(){
            horaExecucao = $(this).val();

            var dateForm = $('#dmddatainiprevatendimento').val();
            var timeForm = $('#hiniatendimento').val();

            if( dateForm != '' && timeForm != ''){
                dataUltimaDemanda = dateForm + ' ' + timeForm + ':' + 00;
                console.info(dataUltimaDemanda);
                countDateTime(dataUltimaDemanda, horaExecucao)
            }

        });

        function countDateTime(dateTimeForm, hJob)
        {
//            var dateTimeForm = '21/12/2013 11:00:00';
//            var hJob = 4;
            var arrDateTimeForm = dateTimeForm.split(' ');

            var dateForm = arrDateTimeForm[0];
            var timeForm = arrDateTimeForm[1];

            var arrDate = dateForm.split('/');
            var arrTime = timeForm.split(':');

            var dd = parseInt(arrDate[0]);
            var MM = parseInt(arrDate[1]);
            var yyyy = parseInt(arrDate[2]);

            var hh = parseInt(arrTime[0]);
            var mm = parseInt(arrTime[1]);
            var ss = parseInt(arrTime[2]);

            var date = new Date();

            // Definindo horario de espediente.
            hIni = 9;
            hMidIni = 12;
            hMidEnd = 14;
            hEnd = 18;

            // Somando
            while (hJob > 0) {
                if (hh >= hIni && hh < hMidIni) {
                    hh = hh + 1;
                } else if (hh >= hMidIni && hh <= hMidEnd) {
                    hh = hMidEnd + 1;
                } else if (hh > hMidEnd && hh < hEnd) {
                    hh = hh + 1;
                } else if (hh >= hEnd) {
                    dd = dd + 1;
                    hh = hIni + 1;
                }

                hJob = hJob - 1;
            }

            //console.info('Horas somadas da ultima demanda: ' + dd);

            date.setDate(dd);
            date.setMonth(MM - 1);
            date.setFullYear(yyyy);
            date.setHours(hh);
            date.setMinutes(mm);
            date.setSeconds(ss);

            // Verificando final de semana.
            // Se for sabado adiciona 2 dias se for domingo adiciona 1 dia.
            day = date.getDay();
            if (day > 5) {
                dd = dd + 2;
                date.setDate(dd);
//                console.info('sabado');
            } else if (day < 1) {
                dd = dd + 1;
                date.setDate(dd);
//                console.info('domingo');
            }


            var month = date.getMonth() +1;

//            $('dmddatainiprevatendimento').val(date.toLocaleDateString());
            $('#dmddatafimprevatendimento').val(date.getDate() + '/' + month  + '/' + date.getFullYear());
            $('#hfimatendimento').val(date.toLocaleTimeString());
//            console.info($('#dmddatafimprevatendimento'));
//            console.info(date.getDay());
//            console.info(date.getDay());
//            console.info(date.toLocaleDateString());
//            console.info(date.toLocaleTimeString());
//            console.info(date.toLocaleString());
        }
        
        function isWeekend(data){
            var data = data.split("/");
            var nDate = new Date(data[2]+'-'+data[1]+'-'+data[0]+' 00:00:00');
            if (nDate.getDay() === 6 || nDate.getDay() === 0) {// Sabado ou domingo
                return true;
            }
            
            return false;
        }

        setTimeout(function(){
            for (var selector in config) {
                $(selector).chosen(config[selector]);
            }
        },500);
    </script>