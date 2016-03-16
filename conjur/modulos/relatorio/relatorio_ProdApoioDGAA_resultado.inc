<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 30/07/2015
 * Time: 11:02
 */

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php
    monta_titulo( 'CONJUR - Produtividade do Protocolo Apoio ao DGAA', 'Período: '.$_POST['dataini']. ' á ' .$_POST['datafim'] );

    include APPRAIZ. 'includes/classes/relatorio.class.inc';


    $sql   = monta_sql( $_POST );
    $dados = is_array($_POST['usuid']) && $_POST['dataini'] != '' && $_POST['datafim'] != '' ? $db->carregar($sql) : '';
    $agrup = monta_agp();
    $col   = monta_coluna();

    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setColuna($col);
    $r->setBrasao(true);
    $r->setMonstrarTolizadorNivel(true);
    $r->setTotNivel(true);
    $r->setEspandir($_POST['expandir']);

    echo $r->getRelatorio();

    function monta_sql( $rel_tipo ){
        $dataini  = $rel_tipo['dataini'] != '' ? $rel_tipo['dataini'] : '';
        $datafim  = $rel_tipo['datafim'] != '' ? $rel_tipo['datafim'] : '';
        $usuarios = is_array($rel_tipo['usuid']) ? "'".implode("', '", $rel_tipo['usuid'])."'" : '';
        $pflcod   = PRF_APOIO_DGAA;

        $sql = "select
                    tab.usunome,
                    tab.cadastrados,
                    tab.arquivados,
                    tab.encerrados,
                    tab.inictramit,
                    tab.cadastrados + tab.arquivados + tab.encerrados + tab.inictramit as total
                from (select
                    usu.usunome,
                    (
                        select
                            count(*)
                        From conjur.estruturaprocesso esp
                        inner join conjur.processoconjur prc on prc.prcid = esp.prcid
                        where esp.usucpf = usu.usucpf
                        and date(prc.prcdtinclusao) between date('{$dataini}') and date('{$datafim}')
                    ) as cadastrados,
                    (
                        select
                            count(*)
                        from conjur.estruturaprocesso esp
                        inner join workflow.documento doc on esp.docid = doc.docid and doc.tpdid = 49
                        where doc.esdid = 377
                        and date(doc.docdatainclusao) between date('{$dataini}') and date('{$datafim}')
                        and esp.usucpf = usu.usucpf
                    ) as arquivados,
                    (
                        select
                            count(*)
                        from conjur.estruturaprocesso esp
                        inner join workflow.documento doc on esp.docid = doc.docid and doc.tpdid = 49
                        where doc.esdid = 375
                        and date(doc.docdatainclusao) between date('{$dataini}') and date('{$datafim}')
                        and esp.usucpf = usu.usucpf
                    ) as encerrados,
                    (
                        select
                            count(*)
                        from conjur.estruturaprocesso esp
                        inner join workflow.documento doc on esp.docid = doc.docid
                        where exists (
                            select
                                1
                            from workflow.historicodocumento hst
                            where aedid in (1050, 1051, 1252, 1485)
                            and hst.docid = esp.docid
                        )
                        and esp.usucpf = usu.usucpf
                        and date(doc.docdatainclusao) between date('{$dataini}') and date('{$datafim}')
                    ) as inictramit
                from seguranca.perfilusuario pfu
                inner join seguranca.usuario usu on usu.usucpf = pfu.usucpf
                --where pfu.pflcod = {$pflcod}
               where usu.usucpf in ({$usuarios})) as tab";

        return $sql;
    }

    function monta_agp(){
        $agp = array(
            "agrupador" => array(
                array(
                    "campo" => "usunome",
                    "label" => "Servidor"
                )
            ),

            "agrupadoColuna" => array(
                'usunome',
                'cadastrados',
                'arquivados',
                'encerrados',
                'inictramit',
                'total'
            )
        );
        return $agp;
    }

    function monta_coluna(){
        $coluna = array(
            array(
                "campo" => "cadastrados",
                "label" => "Cadastrados",
                "type"  => "numeric"
            ),
            array(
                "campo" => "arquivados",
                "label" => "Arquivados",
                "type"  => "numeric"
            ),
            array(
                "campo" => "encerrados",
                "label" => "Encerrados",
                "type"  => "numeric"
            ),
            array(
                "campo" => "inictramit",
                "label" => "Inic Tramit",
                "type"  => "numeric"
            ),
            array(
                "campo" => 'total',
                "label" => "Total",
                "type" => "numeric"
            )
        );
        return $coluna;
    }
?>
<script>
    function chamaproc(prcid){
        window.opener.location="conjur.php?modulo=principal/editarprocesso&acao=A&prcid="+prcid;
        window.opener.focus();
    }
</script>
