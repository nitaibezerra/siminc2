<?php
class Educriativa_Model_Resposta extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "criatividadeeducacao.resposta";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"resid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		'resid' => null,
		'queid' => null,
		'perid' => null,
		'restexto' => null,
	);
	
	public function removerRespostas($pergunta = null) {
		try {
			$this->queid = $_SESSION['queid'];
	
			$sql = "DELETE FROM $this->stNomeTabela WHERE queid = {$this->queid}";
	
			if ($pergunta) {
				$sql.= " AND perid = {$pergunta}";
			}
			
			$this->executar($sql);
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function salvarResposta() {
		try {
			if (!empty($_REQUEST['restexto'])) {
				$this->removerRespostas($_REQUEST['perid']);
				
				$data = array();
				$data['perid'] = $_REQUEST['perid'];
				$data['restexto'] = utf8_decode($_REQUEST['restexto']);
				$data['queid'] = $_SESSION['queid'];

				$resposta = new Educriativa_Model_Resposta();
				$resposta->popularDadosObjeto($data);
				$resposta->salvar();
				$resposta->commit();
			} else {
				$this->removerRespostas($_REQUEST['perid']);
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function salvarRespostas($respostas) {
		try {
			$this->removerRespostas();
	
			if (!empty($respostas)) {
				foreach ($respostas as $pergunta => $resposta)
				{
					if (!empty($resposta)) {
						$data = array();
						$data['perid'] = $pergunta;
						$data['restexto'] = utf8_decode($resposta);
						$data['queid'] = $_SESSION['queid'];
						
						$resposta = new Educriativa_Model_Resposta();
						$resposta->popularDadosObjeto($data);
						$resposta->salvar();
						$resposta->commit();
					}
				}
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}