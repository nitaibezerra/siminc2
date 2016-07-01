<?php
class Certificado extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.certificado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cerid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cerid' => null,
    									'entid' => null, 
    									'cerdsc' => null, 
    									'cerdata' => null, 
    									'cerobs' => null, 
    									'cerarea' => null, 
    									'cernivel' => null, 
    									'certipo' => null, 
    									'cerstatus' => null, 
									  );
									  
	
    public function listarCertificados($tipo,$entid){
    	//require_once APPRAIZ . 'www/rehuf/_constantes.php'; 
    	//define("REHUF_HOSPITAL_ENSINO",1);
		//define("REHUF_HOSPITAL_AMIGO_CRIANCA",2);
		//define("REHUF_HOSPITAL_ACREDITACAO_CERTIFICACAO_QUALIDADE",3);
		//define("REHUF_HOSPITAL_OUTRAS",4);
    	if($tipo == REHUF_HOSPITAL_ENSINO){
    		$sql = "SELECT 
    					'<img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirCertificado(' || cer.cerid || ')\"  />' as acao,
    					cerdsc,
    					coalesce(to_char(cerdata,'DD/MM/YYYY'),'N/A'),
    					cerobs 
    				FROM 
    					{$this->stNomeTabela} cer
    				WHERE 
    					certipo = '{$tipo}' and cerstatus = 'A' and entid = {$entid}";
	    	$arrCabecalho = array (
	    			"Ação",
	    			"Portaria da Homologação",
	    			"Data da Publicação",
	    			"Observação"
	    	);
    	} elseif ($tipo == REHUF_HOSPITAL_AMIGO_CRIANCA){
    		$sql = "SELECT
    					'<img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirCertificado(' || cer.cerid || ')\"  />' as acao,
    					cerdsc,
    					coalesce(to_char(cerdata,'DD/MM/YYYY'),'N/A'),
    					cerobs
    				FROM
    					{$this->stNomeTabela} cer
    				WHERE
    					certipo = '{$tipo}' and cerstatus = 'A' and entid = {$entid}";
    		$arrCabecalho = array (
	    			"Ação",
	    			"Portaria da Homologação",
	    			"Data da Publicação",
	    			"Observação"
	    	);
    	} elseif ($tipo == REHUF_HOSPITAL_ACREDITACAO_CERTIFICACAO_QUALIDADE){
    		$sql = "SELECT
    					'<img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirCertificado(' || cer.cerid || ')\"  />' as acao,
    					cerdsc,
    					coalesce(to_char(cerdata,'DD/MM/YYYY'),'N/A'),
    					cerarea,
    					cernivel,
    					cerobs
    				FROM
    					{$this->stNomeTabela} cer
    				WHERE
    					certipo = '{$tipo}' and cerstatus = 'A' and entid = {$entid}";
    		$arrCabecalho = array (
	    			"Ação",
    				"Nome",
    				"Mês/Ano Obtenção",
    				"Área Certificada",
    				"Nível",
    				"Observação"
	    	);
    	} else {
    		$sql = "SELECT
    					'<img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirCertificado(' || cer.cerid || ')\"  />' as acao,
    					cerdsc,
    					coalesce(to_char(cerdata,'DD/MM/YYYY'),'N/A'),
    					cerobs
    				FROM
    					{$this->stNomeTabela} cer
    				WHERE
    					certipo = '{$tipo}' and cerstatus = 'A' and entid = {$entid}";
    		$arrCabecalho = array (
    				"Ação",
    				"Nome",
    				"Mês/Ano Obtenção",
    				"Observação"
    		);
    	}
    	$this->monta_lista ( $sql, $arrCabecalho, 30, 10, "N", "center", "N", '', '80%' );
    }
    
  	public function excluirCertificado(){
  		$cerid = $_POST['cerid'];
  		$sql = "UPDATE rehuf.certificado SET cerstatus = 'I' where cerid = {$cerid}";
  		$this->executar($sql);
  		$this->commit();
  	}
  	
  	public function adicionarCertificado(){
  		$_POST['entid'] = $_SESSION['rehuf_var']['entid'];
  		$_POST['certipo'] = $_POST['tipo'];  
  		$this->popularDadosObjeto($_POST);
		$cerid = $this->salvar();
		if($this->commit()){
			return $cerid;
		} else {
			return "<script type='text/javascript'>alert('Ocorreu um erro no cadastro do certificado. Contate o administrador')</script>";
		}
		
  	}
  	
  	public function listaCertificados(){
  		$arrCertificado[] = array("codigo" => "Accreditation Canada International" , "descricao" => "Accreditation Canada International"); 
  		$arrCertificado[] = array("codigo" => "Compromisso com a Qualidade Hospitalar (CQH)" , "descricao" => "Compromisso com a Qualidade Hospitalar (CQH)"); 
  		$arrCertificado[] = array("codigo" => "Joint Commission International" , "descricao" => "Joint Commission International"); 
  		$arrCertificado[] = array("codigo" => "ISO 9001:2008" , "descricao" => "ISO 9001:2008"); 
  		$arrCertificado[] = array("codigo" => "Organização Nacional de Acreditação" , "descricao" => "Organização Nacional de Acreditação"); 
  		$arrCertificado[] = array("codigo" => "Outras" , "descricao" => "Outras");
  		 
  		return $arrCertificado;
  	}
  	
  	public function listaNiveis(){
  		$arrNiveis[] = array("codigo" => "Acreditado" , "descricao" => "Acreditado");
  		$arrNiveis[] = array("codigo" => "Acreditado Pleno" , "descricao" => "Acreditado Pleno");
  		$arrNiveis[] = array("codigo" => "Acreditado com Excelência" , "descricao" => "Acreditado com Excelência");
  		
  		return $arrNiveis;
  	}
}