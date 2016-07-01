<?php
/**
 * SQL Active Development Object - Classe que auxilia a criação de SQLs
 * 
 * Exemplo de uso:
 *     	Core::loadComponent('sqlado');
 *
 *      ** Instancia-se o Sqlado 
 *   	$sqlado = new Sqlado();
 *      
 *      ** Define os campos a serem selecionados usando  addToSelect
 *      ** Caso nenhum parâmetro seja passado para o addToSelect então é feito um select
 *      ** A query abaixo gera o output:
 *      ** select pcsf.vl_peso_integralizacao as vl_peso_integralizacao , pcsf.vl_peso_premiacao as vl_peso_premiacao , pcsf.vl_peso_iniciacao_cientifica as vl_peso_iniciacao_cientifica , 
 *		** pcsf.vl_peso_proficiencia as vl_peso_proficiencia , peep.ds_exame_proficiencia as ds_exame_proficiencia
 *   	$sqlado->addToSelect("pcsf.vl_peso_integralizacao as vl_peso_integralizacao");
 *   	$sqlado->addToSelect("pcsf.vl_peso_premiacao as vl_peso_premiacao");
 *   	$sqlado->addToSelect("pcsf.vl_peso_iniciacao_cientifica as vl_peso_iniciacao_cientifica");
 *   	$sqlado->addToSelect("pcsf.vl_peso_proficiencia as vl_peso_proficiencia");
 *   	$sqlado->addToSelect("peep.ds_exame_proficiencia as ds_exame_proficiencia");
 *
 *		** Adiciona informações das tabelas que serão selecionadas
 *		** from projeto_exterior.priorizacao_csf pcsf 
 *      ** left join projeto_exterior.pais_destino pd on pd.id_projeto = $intIdProjetoFomento
 *      ** left join projeto_exterior.exame_proficiencia peep on peep.id_exame_proficiencia = pd.id_exame_proficiencia
 *   	$sqlado->addToFrom("projeto_exterior.priorizacao_csf pcsf");
 *   	$sqlado->addToFrom("left join projeto_exterior.pais_destino pd on pd.id_projeto = :intIdProjetoFomento");
 *		$sqlado->addToFrom("left join projeto_exterior.exame_proficiencia peep on peep.id_exame_proficiencia = pd.id_exame_proficiencia");
 *		$sqlado->addParam("intIdProjetoFomento", $intIdProjetoFomento);
 *   	
 *		** Adiciona item a cláusula WHERE. Usa-se AND para concatenar. Concatenações de OR devem ser feitas com string antes de serem adicionadas
 *		** no SQL.
 *   	$sqlado->addToWhere("pcsf.id_projeto = :intIdProjetoFomento ");
 *
 *		Funcionalidades para limpar array das cláuslas do SQL:
 *		$sqlado->clearFrom(), $sqlado->clearWhere() etc.
 *
 *	****** Para maiores informações veja os métodos da classe. Eles possuem nomes bem lógicos. *****
 *
 * @package Sqlado
 * @subpackage Sqlado
 */
class Sqlado {

	private $objConnection;
	private $objStatement;
	private $objDriver;

	private $arrSelect;
	private $arrFrom;
	private $arrWhere;
	private $arrMapParams;
	private $arrMapParamTypes;
	private $arrGroup;
	private $arrOrder;
	private $intLimit;
	private $intOffset;
	private $strCountColumns;
	private $bolDistinct;
	
	private $strQuery;

	// TODO: Depois é melhor que algumas variáveis como objConnection, objStatement etc sejam controladas pelo Driver.
	// Por enquanto deixa assim e depois faço o refactoring. Pra variar é pra fazer 1000 casos de usos em 2 dias. 
	// Mas próxima vez que for necessário faço um refactoring direitinho para usar direito o driver.
	public function __construct() {
        global $db;


		$this->objConnection = $db;
		$this->arrSelect = array();
		$this->arrFrom = array();
		$this->arrWhere = array();
		$this->arrMapParams = array();
		$this->arrMapParamTypes = array();
		$this->arrGroup = array();
		$this->arrOrder = array();
		$this->bolDistinct = false;
	}

