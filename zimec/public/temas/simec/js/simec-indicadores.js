// Custom scripts
$(document).ready(function () 
{
	attachInit();
});

function attachInit() 
{
	// MetsiMenu
    $('#side-menu').metisMenu();

    $(".nano").nanoScroller({ preventPageScrolling: true });
    
	// Navlist
	//$('.nav').breadcrumb();

	// Chosen
	var config = {
		'.chosen-select'           : {},
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
	
	//$('.tree').treegrid({'initialState': 'collapsed'});
	
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

    // Close ibox function
    $('.close-link').click( function() {
        var content = $(this).closest('div.ibox');
        content.remove();
    });

    // Small todo handler
    $('.check-link').click( function(){
        var button = $(this).find('i');
        var label = $(this).next('span');
        button.toggleClass('fa-check-square').toggleClass('fa-square-o');
        label.toggleClass('todo-completed');
        return false;
    });

    // minimalize menu
    $('.navbar-minimalize').click(function () {
        $("body").toggleClass("mini-navbar");
        SmoothlyMenu();
    })

    // Full height of sidebar
    function fix_height() {
        var heightWithoutNavbar = $("body > #wrapper").height() - 61;
        $(".sidebard-panel").css("min-height", heightWithoutNavbar + "px");
    }
    fix_height();

    $(window).bind("load resize click scroll", function() {
        if(!$("body").hasClass('body-small')) {
            fix_height();
        }
    })
    
    // iCheck
	$('.i-checks').iCheck({
		checkboxClass : 'icheckbox_square-green',
		radioClass : 'iradio_square-green',
	});
    
    // tooltips
    $('.tooltips').tooltip({
        selector: "[data-toggle=tooltip]",
        container: "body"
    })

    $(".messager").messager($(".alerter").data("tipo"), $(".alerter").text());

    $("[data-toggle=popover]").popover();
    
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
    
    $('.confirm').on('click', function(e) {
    	e.preventDefault();
    	$el = $(this);
    	bootbox.confirm($el.data('confirmacao'), function(result) {
			if (result) {
				window.location = $el.attr('href');
			}
		});
    });
}

// For demo purpose - animation css script
function animationHover(element, animation){
    element = $(element);
    element.hover(
        function() {
            element.addClass('animated ' + animation);
        },
        function(){
            //wait for animation to finish before removing classes
            window.setTimeout( function(){
                element.removeClass('animated ' + animation);
            }, 2000);
        });
}

// Minimalize menu when screen is less than 768px
$(function() {
    $(window).bind("load resize", function() {
        if ($(this).width() < 769) {
            $('body').addClass('body-small')
        } else {
            $('body').removeClass('body-small')
        }
    })
})

function SmoothlyMenu() {
    if (!$('body').hasClass('mini-navbar') || $('body').hasClass('body-small')) {
        // Hide menu in order to smoothly turn on when maximize menu
        $('#side-menu').hide();
        // For smoothly turn on menu
        setTimeout(
            function () {
                $('#side-menu').fadeIn(500);
            }, 100);
    } else {
        // Remove all inline style from jquery fadeIn function to reset menu state
        $('#side-menu').removeAttr('style');
    }
}

// Dragable panels
function WinMove() {
    $("div.ibox").not('.no-drop').draggable({
        revert: true,
        zIndex: 2000,
        cursor: "move",
        handle: '.ibox-title',
        opacity: 0.8,
        drag: function(){
            var finalOffset = $(this).offset();
            var finalxPos = finalOffset.left;
            var finalyPos = finalOffset.top;
            // Add div with above id to see position of panel
            $('#posX').text('Final X: ' + finalxPos);
            $('#posY').text('Final Y: ' + finalyPos);
        },
    }).droppable({
        tolerance: 'pointer',
        drop: function (event, ui) {
            var draggable = ui.draggable;
            var droppable = $(this);
            var dragPos = draggable.position();
            var dropPos = droppable.position();
            draggable.swap(droppable);
            setTimeout(function () {
                var dropmap = droppable.find('[id^=map-]');
                var dragmap = draggable.find('[id^=map-]');
                if (dragmap.length > 0 || dropmap.length > 0) {
                    dragmap.resize();
                    dropmap.resize();
                }
                else {
                    draggable.resize();
                    droppable.resize();
                }
            }, 50);
            setTimeout(function () {
                draggable.find('[id^=map-]').resize();
                droppable.find('[id^=map-]').resize();
            }, 250);
        }
    });
}

$.fn.messager = function(e, t) {
	t && (top === window ? $.gritter.add({
		position : "bottom",
		title : "Aviso",
		text : t,
		image : $themeUrl + "/img/gritter/" + e + ".png",
		sticky : !1,
		time : ""
	}) : parent.$.gritter.add({
		position : "bottom",
		title : "Aviso",
		text : t,
		image : $themeUrl + "/img/gritter/" + e + ".png",
		sticky : !1,
		time : ""
	}))
};

function changeSystem(system)
{
	location.href = "../../muda_sistema.php?sisid=" + system;
}