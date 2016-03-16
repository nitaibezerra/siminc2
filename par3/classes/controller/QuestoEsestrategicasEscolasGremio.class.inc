<?php

/**
 * Classe de controle da escolas com Gremio
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_QuestoEsestrategicasEscolasGremio
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */
class Par3_Controller_QuestoEsestrategicasEscolasGremio extends Controle
{
    /* Constantes da Equipe local
     * */
// 	const PREFEITURA          = 1;

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função formEscolasGremio
     * - monta o formulario de escolas com Gremio
     *
     * @return string escreve a lista.
     *
     */
    public function formEscolasGremio()
    {
        require_once APPRAIZ.'par3/modulos/principal/planoTrabalho/questoesEstrategicas/escolas_gremio.php';
    }//end formEscolasGremio()

    public static function formatarEntidComoCheckbox($entid, $entid_salvo, $arrPost) {
        $checkbox =  "<input type=checkbox class=js-switch ";
        $checkbox .= ($entid_salvo != '') ? 'checked="checked"' : '';
        $checkbox .= " qrpid={$arrPost['qrpid']} ";
        $checkbox .= " perid={$arrPost['perid']} ";
        $checkbox .= " entid=$entid />";

        return $checkbox;

    }


    /**
     * Função listaEscolasGremio
     * - monta a lista de escolas com Gremio.
     *
     * @return escreve a lista.
     *
     */
    public function listaEscolasGremio($arrPost)
    {
        $sql = Par3_Model_QuestoEsestrategicasEscolasGremio::pegarSQLLista($arrPost);

        $cabecalho = array('', 'Município', 'Código da Escola', 'Escola');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColuna('id');
        $listagem->addCallbackDeCampo('entid', function($entid, $dados, $id) use ($arrPost){
            return self::formatarEntidComoCheckbox($id, $entid, $arrPost);
        });
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaEscolasGremio()


    /**
     * Função salvarEscolaGremio
     * - inativa membro da equipe.
     *
     * @return void.
     *
     */
    public function salvarEscolaGremio($arrPost)
    {
        $modelEscola = new Par3_Model_QuestoEsestrategicasEscolasGremio();

        if ($arrPost['check'] != 'null') {
            $modelEscola->popularDadosObjeto($arrPost);
            $modelEscola->salvar();
            $modelEscola->commit();
        } else {
            $modelEscola->excluirPorEntid($arrPost);
        }

        $modelEscola->commit();

    }//end salvarEscolaGremio()


}//end class

?>