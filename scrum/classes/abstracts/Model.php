<?php

include APPRAIZ . 'www/gerador/Zend/Zend/Validate/Digits.php';
include APPRAIZ . 'www/gerador/Zend/Zend/Validate/NotEmpty.php';
include APPRAIZ . 'www/gerador/Zend/Zend/Validate/Date.php';
include APPRAIZ . 'www/gerador/Zend/Zend/Validate/StringLength.php';

/**
 * Controle responsavel pelas entidades.
 * 
 * @author Equipe simec - Consultores OEI
 * @since  14/05/2013
 * 
 * @name       Abstract_Model
 * @package    Abstract
 * @version    $Id
 */
abstract class Abstract_Model
{
    protected $_db;
    public $entity;
    public $error = null;
    
    public function __construct()
    {
        $this->_names = $this->_schema . '.' . $this->_name;
        
        global $db; 
        $this->_db = $db;
    }
    
    /**
     * Pega os nomes das cahves primarias da tabela.
     * 
     * @name getPkNames
     * @access public
     * @return array $pkNames - Array com os nomes das chaves primarias.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function getPkNames()
    {
        $pkNames = array();
        foreach ($this->entity as $columnName => $column){
            if($column['contraint'] == 'pk')
                $pkNames[] = $columnName;
        }
        
        return $pkNames;
    }
    
    /**
     * Pega os valores das cahves primarias da tabela.
     * 
     * @name getPkValues
     * @access public
     * @return array $pkValues - Array com os valores das chaves primarias, sendo a chave do array o nome da coluna.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function getPkValues()
    {
        $pkValues = array();
        foreach ($this->entity as $columnName => $column){
            if(!empty($column['contraint']) && $column['contraint'] != 'fk')
                $pkValues[$columnName] = $column['value'];
        }
        
        return $pkValues;
    }
    
    /**
     * Checa se as chaves primarias estao todas com valores.
     * 
     * @name checkPkValues
     * @access public
     * @return boolean - true / false
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function checkPkValues()
    {
        foreach($this->getPkValues() as $pk){
            if(empty($pk)) return false;
        }
        
        return true;
    }
    
    /**
     * Pega os valores da entidade e retorna um array onde a chave dos valores e o nome da coluna na tabela do banco de dados.
     * 
     * @name getEntityValues
     * @access public
     * @return array $entityValues - Array com os valores da entidade e as chaves do array e o nome da coluna.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function getEntityValues($pk = true)
    {
        $entityValues = array();
        foreach($this->entity as $columnName => $column){
            
            if( ($column['contraint'] == 'pk' && $pk == true) || ($column['contraint'] != 'pk') ){
                $entityValues[$columnName] = $column['value'];
            }
        }
        
        return $entityValues;
    }
    
//    public function getEntity($pk = null)
//    {
//        if($pk){
//            
//        }
//    }
    
    /**
     * Limpa todos os valores da entidade.
     * E utilizado mais quando se necessita salvar mais de uma vez na mesma tabela, fazendo com que nao necessite chamar denovo a mesma model.
     * 
     * @name clearEntity
     * @access public
     * @return void
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 14/08/2013
     */
    public function clearEntity()
    {
        foreach($this->entity as $key => &$entity)
            $this->entity[$key]['value'] = "";
    }
    
    /**
     * Coloca todos os valores na entidade de acordo com a chave do array passado para este metodo.
     * 
     * @name populateEntity
     * @param array $data - Valores para preencher a entity
     * @access public
     * @return void
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 14/08/2013
     */
    public function populateEntity($data = null)
    {
        //Preenchendo a entity com os dados novos.
        if($data){
            foreach($data as $key => $value) {
                if(isset($this->entity[$key])) 
                    $this->entity[$key]['value'] = $value;
            }
        }
        
        // Se todas as chaves primarias tiverem valor, pega os valores de acordo com as chaves primarias.
        if($this->checkPkValues()){
            
            $entityValuesBD = $this->getByValues($this->getPkValues());
            // Se tiver valores no banco de acordo de acordo com as chaves primarias ele preenche a entity com esses dados.
            if($entityValuesBD) {
                foreach($entityValuesBD as $key => $value) {
                    if(isset($this->entity[$key]) && !empty($value)) 
                        $this->entity[$key]['value'] = $value;
                }
            }
        }
        
        //Preenchendo a entity com os dados novos.
        if($data){
            foreach($data as $key => $value) {
                if(isset($this->entity[$key])) 
                    $this->entity[$key]['value'] = $value;
            }
        }
    }
    
