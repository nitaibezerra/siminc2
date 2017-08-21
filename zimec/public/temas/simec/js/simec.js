// Custom scripts
$(document).ready(function ()
{
	attachInit();
});

$(document).ready(function(){
	var profile = new Dropzone(document.body, {
        url: '/seguranca/atualizafoto.php',
        thumbnailWidth: 80,
        thumbnailHeight: 80,
        parallelUploads: 20,
        autoProcessQueue: true,
        clickable: ".profile-picture",
        success: function(index, response) {
        	var data = eval('(' + response + ')');
        	if (data.status == 'success') {
        		$('.profile-pic').attr('src', '/seguranca/imagemperfil.php?t=' + new Date().getTime());
        	}
        }
    });

    $('.dataTables').DataTable({
        pageLength: 100,
        responsive: true,
        dom: '<"html5buttons"B>lTfgitp',
        "language": {
            "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
        }
    });

    $('.slim-scroll').slimscroll({
        height: '300px'
    });

    $('.link-excluir').click(function(){
        href = $(this).attr('href');
        swal({
            title: "Atenção!",
            text: "Deseja realmente excluir o registro?",
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55",
            confirmButtonText: "Sim, estou certo!",
            closeOnConfirm: false
        },
        function(){
            window.location.href = href;
        });

        return false;
    });
});

$(window).scroll(function() {
	var coordenada = $(document).scrollTop();
	if (coordenada > 0) {
		$('#top-shadow').fadeIn('fast');
	} else {
		$('#top-shadow').fadeOut('fast');
	}
});

