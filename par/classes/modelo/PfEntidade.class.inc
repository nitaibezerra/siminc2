<?php
	
class PfEntidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.pfentidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pfeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'pfeid' => null, 
									  	'entid' => null, 
									  	'adpid' => null,
    									'tvpid' => null, 
									  	'pffid' => null,
									  	'tfoid' => null,
    									'pfefuncao' => null,
    									'pfevinculo' => null
									  );
									  
	public function recuperarPorAdpid($adpid, $funid, $pfeid = null)
	{
		$sql = "SELECT * FROM {$this->stNomeTabela} WHERE adpid = {$adpid}";
		if($pfeid > 0){
			$stWhere = "AND pfe.pfeid = {$pfeid}";
		}
		
		$sql = "SELECT DISTINCT
					pfe.*
				FROM par.pfentidade pfe
					INNER JOIN entidade.entidade 		ent ON ent.entid = pfe.entid 
					LEFT JOIN entidade.endereco 		eed ON eed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade 	efu ON efu.entid = ent.entid AND efu.funid = {$funid}
					INNER JOIN entidade.funcao 			fun ON fun.funid = efu.funid										
				WHERE pfe.adpid = {$adpid}
				{$stWhere}
				order by 1 desc
				";
		if($pfeid > 0 && $adpid > 0 && $funid > 0 || $funid == FUNID_COORDENADOR_PRO_FUNCIONARIO){
			return $this->pegaLinha($sql);
		}
	}
	
	public function recuperaEntfunassoc($funid, $entid)
	{
		$sql = "SELECT
					ent.entnome,
					ent2.entnome as nome				
				FROM entidade.entidade ent
					INNER JOIN entidade.endereco 		eed ON eed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid} AND fue.fuestatus = 'A'
					INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
					INNER JOIN entidade.funentassoc 	fea ON fue.fueid = fea.fueid
					INNER JOIN entidade.entidade		ent2 ON ent2.entid = fea.entid
				WHERE ent.entstatus = 'A'
				--AND ent.entnumcpfcnpj = ''
				AND ent.entid = {$entid}";
		
		return $this->carregar($sql);		
	}
	
	public function limpaEntidadeAssocPorEntid($entid)
	{
		$sql = "select 
					feaid 
				from entidade.entidade ent
				inner join entidade.funcaoentidade 	fue on fue.entid = ent.entid
				inner join entidade.funentassoc 	fea on fea.fueid = fue.fueid
				where ent.entid = '{$entid}'";
		
		$arDados = $this->carregar($sql);
		
		$arDados = $arDados ? $arDados : array();
		
		foreach($arDados as $dados){
			$arFeaid[] = $dados['feaid']; 	
		}
		
		if(count($arFeaid)){
			$sql = "delete from entidade.funentassoc where feaid in (".implode(', ', $arFeaid).")";			
			$this->executar($sql);
			$this->commit();
		}
	}
	
	public function verificaEntidadeExiste($entid, $adpid, $funid)
	{
		$sql = "SELECT 
					pfe.entid 
				FROM {$this->stNomeTabela} pfe
				INNER JOIN entidade.entidade ent ON ent.entid = pfe.entid
				INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid = {$funid} 
				WHERE pfe.entid = {$entid} 
				AND pfe.adpid = {$adpid}";
		
		return $this->pegaUm($sql);
	}
}