<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class UnidadesView extends View {
	private $erro;
	private $erros;

    function __construct($model,$erro=null) {
    	parent::__construct($model);
    	$this->erro = $erro;
    }

	function comboEstados($disabled, $uf=null,$municipio=null) {
		$this->model->ListarEstados();
		$e = $this->model->getEstados();
		if($disabled)
			$dis = "disabled";
		else
			$dis = null;

		$res = '
    	<select name="uf" id="uf" class="normal" onchange="doBusca(this.options[this.selectedIndex].value);" '.$dis.'>
        <option value="--">--</option>';

		foreach($e as $estado) {
			if($uf == $estado['sg_estado'])
				$res.= '<option value="'.$estado['sg_estado'].'" selected>'.$estado['sg_estado'].'</option>';
			else
				$res.= '<option value="'.$estado['sg_estado'].'">'.$estado['sg_estado'].'</option>';
		}

		$res.= '</select>';
		return $res;
	}

	function comboMunicipios($disabled, $uf=null,$municipio=null) {
		$this->model->ListarMunicipios($uf);
		$e = $this->model->getMunicipios();

		if($disabled)
			$dis = "disabled";
		else
			$dis = null;

		$res = '
    	<select name="municipio" id="municipio" class="normal"'.$dis.'>
        <option value="--">--</option>';
		if ($e) {
		foreach($e as $mun) {
			if($municipio == $mun['co_municipio'])
				$res.= '<option value="'.$mun['co_municipio'].'" selected>'.$mun['no_municipio'].'</option>';
			else
				$res.= '<option value="'.$mun['co_municipio'].'">'.$mun['no_municipio'].'</option>';
		}
		}
		$res.= '</select>';
		return $res;
	}

	function incluir() {
		$this->erros = false;
		if (isset($_POST['Salvar'])) {
			$nome=addslashes($_POST['nome']);
			$uf=addslashes($_POST['uf']);
			$municipio=addslashes($_POST['municipio']);
			if($nome=="") $this->erros[]="Você deve informar um nome para a unidade.";
			if(($uf=="--")||($uf=="")) $this->erros[]="Você deve informar um estado da federação para a unidade.";
			if(($municipio=="Selecione...")||($municipio=="")||($municipio=="--")) $this->erros[]="Você deve informar um município para a unidade.";
			if (!$this->erros) {
				$s = $this->model->InserirUnidade($nome,$municipio);
				if ($s===TRUE) {
					header( 'Location: index.php?view=unidades') ;
				} else {
					$this->erros[]="Erro ao incluir unidade, contacte o administrador do sistema.";
				}
			}
		};
		if (isset($_POST['Cancelar'])) {
				header( 'Location: index.php?view=unidades') ;
		}
		$this->output.= '
		<br>
		<form method="post" action="">
		<table class="formulario" width="1%" nowrap align=center>
		<tr>
		<th colspan="2">Incluir Unidade Acadêmica</th>
		</tr>
		<tr>
		<th class="valoresitens" nowrap>Nome:</th>
		<td><input class="normal" type="text" size=80 name="nome" value="'.$nome.'" autocomplete="off"></td>
		</tr>
		<tr>
		<th class="valoresitens" nowrap>UF:</th>
		<td>'.$this->comboEstados(FALSE,$uf,$municipio).'
		</td>
		<tr>
		<th class="valoresitens" nowrap>Município:</th>
		<td>'.$this->comboMunicipios(FALSE,$uf,$municipio).'
    	</td>
		</tr>
		<tr>
		<th  class="barrabotoes" colspan="2">
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

	function alterar() {
		$codunidade=$this->model->unidade;
		$this->erros = false;
		$this->model->PegarUnidade($codunidade,&$nome,&$uf,&$municipio,&$nomemunicipio);
		if ($_POST['Salvar']) {
			$nome = addslashes($_POST["nome"]);
			$uf = addslashes($_POST["uf"]);
			$municipio = addslashes($_POST["municipio"]);
			if($nome=="") $this->erros[]="Você deve informar um nome para a unidade.";
			if($uf=="--") $this->erros[]="Você deve informar um estado da federação para a unidade.";
			if(($municipio=="Selecione...")||($municipio=="")||($municipio=="--")) $this->erros[]="Você deve informar um município para a unidade.";
			if (!$this->erros) {
				$s = $this->model->AlterarUnidade($codunidade,$nome,$municipio);
				if ($s===TRUE) {
					header( 'Location: index.php?view=unidades') ;
				} else {
					$this->erros[]="Erro ao alterar unidade, contacte o administrador do sistema.";
				}
			}
		};
		if ($_POST['Cancelar']) {
			header( 'Location: index.php?view=unidades') ;
		}
		$this->output.= '
		<br>
		<form method="post" action="">
		<table class="formulario" width="1%" align="center">
		<tr>
		<th colspan="2">Alterar Unidade Acadêmica</th>
		</tr>
		<th class="valoresitens" nowrap>Nome:</th>
		<td><input class="normal" type="text" size=80 name="nome" value="'.$nome.'"></td>
		<tr>
		</tr>
		<th class="valoresitens" nowrap>UF:</th>
		<td>'.$this->comboEstados(FALSE,$uf,$municipio).'
    	</td>
		<tr>
		</tr>
		<th class="valoresitens" nowrap>Município:</th>
		<td>'.$this->comboMunicipios(FALSE,$uf,$municipio).'
    	</td>
		<tr>
		</tr>
		<tr>
		<th  class="barrabotoes" colspan="2">
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
		$codunidade=$this->model->unidade;
		$this->model->PegarUnidade($codunidade,&$unidade,&$uf,&$municipio,&$nomemunicipio);
		if ($_POST['Sim']) {
				$s = $this->model->RemoverUnidade($codunidade);
				if ($s===TRUE) {
					header( 'Location: index.php?view=unidades') ;
				} else {
					if (stripos($s,'fk_graduacao_unidade')===FALSE)
						$this->erros[]="Erro ao excluir unidade, contacte o administrador do sistema.".$s;
					else
						$this->erros[]="Erro ao excluir unidade, você não pode excluir uma unidade que possui cursos associados.";
				}
		}
		else if ($_POST['Nao']) {
			header( 'Location: index.php?view=unidades') ;
		}
		$this->output.= '
		<br>
		<form method="post" action="">
		<table class="formulario" width="1%" align="center">
		<tr>
		<th colspan="2">Excluir Unidade Acadêmica</th>
		</tr>
		<th class="valoresitens" nowrap>Nome:</th>
		<td><input class="normal" type="text" size=80 name="Unidade" value="'.$unidade.'" disabled></td>
		<tr>
		</tr>
		<th class="valoresitens" nowrap>UF:</th>
		<td>'.$this->comboEstados(TRUE,$uf,$municipio).'
    	</td>
		<tr>
		</tr>
		<th class="valoresitens" nowrap>Município:</th>
		<td>'.$this->comboMunicipios(TRUE,$uf,$municipio).'
    	</td>
		<tr>
		<th  class="barrabotoes" colspan="2">
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
		$this->menu='unidades';
		switch ($this->model->action) {
			case 'incluir' : $this->incluir(); break;
			case 'alterar' : $this->alterar(); break;
			case 'excluir' : $this->excluir(); break;
			default : $this->listar();
		}
		parent::display();
	}
}
?>
