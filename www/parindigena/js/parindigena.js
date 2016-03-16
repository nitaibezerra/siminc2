function alteraIcone(id,cod) {
	var img    = 'img_'+id;
	var tabela = document.getElementById('tabela');

	var i = document.getElementById(img);
	if (i && i.src.search("mais.gif") > 0) {
		document.getElementById(img).src = "../imagens/menos.gif";
		
		cont = 0;
		for(i=0; i < tabela.rows.length; i++) {
			if(tabela.rows[i].id.search(id+"_") >= 0) {
				// Se colocar 'table-row' dá erro no IE.
				tabela.rows[i].style.display = "";
				cont++;
			}
		}
		
		if(cont == 0) {
			if(cod == 1)
				carregaGrupos('ajax.php', 'tipo=carrega_grupos&estuf='+id+'');
			if(cod == 2)
				carregaConvenios('ajax.php', 'tipo=carrega_convenios&val='+id+'');
			if(cod == 3)
				carregaItens('ajax.php', 'tipo=carrega_itens&val='+id+'');
            if(cod == 4)
				carregaEtapas('ajax.php', 'tipo=carrega_etapas&val='+id+'');
			
			// incluído por fernando bagno
			if( cod == 5 ){
			
				var index = document.getElementById(id).rowIndex;
			
				carregaObras('parindigena.php?modulo=inicio&acao=C', 'tipo=carrega_obras&val='+id, index, id, cod );
			}
			
		}
	} else {
		document.getElementById(img).src = "../imagens/mais.gif";
		
		if( cod == 5 ){
			document.getElementById( id.substr(0,2) + '_obras_' + cod ).style.display = "none";
		}
		
		for(i=0; i < tabela.rows.length; i++) {
			if(tabela.rows[i].id.search(id+"_") >= 0) {
				tabela.rows[i].style.display = "none";
			}
		}
	}
}

/*** (INÍCIO) MANIPULA ALTERAÇÃO DA DATA DE INÍCIO/TÉRMINO ***/
function montaCalendario(event) {
	removeSlider();
	var objInputGeral = document.getElementById('inputGeral');
	if( trim( this.innerHTML ) != '' )
	{
		objInputGeral.value = trim( this.innerHTML );
	}
	else
	{
		objInputGeral.value = '';
	}
	objInputGeral.parent = this.id;
	displayCalendar( objInputGeral, 'dd/mm/yyyy', this.parentNode.getElementsByTagName("td")[3] );
}

function desmontaCalendario(objInputGeral) {
	if( !objInputGeral || objInputGeral.value == '' )
	{
		return;
	}
	var strSpanId = objInputGeral.parent;
	var objSpan = document.getElementById( strSpanId );
	var strDataAntiga = objSpan.innerHTML;
	
	if( strSpanId.indexOf( 'datainicio_' ) == 0 )
	{
		var id = strSpanId.substr( 'datainicio_'.length );
		strDataAlterada = 'mondatainicio';
	}
	else if( strSpanId.indexOf( 'datatermino_' ) == 0 )
	{
		var id = strSpanId.substr( 'datatermino_'.length );
		strDataAlterada = 'mondatafim';
	}
	
	objSpan.innerHTML = '<img align="absmiddle" src="../imagens/wait.gif"/>';
	alteraDataItem( id , strDataAlterada , objInputGeral.value , strDataAntiga);
}

function aposAlterarDataItem( id , strDataAlterada , strNovaData ) {
	var objDate = strDateToObjDate( strNovaData , 'd/m/Y' , '/' );
	var objToday = new Date();
	
	switch( strDataAlterada )
	{
		case 'mondatainicio':
		{
			strSpanId = 'datainicio_' + id;
			document.getElementById( strSpanId ).innerHTML = strNovaData;		
			break;
		}
		case 'mondatafim':
		{
			strSpanId = 'datatermino_' + id;
			var objSpan = document.getElementById( strSpanId );
			if( objDate > objToday )
			{
				objSpan.style.color = 'green';
				objSpan.style.fontWeight = 'normal';
			}
			else
			{	
				objSpan.style.color = 'red';
				objSpan.style.fontWeight = 'bold';
			}
			
			objSpan.innerHTML = strNovaData;		
			break;
		}
	}
}
/*** (FIM) MANIPULA ALTERAÇÃO DA DATA DE INÍCIO/TÉRMINO ***/


/*** (INÍCIO) MANIPULA ALTERAÇÃO DA SITUAÇÃO ***/
function posicionaSlider(event) {
	try	{
		closeCalendar();
	}
	catch(e) {}
	
	var objSlider 		= document.getElementById('sliderDiv');
	var objSliderValor 	= document.getElementById('valorSlider');
	var objSliderStatus = document.getElementById('situacaoSlider');
		
	var intValor 		= this.percentual;
	var intSelectValue 	= this.status;
	var strIdSpan 		= this.id;
	
	objSlider.style.position = "absolute";
	objSlider.style.left = getleftPos(this) + 'px';
	objSlider.style.top = getTopPos(this) + 'px';	
	objSlider.style.display = "block";
	
	objSliderValor.value = intValor;
	objSliderStatus.value = intSelectValue;	
	objSliderStatus.status = intSelectValue;
	objSliderStatus.id_tarefa = strIdSpan;
	objSliderValor.onchange();
}

