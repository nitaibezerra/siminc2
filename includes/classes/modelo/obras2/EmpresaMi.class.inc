<?php

class EmpresaMi extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.empresami";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("emiid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emiid' => null,
        'emicnpj' => null,
        'emidsc' => null,
        'emistatus' => null,
    );

    public function pegaReponsaveis($obrid, $pflcod = null)
    {
        $pflcod = ($pflcod) ? " AND rpu.pflcod = $pflcod" : "";

        $sql = "
            SELECT
                usu.usucpf   as cpf,
                usu.usunome  as nome,
                usu.usuemail as email
            FROM obras2.obras o
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio mun ON mun.muncod = ed.muncod
            JOIN obras2.empresami_uf euf ON euf.estuf = mun.estuf AND eufstatus = 'A'
            JOIN obras2.usuarioresponsabilidade rpu ON rpu.rpustatus = 'A' AND euf.emiid = rpu.emiid $pflcod
            JOIN seguranca.usuario usu ON usu.usucpf = rpu.usucpf
            WHERE o.obrid = $obrid
        ";

        $result = $this->carregar($sql);
        return (!$result) ? array() : $result;
    }

    public function pegaEmpresaMI($estuf)
    {
        $sql = "SELECT
                '( ' || TO_CHAR(em.emicnpj::int8, '00\".\"000\".\"000\"/\"0000\"-\"00') || ' ) ' || em.emidsc AS descricao
            FROM
                obras2.empresami em
                JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A' AND euf.estuf = '{$estuf}'
            WHERE
                emistatus = 'A'";
        
        return $this->pegaUm($sql);
    }
    
    public function listaCombo($estuf)
    {
        if (!empty($estuf)) {
            $joinEstuf = "JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A' AND euf.estuf = '{$estuf}'";
        }
        $where = '';
        if (possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_ADMINISTRATIVO)) && !$this->testa_superuser()) {
            $where = "
                       AND em.emiid IN (SELECT urs.emiid FROM obras2.usuarioresponsabilidade urs
                                            WHERE  urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'
                                            GROUP BY urs.emiid)
                       ";
        }

        $sql = <<<EOT
            SELECT
                em.emiid AS codigo,
                '( ' || TO_CHAR(em.emicnpj::int8, '00"."000"."000"/"0000"-"00') || ' ) ' || em.emidsc AS descricao
            FROM
                obras2.empresami em
                $joinEstuf
            WHERE
                emistatus = 'A' $where
            ORDER BY
                em.emicnpj, em.emidsc
EOT;

        $dados = $this->carregar($sql);
        return (is_array($dados) ? $dados : array());
    }

    public function listaComboComEstuf()
    {
        $sql = <<<EOT
            SELECT
                em.emiid AS codigo,
                euf.estuf || ' - ' || TO_CHAR(em.emicnpj::int8, '00"."000"."000"/"0000"-"00') || ' ' || em.emidsc AS descricao
            FROM
                obras2.empresami em
            INNER JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A'
            WHERE
                emistatus = 'A'
            ORDER BY
                euf.estuf, em.emicnpj, em.emidsc
EOT;

        $dados = $this->carregar($sql);
        return (is_array($dados) ? $dados : array());
    }

    public function verificaAcesso($obrid)
    {

        if (possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_ADMINISTRATIVO))) {
            $sql = "
            SELECT COUNT(obrid) FROM
              obras2.obras o
            LEFT JOIN entidade.endereco ed ON ed.endid = o.endid
            LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
            LEFT JOIN obras2.empresami_uf euf ON euf.estuf = mun.estuf AND euf.eufstatus = 'A'
            LEFT JOIN obras2.empresami emi ON emi.emiid = euf.emiid AND emi.emistatus = 'A'
            WHERE o.obrid = {$obrid} AND o.obridpai IS NULL AND o.obrstatus = 'A' AND emi.emiid IN (SELECT urs.emiid FROM obras2.usuarioresponsabilidade urs
                                            WHERE  urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'
                                            GROUP BY urs.emiid) AND o.tpoid IN (104, 105)
        ";
            $c = $this->pegaUm($sql);
            return ($c > 0) ? true : false;
        } else {
            return true;
        }
    }

}
