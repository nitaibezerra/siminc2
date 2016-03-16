<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of RelgDados
 *
 * @author LindalbertoFilho
 */
class Acomporc_Model_RelgDados extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.relgdados";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rldid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rldid' => null,
        'rlgid' => null,
        'rldtipo' => null,
        'rldcod' => null,
        'rldtipocod' => null,
        'rldtitulo' => null,
        'rldiniciativa' => null,
        'rldobjetivo' => null,
        'rldcodigoobjetivo' => null,
        'rldprograma' => null,
        'rldcodigoprograma' => null,
        'rldtipoprograma' => null,
        'rldunicod' => null,
        'rldacaoprioritaria' => null,
        'rldacaoprioritariatipo' => null,
        'rlddotacaoinicial' => null,
        'rlddotacaofinal' => null,
        'rlddespempenhada' => null,
        'rlddespliquidada' => null,
        'rlddesppaga' => null,
        'rldrapinscprocessado' => null,
        'rldrapinscnaoprocessado' => null,
        'rlddescmeta' => null,
        'rldunidademedida' => null,
        'rldrapeaem0101' => null,
        'rldrapeavalorliquidado' => null,
        'rldrapeavalorcancelado' => null,
        'rldrapeadescricaometa' => null,
        'rldrapeaunidademedida' => null,
        'rldrapearealizado' => null,
        'rldmontanteprevisto' => null,
        'rldmontantereprogramado' => null,
        'rldmontanterealizado' => null,
        'docid' => null
    );

    /**
     * Preencher rlgid, rldid antes de prosseguir.
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array
     */
    public function buscaRelatorioPOJustificaRelatorio()
    {
        #$columns = implode(', ',array_keys($this->arAtributos));
        $query = <<<DML
            SELECT rld.*,
                doc.esdid,
                usu.usunome,
                rpu.usucpf
            FROM {$this->stNomeTabela} rld
            LEFT JOIN acomporc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
            LEFT JOIN seguranca.usuario usu ON (rpu.usucpf = usu.usucpf)
            LEFT JOIN workflow.documento doc USING(docid)
            WHERE rld.rlgid = {$this->arAtributos['rlgid']}
                AND rld.rldid = {$this->arAtributos['rldid']}
DML;
        return $this->pegaLinha($query);

    }

    /**
     * Preencher rlgid antes de prosseguir.
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array
     */
    public function buscaRelatorioNormal()
    {
        $query = <<<DML
            SELECT DISTINCT rld.rldid,
                rld.rlgid,
                COALESCE(rld.rldtipocod, aca.acatipodsc) AS rldtipocod,
                rlg.acacod AS codigo,
                rlg.unicod,
                COALESCE(rld.rldtitulo, aca.acatitulo) AS rldtitulo,
                COALESCE(rld.rldiniciativa, aca.acainiciativacod || ' - ' || aca.acainiciativadsc) AS rldiniciativa,
                COALESCE(rld.rldobjetivo, aca.acaobjetivodsc) AS rldobjetivo,
                COALESCE(rld.rldcodigoobjetivo, aca.acaobjetivocod) AS rldcodigoobjetivo,
                COALESCE(rld.rldprograma, aca.prgdsc) AS rldprograma,
                COALESCE(rld.rldcodigoprograma, aca.prgcod) AS rldcodigoprograma,
                COALESCE(rld.rldtipoprograma, aca.prgtipo) AS rldtipoprograma,
                rld.rldacaoprioritaria,
                rld.rldacaoprioritariatipo,
                rld.rlddotacaoinicial,
                rld.rlddotacaofinal,
                rlddespempenhada,
                rlddespliquidada,
                rlddesppaga,
                rldrapinscprocessado,
                rldrapinscnaoprocessado,
                COALESCE(rld.rldunidademedida, unm.unmdsc) AS rldunidademedida,
                rld.rlddescmeta,
                rldrapeaem0101,
                rldrapeavalorliquidado,
                rldrapeavalorcancelado,
                rldrapeadescricaometa,
                rldrapeaunidademedida,
                rldrapearealizado,
                rldmontanteprevisto,
                rldmontantereprogramado,
                rldmontanterealizado,
                rld.docid,
                doc.esdid,
                usu.usunome,
                rpu.usucpf
            FROM acomporc.relgestao rlg
            LEFT JOIN acomporc.relgdados rld ON(rlg.rlgid = rld.rlgid AND rld.rldtipo = 'acao')
            LEFT JOIN acomporc.periodoreferencia pr ON(rlg.prfid = pr.prfid)
            LEFT JOIN monitora.acao aca ON(rlg.acacod = aca.acacod AND rlg.unicod = aca.unicod AND pr.prsano = aca.prgano)
            LEFT JOIN public.unidademedida unm USING(unmcod)
            LEFT JOIN acomporc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
            LEFT JOIN seguranca.usuario usu ON (rpu.usucpf = usu.usucpf)
            LEFT JOIN workflow.documento doc USING(docid)
            WHERE rlg.rlgid = {$this->arAtributos['rlgid']}
                AND aca.prgdsc IS NOT NULL
DML;
        return $this->pegaLinha($query);
    }

    /**
     * Preencher rlgid antes de prosseguir.
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array
     */
    public function buscaRelatorioRAP()
    {
        #$columns = array_keys($this->arAtributos);
        #ver($columns,d);
        $query = <<<DML
            SELECT rld.*,
                rlg.acacod AS codigo,
                rlg.unicod,
                usu.usunome,
                rpu.usucpf,
                doc.esdid
            FROM acomporc.relgdados rld
            LEFT JOIN acomporc.relgestao rlg USING(rlgid)
            LEFT JOIN acomporc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
            LEFT JOIN seguranca.usuario usu ON (rpu.usucpf = usu.usucpf)
            LEFT JOIN workflow.documento doc USING(docid)
            WHERE rld.rlgid = {$this->arAtributos['rlgid']}
                AND rld.rldtipo = 'acaorap'
DML;
        return $this->pegaLinha($query);
    }

    public function apagar()
    {
        $query = <<<DML
            DELETE FROM acomporc.relgdados WHERE rlgid = {$this->arAtributos['rlgid']}
DML;
        $this->executar($query);
    }
}
