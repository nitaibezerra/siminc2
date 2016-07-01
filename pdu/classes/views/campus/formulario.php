<?php
$tipos = array('C' => 'Campus', 'R' => 'Reitoria');
?>
<div class="modal-dialog-large">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
            <h3 class="modal-title">
                Edição do Campus
                <?php echo $this->entity['cmpdscrazaosocial']['value'] ?>
                <small>na Instituição <?php echo $_SESSION['instituicao']['intdscrazaosocial'] ?></small>
            </h3>
<!--            <h4 class="modal-title"></h4>-->
        </div>
        <div class="modal-body">
        <form id="form-save" class="form-horizontal">
            <div class="row">
            <div class="col-lg-6">
                <!--<div class="page-header">-->
                <!--    <h3 id="forms">-->
                <!--                Dados da universidade --->
                <!--        <small>-->
                <!--            Salvar dados da universidade-->
                <!--        </small>-->
                <!--    </h3>-->
                <!--</div>-->
                <div class="well">
                        <input name="controller" type="hidden" value="campus">
                        <input name="action" type="hidden" value="salvar">
                        <input name="cmpid" type="hidden" value="<?php echo $this->entity['cmpid']['value'] ?>">
                        <fieldset>
                            <legend>Campus</legend>
                            <div class="form-group">
                                <label for="cmptipo" class="col-lg-2 control-label">Tipo</label>
                                <div class="col-lg-10">
                                    <div class="btn-group" data-toggle="buttons">
                                      <label class="btn btn-default <?php if($this->entity['cmptipo']['value'] == 'C') echo 'active' ?>">
                                        <input id="cmptipo" name="cmptipo" type="radio" value="C" <?php if($this->entity['cmptipo']['value'] == 'C') echo 'checked="checked"' ?>> Campus
                                      </label>
                                      <label class="btn btn-default <?php if($this->entity['cmptipo']['value'] == 'R') echo 'active' ?>">
                                        <input id="cmptipo" name="cmptipo" type="radio" value="R" <?php if($this->entity['cmptipo']['value'] == 'R') echo 'checked="checked"' ?>> Reitoria
                                      </label>
                                    </div>
