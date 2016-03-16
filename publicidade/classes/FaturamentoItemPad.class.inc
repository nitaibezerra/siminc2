<?php

/**
 * Modelo de Faturamento do Item da PAD
 * @author Diego Oliveira
 */
class FaturamentoItemPad extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.faturamentoitempad';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('fipid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'fipid' => null,
		'ipaid' => null,
		'fipdataftura' => null,
		'fipnumsidoc' => null,
		'fipptres' => null,
		'fipnumfaturaagencia' => null,
		'fipvalortotal' => null,
		'fipnumfaturafornecedor' => null,
		'fipvalorservico' => null,
		'fipstatus' => null,
	);

	/**
	 * Atributo que define se a edição será bloqueada 
	 *
	 * @name $bloqueiaEdicao
	 * @var array
	 * @access protected
	 */
	protected $bloqueiaEdicao = false;
	
	/**
	 * Atributo para armazenar os valores dos atributos que estavam setados antes de salvar no banco
	 *
	 * @name $arAtributosAntesSalvar
	 * @var array
	 * @access protected
	 */
	protected $arAtributosAntesSalvar = array();
	
	/**
     * Bloqueia o registro para edição
     * 
     * @name bloquearEdicao
     * @author Diego Oliveira
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
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
	public function checkBloqueioEdicao(){
		return !!$this->bloqueiaEdicao;
	}	

	/**
     * Implementação do método antesSalvar()
     * 
     * @name antesSalvar
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
	public function antesSalvar(){ 
		if (!$this->bloqueiaEdicao){
			$this->arAtributosAntesSalvar = $this->arAtributos;

			if (stripos($this->fipdataftura, '/') !== false){
				$this->fipdataftura = formata_data_sql($this->fipdataftura);
			}

			$this->fipnumsidoc = preg_replace('/[\/.-]/','',$this->fipnumsidoc); // retira mascara
			//$this->fipptres = preg_replace('/[\/.-]/','',$this->fipptres); // retira mascara
			
			if (stripos($this->fipvalortotal, ',') !== false){
				$this->fipvalortotal = desformata_valor($this->fipvalortotal);
			}
			if (stripos($this->fipvalorservico, ',') !== false){
				$this->fipvalorservico = desformata_valor($this->fipvalorservico);
			}

			return true;
		}
		
		return false; 
	}

	/**
     * Implementação do método depoisSalvar()
     * 
     * @name depoisSalvar
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
	public function depoisSalvar(){ 
		$this->fipdataftura = $this->arAtributosAntesSalvar['fipdataftura'];
		$this->fipnumsidoc = $this->arAtributosAntesSalvar['fipnumsidoc'];
		//$this->fipptres = $this->arAtributosAntesSalvar['fipptres'];
		$this->fipvalortotal = $this->arAtributosAntesSalvar['fipvalortotal'];
		$this->fipvalorservico = $this->arAtributosAntesSalvar['fipvalorservico'];

		return true;
	}
	
	/**
     * Carrega os dados para o objeto da ultimo (unica) faturamento do item da PAD
     * 
     * @name carregaFaturamentoIdItem
     * @author Diego Oliveira
     * @param int $ipaid id do item da PAD
     * @access public
     * @return void
     */
	public function carregaFaturamentoIdItem($ipaid){
		if (is_numeric($ipaid)){
			$ipaid = pg_escape_string($ipaid);
			$sql = " SELECT * FROM {$this->stNomeTabela} "
				 . " WHERE ipaid = {$ipaid} "
				 . " ORDER BY ipaid DESC; ";

			$dados = $this->pegaLinha($sql);
			
			if (is_array($dados)){
				$this->popularDadosObjeto($dados);
			}
		};
	}

	/**
     * Atualiza os campos de valor total e valor do serviço com base no valor da glosa
     * 
     * @name atualizaVrFaturamento
     * @author Diego Oliveira
     * @param float $_valor valor da glosa
     * @access public
     * @return void
     */
	public function atualizaVrFaturamento($_valor){
		if (stripos($_valor, ',') !== false){
			$_valor = desformata_valor($_valor);
		}

		if (is_numeric($_valor)){
			if (stripos($this->fipvalortotal, ',') !== false){
				$this->fipvalortotal = desformata_valor($this->fipvalortotal);
			}
			
			if (stripos($this->fipvalorservico, ',') !== false){
				$this->fipvalorservico = desformata_valor($this->fipvalorservico);
			}
			
			$gipvalorItem = new Math($_valor, 0.5, 2);
			$gipvalorItem->mul();
			
			$fipvalortotal = new Math($this->fipvalortotal, $gipvalorItem->getResult(), 2);
			$fipvalorservico = new Math($this->fipvalorservico, $gipvalorItem->getResult(), 2);
			
			$fipvalortotal->sub();
			$fipvalorservico->sub();
			
			$this->fipvalortotal = $fipvalortotal->getResult();
			$this->fipvalorservico = $fipvalorservico->getResult();
		};
	}
	
	/**
     * Carrega a fatura de um item
     * 
     * @name carrega_registro_por_item_pad
     * @author Alysson Rafael
     * @param int $ipaid - Identificador do registro
     * @access public
     * @return array
     */
	public function carrega_registro_por_item_pad($ipaid){
		if(!empty($ipaid)){
			$ipaid = pg_escape_string($ipaid);
			$sql = "SELECT 
					fatura.fipid,
					fatura.ipaid,
					fatura.fipdataftura,
					fatura.fipnumsidoc,
					fatura.fipptres,
					fatura.fipvalortotal,
					fatura.fipvalorservico,
					fatura.fipstatus,
					fatura.fipnumfaturaagencia,
					fatura.fipnumfaturafornecedor
					FROM $this->stNomeTabela fatura
					WHERE fatura.ipaid='{$ipaid}' AND fatura.fipstatus='A' ";
			
			return $this->pegaLinha($sql);
		}
	}
	
	/**
     * Atualiza os valores da fatura do item de acordo com o valor da glosa
     * 
     * @name atualizar_valores_fatura
     * @author Alysson Rafael
     * @param int $ipaid      - Identificador do registro
     * @param float $gipvalor - Valor glosado
     * @param int $qtdItens   - Qtd de itens pela qual será dividido o valor da glosa
     * @param float $ipavalorservico - valor do serviço do item
     * @param float $ipavalortotal - valor total do item
     * @access public
     * @return void
     */
	public function atualizar_valores_fatura($ipaid,$gipvalor,$qtdItens,$ipavalorservico,$ipavalortotal){
		if(!empty($ipaid) && !empty($gipvalor) && $qtdItens>=1 && !empty($ipavalorservico) && !empty($ipavalortotal)){
			$ipaid = pg_escape_string($ipaid);
			$ipavalorservico = pg_escape_string($ipavalorservico);
			$ipavalortotal = pg_escape_string($ipavalortotal);
			
			
			//pega os valores da fatura do item
			$dadosItem = $this->carrega_registro_por_item_pad($ipaid);
			if(is_array($dadosItem) && count($dadosItem) >= 1){
				
				//atualiza a fatura com os valores que foram recalculados
				$sql = "UPDATE $this->stNomeTabela SET 
					    fipvalortotal='{$ipavalortotal}',
					    fipvalorservico='{$ipavalorservico}' 
					    WHERE fipid='{$dadosItem['fipid']}' ";
				
				$this->executar($sql);
				$this->commit();
				
			}
			
			$gipvalorDividido = str_replace('.','',$gipvalor); //edit
						
			//prepara o valor q será subtraído
			//$divide = new Math($gipvalor, $qtdItens, 2);
			//$gipvalorDividido = $divide->div()->getResult();
			
			//pega os valores da fatura do item
			$dadosItem = $this->carrega_registro_por_item_pad($ipaid);

			if(is_array($dadosItem) && count($dadosItem) >= 1){
				//subtrai o valor da fatura
				$subtrai = new Math($dadosItem['fipvalortotal'], $gipvalorDividido,2);
				$dadosItem['fipvalortotal'] = $subtrai->sub()->getResult();
				$subtrai = new Math($dadosItem['fipvalorservico'], $gipvalorDividido,2);
				$dadosItem['fipvalorservico'] = $subtrai->sub()->getResult();
				
				//atualiza a fatura
				$sql = "UPDATE $this->stNomeTabela SET 
					    fipvalortotal='{$dadosItem['fipvalortotal']}',
					    fipvalorservico='{$dadosItem['fipvalorservico']}' 
					    WHERE fipid='{$dadosItem['fipid']}' ";
			
				$this->executar($sql);
				$this->commit();
				
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
		
		$fipdataftura = formata_data_sql(pg_escape_string($fipdataftura));
		$fipnumsidoc = str_replace(array('.','/','-'), array('','',''), pg_escape_string($fipnumsidoc));
		$fipnumfaturaagencia = pg_escape_string($fipnumfaturaagencia);
		$fipvalortotal = str_replace(array('.',','),array('','.'),pg_escape_string($fipvalortotal));
		$fipnumfaturafornecedor = pg_escape_string($fipnumfaturafornecedor);
		$fipvalorservico = str_replace(array('.',','),array('','.'),pg_escape_string($fipvalorservico));
		$ipaid = pg_escape_string($ipaid);
		
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($fipid)){
			$sql = "INSERT INTO $this->stNomeTabela(fipdataftura,fipnumsidoc,fipvalortotal,fipvalorservico,fipnumfaturaagencia,fipnumfaturafornecedor,ipaid) ";
			$sql .= "VALUES('{$fipdataftura}','{$fipnumsidoc}','{$fipvalortotal}','{$fipvalorservico}','{$fipnumfaturaagencia}','{$fipnumfaturafornecedor}','{$ipaid}') ";
		}
		else if(!empty($fipid)){
			$fipid = pg_escape_string($fipid);
			$sql = "UPDATE $this->stNomeTabela SET fipdataftura='{$fipdataftura}',fipnumsidoc='{$fipnumsidoc}',";
			$sql .= "fipvalortotal='{$fipvalortotal}',fipvalorservico='{$fipvalorservico}', ";
			$sql .= "fipnumfaturaagencia='{$fipnumfaturaagencia}',fipnumfaturafornecedor='{$fipnumfaturafornecedor}' ";
			$sql .= "WHERE ipaid='{$ipaid}' ";
		}
		
		$sql .= "RETURNING fipid ";
		
       	$fipid = $this->pegaUm($sql);

       	if(!empty($fipid)){
       		$this->commit();
       		return array(0=>true,1=>$fipid,2=>$ipaid,3=>$id,4=>'cadItemPad');
       	}
       	else{
       		return array(0=>false,1=>$fipid,2=>$ipaid,3=>$id,4=>'faturarItemPad');
       	}
		
	}

}