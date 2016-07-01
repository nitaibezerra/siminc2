<?php
/**
 * Controle responsavel pelas entidades.
 * 
 * @author Equipe simec - Consultores OEI
 * @since  14/05/2013
 * 
 * @name       Entidade
 * @package    classes
 * @subpackage controllers
 * @version $Id
 */
abstract class Abstract_Model
{
    protected $_schema = 'pes';
    protected $_names;
    protected $_db;
    public $entity;
    public $error = null;
    
    public function __construct() {
//        header("Content-Type: text/html;  charset=UTF-8",true);
        
        $this->_names = $this->_schema . '.' . $this->_name;
        
        global $db; 
        $this->_db = $db;
        $this->getEntity();
    }
    
    /**
     * Action que lista os orgaos do usuario logado no sistema.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  14/05/2013
     * 
     * @name   listarOrgaoAction
     * @access public
     * @return void
     */
    public function getEntity()
    {
        $result = $this->_db->carregar("SELECT * FROM information_schema.columns WHERE table_schema = '{$this->_schema}' AND table_name = '{$this->_name}';");
        foreach($result as $key => $value) $this->entity[$value['column_name']] = '';
    }
    
    public function clearEntity()
    {
        foreach($this->entity as $key => &$entity)
            $this->entity[$key] = "";
    }
    
    public function populateEntity($data)
    {
        if(is_array( $this->_primary ) ){
            $primaryKeys = array();
            $booPrimaryKe = true;
            foreach($this->_primary as $primary){
                
                if(!$data[$primary]){
                        $booPrimaryKe = false;
                        break;
                } else $primaryKeys += array($primary => $data[$primary]);
            }
            
            if($booPrimaryKe) $entity = $this->getByValues($primaryKeys);
            else $entity = null;
            
        } else if(isset($data[$this->_primary]) && !empty($data[$this->_primary])){
            $entity = $this->getByValues(array($this->_primary => $data[$this->_primary]));
        } else $entity = null;
        
        if($entity) {
            foreach($entity as $key => $value) {
                if(isset($this->entity[$key]) && !is_null($value)) 
                    $this->entity[$key] = $value;
            }
        }
        
        foreach($data as $key => $value) {
            if(isset($this->entity[$key])) 
                $this->entity[$key] = $value;
        }
    }
    
    public function isValid()
    {
        $result = $this->_db->carregar("SELECT column_name, is_nullable, data_type, character_maximum_length FROM information_schema.columns WHERE table_schema = '{$this->_schema}' AND table_name = '{$this->_name}';");
        foreach($result as $value){
            
            if($value['column_name'] != $this->_primary){
            
                // Validando se é vazio.
                if($value['is_nullable'] == 'NO' && empty($this->entity[$value['column_name']])){
                    $this->error[] = array("name" => $value['column_name'] , "msg" => "Campo nao pode ser vazio!");

                // Validando tipo inteiro.
                } elseif(!empty($this->entity[$value['column_name']]) && ($value['data_type'] == 'integer' || $value['data_type'] == 'numeric' || $value['data_type'] == 'smallint') && !is_numeric($this->entity[$value['column_name']])){
                    $this->error[] = array("name" => $value['column_name'] , "msg" => "Campo nao e numerico!");

                // Validando limite de caracteres.
                } elseif ($value['character_maximum_length'] && $value['character_maximum_length'] < count($this->entity[$value['column_name']])){
                    $this->error[] = array("name" => $value['column_name'] , "msg" => "Campo excede o limite de caracteres!");
                } else if(!empty($this->entity[$value['column_name']]) && $value['data_type'] == 'date'){
                    
                    
                    if(strpos($this->entity[$value['column_name']], '/')){
                        $dateExplode = explode("/",$this->entity[$value['column_name']]);
                    
                        // Validando dia
                        if($dateExplode[0] > 31)
                            $this->error[] = array("name" => $value['column_name'] , "msg" => utf8_encode("Dia não existe no calendário!"));
                        
                        // Validando mes
                        if($dateExplode[1] > 12)
                            $this->error[] = array("name" => $value['column_name'] , "msg" => utf8_encode("Mês não existe no calendário!"));

                        $this->entity[$value['column_name']] = implode("-",array_reverse($dateExplode));
                    } else {
                        if(is_numeric($this->entity[$value['column_name']])){
                            $this->error[] = array("name" => $value['column_name'] , "msg" => utf8_encode("Data não é valida!"));
                        }
//                        $this->entity[$value['column_name']] = implode("-",array_reverse(explode("/",$date)));
                    }
                }
                // Tratando dados.
                if((empty($this->entity[$value['column_name']])) 
                    && 
                    (
                        $value['data_type'] == 'integer' 
                        || $value['data_type'] == 'numeric' 
                        || $value['data_type'] == 'timestamp with time zone' 
                        || $value['data_type'] == 'date'
                        || $value['data_type'] == 'smallint'
                    )
                    ){
    //                unset($this->entity[$value['column_name']]);
                    $this->entity[$value['column_name']] = "NULL";
                } else {
                    
//                    if($value['column_name'] =='natcodigo'){
//                        echo "<pre>";
//                        var_dump($value);
//                        exit;
//                    }
                    $this->entity[$value['column_name']] = "'{$this->entity[$value['column_name']]}'";
                }
            } else if(empty($this->entity[$value['column_name']])) unset($this->entity[$value['column_name']]);
            
        }
        
        if($this->error) return false;
        else return true;
    }
    