    /**
     * Trata os valores da entidade, para ficar de acordo com o banco.
     * 
     * @name treatEntity
     * @access public
     * @return boolean - true / false
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
     * @since 22/08/2013
     */
    public function treatEntity()
    {
        foreach($this->entity as $columnName => &$column){
            if(($column['contraint'] != 'pk' || $column['is_null'] == 'YES') && empty($column['value'])){
                $column['value'] = 'NULL';
                
            
            } else if(!empty($column['value'])){
                
                //
                if($column['type'] == 'date'){
                    $dateExplode = explode("/",$column['value']);
                    $column['value'] = "'". implode("-",array_reverse($dateExplode)) . "'";
                } else {
                    $column['value'] = "'{$column['value']}'";
                }
            }
        }
    }
    
    
    /**
     * Salva a entidade, mas antes de salvar realiza a validacao para manter a integridade dos dados com a tabela do banco de dados.
     * 
     * @name isValid
     * @access public
     * @return boolean - true / false
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
     * @since 22/08/2013
     */
    public function save()
    {
        // Se a entidade estiver valida e nao exisitr nenhuma mensagem de erro.
        if($this->isValid() && !$this->error){
            
            // Se tiver valor na PK significa que esta editando.
            if($this->checkPkValues()){
                
                
                ver('Montar sql de update!!!', $this->entity,d);
                $sql = "UPDATE {$this->_names}
                        SET ";

                $n = 0;
                
                foreach($this->entity as $key => $value){

                    $value = utf8_decode($value);
                    if($n > 0) $sql .= " , ";
                    $sql .= "{$key} = {$value}";
                    $n++;
                }
                
                if(is_array( $this->_primary )){
                    $sql .= " WHERE ";
                    
                    $n = 0;
                    foreach($this->getPkValues() as $pk){
                        
                        if($n > 0) $sql .= ' AND ';
                        $sql .= " {$pk} = {$this->entity[$pk]}";
                        $n++;
                    }
                    
                } else $sql .= " WHERE {$this->_primary} = {$this->entity[$this->_primary]}";
                    
            } else {
                
                $this->treatEntity();
                
                $entityValues = $this->getEntityValues(false);
                
                $sql = "INSERT INTO {$this->_names}";
                
                // Colocando nome das colunas da tabela.
                $sql .= "( " . implode(", ", array_keys($entityValues)) . " )";

                // Colocando valores referente às colunas da tabela.
                $sql .= " VALUES ( " . utf8_decode(implode(", ", $entityValues)) . " )";
            }
            
            ver($sql,d);
            
            if( is_array( $this->_primary ) ) $sql .= " RETURNING {$this->_primary[0]}" ;
            else  $sql .= " RETURNING {$this->_primary}";
            $result = $this->_db->pegaUm($sql);
            $this->_db->commit();
            return $result;
        } else {
            return false;
        }
    }
    
