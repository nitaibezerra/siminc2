<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class ConfiguracaoController extends Controller {

    public function __construct($dao) {
    	parent::__construct(new ConfiguracaoModel($dao));
   		$this->view = new ConfiguracaoView($this->model);
    }

}
?>
