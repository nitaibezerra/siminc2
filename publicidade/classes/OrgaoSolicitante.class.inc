<?php

/**
 * Modelo de Órgão Solicitante
 * @author Alysson Rafael
 */
class OrgaoSolicitante extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.orgao';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('orgid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'orgsg' => null,
		'orgdsc' => null,
		'orgstatus' => null
	);
	
	
	/**
     * Listagem de dados
     * @name listar_por_filtro
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return void
     */
	public function listar_por_filtro($post){
		
		//$where = " WHERE orgao.orgstatus='A' ";
		$where = " WHERE orgao.orgstatus is not null ";
		if(!empty($post['orgsg'])){
			$post['orgsg'] = tirar_acentos(strtoupper(pg_escape_string($post['orgsg'])));
			$where .= " AND(TRANSLATE(orgao.orgsg, 'áéíóúàèìòùãõâêîôôäëïöüçÁÉÍÓÚÀÈÌÒÙÃÕÂÊÎÔÛÄËÏÖÜÇ', 'aeiouaeiouaoaeiooaeioucAEIOUAEIOUAOAEIOOAEIOUC') ILIKE '%{$post['orgsg']}%' OR orgao.orgsg ILIKE '%{$post['orgsg']}%') ";
			//$where .= " AND orgao.orgsg ILIKE '%{$post['orgsg']}%' OR orgao.orgsg ILIKE '%{$post['orgsg']}%') ";
		}
		if(!empty($post['orgdsc'])){
			$post['orgdsc'] = tirar_acentos(pg_escape_string($post['orgdsc']));
			$where .= " AND(TRANSLATE(orgao.orgdsc, 'áéíóúàèìòùãõâêîôôäëïöüçÁÉÍÓÚÀÈÌÒÙÃÕÂÊÎÔÛÄËÏÖÜÇ', 'aeiouaeiouaoaeiooaeioucAEIOUAEIOUAOAEIOOAEIOUC') ILIKE '%{$post['orgdsc']}%' OR orgao.orgdsc ILIKE '%{$post['orgdsc']}%') ";
			//$where .= " AND orgao.orgdsc ILIKE '%{$post['orgdsc']}%' OR orgao.orgdsc ILIKE '%{$post['orgdsc']}%') ";
		}
		
		$sql = "SELECT 
				  '<center>' || 
				  CASE WHEN orgao.orgtp = 'E' THEN ''
				  ELSE
				      '<a style=\"cursor:pointer;\" onclick=\"detalhar(\''||orgao.orgid||'\');\">
	                  <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
	                  </a>
	                  <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM publicidade.campanha campanha WHERE campanha.orgid=orgao.orgid AND campanha.camstatus='A') = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){remover(\''||orgao.orgid||'\');}' ELSE 'alert(\'Registro n�o pode ser exclu�do!\');' END || '\">
	                  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
	                  </a>'		
				  END
                  || '</center>' as acao,
                  orgao.orgsg,
                  orgao.orgdsc,
                  CASE WHEN orgao.orgstatus = 'A' THEN 'Ativo' ELSE 'Inativo' END AS orgstatus
                  
                  FROM publicidade.vworgao orgao 
                  {$where} 
                  ORDER BY orgao.orgdsc ";
		
		$cabecalho = array('A��o','Sigla','�rg�o','Status');
		$this->monta_lista($sql,$cabecalho,10,50,false,"center",'S','','',array('center','center','center','center'));
		
	}
	
	/**
     * Gerar a combo de status
     * @name monta_combo_status
     * @author Alysson Rafael
     * @param string $orgstatus - Valor que receberá o selected
     * @param string $enable - Indica de o campo será habilitado ou não
     * @access public
     * @return void
     */
	public function monta_combo_status($orgstatus='',$enable='S'){
		
		$situacoes = array(array('codigo'=>'A','descricao'=>'Ativo'),array('codigo'=>'I','descricao'=>'Inativo'));

		$this->monta_combo('orgstatus',$situacoes,$enable,'Selecione...','','','','','S','orgstatus','',$orgstatus);
		
	}
	
	
	/**
     * Verifica se o registro já existe
     * @name verifica_unico
     * @author Alysson Rafael
     * @param array $post - Dados do formulário
     * @access public
     * @return bool
     */
	public function verifica_unico($post){
		if(!empty($post['orgsg'])){
			$post['orgsg'] = strtoupper(pg_escape_string($post['orgsg']));
			
			$sql = "SELECT orgao.orgid FROM publicidade.vworgao orgao ";
			$sql .= "WHERE orgao.orgsg='{$post['orgsg']}' AND orgao.orgstatus='A' ";
			
			//caso seja atualização, exclui o registro atual da verificação
			if(!empty($post['orgid'])){
				$post['orgid'] = pg_escape_string($post['orgid']);
				$sql .= "AND orgao.orgid <> '{$post['orgid']}' ";
			}
			
			$orgid = $this->pegaUm($sql);
			
			if($orgid >= 1){
				return true;
			}
			else{
				return false;
			}
		}
	}
	
	
	/**
     * Cadastrar/editar 
     * @name salvar
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function salvar($post){
		extract($post);
		
		$orgsg = strtoupper(pg_escape_string($orgsg));
		$orgdsc = pg_escape_string($orgdsc);
		$orgstatus = pg_escape_string($orgstatus);
		$orgresp = pg_escape_string($orgresp);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($orgid)){
			$sql = "INSERT INTO $this->stNomeTabela(orgsg,orgdsc,orgstatus) ";
			$sql .= "VALUES('{$orgsg}','{$orgdsc}','A') ";
			
		}
		else if(!empty($orgid)){
			$orgid = pg_escape_string($orgid);
			$sql = "UPDATE $this->stNomeTabela SET orgsg='{$orgsg}',orgdsc='{$orgdsc}',";
			$sql .= "orgstatus='{$orgstatus}' WHERE orgid='{$orgid}' ";
		}
		
		$sql .= "RETURNING orgid ";
		
       	$orgid = $this->pegaUm($sql);

       	if(!empty($orgid)){
       		$this->commit();
       		return array(0=>true,1=>$orgid,2=>'listaOrgaoSolicitante');
       	}
       	else{
       		return array(0=>false,1=>$orgid,2=>'listaOrgaoSolicitante');
       	}
		
	}
	
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $orgid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($orgid){
		if(!empty($orgid)){
			$orgid = pg_escape_string($orgid);
			$sql = "SELECT orgao.orgid,
						   orgao.orgsg,
						   orgao.orgdsc,
						   orgao.orgstatus
						   FROM publicidade.vworgao orgao WHERE orgao.orgid='{$orgid}' ";
			
			return $this->pegaLinha($sql);
		}
	}
	
	
	/**
     * Inativa registro
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param int $orgid - Identificador do registro 
     * @return void
     */
	public function inativar($orgid){
		if(!empty($orgid)){
			$orgid = pg_escape_string($orgid);
			$sql = "UPDATE $this->stNomeTabela SET orgstatus='I' WHERE orgid='{$orgid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	
	/**
     * Gerar a combo de Órgãos
     * @name monta_combo_orgao
     * @author Alysson Rafael
     * @access public
     * @param string $orgid       - Qual option receberá o selected
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @param string $origem      - De onde foi chamada a função
     * @return void
     */
	public function monta_combo_orgao($orgid='',$obrigatorio='N',$origem=''){
		
		$sql = "SELECT orgid AS codigo,orgdsc AS descricao FROM publicidade.vworgao ";
		$sql .= "WHERE orgstatus='A' ORDER BY orgdsc ";

		$resultado = $this->carregar($sql);

		$this->monta_combo('orgid',$resultado,'S','Selecione...','','','','',$obrigatorio,'orgid','',$orgid);
	}
	

	
}