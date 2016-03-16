<?php

include_once APPRAIZ . "pet/classes/lib/HtmlTableCustom.php";

class Model_Questionario extends Abstract_Model
{

	protected $_schema = 'pet';
	protected $_name = 'questionario';
	public $entity = array();
	const  MSG_ERRO_EM_PREENCHIMENTO = 'Prazo de edição expirado, questionário não pode ser editado após o inicio do seu preenchimento pelas Universidades.';
	const  MSG_ERRO_JA_CADASTRADO_PERIODO = 'Já existe um questionário para esse periodo';

	public function __construct($commit = true)
	{
		parent::__construct($commit);

		$this->entity['queid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
		$this->entity['dataabertura'] = array('value' => '', 'type' => 'date', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['dataencerramento'] = array('value' => '', 'type' => 'date', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['editavel'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
		$this->entity['questatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
		$this->entity['titulo'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '150', 'contraint' => '');
	}

	public function getTitulo()
	{
		return $this->getAttributeValue('titulo') . ' - ' . $this->getAttributeValue('dataabertura') . ' à ' . $this->getAttributeValue('dataencerramento');
	}

	public function getListaQuestionario()
	{
		$sql = "SELECT qust.queid, qust.titulo,
					to_char(qust.dataabertura, 'DD/MM/YYYY') as dataabertura ,
					to_char(qust.dataencerramento, 'DD/MM/YYYY') as dataencerramento
                FROM pet.questionario  AS qust
                WHERE questatus = 'A' ORDER BY qust.dataabertura  ";
		$dados = $this->_db->carregar($sql);
		$dados = ($dados ? $dados : null);

		$listagem = new Listing(false);
		$listagem->setPerPage(30);
		$listagem->setActions(array('edit' => 'editar_questionario', 'delete' => 'apagar_questionario', 'th-large' => 'selecionar'));
		$listagem->setHead(array('Titulo', 'Data de Abertura', 'Data de Encerramento'));
		$listagem->setEnablePagination(false);
		$listagem->listing($dados);
	}

	public function excluir($id)
	{
		$this->populateEntity(array('queid' => $id));
		$this->setAttributeValue('questatus', 'I');
		$this->setDecode(false);
		$this->treatEntityToUser();
		$this->save();
	}

	public function questionarioEmPreechimento($queid = false)
	{
		if ($queid) {
			$this->getQuestionario($queid);
		}
		$queid_ = $this->getAttributeValue('queid');
		$dataabertura = formata_data_sql($this->getAttributeValue('dataabertura'));

		if (!empty($queid_) and !empty($dataabertura)) {
			$dateAtual = strtotime(date("Y-m-d"));
			return (strtotime($dataabertura) > $dateAtual);
		}
		return true;

	}

	public function salvar()
	{
		$this->populateEntity($_POST);

		if ($this->getQuestionarioExiste()) {
			if ($this->questionarioEmPreechimento()) {
				$this->setAttributeValue('questatus', 'A');
				$this->setAttributeValue('editavel', 't');
				return $this->save();
			} else {
				$this->error[] = array("msg" => (self::MSG_ERRO_EM_PREENCHIMENTO));
				return false;
			}
		} else {
			$this->error[] = array("msg" => (self::MSG_ERRO_JA_CADASTRADO_PERIODO));
			return false;
		}
	}

	public function getQuestionario($queid)
	{
		$dados = $this->getAllByValues(array('queid' => $queid));
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function getComboQuestionario()
	{
		$dados = $this->getAllByValues(array('questatus' => 'A'));
		return $this->getOptions($dados, array('prompt' => 'Selecione ...'), 'queid', 'titulo');
	}

	public function getQuestionarioExiste()
	{
		$sqlAnd = '';
		$dataInicio_ = $this->getAttributeValue('dataabertura');
		$dataFim_ = $this->getAttributeValue('dataencerramento');
		$queid = $this->getAttributeValue('queid');

		if (!empty($queid)) {
			$sqlAnd = " AND queid != {$queid}";
		}
		$sql = "
			SELECT * FROM pet.questionario  AS qust
			WHERE questatus = 'A'
			AND ( qust.dataabertura between '{$dataInicio_}' and '{$dataFim_}' OR qust.dataencerramento between  '{$dataInicio_}' and '{$dataFim_}' )
			{$sqlAnd}
			ORDER BY qust.dataabertura
		 ";
//		ver($sql, d);
		$dados = $this->_db->carregar($sql);
		if ($dados) {
			return false;
		}
		return true;
	}

	public function getQuestionarioAtual()
	{
		$sql = "SELECT qust.queid, qust.titulo,
					to_char(qust.dataabertura, 'MM/DD/YYYY') as dataabertura ,
					to_char(qust.dataencerramento, 'MM/DD/YYYY') as dataencerramento
                FROM pet.questionario  AS qust
                WHERE questatus = 'A'
                	AND qust.dataabertura <= CURRENT_DATE
					AND qust.dataencerramento >= CURRENT_DATE
                ORDER BY qust.dataabertura  ";
		
		$dados = $this->_db->carregar($sql);
		if ($dados) {
			$this->populateEntity($dados[0]);
			$this->treatEntityToUser();
		}
	}

	public function getTablePainel()
	{
		$dados = $this->getDadosPainel();

		$tabelaArvoreQuestionario = new HtmlTableCustom('rel_questionario', 'table table-striped table-bordered');
		$camposTabelaQuestionario = array('visualizar' => '', 'queid' => 'ID Questionario', 'dataabertura' => 'Data de Abertura', 'dataencerramento' => 'Data de Encerramento', 'titulo' => 'Título', 'per_preenc_questionario' => 'Concluído (%)');
		$tabelaArvoreQuestionario->setHeader($camposTabelaQuestionario);
		$tabelaArvoreQuestionario->addTSection('tbody');

		//adiciona a tabela de UF
		$tabelaUf = $this->getTableUf($dados);

		//calcula porcentagem
		if ($tabelaUf['porcentagem']['qtd'] > 0) {
			$per_preenc_questionario = (int)($tabelaUf['porcentagem']['somario'] / $tabelaUf['porcentagem']['qtd']);
		}
		$dados['questionario']['per_preenc_questionario'] = $per_preenc_questionario . '%';

		if ( !empty ($dados['questionario']['queid'] )){
			$tabelaArvoreQuestionario->setBody(array($dados['questionario']), $camposTabelaQuestionario, 'active');

			if (!empty($tabelaUf)) {
				$tabelaArvoreQuestionario->addRow('active', array('style' => 'display: none'));
				$tabelaArvoreQuestionario->addCell($tabelaUf['htmlTable'], 'td_uf', 'data', array('colspan' => 6));
			}
			echo $tabelaArvoreQuestionario->display();
		}else{
			echo '<div class="alert alert-warning" role="alert"><span class="glyphicon glyphicon-warning-sign" aria-hidden="true"></span> Não possui questionário vigente.</div>';
		}


	}

	public function getTableUf($dados)
	{
		$tabelaArvoreUf = new HtmlTableCustom('rel_uf', 'table table-striped table-bordered', array('style' => 'font-size: 14; margin-left: 15px; width: 97%; font-weight: bold;'));
		$camposTabelaUf = array('visualizar' => '', 'uf' => 'UF', 'per_preenc_uf' => 'Concluído (%)');
		$tabelaArvoreUf->setHeader($camposTabelaUf);
		$tabelaArvoreUf->addTSection('tbody');
		$somaPorcentagem = 0;
		$cont = 0;
		$flag = false;

		$uf_old = null;
		foreach ($dados['uf'] as $uf) {
			if ($uf['uf'] != $uf_old) {
				$flag = true;
				//adiciona a tabela de instituicao da UF
				$tableArray = $this->getTableInstituicao($dados, $uf);

				//calcula porcentagem
				if ($tableArray['porcentagem']['qtd'] > 0) {
					$per_preenc_uf = (int)($tableArray['porcentagem']['somario'] / $tableArray['porcentagem']['qtd']);
				}
				$uf['per_preenc_uf'] = $per_preenc_uf . '%';
				$somaPorcentagem += $per_preenc_uf;
				$cont++;

				//adcionar celular de uf
				$tabelaArvoreUf->addRow('warning');
				$tabelaArvoreUf->addCell($uf['visualizar'], 'text-center');
				$tabelaArvoreUf->addCell($uf['uf'], 'text-center uf');
				$tabelaArvoreUf->addCell($uf['per_preenc_uf'], 'text-center per_preenc_uf', 'data', array('data-perc' => $per_preenc_uf));


				if (!empty($tableArray)) {
					$tabelaArvoreUf->addRow('warning', array('style' => 'display: none'));
					$tabelaArvoreUf->addCell($tableArray['htmlTable'], 'td_iesid', 'data', array('colspan' => 3));
				}
			}
			$uf_old = $uf['uf'];
		}
		if ($flag) {
			return array('htmlTable' => $tabelaArvoreUf->display(), 'porcentagem' => array('somario' => $somaPorcentagem, 'qtd' => $cont));
		}
		return array();
	}

	public function getTableInstituicao($dados, $uf)
	{
		$camposTabelaInstituicao = array('visualizar' => '', 'iesid' => 'IESID', 'nome' => 'Nome', 'per_preenc_iesid' => 'Concluído (%)');
		$tabelaArvoreInstituicao = new HtmlTableCustom('rel_inst', 'table table-striped table-bordered', array('style' => 'font-size: 13; margin-left: 15px; width: 97%;'));
		$tabelaArvoreInstituicao->setHeader($camposTabelaInstituicao);
		$tabelaArvoreInstituicao->addTSection('tbody');
		$somaPorcentagem = 0;
		$cont = 0;
		$iesid_old = null;
		$flag = false;

		foreach ($dados['inst'] as $iesid => $instituicoes) {
			foreach ($instituicoes as $instituicao) {

				if ($instituicao['uf'] == $uf['uf'] && $instituicao['iesid'] != $iesid_old) {
					$flag = true;

					//adiciona a tabela de instituicao da UF
					$tableArray = $this->getTableGrupo($dados, $instituicao);

					//calcula porcentagem
					if ($tableArray['porcentagem']['qtd'] > 0) {
						$per_preenc_iesid = (int)($tableArray['porcentagem']['somario'] / $tableArray['porcentagem']['qtd']);
					}
					$instituicao['per_preenc_iesid'] = $per_preenc_iesid . '%';
					$somaPorcentagem += $per_preenc_iesid;
					$cont++;

					//adcionar celular de instituicao
					$tabelaArvoreInstituicao->setBody(array($instituicao), $camposTabelaInstituicao, 'active');

					if (!empty($tableArray)) {
						$tabelaArvoreInstituicao->addRow('active', array('style' => 'display: none'));
						$tabelaArvoreInstituicao->addCell($tableArray['htmlTable'], 'td_grupo', 'data', array('colspan' => 4));
					}
				}
				$iesid_old = $instituicao['iesid'];


			}
		}
		if ($flag) {
			return array('htmlTable' => $tabelaArvoreInstituicao->display(), 'porcentagem' => array('somario' => $somaPorcentagem, 'qtd' => $cont));
		}
		return array();
	}

	public function getTableGrupo($dados, $instituicao)
	{
		$camposTabelaGrupo = array('visualizar' => '', 'reabrir' => '', 'abrangencia' => 'Abrangencia', 'nomegrupo' => 'Grupo', 'per_preenc' => 'Concluído (%)', 'situacao' => 'Situação');
		$tabelaArvoreGrupo = new HtmlTableCustom('rel_inst', 'table table-striped table-bordered', array('style' => 'font-size: 12; margin-left: 15px; width: 97%;'));
		$tabelaArvoreGrupo->setHeader($camposTabelaGrupo);
		$tabelaArvoreGrupo->addTSection('tbody');
		$flag = false;
		$somaPorcentagem = 0;
		$cont = 0;

		foreach ($dados['grupo'] as $iesid => $grupos) {
			foreach ($grupos as $grupo) {
				if ($iesid == $instituicao['iesid']) {
					$flag = true;
					$somaPorcentagem += (int)$grupo['per_preenc'];
					$cont++;
					$tabelaArvoreGrupo->setBody(array($grupo), $camposTabelaGrupo, 'info');
				}
			}
		}

		if ($flag) {
			return array('htmlTable' => $tabelaArvoreGrupo->display(), 'porcentagem' => array('somario' => $somaPorcentagem, 'qtd' => $cont));
		}
		return array();
	}

	public function getDadosPainel()
	{
		$questionario = array();
		$ufs = array();
		$inst = array();
		$grupo = array();

		$sql = $this->getSqlPainel();
		$dados = $this->_db->carregar($sql);
		$dados = ($dados ? $dados : array());

		$key = 0;
		if (is_array($dados)) {
			foreach ($dados as $valor) {
				if(!empty($valor['queid'])){
					$questionario['questionario'] = array('visualizar' => '<a href="#"><span aria-hidden="true" class="glyphicon view_nivel glyphicon-chevron-down"></span></a>', 'queid' => $valor['queid'], 'editavel' => $valor['editavel'], 'titulo' => $valor['titulo'], 'dataabertura' => $valor['dataabertura'], 'dataencerramento' => $valor['dataencerramento'], 'per_preenc_questionario' => $valor['per_preenc_questionario'],);
				}
				$ufs[] = array('visualizar' => '<a href="#"><span aria-hidden="true" class="glyphicon view_nivel glyphicon-chevron-down"></span></a>', 'uf' => $valor['uf'], 'per_preenc_uf' => $valor['per_preenc_uf']);
				$inst[$valor['iesid']][] = array('visualizar' => '<a href="#"><span aria-hidden="true" class="glyphicon view_nivel glyphicon-chevron-down"></span></a>', 'uf' => $valor['uf'], 'iesid' => $valor['iesid'], 'nome' => $valor['nome'], 'per_preenc_iesid' => $valor['per_preenc_iesid']);

				$btReabrirQuestionario = '<a title="reabrir questionario do grupo" class="reabrir" data-id="' . $valor['grpid'] . '" href="#"><span aria-hidden="true" class="glyphicon glyphicon-repeat"></span></a>';

				$grupo[$valor['iesid']][$valor['grpid']] = array('visualizar' => '<a class="visualizarGrupo" data-id="' . $valor['grpid'] . '" data-target="#modalDetalhe" data-toggle="modal" href="#"> <span aria-hidden="true" class="glyphicon glyphicon-eye-open"></span></a>', 'reabrir' => ($valor['situacao'] == 'FINALIZADO' ? $btReabrirQuestionario : ''), 'iesid' => $valor['iesid'], 'abrangencia' => $valor['abrangencia'], 'grpid' => $valor['grpid'], 'situacao' => $valor['situacao'], 'nomegrupo' => $valor['nomegrupo'], 'per_preenc' => $valor['per_preenc']);
				$key++;
			}
		}
		$questionario['uf'] = $ufs;
		$questionario['inst'] = $inst;
		$questionario['grupo'] = $grupo;
		return $questionario;
	}

	public function getSqlPainel()
	{
		$sql = "
SELECT
	 quest.queid
	, quest.titulo
	, to_char(quest.dataabertura, 'DD/MM/YYYY') as dataabertura
	, to_char(quest.dataencerramento, 'DD/MM/YYYY') as dataencerramento,
	CASE
		WHEN quest.editavel = 't' THEN 'SIM'
		WHEN quest.editavel = 'f' THEN 'NÃO'
		ELSE 'NÃO'
	END AS editavel

	, gp.grpid
	, gp.nomegrupo
	, inst.iesid
	, inst.nome
	, inst.uf

	, CASE
		WHEN abrangencia = 'C' THEN 'CURSO ESPECIFICO'
		WHEN abrangencia = 'I' THEN 'INTERDISCIPLINAR'
	END AS abrangencia,
	CASE
		WHEN confin.finalizado = 't' THEN 'FINALIZADO'
		WHEN confin.finalizado = 'f' THEN 'EDITÁVEL'
	END AS situacao,
	COALESCE ( ( (LENGTH(numeroeixorespondido) - LENGTH(REPLACE(numeroeixorespondido, '1', '')) ) * 100 ) / char_length(numeroeixorespondido) || '%' , '0%' ) AS per_preenc
	, '0%'  AS per_preenc_iesid
	, '0%'  AS per_preenc_uf
	, '0%'  AS per_preenc_questionario

	FROM pet.institutoensinosuperior inst
	LEFT JOIN pet.grupopet AS gp ON  gp.iesid = inst.iesid
	LEFT JOIN pet.identificacaogrupo AS idg ON  idg.grpid = gp.grpid
	LEFT JOIN pet.consideracoesfinais AS confin ON  confin.idgid = idg.idgid

	LEFT JOIN pet.resposta AS resp ON  resp.idgid =  idg.idgid
	LEFT JOIN pet.conceito AS con ON  con.conid = resp.conid
	LEFT JOIN pet.questaobinaria AS qb ON  qb.qubid = resp.qubid
	LEFT JOIN pet.categoriaquestoesbinaria AS qbc ON  qbc.cqbid = qb.cqbid
	LEFT JOIN pet.questaomultiplaescolha AS qm ON  qm.qmeid = con.qmeid

	LEFT JOIN pet.eixo AS ex ON (ex.ideixo = qm.ideixo) OR (ex.ideixo = qbc.ideixo)
	LEFT JOIN pet.questionario  AS quest ON  quest.queid = ex.queid AND questatus = 'A' AND quest.dataencerramento >= CURRENT_DATE AND quest.dataabertura <=  CURRENT_DATE

	ORDER BY inst.uf
			   ";
//		ver($sql, d);
		return $sql;
	}


}
