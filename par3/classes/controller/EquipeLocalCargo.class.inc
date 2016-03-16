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
class Par3_Controller_EquipeLocalCargo extends Controle
{
    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função pegarSQLSelectCombo
     * - monta o sql para o combo de cargo da equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function pegarSQLSelectCombo($disabled = 'disabled', $objPessoaJuridica = null)
    {
        $sql = "SELECT
                	ecaid as codigo,
                	ecadsc as descricao
                FROM
                	par3.equipelocal_cargo
                WHERE
                	eseid = 1";

        return $sql;
    }//end formPessoaJuridica()


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