<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class UnidadesListaView extends View {

    public function __construct($model) {
    	parent::__construct($model);
    }

	public function display() {
		$this->menu='unidades';
		$this->model->ListarUnidades();
		$rows = $this->model->getUnidades();
		$headers = array(
			'Unidade Acadêmica'=>'no_unidade',
			'Município'=>'no_municipio',
			'UF'=>'sg_estado'
		);
		$W = new ListaWidget('unidades','lista',$headers,'unidade','co_unidade',$rows);
		$this->output.= $W->display();
		parent::display();
	}

}

?>