    /**
     * Valida a entidade.
     * Pode ser chamada toda vez que necessitar saber se os dados populados na entidade estao corretos de acordo com a tabela do banco de dados.
     * 
     * @name isValid
     * @access public
     * @return boolean - true / false
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
     * @since 22/08/2013
     */
    public function isValid()
    {
        
        foreach($this->entity as $nameColumn => $column){
            
            // Se for chave primaria e tiver valor, valida inteiro somente sem nenhuma outra validacao.
            if($column['contraint'] == 'pk' && $column['value']){
                $validate = new Zend_Validate_Digits();

            } else {
                
//______________________________________________________________________________ VALIDANDO POR TIPO DA COLUNA NO BANCO
                // Se tiver valor comeca validacoes de acordo com o tipo.
                // Se nao pergunta se é obrigatorio, se for obrigatorio valida.
                if($column['value']){
                    
                    // Se for inteiro
                    if($column['type'] == 'integer' || $column['type'] == 'numeric' || $value['data_type'] == 'smallint'){
                        $validate = new Zend_Validate_Digits();
                        
                    // Se for data
                    } else if($column['type'] == 'date' ){
                        $validate = new Zend_Validate_Date('dd/MM/yyyy');
                        
                    // Se for texto
                    } else if($column['type'] == 'character' || $column['type'] == 'character varying'){
                        
                    // Se for Bollean
                    } else if($column['type'] == 'boolean' ) {
                        
                    } else {
                        ver($column,d);
                    }
                    
//______________________________________________________________________________ VALIDANDO POR LIMITE DA COLUNA NO BANCO
                    // Validando quantidade de caracteres.
                    if($column['maximum']){
                        $validateMax = new Zend_Validate_StringLength(array('max' => '11'));
                        $validateMax->isValid($column['value']);
                        if($validateMax->getErrors()){
                           $this->error[] = array("name" => $nameColumn , "msg" => utf8_encode(reset($validateMax->getMessages())));
                        }
                        $validateMax = null;
                    }
                    
                } else {
                    
//______________________________________________________________________________ VALIDANDO POR NAO VAZIO DA COLUNA NO BANCO
                    // Validando se nao tiver valor e ele for obrigatorio.
                    if($column['is_nul'] == 'NO'){
                        $validate = new Zend_Validate_NotEmpty();
                    }
                }
                
//______________________________________________________________________________ VALIDANDO A CLOUNA E COLOCANDO A MENSAGEM
                if($validate){
                    $validate->isValid($column['value']);
                    if($validate->getErrors()){
                       $this->error[] = array("name" => $nameColumn , "msg" => utf8_encode(reset($validate->getMessages())));
                    }
                    
                    $validate = null;
                }
            }
        }
        
        if($this->error) return false;
        else return true;
    }
    
//    public function checkPk(){
//        
//        if(is_array($this->_primary)){
//            foreach($this->_primary as $pk )
//                if(empty($this->entity[$pk]['value']))
//                    return false;
//            
//            return true;
//        } else {
//            if(empty($this->entity[$this->_primary])) return false;
//            else return true;
//        }
//    }
    
    public function getByValues(array $data)
    {
        $sql = "SELECT * FROM {$this->_names} WHERE ";
        
        $n = 0;
        foreach($data as $key => $value){
            
            if($n > 0) $sql .= " AND ";
            
            $sql .= "{$key} = '{$value}'";
            
            $n++;
        }
        
        $result = $this->_db->pegaLinha($sql);
        return $result;
    }
    
    public function getAllByValues(array $data, array $ordersBy = null)
    {
        $sql = "SELECT * FROM {$this->_names} WHERE ";
        
        $n = 0;
        foreach($data as $key => $value){
            
            if($n > 0) $sql .= " AND ";
            
            $sql .= "{$key} = '{$value}'";
            
            $n++;
        }
        
        if($ordersBy){
            $sql .=' ORDER BY ';
            
            $n = 0;
            foreach($ordersBy as $orderBy){
                if($n > 0) $sql .= " , ";
                $sql .= $orderBy;
            }
        }
        
        
        $result = $this->_db->carregar($sql);
        
        return $result;
    }
    
    public function getAll($limit = NULL)
    {
        $sql = "SELECT * FROM {$this->_names} ";
        if($limit) $sql .= " LIMIT " . $limit;
        
        $result = $this->_db->carregar($sql);
        
        return $result;
    }
    
    public function fetchAll($where = array(), $order = null, $limit = NULL)
    {
        $sql  = "select * from {$this->_names} ";
        $sql .= $where ? ' where '. implode(' AND ', (array) $where ) : '' ;
        $sql .= $order ? " order by  $order" : '' ;
        $sql .= $limit ? " limit  $limit" : '' ;
        
        return $this->_db->carregar($sql);
    }
    
    public function delete($id)
    {
        $sql = "DELETE FROM {$this->_names} WHERE {$this->_primary} = {$id}";
        $result = $this->_db->executar($sql);
        $this->_db->commit();
        
        return $result;
    }
}