<?php

class Model_UnidadeOrcamentaria extends Abstract_Model
{
    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesunidadeorcamentaria';
    protected $_schema = 'pes';

    public function carregarUOPorOrgaoECpf($orgcodigo, $cpf)
    {
        $sql = "
                SELECT distinct uo.uorcodigo, uo.uornome
                FROM pes.pesorgao o
                LEFT JOIN pes.pesunidadeorcamentaria uo on uo.orgcodigo = o.orgcodigo
                LEFT JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                LEFT JOIN pes.usuarioresponsabilidade ur ON ur.entcodigo = e.entcodigo
                WHERE ur.usucpf = '{$cpf}'
                AND ur.rpustatus = 'A'
                AND o.orgcodigo = '{$orgcodigo}'
                ORDER BY uo.uornome";

        $result = $this->_db->carregar($sql);
        return $result;
    }

    public function carregarDespesaNaturezaPorUO($uorcodigo)
    {
        $sql = "SELECT DISTINCT nat.*, tid.* FROM pes.pesunidadeorcamentaria uor
                    LEFT JOIN pes.pestipodespesanaturezauo tdn ON (uor.uorcodigo = tdn.uorcodigo )
                    LEFT JOIN pes.pestipodespesanatureza tid ON ( tdn.tidcodigo = tid.tidcodigo )
                    LEFT JOIN pes.pesnaturezadespesa nat ON ( tid.natcodigo = nat.natcodigo )
                WHERE uor.uorcodigo = '26101'
                AND tid.tidcodigo = 6
                AND uor.aexano = " . AEXANO . "
                AND tdn.aexano = " . AEXANO . "
                AND nat.natativo = true
                LIMIT 10";

        $result = $this->_db->carregar($sql);
        return ($result)? $result : array();
    }
}
