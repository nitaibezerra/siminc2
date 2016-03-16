<?php

/**
 * Modelo de Item da Conta contabil
 * @author Silas Matheus
 */
class ItemContaContabil extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.itemcontacontabil";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('icbid','ccbid');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'ccbid' => null,
		'icbid' => null,
		'icbdsc' => null,
		'icbdata' => null
	);
	
	/**
	 * Recupera registro 
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador da conta contabil
	 * @return array
	 */
	public function pegarRegistro($id){
	
		$id = addslashes($id);
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE icbid=$id";
		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	/**
     * Gerar a combo de ítem de conta contábil
     * @name montaComboItemContaContabil
     * @param int    $ccbid - Código da Conta
     * @param int    $icbid - Qual option recebera o selected
     * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N, default N)
     * @author Alysson Rafael
     * @access public
     * @return void
     */
	function montaComboItemContaContabil($ccbid, $icbid='',$obrigatorio='N'){

		//caso venha com id de conta contábil, filtra os itens pelo id
		if(!empty($ccbid)){
			
			$ccbid = addslashes($ccbid);
			
			$sql = "SELECT icbid AS codigo,icbdsc AS descricao FROM sap.itemcontacontabil WHERE ccbid='".$ccbid."' ";

			$resultado = $this->carregar($sql);
			if(!is_array($resultado)){
				$resultado = array();
			}
			$this->monta_combo('icbid',$resultado,'S','Todos...','','','','',$obrigatorio,'icbid','',$icbid);
				
		}else{
			
			$resultado = array();
			$this->monta_combo('icbid',$resultado,'S','Todos...','','','','',$obrigatorio,'icbid','',$icbid);
						
		}
		
	}
	
	/**
     * Gerar a combo de ítem de conta contábil por Classe e Conta
     * @name montaComboPorClasse
     * @param int $clscodclasse - Código da Classe
     * @param int $ccbid - Código da Conta
     * @param int $icbid - Qual option recebera o selected
     * @return void
     */
	function montaComboPorClasse($clscodclasse, $ccbid, $icbid=''){

		//caso venha com id de conta contábil, filtra os itens pelo id
		if(!empty($ccbid) && !empty($clscodclasse)){
			
			$clscodclasse = addslashes($clscodclasse);
			$ccbid = addslashes($ccbid);
			
			$sql = "SELECT DISTINCT
						icbid.icbid AS codigo,
						icbid.icbdsc AS descricao 
					FROM 
						$this->stNomeTabela icbid
					INNER JOIN
						sap.catalogo cat 
						ON icbid.icbid=cat.icbid AND icbid.ccbid=cat.ccbid
					JOIN 
					    sap.classe ON cat.clscodclasse=classe.clscodclasse
					WHERE 
						icbid.ccbid='{$ccbid}'
					AND
						classe.clscodclasse='{$clscodclasse}'";

			$resultado = $this->carregar($sql);
			if(!is_array($resultado)){
				$resultado = array();
			}
			$this->monta_combo("icbid",$resultado,'S',"Todos...","","","","200","S","icbid","",$icbid);
				
		}else{
			
			$this->montaComboItemContaContabil('', '', 'S');
			
		}
		
	}
	
}

