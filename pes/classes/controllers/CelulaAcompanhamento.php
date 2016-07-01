<?php

class Controller_CelulaAcompanhamento extends Abstract_Controller
{

    public function carregarDadosFormularioAction()
    {
    	$modelTipoDespesa = new Model_TipoDespesa();
    	$modelAcompanhamento = new Model_CelulaAcompanhamento;
    	
    	$dados['modelTipoDespesa']     = $modelTipoDespesa->fetchAll();
    	$dados['celulaAcompanhamento'] = $modelAcompanhamento->fetchAll(array('concodigo = 6', 'ceaano in (2012, 2013)'), 'ceaano');
    	
    	return $dados;
    }    
}