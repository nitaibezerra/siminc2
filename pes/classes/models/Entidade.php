<?php

class Model_Entidade extends Abstract_Model
{

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesentidade';

    /**
     * Nome da chave primaria
     * @var string
     */
    protected $_primary = 'entcodigo';

    /**
     * Carrega a ultima entidade que o usuario esteve no sistema.
     *
     * @param numeric $cpf
     * @return array
     */
    public function carregarTudoDaUltimaEntidadePorUsuario($cpf)
    {
        $sql = "SELECT urp.*, o.orgcodigo, o.orgnome, uo.uorcodigo , uo.uornome, e.entcodigo, e.entnome, o.aexano as organo, uo.aexano as uorano, e.aexano as entano
                FROM pes.pesorgao o
                LEFT JOIN pes.pesunidadeorcamentaria uo on uo.orgcodigo = o.orgcodigo
                LEFT JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                LEFT JOIN pes.usuarioresponsabilidade urp ON urp.entcodigo = e.entcodigo

                WHERE urp.usucpf = '{$cpf}'
                AND urp.rpuultimoacesso = true
                --AND o.aexano = " . AEXANO . "
                --AND uo.aexano = " . AEXANO . "
                --AND e.aexano = " . AEXANO;
        $result = $this->_db->pegaLinha($sql);

        return ($result)? $result : array();
    }

    public function carregarTudoDaEntidadePorUsuario($cpf)
    {
        $sql = "SELECT DISTINCT urp.*, o.orgcodigo, o.orgnome, uo.uorcodigo , uo.uornome, e.entcodigo, e.entnome, o.aexano as organo, uo.aexano as uorano, e.aexano as entano
                FROM pes.pesorgao o
                LEFT JOIN pes.pesunidadeorcamentaria uo on uo.orgcodigo = o.orgcodigo
                LEFT JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                LEFT JOIN pes.usuarioresponsabilidade urp ON urp.entcodigo = e.entcodigo
                WHERE urp.usucpf = '{$cpf}'
                --AND o.aexano = " . AEXANO . "
                --AND uo.aexano = " . AEXANO . "
                --AND e.aexano = " . AEXANO;

        $result = $this->_db->carregar($sql);

        return ($result)? $result : array();
    }

    public function carregarTudoDaEntidadePorEntidade($entcodigo)
    {
        $sql = "SELECT DISTINCT urp.*, o.orgcodigo, o.orgnome, uo.uorcodigo , uo.uornome, e.entcodigo, e.entnome, o.aexano as organo, uo.aexano as uorano, e.aexano as entano
                FROM pes.pesorgao o
                LEFT JOIN pes.pesunidadeorcamentaria uo on uo.orgcodigo = o.orgcodigo
                LEFT JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                LEFT JOIN pes.usuarioresponsabilidade urp ON urp.entcodigo = e.entcodigo
                WHERE e.entcodigo = '{$entcodigo}'
                --AND o.aexano = " . AEXANO . "
                --AND uo.aexano = " . AEXANO . "
                --AND e.aexano = " . AEXANO;

        $result = $this->_db->pegaLinha($sql);

        return ($result)? $result : array();
    }

    public function carregarEntidadePorUOECpf($uorcodigo, $cpf)
    {
        $sql = "SELECT DISTINCT o.orgcodigo, o.orgnome, uo.uorcodigo , uo.uornome, e.entcodigo, e.entnome --, o.aexano as organo, uo.aexano as uorano, e.aexano as entano
                FROM pes.pesorgao o
                LEFT JOIN pes.pesunidadeorcamentaria uo on uo.orgcodigo = o.orgcodigo
                LEFT JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                LEFT JOIN pes.usuarioresponsabilidade urp ON urp.entcodigo = e.entcodigo
                WHERE uo.uorcodigo = '{$uorcodigo}'
                AND urp.usucpf = '{$cpf}'
                AND urp.rpustatus = 'A'
                --AND o.aexano = " . AEXANO . "
                --AND uo.aexano = " . AEXANO . "
                --AND e.aexano = " . AEXANO . "
                ORDER BY e.entnome";

        $result = $this->_db->carregar($sql);

        return ($result)? $result : array();
    }
}
