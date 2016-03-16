<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Snapshot
 *
 * @author LindalbertoFilho
 */
class Spo_Service_Snapshot extends Spo_Service_Abstract {
    /*
     * @see Spo_Model_SnapshotSubacao
     */

    protected $_dbSnapshotSubacao;
    /*
     * @see Spo_Model_SnapshotLocalizador
     */
    protected $_dbSnapshotLocalizador;

    public function __construct() {
        
    }

    public function executaCargaAcao($prfid) {
        if (!$prfid) {
            $this->getFlashMessage()->addMensagem('Nenhum período selecionado.', Simec_Helper_FlashMessage::AVISO);
            return;
        }
        $this->_dbSnapshotLocalizador = new Spo_Model_SnapshotLocalizador();
        $this->_dbSnapshotLocalizador->excluirVarios("prfid = $prfid");
        $this->_dbSnapshotLocalizador->__set('prfid', $prfid);
        #$this->_dbSnapshotLocalizador->deletarCargaUsuResponsabilidade();
        if ($this->_dbSnapshotLocalizador->inserirCarga()) {
            #$this->_dbSnapshotLocalizador->inserirCargaUsuResponsabilidade();
        }
    }

    public function executaCargaSubacao($dados) {
        global $db;
        if (!$dados['formacompanhamentosubacao']['prfid']) {
            $resultado['mensagem']['texto'] = "Nenhum período selecionado.";
            $resultado['mensagem']['tipo'] = Simec_Helper_FlashMessage::ERRO;
            return $resultado;
        }

        /*
         * Executando SEM os COORDENADORES
         */
        $sql = "
            DELETE FROM acomporc.snapshotsubacao WHERE prfid = {$dados['formacompanhamentosubacao']['prfid']} ;
            INSERT INTO acomporc.snapshotsubacao (prfid, sbacod, ptres, metafisica, prddescricao, unmdescricao, plocod, plodsc, dataultimaatualizacao, vlrdotacao, vlrempenhado, vlrliquidado, vlrpago)
            SELECT
                pr.prfid,
                ps.sbacod,
                p.ptres,
                COALESCE(po.metafisica,0)        AS metafisica,
                a.unmdsc AS unmdescricao,
                coalesce(a.prodsc,prds.prodsc) AS prddescricao,
                po.plocodigo AS plocod,
                po.plotitulo AS plodsc,
                ps.sbadata AS dataultimaatualizacao,
                coalesce(psd.sadvalor,0.00) AS vlrdotacao,
                coalesce(ptr.total,0.00) AS vlrempenhado,
                coalesce(sum(vlrliquidado),0.00) AS vlrliquidado,
                coalesce(sum(vlrpago),0.00) AS vlrpago
            FROM acomporc.periodoreferencia pr
            INNER JOIN monitora.pi_subacao ps ON (pr.prsano = ps.sbaano AND ps.sbastatus = 'A')
            INNER JOIN monitora.pi_subacaodotacao psd ON (ps.sbaid = psd.sbaid)
            INNER JOIN monitora.ptres p ON (psd.ptrid = p.ptrid AND p.ptrstatus = 'A')
            INNER JOIN monitora.acao a ON (p.acaid = a.acaid)
            LEFT JOIN monitora.produtosof prds ON (a.procod = prds.procodsof )
            LEFT JOIN monitora.planoorcamentario po ON (p.plocod = po.plocodigo AND p.acaid = po.acaid AND pr.prsano = po.exercicio)
            LEFT JOIN siafi.ptrempenho ptr ON (p.ptres = ptr.ptres AND pr.prsano = ptr.exercicio)
            LEFT JOIN spo.siopexecucao se ON (substr(se.plicod,2,4) = ps.sbacod AND se.exercicio = pr.prsano AND se.ptres = p.ptres AND se.anoreferencia = se.exercicio)
            WHERE pr.prfid = {$dados['formacompanhamentosubacao']['prfid']}
            AND  ps.sbaano = '{$_SESSION['exercicio']}'
            AND ps.sbaid IN (
                            SELECT
                                sbaid
                            FROM
                                monitora.pi_subacaounidade
                            WHERE
                                unicod IN ('26101',
                                           '26298',
                                           '26290',
                                           '26291',
                                           '26443')            
                            )    
            GROUP BY pr.prfid, ps.sbacod, p.ptres, metafisica, a.unmdsc, prddescricao, po.plocodigo, po.plotitulo, ps.sbadata, vlrdotacao, ptr.total
            ORDER BY ps.sbacod;";
        #ver($sql, d);
        if ($db->executar($sql)) {
            $db->commit();
            $resultado['mensagem']['texto'] = "Carga de Subações executada com Sucesso.";
            $resultado['mensagem']['tipo'] = Simec_Helper_FlashMessage::SUCESSO;
            if (isset($dados['formAcompanhamentoSubacao']['comCoordenadores'])) {
            #ver($dados,d);
                /*
                 * Executando APENAS os COORDENADORES
                 */
                $sql = "
                    DELETE FROM acomporc.usuarioresponsabilidade 
                        WHERE pflcod = 1351
                        AND rpustatus = 'A'
                        AND prfid = {$dados['formacompanhamentosubacao']['prfid']};
                            
                    INSERT INTO acomporc.usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc , prfid , sbacod)
                    SELECT
                        pflcod,
                        usucpf,
                        'A',
                        rpudata_inc,
                        {$dados['formacompanhamentosubacao']['prfid']},
                        sbacod
                    FROM acomporc.usuarioresponsabilidade
                    WHERE pflcod = 1351
                        AND rpustatus = 'A'
                       AND prfid = (select prfid from acomporc.periodoreferencia where prftipo ='S' and prfid <> {$dados['formacompanhamentosubacao']['prfid']} order by prfid desc limit 1);";
                if ($db->executar($sql)) {
                    $resultado['mensagem']['texto'] = "Carga de Subações e COORDENADORES executada com Sucesso.";
                    $db->commit();
                } else {
                    $resultado['mensagem']['texto'] = "Erro ao executar a carga de Coordenadores.";
                    $resultado['mensagem']['tipo'] = Simec_Helper_FlashMessage::ERRO;
                }
            }
        } else {
            $resultado['mensagem']['texto'] = "Erro ao executar a carga de Subações.";
            $resultado['mensagem']['tipo'] = Simec_Helper_FlashMessage::ERRO;
        }
        return $resultado;


    }

}
