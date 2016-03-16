var completePath = new Array();
var itemSelecionado;

$(function () {
	//uploader
	var uploader = new qq.FileUploader({
        element     : document.getElementById('caminhoLista'),
        action      : 'uploadArquivo.php',
        template    : document.getElementById('caminhoLista').innerHTML,
        fileTemplate: '<div class="itemListaDiv">' +
    		        		'<div class="dscItemLista">' +
    							'<span class="qq-upload-spinner"></span>' +
    							'<span class="qq-upload-file"></span>' +
    						'</div>' +
    						'<span class="qq-upload-failed-text">Falhou</span><br/>' +
    						'<span class="qq-upload-size"></span>' +
    						'<a class="qq-upload-cancel" href="#">Cancelar</a>' +
    					'</div>',
		afterOnComplete  : function(param){
								if( param.success == true ){
									replaceLine(param, true);	
									adicionaMenuDireito();
								}
						   },
	    personalAddToList: function(id, fileName, item){
							    var flag = false;
							    var thisClass = this;
								$('.itemListaDiv:contains(' + fileName + ')').each(function(k, elem){
							    	//faz verificação para pegar itens IGUAIS e não que CONTENHAM
							    	//lógica para evitar pegar a linha de um arquivo com o nome semelhante
									if( $(elem).find('.itemLista').html()      == fileName 
							    	 || $(elem).find('.qq-upload-file').html() == fileName ){
										var elemPai = $(elem);
										elemPai.after(item);
										elemPai.hide();
						    			flag = true;
						    			return flag;
							    	}
							    });									
								return flag;
						   },
	   afterOnCancel    : function(elem){
							   var title = $(elem).find('.qq-upload-file').html();
							   //seleciona o item que foi escondido e o mostra novamente
							   $('.itemListaDiv:contains(' + title + '):not(:visible) .itemLista').each(function(k, elem){
							    	//faz verificação para pegar itens IGUAIS e não que CONTENHAM
							    	//lógica para evitar pegar a linha de um arquivo com o nome semelhante
									if( $(elem).html() == title ){
										$(elem).parent().parent().show();
						    			return true;
							    	}
							    });
							   }
    });
	
	//Árvore
	$("#demo")
		.bind("before.jstree", function (e, data) {
			$("#alog").append(data.func + "<br />");
		})
		.jstree({ 
			// List of active plugins
			"plugins" : [ 
				"themes","json_data","ui","crrm","dnd","search","types","hotkeys","contextmenu" 
			],
	
			// I usually configure the plugin that handles the data first
			// This example uses JSON as it is most common
			"json_data" : { 
				// This tree is ajax enabled - as this is most common, and maybe a bit more complex
				// All the options are almost the same as jQuery's AJAX (read the docs)
				"ajax" : {
					// the URL to fetch the data
					"url" : "./server.php",
					// the `data` function is executed in the instance's scope
					// the parameter is the node being loaded 
					// (may be -1, 0, or undefined when loading the root nodes)
					"data" : function (n) { 
						// the result is fed to the AJAX request `data` option
						return { 
							"operation" : "get_children", 
	                        "id" : n.attr ? n.attr("id").replace("node_","") : 1 
						}; 
					}
				}
			},
			// Configuring the search plugin
			"search" : {
				// As this has been a common question - async search
				// Same as above - the `ajax` config option is actually jQuery's AJAX object
				"ajax" : {
					"url" : "./server.php",
					// You get the search string as a parameter
					"data" : function (str) {
						return { 
							"operation" : "search", 
							"search_str" : str 
						}; 
					}
				}
			},
			// Using types - most of the time this is an overkill
			// read the docs carefully to decide whether you need types
			"types" : {
				// I set both options to -2, as I do not need depth and children count checking
				// Those two checks may slow jstree a lot, so use only when needed
				"max_depth" : -2,
				"max_children" : -2,
				// I want only `drive` nodes to be root nodes 
				// This will prevent moving or creating any other type as a root node
				"valid_children" : [ "drive" ],
				"types" : {
					// The default type
					"default" : {
						// I want this type to have no children (so only leaf nodes)
						// In my case - those are files
						"valid_children" : "none",
						// If we specify an icon for the default type it WILL OVERRIDE the theme icons
						"icon" : {
							"image" : "./file.png"
						}
					},
					// The `folder` type
					"folder" : {
						// can have files and other folders inside of it, but NOT `drive` nodes
						"valid_children" : [ "default", "folder" ],
						"icon" : {
							"image" : "./css/dropbox-icons/folder.gif"
						}
					},
					// The `drive` nodes 
					"drive" : {
						// can have files and folders inside, but NOT other `drive` nodes
						"valid_children" : [ "default", "folder" ],
						"icon" : {
							"image" : "./root.png"
						},
						// those prevent the functions with the same name to be used on `drive` nodes
						// internally the `before` event is used
						"start_drag" : false,
						"move_node" : false,
						"delete_node" : false,
						"remove" : false
					}
				}
			},
			// UI & core - the nodes to initially select and open will be overwritten by the cookie plugin
	
			// the UI plugin - it handles selecting/deselecting/hovering nodes
			"ui" : {
				// this makes the node with ID node_4 selected onload
	//			"initially_select" : [ "node_4" ]
			},
			// the core plugin - not many options here
			"core" : { 
				// just open those two nodes up
				// as this is an AJAX enabled tree, both will be downloaded from the server
	//			"initially_open" : [ "node_2" , "node_3" ] 
			}
		})
		.bind("select_node.jstree", function (event, data) { 
			// `data.rslt.obj` is the jquery extended node that was clicked
			if (data.rslt.obj.attr("href"))
			{
				window.location.href=data.rslt.obj.attr("href");
			}else if (data.rslt.obj.attr("rel") == 'folder')
			{
				completePath[data.rslt.obj.attr('path')] = data.rslt.obj.attr('id');
				chooseFolder(data.rslt.obj.attr("id"), data.rslt.obj.attr("path"));
			}
			
		})
		.bind("create.jstree", function (e, data) {
			$.post(
				"./server.php", 
				{ 
					"operation" : "create_node", 
					"id" : data.rslt.parent.attr("id").replace("node_",""), 
					"position" : data.rslt.position,
					"title" : data.rslt.name,
					"type" : data.rslt.obj.attr("rel")
				}, 
				function (r) {
					if(r.status) {
						$(data.rslt.obj).attr("id", "node_" + r.id);
					}
					else {
						$.jstree.rollback(data.rlbk);
					}
				}
			);
		})
		.bind("remove.jstree", function (e, data) {
			data.rslt.obj.each(function () {
				$.ajax({
					async : false,
					type: 'POST',
					url: "./server.php",
					data : { 
						"operation" : "remove_node", 
						"id" : this.id.replace("node_","")
					}, 
					success : function (r) {
						if(!r.status) {
							data.inst.refresh();
						}
					}
				});
			});
		})
		.bind("rename.jstree", function (e, data) {
			$.post(
				"./server.php", 
				{ 
					"operation" : "rename_node", 
					"id" : data.rslt.obj.attr("id").replace("node_",""),
					"title" : data.rslt.new_name
				}, 
				function (r) {
					if(!r.status) {
						$.jstree.rollback(data.rlbk);
					}
				}
			);
		})
		.bind("move_node.jstree", function (e, data) {
			data.rslt.o.each(function (i) {
				$.ajax({
					async : false,
					type: 'POST',
					url: "./server.php",
					data : { 
						"operation" : "move_node", 
						"id" : $(this).attr("id").replace("node_",""), 
						"ref" : data.rslt.cr === -1 ? 1 : data.rslt.np.attr("id").replace("node_",""), 
						"position" : data.rslt.cp + i,
						"title" : data.rslt.name,
						"copy" : data.rslt.cy ? 1 : 0
					},
					success : function (r) {
						if(!r.status) {
							$.jstree.rollback(data.rlbk);
						}
						else {
							$(data.rslt.oc).attr("id", "node_" + r.id);
							if(data.rslt.cy && $(data.rslt.oc).children("UL").length) {
								data.inst.refresh(data.inst._get_parent(data.rslt.oc));
							}
						}
						$("#analyze").click();
					}
				});
			})
		.resizable();
	});

	//resizable
	//com maxWidth contendo o tamanho da tela menos o tamanho da div da lista menos 20
	$( "#demoResize" ).resizable({
		alsoResize: "#demo",
		handles   : 'e',
		maxWidth  : innerWidth 
				  - new Number(new String($('#contetLista').css('width')).replace('px','')) 
				  - 20
	});
	
	//slim scroll para a lista
	$('#lista').slimScroll({
		height: '479px',
		width: '680px'
	});
	

    $('#lista').contextMenu('menuLista', {
        bindings: {
          'nova': function(t) {
    		criarNovaPasta();
          }}
     });
	
	//carrega a lista inicial
	completePath['/'] = '/'; 
	chooseFolder(completePath['/'], '/');
	addPath(null, '/');
	
	$( "#dialog:ui-dialog" ).dialog( "destroy" );
	$( "#dialogNovaPasta" ).dialog({
        modal: true,
        resizable: false,
        height: 150,
        width: 450,
        draggable: false,
        buttons: [
                  {
                      text: "Ok",
                      click: function() {
                	    var nome = $('#nomeNovaPasta').val();
                	    $('#nomeNovaPasta').val('');
                	    if(nome != ''){
                	    	$.ajax(
                	    			{ 
                	    				"url"		: "./uploadArquivo.php",
                	    				"type"		: "POST",
                	    				"data"      : {'operacao'  :'nova', 
                	    							   'nome'      : nome},
                	    				"async"		: true,
                	    				"dataType"  : 'json',
                	    				"success"   : function (r) {
            	    								   $("#dialogNovaPasta").dialog("close");            	    								    	
            	    								    if(r && r.success && r.success == 'true'){
            	    								    	//trata o retorno e atualiza os itens
            	    								    	r.path = r.path.split('\\').join('');
            	    								    	r.path = r.path.split('//').join('');
            	    								    	recarregaTela(r.path, r.path);
            	    								    }
                	    							  }
                	    			 });
                	    }
            	  	  }
                  }
              ]
    }).dialog( "close" );
	
	$( "#dialogExcluir" ).dialog({
		modal: true,
		resizable: false,
		height: 170,
		width: 450,
		draggable: false,
		buttons: [
		          {
		        	  text: "Sim",
		        	  click: function() {
		        		  $.ajax(
		        				  { 
		        					  "url"		 : "./uploadArquivo.php",
		        					  "type"	 : "POST",
		        					  "data"     : {'operacao'  :'excluir', 
		        					  'path'     : $(itemSelecionado).attr('id')},
		        					  "async"	 : true,
		        					  "dataType" : 'json',
		        					  "success"  : function (r) {
		        						  $("#dialogExcluir").dialog("close");            	    								    	
		        						  if(r && r.success && r.success == 'true'){
		        							  //trata o retorno e atualiza os itens
		        							  r.path = r.path.split('\\').join('');
		        							  r.path = r.path.split('//').join('');
		        							  recarregaTela(r.path, r.path);
		        						  }
		        					  }
		        				  });
		          	}
		          },{  
		        	  text: "Não",
		        	  click: function() {
						  $("#dialogExcluir").dialog("close");            	    								    	
		        	  }
		          	}
		          ]
	}).dialog( "close" );
	
	$( "#dialogExcluirVarios" ).dialog({
		modal: true,
		resizable: false,
		height: 170,
		width: 450,
		draggable: false,
		buttons: [
		          {
		        	  text: "Sim",
		        	  click: function() {
		        	  var arquivosExcluidos = new Array();
		        	  $('.itemListaDivSelected').each(function (){
		        		  arquivosExcluidos.push($(this).attr('id'));
		        	  });
		        	  
		        	  $.ajax(
		        			  { 
		        				  "url"		 : "./uploadArquivo.php",
		        				  "type"	 : "POST",
		        				  "data"     : {'operacao'  :'excluir', 
		        				  'path'     : arquivosExcluidos.join('|')},
		        				  "async"	 : true,
		        				  "dataType" : 'json',
		        				  "success"  : function (r) {
		        					  $("#dialogExcluirVarios").dialog("close");            	    								    	
		        					  if(r && r.success && r.success == 'true'){
		        						  //trata o retorno e atualiza os itens
		        						  r.path = r.path.split('\\').join('');
		        						  r.path = r.path.split('//').join('');
		        						  recarregaTela(r.path, r.path);
		        						  $('#imgExcluirDesab').show();
		        						  $('#imgExcluir').hide();
		        					  }
		        				  }
		        			  });
		          }
		          },{  
		        	  text: "Não",
		        	  click: function() {
		        	  	$("#dialogExcluirVarios").dialog("close");            	    								    	
		              }
		            }
		          ]
	}).dialog( "close" );
	
	$( "#dialogExcluirNenhum" ).dialog({
		modal: true,
		resizable: false,
		height: 170,
		width: 450,
		draggable: false,
		buttons: [
		             {  
		        	  text: "Fechar",
		        	  click: function() {
	        	          $("#dialogExcluirNenhum").dialog("close");            	    								    	
		          	  }
		             }
		         ]
	}).dialog( "close" );

	$( "#dialogRenomearNenhum" ).dialog({
		modal: true,
		resizable: false,
		height: 170,
		width: 450,
		draggable: false,
		buttons: [
		          {  
		        	  text: "Fechar",
		        	  click: function() {
		        	  $("#dialogRenomearNenhum").dialog("close");            	    								    	
		          }
		          }
		          ]
	}).dialog( "close" );
	
	$( "#dialogRenomear" ).dialog({
        modal: true,
        resizable: false,
        height: 150,
        width: 450,
        draggable: false,
        buttons: [
                  {
                      text: "Ok",
                      click: function() {
                	    var nome = $('#nomeRenomear').val();
                	    $('#nomeRenomear').val('');
                	    if(nome != ''){
                	    	$.ajax(
                	    			{ 
                	    				"url"		: "./uploadArquivo.php",
                	    				"type"		: "POST",
                	    				"data"      : {'operacao'  :'renomear', 
                	    							   'nome'      : nome,
                	    							   'path'      : $(itemSelecionado).attr('id')},
                	    				"async"		: true,
                	    				"dataType"  : 'json',
                	    				"success"   : function (r) {
            	    								   $("#dialogRenomear").dialog("close");            	    								    	
            	    								    if(r && r.success && r.success == 'true'){
            	    								    	//trata o retorno e atualiza os itens
            	    								    	r.path = r.path.split('\\').join('');
            	    								    	r.path = r.path.split('//').join('');
            	    								    	recarregaTela(r.path, r.path);
            	    								    }
                	    							  }
                	    			 });
                	    }
            	  	  }
                  }
              ]
    }).dialog( "close" );
	
	// lógica para remover a classe de seleção quando clicar para navegar nas pastas ou fazer download
	$('#lista div a').live('click', function(event){
		$(this).parent().parent().removeClass('itemListaDivSelected');
		verificaSelecionados();
	});
});


