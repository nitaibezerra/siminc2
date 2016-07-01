<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 11/08/2015
 * Time: 15:17
 */

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php
    monta_titulo( 'Demonstrativo de Documentos', 'Período: '.$_POST['dtini']. ' á ' .$_POST['dtfim'] );

    include APPRAIZ. 'includes/classes/relatorio.class.inc';

    $sql   = monta_sql( $_POST );
    $dados = $db->carregar($sql);
    $agrup = monta_agp();
    $col   = monta_coluna();

    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setColuna($col);
    $r->setBrasao(true);
    $r->setMonstrarTolizadorNivel(true);
    $r->setTotNivel(true);
    $r->setEspandir(true);

    echo $r->getRelatorio();

    function monta_sql($dados){
        $dtini = $dados['dtini'];
        $dtfim = $dados['dtfim'];
        $where = '';
        switch($dados['opcRel']){
            case '1':
                $where = 'and dmdprazoemdata >= now() and ed.esdid NOT IN (' . ESDID_ARQUIVADO . ')';
                break;
            case '2':
                $where = 'and dmdprazoemdata < now() and ed.esdid NOT IN (' . ESDID_ARQUIVADO . ')';
                break;
            case '3':
                $where = 'and dmdreiteracao = \'t\'';
                break;
        }
        $sql = "select
                    tab.prcsigla,
                    tab.mp,
                    tab.dpf,
                    tab.defp,
                    tab.cgu,
                    tab.tcu,
                    tab.mp + tab.dpf + tab.defp + tab.cgu + tab.tcu as total
                from (
                    select
                        prc.prcid,
                        prc.prcsigla,
                        (
                            select
                                count(*)
                            from demandasse.demanda dmd
                            left join workflow.documento d ON ( d.docid = dmd.docid)
                            left join workflow.estadodocumento ed on ed.esdid = d.esdid
                            where prcid_orig in (
                            select prcid from demandasse.procedencia where prcdsc like '%Ministério Público%'
                            union
                            select prcid from demandasse.procedencia where prcdsc like '%Procuradoria%'
                            ) and prcid_dest = prc.prcid
                            and dmd.tpdid in (1,2)
                            and date(dmddtinclusao) between date('{$dtini}') and date('{$dtfim}')
                            {$where}
                        ) as mp,
                        (
                            select
                                count(*)
                            from demandasse.demanda dmd
                            left join workflow.documento d ON ( d.docid = dmd.docid)
                            left join workflow.estadodocumento ed on ed.esdid = d.esdid
                            where prcid_orig in (select
                                prcid from demandasse.procedencia
                            where prcdsc
                            like '%Polícia Federal%'
                            ) and prcid_dest = prc.prcid
                            and dmd.tpdid in (1,2)
                            and date(dmddtinclusao) between date('{$dtini}') and date('{$dtfim}')
                            {$where}
                        ) as dpf,
                        (
                            select
                                count(*)
                            from demandasse.demanda dmd
                            left join workflow.documento d ON ( d.docid = dmd.docid)
                            left join workflow.estadodocumento ed on ed.esdid = d.esdid
                            where prcid_orig in (select
                                prcid from demandasse.procedencia
                            where prcdsc
                            like '%Defensoria Pública%'
                            ) and prcid_dest = prc.prcid
                            and dmd.tpdid in (1,2)
                            and date(dmddtinclusao) between date('{$dtini}') and date('{$dtfim}')
                            {$where}
                        ) as defp,
                        (
                            select
                                count(*)
                            from demandasse.demanda dmd
                            left join workflow.documento d ON ( d.docid = dmd.docid)
                            left join workflow.estadodocumento ed on ed.esdid = d.esdid
                            where prcid_orig in (select
                                prcid from demandasse.procedencia
                            where prcdsc
                            like '%Controladoria Geral da União%'
                            ) and prcid_dest = prc.prcid
                            and dmd.tpdid in (1,2)
                            and date(dmddtinclusao) between date('{$dtini}') and date('{$dtfim}')
                            {$where}
                        ) as cgu,
                        (
                            select
                                count(*)
                            from demandasse.demanda dmd
                            left join workflow.documento d ON ( d.docid = dmd.docid)
                            left join workflow.estadodocumento ed on ed.esdid = d.esdid
                            where prcid_orig in (select
                                prcid from demandasse.procedencia
                            where prcdsc
                            like '%Tribunal de Contas da União%'
                            ) and prcid_dest = prc.prcid
                            and dmd.tpdid in (1,2)
                            and date(dmddtinclusao) between date('{$dtini}') and date('{$dtfim}')
                            {$where}
                        ) as tcu
                    from demandasse.procedencia prc
                    where prc.prcid in (18, 27, 63, 19, 6, 21, 39, 41, 168, 35, 13, 43, 110, 12)
                ) as tab";
        return $sql;
    }

    function monta_agp(){
        $agp = array(
            "agrupador" => array(
                array(
                    "campo" => "prcsigla",
                    "label" => "Sigla"
                )
            ),
            "agrupadoColuna" => array(
                "prcsigla",
                "mp",
                "dpf",
                "defp",
                "cgu",
                "tcu",
                "total"
            )
        );
        return $agp;
    }

    function monta_coluna(){
        $coluna = array(
            array(
                "campo" => "mp",
                "label" => "Ministério Público e Procuradorias",
                "type"  => "numeric"
            ),
            array(
                "campo" => "dpf",
                "label" => "Departamento de Polícia Federal",
                "type"  => "numeric"
            ),
            array(
                "campo" => "defp",
                "label" => "Defensoria Pública",
                "type"  => "numeric"
            ),
            array(
                "campo" => "cgu",
                "label" => "CGU",
                "type"  => "numeric"
            ),
            array(
                "campo" => "tcu",
                "label" => "TCU",
                "type"  => "numeric"
            ),
            array(
                "campo" => "total",
                "label" => "Total",
                "type"  => "numeric"
            )
        );
        return $coluna;
    }

?>