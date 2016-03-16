<select id="tidcodigo" name="tidcodigo" class="CampoEstilo" size="1" style="width: auto">
    <option value="">Selecione</option>
    <?php if($this->values): ?>
    <?php foreach($this->values as $value): ?>
        <option value="<?php echo $value['tidcodigo'] ?>"><?php echo $value['tidnome'] ?></option>
    <?php endforeach; ?>
    <?php endif ?>
</select>
<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">