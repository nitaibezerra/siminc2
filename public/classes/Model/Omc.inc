<?php
/**
 * Classe de mapeamento da entidade public.omc
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_Omc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_Omc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.omc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'id',
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
        'id' => null,
        'segmento' => null,
        'paragrafo2' => null,
        'nome' => null,
        'nome_artistico' => null,
        'indicacao' => null,
        'sexo' => null,
        'endereco' => null,
        'cep' => null,
        'telefone_residencial' => null,
        'telefone_celular' => null,
        'email' => null,
        'justificativa' => null,
        'paragrafo13' => null,
        'nome_indicou' => null,
        'sexo_indicou' => null,
        'endereco_indicou' => null,
        'cep_indicou' => null,
        'telefone_residencial_indicou' => null,
        'telefone_celular_indicou' => null,
        'email_indicou' => null,
        'ano' => null,
    );

}//end Class
?>