/*
* 	Autor
*/

 /**
 * Função Pesquisar
 * Método usado para pesquisa de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function pesquisar(){
	var url = "ajax_emenda.php";
	var pars = 'autor=pesquisa&'+$('formulario').serialize();

	$('loader-container').show();
	var myajax = new Ajax.Updater('lista', url, {
		        method:     'post',
		        parameters: pars,
		        asynchronous: false
			  });
	$('loader-container').hide();
}

function novoAutor(){
	window.location.href = 'emenda.php?modulo=principal/cadastroAutor&acao=A';
}

 /**
 * Função Salvar
 * Método usado para inserï¿½ï¿½o ou alteraï¿½ï¿½o de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */
 
function salvar(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "autcod";
	campos[1] 			= "tpaid";
	campos[2] 			= "estuf";
	campos[3] 			= "parid";
	campos[4] 			= "autnome";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "select";
	tiposDeCampos[2] 	= "select";
	tiposDeCampos[3] 	= "select";
	tiposDeCampos[4] 	= "texto";
	
	if( validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		if( $F('autemail') != "" ){
			if( !validaEmail( $F('autemail') ) ){
				alert('O e-mail informado está invalido!');
				return false;
			}
		}
		var url = "ajax_emenda.php";
		
		$('loader-container').show();
		
		if( $F('autid') != "" ){
			var pars = 'autor=altera&'+$('formulario').serialize();
		} else {
			var pars = 'autor=insere&'+$('formulario').serialize();
		}
		
		new Ajax.Request(url,
		{  
			method: 'post',   
			parameters: pars,
			asynchronous: false,
			onComplete: function(resp){
				if(Number(resp.responseText) == 1){
					alert('Operação realizada com sucesso');
					cancelar();
				}else{
					alert('Operação não realizada');
					//$('erro').update(resp.responseText);
				}
			}
		});
		$('loader-container').hide();
	}
}

 /**
 * Função Excluir
 * Método usado para exclusão de um registro do banco
 * @param integer autid - Código do Autor
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function excluiAutor(autid){
	var url = "ajax_emenda.php";
	var pars = 'autor=excluir&autid='+autid;

	$('loader-container').show();
	if(confirm("Tem certeza que deseja excluir este registro?")){
		new Ajax.Request(url,
		{  
			method: 'post',   
			parameters: pars,
			asynchronous: false,
			onComplete: function(resp){
				if(Number(resp.responseText) == 1){
					alert('Operação realizada com sucesso');
					pesquisar();
				}else{
					alert('Operação não realizada');
					//$('erro').update(resp.responseText);
				}
			}
		});
	}
	$('loader-container').hide();
}

function cancelar(){
	window.location.href = 'emenda.php?modulo=principal/listaAutor&acao=A';
}

/*
* 	Partido
*/
 
 /**
 * Função Pesquisar
 * Método usado para pesquisa de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function pesquisaPartido(){

	var url = "ajax_emenda.php";
	var pars = 'partido=pesquisa&'+$('formulario').serialize();

	$('loader-container').show();
	var myajax = new Ajax.Updater('lista', url, {
		        method:     'post',
		        parameters: pars,
		        asynchronous: false
			  });
	$('loader-container').hide();
}

function novoPartido(){
	window.location.href = 'emenda.php?modulo=principal/cadastroPartido&acao=A';
}

 /**
 * Função Salvar
 * Método usado para inserï¿½ï¿½o ou alteraï¿½ï¿½o de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */

function salvarPartido(){
	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	$('acao').value = 'true';
	
	campos[0] 			= "parcodigo";
	campos[1] 			= "parnome";
	campos[2] 			= "parsigla";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "texto";
	tiposDeCampos[2] 	= "texto";
	
	validaForm(nomeform, campos, tiposDeCampos, submeterForm )
}
function cancelarPartido(){
	window.location.href = 'emenda.php?modulo=principal/listaPartido&acao=A';
}

 /**
 * Função Excluir
 * Método usado para exclusão de um registro do banco
 * @param integer parid - Código do Partido
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */
 
