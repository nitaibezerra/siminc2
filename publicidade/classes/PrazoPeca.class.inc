<?php

/**
 * Modelo de PrazoPeca
 * @author Alysson Rafael
 */
class PrazoPeca extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.prazopeca';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('ppeid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'ppeprazo' => null,
		'ppeprazoinicio' => null,
		'ppeprazotermino' => null,
		'ppestatus' => null,
		'aneid' => null
	);
	
	
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
		
		$ppeprazo = pg_escape_string($ppeprazo);
		
		if(!empty($ppeprazoinicio)){
			$ppeprazoinicio = formata_data_sql(pg_escape_string($ppeprazoinicio));
		}
		if(!empty($ppeprazotermino)){
			$ppeprazotermino = formata_data_sql(pg_escape_string($ppeprazotermino));
		}
		
		$aneid = pg_escape_string($aneid);
		$ipaid = pg_escape_string($ipaid);
		
		//caso a checkbox de prazo indetermindao venha checked,
		//exclui as vigências que estão vinculadas ao anexo
		if($post['ppeprazo'] == 'S'){
			$this->excluir_prazos_anexo($aneid);
			$sql = "INSERT INTO $this->stNomeTabela(ppeprazo,aneid) ";
			$sql .= "VALUES('{$ppeprazo}','{$aneid}') ";
		}
		else{
			$this->excluir_prazo_indeterminado($aneid);
			$sql = "INSERT INTO $this->stNomeTabela(ppeprazo,ppeprazoinicio,ppeprazotermino,aneid) ";
			$sql .= "VALUES('{$ppeprazo}','{$ppeprazoinicio}','{$ppeprazotermino}','{$aneid}') ";
		}
		
		$sql .= "RETURNING ppeid ";
		
       	$ppeid = $this->pegaUm($sql);

       	if(!empty($ppeid)){
       		$this->commit();
       		return array(0=>true,1=>$ppeid,2=>$ipaid,3=>'cadAnexo');
       	}
       	else{
       		return array(0=>false,1=>$ppeid,2=>$ipaid,3=>'cadAnexo');
       	}
		
	}
	
	
	/**
     * Pega a qtd de prazos de um anexo
     * @name pega_qtd_prazos
     * @author Alysson Rafael
     * @access public
     * @param int $aneid - Identificador do registro pai
     * @return int
     */
	public function pega_qtd_prazos($aneid){
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela WHERE aneid='{$aneid}' AND ppestatus='A' ";
			$resultado = $this->pegaUm($sql);
			return $resultado;
		}
	}
	
	
	/**
     * Listagem dos dados
     * @name listar_por_anexo
     * @author Alysson Rafael
     * @param int $aneid - identificador do registro pai
     * @access public
     * @return void
     */
	public function listar_por_anexo($aneid){
		
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			
			$sql = " SELECT
					 '<center>
                  	  <a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){ excluirPrazo(\''||prazo.ppeid||'\'); }\">
                  	  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir prazo\">
                  	  </a>
                  	  </center>' AS acao, 
					 
					 CASE WHEN prazo.ppeprazo = 'S' 
	                  	  THEN 'Indeterminado'	
	                      ELSE TO_CHAR(prazo.ppeprazoinicio,'DD/MM/YYYY')||' a '||TO_CHAR(prazo.ppeprazotermino,'DD/MM/YYYY') 
	                 END AS vigencia
	                 FROM $this->stNomeTabela prazo
					 WHERE prazo.aneid='{$aneid}' AND prazo.ppestatus='A'
					 ORDER BY prazo.ppeid ";
			$cabecalho = array('A&ccedil;&atilde;o','Prazo Autorizado de Exibi&ccedil;&atilde;o');
			$this->monta_lista($sql,$cabecalho,10,50,false,"center");
		}
	}
	
	
	/**
     * Verifica se a data final é menor que a inicial
     * @name verifica_data
     * @author Alysson Rafael
     * @access public
     * @param array $post - array com os dados
     * @return string
     */
	public function verifica_data($post){
		if(!empty($post['ppeprazoinicio']) && !empty($post['ppeprazotermino'])){
			
			$auxInicial = explode('/',$post['ppeprazoinicio']);
			$auxFinal = explode('/',$post['ppeprazotermino']);
			
			$post['ppeprazoinicio'] = strtotime(formata_data_sql($post['ppeprazoinicio']));
			$post['ppeprazotermino'] = strtotime(formata_data_sql($post['ppeprazotermino']));
			
			$erro = '';
			
			if($auxInicial[2] < 2000){
				$erro = 'anoinicialinvalido';
			}
			else if($auxFinal[2] < 2000){
				$erro = 'anofinalinvalido';
			}
			
			if($post['ppeprazotermino'] < $post['ppeprazoinicio']){
				$erro = 'datamenor';
			}
			
			return $erro;
		}
	}
	
	/**
     * Exclui os prazos de um anexo
     * @name excluir_prazos_anexo
     * @author Alysson Rafael
     * @access public
     * @param int $aneid - Identificador do registro pai
     * @return void
     */
	public function excluir_prazos_anexo($aneid){
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			$sql = "DELETE FROM $this->stNomeTabela WHERE aneid='{$aneid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	/**
     * Exclui o prazo indeterminado de um anexo
     * @name excluir_prazo_indeterminado
     * @author Alysson Rafael
     * @access public
     * @param int $aneid - Identificador do registro pai
     * @return void
     */
	public function excluir_prazo_indeterminado($aneid){
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			$sql = "DELETE FROM $this->stNomeTabela WHERE aneid='{$aneid}' AND ppeprazo='S' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	/**
     * Inativa um prazo
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param int $ppeid - Identificador do registro
     * @return void
     */
	public function inativar($ppeid){
		if(!empty($ppeid)){
			$ppeid = pg_escape_string($ppeid);
			$sql = "UPDATE $this->stNomeTabela SET ppestatus='I' WHERE ppeid='{$ppeid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}

	
}