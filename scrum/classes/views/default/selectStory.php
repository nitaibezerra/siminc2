<label for="estid" class="col-lg-2 control-label" for="Estória">Estória</label>
<div class="col-lg-10">
    <select name="estid" id="estid" class="form-control chosen-select"  data-placeholder="Selecione" required="required">
        <option value=""></option>
        <?php foreach($this->storys as $story): ?>
            <option <?php if( $this->estid == $story['estid'])  echo 'selected="selected"'?> value="<?php echo $story['estid'] ?>"><?php echo $story['esttitulo'] ?></option>';
        <?php endforeach; ?>
    </select>
</div>

<script>
    setTimeout(function(){
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
    },300);
</script>