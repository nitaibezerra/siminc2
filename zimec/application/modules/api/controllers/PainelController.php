<?php
import('api.business.PainelBusiness');

class Api_PainelController extends Simec_Controller_Rest
{
    private $businessPainel;

    public function init()
    {
        $this->businessPainel = new Api_Business_PainelBusiness();
    }

    /**
     * @Rest(uri="/api/painel/caixas", method="GET")
     *
     */
    public function caixasAction()
    {
        $caixas = $this->businessPainel->listaCaixasPesquisa();
        $this->_encode($caixas);
    }

    /**
     * @Rest(uri="/api/painel/escolas", method="GET")
     *
     */
    public function escolasAction()
    {
        $busca = $this->getRequest()->getParam('filtro');
        //   die();
        $escolas = $this->businessPainel->listaEscolas($busca);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/estados", method="GET")
     *
     */
    public function estadosAction()
    {
        $busca = $this->getRequest()->getParam('filtro');
        //   die();
        $escolas = $this->businessPainel->listaEstados($busca);
        $this->_encode($escolas);
    }


    /**
     * @Rest(uri="/api/painel/indicadores", method="GET")
     *
     */
    public function indicadoresAction()
    {
        $busca = $this->getRequest()->getParam('filtro');
        //   die();
        $escolas = $this->businessPainel->listaIndicadores($busca);
        $this->_encode($escolas);
    }



    /**
     * @Rest(uri="/api/painel/municipios", method="GET")
     *
     */
    public function municipiosAction()
    {
        $busca = $this->getRequest()->getParam('filtro');
        //   die();
        $municipios = $this->businessPainel->listaMunicipios($busca);
        $this->_encode($municipios);
    }

    /**
     * @Rest(uri="/api/painel/escolasdetalhamento", method="GET")
     *
     */
    public function escolasdetalhamentoAction()
    {
        $busca = $this->getRequest()->getParam('filtro');
        //   die();
        $escolas = $this->businessPainel->listaDetalhamentoEscolas($busca);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/municipiosdetalhamento", method="GET")
     *
     */
    public function municipiosdetalhamentoAction()
    {
        $busca = $this->getRequest()->getParam('filtro');
        //   die();
        $escolas = $this->businessPainel->listaDetalhamentoMunicipios($busca);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/estadosdetalhamento", method="GET")
     *
     */
    public function estadosdetalhamentoAction()
    {
        $busca = $this->getRequest()->getParam('filtro');
        //   die();
        $escolas = $this->businessPainel->listaDetalhamentoEstados($busca);
        $this->_encode($escolas);
    }



    /**
     * @Rest(uri="/api/painel/escolasmapa", method="GET")
     *
     */
    public function escolasmapaAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        //   die();
        $escolas = $this->businessPainel->listaMapaEscolas($indid, $dshcod);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/indicadoresmapa", method="GET")
     *
     */
    public function indicadoresmapaAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        //   die();
        $escolas = $this->businessPainel->listaMapaIndicadores($indid, $dshcod);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/municipiosmapa", method="GET")
     *
     */
    public function municipiosmapaAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        //   die();
        $escolas = $this->businessPainel->listaMapaMunicipios($indid, $dshcod);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/estadosmapa", method="GET")
     *
     */
    public function estadosmapaAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        //   die();
        $escolas = $this->businessPainel->listaMapaEstados($indid, $dshcod);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/detalhamentoindicador", method="GET")
     *
     */
    public function detalhamentoindicadorAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $escolas = $this->businessPainel->detalhamentoIndicador($indid);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/tabelaindicadorescolas", method="GET")
     *
     */
    public function tabelaindicadorescolasAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        $escolas = $this->businessPainel->tabelaIndicadorEscolas($indid,$dshcod);
        $this->_encode($escolas);
    }
    /**
     * @Rest(uri="/api/painel/tabelaindicadormunicipios", method="GET")
     *
     */
    public function tabelaindicadormunicipiosAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        $escolas = $this->businessPainel->tabelaIndicadorMunicipios($indid,$dshcod);
        $this->_encode($escolas);
    }
    /**
     * @Rest(uri="/api/painel/tabelaindicadorestados", method="GET")
     *
     */
    public function tabelaindicadorestadosAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        $escolas = $this->businessPainel->tabelaIndicadorEstados($indid,$dshcod);
        $this->_encode($escolas);
    }
    /**
     * @Rest(uri="/api/painel/tabelaindicadorindicadores", method="GET")
     *
     */
    public function tabelaindicadorindicadoresAction()
    {
        $indid = $this->getRequest()->getParam('indid');
        $dshcod = $this->getRequest()->getParam('dshcod');
        $escolas = $this->businessPainel->tabelaIndicadorIndicadores($indid,$dshcod);
        $this->_encode($escolas);
    }

    /**
     * @Rest(uri="/api/painel/unidadeindicador", method="GET")
     *
     */
    public function unidadeindicadorAction()
{
    $indid = $this->getRequest()->getParam('indid');
    $escolas = $this->businessPainel->UnidademetaIndicador($indid);
    $this->_encode($escolas);
}


}