<?php

class Model_Questaobinaria extends Abstract_Model
{
	protected $_schema = 'pet';
	protected $_name = 'questaobinaria';
	public $entity = array();

	public function __construct($commit = true)
	{
		$this->categoria = new Model_Categoriaquestoesbinaria();
		$this->questionario = new Model_Questionario();
		$this->eixo = new Model_Eixo();

		parent::__construct($commit);
		$this->entity['qubid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['cqbid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['titulo'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '150', 'contraint' => '');
		$this->entity['numeroquestao'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
		$this->entity['qubstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
	}

	public function getLista($cqbid)
	{
		$dados = $this->getAllByValues(array('qubstatus' => 'A', 'cqbid' => $cqbid), array('numeroquestao'));
		$dados = $this->tratarDados($dados);
		$dados = ($dados ? $dados : null);

		$listagem = new Listing(false);
		$listagem->setPerPage(30);
		$listagem->setActions(array('edit' => 'editar_binaria', 'delete' => 'apagar_binaria'));
		$listagem->setHead(array('Número da Questão', 'Título'));
		$listagem->setEnablePagination(false);
		$listagem->listing($dados);
	}

	public function tratarDados($dados)
	{
		if (is_array($dados)) {
			$data = array();
			foreach ($dados as $key => $questao) {
				$data[$key]['qubid'] = $questao['qubid'];
				$data[$key]['numeroquestao'] = $questao['numeroquestao'];
				$data[$key]['titulo'] = $questao['titulo'];
			}
			return $data;
		} else {
			return array();
		}

	}

	public function excluir($id)
	{

		$this->populateEntity(array('qubid' => $id));

		$this->categoria->getById($this->getAttributeValue('cqbid'));
		$this->eixo->getEixo($this->categoria->getAttributeValue('ideixo'));
		$this->questionario->getQuestionario($this->eixo->getAttributeValue('queid'));
		$asdf = $this->questionario->questionarioEmPreechimento();

		if ($this->questionario->questionarioEmPreechimento()) {
			$this->setAttributeValue('qubstatus', 'I');
			$this->setDecode(false);
			$this->treatEntityToUser();
			return $this->save();
		} else {
			return false;
		}
	}

	public function salvar()
	{
		$this->populateEntity($_POST);

		$this->categoria->getById($this->getAttributeValue('cqbid'));
		$this->eixo->getEixo($this->categoria->getAttributeValue('ideixo'));
		$this->questionario->getQuestionario($this->eixo->getAttributeValue('queid'));

		if ($this->questionario->questionarioEmPreechimento()) {
			$this->setAttributeValue('titulo', trim($this->getAttributeValue('titulo')));
			$this->setAttributeValue('qubstatus', 'A');

			$qubid = $this->getAttributeValue('qubid');
			if (empty($qubid)) {
				$dados = $this->getAllByValues(array('qubstatus' => 'A', 'cqbid' => $this->getAttributeValue('cqbid')));
				$cont = count($dados);
				$this->setAttributeValue('numeroquestao', $cont + 1);
			}
			return $this->save();
		} else {
			$this->error[] = array("msg" => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO));
			return false;
		}
	}

	public function getById($qubid)
	{
		$dados = $this->getAllByValues(array('qubid' => $qubid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function getQuestoes(array $criterio = array())
	{
		$where = '';
		if (!empty($criterio)) {
			$where = 'AND ' . implode(' ', $criterio);
		}
		$sql = "
			SELECT qb.qubid, cqb.nome, qb.titulo,
				(SELECT COUNT(*) FROM pet.questaobinaria WHERE cqbid = cqb.cqbid AND qubstatus = 'A' ) AS totalporcategoria
				FROM pet.questaobinaria qb
			INNER JOIN pet.categoriaquestoesbinaria cqb ON cqb.cqbid = qb.cqbid
			INNER JOIN pet.eixo ex ON ex.ideixo = cqb.ideixo
			INNER JOIN pet.questionario qst ON qst.queid = ex.queid
			WHERE cqb.cqbstatus = 'A'
				AND qb.qubstatus = 'A'
				AND qst.dataabertura <= CURRENT_DATE
				AND qst.dataencerramento >  CURRENT_DATE
				{$where}
			ORDER BY cqb.nome, qb.numeroquestao
		";
//		ver($sql);
		$dados = $this->_db->carregar($sql);
		return ($dados ? $dados : array());
	}

	public function existe($ideixo)
	{
		$dados = $this->getQuestoes(array("ex.ideixo = $ideixo"));
		return (is_array($dados) and count($dados) > 0);
	}

	public function getQtdQuestoesPorEixo($ideixo)
	{
		$dados = $this->getQuestoes(array("ex.ideixo = $ideixo"));
		return count($dados);
	}

	public function criarTds($respostas, $idgid, $queid, $somenteLeitura = false)
	{
		$dados = $this->getQuestoes(array(" qst.queid = {$queid} "));

		$tds = '';
		$nomeCategoriaOld = null;

		foreach ($dados as $key => $questao) {

			foreach ($respostas as $resp) {
				if ($resp['idgid'] == $idgid && $resp['qubid'] == $questao['qubid']) {
					$respostaJaCadastrada = $resp;
				}
			}
			$checked_n = '';
			$checked_s = '';
//			ver($respostaJaCadastrada);
			$quantidade = $respostaJaCadastrada['quantidade'];
			if ($respostaJaCadastrada['opcaoescolhida'] == 't') {
				$checked_s = 'checked';
			} elseif ($respostaJaCadastrada['opcaoescolhida'] == 'f' || (empty($respostaJaCadastrada['opcaoescolhida']) && !empty($respostaJaCadastrada['rmpid'])) ) {
				$checked_n = 'checked';
			}

			$tds .= '<tr>';
			if ($questao['nome'] != $nomeCategoriaOld) {
				$tds .= "<td rowspan='{$questao['totalporcategoria']}' class='text-center' style='vertical-align: middle'>{$questao['nome']}</td>";
			}

			$nomeCategoriaOld = $questao['nome'];
			$tds .= "<td>{$questao['titulo']}</td>";

			if (!$somenteLeitura) {
				$tds .= "<td class='text-center'><input type='radio' class='questaoBinario' data-qubid='{$questao['qubid']}' name='questao[{$questao['qubid']}][]' value='s' {$checked_s}> </td>";
				$tds .= "<td class='text-center'><input type='radio' class='questaoBinario' data-qubid='{$questao['qubid']}' name='questao[{$questao['qubid']}][]' value='n' {$checked_n}> </td>";
				$tds .= "<td class='form-group'><input type='number' class='form-control qtdBinario inteiro' id='qtd{$questao['qubid']}' name='qtd[{$questao['qubid']}][]' value='{$quantidade}'></td>";
			} else {
				$tds .= "<td class='text-center'>" . ($checked_s ? 'SIM' : '') . "</td>";
				$tds .= "<td class='text-center'>" . ($checked_n ? 'NÃO' : '') . "</td>";
				$tds .= "<td class='text-center'>" . $quantidade . "</td>";
			}
			$tds .= '</tr>';
		}
		echo $tds;
	}
}
