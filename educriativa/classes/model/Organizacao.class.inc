<?php
class Educriativa_Model_Organizacao extends Modelo {

    const K_ESFERA_FEDERAL   = 'F';
    const K_ESFERA_ESTADUAL  = 'E';
    const K_ESFERA_MUNICIPAL = 'M';

    const K_PODER_LEGISLATIVO = 'L';
    const K_PODER_EXECUTIVO   = 'E';
    const K_PODER_JUDICIARIO  = 'J';

	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.organizacao";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"orgid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'orgid' => null,
		'orgrazaosocial' => null,
		'orgnomefantasia' => null,
		'orgsite' => null,
		'orgresponsavel' => null,
		'orgcep' => null,
		'orglogradouro' => null,
		'orgcompendereco' => null,
		'orgbairro' => null,
		'orgnumeroendereco' => null,
		'estuf' => null,
		'muncod' => null,
		'orgsemicep' => null,
		'orgqtdfuncionarios' => null,
		'orgqtdestudantes' => null,
		'orgcnpj' => null,
		'gruid' => null,
		'ortid' => null,
		'ortdscoutro' => null,
		'orgesfera' => null,
		'orgpoder' => null,
		'orglinkvideo' => null,
	);
	
	public function atualizar() {
		try {
			$data = simec_utf8_decode_recursive($_REQUEST);

			$this->carregarPorId($_SESSION['orgid']);
	
			$this->orgsite 				= $data['orgsite'] ? $data['orgsite'] : null;
			$this->orgcnpj 				= $data['orgcnpj'] ? limpar_numero($data['orgcnpj']) : null;
			$this->orgrazaosocial 		= $data['orgrazaosocial'] ? $data['orgrazaosocial'] : null;
			$this->orgnomefantasia 		= $data['orgnomefantasia'] ? $data['orgnomefantasia'] : null;
			$this->orgresponsavel 		= $data['orgresponsavel'] ? $data['orgresponsavel'] : null;
			$this->orgcep 				= $data['orgcep'] ? limpar_numero($data['orgcep']) : null;
			$this->orglogradouro 		= $data['orglogradouro'] ? $data['orglogradouro'] : null;
			$this->orgcompendereco 		= $data['orgcompendereco'] ? $data['orgcompendereco'] : null;
			$this->orgbairro 			= $data['orgbairro'] ? $data['orgbairro'] : null;
			$this->orgnumeroendereco	= $data['orgnumeroendereco'] ? $data['orgnumeroendereco'] : null;
			$this->estuf 				= $data['estuf'] ? $data['estuf'] : null;
			$this->muncod 				= $data['muncod'] ? $data['muncod'] : null;
			$this->orgsemicep 			= $data['orgsemicep'] ? $data['orgsemicep'] : 'f';
			$this->orgqtdfuncionarios 	= $data['orgqtdfuncionarios'] ? $data['orgqtdfuncionarios'] : null;
			$this->orgqtdestudantes 	= $data['orgqtdestudantes'] ? $data['orgqtdestudantes'] : null;
			$this->ortid 				= $data['ortid'] ? $data['ortid'] : null;
			$this->ortdscoutro 			= $data['ortdscoutro'] ? $data['ortdscoutro'] : null;
			$this->orgesfera 			= $data['orgesfera'] ? $data['orgesfera'] : null;
			$this->orgpoder 			= $data['orgpoder'] ? $data['orgpoder'] : null;
			$this->orglinkvideo 		= $data['orglinkvideo'] ? $data['orglinkvideo'] : null;
			$this->gruid 				= $data['gruid'] ? $data['gruid'] : null;
			
			$atuacao = new Educriativa_Model_OrganizacaoAreaAtuacao();
			$atuacao->salvarAreaAtuacao($_REQUEST['araid']);
			
			$nivel = new Educriativa_Model_OrganizacaoNivelEnsino();
			$nivel->salvarNiveis($_REQUEST['nieid']);
			
			$rede = new Educriativa_Model_OrganizacaoRede();
			$rede->salvarRedes($_REQUEST['redid']);
			
			$site = new Educriativa_Model_OrganizacaoSiteWeb();
			$site->salvarSites($_REQUEST['sitid']);
			
			$faixa = new Educriativa_Model_OrganizacaoFaixaEtaria();
			$faixa->salvarFaixaEtaria($_REQUEST['faeid']);
				
			$this->alterar();
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function listarEstados() {
		$sql = " SELECT iduf, descricaouf, regcod FROM uf WHERE idpais = 1;";
		$result = $this->carregar($sql);
		
		return $result; 
	}
	
	public function listarMunicipios($uf) {
		$sql = " SELECT muncod, mundsc, regcod FROM municipio WHERE regcod = '{$uf}' ORDER BY mundsc;";
		$result = $this->carregar($sql);
		return $result;
	}
	
	public function detalharVideo($link) {
		if (strlen($link) >= 22) {
			$parts = parse_url($link);
			parse_str($parts['query'], $query);
			$id = $query['v'];
		} else {
			$id = $link;
		}
		
		$option = array(
			'id' => $id,
			'part' => 'contentDetails',
			'key' => 'AIzaSyA66lAjLjloi6nzJJXaznZkm4Ucd975VOs'
		);
		
		$url = "https://www.googleapis.com/youtube/v3/videos?".http_build_query($option, 'a', '&');
		
		$curl = curl_init($url);
		
		curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
		
		if (curl_exec($curl) === false)
		{
			print json_encode(array('error' => true, 'message' => curl_error($curl)));
		}
		else
		{
			$json = curl_exec($curl);
			
			curl_close($curl);

			print $json;
		}
	}
	
	public function carregarPessoaJuridica($cnpj) {
		$pessoaJuridicaClient = new PessoaJuridicaClient('http://ws.mec.gov.br/PessoaJuridica/wsdl');

		$xmlstring = $pessoaJuridicaClient->solicitarDadosResumidoPessoaJuridicaPorCnpj($cnpj);
		$xml = simplexml_load_string($xmlstring);
		$json = json_encode($xml);
		$array = json_decode($json);
		
		ob_clean();
		print simec_json_encode($array->PESSOA);
		die;
	}
	
	public function carregarMunicipios($uf) {
        ob_clean();
		print json_encode($this->listarMunicipios($uf));
		die;
	}

	public function getEsferas() {
		return array(
            Educriativa_Model_Organizacao::K_ESFERA_MUNICIPAL => 'Municipal',
            Educriativa_Model_Organizacao::K_ESFERA_ESTADUAL  => 'Estadual',
            Educriativa_Model_Organizacao::K_ESFERA_FEDERAL   => 'Federal',
        );
	}

	public function getPoderes() {
		return array(
            Educriativa_Model_Organizacao::K_PODER_LEGISLATIVO => 'Legislativo',
            Educriativa_Model_Organizacao::K_PODER_EXECUTIVO   => 'Executivo',
            Educriativa_Model_Organizacao::K_PODER_JUDICIARIO  => 'Judiciário',
        );
	}
}