<?php

    /**
     * Monta a combo de UGs filtrando por UO
     *
     * @param $filtros
     * @return VOID
     */
    function montarComboUG(stdClass $filtros) {
        global $simec;

        return $simec->select(
            'suocod',
            'Subunidade',
            $filtros->ungcod,
            Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                'exercicio' => $filtros->exercicio,
                'unicod' => $filtros->unicod)));
    }

    /**
     * Monta a combo de metas PPA
     * 
     * @global cls_banco $db
     * @param integer $oppid
     * @param integer $mppid
     * @param integer $suocod
     */
    function carregarMetasPPA($oppid, $mppid, $suocod = null) {
        global $db;

        $join = '';
        if($suocod){
            $join = "inner join (
                        select smp.mppid 
                        from spo.subunidademetappa smp
                            inner join public.vw_subunidadeorcamentaria suo on suo.suoid = smp.suoid and suo.prsano = '{$_SESSION['exercicio']}'
                        where suo.suocod = '$suocod'                    
                        union all
                        select mpp.mppid from public.metappa mpp
                                left join spo.subunidademetappa smp on smp.mppid = mpp.mppid
                        where mpp.prsano = '{$_SESSION['exercicio']}'       
                        and smp.mppid is null               
                    ) smp on smp.mppid = om.mppid";
        }

        $sql = "
            SELECT DISTINCT
                m.mppid AS codigo,
                m.mppcod || ' - ' || m.mppdsc AS descricao
            FROM public.metappa m
                    JOIN public.objetivometappa om ON m.mppid = om.mppid
            $join
            WHERE
                m.mppstatus = 'A'
                AND m.prsano = '{$_SESSION['exercicio']}'
                AND om.oppid = ". (int)$oppid. "
            ORDER BY
                descricao
        ";
        $db->monta_combo('mppid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'mppid', null, (isset($mppid)? $mppid: null), null, 'class="form-control chosen-select" style="width=100%;"');
    }

