<?php

/**
 * Modelo de ContratoAditivo
 * @author Alysson Rafael
 */
class ContratoAditivo extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.contratoaditivo';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('ctaid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'cttid' => null,
		'ctanumaditivo' => null,
		'ctavigenciainicio' => null,
		'ctavigenciatermino' => null,
		'ctostatus' => null
	);
	
	
	/**
     * Pega o último aditivo do contrato
     * @name pega_ultimo_numero
     * @author Alysson Rafael
     * @param int $cttid - identificador do registro
     * @access public
     * @return int
     */
	public function pega_ultimo_numero($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "SELECT MAX(ctanumaditivo) FROM $this->stNomeTabela WHERE cttid='{$cttid}' ";
			$ctanumaditivo = $this->pegaUm($sql);
			return $ctanumaditivo;
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
		
		$cttid = pg_escape_string($cttid);
		$ctanumaditivo = pg_escape_string($ctanumaditivo);
		
		$ctavigenciainicio = pg_escape_string($ctavigenciainicio);
		$ctavigenciainicio = formata_data_sql($ctavigenciainicio);
		
		$ctavigenciatermino = pg_escape_string($ctavigenciatermino);
		$ctavigenciatermino = formata_data_sql($ctavigenciatermino);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($ctaid)){
			$sql = "INSERT INTO $this->stNomeTabela(cttid,ctanumaditivo,ctavigenciainicio,ctavigenciatermino) ";
			$sql .= "VALUES('{$cttid}','{$ctanumaditivo}','{$ctavigenciainicio}','{$ctavigenciatermino}') ";
		}
		else if(!empty($ctaid)){
			$ctaid = pg_escape_string($ctaid);
			$sql = "UPDATE $this->stNomeTabela SET cttid='{$cttid}',ctanumaditivo='{$ctanumaditivo}',";
			$sql .= "ctavigenciainicio='{$ctavigenciainicio}',ctavigenciatermino='{$ctavigenciatermino}' ";
			$sql .= "WHERE ctaid='{$ctaid}' ";
		}
		
		$sql .= "RETURNING ctaid ";
		
       	$ctaid = $this->pegaUm($sql);

       	if(!empty($ctaid)){
       		$this->commit();
       		return array(0=>true,1=>$cttid,2=>$ctaid,3=>'cadContrato');
       	}
       	else{
       		return array(0=>false,1=>$cttid,2=>$ctaid,3=>'cadContrato');
       	}
		
	}
	
	
	/**
     * Listagem dos dados
     * @name listar_por_id
     * @author Alysson Rafael
     * @param int $cttid - identificador do registro pai
     * @access public
     * @return void
     */
	public function listar_por_id($cttid){
		
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			
			$where = " WHERE contadi.cttid='{$cttid}' ";
				
			//só mostra este trecho caso o fluxo NÃO seja advindo do ícone visualizar
			if($_SESSION['visualizar'] != 'S'){
				$sql = "SELECT 
					   '<center>
	                    <a style=\"cursor:pointer;\" onclick=\"detalharAditivo(\''||contadi.ctaid||'\');\">
	                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Editar aditivo\">
	                    </a>
	                    </center>' as acao,";
			}
			else if($_SESSION['visualizar'] == 'S'){
				$sql = "SELECT 
					   '' as acao,";
			}
			
			$sql .= " contadi.ctanumaditivo AS ctanumaditivo,
	                  TO_CHAR(contadi.ctavigenciainicio,'DD/MM/YYYY')||' a '||TO_CHAR(contadi.ctavigenciatermino,'DD/MM/YYYY')
	                  
	                  FROM $this->stNomeTabela contadi 
	                  {$where} 
	                  ORDER BY contadi.ctanumaditivo ";
			
			$cabecalho = array('A&ccedil;&atilde;o','N&ordm; Aditivo','Vig&ecirc;ncia');
			$this->monta_lista($sql,$cabecalho,10,50,false,"center",'S','','');
		}
		
		
	}
	
	/**
     * Carrega um registro pelo id
     * @name carrega_registro_por_id
     * @author Alysson Rafael
     * @access public
     * @param int $ctaid - Identificador do registro
     * @return array
     */
	public function carrega_registro_por_id($ctaid){
		if(!empty($ctaid)){
			$ctaid = pg_escape_string($ctaid);
			$sql = "SELECT contadi.ctaid,
						   contadi.ctanumaditivo,
						   TO_CHAR(contadi.ctavigenciainicio,'DD/MM/YYYY') AS ctavigenciainicio,
						   TO_CHAR(contadi.ctavigenciatermino,'DD/MM/YYYY') AS ctavigenciatermino,
						   contadi.ctastatus	
						   FROM $this->stNomeTabela contadi WHERE contadi.ctaid='{$ctaid}' ";
			
			return $this->pegaLinha($sql);
		}
	}
	
	/**
     * Exclui registros filhos
     * @name excluir_aditivos_filhos
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @return void
     */
	public function excluir_aditivos_filhos($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "DELETE FROM $this->stNomeTabela WHERE cttid='{$cttid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	/**
     * Inativa registros filhos
     * @name inativar_aditivos_filhos
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @return void
     */
	public function inativar_aditivos_filhos($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "UPDATE $this->stNomeTabela SET ctastatus='I' WHERE cttid='{$cttid}' ";
			$this->executar($sql);
			$this->commit();
		}
	}
	
	/**
     * Pega a qtd de aditivos de um contrato
     * @name pega_qtd_aditivos
     * @author Alysson Rafael
     * @access public
     * @param int $cttid - Identificador do registro pai
     * @return int
     */
	public function pega_qtd_aditivos($cttid){
		if(!empty($cttid)){
			$cttid = pg_escape_string($cttid);
			$sql = "SELECT COUNT(*) AS qtd FROM $this->stNomeTabela WHERE cttid='{$cttid}' ";
			$resultado = $this->pegaUm($sql);
			return $resultado;
		}
	}
	
	
	/**
     * Verifica se a vigência do aditivo é maior que a do contrato
     * @name verifica_data
     * @author Alysson Rafael
     * @access public
     * @param array $post - array com os dados
     * @return string
     */
	public function verifica_data($post){
		if(!empty($post['ctavigenciainicio']) && !empty($post['ctavigenciatermino'])){
			
			$post['cttvigenciainicio'] = strtotime(formata_data_sql($post['cttvigenciainicio']));
			$post['cttvigenciatermino'] = strtotime(formata_data_sql($post['cttvigenciatermino']));
			
			$auxInicial = explode('/',$post['ctavigenciainicio']);
			$auxFinal = explode('/',$post['ctavigenciatermino']);
			
			$post['ctavigenciainicio'] = strtotime(formata_data_sql($post['ctavigenciainicio']));
			$post['ctavigenciatermino'] = strtotime(formata_data_sql($post['ctavigenciatermino']));
			
			$erro = '';
			
			if($post['qtdAditivos'] <= 0){
				if($post['ctavigenciainicio'] <= $post['cttvigenciatermino']){
					$erro = 'datamenor';
				}
				
			}
			else if($post['qtdAditivos'] > 0){
				$ultimoAditivo = $this->pega_ultimo_aditivo($post);
				
				if(!empty($ultimoAditivo['ctavigenciatermino'])){
					$ultimoAditivo['ctavigenciatermino'] = strtotime($ultimoAditivo['ctavigenciatermino']);
					if($post['ctavigenciainicio'] <= $ultimoAditivo['ctavigenciatermino']){
						$erro = 'datamenor';
					}
				}
				else if(empty($ultimoAditivo['ctavigenciatermino'])){
					if($post['ctavigenciainicio'] <= $post['cttvigenciatermino']){
						$erro = 'datamenor';
					}
				}
				
				if(!empty($ultimoAditivo['ctavigenciainicio'])){
					$ultimoAditivo['ctavigenciainicio'] = strtotime($ultimoAditivo['ctavigenciainicio']);
					if($post['ctavigenciatermino'] >= $ultimoAditivo['ctavigenciainicio']){
						$erro = 'datamaior';
					}
				}

			}
			
			//caso não tenha encontrado erro acima
			if(empty($erro)){
				if($post['ctavigenciatermino'] < $post['ctavigenciainicio']){
					$erro = 'datamenor';
				}
			}
			
			if(empty($erro)){
				if($auxFinal[2] < 2000){
					$erro = 'anofinalinvalido';
				}
				else if($auxInicial[2] < 2000){
					$erro = 'anoinicialinvalido';
				}
			}
			
			return $erro;
		}
	}
	
	/**
     * Pega o último aditivo de um contrato
     * @name pega_ultimo_aditivo
     * @author Alysson Rafael
     * @access public
     * @param array $post - Array com os dados do form
     * @return array
     */
	public function pega_ultimo_aditivo($post){
		if(!empty($post['cttid'])){
			$post['cttid'] = pg_escape_string($post['cttid']);
			
			//verifica se é edição de um aditivo
			if(!empty($post['ctaid'])){
				$post['ctaid'] = pg_escape_string($post['ctaid']);
				
				//verifica se existe aditivo anterior e posterior
				$sql = "SELECT ctanumaditivo FROM $this->stNomeTabela WHERE ctaid='{$post['ctaid']}' ";
				$ctanumaditivo = $this->pegaUm($sql);
				
				$sql = "SELECT ctanumaditivo FROM $this->stNomeTabela WHERE ctanumaditivo < '{$ctanumaditivo}' AND cttid='{$post['cttid']}' AND ctastatus='A' ORDER BY ctanumaditivo DESC LIMIT 1 ";
				$ctanumaditivoAnterior = $this->pegaUm($sql);
				if(!empty($ctanumaditivoAnterior)){
					$sql = "SELECT ctavigenciatermino FROM $this->stNomeTabela WHERE ctanumaditivo < '{$ctanumaditivo}' AND cttid='{$post['cttid']}' AND ctastatus='A' ORDER BY ctanumaditivo DESC LIMIT 1 ";
					$ctavigenciatermino = $this->pegaUm($sql);
				}
				
				
				$sql = "SELECT ctanumaditivo FROM $this->stNomeTabela WHERE ctanumaditivo > '{$ctanumaditivo}' AND cttid='{$post['cttid']}' AND ctastatus='A' ORDER BY ctanumaditivo LIMIT 1 ";
				$ctanumaditivoPosterior = $this->pegaUm($sql);
				if(!empty($ctanumaditivoPosterior)){
					$sql = "SELECT ctavigenciainicio FROM $this->stNomeTabela WHERE ctanumaditivo > '{$ctanumaditivo}' AND cttid='{$post['cttid']}' AND ctastatus='A' ORDER BY ctanumaditivo LIMIT 1 ";
					$ctavigenciainicio = $this->pegaUm($sql);
				}
				

			}
			else{
				$sql = "SELECT ctavigenciatermino FROM $this->stNomeTabela WHERE cttid='{$post['cttid']}' AND ctastatus='A' ORDER BY ctaid DESC LIMIT 1 ";
				$ctavigenciatermino = $this->pegaUm($sql);
			}
			
			
			
			return array('ctavigenciatermino'=>$ctavigenciatermino,'ctavigenciainicio'=>$ctavigenciainicio);
			
		}
	}

	
}