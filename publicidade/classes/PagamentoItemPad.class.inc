<?php

/**
 * Modelo de Pagamento de Item da PAD
 * @author Alysson Rafael
 */
class PagamentoItemPad extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.pagamentoitempad';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('pipid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'ipaid' => null,
		'pipdatapagamento' => null,
		'pipnumordembancaria' => null,
		'pipvalorfaturado' => null,
		'pipstatus' => null
	);
	
	/**
	 * Atributo que define se a edição será bloqueada 
	 *
	 * @name $bloqueiaEdicao
	 * @var bool
	 * @access protected
	 */
	protected $bloqueiaEdicao = false;
	
	/**
     * Bloqueia o registro para edição
     * 
     * @name bloquearEdicao
     * @author Alysson Rafael
     * @access public
     * @return void
     */
	public function bloquearEdicao(){
		$this->bloqueiaEdicao = true;
	}
	
	/**
     * Verifica se o registro está bloqueado para edição
     * 
     * @name checkBloqueioEdicao
     * @author Alysson Rafael
     * @access public
     * @return bool
     */
	public function checkBloqueioEdicao(){
		return !!$this->bloqueiaEdicao;
	}
	
	public function carregar_registro_por_item($ipaid){
		if(!empty($ipaid)){
			$ipaid = pg_escape_string($ipaid);
			$sql = "SELECT pg.pipid,
						   TO_CHAR(pg.pipdatapagamento,'DD/MM/YYYY') AS pipdatapagamento,
						   pg.pipnumordembancaria,
						   pg.pipvalorfaturado 
						   FROM $this->stNomeTabela pg 
						   WHERE pg.ipaid='{$ipaid}' ";
			
			return $this->pegaLinha($sql);
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
		
		$pipdatapagamento = formata_data_sql(pg_escape_string($post['pipdatapagamento']));
		$pipnumordembancaria = pg_escape_string($post['pipnumordembancaria']);
		$pipvalorfaturado = str_replace(array('.',','),array('','.'),pg_escape_string($post['pipvalorfaturado']));
		$ipaid = pg_escape_string($ipaid);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($pipid)){
			$sql = "INSERT INTO $this->stNomeTabela(ipaid,pipdatapagamento,pipnumordembancaria,pipvalorfaturado) ";
			$sql .= "VALUES('{$ipaid}','{$pipdatapagamento}','{$pipnumordembancaria}','{$pipvalorfaturado}') ";
		}
		else if(!empty($pipid)){
			$pipid = pg_escape_string($pipid);
			$sql = "UPDATE $this->stNomeTabela SET ipaid='{$ipaid}',pipdatapagamento='{$pipdatapagamento}',";
			$sql .= "pipnumordembancaria='{$pipnumordembancaria}',pipvalorfaturado='{$pipvalorfaturado}' ";
			$sql .= "WHERE ipaid='{$ipaid}' ";
		}
		
		$sql .= "RETURNING pipid ";
		
       	$pipid = $this->pegaUm($sql);

       	if(!empty($pipid)){
       		$this->commit();
       		return array(0=>true,1=>$pipid,2=>$ipaid,3=>$id,4=>'cadItemPad');
       	}
       	else{
       		return array(0=>false,1=>$pipid,2=>$ipaid,3=>$id,4=>'pagarItemPad');
       	}
		
	}
	
	
	/**
     * Verifica o ano 
     * @name verifica_data
     * @author Alysson Rafael
     * @access public
     * @param array $post - array com os dados
     * @return string
     */
	public function verifica_data($post){
		if(!empty($post['pipdatapagamento'])){
			
			$aux = explode('/',$post['pipdatapagamento']);
			
			$erro = '';
			
			if($aux[2] < 2000){
				$erro = 'anoinvalido';
			}
			
			return $erro;
		}
	}
	
	

	
}