function excluiPartido(parid){
	var url = "ajax_emenda.php";
	var pars = 'partido=excluir&parid='+parid;

	$('loader-container').show();
	if(confirm("Tem certeza que deseja excluir este registro?")){
		new Ajax.Request(url,
		{  
			method: 'post',   
			parameters: pars,
			asynchronous: false,
			onComplete: function(resp){
				if(Number(resp.responseText) == 1){
					alert('Operação realizada com sucesso');
					pesquisaPartido();
				}else{
					alert('Operação não realizada');
					//$('erro').update(resp.responseText);
				}
			}
		});
	}
	$('loader-container').hide();
}

/*
* 	Tipo Autor
*/
 
 /**
 * Função Pesquisar
 * Método usado para pesquisa de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function pesquisaTipoAutor(){

	var url = "ajax_emenda.php";
	var pars = 'tipoautor=pesquisa&'+$('formulario').serialize();

	$('loader-container').show();
	var myajax = new Ajax.Updater('lista', url, {
		        method:     'post',
		        parameters: pars,
		        asynchronous: false
			  });
	$('loader-container').hide();
}

/**
 * Função Excluir
 * Método usado para exclusão de um registro do banco
 * @param integer tpaid - Código do Tipo de Autor
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */
 
function excluiTipoAutor(tpaid){
	var url = "ajax_emenda.php";
	var pars = 'tipoautor=excluir&tpaid='+tpaid;

	$('loader-container').show();
	if(confirm("Tem certeza que deseja excluir este registro?")){
		new Ajax.Request(url,
		{  
			method: 'post',   
			parameters: pars,
			asynchronous: false,
			onComplete: function(resp){
				if(Number(resp.responseText) == 1){
					alert('Operação realizada com sucesso');
					pesquisaTipoAutor();
				}else{
					alert('Operação não realizada');
					//$('erro').update(resp.responseText);
				}
			}
		});
	}
	$('loader-container').hide();
}

function novoTipoAutor(){
	window.location.href = 'emenda.php?modulo=principal/cadastroTipoAutor&acao=A';
}

/**
 * Função Salvar
 * Método usado para inserï¿½ï¿½o ou alteraï¿½ï¿½o de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 31/08/2009
 */
 
function salvarTipoAutor(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "tpanome";
	campos[1] 			= "gpaid";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "select";
	
	if( validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){

		var url = "ajax_emenda.php";
		
		$('loader-container').show();
		
		if( $F('tpaid') != "" ){
			var pars = 'tipoautor=altera&'+$('formulario').serialize();
		} else {
			var pars = 'tipoautor=insere&'+$('formulario').serialize();
		}
		
		new Ajax.Request(url,
		{  
			method: 'post',   
			parameters: pars,
			asynchronous: false,
			onComplete: function(resp){
				if(Number(resp.responseText) == 1){
					alert('Operação realizada com sucesso');
					cancelarTipoAutor();
				}else{
					alert('Operação não realizada');
					//$('erro').update(resp.responseText);
				}
			}
		});
		$('loader-container').hide();
	}
}

function cancelarTipoAutor(){
	window.location.href = 'emenda.php?modulo=principal/listaTipoAutor&acao=A';
}

/*
*  Grupo de Autores
*/

 /**
 * Função Pesquisar
 * Método usado para pesquisa de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function pesquisaGrupoAutor(){

	var url = "ajax_emenda.php";
	var pars = 'grupoautor=pesquisa&'+$('formulario').serialize();

	$('loader-container').show();
	var myajax = new Ajax.Updater('lista', url, {
		        method:     'post',
		        parameters: pars,
		        asynchronous: false
			  });
	$('loader-container').hide();
}

/**
 * Função Excluir
 * Método usado para exclusão de um registro do banco
 * @param integer gpaid - Código do Tipo de Autor
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */

function excluiGrupoAutor(gpaid){
	var url = "ajax_emenda.php";
	var pars = 'grupoautor=excluir&gpaid='+gpaid;

	$('loader-container').show();
	if(confirm("Tem certeza que deseja excluir este registro?")){
		new Ajax.Request(url,
		{  
			method: 'post',   
			parameters: pars,
			asynchronous: false,
			onComplete: function(resp){
				if(Number(resp.responseText) == 1){
					alert('Operação realizada com sucesso');
					pesquisaGrupoAutor();
				}else{
					alert('Operação não realizada');
					//$('erro').update(resp.responseText);
				}
			}
		});
	}
	$('loader-container').hide();
}

