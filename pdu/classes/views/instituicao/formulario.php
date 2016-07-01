<?php
    $orgãos = array( 'S' => 'Educação Superior' , 'P' => 'Educação Profissional' );
?>
<div class="col-lg-12">
<div class="page-header">
    <h3 id="forms">
<!--        Dados da universidade --->
        Salvar Dados da Instituição
        <small>
<!--            Salvar dados da universidade-->
        </small>
    </h3>
</div>

<form id="form-save" class="form-horizontal">
    <input name="controller" type="hidden" value="instituicao" >
    <input name="action" type="hidden" value="salvar" >
    <input name="intid" type="hidden" value="<?php echo $this->entity['intid']['value'] ?>" >
    <div class="row">
        <div class="col-md-6">
            <div class="well">
                <fieldset>
                    <legend>Instituição</legend>
                    <div class="form-group">
                        <label for="intorgao" class="col-lg-2 control-label">Orgão</label>
                        <div class="col-lg-10">
                            <select disabled id="intorgao" name="intorgao" class="form-control" data-placeholder="Selecione..." >
                                <option value="S">Educação Superior</option>
        <!--                        <option value="">Selecione...</option>-->
        <!--                        --><?php //foreach($orgãos as $idOrgao => $orgao): ?>
        <!--                            <option --><?php //if( $idOrgao == $this->entity['intorgao']['value']) echo 'selected'; ?><!-- value="--><?php //echo $idOrgao; ?><!--">--><?php //echo $orgao?><!--</option>-->
        <!--                        --><?php //endforeach; ?>
                            </select>
                        </div>
                    </div>
                    <div class="form-group has-warning">
                        <label for="intcnpj" class="col-lg-2 control-label">CNPJ</label>
                        <div class="col-lg-10">
                            <input id="intcnpj" name="intcnpj" type="text" class="form-control" placeholder="" required="required"  value="<?php echo $this->entity['intcnpj']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group has-warning">
                        <label for="intdscrazaosocial" class="col-lg-2 control-label">Nome da instituição</label>
                        <div class="col-lg-10">
                            <input id="intdscrazaosocial" name="intdscrazaosocial" type="text" class="form-control" placeholder="" required="required" value="<?php echo $this->entity['intdscrazaosocial']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="intdscsigla" class="col-lg-2 control-label">Sigla da instituição</label>
                        <div class="col-lg-10">
                            <input id="intdscsigla" name="intdscsigla" type="text" class="form-control" placeholder=""  value="<?php echo $this->entity['intdscsigla']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="intcodunidade" class="col-lg-2 control-label">Unidade orçamentária</label>
                        <div class="col-lg-10">
                            <input id="intcodunidade" name="intcodunidade" type="text" class="form-control" placeholder="" required="required"  value="<?php echo $this->entity['intcodunidade']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group has-warning">
                        <label for="intemail" class="col-lg-2 control-label">Email</label>
                        <div class="col-lg-10">
                            <input id="intemail" name="intemail" type="text" class="form-control" placeholder="" required="required"  value="<?php echo $this->entity['intemail']['value'] ?>">
                        </div>
                    </div><div class="form-group">
                        <label for="intsiteinstitucional" class="col-lg-2 control-label">Site da instituição</label>
                        <div class="col-lg-10">
                            <input id="intsiteinstitucional" name="intsiteinstitucional" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['intsiteinstitucional']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group has-warning">
                        <label for="intfonecomercial" class="col-lg-2 control-label">Telefone comercial</label>
                        <div class="col-lg-10">
                            <input id="intfonecomercial" name="intfonecomercial" type="text" class="form-control" placeholder=""  required="required"  value="<?php echo $this->entity['intfonecomercial']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="intfonefax" class="col-lg-2 control-label">Fax</label>
                        <div class="col-lg-10">
                            <input id="intfonefax" name="intfonefax" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['intfonefax']['value'] ?>">
                        </div>
                    </div>
        <!--            <div class="form-group">-->
        <!--                <label for="intvincentid" class="col-lg-2 control-label">Vinculação</label>-->
        <!--                <div class="col-lg-10">-->
        <!--                    <input id="intvincentid" name="intvincentid" type="text" class="form-control" placeholder="" value="--><?php //echo $this->entity['intvincentid']['value'] ?><!--">-->
        <!--                </div>-->
        <!--            </div>-->
        <!--            <div class="form-group">-->
        <!--                <label for="intstatus" class="col-lg-2 control-label">Status</label>-->
        <!--                <div class="col-lg-10">-->
        <!--                    <input id="intstatus" name="intstatus" type="text" class="form-control" placeholder="" value="--><?php //echo $this->entity['intstatus']['value'] ?><!--">-->
        <!--                </div>-->
        <!--            </div>-->
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
                            <input id="endcep1" name="endcep1" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['intcep']['value'] ?>">
                        </div>
                    </div>
                    <?php
                    $controllerGeral = new Controller_Geral();
                    $controllerGeral->setUfName('estuf1');
                    $controllerGeral->setUfValue($this->entity['estuflogradouro']['value']);
                    $controllerGeral->setMunicipioName('muncod1');
                    $controllerGeral->setMunicipioValue($this->entity['muncodlogradouro']['value']);

                    $controllerGeral->ufAction();
                    $controllerGeral->municipioAction();
                    ?>
                    <div class="form-group">
                        <label for="intlogradouro" class="col-lg-2 control-label">Logradouro</label>
                        <div class="col-lg-10">
                            <input id="intlogradouro" name="intlogradouro" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['intlogradouro']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="intbairrologradouro" class="col-lg-2 control-label">Bairro</label>
                        <div class="col-lg-10">
                            <input id="endbai1" name="endbai1" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['intbairrologradouro']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="intcompllogradouro" class="col-lg-2 control-label">Complemento</label>
                        <div class="col-lg-10">
                            <input id="intcompllogradouro" name="intcompllogradouro" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['intcompllogradouro']['value'] ?>">
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="intnumlogradouro" class="col-lg-2 control-label">Numero</label>
                        <div class="col-lg-10">
                            <input id="intnumlogradouro" name="intnumlogradouro" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['intnumlogradouro']['value'] ?>">
                        </div>
                    </div>
                    <?php
                        $latitude = explode('.',$this->entity['intlatitude']['value']);
                        $longitude = explode('.',$this->entity['intlongitude']['value']);
                    ?>
                    <div class="form-group">
                        <label for="intlatitude" class="col-lg-2 control-label">Latitude</label>
                        <div class="col-lg-10">
                            <input id="latitude"  type="hidden" >
                            <input  name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden">
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
                            <input id="longitude"  type="hidden" >
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
<!--                            <a href="#" onclick="abreMapaEntidade('1');"> Visualizar / Buscar No Mapa</a>-->
<!--                            <button onclick="abreMapaEntidade('1');" type="button" class="btn btn-warning">Definir local pelo mapa</button>-->
                            <button onclick="abreMapaEntidade('1');" type="button" class="btn btn-primary">Visualizar / Definir local</button>
                            <input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
                        </div>
                    </div>
                </fieldset>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <div class="well">
                <fieldset>
                    <legend>Caracterização da Instituição</legend>
                    <div class="form-group has-warning">
                        <label style="display: none;" for="intdsccaracteristica" class="col-lg-2 control-label">Caracterização da Instituição</label>
                        <div class="col-lg-12">
                            <textarea id="intdsccaracteristica" name="intdsccaracteristica" class="form-control" rows="3" required="required"><?php echo $this->entity['intdsccaracteristica']['value'] ?></textarea>
                                <span class="help-block">Texto sucinto destacando informações
                                importantes sobre a unidade.
                                Incluir informações sobre sua data de criação
                                e sua importância para a região, dentre outras</span>
                        </div>
                    </div>
                </fieldset>
            </div>
        </div>
    </div>
