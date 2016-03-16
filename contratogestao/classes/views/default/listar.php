<script language="javascript" src="/contratogestao/js/contrato_gestao.js"></script>
<input name="hqcidpai_interagido" id="hqcidpai_interagido" type="hidden" value="<?php echo $this->hqcidpai_interagido ?>">
<input name="path_hqcidpai" id="path_hqcidpai" type="hidden" value="<?php echo $this->path_hqcidpai ?>">
<?php echo $this->modelHierarquiacontrato->getArvore(' AND level = 1 '); ?>


<?php if($this->view->hqcidpai_interagido): ?>
<script>
	abrirItemInteragido();
</script>
<?php endif; ?>