//Adiciona um novo item na lista
function addItemLista(item){
	$('#lista').append(createLine(item));

	adicionaMenuDireito();
}

function adicionaMenuDireito(){
    $('.itemListaDiv').contextMenu('menuItem', {
        bindings: {
          'nova': function(t) {
    		criarNovaPasta();
          },
          'renomear': function(t) {
        	  $('#nomeRenomear').val($(itemSelecionado).find('.itemLista').html());
    	      $( "#dialogRenomear" ).dialog('open');
            },
          'excluir': function(t) {
        	  $('#arqExcluir').html($(itemSelecionado).find('.itemLista').html());
        	  $( "#dialogExcluir" ).dialog('open');
          }
        }
     });
    
    //quando clicado com o botão direito, seleciona o elemento
    $('.itemListaDiv').mousedown(function(e) {
        if (e.which === 3) {
        	itemSelecionado = this;
        }
    });
}

//cria uma nova linha
function createLine(item){
	var tagA         = document.createElement('a');
	var tagImg       = document.createElement('img');
	var tagDiv       = document.createElement('div');
	tagImg.innerHTML = '&nbsp;';
	item.attr.path   = item.attr.path.split('//').join('/');
	item.attr.path   = item.attr.path.split('//').join('/');
	if ( item.attr.rel == 'folder' ){
		$(tagA).click(function () { chooseFolder(item.attr.id, item.attr.path);});
		completePath[item.attr.path] = item.attr.id;
	}else{
		$(tagA).click(function () { document.location = item.attr.href;});
	}
	
	$(tagDiv).click(function () { chooseItem(item.attr.id, this);});

	tagImg.src = './css/dropbox-icons/' + item.attr.icon + '.gif';
	
	$(tagA).addClass('itemLista');
	$(tagA).append(item.data.title);
	
	var tagDivA = document.createElement('div');
	$(tagDivA).addClass('dscItemLista');
	$(tagDivA).append(tagImg);
	$(tagDivA).append(tagA);
	
	$(tagDiv).addClass('itemListaDiv');
	tagDiv.setAttribute('id', item.attr.path);
	$(tagDiv).append(tagDivA);

	var tagA = document.createElement('a');
	$(tagA).append(item.attr.date);
	$(tagA).addClass('dataItemListaDiv');
	$(tagDiv).append(tagA);

	var tagA = document.createElement('a');
	$(tagA).append(item.attr.modified);
	
	$(tagDiv).append(tagA);
	
	return tagDiv;
}

