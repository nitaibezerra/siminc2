jQuery.noConflict();

jQuery(document).ready(
	function () {
		jQuery('a.closeEl').bind('click', toggleContent);
		jQuery('div.groupWrapper').Sortable(
			{	
				accept: 'groupItem',
				helperclass: 'sortHelper',
				activeclass : 	'sortableactive',
				hoverclass : 	'sortablehover',
				handle: 'div.itemHeader',
				tolerance: 'pointer',
				onChange : function(ser)
				{
				},
				onStart : function()
				{	
					jQuery.iAutoscroller.start(this, document.getElementsByTagName('body'));
				},
				onStop : function()
				{
					jQuery.iAutoscroller.stop();
					atualizaColunas();
				}
			}
		);
	}
);
var toggleContent = function(e)
{	
	var targetContent = jQuery('div.itemContent', this.parentNode.parentNode);
	if (targetContent.css('display') == 'none') {
		
		if(document.getElementById( 'autoscroll_' + this.id).checked == true){
			targetContent.css({'display' : ''});
			jQuery(this).html('<img src="../imagens/menos.gif" border=0 />');
			mundaEstado(this.id,'max');
			executarEditar(this.id,true);
		}
		if(document.getElementById( 'autoscroll_' + this.id).checked == false){
			targetContent.slideDown(300);
			jQuery(this).html('<img src="../imagens/menos.gif" border=0 />');
			mundaEstado(this.id,'max');
		}		
	} else {
		targetContent.slideUp(300);
		executarEditar(this.id,true);
		mundaEstado(this.id,'min');
		jQuery(this).html('<img src="../imagens/mais.gif" border=0 />');
	}
	return false;
};

function mundaEstado(estid,str){
	var id = estid;
	var estado = str;
	document.getElementById( 'estado_' + id ).value = estado; 
}

function mundaScroll(estid,str){
	var id = estid;
	var scroll = str;
	document.getElementById( 'scroll_' + id ).value = scroll; 
}

function atualizaColunas(s){
	var serial = jQuery.SortSerialize(s); 
	var colunas = serial.hash;
	var colExistente = 0;
	var qtdColunas = document.getElementById('qtdcolunas').value;
	
	var objeto = colunas.toQueryParams();
	var x = new Array();
	var y = new Array();
	var got = new Array();
	var c = 0;
	for(a in objeto){
		colExistente++;
		x.push(a.charAt(4));
		c++;
	}
	for(var cont = 0; cont < qtdColunas; cont++ ){
		y.push(cont);
	}
			
	for( i = 0; i<y.length; i++ ){
		if( !x.in_array( y[i] ) ){
			got.push(y[i]);
		}
	}
	
	if(colExistente < qtdColunas){
		var coldeletar = colExistente;
		for(var cont = 0; cont < got.length; cont++ ){
			document.getElementById('sort'+got[cont]).style.display = 'none';
		}

		calculo 	= 100 * (colExistente * 2);
		total 		= colExistente * 100;
		resultado 	= calculo / total;
		borda 		= 100 - resultado;
		tamanho 	=   borda / colExistente ;
		tamanho = new Number(tamanho);
		tamanho = tamanho.toFixed(0);
		qtdColunas = qtdColunas -1;
		for(var cont = 0; cont <= qtdColunas; cont++ ){
		//alert(cont);
			//if(got != cont ){
			document.getElementById('sort'+cont).style.width = tamanho+'%';
			//}
		}
	}	
}

Array.prototype.in_array = function(p_val) {
	for(var i = 0, l = this.length; i < l; i++) {
		if(this[i] == p_val) {
			return true;
		}
	}
	return false;
}

