<?php
/**
 * @versao $Id: MultiCadastro.php 97473 2015-05-19 13:34:52Z OrionMesquita $
 */

/**
 *
 */
abstract class Simec_Controller_MultiCadastro extends Simec_Controller_Action
{
    protected $campoPai;
    protected $modelName;

    public function indexAction()
    {
        $baseUrl = $this->view->baseUrl($this->_request->getModuleName() . '/' . $this->_request->getControllerName() . '/');

        $multiCadastro = new Simec_MultiCadastro();
        $multiCadastro->setListagem($baseUrl   . 'listagem/' . $this->campoPai . '/' . $this->_getParam($this->campoPai))
                      ->setFormulario($baseUrl . 'formulario/' . $this->campoPai . '/' . $this->_getParam($this->campoPai))
                      ->setActionForm($baseUrl . 'gravar')
                      ;

        $this->view->multiCadastro = $multiCadastro;
    }

    public function listagemAction()
    {
        $this->_helper->layout()->disableLayout();

        $model = new $this->modelName;
        $this->view->rowSet = $model->fetchAll(array($this->campoPai . ' = ?' => $this->_getParam($this->campoPai)));

        $dados = $this->_getAllParams();

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $listagem->setDados($this->view->rowSet);

        $this->view->listagem = $listagem->render(
            Simec_Listagem::SEM_REGISTROS_MENSAGEM,
            Simec_Listagem::RETORNO_BUFFERIZADO
        );
    }

    public function formularioAction()
    {
        $this->_helper->layout()->disableLayout();

        $model = new $this->modelName;
        $this->view->row = $model->getRow($this->_getParam($this->campoPai));
        
        $this->view->camposComErro = Simec_Util::getSession('form_validation_error');
        Simec_Util::clear('form_validation_error');
    }
}
