<?php 
class SituacaoAuditoriaRepositorio {

	private $situacoesAuditoria;
	
	public function __construct(){
		$situacaoAuditoria = new SituacaoAuditoria();
		$situacaoAuditoria->setId(1);
		$situacaoAuditoria->setDescricao("Pendente");
		$this->situacoesAuditoria[] = $situacaoAuditoria;
		
		$situacaoAuditoria = new SituacaoAuditoria();
		$situacaoAuditoria->setId(2);
		$situacaoAuditoria->setDescricao("Concluída");
		$this->situacoesAuditoria[] = $situacaoAuditoria;
	}
	
	public function recupereTodos(){
		return $this->situacoesAuditoria;
	}
	
	public function recuperePeloId($idSituacaoAuditoria){
		$situacaoAuditoria = new SituacaoAuditoria();
		if ($idSituacaoAuditoria==1){
			$situacaoAuditoria->setId(1);
			$situacaoAuditoria->setDescricao("Pendente");
		} else if($idSituacaoAuditoria==2){
			$situacaoAuditoria->setId(2);
			$situacaoAuditoria->setDescricao("Concluída");
		}
		return $situacaoAuditoria;
	}
	
}