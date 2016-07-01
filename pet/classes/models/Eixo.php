<?php

class Model_Eixo extends Abstract_Model
{
	protected $_schema = 'pet';
	protected $_name = 'eixo';
	public $entity = array();
	const TIPO_BINARIO = 'B';
	const TIPO_MULTIPLA_ESCOLHA = 'M';


	public function __construct($commit = true)
	{
		parent::__construct($commit);
		$this->questionario = new Model_Questionario(false);

		$this->entity['ideixo'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['queid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['nome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '150', 'contraint' => '');
		$this->entity['descricao'] = array('value' => '', 'type' => 'text', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['numeroeixo'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['tipo'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '');
		$this->entity['eixstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
	}

	public function getDados($campos = false, $queid = null)
	{
		$m = Model_Eixo::TIPO_MULTIPLA_ESCOLHA;
		$b = Model_Eixo::TIPO_BINARIO;
		$descricaoSql = '';
		$where = '';
		if ($campos) {
			$descricaoSql = ', eixo.descricao, eixo.queid ';
		}
		if ($queid) {
			$where .= " AND queid = {$queid} ";
		}
		$sql = "SELECT eixo.ideixo, eixo.numeroeixo, eixo.nome,
					CASE
						WHEN tipo='{$b}' THEN 'Binárias'
						WHEN tipo='{$m}' THEN 'Multipla Escolha'
					END as tipo
					$descricaoSql
                FROM pet.eixo  AS eixo
                WHERE eixo.eixstatus = 'A'
                {$where}
                ORDER BY eixo.numeroeixo   ";

		$dados = $this->_db->carregar($sql);
		$dados = $dados ? $dados : array();
		return $dados;
	}

	public function getMenuEixo($queid)
	{
		$dados = $this->getDadosComQuestoes($queid);
		$HtmlLi = '';

		foreach ($dados as $key => $valor) {
			if ($key == 0) {
				$active = 'active';
			} else {
				$active = '';
			}

			if ($valor['tipo'] == 'Binárias') {
				$tipo = 'B';
			} elseif ($valor['tipo'] == 'Multipla Escolha') {
				$tipo = 'M';
			}
			$HtmlLi .= "<li class='{$active}'><a class='menuEixo' data-toggle='tab' data-id='{$valor['ideixo']}' data-tipo='{$tipo}'
				href='#tabEixo{$valor['numeroeixo']}'>
				<span class='glyphicon glyphicon-pushpin' aria-hidden='true'></span>
				Eixo {$valor['numeroeixo']}
		    </a></li>";
		}
		echo $HtmlLi;
	}

	public function getListarEixo($queid)
	{
		$dados = $this->getDados(false, $queid);
		$dados = ($dados ? $dados : null);

		$listagem = new Listing(false);
		$listagem->setPerPage(30);
		$listagem->setActions(array('edit' => 'editar_eixo', 'delete' => 'apagar_eixo', 'list-alt' => 'selecionar_eixo'));
		$listagem->setHead(array('Número', 'Nome', 'Tipo da Questão'));
		$listagem->setEnablePagination(false);
		$listagem->listing($dados);
	}

	public function excluir($id)
	{
		$this->populateEntity(array('ideixo' => $id));

		$this->questionario->getQuestionario($this->getAttributeValue('queid'));

		if ($this->questionario->questionarioEmPreechimento()) {
			$this->setAttributeValue('eixstatus', 'I');
			$this->setDecode(false);
			$this->treatEntityToUser();
			return $this->save();
		} else {
			ver('123123', d);
			return false;
		}
	}

	public function salvar($queid)
	{
		$this->questionario->getQuestionario($queid);
		if ($this->questionario->questionarioEmPreechimento()) {
			$this->populateEntity($_POST);
			$this->setAttributeValue('eixstatus', 'A');
			$this->setAttributeValue('queid', $queid);

			$ideixo = $this->getAttributeValue('ideixo');
			if (empty($ideixo)) {
				$dados = $this->getDados(false, $queid);
				$cont = count($dados);
				$this->setAttributeValue('numeroeixo', $cont + 1);
			}

		} else {
			$this->error[] = array("msg" => (Model_Questionario::MSG_ERRO_EM_PREENCHIMENTO));
			return false;
		}
		return $this->save();
	}

	public function getEixo($ideixo)
	{
		$dados = $this->getAllByValues(array('ideixo' => $ideixo));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function getTitulo()
	{
		return $this->getAttributeValue('numeroeixo') . ' - ' . $this->getAttributeValue('nome');
	}

	public function getDadosComQuestoes($queid = null)
	{
		$m = Model_Eixo::TIPO_MULTIPLA_ESCOLHA;
		$b = Model_Eixo::TIPO_BINARIO;
		$where = '';
		if ($queid) {
			$where .= " AND queid = {$queid} ";
		}

		$arrayIds = $this->getIdsEixoQuestao();
		if (!empty($arrayIds)) {
			$ids = implode(',', $arrayIds);
			$where .= " AND ideixo in ({$ids})";
		}
		$sql = "SELECT eixo.ideixo, eixo.numeroeixo, eixo.nome,
					eixo.descricao, eixo.queid,
					CASE
						WHEN tipo='{$b}' THEN 'Binárias'
						WHEN tipo='{$m}' THEN 'Multipla Escolha'
					END as tipo

                FROM pet.eixo  AS eixo
                WHERE eixo.eixstatus = 'A'
                {$where}
                ORDER BY eixo.numeroeixo   ";

		$dados = $this->_db->carregar($sql);
		$dados = $dados ? $dados : array();
		return $dados;
	}

	public function getIdsEixoQuestao()
	{
		$sql = 'SELECT DISTINCT ideixo from pet.categoriaquestoesbinaria AS cqb
					INNER JOIN pet.questaobinaria qb ON cqb.cqbid = qb.cqbid
					UNION
				SELECT DISTINCT ideixo from pet.questaomultiplaescolha ';
		$dados = $this->_db->carregar($sql);
		$ids = array();
		if ($dados) {
			foreach ($dados as $valor) {
				$ids[] = $valor['ideixo'];
			}
		}
		return $ids;
	}
}
