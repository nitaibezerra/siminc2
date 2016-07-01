<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PlanilhasController extends Controller {
	private $matriz;

    public function __construct($dao) {
    	parent::__construct(new PlanilhasModel($dao));
   		$this->view = new PlanilhasView($this->model);
    }

	public function getMatriz() {
		$this->montaMatriz();
		return $this->matriz;
	}
}
?>
