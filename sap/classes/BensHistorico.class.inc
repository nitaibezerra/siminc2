<?php

/**
 * Modelo de Termo
 * @author Silas Matheus
 */
class BensHistorico extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.benshistorico";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('bhsid');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'bhsid' => null,
		'bebid' => null,
		'benid' => null,
		'mvbid' => null,
		'usucpf' => null,
		'bhsjustificativa' => null,
		'bhsdata' => null
	);

	/**
	 * Salva novo registro no historico
	 * @name gravar
	 * @author Silas Matheus
	 * @access public
	 * @param string(800) $bhsjustificativa - Justificativa de alteração
	 * @param int $bebid - Identificador de Baixa
	 * @param int $benid - Identificador de Entrada
	 * @param int $mvbid - Identificador de Movimentação
	 * @return int
	 */
	public function gravar($bhsjustificativa, $bebid = 'NULL', $benid = 'NULL', $mvbid = 'NULL'){
	
		$bhsid = NULL;
		$bhsjustificativa = trim($bhsjustificativa);
		
		if(!empty($bhsjustificativa)){
		
			$usucpf = $_SESSION['usucpf'];
			
			$sql = "INSERT INTO 
						$this->stNomeTabela 
						(usucpf, bhsjustificativa, bebid, benid, mvbid) 
					VALUES 
						('{$usucpf}','{$bhsjustificativa}',$bebid, $benid, $mvbid) 
					RETURNING bhsid";
	
			$sbmid = $this->pegaUm($sql);
		 	$this->commit(); 
		
		}

		return $bhsid;
	
	}
	
	
}