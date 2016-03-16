<?php
/**
 * Classe de mapeamento da entidade publicacao.solicitacao.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade publicacao.solicitacao.
 *
 * @see Modelo
 */
class Publicacao_Model_Solicitacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'publicacao.solicitacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'solid'
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
        'solid' => null,
        'usucpf' => null,
        'solconteudo' => null,
        'soltestado' => null,
        'solexecutado'=>null,
        'solquery'=>null,
        'solpublicado'=>null,
        'solqueryexecutada'=>null,
        'solurgente'=>null,
        'soldemanda'=>null,
        'docid'=>null
    );
    
    public function recuperaSolid(){
    $query = <<<DML
    SELECT 
        solid 
    FROM {$this->stNomeTabela} 
    WHERE usucpf = '{$this->arAtributos['usucpf']}'
    ORDER BY solid DESC limit 1
DML;
    return $this->pegaUm($query);
}

}