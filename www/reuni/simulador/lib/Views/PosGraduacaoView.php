<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PosGraduacaoView extends View {
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
    	<select class="normal" name="Unidade" '.$dis.'>
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

	function comboModalidades($disabled, $mod=null) {
		if($disabled)
			$dis = "disabled";
		else
			$dis = null;

		$res = '
    	<select class="normal" name="Modalidade" '.$dis.' onChange="alteraConceito(this.options[this.selectedIndex].value);">
        <option value="--">--</option>';

		if($mod == "D")
			$res.= '<option value="D" selected>Doutorado</option>';
		else
			$res.= '<option value="D">Doutorado</option>';

		if($mod == "M")
			$res.= '<option value="M" selected>Mestrado</option>';
		else
			$res.= '<option value="M">Mestrado</option>';

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
    	<select class="normal" name="Area" '.$dis.'>
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
		$modcombo = false;
		if($act == 'alterar') {
			$modcombo = true;
			$this->model->PegarCursoPos($codCurso, &$co_curso, &$co_unidade, &$co_capes, &$no_curso, &$tp_modalidade, &$dt_ano_inicio, &$co_area, &$co_conceito);
		}

		$this->erros = false;
		if ($_POST['Salvar']) {
			$co_unidade    = addslashes($_POST['Unidade']);
			$no_curso      = addslashes($_POST['NomeCurso']);
			$tp_modalidade = addslashes($_POST['Modalidade']);
			$dt_ano_inicio = addslashes($_POST['Inicio']);
			$co_area       = addslashes($_POST['Area']);
			if($act == 'incluir') {
				if ($tp_modalidade === "--") $this->erros[]="Você deve informar uma modalidade para o curso.";
				if ($tp_modalidade == "M") $co_conceito = 3; else $co_conceito = 4;
				if ($tp_modalidade == "--") $co_conceito = "";
			}

			if($co_unidade    === "--") $this->erros[]="Você deve informar uma unidade para o curso.";
			if($no_curso      === "")   $this->erros[]="Você deve informar um nome para o curso.";
			if(((int)$dt_ano_inicio < 1800)||((int)$dt_ano_inicio > 2012)) $this->erros[]="Somente cursos com ano de início entre 1800 e 2012.";
			if($co_area       === "--") $this->erros[]="Você deve informar uma área de conhecimento para o curso.";

			if (!$this->erros) {
				switch($act) {
					case 'alterar': $s = $this->model->AlterarCursoPos($codCurso, $co_unidade, $no_curso, $dt_ano_inicio, $co_area); break;
					case 'incluir': $s = $this->model->InserirCursoPos($co_unidade, $no_curso, $tp_modalidade, $dt_ano_inicio, $co_area); break;
				}
				if ($s===TRUE) {
					header( 'Location: index.php?view=pos_graduacao');
				} else {
					$this->erros[]="Erro ao inserir ou alterar Curso do banco de dados.";
				}
			}
		}
		elseif ($_POST['Cancelar']) {
				header( 'Location: index.php?view=pos_graduacao') ;
		}
		$this->output.= '
		<br>
		<form method="post" action="">
		<table class="formulario" width="600" align="center">
		<tr>
			<th colspan="2">'.ucfirst ($act).' Curso de Pós-Graduação</th>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Unidade:</th>
			<td>'.$this->comboUnidades(FALSE, $co_unidade).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Nome do Curso:</th>
			<td><input class="normal" type="text" size=80 name="NomeCurso" value="'.$no_curso.'"></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Modalidade:</th>
			<td>'.$this->comboModalidades($modcombo, $tp_modalidade).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Ano de Início:</th>
			<td><input class="normal" STYLE="text-align:right" type="text" size=5 name="Inicio"  onkeyup="this.value = mascaraglobal(\'####\',this.value);" value="'.$dt_ano_inicio.'"></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Área:</td>
			<td>'.$this->comboAreas(FALSE, $co_area).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Conceito:</th>
			<td><input class="normal" type="text" size=10 name="Conceito" id="Conceito" value="'.$co_conceito.'" disabled></td>
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
			$s = $this->model->RemoverCursoPos($codCurso);
			if ($s===TRUE) {
				header( 'Location: index.php?view=pos_graduacao');
			} else {
				$this->erros[]="Erro ao excluir Curso do banco de dados.";
			}
		}
		elseif ($_POST['Nao']) {
			header( 'Location: index.php?view=pos_graduacao') ;
		}
		$this->model->PegarCursoPos($codCurso, &$co_curso, &$co_unidade, &$co_capes, &$no_curso, &$tp_modalidade, &$dt_ano_inicio, &$co_area, &$co_conceito);

		$this->output.= '
		<br>
		<form method="post" action="">
		<table class="formulario" width="600" align="center">
		<tr>
			<th colspan="2">Excluir Curso de Pós-Graduação</th>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Unidade:</th>
			<td>'.$this->comboUnidades(TRUE, $co_unidade).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Nome do Curso:</th>
			<td><input class="normal" type="text" size=80 name="NomeCurso" value="'.$no_curso.'" disabled></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Modalidade:</th>
			<td>'.$this->comboModalidades(TRUE, $tp_modalidade).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Ano de Início:</th>
			<td><input class="normal" type="text" size=10 name="Inicio" value="'.$dt_ano_inicio.'" disabled></td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Área:</th>
			<td>'.$this->comboAreas(TRUE, $co_area).'</td>
		</tr>
		<tr>
			<th class="valoresitens" nowrap>Conceito:</th>
			<td><input class="normal" type="text" size=10 name="Conceito" id="Conceito" value="'.$co_conceito.'" disabled></td>
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
    	$this->menu='pos_graduacao';
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
