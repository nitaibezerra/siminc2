<div class="modal-dialog-large">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
            <h4 class="modal-title">Salvar dirigente  - <?php echo $this->tpddsc; ?></h4>
        </div>
        <form class="form-horizontal">
            <input name="controller" type="hidden" value="dirigente">
            <input name="action" type="hidden" value="salvar">
            <input name="drgid" type="hidden" value="<?php echo $this->entity['drgid']['value'] ?>">
            <input name="tpdid" type="hidden" value="<?php echo $this->tpdid ?>">
            <div class="modal-body">
                <div class="row">
                    <div class="col-md-6">
                        <div class="well">
                            <fieldset>
                                <legend>Dirigente</legend>
                                <div class="form-group has-warning">
                                    <label for="drgcpf" class="col-lg-2 control-label">CPF</label>

                                    <div class="col-lg-10">
                                        <input id="drgcpf" name="drgcpf" type="text" class="form-control"
                                               placeholder="" maxlength="14"
                                               value="<?php echo $this->entity['drgcpf']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="drgnome" class="col-lg-2 control-label">Nome</label>

                                    <div class="col-lg-10">
                                        <input id="drgnome" name="drgnome" type="text"
                                               class="form-control" placeholder="" required="required"
                                               value="<?php echo $this->entity['drgnome']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="drgfuncao" class="col-lg-2 control-label">Função</label>

                                    <div class="col-lg-10">
                                        <input id="drgfuncao" name="drgfuncao" type="text" class="form-control"
                                               placeholder=""
                                               required="required"
                                               value="<?php echo $this->entity['drgfuncao']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="drgemail" class="col-lg-2 control-label">Email</label>

                                    <div class="col-lg-10">
                                        <input id="drgemail" name="drgemail" type="text" class="form-control"
                                               placeholder=""
                                               required="required"
                                               value="<?php echo $this->entity['drgemail']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="drgfonecomercial" class="col-lg-2 control-label">Telefone
                                        comercial</label>

                                    <div class="col-lg-10">
                                        <input id="drgfonecomercial" name="drgfonecomercial" type="text"
                                               class="form-control" placeholder="" required="required"
                                               value="<?php echo $this->entity['drgfonecomercial']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="drgfonefax" class="col-lg-2 control-label">Fax</label>

                                    <div class="col-lg-10">
                                        <input id="drgfonefax" name="drgfonefax" type="text" class="form-control"
                                               placeholder=""
                                               value="<?php echo $this->entity['drgfonefax']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="drgfonecelular" class="col-lg-2 control-label">Celular</label>

                                    <div class="col-lg-10">
                                        <input id="drgfonecelular" name="drgfonecelular" type="text"
                                               class="form-control"
                                               placeholder=""
                                               value="<?php echo $this->entity['drgfonecelular']['value'] ?>">
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="well">
                            <fieldset>
                                <legend>Endereço</legend>
                                <div class="form-group has-warning">
                                    <label for="endcep1" class="col-lg-2 control-label">CEP</label>

                                    <div class="col-lg-10">
                                        <input id="endcep1" name="endcep1" type="text" class="form-control" placeholder=""
                                               value="<?php echo $this->entity['drgcep']['value'] ?>">
                                    </div>
                                </div>
                                <?php
                                $controllerGeral = new Controller_Geral();
                                $controllerGeral->setUfName('estuf1');
                                $controllerGeral->setUfValue($this->entity['estuflogradouro']['value']);
                                $controllerGeral->setChosen(false);
                                $controllerGeral->setMunicipioName('muncod1');
                                $controllerGeral->setMunicipioValue($this->entity['muncodlogradouro']['value']);

                                $controllerGeral->ufAction();
                                $controllerGeral->municipioAction();
                                ?>
                                <div class="form-group">
                                    <label for="drglogradouro" class="col-lg-2 control-label">Logradouro</label>

                                    <div class="col-lg-10">
                                        <input id="drglogradouro" name="drglogradouro" type="text" class="form-control"
                                               placeholder=""
                                               value="<?php echo $this->entity['drglogradouro']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="drgcompllogradouro" class="col-lg-2 control-label">Complemento</label>

                                    <div class="col-lg-10">
                                        <input id="drgcompllogradouro" name="drgcompllogradouro" type="text"
                                               class="form-control" placeholder=""
                                               value="<?php echo $this->entity['drgcompllogradouro']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="endbai1" class="col-lg-2 control-label">Bairro</label>

                                    <div class="col-lg-10">
                                        <input id="endbai1" name="endbai1" type="text"
                                               class="form-control" placeholder=""
                                               value="<?php echo $this->entity['drgbairrologradouro']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="drgnumlogradouro" class="col-lg-2 control-label">Numero</label>

                                    <div class="col-lg-10">
                                        <input id="drgnumlogradouro" name="drgnumlogradouro" type="text"
                                               class="form-control" placeholder=""
                                               value="<?php echo $this->entity['drgnumlogradouro']['value'] ?>">
                                    </div>
                                </div>
                                <?php
                                $latitude = explode('.',$this->entity['drglatitude']['value']);
                                $longitude = explode('.',$this->entity['drglongitude']['value']);
                                ?>
                                <div class="form-group">
                                    <label for="intlatitude" class="col-lg-2 control-label">Latitude</label>
                                    <div class="col-lg-10">
                                        <input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden">
                            <span id="_graulatitude1">
                                <?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?>
                            </span>
                                        º
                                        <input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden">
                                        <span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>
                                        ' <input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden">
                            <span id="_seglatitude1">
                                <?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?>
                            </span>
                                        " <input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
                            <span id="_pololatitude1">
                                <?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?>
                            </span>

                                        <!--                            <input id="intlatitude" name="intlatitude" type="text" class="form-control" placeholder="" value="--><?php //echo $this->entity['intlatitude']['value'] ?><!--">-->
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="intlongitude" class="col-lg-2 control-label">Longitude</label>
                                    <div class="col-lg-10">
                                        <input name="longitude[]" id="graulongitude1" maxlength="2"
                                               size="3" value="<? echo $longitude[0]; ?>" type="hidden">
                                        <span
                                            id="_graulongitude1">
                                        <?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?>
                                        </span>
                                                    º <input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden">
                                        <span id="_minlongitude1">
                                            <?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?>
                                        </span>
                                                    ' <input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden">
                                        <span id="_seglongitude1">
                                            <?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?>
                                        </span>
                                                    " <input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden">
                                        <span id="_pololongitude1">
                                            <?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?>
                                        </span>
                                                    <input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
                                        <!--                            <input id="intlongitude" name="intlongitude" type="text" class="form-control" placeholder="" value="--><?php //echo $this->entity['intlongitude']['value'] ?><!--">-->
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-md-10 col-md-offset-2">
<!--                                        <a href="#" onclick="abreMapaEntidade('1');"> Visualizar / Buscar No Mapa</a>-->
                                        <button onclick="abreMapaEntidade('1');" type="button" class="btn btn-primary">Visualizar / Definir local</button>
                                        <input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    <button id="bt-salvar" type="button" class="btn btn-success">Salvar</button>
                </div>
            </div>
        </form>
    </div>
