<?php

/**
 * Class Ted_Model_UnidadeOrcamentaria
 */
class Ted_Model_UnidadeOrcamentaria extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.unidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('unicod');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'unicod' => NULL,
        'unitpocod' => NULL,
        'orgcod' => NULL,
        'organo' => NULL,
        'tpocod' => NULL,
        'uniano' => NULL,
        'unidsc' => NULL,
        'unistatus' => NULL,
        'uniid' => NULL,
        'uniabrev' => NULL,
        'gunid' => NULL,
        'ungcodresponsavel' => NULL,
        'gstcod' => NULL,
        'orgcodsupervisor' => NULL,
        'uniddd' => NULL,
        'unitelefone' => NULL,
        'uniemail' => NULL,
        'unidataatualiza' => NULL
    );

    public function __construct()
    {

    }

    public function getUO()
    {
        $strSQL = "
            select unicod, unicod ||' - '|| unidsc as descricao 
            from public.unidade 
            where orgcod = '". CODIGO_ORGAO_SISTEMA. "'
            and unistatus = 'A'
            order by 1
        ";

        $options = array();
        $collection = $this->carregar($strSQL);
        if(is_array($collection)) {
            foreach ($collection as $row) {
                $options[$row['unicod']] = $row['descricao'];
            }
        }

        return $options;
    }

    /**
     *
     */
    public function pegaUnidades()
    {
        $strSQL = "
            SELECT uni.unicod AS codigo,
                   uni.unicod || ' - ' || unidsc AS descricao
              FROM {$this->stNomeTabela} uni
            WHERE uni.unistatus = 'A'
            ORDER BY uni.unicod
        ";

        return $this->carregar($strSQL);
    }
}