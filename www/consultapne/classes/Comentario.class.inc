<?php
class Comentario extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "consultapne.comentario";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"comid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'comid' => null,
			'avaid' => null,
			'comdsc' => null
	);
	
	public function recuperarPorAvaliacao($avaid) {
		$sql = " SELECT * FROM $this->stNomeTabela";
		$sql.= " WHERE avaid = '{$avaid}'";
	
		$comentario = $this->pegaLinha( $sql );
	
		return $comentario ? $comentario : array();
	}
	
	public function comentar() {
		try {
			$avaliacao = new Avaliacao($_REQUEST['iteid'], $_REQUEST['subid']);

			$comentario = $this->recuperarPorAvaliacao($avaliacao->avaid);
			
			$data = Encode::utf8_decode_recursive($_REQUEST);
	
			$this->popularDadosObjeto($data);
			$this->comid = $comentario['comid'];
			$this->avaid = $avaliacao->avaid;

			$this->comid ? $this->alterar() : $this->salvar();
	
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
}