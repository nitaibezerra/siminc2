$(document).ready(function() {
	$niveis = {'1': 'btn-danger', '2': 'btn-warning', '3': 'btn-default', '4': 'btn-primary', '5': 'btn-success'};
	
	$(".cnpj").mask("99.999.999/9999-99");
	$(".cpf, .campocpf").mask("999.999.999-99");
	$(".cep").mask("99.999-999");
	$('.chosen-select').chosen();
        $('.representacao_entidade').hide().removeClass('required');
        //$('.atuoutro').hide().removeClass('required');

	$('.ratepop').popover({
        container: 'body',
        html: true,
        content: function () {
            var clone = $($(this).data('popover-content')).clone(true).removeClass('hide');
            return clone;
        }
    }).click(function(e) {
        e.preventDefault();
    });
	
	$(".accordion").off('click').on('click', function(e) {
		$(this).hasClass('collapsed') ? 
		$(this).children('.angle').removeClass('fa-angle-down').addClass('fa-angle-up') :
		$(this).children('.angle').removeClass('fa-angle-up').addClass('fa-angle-down');
	});
	
	$('body').on('click', '.btn-raty', function(e) {
            $value = $(this).children().val();    
            
            $(this).parent().find(".btn-raty").removeClass("btn-default btn-success btn-primary btn-warning btn-danger").addClass('btn-raty-padrao');                
            $(this).addClass($niveis[$value]);             
            $.ajax({
                url: '/consultapne/formulario.php?action=avaliar',
                data: {iteid: $(this).children().data('iteid'), subid: $(this).children().data('subid'), avaresposta: $(this).children().val()},
                method: 'post',
                success: function (result) {}
            });
	});

    $('body').on('change', '.comment', function(e) {
		$.ajax({
			url: '/consultapne/formulario.php?action=comentar',
			data: {iteid: $(this).data('iteid'), subid: $(this).data('subid'), comdsc: $(this).val()},
			method: 'post',
			success: function (result) {}
		});
	});
	
	$(".persisted").off('change').on('change', function(e) {
		$.ajax({
			url: '/consultapne/formulario.php?action=atualizar',
			data: $("#avaliacao").serialize(),
			method: 'post',
			success: function (result) {}
		});
	});
	
	$('.sair').off("click").on("click", function () {
		console.log('dsasda');
		window.location = "/consultapne/index.php";
	});

	$('.cpf').change(function() {
            $.ajax({
                url: '/consultapne/index.php?action=carregar',
                data: {usucpf: $('#usucpf').val()},
                method: 'post',
                dataType : "json",
                success: function (result) {
                    $('#parrepresentacaoCombo').val(result.parrepresentacao).change();
                    $('#usucnpjCombo').val(result.parcnpj).focus();
                    $('#parrepresentacao').val(result.parrepresentacao);
                    $('#usucnpj').val(result.parcnpj);                    
                    if(result.parrepresentacao){
                        $('#parrepresentacaoCombo, #usucnpjCombo').attr('disabled', 'disabled');
                    } else {
                        $('#parrepresentacaoCombo, #usucnpjCombo').removeAttr('disabled', 'disabled');
                    }
                }
            });
    });

    $('#parrepcpf').change(function() {
        $.ajax({
            url: '/consultapne/controlador.php?action=carregarCPFRep',
            data: {
                usucpf: $('#parrepcpf').val()
            },
            method: 'post',
            dataType : "json",
            success: function (result) {
                $('#parrepnome').val(result.parnome).change();
                $('#repnome').val(result.parnome).change();
            }
        });
    });
	$(".comment").on('keyup', function(){
            var id = $(this).attr('id');
            var idN = id.substring(3,id.length);
            var digitado = $("#"+id).val().length;
            if (parseInt(1440)<parseInt(digitado)){
                var texto = $("#"+id).val();
                texto = texto.substring(0,1440);                
                $("#"+id).val(texto);
                $("#charleft"+idN).text(1440-texto.length);
                return false;
            }
            var soma = parseInt(1440)-parseInt(digitado);
            $("#charleft"+idN).text(soma);
        }).keyup();
        
        
	$("#avaliacao").validate({
		submitHandler : function(form) {
			bootbox.confirm("Tem certeza de que deseja finalizar sua avaliação?<br>Lembre-se: após finalizar não será possível alterar as informações.", function(result) {
				if (result) {
					form.submit();
				}
			});
		}
	});
	
    $('#representacaoCombo').change(function() {
        if(!$('#representacaoCombo').val() || $('#representacaoCombo').val() == 3) {
            $('.representacao_entidade').removeClass('select').hide();
            $('.representante_cnpj').removeClass('select').hide();                        
        } else {
            
            $('.representacao_entidade').addClass('select').show();
            $('.representante_cnpj').addClass('select').show();
            $("#representacao").val($(this).val());
            $("#tpoid").hide();
        }
    });
	
    $('#parrepresentacaoCombo').change(function() {
        $("#parrepresentacao").val($(this).val());
        if(!$('#parrepresentacaoCombo').val() || $('#parrepresentacaoCombo').val() == 3) {
            $('.representacao_entidade').removeClass('select').hide();
            $('.representante_cnpj').removeClass('select').hide();                        
        } else {
            $('.representacao_entidade').addClass('select').show();
            $('.representante_cnpj').addClass('select').show();
            $("#tpoid").hide();
        }
    });
    $(' #representacao').change(function() {
        if(!$('#representacao').val() || $('#representacao').val() == 3) {
            $('.representacao_entidade').removeClass('select').hide();
            $('.representante_cnpj').removeClass('select').hide();                        
        } else {
            
            $('.representacao_entidade').addClass('select').show();
            $('.representante_cnpj').addClass('select').show();
            $("#representacao").val($(this).val());
            $("#tpoid").hide();
        }
    });    
    
    $('body').on('change', '#usucnpjCombo', function() {
        $("#usucnpj").val($(this).val());        
    });
    
    $('body').on('change', '#atuid', function(){                 
        if ($(this).val()==14){
            $('.atuoutro').show().addClass('required');
        }else{
            $('.atuoutro').hide().removeClass('required');
            $("#atuoutro").val('');
        }
    });  
    
    $('body').on('change', '#intid', function(){                 
        if ($(this).val()==13){
            $('.intoutro').show().addClass('required');
        }else{
            $('.intoutro').hide().removeClass('required');
            $("#intoutro").val('');
        }
    }); 
    
    
    
    $('body').on('click', '.anterior-button', function(){                 
        window.location = "/consultapne/formulario.php?metid="+$("#anteriorMeta").val()+"&tpoForm=Quest";
    }); 
    $('body').on('click', '.proxima-button', function(){                 
        window.location = "/consultapne/formulario.php?metid="+$("#proximaMeta").val()+"&tpoForm=Quest";
    });     
    $('body').on('click', '.anterior-button-vizualizar', function(){                 
        window.location = "/consultapne/vizualizar.php?metid="+$("#anteriorMetaVizualizar").val()+"&tpoForm=Quest";
    }); 
    $('body').on('click', '.proxima-button-vizualizar', function(){                 
        window.location = "/consultapne/vizualizar.php?metid="+$("#proximaMetaVizualizar").val()+"&tpoForm=Quest";
    });       
    $('body').on('change', '#estuf', function() {
    	$('.chosen-container').removeClass('error');
        $('#div_municipio').load('?action=carregarMunicipio&estuf=' + $(this).val(), function() {
            $('.chosen-select').chosen();
        });
    });    
    
    $('body').on('change', '#parrepuf', function() {
    	$('.chosen-container').removeClass('error');
        $('#div_municipio_rep').load('?action=carregarParRepMunicipio&estuf=' + $(this).val(), function() {
            $('.chosen-select').chosen();
        });
    });
    
    var offset = 50;
    var duration = 500;

    $(window).scroll(function(e) {
        console.log();
            if (jQuery(this).scrollTop() > offset) {
                $('.save-button').css({top: $(window).scrollTop()+200});
                $('.save-button').fadeIn(duration);
            } else {
                $('.save-button').css({top: $(window).scrollTop()+200});
                $('.save-button').fadeOut(duration);
            }
    });

    $('.save-button').click(function(event) {
            $check = $(this).find('.btn-check');
            $loading = $(this).find('.btn-loading');

            $check.hide();
            $loading.show();

            setInterval(function() {
                    $loading.hide();
                    $check.show();
            }, 600);

            event.preventDefault();
            return false;
    });

    var labels = ['dias', 'horas', 'minutos', 'segundos'],
    endDate = $('#datatermino').val(),
    template = _.template($('#data-termino-template').html()),
    currDate = '00:00:00:00',
    nextDate = '00:00:00:00',
    parser = /([0-9]{2})/gi,
    $contador = $('.data-termino');

	function strfobj(str) {
		var parsed = str.match(parser), obj = {};
	    labels.forEach(function(label, i) {
	    	obj[label] = parsed[i]
	    });
	    return obj;
	}

	function diff(obj1, obj2) {
		var diff = [];
	    labels.forEach(function(key) {
	    	if (obj1[key] !== obj2[key]) {
	    		diff.push(key);
	    	}
	    });
	    return diff;
	}
	
	var initData = strfobj(currDate);
		labels.forEach(function(label, i) {
	    $contador.append(template({
	    	curr: initData[label],
	    	next: initData[label],
	    	label: label
	    }));
	});
	
	$contador.countdown(endDate, function(event) {
		var newDate = event.strftime('%D:%H:%M:%S'), data;
		if (newDate !== nextDate) {
			currDate = nextDate;
			nextDate = newDate;
			data = {
				'curr': strfobj(currDate),
				'next': strfobj(nextDate)
			};
			
			diff(data.curr, data.next).forEach(function(label) {
				var selector = '.%s'.replace(/%s/, label),
				$node = $contador.find(selector);
				$node.removeClass('flip');
				$node.find('.curr').text(data.curr[label]);
				$node.find('.next').text(data.next[label]);
				_.delay(function($node) {
					$node.addClass('flip');
				}, 50, $node);
			});
		}
	});
})