</div>
<script language="javascript">

    setTimeout(function () {
        $('#estuf1').chosen({no_results_text: "Sem resultado!", allow_single_deselect: true});
        $('#muncod1').chosen({no_results_text: "Sem resultado!", allow_single_deselect: true});
    }, 350);

    $('#drgcpf').mask('999.999.999-99');
    $('#drgfonecomercial').mask('(99)9999-9999');
    $('#drgfonefax').mask('(99)9999-9999');
    $('#drgfonecelular').mask('(99)9999-9999');
    $('#endcep1').mask('99999-999');


    $('#drgcpf').focusout(function(){
        element = $(this);

        console.info(element.val());
//        if(element.val().length >13 ){
            var data = {controller: 'dirigente', action: 'carregarDadosDirigentePorCpf' , cpf:element.val()};
            $.post(window.location.href, data, function(result){
//                result = get_json(result);
                $('#drgnome').val(result.drgnome);
                $('#drgemail').val(result.drgemail);
                $('#drgfuncao').val(result.drgfuncao);
                $('#drgfonecomercial').val(result.drgfonecomercial);
                $('#drgfonefax').val(result.drgfonefax);
                $('#drgfonecelular').val(result.drgfonecelular);
                $('#endcep1').val(result.drgcep);
                $('#drglogradouro').val(result.drglogradouro);
                $('#drgcompllogradouro').val(result.drgcompllogradouro);
                $('#drgbairrologradouro').val(result.drgbairrologradouro);
                $('#drgnumlogradouro').val(result.drgnumlogradouro);

                endcep = result.drgcep;
                tipoendereco = '1';

                if(endcep){
                    $.post('/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP&endcep=' + endcep, function(resp){
                        var dados = resp.split("||");
                        $('#drglogradouro').val(dados[0]);
                        $('#endbai1').val(dados[1]);
                        $('#drgcompllogradouro').val(dados[2]);

                        $('#estuf1').val(dados[3]);
                        $('#muncod1').val(dados[4]);


                        element = $('#estuf1');
                        $.post(window.location.href , {controller: 'geral' , action: 'uf' , id: dados[3] , name:'estuf1'} , function(html){
                            element.closest('form').find('.container-select-uf').replaceWith(function(){
                                return $(html).hide().fadeIn();
                            });
                        });

                        element = $('#muncod1');
                        $.post(window.location.href , {controller: 'geral' , action: 'municipio' , estuf: dados[3], id: dados[4] , name: 'muncod1'} , function(html){
                            element.closest('form').find('.container-select-municipio').replaceWith(function(){
                                return $(html).hide().fadeIn();
                            });
                        });
                    });
                }



            }, "json");
//        }
    });

    $('#endcep1').change(function(){

        endcep = $(this).val();
        tipoendereco = '1';

        $.post('/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP&endcep=' + endcep, function(resp){
            var dados = resp.split("||");
            $('#drglogradouro').val(dados[0]);
            $('#endbai1').val(dados[1]);
            $('#drgcompllogradouro').val(dados[2]);

            $('#estuf1').val(dados[3]);
            $('#muncod1').val(dados[4]);

            element = $('#estuf1');
            $.post(window.location.href , {controller: 'geral' , action: 'uf' , id: dados[3] , name:'estuf1'} , function(html){
                element.closest('form').find('.container-select-uf').replaceWith(function(){
                    return $(html).hide().fadeIn();
                });
            });

            element = $('#muncod1');
            $.post(window.location.href , {controller: 'geral' , action: 'municipio' , estuf: dados[3], id: dados[4] , name: 'muncod1'} , function(html){
                element.closest('form').find('.container-select-municipio').replaceWith(function(){
                    return $(html).hide().fadeIn();
                });
            });
        });
    });

    function abreMapaEntidade(tipoendereco){
        var graulatitude = window.document.getElementById("graulatitude"+tipoendereco).value;
        var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
        var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
        var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;
        var pololongitude = window.document.getElementById("pololongitude"+tipoendereco).value;


//        if(!pololatitude) pololatitude = 0;
//        if(!pololongitude) pololongitude = 0;

        var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
        var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
        var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;

        var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
        var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
//        var entid = window.document.getElementById("entid").value;
        var entid = 0;
        var janela=window.open('../apigoogle/php/mapa_padraon.php?tipoendereco='+tipoendereco+'&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&poloLong='+pololongitude, 'mapa','height=650,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

    }

    $('#bt-salvar').click(function () {
//        $(this).closest('form').ajaxSubmit(function(){
        console.info($(this).closest('form'))
//            {target: $('#listar-pesquisa').hide().fadeIn()}
//        });

        $(this).closest('form').saveAjax({clearForm: true, functionSucsess: 'fecharModal'});
        return false;
    });

</script>