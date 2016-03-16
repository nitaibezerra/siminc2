<?php
	
class Dimensao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.dimensao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dimid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'dimid' => null, 
									  	'dimcod' => null, 
									  	'dimdsc' => null, 
									  	'dimstatus' => null,
    									'itrid' => null,
									  );
	protected $stOrdem = null;

	public function lista($retorno, $itrid, $whereExterno = NULL, $whereInterno= NULL, $innerInterno = NULL)
	{  
		
		if(!$itrid || !$_SESSION['par']['inuid']){
			return false; 
		}
		$where = '';
		if($_SESSION['par']['inuid'] != 1){ // verifica se o inuid é DF
			$where = " and i.indid not in (213, 214, 220, 267, 268, 269, 270) ";
		}else{
			if($_SESSION['par']['estuf'] != "DF"){
				$where = " and i.indid not in (213,214,220,267,268,269,270) ";
			}
		}
		
		if( $_REQUEST['strdimensaopar'] ) $where .= " and d.dimcod = ".$_REQUEST['strdimensaopar'];
		if( $_REQUEST['areapar'] ) $where .= " and a.arecod = ".$_REQUEST['areapar'];
		if( $_REQUEST['indicadorpar'] ) $where .= " and i.indcod = ".$_REQUEST['indicadorpar'];
		if( $_REQUEST['acaopar'] ) $where .= " and ds.sbaordem = ".$_REQUEST['acaopar'];
		
		$sql = "SELECT 	 d.dimid,
						 d.dimcod,
						 d.dimdsc,
						 a.areid,
						 a.arecod,
						 a.aredsc,
						 i.indid,
						 i.indcod,
						 i.inddsc,
						 ds.ptoid,
						 ds.aciid,
						 ds.acidsc,
						 ds.sbaid,
						 ds.sbadsc,
						 ds.sbaordem,
						 ds.frmid,
						 ds.crtpontuacao, 
						 ds.sbaextraordinaria,
						 ds.sbareformulacao,
						 ds.esddsc,
						 ds.esdid,
						 sbd.sbdid,
						 sbd.sbdparecer,
						 sbd.sbdquantidade,
						 sbd.sbdano,
						 sbd.sbdinicio,
						 sbd.sbdfim,
						 sbd.ssuid,
						 sbd.sbdanotermino,
						 sbd.sbdnaturezadespesa,
						 sbd.sbddetalhamento,
						 sbd.prpid,
						 sbd.sbdplanointerno,
						 sbd.sbdparecerdemerito,
						 sbd.sbdplicod,
						 sbd.sbdptres
					FROM 
						par.dimensao d
					INNER JOIN par.area a ON a.dimid = d.dimid
					INNER JOIN par.indicador i ON i.areid = a.areid
					LEFT  JOIN 
						(SELECT  p.ptoid,
							 ac.aciid,
							 ac.acidsc,
							 s.sbaid,
							 s.sbadsc,
							 s.sbaordem,
							 s.frmid,
							 s.sbaextraordinaria,
							 s.sbareformulacao,
							 c.indid,
							 c.crtpontuacao,
							 esd.esddsc,
							 esd.esdid
						 FROM par.criterio 	c 
						 INNER JOIN par.pontuacao 	p  ON p.crtid  = c.crtid  AND p.ptostatus  = 'A'
						 LEFT JOIN par.acao 		ac ON ac.ptoid = p.ptoid  AND ac.acistatus = 'A'
						 LEFT JOIN par.subacao 		s  ON s.aciid  = ac.aciid AND s.sbastatus  = 'A'
						 LEFT JOIN workflow.documento doc ON doc.docid = s.docid
						 LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
						 ".$innerInterno."
						 WHERE p.inuid = {$_SESSION['par']['inuid']}
						  AND c.crtstatus = 'A'
						  ".$whereInterno."
						 ) AS ds ON i.indid = ds.indid  
					LEFT JOIN par.subacaodetalhe sbd ON sbd.sbaid = ds.sbaid and sbd.sbdano = date_part('year', now())
					WHERE 	d.itrid = $itrid
						AND dimstatus = 'A'
						AND arestatus = 'A'
						AND indstatus = 'A'
						".$where.$whereExterno."
					ORDER BY d.dimcod,a.arecod,i.indcod, ds.sbaordem
							";
		// dbg($sql,1);
		/*
			$sql = "SELECT DISTINCT d.dimid,
	                         d.dimcod,
	                         d.dimdsc,
	                         a.areid,
	                         a.arecod,
	                         a.aredsc,
	                         i.indid,
	                         i.indcod,
	                         i.inddsc,
	                         p.ptoid,
	                         ac.aciid,
	                         ac.acidsc,
	                         s.sbaid,
	                         s.sbadsc,
	                         s.sbaordem,
	                         s.frmid,
	                         count(sd.sbdid) as sbdid
	             FROM par.dimensao d
	             INNER JOIN par.area a ON a.dimid = d.dimid
	             INNER JOIN par.indicador i ON i.areid = a.areid
	             LEFT JOIN par.criterio c ON c.indid = i.indid
	             LEFT JOIN par.pontuacao p ON p.crtid = c.crtid AND ptostatus = 'A' AND p.inuid = {$_SESSION['par']['inuid']}
                 LEFT JOIN par.acao ac ON ac.ptoid = p.ptoid
                 LEFT JOIN par.subacao s ON s.aciid = ac.aciid
                 LEFT JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
                 INNER JOIN par.instrumentounidade iu ON p.inuid = iu.inuid -- AND iu.inuid = {$_SESSION['par']['inuid']}
	             WHERE d.itrid = $itrid
	                        AND dimstatus = 'A'
	                        AND arestatus = 'A'
	                        AND indstatus = 'A'
	                        ".$where.$whereExterno."
	             GROUP BY d.dimid,
							 d.dimcod,
							 d.dimdsc,
							 a.areid,
							 a.arecod,
							 a.aredsc,
							 i.indid,
							 i.indcod,
							 i.inddsc,
							 p.ptoid,
							 ac.aciid,
							 ac.acidsc,
							 s.sbaid,
							 s.sbadsc,
							 s.frmid,
							 s.sbaordem
	             ORDER BY dimcod, arecod, indcod, s.sbaordem";
			*/
	                    //  dbg($sql,1);
		if($retorno == 'array'){
			return $this->carregar($sql);
		} else {
			return $sql;
		}
	}
	
	public function recuperarDimensoesGuia($itrid)
	{
		$sql = "SELECT DISTINCT 
					d.dimid, 
					d.dimcod, 
					d.dimdsc,
					d.itrid					
				FROM {$this->stNomeTabela} d
				INNER JOIN par.instrumento i ON i.itrid = d.itrid 
				WHERE d.dimstatus = 'A'
				AND d.itrid = {$itrid}
				ORDER BY d.dimcod
				--limit 1
				";

		return $this->carregar($sql);
	}
	
	public function recuperarOrdemDimensaoPorItrid($itrid)
	{
		$sql = "SELECT 
					max(dimcod) as ordem 
				FROM {$this->stNomeTabela}
				WHERE itrid = {$itrid}";
				
		$this->stOrdem =  $this->pegaUm($sql)+1;
	}
	
	public function verificaSubitensGuia($dimid)
	{
		
		$sql = "SELECT 
					count(areid) 
				FROM par.area 
				WHERE dimid = {$dimid}";
		
		if($this->pegaUm($sql) > 0){
			return false;
		} else {
			return true;
		}
		
	}
	
	public function deletarDimensaoGuia($dimid)
	{
		
		if(!$this->verificaSubitensGuia($dimid)){
			
			alert("Não foi possível excluir a dimensão, existem subitens cadastrados.");
			echo "<script>window.location.href = 'par.php?modulo=principal/configuracao/guia&acao=A';</script>";		
			return false;
		}		
		
		$this->dimid = $dimid;
		$this->excluir();
		$this->commit();		
	}
	
}