<?php
class Educriativa_Model_Participante extends Modelo {

	public static $tiposRepresentacao = array
	(
		3 => 'Pessoa Física',
		1 => 'Órgão, Entidade ou Insituição PÚBLICA',
		2 => 'Órgão, Entidade ou Insituição PRIVADA',
	);
	
	public static $orgaos = array
	(
		1 => 'Federal',
		2 => 'Estadual',
		3 => 'Municipal',
		4 => 'Outro',
	);
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.participante";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"parid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'parid' => null,
		'parcpf' => null,
		'parnome' => null,
		'pardatanascimento' => null,
		'estuf' => null,
		'muncod' => null,
		'parsexo' => null,
		'paremail' => null,
		'parcep' => null,
		'parlogradouro' => null,
		'parcompendereco' => null,
		'parbairro' => null,
		'parnumeroendereco' => null,
		'parcargo' => null,
		'partelefone' => null,
		'parramal' => null,
		'parcelular' => null,
		'partelefoneoutro' => null
	);
	
	public function carregarPessoaFisica($cpfSomenteNumero) {
		$pessoaFisicaClient = new PessoaFisicaClient('http://ws.mec.gov.br/PessoaFisica/wsdl');
		
		$xmlstring = $pessoaFisicaClient->solicitarDadosResumidoPessoaFisicaPorCpf($cpfSomenteNumero);
		$xml = simplexml_load_string($xmlstring);
		$json = simec_json_encode($xml);
		$array = simec_json_decode($json);
		
		return $array->PESSOA;
	}
	
	public function carregarEndereco($cep) {
		$cep = preg_replace("/[^0-9]/", "", trim($cep));
		$sql = " SELECT logradouro, bairro, muncod, estado FROM cep.v_endereco2 WHERE cep = '{$cep}'; ";
		$result = $this->pegaLinha($sql);

		print simec_json_encode($result);
		die;
	}
	
	public function atualizar() {
		try {
			$data = simec_utf8_decode_recursive($_REQUEST);
			
			$this->carregarPorId($_SESSION['parid']);
			
			$this->parcargo 		= $data['parcargo'] ? $data['parcargo'] : null;
			$this->partelefone 		= $data['partelefone'] ? $data['partelefone'] : null;
			$this->parramal 		= $data['parramal'] ? $data['parramal'] : null;
			$this->parcelular 		= $data['parcelular'] ? $data['parcelular'] : null;
			$this->partelefoneoutro = $data['partelefoneoutro'] ? $data['partelefoneoutro'] : null;
			$this->paremail    		= $data['paremail'] ? $data['paremail'] : null;
	
			$this->alterar();
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}