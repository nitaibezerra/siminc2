<h4>
	<label for="estid"><?= $this->aeEstrategia->getAttributeLabel('estnome'); ?></label>
	<span class="alert-danger" style="background-color: #FFF; font-size: 19px; ">*</span>
</h4>

<div class="col-lg-12">
	<select class="form-control" name="estid[]" id="estid" multiple="multiple">
		<?= $this->aeEstrategia->getOptionsEstrategia($this->where, $this->dados); ?>
	</select>
</div>

<script type="text/javascript">
    $(function () {
        $('#estid').multiSelect({
            keepOrder: true,
            selectableHeader: "<h5 style='margin: 7px 0 5px 0; font-weight: bold'>SELECIONAR</h5><div class='input-group'><input type='text' class='form-control input-sm selectableSearch' autocomplete='off' placeholder='pesquisar itens'><div class='input-group-addon'><span class='glyphicon glyphicon-search'></span></div></div>",
            selectionHeader: "<h5 style='margin: 5px 0 5px 0; font-weight: bold'>SELECIONADOS</h5><div class='input-group'><input type='text' class='form-control input-sm selectionSearch' autocomplete='off' placeholder='pesquisar itens selecionados'><div class='input-group-addon'><span class='glyphicon glyphicon-search'></span></div></div>",
            afterInit: function (ms) {
                var that = this,
                    $selectableSearch = that.$selectableUl.prev().children('input'),
                    $selectionSearch = that.$selectionUl.prev().children('input'),
                    selectableSearchString = '#' + that.$container.attr('id') + ' .ms-elem-selectable:not(.ms-selected)',
                    selectionSearchString = '#' + that.$container.attr('id') + ' .ms-elem-selection.ms-selected';

                that.qs1 = $selectableSearch.quicksearch(selectableSearchString).on('keydown', function (e) {
                    if (e.which === 40) {
                        that.$selectableUl.focus();
                        return false;
                    }
                });
                that.qs2 = $selectionSearch.quicksearch(selectionSearchString).on('keydown', function (e) {
                    if (e.which == 40) {
                        that.$selectionUl.focus();
                        return false;
                    }
                });
            }, afterSelect: function (value) {
                return false;
            }, afterDeselect: function (value) {}
        });
    });
</script>