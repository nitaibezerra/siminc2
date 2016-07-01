<?php
	
class OrdemBancariaFatura extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.ordembancariafatura";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "obfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'obfid' => null, 
									  	'ftcid' => null, 
									  	'epsid' => null, 
									  	'obfnumero' => null, 
									  	'obfvalor' => null, 
									  	'usucpf' => null, 
									  	'obfdata' => null, 
									  	'obfdatatransacao' => null, 
    									'obfsiafi' => null,
							    		'ungcod' => null,
							    		'unicod' => null,
									  );
    
	public function pegaValorTotalPorCtrid( $ctrid ){
		$where = array();
		$join  = array();
		
// 		if ( $param['esdid'] ){
// 			$param['esdid']   		 = (array) $param['esdid'];
// 			$where[] 		  		 = "ed.esdid IN (" . implode(", ", $param['esdid']) . ")";
// 			$join['documento'] 		 = "JOIN workflow.documento d ON d.docid = f.docid";
// 			$join['estadodocumento'] = "JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid";
// 		}
		
		$sql = "SELECT
					SUM(a.ftcvalor) AS ftcvalor 	
				FROM( 
					(
						SELECT
		    				SUM(of.obfvalor) AS ftcvalor
		    			FROM
		    				contratos.faturacontrato f
						JOIN contratos.ordembancariafatura of ON of.ftcid = f.ftcid
		    				" . (count($join) ? implode(" ", $join) : "") . "
		    			WHERE
		    				f.ftcstatus = 'A' AND
		    				f.ctrid = {$ctrid}
		    				" . (count($where) ? " AND " . implode(' AND ',$where) : "") . "
					)UNION ALL(
						SELECT
							SUM(obs.valor) AS ftcvalor
						FROM
							contratos.ctcontrato c
						JOIN entidade.entidade e ON e.entid = c.entidcontratada	
						JOIN contratos.hospital h ON h.hspid = c.hspid AND
									     			 h.hspstatus = 'A'
						JOIN contratos.hospitalug hu ON hu.hspid = h.hspid 			     
						JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid			     
						JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
														   es.ungcod = hu.ungcod AND
														   es.co_favorecido = e.entnumcpfcnpj
						JOIN contratos.ob_siafi obs ON obs.empenho = es.nu_empenho AND
												       obs.unidade = hu.ungcod AND
												       obs.it_co_credor = e.entnumcpfcnpj
						WHERE
							c.ctrid = {$ctrid} AND
							obs.ob NOT IN (
											SELECT 
												obf.obfnumero 
											FROM 
												contratos.faturacontrato fc
											JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
											WHERE 
												fc.ftcstatus = 'A' AND
												fc.ctrid = {$ctrid}
											)
							" . (count($where) ? " AND " . implode(' AND ',$where) : "") . "				
					) 
				) a";
		
		$totalValor = $this->pegaUm($sql);
		 
		return ($totalValor ? $totalValor : 0);
	}    
    
    public function salvarOrdemBancaria($ftcid)
    {
    	$arrDados['usucpf'] = $_SESSION['usucpf'];
    	$arrDados['ftcid']  = $ftcid;
    	$arrDados['orbid']  = $_POST['orbid'];
    	
    	$this->apagarTodos($ftcid);
    	
    	if($_POST['obfnumero']){
    		foreach($_POST['obfnumero'] as $chave => $obfnumero){
    			if($obfnumero && $_POST['obfvalor'][$chave] && $_POST['epsid'][$chave]){
    				
    				if(strlen(trim($obfnumero)) == 23){
		    			$arrDados['ungcod'] 	= substr(trim($obfnumero),0,6);	    			
		    			$arrDados['unicod'] 	= substr(trim($obfnumero),6,5);
		    			$arrDados['obfnumero'] 	= substr(trim($obfnumero),11,12);
    				}else{
    					$arrDados['obfnumero'] 	= trim($obfnumero);
    				}
    				
	    			$arrDados['epsid'] 			  = $_POST['epsid'][$chave];
	    			$arrDados['obfdatatransacao'] = $_POST['obfdatatransacao'][$chave];
	    			$arrDados['obfvalor'] 		  = str_replace(array(".",","),array("","."),$_POST['obfvalor'][$chave]);
	    			$arrDados['obfsiafi'] 		  = $_POST['obfsiafi'][$chave];	    
	    						
	    			$ob = new OrdemBancariaFatura();
		    		$ob->popularDadosObjeto($arrDados);
			    	if($ob->salvar()){
			    		$ob->commit();
			    	}
    			}
    		}
    	}else{
    		return true;
    	}
    }
    
    public function getOrdemBancaria($ftcid)
    {
    	$sql = "SELECT 
    				obf.*,
    				es.epsid,
    				es.nu_empenho,
    				es.co_favorecido AS cnpj 
    			FROM 
    				contratos.ordembancariafatura obf
    			JOIN contratos.empenho_siafi es ON es.epsid = obf.epsid
    			WHERE 
    				ftcid = $ftcid
    			ORDER BY
    				obfdatatransacao";
    	return $this->carregar($sql);
    }
    
    public function getOrdemBancariaSiafiPorCtrid( $ctrid )
    {
    	$sql = "SELECT
					obs.ob AS obfnumero,
					obs.empenho AS nu_empenho,
					obs.valor AS obfvalor,
					obs.datatransacao AS obfdatatransacao,
					obs.it_co_credor AS cnpj
				FROM
					contratos.ctcontrato c
				JOIN entidade.entidade e ON e.entid = c.entidcontratada	
				JOIN contratos.hospital h ON h.hspid = c.hspid AND
										 h.hspstatus = 'A'
				JOIN contratos.hospitalug hu ON hu.hspid = h.hspid 
				JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid			     
				JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
												   TRIM(es.ungcod) = TRIM(hu.ungcod) AND
												   TRIM(es.co_favorecido) = TRIM(e.entnumcpfcnpj)
				JOIN contratos.ob_siafi obs ON TRIM(obs.empenho) = TRIM(es.nu_empenho) AND
											   TRIM(obs.unidade) = TRIM(hu.ungcod) AND
											   TRIM(obs.it_co_credor) = TRIM(e.entnumcpfcnpj)
				WHERE
					c.ctrid = {$ctrid} AND
					obs.ob || ec.epsid NOT IN (
												SELECT 
													obf.obfnumero || obf.epsid
												FROM 
													contratos.faturacontrato fc
												JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
												WHERE 
													fc.ftcstatus = 'A' AND
													fc.ctrid = {$ctrid}
												)
				ORDER BY
					datatransacao";
    	$dado = $this->carregar($sql);
    	return ($dado ? $dado : array());
    }
    
    public function apagarTodos($ftcid)
    {
    	$sql = "delete from contratos.ordembancariafatura where ftcid = $ftcid";
    	$this->executar($sql);
    	$this->commit();
    }
    
}