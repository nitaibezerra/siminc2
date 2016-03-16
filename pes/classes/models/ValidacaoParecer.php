<?php

class Model_ValidacaoParecer extends Abstract_Model
{
    
    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesvalidacaoparecer';
    
    /**
     * Nome da chave primaria
     * @var string
     */
    protected $_primary = 'vapcodigo';
    
    
    public function carregarResponsaveisPorEntidade($entcodigo)
    {
        $sql = "SELECT * FROM pes.usuarioresponsabilidade usr
                INNER JOIN seguranca.usuario u on usr.usucpf = u.usucpf
                WHERE usr.entcodigo = {$entcodigo}
                AND usr.pflcod = " . K_PERFIL_CADASTRADOR_UO;
                
        $result = $this->_db->carregar($sql);
        
        return ($result)? $result : array();
    }
}