	public function insert( $strTabela, $arrMapColunas ) {
		$arrChavesColunas = array_keys($arrMapColunas);
		$arrColunas = implode(", ", $arrChavesColunas);
		$arrValores = ":" . implode(", :", $arrChavesColunas);

		$strSql = "insert into $strTabela ($arrColunas) values ($arrValores)";

		$this->objStatement = $this->objConnection->prepare( $strSql );

		foreach ($arrMapColunas as $k => $v) {
			$this->objStatement->bindValue(":$k", $v);
		}

		$bolRetorno = $this->objStatement->execute();

        if ( !$bolRetorno ) 
        {
        	throw new Exception("Não foi possível realizar a inserção: " . $this->objStatement->queryString);
        }

        $this->close();
	}

	public function update( $strTabela, $arrMapColunas ) {
		$arrChavesColunas = array_keys($arrMapColunas);
		$arrColunas = implode(", ", $arrChavesColunas);
		$arrValores = ":" . implode(", :", $arrChavesColunas);

		$arrSets = array();

		foreach ($arrMapColunas as $k => $v) {
			$arrSets[] = "$k = :$k";
		}

		$strSql = "update $strTabela set " . implode(", ", $arrSets) . " ";
		$this->strQuery = $strSql;
	
		if (empty($this->arrWhere)) {
			throw new Exception("Não é possível executar um update sem where");
		} else {
			$this->append( $this->strQuery, "where", $this->arrWhere, "and\n" );
		}
		
		foreach ( $this->arrMapParams as $key => $value ) {
			if ( is_array( $value ) ) {
				$this->prepareArray( $key, $value, $this->strQuery );
			}
		}
		
		foreach ( $this->arrMapParams as $key => $value ) {
			if ( is_array( $value ) ) {
				$this->bindArray( $key, $value );
			} else {
				if ($this->arrMapParamTypes[$key] != null) {
					$this->bindParam( ":" . $key, $value, $this->arrMapParamTypes[$key] );
					// $this->objStatement->bindValue( ":" . $key, $value, $this->arrMapParamTypes[$key] );
				} else {
					// TODO: Fazer dedução de tipo de parametro
					$this->bindParam( ":" . $key, $value );
					// $this->objStatement->bindValue( ":" . $key, $value );
				}
			}
		}

		$this->objStatement = $this->objConnection->prepare( $this->strQuery );
		foreach ($arrMapColunas as $k => $v) {
			$this->objStatement->bindValue(":$k", $v);	
		}

		$bolRetorno = $this->objStatement->execute();

        if ( !$bolRetorno ) {
        	throw new Exception("Não foi possível realizar a consulta: " . $this->objStatement->queryString);
        }

        $this->close();
	}

	public function delete( $strTabela ) {

		$this->strQuery = "delete from $strTabela ";

		if (empty($this->arrWhere)) {
			throw new Exception("Não é possível executar um delete sem where");
		} else {
			$this->append( $this->strQuery, "where", $this->arrWhere, "and\n" );
		}

		foreach ( $this->arrMapParams as $key => $value )
		{
			if ( is_array( $value ) )
			{
				$this->prepareArray( $key, $value, $this->strQuery );
			}
		}

		foreach ( $this->arrMapParams as $key => $value ) 
		{
			if ( is_array( $value ) )
			{
				$this->bindArray( $key, $value );
			}
			else
			{
				if ($this->arrMapParamTypes[$key] != null) {
					$this->bindParam( ":" . $key, $value, $this->arrMapParamTypes[$key] );
					// $this->objStatement->bindValue( ":" . $key, $value, $this->arrMapParamTypes[$key] );
				} else {
					// TODO: Fazer dedução de tipo de parametro
					$this->bindParam( ":" . $key, $value );
					// $this->objStatement->bindValue( ":" . $key, $value );
				}
				
			}
		}

		$this->objStatement = $this->objConnection->prepare( $this->strQuery );

		$bolRetorno = $this->objStatement->execute();	

        if ( !$bolRetorno ) 
        {
        	throw new Exception("Não foi possível realizar a consulta: " . $this->objStatement->queryString);
        }

        $this->close();
	}

	public function addToSelect( $strSelect ) {
		$this->arrSelect[] = $strSelect;
	}

	public function removeFromSelect( $intIndex ) {
		unset( $this->arrSelect[ $intIndex ] );
	}

	public function clearSelect( ) {
		unset($this->arrSelect);
		$this->arrSelect = array();
	}

	public function addToFrom( $strFrom ) {
		$this->arrFrom[] = $strFrom;
	}

	public function removeFromFrom( $intIndex ) {
		unset( $this->arrFrom[ $intIndex ] );
	}

