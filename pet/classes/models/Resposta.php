<?php

class Model_Resposta extends Abstract_Model
{

	protected $_schema = 'pet';
	protected $_name = 'resposta';
	public $entity = array();
	const PREENCHA_TODO_FORMULARIO = 'Preencha todo o questionário';
	const FORMULARIO_FINALIZADO = 'O formulário encontra-se Finalizado';

	public function __construct($commit = true)
	{
		parent::__construct($commit);
		$this->entity['rmpid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['conid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['qubid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['idgid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['opcaoescolhida'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
		$this->entity['justificativa'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
		$this->entity['quantidade'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
	}

	public function salvarBinaria()
	{
		$questoes = $_POST['questao'];
		$qtds = $_POST['qtd'];
		$questaoBinaria = new Model_Questaobinaria();
		$qtdQuestoes = $questaoBinaria->getQtdQuestoesPorEixo($_POST['ideixo']);
		$consideracoesfinais = new Model_Consideracoesfinais();

		if (!$_SESSION['finalizado']) {
			try {
				if (!empty($questoes) AND count($questoes) == $qtdQuestoes) {

					foreach ($questoes as $key => $questao) {
						$qtd = $qtds[$key][0];
						$questao = ($questao[0] == 's' ? 't' : false);

						if (empty($qtd) && $questao == 't') {
							$this->error[] = array("name" => 'qtdBinario', "msg" => ('É necessário preecher a quantidade correspondente'));
							return false;
						}

						$resposta = new Model_Resposta(false);
						$resposta->populateEntity($_POST);
						$resposta->getByIdQuestionario($_POST['idgid'], $key);
						$resposta->setAttributeValue('opcaoescolhida', $questao);
						$resposta->setAttributeValue('qubid', $key);
						$resposta->setAttributeValue('quantidade', $qtd);
						$resposta->save();
					}
					$resposta->commit();
					$consideracoesfinais->salvarEixoRespondido($_POST['numeroeixo']);
					return true;
				} else {
					$this->error[] = array("name" => 'qtdBinario', "msg" => ('É necessário selecionar todas as Opções'));
					return false;
				}


			} catch (Exception $exc) {
				if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
					echo $exc->getTraceAsString();
				}
				$resposta->rollback();
				return false;
			}
		} else {
			$this->error[] = array("msg" => (self::FORMULARIO_FINALIZADO));
			return false;
		}

	}

	public function salvarMultiplaEscolha()
	{
		if (!$_SESSION['finalizado']) {

			$justificativas = $_POST['justificativa'];
			try {
				foreach ($justificativas as $indice => $justificativa) {
					$conidEscolhido = $_POST['opcaoescolhida' . $indice];
					$consideracoesfinais = new Model_Consideracoesfinais();

					if (!empty($conidEscolhido) AND !empty($justificativa)) {
						$resposta = new Model_Resposta(false);
						
						$rmpid = (int)$_POST['rmpid'][$indice];
						if(!empty($rmpid)){
							$resposta->populateEntity(array('rmpid'=>$rmpid));
						}else{
							unset($_POST['rmpid']);
							$resposta->populateEntity($_POST);
						}

						$resposta->setAttributeValue('justificativa', $justificativa);
						$resposta->setAttributeValue('conid', $conidEscolhido);
						$resposta->save();
						$consideracoesfinais->salvarEixoRespondido($_POST['numeroeixo']);
						$resposta->commit();

					} else {
						if (empty($conidEscolhido)) {
							$this->error[] = array("name" => 'opcaoescolhida', "msg" => ('É necessário selecionar uma Opção'));
						}
						if (empty($justificativa)) {
							$this->error[] = array("name" => 'justificativa', "msg" => ('Campo Obrigatório'));
						}
						return false;
					}
				}
				return true;
			} catch (Exception $exc) {
				if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
					echo $exc->getTraceAsString();
				}
				$resposta->rollback();
				return false;
			}
		} else {
			$this->error[] = array("msg" => (self::FORMULARIO_FINALIZADO));
			return false;
		}
	}

	public function salvarConsideracaoFinal()
	{
		if (!$_SESSION['finalizado']) {

			$consideracoesfinais = new Model_Consideracoesfinais();
			$consideracoesfinais->getByIdQuestionarioIdGrupo($_POST['idgid'], $_POST['queid']);
			$numeroeixorespondido = $consideracoesfinais->getAttributeValue('numeroeixorespondido');

			// verfica se possui algum eixo nao respondido( procura 0 no campo $numeroeixorespondido)
			if (strpos($numeroeixorespondido, '0') === false and !empty($numeroeixorespondido)) {
				$consideracoesfinais->populateEntity($_POST);
				$consideracoesfinais->setAttributeValue('finalizado', 't');

				if($consideracoesfinais->save()){
					return true;
				}else{
					$this->error = $consideracoesfinais->error;
				}
			} else {
				$this->error[] = array("msg" => ('Preencha todo o questionário'));
				return false;
			}
		} else {
			$this->error[] = array("msg" => (self::FORMULARIO_FINALIZADO));
			return false;
		}
	}

	/**
	 ** verifica se o formulario esta finalizado
	 ** @return boolean
	 **/
	public function finalizado($idgid, $queid)
	{
		$consideracoesfinais = new Model_Consideracoesfinais();
		$finalizado = false;

		if (!empty($idgid) and !empty($queid)) {
			$arrayResultado = $consideracoesfinais->getByIdgidQueid($idgid, $queid);
			$consideracoesfinais->populateEntity($arrayResultado);
			$finalizado = $consideracoesfinais->getAttributeValue('finalizado');
		}
		return ($finalizado == 't' ? true : false);

	}

	public function getByGrupo($idgid)
	{
		$sql = "
			SELECT * FROM pet.resposta  AS resp
			LEFT JOIN pet.questaobinaria AS qb ON  qb.qubid = resp.qubid
			LEFT JOIN pet.conceito AS con ON  con.conid = resp.conid
			LEFT JOIN pet.questaomultiplaescolha AS mult ON  mult.qmeid = con.qmeid
			WHERE idgid = {$idgid}
		 ";
		$dados = $this->_db->carregar($sql);

		if($dados){
			foreach ($dados as $key=>$valor){
				if(!empty($valor['conid'])){
					$qme = new Model_Questaomultiplaescolha();
					$qme->getById($valor['conid']);
					$dados[$key]['idEixo'] = $qme->getAttributeValue('ideixo');
				}else{
					$dados[$key]['idEixo'] = null;
				}

			}
		}
		return ($dados ? $dados : array());
	}

	public function getByIdQuestionario($idgid, $qubid)
	{
		$dados = $this->getAllByValues(array('idgid' => $idgid, 'qubid' => $qubid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}
}
