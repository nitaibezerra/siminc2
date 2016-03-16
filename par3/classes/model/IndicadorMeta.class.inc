<?php

class Par3_Model_IndicadorMeta extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.indicadormeta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "inmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'inmid' => null,
									  	'indid' => null,
									  	'inmmeta' => null
									  );

	protected $stOrdem = null;

	public function recuperarDados($indid)
	{
		$sql = "SELECT
		            inmid,
		            indid,
				    inmmeta
				FROM {$this->stNomeTabela}
				WHERE
				    indid = {$indid}";

		return $this->carregar($sql);
	}

	public function preparaSalvar($indid, $meta){
	    $this->indid 		= $indid;
	    $this->inmmeta 	    = $meta;
	    $this->salvar();
	    $this->commit();
	}

	public function preparaExcluir($inmid){
	    $this->inmid 		= $inmid;
	    $this->excluir();
	    $this->commit();
	}
}