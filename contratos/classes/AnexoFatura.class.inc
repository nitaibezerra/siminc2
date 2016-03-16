<?php
	
class AnexoFatura extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.anexofatura";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "anfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'anfid' => null, 
									  	'ftcid' => null, 
									  	'arqid' => null, 
									  	'usucpf' => null, 
									  	'anfdescricao' => null, 
									  	'anfstatus' => null, 
									  );
    
    public function listaAnexo($ftcid,$habilitado = false)
    {
    	if($habilitado){
    		$acao = "'<img src=\"../imagens/clipe.gif\" class=\"link\" onclick=\"downloadAnexo(' || arq.arqid || ')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirAnexo(' || anf.anfid || ',' || arq.arqid || ',$ftcid)\" />'";
    	}else{
    		$acao = "'<img src=\"../imagens/clipe.gif\" class=\"link\" onclick=\"downloadAnexo(' || arq.arqid || ')\" />'";
    	}
    	$sql = "select
    				$acao as acao,
    				arq.arqdescricao,
    				arq.arqnome || '.' || arq.arqextensao as descricao,
    				to_char(arqdata,'DD/MM/YYYY') as data,
    				usu.usunome
    			from
    				contratos.anexofatura anf
    			inner join
    				public.arquivo arq ON arq.arqid = anf.arqid
    			inner join
    				seguranca.usuario usu ON usu.usucpf = arq.usucpf
    			where
    				anf.anfstatus = 'A'
    			and
    				anf.ftcid = $ftcid
    			and
    				arq.arqstatus = 'A' 
    			order by
    				arqdata desc";
    	$arrCab = array("Ação","Descrição","Arquivo","Data","Responsável");
    	$this->monta_lista($sql,$arrCab,30,10,"N","");
    }
    
    public function salvarAnexo($ftcid)
    {
    	require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    	$arrCampos = array("ftcid"=>$ftcid,"usucpf"=>"'{$_SESSION['usucpf']}'");
    	
    	if($_FILES['arquivo']['tmp_name']){
    		foreach($_FILES['arquivo']['tmp_name'] as $n => $arquivo){
    			if($_FILES['arquivo']['tmp_name'][$n]){
    				$_FILES['arquivo_'.$n]['tmp_name'] = $_FILES['arquivo']['tmp_name'][$n];
    				$_FILES['arquivo_'.$n]['type'] = $_FILES['arquivo']['type'][$n];
    				$_FILES['arquivo_'.$n]['error'] = $_FILES['arquivo']['error'][$n];
    				$_FILES['arquivo_'.$n]['size'] = $_FILES['arquivo']['size'][$n];
    				$_FILES['arquivo_'.$n]['name'] = $_FILES['arquivo']['name'][$n];
	    			$file = new FilesSimec("anexofatura",$arrCampos,"contratos");
	    			$file->setUpload(($_POST['arqdescricao'][$n] ? $_POST['arqdescricao'][$n] : "Anexo da Fatura"),'arquivo_'.$n,true,false);
    			}
    		}
    	}
    	return true;
    }
    
    public function downloadAnexo()
    {
    	include_once APPRAIZ . "includes/classes/file.class.inc";
    	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    	$file = new FilesSimec();
    	$file-> getDownloadArquivo($_REQUEST['arqid']);
    	exit;
    }
    
    public function excluirAnexo()
    {
    	$ftcid = $_REQUEST['ftcid'];
    	$arqid = $_REQUEST['arqid'];
    	$anfid = $_REQUEST['anfid'];    	
    	if($anfid && $arqid){
	    	$sql = "update contratos.anexofatura set anfstatus = 'I' where anfid = $anfid;
	    			update public.arquivo set arqstatus = 'I' where arqid = $arqid";
	    	if($this->executar($sql)){
	    		$this->commit();
	    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Operação realizada com sucesso.';
	    	}else{
	    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Não foi possível realizar a operação.';
	    	}
    	}else{
    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Não foi possível realizar a operação.';
    	}
    	header("Location: contratos.php?modulo=principal/addNotaContratos&acao=A&ftcid=$ftcid");
    	exit;
    }
}