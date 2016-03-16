<?php

class Acomporc_Model_AcompanhamentoLocalizador extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.acompanhamentolocalizador";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aclid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aclid' => null,
        'acaid' => NULL,
        'prfid' => NULL,
        'docid' => NULL,
        'fisicoexecutado' => NULL,
        'fisicoexecutadorap' => NULL,
        'dataapuracao' => NULL,
        'reprogramacaofisica' => NULL,
        'reprogramacaofinanceira' => NULL,
        'dataultimaatualizacao' => NULL
    );

    public function gravarAcompanhamento()
    {
        if($this->arAtributos['aclid'] == ''){
            $this->arAtributos['aclid'] = $this->inserir();
            $this->commit();
        }else{
            if($this->alterar()){
                $this->commit();
            }
        }
        return $this->arAtributos['aclid'];
    }

    public function pegaDadosAcompanhamento(array $criterio)
    {
        $sql = <<<DML
SELECT acaid,
       aca.esfcod,
       aca.unicod,
       ssl.funcod,
       ssl.sfncod,
       ssl.prgcod,
       ssl.acacod,
       ssl.tipoinclusaoacao,
       ssl.loccod,
       ssl.tipoinclusaolocalizador,
       acl.fisicoexecutado, -- realizadoLOA
       acl.fisicoexecutadorap, -- realizadoRAP
       acl.reprogramacaofisica, -- reprogramado
       acl.reprogramacaofinanceira, -- limite
       acl.aclid,
       TO_CHAR(acl.dataapuracao, 'DD/MM/YYYY') AS dataapuracao -- LOA e RAP
  FROM {$this->stNomeTabela} acl
    INNER JOIN acomporc.snapshotlocalizador ssl USING(prfid, acaid)
    INNER JOIN monitora.acao aca USING(acaid)
  WHERE ssl.acacod = :acacod
    AND prfid = :prfid
    AND ssl.unicod = :unicod
DML;
        $dml = new Simec_DB_DML($sql);
        $dml->addParams($criterio);

        return $this->carregar($dml);
    }

    public static function getPermissaoAcoes($usucpf, array $listaPerfis)
    {
        $restricao = '';
        if (in_array(PFL_SUPERUSUARIO, $listaPerfis)
            || in_array(PFL_CGP_GESTAO, $listaPerfis)
            || in_array(PFL_CGP_OPERACIONAL, $listaPerfis))
        {
            return '';
        }

        $restricao = <<<DML
            AND EXISTS (SELECT 1
                          FROM acomporc.usuarioresponsabilidade ur
                          WHERE ur.acacod = ssl.acacod
                            AND ur.unicod = ssl.unicod
                            AND ur.prfid = ssl.prfid
                            AND ur.usucpf = '{$usucpf}'
                            AND ur.pflcod = %d
                            AND ur.rpustatus = 'A')
DML;
        $restricao = sprintf(
            $restricao,
            in_array(PFL_COORDENADORACAO, $listaPerfis)
                ?PFL_COORDENADORACAO
                :PFL_CONSULTA
        );

        return $restricao;
    }
}

