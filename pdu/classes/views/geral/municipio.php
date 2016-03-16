
<?php //ver($this->_municipioValue, d); ?>
<input type="hidden" id="mundescricao1" value="
<?php foreach($this->municipios as  $municipio): ?>
                <?php if($municipio['codigo'] == $this->_municipioValue) echo $municipio['descricao'];; ?>
<?php endforeach; ?>">
<div class="form-group container-select-municipio" >
    <label for="<?php echo $this->_municipioName; ?>" class="col-lg-2 control-label">Município</label>
    <div class="col-lg-10">
        <select id="<?php echo $this->_municipioName; ?>" name="<?php echo $this->_municipioName; ?>" class="form-control" data-placeholder="Selecione...">
            <option value=""></option>
            <?php foreach($this->municipios as  $municipio): ?>
                <option <?php if($municipio['codigo'] == $this->_municipioValue) echo 'selected'; ?> value="<?php echo $municipio['codigo']; ?>"><?php echo $municipio['descricao']; ?></option>
            <?php endforeach; ?>
        </select>
    </div>
</div>
<?php if($this->_chosen): ?>
<script language="JavaScript">
    $('#<?php echo $this->_municipioName; ?>:last').chosen({no_results_text: "Sem resultado!" , allow_single_deselect: true});
</script>
<?php endif; ?>