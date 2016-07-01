<?php

class Acomporc_Model_AcompanhamentoSubacao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.acompanhamentosubacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "asaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'asaid' => null,
        'prfid' => null,
        'sbacod' => null,
        'docid' => null,
        'usucpf' => null,
        'dataultimaatualizacao' => null
    );

    /**
     * Popular prfid com __set antes de chamar esta função.
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array(asaid,sbacod,esddsc)
     */
    public function listar()
    {
        $pflcod = PFL_COORDENADORSUBACAO;
        $query = <<<DML
            SELECT
                ss.sbacod as codigo,
                ss.sbacod,
                COALESCE(SPLIT_PART(usu.usunome, ' ', 1)||' '|| CASE WHEN LENGTH(SPLIT_PART(usu.usunome, ' ', 2)) < 3 THEN SPLIT_PART(usu.usunome, ' ', 2)||' '|| SPLIT_PART(usu.usunome, ' ', 3) ELSE SPLIT_PART(usu.usunome, ' ', 2) END, 'SUBAÇÃO SEM COORDENADOR') AS coordenador,
                COALESCE(esd.esddsc,'Não iniciado') AS situacao,
                ss.prfid,
                usu.usucpf,
                ((SELECT COUNT(analiseexecucao) FROM acomporc.acompanhamentosubacaoptres WHERE sbacod = ss.sbacod AND prfid = ss.prfid and analiseexecucao <> '') +
		(SELECT COUNT(metafisicareprogramada) FROM acomporc.acompanhamentosubacaoptres WHERE sbacod = ss.sbacod AND prfid = ss.prfid )) * 100 /
		(COUNT(ss.ptres) *2) AS progresso
            FROM acomporc.snapshotsubacao ss
            LEFT JOIN {$this->stNomeTabela} sa ON (ss.sbacod = sa.sbacod AND ss.prfid = sa.prfid)
            LEFT JOIN workflow.documento doc ON (sa.docid = doc.docid)
            LEFT JOIN workflow.estadodocumento esd ON (doc.esdid = esd.esdid)
            LEFT JOIN acomporc.usuarioresponsabilidade ur ON (ss.sbacod = ur.sbacod AND ss.prfid = ur.prfid AND ur.pflcod = {$pflcod} and ur.rpustatus = 'A')
            LEFT JOIN seguranca.usuario usu ON(ur.usucpf = usu.usucpf)
            WHERE ss.prfid = {$this->arAtributos['prfid']}
            GROUP BY ss.sbacod,coordenador,esd.esddsc,ss.prfid,usu.usucpf
            ORDER BY ss.sbacod;
DML;
        $retorno = $this->carregar($query);
        if(!$retorno){
            return array();
        }
        return $retorno;
    }

    /**
     *
     * Popular asaid com __set antes de chamar esta função.
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array('asaid,sbacod,sbatitulo')
     */
    public function pegaDadosSubacao()
    {
        $pflcod = PFL_COORDENADORSUBACAO;
        $query = <<<DML
            SELECT
                sa.asaid,
                ss.sbacod,
                mps.sbatitulo,
                mpg.pigdsc,
                usu.usucpf,
                usu.usunome,
                sa.docid
            FROM acomporc.snapshotsubacao ss
            LEFT JOIN {$this->stNomeTabela} sa ON (ss.sbacod = sa.sbacod AND ss.prfid = sa.prfid)
            JOIN monitora.pi_subacao mps ON (ss.sbacod = mps.sbacod AND mps.sbaano = '{$_SESSION['exercicio']}' AND mps.sbastatus = 'A')
            LEFT JOIN monitora.pi_gestor mpg ON (mps.pigid = mpg.pigid)
            LEFT JOIN acomporc.usuarioresponsabilidade ur ON ss.sbacod = ur.sbacod AND ss.prfid = ur.prfid AND ur.pflcod = {$pflcod} AND ur.rpustatus = 'A'
            LEFT JOIN seguranca.usuario usu ON(ur.usucpf = usu.usucpf)
            WHERE ss.sbacod = '{$this->arAtributos['sbacod']}' AND ss.prfid = {$this->arAtributos['prfid']}
DML;
        return $this->pegaLinha($query);
    }

   
}