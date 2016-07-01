
<div class="form-group  container-select-uf">
    <label for="<?php echo $this->_ufName; ?>" class="col-lg-2 control-label">UF</label>
    <div class="col-lg-10">
        <select id="<?php echo $this->_ufName; ?>" name="<?php echo $this->_ufName; ?>" class="form-control" data-placeholder="Selecione...">
            <option value=""></option>
            <?php foreach($this->uf as  $uf): ?>
                <option <?php if($uf['codigo'] == $this->_ufValue) echo 'selected'; ?> value="<?php echo $uf['codigo']; ?>"><?php echo $uf['descricao']; ?></option>
            <?php endforeach; ?>
        </select>
    </div>
</div>
<script language="JavaScript">
    <?php if($this->_chosen): ?>
        $('#<?php echo $this->_ufName; ?>').chosen({no_results_text: "Sem resultado!" , allow_single_deselect: true});
    <?php endif; ?>

    $('#<?php echo $this->_ufName; ?>').change(function(){

        element = $(this);
        $estuf = element.val();
        $.post(window.location.href , {controller: 'geral' , action: 'municipio' , estuf: $estuf} , function(html){
            element.closest('form').find('.container-select-municipio').replaceWith(function(){
                return $(html).hide().fadeIn();
            });
        });
    });
</script>