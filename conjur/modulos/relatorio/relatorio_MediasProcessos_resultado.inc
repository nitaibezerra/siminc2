<?PHP

    set_time_limit(0);

    if ( $_REQUEST['filtrosession'] ){
        $filtroSession = $_REQUEST['filtrosession'];
    }

    if ($_POST['agrupador']){
        header( 'Content-Type: text/html; charset=iso-8859-1' );
    }
?>
<!--    <html>
        <head>
            <script type="text/javascript" src="../includes/funcoes.js"></script> -->

            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        <!--</head>

        <br>

        <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">-->
<?PHP
    monta_titulo( 'CONJUR - Médias de Processos', 'Período: '.$_POST['htddatainicial']. ' á ' .$_POST['htddatfinal'] );

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
    $r->setEspandir($_POST['expandir']);

    echo $r->getRelatorio();
    
    /*
    if( $_POST['req'] == 2 ){
        ob_clean();
        $nomeDoArquivoXls="Relatório_CONJUR_medias_processo_".date('d-m-Y_H_i');
        echo $r->getRelatorioXls();
    }else{
        echo $r->getRelatorio();
    }
    */
    
    
    function monta_sql( $rel_tipo ){

	extract($_POST);

        $data_ini = formata_data_sql( $_POST['htddatainicial'] );
        $data_fim = formata_data_sql( $_POST['htddatfinal'] );

        if( $agrupamentoCoordenacao != '' ){
            $colunas = "aca.aeddscrealizada as coordenacao";

            $join = "
                LEFT JOIN conjur.historicocoordenacoes hco ON hco.hstid = his.hstid
                LEFT JOIN conjur.coordenacao co1 ON co1.coonid = hco.coonid
            ";

            $group = "GROUP BY coordenacao";

            if( $coonid[0] ){
                $where[] = " aca.aedid IN ('" . implode( "','", $coonid ) . "') ";
            }
        }

        if( $agrupamentoAdvogados != '' ){
            $colunas = "ent.entnome AS nome_advogado";

            $join = "
                LEFT JOIN conjur.historicoadvogados had ON had.hstid = his.hstid
                LEFT JOIN conjur.advogados adv ON adv.advid = had.advid
                LEFT JOIN entidade.entidade ent ON ent.entid = adv.entid

                LEFT JOIN conjur.coordenacao co2 ON co2.coonid = adv.coonid
            ";

            $group = "GROUP BY nome_advogado";

            $where[] = "aca.aedid in ( ".ACAO_ENCAMINHAR_PARA_ADVOGADO." )";

            if( $advid[0] ){
                $where[] = " had.advid IN ('" . implode( "','", $advid ) . "') ";
            }
        }

        if( count($where) > 0 ){
            $and = ' AND ' . implode(' AND ', $where);
        }
        
        $sql = "
            SELECT  {$colunas},
                
                    count(prc.prcid) AS total_processo,

                    ( ( count(prc.prcid) ) / ( extract(year from age('{$data_fim}', '{$data_ini}')) * 360 + extract(month from age('{$data_fim}', '{$data_ini}') ) * 30 + extract(day from age('{$data_fim}', '{$data_ini}') ) ) ) AS media_processo_dia,

                    SUM(
                        CASE WHEN (
                            extract( year from age( 
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) * 360 +
                            extract( month from age(
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) * 30 +
                            extract( day from age( 
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) ) >= 15 
                                    THEN 1
                                    ELSE 0
                        END 
                    ) AS proc_maior_15,

                    SUM(
                        CASE WHEN (
                            extract( year from age( 
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) * 360 +
                            extract( month from age(
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) * 30 +
                            extract( day from age( 
                                    ( COALESCE( ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1479) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), ( SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1469) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ), '{$data_fim}' ) )::DATE,
                                    ( (SELECT max(htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) )::DATE
                            ) ) )  < 15 
                                    THEN 1
                                    ELSE 0
                        END 
                    ) AS proc_menor_15

            FROM workflow.historicodocumento his

            JOIN workflow.documento doc ON doc.docid = his.docid
            JOIN workflow.acaoestadodoc aca ON aca.aedid = his.aedid
            JOIN workflow.estadodocumento eca ON eca.esdid = aca.esdidorigem
            
            JOIN conjur.estruturaprocesso est ON est.docid = his.docid
            JOIN conjur.processoconjur prc ON prc.prcid = est.prcid

            {$join}
            
            WHERE his.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' AND his.aedid in (1072) {$and}

            AND (SELECT max(htddata)::date FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1059) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' ) > (SELECT max(htddata)::date FROM workflow.historicodocumento h WHERE h.docid = doc.docid AND h.aedid in (1072) AND h.htddata BETWEEN '{$data_ini}' AND '{$data_fim}' )

            {$group}

            ORDER BY 1
        ";
	return $sql;
    }

    function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
            "agrupador" => array(),

            "agrupadoColuna" => array(
                    "nome_advogado",
                    "coordenacao",
                    "total_processo",
                    "media_processo_dia",
                    "proc_menor_15",
                    "proc_maior_15"
            )
        );

        /*
	foreach ($agrupador as $val){
            switch ($val) {
                case 'solicitante':
                    array_push($agp['agrupador'], array( "campo" => "solicitante", "label" => "Solicitante") );
                    continue;
                case 'nvcdsc':
                    array_push($agp['agrupador'], array( "campo" => "nvcdsc", "label" => "Nível") );
                    continue;
            }
        }
        */
        if( $_POST['agrupamentoCoordenacao'] != '' ){
            array_push($agp['agrupador'], array( "campo" => "coordenacao", "label" => "Cordenação") );
        }else{
            array_push($agp['agrupador'], array( "campo" => "nome_advogado", "label" => "Advogado") );
        }

	return $agp;
    }

    function monta_coluna(){

	$coluna = array(
            /*array(
                "campo" => "nome_advogado",
                "label" => "Advogado:",
                "type"  => "string"
            ),*/
            array(
                "campo" => "total_processo",
                "label" => "Total de Processos",
                "type"  => "numeric"
            ),
            array(
                "campo" => "media_processo_dia",
                "label" => "Média no Período",
                "type"  => "float"
            ),
            array(
                "campo" => "proc_menor_15",
                "label" => "Prazo menor que 15 dias",
                "type"  => "numeric"
            ),
            array(
                "campo" => "proc_maior_15",
                "label" => "Prazo igual ou maior que 15 dias",
                "type"  => "numeric"
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
<!--</body>

</html>-->