function addColuna(){
	var qtdColunas = document.getElementById('qtdcolunas').value;
	var colExistente = 0;
	var add = false;
	for(var cont = 0; cont < qtdColunas; cont++ ){
		var style = document.getElementById('sort'+cont).style.display;
		if(style != "none"){
			colExistente++;
		}
	}
	//alert(qtdColunas);
	if(colExistente < qtdColunas){
		for(var conts = 0; conts < qtdColunas; conts++ ){
			var style = document.getElementById('sort'+conts).style.display;
			if(style == "none" && !add){
				document.getElementById('sort'+conts).style.display = '';
				add = true;
				break;
			}
		}
		
		teste = colExistente + 1;
		//teste = colExistente;
		f = qtdColunas -1;
		//alert(qtdColunas);
		calculo 	= 100 * (teste * 2);
		total 		= teste * 100;
		resultado 	= calculo / total;
		borda 		= 100 - resultado;
		tamanho 	=   borda / teste ;
		tamanho = new Number(tamanho);
		tamanho = tamanho.toFixed(0);
		//alert(tamanho);
		for(var cont = 0; cont <= f; cont++ ){
			if(document.getElementById('sort'+cont)){
				document.getElementById('sort'+cont).style.width = tamanho+'%';
			}
		}
		
	}
}

function salvaColunas(s){
			//Estado das CXs
		 	var estados = document.getElementsByTagName("input");
			var num = estados.length;
		 	var ckeckados = 0;
		 	var arrID = new Array();
		 	
		 	for(var i=0; i < num; i++){
				if(document.getElementsByTagName('input')[i].type == 'hidden'){
		        	if(document.getElementsByTagName('input')[i].value == 'max' || document.getElementsByTagName('input')[i].value == 'min'){
		        		arrID[ckeckados] = document.getElementsByTagName('input')[i].value;
		        		ckeckados ++;
		       		}
		       	}
		     }
		     
		     //Estado das CXs em Scroll
		 	var scroll = document.getElementsByTagName("input");
			var num2 = scroll.length;
		 	var ckeckados2 = 0;
		 	var arrID2 = new Array();
		 	
		 	for(var i=0; i < num2; i++){
				if(document.getElementsByTagName('input')[i].type == 'hidden'){
		        	if(document.getElementsByTagName('input')[i].value == 'scrolltrue' || document.getElementsByTagName('input')[i].value == 'scrollfalse'){
		        		arrID2[ckeckados2] = document.getElementsByTagName('input')[i].value;
		        		ckeckados2 ++;
		       		}
		       	}
		     }
		     
			//var div = document.getElementById('erro'); 
			
			serial = jQuery.SortSerialize(s); 

			// Faz uma requisição ajax, passando o parametro 'ordid', via POST
			var req = new Ajax.Request('../geral/requisicaoConteudoFlutuante.php', {
					        method:     'post',
					        parameters: '?salvaParametros=1&' + serial.hash + '&estados=' + arrID + '&scroll=' + arrID2,
					        onComplete: function (res)
					   
					        {	
					        //div.innerHTML = res.responseText;
					        if(res.responseText == 1){
									alert('Operação Realizada com Sucesso!');
									location.reload();
									
								}else{
									alert('Ocorreu um erro ao tentar salvar os dados.');
								}
					        }
					  });
		}
		

function exibeEditar(id){
	var eixoid = id;
	document.getElementById( 'editar_' + eixoid ).style.display = '';
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="escondeEditar(\'' + eixoid + '\');executarEditar(\'' + eixoid + '\',true)" />';
}

function escondeEditar(id){
	var eixoid = id;
	document.getElementById( 'editar_' + eixoid ).style.display = 'none';
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="exibeEditar(\'' + eixoid + '\');executarEditar(\'' + eixoid + '\',true)" />';
}

function executarEditar(id,display){
	var eixoid = id;
	if(document.getElementById( 'autoscroll_' + eixoid)){
		if(document.getElementById( 'autoscroll_' + eixoid).checked == true){
			document.getElementById( 'itemContent_' + eixoid ).style.height = '300px';
			document.getElementById( 'itemContent_' + eixoid ).style.overflow = 'auto';
			document.getElementById( 'scroll_' + eixoid ).value = 'scrolltrue';
		}
		if(document.getElementById( 'autoscroll_' + eixoid).checked == false){
			document.getElementById( 'itemContent_' + eixoid ).style.height = 'auto';
			document.getElementById( 'itemContent_' + eixoid ).style.overflow = 'hidden';
			document.getElementById( 'scroll_' + eixoid ).value = 'scrollfalse';
		}
	}
	if(display == true){
		document.getElementById( 'editar_' + eixoid ).style.display = 'none';
	}
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="exibeEditar(\'' + eixoid + '\');" />';
}