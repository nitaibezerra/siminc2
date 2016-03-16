<?php

/**
 * Modelo de Conta contabil
 * @author Silas Matheus
 */
class ContaContabil extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.contacontabil";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('ccbid');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'ccbid' => null,
		'ccbdsc' => null,
		'ccbstatus' => null,
		'ccbdata' => null,
		'ccbvidautil' => null,
		'ccbpercresidual' => null
	);
	
	/**
	 * Recupera registro 
	 * @name pegarRegistro
	 * @param int $id - Identificador
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE ccbid='".$id."'";
		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	/**
	 * Retorna todas as Contas que tem ligação com a classe
	 * @name montaComboPorClasse
	 * @author Silas Matheus
	 * @access public
	 * @param int $clscodclasse - Código da Classe
	 * @param int $ccbid - Qual option receberá o selected 
	 * @return void
	 */
	public function montaComboPorClasse($clscodclasse, $ccbid = ''){
	
		if(!empty($clscodclasse)){
		
			$clscodclasse = addslashes($clscodclasse);
			
			/*$sql = "SELECT ccbid AS codigo,ccbdsc AS descricao FROM $this->stNomeTabela WHERE ccbid IN 
						 (SELECT ccbid FROM sap.itemcontacontabil WHERE icbid in 
						 	(SELECT icbid FROM sap.catalogo WHERE catcodclasse='$catcodclasse'))";*/
			
			$sql = "SELECT ccbid AS codigo,ccbdsc AS descricao FROM $this->stNomeTabela WHERE ccbid IN 
						 (SELECT ccbid FROM sap.catalogo JOIN sap.classe ON catalogo.clscodclasse=classe.clscodclasse WHERE classe.clscodclasse='$clscodclasse')";
			
			$resultado = $this->carregar($sql);
	
			$this->monta_combo("ccbid", $resultado, 'S', "Todos...", "carregarItemContaContabilPorClasse", "", "", "200", "S", "ccbid", "", $ccbid);
		
		}else{
		
			$this->montaComboContaContabil($ccbid, 'S');
		
		}
	
	}
	
	/**
     * Gera a combo de conta contábil
     * @name montaComboContaContabil
     * @author Alysson Rafael
	 * @access public
	 * @param int $ccbid - Qual option receberá o selected
	 * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @return void
     */
	function montaComboContaContabil($ccbid = '',$obrigatorio='N'){
		
		$sql = "SELECT ccbid AS codigo,ccbdsc AS descricao FROM sap.contacontabil ORDER BY ccbdsc ";
		
		$resultado = $this->carregar($sql);

		$this->monta_combo('ccbid',$resultado,'S','Todos...','carregarItemContaContabil(this.value,\''.$obrigatorio.'\');','','','',$obrigatorio,'ccbid','',$ccbid);
		
	}
	
}

