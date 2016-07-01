<?php
class Educriativa_Model_OrganizacaoSiteWeb extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.organizacaositeweb";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"osiid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'osiid' => null,
		'orgid' => null,
		'sitid' => null,
		'osidsc' => null,
	);
	
	public function removerSites() {
		try {
			$this->orgid = $_SESSION['orgid'];
	
			$sql = "DELETE FROM $this->stNomeTabela WHERE orgid = {$this->orgid}";
	
			$this->executar($sql);
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function salvarSites($sites) {
		try {
			$this->removerSites();
			
			if (!empty($sites)) {
				foreach ($sites as $id => $descricao)
				{
					if (!empty($descricao)) {
						$data = array();
						$data['sitid'] = $id;
						$data['osidsc'] = $descricao;
						$data['orgid'] = $_SESSION['orgid'];
							
						$site = new Educriativa_Model_OrganizacaoSiteWeb();
						$site->popularDadosObjeto($data);
						$site->salvar();
						$site->commit();
					}
				}
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}