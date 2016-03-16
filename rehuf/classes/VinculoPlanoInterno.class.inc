<?php
	
class VinculoPlanoInterno extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.vinculoplanointerno";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "vpiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'vpiid' => null,
    									'pliid' => null, 
    									'pltid' => null,
									  	'prtid' => null, 
									  	'vpidtinicio' => null, 
									  	'vpidttermino' => null, 
									  	'plitid' => null,
    									'arqid' => null
									  );
									  
									  
	public function buscarplanointerno($vpiid){

			$sql = "SELECT 
							pli.*,
							plt.*,
							prt.*,
							vpiq.*,
							vpi.*,
							arq.arqnome
					FROM 
						{$this->stNomeTabela} vpi 
				   INNER JOIN 
						rehuf.planointerno pli ON vpi.pliid = pli.pliid
					LEFT JOIN 
						rehuf.planotrabalho plt ON vpi.pltid = plt.pltid
					LEFT JOIN
						rehuf.portaria prt ON vpi.prtid = prt.prtid
					LEFT JOIN 
						rehuf.vinculointernoquestionario vpiq ON vpi.vpiid = vpiq.vpiid
					LEFT JOIN
						public.arquivo arq ON arq.arqid = vpi.arqid AND arq.arqstatus = 'A'
				   	WHERE 
				   		vpi.vpiid = " .$vpiid;
			$arrDados = $this->pegaLinha($sql);
		return $arrDados ? $arrDados : array();
	}

	public function salvarQuestionarioPorVinculoInterno($vpiid,$arrQuestionario)
	{
		$sql = "delete from rehuf.vinculointernoquestionario where vpiid = $vpiid;";
		if($arrQuestionario[0])
		{
			foreach($arrQuestionario as $queid)
			{
				$sql.="insert into rehuf.vinculointernoquestionario (vpiid,queid) values ($vpiid,$queid);";
			}
		}
		$this->executar($sql);
		$this->commit($sql);
	}
	
	public function removerArquivo($vpiid,$arqid)
	{
		
		$sql1 = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = $arqid;";
		$this->executar($sql1);
		$this->commit($sql1);
	
		$sql = "UPDATE {$this->stNomeTabela} SET arqid = null where vpiid = $vpiid;";
		$this->executar($sql);
		$this->commit($sql);
	}
	
	public function buscarplanointernoByPortariaORPlanoTrabalho($prcug,$prtid,$pltid){
		if($prtid){
			$where = " 
						AND 
					   		prt.prtid = {$prtid}";
		}else{
			$where = " 
						AND 
					   		plt.pltid = {$pltid}";
		}

			$sql = "SELECT DISTINCT
							pli.*,
							plt.*,
							prt.*,
							vpi.*,
							prc.prcid,
							arq.arqnome,
							prc.docid
					FROM 
						{$this->stNomeTabela} vpi 
				   LEFT JOIN 
						rehuf.planointerno pli ON vpi.pliid = pli.pliid
					LEFT JOIN 
						rehuf.planotrabalho plt ON vpi.pltid = plt.pltid
					LEFT JOIN
						rehuf.portaria prt ON vpi.prtid = prt.prtid
					LEFT JOIN
						public.arquivo arq ON arq.arqid = vpi.arqid AND arq.arqstatus = 'A'
					LEFT JOIN 
						rehuf.prestacaocontas prc ON vpi.vpiid = prc.vpiid
					WHERE
						prcug = '$prcug'".$where;
			$arrDados = $this->pegaLinha($sql);
		return $arrDados ? $arrDados : array();
	}
	
	public function buscarVinculoPlanoInterno($prtid,$pltid){
		if($prtid){
			$where = " 
						AND 
					   		prtid = {$prtid}";
		}else{
			$where = " 
						AND 
					   		pltid = {$pltid}";
		}

			$sql = "SELECT
							vpi.vpiid
					FROM 
						{$this->stNomeTabela} vpi 
					WHERE 1=1 ".$where;
			return $this->pegaUm($sql);;
	}
	
	public function buscarQuestionarioPlanoInterno($vpiid){
		$sql = "SELECT
						vpqid
				from
					rehuf.vinculointernoquestionario
			   where 
			   		vpiid = $vpiid";
		return $this->carregarColuna($sql);
	}

}