function criarNovaPasta(){
	$( "#dialogNovaPasta" ).dialog('open');
}

function renomearArquivo(){
	itemSelecionado = $('.itemListaDivSelected')[0];
	$('#nomeRenomear').val($(itemSelecionado).find('.itemLista').html());
    $( "#dialogRenomear" ).dialog('open');
}

function recarregaTela(id, path){
	chooseFolder(id, path);
	var tree = jQuery.jstree._reference("#demo");
	tree.refresh();
}

//Adiciona o caminho do item na div
function addPath(item, path){
	var path     = path.split('//').join('/');
	path     	 = path.split('/');
	var pathTmp  = '';
	var aPathTmp = new Array();
	var tagA;
	var tagDiv;
	$('#caminho').html('');
	$(path).each(function (k, value){
		if(value != ''){
			//o segundo item já possui a /
			if( k != 1){
				pathTmp    += '/';
			}
			pathTmp    += value;
			aPathTmp[k] = pathTmp;

			tagA        = document.createElement('a');
			$(tagA).append(value);
			tagDiv      = document.createElement('div');
			$(tagDiv).click(function () { chooseFolder(completePath[aPathTmp[k]], aPathTmp[k]);});
			$(tagDiv).addClass('itemPath');
			$(tagDiv).append(tagA);
			$('#caminho').append(tagDiv);
		}else if (k == 0){
			pathTmp     = '/';
			aPathTmp[k] = pathTmp;
			tagA        = document.createElement('a');
			$(tagA).append('Início');
			tagDiv      = document.createElement('div');
			$(tagDiv).click(function () { chooseFolder(completePath[aPathTmp[k]], aPathTmp[k]);});
			$(tagDiv).addClass('itemPath');
			$(tagDiv).append(tagA);
			$('#caminho').append(tagDiv);
		}

		if(k != (path.length - 1) && path[k + 1] != ''){
			tagDiv = document.createElement('div');
			$(tagDiv).append('&#187;');
			$(tagDiv).addClass('itemPathNext');
			$('#caminho').append(tagDiv);
		}
		
	});
}

