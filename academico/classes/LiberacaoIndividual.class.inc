<?php

/**
 * @author Rondomar França
 * @name Liberação Individual
 *
 */
class LiberacaoIndividual extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.liberacaoindividual";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'libindtinicio' => null,
        'libindtfinal'  => null,
    	'entid' 	 	=> null
    );
    
    public function existe( $entid ){
    
    	$sql = "SELECT
		    		COUNT(*)
		    	FROM
		    		$this->stNomeTabela
		    	WHERE
		    		entid=$entid";
    
    	return ($this->pegaUm($sql) > 0);
    }

    public function atualizar($libindtinicio, $libindtfinal, $entid){

        $libindtinicio = ($libindtinicio) ? "libindtinicio='".formata_data_sql($libindtinicio). "'": "libindtinicio=null";
        $libindtfinal  = ($libindtfinal)  ? "libindtfinal ='".formata_data_sql($libindtfinal) . "'": "libindtfinal=null";

        $sql = "UPDATE
                    $this->stNomeTabela
                SET
                    $libindtinicio,
                    $libindtfinal
        		WHERE entid=" . $entid;
        	

        $this->executar($sql);
    }
    
    public function salvar($libindtinicio, $libindtfinal, $entid){
    
    	$libindtinicio = formata_data_sql($libindtinicio);
    	$libindtfinal  = formata_data_sql($libindtfinal);
    
    	$sql = "INSERT INTO $this->stNomeTabela";
    	$sql.= "(entid, libindtinicio, libindtfinal)";
    	$sql.= " VALUES(".$entid.",'".$libindtinicio."','".$libindtfinal."')";
    
    	$this->executar($sql);
    }

    public function pegarDados( $entid ){

        $sql = "SELECT * FROM $this->stNomeTabela WHERE entid=" . $entid;

        return $this->pegaLinha($sql);

    }

    public function noPrazo( $entid ){
        
        $arrDados = $this->pegarDatas( $entid );

        $dataAtual = date('Y-m-d');

        return (
                    ( strtotime($arrDados['libindtinicio']) < strtotime($dataAtual) ) &&
                    ( strtotime($dataAtual) <= strtotime($arrDados['libindtfinal']) )
               );

    }
    
    public function pegarDatas( $entid ){
    
    	$sql = "SELECT * 
    			FROM $this->stNomeTabela
    			WHERE 
    				libindtfinal is not null
    			AND entid={$entid} 
    			ORDER BY libindtfinal DESC";
    
    	return $this->pegaLinha($sql);
    
    }
    
    public function validaPeriodoIndividual($libindtinicio, $libindtfinal){
    	
    	$libindtinicio = formata_data_sql($libindtinicio);
    	$libindtfinal  = formata_data_sql($libindtfinal);
    	
    	$sql = "select count(*) from academico.liberacao
    			where ('{$libindtinicio}' between libdtinicio and libdtfinal)
    			or ('{$libindtfinal}' between libdtinicio and libdtfinal)";
    	
    	return ($this->pegaUm($sql) > 0);
    }

}