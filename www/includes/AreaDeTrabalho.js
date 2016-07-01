

AreaDeTrabalho = {
	blackOverlay: jQuery('<div/>')
	    .attr('id', 'overlay')
	    .css({
	        'position': 'fixed',
	        'width': '100%',
	        'height': '100%',
	        'background-color': 'black',
	        'top': 0,
	        'left': 0,
	        'z-index': 1001,
	        'opacity': .75,
	        'display': 'none'
    	}),
    boxOverlay: jQuery('<div/>')
    	.attr({
    		id:'boxOverlay',
    		class:'boxOverlay'
    	})
    	.css({
    		'position':'fixed',
    		'margin-left':'-30%',
    		'left':'50%',
    		'width':'60%',
    		'height':'80%',
    		'top':'30px',
    		'z-index':1002,
    		'display':'none',
    		'-webkit-border-radius': '5px',
			'-moz-border-radius': '5px',
			'border-radius': '5px',
			'overflow': 'auto'
    	}),
    boxOverlayTransparent: jQuery('<div/>')
    	.attr({
    		id:'boxOverlayTransparent'
    	})
    	.css({
    		'position':'fixed',
    		'margin-left':'-30%',
    		'left':'50%',
    		'width':'60%',
    		'height':'80%',
    		'top':'30px',
    		'background':'#ccc',
    		'z-index':1001,
    		'display':'none',
    		'-webkit-border-radius': '5px',
			'-moz-border-radius': '5px',
			'border-radius': '5px',
			'opacity': .75,
    	}),
    botaoConfiguracoes: jQuery('<div/>')
		.css({
			'float':'right',
			'margin-top':'-25px',
			'font-size':'16px',
			'font-weight':'bold',
			'cursor':'pointer'
		})
		.attr({
			'class': 'personalizar'
		})
		.hover(function( event ){
			jQuery(this).css({
				'text-decoration':'underline'
			});
		},function( event ){
			jQuery(this).css({
				'text-decoration':'none'
			});
		})
		.html('<img src="../imagens/menu/bt_menu_sistema.png">&nbsp;Configura&ccedil;&otilde;es'),
	init: function( mnuid, usucpf, idDraggable, idDropabble, tamanho_icone, slideShows, sisid, elementosPorPagina ){

		var that = this;

		this.dropabble = jQuery('#'+idDropabble);
		this.draggable = jQuery("#"+idDraggable);
		this.draggableArea = jQuery("#draggable_area");
		this.elementosPorPagina = elementosPorPagina;

		this.dropabble
			.prepend('<input name="usucpf" id="usucpf" type="hidden" value="'+usucpf+'" />')
			.prepend('<input name="sisid" id="sisid" type="hidden" value="'+sisid+'" />');

		// constroi cesto de lixo
		this.draggableArea
			.prepend("<div id='cesto_lixo' style='margin-bottom:20px;height:30px;background:url(\"../imagens/garbage_little.png\") center no-repeat #e36c6c;background-size:18px 19px'></div>")
			.prepend( this.botaoConfiguracoes );

		jQuery('.personalizar')
			.click(function(){
				that.displayOptionsModal( that.draggable );
				setTimeout(function(){
					that.boxOverlay.bind("clickoutside",function(){
			        	that.fadeOutFullList();
			        });
				},300);
			});

		// elemento da lista geral
		this.draggable.find("img")
			.draggable({
				appendTo: "body",
				helper: "clone",
				drag: function( event, ui ){
					that.fadeOutFullList();
				}
			});

		// caixa da lista selecionada
		this.dropabble
			.droppable({
				accept: '.ferramenta',
				drop: function( event, ui ){
					var elementosPreExistentes = jQuery(this).find('.ferramenta[mnuid="'+ui.draggable.attr('mnuid')+'"]');
					if( elementosPreExistentes.length < 1 ){
						var clone = ui.draggable.clone();
						clone.attr({
								mnuid:  jQuery('#mnuid').val(),
								usucpf: jQuery('#usucpf').val(),
								sisid: jQuery('#sisid').val() });
						var cloneContainer = jQuery('<div />')
							.css({
								width:'10%',
								height:'150px',
								float:'left',
								margin:'0',
								padding:'0',
								'text-align':'center'
							});
						cloneContainer.append( clone );

						var paginadisponivel = that.verificaDisponibilidadeDasPaginas();
						if( paginadisponivel.disponibilidade == 'n' ){
							that.criaPaginaNovaEAplicaElementoNovoNela( paginadisponivel.numero, cloneContainer, jQuery(this) );
						}else{
							cloneContainer.appendTo( jQuery(this).find('[id^=container_Pag_'+(paginadisponivel.numero)+']') );
						}

						that.fazDraggableDeSelecionados( clone )
						that.salvaMenu( clone );
						that.remontaListaFerramentas();
						that.disponibilizaPaginas();
					}else{
						that.redAlert();
					}
				}
			});


		// elemento da lista selecionada
		this
			.fazDraggableDeSelecionados( this.dropabble.find(".ferramenta") );

		
		jQuery('#cesto_lixo')
			.droppable({
				accept: '.ferramenta',
				drop: function( event, ui ){
					var clone = ui.draggable.clone();
					ui.draggable.parent().remove();
					ui.draggable.remove();
					// trata caso de ultimo 
						jQuery('<img id="extra_vazio" width="'+ui.draggable.attr('width')+'" height="'+ui.draggable.attr('width')+'"/>').appendTo( that.dropabble );
					jQuery('#extra_vazio').remove();

					that.retiraMenu( clone );

					that.remontaListaFerramentas();
					that.limpaPaginas();
				}
			});

		this.disponibilizaPaginas();

	},
	criaPaginaNovaEAplicaElementoNovoNela: function( paginadisponivel, cloneContainer, elementoDropabble ){
		// cria pagina
		var paginaNova = jQuery('<div />')
			.attr({
				pagina: paginadisponivel,
				id: 'container_Pag_'+paginadisponivel,
				'class':'bloco'
			})
			.css({
				display: 'none',
				'padding-top': '10px'
			});
		paginaNova.append( cloneContainer );
		elementoDropabble.append( paginaNova );

		this.disponibilizaPaginas();
	},
	limpaPaginas: function(){
		jQuery.each( jQuery('[id^=container_Pag_]'), function( index, value ){
			if( jQuery( value ).find('div').length < 1 ){
				jQuery('.paginaLink[pagina='+(index+1)+']').remove();
			}
		});
	},
	verificaDisponibilidadeDasPaginas: function(){
		var resposta = {};
		var that = this;
		jQuery.each( jQuery('[id^=container_Pag_]'), function( index, value ){
			if( jQuery( value ).find('div').length < that.elementosPorPagina ){
				resposta = {disponibilidade:'s',numero:(index+1)};
				return false;
			}

			if( index+1 == jQuery('[id^=container_Pag_]').length && !resposta.disponibilidade ){
				resposta = {disponibilidade:'n',numero:(jQuery('[id^=container_Pag_]').length+1)};
			}
		});

		return resposta;
	},
	disponibilizaPaginas: function(){
		if( jQuery('#listaPaginas') ){
			jQuery('#listaPaginas').remove();
		}

		var that = this;
		var numeroPaginas = jQuery('[id^=container_Pag_]').length;
		var itensPaginas = "";
		if( numeroPaginas > 1 ){

			for (var i = 0; i < numeroPaginas; i++) {
				if( jQuery('[id^=container_Pag_'+(i+1)+']').css('display') != 'none' ){
					itensPaginas += "<img src='../imagens/icones/bg.png' class='paginaLink' pagina='"+(i+1)+"' />&nbsp;&nbsp;";
				}else{
					itensPaginas += "<img src='../imagens/icones/bg-free.png' class='paginaLink' pagina='"+(i+1)+"' style='cursor:pointer' />&nbsp;&nbsp;";
				}
			};

			var paginas = jQuery("<div />")
				.attr({
					id: 'listaPaginas'
				})
				.css({
					clear:'both',
					margin: '0 auto',
					width: '100%',
					'text-align': 'center'
				})
				.html( itensPaginas );
			this.draggableArea
				.append( paginas );

		}

		jQuery('.paginaLink').click(function(){
			
			that.mudaPagina( this );

		});
	},
	mudaPagina: function( elementoClicado ){
		// some com todos e aparece com o clicado
		jQuery('.bloco').hide();
		jQuery('#container_Pag_'+jQuery(elementoClicado).attr('pagina') ).show();

		// muda imagem paginador
		jQuery.each( jQuery('[id^=container_Pag]'), function( index, value ){
			var linkPagina = jQuery('.paginaLink[pagina='+(jQuery(this).attr('pagina'))+']');
			if( linkPagina.attr('src') == '' ){
				linkPagina
					.css({
						border:'2px solid #000',
						cursor:'pointer'
					})
					.attr({
						secretSrc: '../imagens/icones/bg-free.png'
					});
			}else{
				linkPagina
					.attr({src: '../imagens/icones/bg-free.png'})
					.css({cursor:'pointer'});
			}
		});

		// muda imagem paginador
		if( jQuery(elementoClicado).attr('src') == '' ){
			jQuery(elementoClicado)
				.css({
					border:'2px solid green',
					cursor:'pointer'
				})
				.attr({
					secretSrc: '../imagens/icones/bg.png'
				});
		}else{
			jQuery(elementoClicado)
				.attr({src: '../imagens/icones/bg.png'})
				.css({cursor:'default'});
		}
	},
	// selecionados sao os elementos que serao salvos como escolhas do usuario
	fazDraggableDeSelecionados: function( elemento ){
		var that = this;
		
		jQuery( elemento ).draggable({
			appendTo: "body",
			helper: "clone",
			start: function( event, ui ){
				jQuery('#cesto_lixo').animate({
					height:'60px',
					'background-size':'50px 51px'});
				that.disponibilizaDropNasPaginas();
			},
			stop: function( event, ui ){
				jQuery('#cesto_lixo').animate({
					height:'30px',
					'background-size':'18px 19px'
				});
				that.indisponibilizaDropNasPaginas();
			}
		});

		this.fazDroppableDeSelecionados( elemento );

	},
	fazDroppableDeSelecionados: function( elemento ){
		var that = this;

		jQuery( elemento )
			.droppable({
				over: function( event, ui ){
					jQuery(this).css({'border-bottom':'2px solid #000'});
				},
				out: function( event, ui ){
					jQuery(this).css({'border-bottom':''});
				},
				drop: function( event, ui ){
					event.stopPropagation();
					jQuery(this).css({'border-bottom':''});
					
					that.trocaDePosicaoDoisElementos( jQuery(this).parent().get(0), ui.draggable.parent().get(0) );
				}
			});
	},
	trocaDePosicaoDoisElementos: function( elementoBase, elementoMovel ){
		var paginaElementoBase = jQuery(elementoBase).parent().attr('id');
		var paginaElementoMovel = jQuery(elementoMovel).parent().attr('id');
		
		if( paginaElementoBase == paginaElementoMovel ){
			this.trocaDePosicaoEntreElementosDeMesmaPagina( elementoBase, elementoMovel );
		}else{
			this.trocaDePosicaoEntreElementosDePaginasDiferentes( elementoBase, elementoMovel );
		}
	},
	trocaDePosicaoEntreElementosDePaginasDiferentes: function( elementoBase, elementoMovel ){
		var listaFerramentas = jQuery(elementoBase).parent().parent().find('.ferramenta');

		var posicaoElementoBase = jQuery(elementoBase).parent().parent().find('.ferramenta').index(jQuery(elementoBase).find('img'));
		var posicaoElementoMovel = jQuery(elementoMovel).parent().parent().find('.ferramenta').index(jQuery(elementoMovel).find('img'));

		var posicaoElementoDepoisBase = jQuery(elementoBase).parent().parent().find('.ferramenta').index(jQuery(elementoBase).find('img'));

		if( posicaoElementoBase > posicaoElementoMovel ){
			jQuery(elementoBase).after( jQuery(elementoMovel) );
		}else{
			jQuery(elementoBase).before( jQuery(elementoMovel) );
		}

		this.remontaListaFerramentas();
	},
	remontaListaFerramentas: function(){
		var listaFerramentas = this.dropabble.find('.ferramenta');
		var paginas = jQuery('.bloco');
		var contadorFerramentas = 0;
		for( var i = 0; i < paginas.length; i++ ){
			for (var i2 = 0; i2 <= 1; i2++) {
				jQuery(listaFerramentas[contadorFerramentas]).parent().appendTo( paginas[i] );
				contadorFerramentas++;
			}
		}

		this.salvaOrdemElementos();
	},
	salvaOrdemElementos: function(){
		var that = this;
		
		jQuery.ajax({
			url: window.location.href,
			type: 'POST',
			data: {req:'aplicaEditabilidadeDragDropSlideShow',method:'salvarOrdemLista',params:that.preparaPacoteLista()},
			success: function( resposta ){
				if( resposta.substring( resposta.length, resposta.length-2 ) == 'ok' ){
					// that.greenAlert();
				}else{
					// that.redAlert();
				}
			}
		});
	},
	preparaPacoteLista: function(){
		var pacote = [];
		jQuery.each( this.dropabble.find('.ferramenta'), function( index, value ){
			pacote[index] = {
				mnuid: jQuery(value).attr('mnuid'),
				sisid: jQuery(value).attr('sisid'),
				usucpf: jQuery(value).attr('usucpf') };
		});
		return pacote;
	},
	trocaDePosicaoEntreElementosDeMesmaPagina: function( elementoBase, elementoMovel ){
		var posicaoElementoBase = jQuery(elementoBase).parent().parent().find('.ferramenta').index(jQuery(elementoBase).find('img'));
		var posicaoElementoMovel = jQuery(elementoMovel).parent().parent().find('.ferramenta').index(jQuery(elementoMovel).find('img'));

		if( posicaoElementoBase > posicaoElementoMovel ){
			jQuery(elementoMovel).before( jQuery(elementoBase) );
		}else{
			jQuery(elementoMovel).after( jQuery(elementoBase) );
		}

		this.remontaListaFerramentas();
	},
	disponibilizaDropNasPaginas: function(){
		var that = this;

		jQuery.each( jQuery('.paginaLink'), function( index, value ){
			var srcPresente = jQuery(value).attr('src');

			if( srcPresente.indexOf('free') == -1 ){
				jQuery(value).css({background:'green'});
			}

			jQuery(value)
				.css({
					width:'80px',
					height:'80px',
					border:'2px solid #000'
				})
				.attr({
					secretSrc: srcPresente,
					src: ''
				})
				.droppable({
					over: function( event, ui ){
						jQuery(this).css({border:'2px solid green'});
						that.mudaPagina( this );
					},
					out: function( event, ui ){
						jQuery(this).css({border:'2px solid #000'});
					},
					drop: function( event, ui ){
						that.mudaPosicaoElementoParaPagina( ui.draggable, jQuery(this) );
					}
				});
		});

	},
	mudaPosicaoElementoParaPagina: function( elemento, pagina ){
		var ultimoElementoDaPagina = jQuery('.bloco[pagina='+pagina.attr('pagina')+']').find('div').last();
		ultimoElementoDaPagina.after( elemento.parent() );

		this.remontaListaFerramentas();
	},
	indisponibilizaDropNasPaginas: function(){
		jQuery('.paginaLink').each(function( index, value ){
			var srcPresente = jQuery(value).attr('secretSrc');
			jQuery(value)
				.css({
					width:'9px',
					height:'14px',
					border:'0',
					background:'none'
				})
				.attr({
					src: srcPresente
				})
				.droppable({
					drop: function( event, ui ){
						// console.log(ui);
					}
				});
		});
	},
	// remonta slideshow para disponibilizar para o usuário
	refazSlideShow: function( slideShows ){
		
		if( slideShows instanceof Array ){
			jQuery.each( slideShows, function( key, value ){
				jQuery("#"+value).carouFredSel({
				    circular: false,
				    infinite: false,
				    auto    : false,
					width	: jQuery("#"+value).css('width').substring( 0, jQuery("#"+value).css('width').length-2 ),
				    prev    : {
				        button  : "#"+value+"_prev",
				        key     : "left"
				    },
				    next    : {
				        button  : "#"+value+"_next",
				        key     : "right"
				    }
				});
			});
		}else{
			jQuery("#"+slideShows).carouFredSel({
			    circular: false,
			    infinite: false,
			    auto    : false,
				width	: jQuery("#"+slideShows).css('width').substring( 0, jQuery("#"+slideShows).css('width').length-2 ),
			    prev    : {
			        button  : "#"+slideShows+"_prev",
			        key     : "left"
			    },
			    next    : {
			        button  : "#"+slideShows+"_next",
			        key     : "right"
			    }
			});
		} 

	},
	salvaMenu: function( elemento ){

		var that = this;

		var paramsObject = {};
		paramsObject.mnuid = elemento.attr('mnuid');
		paramsObject.sisid = elemento.attr('sisid');
		paramsObject.usucpf = elemento.attr('usucpf');
		paramsObject.posicao = elemento.parent().parent().parent().find('.ferramenta').index( jQuery('[mnuid='+elemento.attr('mnuid')+']') );

		jQuery.ajax({
			url: window.location.href,
			type: 'POST',
			data: {req:'aplicaEditabilidadeDragDropSlideShow',method:'salvarListaSelecionada',params:paramsObject},
			success: function( resposta ){
				// console.log( resposta );
				if( resposta.substring( resposta.length, resposta.length-2 ) == 'ok' ){
					that.greenAlert();
				}else{
					that.redAlert();
				}
			}
		});
	},
	retiraMenu: function( elemento ){

		var that = this;

		var paramsObject = {};
		paramsObject.mnuid = elemento.attr('mnuid');
		paramsObject.sisid = elemento.attr('sisid');
		paramsObject.usucpf = elemento.attr('usucpf');

		jQuery.ajax({
			url: window.location.href,
			type: 'POST',
			data: {req:'aplicaEditabilidadeDragDropSlideShow',method:'retiraDaListaSelecionada',params:paramsObject},
			success: function( resposta ){
				// console.log( resposta );
				if( resposta.substring( resposta.length, resposta.length-2 ) == 'ok' ){
					that.greenAlert();
				}else{
					that.redAlert();
				}
			}
		});
	},
	displayOptionsModal: function( options ){

        options.css('z-index', 1003);

        jQuery('body').css('text-align','center');
        jQuery('body').append( this.blackOverlay );
		this.blackOverlay.fadeIn('fast');

		jQuery('body').append( this.boxOverlay );
		jQuery('body').append( this.boxOverlayTransparent );
        this.boxOverlay.append( options );

		this.boxOverlayTransparent.fadeIn('fast');
		options.fadeIn('fast');
		this.boxOverlay.fadeIn('fast');

	},
	redAlert: function(){
		this.dropabble
						.animate({backgroundColor:'red'})
						.animate({backgroundColor:'#e9e9e9'});
	},
	greenAlert: function(){
		this.dropabble
						.animate({backgroundColor:'green'})
						.animate({backgroundColor:'#e9e9e9'});
	},
	fadeOutFullList: function(){
		this.blackOverlay.fadeOut('fast').remove();
		this.boxOverlayTransparent.fadeOut('fast').remove();
    	this.boxOverlay.fadeOut('fast');
    	this.draggable.fadeOut('fast');
    	this.boxOverlay.unbind("clickoutside");

	}
};