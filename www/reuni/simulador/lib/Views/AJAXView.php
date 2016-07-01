<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class AJAXView {
    protected $model;
    protected $output;

    public function __construct($model) {
        $this->model = $model;
    }

	protected function display() {
		if (substr_count($this->model->accept_encoding,'gzip'))
			ob_start("ob_gzhandler");
		else
			ob_start();
		header("Pragma: no-cache");
		echo $this->output;
		ob_end_flush();
	}

}

?>