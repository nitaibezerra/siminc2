<?php

/**
 * Modelo de Glosa do Item da PAD
 * @author Diego Oliveira
 */
class GlosaItemPad extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.glosaitempad';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('gipid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'gipid' => null,
		'ipaid' => null,
		'gipdata' => null,
		'gipvalor' => null,
		'gipmotivo' => null,
		'gipstatus' => null,
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
			
			if (stripos($this->gipdata, '/') !== false){
				$this->gipdata = formata_data_sql($this->gipdata);
			}
			
			if (stripos($this->gipvalor, ',') !== false){
				$this->gipvalor = desformata_valor($this->gipvalor);
			}

			return true;
		}
		
		return false; 
	}

	/**
     * Implementação do método depoisSalvar()
     * 
     * @name antesSalvar
     * @author Diego Oliveira
     * @access public
     * @return bool
     */
	public function depoisSalvar(){ 
		$this->gipdata = $this->arAtributosAntesSalvar['gipdata'];
		$this->gipvalor = $this->arAtributosAntesSalvar['gipvalor'];

		return true;
	}
	
	/**
     * Carrega os dados para o objeto da ultima (unica) glosa do item da PAD
     * 
     * @name carregaGlosaIdItem
     * @author Diego Oliveira
     * @param int $ipaid id do item da PAD
     * @access public
     * @return void
     */
	public function carregaGlosaIdItem($ipaid){
		if (is_numeric($ipaid)){
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
     * Verifica se todos os itens possuem saldo para a glosa
     * @name verifica_saldo_para_glosa
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados da tela
     * @return bool(glosa de 1 item)/array(glosa em lote)
     */
	public function verifica_saldo_para_glosa($post){
		if(!empty($post['gipvalor'])){
			
			//formata o valor informado para glosa
			$post['gipvalor'] = str_replace(array('.',','),array('','.'),pg_escape_string($post['gipvalor']));
			
			//se for glosa em lote divido o valor total pelo numero de items
	/*		if ($post['ipaids']){
				$aux_ipaids = explode(',', $post['ipaids']);
				$verifica = new Math($post['gipvalor'], count($aux_ipaids),2);
				$post['gipvalor'] = $verifica->div()->getResult();
			}*/
			
			$oFaturamentoItemPad = new FaturamentoItemPad();
			
			//caso seja glosa de 1 item
			if(empty($_POST['ipaids'])){
				
				$dadosFatura = $oFaturamentoItemPad->carrega_registro_por_item_pad($post['ipaid']);
				//echo $_GET['ipaid'];exit;//EDIT

				//$verifica = new Math($dadosFatura['fipvalortotal'], $post['gipvalor'],2);
				$verifica = new Math($post['ipavaloritem'], $post['gipvalor'],2);
				return $verifica->isLess();
			}
			//caso seja glosa em lote
			else if(!empty($_POST['ipaids'])){
				$aux = explode(',',$_POST['ipaids']);
				
				//divide o valor da glosa pela qtd de itens(glosa em lote)
				$divide = new Math($post['gipvalor'], count($aux),2);
				$post['gipvalor'] = $divide->div()->getResult();
				
				$numerosItens = '';
				$retorno = array();
				foreach($aux as $key => $value){
					//carrega os dados da fatura do item
					$dadosFatura = $oFaturamentoItemPad->carrega_registro_por_item_pad($value);
					$verifica = new Math($dadosFatura['fipvalortotal'], $post['gipvalor'],2);
					//caso não possua saldo
					if($verifica->isLess()){
						$oItemPad = new ItemPad();
						$dadosItem = $oItemPad->carrega_registro_por_id($value);
						$numerosItens .= $dadosItem['ipanumitempad'].', ';
						$retorno[0] = true;
					}
				}
				$retorno[1] = substr($numerosItens,0,strrpos($numerosItens,','));
				return $retorno;
			}
			
			
		}
		
		
		
	}
	
	
}