//Seleciona uma pasta na lista
function chooseFolder(id, path){
	carregando('on', $('#lista'));

	$.ajax(
			{ 
				"url"		: "./server.php",
				"type"		: "POST",
				"data"      : {'type'      :'list', 
							   'id'        : id.replace("node_",""), 
							   'operation' : 'list_node'},
				"async"		: true,
				"success"   : function (r) {
								carregando('off', $('#lista'));
								//reset a lista
								$('#lista').html('');
								//cria o caminho
								addPath(null, path);
								$(r).each(function (k, item){
									//adiciona os itens
									addItemLista(item);
								});
								
						    	verificaSelecionados();
							  }
			 });
}

//função para transformar uma linha de upload em uma linha de download
function replaceLine(param, remove){
	//flag para substituir o primeiro e remover os demais itens encontrados
	var firstMatch = true;

	$('.itemListaDiv:contains(' + param.item.data.title + ')').each(function(k, elem){
    	//faz verificação para pegar itens IGUAIS e não que CONTENHAM
    	//lógica para evitar pegar a linha de um arquivo com o nome semelhante
    	if( $(elem).find('.itemLista').html()      ==  param.item.data.title 
    	 || $(elem).find('.qq-upload-file').html() == param.item.data.title ){
    		//substitui o primeiro e remove os demais
    		if ( firstMatch ){
    			$(createLine(param.item)).replaceAll(elem);
    		}else{
    			if(remove){
    				$(elem).remove();
    			}else{
    				$(elem).hide();
    			}
    		}
    		firstMatch = false;
    	}
    });
}

