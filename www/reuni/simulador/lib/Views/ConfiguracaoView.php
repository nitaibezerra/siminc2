<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class ConfiguracaoView extends View {
	private $erro;
	private $erros;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

    function display() {
        $this->menu('configuracao');
        parent::display();
    }

}

?>