	public function clearFrom( ) {
		unset($this->arrFrom);
		$this->arrFrom = array();
	}

	public function addToWhere( $strWhere, $strKey = null ) {
		if ($strKey) {
			$this->arrWhere[$strKey] = $strWhere;
		} else {
			$this->arrWhere[] = $strWhere;
		}
	}

	public function removeFromWhere( $intIndex ) {
		unset( $this->arrWhere[ $intIndex ] );
	}

	public function countWhere() {
		return count($this->arrWhere);
	}

	public function clearWhere( ) {
		unset($this->arrWhere);
		$this->arrWhere = array();
	}

	public function addParam( $strParamName, $strParamValue, $intDataType = null) {
		$this->arrMapParams[$strParamName] = $strParamValue;
		$this->arrMapParamTypes[$strParamName] = $intDataType;
	}

	public function removeFromParam( $strParamName ) {
		unset( $this->arrMapParams[ $strParamName ] );
		unset( $this->arrMapParamsTypes[ $strParamName ] );
	}

	public function clearParams( ) {
		unset($this->arrMapParams);
		$this->arrMapParams = array();
		unset($this->arrMapParamTypes);
		$this->arrMapParamTypes = array();
	}

	public function addToGroup( $strGroup ) {
		$this->arrGroup[] = $strGroup;
	}

	public function removeFromGroup( $intIndex ) {
		unset( $this->arrGroup[ $intIndex ] );
	}

	public function clearGroup( ) {
		unset($this->arrGroup);
		$this->arrGroup = array();
	}

	public function addToOrder( $strOrder ) {
		$this->arrOrder[] = $strOrder;
	}

	public function removeFromOrder( $intIndex ) {
		unset( $this->arrOrder[ $intIndex ] );
	}

	public function clearOrder( ) {
		unset($this->arrOrder);
		$this->arrOrder = array();
	}

	public function setLimit( $intLimit ) {
		$this->intLimit = $intLimit;
	}

	public function setOffset( $intOffset ) {
		$this->intOffset = $intOffset;
	}

	public function setDistinct( $bolDistinct ) {
		$this->bolDistinct = $bolDistinct;
	}

	public function isDistinct() {
		return $this->bolDistinct;
	}

	public function setCount( $strCountColumns ) {
		$this->strCountColumns = $strCountColumns;
	}
	
	private function append( &$strSql, $strBefore, $arrFilter = null, $strSeparator = null ) 
	{
		$strSql .= $strBefore . " ";

		if ( $arrFilter != null ) 
		{
			if ( $strSeparator == null ) {
				$strSeparator = " ";
			} else {
				$strSeparator = " " . $strSeparator . " ";
			}

			if ( is_array($arrFilter)) {
				$strSql .= implode( $strSeparator, $arrFilter ) . " ";
			} 
			else 
			{
				$strSql .= $arrFilter . " ";
			}
			
		}
		
		return $strSql;
	}

	private function prepareForCount() 
 	{
 		$strCountColumns = $this->strCountColumns;
		if ($strCountColumns == null) {
			$this->strCountColumns = "*";
		}
 
     	if ($this->isDistinct()) {
     		$strSelect = "distinct $strCountColumns";
     	} else {
     		$strSelect = $strCountColumns;
     	}

    	$this->clearSelect();
        $this->addToSelect("count($strSelect) as quantidade");
        $this->clearOrder();
        $this->setLimit(null);
        $this->setOffset(null);
 	}

	public function getQuery( $bolClearCache = false ) {
		if ($bolClearCache) {
			$this->strQuery = null;	
		}
		
		if ( $this->strQuery != null ) {
			return $this->strQuery;
		}

		if ($this->strCountColumns) {
			$this->prepareforCount();
		}

		$strSelectClause = "select";
		if ($this->bolDistinct && !$this->strCountColumns) {
			$strSelectClause = "select distinct";
		}

		$this->strQuery = "";
		if ( !empty( $this->arrSelect ) ) 
		{
			$this->append( $this->strQuery, $strSelectClause, $this->arrSelect, ",\n" );
		} 
		else 
		{
			$this->append( $this->strQuery, "$strSelectClause *" );
		}

		$this->append( $this->strQuery, "from", $this->arrFrom, "\n");

		if ( !empty( $this->arrWhere ) ) 
		{
			$this->append( $this->strQuery, "where", $this->arrWhere, "and\n" );
		}

		if ( !empty( $this->arrGroup ) ) 
		{
			$this->append( $this->strQuery, "group by", $this->arrGroup, ",\n");
		}

		if ( !empty( $this->arrOrder ) ) 
		{
			$this->append( $this->strQuery, "order by", $this->arrOrder, ",\n" );
		}

		if ( !empty( $this->intOffset ) ) 
		{
			$this->append( $this->strQuery, "offset", $this->intOffset, "\n" );
        }

		if ( !empty( $this->intLimit ) ) 
		{
			$this->append( $this->strQuery, "limit", $this->intLimit, "\n" );
		}

		foreach ( $this->arrMapParams as $key => $value )
		{
			if ( is_array( $value ) )
			{
				$this->prepareArray( $key, $value, $this->strQuery );
			}
		} 

		return $this->strQuery;
	}

