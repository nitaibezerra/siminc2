<select id="muncodigo" name="muncodigo" class="CampoEstilo" size="1" style="width: auto">
    <option value="">Selecione</option>
    <?php if($this->municipios): ?>
    <?php foreach($this->municipios as $value): ?>
        <option value="<?php echo $value['muncodigo'] ?>"  <?php if ($this->muncodigo == $value['muncodigo']) echo 'selected="true"' ?>><?php echo $value['munnome'] ?></option>
    <?php endforeach; ?>
    <?php endif ?>
</select>
<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">