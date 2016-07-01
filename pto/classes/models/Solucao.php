<?php

class Model_Solucao extends Abstract_Model
{
	protected $_schema = 'pto';
	protected $_name = 'solucao';
	public $entity = array();
	public $perfilUsuario;

	public function __construct($commit = true)
	{
		$this->perfilUsuario = new Model_PerfilUsuario();
		parent::__construct($commit);

		$this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'Código');
		$this->entity['soldsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '', 'label' => 'Projeto');
		$this->entity['solstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '', 'label' => 'Status');
		$this->entity['solmetajustificativa'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => 'Justificativa');
		$this->entity['solnumero'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => 'Número');
		$this->entity['solobs'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Observação');
		$this->entity['solordem'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Ordem');
		$this->entity['solapelido'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => 'Apelido');
		$this->entity['solprazo'] = array('value' => '', 'type' => 'date', 'is_null' => '', 'maximum' => '', 'contraint' => '', 'label' => 'Prazo');
		$this->entity['solcorpolei'] = array('value' => '', 'type' => 'boolean', 'is_null' => '', 'maximum' => '', 'contraint' => '', 'label' => 'solcorpolei');
	}

	public function getListing()
	{
		$listing = new Listing();
		$listing->setPerPage(999999);
		$listing->setIdTable('table_solucao');
		$listing->setClassTable('table table-striped table-bordered sorted_table');
		$listing->setHead(array($this->getAttributeLabel('solnumero'), $this->getAttributeLabel('soldsc'), $this->getAttributeLabel('solapelido'), $this->getAttributeLabel('solprazo'), $this->getAttributeLabel('solobs')));
		if ($this->perfilUsuario->possuiAcessoConsulta()) {
			$listing->setActions(array('chevron-down' => 'visualizar_etapa', 'edit' => 'editar', 'th' => 'painel'));
		} else {
			$listing->setActions(array('chevron-down' => 'visualizar_etapa', 'resize-vertical' => 'ordenar_solucao', 'edit' => 'editar', 'delete' => 'excluir', 'th' => 'painel'));
		}

		return $listing;
	}

	public function getDadosGrid($params = array())
	{
		$where = $this->getWhere($params);
		$sql = $this->getSqlBusca($where);


		$data = $this->_db->carregar($sql);
		if ($data) {
			foreach ($data as $key => $valor) {

				$data[$key]['soldsc'] = simec_htmlentities($valor['soldsc']);
				if(!empty($valor['solprazo'])){
					$data[$key]['solprazo'] = formata_data($valor['solprazo']);
				}
				unset($data[$key]['solordem']);
			}
		}
		return $data;
	}

	public function getWhere($params)
	{
		$where = ' ';
		$numeroSql = '';
		if ($params['solid']) {
			$params['solid'] = (int)$params['solid'];
			$where .= " AND sol.solid =  {$params['solid']} ";
		}

		if (!empty( $params['solprazo']) ) {
			$solprazo = formata_data_sql( $params['solprazo']);
			$where .= " AND sol.solprazo =  '{$solprazo}' ";
		}

		if ($params['diverso']) {
			$dsc = trim(utf8_decode($params['diverso']));
			$int = (int)$params['diverso'];
			if (!empty($int)) {
				$numeroSql = " sol.solnumero = {$int} OR ";
			}
			$where .= " AND (
                              {$numeroSql}
                               sol.soldsc ILIKE '%{$dsc}%'
                               OR sol.solapelido ILIKE '%{$dsc}%'
                               OR etapa.etpdsc ILIKE '%{$dsc}%'
                               OR atv.atvdsc ILIKE '%{$dsc}%'
                              )
                      ";
		}

		if (is_array($params['acaid'])) {
			$acaids = implode(',', $params['acaid']);
			$where .= " AND acao_solucao.acaid in ( {$acaids} ) ";
		}
		if (is_array($params['temid'])) {
			$temids = implode(',', $params['temid']);
			$where .= " AND temasolucao.temid in ( {$temids} ) ";
		}
		if (is_array($params['mpneid'])) {
            $where .= "AND (";
            if (in_array('99999999',$params['mpneid'])){
                $where .= " sol.solcorpolei IS TRUE OR ";
            }
            if (in_array('nenhuma',$params['mpneid'])){
                $where .= " metasolucao.mpneid IS NULL OR ";
            }
            $mpneids = implode(',', $params['mpneid']);
            $mpneids = str_replace('nenhuma,','', $mpneids);
            $mpneids = str_replace('99999999,','', $mpneids);
            $where .= " metasolucao.mpneid in ( {$mpneids} ) ) ";
		}

		return $where;
	}

	public function getSqlBusca($where, $all = false)
	{
		if ($all) {
			$campos = '*';
			$order = "ORDER BY sol.solordem ASC, etapa.etpordem ASC";
		} else {
			$campos = 'DISTINCT sol.solid, sol.solnumero, sol.soldsc, sol.solapelido, sol.solprazo, sol.solobs, sol.solordem';
			$order = "ORDER BY sol.solordem ASC;";
		}

        return "SELECT {$campos}
                FROM pto.solucao sol
                LEFT JOIN pto.etapa etapa ON etapa.solid = sol.solid AND etapa.etpstatus = 'A'
                LEFT JOIN pto.acaosolucao acao_solucao ON acao_solucao.solid  = sol.solid
                LEFT JOIN pto.metasolucao metasolucao  ON metasolucao.solid  = sol.solid
                INNER JOIN pto.temasolucao temasolucao ON temasolucao.solid  = sol.solid
                LEFT JOIN pto.objetivosolucao objetivosolucao ON objetivosolucao.solid  = sol.solid
	        	LEFT JOIN pto.responsavelsolucao responsavelsolucao ON responsavelsolucao.solid  = sol.solid
	        	LEFT JOIN pto.atividade atv ON atv.etpid = etapa.etpid AND atv.atvstatus = 'A'

                WHERE sol.solstatus = 'A'  {$where} {$order}
         ";
	}

	public function inserirSolucao()
	{
		if ($_POST['solid']) {
			$this->setAttributeValue('solid', $_POST['solid']);
		}
		$this->setAttributeValue('soldsc', $_POST['soldsc']);
		$this->setAttributeValue('solmetajustificativa', $_POST['solmetajustificativa']);
		$this->setAttributeValue('solnumero', $_POST['solnumero']);
		$this->setAttributeValue('solobs', trim($_POST['solobs']));
		$this->setAttributeValue('solstatus', 'A');
		$this->setAttributeValue('solordem', 1);
		$this->setAttributeValue('solapelido', $_POST['solapelido']);
		if (!empty($_POST['solprazo'])) {

			$this->setAttributeValue('solprazo', $_POST['solprazo']);
		}
		$idSolucao = $this->save();

		if ($idSolucao == false) {
			throw new Exception('Erro ao inserir o Projeto.');
		} else {
			return $idSolucao;
		}
	}

	public function inativar($id)
	{
		$this->populateEntity(array('solid' => $id));
		$this->setAttributeValue('soldsc', ($this->getAttributeValue('soldsc')));
		$this->setAttributeValue('solstatus', 'I');
		$idSolucao = $this->update();
		if ($idSolucao == false) {
			throw new Exception('Erro ao atualizar o Projeto.');
		} else {
			return $idSolucao;
		}
	}

	public function getTituloSolucao()
	{
		return ("<b>Projeto: </b> {$this->getAttributeValue('solnumero')} - {$this->getAttributeValue('soldsc')}");
	}

	public function setSolucao($parans)
	{
		return array('solid' => $parans['solid'], 'soldsc' => $parans['soldsc'], 'solstatus' => $parans['solstatus'], 'solmetajustificativa' => $parans['solmetajustificativa'], 'solnumero' => $parans['solnumero'], 'solobs' => $parans['solobs']);
	}

	public function setEtapa($parans)
	{
		return array('etpid' => $parans['etpid'], 'acaid' => $parans['acaid'], 'etpordem' => $parans['etpordem'], 'etpdsc' => $parans['etpdsc'], 'etpobs' => $parans['etpobs'], 'etpstatus' => $parans['etpstatus'],);
	}

	public function setAtividade($parans)
	{
		return array('etpid' => $parans['etpid'], 'atvid' => $parans['atvid'], 'docid' => $parans['docid'], 'usucpf' => $parans['usucpf'], 'atvordem' => $parans['atvordem'], 'atvdsc' => $parans['atvdsc'], 'atvprazo' => $parans['atvprazo'], 'atvstatus' => $parans['atvstatus']);
	}

	public function getAllSolucao($solid)
	{
		$sql = $this->getSqlBusca(" AND sol.solid = {$solid}", true);
		$dados = $this->_db->carregar($sql);

		$retorno = array();
		$etapas = array();
		$idsExternos = array();
		$parans_old = array();

		$acsids_old = array();
		$mpneid_old = array();
		$usucpf_old = array();
		$temid_old = array();

		foreach ($dados as $parans) {
			$retorno['solucao'] = $this->setSolucao($parans);

			if (isset($parans['etpid']) && $parans_old['etpid'] != $parans['etpid']) {
				$etapas[] = $this->setEtapa($parans);
				$parans_old['etpid'] = $parans['etpid'];
			}

			/********************************************/
			if (isset($parans['acsid']) && !in_array($parans['acsid'], $acsids_old)) {
				$idsExternos['acsid'][] = $parans['acsid'];
				$acsids_old[] = $parans['acsid'];
			}
			if (isset($parans['mpneid']) && !in_array($parans['mpneid'], $mpneid_old)) {
				$idsExternos['mpneid'][] = $parans['mpneid'];
				$mpneid_old[] = $parans['mpneid'];
			}
			if (isset($parans['temid']) && !in_array($parans['temid'], $temid_old)) {
				$idsExternos['temid'][] = $parans['temid'];
				$temid_old[] = $parans['temid'];
			}
			if (isset($parans['usucpf']) && !in_array($parans['usucpf'], $usucpf_old)) {
				$idsExternos['usucpf'][] = $parans['usucpf'];
				$usucpf_old[] = $parans['usucpf'];
			}
			/********************************************/
			$parans_old = $parans;
		}

		$retorno['etapas'] = $etapas;
		$retorno['idsExternos'] = $idsExternos;
		return $retorno;
	}

	public function getOptionsSolucao()
	{
		$dados = array();
		return $this->getOptions($dados, array('prompt' => 'todos'), 'solid', 'soldsc');
	}


	public function alterarOrdem($solid, $ordem)
	{
		$this->populateEntity(array('solid' => $solid));
		$this->setAttributeValue('solordem', $ordem);
		$this->setDecode(false);
		$id = $this->update();

		if ($id == false) {
			throw new Exception('Erro ao ordenar a Etapa.');
		} else {
			return $id;
		}
	}
}
