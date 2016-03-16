<?php
class Api_Business_PainelBusiness
{
    private $modelCaixaPesquisa;
    private $modelCategoriaIndicador;
    private $modelIndicador;

    public function __construct()
    {
        $this->modelCaixaPesquisa = new Model_Painel_CaixaPesquisa();
        $this->modelCategoriaIndicador = new Model_Painel_CategoriaIndicador();
        $this->modelIndicador = new Model_Painel_Indicador();
    }

    public function listaCaixasPesquisa()
    {
        return $this->modelCaixaPesquisa->getCaixasPesquisa()->toArray();
    }

    public function listaEscolas($busca)
    {

        return $this->modelCaixaPesquisa->getEscolas($busca)->toArray();
    }

    public function listaMunicipios($busca)
    {

        return $this->modelCaixaPesquisa->getMunicipios($busca);
    }
    public function listaEstados($busca)
    {

        return $this->modelCaixaPesquisa->getEstados($busca)->toArray();
    }
    public function listaIndicadores($busca)
    {

        return $this->modelCaixaPesquisa->getIndicadores($busca);
    }


    public function listaDetalhamentoEscolas($busca)
    {
        return $this->modelCategoriaIndicador->getDetalhamentoEscolas($busca);
    }

    public function listaDetalhamentoEstados($busca)
    {
        return $this->modelCategoriaIndicador->getDetalhamentoEstados($busca);
    }

    public function listaDetalhamentoMunicipios($busca)
    {
        return $this->modelCategoriaIndicador->getDetalhamentoMunicipios($busca);
    }


    public function listaMapaEscolas($indid,$dshcod)
    {
        return $this->modelIndicador->getMapaEscolas($indid,$dshcod);
    }
    public function listaMapaMunicipios($indid,$dshcod)
    {
        return $this->modelIndicador->getMapaMunicipios($indid,$dshcod);
    }
    public function listaMapaIndicadores($indid,$dshcod)
    {
        return $this->modelIndicador->getMapaIndicadores($indid,$dshcod);
    }
    public function listaMapaEstados($indid,$dshcod)
    {
        return $this->modelIndicador->getMapaEstados($indid,$dshcod);
    }

    public function detalhamentoIndicador($indid)
    {
        return $this->modelIndicador->getDetalhamentoIndicador($indid);
    }

    //TABELAS

    public function tabelaIndicadorEscolas($indid,$dshcod)
    {
        return $this->modelIndicador->getTabelaIndicadorEscolas($indid,$dshcod);
    }
    public function tabelaIndicadorMunicipios($indid,$dshcod)
    {
        return $this->modelIndicador->getTabelaIndicadorMunicipios($indid,$dshcod);
    }
    public function tabelaIndicadorEstados($indid,$dshcod)
    {
        return $this->modelIndicador->getTabelaIndicadorEstados($indid,$dshcod);
    }
    public function tabelaIndicadorIndicadores($indid,$dshcod)
    {
        return $this->modelIndicador->getTabelaIndicadorIndicadores($indid,$dshcod);
    }


    public function UnidademetaIndicador($indid)
    {
        return $this->modelIndicador->getUnidademeta($indid);
    }
}