function novoGrupoAutor(){
	window.location.href = 'emenda.php?modulo=principal/cadastraGrupoAutor&acao=A';
}

/**
 * Função Salvar
 * Método usado para inserï¿½ï¿½o ou alteraï¿½ï¿½o de um registro do banco
 * @return void 
 * @access public
 * @author Wesley Romualdo da Silva
 * @since 01/09/2009
 */
 
function salvarGrupoAutor(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "gpanome";
	campos[1] 			= "gpacategoria";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "texto";
	
	if( validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){

		var url = "ajax_emenda.php";
		
		$('loader-container').show();
		
		if( $F('gpaid') != "" ){
			var pars = 'grupoautor=altera&'+$('formulario').serialize();
		} else {
			var pars = 'grupoautor=insere&'+$('formulario').serialize();
		}
		
		new Ajax.Request(url,
		{  
			method: 'post',   
			parameters: pars,
			asynchronous: false,
			onComplete: function(resp){
				if(Number(resp.responseText) == 1){
					alert('Operação realizada com sucesso');
					cancelarGrupoAutor();
				}else{
					alert('Operação não realizada');
					//$('erro').update(resp.responseText);
				}
			}
		});
		$('loader-container').hide();
	}
}

function cancelarGrupoAutor(){
	window.location.href = 'emenda.php?modulo=principal/listaGrupoAutor&acao=A';
}

/*
* Manter Responsável
*/

/**
* Função Pesquisa Responsavel
* Metodo usado para pesquisar registro na base de dados
* @return void
* @access public
* @author Wesley Romualdo da Silva
* @since 04/09/2009
*/

function pesquisaResponsavel(){	
	$('loader-container').show();
	var myajax = new Ajax.Request('emenda.php?modulo=principal/listaResponsavel&acao=A', {
		        method:     'post',
		        parameters: '&pesquisaAjax=true&'+formulario.serialize(),
		        asynchronous: false,
		        onComplete: function (res){
					$('lista').innerHTML = res.responseText;
		        }
		  });
	$('loader-container').hide();
}

/**
* Função Exclui Responsavel
* Método usado para exclusão de um registro do banco
* @param integer resid - Código do Responsável
* @return void
* @access public
* @author Wesley Romualdo da Silva
* @since 04/09/2009
*/

function excluiResponsavel(resid){
	
	if(confirm("Tem certeza que deseja excluir este registro?")){
		$('loader-container').show();
	
		var myAjax = new Ajax.Request('emenda.php?modulo=principal/listaResponsavel&acao=A', {
					        method:     'post',
					        parameters: '&excluirResponsavelAjax=true&resid='+resid,
					        onComplete: function (res){	
								if(Number(res.responseText) ){
									alert('Operação realizada com sucesso!');
									pesquisaResponsavel(); 
								}else{
									alert('Operação não realizada!');
								}
					        }
					  });
		$('loader-container').hide();
	}
}
function novoResponsavel(){
	window.location.href = 'emenda.php?modulo=principal/cadastraResponsavel&acao=A';
}

function listaSubfuncao( funcod, acao, sfucod ){
	
	var subfuncao = sfucod ? '&sfucod=' + sfucod : '';
	
	if ( acao == 'popup' ){
		var url = '?modulo=principal/popupEmendasResponsavel&acao=A&subacao=listasubfuncao&funcod=' + funcod + subfuncao;
	}else{
		var url = '?modulo=principal/emendasResponsavel&acao=A&subacao=listasubfuncao&funcod=' + funcod + subfuncao;
	}

	var myAjax = new Ajax.Updater(
		"subfuncao",
		url,
		{
			method: 'post',
			asynchronous: false
	});

}

function excluiVinculoEmendas( emeid ){
	if ( confirm( "Deseja realmente excluir este vinculo?" ) ){
		window.location.href = "?modulo=principal/emendasResponsavel&acao=A&requisicao=excluir&emeid=" + emeid;
	}
}

