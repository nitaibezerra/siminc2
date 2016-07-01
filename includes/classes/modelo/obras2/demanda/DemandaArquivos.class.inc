<?php
	
class DemandaArquivos extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.demanda_arquivos";	

    /**
     * Chave primaria.
     * @var array 
     * @access protected
     */
    protected $arChavePrimaria = array( "darid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'darid' => null, 
									  	'demid' => null, 
									  	'arqid' => null, 
									  	'dartipo' => null, 
									  	'darstatus' => null, 
									  );
    
    public function listaSql( Array $param = array(), $tipoSelecao = null ){
    
    	$where = Array("darstatus = 'A'");
    	 
    	if( $param['dartipo'] ){
    		$where[] = "dartipo = '".$param['dartipo']."' ";
    	}
    	
    	$sql = "SELECT
    				'<center>
		    			<img
			    			align=\"absmiddle\"
			    			src=\"/imagens/icone_lupa.png\"
			    			style=\"cursor: pointer\"
			    			id=\"' || arqid || '\"
			    			class=\"download\"
			    			title=\"Baixar Arquivo\">
		    			<img
			    			align=\"absmiddle\"
			    			src=\"/imagens/excluir_2.gif\"
			    			width=\"19px\"
			    			style=\"cursor: pointer; margin-left: 3px;\"
			    			id=\"' || arqid || '\"
			    			class=\"excluir\"
			    			title=\"Excluir Arquivo\">
	    			</center>' as acao,
	    			darnome,
	    			to_char(dardtinclusao,'DD/MM/YYYY') as data
		    	FROM
		    		obras2.demanda_arquivos
		    	WHERE
		    		".implode(' AND ',$where)."
		    	ORDER BY
		    		2";
    
    			return $sql;
    }

    public function excluirArquivo(){
    	
    	$sql = "UPDATE obras2.demanda_arquivos SET
    				darstatus = 'I'
    			WHERE
    				demid = ".$_SESSION['obras2']['demid']." AND
    				arqid = ".$_REQUEST['arqid'];
    	$this->executar($sql);
    	$this->commit();
    }
}