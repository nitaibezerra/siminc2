<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class GraduacaoView extends View {
	private $erro;
	private $erros;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

	function comboUnidades($disabled, $un=null) {
		$this->model->ListarUnidades($_SESSION["USUARIO_INSTITUICAO"]);
		$c = $this->model->getUnidades();

		if($disabled)
			$dis = "disabled";
		else
			$dis = null;

		$res = '
    	<select class="normal" name="unidade" '.$dis.'>
        <option value="--">--</option>';

		foreach($c as $curso) {
			if($un == $curso['co_unidade'])
				$res.= '<option value="'.$curso['co_unidade'].'" selected>'.$curso['no_unidade'].'</option>';
			else
				$res.= '<option value="'.$curso['co_unidade'].'">'.$curso['no_unidade'].'</option>';
		}

		$res.= '</select>';
		return $res;
	}

	function comboTurnos($disabled, $tur=null) {
		if($disabled)
			$dis = "disabled";
		else
			$dis = null;

		$res = '
    	<select class="normal" name="turno" '.$dis.'>
        <option value="--">--</option>';

		if($tur == "D")
			$res.= '<option value="D" selected>Diurno</option>';
		else
			$res.= '<option value="D">Diurno</option>';

		if($tur == "N")
			$res.= '<option value="N" selected>Noturno</option>';
		else
			$res.= '<option value="N">Noturno</option>';

		$res.= '</select>';
		return $res;
	}

	function comboAreas($disabled, $ar=null) {
		$this->model->ListarAreas();
		$a = $this->model->getAreas();

		if($disabled)
			$dis = "disabled";
		else
			$dis = null;

		$res = '
    	<select class="normal" name="area" '.$dis.'>
        <option value="--">--</option>';

		foreach($a as $area) {
			if($ar == $area['co_area'])
				$res.= '<option value="'.$area['co_area'].'" selected>'.$area['ds_area'].'</option>';
			else
				$res.= '<option value="'.$area['co_area'].'">'.$area['ds_area'].'</option>';
		}

		$res.= '</select>';
		return $res;
	}

	function alterar() {
		$codCurso = addslashes($_GET['curso']);
		$act = addslashes($_GET['action']);

		$this->erros = false;

		if($act == 'alterar') {
			$this->model->PegarCursoGrad($codCurso, &$co_curso, &$co_unidade, &$co_inep, &$no_curso, &$co_turno, &$dt_ano_inicio, &$co_area, &$vl_duracao);
		}

		if ($_POST['Salvar']) {
			$co_unidade    = addslashes($_POST['unidade']);
			$no_curso      = addslashes($_POST['nome']);
			$co_turno      = addslashes($_POST['turno']);
			$dt_ano_inicio = addslashes($_POST['inicio']);
			$co_area       = addslashes($_POST['area']);
			$vl_duracao    = addslashes($_POST['duracao']);

  			$valor = str_replace(".", "", $vl_duracao);
  			$vl_duracao = str_replace(",", ".",$valor);

			if($co_unidade    === "--") $this->erros[]="Você deve informar uma unidade para o curso.";
			if($no_curso      === "")   $this->erros[]="Você deve informar um nome para o curso.";
			if($co_turno      === "--") $this->erros[]="Você deve informar um turno para o curso.";
			if(((int)$dt_ano_inicio < 1800)||((int)$dt_ano_inicio > 2012)) $this->erros[]="Somente cursos com ano de início entre 1800 e 2012.";
			if($co_area       === "--") $this->erros[]="Você deve informar uma área de conhecimento para o curso.";
			if(((double)$vl_duracao < 3)||((double)$vl_duracao > 6)) $this->erros[]="Somente cursos com duração entre 3 e 6 anos";
			if($vl_duracao    === "")   $this->erros[]="Você deve informar uma duração para o curso.";

			if (!$this->erros) {
				switch($act) {
					case 'alterar': $s = $this->model->AlterarCursoGrad($codCurso, $co_unidade, $no_curso, $co_turno, $dt_ano_inicio, $co_area, $vl_duracao); break;
					case 'incluir': $s = $this->model->InserirCursoGrad($co_unidade, $no_curso, $co_turno, $dt_ano_inicio, $co_area, $vl_duracao); break;
				}
				if ($s===TRUE) {
					header( 'Location: index.php?view=graduacao');
				} else {
					$this->erros[]="Erro ao inserir ou alterar curso no banco de dados.";
				}
			}
		}
		if ($_POST['Cancelar']) {
				header( 'Location: index.php?view=graduacao') ;
		}

		$this->output.= '
		<br>
		<form method="post" action="">
		<table class="formulario" width="%1" align="center">
		<tr>
			<th colspan="2">'.ucfirst ($act).' Curso de Graduação</th>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Unidade:</th>
			<td>'.$this->comboUnidades(FALSE, $co_unidade).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Nome do Curso:</th>
			<td><input class="normal" type="text" size=80 name="nome" value="'.$no_curso.'"></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Turno:</th>
			<td>'.$this->comboTurnos(FALSE, $co_turno).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Ano de Início:</th>
			<td><input class="normal" STYLE="text-align:right" type="text" size=5 name="inicio"  onkeyup="this.value = mascaraglobal(\'####\',this.value);" value="'.$dt_ano_inicio.'"></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Área:</th>
			<td>'.$this->comboAreas(FALSE, $co_area).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Duração:</th>
			<td><input class="normal" STYLE="text-align:right" "type="text" size=3 name="duracao" onkeyup="this.value = mascaraglobal(\'#,#\',this.value);" value="'.number_format($vl_duracao,1,',','.').'"> anos</td>
		</tr>
		<tr>
			<th  class="barrabotoes" colspan="5">
			<input type="submit" class="formbutton" name="Salvar" value="Salvar"/>
			<input type="submit" class="formbutton" name="Cancelar" value="Cancelar"/>
			</th>
		</tr>
		<tr><th class="esp">&nbsp;</th></tr>
		';
		if ($this->erros) {
			foreach ($this->erros as $er) {
				$this->output.='<tr><td colspan="2" class="erroinfo">'.$er.'</td></tr>';
			};
		}
		$this->output.= '</table></form><br>';
	}

	function excluir() {
		$codCurso = addslashes($_GET['curso']);

		if ($_POST['Sim']) {
			$s = $this->model->RemoverCursoGrad($codCurso);
			if ($s===TRUE) {
				header( 'Location: index.php?view=graduacao');
			} else {
				$this->erros[]="Erro ao excluir Curso do banco de dados.";
			}
		}
		elseif ($_POST['Nao']) {
			header( 'Location: index.php?view=graduacao') ;
		}

		$this->model->PegarCursoGrad($codCurso, &$co_curso, &$co_unidade, &$co_inep, &$no_curso, &$co_turno, &$dt_ano_inicio, &$co_area, &$vl_duracao);

		$this->output.= '
		<br>
		<form method="post" action="">
		<table class="formulario" width="%1" align="center">
		<tr>
			<th colspan="2">Excluir Curso de Graduação</th>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Unidade: </th>
			<td>'.$this->comboUnidades(TRUE, $co_unidade).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Nome do Curso: </th>
			<td><input class="normal" type="text" size=80 name="nome" value="'.$no_curso.'" disabled></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Turno: </th>
			<td>'.$this->comboTurnos(TRUE, $co_turno).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Ano de Início: </th>
			<td><input class="normal" type="text" size=10 name="inicio" value="'.$dt_ano_inicio.'" disabled></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Área: </th>
			<td>'.$this->comboAreas(TRUE, $co_area).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Duração: </th>
			<td><input class="normal" type="text" size=10 name="duracao" value="'.number_format($vl_duracao,1,',','.').'" disabled></td>
		</tr>
		<tr>
			<th class="barrabotoes" colspan="5" align="center">
			<input type="submit" class="formbutton" name="Sim" value="Sim"/>
			<input type="submit" class="formbutton" name="Nao" value="Não"/>
			</th>
		</tr>
		<tr><th class="esp">&nbsp;</th></tr>
		';
		if ($this->erros) {
			foreach ($this->erros as $er) {
				$this->output.='<tr><td colspan="2" class="erroinfo">'.$er.'</td></tr>';
			};
		}
		$this->output.= '</table></form><br>';

	}

    function display() {
    	$this->menu='graduacao';
		switch ($this->model->action) {
			case 'incluir' : $this->alterar(); break;
			case 'alterar' : $this->alterar(); break;
			case 'excluir' : $this->excluir(); break;
			default : $this->listar();
		}
        parent::display();
    }

}

?>
