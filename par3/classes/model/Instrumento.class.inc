<?php

class Par3_Model_Instrumento extends Modelo
{
	/*
	 * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.instrumento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'itrid' => null,
									  	'itrdsc' => null,
									  );

	public function recuperarIntrumentosGuia()
	{
		$sql = "SELECT DISTINCT
					it.itrid,
					it.itrdsc
			    FROM par3.instrumento it
			    WHERE it.itrid IN ( 1, 2, 3 )
			    ORDER BY it.itrid
			    ";

		return $this->carregar($sql);
	}


}