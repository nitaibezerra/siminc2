<?php
/*
 * Created on 11/09/2007 by MOC
 *
 */
class PosGraduacaoMatriculadosView extends View {
	private $erro;
	private $erros;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

	function listar(){
		$this->model->listar;
		$this->output.='<BR>';
		$this->output.= '<div id="listar">';

		$this->model->PegarCursoPos($this->model->curso, &$co_curso, &$co_unidade, &$co_capes, &$no_curso, &$tp_modalidade, &$dt_ano_inicio, &$co_area, &$co_conceito);
		$this->model->PegarUnidadeCursoPos($co_unidade, &$unidade, &$municipio, &$uf);
		$this->model->ListarPosGraduacaoMatriculados($this->model->curso);
		$l = $this->model->getPosGraduacaoMatriculados();
		if ($tp_modalidade=='D') $modalidade='Doutorado'; else $modalidade='Mestrado';
		$cont=1;
		foreach ($l as $matriculados) {
			if ($matriculados['nu_ano'] >= $dt_ano_inicio) {
				$cont+=1;
			}
		}
		$this->output.= '
		<form method="post" action="">
		<table class="lista" width="300" align=center>
		<tr>
			<td class="curso" colspan="10" align="center">'.$no_curso.' [ '.$modalidade.' ]</td>
		</tr>
		<tr>
			<td class="curso" colspan="10" align="center">'.$unidade.' [ '.$municipio.' / '.$uf.' ]</td>
		</tr>
		<tr>
		<th>Ano</th>';
		foreach ($l as $matriculados) {
			if ($matriculados['nu_ano'] >= $dt_ano_inicio) {
				$this->output.= '<th class="valores" width="1%">'.$matriculados['nu_ano'].'</th>';
			}
		}
		$this->output.= '
		</tr>
		<tr>
		<th>Matriculados</th>';
		foreach ($l as $matriculados) {
			if ($matriculados['nu_ano'] >= $dt_ano_inicio) {
				$this->output.= '<td class="valores" width="1%"><input class="valores" autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###\',this.value);" type="text" size=5  name="Matriculados['.$matriculados['nu_ano'].']" value='.number_format($matriculados['nu_matriculados'],0,',','.').'></td>';
			}
		}
		$this->output.= '
		</tr>
		<tr>
		<th class="barrabotoes" colspan="'.$cont.'" align="center">
		<input type="submit" class="formbutton" name="Salvar" value="Salvar"/>
		<input type="submit" class="formbutton" name="Cancelar" value="Retornar"/>
		</th>
		</tr>
		</table>
		</form>';
		$this->output.= '</div>';
		$this->output.='<BR>';
	}

    function display() {
    	$this->menu='pos_graduacao';
        $this->listar();
        parent::display();
    }

}

?>
