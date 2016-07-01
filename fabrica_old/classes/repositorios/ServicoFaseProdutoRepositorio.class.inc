<?php 
class ServicoFaseProdutoRepositorio extends Modelo {
	
	private $faseDisciplinaProdutoRepositorio;
	
	public function __construct(){
		parent::__construct();
		$this->faseDisciplinaProdutoRepositorio = new FaseDisciplinaProdutoRepositorio();
	}
	
	public function recuperePorId($idServicoFaseProduto){
		$sql = "select sfpid, fdpid, ansid, tpeid, sfprepositorio, sfpadraonome, sfppadraodiretorio, sfpencontrado, sfpatualizado, sfpnecessario
				from fabrica.servicofaseproduto where sfpid = $idServicoFaseProduto";
		$linha = parent::pegaLinha($sql);
		$servicoFaseProduto = $this->montaObjeto($linha);
		return $servicoFaseProduto;
	}
	
	public function recuperePorAnaliseSolicitacaoTipoExecucao( $idAnaliseSolicitacao, $idTipoExecucao ){
		$sql = "select sfpid, fdpid, ansid, tpeid, sfprepositorio, sfpadraonome, sfppadraodiretorio, sfpencontrado, sfpatualizado, sfpnecessario
				from fabrica.servicofaseproduto where ansid = $idAnaliseSolicitacao AND tpeid = $idTipoExecucao";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recuperePeloIdAnaliseSolicitacaoIdFaseDisciplinaProduto( $idAnaliseSolicitacao, $idFaseDisciplinaProduto ) {
		$sql = "SELECT sfpid, fdpid, ansid, tpeid, sfprepositorio, sfpadraonome, sfppadraodiretorio, sfpencontrado, sfpatualizado, sfpnecessario
				FROM fabrica.servicofaseproduto WHERE ansid = $idAnaliseSolicitacao and fdpid = $idFaseDisciplinaProduto";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	public function verificaSeFoiAuditado( $idServicoFaseProduto ){
		$sql = "SELECT count(*) FROM fabrica.detalhesauditoria WHERE sfpid = $idServicoFaseProduto";
		return (bool) parent::pegaUm($sql);
	}
	
	public function verificaSeExistePorId( $idServicoFaseProduto ){
		$sql = "SELECT count(*) FROM fabrica.servicofaseproduto WHERE sfpid = $idServicoFaseProduto";
		return (bool) parent::pegaUm($sql);
	}
	
	public function verificaSeExistePorIdAnaliseSolicitacaoIdFaseDisciplinaProduto( $idAnaliseSolicitacao, $idFaseDisciplinaProduto ){
		$sql = "SELECT count(*) FROM fabrica.servicofaseproduto WHERE ansid = $idAnaliseSolicitacao AND fdpid = $idFaseDisciplinaProduto";
		return (bool) parent::pegaUm($sql);
	}
	
