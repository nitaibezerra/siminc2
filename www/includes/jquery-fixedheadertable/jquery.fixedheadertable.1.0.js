/*!
* jquery.fixedHeaderTable. The jQuery fixedHeaderTable plugin
*
* Copyright (c) 2009 Mark Malek
* http://fixedheadertable.mmalek.com
*
* Licensed under MIT
* http://www.opensource.org/licenses/mit-license.php
* 
* http://docs.jquery.com/Plugins/Authoring
* jQuery authoring guidelines
*
* Launch  : October 2009
* Version : 1.0 beta
* Released: TBA
*
* 
* all CSS sizing (width,height) is done in pixels (px)
*/
(function($)
{

	$.fn.fixedHeaderTable = function(options) {
		var defaults = {
			loader: false,
			footer: false,
			colBorder: true,
			cloneHeaderToFooter: false,
			autoResize: false,
			fixCol1: false,
			footerScroll: false
		};
		
		var options = $.extend(defaults, options); // get the defaults or any user set options
		
		return this.each(function() {
			var obj = $(this); // the jQuery object the user calls fixedHeaderTable on
			
			buildTable(obj,options);
			
			if(options.autoResize == true) {
				// if true resize the table when the browser resizes
				$(window).resize( function() {
					if (table.resizeTable) {
						// if a timeOut is active cancel it because the browser is still being resized
						clearTimeout(table.resizeTable);
					}
				
					// setTimeout is used for resizing because some browsers will call resize() while the browser is still being resized which causes performance issues.
					// if the browser hasn't been resized for 200ms then resize the table
					table.resizeTable = setTimeout(function() {
					
						buildTable(obj,options);
						
					}, 200);
				});
			}
		});
	};
	
	var table = function() {
		this.resizeTable;
	}
	
	function buildTable(obj,options) {
		var objClass = obj.attr('class');
			
		var hasTable = obj.find("table").size() > 0; // returns true if there is a table
		var hasTHead = obj.find("thead").size() > 0; // returns true if there is a thead
		var hasTBody = obj.find("tbody").size() > 0; // returns true if there is a tbody
			
		if(hasTable && hasTHead && hasTBody) {
			var parentDivWidth = obj.width() - 5; // get the width of the parent DIV
			var parentDivHeight = obj.height() - 5; // get the height of the parent DIV
			var tableBodyWidth = parentDivWidth; // width of the div surrounding the tbody (overflow:auto)
			
			obj.css('position', 'relative'); // set the jQuery object the user passsed in to position:relative (just incase they did not set it in their stylesheet)
			
			if (obj.find('.fht_parent').size() == 0) {
				// if returns false then the plugin has not been used on this jQuery object
				obj.find('table').wrap('<div class="fht_parent"><div class="fht_table_body"></div></div>');
			}
			obj.find('.fht_parent').css('width', parentDivWidth+'px'); // set the width of the parent div
			obj.find('.fht_table_body').css('width', parentDivWidth+'px'); // set the width of the main table body (where the data will be displayed)
			
			var tableWidthNoScroll = parentDivWidth; // this is the width of the table with no scrollbar (used for the fixed header)
			
			obj.find('.fht_parent .fht_table_body table').addClass('fht_orig_table'); // add a class to identify the orignal table later on
			
			if(options.loader) {
				// if true display a loading image while the table renders (default is false)
				obj.find('.fht_parent').prepend('<div class="fht_loader"></div>');
				obj.find('.fht_loader').css({'width':parentDivWidth+'px', 'height': parentDivHeight+'px'});
			}
			
			obj.attr('id', obj.attr('class'));
			var tableWidthScroll = parentDivWidth;
			
			if(options.fixCol1) {
				if (obj.find('.fht_parent .fht_fixed_col_fulltable').size() > 0 == false) {
					obj.find('.fht_parent').prepend('<div class="fht_fixed_col_fulltable"></div>');
					obj.find('.fht_parent').prepend('<div class="fht_fixed_col"></div>');
					
					obj.find('.fht_parent .fht_table_body').prependTo('.fht_parent .fht_fixed_col_fulltable');
				}
			}
			
			
			
			if (options.fixCol1) {
				tableWidthScroll = tableWidthScroll - obj.find('.fht_parent .fht_orig_table tbody tr:first td:first-child').width();
			}
			else {			
				if ($.browser.msie == true) {
					// if IE subtract an additional 2px from the table to compensate for the scrollbar.  Allows the outside border of the table to be flush with the outside border of the fixed header
					tableWidthScroll = tableWidthScroll - 20; // default for IE
				}
				else if (jQuery.browser.safari == true) {
					// if Safari subtract 14px to compensate for the scrollbar
					tableWidthScroll = tableWidthScroll - 16; // default for Safari
				}
				else {
					// if everything else subtract 15px to compensate for the scrollbar
					tableWidthScroll = tableWidthScroll - 19; // default for everyone else
				}
			}

			obj.find('table.fht_orig_table').css({'width': tableWidthScroll+'px'}); // set the width of the table minus the scrollbar
			obj.find('table tbody tr:even td').addClass('even');
			obj.find('table tbody tr:odd td').addClass('odd');
			
			// Highlighting
			/*obj.find('table tbody tr').mouseover(function() {
				$('td', this).addClass('highlight');
			});
			obj.find('table tbody tr').mouseout(function() {
				$('td', this).removeClass('highlight');
			});*/
			// end Highlighting
			
//			obj.find('table tbody tr:even').addClass('even'); // para fazer as tr brilharem basta descomentar estas duas linhas e comentar as duas linhas acima
//			obj.find('table tbody tr:odd').addClass('odd');

			if (obj.find('table tbody tr td div.tableData').size() > 0 == false) {
				obj.find('table tbody tr td').wrapInner('<div class="tableData"><p class="tableData"></p></div>');
			}
			else {
				obj.find('table tbody tr td div.tableData').css('width','auto');
			}
			
			obj.find('table.fht_orig_table thead tr').css('display', '');
			
			if (obj.find('table thead tr th div.tableHeader').size() > 0 == false) {
				obj.find('table thead th').wrapInner('<div class="tableHeader"><p class="tableHeader"></p></div>');
			}
			else {
				obj.find('div.tableHeader').css('width', 'auto');
			}
			
			if (options.colBorder) {
				obj.find('.fht_parent table tr td:not(:last-child)').addClass('borderRight');
				obj.find('.fht_parent table tr th:not(:last-child)').addClass('borderRight');
			}
			
			obj.find('.fht_fixed_header_table_parent').remove();
			
			var html = "";
			html += "<div class='fht_fixed_header_table_parent'>"; // wraps around the entire fixed header
			html += "<!--[if IE]><div class='fht_top_right_header'></div><![endif]-->"; // adds a rounded corner to the top right of the header
			html += "<!--[if IE]><div class='fht_top_left_header'></div><![endif]-->"; // adds a rounded corner to the top left of the header
			html += "<div class='fht_fixed_header_table_border'>"; // creates the border for the header
			html += "<table class='fht_fixed_header_table'>"; // holds the thead that is cloned from the original table body
			html += "</table></div></div>"; // close all open div's and table tags
			
			if (options.fixCol1) {
				obj.find('.fht_fixed_col_fulltable').prepend(html);
			}
			else {
				obj.find('.fht_parent').prepend(html); // add the html output to the beginning of the parent div
			}
			
			//obj.find('.fht_parent').prepend(html); //isso pode ser removido. Está dando erro quando gera a primeira coluna fixa
			
			obj.find('.fht_fixed_header_table_border').css('width', tableWidthScroll + 'px');
			
			obj.find('.fht_fixed_header_table_parent').css('width', parentDivWidth+'px');
			obj.find('table.fht_fixed_header_table').empty();
			
			obj.find('.fht_parent .fht_orig_table thead').clone().prependTo('.' + objClass + ' .fht_fixed_header_table');
			
			obj.find('table.fht_fixed_header_table').css({'width': tableWidthScroll+'px'});

			var x = 0;
			var widthHidden = new Array();
			obj.find('.fht_parent table.fht_orig_table th').each(function() {
				if($(this).hasClass('th'+x) == false) {
					$(this).addClass('th'+x); // used to identify which column we are looking at
				}
				
				widthHidden[x] = $(this).width();
				x++;
			});
			
			var i = 0;
			var width = new Array();
			obj.find('.fht_parent table.fht_fixed_header_table th').each(function() {
				if($(this).hasClass('th'+i) == false) {
					$(this).addClass('th'+i);
				}
				width[i] = widthHidden[i];
				i++;
			});
			
			if(obj.find('table.fht_orig_table tbody tr td:first-child').hasClass('firstCell') == false) {
				obj.find('table.fht_orig_table tbody tr td:first-child').addClass('firstCell');
			}
			
			var thCount = 0;
			var thWidth;
			var tdWidth;
			obj.find('table.fht_orig_table tbody tr:first td').each(function() {
				
				if ($(this).hasClass('firstCell')) {
					thCount = 0;
				}
				
				thWidth = width[thCount];
				tdWidth = $(this).width();
		
				$(this).children('div.tableData').css('width',thWidth+'px');
				
				
				obj.find('.fht_parent table.fht_fixed_header_table th.th'+thCount+' div.tableHeader').css('width', thWidth+'px');
				
				thCount++;
			});
			
			var footerHeight = 0;
			
			if (options.footer && !options.cloneHeaderToFooter) {
				if (!options.footerId) {
						// notify the developer they wanted a footer and didn't provide content
						$('body').css('background', '#f00');
						alert('Footer ID required');
				}else{
					var footerId = options.footerId;
					if (obj.find('.fht_fixed_footer_border').size() == 1) {
						var footerContent = obj.find('.fht_fixed_footer_border').html();
					}else{
						$('#'+footerId).appendTo('.fht_parent');
						
						var footerContent = obj.find('#'+footerId).html();
					}
					obj.find('#'+footerId).empty();
					obj.find('#'+footerId).prepend('<div class="fht_cloned_footer"><!--[if IE 6]><div class="fht_bottom_left_header"></div><div class="fht_bottom_right_header"></div><![endif]--><div class="fht_fixed_footer_border"></div></div>');
					obj.find('.fht_fixed_footer_border').html(footerContent);
					obj.find('.fht_cloned_footer').css('width', obj.find('.fht_fixed_header_table_parent').width()+'px');
					obj.find('#'+footerId).css({'height': obj.find('#'+footerId).height() + 'px', 'width': obj.find('.fht_fixed_header_table_parent').width()+'px'});
					footerHeight = obj.find('#'+footerId).height();
					
				}
			}
			else if (options.footer && options.cloneHeaderToFooter) {
				
				// if footer is true and cloneHeaderToFooter is true. Clone the fixed header as a fixed footer
				obj.find('.fht_parent .fht_cloned_footer').remove(); // remove any previously genereated cloned footer
				
				var html = "";
				html += "<div class='fht_cloned_footer'>"; // wraps around the entire fixed header
				html += "<!--[if IE]><div class='fht_bottom_right_header'></div><![endif]-->"; // adds a rounded corner to the top right of the header
				html += "<!--[if IE]><div class='fht_bottom_left_header'></div><![endif]-->"; // adds a rounded corner to the top left of the header
				html += "<div class='fht_fixed_footer_border'>"; // creates the border for the header
				html += "</div></div>"; // close all open div's and table tags
	
				obj.find('.fht_parent').append(html);

				obj.find('.fht_parent .fht_fixed_header_table_parent .fht_fixed_header_table_border table').clone().prependTo('.' + objClass + ' .fht_cloned_footer .fht_fixed_footer_border');
				obj.find('.fht_cloned_footer').css({'width': obj.find('.fht_parent .fht_fixed_header_table_parent').width()+'px', 'height': (obj.find('.fht_parent .fht_fixed_header_table_parent').height()-1)+'px'});
	
				footerHeight = obj.find('.fht_cloned_footer').height();
			}
			
			var headerHeight = obj.find('.fht_parent .fht_fixed_header_table_parent').height();
			var scrollDivHeight = parentDivHeight - footerHeight - headerHeight;
	
			obj.find('.fht_table_body').css({'width': tableBodyWidth+'px','height': scrollDivHeight+'px'}); // set the height of the main table body (where the data will be displayed) this also determines how much of the data is visible before a scroll bar is needed
			
			obj.find('table.fht_orig_table thead tr').css('display', 'none'); // hide the table body's header
			
			if (options.fixCol1) {
				if (obj.find('.fht_fixed_col_fixed_header').size() > 0 == false) {
					obj.find('.fht_parent .fht_fixed_col').prepend('<div class="fht_fixed_col_fixed_header"><table><thead><tr></tr></thead></table></div>');
					obj.find('.fht_parent .fht_fixed_header_table thead tr th:first').prependTo('.fht_parent .fht_fixed_col_fixed_header table thead tr');
				}
				
				obj.find('.fht_parent .fht_fixed_col_fixed_header table thead tr th').css({'height':obj.find('.fht_parent .fht_fixed_header_table thead tr th:first').height()+'px'});
				
				if (obj.find('.fht_fixed_col_body').size() > 0 == false ) {
					obj.find('.fht_parent .fht_fixed_col').append('<div class="fht_fixed_col_body"><table><tbody></tbody></table></div>');
				
				
					var rowCount = 1;
					obj.find('.fht_parent .fht_fixed_col_fulltable .fht_table_body table tbody tr td:first-child').each(function() {
						obj.find('.fht_parent .fht_fixed_col_body table tbody').append('<tr class="row'+rowCount+'"></tr>');
						$(this).appendTo('.fht_parent .fht_fixed_col_body table tbody tr.row'+rowCount);
						rowCount++;
					});
				}
				var firstRowTableData = obj.find('.fht_parent .fht_fixed_col_body tr.row1 td div.tableData').width();
				var rowHeight = obj.find('.fht_parent .fht_table_body table tbody tr td').height();
				obj.find('.fht_parent .fht_fixed_col_body tr td div.tableData').css({'width':firstRowTableData+'px'});
				obj.find('.fht_parent .fht_fixed_col_body tr td').css({'height':rowHeight+'px'});
				
				var fixedColTableWidthScroll = tableWidthScroll - obj.find('.fht_parent .fht_fixed_col').width();
				obj.find('.fht_parent .fht_fixed_col_fulltable').css({'width': fixedColTableWidthScroll+'px'});
				obj.find('.fht_parent .fht_fixed_header_table_parent').css({'width': fixedColTableWidthScroll+'px'});
				obj.find('.fht_parent .fht_fixed_col_body').css({'width': fixedColTableWidthScroll+'px'});
				obj.find('.fht_parent .fht_fixed_col').css({'width': firstRowTableData+'px'});
				obj.find('.fht_parent .fht_fixed_col_body').css({'width': firstRowTableData+'px'});
				obj.find('.fht_parent .fht_fixed_col .fht_fixed_col_fixed_header').css({'width': firstRowTableData+'px'});
				obj.find('.fht_fixed_col').css({'height':scrollDivHeight+'px'});
				tableBodyWidth = tableBodyWidth - obj.find('.fht_fixed_col').width();
				
				obj.find('.fht_fixed_header_table_parent').css({'width': tableBodyWidth+'px'});
				obj.find('.fht_table_body').css({'width': tableBodyWidth+'px','height': scrollDivHeight+'px'});
				
				//Alteração feita para corrigir as tds desalinhadas
				var aTdFixa = obj.find('.fht_parent .fht_fixed_col_body tr td ')
				var i = 0;
				obj.find('.fht_parent .fht_table_body table tbody tr').each(function(){

					aTdFixa[i].style.height = $(this).height() + 'px';
					i++;
				});
				
			}
			
			if(options.loader) {
				// if true hide the loader
				obj.find('.fht_loader').css('display', 'none');
			}
			
			//corrigindo o rodapé
			if(options.footer && !options.cloneHeaderToFooter){
				$('thead > tr > th > div > .tableHeader').each(function(i){
					var th = obj.find('#footer').find('th');
					var div = th.find('div');
				    if($(this).width() != 0 && div[i]){
				    	
				    	th[i].className = 'borderRight th'+i;
				    	div[i].className = 'tableHeader';
				    	div[i].style.width = ( $(this).width()+10 )+'px';
				    	div.find('p')[i].className = 'tableHeader';
				    	
				    }

				});
				//colocando o mesmo tamanho do cabeçalho no rodapé
				var tamTotal = obj.find('.fht_fixed_header_table').width();
				$('#footer')[0].width = tamTotal;
			}//fim
			
			//teste: Para que o rodapé apareça corretamente em qualquer outro navegador
			if(options.fixCol1 && options.footer && options.footerScroll && !$.browser.msie){
				var altura = obj.height()-20;
				var largura = obj.width()-246;
				$('.fht_cloned_footer').css('top', altura+'px');
				$('.fht_cloned_footer').css('left', '-'+largura+'px');				
			}
			
			//teste: Para que o rodapé apareça corretamente no IE
			if(options.fixCol1 && options.footer && options.footerScroll && $.browser.msie){
				var tamanho = obj.height()-100;
				$('div.fht_fixed_col_body').css('height', tamanho+'px');
			}
			
			obj.find('.fht_table_body').scroll(function() {
				// if a horizontal scrollbar is present
				obj.find('.fht_fixed_header_table_border').css('margin-left',(-this.scrollLeft)+'px');
				if (options.footer && options.cloneHeaderToFooter) {
					// if a cloned footer is visible it needs to be scrolled too
					obj.find('.fht_fixed_footer_border').css('margin-left',(-this.scrollLeft)+'px');
				}
				
				//verificando se o footer precisa de scroll
				if(options.footer && options.footerScroll){
					obj.find('.fht_fixed_footer_border').css('margin-left',(-this.scrollLeft)+'px');
				}
				
				//scroll horizontal
				obj.find('.fht_fixed_col_body table').css('margin-top',(-this.scrollTop)+'px'); // scroll the fixed header equal to the table body's scroll offset
			});
		}
		else {
			
			$('body').css('background', '#f00');
			
			// build a dialog window that indicates an error in implementation
		}
	}	
})(jQuery);