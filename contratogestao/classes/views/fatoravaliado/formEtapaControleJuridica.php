<script language="javascript" src="/contratogestao/js/form_fator_avaliado.js"></script>
<hr>
<fieldset>
    <div class="row">
        <div class="col-lg-6">
  
            <div class="form-group">
                <label for="entnumcpfcnpj" class="col-lg-2 control-label">CNPJ</label>
                <div class="col-lg-3">
                    <input id="entnumcpfcnpj" name="entnumcpfcnpj" type="text" class="form-control" placeholder="__.___.___/____-__" maxlength="19">
                </div>
            </div>

            <div class="form-group">
                <label for="entnome" class="col-lg-2 control-label">Nome</label>
                <div class="col-lg-8">
                    <input id="entnome" name="entnome" type="text" class="form-control" maxlength="80">
                </div>
            </div>

            <div class="form-group">
                <label for="entrazaosocial" class="col-lg-2 control-label">Razão Social</label>
                <div class="col-lg-8">
                    <input id="entrazaosocial" name="entrazaosocial" type="text" class="form-control" maxlength="80">
                </div>
            </div>

            <div class="form-group">
                <label for="entsig" class="col-lg-2 control-label">Sigla</label>
                <div class="col-lg-4">
                    <input id="entsig" name="entsig" type="text" class="form-control">
                </div>
            </div>

            <div class="form-group">
                <label for="entobs" class="col-lg-2 control-label">Observação</label>
                <div class="col-lg-8">
                    <textarea id="entobs" name="entobs" rows="3" class="form-control"></textarea>
                </div>
            </div>
        </div>

        <div class="col-lg-6">
            
            <div class="form-group">
                <label for="njuid" class="col-lg-3 control-label">Natureza Jurídica</label>
                <div class="col-lg-7">
                    <select id="njuid" name="njuid" class="form-control">
                        <?= $this->entidade->getCombo(); ?>
                    </select>
                </div>
            </div>
            
            <div class="form-group">
                <label for="entemail" class="col-lg-3 control-label">E-mail</label>
                <div class="col-lg-7">
                    <input id="entemail" name="entemail" type="text" class="form-control">
                </div>
            </div>

            <div class="form-group">
                <label for="entnumcomercial" class="col-lg-3 control-label">Telefone Comercial</label>
                <div class="col-lg-2">
                    <input id="entnumdddcomercial" name="entnumdddcomercial" type="text" class="form-control" placeholder="__" maxlength="2">
                </div>
                <div class="col-lg-3">
                    <input id="entnumcomercial" name="entnumcomercial" type="text" class="form-control" placeholder="Telefone" maxlength="10">
                </div>

                <div class="col-lg-2">
                    <input id="entnumramalcomercial" name="entnumramalcomercial" type="text" class="form-control" placeholder="Ramal" maxlength="4">
                </div>
            </div>

            <div class="form-group">
                <label for="entnumfax" class="col-lg-3 control-label">Fax</label>
                <div class="col-lg-2">
                    <input id="entnumdddfax" name="entnumdddfax" type="text" class="form-control" placeholder="__" maxlength="2">
                </div>
                <div class="col-lg-3">
                    <input id="entnumfax" name="entnumfax" type="text" class="form-control" placeholder="Telefone" maxlength="10">
                </div>

                <div class="col-lg-2">
                    <input id="entnumramalfax" name="entnumramalfax" type="text" class="form-control" placeholder="Ramal" maxlength="4">
                </div>
            </div>
        </div>
    </div>
</fieldset>