	public function recupereContratadosOrdenadosPorFaseDisciplinaPeloIdSolicitacao($idSolicitacao){
		$sql = "SELECT fdp.prdid,
					pro.prddsc,
					pro.prdstatus,
					fdp.fdpid,
					fd.fsddsc,
					fd.fsdid,
					sfp.sfpid,
					dis.dspid,
					sfp.sfprepositorio,
					sfp.sfpadraonome,
					sfp.sfppadraodiretorio,
					sfp.sfpencontrado,
					sfp.sfpatualizado,
					sfp.sfpnecessario
				FROM fabrica.fasedisciplinaproduto fdp
				JOIN fabrica.fasedisciplina fd
					ON fdp.fsdid = fd.fsdid
				JOIN fabrica.produto pro
					ON fdp.prdid = pro.prdid
				JOIN fabrica.disciplina dis
					ON dis.dspid = fd.dspid
				JOIN fabrica.servicofaseproduto sfp
					ON sfp.fdpid = fdp.fdpid
				JOIN fabrica.analisesolicitacao ans
					ON ans.ansid = sfp.ansid
				WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1
				ORDER BY fd.fsdid";
		
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recupereQtdeProdutosContratadosPeloIdSolicitacao( $idSolicitacao ){
		$sql = "SELECT count(*) as qtde
				FROM fabrica.fasedisciplinaproduto fdp
				JOIN fabrica.fasedisciplina fd
					ON fdp.fsdid = fd.fsdid
				JOIN fabrica.produto pro
					ON fdp.prdid = pro.prdid
				JOIN fabrica.disciplina dis
					ON dis.dspid = fd.dspid
				JOIN fabrica.servicofaseproduto sfp
					ON sfp.fdpid = fdp.fdpid
				JOIN fabrica.analisesolicitacao ans
					ON ans.ansid = sfp.ansid
				WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1";
		
		return parent::pegaUm($sql);
	}
	
	public function recupereCotratadosDaSolicitacao($idSolicitacao, $orderBy = 'fsddsc', $ordem = Ordenador::ORDEM_PADRAO, 
		$limit = Paginador::LIMIT_PADRAO, $offset = Paginador::OFFSET_PADRAO, $where = ''){
			
		$sql = "SELECT * FROM (SELECT fdp.prdid,
					pro.prddsc,
					pro.prdstatus,
					fdp.fdpid,
					fd.fsddsc,
					fd.fsdid,
					sfp.sfpid,
					dis.dspid,
					sfp.ansid,
					sfp.sfprepositorio,
					sfp.sfpadraonome,
					sfp.sfppadraodiretorio,
					sfp.sfpencontrado,
					sfp.sfpatualizado,
					sfp.sfpnecessario
				FROM fabrica.fasedisciplinaproduto fdp
				JOIN fabrica.fasedisciplina fd
					ON fdp.fsdid = fd.fsdid
				JOIN fabrica.produto pro
					ON fdp.prdid = pro.prdid
				JOIN fabrica.disciplina dis
					ON dis.dspid = fd.dspid
				JOIN fabrica.servicofaseproduto sfp
					ON sfp.fdpid = fdp.fdpid
				JOIN fabrica.analisesolicitacao ans
					ON ans.ansid = sfp.ansid
				WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1
				ORDER BY $orderBy LIMIT $limit OFFSET $offset) as produtos";
		
		if ($where != '') {
			$sql .= $where;		
		}
		
		$sql .= " ORDER BY $orderBy $ordem";
		
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recupereTodosCotratadosDaSolicitacao( $idSolicitacao ){
			
		$sql = "SELECT * FROM (SELECT fdp.prdid,
					pro.prddsc,
					pro.prdstatus,
					fdp.fdpid,
					fd.fsddsc,
					fd.fsdid,
					sfp.sfpid,
					dis.dspid,
					sfp.ansid,
					sfp.sfprepositorio,
					sfp.sfpadraonome,
					sfp.sfppadraodiretorio,
					sfp.sfpencontrado,
					sfp.sfpatualizado,
					sfp.sfpnecessario
					FROM fabrica.fasedisciplinaproduto fdp
					JOIN fabrica.fasedisciplina fd
					ON fdp.fsdid = fd.fsdid
					JOIN fabrica.produto pro
					ON fdp.prdid = pro.prdid
					JOIN fabrica.disciplina dis
					ON dis.dspid = fd.dspid
					JOIN fabrica.servicofaseproduto sfp
					ON sfp.fdpid = fdp.fdpid
					JOIN fabrica.analisesolicitacao ans
					ON ans.ansid = sfp.ansid
					WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1
					ORDER BY fd.fsddsc) as produtos";
	
		$sql .= " ORDER BY fsddsc ASC";
	
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recupereCotratadosDaSolicitacaoAgrupados($idSolicitacao){
			
		$sql = "SELECT count(*) FROM (SELECT DISTINCT fdp.prdid,
				pro.prddsc,
				pro.prdstatus,
				dis.dspid,
				dis.dspdsc
				FROM fabrica.fasedisciplinaproduto fdp
				JOIN fabrica.fasedisciplina fd
				ON fdp.fsdid = fd.fsdid
				JOIN fabrica.produto pro
				ON fdp.prdid = pro.prdid
				JOIN fabrica.disciplina dis
				ON dis.dspid = fd.dspid
				JOIN fabrica.servicofaseproduto sfp
				ON sfp.fdpid = fdp.fdpid
				JOIN fabrica.analisesolicitacao ans
				ON ans.ansid = sfp.ansid
				WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1) as produtos";
		return $this->pegaUm($sql);
	}
	
