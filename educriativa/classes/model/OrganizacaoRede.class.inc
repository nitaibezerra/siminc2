<?php
class Educriativa_Model_OrganizacaoRede extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.organizacaorede";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"oreid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'oreid' => null,
		'orgid' => null,
		'redid' => null,
	);
	
	public function removerRedes() {
		try {
			$this->orgid = $_SESSION['orgid'];
	
			$sql = "DELETE FROM $this->stNomeTabela WHERE orgid = {$this->orgid}";
	
			$this->executar($sql);
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function salvarRedes($redes) {
		try {
			$this->removerRedes();
				
			if (!empty($redes)) {
				foreach ($redes as $id)
				{
					$data = array();
					$data['redid'] = $id;
					$data['orgid'] = $_SESSION['orgid'];
						
					$rede = new Educriativa_Model_OrganizacaoRede();
					$rede->popularDadosObjeto($data);
					$rede->salvar();
					$rede->commit();
				}
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}