//Função para exibir div de carregamento
//status on|off
//elem Elemento onde a div será carregada
function carregando(status, elem){
	if ( status == 'on' ){
		var d = document;
		var div, span, img, h, w, topImg;

		h = new Number(elem.height());
		w = new Number(elem.width());

		topImg = (h/4);
		
		if (!jQuery("#temporizador1")[0]){
			div = d.createElement("div");
		}else{
			div = jQuery("#temporizador1");		
			jQuery(div).remove(div);
		}
		
		// Monta Span
		if (jQuery("span", div).length == 0){
			span = d.createElement("span");
		}else{
			span = jQuery("span", div).eq(0);
			jQuery("span", div).remove();
		}
		
		jQuery(span).attr({
			id : 'spanCarregando'
		})
		.css({
			'position' : 'relative',
			'top'	   : topImg + 'px'
		})
		.append('<center>Carregando...</center>');
		// Monta Imagem
		if (jQuery("img", span).length == 0){
			img = d.createElement("img");
		}else{
			img = jQuery("img", span).eq(0);
			jQuery("img", span).eq(0).remove();
		}
		
		jQuery(img).attr({
			src : 'js/tree/themes/default/throbber.gif'
		});
		jQuery("center", span).before(img);
		
		// Insere span com img na div, e prepara a mesma
		jQuery(div).append(span)
		.attr({
			id : 'temporizador1'			
		})
		.css({
			'-moz-opacity' : '0.8', 
			'filter' 	   : 'alpha(opacity=80)', 
			'background'   : '#ffffff',
			'text-align'   : 'center',
			'position' 	   : 'absolute', 
			'top'		   : '0px', 
			'width'		   : w + 'px', 
			'height' 	   : h + 'px', 
			'z-index' 	   : '9999'				
		});
		
		// Insere a div	
		jQuery(elem).append(div);
		
		return;
	}else if(status == 'off'){
		jQuery('#temporizador1').hide(300, function (){jQuery('#temporizador1').remove();});  
	}
}

