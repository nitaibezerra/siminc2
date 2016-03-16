<?php

header( 'Content-type: text/html; charset=iso-8859-1' );

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// carrega libs necessarias
include_once APPSAP . 'libs/Math.class.inc';

// carregando classes automaticamente
function __autoload($classe) {
	require_once(APPSAP . 'classes/' . $classe . '.class.inc');
}

// se requisicao for para carregar o endereco
if($_POST['servico'] == 'carregarEndereco'){

	// incluindo classes
	include_once APPSAP . 'classes/Endereco.class.inc';
	$oEndereco = new Endereco();

	// se o codigo vier
	if(!empty($_POST['cep'])){

		// retorna os dados da unidade
		$endereco = $oEndereco->pegarRegistro($_POST['cep']);
		// se encontrar
		if(is_array($endereco)){

			//retorna os dados
			$return = array(
				'cep' => utf8_encode($endereco['cep']),
				'log' => utf8_encode($endereco['logradouro']),
				'bai' => utf8_encode($endereco['bairro']),
				'cid' => utf8_encode($endereco['cidade']),
				'uf' => utf8_encode($endereco['estado']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);


}
// se requisicao for para carregar os dados da unidade
else if($_POST['servico'] == 'carregarUnidade'){

	// incluindo classes
	include_once APPSAP . 'classes/Unidade.class.inc';
	$oUnidade = new Unidade();

	// se o codigo vier
	if(!empty($_POST['co_interno_uorg'])){

		// retorna os dados da unidade
		$unidade = $oUnidade->pegarRegistro($_POST['co_interno_uorg']);
		// se encontrar
		if($unidade){

			//retorna os dados
			$return = array(
				'co_interno_uorg' => utf8_encode($unidade['co_interno_uorg']),
				'no_unidade_org' => utf8_encode($unidade['no_unidade_org']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para carregar a lista de endereco por Unidade
else if($_POST['servico'] == 'atualizaListaEnderecos'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	echo '<div class="tituloLista"> Endere&ccedil;os Cadastrados para esta Unidade </div>';
	$oEnderecoUnidade->filtrar($_POST['co_interno_uorg']);

}
//se a requisição for para carregar os itens da conta contábil
else if($_POST['servico'] == 'carregarItemContaContabil'){

	// incluindo classes
	include_once APPSAP . 'classes/ItemContaContabil.class.inc';
	$oItemContaContabil = new ItemContaContabil();

	// monta o combo
	$oItemContaContabil->montaComboItemContaContabil($_POST['ccbid'],'',$_POST['obrigatorio']);

}
//se a requisição for para carregar os motivos de estado de conservacao
else if($_POST['servico'] == 'montaComboMotivoEstadoConservacao'){

	// incluindo classes
	$oMotivoEstadoConservacao = new MotivoEstadoConservacao();

	// monta o combo
	$oMotivoEstadoConservacao->montaComboMotivoEstadoConservacao($_POST['ecoid'],$_POST['mecid'],$_POST['obrigatorio']);

}
//se a requisição for para carregar os itens da conta contábil por classe
else if($_POST['servico'] == 'carregarItemContaContabilPorClasse'){

	// incluindo classes
	include_once APPSAP . 'classes/ItemContaContabil.class.inc';
	$oItemContaContabil = new ItemContaContabil();

	// monta o combo
	if(!empty($_POST['clscodclasse']))
		$oItemContaContabil->montaComboPorClasse($_POST['clscodclasse'], $_POST['ccbid']);
}
//se a requisição for para carregar as contas contábeis por classe
else if($_POST['servico'] == 'carregarContaContabil'){

	// incluindo classes
	include_once APPSAP . 'classes/ContaContabil.class.inc';
	$oContaContabil = new ContaContabil();

	// monta o combo
	$oContaContabil->montaComboPorClasse($_POST['clscodclasse']);

}
// se requisicao for para carregar os dados da classe
else if($_POST['servico'] == 'carregarClasse'){

	// incluindo classes
	include_once APPSAP . 'classes/Classe.class.inc';
	$oClasse = new Classe();

	// se o codigo vier
	if(!empty($_POST['clscodclasse'])){

		// retorna os dados da classe
		$classe = $oClasse->pegarRegistroClasse($_POST['clscodclasse']);
		// se encontrar
		if($classe){

			//retorna os dados
			$return = array(
				'clscodclasse' => utf8_encode($classe['clscodclasse']),
				'clsdescclasse' => utf8_encode($classe['clsdescclasse']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para carregar os dados do material
else if($_POST['servico'] == 'carregarMaterial'){

	// incluindo classes
	include_once APPSAP . 'classes/Material.class.inc';
	$oMaterial = new Material();

	// se o codigo vier
	if(!empty($_POST['matid'])){

		// retorna os dados da classe
		$material = $oMaterial->pegarRegistro($_POST['matid']);
		// se encontrar
		if($material){

			//retorna os dados
			$return = array(
				'matid' => utf8_encode($material['matid']),
				'matdsc' => utf8_encode($material['matdsc']),
				'ccbid' => utf8_encode($material['ccbid']),
				'ccbdsc' => utf8_encode($material['ccbdsc']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para carregar os dados do endereco de unidade
else if($_POST['servico'] == 'carregarEnderecoUnidade'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	// se o codigo vier
	if(!empty($_POST['eudid'])){

		// retorna os dados da classe
		$enderecoUnidade = $oEnderecoUnidade->pegarRegistro($_POST['eudid']);
		// se encontrar
		if($oEnderecoUnidade){

			//retorna os dados
			$return = array(
				'eudid' => utf8_encode($enderecoUnidade['eudid']),
				'no_unidade_org' => utf8_encode($enderecoUnidade['no_unidade_org']),
				'eudlog' => utf8_encode($enderecoUnidade['eudlog']),
				'eudcom' => utf8_encode($enderecoUnidade['eudcom']),
				'eudnum' => utf8_encode($enderecoUnidade['eudnum']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para carregar a lista de materiais por Classe
else if($_POST['servico'] == 'atualizaListaMateriais'){

	// incluindo classes
	include_once APPSAP . 'classes/Material.class.inc';
	$oMaterial = new Material();

	echo '<div class="tituloLista"> Materiais Cadastrados para esta Classe </div>';
	$oMaterial->filtrarPorClasse($_POST['clscodclasse']);

}
//se a requisição for para carregar as cidades de acordo com a uf
else if($_POST['servico'] == 'carregarCidade'){

	// incluindo classes
	include_once APPSAP . 'classes/Endereco.class.inc';
	$oEndereco = new Endereco();

	// monta o combo
	$oEndereco->montaComboCidade($_POST['uf']);

}
//caso a requisição seja para carregar responsável pela matrícula
else if($_POST['servico'] == 'carregarResponsavel'){

	// incluindo classes
	include_once APPSAP . 'classes/Responsavel.class.inc';
	$oResponsavel = new Responsavel();

	//array a ser usado no retorno dos dados
	$return = array();

	//variável que indicará se foi carregado o endereço único da unidade
	$passou = 'N';

	//carrega os dados do responsável
	$resultado = $oResponsavel->carregaResponsavel($_POST['nu_matricula_siape']);
	if(is_array($resultado)){

		//verifica se veio a chave da tabela de unidade
		if(!empty($resultado['co_uorg_lotacao_servidor'])){

			// incluindo classes
			include_once APPSAP . 'classes/enderecoUnidade.class.inc';
			$oEnderecoUnidade = new EnderecoUnidade();

			//verifica qtos endereços estão associados à unidade do responsável em questão
			$tot = $oEnderecoUnidade->contaEnderecosDaUnidade($resultado['co_uorg_lotacao_servidor']);

			//carrega os dados do endereço único da unidade
			$resultadoEndereco = $oEnderecoUnidade->carregaEnderecosDaUnidade($resultado['co_uorg_lotacao_servidor']);

			//caso haja apenas um endereço associado à unidade do responsável em questão
			if($tot == 1){

				//monta array de retorno
				$return['endcep'] = utf8_encode($resultadoEndereco[0]['endcep']);
				$return['enduf']  = utf8_encode($resultadoEndereco[0]['enduf']);
				$return['endcid'] = utf8_encode($resultadoEndereco[0]['endcid']);
				$return['endbairro'] = utf8_encode($resultadoEndereco[0]['endbairro']);
				$return['endlog'] = utf8_encode($resultadoEndereco[0]['endlog']);
				$return['endcom'] = utf8_encode($resultadoEndereco[0]['endcom']);
				$return['endnum'] = utf8_encode($resultadoEndereco[0]['endnum']);
				$return['uorno'] = utf8_encode($resultadoEndereco[0]['uorno']);
				$return['uorco_uorg_lotacao_servidor'] = utf8_encode($resultadoEndereco[0]['uorco_uorg_lotacao_servidor']);
				$return['uorsg'] = utf8_encode($resultadoEndereco[0]['uorsg']);
				$return['uendid'] = utf8_encode($resultadoEndereco[0]['uendid']);
				$return['enadescricao'] = utf8_encode($resultadoEndereco[0]['enadescricao']);
				$return['easdescricao'] = utf8_encode($resultadoEndereco[0]['easdescricao']);

				$passou = 'S';
			}
			else if($tot != 1){

				//monta array de retorno
				$return['endcep'] = '';
				$return['enduf']  = '';
				$return['endcid'] = '';
				$return['endbairro'] = '';
				$return['endlog'] = '';
				$return['endcom'] = '';
				$return['endnum'] = '';
				$return['uorno'] = utf8_encode($resultadoEndereco[0]['uorno']);
				$return['uorco_uorg_lotacao_servidor'] = utf8_encode($resultadoEndereco[0]['uorco_uorg_lotacao_servidor']);
				$return['uorsg'] = utf8_encode($resultadoEndereco[0]['uorsg']);
				$return['uendid'] = '';
				$return['enadescricao'] = '';
				$return['easdescricao'] = '';

				$passou = 'S';
			}
		}

		//array de retorno
		if($passou == 'N'){
			$return['endcep'] = '';
			$return['enduf']  = '';
			$return['endcid'] = '';
			$return['endbairro'] = '';
			$return['endlog'] = '';
			$return['endcom'] = '';
			$return['endnum'] = '';
			$return['uorno'] = '';
			$return['uorco_uorg_lotacao_servidor'] = '';
			$return['uorsg'] = '';
			$return['uendid'] = '';
			$return['enadescricao'] = '';
			$return['easdescricao'] = '';
		}
		$return['no_servidor'] = utf8_encode($resultado['no_servidor']);
		$return['status'] = 'ok';
	}
	else{
		// retorna erro
		$return['status'] = 'erro';
		$return['nu_matricula_siape'] = $_POST['nu_matricula_siape'];
	}

	echo simec_json_encode($return);

}
//caso a requisição seja para carregar fornecedor pelo cnpj
else if($_POST['servico'] == 'carregaFornecedor'){

	// incluindo classes
	include_once APPSAP . 'classes/Fornecedor.class.inc';
	$oFornecedor = new Fornecedor();

	$resultado = $oFornecedor->carregaFornecedor($_REQUEST['forcpfcnpj']);
	if(is_array($resultado)){
		//retorna os dados
		$return = array(
			'forrazaosocial' => utf8_encode($resultado['forrazaosocial']),
			'status' => 'ok'
		);
	}
	else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para carregar os dados do rgp
else if($_POST['servico'] == 'carregarDadosRGP'){

	// incluindo classes
	$oRgp = new Rgp();

	// se o codigo vier
	if(!empty($_POST['rgpnum'])){

		// retorna os dados da classe
		$arDados = $oRgp->carregaDadosRGP($_POST['rgpnum']);

		// se encontrar
		if($arDados){
			//retorna os dados
			$return = array(
				'rgpid' => utf8_encode($arDados['rgpid']),
				'matdsc' => utf8_encode($arDados['matdsc']),
				'sbmdsc' => utf8_encode($arDados['sbmdsc']),
				'ecoid' => utf8_encode($arDados['ecoid']),
				'mecid' => utf8_encode($arDados['mecid']),
				'rgpnumserie' => utf8_encode($arDados['rgpnumserie']),
				'uorno' => utf8_encode($arDados['uorno']),
				'easdescricao' => utf8_encode($arDados['easdescricao']),
				'enadescricao' => utf8_encode($arDados['enadescricao']),
				'endcep' => utf8_encode($arDados['endcep']),
				'enduf' => utf8_encode($arDados['enduf']),
				'endcid' => utf8_encode($arDados['endcid']),
				'endlog' => utf8_encode($arDados['endlog']),
				'no_servidor' => utf8_encode($arDados['no_servidor']),
				'nu_cpf' => utf8_encode($arDados['nu_cpf']),
				'nu_matricula_siape' => utf8_encode($arDados['nu_matricula_siape']),
				'co_orgao' => utf8_encode($arDados['co_orgao']),
				'co_cargo_emprego' => utf8_encode($arDados['co_cargo_emprego']),
				'dt_ocor_exclusao_serv' => utf8_encode($arDados['dt_ocor_exclusao_serv']),
				'sg_funcao' => utf8_encode($arDados['sg_funcao']),
				'co_nivel_funcao' => utf8_encode($arDados['co_nivel_funcao']),
				'co_uorg_localizacao_serv' => utf8_encode($arDados['co_uorg_localizacao_serv']),
				'co_uorg_lotacao_servidor' => utf8_encode($arDados['co_uorg_lotacao_servidor']),
				'co_cargo_emprego' => utf8_encode($arDados['co_cargo_emprego']),
				'status' => 'ok'
			);

		}else{
			// retorna erro
			$return = array('status' => 'erro');
		}
	}else{
		// retorna erro
		$return = array('status' => 'erro');
	}

	echo simec_json_encode($return);

}

// se requisicao for para carregar os dados do empenho
else if($_POST['servico'] == 'carregarEmpenho'){

	// incluindo classes
	include_once APPSAP . 'classes/Empenho.class.inc';
	$oEmpenho = new Empenho();

	// se o codigo vier
	if(!empty($_POST['empid'])){

		// retorna os dados da classe
		$empenho = $oEmpenho->pegarRegistro($_POST['empid']);
		// se encontrar
		if($empenho){

			$aux = explode('-',$empenho['empdata']);
			$empenho['empdata'] = $aux[2].'/'.$aux[1].'/'.$aux[0];

			//retorna os dados
			$return = array(
				'empnumero' => utf8_encode($empenho['empnumero']),
				'empdata' => utf8_encode($empenho['empdata']),
				'empvalorper' => utf8_encode($empenho['empvalorper']),
				'empid' => utf8_encode($empenho['empid']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}

// se requisicao for para carregar os dados do fornecedor
else if($_POST['servico'] == 'carregarFornecedor'){

	// incluindo classes
	include_once APPSAP . 'classes/Fornecedor.class.inc';
	$oFornecedor = new Fornecedor();

	// se o codigo vier
	if(!empty($_POST['forcpfcnpj'])){

		// retorna os dados da classe
		$fornecedor = $oFornecedor->carregaFornecedor($_POST['forcpfcnpj']);
		// se encontrar
		if($fornecedor){

			if(strlen($fornecedor['forcpfcnpj']) == 14){
				$fornecedor['forcpfcnpj'] = substr($fornecedor['forcpfcnpj'], 0,2).'.'.substr($fornecedor['forcpfcnpj'], 2,3).'.'.substr($fornecedor['forcpfcnpj'], 5,3).'/'.substr($fornecedor['forcpfcnpj'], 8,4).'-'.substr($fornecedor['forcpfcnpj'], 12,2);
			}
			else if(strlen($fornecedor['forcpfcnpj']) == 11){
				$fornecedor['forcpfcnpj'] = substr($fornecedor['forcpfcnpj'], 0,3).'.'.substr($fornecedor['forcpfcnpj'], 3,3).'.'.substr($fornecedor['forcpfcnpj'], 6,3).'-'.substr($fornecedor['forcpfcnpj'], 9,2);
			}

			//retorna os dados
			$return = array(
				'forcpfcnpj' => utf8_encode($fornecedor['forcpfcnpj']),
				'forrazaosocial' => utf8_encode($fornecedor['forrazaosocial']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
//se a requisição for para carregar os andares do endereço selecionado
else if($_POST['servico'] == 'carregarAndar'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	// monta o combo
	$oEnderecoUnidade->montaComboAndar($_POST['endid']);

}
//se a requisição for para carregar as salas do andar selecionado
else if($_POST['servico'] == 'carregarSala'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	// monta o combo
	$oEnderecoUnidade->montaComboSala($_POST['enaid']);

}
// se requisicao for para retornar os dados do endereço pelo id da tabela uorgendereco
else if($_POST['servico'] == 'carregaDadosEndereco'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	// se o id vier
	if(!empty($_POST['uendid'])){

		// retorna os dados do endereço
		$endereco = $oEnderecoUnidade->carregaEndereco($_POST['uendid']);
		// se encontrar
		if(is_array($endereco)){

			//verifica se existe número e complemento para o endereço
			//se houver, concatena com a informação de logradouro
			$endlog = '';
			if(!empty($endereco['endnum'])){
				$endlog .= ', '.utf8_encode($endereco['endnum']);
			}
			if(!empty($endereco['endcom'])){
				$endlog .= ', '.utf8_encode($endereco['endcom']);
			}

			//retorna os dados
			$return = array(
				'endcep' => utf8_encode($endereco['endcep']),
				'enduf' => utf8_encode($endereco['enduf']),
				'endcid' => utf8_encode($endereco['endcid']),
				'endbairro' => utf8_encode($endereco['endbairro']),
				'endlog' => utf8_encode($endereco['endlog']).$endlog,
				'endcom' => utf8_encode($endereco['endcom']),
				'endnum' => utf8_encode($endereco['endnum']),
				'uorno' => utf8_encode($endereco['uorno']),
				'uendid' => utf8_encode($endereco['uendid']),
				'enadescricao' => utf8_encode($endereco['enadescricao']),
				'easdescricao' => utf8_encode($endereco['easdescricao']),
				'uorco_uorg_lotacao_servidor' => utf8_encode($endereco['uorco_uorg_lotacao_servidor']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}



	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para retornar os dados do endereço pelo id do endereço
else if($_POST['servico'] == 'carregaDadosEnderecoPorEndereco'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	// se o id vier
	if(!empty($_POST['endid'])){

		// retorna os dados do endereço
		$endereco = $oEnderecoUnidade->carregaEnderecoPorEndereco($_POST['endid']);
		// se encontrar
		if(is_array($endereco)){

			//retorna os dados
			$return = array(
				'endcep' => utf8_encode($endereco['endcep']),
				'enduf' => utf8_encode($endereco['enduf']),
				'endcid' => utf8_encode($endereco['endcid']),
				'endbairro' => utf8_encode($endereco['endbairro']),
				'endlog' => utf8_encode($endereco['endlog']),
				'endcom' => utf8_encode($endereco['endcom']),
				'endnum' => utf8_encode($endereco['endnum']),
				'enadescricao' => utf8_encode($endereco['enadescricao']),
				'easdescricao' => utf8_encode($endereco['easdescricao']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}



	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para retornar o nome da unidade
else if($_POST['servico'] == 'carregarNomeUnidade'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	// se o id vier
	if(!empty($_POST['uorco_uorg_lotacao_servidor'])){

		// retorna os dados do endereço
		$resultado = $oEnderecoUnidade->carregaNomeUnidade($_POST['uorco_uorg_lotacao_servidor']);

		// se encontrar
		if(is_array($resultado)){

			//retorna os dados
			$return = array(
				'uorno' => utf8_encode($resultado[0]['uorno']),
				'uendid' => utf8_encode($resultado[0]['uendid']),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
//caso a requisição seja para carregar responsável pela matrícula e atualizar a combo de unidades
else if($_POST['servico'] == 'carregaResponsavelUnidade'){

	// incluindo classes
	include_once APPSAP . 'classes/Responsavel.class.inc';
	$oResponsavel = new Responsavel();

	//array a ser usado no retorno dos dados
	$return = array();

	//variável que indicará se foi carregado o endereço único da unidade
	$passou = 'N';

	//carrega os dados do responsável
	$resultado = $oResponsavel->carregaResponsavel($_POST['nu_matricula_siape']);

	if($resultado['no_servidor'] != ''){
		$return['no_servidor'] = utf8_encode($resultado['no_servidor']);
		$return['co_uorg_lotacao_servidor'] = utf8_encode($resultado['co_uorg_lotacao_servidor']);
		$return['status'] = 'ok';
	}
	else{
		$return['status'] = 'erro';
		$return['co_uorg_lotacao_servidor'] = '';
	}

	echo simec_json_encode($return);
}
//se a requisição for para carregar as unidades de acordo com o responsável
else if($_POST['servico'] == 'filtrarUnidades'){

	// incluindo classes
	include_once APPSAP . 'classes/enderecoUnidade.class.inc';
	$oEnderecoUnidade = new EnderecoUnidade();

	// monta o combo
	$oEnderecoUnidade->montaComboUnidade($_POST['co_uorg_lotacao_servidor']);

}
// se requisicao for para carregar os dados do processo
else if($_POST['servico'] == 'carregarProcesso'){

	// incluindo classes
	$oProcesso = new Processo();

	$numprocesso = $_POST['numprocesso'];

	// se o codigo vier
	if(!empty($numprocesso)){

		// retorna os dados do processo
		$processo = $oProcesso->pegarRegistro($numprocesso);
		
		// se encontrar
		if($processo){

			//retorna os dados
			$return = array(
				'data' => utf8_encode($processo[1]),
				'status' => 'ok'
			);

		}else{

			// retorna erro
			$return = array('status' => 'erro');

		}
		
		

	}else{

		// retorna erro
		$return = array('status' => 'erro');

	}

	echo simec_json_encode($return);

}
// se requisicao for para verificar quais processos de entrada de bens
//estão utilizando o empenho em questão e qual valor ainda tem disponível
else if($_POST['servico'] == 'verificarEmpenhoUsado'){

	$oBens = new Bens();
	$valortotalusado = $oBens->somarValoresTotaisDocumentos($_POST['empid']);

	if(!empty($valortotalusado)){
		$processos = $oBens->carregaBensPorEmpenho($_POST['empid']);
		$oEmpenho = new Empenho();
		$valortotalempenho = $oEmpenho->pegarValorEmpenhoPermanente($_POST['empid']);

		$subtrai = new Math($valortotalempenho, $valortotalusado,2);
		$disponivel = $subtrai->sub()->getResult();

		$return = array('disponivel' => utf8_encode($disponivel),'processos'=>utf8_encode($processos));
	}
	else{
		$return = array('disponivel' => '','processos'=>'');
	}
	echo simec_json_encode($return);

}
// se requisicao for para verificar se o empenho está totalmente utilizado
//ou se existe valor disponível para novas entradas de bem
else if($_POST['servico'] == 'verificarEmpenhoDisponivel'){

	// soma o valor total dos documentos para este empenho
	$oBens = new Bens();
	$totalDocumentos = $oBens->somarValoresTotaisDocumentos($_POST['empid']);

	// recupera o valor total do empenho permanente
	$oEmpenho = new Empenho();
	$valorEmpenhoPermanente = $oEmpenho->pegarValorEmpenhoPermanente($_POST['empid']);

	//verificando se o valor total do empenho é maior que zero
	$maior = new Math($valorEmpenhoPermanente, 0.00000000);

	//caso o usuário tenha preenchido o valor do documento, soma no total usado
	if(!empty($_POST['benvlrdoc'])){
		// soma o valor requerido
		$soma = new Math($totalDocumentos, $_POST['benvlrdoc']);
		$totalDocumentos = $soma->sum()->getResult();
	}

	// valida se o valor do empenho é menor ou igual ao total utilizado
	$validacao = new Math($valorEmpenhoPermanente, $totalDocumentos);
	if(!$validacao->isLess() && !$validacao->isEqual() && $maior->isLarger()){
		$return = array('totalmenteusado'=>'');
	}
	else{
		$return = array('totalmenteusado'=>'S');
	}
	echo simec_json_encode($return);

}
//se a requisição for para carregar a sigla do tipo de entrada
else if($_POST['servico'] == 'carregarSiglaTipoEntrada'){
	$oTipoEntradaSaida = new TipoEntradaSaida();
	//carrega os dados do tipo de entrada pelo id
	$dados = $oTipoEntradaSaida->carregaTipoEntradaSaidaPorId($_POST['tipoEntrada']);

	//retorna a sigla se houver
	if(!empty($dados['tesslg'])){
		$return = array('status'=>'ok','sigla'=>$dados['tesslg']);
	}
	else{
		$return = array('status'=>'erro');
	}

	echo simec_json_encode($return);
}
//se a requisição for para carregar o empenho pelo número
else if($_POST['servico'] == 'buscarEmpenho'){
	$oEmpenho = new Empenho();
	$resultado = $oEmpenho->carregarEmpenhoPorNumero($_POST['empnumero']);
	if(is_array($resultado) && count($resultado) >= 1){
		$return = array('status'=>'ok','empid'=>$resultado['empid']);
	}
	else{
		$return = array('status'=>'erro');
	}

	echo simec_json_encode($return);
}
else if($_POST['servico'] == 'verificaExisteMaterialVinculado'){
	$oBensMaterial = new BensMaterial();
	$resultado = $oBensMaterial->quantidadeMaterial($_POST['benid']);

	if($resultado == $_POST['benitdoc']){
		$return = array('status'=>'ok','tot'=>$resultado);
	}
	else{
		$return = array('status'=>'erro','tot'=>$resultado);
	}

	echo simec_json_encode($return);
}
else if($_POST['servico'] == 'verificaTotalValorVinculado'){
	$oBensMaterial = new BensMaterial();
	$resultado = $oBensMaterial->somaBensMaterial($_POST['benid']);

	if(strpos($_POST['benvlrdoc'],'.') !== false){
		$_POST['benvlrdoc'] = str_replace('.','',$_POST['benvlrdoc']);
	}
	$verifica = new Math($resultado, $_POST['benvlrdoc']);
	$eIgual = $verifica->isEqual();

	if($eIgual){
		$return = array('status'=>'ok','eIgual'=>'S');
	}
	else{
		$return = array('status'=>'erro','eIgual'=>'N');
	}

	echo simec_json_encode($return);
}