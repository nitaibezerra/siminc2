<?php
class Avaliacao extends Modelo {
	
	public static $niveis = array
	(
		5 => 'active btn-success',
		4 => 'active btn-primary',
		3 => 'active btn-default',
		2 => 'active btn-warning',
		1 => 'active btn-danger',
	);
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "consultapne.avaliacao";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"avaid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'avaid' => null,
			'queid' => null,
			'iteid' => null,
			'avaresposta' => null,
                        'subid' => null
	);
	
	public function __construct( $iteid = null, $subid = null ){
		parent::__construct();
	
		if ($iteid && $subid) {
			$arResultado = $this->carregarPorItem($iteid, $subid);
			$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
		}
	}
	
	public function inicializar($iteid, $subid = null) {
		try {
			$this->queid = $_SESSION['queid_pne'];
			$this->iteid = $iteid;
			$this->subid = $subid;

			$avaid = $this->salvar();
			$this->commit();
			
			$this->carregarPorId($avaid);
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function carregarPorItem($iteid, $subid) {
		$sql = " SELECT * FROM $this->stNomeTabela";
                $sql.= " WHERE iteid = '{$iteid}' AND queid = {$_SESSION['queid_pne']} AND subid = '{$subid}'";
	
		$arResultado = $this->pegaLinha( $sql );
		
		if ($arResultado) {
			$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
		} else {
			$this->inicializar($iteid, $subid);
		}
		
		return $this;
	}
	
	public function carregarRespostas() {
		$sql = " SELECT a.avaid, a.queid, a.iteid, a.avaresposta, c.comdsc FROM $this->stNomeTabela a ";
		$sql.= " LEFT JOIN consultapne.comentario c ON c.avaid = a.avaid ";
		$sql.= " WHERE queid = {$_SESSION['queid_pne']}";
	
		$respostas = $this->carregar( $sql );
	
		return $respostas ? $respostas : array();
	}
	
	public function carregarRespostasPorSubMeta($subMeta) {
		$sql = " SELECT a.avaid, a.queid, a.iteid, a.avaresposta, c.comdsc FROM $this->stNomeTabela a ";
		$sql.= " LEFT JOIN consultapne.comentario c ON c.avaid = a.avaid ";
		$sql.= " WHERE queid = {$_SESSION['queid_pne']}";
                $sql.= "   AND a.subid = '{$subMeta}'";
	
		$respostas = $this->carregar( $sql );
		return $respostas ? $respostas : array();
	}
	
	public function avaliar() {
		try {                    
            $avaliacao = $this->carregarPorItem($_REQUEST['iteid'], $_REQUEST['subid']);
            $data = Encode::utf8_decode_recursive($_REQUEST);
            $this->popularDadosObjeto($data);
            $this->avaid = $avaliacao->avaid;
            $this->queid =  $_SESSION['queid_pne'];

            $this->alterar();

            $this->commit();
		} catch (Exception $e) {
                    throw new Exception($e->getMessage());
		}
	}
}