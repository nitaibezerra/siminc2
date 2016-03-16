<?php
class HistoricoAuditoriaRepositorio extends Modelo {
	
	private $fiscalRepositorio;
	
	public function __construct(){
		parent::__construct();
		$this->fiscalRepositorio = new FiscalRepositorio();
	}
	
	public function recuperePorIdAuditoria( $idAuditoria ){
		$sql = "SELECT
					ha.haid, ha.audid, ha.usucpf, ha.dtamotivohistorico, ha.dtaobservacao, ha.dtaresultado
				FROM fabrica.historicoauditoria ha
				WHERE audid = $idAuditoria";
		
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
		
	}
	
	public function salvar( DetalhesAuditoria $detalhesAuditoria, $ifFiscal, $arrItensAuditoria){
		$sql = "INSERT INTO fabrica.historicoauditoria (audid, usucpf, dtamotivohistorico, dtaobservacao, dtaresultado) 
				VALUES ({$detalhesAuditoria->getAuditoria()->getId()}, '$ifFiscal', 
				'{$detalhesAuditoria->getMotivo()}', '{$detalhesAuditoria->getObservacao()}', {$detalhesAuditoria->getResultado()}) returning haid";
		
		$haid = parent::pegaUm($sql);
		
		if (!empty($arrItensAuditoria)){
			$sql = "INSERT INTO fabrica.historicoitemauditoria (haid, itemid) VALUES ";
			foreach ($arrItensAuditoria as $id){
				$valores[] = "($haid, $id)";
			}
			$sql .=  implode(", ", $valores);
			return parent::executar($sql) && parent::commit();
		}
		
		return (bool) $haid;
	}
	
	private function montaObjeto( $linha ){
		$auditoria = new Auditoria();
		$auditoria->setId($linha['audid']);
		
		$fiscal = $this->fiscalRepositorio->recuperePorId($linha['usucpf']);
		
		$dataAuditoria = new DateTime($linha['hadata']);
		
		$situacaoAuditoria = new SituacaoAuditoria();
		$situacaoAuditoria->setId($linha['dtaresultado']);
		
		$historicoAuditoria = new HistoricoAuditoria();
		$historicoAuditoria->setId($linha['haid']);
		$historicoAuditoria->setAuditoria($auditoria);
		$historicoAuditoria->setFiscal($fiscal);
		$historicoAuditoria->setDataAuditoria($dataAuditoria);
		$historicoAuditoria->setMotivo($linha['dtamotivohistorico']);
		$historicoAuditoria->setObservacao($linha['dtaobservacao']);
		$historicoAuditoria->setSituacaoAuditoria($situacaoAuditoria);
		return $historicoAuditoria;
	}
}