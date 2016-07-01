function _totalizar(x){
	total = x;
}

function carregarBeneficiarios()
{
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&req=carregarBeneficiarios&sabano=' + anoExercicio,
                                    asynchronous: false,
                                    onComplete: function(res)
                                    {
                                        $('beneficiariosSubAcao' + anoExercicio).innerHTML = res.responseText;
                                    }
                                });
}

function carregarParecerPadrao(){
	return new Ajax.Request(window.location.href,
    						{
                            	method: 'post',
								parameters: '&parecerPadrao=true&cosano=' + anoExercicio,
								onComplete: function(res) {
									if( $( 'parecerPadrao_'+anoExercicio ).checked ){
										$( 'sptparecer_' + anoExercicio ).value = res.responseText;
										$( 'sptparecer_' + anoExercicio ).disabled = true;
									}
									else{
										$( 'sptparecer_' + anoExercicio ).disabled = false;
										$( 'sptparecer_' + anoExercicio ).value = "";
									}
								}
							});
}

function carregarItensComposicao()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&req=carregarItensComposicao&cosano=' + anoExercicio + '&sbaporescola='+porEscola,
                                asynchronous: false,
                                onComplete: function(res)
                                {	
                                    $('itensComposicaoSubAcao' + anoExercicio).innerHTML = res.responseText;
                                }
                            });
}


function carregarDadosParecer()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&req=carregarDadosParecer&sbtano=' + anoExercicio,
                                asynchronous: false,
                                onComplete: function(res)
                                {
                                 
                                    var sptparecer      		= $('sptparecer_' + anoExercicio);
                                    var sptunt          		= $('sptunt_'     + anoExercicio);
                                    var sptuntdsc       		= $('sptuntdsc_'  + anoExercicio);
                                    var sptinicio       		= $('sptinicio_'  + anoExercicio);
                                    var sptfim          		= $('sptfim_'     + anoExercicio);
                                    var ssuid           		= $('ssuid_'      + anoExercicio);
                                    var sptanoterminocurso      = $('sptanoterminocurso_' + anoExercicio);

                                    var ssuidValue      		= getElementText('ssuid'    , res);
                                    var sptinicioValue  		= getElementText('sptinicio', res);
                                    var sptfimValue     		= getElementText('sptfim'   , res);
                                    var prgidTesteValor =  $('prgidteste');
									
                                    if(prgidTesteValor.value && PIVisivel != 0){
                                     	 var plinumplanointerno 		= $('plinumplanointerno_'     + anoExercicio);
                                     	 var plinumplanointernoValue = getElementText('plinumplanointerno', res);
                                     	 if (plinumplanointerno) {
                                        	for (var i = 0; i < plinumplanointerno.options.length; i++) {
                                            	if (plinumplanointernoValue == plinumplanointerno.options[i].value)
                                               		plinumplanointerno.selectedIndex = plinumplanointerno.options[i].index;
                                        		}
                                    		}
                                    }
                                    
                                   // if(prgidteste.value && PIVisivel != 0){
                                     	 var cvrnumprocesso 		= $('cvrnumprocesso_'     + anoExercicio);
                                     	 var cvrnumprocessoValue = getElementText('sptnumprocesso', res);
                                     	
                                     	 if (cvrnumprocesso) {
                                        	for (var i = 0; i < cvrnumprocesso.options.length; i++) {
                                            	if (cvrnumprocessoValue == cvrnumprocesso.options[i].value){
                                               		cvrnumprocesso.selectedIndex = cvrnumprocesso.options[i].index;
                                               	}
                                        	}
                                    	}
                                   // }
									
                                    if (ssuid) {
                                        for (var i = 0; i < ssuid.options.length; i++) {
                                            if (ssuidValue == ssuid.options[i].value)
                                                ssuid.selectedIndex = ssuid.options[i].index;
                                        }
                                    }

                                    if (sptinicio) {
                                        for (var i = 0; i < sptinicio.options.length; i++) {
                                            if (sptinicioValue == sptinicio.options[i].value)
                                                sptinicio.selectedIndex = sptinicio.options[i].index;
                                        }
                                    }
                                    
                                    if (sptfim)
                                        for (var i = 0; i < sptfim.options.length; i++) {
                                            if (sptfimValue == sptfim.options[i].value)
                                                sptfim.selectedIndex = sptfim.options[i].index;
                                        }

                                    if (sptanoterminocurso)
                                        sptanoterminocurso.value = getElementText('sptanoterminocurso', res);
                                        
                                    if (sptunt)
                                        sptunt.value = getElementText('sptunt', res);

                                    if (sptparecer)
                                        sptparecer.value = getElementText('sptparecer', res);

                                    if (sptuntdsc)
                                        sptuntdsc.value  = getElementText('sptuntdsc', res);
                                }
                            });
}

