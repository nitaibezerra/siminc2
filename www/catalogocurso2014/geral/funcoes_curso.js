$(document).ready(function() {
	
	mostraPreExigida();
	
	$('#addArquivo').click(function(){
		
		var qtd = $('#arquivos tr').length-1;
		
		if( $('#'+qtd).attr('name') != '' ){
			$('.linha').each(function(){
				qtd = parseInt($(this).attr('name'))+1;
			});
		}
		
		var html =  '<tr class="linha" id="arq'+qtd+'" name="'+qtd+'">'+
						'<td style="border-bottom: 1px solid #cccccc;">'+
							'<input type="text" class=" normal" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" value="" maxlength="255" size="80" name="arqdsc['+qtd+']" style="text-align:left;">'+
						'</td>'+
						'<td style="border-bottom: 1px solid #cccccc;">'+
							'<input type="file" name="arq'+qtd+'"/>'+
						'</td>'+
						'<td style="border-bottom: 1px solid #cccccc;">'+
							'<center>'+
								'<img src="../imagens/excluir.gif" title="Excluir" class="excluirarq" name="arq'+qtd+'" />'+
							'</center>'+
						'</td>'+
					'</tr>'
		$('#bordainferior').before(html);
	});
	
	$('.excluirarq').live('click',function(){
		
		if($(this).attr('id')!=''){
			if(confirm('Deseja excluir o arquivo?')){
				var arq = $(this).attr('name');
				jQuery.ajax({
					type: "POST",
					url: window.location,
					data: "req=excluirArquivo&arcid="+$(this).attr('id'),
					async: false,
					success: function(msg){ 
						if(msg){
							$('#'+arq).remove();
						}else{
							alert('Arquivo não pôde ser removido. Contate o Administrador.');
						}
					}
				});
			}
		}else{
			if(confirm('Deseja excluir o arquivo?')){
				$('#'+$(this).attr('name')).remove();
			}
		}
	});
	
	$('#voltar').click(function(){
		window.location = 'catalogocurso2014.php?modulo=inicio&acao=C';
	});

	$('#proximo').click(function(){
		window.location = 'catalogocurso2014.php?modulo=principal/cadOrganizacaoCurso&acao=A';
	});

	$('#salvarC').click(function(){
		$('#link').val('proximo');
		$('#salvar').click();
	});
});

function forceFocus( obj ){
	$('#'+obj).focus();
	if(!$('#'+obj).is(":focus")){
		forceFocus( obj );
	}
}

function mostraPreExigida( ){

	var teste = true;

	$('[name="modid[]"]').each(function(){
		if( $(this).attr('checked') && $(this).val()!='1' ){
			teste = false;
		}
	});
	if( teste ){
		$('#preexigida').hide();
		$('#curpercpremim').removeClass('obrigatorio');
		$('#curpercpremax').removeClass('obrigatorio');
	}else{
		$('#preexigida').show();
		$('#curpercpremim').addClass('obrigatorio');
		$('#curpercpremax').addClass('obrigatorio');
	}
}

function abreArquivo( arqid ){
	window.open( 'catalogocurso2014.php?modulo=principal/cadCatalogo&acao=A&req=abreArquivo&arqid='+arqid, 'guia', 'width=40,height=40,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
}