function inserirEmendaResponsavel(){
	window.open("?modulo=principal/popupEmendasResponsavel&acao=A", "EmendasResponsavel","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=600");
}

function abreEmenda( emeid ){
	window.location.href = "?modulo=principal/cadastroEmendas&acao=A&emeid=" + emeid;
}

function abreDetalheEmenda( emdid ){
	window.location.href = "?modulo=principal/cadastraDetalheEmenda&acao=A&emdid=" + emdid;
}

function excluirEntidadeDetalhe( edeid ){
	if ( confirm( "Deseja realmente excluir esta entidade beneficiada?" ) ){
		window.location.href = "?modulo=principal/cadastraDetalheEmenda&acao=A&requisicao=excluir&edeid=" + edeid;
	}
}

function insereDadosEntidade(){
	
	var form = document.getElementById( 'formulario' );
	
	selectAllOptions( document.getElementById( 'edeobjeto' ) );
	
	var mensagem  = 'Os seguintes campos devem ser preenchidos: \n\n';
	var validacao = true;

	var enbcnpj 	  = document.getElementById("enbcnpj");
	var enbnome 	  = document.getElementById("enbnome");
	var estuf 		  = document.getElementById("estuf");
	var edevalor 	  = document.getElementById("edevalor");
	var emdid 	  	  = document.getElementById("emdid");
	var enbid		  = document.getElementById("enbid");
	var edeid		  = document.getElementById("edeid");
	var edeobjeto	  = document.getElementById("edeobjeto");
	var edecpfresp	  = document.getElementById("edecpfresp");
	var edenomerep	  = document.getElementById("edenomerep");
	var edemailresp	  = document.getElementById("edemailresp");
	var ededddresp	  = document.getElementById("ededddresp");
	var edetelresp	  = document.getElementById("edetelresp");
	var edefuncaoresp = document.getElementById("edefuncaoresp");
	var exercicio 	  = document.getElementById("exercicio");
	var anoatual 	  = document.getElementById("anoatual");
	var emetipo 	  = document.getElementById("emetipo");
		
	if( exercicio.value == anoatual.value && edeid.value == '' && emetipo.value != 'X' ){
		alert('E necessário fazer a indicação no sistema SIOP.');
		return false;
	}

	if ( enbcnpj.value == '' ){
		mensagem += 'CNPJ \n';
		validacao = false;
	}
	
	if ( enbnome.value == '' ){
		mensagem += 'Entidade Beneficiada \n';
		validacao = false;
	}
	
	if ( estuf.value == '' ){
		mensagem += 'UF \n';
		validacao = false;
	}
	
	if ( trim(edevalor.value) == '' ){
		mensagem += 'Valor (R$) \n';
		validacao = false;
	}
	
	var j = edeobjeto.options.length;
	if( edeobjeto.options[0].value == '' ){
		mensagem += 'Objeto \n';
		validacao = false;	
	}
	
	if ( trim(edecpfresp.value) == '' ){
		mensagem += 'CPF \n';
		validacao = false;
	}
	
	if ( edenomerep.value == '' ){
		mensagem += 'Nome \n';
		validacao = false;
	}

	if ( trim(ededddresp.value) == '' ){
		mensagem += '(DDD) \n';
		validacao = false;
	}
	
	if ( trim(edetelresp.value) == '' ){
		mensagem += 'Telefone \n';
		validacao = false;
	}
	if ( edetelresp.value.length != 9 ){
		mensagem += 'Telefone Inválido \n';
		validacao = false;
	}

	if ( trim(edefuncaoresp.value) == '' ){
		mensagem += 'Função/Cargo \n';
		validacao = false;
	}
	
	if ( trim(edemailresp.value) == '' ){
		mensagem += 'e-mail \n';
		validacao = false;
	}
	
	if( edeid.value != '' ){	
		var retorno = validaEmendaVinculoPTA();
		
		if( retorno == 'false' ){
			return false;
		}
	}
		
	if ( !validacao ){
		alert( mensagem );
		return validacao;
	}else{
		form.submit();
	}

}

