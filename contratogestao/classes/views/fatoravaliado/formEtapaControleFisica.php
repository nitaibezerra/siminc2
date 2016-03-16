<?php // require_once APPRAIZ . "includes/funcoesspo_componentes.php"; ?>
<hr>
<fieldset id="fieldset_pessoa_fisica">

    <input name="novoUsuario" id="novoUsuario" type="hidden">

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
                <label class="col-lg-3 control-label" for="regcod"><?= $this->usuario->getAttributeLabel('regcod'); ?></label>

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
                    <input style="display: none;" type="text" class="form-control" id="orgao" name="orgao" maxlength="50" size="51" />

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