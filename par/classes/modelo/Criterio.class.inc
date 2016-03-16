<?php
	
class Criterio extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.criterio";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "crtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'crtid' => null, 
									  	'indid' => null, 
									  	'crtdsc' => null, 
									  	'crtpontuacao' => null,
    									'crtpeso' => null, 
    									'crtstatus' => null 
									  );
									  
	public function recuperarCriteriosAcoesGuia($indid)
	{
		$sql = "SELECT DISTINCT 
					cr.crtid, 
					cr.crtpontuacao, 
					cr.crtdsc, 
					cr.indid
			    FROM {$this->stNomeTabela} cr			    
			    WHERE cr.indid = {$indid} AND cr.crtstatus = 'A'
			    ORDER BY cr.crtpontuacao";
		
//		ver($sql);
//		die();

		return $this->carregar($sql);
	}
	
	public function verificaPesoPontuacao($indid)
	{
		$sql = "SELECT 
					sum(crtpeso) AS peso
				FROM par.criterio c
				INNER JOIN par.indicador i ON i.indid = c.indid
				WHERE i.indid = {$indid} AND c.crtstatus = 'A'";		
				
		return $this->pegaUm($sql);
	}
	
	public function verificaPontuacao($indid, $crtid = null)
	{
		if($crtid){
			$stWhere = "AND crtid <> {$crtid}";
		}
		
		$sql = "SELECT 
					crtpontuacao AS peso
				FROM par.criterio c
				WHERE c.indid = {$indid}
				AND c.crtstatus = 'A'
				{$stWhere}";
						
		return $this->carregar($sql);
	}
	
	public function verificaSubitensGuia($crtid)
	{
		$sql = "SELECT 
					count(crtid) 
				FROM par.propostaacao 
				WHERE crtid = {$crtid}";
				
		$nrCriterio = $this->pegaUm($sql);
		
		if($nrCriterio > 0){
			return false;
		} else {
			return true;
		}
		
	}
	
	public function deletarCriterioGuia($crtid)
	{
		if(!$this->verificaSubitensGuia($crtid)){
			
			alert("Não foi possível excluir o critério, existem subitens cadastrados.");
			echo "<script>window.location.href = 'par.php?modulo=principal/configuracao/guia&acao=A';</script>";		
			return false;
		}		
		
		$this->crtid = $crtid;
		$this->excluir();
		$this->commit();			
	}
	
	public function recuperarCriterioSubacoesPorIndicador($indid)
	{
		$sql = "SELECT * FROM par.propostaacao ppa 
				INNER JOIN par.criterio crt ON crt.crtid = ppa.crtid AND crt.crtstatus = 'A'
				WHERE crt.indid = {$indid}";		
		
		return $this->carregar($sql);
	} 
									  
}									  