<!--                                    <select id="cmptipo" name="cmptipo" class="form-control" data-placeholder="Selecione..." >-->
<!--                                        <option value="">Selecione...</option>-->
<!--                                        --><?php //foreach($tipos as $idTipo => $tipo): ?>
<!--                                            <option --><?php //if( $idTipo == $this->entity['cmptipo']['value'] ) echo 'selected'; ?><!-- value="--><?php //echo $idTipo; ?><!--">--><?php //echo $tipo?><!--</option>-->
<!--                                        --><?php //endforeach; ?>
<!--                                    </select>-->
                                </div>
                            </div>
                            <div class="form-group has-warning">
                                <label for="cmpcnpj" class="col-lg-2 control-label">CNPJ</label>
                                <div class="col-lg-10">
                                    <input id="cmpcnpj" name="cmpcnpj" type="text" class="form-control" placeholder=""
                                           required="required" value="<?php echo $this->entity['cmpcnpj']['value'] ?>">
                                </div>
                            </div>
                            <div class="form-group has-warning">
                                <label for="cmpdscrazaosocial" class="col-lg-2 control-label">Nome do campus</label>

                                <div class="col-lg-10">
                                    <input id="cmpdscrazaosocial" name="cmpdscrazaosocial" type="text"
                                           class="form-control" placeholder="" required="required"
                                           value="<?php echo $this->entity['cmpdscrazaosocial']['value'] ?>">
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="cmpdscsigla" class="col-lg-2 control-label">Sigla da instituição</label>

                                <div class="col-lg-10">
                                    <input id="cmpdscsigla" name="cmpdscsigla" type="text" class="form-control"
                                           placeholder="" maxlength="20"
                                           value="<?php echo $this->entity['cmpdscsigla']['value'] ?>">
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="cmpcodunidade" class="col-lg-2 control-label">Unidade orçamentária</label>

                                <div class="col-lg-10">
                                    <input id="cmpcodunidade" name="cmpcodunidade" type="text" class="form-control"
                                           placeholder="" required="required"
                                           value="<?php echo $this->entity['cmpcodunidade']['value'] ?>">
                                </div>
                            </div>
                            <div class="form-group has-warning">
                                <label for="cmpfonecomercial" class="col-lg-2 control-label">Telefone comercial</label>

                                <div class="col-lg-10">
                                    <input id="cmpfonecomercial" name="cmpfonecomercial" type="text"
                                           class="form-control" placeholder="" required="required"
                                           value="<?php echo $this->entity['cmpfonecomercial']['value'] ?>">
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="intfonefax" class="col-lg-2 control-label">Fax</label>

                                <div class="col-lg-10">
                                    <input id="cmpfonefax" name="intfonefax" type="text" class="form-control"
                                           placeholder="" value="<?php echo $this->entity['cmpfonefax']['value'] ?>">
                                </div>
                            </div>
                            <div class="form-group has-warning">
                                <label for="cmpemail" class="col-lg-2 control-label">Email</label>

                                <div class="col-lg-10">
                                    <input id="cmpemail" name="cmpemail" type="text" class="form-control" placeholder=""
                                           required="required" value="<?php echo $this->entity['cmpemail']['value'] ?>">
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="cmpsiteinstitucional" class="col-lg-2 control-label">Site do campus</label>

                                <div class="col-lg-10">
                                    <input id="cmpsiteinstitucional" name="cmpsiteinstitucional" type="text"
                                           class="form-control" placeholder=""
                                           value="<?php echo $this->entity['cmpsiteinstitucional']['value'] ?>">
                                </div>
                            </div>
                        </fieldset>
                </div>
            </div>
            <div class="row">
                <div class="col-md-6">
                    <div class="well">
                    <fieldset>
                        <legend>Endereço</legend>
                        <div class="form-group has-warning">
                            <label for="endcep1" class="col-lg-2 control-label">CEP</label>

                            <div class="col-lg-10">
                                <input id="endcep1" name="endcep1" type="text" class="form-control" placeholder=""
                                       value="<?php echo $this->entity['cmpcep']['value'] ?>">
                            </div>
                        </div>
                        <?php
                        $_POST['id'] = false;
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
                            <label for="cmplogradouro" class="col-lg-2 control-label">Logradouro</label>

                            <div class="col-lg-10">
                                <input id="cmplogradouro" name="cmplogradouro" type="text" class="form-control"
                                       placeholder="" value="<?php echo $this->entity['cmplogradouro']['value'] ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="cmpcompllogradouro" class="col-lg-2 control-label">Complemento</label>

                            <div class="col-lg-10">
                                <input id="cmpcompllogradouro" name="cmpcompllogradouro" type="text"
                                       class="form-control" placeholder=""
                                       value="<?php echo $this->entity['cmpcompllogradouro']['value'] ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="endbai1" class="col-lg-2 control-label">Bairro</label>

                            <div class="col-lg-10">
                                <input id="endbai1" name="endbai1" type="text"
                                       class="form-control" placeholder=""
                                       value="<?php echo $this->entity['cmpbairrologradouro']['value'] ?>">
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="cmpnumlogradouro" class="col-lg-2 control-label">Numero</label>

                            <div class="col-lg-10">
                                <input id="cmpnumlogradouro" name="cmpnumlogradouro" type="text"
                                       class="form-control" placeholder=""
                                       value="<?php echo $this->entity['cmpnumlogradouro']['value'] ?>">
                            </div>
                        </div>
                        <?php
                        $latitude = explode('.',$this->entity['cmplatitude']['value']);
                        $longitude = explode('.',$this->entity['cmplongitude']['value']);
                        ?>
                        <div class="form-group">
                            <input id="latitude"  type="hidden" >
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
<!--                                <a href="#" onclick="abreMapaEntidade('1');"> Visualizar / Buscar No Mapa</a>-->
                                <button onclick="abreMapaEntidade('1');" type="button" class="btn btn-primary">Visualizar / Definir local</button>
                                <input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
                            </div>
                        </div>
                    </fieldset>
                    </div>
                </div>
            </div>
                <div class="row">
                    <div class="col-md-6">
                        <div class="well">
                            <fieldset>
                                <legend>
                                    Dados específicos
                                </legend>
                                <div class="form-group">
                                    <label for="cmpdtcriacao" class="col-lg-4 control-label">Data de criação</label>
                                    <div class="col-lg-8">
                                        <input id="cmpdtcriacao" name="cmpdtcriacao" type="text"
                                               class="form-control" placeholder="mm/aaaa"
                                               value="<?php echo $this->entity['cmpdtcriacao']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmpinicioatv" class="col-lg-4 control-label">Data de início das atividades</label>
                                    <div class="col-lg-8">
                                        <input id="cmpinicioatv" name="cmpinicioatv" type="text"
                                               class="form-control" placeholder="mm/aaaa"
                                               value="<?php echo $this->entity['cmpinicioatv']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmpsitinauguracao" class="col-lg-4 control-label">Inauguração</label>
                                    <div class="col-lg-8">
                                        <div class="btn-group" data-toggle="buttons">
                                          <label class="btn btn-default <?php if($this->entity['cmpsitinauguracao']['value'] == 't') echo 'active' ?>">
                                            <input id="cmpsitinauguracao" name="cmpsitinauguracao" type="radio" value="t"
                                              <?php if($this->entity['cmpsitinauguracao']['value'] == 't') echo 'checked="checked"' ?> > Inaugurado
                                          </label>
                                          <label class="btn btn-default <?php if($this->entity['cmpsitinauguracao']['value'] == 'f') echo 'active' ?>">
                                            <input id="cmpsitinauguracao" name="cmpsitinauguracao" type="radio" value="f"
                                                <?php if($this->entity['cmpsitinauguracao']['value'] == 'f') echo 'checked="checked"' ?> > Não Inaugurado
                                          </label>
                                        </div>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmpdtinauguracao" class="col-lg-4 control-label">Data de inauguração</label>
                                    <div class="col-lg-8">
                                        <input id="cmpdtinauguracao" name="cmpdtinauguracao" type="text"
                                                                                           class="form-control" placeholder="dd/mm/aaaa"
                                                                                           value="<?php echo $this->entity['cmpdtinauguracao']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="tecid" class="col-lg-4 control-label">Existência do Campus/Uned</label>
                                    <div class="col-lg-8">
                                        <select id="tecid" name="tecid" class="form-control" data-placeholder="Selecione...">
                                            <option value=""></option>
                                            <?php foreach($this->tiposExistencia as $tiposExistencia): ?>
                                                <option <?php if($this->entity['tecid']['value'] == $tiposExistencia['tecid']) echo 'selected="selected"' ?>  value="<?php echo $tiposExistencia['tecid'] ?>"><?php echo $tiposExistencia['tecdsc'] ?></option>
                                            <?php endforeach; ?>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmpsitcampus" class="col-lg-4 control-label">Situação do Campus/Uned</label>
                                    <div class="col-lg-8">
                                        <div class="btn-group" data-toggle="buttons">
                                          <label class="btn btn-default <?php if($this->entity['cmpsitcampus']['value'] == 't') echo 'active' ?>">
                                            <input id="cmpsitcampus" name="cmpsitcampus" type="radio" value="t" <?php if($this->entity['cmpsitcampus']['value'] == 't') echo 'checked="checked"' ?>> Funcionando
                                          </label>
                                          <label class="btn btn-default <?php if($this->entity['cmpsitcampus']['value'] == 'f') echo 'active' ?>">
                                            <input id="cmpsitcampus" name="cmpsitcampus" type="radio" value="f" <?php if($this->entity['cmpsitcampus']['value'] == 'f') echo 'checked="checked"' ?>> Não Funcionando
                                          </label>
                                        </div>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmpinstalacoes" class="col-lg-4 control-label">Instalações</label>
                                    <div class="col-lg-8">
                                        <div class="btn-group" data-toggle="buttons">
                                          <label class="btn btn-default <?php if($this->entity['cmpinstalacoes']['value'] == 'P') echo 'active' ?>">
                                            <input id="cmpinstalacoes" name="cmpinstalacoes" type="radio" value="P" <?php if($this->entity['cmpinstalacoes']['value'] == 'P') echo 'checked="checked"' ?>> Provisórias
                                          </label>
                                          <label class="btn btn-default <?php if($this->entity['cmpinstalacoes']['value'] == 'D') echo 'active' ?>">
                                            <input id="cmpinstalacoes" name="cmpinstalacoes" type="radio" value="D" <?php if($this->entity['cmpinstalacoes']['value'] == 'D') echo 'checked="checked"' ?>> Definitivas
                                          </label>
                                          <label class="btn btn-default <?php if($this->entity['cmpinstalacoes']['value'] == 'S') echo 'active' ?>">
                                            <input id="cmpinstalacoes" name="cmpinstalacoes" type="radio" value="S" <?php if($this->entity['cmpinstalacoes']['value'] == 'S') echo 'checked="checked"' ?>> Sem Instalações
                                          </label>
                                        </div>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmpobrascampus" class="col-lg-4 control-label">Obras no campus</label>
                                    <div class="col-lg-8">
                                        <div class="btn-group" data-toggle="buttons">
                                          <label  class="btn btn-default <?php if($this->entity['cmpobrascampus']['value'] == 't') echo 'active' ?>" >
                                            <input id="cmpobrascampus" name="cmpobrascampus" type="radio" value="t" <?php if($this->entity['cmpobrascampus']['value'] == 't') echo 'checked="checked"' ?>> Sim
                                          </label>
                                          <label class="btn btn-default <?php if($this->entity['cmpobrascampus']['value'] == 'f') echo 'active' ?>">
                                            <input id="cmpobrascampus" name="cmpobrascampus" type="radio" value="f" <?php if($this->entity['cmpobrascampus']['value'] == 'f') echo 'checked="checked"' ?>> Não
                                          </label>
                                        </div>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmptipocampus" class="col-lg-4 control-label">Tipo</label>
                                    <div class="col-lg-8">
                                        <select id="cmptipocampus" name="cmptipocampus" class="form-control" data-placeholder="Selecione...">
                                            <option value=""></option>
                                            <option value="C" <?php if($this->entity['cmptipocampus']['value'] == 'C') echo 'selected="selected"' ?>>Campus</option>
                                            <option value="A" <?php if($this->entity['cmptipocampus']['value'] == 'A') echo 'selected="selected"' ?>>Campus avançado</option>
                                            <option value="S" <?php if($this->entity['cmptipocampus']['value'] == 'S') echo 'selected="selected"' ?>>Campus sede</option>
                                            <option value="U" <?php if($this->entity['cmptipocampus']['value'] == 'U') echo 'selected="selected"' ?>>Unidade</option>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="cmpcaracteristicaund" class="col-lg-4 control-label">Caracterização da Unidade</label>
                                    <div class="col-lg-8">
                                        <div class="input-group">
                                            <textarea id="cmpcaracteristicaund" name="cmpcaracteristicaund" class="form-control"><?php echo $this->entity['cmpcaracteristicaund']['value'] ?></textarea>
                                            <div class="input-group-addon">
                                                <i style="cursor: help;" class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="popover" title="Ajuda" data-content="Texto sucinto destacando informações importantes sobre a unidade.
                                                Incluir informações sobre sua data de criação e a sua importância para a região, dentre outras."></i>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                <div class="form-group">

                                    <label for="cmpinfadicionais" class="col-lg-4 control-label">Inbformações adicionais</label>
                                    <div class="col-lg-8">
                                        <textarea id="cmpinfadicionais" name="cmpinfadicionais" class="form-control"><?php echo $this->entity['cmpinfadicionais']['value'] ?></textarea>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="well">
                            <fieldset>
                                <legend>
                                    Área do campus
                                </legend>
                                <div class="col-md-12">
                                    <div class="form-group">
                                        <label for="cmpareatotal" class="col-lg-4 control-label">Área total do campus (m²)</label>
                                        <div class="col-lg-8">
                                            <input id="cmpareatotal" name="cmpareatotal" type="text" class="form-control" placeholder="" value="<?php echo $this->entity['cmpareatotal']['value'] ?>" maxlength="9" autocomplete="off">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="cmpareaconstgeral" class="col-lg-4 control-label">Área total do campus construída (m²)</label>

                                        <div class="col-lg-8">
                                            <input id="cmpareaconstgeral" name="cmpareaconstgeral" type="text"
                                                   class="form-control" placeholder=""
                                                   value="<?php echo $this->entity['cmpareaconstgeral']['value'] ?>">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="cmpareaconstlab" class="col-lg-4 control-label">Área total construída - Laboratórios</label>

                                        <div class="col-lg-8">
                                            <input id="cmpareaconstlab" name="cmpareaconstlab" type="text"
                                                   class="form-control" placeholder=""
                                                   value="<?php echo $this->entity['cmpareaconstlab']['value'] ?>">
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label for="cmpareaconstsala" class="col-lg-4 control-label">Área total construída - Salas de aula</label>

                                        <div class="col-lg-8">
                                            <input id="cmpareaconstsala" name="cmpareaconstsala" type="text"
                                                   class="form-control" placeholder=""
                                                   value="<?php echo $this->entity['cmpareaconstsala']['value'] ?>">
                                        </div>
                                    </div>
                                </div>
                                </fieldset>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="well">
                            <fieldset>
                                <legend>
                                    Caracterização do campus
                                </legend>
                                <div class="form-group has-warning">
                                <label style="display: none;" for="intdsccaracteristica" class="col-lg-2 control-label">Caracterização do campus</label>
                                    <div class="col-lg-12">
                                        <div class="input-group">
                                            <textarea  id="cmpdsccaracteristica" name="cmpdsccaracteristica" class="form-control"
                                                      rows="10"
                                                      required="required"><?php echo $this->entity['cmpdsccaracteristica']['value'] ?>
                                            </textarea>
                                            <div class="input-group-addon">
                                                <i style="cursor: help;" class="glyphicon glyphicon-question-sign" data-placement="top" data-toggle="popover"
                                                   title="Ajuda"
                                                   data-content="Texto sucinto destacando informações importantes sobre a unidade.
                                                               Incluir informações sobre sua data de criação e sua importância para a região, dentre outras">
                                               </i>
                                            </div>
                                        </div>

