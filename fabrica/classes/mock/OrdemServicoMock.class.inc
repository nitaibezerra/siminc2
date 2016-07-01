<?php
/**
 * Ordem de Servico MOCK do <strong>Modulo Fábrica</strong>.<br/>
 * Criado para simular recuperação de OS no banco de dados para Ordens de Serviço candidatas a irem para o memorando.
 * @author rayner
 *
 */
class OrdemServicoMock extends Modelo {

	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "fabrica.ordemservico";

	/**
	 * Chave primaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("odsid");

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
        'odsid' => null,
        'scsid' => null,
        'odsdetalhamento' => null,
        'odsdtprevinicio' => null,
        'odsdtprevtermino' => null,
        'docid' => null,
		'odsidpai' => null,
		'odsqtdpfdetalhada' => null,
		'odscontratada' => null,
		'odsidpf'=>null,
		'docidpf'=>null,
		'tosid'=>null,
		'tpeid'=> null,
		'odsqtdpffinal'=>null,
		'odsenderecosvn'=>null,
		'odssubtotalpf'=>null,
		'odssubtotalpfdetalhada'=>null,
		'ctrid'=>null,
		'memoid'=>null,
		'glosaid'=>null
	);

	private $porcentagemDisciplina;

	private $valorUnitarioDePf;

	private $valorAReceberDaOs;

	private $statusOrdemServico;

	protected $stCampos = "os.odsid, os.scsid, os.odsdetalhamento, os.odsdtprevinicio, os.odsdtprevtermino, os.docid, os.odsidpai,
		os.odsqtdpfdetalhada, os.odscontratada, os.odsidpf, os.docidpf, os.tosid, os.tpeid, os.odsqtdpffinal, os.odsenderecosvn, 
		os.odssubtotalpf, os.odssubtotalpfdetalhada, os.ctrid , os.memoid, os.glosaid";

	protected $stTodosOsCampos = "*";


	public function setId($id){
		$this->arAtributos['odsid'] = $id;
	}
	public function setIdSolicitacaoServico($idSolicitacaoServico){
		$this->arAtributos['scsid'] = $idSolicitacaoServico;
	}
	public function setDetalhamento($detalhamento){
		$this->arAtributos['odsdetalhamento'] = $detalhamento;
	}
	public function setDataPrevisaoDeInicio($dataPrevisaoInicio){
		$this->arAtributos['odsdtprevinicio'] = new DateTime($dataPrevisaoInicio);
	}
	public function setDataPrevisaoDeTermino($dataPrevisaoTermino){
		$this->arAtributos['odsdtprevtermino'] = new DateTime($dataPrevisaoTermino);
	}
	public function setIdDocumento($idDocumento){
		$this->arAtributos['docid'] = $idDocumento;
	}
	public function setIdPai($idPai){
		$this->arAtributos['odsidpai'] = $idPai;
	}
	public function setQtdePfDetalhada($qtdePfDetalhada){
		$this->arAtributos['odsqtdpfdetalhada'] = $qtdePfDetalhada;
	}
	public function setContratada($contratada){
		$this->arAtributos['odscontratada'] = $contratada;
	}
	public function setIdPf($idPf){
		$this->arAtributos['odsidpf'] = $idPf;
	}
	public function setIdDocumentoPf($IdDocumentoPf){
		$this->arAtributos['docidpf'] = $IdDocumentoPf;
	}
	public function setIdTipoOrdemServico($IdTipoOrdemServico){
		$this->arAtributos['tosid'] = $IdTipoOrdemServico;
	}
	public function setIdTipoExecucao($IdTipoExecucao){
		$this->arAtributos['tpeid'] = $IdTipoExecucao;
	}
	public function setQtdePfFinal($qtdePfFinal){
		$this->arAtributos['odsqtdpffinal'] = $qtdePfFinal;
	}
	public function setEnderecoSvn($enderecoSvn){
		$this->arAtributos['odsenderecosvn'] = $enderecoSvn;
	}
	public function setSubTotalPf($subTotalPf){
		$this->arAtributos['odssubtotalpf'] = $subTotalPf;
	}
	public function setSubTotalPfDetalhada($subTotalPfDetalhada){
		$this->arAtributos['odssubtotalpfdetalhada'] = $subTotalPfDetalhada;
	}
	public function setIdContrato($IdContrato){
		$this->arAtributos['ctrid'] = $IdContrato;
	}
	public function setIdMemorando($idMemorando){
		$this->arAtributos['memoid'] = $idMemorando;
	}
	public function setIdGlosa($idGlosa){
		$this->arAtributos['glosaid'] = $idGlosa;
	}
	public function setPorcentagemDisciplina($porcentagemDisciplina){
		$this->porcentagemDisciplina = $porcentagemDisciplina;
	}
	public function setValorUnitarioDePf($valorUnitarioDePf){
		$this->valorUnitarioDePf = $valorUnitarioDePf;
	}
	public function setValorAReceberDaOs($valorAReceberDaOs){
		$this->valorAReceberDaOs = $valorAReceberDaOs;
	}
	public function setStatusOrdemServico($statusOrdemServico){
		$this->statusOrdemServico = $statusOrdemServico;
	}


	public function getId(){
		return $this->arAtributos['odsid'];
	}
	public function getIdSolicitacaoServico(){
		return $this->arAtributos['scsid'];
	}
	public function getDetalhamento(){
		return $this->arAtributos['odsdetalhamento'];
	}
	public function getDataPrevisaoDeInicio(){
		return new DateTime($this->arAtributos['odsdtprevinicio']);
	}
	public function getDataPrevisaoDeTermino(){
		return new DateTime($this->arAtributos['odsdtprevtermino']);
	}
	public function getIdDocumento(){
		return $this->arAtributos['docid'];
	}
	public function getIdPai(){
		return $this->arAtributos['odsidpai'];
	}
	public function getQtdePfDetalhada(){
		return $this->arAtributos['odsqtdpfdetalhada'];
	}
	public function getContratada(){
		return $this->arAtributos['odscontratada'];
	}
	public function getIdPf(){
		return $this->arAtributos['odsidpf'];
	}
	public function getIdDocumentoPf(){
		return $this->arAtributos['docidpf'];
	}
	public function getIdTipoOrdemServico(){
		return $this->arAtributos['tosid'];
	}
	public function getIdTipoExecucao(){
		return $this->arAtributos['tpeid'];
	}
	public function getQtdePfFinal(){
		return $this->arAtributos['odsqtdpffinal'];
	}
	public function getEnderecoSvn(){
		return $this->arAtributos['odsenderecosvn'];
	}
	public function getSubTotalPf(){
		return $this->arAtributos['odssubtotalpf'];
	}
	public function getSubTotalPfDetalhada(){
		return $this->arAtributos['odssubtotalpfdetalhada'];
	}
	public function getIdContrato(){
		return $this->arAtributos['ctrid'];
	}
	public function getIdMemorando(){
		return $this->arAtributos['memoid'];
	}
	public function getIdGlosa(){
		return $this->arAtributos['glosaid'];
	}
	public function getPorcentagemDisciplina(){
		return $this->porcentagemDisciplina;
	}
	public function getValorUnitarioDePf(){
		return $this->valorUnitarioDePf;
	}
	public function getValorAReceberDaOs(){
		return $this->valorAReceberDaOs;
	}
	public function getStatusOrdemServico(){
		return $this->statusOrdemServico;
	}
	public function getValorAReceberGlosado(){
		if ($this->possuiGlosa()) {
			$glosa = new Glosa();
			$glosa = $glosa->recupereGlosaPeloId($this->getIdGlosa());
			$valorAReceberGlosado = 
				($this->getQtdePfDetalhada() * $this->getPorcentagemDisciplina() - $glosa->getValorEmPf()) * $this->getValorUnitarioDePf(); 
			return $valorAReceberGlosado;
		}
		return null;
	}

	private function montaObjeto($linha){
		$os = new OrdemServico();
		$os->setId($linha['odsid']);
		$os->setIdSolicitacaoServico($linha['scsid']);
		$os->setDetalhamento($linha['odsdetalhamento']);
		$os->setDataPrevisaoDeInicio($linha['odsdtprevinicio']);
		$os->setDataPrevisaoDeTermino($linha['odsdtprevtermino']);
		$os->setIdDocumento($linha['docid']);
		$os->setIdPai($linha['odsidpai']);
		$os->setQtdePfDetalhada($linha['odsqtdpfdetalhada']);
		$os->setContratada($linha['odscontratada']);
		$os->setIdPf($linha['odsidpf']);
		$os->setIdDocumentoPf($linha['docidpf']);
		$os->setIdTipoOrdemServico($linha['tosid']);
		$os->setIdTipoExecucao($linha['tpeid']);
		$os->setQtdePfFinal($linha['odsqtdpffinal']);
		$os->setEnderecoSvn($linha['odsenderecosvn']);
		$os->setSubTotalPf($linha['odssubtotalpf']);
		$os->setSubTotalPfDetalhada($linha['odssubtotalpfdetalhada']);
		$os->setIdContrato($linha['ctrid']);
		$os->setPorcentagemDisciplina($linha['porcentagem_disciplinas']);
		$os->setValorUnitarioDePf($linha['valor_pf']);
		$os->setValorAReceberDaOs($linha['valor_a_receber']);
		$os->setStatusOrdemServico($linha['esdid']);
		$os->setIdGlosa($linha['glosaid']);
		return $os;
	}

	public function recuperePorId($idOrdemServico){
		$sql = "SELECT *
			FROM fabrica.ordemservico os
			JOIN workflow.documento d
				ON d.docid = os.docid 
			WHERE odsid = $idOrdemServico";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}

	public function recupereTodosQueEstaoEmMemorando($idMemorando){
		$where = array("memoid = $idMemorando");
		$resultSet = parent::recuperarTodos($this->stTodosOsCampos, $where, "odsid");
		foreach ($resultSet as $linha){
			$ordensServico[] = $this->montaObjeto($linha);
		}
		return $ordensServico;
	}

	public function passivelDeGlosa(){
		$passivel = false;
		$statusAtual = $this->getStatusOrdemServico();
		switch ($statusAtual) {
			case StatusOrdemServico::EM_AVALIACAO:
				$passivel = true;
				break;
			case StatusOrdemServico::EM_APROVACAO:
				$passivel = true;
				break;
			case StatusOrdemServico::AGUARDANDO_HOMOLOGACAO_GESTOR:
				$passivel = true;
				break;
		}
		return $passivel;
	}

	public function possuiGlosa(){
		return $this->getIdGlosa() != null;
	}

	public function recupereOsCanditadaParaMemorandoPeloId($idMemorando){
		$where = null;
		$resultSet = parent::recuperarTodos($this->stTodosOsCampos, $where, "odsid");
		foreach ($resultSet as $linha){
			$ordensServico[] = $this->montaObjeto($linha);
		}
		return $ordensServico;
	}

	public function recupereOsCanditadaParaMemorandoPeloIdDaEmpresaDeAuditoria($idMemorando){
		return $this->recupereOsCanditadaParaMemorandoPeloId(null);
	}

	public function recupereTodasOsCandidatasParaMemorando(){
		return $this->recupereOsCanditadaParaMemorandoPeloId(null);
	}

	public function recupereTodasOsCandidatasParaMemorandoDaEmpresaDeAuditoria(){
		return $this->recupereOsCanditadaParaMemorandoPeloId(null);
	}

}