<?php
	
class Escolamunpse2009 extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.escolamunpse2009";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "esmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'esmid' => null, 
									  	'empid' => null, 
									  	'entid' => null, 
									  );
	function carregarEscolas()
	{/*
		$sql = "SELECT esp.espid as codigo, ent.entnome as descricao 
				FROM 
					$this->stNomeTabela as esm, entidade.entidade  ent, pse.escolapse esp
				WHERE 
					esm.espid =  esp.espid AND
					ent.entid = esp.entid ";
		*/
		
		
		$sql = "SELECT distinct esm.entid as codigo, ent.entnome as descricao 
				FROM 
					$this->stNomeTabela as esm, entidade.entidade  ent
				WHERE 
					esm.entid =  ent.entid";
					
		if($this->empid)						
				$sql .= " AND esm.empid = {$this->empid} order by ent.entnome";
		
/*
		$sql = "select distinct
    					e.entid,
                		e.entnome
			     from entidade.entidade e
			     inner join entidade.endereco en on e.entid = en.entid and en.tpeid = 1 
			     inner join entidade.funcaoentidade fe ON fe.entid = e.entid
			     inner join territorios.municipio m on m.muncod = en.muncod 
			     where (e.entescolanova = false or e.entescolanova is null) 
			     and e.entnome <> '' 
			     and fe.funid = 3
			     and e.tpcid = 1
			     and  m.estuf = '{$_SESSION["uf"]}'
				 group by
                	e.entid,
                	e.entnome,
                	m.mundescricao 
                order by
                	m.mundescricao, 
                	e.entnome"
		*/
		$rs = $this->carregar($sql);
		
		$matriz = array();
		if($rs)
		{
			foreach($rs as $value)
			{
				$matriz[] = array("codigo"=> $value["codigo"],"descricao"=> $value["descricao"]);
			}
			
		}
		
		if(count($matriz) == 0)
			$matriz = null;
		
		return $matriz;
		
		
	}	
	
	function carregarEscolasPadrao($uf, $muncod){

/*
			$sql = "SELECT 
					esp.espid as codigo,  ent.entnome as descricao
				FROM
					pse.escolapse esp, entidade.entidade  ent, entidade.entidade2 ent2, entidade.entidadedetalhe entd, entidade.endereco ende,territorios.municipio ter  
				
				WHERE 
					ent.entid = esp.entid	AND
					ent.entid=ent2.entid AND 
					ent.entid=entd.entid AND
					ent.entid=ende.entid AND
					ende.muncod=ter.muncod AND
					ent.entstatus='A' AND 
					ent2.funid in(3,4)
					AND ende.estuf = '{$uf}'
					";
 */				
			$sql = "select distinct
						e.entid as codigo,
                		e.entnome as descricao 
			     from entidade.entidade e
			     inner join entidade.endereco en on e.entid = en.entid and en.tpeid = 1 
			     inner join entidade.funcaoentidade fe ON fe.entid = e.entid
			     inner join territorios.municipio m on m.muncod = en.muncod 
			     where (e.entescolanova = false or e.entescolanova is null) 
			     and e.entnome <> '' 
			     and fe.funid = 3
			     and (e.tpcid = 1 or e.tpcid = 3)
			     and  m.estuf = '{$uf}'
				 AND en.muncod='{$muncod}' group by
                	e.entid,
                	e.entnome,
                	m.mundescricao 
                order by
                	e.entnome"	;		
     
      		$rs = $this->carregar($sql);
				
				$matriz = array();
				if($rs)
				{
					foreach($rs as $value)
					{
						$matriz[] = array("codigo"=> $value["codigo"],"descricao"=> $value["descricao"]);
					}
					
				}
				if(count($matriz) == 0)
					$matriz = null;
					
				return $matriz;
	}

	function deletar()
	{
		$sql = "DELETE FROM $this->stNomeTabela WHERE empid = $this->empid";
		
		if($this->executar($sql))
			$this->commit();
		else
			$this->rollback();	
	}
}