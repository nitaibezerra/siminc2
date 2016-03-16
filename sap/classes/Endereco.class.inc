<?php

/**
 * Modelo da visao de Endereco
 * @author Silas Matheus
 */
class Endereco extends Modelo{
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "cep.v_endereco2 ";
	
	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('cep');
	
	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'logradouro' => null,
		'bairro' => null,
		'cidade' => null,
		'estado' => null,
		'latitude' => null,
		'hemisferio' => null,
		'longitude' => null,
		'meridiano' => null,
		'altitude' => null,
		'medidaarea' => null,
		'medidaraio' => null,
		'muncod' => null,
		'muncodcompleto' => null
	);
	
	/**
	 * Recupera endereço pelo cep
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param string $cep - Cep
	 * @return array
	 */
	public function pegarRegistro($cep){
	
		$cep = str_replace('.', '', $cep);
		$cep = str_replace('-', '', $cep);
		$cep = addslashes($cep);
		
		$sql = " SELECT * FROM $this->stNomeTabela WHERE cep='$cep'";

		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	/**
	 * Monta Combo de Estado
	 * @name montaComboUf
	 * @author Silas Matheus
	 * @access public
	 * @param string $estado - Option que recebera o selected
	 * @return void
	 */
	public function montaComboUf($estado = ""){
	
		$sql = "SELECT DISTINCT 
					ufesg as codigo, 
					ufesg as descricao  
				FROM
					cep.loglogradouro
				ORDER BY ufesg ASC
				LIMIT 27";
		
		$return = $this->carregar($sql);		
		$this->monta_combo("estado", $return, 'S', " -- Selecione -- ", "carregarCidade", "", "", "200", "S", "estado", "", $estado);
		
	}
	
	/**
	 * Monta combo de Cidade de acordo com o estado
	 * @name montaComboCidade
	 * @author Silas Matheus
	 * @access public
	 * @param string $estado - Estado
	 * @param string $cidade - Cidade
	 * @return void
	 */
	public function montaComboCidade($estado = '', $cidade = ""){
	
		if(!empty($estado)){

			$sql = "SELECT DISTINCT
						cidade AS codigo,
						cidade AS descricao
					FROM 
						$this->stNomeTabela
					WHERE
						estado='$estado' 
					ORDER BY 
						cidade";
			
			$return = $this->carregar($sql);		
			
		}else{
		
			$return = array();
		
		}	

		$this->monta_combo("cidade", $return, 'S', " -- Selecione -- ", "", "", "", "200", "S", "cidade", "", $cidade);
		
	}
	
	/**
	 * Monta combo de Bairro de acordo com a cidade
	 * @name montaComboBairro
	 * @author Silas Matheus
	 * @access public
	 * @param string $cidade - Cidade
	 * @param string $bairro - Bairro
	 * @return void
	 */
	public function montaComboBairro($cidade = '', $bairro = ""){
	
		if(!empty($cidade)){

			$sql = "SELECT DISTINCT
						bairro AS codigo,
						bairro AS descricao
					FROM 
						$this->stNomeTabela
					WHERE
						cidade='$cidade' 
					ORDER BY 
						bairro";
			
			$return = $this->carregar($sql);		
			
		}else{
		
			$return = array();
		
		}	

		$this->monta_combo("bairro", $return, 'S', " -- Selecione -- ", "", "", "", "200", "S", "bairro", "", $bairro);
		
	}
	
	/**
	 * Monta lista de endereço de acordo com os filtros
	 * @name filtrarCep
	 * @author Silas Matheus
	 * @access public
	 * @param string $estado - Estado
	 * @param string $logradouro - Logradouro
	 * @param string $bairro - Bairro
	 * @param string $cidade - Cidade 
	 * @return void
	 */
	public function filtrarCep($estado, $logradouro, $bairro, $cidade){
	
		$sql = "SELECT 
						'<center>
							<a style=\"cursor:pointer;\" onclick=\"selecionar(\''||substr(cep,1,2) || '.' || substr(cep,3,3) || '-' || substr(cep,6,3)||'\');\">
                    		<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
                    		</a>
						</center>'					
					as comandos,
					substr(cep,1,2) || '.' || substr(cep,3,3) || '-' || substr(cep,6,3) cep, 
					estado,
					cidade,
					bairro,
					logradouro 
				FROM 
					$this->stNomeTabela 
				WHERE 1=1 ";
		
		if($estado)
			$sql .= "AND estado='$estado'";
			
		if($logradouro)	
			$sql .= "AND logradouro ilike '%$logradouro%' ";

		if($bairro)
			$sql .= "AND bairro ilike '%$bairro%' ";
			
		if($cidade)	
			$sql .= "AND cidade ilike '%$cidade%' ";
			
		$sql .= " ORDER BY logradouro ASC";
				
		$cabecalho = array("Comando", "Cep", "UF", "Cidade", "Bairro", "Logradouro");
		$this->monta_lista($sql, $cabecalho, 20, 10, false, "center",'S','','',array('center','center','center','center','center','center'));
	
	}
	
}