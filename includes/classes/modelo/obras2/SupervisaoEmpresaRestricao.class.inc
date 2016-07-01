<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of SupervisaoEmpresaRestricao
 *
 * @author lindalbertofilho
 */
class SupervisaoEmpresaRestricao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisaoempresarestricao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "id" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'sueid' => null,
        'dtinclusao' => null,
        'status' => null,
        'resposta' => null,
        'rstid' => null
    );

    public function verificaPreenchimento($obrid, $sueid)
    {
        $superada = ESDID_SUPERADA;
        $cancelada = ESDID_CANCELADA;
        $justificada = ESDID_JUSTIFICADA;
        $sql = <<<DML
            SELECT
                count(r.rstid)
            FROM obras2.restricao r
            INNER JOIN workflow.documento doc ON doc.docid  = r.docid
            WHERE r.rststatus = 'A'
                AND r.obrid = {$obrid}
                AND r.tprid = 13
                AND doc.esdid NOT IN ($superada,$cancelada,$justificada)
                AND r.rstid NOT IN (SELECT rstid FROM obras2.supervisaoempresarestricao ser WHERE rstid = r.rstid AND ser.sueid = $sueid AND ser.status = 'A')
DML;
        if ($this->pegaUm($sql) > 0) {
            return false;
        }
        return true;
    }
    
    public function salvarSupervisaoEmpresaRestricao($respostas, $sueid)
    {
        if(!$respostas) {
            return;
        }
        foreach($respostas as $rstid => $resposta) {
            if($resposta == '') {
                continue;
            }
            $sql = <<<DML
                SELECT 1 FROM obras2.supervisaoempresarestricao WHERE sueid = $sueid AND rstid = $rstid AND status = 'A';
DML;
            if(!$this->pegaUm($sql)) {
                $sql = <<<DML
                    INSERT INTO obras2.supervisaoempresarestricao (sueid, resposta, rstid) VALUES ($sueid,'$resposta',$rstid);
DML;
            } else {
                $sql = <<<DML
                    UPDATE obras2.supervisaoempresarestricao SET resposta = '$resposta' WHERE sueid = $sueid AND rstid = $rstid;
DML;
            }
            $this->executar($sql);
        }
    }

    public function buscarDocumentosRestricoesPosHomologar($sueid)
    {
        $aguardandoCorrecao = ESDID_AGUARDANDO_CORRECAO;
        $aguardandoAnaliseFnde = ESDID_AGUARDANDO_ANALISE_FNDE;
        $aguardandoProvidencia = ESDID_AGUARDANDO_PROVIDENCIA;
        $sql = <<<DML
            SELECT
                r.docid,
                doc.esdid
            FROM obras2.supervisaoempresarestricao  ser
            INNER JOIN obras2.restricao r ON ser.rstid = r.rstid AND r.rststatus ='A'
            INNER JOIN workflow.documento doc ON r.docid = doc.docid
            WHERE ser.sueid = $sueid
                AND ser.resposta IN ('S','SR')
                AND ser.status = 'A'
                AND doc.esdid IN ($aguardandoCorrecao, $aguardandoAnaliseFnde, $aguardandoProvidencia);
DML;
        return $this->carregar($sql);
    }

    public function buscarDadosPosHomologacaoEmpresa($sueid)
    {
        $sql = <<<DML
            SELECT
                ent.entnome
            FROM obras2.supervisaoempresa se
            JOIN obras2.supervisao_os so ON se.sosid = so.sosid
            LEFT JOIN obras2.supervisao_grupo_empresa sge ON so.sgeid = sge.sgeid
            LEFT JOIN entidade.entidade ent ON sge.entid = ent.entid
            WHERE se.sueid = $sueid
DML;
        return $this->pegaUm($sql);
    }

    public function retornaQueryCadastramento($obrid, $sueid)
    {
        $superada = ESDID_SUPERADA;
        $cancelada = ESDID_CANCELADA;
        $justificada = ESDID_JUSTIFICADA;
        $sr = '';
        if(possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))){
            $sr = "<label><input type=\'radio\' name=\'resposta['||r.rstid||']\' value=\'SR\' '||CASE WHEN ser.resposta = 'SR' THEN 'checked' ELSE '' END ||'> Sim com Ressalva </label>";
        }
        $sql = <<<DML
            SELECT
                '<label><input type=\'radio\' name=\'resposta['||r.rstid||']\' value=\'N\''||CASE WHEN ser.resposta = 'N' THEN 'checked' ELSE '' END ||' > Não </label><label><input type=\'radio\' name=\'resposta['||r.rstid||']\' value=\'S\' '||CASE WHEN ser.resposta = 'S' THEN 'checked' ELSE '' END ||' > Sim </label> $sr' AS acao,
                r.rstid,
                CASE WHEN r.rstitem = 'R' THEN 'Restrição' ELSE 'Inconformidade' END AS item,
                CASE WHEN r.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
                tr.tprdsc,
                TO_CHAR(r.rstdtinclusao, 'DD/MM/YYYY') AS rstdtinclusao,
                r.rstdsc,
                r.rstdscprovidencia,
                TO_CHAR(r.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
                CASE WHEN
                    (SELECT MAX(p.pflcod) FROM seguranca.perfil p  INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = usu.usucpf AND sisid = 147 WHERE p.pflstatus='A' GROUP BY pu.usucpf) = 910
                    THEN (SELECT DISTINCT e.entnome AS descricao FROM obras2.usuarioresponsabilidade ur INNER JOIN entidade.entidade e ON e.entid = ur.entid WHERE ur.usucpf = usu.usucpf AND ur.pflcod = 910 AND ur.rpustatus='A')
                    ELSE usu.usunome
                END AS criadopor,
                CASE WHEN r.rstsituacao = TRUE THEN TO_CHAR(r.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
                esd.esddsc,
                sup.usunome as ususuperacao,
                CASE WHEN esd.esdid NOT IN (1142, 1503) THEN 'NÃO' WHEN esd.esdid IN (1142, 1503) AND r.rstflressalva = 'S' THEN 'SIM' ELSE 'NÂO' END,
                usuh.usunome as usuh,
                TO_CHAR(h.htddata, 'DD/MM/YYYY HH:MM')
            FROM obras2.restricao r
            LEFT JOIN obras2.tiporestricao       tr ON tr.tprid = r.tprid AND tr.tprstatus = 'A'
            LEFT JOIN obras2.faserestricao       fr ON fr.fsrid = r.fsrid AND fr.fsrstatus = 'A'
            LEFT JOIN seguranca.usuario         usu ON usu.usucpf = r.usucpf
            LEFT JOIN seguranca.usuario         sup ON sup.usucpf = r.usucpfsuperacao
            LEFT JOIN workflow.documento        doc ON doc.docid  = r.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid  = doc.esdid
            LEFT JOIN workflow.historicodocumento h ON doc.hstid = h.hstid
            LEFT JOIN seguranca.usuario         usuh ON usuh.usucpf = h.usucpf
            LEFT JOIN obras2.supervisaoempresarestricao ser ON (r.rstid = ser.rstid AND ser.sueid = $sueid AND ser.status = 'A')
            WHERE r.rststatus = 'A'
                AND r.obrid = {$obrid}
                AND r.tprid = 13
                AND doc.esdid NOT IN ($superada,$cancelada,$justificada)
            ORDER BY r.rstid;
DML;
        return $sql;
    }

    public function retornaQueryPosCadastramento($sueid)
    {
        $sql = <<<DML
            SELECT
                '<label><input type=\'radio\' disabled name=\'resposta['||r.rstid||']\' value=\'N\''||CASE WHEN ser.resposta = 'N' THEN 'checked' ELSE '' END ||' > Não </label><label><input type=\'radio\' disabled name=\'resposta['||r.rstid||']\' value=\'S\' '||CASE WHEN ser.resposta = 'S' THEN 'checked' ELSE '' END ||' > Sim </label> <label><input type=\'radio\' disabled name=\'resposta['||r.rstid||']\' value=\'SR\' '||CASE WHEN ser.resposta = 'SR' THEN 'checked' ELSE '' END ||'> Sim com Ressalva </label>' AS acao,
                r.rstid,
                CASE WHEN r.rstitem = 'R' THEN 'Restrição' ELSE 'Inconformidade' END AS item,
                CASE WHEN r.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
                tr.tprdsc,
                TO_CHAR(r.rstdtinclusao, 'DD/MM/YYYY') AS rstdtinclusao,
                r.rstdsc,
                r.rstdscprovidencia,
                TO_CHAR(r.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
                CASE WHEN
                    (SELECT MAX(p.pflcod) FROM seguranca.perfil p  INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = usu.usucpf AND sisid = 147 WHERE p.pflstatus='A' GROUP BY pu.usucpf) = 910
                    THEN (SELECT DISTINCT e.entnome AS descricao FROM obras2.usuarioresponsabilidade ur INNER JOIN entidade.entidade e ON e.entid = ur.entid WHERE ur.usucpf = usu.usucpf AND ur.pflcod = 910 AND ur.rpustatus='A')
                    ELSE usu.usunome
                END AS criadopor,
                CASE WHEN r.rstsituacao = TRUE THEN TO_CHAR(r.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
                esd.esddsc,
                sup.usunome as ususuperacao,
                CASE WHEN esd.esdid NOT IN (1142, 1503) THEN 'NÃO' WHEN esd.esdid IN (1142, 1503) AND r.rstflressalva = 'S' THEN 'SIM' ELSE 'NÂO' END,
                usuh.usunome as usuh,
                TO_CHAR(h.htddata, 'DD/MM/YYYY HH:MM')
            FROM obras2.supervisaoempresarestricao ser
            LEFT JOIN obras2.restricao            r ON ser.rstid = r.rstid AND r.rststatus = 'A'
            LEFT JOIN obras2.tiporestricao        tr ON tr.tprid = r.tprid AND tr.tprstatus = 'A'
            LEFT JOIN obras2.faserestricao        fr ON fr.fsrid = r.fsrid AND fr.fsrstatus = 'A'
            LEFT JOIN seguranca.usuario           usu ON usu.usucpf = r.usucpf
            LEFT JOIN seguranca.usuario           sup ON sup.usucpf = r.usucpfsuperacao
            LEFT JOIN workflow.documento          doc ON doc.docid  = r.docid
            LEFT JOIN workflow.estadodocumento    esd ON esd.esdid  = doc.esdid
            LEFT JOIN workflow.historicodocumento h ON doc.hstid = h.hstid
            LEFT JOIN seguranca.usuario           usuh ON usuh.usucpf = h.usucpf
            WHERE ser.sueid = $sueid
                AND ser.status = 'A'
            ORDER BY r.rstid;
DML;
        return $sql;
    }

}