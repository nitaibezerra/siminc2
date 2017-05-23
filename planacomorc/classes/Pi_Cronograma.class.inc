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
class Pi_Cronograma extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planacomorc.pi_cronograma";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pcrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'pcrid' => null,
                                        'pliid' => null,
                                        'mescod' => null,
                                        'pcrvalor' => null,
                                        'crvid' => null,
    );

    public function recuperarCronogramas()
    {
        $sql = "select * 
                from planacomorc.cronograma_valor cv
                    inner join planacomorc.cronograma c on c.croid = cv.croid
                order by crvid";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}