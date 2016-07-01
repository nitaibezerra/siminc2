/**
 * Abre a janela de interação do combo_popup. Veja restante dos
 * comentário do arquivo www/geral/combopopup.php
 * 
 * @param string nome
 * @param integer height
 * @param integer width
 * @param string funcao
 * @return void
 */
function combo_popup_abre_janela_uo( nome, height, width, funcao )
{
	var campo_select = document.getElementById( nome );
	for ( var i = 0; i < campo_select.options.length; i++ )
	{
		campo_select.options[i].selected = false;
	}
	
	if(funcao != false)
		funcao = '&funcao=' + funcao;
	else
		funcao = '';
	
	//window.open( '../geral/combopopup.php?nome=' + nome, nome, "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	a = window.open( '../elabrev/geral/combopopupuo.php?nome=' + nome + funcao, 'Combopopup', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	a.focus();
}

function combo_popup_remove_selecionados_uo( event, nome_combo )
{
	if( window.event ) // IE
	{
		var keynum = event.keyCode
	}
	else if( event.which ) // Netscape/Firefox/Opera
	{
		var keynum = event.which
	}
	if ( keynum != 46 )
	{
		return;
	}
	var campo_select = document.getElementById( nome_combo );
	for( var i = 0; i <= campo_select.length-1; )
	{
		if ( campo_select.options[i].selected )
		{
			combo_popup_remover_item( nome_combo, campo_select.options[i].value, false, true );
		}
		else
		{
			i++;
		}
	}
	var evento = campo_select.getAttribute( 'onpop' );
	
	//alert( evento );
	if ( evento )
	{
		eval( evento );
	}
	sortSelect( campo_select );
}

function combo_popup_alterar_campo_busca_uo( campo_select )
{
	var campo_busca_id = 'combopopup_campo_busca_' + campo_select.id;
	var campo_busca = document.getElementById( campo_busca_id );
	if ( !campo_busca )
	{
		return;
	}
	var selecionados = 0
	var opcao = null;
	for ( var i = 0; i < campo_select.options.length; i++ )
	{
		if ( campo_select.options[i].selected )
		{
			selecionados++;
			opcao = campo_select.options[i]
		}
	}
	if ( selecionados != 1 )
	{
		return;
	}
	campo_busca.value = opcao.value;
}