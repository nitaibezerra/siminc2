<?php
/**
 * Classe de mapeamento da entidade wssof.ws_indicadoresdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_indicadoresdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_IndicadoresDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_indicadoresdto';

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
        'descricao' => null,
        'fonte' => null,
        'valorreferencia' => null,
        'codigomomento' => null,
        'codigoperiodicidade' => null,
        'snexclusaologica' => null,
        'dataapuracao' => null,
        'exercicio' => null,
        'snapuracaoreferencia' => null,
        'codigoindicador' => null,
        'codigounidademedidaindicador' => null,
        'codigobasegeografica' => null,
        'formula' => null,
        'identificadorunico' => null,
        'dataultimaatualizacao' => null,
        'codigoprograma' => null,
    );

}//end Class
?>