<?php
/**
 * Classe de mapeamento da entidade wssof.ws_programasdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_programasdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_ProgramasDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_programasdto';

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
        'horizontetemporalcontinuo' => null,
        'estrategiaimplementacao' => null,
        'objetivo' => null,
        'codigomomento' => null,
        'snexclusaologica' => null,
        'codigoorgao' => null,
        'publicoalvo' => null,
        'titulo' => null,
        'exercicio' => null,
        'objetivosetorial' => null,
        'codigotipoprograma' => null,
        'unidaderesponsavel' => null,
        'problema' => null,
        'justificativa' => null,
        'objetivogoverno' => null,
        'identificadorunico' => null,
        'dataultimaatualizacao' => null,
        'codigoprograma' => null,
        'codigomacrodesafio' => null,
    );

}//end Class
?>