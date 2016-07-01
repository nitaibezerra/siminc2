<?php

include_once APPLICATION_PATH . '/../library/Simec/legacy/Listagem.php';

class Seguranca_ResourceController extends Simec_Controller_Action

{

    public function indexAction()
    {
        try {
            $model = new Model_Seguranca_Resource();

            $requet = $this->getRequest();

            $campos = array('rscmodulo', 'rscontroller');

            $rows = $model->fetchAll($this->_filter($campos), $this->_order('rscmodulo'), $this->_count(), $this->_offset());

            $this->view->rows = $rows;
        } catch (Exception $e) {
            // xd($e->getMessage());
            $this->_message(MSG_ERROR, $e->getMessage());
        }
    }

    public function formularioAction()
    {
        $model = new Model_Aspar_Proposicao();

        $this->view->row = $model->getRow(Simec_Util::decode($this->_getParam('prpid')));
    }

    public function gravarAction()
    {
        $dados = $this->getRequest()->getPost();

        $model = new Model_Seguranca_Resource();

        try {
            $model->beginTransaction();
            $dados['sisid'] = '226';

            $id = $model->gravar($dados);

            $model->commit();

            $mensagem = 'Operação realizada com sucesso.';

            $this->_transport(MSG_SUCCESS, $mensagem, 'seguranca/resource');
        } catch (Exception $e) {
            $model->rollBack();

            if (isset($dados['rscid'])) {
                $this->_transport(MSG_ERROR, $e->getMessage(), 'seguranca/resource/cadastroUrl/rscid/' . Simec_Util::encode($dados['rscid']));
            } else {
                $this->_transport(MSG_ERROR, $e->getMessage(), 'seguranca/resource/cadastroUrl');
            }
        }
    }

    public function excluirAction()
    {
        $rscid = Simec_Util::decode($this->_getParam('rscid'));

        $model = new Model_Seguranca_Resource();

        try {
            $model->beginTransaction();

            $model->excluir(array('rscid = ?' => $rscid));

            $model->commit();

            $mensagem = 'Registro removido com sucesso.';

            $this->_transport(MSG_SUCCESS, $mensagem, 'seguranca/resource');
        } catch (Exception $e) {
            $model->rollBack();

            $this->_message(MSG_ERROR, $e->getMessage());
        }
    }

    public function getFileNames($path)
    {
//        $path = realpath(APPLICATION_PATH . '/modules/par/controllers');
//        $path = realpath(APPLICATION_PATH . '/' . $caminho);
        $diretorio = dir($path);

        $arquivos = array();
        while ($arquivo = $diretorio->read()) {
            if ($arquivo == '.' || $arquivo == '..') {
                continue;
            }
            $arquivos[] = substr(trim($arquivo), 0, -4);
        }
        $diretorio->close();
        return $arquivos;
    }

    public function cadastroUrlAction()
    {
        $model = new Model_Seguranca_Resource();
        $this->view->row = $model->getRow(Simec_Util::decode($this->_getParam('rscid')));
    }

    public function listarAclAction()
    {

        $module_dir = substr(str_replace("\\", "/", $this->getFrontController()->getModuleDirectory()), 0, strrpos(str_replace("\\", "/", $this->getFrontController()->getModuleDirectory()), '/'));
        $temp = array_diff(scandir($module_dir), Array(".", "..", ".svn"));
        $modules = array();
        $controller_directorys = array();
        foreach ($temp as $module) {
            if (is_dir($module_dir . "/" . $module)) {
                array_push($modules, $module);
                array_push($controller_directorys, str_replace("\\", "/", $this->getFrontController()->getControllerDirectory($module)));
            }
        }

        foreach ($controller_directorys as $dir) {
            foreach (scandir($dir) as $dirstructure) {
                if (is_file($dir . "/" . $dirstructure)) {
                    if (strstr($dirstructure, "Controller.php") != false) {
                        include_once($dir . "/" . $dirstructure);
                    }
                }

            }
        }

        $default_module = $this->getFrontController()->getDefaultModule();

        $db_structure = array();

        foreach (get_declared_classes() as $c) {
            if (is_subclass_of($c, 'Zend_Controller_Action')) {
                $functions = array();
                foreach (get_class_methods($c) as $f) {
                    if (strstr($f, "Action") != false) {
                        array_push($functions, substr($f, 0, strpos($f, "Action")));
                    }
                }
                $c = strtolower(substr($c, 0, strpos($c, "Controller")));

                if (strstr($c, "_") != false) {
                    $db_structure[substr($c, 0, strpos($c, "_"))][substr($c, strpos($c, "_") + 1)] = $functions;
                } else {
                    $db_structure[$default_module][$c] = $functions;
                }
            }
        }

        $model = new Model_Seguranca_Resource();
        $model_sistema = new Model_Seguranca_Sistema();


        foreach ($db_structure as $indice_modulo => $modulo) {

            $row = $model_sistema->fetchRow($model_sistema->select('sisid')->where("sisdiretorio like '" . $indice_modulo . "'"));

            foreach ($modulo as $controller => $actions) {
                if ($controller == '0') {
                    $controller = "index";
                };

                try {
                    foreach ($actions as $action) {

                        if (!empty($row->sisid)) {
                            $dados = array();
                            $dados['sisid'] = $row->sisid;
                            $dados['rscmodulo'] = $indice_modulo;
                            $dados['rscdsc'] = $indice_modulo.'::'.$controller.'::'.$action;
                            $dados['rsccontroller'] = $controller;
                            $dados['rscaction'] = $action;
                            $dados['rsctipo'] = 'P';
                            $model->beginTransaction();
                            $id = $model->gravar($dados);
                            $model->commit();
                        }
                    }
                } catch (Exception $e) {
                    $model->rollBack();
                    $this->_message(MSG_ERROR, $e->getMessage());
                }
            };
        }
    }

//$this->view->row = $model->getRow(Simec_Util::decode($this->_getParam('rscid')));

}