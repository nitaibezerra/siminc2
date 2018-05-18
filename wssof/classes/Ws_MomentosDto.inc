<?php
/**
 * Classe de mapeamento da entidade wssof.ws_momentosdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_momentosdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_MomentosDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_momentosdto';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'codigomomento'
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
        'descricao' => null,
        'codigomomento' => null,
        'snativo' => null,
        'datahoraalteracao' => null,
        'dataultimaatualizacao' => null,
    );
    
    public function recuperarSqlCombo($descricao = null, $where = [], $order = 'codigomomento') {
        $pk = $this->arChavePrimaria[0];

        $where[] = "snativo = '1'";
        if(!$descricao) {
            $descricao = "{$this->stNomeTabela}.codigomomento || ' - ' || {$this->stNomeTabela}.descricao";
        }
        $order = $order ? $order : $descricao;
        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "select {$pk} AS codigo, {$descricao} AS descricao
                from {$this->stNomeTabela}
                $where
                order by $order   
        ";

        return $sql;
    }

}//end Class
?>