	public function prepareQuery()
	{
		$this->getQuery(true);
		foreach ( $this->arrMapParams as $key => $value ) 
		{
			if ( is_array( $value ) )
			{
				$this->bindArray( $key, $value );
			}
			else
			{
				if ($this->arrMapParamTypes[$key] != null) {
					$this->bindParam( ":" . $key, $value, $this->arrMapParamTypes[$key] );
					// $this->objStatement->bindValue( ":" . $key, $value, $this->arrMapParamTypes[$key] );
				} else {
					// TODO: Fazer dedução de tipo de parametro
					$this->bindParam( ":" . $key, $value );
					// $this->objStatement->bindValue( ":" . $key, $value );
				}
				
			}
		}

		$this->objStatement = $this->objConnection->executar( $this->getQuery() );
	}

	private function bindParam( $key, $value, $paramType = null ) 
	{
		// Somente para atualizar a variável $this->strQuery
		$this->getQuery();

		if ( $paramType == null ) {
			if ($value == null) {
				$value = 0;
			} else if (is_int($value)) {
				$paramType = PDO::PARAM_INT;
			} else if (is_float($value)) {
				$paramType = PDO::PARAM_FLOAT;
			} else if (is_string($value)) {
				$paramType = PDO::PARAM_STR;
			} else {
				$paramType = PDO::PARAM_STR;
			}
		}
		
		$value = pg_escape_string($value);
		$strReplacement = $value;

		if ( $paramType == PDO::PARAM_STR ) {
			$strReplacement = "'$value'";
		}

		// TODO: Verificar erro pois está substituíndo quando duas variáveis possuem o mesmo início.
		$this->strQuery = preg_replace('/' . $key . '(\D)/',$strReplacement . '$1', $this->strQuery);
	}

	private function prepareArray( $strKey, $arrValue ) 
	{	
		// Primeiro cria a string separando os parametros
		$arrValues = array();
		foreach ( $arrValue as $key => $value ) 
		{
			$tempKey = $strKey . $key;
			$arrValues[] = ":$tempKey";
		}

		// Depois substitui a string com o nome
		$this->strQuery = str_replace( ":$strKey", "(" . implode(", ",$arrValues) . ")" , $this->strQuery );
	}

	private function bindArray( $strKey, $arrValue, $intPdoType = PDO::PARAM_STR ) 
	{
		// Depois faz o bind em si
		foreach ( $arrValue as $key => $value ) 
		{
			$tempKey = $strKey . $key;
			$this->bindParam( ":$tempKey", $value, $intPdoType );
			// $this->objStatement->bindValue(":$tempKey", $value, $intPdoType);
		}
	}

	public function fetch($intReturnType = PDO::FETCH_ASSOC) 
	{
		// echo $this->debug(); die;
		$this->prepareQuery();

        $arrResult = $this->objConnection->carrega_tudo($this->objStatement);

        $this->close();
        
        return $arrResult;
	}

	public function fetchOne()
	{
		$this->setOffset(null);
		$this->setLimit(1);
		$arrRetorno = $this->fetch();
		$this->setLimit(null);

		if (count($arrRetorno) > 0) {
			return $arrRetorno[0];
		} else {
			return null;
		}
	}

	public function close()
	{
        $this->objConnection->close();
	}

	public function debug()
	{
		$this->prepareQuery();
		$strRetorno = '<pre>';
		$strRetorno .= $this->objStatement->queryString;
		$strRetorno .= "\n";
		$strRetorno .= print_r($this->arrMapParams, true);
		$strRetorno .= '</pre>';
		return $strRetorno;
	}
}
?>