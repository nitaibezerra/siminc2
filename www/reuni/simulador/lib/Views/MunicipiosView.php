<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class MunicipiosView extends AJAXView {

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    }

    function display() {
    	if ($this->model->getMunicipios()) {
    		$this->output = "{";
    		foreach ($this->model->getMunicipios() as $municipio) {
    			if ($this->output<>"{")
    				$this->output .=',';
    			$this->output .= '"'.$municipio['municipio'].'":"'.$municipio['codigo'].'"';
    		}
    		$this->output .= "}";
        	parent::display();
    	}
    }

}

?>
