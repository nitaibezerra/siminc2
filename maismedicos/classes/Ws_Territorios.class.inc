<?php

include_once APPRAIZ . 'maismedicos/classes/Ws_Unasus_Soap.class.inc';

class Ws_Territorios extends Ws_Unasus_Soap{
	
	protected $arrInstituicoes;
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.ws_territorios";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "wstid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'wstid' => null, 
									  	'wssdata' => null, 
									  	'idinstituicao' => null, 
									  	'noinstituicao' => null, 
									  	'nucnpj' => null, 
									  	'desigla' => null, 
									  	'idibge' => null, 
									  	'uf' => null, 
									  	'municipio' => null, 
									  	'regiao' => null, 
									  );
    
    /**
     * Puxa as instituicoes e responsaveis do UNASUS via WebService
     *
     * @return array
     */
    private function getInstituicoesWS()
    {
    	$conexao_ws = $this->conexaoWS();
    	$this->arrInstituicoes = $conexao_ws->getInstituicoes(self::TOKEN_WS,4);
    	return $this->arrInstituicoes;
    }
    
    public function importarInstituicoesWS()
    {
    	$this->getInstituicoesWS();
    	 
    	if($this->arrInstituicoes->item){
    		
    		$countInstituicoes=0;
    		$countResponsabilidades=0;
    		
    		$this->inativarUniversidadeMunicipioCadastradas();
    		
    		include_once APPRAIZ . 'maismedicos/classes/Ws_Territorios_Responsavel.class.inc';
    		$resp = new Ws_Territorios_Responsavel();
//     		ver($this->arrInstituicoes->item, d);
    		foreach($this->arrInstituicoes->item as $obInstituicao){
    			
    			$countInstituicoes++;
    			
    			$arInst['idinstituicao'] 	= $obInstituicao->idInstituicao;
    			$arInst['noinstituicao'] 	= $obInstituicao->noInstituicao;
    			$arInst['nucnpj'] 			= str_replace(array('.','/','-'), '', $obInstituicao->nuCnpj);
    			$arInst['desigla'] 			= $obInstituicao->deSigla;
    			$arInst['idibge'] 			= $obInstituicao->ibge->item->idIbge;
    			$arInst['uf'] 				= $obInstituicao->ibge->item->uf;
    			$arInst['municipio'] 		= $obInstituicao->ibge->item->municipio;
    			$arInst['regiao'] 			= $obInstituicao->ibge->item->regiao;
    			
    			$this->popularDadosObjeto($arInst);
    			$idInst = $this->salvar();
    			$this->clearDados();
    			
    			if($idInst){
    			
    				if($obInstituicao->ibgeResponsavel->item){
    					
    					// Insere quando tiver mais de uma resposabilidade 
    					if(is_array($obInstituicao->ibgeResponsavel->item)){
	    					foreach($obInstituicao->ibgeResponsavel->item as $obResp){
	    						
	    						if( !empty($obResp->idIbge) && !empty($obResp->uf) ){
	    							
	    							$countResponsabilidades++;
	    							
	    							$arResp['idibge'] = $obResp->idIbge;
	    							$arResp['uf'] = $obResp->uf;
	    							$arResp['municipio'] = $obResp->municipio;
	    							$arResp['regiao'] = $obResp->regiao;
	    							$arResp['wstid'] = $idInst;
	    							 
	    							$resp->popularDadosObjeto($arResp);
	    							$resp->salvar();
	    							$resp->clearDados();
	    			
	    						}
	    					}
	    					
    					}else{
    						
    						// Insere quando tiver somente uma resposabilidade 
    						$obResp = $obInstituicao->ibgeResponsavel->item;
    						
    						if( !empty($obResp->idIbge) && !empty($obResp->uf) ){
    						
    							$countResponsabilidades++;
    						
    							$arResp['idibge'] = $obResp->idIbge;
    							$arResp['uf'] = $obResp->uf;
    							$arResp['municipio'] = $obResp->municipio;
    							$arResp['regiao'] = $obResp->regiao;
    							$arResp['wstid'] = $idInst;
    								
    							$resp->popularDadosObjeto($arResp);
    							$resp->salvar();
    							$resp->clearDados();
    						
    						}
    					}
    				}
    			}
    			
    		}
    		$this->commit();
    		
    		$this->insereUniversidadesMuniocipio();
    	}
    	
    	$txt = "<p>{$countInstituicoes}&nbsp;Instituições de ensino</p>
    			<p>{$countResponsabilidades}&nbsp;Municípios vinculados às instituições de ensino</p>";
    			
    	echo $txt;
    	
    	$txt = "Atualização da Vinculação de Instituições/Município do Mais Médicos pelo WS.
    			<br/><br/>".$txt;
    	
    	$this->enviarEmailAuditoriaRotinas('Atualização da Vinculação de Instituições/Município do Mais Médicos', $txt);
    }
    
    private function inativarUniversidadeMunicipioCadastradas()
    {
    	$sql = "update maismedicos.ws_territorios_responsavel set wssstatus = 'I'
    			where to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wssstatus = 'A';
    			update maismedicos.ws_territorios set wssstatus = 'I'
    			where to_char(wssdata,'YYYY-MM-DD') = to_char(now(),'YYYY-MM-DD') and wssstatus = 'A';";
    	 
    	$this->executar($sql);
    	$this->commit();
    }
    
    private function insereUniversidadesMuniocipio()
    {
    	$sql = "select wstid, uniid, idunasus, m.muncod from maismedicos.ws_territorios t
				join maismedicos.universidade u on u.idunasus = t.idinstituicao
				join territorios.municipio m on substr(m.muncod, 1, 6) = t.idibge
				where to_char(wssdata,'DD-MM-YYYY') = to_char(now(),'DD-MM-YYYY')
    			and wssstatus = 'A' and unistatus = 'A'";
    	
    	$rsInst = $this->carregar($sql);
    	 
    	$msg='';
    	$totalMun=0;
    	$totalIns=0;
    	foreach($rsInst as $inst){
    	
    		$totalIns++;
    	
    		$sql = "select distinct m.muncod from maismedicos.ws_territorios_responsavel r
    		join territorios.municipio m on substr(m.muncod, 1, 6) = r.idibge
    		where r.wstid = {$inst['wstid']}
    				and wssstatus = 'A'";
    	
			$rsResp = $this->carregar($sql);
    	
			if($rsResp){
    		 
	    		foreach($rsResp as $resp){
	    	
	    			$sqlInsertResp .= "insert into maismedicos.universidademunicipio
	    			(uniid, muncod)
	    					values
	    					({$inst['uniid']}, '{$resp['muncod']}');";
	    					$totalMun++;
	    	
				}
			}
		}
    					 
		if($sqlInsertResp){
			$sqlInsertResp = "delete from maismedicos.universidademunicipio;".$sqlInsertResp;
			$this->executar($sqlInsertResp);
			$this->commit();
    	}
   
    	$msg .= '<br/>Total de universidades vinculadas aos municípios: '.$totalIns;
    	    	$msg .= '<br/>Total de municípios vinculados as universidades: '.$totalMun;
    	    	
    	return $msg;
    }
    
    
}