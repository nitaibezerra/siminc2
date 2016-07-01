<?php

class Model_UsuarioResponsabilidade extends Abstract_Model
{
    
    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'usuarioresponsabilidade';
    
    /**
     * Nome da chave primaria
     * @var string
     */
    protected $_primary = 'rpuid';

    public function desativarUltimoAcessoPorCpf($cpf)
    {
        $sql = "UPDATE {$this->_names} SET rpuultimoacesso = false WHERE usucpf = '{$cpf}' RETURNING usucpf";
        $return = $this->_db->carregar($sql);
        $this->_db->commit();
        return $return;
    }
    
    public function carregarTudoPorCpfPerfilEntidade($cpf , array $arrPflcod , $entcodigo)
    {
        $sql = "SELECT * 
                FROM {$this->_names} 
                WHERE usucpf = '{$cpf}'
                AND entcodigo = {$entcodigo}";
                
                if(count($arrPflcod) > 0){
                    $sql .= " AND pflcod IN (";
                    foreach($arrPflcod as $key => $pflcod){
                        if($key > 0) $sql .= ", ";
                        $sql .= $pflcod;
                    }
                    $sql .= " )";
                }
        $result = $this->_db->carregar($sql);
        
        return ($result)? $result : array();
    }
}
