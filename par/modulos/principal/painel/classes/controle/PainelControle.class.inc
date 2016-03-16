<?php
require (APPRAIZ . 'par/modulos/principal/painel/classes/modelo/Painel.class.inc');

class PainelControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    public function gerenciarPainel($arrSessao, $arrPost) {
        
        #Formatando dados
        $idgrafico = key($arrSessao);
        $params = array();
        parse_str($arrPost['arrPost'], $params);
        $params['idArea'] = $arrSessao[$idgrafico];
        $params['idGrafico'] = $idgrafico;
        
        $arrResultado = array();
        switch ($idgrafico) {
            case 'pagamentoobrapar':
            case 'pagamentoobrapac':
            case 'obrasaprovadaspac':
                $obPreObraControle = new PreObraControle();
                $arrResultado = $obPreObraControle->recuperarObrasAprovadasEmpenhoPac($params);
                break;
            case 'obrasaprovadaspar':
                $obPreObraControle = new PreObraControle();
                $arrResultado = $obPreObraControle->recuperarObrasAprovadasEmpenhoPar($params);
                break;
        }

        return $arrResultado;
    }
    
    /**
     * Retorna a lista de paineis de acordo com o usuario logado.
     * @return array $arrPainel
     */
    public function getPainelByUsuario() {
        $obPainel = new Painel();
        return $obPainel->getPainelByUsuario();
    }

}
