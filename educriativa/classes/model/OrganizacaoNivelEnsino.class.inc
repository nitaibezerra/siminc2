<?php
class Educriativa_Model_OrganizacaoNivelEnsino extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.organizacaonivelensino";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"oneid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'oneid' => null,
		'orgid' => null,
		'nieid' => null,
	);
	
	public function removerNiveis() {
		try {
			$this->orgid = $_SESSION['orgid'];
	
			$sql = "DELETE FROM $this->stNomeTabela WHERE orgid = {$this->orgid}";
	
			$this->executar($sql);
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function salvarNiveis($niveis) {
		try {
			$this->removerNiveis();
			
			if (!empty($niveis)) {
				foreach ($niveis as $id)
				{
					$data = array();
					$data['nieid'] = $id;
					$data['orgid'] = $this->orgid;
			
					$nivel = new Educriativa_Model_OrganizacaoNivelEnsino();
					$nivel->popularDadosObjeto($data);
					$nivel->salvar();
					$nivel->commit();
				}
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}