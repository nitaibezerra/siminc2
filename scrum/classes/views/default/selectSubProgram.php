<label for="inputEmail" class="col-lg-2 control-label">Subprograma</label>
<div class="col-lg-10">
    <select name="subprgid" id="subprgid" class="form-control chosen-select"  data-placeholder="Selecione" required="required">
        <option value=""></option>
    <?php foreach($this->subprograms as $story): ?>
        <option <?php if($this->subprgid == $story['subprgid']) echo 'selected="selected"'?>value="<?php echo $story['subprgid'] ?>"><?php echo $story['subprgdsc'] ?></option>';
    <?php endforeach; ?>
    </select>
</div>

<script>
    
    $("form #subprgid").change(function() {
        var form = $(this).parents('form:first');
        $.post(window.location.href, {controller: 'default', action: 'selectStory', subprgid: $(this).val()}, function(html) {
            form.find('#container_select_story').hide().html(html).fadeIn();
        });
    });
    
    setTimeout(function(){
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
    },200);
</script>