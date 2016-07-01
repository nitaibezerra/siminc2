<?php
/**
 * Classe de controle da Equipe Local
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */

/**
 * Par3_Model_EquipeLocalCargo
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */
class Par3_Model_EquipeLocalCargo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.equipelocal_cargo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ecaid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'eseid' => array('tabela' => 'par3.equipelocal_seguimento', 'pk' => 'eseid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ecaid' => null,
        'eseid' => null,
        'ecadsc' => null,
    );


    /**
     * Função pegarSQLSelectCombo
     * - monta o sql para o combo de cargo da equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function pegarSQLSelectCombo($arrPost)
    {
        $sql = "SELECT '' as codigo, '' as descricao";
        if($arrPost['eseid'] != ''){
            $sql = "SELECT
                    	ecaid as codigo,
                    	ecadsc as descricao
                    FROM
                    	par3.equipelocal_cargo
                    WHERE
                    	eseid = {$arrPost['eseid']}";
        }

        return $sql;
    }//end formPessoaJuridica()


    /**
     * Função carregarJSONCombo
     * - monta JSON para combo
     *
     * @return string [JSON].
     *
     */
    public function carregarJSONCombo($arrPost)
    {
        $sql = self::pegarSQLSelectCombo($arrPost);

        $arrDados = self::carregar($sql);

        return json_encode($arrDados);
    }//end carregarJSONCombo()


    /**
     * Função listaEquipe
     * - monta a lista de equipe.
     *
     * @return escreve a lista.
     *
     */
    public function listaEquipe($arrPost)
    {
        $sql = Par3_Model_EquipeLocal::pegarSQLLista($arrPost);

        $cabecalho = array('CPF', 'Nome', 'Função/Cargo', 'Segmento', 'E-mail');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addAcao('delete', 'dadosUnidade');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaEquipe()


}//end Class
?>