<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class PosGraduacaoController extends Controller {

    public function __construct($dao) {
   		parent::__construct(new PosGraduacaoModel($dao));
    	if (isset($_GET["action"])) $this->model->action = addslashes($_GET["action"]);
    	if (isset($_GET["curso"])) $this->model->curso = addslashes($_GET["curso"]);
   		$actions = array('incluir','alterar','excluir','vagas');
   		if (in_array($this->model->action,$actions)===TRUE) {
			if ($this->model->action == 'vagas') {
   				if ($_POST["Salvar"]) {
					while($a = each($_POST["Matriculados"]))
  					{
  						$valor = str_replace(".", "", $a['value']);
	  					$valor = str_replace(",", ".",$valor);
						if (empty($valor)) $valor=0;
  						$this->model->AtualizarPosGraduacaoMatriculados($this->model->curso,$a['key'],$valor);
   					}
   					$this->view = new PosGraduacaoMatriculadosView($this->model);
   				} elseif ($_POST["Cancelar"]) {
   					$this->view = new PosGraduacaoListaView($this->model);
   				} else {
   					$this->view = new PosGraduacaoMatriculadosView($this->model);
   				}
   			} else {
				$this->view = new PosGraduacaoView($this->model);
   			}

   		} else {
			$this->view = new PosGraduacaoListaView($this->model);
		}
    }

}
?>
