<?php

class Controller_IdentificacaoGrupo extends Abstract_Controller
{
    public function __construct()
    {
        parent::__construct();
        $this->view->identificacaoGrupo = new Model_Identificacaogrupo();
    }

    public function salvarAction()
    {
        $identificacaoGrupo = new Model_Identificacaogrupo();
        $identificacaoGrupo->populateEntity($_POST);
        if ($identificacaoGrupo->save()) {
            $return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
        } else {
            $return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $identificacaoGrupo->error);
        }
        echo simec_json_encode($return);
    }

}