function attachInit()
{
    // Campos de formulário
    $('div.date').datepicker({
        todayBtn: "linked",
        format: 'dd/mm/yyyy',
        language: 'pt-BR',
        keyboardNavigation: false,
        forceParse: false,
        calendarWeeks: true,
        autoclose: true
    });

    $('.input-daterange').datepicker({
        todayBtn: "linked",
        format: 'dd/mm/yyyy',
        language: 'pt-BR',
        keyboardNavigation: false,
        forceParse: false,
        calendarWeeks: true,
        autoclose: true
    });

    $('.summernote').summernote({
        height: 300,
        lang: 'pt-BR'
    });

	// Chosen
	var config = {
		'.chosen-select'           : {allow_single_deselect:true},
        '.chosen-select-deselect'  : {allow_single_deselect:true},
        '.chosen-select-no-single' : {disable_search_threshold:10},
        '.chosen-select-no-results': {no_results_text:'Oops, nothing found!'},
        '.chosen-select-width'     : {width:"95%"}
	}
	for (var selector in config) {
		$(selector).chosen(config[selector]);
	}
	
	$('.fileinput').fileinput({
        language: 'pt-br',
        allowedFileExtensions : ['jpg', 'png','gif'],
        uploadAsync: false,
		overwriteInitial: true,
	    showRemove: false,
	    showUpload: false
    }).on("filebatchselected", function(event, files) {
    	console.log($(this).data('image-preview'));
    });
	
	$('.tree').jstree({
		"state" : { "key" : $(this).id },
		"plugins" : [ "state" ]
	});
	
	$(".tree").bind("open_node.jstree", function (event, data) { 
		$("[data-toggle=tooltip]").tooltip();
	});
	
	$('[data-toggle="tooltip"]').tooltip();
	
	$(":input").inputmask();
	
    // Collapse ibox function
    $('.collapse-link').click( function() {
        var ibox = $(this).closest('div.ibox');
        var button = $(this).find('i');
        var content = ibox.find('div.ibox-content');
        content.slideToggle(200);
        button.toggleClass('fa-chevron-up').toggleClass('fa-chevron-down');
        ibox.toggleClass('').toggleClass('border-bottom');
        setTimeout(function () {
            ibox.resize();
            ibox.find('[id^=map-]').resize();
        }, 50);
    });

    if (!$.fn.DataTable.isDataTable('.dataTable')) {
    	$('.dataTable').DataTable({
	    	'aoColumnDefs' : [{
	    		'bSortable' : false,
	    		'aTargets' : [ 'unsorted' ]
	    	}],
	    	'oLanguage' : {
	    		'sProcessing' : "Processando...",
	    		'sLengthMenu' : "Mostrar _MENU_ registros",
	    		'sZeroRecords' : "N&atilde;o foram encontrados resultados",
	    		'sInfo' : "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
	    		'sInfoEmpty' : "Mostrando de 0 at&eacute; 0 de 0 registros",
	    		'sInfoFiltered' : "(filtrado de _MAX_ registros no total)",
	    		'sInfoPostFix' : ".",
	    		'sSearch' : "Pesquisar :&nbsp;&nbsp;",
	    		'sUrl' : "",
	    		'oPaginate' : {
	    			'sFirst' : "Primeiro",
	    			'sPrevious' : "Anterior",
	    			'sNext' : "Seguinte",
	    			'sLast' : "&Uacute;ltimo"
	    		}
		    }
	    });
    }
    
    $('.confirm').on('click', function(e) {
    	e.preventDefault();
    	$el = $(this);
    	bootbox.confirm($el.data('confirmacao'), function(result) {
			if (result) {
				window.location = $el.attr('href');
			}
		});
    });
    
    $("#btUser").click(function(){
    	$(".mensagensAvisos").hide("slow");
        url = window.location.href;
		$.get(url,{limparAvisos:"true"},function(response){});
	});

	$(".confirmarAviso").click(function(e){
		e.preventDefault();
		url = $(this).attr('href');
		bootbox.confirm('Deseja abrir a localização da referência agora? <br> Trabalhos não salvos serão perdidos.', function(opcao){
			if(opcao){
				location.href = url;
			}
		});
	});

	var config = {allow_single_deselect: true};
	
	$('#btPainelWorkflow').click(function(){
		$('#modal-painel-workflow').modal();
			setTimeout(function(){
				$( '#fluxo-painel-workflow' ).chosen(config);
				$( '#estado-documento-situacao-workflow' ).chosen(config);
			} , 450);
	});


	$('#fluxo-painel-workflow').change(function(){
		url = window.location.href;
		dataForm = $('#form-painel-workflow').serialize();
		data = dataForm + '&carregarSituacaoPainel=true';
		$.post(
			url
			, data
			, function(html){
				$( '#situacao-painel-workflow' ).hide().html(html).fadeIn('slow');
				$( '#estado-documento-situacao-workflow' ).attr('multiple' , 'multiple');
				$( '#estado-documento-situacao-workflow').attr('data-placeholder' , 'Selecione...');
				$( '#estado-documento-situacao-workflow').find('option').attr('selected', 'selected');
				$( '#estado-documento-situacao-workflow').chosen(config);
			}
		);
	});
	
	$('.js-switch[data-switchery!="true"]').each(function()
	{
        var switchery = new Switchery(this, { color: '#1AB394' });
	});
	
	$('#bt-procurar-painel-workflow').click(function(){
		$('.has-error').removeClass('has-error');
		isValid = true;
		html = '';
		if($('#fluxo-painel-workflow').val() == ''){
			element = $('#fluxo-painel-workflow');
			label = element.parent().parent('.form-group').children('label').text();
			html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> não pode ser vazio!.<a class="alert-link" href="#"></a></div></div>';
			$('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
			element.parent().parent('.form-group').addClass('has-error');
			$('#fluxo-painel-workflow').focus();
			isValid = false;
			return false;
		}
		if($('#dt-inicio-painel-workflow').val() == ''){
			element = $('#dt-inicio-painel-workflow');
			label = element.parent().parent().parent('.form-group').children('label').text();
			html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>Período (Data de início):</strong> não pode ser vazio!.<a class="alert-link" href="#"></a></div></div>';
			$('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
			element.parent().parent().parent('.form-group').addClass('has-error');
			$('#fluxo-painel-workflow').focus();
			isValid = false;
			return false;
		}
		if($('#dt-fim-painel-workflow').val() == ''){
			element = $('#dt-fim-painel-workflow');
			label = element.parent().parent().parent().parent('.form-group').children('label').text();
			html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>Período (Data fim):</strong> não pode ser vazio!.<a class="alert-link" href="#"></a></div></div>';
			$('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
			element.parent().parent().parent('.form-group').addClass('has-error');
			$('#fluxo-painel-workflow').focus();
			isValid = false;
			return false;
		}

		if(isValid){
			$('.has-error').removeClass('has-error');
			url = window.location.href;
			dataForm = $('#form-painel-workflow').serialize();
			data = dataForm + '&gerarGraficoPainel=true';
			$.post(
				url
				, data
				, function(html){
					$('#container-grafico-painel-workflow').hide().html(html).fadeIn('slow');
				}
			);
		}
		return false;
	});
}

function changeSystem(system)
{
	location.href = "../muda_sistema.php?sisid=" + system;
}

function changeTheme(theme)
{
	$('#theme').val(theme);
    $('#form_theme').submit();
}
	
function simularUsuariosSistemas(cpf) {
	$.post(window.location.href, {simularUsuariosSistema: 'true', cpf: cpf}, function(html) {
		if (html == 'ok') {
			window.location='../muda_sistema.php';
		}
	});
}

function abrirUsuariosOnline()
{
	window.open(
		'../geral/usuarios_online.php',
		'usuariosonline',
		'height=500,width=600,scrollbars=yes,top=50,left=200'
	);
}