function chooseItem(id, elem){
	if($(elem).hasClass('itemListaDivSelected')){
		$(elem).removeClass('itemListaDivSelected');
	}else{
		$(elem).addClass('itemListaDivSelected');
	}
	
	verificaSelecionados();
}

function verificaSelecionados(){
	if($('.itemListaDivSelected').length > 0){
		$('#imgExcluirDesab').hide();
		$('#imgExcluir').show();
	}else{
		$('#imgExcluirDesab').show();
		$('#imgExcluir').hide();
	}

	if($('.itemListaDivSelected').length == 1){
		$('#imgRenomearDesab').hide();
		$('#imgRenomear').show();
	}else{
		$('#imgRenomearDesab').show();
		$('#imgRenomear').hide();
	}
}

function ordenar(tipo, elem){
	carregando('on', $('#lista'));
	$.ajax(
			{ 
				"url"		: "./server.php",
				"type"		: "POST",
				"data"      : {'type'      :'list', 
							   'order'     : tipo, 
							   'operation' : 'list_node'},
				"async"		: true,
				"success"   : function (r) {
								carregando('off', $('#lista'));
								//reset a lista
								$('#lista').html('');
								//cria o caminho
								$(r).each(function (k, item){
									//adiciona os itens
									addItemLista(item);
								});
								
								$('.orderSelecionado').removeClass('orderSelecionado');
								$(elem).addClass('orderSelecionado');
							  }
			 });
}