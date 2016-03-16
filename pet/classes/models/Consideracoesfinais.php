<?php

class Model_Consideracoesfinais extends Abstract_Model
{

	protected $_schema = 'pet';
	protected $_name = 'consideracoesfinais';
	public $entity = array();

	public function __construct($commit = true)
	{
		parent::__construct($commit);

		$this->entity['cofid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['idgid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['queid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
		$this->entity['consideracoes'] = array('value' => '', 'type' => 'text', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['numeroeixorespondido'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
		$this->entity['finalizado'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
	}

	public function getByIdgidQueid ($idgid, $queid)
	{
		$dados = $this->getAllByValues(array('idgid' => $idgid, 'queid' => $queid));
		return ($dados ? $dados[0] : array());
	}

	public function salvarEixoRespondido($numeroeixo)
	{
		$this->entity['consideracoes'] = array('is_null' => 'YES');
		$this->populateEntity($_POST);

		$eixo = new Model_Eixo();
		$dados = $eixo->getDadosComQuestoes($this->getAttributeValue('queid') );

		$this->getByIdQuestionarioIdGrupo( $this->getAttributeValue('idgid'),  $this->getAttributeValue('queid'));
		$eixoRespOld = $this->getAttributeValue('numeroeixorespondido');

		$eixoResp = str_split($this->getAttributeValue('numeroeixorespondido'));
		if ($dados) {
			if(empty($eixoRespOld)){
				foreach( $dados as $eixo){
					$eixoResp[$eixo['numeroeixo']-1] = 0;
				}
			}
			$eixoResp[$numeroeixo-1] = 1;
			$eixoResp = implode('',$eixoResp);
			$this->setAttributeValue('numeroeixorespondido', $eixoResp);

			$this->save();
		}

	}

	public function getByIdQuestionario($queid)
	{
		$dados = $this->getAllByValues(array('queid' => $queid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function reabrirQuestionario($cofid)
	{
		$this->getConsideracoesfinais($cofid);
		$this->setAttributeValue('finalizado', 'f');
		return $this->save();
	}

	public function getConsideracoesfinais($cofid)
	{
		$dados = $this->getAllByValues(array('cofid' => $cofid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function getByIdQuestionarioIdGrupo($idgid, $queid)
	{
		$dados = $this->getAllByValues(array('idgid' => $idgid, 'queid' => $queid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}
}