function removeSlider() {
	var objSlider = document.getElementById('sliderDiv');
	objSlider.style.display = "none";
}

function slicerSubmit() {
	var objSliderValor	= document.getElementById( 'valorSlider' );
	var objSliderStatus	= document.getElementById( 'situacaoSlider' );
	var strIdSpan		= objSliderStatus.id_tarefa;
	var objSpan			= document.getElementById( strIdSpan );
	
	var strStatus		= document.getElementById( "situacaoSlider" ).options[ objSliderStatus.value - 1 ].innerHTML;
	var intPercentual	= objSliderValor.value;
	
	atualizaBarraStatus( strIdSpan , strStatus , objSliderStatus.value  , intPercentual )
	removeSlider();
}

function aposAtualizarBarraStatus(intBarraStatusId, strStatus, intStatus, intPercentual) {
	if(window.arrSituacoes == undefined)
	{
		var arrSituacoes 	= Array();
		
		// Status: 'Não iniciado'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#909090';
		arrSituacao.barra	= '#bbbbbb';
		arrSituacao.sombra	= '#efefef';
		arrSituacoes[1] 	= arrSituacao;
		
		// Status: 'Em andamento'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#209020';
		arrSituacao.barra	= '#339933';
		arrSituacao.sombra	= '#dcffdc';
		arrSituacoes[2] 	= arrSituacao;
		
		// Status: 'Suspenso'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#aa9020';
		arrSituacao.barra	= '#bba131';
		arrSituacao.sombra	= '#feffbf';
		arrSituacoes[3] 	= arrSituacao;
		
		// Status: 'Cancelado'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#aa2020';
		arrSituacao.barra	= '#cc3333';
		arrSituacao.sombra	= '#ffe7e7';
		arrSituacoes[4] 	= arrSituacao;
		
		// Status: 'Concluído'
		var arrSituacao		= new Object();
		arrSituacao.texto	= '#2020aa';
		arrSituacao.barra	= '#3333cc';
		arrSituacao.sombra	= '#d4e7ff';
		arrSituacoes[5] 	= arrSituacao;
		
		window.arrSituacoes = arrSituacoes;
	}
			
	arrSituacaoAtual = window.arrSituacoes[ intStatus ];
	
	var strNewSpanInnerHTML = '' +
	'<span style="color: '+ arrSituacaoAtual.texto + ';font-size: 10px;">' + strStatus + '</span>' +
	'<div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; ' + 
	'height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; ' +
	'background-color: ' + arrSituacaoAtual.sombra  + ';" title="' + intPercentual + '%">' +
		'<div style="font-size:4px;width: ' + intPercentual + '%; height: 6px; max-height: 6px; background-color: ' + arrSituacaoAtual.barra + ';">' +
		'</div>' + 
	'</div>';
	
	var objSpan = document.getElementById( intBarraStatusId );
	
	objSpan.status = intStatus;
	objSpan.percentual = intPercentual;

	objSpan.innerHTML = strNewSpanInnerHTML;
}

function alteraStatus(objSliderStatus) {
	var objSliderValor = document.getElementById('valorSlider');
	 
	switch('' + objSliderStatus.value)
	{
		case '1':
		{
			objSliderValor.value = 0;
			break;
		}
		case '2':
		case '3':
		case '4':
		{
			switch( '' + objSliderValor.value )
			{
				case '100':
				{
					objSliderValor.value = 90;
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case '5':
		{
			objSliderValor.value = 100;
			break;
		}
		default:
		{
			break;
		}
	}
	
	objSliderValor.onchange();
}
		
function arredonda(objInput) {
	if( objInput.value % 10 != 0 )objInput.value -= objInput.value % 10;
	
	var objSliderStatus = document.getElementById('situacaoSlider');
			
	var intOriginalStatus = objSliderStatus.status;
	
	switch( '' + objInput.value )
	{
		case '100':
		{
			objSliderStatus.value = 5;
			break;
		}
		case '0':
		{
			switch( '' + objSliderStatus.value )
			{
				case '5':
				{
					switch( intOriginalStatus )
					{
						case '5':
						{
							objSliderStatus.value = 2;
						}
						default:
						{
							objSliderStatus.value = intOriginalStatus;
							break;
						}
					}
					break;
				}
			}
			break;
		}
		default:
		{
			switch( '' + objSliderStatus.value )
			{
				case '5':
				case '1':
				{
					if( ( intOriginalStatus == 5 ) || ( intOriginalStatus == 1 ) )
					{ 
						objSliderStatus.value = 2;
					}
					else
					{
						objSliderStatus.value = intOriginalStatus;
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
	}
}
/*** (FIM) MANIPULA ALTERAÇÃO DA SITUAÇÃO ***/

function verObra( obrid ){
	janela('?modulo=principal/extratoDeObra&acao=A&obrid=' + obrid, 900, 480, 'extratoDeObra');
}