</form>

<div class="row">
    <div class="col-md-12">
        <div class="well">
            <fieldset>
                <legend>Fotos</legend>
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
                <script src="../library/bootstrap-file-upload-9.5.1/js/main.js"></script>
            </fieldset>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12 text-center">
        <div class="form-group">
                <button id="bt-salvar" type="submit" class="btn btn-success">Salvar</button>
            </div>
        </div>
    </div>
</div>
<script language="javascript">

    $('#intcnpj').mask('99.999.999/9999-99');
    $('#intfonecomercial').mask('(99)9999-9999');
    $('#endcep1').mask('99999-999');

    $('#endcep1').change(function(){

        endcep = $(this).val();
        tipoendereco = '1';

        $.post('/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP&endcep=' + endcep, function(resp){
            var dados = resp.split("||");
            $('#intlogradouro').val(dados[0]);
            $('#endbai1').val(dados[1]);
            $('#intcompllogradouro').val(dados[2]);

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


    $('#bt-salvar').click(function(){
//        $(this).closest('form').ajaxSubmit(function(){
            console.info($(this))
//            {target: $('#listar-pesquisa').hide().fadeIn()}
//        });

//        $(this).closest('form').saveAjax({clearForm: false});
        $('#form-save').saveAjax({clearForm: false});
        return false;
    });
</script>