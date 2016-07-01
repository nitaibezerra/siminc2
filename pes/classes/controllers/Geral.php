<?php


class Controller_Geral extends Abstract_Controller
{
    public function campoMunicipioAction()
    {
        $modelMunicipio = new Model_Municipio();

        $ufecodigo = (isset($_POST['ufecodigo']))? $_POST['ufecodigo'] : NULL;
        $muncodigo = (isset($_POST['muncodigo']))? $_POST['muncodigo'] : NULL;

        if($ufecodigo)
            $municipios = $modelMunicipio->getAllByValues(array('ufecodigo' => $ufecodigo));
        else 
            $municipios = null;
        
        $this->view->municipios = $municipios;
        $this->view->muncodigo = $muncodigo;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function campoTipoDespesaAction()
    {
        $modelTipoDespesa = new Model_TipoDespesa();
        $tipoDespesas = $modelTipoDespesa->getAll();
        
        $this->view->values = $tipoDespesas;
        
        $this->render(__CLASS__, __FUNCTION__);
    }
}