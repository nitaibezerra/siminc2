<?php
/**
 * Classe de mapeamento do seguimento da equipe local
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-10-2015
 * @link     no link
 */


/**
 * Classe Par3_Model_EquipeLocalSeguimento
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-10-2015
 * @link     no link
 */
class Par3_Model_EquipeLocalSeguimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.equipelocal_seguimento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'eseid',
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
        'eseid' => null,
        'esedsc' => null,
    );


    /**
     * Função pegarSQLSelectCombo
     * - retorna uma query para o monta combo.
     *
     * @return string $sql.
     *
     */
    public function pegarSQLSelectCombo()
    {
        $sql = "SELECT ese.eseid as codigo, ese.esedsc as descricao
                FROM par3.equipelocal_seguimento ese
                INNER JOIN par3.equipelocal_cargo eca ON eca.eseid = ese.eseid";

        return $sql;

    }//end pegarSQLSelectCombo()

}