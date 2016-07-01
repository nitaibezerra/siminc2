<?php
/**
 * Classe de mapeamento da entidade aspar.tprperfil.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade aspar.tprperfil.
 *
 * @see Modelo
 */
class Aspar_Model_TprPerfil extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.tprperfil';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prfid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'tprcod' => array('tabela' => 'aspar.tiporesponsabilidade', 'pk' => 'tprcod'),
        'pflcod' => array('tabela' => 'perfil', 'pk' => 'pflcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prfid' => null,
        'tprcod' => null,
        'pflcod' => null,
    );

}