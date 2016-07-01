function verificaTipoProcesso(valor){
	$('formulario').submit();
}
function salvarProcesso( tpcid ){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "prcnum";
	campos[2] 			= "prcobjeto";
	campos[3] 			= "dreid";
	campos[4] 			= "prcdatainiciovigencia";
	campos[5] 			= "prcdatafimvigencia";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[2] 	= "textarea";
	tiposDeCampos[3] 	= "select";
	tiposDeCampos[4] 	= "texto";
	tiposDeCampos[5] 	= "texto";
	
	if( tpcid == 1 ){
		//campos[6] 			= "intid"; //interessado
		campos[7] 			= "prcvalor";
		campos[8] 			= "prcnumconvenio";
		campos[9] 			= "prcnumconveniosiafi";
		campos[10] 			= "gerid"; //gerente
		
		//tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "texto";
		tiposDeCampos[8] 	= "texto";
		tiposDeCampos[9] 	= "texto";
		tiposDeCampos[10] 	= "select";
		
	} else if( tpcid == 2 ){
		campos[1] 			= "prcdataentradaccon";
		//campos[6] 			= "intid"; //interessado
		campos[7] 			= "prcvalor";
		campos[8] 			= "prcnumcontrato";
		campos[9] 			= "tilid"; //Modalidade da Licitação
		campos[10] 			= "prcnuminegixibilidade";
		campos[11] 			= "gerid"; //gerente
		
		tiposDeCampos[1] 	= "texto";
		//tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "texto";
		tiposDeCampos[8] 	= "texto";
		tiposDeCampos[9] 	= "select";
		tiposDeCampos[10] 	= "texto";
		tiposDeCampos[11] 	= "select";	
	} else if( tpcid == 3 ){
		campos[1] 			= "prcdataentradaccon";
		//campos[6] 			= "intid"; //interessado
		campos[7] 			= "prcvalor";
		campos[8] 			= "prcnumportaria";
		campos[9] 			= "prcretificacao";
		campos[11] 			= "gerid"; //gerente
		
		tiposDeCampos[1] 	= "texto";
		//tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "texto";
		tiposDeCampos[8] 	= "texto";
		tiposDeCampos[9] 	= "textarea";
		tiposDeCampos[11] 	= "select";	
	} else if( tpcid == 4 ){
		campos[1] 			= "prcdataentradaccon";
		//campos[6] 			= "intid"; //interessado
		campos[7] 			= "prcvalor";
		campos[8] 			= "prcnumcontrato";
		campos[9] 			= "prcnumdispensa";
		
		tiposDeCampos[1] 	= "texto";
		//tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "texto";
		tiposDeCampos[8] 	= "texto";
		tiposDeCampos[9] 	= "texto";
	} else if( tpcid == 5 ){
		campos[1] 			= "prcdataentradaccon";
		//campos[6] 			= "intid"; //interessado
		
		tiposDeCampos[1] 	= "texto";
		tiposDeCampos[6] 	= "select";
	} else if( tpcid == 6 ){
		campos[1] 			= "prcdataentradaccon";
		//campos[6] 			= "intid"; //interessado
		campos[7] 			= "prcnumdoacao";
		
		tiposDeCampos[1] 	= "texto";
		//tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "texto";
	} else if( tpcid == 7 ){
		campos[1] 			= "prcdataentradaccon";
		campos[6] 			= "secid"; //interessado
		campos[7] 			= "prcnumacordo";
		campos[8] 			= "prcdou";
		
		tiposDeCampos[1] 	= "texto";
		tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "texto";
		tiposDeCampos[8] 	= "texto";
	} else if( tpcid == 8 ){
		campos[1] 			= "prcdataentradaccon";
		//campos[6] 			= "intid"; //interessado
		campos[7] 			= "prccedente";
		campos[8] 			= "cesid";
		
		tiposDeCampos[1] 	= "texto";
		//tiposDeCampos[6] 	= "select";
		tiposDeCampos[7] 	= "texto";
		tiposDeCampos[8] 	= "select";
	} else if( tpcid == 9 ){
		campos[1] 			= "prcdataentradaccon";
		//campos[6] 			= "intid"; //interessado
		
		tiposDeCampos[1] 	= "texto";
		//tiposDeCampos[6] 	= "select";
	}
	
	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm )){
		if( validaDataProcesso( tpcid ) ){		
			$('requisicao').value = 'salvar';
			$('formulario').submit();
		}
	}
}