<!--                                        <span class="help-block">-->
<!--                                            Texto sucinto destacando informações importantes sobre a unidade.-->
<!--                                            Incluir informações sobre sua data de criação-->
<!--                                            e sua importância para a região, dentre outras-->
<!--                                        </span>-->
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                </div>
            </form>
        <?php if($this->entity['cmpid']['value']): ?>
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
        <?php endif; ?>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button id="bt-salvar" type="button" class="btn btn-success">Salvar</button>
            </div>
        </div>
    </div>
</div>
<script language="javascript">

    $('[data-toggle="popover"]').popover();

    setTimeout(function(){
        $('#estuf1').chosen({no_results_text: "Sem resultado!" , allow_single_deselect: true});
        $('#muncod1').chosen({no_results_text: "Sem resultado!" , allow_single_deselect: true});
        $('#tecid').chosen({no_results_text: "Sem resultado!" , allow_single_deselect: true});
        $('#cmptipocampus').chosen({no_results_text: "Sem resultado!" , allow_single_deselect: true});
    },350);

//    $('#cmpdtcriacao').datepicker();
//    $('#cmpinicioatv').datepicker();
    $('#cmpdtinauguracao').datepicker();

    $('#cmpdtcriacao').mask('99/9999');
    $('#cmpinicioatv').mask('99/9999');
    $('#cmpdtinauguracao').mask('99/99/9999');


    $('#cmpcnpj').mask('99.999.999/9999-99');
    $('#cmpfonecomercial').mask('(99)9999-9999');
    $('#endcep1').mask('99999-999');

    $('#cmpareatotal').mask('999999.99');
    $('#cmpareaconstgeral').mask('999999.99');
    $('#cmpareaconstlab').mask('999999.99');
    $('#cmpareaconstsala').mask('999999.99');

    $('#endcep1').change(function(){

        endcep = $(this).val();
        tipoendereco = '1';

        $.post('/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP&endcep=' + endcep, function(resp){
            var dados = resp.split("||");
            $('#cmplogradouro').val(dados[0]);
            $('#endbai1').val(dados[1]);
            $('#cmpcompllogradouro').val(dados[2]);

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
        console.info($(this))
//            {target: $('#listar-pesquisa').hide().fadeIn()}
//        });

//        $(this).closest('form').saveAjax({clearForm: true, functionSucsess:'fecharModal'});
        $('#form-save').saveAjax({clearForm: true, functionSucsess:'fecharModal'});
        return false;
    });

</script>