function carregarTotalizadores()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&req=carregarTotalizadores&sbaporescola='+porEscola,
                                onComplete: function(res)
                                {
                                    $('totalizadoresSubacao').innerHTML = res.responseText;
                                }
                            });
}

function extratoEscolas(qfaid)
    {
        return windowOpen('/cte/cte.php?modulo=principal/extratoescolassubacao&acao=A&sbaid='+ subacao +'&qfaid=' + qfaid,
                          'extratoEscolas',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

 
/************** ADITIVOS *********************/
	/*******************************
	* 
	*	FUNCTION: adicionarAditivo(ano);
	* 	DATE: 03/12/2008
	* 	DESCRIÇÃO:
	* 	Adiciona um aditivo referente ao ano corrente
	* 
	* 	@PARAM ano - Ano referente a aba a que se encontra.
	* 
	*******************************/
	function adicionarAditivo(ano){
		var resposta=confirm('Deseja criar um aditivo para está subação?'); //**@@&& mudar depois para aditivo
		if (resposta==true) {
			return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&req=novoAditivo&ano='+ano,
	                                    onComplete: function(res)
	                                    {
	                                        subacaoPai = subacao;
	                                        subacaoAditivo = res.responseText;
	                                        window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid='+subacaoAditivo+'&sbaidpai='+subacao+'&anoconvenio='+ano+'&ad=1';
	                                    }
	                                });	
		} else {
			return false;
		}
	
		}
		
		function irAditivo(sbaidAditivo, ano){
			 window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&aditivo=1&sbaid='+sbaidAditivo+'&sbaidpai='+subacao+'&anoconvenio='+ano+'&ad=1';
		}
		
		function voltarSubacaoPai(sbaidPai){
			 return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&req=voltarsubacaooriginal',
	                                    onComplete: function(res)
	                                    {
	                                    	subacaoOriginal = res.responseText;
	                                        window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid='+subacaoOriginal;
	                                    }
	                                });
		}


		/*******************************
		* 
		*	FUNCTION: travaAnosAnteriores(cosano);
		* 	DATE: 03/12/2008
		* 	DESCRIÇÃO:
		* 	Se estiver em uma aba onde o ano e menor que o ano atual, e a forma de execução da subação
		* 	for assistencia financeira, transferencia voluntaria ou assistencia tecnica com complementação financeira,
		*   a aba dos anos anteriores são travadas.
		* 
		* 	@PARAM cosano - Ano referente a aba a que se encontra.
		* 
		*******************************/
		function travaAnosAnteriores(cosano){
			if(cosano < anoAtualTrava){
				bloquearDados(cosano);
			}else{
				bloquearDados(0);
			}
		
		}
		
		/*******************************
		* 
		*	FUNCTION: travaAbaAnoJaAnalisada(cosano);
		* 	DATE: 02/12/2008
		* 	DESCRIÇÃO:
		* 	Se estiver em Elaboração do PAR ou em  Validação do Município a aba (Ano)
		*   onde o parecer já foi dado trava.
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function travaAbaAnoJaAnalisada(cosano){
		
			if(	anoParecer2007 == cosano || // Elaboração
				anoParecer2008 == cosano || // Elaboração
				anoParecer2009 == cosano || // Elaboração
				anoParecer2010 == cosano || // Elaboração
				anoParecer2011 == cosano 
				)
			{ // Se o ano conveniado for igual ao ano da aba trava.
				bloquearDados(cosano);
			}else{
				bloquearDados(0);
			}
		}
		
		/*******************************
		* 	FUNÇÃO DE ADITIVO
		*	FUNCTION: travaAbaAnoSeAditivada(cosano);
		* 	DATE: 02/12/2008
		* 	DESCRIÇÃO:
		* 	Se a subação foi conveniada trava a aba que foi conveniada 
		*   onde o parecer já foi dado trava.
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function travaAbaAnoSeAditivada(cosano){
			if(	anoConvenio == cosano && !eAditivo ){
				$('divAditivo' + cosano).style.display="table-row";// Mostra botão de Adicionar Aditivo.
				if(anoAtualTrava == cosano){ // se for o ano corrente e foi convenida mostra botão Adicionar Aditivo.
					//$('divAditivo' + cosano).style.display="table-row";// Mostra botão de Adicionar Aditivo.
				}
				bloquearDados(cosano);
			}else{
				if(eAditivo){
					bloquearDados(cosano);
				}else{
					bloquearDados(0);
				}
			}
		}
		
				/*******************************
		* 
		*	FUNCTION: bloquearDados(cosano);
		* 	12/11/2008
		* 	DESCRIÇÃO:
		* 	Trava a subação de acordo com o ano em que foi conveniada com o FNDE.
		*   Exemplo: Se a subação foi conveniada o ano de 2008 a aba de 2008 ficará desablilitada,
		*   será possivel apenas visualizar os dados. 	
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function bloquearDados(cosano)
	    { 	
			if(!novaSub){ // Se for nova subação não trava os dados
				var dadosForms 	= $('frmParSubacao').elements;
				var tamanho 	= $('frmParSubacao').elements.length;
				if( cosano != 0 )
				{ // Se o ano conveniado for igual ao ano da aba trava.
					if(cosano == "2007"){
						ind = 0;
					}else if(cosano == "2008"){
						ind = 1;
					}else if(cosano == "2009"){
						ind = 2;
					}
					else if(cosano == "2010"){
						ind = 3;
					}
					else if(cosano == "2011"){
						ind = 4;
					}
					
					if(document.getElementsByName("adItensComposicao")[ind]){
						document.getElementsByName("adItensComposicao")[ind].style.display="none";
					}else if(document.getElementsByName("adItensComposicao")[0]){
						document.getElementsByName("adItensComposicao")[0].style.display="none";
					}
					if(document.getElementsByName("adBeneficiarios")[ind]){
						document.getElementsByName("adBeneficiarios")[ind].style.display="none";
					}else if(document.getElementsByName("adBeneficiarios")[0]){
						document.getElementsByName("adBeneficiarios")[0].style.display="none";
					}
					if(document.getElementsByName("adEscolas")[ind]){
						document.getElementsByName("adEscolas")[ind].style.display="none";
						//alert(document.getElementsByName("adEscolas")[ind].style.display);
					}else if(document.getElementsByName("adEscolas")[0]){
						document.getElementsByName("adEscolas")[0].style.display="none";
					}
					if(document.getElementById('sptunt_'+cosano)){
						document.getElementById('sptunt_'+cosano).disabled="disabled";
					}
					document.getElementById('sptinicio_'+cosano).disabled="disabled";
					document.getElementById('sptfim_'+cosano).disabled="disabled";
					if(document.getElementById('sptanoterminocurso_'+cosano)){
						document.getElementById('sptanoterminocurso_'+cosano).disabled="disabled";
					}
					document.getElementById('sptuntdsc_'+cosano).disabled="disabled";
					
					if($( 'sptparecer_' + cosano )){
						$( 'sptparecer_' + cosano ).disabled = true;
					}
					if($( 'ssuid_' + cosano )){
						$( 'ssuid_' + cosano ).disabled = true;
					}
					// Desabilita btns.
					
					var btnExcluirItensComposicao = document.getElementsByName('removeItens');
					for( cont=0; cont<btnExcluirItensComposicao.length; cont++ ){
						btnExcluirItensComposicao[cont].style.display="none";
					}
					
					var btnExcluirBenficiario = document.getElementsByName('removeBeneficiario');
					for( cont=0; cont<btnExcluirBenficiario.length; cont++ ){
						btnExcluirBenficiario[cont].style.display="none";
					}
					
					var btnExcluirEscolas = document.getElementsByName('removeEscolas');
					for( cont=0; cont<btnExcluirEscolas.length; cont++ ){
						btnExcluirEscolas[cont].style.display="none";
					}
					var inputsItens   = $('frmParSubacao').getInputs('text');
					for (var a = 0; a < inputsItens.length; a++) {
	                    if (/cosvlruni/.test(inputsItens[a].getAttribute('name')) ||
	                        /cosqtd/   .test(inputsItens[a].getAttribute('name'))) {
	                        inputsItens[a].disabled="disabled";   
	                    }
	                }
					for( i=0; i<tamanho; i++ ){
						if(dadosForms[i].type == "button"){
							if( dadosForms[i].id != "btnAnterior" && dadosForms[i].id != "btnProximo" && dadosForms[i].id != "btnVoltar" ){
								dadosForms[i].disabled="disabled";
							}
						}
					}
					// Fim desabilita Btns
				}else{ // Se não foi conveniada não bloqueia (OBS: libera os btns para as outras abas.)
					for( i=0; i<tamanho; i++ ){
						if(dadosForms[i].type == "button"){
							dadosForms[i].disabled="";
						}
					}
				}  
    		}
    		
	    }
