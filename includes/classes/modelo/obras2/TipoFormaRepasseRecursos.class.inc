<?php

class TipoFormaRepasseRecursos extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoformarepasserecursos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("frpid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'frpid' => null,
        'frpdesc' => null,
        'frpstatus' => null,
        'frpdtinclusao' => null,
    );

    public function listaCombo($frpid = null)
    {
        $where = ($frpid) ? "frpstatus = 'A' OR frpid IN ($frpid)" : "frpstatus = 'A'";
        $sql = "SELECT
			    	frpid AS codigo,
			    	frpdesc AS descricao
		    	FROM
		    		obras2.tipoformarepasserecursos
		    	WHERE
		    		$where
		    	ORDER BY
					frpdesc ASC";
        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    }
}