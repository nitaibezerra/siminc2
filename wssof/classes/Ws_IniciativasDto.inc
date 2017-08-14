<?php
/**
 * Classe de mapeamento da entidade wssof.ws_iniciativasdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_iniciativasdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_IniciativasDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_iniciativasdto';

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
        'codigoobjetivo' => null,
        'codigoiniciativa' => null,
        'codigomomento' => null,
        'snexclusaologica' => null,
        'codigoorgao' => null,
        'titulo' => null,
        'exercicio' => null,
        'identificadorunico' => null,
        'dataultimaatualizacao' => null,
        'snindividualizada' => null,
        'codigoprograma' => null,
    );

}//end Class
?>