<?php
class Educriativa_Model_OrganizacaoAreaAtuacao extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.organizacaoareaatuacao";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"oaaid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'oaaid' => null,
		'orgid' => null,
		'araid' => null,
		'oaadscoutro' => null,
	);
	
	public function removerAreaAtuacao() {
		try {
			$this->orgid = $_SESSION['orgid'];
	
			$sql = "DELETE FROM $this->stNomeTabela WHERE orgid = {$this->orgid}";
	
			$this->executar($sql);
				
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function salvarAreaAtuacao($areas) {
		try {
			$this->removerAreaAtuacao();

			if (!empty($areas)) {
				foreach ($areas as $id)
				{
					$data = array();
					$data['araid'] = $id;
					$data['orgid'] = $_SESSION['orgid'];
					
					$atuacao = new Educriativa_Model_OrganizacaoAreaAtuacao();
					$atuacao->popularDadosObjeto($data);
					$atuacao->salvar();
					$atuacao->commit();
				}
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}