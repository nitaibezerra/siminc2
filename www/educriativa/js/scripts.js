jQuery.extend(jQuery.validator.messages, {
    required: "Este campo &eacute; requerido.",
    remote: "Por favor, corrija este campo.",
    email: "Por favor, forne&ccedil;a um endere&ccedil;o eletr&ocirc;nico v&aacute;lido.",
    url: "Por favor, forne&ccedil;a uma URL v&aacute;lida.",
    date: "Por favor, forne&ccedil;a uma data v&aacute;lida.",
    dateISO: "Por favor, forne&ccedil;a uma data v&aacute;lida (ISO).",
    dateDE: "Bitte geben Sie ein gültiges Datum ein.",
    number: "Por favor, forne&ccedil;a um n&uacute;mero v&aacute;lida.",
    numberDE: "Bitte geben Sie eine Nummer ein.",
    digits: "Por favor, forne&ccedil;a somente d&iacute;gitos.",
    creditcard: "Por favor, forne&ccedil;a um cart&atilde;o de cr&eacute;dito v&aacute;lido.",
    equalTo: "Por favor, forne&ccedil;a o mesmo valor novamente.",
    accept: "Por favor, forne&ccedil;a um valor com uma extens&atilde;o v&aacute;lida.",
    maxlength: jQuery.validator.format("Por favor, forne&ccedil;a n&atilde;o mais que {0} caracteres."),
    minlength: jQuery.validator.format("Por favor, forne&ccedil;a ao menos {0} caracteres."),
    rangelength: jQuery.validator.format("Por favor, forne&ccedil;a um valor entre {0} e {1} caracteres de comprimento."),
    range: jQuery.validator.format("Por favor, forne&ccedil;a um valor entre {0} e {1}."),
    max: jQuery.validator.format("Por favor, forne&ccedil;a um valor menor ou igual a {0}."),
    min: jQuery.validator.format("Por favor, forne&ccedil;a um valor maior ou igual a {0}.")
});

jQuery.validator.addMethod("datePTBR", function(value) { 
  return this.optional(element) || /^\d\d?\/\d\d?\/\d\d\d?\d?$/.test(value); 
}, "Por favor, forne&ccedil;a uma data v&aacute;lida.");

