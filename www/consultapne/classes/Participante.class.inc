<?php
class Participante extends Modelo {

	public static $tiposRepresentacao = array
	(
		3 => 'Pessoa Física',
		1 => 'Órgão, Entidade ou Insituição PÚBLICA',
		2 => 'Órgão, Entidade ou Insituição PRIVADA',
	);
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "consultapne.participante";
	
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
			'parnome' => null,
			'parcpf' => null,
			'pardatanascimento' => null,
			'estuf' => null,
			'muncod' => null,
			'parsexo' => null,
			'paremail' => null,
			'partempoatuacao' => null,
			'codigoinep' => null,
			'parcep' => null,
			'parlogradouro' => null,
			'parcompendereco' => null,
			'parbairro' => null,
			'parnumeroendereco' => null,
			'parrepresentacao' => null,			
			'parorgao' => null,
			'parcnpj' => null,
			'parrepnomefantasia' => null,
			'parreprazaosocial' => null,
			'escid' => null,
			'atuid' => null,
			'atuoutro' => null,
                        'intid' => null,
			'parrepnome' => null,
                        'parrepcpf' => null,
                        'intoutro' => null,
                        'parrepemail' => null,
                        'parrepuf' => null,
                        'parrepmuncod' => null
	);
	
	public function carregarPessoaFisica($cpfSomenteNumero) {
            
		$pessoaFisicaClient = new PessoaFisicaClient('http://ws.mec.gov.br/PessoaFisica/wsdl');
		
		$xmlstring = $pessoaFisicaClient->solicitarDadosResumidoPessoaFisicaPorCpf($cpfSomenteNumero);
		$xml = simplexml_load_string($xmlstring);
		$json = json_encode($xml);
		$array = json_decode($json); 
                
		return $array->PESSOA;
	}       
	
	public function carregarPessoaJuridica($cnpj) {
		$pessoaJuridicaClient = new PessoaJuridicaClient('http://ws.mec.gov.br/PessoaJuridica/wsdl');
	
		$xmlstring = $pessoaJuridicaClient->solicitarDadosResumidoPessoaJuridicaPorCnpj($cnpj);
		$xml = simplexml_load_string($xmlstring);
		$json = json_encode($xml);
		$array = json_decode($json);                
		return $array->PESSOA;
	}
	
	public function carregarEndereco($cep) {
		$cep = preg_replace("/[^0-9]/", "", trim($cep));
		$sql = " SELECT logradouro, bairro, muncod, estado FROM cep.v_endereco2 WHERE cep = '{$cep}'; ";
		$result = $this->pegaLinha($sql);

		print json_encode(Encode::utf8_encode_recursive($result));
		die;
	}
	
	public function recuperarParticipante($cpfSomenteNumero) {
		$sql = " SELECT * FROM $this->stNomeTabela WHERE parcpf = '{$cpfSomenteNumero}'";                
		$arResultado = $this->pegaLinha( $sql );                
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
	}
	
	public function atualizar() {
		try {
			$this->carregarPorId($_SESSION['parid_pne']);
			
			$data = Encode::utf8_decode_recursive($_REQUEST);
                        $this->popularDadosObjeto($data);
//                        foreach ($this->arAtributos as $key => $value){                            
//                            if (isset($data[$key])){      
//                                
//                                $this->arAtributos[$key] = $data[$key];
//                            }
//                        }                        
			$this->estuf    = $data['estuf'] ? $data['estuf'] : null;
			$this->intid    = $data['intid'] ? $data['intid'] : null;
			$this->muncod   = $data['muncod'] ? $data['muncod'] : null;
                        $this->parrepcpf   = $data['parrepcpf'] ? preg_replace("/[^0-9]/", "", trim($data['parrepcpf'])) : null;
			$this->parorgao = $data['parorgao'] ? $data['parorgao'] : null;
			$this->parrepresentacao = $data['parrepresentacao'] ? $data['parrepresentacao'] : null;

			$this->alterar(array('estuf', 'muncod', 'parorgao', 'tpoid'));
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}