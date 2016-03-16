<?php
/**
 * Modelo de Plano de Carreira
 * @author Wesley Romualdo da Silva
 */
class ParametrosPontuacao extends Modelo{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pisosalarial.parametrospontuacao";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "co_pontuacao" );

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'co_pontuacao' => null,
									  	'co_tabela' => null,
									  	'nu_coluna' => null,
									  	'nu_pontuacao' => null,
									  	'ds_parametro' => null
									  );

	public static $_pontos = array(1,2,3,4,5,6,7,8,9,10);

	public function recuperarPontuacaoPorTabela($co_tabela)
	{
        $sql = "SELECT
                    co_pontuacao,
                    nu_coluna,
                    nu_pontuacao,
                    ds_parametro
                FROM {$this->stNomeTabela}
                WHERE co_tabela = {$co_tabela}";

        return $this->carregar($sql);
	}
}
?>