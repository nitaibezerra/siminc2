<?php

/**
 * Classe de controle do DocumentoLegado
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_DocumentoLegado
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */
class Par3_Controller_DocumentoLegado extends Controle
{
    /* Tipo de documento
     * */
// 	const TERMO_COMPROMISSO = 1;

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função lista
     * - monta a lista de documentos.
     *
     * @return escreve a lista.
     *
     */
    public function lista($arrPost)
    {
//         $sql = Par3_Model_Documento::pegarSQLLista();
        $filtro = "AND dopstatus = 'A' AND dopidpai IS NULL";
        if ($arrPost['dopid'] != '') {
            $filtro = "AND dopstatus = 'I' AND dopid = {$arrPost['dopid']}";
        }

        $sql = "SELECT dopidpai, dopnumerodocumento FROM par.documentopar WHERE prpid IN (17922, 208, 20917, 7405, 19979, 9140, 22099) $filtro";

        $cabecalho = array('documento');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setAcaoComoCondicional('plus', array(array('campo' => 'dopidpai', 'valor' => '', 'operacao' => 'diferente')));
        $listagem->addAcao('edit', 'dadosUnidade');


        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end lista()


    /**
     * Função listaPAR
     * - monta a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function listaPAR($arrPost)
    {
        $arrPost['inuid'] = Par3_Model_InstrumentoUnidade::recuperarInuidPar($arrPost['inuid']);

        $sql = Par3_Model_DocumentoParLegado::montarSQLPAR($arrPost);

        $cabecalho = array('N° do Processo', 'N° do Documento', 'Tipo de Documento',
                           'Data de Vigência', 'Valor do Termo', 'Valor Empenhado',
                           'Pagamento Solicitado', 'Pagamento Efetivado', 'Dados Bancários',
                           'Saldo Bancário (CC + CP + Fundo)');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('processo','formata_numero_processo');
        $listagem->addCallbackDeCampo(Array('vt', 've', 'ps', 'vp', 'sb'),'formata_numero_monetario');
        $listagem->esconderColuna(array('arqid', 'dopidpai'));
        $listagem->addAcao('plus', 'listaHistorico');
        $listagem->setAcaoComoCondicional('plus', array(array('campo' => 'dopidpai', 'valor' => null, 'op' => 'diferente')));
        $listagem->addAcao('view', 'vizualizarTermo');
        $listagem->addAcao('download', 'baixarArquivoDopid');
        $listagem->setAcaoComoCondicional('download', array(array('campo' => 'arqid', 'valor' => null, 'op' => 'diferente')));

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaPAR()


    /**
     * Função listaObrasPAR
     * - monta a lista de documentos de Obras PAR.
     *
     * @return escreve a lista.
     *
     */
    public function listaObrasPAR($arrPost)
    {
        $arrPost['inuid'] = Par3_Model_InstrumentoUnidade::recuperarInuidPar($arrPost['inuid']);

        $sql = Par3_Model_DocumentoparLegado::montarSQLObrasPAR($arrPost);

        $cabecalho = array('N° do Processo', 'N° do Documento', 'Tipo de Documento',
                           'Data de Vigência', 'Qnt deObras', 'Valor do Termo', 'Valor Empenhado',
                           'Pagamento Solicitado', 'Pagamento Efetivado', 'Dados Bancários',
                           'Saldo Bancário (CC + CP + Fundo)');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
        $listagem->addCallbackDeCampo(Array('vt', 've', 'ps', 'vp', 'sb'),'formata_numero_monetario');
        $listagem->esconderColuna(array('arqid', 'dopidpai'));
        $listagem->addAcao('plus', 'listaHistorico');
        $listagem->setAcaoComoCondicional('plus', array(array('campo' => 'dopidpai', 'valor' => null, 'op' => 'diferente')));
        $listagem->addAcao('view', 'vizualizarTermo');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaObrasPAR()


    /**
     * Função listaObras
     * - monta a lista de documentos de Obras PAC.
     *
     * @return escreve a lista.
     *
     */
    public function listaObras($arrPost)
    {
        $arrPost['inuid'] = Par3_Model_InstrumentoUnidade::recuperarInuidPar($arrPost['inuid']);

        $sql = Par3_Model_TermocompromissopacLegado::montarSQLLista($arrPost);

        $cabecalho = array('N° do Processo', 'N° do Documento', 'Data da Validação',
                           'Vigência do Termo', 'Usuário da Validação', 'Qnt deObras',
                           'Valor do Termo', 'Valor Empenhado', 'Pagamento Solicitado',
                           'Pagamento Efetivado', 'Dados Bancários',
                           'Saldo Bancário (CC + CP + Fundo)');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
        $listagem->addCallbackDeCampo(Array('valor_termo', 'valorempenho', 'valorpagamentosolicitado', 'valorpagamento', 'sb'),'formata_numero_monetario');
        $listagem->esconderColuna(array('arqid', 'teridpai'));
        $listagem->addAcao('plus', 'listaHistoricoPAC');
        $listagem->setAcaoComoCondicional('plus', array(array('campo' => 'teridpai', 'valor' => null, 'op' => 'diferente')));
        $listagem->addAcao('view', 'vizualizarTermo');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaObras()


    /**
     * Função baixarArquivoDopid
     * - monta a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function baixarArquivoDopid($arrPost)
    {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $modelDocumento = new Par3_Model_DocumentoparLegado($arrPost['dopid']);

        $file = new FilesSimec("processopar",array(),"par");
        $file->getDownloadArquivo($modelDocumento->arqid);

        die();
    }//end baixarArquivoDopid()


}//end class

?>