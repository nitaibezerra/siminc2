<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class GraduacaoController extends Controller {

    public function __construct($dao) {
   		parent::__construct(new GraduacaoModel($dao));
    	if (isset($_GET["action"])) $this->model->action = addslashes($_GET["action"]);
    	if (isset($_GET["curso"])) $this->model->curso = addslashes($_GET["curso"]);
   		$actions = array('incluir','alterar','excluir','vagas','vagastcg');
   		if (in_array($this->model->action,$actions)===TRUE) {
   			if ($this->model->action == 'vagastcg') {
   				if ($_POST["Salvar"]) {
					while($a = each($_POST["Vagas"]))
  					{
  						$valor = str_replace(".", "", $a['value']);
	  					$valor = str_replace(",", ".",$valor);
						if (empty($valor))  $valor=0;
  						$this->model->AtualizarGraduacaoVagasTCG($this->model->instituicao,$a['key'],$valor);
  					}
   					$this->view = new GraduacaoVagasTCGView($this->model);
   				} elseif ($_POST["Cancelar"]) {
   					$this->view = new GraduacaoListaView($this->model);
   				} else {
   					$this->view = new GraduacaoVagasTCGView($this->model);
   				}
   			}
   			elseif ($this->model->action == 'vagas') {
   				if ($_POST["Salvar"]) {
					while($a = each($_POST["Vagas"]))
  					{
  						$valor = str_replace(".", "", $a['value']);
	  					$valor = str_replace(",", ".",$valor);
						if (empty($valor))  $valor=0;
  						$this->model->AtualizarGraduacaoVagas($this->model->curso,$a['key'],$valor);
  					}
					while($a = each($_POST["Concluintes"]))
  					{
  						$valor = str_replace(".", "", $a['value']);
	  					$valor = str_replace(",", ".",$valor);
						if (empty($valor))  $valor=0;
  						$this->model->AtualizarGraduacaoConcluintes($this->model->curso,$a['key'],$valor);
   					}
   					$this->view = new GraduacaoVagasView($this->model);
   				} elseif ($_POST["Cancelar"]) {
   					$this->view = new GraduacaoListaView($this->model);
   				} else {
   					$this->view = new GraduacaoVagasView($this->model);
   				}
   			} else {
				$this->view = new GraduacaoView($this->model);
   			}
   		} else {
			$this->view = new GraduacaoListaView($this->model);
		}
    }

}
?>
