<?php

/**
 * Modelo de Fornecedor
 * @author Alysson Rafael
 */
class Fornecedor extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.fornecedor";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('forcpfcnpj');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'forcpfcnpj' => null,
		'fortipo' => null,
		'forrazaosocial' => null,
		'fornomefantasia' => null,
		'foruf' => null,
		'forcid' => null,
		'forcep' => null,
		'forlog' => null,
		'forcomp' => null,
		'fornum' => null,
		'forbai' => null,
		'foremail' => null,
		'dtcadastro' => null
	);
	
	
	
	/**
	 * Lista fornecedores através dos filtros informados
	 * @name listarFornecedorPorFiltro
	 * @author Alysson Rafael
	 * @access public
	 * @param string $forcpfcnpj      - CNPJ/CPF a ser pesquisado
	 * @param string $fortipo         - Indica se é uma pessoa jurídica, pessoa física ou estrangeiro
	 * @param string $forrazaosocial  - Razão social
	 * @param string $fornomefantasia - Nome fantasia
	 * @return void
	 */
	public function listarFornecedorPorFiltro($forcpfcnpj,$fortipo,$forrazaosocial,$fornomefantasia){

		$where = " WHERE 1=1 ";
		if(!empty($forcpfcnpj)){
			
			$forcpfcnpj = str_replace('.', '', $forcpfcnpj);
			$forcpfcnpj = str_replace('/', '', $forcpfcnpj);
			$forcpfcnpj = str_replace('-', '', $forcpfcnpj);
			
			$forcpfcnpj = addslashes($forcpfcnpj);

			$where .= "AND fornecedor.forcpfcnpj='".$forcpfcnpj."' ";
		}
		if(!empty($fortipo)){
			$fortipo = addslashes($fortipo);
			$where .= "AND fornecedor.fortipo='".$fortipo."' ";
		}
		if(!empty($forrazaosocial)){
			$forrazaosocial = addslashes($forrazaosocial);
			$where .= "AND fornecedor.forrazaosocial ILIKE '%".addslashes($forrazaosocial)."%' ";
		}
		if(!empty($fornomefantasia)){
			$fornomefantasia = addslashes($fornomefantasia);
			$where .= "AND fornecedor.fornomefantasia ILIKE '%".addslashes($fornomefantasia)."%' ";
		}

		$sql = "SELECT 
		 			'<center>
                    <a style=\"cursor:pointer;\" onclick=\"detalharFornecedor(\''||fornecedor.forcpfcnpj||'\');\">
                    <img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    </a>
                    <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM sap.bens WHERE bens.forcpfcnpj=fornecedor.forcpfcnpj) = 0 THEN 'if(confirm(\'Deseja excluir o registro?\')){removerFornecedor(\''||fornecedor.forcpfcnpj||'\');}' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    </a>
                    </center>' as acao,
                    CASE
                    	WHEN fornecedor.fortipo = 'PJ' THEN 'Pessoa Jurídica'
                    	WHEN fornecedor.fortipo = 'PF' THEN 'Pessoa Física'
                    	ELSE 'Estrangeiro' 
                    END AS fortipo,
                    
                    CASE 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 14 THEN
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 11 THEN
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 10 , 2)
						ELSE fornecedor.forcpfcnpj||' '
					END as forcpfcnpj,
                    SUBSTR(CAST(fornecedor.forcep AS text),1,2)||'.'||SUBSTR(CAST(fornecedor.forcep AS text),3,3)||'-'||SUBSTR(CAST(fornecedor.forcep AS text),6,3) as forcep,
                    fornecedor.foremail as foremail,
                    '<span style=\"display:none;\">' || fornecedor.dtcadastro || '</span>' || TO_CHAR(fornecedor.dtcadastro,'DD/MM/YYYY') as dtcadastro
                    FROM sap.fornecedor fornecedor ".$where;

		$cabecalho = array('Comando','Tipo','Documento','CEP','E-mail','Data de Cadastro');
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center','center','center'));

	}
	
	

	/**
     * Cadastrar/editar fornecedor
     * @name salvarFornecedor
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function salvarFornecedor($post){
		extract($post);
		
		$forcpfcnpj = str_ireplace('.', '', $forcpfcnpj);
		$forcpfcnpj = str_ireplace('-', '', $forcpfcnpj);
		$forcpfcnpj = str_ireplace('/', '', $forcpfcnpj);
		
		$forcpfcnpj = addslashes($forcpfcnpj);
		$fortipo = addslashes($fortipo);
		$forrazaosocial = addslashes($forrazaosocial);
		$fornomefantasia = addslashes($fornomefantasia);
		
		if(!empty($forcep)){
			$forcep = addslashes($forcep);
			$forcep = str_ireplace('.','',$forcep);
			$forcep = str_ireplace('-','',$forcep);
		}
		else{
			$forcep = 'null';
		}
		
		$forlog = addslashes($forlog);
		$forcomp = addslashes($forcomp);
		$fornum = addslashes($fornum);
		$forbai = addslashes($forbai);
		$forcid = addslashes($forcid);
		$foruf = addslashes($foruf);
		$foremail = addslashes($foremail);
		$operacao = '';
		
		//formata a data
		$dtcadastro = formata_data_sql($dtcadastro);
		
		if(empty($fornum)){
			$fornum = 'null';
		}
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if($edicao == 'N'){
			$operacao = 'adicionar';
			$sql = "INSERT INTO sap.fornecedor(forcpfcnpj,fortipo,forrazaosocial,fornomefantasia,foruf,forcid,forcep,forlog,forcomp,fornum,forbai,foremail,dtcadastro) ";
			$sql .= "VALUES('".$forcpfcnpj."','".$fortipo."','".$forrazaosocial."','".$fornomefantasia."','".$foruf."','".$forcid."',".$forcep.",'".$forlog."','".$forcomp."',".$fornum.",'".$forbai."','".$foremail."','".$dtcadastro."') ";
			
		}
		else if($edicao == 'S'){
			$operacao = 'editar';
			$sql = "UPDATE sap.fornecedor SET forrazaosocial='".$forrazaosocial."',";
			$sql .= "fornomefantasia='".$fornomefantasia."',foruf='".$foruf."',forcid='".$forcid."',forcep=".$forcep.",";
			$sql .= "forlog='".$forlog."',forcomp='".$forcomp."',fornum=".$fornum.",forbai='".$forbai."',foremail='".$foremail."' ";
			$sql .= "WHERE forcpfcnpj='".$forcpfcnpj."' ";
		}
		
		$sql .= " returning forcpfcnpj ";
		
       	$forcpfcnpj = $this->pegaUm($sql);

       	if(!empty($forcpfcnpj)){
       		$this->commit();
       		return array(0=>true,1=>$forcpfcnpj,2=>$operacao);
       	}
       	else{
       		return array(0=>false,1=>$forcpfcnpj,2=>$operacao);
       	}
		
	}
	
	
	/**
	 * Carregar um fornecedor pelo documento
	 * @name carregaFornecedorPorId
	 * @author Alysson Rafael
	 * @access public
	 * @param int $forcpfcnpj - Documento a ser pesquisado
	 * @return array
	 */
	public function carregaFornecedorPorId($forcpfcnpj){

		$forcpfcnpj = addslashes($forcpfcnpj);
		
		$sql = "SELECT 
					CASE 
						WHEN CHAR_LENGTH(CAST(forcpfcnpj AS text)) = 14 THEN
							SUBSTR(CAST(forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(forcpfcnpj AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(forcpfcnpj AS text)) = 11 THEN
							SUBSTR(CAST(forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(forcpfcnpj AS text) , 10 , 2)
						ELSE CAST(forcpfcnpj AS text)
					END,
					fortipo,forrazaosocial,fornomefantasia,foruf,forcid,
					SUBSTR(CAST(forcep AS text) , 1 , 2)||'.'||SUBSTR(CAST(forcep AS text) , 3 , 3)||'-'||SUBSTR(CAST(forcep AS text) , 6 , 3) AS forcep,
					forlog,forcomp,fornum,forbai,foremail,
					TO_CHAR(dtcadastro, 'DD/MM/YYYY') AS dtcadastro ";
		$sql .= "FROM sap.fornecedor WHERE forcpfcnpj='".$forcpfcnpj."' ";
		return $this->pegaLinha($sql);

	}
	
	
	
	
	/**
     * Carregar um fornecedor pelo CPF/CNPJ
     *
     * @name carregaFornecedor
     * @author Alysson Rafael
     * @access public
     * @param string $forcpfcnpj - CPF/CNPJ a ser pesquisado
     * @return array
     */
	public function carregaFornecedor($forcpfcnpj){
		
		$forcpfcnpj = addslashes($forcpfcnpj);
		
		$sql = "SELECT * FROM sap.fornecedor WHERE forcpfcnpj='".$forcpfcnpj."' ";
		$resultado = $this->carregar($sql);
		return $resultado[0];
		
	}
	
	
	
	
	
	/**
	 * Monta sql do filtro de fornecedor
	 * @name filtrarFornecedor
	 * @author Alysson Rafael
	 * @access public
	 * @param string $forcpfcnpj      - CNPJ/CPF a ser pesquisado
	 * @param string $fortipo         - Indica se é uma pessoa jurídica, pessoa física ou estrangeiro
	 * @param string $forrazaosocial  - Razão social
	 * @param string $fornomefantasia - Nome fantasia
	 * @return void
	 */
	public function filtrarFornecedor($forcpfcnpj,$fortipo,$forrazaosocial,$fornomefantasia){

		$sql = "SELECT
					'<center>
						<a style=\"cursor:pointer;\" onclick=\"selecionar(\''||fornecedor.forcpfcnpj||'\');\">
                   	 	<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
                    	</a>	
                    	<a style=\"cursor:pointer;\" onclick=\"detalharFornecedor(\''||fornecedor.forcpfcnpj||'\');\">
                    	<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    	</a>
                    	<a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM sap.bens WHERE bens.forcpfcnpj=fornecedor.forcpfcnpj) = 0 THEN 'if(confirm(\'Deseja excluir o registro?\')){removerFornecedor(\''||fornecedor.forcpfcnpj||'\');}' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
                    	<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    	</a>
					</center>'			
					as comandos,
					
					CASE 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 14 THEN
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 11 THEN
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 10 , 2)
						ELSE fornecedor.forcpfcnpj||' '
					END as forcpfcnpj,
					
					fornecedor.fornomefantasia,
					fornecedor.foruf,
					fornecedor.forcid
					
				FROM 
				$this->stNomeTabela fornecedor
				WHERE 1=1 ";

		if(!empty($forcpfcnpj)){
			
			$forcpfcnpj = str_replace('.', '', $forcpfcnpj);
			$forcpfcnpj = str_replace('/', '', $forcpfcnpj);
			$forcpfcnpj = str_replace('-', '', $forcpfcnpj);
			
			$forcpfcnpj = addslashes($forcpfcnpj);
			
			$sql .= "AND fornecedor.forcpfcnpj='".$forcpfcnpj."' ";
		}
		if(!empty($fortipo)){
			$fortipo = addslashes($fortipo);
			$sql .= "AND fornecedor.fortipo='".$fortipo."' ";
		}
		if(!empty($forrazaosocial)){
			$forrazaosocial = addslashes($forrazaosocial);
			$sql .= "AND fornecedor.forrazaosocial ILIKE '%".$forrazaosocial."%' ";
		}
		if(!empty($fornomefantasia)){
			$fornomefantasia = addslashes($fornomefantasia);
			$sql .= "AND fornecedor.fornomefantasia ILIKE '%".$fornomefantasia."%' ";
		}

		$cabecalho = array('Comando','Documento','Nome/Nome fantasia','UF','Cidade');
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center','center','center'));

	}
	
	
	
	
	/**
	 * Verifica se existe fornecedor cadastrado com o documento informado
	 * @name verificaUnico
	 * @author Alysson Rafael
	 * @access public
	 * @param string $forcpfcnpj - CNPJ/CPF a ser pesquisado
	 * @return bool
	 */
	function verificaUnico($forcpfcnpj){
		
		$forcpfcnpj = str_ireplace('.', '', $forcpfcnpj);
		$forcpfcnpj = str_ireplace('-', '', $forcpfcnpj);
		$forcpfcnpj = str_ireplace('/', '', $forcpfcnpj);
		
		$forcpfcnpj = addslashes($forcpfcnpj);
		
		$sql = "SELECT COUNT(*) AS tot FROM sap.fornecedor WHERE forcpfcnpj='".$forcpfcnpj."' ";
		$resultado = $this->pegaUm($sql);
		if($resultado >= 1){
			return true;
		}
		else{
			return false;
		}
		
	}
	
	
	
	/**
	 * Excluir um fornecedor
	 * @name excluirFornecedor
	 * @author Alysson Rafael
	 * @access public
	 * @param int $forcpfcnpj - Id do registro a ser excluído
	 * @param string $popup - Indica se a chamada veio do popup
	 * @return void
	 */
	public function excluirFornecedor($forcpfcnpj,$popup='N'){
		
		$forcpfcnpj = addslashes($forcpfcnpj);
		
		$sql = "DELETE FROM sap.fornecedor WHERE forcpfcnpj='".$forcpfcnpj."' ";
		$this->executar($sql);
		$this->commit();
		
		if($popup == 'N'){
			direcionar('?modulo=principal/fornecedor/pesquisar&acao=A','Operação realizada com sucesso!');	
		}
		else if($popup == 'S'){
			direcionar('?modulo=principal/fornecedor/pesquisarFornecedor&acao=A&popup=S&viaEntradaBens=S','Operação realizada com sucesso!');
		}
		
	}

	
}