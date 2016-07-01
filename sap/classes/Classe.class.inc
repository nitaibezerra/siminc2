<?php

/**
 * Modelo de Classe
 * @author Alysson Rafael
 */
class Classe extends Modelo{
	
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.classe";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('clscodclasse');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'clscodclasse' => null,
		'clsdescclasse' => null
	);
	
	
	
	/**
     * Gera a combo de classe
     * @name montaComboClasse
     * @author Alysson Rafael
	 * @access public
	 * @param int $clscodclasse - Qual option receberá o selected
	 * @param string $obrigatorio - Se a combo vai ou não exibir a imagem de obrigatório(S ou N)
     * @return void
     */
	function montaComboClasse($clscodclasse = '',$obrigatorio='N'){
		
		$sql = "SELECT clscodclasse AS codigo,clsdescclasse AS descricao FROM $this->stNomeTabela ORDER BY clsdescclasse ";
		
		$resultado = $this->carregar($sql);

		$this->monta_combo('clscodclasse',$resultado,'S','Todos...','','','','',$obrigatorio,'clscodclasse','',$clscodclasse);
		
	}
	
	
	
	
	/**
	 * Monta lista de classes de acordo com filtro
	 * @name filtrarClasse
	 * @author Silas Matheus
	 * @access public
	 * @param int $clscodclasse - Código da Classe
	 * @param string $clsdescclasse - Descrição da Classe
	 * @return void
	 */
	public function filtrarClasse($clscodclasse, $clsdescclasse){
		
		$sql = "SELECT distinct
						'<center>
							<a style=\"cursor:pointer;\" onclick=\"selecionar(\''||clscodclasse||'\');\">
                    		<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
                    		</a>	
						</center>'					
					as comandos,
					clscodclasse||' ' AS clscodclasse, 
					clsdescclasse 
				FROM 
					$this->stNomeTabela 
				WHERE 1=1 ";
		
		if(!empty($clscodclasse)){
			$clscodclasse = addslashes($clscodclasse);
			$sql .= "AND clscodclasse='".$clscodclasse."' ";
		}
			
		if(!empty($clsdescclasse)){	
			$clsdescclasse = addslashes($clsdescclasse);
			$sql .= "AND clsdescclasse ILIKE '%".$clsdescclasse."%' ";
		}	
			
		$sql .= " ORDER BY clsdescclasse ASC";
				
		$cabecalho = array("Selecionar","Código","Classe");
		$this->monta_lista($sql, $cabecalho, 20, 10, false, "center",'S','','',array('center','center','center'));
	
	}
	
	
	
	/**
	 * Recupera dados da classe
	 * @name pegarRegistroClasse
	 * @author Silas Matheus
	 * @access public
	 * @param int $clscodclasse - Código da Classe
	 * @return array
	 */
	public function pegarRegistroClasse($clscodclasse){
	
		$catcodclasse = addslashes($catcodclasse);
		
		$sql = " SELECT clscodclasse, clsdescclasse FROM $this->stNomeTabela WHERE clscodclasse=$clscodclasse";
		$return = $this->carregar($sql);
		
		return $return[0];
		
	}
	
	
	
}