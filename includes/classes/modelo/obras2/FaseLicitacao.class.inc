<?php
	
class FaseLicitacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.faselicitacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "flcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'flcid' => null, 
                                        'tflid' => null, 
                                        'licid' => null, 
                                        'arqid' => null, 
                                        'flcpubleditaldtprev' => null, 
                                        'flcaberpropdtprev' => null, 
                                        'flcrecintermotivo' => null, 
                                        'flchomlicdtprev' => null, 
                                        'flcordservdt' => null, 
                                        'flcordservnum' => null, 
                                        'flcstatus' => null, 
                                        'flcdtinclusao' => null, 
                                        'flcdtrecintermotivo' => null, 
                                        'flcdata' => null, 
                                        'flcmeiopublichomol' => null, 
                                        'flcobshomol' => null, 
                                  );

	public function antesSalvar(){
		
		$this->flcpubleditaldtprev = (strlen( $this->flcpubleditaldtprev ) > 10 || !($this->flcpubleditaldtprev) 
										? ($this->flcpubleditaldtprev ? $this->flcpubleditaldtprev : null) 
										: formata_data_sql($this->flcpubleditaldtprev) );
		$this->flcdtrecintermotivo = (strlen( $this->flcdtrecintermotivo ) > 10 || !($this->flcdtrecintermotivo) 
										? ($this->flcdtrecintermotivo ? $this->flcdtrecintermotivo : null) 
										: formata_data_sql($this->flcdtrecintermotivo) );
		$this->flcordservdt 	   = (strlen( $this->flcordservdt ) > 10 || !($this->flcordservdt) 		
										? ($this->flcordservdt ? $this->flcordservdt : null) 
										: formata_data_sql($this->flcordservdt) );
		$this->flchomlicdtprev 	   = (strlen( $this->flchomlicdtprev ) > 10 || !($this->flchomlicdtprev) 	
										? ($this->flchomlicdtprev ? $this->flchomlicdtprev : null) 
										: formata_data_sql($this->flchomlicdtprev) );
		$this->flcaberpropdtprev   = (strlen( $this->flcaberpropdtprev ) > 10 || !($this->flcaberpropdtprev)   
										? ($this->flcaberpropdtprev ? $this->flcaberpropdtprev : null) 
										: formata_data_sql($this->flcaberpropdtprev) );

		$this->licid = ($this->licid ? $this->licid : null);										
		return true;
	}									  
									  
	public function listaPorLicitacao( $licid, Array $arFlcid = array() ){
            
            if(count($arFlcid) > 0){
                $strWhereLicid = " fl.flcid IN(" . implode(", ", $arFlcid) . ") AND " ;
            }else{
                $licid = trim($licid);
                if(!empty($licid)){
                    $strWhereLicid = " fl.licid = '{$licid}' AND ";
                } else {
                    $strWhereLicid = '';
                }
            }
            
		$sql = "SELECT 
					fl.*,
					tfl.tfldesc, 
					tfl.tflordem,
					al.*,
					a.arqextensao,
					a.arqnome,
					fl.flcid   
				FROM 
					obras2.faselicitacao fl 
				JOIN 
					obras2.tiposfaseslicitacao tfl ON tfl.tflid = fl.tflid AND
													  tfl.tflstatus = 'A'
				LEFT JOIN ( SELECT
								MAX(al.arqid) AS arqid,
								al.flcid
							FROM
								obras2.arquivolicitacao al
							WHERE
								al.aqlstatus = 'A'
							GROUP BY
								al.flcid) al ON al.flcid = fl.flcid
				LEFT JOIN public.arquivo a ON a.arqid = al.arqid								
				WHERE ".$strWhereLicid."
					fl.flcstatus = 'A' 
				ORDER BY tfl.tflordem";
//		dbg($sql, d);
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	}

	public function getArquivo( $flcid ){
		if ( empty($flcid) )
			return array();
		
		$sql = "SELECT
					a.*
				FROM
					obras2.faselicitacao fl
				JOIN public.arquivo a ON a.arqid = fl.arqid
				WHERE
					fl.flcstatus = 'A' AND
					fl.flcid = {$flcid}";
		
		$dados = $this->pegaLinha( $sql );
		return ($dados ? $dados : array());		
	}

	public function verificaHomologacaoPorObra( $obrid ){
		$sql = "SELECT
					count(*)
				FROM
					obras2.obralicitacao ol
				JOIN obras2.faselicitacao fl ON fl.licid = ol.licid AND
												fl.flcstatus = 'A' AND
												fl.tflid = " . FASE_LIC_HOMOLOGACAO . "
				JOIN obras2.arquivolicitacao al ON al.flcid = fl.flcid AND
								   				   al.aqlstatus = 'A'
				WHERE
					ol.oblstatus = 'A' AND
					ol.obrid = {$obrid}";
		
		return $this->pegaUm( $sql );
	}
}