function validaEmendaVinculoPTA(){
	var retorno = '';
	
	var myajax = new Ajax.Request('emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A', {
			        method:     'post',
			        parameters: '&verificavinculo=true&'+$('formulario').serialize(),
			        asynchronous: false,
			        onComplete: function (res){
			        	//$('divTeste').update(res.responseText);
			        	if( res.responseText != '' ){
			        		alert( 'A(s) iniciativa(s) abaixo não pode(m) ser excluída(s), porque est(a)(ão vinculada(s) ao PTA:\n' +  res.responseText );
							retorno = 'false';
			        	} else {
			        		retorno = 'true';
			        	}
			        }
			  });
	return retorno;
}

function procurarNome( cnpj, confirmacao ){
	document.getElementById('aguarde').style.visibility = 'visible';
	document.getElementById('aguarde').style.display = '';
	
	var comp     = new dCNPJ();
	var nome     = document.getElementById( 'entnome' );
	var uf       = document.getElementById( 'estuf' );
	var muncod   = document.getElementById( 'muncod' );
	comp.buscarDados( cnpj );	
	
	var url = '?modulo=principal/cadastraDetalheEmenda&acao=A';
	var parametros = '&requisicao=verificaentidadebase&entnumcpfcnpj=' + cnpj+'&entnome='+comp.dados.no_empresarial_rf+'&uf='+comp.dados.sg_uf;
	var boReceita = true;
	var myAjax = new Ajax.Request(
	url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				//$('divTeste').update(resp.responseText);
				if(resp.responseText != "naoexiste") {
					var json = resp.responseText.evalJSON();
					
					$('entid').value 		 = json.entid;
					$('entnumcpfcnpj').value = json.cnpj;
					$('entnome').value	   	 = json.entnome;
					$('estuf').value		 = json.estuf;
					
					boReceita = false;
				}
			}
			 
		}
	);
	
	if(boReceita){
		if ( confirmacao ){
			if (confirm('Verifique se hï¿½ diferenï¿½a entre a Razï¿½o Social e o Nome Fantasia.\nDeseja prosseguir a consulta na base da Receita Federal?')) {
				if( comp.dados.no_empresarial_rf != '' ){
					nome.value = comp.dados.no_empresarial_rf;
					uf.value   = comp.dados.sg_uf;
					nome.readOnly = true;
					
					document.getElementById('entid').value = '';
					trReceita.style.display = 'none';
					
				}
			}
		} else {
			if( comp.dados.no_empresarial_rf != '' ){
				nome.value = comp.dados.no_empresarial_rf;
				uf.value   = comp.dados.sg_uf;
				nome.readOnly = true;
				
				trReceita.style.display = 'none';
			}
		}		
	}
	
	document.getElementById('aguarde').style.visibility = 'hidden';
	document.getElementById('aguarde').style.display = 'none';
}	

function populaEntidadeNossaBase( entnumcpfcnpj ){
	
	var url = '?modulo=principal/cadastraDetalheEmenda&acao=A';
	var parametros = '&requisicao=populaentidadebase&entnumcpfcnpj=' + entnumcpfcnpj;
	
	var myAjax = new Ajax.Request(
	url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: true,
			onComplete: function(resp) {
				
				var json = resp.responseText.evalJSON();
		
				$('entid').value 		 = json.entid;
				$('entnumcpfcnpj').value = json.cnpj;
				$('entnome').value	   	 = json.entnome;
				$('estuf').value		 = json.estuf;
				
			}
			 
		}
	);

}
/*
 * Novas funcões de tratamento
 */
