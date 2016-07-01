<?php
class DetalhesAuditoriaRepositorio extends Modelo {

	private $servicoFaseProdutoRepositorio;
	private $auditoriaRepositorio;

	protected $stNomeTabela = "fabrica.detalhesauditoria";
	protected $arChavePrimaria = array( "dtaid" );
	protected $arAtributos = array(
	  	'dtaid' => null, 
	  	'sfpid' => null,
	  	'datdata' => null,
		'dtaresultado'=> null,
		'dtamotivo'=> null,
		'dtaobservacao'=> null,
		'audid'=> null
	);
	protected $stCampos = "dtaid, sfpid, datdata, dtaresultado, dtamotivo, dtaobservacao, audid";
	
	public function __construct(){
		parent::__construct();
		$this->servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();
		$this->auditoriaRepositorio = new AuditoriaRepositorio();
	}

	public function salvar($detalhesAuditoria){
		if ($detalhesAuditoria->getId()=='' || $detalhesAuditoria->getId()==null){

			$sfp = $detalhesAuditoria->getServicoFaseProduto() != null ? 
				$detalhesAuditoria->getServicoFaseProduto()->getId() : null;
			$dataAuditoria = $detalhesAuditoria->getDataAuditoria();
			$resultado = $detalhesAuditoria->getResultado() != null ? 
				$detalhesAuditoria->getResultado() : null;
			$motivo = $detalhesAuditoria->getMotivo() != null ?
				$detalhesAuditoria->getMotivo() : null;
			$observacao = $detalhesAuditoria->getObservacao() != null ?
				$detalhesAuditoria->getObservacao() : null;
			$idAuditoria = $detalhesAuditoria->getAuditoria() != null ?
				$detalhesAuditoria->getAuditoria()->getId() : null;
		
			$sql = "INSERT INTO fabrica.detalhesauditoria (sfpid, datdata, dtaresultado, dtamotivo, dtaobservacao, audid)
					VALUES ($sfp, '$dataAuditoria', $resultado, '$motivo', '$observacao', $idAuditoria) returning dtaid";
			 
			$novoId = parent::pegaUm($sql);
			parent::commit();
			return $novoId;
			
		} else {
			$id = $detalhesAuditoria->getId();
			$idServicoFaseProduto = $detalhesAuditoria->getServicoFaseProduto()->getId();
			$data = $detalhesAuditoria->getDataAuditoria();
			$resultado = $detalhesAuditoria->getResultado();
			$motivo = $detalhesAuditoria->getMotivo();
			$observacao = $detalhesAuditoria->getObservacao();
			$idAuditoria = $detalhesAuditoria->getAuditoria()!='' ? $detalhesAuditoria->getAuditoria()->getId() : "null";
			

			$sql = "UPDATE fabrica.detalhesauditoria SET sfpid = $idServicoFaseProduto, datdata = '$data', 
				dtaresultado = $resultado, dtamotivo = '$motivo', dtaobservacao = '$observacao', 
				audid = $idAuditoria WHERE dtaid = $id returning dtaid ";
			
			$novoId = parent::pegaUm($sql);
			parent::commit();
			return $novoId;
		}
	}
	
	public function recuperePorId($idDetalhesAuditoria){
		$sql = "select * from fabrica.detalhesauditoria where dtaid = $idDetalhesAuditoria";
		try {
			$linha = parent::pegaLinha($sql);
			$detalhesAuditoria = $this->montaObjeto($linha);
			return $detalhesAuditoria;
		}catch(Exception $e){
			return null;
		}
	}

	public function recuperePorIdServicoFaseProduto($idServicoFaseProduto){
		$sql = "select * from fabrica.detalhesauditoria where sfpid = $idServicoFaseProduto";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto($linha){
		$detalhesAuditoria = new DetalhesAuditoria();
		$detalhesAuditoria->setId($linha['dtaid']);
		$detalhesAuditoria->setDataAuditoria(DateTimeUtil::retiraMascaraRetornandoObjetoDateTime($linha['datdata']));
		$detalhesAuditoria->setResultado($linha['dtaresultado']);
		$detalhesAuditoria->setMotivo($linha['dtamotivo']);
		$detalhesAuditoria->setObservacao($linha['dtaobservacao']);

		if ($linha['sfpid']!=""){
			$servicoFaseProduto = $this->servicoFaseProdutoRepositorio->recuperePorId($linha['sfpid']);
			$detalhesAuditoria->setServicoFaseProduto($servicoFaseProduto);
		}
		
		if ($linha['audid']!=""){
			$auditoria = $this->auditoriaRepositorio->recuperePorId($linha['audid']);
			$detalhesAuditoria->setAuditoria($auditoria);
		}
		return $detalhesAuditoria;
	}

}