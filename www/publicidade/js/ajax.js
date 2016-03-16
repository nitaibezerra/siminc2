/**
 * Carrega a combo de municípios
 * @name carregarMunicipio
 * @param estuf       - Identificador de estado(UF)
 * @param muncod      - Identificador de município
 * @param obrigatorio - Indica se o campo será obrigatório
 * @return void
 */
function carregarMunicipio(estuf,muncod,obrigatorio){
	
	// buscando
	new Ajax.Updater('municipio', 'ajax.php', {
		method : 'post',
		asynchronous: false,
		parameters : {'estuf':estuf, 'muncod':muncod, 'obrigatorio':obrigatorio, 'servico':'carregarMunicipio'}
	});
	
}

/**
 * Carrega endereço pelo cep
 * @name carregarEndereco
 * @param cep - CEP a ser pesquisado
 * @param success  - Função a ser executada no caso de sucesso
 * @param failure  - Função a ser executada no caso de falha
 * @param create   - Função a ser executada na criação
 * @param complete - Função a ser executada ao completar
 * @return void
 */
function carregarEndereco(cep,success,failure,create,complete){
	
	if(cep != '' && cep.length < 8){
		
		alert('CEP Inválido');
		limparEndereco();
		
	}else if(cep != ''){
	
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			onCreate: eval(create),
			parameters: {'cep':cep, 'servico':'carregarEndereco'},
			onSuccess: eval(success),
			onFailure: eval(failure),
			onComplete: eval(complete)
		});
		
	}else if(cep == ''){
		limparEndereco();
	}
	
}

/**
 * Carrega a combo de categorias
 * @name carregarCategoria
 * @param tseid       - Identificador de tipo de serviço
 * @param catid       - Identificador de categoria
 * @param obrigatorio - Indica se o campo será obrigatório
 * @return void
 */
function carregarCategoria(tseid,catid,obrigatorio){
	
	// buscando
	new Ajax.Updater('categoria', 'ajax.php', {
		method : 'post',
		asynchronous: false,
		parameters : {'tseid':tseid, 'catid':catid, 'obrigatorio':obrigatorio, 'servico':'carregarCategoria'}
	});
	
}

/**
 * Carrega contrato pela agência
 * @name carregarContrato
 * @param forid    - Fornecedor(agência) a ser pesquisado
 * @param success  - Função a ser executada no caso de sucesso
 * @param failure  - Função a ser executada no caso de falha
 * @param create   - Função a ser executada na criação
 * @param complete - Função a ser executada ao completar
 * @return void
 */
function carregarContrato(forid,success,failure,create,complete){
	if(forid != ''){
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			onCreate: eval(create),
			parameters: {'forid':forid, 'servico':'carregarContrato'},
			onSuccess: eval(success),
			onFailure: eval(failure),
			onComplete: eval(complete)
		});
	}
}

/**
 * Carrega descrição do tipo de serviço
 * @name pegarDscTipoServico
 * @param tseid    - Tipo de serviço a ser pesquisado
 * @param success  - Função a ser executada no caso de sucesso
 * @param failure  - Função a ser executada no caso de falha
 * @param create   - Função a ser executada na criação
 * @param complete - Função a ser executada ao completar
 * @return void
 */
function pegarDscTipoServico(tseid,success,failure,create,complete){
	if(tseid != ''){
		new Ajax.Request('ajax.php',{
			method: 'post',
			asynchronous: false,
			onCreate: eval(create),
			parameters: {'tseid':tseid, 'servico':'pegarDscTipoServico'},
			onSuccess: eval(success),
			onFailure: eval(failure),
			onComplete: eval(complete)
		});
	}
}

/**
 * Carrega os honorários de um contrato
 * @name carregarHons
 * @param cttid  - Identificador do contrato
 * @param tseid  - Identificador do tipo de serviço
 * @return void
 */
function carregarHons(cttid,tseid){
	if(cttid != ''){
		new Ajax.Updater('hons', 'ajax.php', {
			method : 'post',
			asynchronous: false,
			parameters : {'cttid':cttid, 'tseid':tseid, 'servico':'carregarHons'}
		});
	}
}

/**
 * Recarrega a combo de fornecedores
 * @name refreshFornecedores
 * @return void
 */
function refreshFornecedores(){
	new Ajax.Updater('fornecedor', 'ajax.php', {
		method : 'post',
		asynchronous: false,
		parameters : {'servico':'refreshFornecedores'}
	});
}