<?php
class Educriativa_Model_OrganizacaoFaixaEtaria extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.organizacaofaixaetaria";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"orfid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'orfid' => null,
		'orgid' => null,
		'faeid' => null,
	);
	
	public function removerFaixaEtaria() {
		try {
			$this->orgid = $_SESSION['orgid'];
	
			$sql = "DELETE FROM $this->stNomeTabela WHERE orgid = {$this->orgid}";
	
			$this->executar($sql);
				
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function salvarFaixaEtaria($faixaEtarias) {
		try {
			$this->removerFaixaEtaria();

			if (!empty($faixaEtarias)) {
				foreach ($faixaEtarias as $id)
				{
					$data = array();
					$data['faeid'] = $id;
					$data['orgid'] = $_SESSION['orgid'];
					
					$faixa = new Educriativa_Model_OrganizacaoFaixaEtaria();
					$faixa->popularDadosObjeto($data);
					$faixa->salvar();
					$faixa->commit();
				}
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}