function verificarEntidadeBeneficiada() {

	if(document.getElementById("enbcnpj").value != '') {
	
		var url = '?modulo=principal/cadastraDetalheEmenda&acao=A';
		var parametros = '&requisicao=buscarentidade&enbcnpj=' + document.getElementById("enbcnpj").value;
		
		// verificando se existe a entidade beneficiada, banco de dados
		var myAjax = new Ajax.Request(
		url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					if (resp.responseText != "naoexiste"){
						var enbentidade = eval('('+resp.responseText+')');
						document.getElementById( 'enbid' ).value=enbentidade.enbid;
						document.getElementById( 'enbnome' ).value=enbentidade.enbnome;
						document.getElementById( 'estuf' ).value=enbentidade.estuf;
						document.getElementById( 'muncod' ).value=enbentidade.muncod;

						
					} else {
					
						var comp     = new dCNPJ();
						comp.buscarDados( document.getElementById("enbcnpj").value );
						document.getElementById( 'enbnome' ).value=comp.dados.no_empresarial_rf;
						document.getElementById( 'estuf' ).value=comp.dados.sg_uf;
						document.getElementById( 'muncod' ).value=comp.dados.co_cidade;

						document.getElementById( 'enbcep' ).value=comp.dados.nu_cep.substr(0,5)+"-"+comp.dados.nu_cep.substr(5,3);
						document.getElementById( 'enbbai' ).value=comp.dados.ds_bairro;
						document.getElementById( 'enblog' ).value=comp.dados.ds_logradouro;
						document.getElementById( 'enbnum' ).value=comp.dados.ds_numero;
						
					}			
					
				}
				 
			}
		);
		
	}
}
/*
 * FIM - Novas funcões tratamento
 */
function verificaEntidade( ){

	var enbcnpj = document.getElementById("enbcnpj");
	var enbnome		  = document.getElementById("enbnome");
	
	//busca os dados da entidade na receita com base no cnpj ou no cpf digitado pelo usuário
	//procurarNome( entnumcpfcnpj.value, false );
	
	/*
	O codigo abaixo foi comentado porque analista pedio que muda-se a regra do sistema,
	antes o sistema buscava na base de dados de entidade e verificava se os dados eram divergentes com
	os da receita se sim jogava uma mensagem na tela informando a diveregencia.
	Hoje e para buscar diretamente na base de dados; 
	*/

	var url = '?modulo=principal/cadastraDetalheEmenda&acao=A';
	var parametros = '&requisicao=verificaentidade&enbcnpj=' + enbcnpj.value;
	
	var myAjax = new Ajax.Request(
	url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: true,
			onComplete: function(resp) {

				if (resp.responseText == 1){
					
					populaEntidadeNossaBase(enbcnpj.value);
					if (document.selection){
						trReceita.style.display = 'block';
					}else{
						trReceita.style.display = 'table-row';
					}
				
				}else if( resp.responseText > 1 && enbcnpj.value != '' ){
					
					displayMessage('emenda.php?modulo=principal/selecionaEntidade&acao=A&enbcnpj=' + enbcnpj.value);
					$('lupaCnpj').innerHTML = '<img style="cursor:pointer;" src="/imagens/consultar.gif" border="0" title="Consultar CNPJ" onclick=" verificaEntidade( );">';
					
				}else if (resp.responseText == 0){
					procurarNome( enbcnpj.value, false );
					
				}			
				
			}
			 
		}
	);

}

function selecionaEntidade( entid ){

	var url = '?modulo=principal/cadastraDetalheEmenda&acao=A';
	var parametros = '&requisicao=selecioaEntidade&entid=' + entid;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				
				var json = resp.responseText.evalJSON();
				
				$('entid').value 		 = json.entid;
				$('entnumcpfcnpj').value = json.cnpj;
				$('entnome').value	   	 = json.entnome;
				$('estuf').value		 = json.estuf;
				
				
			}
		}
	);	

	closeMessage();
	
}

function limpaDadosEntidade( ){
	window.location.href = window.location;
	/*var edeid	 	  = document.getElementById("edeid");
	var entnumcpfcnpj = document.getElementById("entnumcpfcnpj");
	var entnome 	  = document.getElementById("entnome");
	var estuf 		  = document.getElementById("estuf");
	var edevalor 	  = document.getElementById("edevalor");
	var edeobjeto 	  = document.getElementById("edeobjeto");
	
	entnumcpfcnpj.value = '';
	entnome.value 		= '';
	estuf.value 		= '';
	edevalor.value 		= '';
	edeobjeto.value	 	= '';
	edeid.value 		= '';
	$('lupaCnpj').innerHTML = '';
	
	$('entnumcpfcnpj').disabled = false;*/
	
}

