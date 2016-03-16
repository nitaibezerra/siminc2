<?php
class MaisMedicosResponsabilidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.maismedicosresponsabilidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mmrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mmrid' => null, 
    									'uniid' => null,
									  	'entiduniversidade' => null,
							    		'entidresponsavel' => null,
							    		'usucpfcadastro' => null,
							    		'mmrdatainclusao' => null,
    									'mmrtipo' => null,
									  );
    
    function salvarMaisMedicosResponsabilidade()
    {
    	$_POST['mmrdatainclusao'] = date("Y-m-d H:m:s");
    	$_POST['mmrtipo'] = "T";
    	$this->popularDadosObjeto($_POST);
    	$msmid = $this->salvar();
    	if($msmid){
    		$this->commit();
    		$_SESSION['maismedicos']['maismedicosresponsabilidade']['alert'] = "Operação realizada com sucesso.";
    	}else{
    		$_SESSION['maismedicos']['maismedicosresponsabilidade']['alert'] = "Não foi possível realizar a operação.";
    	}
    	if($_POST['uniid']){
    		header("Location: maismedicos.php?modulo=principal/maisMedicosUnidade&acao=A");
    	}else{
    		header("Location: maismedicos.php?modulo=principal/maisMedicos&acao=A");
    	}
    	exit;
    }
    
    public function recuperarPorEntid($entid,$uniid = null,$mmrtipo = "T")
    {
    	if($uniid){    		
    		$sql = "SELECT * FROM $this->stNomeTabela where entidresponsavel = $entid and uniid = $uniid and mmrtipo = '$mmrtipo'";
    	}else{
    		$sql = "SELECT * FROM $this->stNomeTabela where entidresponsavel = $entid and uniid is null and mmrtipo = '$mmrtipo'";
    	}
    	$arrDados = $this->pegaLinha( $sql );
    	if($arrDados){
    		$this->carregarPorId($arrDados['mmrid']);
    	}
    	return $arrDados;
    }
    
    public function listaTutores($entid = null, $uniid = null,$tipo = "T")
    {
    	if(!$entid && !$uniid){
    		echo "Não existem tutores cadastrados.";
    		return false;
    	}
    	if($entid){
    		$where = "entiduniversidade = $entid";
    		$excluir = "tut.entiduniversidade";
    	}
    	if($uniid){
    		$where = "uniid = $uniid";
    		$excluir = "uniid";
    	}
    	
    	$sql = "select
    				'<img src=\"/imagens/alterar.gif\" class=\"link\" onclick=\"alterarTutor(' || tut.entidresponsavel || ',\'$tipo\')\" />
    				 <img src=\"/imagens/excluir.gif\" class=\"link\" onclick=\"excluirTutor(' || tut.mmrid || ',' || $excluir || ',\'$tipo\')\" />' as acao,
    				 case when mmrtitular is true
    					then '<input type=\"radio\" name=\"tuttitular$tipo\" onclick=\"atribuirTitularidade(' || tut.entidresponsavel || ',' || $excluir || ',\'$tipo\')\"  checked=\"checked\" value=\"1\" />'
    					else '<input type=\"radio\" name=\"tuttitular$tipo\" onclick=\"atribuirTitularidade(' || tut.entidresponsavel || ',' || $excluir || ',\'$tipo\')\" value=\"1\"  />'
    				end as titular,
    				ent.entnumcpfcnpj,
    				ent.entnome,
    				ent.entemail
    			from
    				$this->stNomeTabela tut
    			inner join
    				entidade.entidade ent ON ent.entid = tut.entidresponsavel
    			where
    				ent.entstatus= 'A'
    			and
    				mmrtipo = '$tipo'
    			and 
    				$where";
    	$arrCab = array("Ação","Titular","CPF","Nome","Email");
    	$this->monta_lista_simples($sql, $arrCab,1000,1000);
    }
    
    public function excluirTutor()
    {
    	$mmrid = $_POST['mmrid'];
    	$entid = $_POST['entid'];
    	$uniid = $_POST['uniid'];
    	$tipo = $_POST['tipo'] ? $_POST['tipo'] : "T";
    	$sql = "delete from $this->stNomeTabela where mmrid = $mmrid";
    	$this->executar($sql);
    	$this->commit();
    	$this->listaTutores($entid,$uniid,$tipo);
    }
    
    public function listaTutoresAjax()
    {
    	$entid = $_POST['entid'];
    	$uniid = $_POST['uniid'];
    	$tipo = $_POST['tipo'] ? $_POST['tipo'] : "T";
    	$this->listaTutores($entid,$uniid,$tipo);
    }
    
    public function atribuirTitularidade()
    {
    	$entiduniversidade = $_POST['entiduniversidade'];
    	$uniid = $_POST['uniid'];
    	$entidresponsavel   = $_POST['entidtutor'];
    	$mmrtipo  = $_POST['mmrtipo'] ? $_POST['mmrtipo'] : "T";
    	if($entiduniversidade){
    		$sql = "update $this->stNomeTabela set mmrtitular = null where entiduniversidade = $entiduniversidade and mmrtipo = '$mmrtipo';
    		update $this->stNomeTabela set mmrtitular = true where entidresponsavel = $entidresponsavel and uniid is null and mmrtipo = '$mmrtipo';";
    	}
    	
    	if($uniid){
    		$sql = "update $this->stNomeTabela set mmrtitular = null where uniid = $uniid and mmrtipo = '$mmrtipo;
    				update $this->stNomeTabela set mmrtitular = true where entidresponsavel = $entidresponsavel and uniid = $uniid and mmrtipo = '$mmrtipo';";
    	}
    	$this->executar($sql);
    	$this->commit();
    	die;
    }
}