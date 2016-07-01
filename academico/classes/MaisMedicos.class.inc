<?php
	
class MaisMedicos extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.maismedicos";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "msmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'msmid' => null, 
									  	'entid' => null,
    									'hsuid' => null,
							    		'msminteresse' => null,
							    		'msmadesao' => null,
							    		'msmsetorresponsavel' => null,
							    		'msmemailsetor' => null,
							    		'usucpfcadastro' => null,
							    		'usucpfalteracao' => null,
							    		'msmdatainclusao' => null,
							    		'msmdataalteracao' => null,
										'msmfonesetor' => null,
										'msmresponsavelsetor' => null,
    									'msmdddsetor' => null,
    									'arqid' => null,
    									'arqidtermo' => null,
									  );
    
    public function recupaPorEntidade($entid){
    
    	$sql = "SELECT * FROM $this->stNomeTabela where entid = $entid";
    	$arrDados = $this->pegaLinha( $sql );
    	if($arrDados){
    		$this->carregarPorId($arrDados['msmid']);
    	}
    	return $arrDados;
    	
    }
    
    public function recupaPorUnidade($hsuid){
    
    	$sql = "SELECT * FROM $this->stNomeTabela where hsuid = $hsuid";
    	$arrDados = $this->pegaLinha( $sql );
    	if($arrDados){
    		$this->carregarPorId($arrDados['msmid']);
    	}
    	return $arrDados;
    	 
    }
    
    function salvarMaisMedicos()
    {
    	$_POST['msminteresse'] = $_POST['msminteresse'] == "sim" ? "true" : null;
    	if(!$_POST['msmid']){
    		$_POST['usucpfcadastro'] = $_SESSION['usucpf'];
    		$_POST['msmdatainclusao'] = date("Y-m-d H:m:s");
    	}
    	$_POST['usucpfalteracao'] = $_SESSION['usucpf'];
    	$_POST['msmdataalteracao'] = date("Y-m-d H:m:s");
    	
    	if($_FILES['arquivo']['tmp_name']){
    		$_POST['arqid'] = $_REQUEST['arquivo'] ? $_REQUEST['arquivo'] : $this->salvarArquivoMaisMedico("Anexo do Termo de Pré-Adesão");
    	}
    	
    	if($_FILES['arquivotermo']['tmp_name']){
    		$_POST['arqidtermo'] = $_REQUEST['arquivotermo'] ? $_REQUEST['arquivotermo'] : $this->salvarArquivoMaisMedico("Anexo do Termo de Adesão","arquivotermo");
    	}
    	
    	$this->popularDadosObjeto($_POST);
    	$msmid = $this->salvar();
    	if($msmid){
    		$this->commit();
    		$_SESSION['academico']['maismedicos']['alert'] = "Operação realizada com sucesso.";
    	}else{
    		$_SESSION['academico']['maismedicos']['alert'] = "Não foi possível realizar a operação.";
    	}
    	if($_POST['hsuid']){
    		header("Location: academico.php?modulo=principal/maisMedicosUnidade&acao=A");
    	}else{
    		header("Location: academico.php?modulo=principal/maisMedicos&acao=A");
    	}
    	exit;
    }
    
    public function salvarArquivoMaisMedico($descricao = "Termo de Pré-Adesão",$campo = "arquivo")
    {
    
    	require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    	$file = new FilesSimec(null,null,"academico");
    	$file->setUpload($descricao,$campo,false,false);
    	return $file->getIdArquivo();
    }
    
    public function excluirAnexo()
    {
    	$arqid = $_POST['arqid'];
    	$arqidtermo = $_POST['arqidtermo'];
    	if($arqid){
    		$sql = "update $this->stNomeTabela set arqid = null where arqid = $arqid";
    	}
    	if($arqidtermo){
    		$sql = "update $this->stNomeTabela set arqidtermo = null where arqidtermo = $arqidtermo";
    	}
    	if($sql){
    		$this->executar($sql);
    		$this->commit();
    	}
    }
}