<?php
if($prfid){
    $whereResp = '';
    if (in_array(PFL_RELATORIO_TCU, $perfis)) {
        $whereResp = <<<DML
        AND EXISTS (
            SELECT 1
            FROM acomporc.relgdados rld
            INNER JOIN acomporc.usuarioresponsabilidade rpu USING(rldid)
            WHERE rlg.rlgid = rld.rlgid
                AND rpu.usucpf = '%s'
                AND rpu.pflcod = '%s'
                AND rpu.rpustatus = 'A')
DML;
        $whereResp = sprintf($whereResp, $_SESSION['usucpf'], PFL_RELATORIO_TCU);
    }

    $where = array();
    if (isset($_GET['unicod']) && !empty($_GET['unicod'])) {
        $where[] = sprintf("rlg.unicod = '%s'", $_GET['unicod']);
    }
    if (isset($_GET['acacod']) && !empty($_GET['acacod'])) {
        $where[] = sprintf("rlg.acacod = '%s'", $_GET['acacod']);
    }
    if (isset($_GET['acatipo']) && !empty($_GET['acatipo'])) {
        $where[] = sprintf("rlg.acatipo = '%s'", $_GET['acatipo']);
    }
    if (!empty($where)) {
        $where = ' AND ' . implode(' AND ', $where);
    } else {
        $where = '';
    }

    $select = <<<DML
        SELECT
            rlg.rlgid,
            rlg.unicod || ' - ' || uni.unidsc AS unicod,
            rlg.acacod,
            rlg.acatipo,
            TO_CHAR(rlg.dtultimaatualizacao, 'DD/MM/YYYY') AS dtultimaatualizacao,
            COALESCE((
                SELECT usu.usunome
                FROM acomporc.relgdados rld
                LEFT JOIN acomporc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
                LEFT JOIN seguranca.usuario usu USING(usucpf)
                WHERE rld.rlgid = rlg.rlgid
                    AND rld.rldtipo IN('acao', 'acaorap')
                    AND rpustatus = 'A'
                ORDER BY rpu.rpudata_inc DESC
                LIMIT 1
            ),'NÃO ENCONTRADO') AS usunome,
            COALESCE(
                (SELECT esddsc
                FROM acomporc.relgdados rld
                INNER JOIN workflow.documento using(docid)
                INNER JOIN workflow.estadodocumento USING(esdid)
                WHERE rld.rlgid = rlg.rlgid
                    AND rld.rldtipo IN('acao', 'acaorap')
                LIMIT 1
                ), 'Em preenchimento') AS esddsc,
            COALESCE(
                (SELECT esdid
                FROM acomporc.relgdados rld
                INNER JOIN workflow.documento using(docid)
                WHERE rld.rlgid = rlg.rlgid
                    AND rld.rldtipo IN('acao', 'acaorap')
                LIMIT 1), %d) AS esdid
        FROM acomporc.relgestao rlg
        LEFT JOIN public.unidade uni USING(unicod)
        WHERE rlg.prfid = '%s'
        {$whereResp}
        {$where}
DML;
    $stmt = sprintf(
        $select,
        ESDID_TCU_EM_PREENCHIMENTO,
        $prfid
    );

    $cabecalho = array('Unidade Orçamentária', 'Ação', 'Tipo', 'Data criação', 'Responsável da ação', 'Estado da ação', '&nbsp;');
    $list = new Simec_Listagem();
    $list->setQuery($stmt)
        ->setCabecalho($cabecalho)
        ->addAcao('edit', 'editarRelatorio')
        ->addAcao(
            'download',
            array(
                'func' => 'exportarItem',
                'formatos' => array('pdf', 'xls')
            )
        )
        ->addAcao('delete', 'apagarRelatorio')
        ->addCallbackDeCampo('acatipo', 'formatarAcaTipo')
        ->addCallbackDeCampo('esdid', 'formatarStatusIcone')
        ->addCallbackDeCampo(array('unicod', 'usunome'), 'alinhaParaEsquerda')
        ->turnOnPesquisator()
        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}