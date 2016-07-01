<?php
	
class PreTipoDocumento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.pretipodocumento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "podid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'podid' => null, 
									  	'poddescricao' => null, 
									  	'podstatus' => null,									  	 
									  );
									  

	public function recuperarTiposObraDocumentos()
	{
		$sql = "SELECT 
					podid as codigo,
					poddescricao as descricao
				FROM {$this->stNomeTabela} 
				WHERE podstatus = 'A'
				ORDER BY podid";
		
		return $this->carregar($sql);
	}
	
	public function recuperarTiposObraDocumentosProInfancia($preid, $tipoObra = null, $tipoA)
	{		
		if($tipoObra){
			$stWhere = "AND tdo.ptoid = {$tipoObra}";
		}
		
		if($tipoObra == 1 && $tipoA == false){
			$stWhere = "AND tob.podid NOT IN (10,11,12,13,14,15,16,17)";
		}
		
		if($tipoObra == 1 && $tipoA == true){
			$stWhere = "AND tob.podid NOT IN (1,3,4,5,6,7,8,9)";
		}
		
		$esfera = $_SESSION['par']['muncod'] ? 'M' : 'E';
		
		$sql = "SELECT 
					tob.podid as codigo,
					tob.poddescricao as descricao,
					--pod.arqid as anexo
					max(pod.arqid) as anexo, 
					max(pod.poasituacao) as situacao,
					tob.podmodelo,
					tob.podajuda
				FROM obras.pretipodocumento tob
				INNER JOIN obras.pretipodocumentoobra tdo ON tdo.podid = tob.podid 
				LEFT JOIN obras.preobraanexo pod ON pod.podid = tob.podid AND pod.preid = {$preid}
				LEFT JOIN obras.preobra pre ON pre.preid = pod.preid 
				WHERE 
					tob.podstatus = 'A'
					AND podesfera in ('T','$esfera')
					AND tob.podid NOT IN ( SELECT DISTINCT 3 FROM obras.preobra WHERE preid = {$preid} AND muncod IS NULL  )
				{$stWhere}
				GROUP BY pod.podid, tob.podid, tob.poddescricao, tob.podmodelo, tob.podajuda
				ORDER BY tob.podid";
//		ver($sql);
		return $this->carregar($sql);
	}

	public function recuperarArTiposObraDocumentos($preid, $tipoObra = null, $tipoA)
	{		
		if($tipoObra){
			$stWhere = "AND tdo.ptoid = {$tipoObra}";
		}
		
		if($tipoObra == 1 && $tipoA == false){
			$stWhere = "AND tob.podid NOT IN (10,11,12,13,14,15,16,17)";
		}
		
		if($tipoObra == 1 && $tipoA == true){
			$stWhere = "AND tob.podid NOT IN (1,3,4,5,6,7,8,9)";
		}
		
		$sql = "SELECT 
					tob.podid as codigo,
					tob.poddescricao as descricao,
					--pod.arqid as anexo
					max(pod.arqid) as anexo, 
					max(pod.poasituacao) as situacao,
					tob.podmodelo,
					tob.podajuda
				FROM obras.pretipodocumento tob
				INNER JOIN obras.pretipodocumentoobra tdo ON tdo.podid = tob.podid 
				LEFT JOIN obras.preobraanexo pod ON pod.podid = tob.podid AND pod.preid = {$preid}
				LEFT JOIN obras.preobra pre ON pre.preid = pod.preid 
				WHERE 
					tob.podstatus = 'A'
					AND tob.podid NOT IN ( SELECT DISTINCT 3 FROM obras.preobra WHERE preid = {$preid} AND muncod IS NULL  )
				{$stWhere}
				GROUP BY pod.podid, tob.podid, tob.poddescricao, tob.podmodelo, tob.podajuda
				ORDER BY tob.podid";
//		ver($sql);
		return $this->carregar($sql);
	}
	
	public function recuperarDocumentosAnexoPorPodid($podid, $preid)
	{
		$sql = "SELECT
					arq.arqid as codigo,
					arq.arqnome as descricao,
					arq.arqextensao as extensao
				FROM obras.preobraanexo anx
				INNER JOIN public.arquivo arq ON arq.arqid = anx.arqid AND anx.poasituacao = 'A'
				WHERE anx.preid = {$preid} AND anx.podid = {$podid}";
				
		return $this->carregar($sql);
	}
	
	public function verificaDocumentoDominialidade($preid)
	{
		$sql = "select 
					poaid 
				from obras.preobraanexo 
				where preid = {$preid} 
				and podid = 9";
		
		return $this->pegaUm($sql);
	}
									  
}