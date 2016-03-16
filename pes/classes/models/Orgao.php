<?php

class Model_Orgao extends Abstract_Model
{
    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesorgao';

    public function carregarOrgaoPorCpf($cpf)
    {
        $sql = "SELECT distinct o.orgcodigo, o.orgnome
                FROM pes.pesorgao o
                LEFT JOIN pes.pesunidadeorcamentaria uo on uo.orgcodigo = o.orgcodigo
                LEFT JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                LEFT JOIN pes.usuarioresponsabilidade ur ON ur.entcodigo = e.entcodigo
                WHERE ur.usucpf = '{$cpf}'
                AND ur.rpustatus = 'A'
                ORDER BY o.orgnome";

        $result = $this->_db->carregar($sql);
        return $result;
    }
}
