<?php
/**
 * Classe de mapeamento da entidade wssof.ws_financeirodto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_financeirodto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_FinanceiroDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_financeirodto';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
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
        'proid' => null,
        'fonte' => null,
        'resultadoprimariolei' => null,
        'iduso' => null,
        'naturezadespesa' => null,
        'resultadoprimarioatual' => null,
        'codigoplanoorcamentario' => null,
        'valor' => null,
        'identificadorplanoorcamentario' => null,
        'expansaoconcedida' => null,
        'dataultimaatualizacao' => null,
        'expansaosolicitada' => null,
        'idoc' => null,
    );

}//end Class
?>