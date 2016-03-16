<?php

/**
 * Modelo de PeriodoPeca
 * @author Rafael J
 */
class PeriodoPeca extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.periodopeca';

	/**
	 * Chave primaria
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
		'ppeperiodo' => null,
		'ppeperiodoinicio' => null,
		'ppeperiodotermino' => null,
		'ppestatus' => null,
		'aneid' => null
	);
	
	/**
     * Cadastrar/editar 
     * @name salvar
     * @author Rafael J
     * @access public
     * @param array $post - Dados do formulario
     * @return array
     */
	public function salvar($post){
		extract($post);
		
		$ppeperiodo = pg_escape_string( $ppeperiodo );
		
		if(!empty($ppeperiodoinicio)){
			$ppeperiodoinicio = formata_data_sql(pg_escape_string($ppeperiodoinicio));
		}
		if(!empty($ppeperiodotermino)){
			$ppeperiodotermino = formata_data_sql(pg_escape_string($ppeperiodotermino));
		}
		
		$aneid = pg_escape_string($aneid);
		$ipaid = pg_escape_string($ipaid);
		
		//caso a checkbox de periodo indetermindao venha checked,
		//exclui as vigências que estão vinculadas ao anexo
		if($post['ppeperiodo'] == 'S'){
			$this->excluir_periodos_anexo($aneid);
			$sql = "INSERT INTO $this->stNomeTabela(ppeperiodo,aneid) ";
			$sql .= "VALUES('{$ppeperiodo}','{$aneid}') ";
		}
		else{
			$this->excluir_periodo_indeterminado($aneid);
			$sql = "INSERT INTO $this->stNomeTabela(ppeperiodo,ppeperiodoinicio,ppeperiodotermino,aneid) ";
			$sql .= "VALUES('{$ppeperiodo}','{$ppeperiodoinicio}','{$ppeperiodotermino}','{$aneid}') ";
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
     * Pega a qtd de periodos de um anexo
     * @name pega_qtd_periodos
     * @author Rafael J
     * @access public
     * @param int $aneid - Identificador do registro pai
     * @return int
     */
	public function pega_qtd_periodos($aneid){
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
     * @author Rafael J
     * @param int $aneid - identificador do registro pai
     * @access public
     * @return void
     */
	public function listar_por_anexo($aneid){
		
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			
			$sql = " SELECT
					 '<center>
                  	  <a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){ excluirPeriodo(\''||periodo.ppeid||'\'); }\">
                  	  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir periodo\">
                  	  </a>
                  	  </center>' AS acao, 
					 
					 CASE WHEN periodo.ppeperiodo = 'S' 
	                  	  THEN 'Indeterminado'	
	                      ELSE TO_CHAR(periodo.ppeperiodoinicio,'DD/MM/YYYY')||' a '||TO_CHAR(periodo.ppeperiodotermino,'DD/MM/YYYY') 
	                 END AS vigencia
	                 FROM $this->stNomeTabela periodo
					 WHERE periodo.aneid='{$aneid}' AND periodo.ppestatus='A'
					 ORDER BY periodo.ppeid ";
			$cabecalho = array('A&ccedil;&atilde;o','Per&iacute;odo de Pe&ccedil;a Publicit&aacute;ria');
			$this->monta_lista($sql,$cabecalho,10,50,false,"center");
		}
	}
	
	
	/**
     * Verifica se a data final é menor que a inicial
     * @name verifica_data
     * @author Rafael J
     * @access public
     * @param array $post - array com os dados
     * @return string
     */
	public function verifica_data($post){
		if(!empty($post['ppeperiodoinicio']) && !empty($post['ppeperiodotermino'])){
			
			$auxInicial = explode('/',$post['ppeperiodoinicio']);
			$auxFinal = explode('/',$post['ppeperiodotermino']);
			
			$post['ppeperiodoinicio'] = strtotime(formata_data_sql($post['ppeperiodoinicio']));
			$post['ppeperiodotermino'] = strtotime(formata_data_sql($post['ppeperiodotermino']));
			
			$erro = '';
			
			if($auxInicial[2] < 2000){
				$erro = 'anoinicialinvalido';
			}
			else if($auxFinal[2] < 2000){
				$erro = 'anofinalinvalido';
			}
			
			if($post['ppeperiodotermino'] < $post['ppeperiodoinicio']){
				$erro = 'datamenor';
			}
			
			return $erro;
		}
	}
	
	/**
     * Exclui os periodos de um anexo
     * @name excluir_periodos_anexo
     * @author Rafael J
     * @access public
     * @param int $aneid - Identificador do registro pai
     * @return void
     */
	public function excluir_periodos_anexo($aneid){
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			$sql = "DELETE FROM $this->stNomeTabela WHERE aneid='{$aneid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	/**
     * Exclui o periodo indeterminado de um anexo
     * @name excluir_periodo_indeterminado
     * @author Rafael J
     * @access public
     * @param int $aneid - Identificador do registro pai
     * @return void
     */
	public function excluir_periodo_indeterminado($aneid){
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			$sql = "DELETE FROM $this->stNomeTabela WHERE aneid='{$aneid}' AND ppeperiodo='S' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	/**
     * Inativa um periodo
     * @name inativar
     * @author Rafael J
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