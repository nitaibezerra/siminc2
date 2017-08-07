<?php
/**
 * Classe de mapeamento da entidade wssof.ws_propostadto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_propostadto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_PropostaDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_propostadto';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('proid');
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
        'expansaofisicasolicitada' => null,
        'codigosubfuncao' => null,
        'codigomomento' => null,
        'codigotipoinclusaolocalizador' => null,
        'codigoacao' => null,
        'justificativaexpansaosolicitada' => null,
        'codigoorgao' => null,
        'identificadorunicoacao' => null,
        'codigofuncao' => null,
        'exercicio' => null,
        'valorfisico' => null,
        'codigotipodetalhamento' => null,
        'expansaofisicaconcedida' => null,
        'codigotipoinclusaoacao' => null,
        'codigolocalizador' => null,
        'justificativa' => null,
        'snatual' => null,
        'dataultimaatualizacao' => null,
        'quantidadefisico' => null,
        'codigoprograma' => null,
        'codigoesfera' => null,
        'justificativaexpansaoconcedida' => null,
    );

}//end Class
?>