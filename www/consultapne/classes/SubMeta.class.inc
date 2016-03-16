<?php
class SubMeta extends Modelo {

	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "consultapne.submeta";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"subid" 
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
                        'subid' => null,
			'metid' => null,
			'subtitulo' => null,
			'subtaxa' => null,
			'subtaxacrescnacional' => null,
			'submetanacional' => null,
			'subtempoconvergencia' => null,
			'subtaxaconvergencia' => null,
			'subpatamarminimo' => null,
			'substatus' => null,
			'subformulatcn' => null,
			'subformulatpc' => null,
			'subordem' => null,
			'subnotatecnica' => null,
			'subvalormetabrasil' => null,
			'subtipometabrasil' => null
	);
	
	public function carregaSubMetasPorMeta($meta) {
		$sql = " SELECT * FROM $this->stNomeTabela WHERE metid = '{$meta}' ORDER BY subordem";        
		$arResultado = $this->carregar( $sql );
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
                return $arResultado ? $arResultado : array();
	}
}