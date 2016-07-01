<?php

/**
 * Modelo de PrazoVeiculacao
 * @author Alysson Rafael
 */
class PrazoVeiculacao extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.prazoveiculacao';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('pveid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'pveprazoinicio' => null,
		'pveprazotermino' => null,
		'pvestatus' => null,
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
		
		if(!empty($pveprazoinicio)){
			$pveprazoinicio = formata_data_sql(pg_escape_string($pveprazoinicio));
		}
		if(!empty($pveprazotermino)){
			$pveprazotermino = formata_data_sql(pg_escape_string($pveprazotermino));
		}
		
		$aneid = pg_escape_string($aneid);
		$ipaid = pg_escape_string($ipaid);
		
		$sql = "INSERT INTO $this->stNomeTabela(pveprazoinicio,pveprazotermino,aneid) ";
		$sql .= "VALUES('{$pveprazoinicio}','{$pveprazotermino}','{$aneid}') ";
		
		
		$sql .= "RETURNING pveid ";
		
       	$pveid = $this->pegaUm($sql);

       	if(!empty($pveid)){
       		$this->commit();
       		return array(0=>true,1=>$pveid,2=>$ipaid,3=>'cadAnexo');
       	}
       	else{
       		return array(0=>false,1=>$pveid,2=>$ipaid,3=>'cadAnexo');
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
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela WHERE aneid='{$aneid}' AND pvestatus='A' ";
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
                  	  <a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){ excluirPrazoVeiculacao(\''||prazo.pveid||'\'); }\">
                  	  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir prazo\">
                  	  </a>
                  	  </center>' AS acao, 
					 TO_CHAR(prazo.pveprazoinicio,'DD/MM/YYYY')||' a '||TO_CHAR(prazo.pveprazotermino,'DD/MM/YYYY') AS vigencia
	                 FROM $this->stNomeTabela prazo
					 WHERE prazo.aneid='{$aneid}' AND prazo.pvestatus='A'
					 ORDER BY prazo.pveid ";
			$cabecalho = array('A&ccedil;&atilde;o','Per&iacute;odo de Veicula&ccedil;&atilde;o');
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
		if(!empty($post['pveprazoinicio']) && !empty($post['pveprazotermino'])){
			
			$auxInicial = explode('/',$post['pveprazoinicio']);
			$auxFinal = explode('/',$post['pveprazotermino']);
			
			$post['pveprazoinicio'] = strtotime(formata_data_sql($post['pveprazoinicio']));
			$post['pveprazotermino'] = strtotime(formata_data_sql($post['pveprazotermino']));
			
			$erro = '';
			
			if($auxInicial[2] < 2000){
				$erro = 'anoinicialinvalidoveiculacao';
			}
			else if($auxFinal[2] < 2000){
				$erro = 'anofinalinvalidoveiculacao';
			}
			
			if($post['pveprazotermino'] < $post['pveprazoinicio']){
				$erro = 'datamenorveiculacao';
			}
			
			return $erro;
		}
	}
	
	
	/**
     * Inativa um prazo
     * @name inativar
     * @author Alysson Rafael
     * @access public
     * @param int $pveid - Identificador do registro
     * @return void
     */
	public function inativar($pveid){
		if(!empty($pveid)){
			$pveid = pg_escape_string($pveid);
			$sql = "UPDATE $this->stNomeTabela SET pvestatus='I' WHERE pveid='{$pveid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}

	
}