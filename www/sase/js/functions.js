// ------------------------------ código pego da fonte do multiselect ------------------------- //

	/**
	* Gets whether all the options are selected
	* @param {jQuery} $el
	* @returns {bool}
	*/
	function multiselect_selected($el) {
		var ret = true;
		jQuery('option', $el).each(function(element) {
			if (!!!jQuery(this).prop('selected')) {
				ret = false;
			}
		});
		return ret;
	}

	/**
	* Selects all the options
	* @param {jQuery} $el
	* @returns {undefined}
	*/
	function multiselect_selectAll($el) {
		jQuery('option', $el).each(function(element) {
			$el.multiselect('select', jQuery(this).val());
		});
	}
	/**
	* Deselects all the options
	* @param {jQuery} $el
	* @returns {undefined}
	*/
	function multiselect_deselectAll($el) {
		jQuery('option', $el).each(function(element) {
			$el.multiselect('deselect', jQuery(this).val());
		});
	}

	/**
	* Clears all the selected options
	* @param {jQuery} $el
	* @returns {undefined}
	*/
	function multiselect_toggle($el, $btn) {
		if (multiselect_selected($el)) {
			multiselect_deselectAll($el);
			$btn.text("Selecionar Todos");
		} else {
			multiselect_selectAll($el);
			$btn.text("Desselecionar Todos");
		}
	}

// ------------------------------ / código pego da fonte do multiselect ------------------------- //