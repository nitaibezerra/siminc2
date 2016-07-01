<?php
class Item extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "consultapne.item";
	
	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"iteid"
	);
	
	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
		"iteid" => null,
		"itedsc" => null,
		"itetipo" => null,
		"iteidpai" => null,
		"iteordem" => null,
	);
	
	public function carregarItens($iteidpai = null) {
		$sql = " SELECT * FROM $this->stNomeTabela";
		$sql.= $iteidpai ? " WHERE iteidpai = '{$iteidpai}'" : " WHERE iteidpai IS NULL";
		$sql.= " ORDER BY iteordem";
		
		$resultado = $this->carregar( $sql );
	
		return $resultado ? $resultado : array();
	}
	
	/**
	 * Função _inserir
	 * Método usado para inserção de um registro do banco
	 * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
	 * @access private
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function inserir($arCamposNulo = Array()) {
		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
	
		$arCampos  = array();
		$arValores = array();
		$arSimbolos = array();
	
		$troca = array("'", "\\");
	
		foreach ($this->arAtributos as $campo => $valor ){
			if( $valor !== null ){
				$arCampos[]  = $campo;
				$valor = str_replace($troca, "", $valor);
				$arValores[] = trim( pg_escape_string( $valor ) );
			}
		}
	
		if (count($arValores)) {
			$sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
				     values ( '". implode( "', '", $arValores ) ."' )
					 returning {$this->arChavePrimaria[0]}";
			
			$stChavePrimaria = $this->arChavePrimaria[0];
			
			return $this->$stChavePrimaria = $this->pegaUm( $sql );
		}
	}
}