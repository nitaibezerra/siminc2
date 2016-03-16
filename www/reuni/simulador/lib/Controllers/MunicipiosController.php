<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class MunicipiosController extends Controller {

    public function __construct($dao) {
    	parent::__construct(new MunicipiosModel($dao),FALSE);
    	if (isset($_GET["uf"])) {
    		$this->model->ListarMunicipios(addslashes($_GET["uf"]));
    	} else {
    		$this->model->ListarMunicipios();
    	}
   		$this->view = new MunicipiosView($this->model);
    }

}
?>