function validaDataProcesso( tpcid ){
	if( tpcid != 1 ){
		if(!validaData($('prcdataentradaccon') ) ) {
			alert('Data entrada está no formato incorreto.');
			$('prcdataentradaccon').focus();
			return false;
		}	
	} 
	if(!validaData($('prcdatainiciovigencia') ) ) {
		alert('Data início está no formato incorreto.');
		$('prcdatainiciovigencia').focus();
		return false;
	}else if(!validaData( $('prcdatafimvigencia') ) ) {
		alert('Data fim está no formato incorreto.');
		$('prcdatafimvigencia').focus();
		return false;
	}else if( !validaDataMaior( $('prcdatainiciovigencia'), $('prcdatafimvigencia') ) ){
		alert("A data inicial não pode ser maior que data final.");
		$('prcdatainiciovigencia').focus();
		return false;
	} else {
		return true;
	}
}

function voltarProcesso(){
	window.location.href = 'seed.php?modulo=principal/listaProcessos&acao=A';
}

function formataNumDocumento( id ){
	if( id.value.length == 3 ){
		if( Number(id.value) ){
			id.value = id.value + '/';
		} else {
			id.value = '';
		}
	} 
	/*var n = id.value.split('/');
	var ar0 = n[0];
	var ar1 = n[1];
	
	var valor = ar0 + ar1;
	alert(valor);*/
	
	return id.value;
}

function alterarProcesso( prcid ){
	window.location.href = 'seed.php?modulo=principal/alterarProcesso&acao=A&prcid='+prcid;
}

function pesquisar(){
	if( $('prcdataentradaccon').value != '' ){
		if(!validaData( $('prcdataentradaccon') ) ) {
			alert('Data entrada está no formato incorreto.');
			$('prcdataentradaccon').focus();
			return false;
		}
	}
	if( $('prcdatainiciovigencia').value != '' && $('prcdatafimvigencia').value != '' ){
		if(!validaData($('prcdatainiciovigencia') ) ) {
			alert('Data início está no formato incorreto.');
			$('prcdatainiciovigencia').focus();
			return false;
		}else if(!validaData( $('prcdatafimvigencia') ) ) {
			alert('Data fim está no formato incorreto.');
			$('prcdatafimvigencia').focus();
			return false;
		}else if( !validaDataMaior( $('prcdatainiciovigencia'), $('prcdatafimvigencia') ) ){
			alert("A data inicial não pode ser maior que data final.");
			$('prcdatainiciovigencia').focus();
			return false;
		}
	}
	$('requisicao').value = 'pesquisar';
	$('formulario').submit();
}

function excluirProcesso( prcid ){
	$('requisicao').value = 'excluir';
	$('prcid').value = prcid;
	$('formulario').submit();
}

/*
Manter Situação
*/

function incluirSituacao(){
    var hstid			= document.getElementById('hstid');
    var sitid			= document.getElementById('sitid');
    var hstdsc			= document.getElementById('hstdsc');

    if( sitid.value == ''){
        alert( 'Campo Situação é obrigatório.' );
        return false;
    }

    if( hstdsc.value == ''){
        alert( 'Campo Justificativa é obrigatório.' );
        return false;
    }
    
    document.getElementById('requisicao').value = 'salvar'
        
    document.formulario.submit();
}

/*
Manter Anexos
*/

function salvarAnexo(){
	$('requisicao').value = 'salvar';
	$('formulario').submit();
}
function excluirAnexo( arqid ){
	$('arqid').value = arqid;
	$('requisicao').value = 'excluir';
	$('formulario').submit();
}