$(document).ready(function() {

    $('#myModal').modal();

	$('.tipo-organizacao').change(function() {
		$('.tipos-org').addClass('hidden');
		$('.tipo-org-' + $(this).val()).removeClass('hidden');
	})

    ocultarCamposAeraAtuacao();
	
	$('body').on('click', '.check_area_atuacao', function(e) {
        ocultarCamposAeraAtuacao();
    });

    ocultarCamposGrupo();
    
	$('body').on('change', '#gruid', function(e) {
        e.preventDefault();
        var options = $("#ortid");

        ocultarCamposGrupo();

        $.ajax({
            url: '/educriativa/formulario.php?action=carregarTipo',
            data: {'gruid' : $(this).val()},
            method: 'post',
            success: function (result) {
                options.empty();
                var result = JSON.parse(result);
                $.each(result, function() {
                    options.append(new Option(this.ortdsc, this.ortid));
                });
                options.focus();
                $("#ortid").change();
            }
        });
	});
	
	$('.valida-cep').change(function(e) {
		if ($('#orgsemicep').is(':checked')) {
			$('.endereco').hide();
			$('.cep').removeProp('required');
		} else {
			$('.endereco').show();
			$('.cep').prop('required', true);
		}
	})

    $('#btn-finalizar').click(function(e){
        e.preventDefault();
        var action = 'salvarQuestionario';
        if ($('#form').valid()) {
            bootbox.confirm("Tem certeza de que deseja finalizar seu questionário?<br>Lembre-se: após finalizar não será possível alterar as informações.", function(result) {
                if (result) {
                    salvarFormulario(action, true);
                }
            });
        }
    });

    $('.save_step').click(function(){
        if($(this).attr('href') == '#step3' && !$('#form').valid()){
            setTimeout(function (){ $('a[href="#step2"]').click(); }, 500);
        }
    });

	$('.save, .save_step').click(function(e) {
		e.preventDefault();
		var action = $('.nav-pills .active > a').data('action');
		if ($('#form').valid()) {
			if (action == 'salvarQuestionario') {
				bootbox.confirm("Tem certeza de que deseja finalizar seu questionário?<br>Lembre-se: após finalizar não será possível alterar as informações.", function(result) {
					if (result) {
						salvarFormulario(action, true);
					}
				});
			} else {
				salvarFormulario(action, false);
			}
		}
	});
	
	$('.prev').click(function(e) {
		e.preventDefault();
        console.log(parseInt($('.nav-pills .active > a .step').text()));
        $('#btn-finalizar').hide();
        $('#btn-proximo').show();
	});
	
    $('.uf').change(function(e) {
    	e.preventDefault();
    	var options = $("#muncod");
    	$.ajax({
			url: '/educriativa/formulario.php?action=carregarMunicipio',
			data: {'estuf' : $(this).val()},
			method: 'post',
			success: function (result) {
				options.empty();
				var result = JSON.parse(result);
				$.each(result, function() {
				    options.append(new Option(this.mundsc, this.muncod));
				});
				options.focus();
			}
		});
    });
    
    $('.cnpj').change(function(e) {
    	e.preventDefault();
    	$.ajax({
			url: '/educriativa/formulario.php?action=buscarOrganizacao',
			data: {'orgcnpj' : $(this).val().replace(/\D/g, '')},
			method: 'post',
			success: function (result) {
				var result = JSON.parse(result);
				if (result.dt_cadastro) {
					$('#orgrazaosocial').val(result.no_empresarial_rf).change();
					$('#orgnomefantasia').val(result.no_fantasia_rf).change();
					$('#orgresponsavel').val(result.no_responsavel_rf).change();
				}
			}
		});
    });
    
    $('.question').change(function(e) {
    	$.ajax({
			url: '/educriativa/formulario.php?action=salvarResposta',
			data: {
				'perid': $(this).data('perid'), 
				'restexto': $(this).val()
			},
			method: 'post',
			success: function (result) {}
		});
	})

	$('.cep').change(function() {
		var action = $('.nav-pills .active > a').data('action');
		$.ajax({
			url: '/educriativa/formulario.php?action=buscarEndereco',
			data: {'orgcep' : $("#orgcep").serialize()},
			method: 'post',
			success: function (result) {
				var result = JSON.parse(result);
				if (result.muncod) {
					$('#orglogradouro').val(result.logradouro).change();
					$('#orgbairro').val(result.bairro).change();
					$('#estuf').val(result.estado).change();
					$('#estuf').trigger('change').change();
					setTimeout(function (){ $('#muncod').val(result.muncod).change() }, 500);
				} else {
					$('#orglogradouro').val('').change();
					$('#orgbairro').val('').change();
					$('#estuf').val('').change();
					$('#muncod').val('').change();
				}
			}
		});
	})
	
	$('.youtube').change(function(e) {
    	e.preventDefault();
    	$.ajax({
			url: '/educriativa/formulario.php?action=detalharVideo',
			data: {'orglinkvideo' : $('.youtube').val()},
			method: 'post',
			success: function (result) {
				var result = JSON.parse(result);
				
				$('.youtube-time').html('');
				
				if (result.items[0]) {
					var duration = result.items[0].contentDetails.duration.replace('PT', '').replace('H', 'h ').replace('M', 'm ').replace('S', 's ');
					var time = result.items[0].contentDetails.duration.replace('PT', '').replace('H', ':').replace('M', ':').replace('S', '');
					var seconds = parseSeconds(time);
					
					if (seconds > 300) {
						$('.youtube-time').html('');
						$('.youtube').val('');
						$('.youtube').parent().addClass('has-error');
						$('.youtube').after('<span id="youtube-error" class="help-block youtube-error" style="left: 0;">Vídeo maior que 5 minutos de duração.</span>');
					} else {
						$('.youtube').parent().removeClass('has-error');
						$('.youtube-error').remove();
						$('.youtube-time').html('<i class="fa fa-clock-o"></i> ' + duration);
					}
				} else {
					$('.youtube-time').html('');
					$('.youtube').val('');
				}
			}
		});
    });
	
	$('.countdown').each(function() {
		var countdown = $(this);
		$(countdown.data('input')).attr('maxlength', countdown.data('max-lenght'));
		$(countdown.data('input')).keyup(function() {
			var length = $(this).val().length;
			var lineBreaks = ($(this).val().match(/\n/g)||[]).length;
			var maxlength = countdown.data('max-lenght');
			var remaining = maxlength - (lineBreaks + length);
			countdown.html(remaining + ' restando');
		});
		$(countdown.data('input')).trigger('keyup');
	})
});

function salvarFormulario(action, redirect) {
	$.ajax({
		url: '/educriativa/formulario.php?action=' + action,
		data: $("#form").serialize(),
		method: 'post',
		success: function (result) {
            checarBotoes();
            if(result){
                alert(result);
            } else {
                if (redirect) {
                    window.location = "/educriativa/login.php?sucesso=1";
                }
            }
		}
	});
}

function parseSeconds(time) {
    var total = null;
    var parts = time.split(":");
    
    if (parts.length == 3) {
    	total = parseInt(parts[0] * 60 * 60) + parseInt(parts[1] * 60) + parseInt(parts[2]);
    } else if (parts.length == 2) {
    	total = parseInt(parts[0] * 60) + parseInt(parts[1]);
    } else {
    	total = parseInt(parts[0]);
    }
    
    return total;
}

function ocultarCamposGrupo(){
    $('.campos_grupo_outro').hide();

    // Organização Pública (Grupo 3)
    if($('#gruid').val() == '3'){
        $('.campos_grupo_publico').show();
    } else {
        // Outra Organização (Grupo 5)
        if($('#gruid').val() == '5'){
            $('.campos_grupo_outro').show();
            $('.campos_grupo').hide();
        } else {
        	$('.campos_grupo').show();
            $('.campos_grupo_publico').hide();
        }
    }
}

function ocultarCamposAeraAtuacao(){
    // Área de Atuação Escolar (1)
    if($('#check_area_atuacao_1').is(':checked')){
        $('.campos_area_escolar').show();
        $('.check_area_escolar').prop('required', true);
    } else {
        $('.campos_area_escolar').hide();
        $('.check_area_escolar').removeProp('required');
    }
}

function checarBotoes(){
    // Se for a última aba do wizard, hobilitar botão de finalizar
    if (parseInt($('.nav-pills .active > a .step').text()) == 3) {
        $('#btn-finalizar').show();
        $('#btn-proximo').hide();
    } else {
        $('#btn-finalizar').hide();
        $('#btn-proximo').show();
    }
}