<?php
/**
 * Classe de abstração da tabela monitora.pi_complemento.
 *
 * @version $Id$
 * @see Pi_PlanoInterno
 */

/**
 *
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 *
 */
class Pi_Complemento extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planacomorc.pi_complemento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "picid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'picid' => null,
                                        'pliid' => null,
                                        'oppid' => null,
                                        'mppid' => null,
                                        'ippid' => null,
                                        'mpnid' => null,
                                        'ipnid' => null,
                                        'pprid' => null,
                                        'pumid' => null,
                                        'picquantidade' => null,
                                        'picted' => null,
                                        'picedital' => null,
                                        'esfid' => null,
                                        'picvalorcusteio' => null,
                                        'picvalorcapital' => null,
                                        'mescod' => null,
                                        'acuid' => null,
                                        'secid' => null,
									  );
}