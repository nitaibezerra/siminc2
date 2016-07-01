<?php
	
class PrestacaoContas extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.prestacaocontas";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prcid' => null,
    									'prcug' => null, 
    									'vpiid' => null,
									  	'pceid' => null, 
									  	'prcprovisaoconcedida' => null, 
									  	'prcprovisaorecebida' => null, 
									  	'prcdestaqueconcedido' => null,
    									'prcdestaquerecebido' => null,
    									'usucpf' => null,
    									'dataatualizacao' => null,
    									'docid' => null
									  );
									  
									  
	public function buscarRespostaQuestionarioPorPrestacaoContas($prcid = null){

			if(!$prcid){
				return array();
			}
			$sql = "select distinct
						res.perid,
						res.resid,
						res.qrpid,
						res.resdsc,
						qrp.queid
					from 
						questionario.resposta res
					inner join
						questionario.questionarioresposta qrp ON qrp.qrpid = res.qrpid
					inner join
						rehuf.prestacaocontasresposta pcr ON pcr.qrpid  = qrp.qrpid
					where
						prcid = $prcid
					order by
						queid,perid";
			$arrDados = $this->carregar($sql);
			if($arrDados){
				foreach($arrDados as $dado){
					$arrResultado[$dado['queid']][$dado['perid']] = array("qrpid"=>$dado['qrpid'],"resid"=>$dado['resid'],"resdsc"=>$dado['resdsc']);
				}
			}
			
		return $arrResultado ? $arrResultado : array();
	}
	
	public function buscarPrestacaoContasUg($entungcod,$vpiid){
		if($entungcod){
			$sql = "select
							prcug
						from 
							{$this->stNomeTabela}  
						where
							prcug = '$entungcod'
						and
							vpiid = '$vpiid'";
			return  $this->pegaUm($sql);
		}else{
			return false;
		}
	}
	
	public function buscarPrestacaoContasVinculoPlanoInterno($vpiid){
			$sql = "select
							prcid
						from 
							{$this->stNomeTabela}  
						where
							vpiid = '$vpiid'";
			return  $this->carregar($sql);
	}
									  
}