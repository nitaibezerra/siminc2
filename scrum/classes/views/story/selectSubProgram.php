<label for="inputEmail" class="col-lg-2 control-label">Subprograma</label>
<div class="col-lg-10">
    <select name="subprgid" id="subprgid" class="form-control chosen-select"  data-placeholder="Selecione" required="required">
        <option value=""></option>
    <?php foreach($this->subprograms as $story): ?>
        <option value="<?php echo $story['subprgid'] ?>"><?php echo $story['subprgdsc'] ?></option>';
    <?php endforeach; ?>
    </select>
</div>

<script>
    setTimeout(function(){
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
    },200);
</script>