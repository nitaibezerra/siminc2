<?PHP

    set_time_limit(0);

    if ( $_REQUEST['filtrosession'] ){
            $filtroSession = $_REQUEST['filtrosession'];
    }

    if ($_POST['agrupador']){
            header( 'Content-Type: text/html; charset=iso-8859-1' );
    }
?>
    <html>
        <head>
            <script type="text/javascript" src="../includes/funcoes.js"></script>

            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        </head>

        <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?PHP

    include APPRAIZ. 'includes/classes/relatorio.class.inc';

    $sql   = monta_sql( $_POST['req'] );
    $dados = $db->carregar($sql);
    $agrup = monta_agp();
    $col   = monta_coluna();

    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setColuna($col);
    $r->setBrasao($true ? true : false);
    $r->setMonstrarTolizadorNivel(true);
    $r->setTotNivel(true);
    $r->setEspandir($_POST['expandir']);

    if( $_POST['req'] == 2 ){
            ob_clean();
            $nomeDoArquivoXls="Relatório_Consolidado_Gestao_SERES_".date('d-m-Y_H_i');
            echo $r->getRelatorioXls();
    }else{
            echo $r->getRelatorio();
    }

    function monta_sql( $rel_tipo ){

	extract($_POST);

        #DATA DE INCLUSÃO DA DEMANDA.
        $tardtinclusao_1 = formata_data_sql($tardtinclusao_1);
        $tardtinclusao_2 = formata_data_sql($tardtinclusao_2);
        
        if( $tardtinclusao_1 && $tardtinclusao_2 ){
            $where[] = "t.tardatarecebimento BETWEEN '{$tardtinclusao_1}' AND '{$tardtinclusao_2}'";
        }elseif( !$tardtinclusao_1 && $tardtinclusao_2 ){
            $where[] = "t.tardatarecebimento <= '{$tardtinclusao_2}'";
        }elseif( $tardtinclusao_1 && !$tardtinclusao_2 ){
            $where[] = "t.tardatarecebimento >= '{$tardtinclusao_1}'";
        }
        
        #DATA DE INICIO E FIM PARA O PRAZO DE ATENDIMENTO DA DEMANDA.
        $tardataprazoatendimento_1 = formata_data_sql($tardataprazoatendimento_1);
        $tardataprazoatendimento_2 = formata_data_sql($tardataprazoatendimento_2);
        
        if($tardataprazoatendimento_1 && $tardataprazoatendimento_1){
            $where[] = "t.tardataprazoatendimento BETWEEN '$tardataprazoatendimento_1' AND '$tardataprazoatendimento_2'";
        } else if($tardataprazoatendimento_1 && !$tardataprazoatendimento_2){
            $where[] = "t.tardataprazoatendimento >= '$tardataprazoatendimento_1'";
        } else if(!$tardataprazoatendimento_1 && $tardataprazoatendimento_2){
            $where[] = "t.tardataprazoatendimento <= '$tardataprazoatendimento_2'";
        }

	if( $tpsid[0] && ( $tpsid_campo_flag || $tpsid_campo_flag== '1' )){
            $where[] = " sol_tipo.tpsid " . (( $tpsid_campo_excludente == null || $tpsid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpsid ) . "') ";
	}

	if( $nvcid[0] && ( $nvcid_campo_flag || $nvcid_campo_flag == '1' )){
            $where[] = " t.nvcid " . (( $nvcid_campo_excludente == null || $nvcid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $nvcid ) . "') ";
	}

	if( $esdid[0] && ( $esdid_campo_flag || $esdid_campo_flag == '1' )){
            $where[] = " ed.esdid " . (( $esdid_campo_excludente == null || $esdid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ";
	}

	if( $unaid[0] && ( $unaid_campo_flag || $unaid_campo_flag == '1' )){
            $where[] = " t.unaidsetororigem " . (( $unaid_campo_excludente == null || $unaid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unaid ) . "') ";
	}

	if( $temid[0] && ( $temid_campo_flag || $temid_campo_flag == '1' )){
            $where[] = " t.temid " . (( $temid_campo_excludente == null || $temid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $temid ) . "') ";
	}

	if( $sitid[0] && ( $sitid_campo_flag || $sitid_campo_flag == '1' )){
            $where[] = " sit.sitid " . (( $sitid_campo_excludente == null || $sitid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sitid ) . "') ";
	}

	if( $tpeid[0] && ( $tpeid_campo_flag || $tpeid_campo_flag == '1' )){
            $where[] = " t.tpeid " . (( $tpeid_campo_excludente == null || $tpeid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpeid ) . "') ";
	}

	if( $usucpf[0] && ( $usucpf_campo_flag || $usucpf_campo_flag == '1' )){
            $where[] = " t.usucpfresponsavel " . (( $usucpf_campo_excludente == null || $usucpf_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $usucpf ) . "') ";
	}

        if( $rel_tipo == 1 ){
            $link = "<a style=\"cursor:pointer;\" onclick=\"parent.opener.window.location.href=\'/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadTarefa&acao=A&tarid='||t.tarid||'\';parent.opener.window.focus();\">";
            $a = "</a>";
        }else{
            $link = "";
            $a = "";
        }

	$sql = "
            SELECT  t.tarid,
                    '{$link}' ||
                        CASE WHEN char_length(trim(tarnumsidoc)) = 12 THEN  substr(tarnumsidoc,0,7) || '.' || substr(tarnumsidoc,7,4) || '-' || substr(tarnumsidoc,11,2)
                             WHEN char_length(trim(tarnumsidoc)) = 17 THEN substring(tarnumsidoc,0,6) || '.' || substr(tarnumsidoc,6,6) || '/' || substr(tarnumsidoc,12,4) || '-' || substr(tarnumsidoc,16,2)
                             ELSE ''
                        END
                     || '{$a}' AS tarnumsidoc,

                    t.tartitulo,

                    te.tpedsc,

                    sol.solicitante,

                    n.nvcdsc,

                    CASE WHEN t.temid IS NULL
                        THEN 'Assunto não definido'
                        ELSE tm.temdescricao
                    END AS temdescricao,

                    uo.unasigla ||''||uo.unadescricao as uni_orig,

                    CASE WHEN u.usunome IS NULL
                        THEN 'Usuário Indefinido'
                        ELSE ''||u.usunome||''
                    END AS nome_responsavel,

                    to_char(t.tardatarecebimento, 'DD/MM/YYYY') AS tardatarecebimento,

                    CASE WHEN (ed.esdid = 849 OR ed.esdid = 844) THEN '' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || ''
                         WHEN t.tardataprazoatendimento <= CURRENT_DATE THEN '' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || ''
                         WHEN ((t.tardataprazoatendimento + '1 month'::INTERVAL) >= (CURRENT_DATE + '1 month'::INTERVAL) AND  (t.tardataprazoatendimento + '1 month'::INTERVAL) <= ((CURRENT_DATE + '1 month'::INTERVAL) + '4 day'::INTERVAL)) THEN '' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || ''
                         ELSE '' || to_char(t.tardataprazoatendimento, 'DD/MM/YYYY') || ''
                    END AS tardataprazoatendimento,

                    sit.tarporcentoexec || '%' AS tarporcentoexec,

                    ed.esddsc,

                    sit.sitdsc

            FROM gestaodocumentos.tarefa t

            LEFT JOIN gestaodocumentos.tipoexpediente te ON te.tpeid = t.tpeid

            LEFT JOIN gestaodocumentos.tema tm ON tm.temid = t.temid

            LEFT JOIN seguranca.usuario u on t.usucpfresponsavel = u.usucpf

            LEFT JOIN gestaodocumentos.unidade uo ON uo.unaid = t.unaidsetororigem

            LEFT JOIN gestaodocumentos.nivelcomplexidade AS n ON n.nvcid = t.nvcid

            LEFT JOIN workflow.documento d ON d.docid = t.docid
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
            LEFT JOIN workflow.tipodocumento td ON td.tpdid = ed.tpdid AND td.sisid = 139

            LEFT JOIN gestaodocumentos.reiteracoes AS r ON r.taridprincipal = t.tarid

            LEFT JOIN gestaodocumentos.instituicaosolicitante AS sol_tipo ON sol_tipo.tarid = t.tarid

            LEFT JOIN(
                SELECT min(htddata) as data_inicio, d.docid
                FROM workflow.documento d
                INNER JOIN workflow.historicodocumento h on h.docid = d.docid
                INNER JOIN workflow.acaoestadodoc a on a.aedid = h.aedid

                WHERE esdiddestino = 845
                GROUP BY d.docid
            ) AS di ON di.docid = t.docid

            LEFT JOIN(
                SELECT d.tarid, sd.sitid, sd.sitdsc, d.tarporcentoexec
                FROM gestaodocumentos.tarefa d
                INNER JOIN gestaodocumentos.situacaotarefa sd ON d.sitid = sd.sitid
            ) AS sit ON sit.tarid = t.tarid

            LEFT JOIN(
                SELECT  tarid,
                        TRIM(array_to_string(
                            array(
                                SELECT  CASE
                                            WHEN ts.tpsid = 1 THEN '- ' || ts.tpsdsc --|| ':' || SUM(s.qtdsol)
                                            WHEN ts.tpsid = 2 THEN '- ' || ts.tpsdsc || ':' || og.ogsdsc
                                            WHEN ts.tpsid = 3 THEN '- ' || ts.tpsdsc || ':' || ie.iesdsc
                                            WHEN ts.tpsid = 4 THEN '- ' || ts.tpsdsc || ':' || og.ogsdsc
                                            WHEN ts.tpsid = 5 THEN '- ' || ts.tpsdsc || ':' || u.uamdsc
                                            WHEN ts.tpsid = 6 THEN '- ' || ts.tpsdsc || ':' || og.ogsdsc
                                            WHEN ts.tpsid = 7 THEN '- ' || ts.tpsdsc || ':' || u.uamdsc
                                            WHEN ts.tpsid = 8 THEN '- ' || ts.tpsdsc || ':' || og.ogsdsc
                                            WHEN ts.tpsid = 9 THEN '- ' || ts.tpsdsc || ':' || og.ogsdsc
                                        ELSE ''
                                    END AS descricao
                                FROM gestaodocumentos.instituicaosolicitante AS iss

                                JOIN gestaodocumentos.tiposolicitante AS ts ON ts.tpsid = iss.tpsid

                                LEFT JOIN (SELECT iesid, iesdsc FROM gestaodocumentos.instituicaoensino GROUP BY iesid, iesdsc) AS ie ON ie.iesid = iss.iesidinstituicaoensino --INSTITUIÇÃO DE ENSINO
                                LEFT JOIN (SELECT uamid, uamdsc FROM public.unidadeareamec GROUP BY uamid, uamdsc) AS u  ON u.uamid = iss.uamid --ÁREA MEC
                                LEFT JOIN (SELECT ogsid, ogsdsc FROM gestaodocumentos.orgaosolicitante GROUP BY ogsid, ogsdsc) AS og ON og.ogsid = iss.ogsid --ÓRGÃO
                                LEFT JOIN (SELECT COUNT(solid) AS qtdsol, solid FROM gestaodocumentos.solicitantepessoa GROUP BY solid) AS s ON s.solid = iss.solidpessoafisica --SOLICITANTE

                                WHERE iss.tarid = tt.tarid
                                GROUP BY ts.tpsid, ts.tpsdsc, og.ogsdsc, ie.iesdsc, u.uamdsc
                            ), ''
                        ) ) AS solicitante
                FROM gestaodocumentos.tarefa tt
            ) as sol On sol.tarid = t.tarid

            ".(is_array($where) ? ' WHERE '.implode(' AND ', $where) : '')."

            ORDER BY t._tarordem
        ";
//        /ver($sql, d);
	return $sql;
    }

    function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
            "agrupador" => array(),

            "agrupadoColuna" => array(
                    "tarnumsidoc",
                    "tartitulo",
                    "solicitante",
                    "nvcdsc",
                    "esddsc",
                    "tarporcentoexec",
                    "uni_orig",
                    "tpedsc",
                    "temdescricao",
                    "nome_responsavel",
                    "tardatarecebimento",
                    "tardataprazoatendimento",
                    "sitdsc"
            )
        );

	foreach ($agrupador as $val){
            switch ($val) {
                case 'solicitante':
                    array_push($agp['agrupador'], array( "campo" => "solicitante", "label" => "Solicitante") );
                    continue;
                case 'nvcdsc':
                    array_push($agp['agrupador'], array( "campo" => "nvcdsc", "label" => "Nível") );
                    continue;
                case 'esddsc':
                    array_push($agp['agrupador'], array( "campo" => "esddsc", "label" => "Status WorkFlow") );
                    continue;
                case 'tarporcentoexec':
                    array_push($agp['agrupador'], array( "campo" => "tarporcentoexec", "label" => "% Executado") );
                    continue;
                case 'uni_orig':
                    array_push($agp['agrupador'], array( "campo" => "uni_orig", "label" => "Setor de Origem") );
                    continue;
                case 'tpedsc':
                    array_push($agp['agrupador'], array( "campo" => "tpedsc", "label" => "Expediente") );
                    continue;
                case 'temdescricao':
                    array_push($agp['agrupador'], array( "campo" => "temdescricao", "label" => "Assunto") );
                    continue;
                case 'nome_responsavel':
                    array_push($agp['agrupador'], array( "campo" => "nome_responsavel", "label" => "Responsável") );
                    continue;
                case 'tardatarecebimento':
                    array_push($agp['agrupador'], array( "campo" => "tardatarecebimento", "label" => "Data do Recebimento") );
                    continue;
                case 'tardataprazoatendimento':
                    array_push($agp['agrupador'], array( "campo" => "tardataprazoatendimento", "label" => "Data do Atendimento") );
                    continue;
                case 'sitdsc':
                    array_push($agp['agrupador'], array( "campo" => "sitdsc", "label" => "Situação da Demanda") );
                    continue;
            }
        }
	array_push($agp['agrupador'], array( "campo" => "tarnumsidoc", "label" => "Nº do SIDOC") );

	return $agp;
    }

    function monta_coluna(){

	$coluna = array(
            array(
                "campo" => "tartitulo",
                "label" => "Identificação da Demanda (Título):"
            )
            /*,
            array(
                "campo" => "tbraluno",
                "label" => "Alunos",
                "type"  => "numeric"
            ),
            array(
                "campo" => "custeio2013",
                "label" => "Custeio 2013",
                "type"  => "numeric"
            ),*/
        );

	return $coluna;
    }

?>