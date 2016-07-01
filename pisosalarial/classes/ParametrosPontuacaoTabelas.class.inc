<?php
/**
 * Modelo de Plano de Carreira
 * @author Wesley Romualdo da Silva
 */
class ParametrosPontuacaoTabelas extends Modelo{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pisosalarial.parametrospontuacaotabelas";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "co_tabela" );

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'co_tabela' => null,
									  	'ds_tabela' => null,
									  	'st_tabela' => null,
									  	'sg_tabela' => null,
									  	'ds_grid' => null
									  );

	public function recuperarTabelaPorId($co_tabela)
	{
        $sql = "SELECT
                    co_tabela,
                    ds_tabela,
                    st_tabela,
                    sg_tabela,
                    ds_grid
                FROM {$this->stNomeTabela}
                WHERE co_tabela = {$co_tabela}";

        return $this->pegaLinha($sql);
	}

	public function recuperarTodos()
	{
	    $sql = "SELECT
	               *
	            FROM {$this->stNomeTabela}
	            WHERE bo_status = 'A'";

	    return $this->carregar($sql);
	}
}
?>