function alterarEntidadeDetalhe( edeid ){

	$('enbcnpj').className = 'disabled';
	$('enbcnpj').readOnly = true;
	$('loader-container').show();
	var url = '?modulo=principal/cadastraDetalheEmenda&acao=A';
	var parametros = '&ajax=alterarentidade&edeid=' + edeid;
	
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				var json = resp.responseText.evalJSON();
				
				if( retiraPontosEmendas(json.valor) > 0 ){
					$('edevalor').readOnly = true;
					$('edevalor').className = 'disabled';
				}
				
				$('edeid').value 		 = json.id;
				$('enbid').value 		 = json.enbid;
				$('enbcnpj').value = json.cnpj;
				$('enbnome').value	     = json.nome;
				$('estuf').value		 = json.uf;
				$('edevalor').value 	 = json.valor;
				$('combo_objeto').innerHTML = json.objeto + json.ideid;
				$('usucpfalteracao').innerHTML 	 = json.usucpfalteracao;
				$('ededataalteracao').innerHTML	 = json.ededataalteracao;
				
				$('edecpfresp').value  = json.edecpfresp;
				$('edenomerep').value  = json.edenomerep;
				$('edemailresp').value = json.edemailresp;
				$('ededddresp').value  = json.ededddresp;
				$('edetelresp').value  = json.edetelresp;								
				$('edefuncaoresp').value  = json.usufuncao;
				
				$('lupaCnpj').innerHTML = '<img style="cursor:pointer;" src="/imagens/consultar.gif" border="0" title="Consultar CNPJ" onclick=" verificaEntidade( );">';
				  
			}
		}
	);
	verificarEntidadeBeneficiada();
	$('loader-container').hide();	
}

function retiraPontosEmendas(v){
	if( v != 0 ){
		var valor = v.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");
	} else {
		var valor = v;
	}
	
	return valor;
}

function verificaTotal( valor, objeto ){

	valor = valor.replace(',', '.');
	
	if ( valor > 100.00 ){
		
		alert( 'A Contrapartida Mínima não pode ultrapassar 100%!' );
		objeto.value = ''
			
	}
	

}

function liberaEntidades(){

	var form = document.getElementById('formLibera');
	form.action = 'emenda.php?modulo=principal/liberaEmenda&acao=A&requisicao=libera';
	form.submit();

}

function visualizarPTA( ptrid ){
	window.open("?modulo=principal/alteraDefinirRecursoPTA&acao=A&ptrid=" + ptrid+'&tipo=popup', "PTA","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=1200,height=600");
}
function visualizarHistoricoPTA( ptrid ){
	window.open("?modulo=principal/historicoPTA&acao=A&ptrid=" + ptrid+'&tipo=popup', "PTA","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=600");
}

/**
 * Função preenche os zeros automaticamente, pra completar a quantidade de caracteres. 
 *
 * @param string campo
 * @return string
 */
function validaQuantidadeCaracter(campo){
	
	if(campo.value){
		var valor = campo.value.replace("/", "");
		valor = valor.replace("-", "");
		
		var quant = valor.length;
	
		var comp = "";
		if(quant < 12 ){
			comp = 0;
			quant++;
			for(i=quant; i<12; i++){
				comp = comp + '0';
			}
		}
		
		campo.value = mascaraglobal('#######/####-#', comp + campo.value);
	}
}

function buscaCPFReceita(cpf){
	if(cpf){		
		var valor = cpf.replace(".", "");
		valor = valor.replace(".", "");
		valor = valor.replace("-", "");
		
		if( validar_cpf(valor) ){		
			var comp = new dCPF();
			comp.buscarDados( valor );
			
			$('edenomerep').value = comp.dados.no_pessoa_rf;
		} else {
			alert('CPF informado é Inválido');
			$('edecpfresp').value = '';
			$('edecpfresp').focus();
			return false;
		}
	}
}

