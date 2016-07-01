<?php
/**
 * Classe de mapeamento da entidade proporc.cargapreenchimento.
 *
 * $Id: Cargapreenchimento.php 100665 2015-07-31 18:36:43Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.cargapreenchimento.
 *
 * @see Modelo
 */
class Proporc_Model_Cargapreenchimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.cargapreenchimento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'cprid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cprid' => null,
        'ungcod' => null,
        'acacod' => null,
        'loccod' => null,
        'plocod' => null,
        'sbacod' => null,
        'ndpcod' => null,
        'foncod' => null,
        'metalocalizador' => null,
        'metapo' => null,
        'valor' => null,
        'unicod' => null,
    );

    public function antesSalvar()
    {
        $this->valor = str_replace(array('.', ','), array('', '.'), $this->valor);
        return parent::antesSalvar();
    }
}
