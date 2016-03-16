<?php

class Acomporc_Model_SnapshotLocalizador extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.snapshotlocalizador";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sslid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prfid' => null,
        'sslid' => null,
        'prfid' => null,
        'acaid' => null,
        'acacod' => null,
        'unicod' => null,
        'loccod' => null,
        'funcod' => null,
        'sfncod' => null,
        'prgcod' => null,
        'tipoinclusaoacao' => null,
        'tipoinclusaolocalizador' => null,
        'prddescricao' => null,
        'unmdescricao' => null,
        'dotacaoinicial' => null,
        'dotacaoatual' => null,
        'empenhado' => null,
        'liquidado' => null,
        'pago' => null,
        'rapinscritoliquido' => null,
        'rapliquidadoapagar' => null,
        'rappago' => null,
        'rapliquidadoefetivo' => null,
        'metafisica' => null,
        'financeiro' => null,
        'dataultimatualizacao' => null
    );


  

    public function resumoLocalizadores()
    {
        if (isset($this->arAtributos['prfid'])) {
            $prfid = $this->arAtributos['prfid'];
        } else {
            $prfid = 12; // COLOCAR A FUNÇÃO PARA PEGAR O PERIODO ATUAL DE AÇÃO
        }
            
        $sql = <<<DML
SELECT DISTINCT aca.esfcod,
                aca.funcod,
                aca.sfucod,
                ssl.unicod,
                ssl.prgcod,
                ssl.acacod,
                ssl.loccod,
                doc.esdid
  FROM {$this->stNomeTabela} ssl
    LEFT JOIN monitora.acao aca ON aca.acaid = ssl.acaid
    LEFT JOIN acomporc.acompanhamentolocalizador acl ON acl.acaid = ssl.acaid AND acl.prfid = ssl.prfid
    LEFT JOIN workflow.documento doc ON doc.docid = acl.docid
  WHERE ssl.unicod = '{$this->arAtributos['unicod']}'
    AND ssl.prfid = '{$prfid}'
    AND ssl.acacod = '{$this->acacod}'
  ORDER BY ssl.unicod,
           ssl.prgcod,
           ssl.acacod,
           ssl.loccod
DML;
        return $this->carregar($sql);
    }

    /**
     * Popular sslid com __set antes de chamar esta função.
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array()
     */
    public function pegaDadosAcao()
    {
        $validAcao = PFL_VALIDADORACAO;
        $coordAcao = PFL_COORDENADORACAO;
        $query = <<<DML
            SELECT
                ssl.sslid,
                ssl.prfid,
                ssl.acacod,
                ssl.unicod,
                ssl.loccod,
                ssl.prgcod,
                ssl.prddescricao as produto,
                ssl.unmdescricao as unidade_medida,
                ssl.dotacaoinicial,
                ssl.dotacaoatual,
                ssl.empenhado,
                ssl.liquidado,
                ssl.pago,
                ssl.rapinscritoliquido,
                ssl.rapliquidadoapagar,
                ssl.rappago,
                ssl.rapliquidadoefetivo,
                ssl.metafisica,
                ssl.prgcod || '.<b>' || ssl.acacod || '</b>.' || ssl.unicod || '.' || ssl.loccod AS ptres,
                ssl.acaid,
                ssl.prgcod as prgcod,
                ssl.financeiro,
                to_char(ssl.dataultimatualizacao, 'DD/MM/YYYY') AS dataultimatualizacao,
                doc.esdid,
                acl.aclid,
                acl.docid,
                acl.fisicoexecutado,
                acl.fisicoexecutadorap,
                to_char(acl.dataapuracao,'DD/MM/YYYY' )AS dataapuracao,
                acl.reprogramacaofisica,
                acl.reprogramacaofinanceira,
                acl.ultimoretornosiop,
                aca.acadsc AS descricao,
                aca.prgdsc AS programa,
                (SELECT ur.usucpf||','|| SPLIT_PART(usu.usunome, ' ', 1)||' '|| CASE WHEN LENGTH(SPLIT_PART(usu.usunome, ' ', 2)) < 3 THEN SPLIT_PART(usu.usunome, ' ', 2)||' '|| SPLIT_PART(usu.usunome, ' ', 3) ELSE SPLIT_PART(usu.usunome, ' ', 2) END ||','|| usu.usuemail||','|| ur.rpudata_inc||','|| usu.usufoneddd||','|| usu.usufonenum FROM acomporc.usuarioresponsabilidade ur JOIN seguranca.usuario usu ON (ur.usucpf = usu.usucpf) WHERE ur.pflcod = '{$coordAcao}' AND ur.acacod = '{$this->arAtributos['acacod']}' AND ur.prfid = {$this->arAtributos['prfid']} AND ur.unicod = '{$this->arAtributos['unicod']}' AND ur.rpustatus = 'A' ORDER BY rpudata_inc desc LIMIT 1) AS coordenador,
                (SELECT ur.usucpf||','|| SPLIT_PART(usu.usunome, ' ', 1)||' '|| CASE WHEN LENGTH(SPLIT_PART(usu.usunome, ' ', 2)) < 3 THEN SPLIT_PART(usu.usunome, ' ', 2)||' '|| SPLIT_PART(usu.usunome, ' ', 3) ELSE SPLIT_PART(usu.usunome, ' ', 2) END||','|| usu.usuemail||','|| ur.rpudata_inc||','|| usu.usufoneddd||','|| usu.usufonenum FROM acomporc.usuarioresponsabilidade ur JOIN seguranca.usuario usu ON (ur.usucpf = usu.usucpf) WHERE ur.pflcod = '{$validAcao}' AND ur.acacod = '{$this->arAtributos['acacod']}' AND ur.prfid = {$this->arAtributos['prfid']} AND ur.unicod = '{$this->arAtributos['unicod']}' AND ur.rpustatus = 'A' ORDER BY rpudata_inc desc LIMIT 1) AS validador
            FROM acomporc.snapshotlocalizador ssl
            LEFT JOIN monitora.acao aca ON aca.acaid = ssl.acaid
            LEFT JOIN public.localizador loc ON loc.loccod = ssl.loccod
            LEFT JOIN acomporc.acompanhamentolocalizador acl ON acl.acaid = ssl.acaid AND acl.prfid = ssl.prfid
            LEFT JOIN workflow.documento doc ON acl.docid = doc.docid
            WHERE ssl.unicod = '{$this->arAtributos['unicod']}'
                AND ssl.acacod ='{$this->arAtributos['acacod']}'
                AND ssl.prfid = {$this->arAtributos['prfid']}
DML;
        return $this->pegaLinha($query);
    }
}