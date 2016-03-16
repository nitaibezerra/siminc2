<?php 
class SolicitacaoRepositorio extends Modelo {
	
	private $sistemaRepositorio;
	private $requisitanteRepositorio;
	private $situacaoSolicitacaoRepositorio;
	private $servicoFaseProdutoRepositorio;
	private $analiseSolicitacaoRepositorio;
	private $auditoriaRepositorio;
	private $ordemServico;
	private $fiscalRepositorio;
	
	function __construct(){
		parent::__construct();
		$this->sistemaRepositorio = new SistemaRepositorio();
		$this->requisitanteRepositorio = new RequisitanteRepositorio();
		$this->situacaoSolicitacaoRepositorio = new SituacaoSolicitacaoRepositorio();
		$this->servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();
		$this->analiseSolicitacaoRepositorio = new AnaliseSolicitacaoRepositorio();
		$this->auditoriaRepositorio = new AuditoriaRepositorio();
		$this->ordemServico = new OrdemServico();
		$this->fiscalRepositorio = new FiscalRepositorio();
	}
	
	private function getTabelaSQLSolicitacoesPassiveisAuditoria(){
		return "SELECT DISTINCT ss.scsid, ss.odsidorigem, 
					ss.prgid, ss.prgcod, ss.prgano, ss.usucpfrequisitante, u.usunome,
					ss.docid, ss.scsnecessidade, ss.scsjustificativa, ss.scsprevatendimento, 
					ss.scsstatus, ss.scsatesto, ss.scsdtatesto, ss.usucpforigem, ss.sidid, sd.siddescricao,
					ss.dataabertura, est.esdid, est.esddsc,
					ans.ansid, data_finalizacao.data_finalizacao,
					produtoscontratados.quantidadecontratados,
					produtosauditados.quantidadeauditados,
					estadoOS.esdid as estadoOsItemUmId,
					estadoOS.esddsc as estadoOSItemUmDescricao,
					auditoria.usucpf as cpfauditor
				FROM fabrica.solicitacaoservico ss 
				JOIN fabrica.analisesolicitacao ans 
					ON ss.scsid = ans.scsid 
				JOIN fabrica.servicofaseproduto sfp 
					ON sfp.ansid = ans.ansid 
				JOIN fabrica.fasedisciplinaproduto fdp 
					ON sfp.fdpid = fdp.fdpid 
				JOIN fabrica.produto p 
					ON fdp.prdid = p.prdid 
				JOIN fabrica.disciplina dis 
					ON p.dspid = dis.dspid 
				JOIN workflow.documento doc 
					ON doc.docid = ss.docid 
				JOIN workflow.estadodocumento est 
					ON est.esdid = doc.esdid
				JOIN demandas.sistemadetalhe sd
					ON ss.sidid = sd.sidid
				JOIN seguranca.usuario u
					ON ss.usucpfrequisitante = u.usucpf
				JOIN (	SELECT os.scsid, esdc.esdid, esdc.esddsc
					FROM fabrica.ordemservico os
					JOIN workflow.documento d
						ON os.docid = d.docid
					JOIN workflow.estadodocumento esdc
						ON d.esdid = esdc.esdid
					WHERE os.tosid = 1) as estadoOS
					ON estadoOS.scsid = ss.scsid
				JOIN (
					SELECT count (*) as quantidadecontratados, scsid
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
						WHERE pro.prdid <> 44 
							and sfp.tpeid = 1 GROUP BY scsid) as produtoscontratados
					ON produtoscontratados.scsid = ss.scsid
				LEFT JOIN (SELECT count(*) as quantidadeauditados, tmp.scsid FROM (SELECT DISTINCT
							fdp.prdid,
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
							sfp.sfpnecessario,
							sss.scsid
						FROM fabrica.fasedisciplinaproduto fdp
						JOIN fabrica.fasedisciplina fd
							ON fdp.fsdid = fd.fsdid
						JOIN fabrica.produto pro
							ON fdp.prdid = pro.prdid
						JOIN fabrica.disciplina dis
							ON dis.dspid = fd.dspid
						JOIN fabrica.servicofaseproduto sfp
							ON sfp.fdpid = fdp.fdpid
                        JOIN fabrica.detalhesauditoria deta 
                            ON sfp.sfpid = deta.sfpid
                            AND deta.dtaresultado <> 0
						JOIN fabrica.analisesolicitacao ans
							ON ans.ansid = sfp.ansid
						JOIN fabrica.solicitacaoservico sss
							ON ans.scsid = sss.scsid
						JOIN fabrica.detalhesauditoria dtaud
							ON dtaud.sfpid = sfp.sfpid
						WHERE pro.prdid <> 44 and ans.scsid = sss.scsid and sfp.tpeid = 1) as tmp GROUP BY scsid) as produtosauditados 
					ON produtosauditados.scsid = ss.scsid
				LEFT JOIN ( SELECT sss.scsid, htddata AS data_finalizacao 
							FROM fabrica.solicitacaoservico sss 
							JOIN workflow.documento d 
								ON d.docid = sss.docid 
							JOIN workflow.historicodocumento hsd 
								ON d.docid = hsd.docid 
							JOIN workflow.acaoestadodoc aed 
								ON hsd.aedid = aed.aedid 
							WHERE aed.esdidorigem = 252 
								AND aed.esdiddestino = 253 
							ORDER BY htddata DESC LIMIT 1) as data_finalizacao
					ON ss.scsid = data_finalizacao.scsid
				LEFT JOIN (SELECT ansid, usucpf FROM fabrica.auditoria) as auditoria
					ON auditoria.ansid = ans.ansid
				WHERE 
					est.esdid IN (" . 
						StatusSolicitacaoServico::AGUARDANDO_PAGAMENTO . "," . 
						StatusSolicitacaoServico::FINALIZADA . "," .
						StatusSolicitacaoServico::EM_EXECUCAO .
					") 
					AND estadoOS.esdid IN (" .
						StatusOrdemServico::EM_AVALIACAO . "," .
						StatusOrdemServico::AGUARDANDO_HOMOLOGACAO_GESTOR . "," .
						StatusOrdemServico::FINALIZADA . "," .
						StatusOrdemServico::EM_APROVACAO .
					")";
	}
	
	private function getTabelaSQL(){
		return "SELECT DISTINCT ss.scsid, ss.odsidorigem, 
					ss.prgid, ss.prgcod, ss.prgano, ss.usucpfrequisitante, u.usunome,
					ss.docid, ss.scsnecessidade, ss.scsjustificativa, ss.scsprevatendimento, 
					ss.scsstatus, ss.scsatesto, ss.scsdtatesto, ss.usucpforigem, ss.sidid, sd.siddescricao,
					ss.dataabertura, est.esdid, est.esddsc,
					ans.ansid, data_finalizacao.data_finalizacao,
					produtoscontratados.quantidadecontratados,
					produtosauditados.quantidadeauditados,
					estadoOS.esdid as estadoOsItemUmId,
					estadoOS.esddsc as estadoOSItemUmDescricao,
					auditoria.usucpf as cpfauditor
				FROM fabrica.solicitacaoservico ss 
				JOIN fabrica.analisesolicitacao ans 
					ON ss.scsid = ans.scsid 
				JOIN fabrica.servicofaseproduto sfp 
					ON sfp.ansid = ans.ansid 
				JOIN fabrica.fasedisciplinaproduto fdp 
					ON sfp.fdpid = fdp.fdpid 
				JOIN fabrica.produto p 
					ON fdp.prdid = p.prdid 
				JOIN fabrica.disciplina dis 
					ON p.dspid = dis.dspid 
				JOIN workflow.documento doc 
					ON doc.docid = ss.docid 
				JOIN workflow.estadodocumento est 
					ON est.esdid = doc.esdid
				JOIN demandas.sistemadetalhe sd
					ON ss.sidid = sd.sidid
				JOIN seguranca.usuario u
					ON ss.usucpfrequisitante = u.usucpf
				JOIN (	SELECT os.scsid, esdc.esdid, esdc.esddsc
					FROM fabrica.ordemservico os
					JOIN workflow.documento d
						ON os.docid = d.docid
					JOIN workflow.estadodocumento esdc
						ON d.esdid = esdc.esdid
					WHERE os.tosid = 1) as estadoOS
					ON estadoOS.scsid = ss.scsid
				JOIN (
					SELECT count (*) as quantidadecontratados, scsid
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
						WHERE pro.prdid <> 44 
							and sfp.tpeid = 1 GROUP BY scsid) as produtoscontratados
					ON produtoscontratados.scsid = ss.scsid
				LEFT JOIN (SELECT count(*) as quantidadeauditados, tmp.scsid FROM (SELECT DISTINCT
							fdp.prdid,
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
							sfp.sfpnecessario,
							sss.scsid
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
						JOIN fabrica.solicitacaoservico sss
							ON ans.scsid = sss.scsid
						JOIN fabrica.detalhesauditoria dtaud
							ON dtaud.sfpid = sfp.sfpid
						WHERE pro.prdid <> 44 and ans.scsid = sss.scsid and sfp.tpeid = 1) as tmp GROUP BY scsid) as produtosauditados 
					ON produtosauditados.scsid = ss.scsid
				LEFT JOIN ( SELECT sss.scsid, htddata AS data_finalizacao 
							FROM fabrica.solicitacaoservico sss 
							JOIN workflow.documento d 
								ON d.docid = sss.docid 
							JOIN workflow.historicodocumento hsd 
								ON d.docid = hsd.docid 
							JOIN workflow.acaoestadodoc aed 
								ON hsd.aedid = aed.aedid 
							WHERE aed.esdidorigem = 252
								AND aed.esdiddestino = 253 
							ORDER BY htddata DESC LIMIT 1) as data_finalizacao
					ON ss.scsid = data_finalizacao.scsid
				LEFT JOIN (SELECT ansid, usucpf FROM fabrica.auditoria) as auditoria
					ON auditoria.ansid = ans.ansid";
	}
	
	public function recupereQtdeRegistrosPassiveisAuditoria(){
		$cpf = $_SESSION['usucpf'];
		$sql = "SELECT count(scsid) as qtde FROM ({$this->getTabelaSQLSolicitacoesPassiveisAuditoria()}) as solicitacao where cpfauditor = '$cpf' OR cpfauditor is null ";
		return parent::pegaUm($sql);
	}
	
	public function recupereTodos( $orderBy = 'scsid', $ordem = Ordenador::ORDEM_PADRAO, 
		$limit = Paginador::LIMIT_PADRAO, $offset = Paginador::OFFSET_PADRAO ){
		$sql = "SELECT * FROM ({$this->getTabelaSQL()} ORDER BY $orderBy LIMIT $limit OFFSET $offset) as solicitacao ";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recupereTodosPassiveisAuditoria( $orderBy = 'scsid', $ordem = Ordenador::ORDEM_PADRAO, 
		$limit = Paginador::LIMIT_PADRAO, $offset = Paginador::OFFSET_PADRAO, $where = '' ){
			
		$sql = "SELECT * 
				FROM ({$this->getTabelaSQLSolicitacoesPassiveisAuditoria()} 
				ORDER BY $orderBy LIMIT $limit OFFSET $offset) as solicitacao ";
		
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
	
	public function recuperePorId($idSolicitacao){
		$sql = "SELECT * FROM ({$this->getTabelaSQL()}) as solicitacao
				WHERE scsid = $idSolicitacao";
		try {
			$linha = parent::pegaLinha($sql);
			$situacao= $this->montaObjeto($linha);
			return $situacao;
		} catch (Exception $e){
			return null;
		}
	}
	
	public function recuperePorIdDaAnaliseSolicitacao($idAnaliseSolicitacao){
		$sql = "SELECT * FROM ({$this->getTabelaSQL()}) as solicitacao
				WHERE ansid = $idAnaliseSolicitacao";
		try {
			$linha = parent::pegaLinha($sql);
			$situacao= $this->montaObjeto($linha);
			return $situacao;
		} catch (Exception $e){
			return null;
		}
	}
	
	private function montaObjeto($linha){
		$solicitacao = new Solicitacao();
		$solicitacao->setId($linha['scsid']);
		
		$sistema = new Sistema();
		$sistema->setId($linha['sidid']);
		$sistema->setDescricao($linha['siddescricao']);
		$solicitacao->setSistema($sistema);
		
		$requisitante = new Requisitante();
		$requisitante->setCpf($linha['usucpfrequisitante']);
		$requisitante->setNome($linha['usunome']);
		$solicitacao->setRequisitante($requisitante);
		
		$estadoDocumento = new EstadoDocumento();
		$estadoDocumento->setId($linha['esdid']);
		$estadoDocumento->setDescricao($linha['esddsc']);
		$solicitacao->setEstadoDocumento($estadoDocumento);
		
		$solicitacao->setQuantidadeProdutosContratados($linha['quantidadecontratados']);
		$solicitacao->setQuantidadeProdutosAuditados($linha['quantidadeauditados']);
		
		$solicitacao->setDataAbertura($linha['dataabertura']);
		$solicitacao->setDataFinalizacao($linha['data_finalizacao']);
		
		$analiseSolicitacao = new AnaliseSolicitacao();
		$analiseSolicitacao->setId($linha['ansid']);
		$solicitacao->setAnaliseSolicitacao($analiseSolicitacao);
		
		$auditoria = $this->auditoriaRepositorio->recuperePeloIdSolicitacao($linha['scsid']);
		if ($auditoria!=null){
			$solicitacao->setAuditoria($auditoria);
		}
		
		$ordemServicoItemUm = $this->ordemServico->recuperaOrdemServicoItemUmPorIdSolicitacao($linha['scsid']);
		$solicitacao->setOrdemServicoItemUm($ordemServicoItemUm);
		
		$fiscal = $this->fiscalRepositorio->recuperePorId($linha['cpfauditor']);
		$solicitacao->setFiscal($fiscal);
		
		return $solicitacao;
	}

	
}
