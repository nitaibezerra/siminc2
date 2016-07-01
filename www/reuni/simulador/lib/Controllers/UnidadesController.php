<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class UnidadesController extends Controller {

    public function __construct($dao) {
    	parent::__construct(new UnidadesModel($dao));
    	if (isset($_GET["action"])) $this->model->action = addslashes($_GET["action"]);
    	if (isset($_GET["unidade"])) $this->model->unidade = addslashes($_GET["unidade"]);
   		$actions = array('incluir','alterar','excluir');
   		if (in_array($this->model->action,$actions)===TRUE) {
			$this->view = new UnidadesView($this->model);
   		} else {
			$this->view = new UnidadesListaView($this->model);
		}
    }

}
?>
