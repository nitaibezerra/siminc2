<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class GraduacaoListaView extends View {

    public function __construct($model) {
    	parent::__construct($model);
    }

	public function display() {
		$this->menu='graduacao';
		$this->model->ListarCursosGrad();
		$rows = $this->model->getCursosGrad();
		$headers = array(
			'Unidade'=>'no_unidade',
			'INEP'=>'co_inep',
			'Curso'=>'no_curso',
			'Turno'=>'co_turno',
			'Início'=>'dt_ano_inicio',
			'Área'=>'co_area',
			'Duração'=>'vl_duracao',
		);
		$W = new ListaWidget('graduacao','lista',$headers,'curso','co_curso',$rows);
		$this->output.= $W->display();
		parent::display();
	}

}

?>
