<?php
	
class ItensComposicao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.itenscomposicao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'itcid' => null, 
									  	'micid' => null, 
									  	'itcdesc' => null, 
									  	'itcdtinclusao' => null, 
									  	'itcdescservico' => null, 
									  	'itcordem' => null, 
									  	'itcstatus' => null 
									  );
					
    public function listaCombo(){
        $sql = "
            SELECT 
                itcid AS codigo, 
                itcdesc AS descricao 
            FROM obras2.itenscomposicao 
            WHERE itcstatus='A'
            ORDER BY itcdesc, itcordem ASC
            ";
        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }
    
	public function getByObraAndMacro($obrid, $micid){
		$sql = "SELECT 
					SUM(icovlritem) AS icovlritemsoma,
					TO_CHAR(MIN(icodtinicioitem), 'dd/mm/YYYY') AS icodtinicioitem,
					TO_CHAR(MAX(icodterminoitem), 'dd/mm/YYYY') AS icodterminoitem,
					(SUM(spivlrfinanceiroinfsupervisor) / ocr.ocrvalorexecucao * 100) / COUNT(spiid) AS vlrexecutado
				FROM 
					obras2.itenscomposicao ic
				JOIN obras2.itenscomposicaoobra io ON io.itcid = ic.itcid AND
												      io.icostatus = 'A'
                JOIN obras2.cronograma cro ON cro.crostatus = 'A' AND cro.obrid = {$obrid} AND io.croid = cro.croid AND io.obrid = cro.obrid
				JOIN (SELECT 
						MAX(supid) AS supid,
						obrid
					  FROM
						obras2.supervisao
					  WHERE
						emsid IS NULL AND
						sueid IS NULL AND
						supstatus = 'A'
					  GROUP BY
						obrid) s ON s.obrid = io.obrid
				JOIN obras2.supervisaoitem si ON si.icoid = io.icoid AND
								 si.supid = s.supid 
				LEFT JOIN (SELECT 
							ocrvalorexecucao, obrid 
						   FROM 
							obras2.obrascontrato 
						   WHERE  
							ocrstatus = 'A') ocr ON ocr.obrid = io.obrid 				 
				WHERE
					ic.itcstatus = 'A' AND
					ic.micid = {$micid}
				GROUP BY
					ocr.ocrvalorexecucao";
		
		$dado = $this->pegaLinha($sql);
		return ($dado ? $dado : array());
	}									 
}