	public function recuperaAuditadosDaSolicitacao($idSolicitacao){
		$sql = "SELECT DISTINCT	fdp.prdid,
					pro.prddsc,
					pro.prdstatus,
					fdp.fdpid,
					fd.fsddsc,
					fd.fsdid,
					sfp.sfpid,
					dis.dspid,
					sfp.sfprepositorio,
					sfp.sfpadraonome,
					sfp.sfppadraodiretorio,
					sfp.sfpencontrado,
					sfp.sfpatualizado,
					sfp.sfpnecessario
				FROM fabrica.fasedisciplinaproduto fdp
				JOIN fabrica.fasedisciplina fd
					ON fdp.fsdid = fd.fsdid
				JOIN fabrica.produto pro
					ON fdp.prdid = pro.prdid
				JOIN fabrica.disciplina dis
					ON dis.dspid = fd.dspid
				JOIN fabrica.servicofaseproduto sfp
					ON sfp.fdpid = fdp.fdpid
				JOIN fabrica.analisesolicitacao ans
					ON ans.ansid = sfp.ansid
				JOIN fabrica.detalhesauditoria dtaud
					ON dtaud.sfpid = sfp.sfpid
				WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1
				ORDER BY fd.fsdid";
		
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}	
	
	public function atualizar($servicoFaseProduto){
		$id = $servicoFaseProduto->getId();
		$repositorio = $servicoFaseProduto->getRepositorio()!='' ? "'" . $servicoFaseProduto->getRepositorio() . "'" : "null";
		
		if($servicoFaseProduto->getPadraoNome()==""){
			$padraoNome = "null";
		} else if ($servicoFaseProduto->getPadraoNome()=="S") {
			$padraoNome = "true";
		} else if ($servicoFaseProduto->getPadraoNome()=="N") {
			$padraoNome = "false";
		}
		
		if($servicoFaseProduto->getPadraoDiretorio()==""){
			$padraoDiretorio = "null";
		} else if ($servicoFaseProduto->getPadraoDiretorio()=="S") {
			$padraoDiretorio = "true";
		} else if ($servicoFaseProduto->getPadraoDiretorio()=="N") {
			$padraoDiretorio = "false";
		}
		
		if($servicoFaseProduto->getEncontrado()==""){
			$encontrado = "null";
		} else if ($servicoFaseProduto->getEncontrado()=="S") {
			$encontrado = "true";
		} else if ($servicoFaseProduto->getEncontrado()=="N") {
			$encontrado = "false";
		}
		
		if($servicoFaseProduto->getAtualizado()==""){
			$atualizado = "null";
		} else if ($servicoFaseProduto->getAtualizado()=="S") {
			$atualizado = "true";
		} else if ($servicoFaseProduto->getAtualizado()=="N") {
			$atualizado = "false";
		}
		
		if($servicoFaseProduto->getNecessario()==""){
			$necessario = "null";
		} else if ($servicoFaseProduto->getNecessario()=="S") {
			$necessario = "true";
		} else if ($servicoFaseProduto->getNecessario()=="N") {
			$necessario = "false";
		}
		
		$sql = "UPDATE fabrica.servicofaseproduto SET sfprepositorio = $repositorio,
			sfpadraonome = $padraoNome, sfppadraodiretorio = $padraoDiretorio, 
			sfpencontrado = $encontrado, sfpatualizado = $atualizado, sfpnecessario = $necessario WHERE sfpid = $id";

		return parent::executar($sql) && parent::commit();
	}
	
	private function montaObjeto($linha){
		$servicoFaseProduto = new ServicoFaseProduto();
		$servicoFaseProduto->setId($linha['sfpid']);
		
		$analiseSolicitacao = new AnaliseSolicitacao();
		$analiseSolicitacao->setId($linha['ansid']);
		$servicoFaseProduto->setAnaliseSolicitacao($analiseSolicitacao);
		
		$servicoFaseProduto->setAtualizado($linha['sfpatualizado']);
		$servicoFaseProduto->setEncontrado($linha['sfpencontrado']);
		
		$faseDisciplinaProduto = $this->faseDisciplinaProdutoRepositorio->recuperePorId($linha['fdpid']);
		$servicoFaseProduto->setFaseDisciplinaProduto($faseDisciplinaProduto);
		
		$servicoFaseProduto->setNecessario($linha['sfpnecessario']);
		$servicoFaseProduto->setPadraoDiretorio($linha['sfppadraodiretorio']);
		$servicoFaseProduto->setPadraoNome($linha['sfpadraonome']);
		$servicoFaseProduto->setRepositorio($linha['sfprepositorio']);
		
		$tipoExecucao = new TipoExecucao();
		$tipoExecucao->setId($linha['tpeid']);
		$servicoFaseProduto->setTipoExecucao($tipoExecucao);
		
		return $servicoFaseProduto;
	}
}
