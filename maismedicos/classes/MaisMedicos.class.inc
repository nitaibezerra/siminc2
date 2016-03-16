<?php
	
class MaisMedicos extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.maismedicos";	

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
    									'uniid' => null,
							    		'msminteresse' => null,
							    		'msmadesao' => null,
							    		'msmsetorresponsavel' => null,
							    		'msmemailsetor' => null,
							    		'usucpfcadastro' => null,
							    		'usucpfalteracao' => null,
    									'usucpfpreadesao' => null,
    									'usucpfadesao' => null,
							    		'msmdatainclusao' => null,
							    		'msmdataalteracao' => null,
										'msmfonesetor' => null,
										'msmresponsavelsetor' => null,
    									'msmdddsetor' => null,
    									'arqid' => null,
    									'arqidtermo' => null,
									  );
    
    public function recupaPorUnidade($uniid){
    
    	$sql = "SELECT * FROM $this->stNomeTabela where uniid = $uniid";
    	$arrDados = $this->pegaLinha( $sql );
    	if($arrDados){
    		$this->carregarPorId($arrDados['msmid']);
    	}
    	return $arrDados;
    	 
    }
    
    function salvarMaisMedicos()
    {
    	$_POST['msminteresse'] = $_POST['msminteresse'] == "sim" ? "true" : null;
    	$_POST['msmadesao'] = $_POST['msmadesao'] == "sim" ? "true" : null;
    	
    	if($_POST['msminteresse']){
    		$_POST['usucpfpreadesao'] = $_SESSION['usucpf'];
    	}
    	
    	if($_POST['msmadesao']){
    		$_POST['usucpfadesao'] = $_SESSION['usucpf'];
    	}
    	if($_POST['gerartermoadesao']){
    		$_POST['arqidtermo'] = $this->gerarTermoAdesao($_POST['uniid']);
    	}
    	
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
    		$_SESSION['maismedicos']['maismedicos']['alert'] = "Operação realizada com sucesso.";
    	}else{
    		$_SESSION['maismedicos']['maismedicos']['alert'] = "Não foi possível realizar a operação.";
    	}
    	$url = $_POST['url'] ? $_POST['url'] : "maismedicos.php?modulo=principal/maisMedicos&acao=A";
    	header("Location: $url");
    	exit;
    }
    
    public function salvarArquivoMaisMedico($descricao = "Termo de Pré-Adesão",$campo = "arquivo")
    {
    
    	require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    	$file = new FilesSimec(null,null,"maismedicos");
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
    
    public function gerarTermoAdesao($uniid)
    {
    	ob_clean();
    	ob_start();
    	documentoAdesao($uniid,true);
    	$termo = ob_get_contents();
    	ob_end_clean();
    	    	
    	include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";
    	
    	$dompdf = new DOMPDF();
    	$dompdf->load_html($termo);
    	$dompdf->render();
    	
    	$pdfoutput = $dompdf->output();
    	
    	if(!is_dir(APPRAIZ . 'arquivos/maismedicos')) {
    		mkdir(APPRAIZ . 'arquivos/maismedicos', 0777);
    	}
    	
    	if(!is_dir(APPRAIZ . 'arquivos/maismedicos/termoadesao')) {
    		mkdir(APPRAIZ . 'arquivos/maismedicos/termoadesao', 0777);
    	}
    	
    	$caminho = APPRAIZ . 'arquivos/maismedicos/termoadesao/'.$uniid.'.pdf';
    	 
    	$fp = fopen($caminho, "w+");
    	
    	stream_set_write_buffer($fp, 0);
    	fwrite($fp, $pdfoutput);
    	fclose($fp);
    	
    	require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    	$file = new FilesSimec(null,null,"maismedicos");
    	$file->setMover($caminho, "pdf", false);
    	return $file->getIdArquivo();
    	
    }
    
    function excluirTermoAdesao()
    {
    	$arqidtermo = $_POST['arqidtermo'];
    	if($arqidtermo){
    		$sql = "update $this->stNomeTabela set arqidtermo = null where arqidtermo = $arqidtermo";
    	}
    	if($sql){
    		$this->executar($sql);
    		$this->commit();
    	}
    }
}