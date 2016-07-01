<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PosGraduacaoListaView extends View {

    public function __construct($model) {
    	parent::__construct($model);
    }

	public function display() {
		$this->menu='pos_graduacao';
		$this->model->ListarCursosPos();
		$rows = $this->model->getCursosPos();
		$headers = array(
			'Unidade'=>'no_unidade',
			'Modalidade'=>'tp_modalidade',
			'CAPES'=>'co_capes',
			'Curso'=>'no_curso',
			'Conceito'=>'co_conceito',
			'Início'=>'dt_ano_inicio',
			'Área'=>'co_area',
		);
		$W = new ListaWidget('pos_graduacao','lista',$headers,'curso','co_curso',$rows);
		$this->output.= $W->display();
		parent::display();
	}

}

?>
