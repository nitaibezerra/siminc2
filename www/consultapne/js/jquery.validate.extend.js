$.extend($.validator, {
	messages: {
		required: "",
		mail: "",
		url: "",
		date: "",
		dateISO: "",
		dateDE: "",
    }
});

$.validator.addMethod(
    "required",
    function ( value, element ) {
    	return (value != '');
    },
    ""
);

$.validator.addMethod(
    "date",
    function ( value, element ) {
        var bits = value.match( /([0-9]+)/gi ), str;
        if ( ! bits )
            return this.optional(element) || false;
        str = bits[ 1 ] + '/' + bits[ 0 ] + '/' + bits[ 2 ];
        return this.optional(element) || !/Invalid|NaN/.test(new Date( str ));
    },
    ""
);

$.validator.setDefaults({ ignore: ":hidden:not(select)" });

$.validator.addMethod(
    "select",
    function ( value, element ) {
        if (value == '0' || value == '') {
			$(element).next('.chosen-container').addClass('error');
            $('html, body').scrollTop(0);
            return false;
        } else {
            $(element).next('.chosen-container').removeClass('error');
        	$(element).removeClass('error');
        }
        return true;
    },
    ""
);