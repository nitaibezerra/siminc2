<?php

header( 'Content-type: text/html; charset=iso-8859-1' );

include_once 'config.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// carrega libs necessarias
include_once APPPUB . 'libs/Math.class.inc';

// carregando classes automaticamente
function __autoload($classe) {
	require_once(APPPUB . 'classes/' . $classe . '.class.inc');
}

//se a requisição for para carregar os municípios
if($_POST['servico'] == 'carregarMunicipio'){
	
	// instancia a classe
	$oMunicipio = new Municipio();
	
	// monta a combo
	$oMunicipio->monta_combo_municipio($_POST['estuf'],$_POST['muncod'],$_POST['obrigatorio']);
	
}
//se a requisição for para carregar o endereço
else if($_POST['servico'] == 'carregarEndereco'){

	// se o cep vier
	if(!empty($_POST['cep'])){
		
		// instancia a classe
		$oEndereco = new Endereco();
		
		// retorna os dados do endereço
		$endereco = $oEndereco->pegar_registro($_POST['cep']);
		
		// se encontrar
		if(is_array($endereco) && count($endereco) >= 1){
			
			//formata o muncodcompleto que vem da view
			//da forma q o esquema territorios.municipio trabalha
			$endereco['muncod'] = substr($endereco['muncodcompleto'],0,2).substr($endereco['muncodcompleto'],7);
			
			//retorna os dados
			$return = array(
				'cep' => utf8_encode($endereco['cep']),
				'logradouro' => utf8_encode($endereco['logradouro']),
				'bairro' => utf8_encode($endereco['bairro']),
				'muncod' => utf8_encode($endereco['muncod']),
				'estado' => utf8_encode($endereco['estado']),
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
//se a requisição for para carregar as categorias
else if($_POST['servico'] == 'carregarCategoria'){
	
	// instancia a classe
	$oTipoServico = new TipoServico();
	
	// monta a combo
	$oTipoServico->monta_combo_categoria($_POST['tseid'],$_POST['catid'],$_POST['obrigatorio']);
	
}
//se a requisição for para buscar o contrato de um fornecedor
else if($_POST['servico'] == 'carregarContrato'){
	if(!empty($_POST['forid'])){
		// instancia a classe
		$oContrato = new Contrato();
		
		//retorna os dados do contrato
		$contrato = $oContrato->pegar_registro_por_agencia($_POST['forid']);
		
		if(is_array($contrato) && count($contrato) >= 1){
			//retorna os dados
			$return = array(
				'cttid' => utf8_encode($contrato['cttid']),
				'cttnumcontrato' => utf8_encode($contrato['cttnumcontrato']),
				'status' => 'ok'
			);
		}
		else{
			
			// retorna erro
			$return = array('status' => 'erro');
			
		}
	}
	else{
			
		// retorna erro
		$return = array('status' => 'erro');
			
	}
	echo simec_json_encode($return);
}
//se a requisição for para buscar a descrição do tipo de serviço
else if($_POST['servico'] == 'pegarDscTipoServico'){
	if(!empty($_POST['tseid'])){
		// instancia a classe
		$oTipoServico = new TipoServico();
		
		$tsedsc = $oTipoServico->pegar_descricao($_POST['tseid']);
		
		if(!empty($tsedsc)){
			$return = array('status'=>'ok','tsedsc'=>utf8_encode($tsedsc));
		}
		else{
			// retorna erro
			$return = array('status' => 'erro');
		}
	}
	else{
			
		// retorna erro
		$return = array('status' => 'erro');
			
	}
	
	echo simec_json_encode($return);
}
//se a requisição for para buscar os honorários(ex: ao cadastrar item da pad)
else if($_POST['servico'] == 'carregarHons'){

	if(!empty($_POST['cttid'])){//cttid na verdade � o forid;
		
		$oContrato = new Contrato();
		$oContratoHonorario = new ContratoHonorario();
		
		//retorna os dados do contrato
		$ctt = $oContrato->pegar_registro_por_agencia($_POST['cttid']);		
		
		$hons = $oContratoHonorario->carrega_registros($ctt['cttid'],$_POST['tseid']);
		
		if(is_array($hons) && count($hons) >= 1){
			foreach($hons as $key => $value){
				echo "<input type='radio' name='ipahonorario' id='ipahonorario' value='".$value['cthhonorario']."' onclick='javascript:calcularValores();'>".$value['cthhonorario']."%";
			}
		}
	}
}
//se a requisição for para recarregar a combo de fornecedores
else if($_POST['servico'] == 'refreshFornecedores'){
	//instancia a classe
	$oFornecedor = new Fornecedor();
	
	$oFornecedor->monta_combo_fornecedor('','S','','cadItemPad');
}