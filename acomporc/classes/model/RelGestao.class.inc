<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of RelGestao
 *
 * @author LindalbertoFilho
 */
class Acomporc_Model_RelGestao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.relgestao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rlgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rlgid' => null,
        'unicod' => null,
        'acacod' => null,
        'prfid' => null,
        'dtultimaatualizacao' => null,
        'usucpf' => null,
        'acatipo' => null
    );

    public function verificaExistencia()
    {
        $query = <<<DML
            SELECT rlgid
            FROM {$this->stNomeTabela}
            WHERE unicod = '{$this->arAtributos['unicod']}'
                AND prfid = '{$this->arAtributos['prfid']}'
                AND acacod = '{$this->arAtributos['acacod']}'
DML;
        return $this->pegaUm($query);
    }

    public function novo()
    {
        $query = <<<DML
            INSERT INTO acomporc.relgestao(unicod, acacod, prfid, usucpf, acatipo)
                VALUES('{$this->arAtributos['unicod']}',
                    '{$this->arAtributos['acacod']}',
                    '{$this->arAtributos['prfid']}',
                    '{$this->arAtributos['usucpf']}',
                    '{$this->arAtributos['acatipo']}')
            RETURNING rlgid
DML;
        $rlgid = $this->pegaUm($query);
        $this->commit();
        return $rlgid;
    }

    public function buscaRelatorio()
    {
        $query = <<<DML
            SELECT
                rgt.unicod,
                uni.unidsc,
                rgt.acacod,
                per.prftitulo,
                rgt.acatipo
            FROM acomporc.relgestao rgt
            INNER JOIN public.unidade uni USING(unicod)
            INNER JOIN acomporc.periodoreferencia per USING (prfid)
            WHERE rlgid = {$this->arAtributos['rlgid']}
DML;
        return $this->pegaLinha($query);
    }

    public function consultarDadosTCU($rldid)
    {
        $query = <<<DML
            SELECT
                CASE
                    WHEN rld.rldtipo = 'po'
                    THEN rld.rldcod
                ELSE rlg.acacod
                END AS codigo,
                rlg.unicod,
                per.prsano as exercicio,
                rld.*
            FROM {$this->stNomeTabela} rlg
            INNER JOIN acomporc.relgdados rld USING(rlgid)
            INNER JOIN acomporc.periodoreferencia per ON (rlg.prfid = per.prfid)
            WHERE rlg.rlgid = %d __RLDID__
            ORDER BY rld.rldtipo
DML;

        $params = is_null($rldid)? array($this->arAtributos['rlgid']): array($this->arAtributos['rlgid'], $rldid);
        $query = str_replace(
           '__RLDID__',
           (is_null($rldid)?'':' AND rld.rldid = %d'),
           $query
        );
        $stmt = vsprintf($query, $params);

        if (!$dados = $this->carregar($stmt)) {
            throw new Exception('Dados não encontrados.');
        }
        return $dados;
    }

    public function listarAcompanhamento()
    {
        $query = <<<DML
            SELECT rg.rlgid, rg.unicod ||' - '|| un.unidsc as unidade, acacod, acatipo,
                COALESCE(
                    (SELECT esddsc
                    FROM acomporc.relgdados rld
                    INNER JOIN workflow.documento using(docid)
                    INNER JOIN workflow.estadodocumento USING(esdid)
                    WHERE rld.rlgid = rg.rlgid
                        AND rld.rldtipo IN('acao', 'acaorap')
                    LIMIT 1
                    ), 'Em preenchimento') AS esddsc
            FROM {$this->stNomeTabela}  rg
            JOIN public.unidade un ON (rg.unicod = un.unicod)
            WHERE prfid = {$this->arAtributos['prfid']}
DML;
        $retorno = $this->carregar($query);
        if(!$retorno){
            return array();
        }
        return $retorno;
    }
}