$.extend($.validator, {
	messages: {
		required: "Preenchimento obrigatório.",
		mail: "Por favor informe um e-mail válido.",
		url: "Por favor informe uma URL válida.",
		dateISO: "Por favor informe uma data válida.",
		dateDE: "Por favor informe uma data válida.",
		number: "Por favor informe um número válido.",
		numberDE: "Por favor informe um numero válido.",
		digits: "Por favor informe somente números",
		creditcard: "Por favor informe um cartão de crédito válido.",
		equalTo: "Por favor informe o mesmo valor novamente.",
		accept: "Please enter a value with a valid extension.",
		maxlength: $.validator.format("Por favor não informe um valor maior que {0} caracteres."),
		minlength: $.validator.format("Por favor informe pelo menos {0} caracteres."),
		rangelength: $.validator.format("Por favor informe um valor entre {0} e {1}."),
		rangevalue: $.validator.format("Por favor informe um valor entre {0} e {1}."),
		maxvalue: $.validator.format("Por favor informe um valor menor ou igual a {0}."),
		minvalue: $.validator.format("Por favor informe um valor maior ou igual a {0}.")
    }
});

$.validator.addMethod(
    "date",
    function ( value, element ) {
        var bits = value.match( /([0-9]+)/gi ), str;
        if ( ! bits )
            return this.optional(element) || false;
        str = bits[ 1 ] + '/' + bits[ 0 ] + '/' + bits[ 2 ];
        return this.optional(element) || !/Invalid|NaN/.test(new Date( str ));
    },
    "Por favor informe uma data válida."
);

$.validator.addMethod(
    "select",
    function ( value, element ) {
    	return (value != '0');
    },
    "Por favor informe uma opcao válido."
);