    public function checkPk(){
        
        if(is_array($this->_primary)){
            foreach($this->_primary as $pk )
                if(empty($this->entity[$pk]))
                    return false;
            
            return true;
        } else {
            if(empty($this->entity[$this->_primary])) return false;
            else return true;
        }
    }
    
    public function insert()
    {
        if($this->isValid() && !$this->error){
            
         $sql = "INSERT INTO {$this->_names}";
                
        // Colocando nome das colunas da tabela.
        $sql .= "( " . implode(", ", array_keys($this->entity)) . " )";

        // Colocando valores referente às colunas da tabela.
        $sql .= " VALUES ( " . utf8_decode(implode(", ", array_values($this->entity))) . " )";
        
        if( is_array( $this->_primary ) ) $sql .= " RETURNING {$this->_primary[0]}" ;
            else  $sql .= " RETURNING {$this->_primary}";
            
        $result = $this->_db->pegaUm($sql);
        $this->_db->commit();
//        $this->_db->rollback();

        return $result;
        } else return false;
    }
    
    public function save()
    {
        if($this->isValid() && !$this->error){
            
            // Se tiver valor na PK significa que esta editando.
            if($this->checkPk()){

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
                    foreach($this->_primary as $pk){
                        
                        if($n > 0) $sql .= ' AND ';
                        $sql .= " {$pk} = {$this->entity[$pk]}";
                        $n++;
                    }
                    
                } else $sql .= " WHERE {$this->_primary} = {$this->entity[$this->_primary]}";
                    
            } else {
                $sql = "INSERT INTO {$this->_names}";
                
                // Colocando nome das colunas da tabela.
                $sql .= "( " . implode(", ", array_keys($this->entity)) . " )";

                // Colocando valores referente às colunas da tabela.
//                $sql .= " VALUES ( " . implode(", ", array_values(utf8_decode($this->entity))) . " )";
                $sql .= " VALUES ( " . utf8_decode(implode(", ", array_values($this->entity))) . " )";
            }
            
            if( is_array( $this->_primary ) ) $sql .= " RETURNING {$this->_primary[0]}" ;
            else  $sql .= " RETURNING {$this->_primary}";
            $result = $this->_db->pegaUm($sql);
            $this->_db->commit();
            return $result;
        } else {
            return false;
        }
    }
    
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

ver($sql, d);
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