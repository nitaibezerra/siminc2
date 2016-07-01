<?php
/*
 * Created on 11/09/2007 by MOC
 *
 */
class GraduacaoVagasView extends View {
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

		$this->model->PegarCursoGrad($this->model->curso, &$co_curso, &$co_unidade, &$co_inep, &$no_curso, &$co_turno, &$dt_ano_inicio, &$co_area, &$vl_duracao);
		$this->model->PegarUnidadeCursoGrad($co_unidade, &$unidade, &$municipio, &$uf);
		$this->model->ListarGraduacaoVagas($this->model->curso);
		$l = $this->model->getGraduacaoVagas();
		if ($co_turno=='D') $turno='Diurno'; else $turno='Noturno';
		$cont=1;
		foreach ($l as $vagas) {
			if ($vagas['nu_ano'] >= $dt_ano_inicio) {
				$cont+=1;
			}
		}
		$this->output.= '
		<form method="post" action="">
		<table class="lista" width="300" align=center>
		<tr>
			<td class="curso" colspan="'.$cont.'" align="center"><strong>'.$no_curso.' [ '.$turno.' ]</strong></td>
		</tr>
		<tr>
			<td class="curso" colspan="'.$cont.'" align="center">'.$unidade.' [ '.$municipio.' / '.$uf.' ]</td>
		</tr>
		<tr>
		<th>Ano</th>';
		foreach ($l as $vagas) {
			if ($vagas['nu_ano'] >= $dt_ano_inicio) {
				$this->output.= '<th class="valores" width="1%">'.$vagas['nu_ano'].'</th>';
			}
		}
		$this->output.= '
		</tr>
		<tr>
		<th>Vagas</th>';
		foreach ($l as $vagas) {
			if ($vagas['nu_ano'] >= $dt_ano_inicio) {
				$this->output.= '<td class="valores" width="1%"><input class="valores" autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###\',this.value);" type="text" size=5  name="Vagas['.$vagas['nu_ano'].']" value='.number_format($vagas['nu_vagas'],0,',','.').'></td>';
			}
		}
		$this->output.= '
		</tr>
		<tr>
		<th>Concluintes</th>';
		foreach ($l as $vagas) {
			if ($vagas['nu_ano'] >= $dt_ano_inicio) {
				$this->output.= '<td class="valores" width="1%"><input class="valores" autocomplete="off" onkeyup="this.value = mascaraglobal(\'[###.]###\',this.value);" type="text" size=5  name="Concluintes['.$vagas['nu_ano'].']" value='.number_format($vagas['nu_concluintes'],0,',','.').'></td>';
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
    	$this->menu='graduacao';
        $this->listar();
        parent::display();
    }

}

?>