function validaDataEmenda(dataform){	
		//Funcionalidade:	Valida a Data retornando True se for uma Data
        //					valida e False se não for.
        //					Antes de se usar esta Função deve-se garantir que os PARAMETROS
        //					passados sejam numérico e inteiros.
        // PARAMETROS:
        //		Dia = Dia da Data(caracteres numericos),
        //		Mes = Mes da Data(caracteres numericos),
        //		Ano = Ano da Data(caracteres numericos)

        //alert(dia +"/"+ mes +"/"+ ano);
		var dia = dataform.substring(0,2);
		var mes = dataform.substring(3,5);
		var ano = dataform.substring(6,10);
        var v_dia;
        var v_mes;
        var v_ano;

        if (!validaInteiro(dia)){
                return (false);
        }
        if (!validaInteiro(mes)){
                return (false);
        }
        if (!validaInteiro(ano)){
                return (false);
        }

        v_dia = dia;
        v_mes = mes;
        v_ano = ano;

        if (v_dia.length < 2)
        {
                return(false);
        }

        if (v_mes.length < 2)
        {
                return(false);
        }

        if (v_ano.length < 4)
        {
                return(false);
        }

        if (((v_ano < 1900) || (v_ano > 2079)) && (v_ano.length != 0))
        {
                return(false);
        }

        if (v_dia > 31 || v_dia < 1)
        {
                return(false);
        }

        if (v_mes > 12 || v_mes < 1)
        {
                return(false);
        }

        if (v_dia == "31")
        {
                if ((v_mes == "04") || (v_mes == "06") || (v_mes == "09") || (v_mes == "11"))
                {
                        return(false);
                }
        }

        //Validação de Ano Bissexto
        if (v_mes == "02")
        {
                if (!(v_ano%4))
                {
                        if (v_dia > 29)
                        {
                                return(false);
                        }
                }
                else if (v_dia > 28)
                {
                        return(false);
                }
        }

        //o -if- abaixo testa se algum campo foi preenchido e outro deixado em branco deixando a data incompleta

        if (((v_dia != "") || (v_mes != "") || (v_ano != "")) && ((v_dia == "") || (v_mes == "") || (v_ano == "")))
        {
                return(false);
        }

        return(true);
}

function verificaIndicacoesSIOP( emeid, tipo, emdid ){
	
	if( jQuery('[name="exercicio"]').val() == jQuery('[name="anoatual"]').val() ){
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=verificaIndicacoesSIOP&emeid="+emeid,
	   		async: false,
	   		success: function(msg){
		   		if( msg != 'tem' ){
		   			jQuery( "#dialog_acoes" ).show();
					jQuery( "#mostraRetorno" ).html(msg);
					jQuery( '#dialog_acoes' ).dialog({
							resizable: false,
							width: 400,
							modal: true,
							show: { effect: 'drop', direction: "up" },
							buttons: {
								'Fechar': function() {
									jQuery( this ).dialog( 'close' );
								}
							}
					});
	   			} else {
		   			if( tipo == 'D' ){
		   				abreDetalheEmenda( emdid );
			   		} else {
	   					abreEmenda(emeid);
			   		}
		   		}
		   	}
		});
	} else {
		if( tipo == 'D' ){
				abreDetalheEmenda( emdid );
   		} else {
			abreEmenda(emeid);
   		}
	}
}

function abreEmpedimentoEmenda( emdid, valor, edeid, tipo ){
		
	var largura = 1000;
	var altura = 700;
	//pega a resolução do visitante
	w = screen.width;
	h = screen.height;
	
	//divide a resolução por 2, obtendo o centro do monitor
	meio_w = w/2;
	meio_h = h/2;
	
	//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
	altura2 = altura/2;
	largura2 = largura/2;
	meio1 = meio_h-altura2;
	meio2 = meio_w-largura2;
		
	//window.open('emenda.php?modulo=principal/emendaImpositivo&acao=A&emdid='+emdid+'&valor='+valor+'&edeid='+edeid+'&tipo='+tipo,'Emenda Empendimento','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+',scrollbars=yes,location=no,toolbar=no,menubar=no');
	newWindow = window.open('emenda.php?modulo=principal/emendaImpositivo&acao=A&emdid='+emdid+'&valor='+valor+'&edeid='+edeid+'&tipo='+tipo,'Emenda Empendimento','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+',fullscreen=yes,scrollbars=yes,location=no,toolbar=no,menubar=no');
	newWindow.focus();
	return newWindow.name;
}