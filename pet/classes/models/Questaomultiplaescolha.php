<?php

class Model_Questaomultiplaescolha extends Abstract_Model
{

	protected $_schema = 'pet';
	protected $_name = 'questaomultiplaescolha';
	public $entity = array();

	public function __construct($commit = true)
	{
		$this->questionario = new Model_Questionario();
		$this->eixo = new Model_Eixo();

		parent::__construct($commit);
		$this->entity['qmeid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['ideixo'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['titulo'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '150', 'contraint' => '');
		$this->entity['descricao'] = array('value' => '', 'type' => 'text', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['descricao'] = array('value' => '', 'type' => 'text', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['numeroquestao'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['qmestatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
	}

	public function getSqlLista($ideixo)
	{
		$sql = "  SELECT qm.qmeid, qm.numeroquestao, qm.titulo
					, ( SELECT array_to_string(array_agg(texto), ', ') FROM pet.conceito WHERE qmeid = qm.qmeid) as conceitos
					FROM pet.questaomultiplaescolha  AS qm
					WHERE qm.qmestatus = 'A' AND qm.ideixo = {$ideixo}
                ";
		return $sql;
	}

	public function getLista()
	{
		$sql = $this->getSqlLista($this->getAttributeValue('ideixo'));

		$dados = $this->_db->carregar($sql);
		$dados = ($dados ? $dados : null);

		$listagem = new Listing(false);
		$listagem->setPerPage(30);
		$listagem->setActions(array('edit' => 'editar_multipla_escolha', 'delete' => 'apagar_multipla_escolha'));
		$listagem->setHead(array('Número do Item', 'Título', 'Conceitos'));
		$listagem->setEnablePagination(false);
		$listagem->listing($dados);

	}

	public function excluir($id)
	{
		$this->populateEntity(array('qmeid' => $id));

		$this->eixo->getEixo($this->getAttributeValue('ideixo'));
		$this->questionario->getQuestionario($this->eixo->getAttributeValue('queid'));

		if ($this->questionario->questionarioEmPreechimento()) {
			$this->setAttributeValue('qmestatus', 'I');
			$this->setDecode(false);
			$this->treatEntityToUser();
			return $this->save();
		} else {
			return false;
		}
	}

	public function salvar()
	{
		$conceito = new Model_Conceito();
		$this->populateEntity($_POST);

		$this->eixo->getEixo($this->getAttributeValue('ideixo'));
		$this->questionario->getQuestionario($this->eixo->getAttributeValue('queid'));

		if ($this->questionario->questionarioEmPreechimento()) {

			$this->setAttributeValue('titulo', trim($this->getAttributeValue('titulo')));
			$this->setAttributeValue('descricao', trim($this->getAttributeValue('descricao')));
			$this->setAttributeValue('qmestatus', 'A');

			$qmeid = $this->getAttributeValue('qmeid');
			if (empty($qmeid)) {
				$dados = $this->getAllByValues(array('qmestatus' => 'A', 'ideixo' => $this->getAttributeValue('ideixo')));
				$cont = count($dados);
				$this->setAttributeValue('numeroquestao', $cont + 1);
			}
			$qmeid = $this->save();

			if (!empty($qmeid)) {
				$conceito->deleteAllByValues(array('qmeid' => $qmeid));
				$conceitos = $_POST['conceito'];

				if (is_array($_POST['conceito'])) {
					foreach ($conceitos as $key => $value) {
						$conceito = new Model_Conceito();
						$conceito->setAttributeValue('ordem', $key + 1);
						$conceito->setAttributeValue('qmeid', $qmeid);
						$conceito->setAttributeValue('texto', trim($value));
						$conceito->save();
					}
				}

			}

			return $qmeid;
		} else {
			$this->error[] = array("msg" => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO));
			return false;
		}
	}

	public function getById($conid)
	{
		$conceito = new Model_Conceito();
		$dadosConceito = $conceito->getAllByValues(array('conid' => $conid));
		$qmeid = $dadosConceito[0]['qmeid'];
		$dados = $this->getAllByValues(array('qmeid' => $qmeid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function getQuestaoByIdEixo($ideixo)
	{
		$sql = "
			SELECT qm.*
				FROM pet.questaomultiplaescolha qm
			INNER JOIN pet.eixo ex ON ex.ideixo = qm.ideixo
			INNER JOIN pet.questionario qst ON qst.queid = ex.queid
			WHERE  qm.qmestatus = 'A'
				AND qst.dataabertura <= CURRENT_DATE
				AND qst.dataencerramento >  CURRENT_DATE
				AND ex.ideixo = $ideixo
			ORDER BY qm.qmestatus, qm.ideixo
		";
		$dados = $this->_db->carregar($sql);
		return ($dados ? $dados : array());
	}

	public function getByEixo($ideixo)
	{
		$dados = $this->getAllByValues(array('ideixo' => $ideixo));
		$questaomultiplaescolha = new Model_Questaomultiplaescolha();
		if ($dados) {
			$questaomultiplaescolha->populateEntity($dados[0]);
			$questaomultiplaescolha->treatEntityToUser();
		}
		return $questaomultiplaescolha;
	}

	public function getOpcoesEscolhidas($respostas, $conid, $qmeid)
	{
		$resp_ = array();
		if (is_array($respostas) and !empty($respostas)) {

			foreach ($respostas as $resp) {
				if($resp['conid'] == $conid ){
					$resp_[$conid] = array ( 'justificativa' =>$resp['justificativa'] , 'rmpid' =>$resp['rmpid'] );
				}
			}
		}

		return $resp_;
	}
}
