<?php
ini_set("memory_limit", "512M");

if ( $_REQUEST['prtid'] ){
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
	unset( $_REQUEST['salvar'] );
}


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();
// monta o sql, agrupador e coluna do relatório
$sql       = sql_relatorio();
$agrupador = agp_relatorio();
$coluna    = coluna_relatorio();

$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTotNivel(true);
$rel->setBrasao(true);
$rel->setEspandir(true);

if ($_POST['tipo'] == 'exibirXLS'){
    $nomeDoArquivoXls = "Relatório_Plano_Ação_".date("YmdHis");
    echo $rel->getRelatorioXls();
    die;
}
?>
<html>
<head>
	<title>SIG-IPHAN : Sistema Integrado de Gestão do IPHAN</title>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
</head>
<body>
	<!--  Monta o Relatório -->
	<?php echo $rel->getRelatorio(); ?>
</body>
</html>
<?php
function agp_relatorio(){

	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("condatainicio",
										  "condatatermino",
	                                      "usunome",
	                                      "convalorestimado"),
				);


	foreach ( $agrupador as $val ){
		switch( $val ){
			case 'conobjeto':
                array_push($agp['agrupador'], array("campo"    => "conobjeto",
                                                    "label"    => "Objeto"));
            break;
			case "atidescricao":
				array_push($agp['agrupador'], array(
													"campo" => "atidescricao",
											  		"label" => "Atividade")
									   				);
			break;
			case "stpdsc":
				array_push($agp['agrupador'], array(
													"campo" => "stpdsc",
											  		"label" => "Situação do processo de contratação")
									   				);
			break;
			case "itrdsc":
				array_push($agp['agrupador'], array(
													"campo" => "itrdsc",
											  		"label" => "Forma de Contratação")
									   				);
			break;
			case "situacao":
				array_push($agp['agrupador'], array(
													"campo" => "situacao",
											  		"label" => "Situação do acompanhamento")
									   				);
			break;

		}
	}

	return $agp;
}

function coluna_relatorio(){

	$coluna = array();

	foreach ( $_REQUEST['coluna'] as $valor ){

		switch( $valor ){
			case 'condatainicio':
				array_push( $coluna, array("campo" 	  => "condatainicio",
							               "label" 	  => "Data de Início"));
			break;
			case 'condatatermino':
				array_push( $coluna, array("campo" 	  => "condatatermino",
							               "label" 	  => "Data Prevista de Término"));
			break;
			case 'convalorestimado':
				array_push( $coluna, array("campo" 	  => "convalorestimado",
								   		   "label" 	  => "Valor Estimado"));
			break;
			case 'usunome':
				array_push( $coluna, array("campo" 	  => "usunome",
								   		   "label" 	  => "Responsável"));
			break;
		}
	}

	return $coluna;
}

function sql_relatorio(){

    $where = array();

    extract($_REQUEST);

    // Situação do processo de contratação
    if( $stpid[0]  && $stpid_campo_flag ){
        array_push($where, " con.stpid " . (!$stpid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $stpid ) . "') ");
    }

    // Forma de Contratação
    if( $itrid[0]  && $itrid_campo_flag ){
        array_push($where, " con.itrid " . (!$itrid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $itrid ) . "') ");
    }

    // Data Início
    if($condatainicio1){
        array_push($where, "con.condatainicio >= '{$condatainicio1}'");
    }

    // Data Início
    if($condatainicio2){
        array_push($where, "con.condatainicio <= '{$condatainicio2}'");
    }

    // Data Término
    if($condatatermino1){
        array_push($where, "con.condatatermino >= '{$condatatermino1}'");
    }

    // Data Término
    if($condatatermino2){
        array_push($where, "con.condatatermino <= '{$condatatermino2}'");
    }

    // Fase com atraso
    if($faseAtraso){
        $contratacao  = new Contratacao();
        $contratacoes = $contratacao->getContratacoesAtrasadas();
        if($faseAtraso == 'sim'){
            if(is_array($contratacoes)){
                $contratacoes = implode(',', $contratacoes);
                array_push($where, "con.conid IN ({$contratacoes})");
            }else{
                array_push($where, "FALSE");
            }
        }elseif($faseAtraso == 'nao'){
            if(is_array($contratacoes)){
                $contratacoes = implode(',', $contratacoes);
                array_push($where, "NOT con.conid IN ({$contratacoes})");
            }else{
                array_push($where, "TRUE");
            }
        }
    }

    $whereOr = array();
    // Situação
    if($situacaoConcluida){
        array_push($whereOr, "numtotalf.numtotalfases = numcc.numconcluidacancelada");
    }
    if($situacaoEstavel){
        array_push($whereOr, "numamenormenos1.numatrasomenormenos1 > 0");
    }
    if($situacaoAtencao){
        array_push($whereOr, "numamenor10.numatrasomenor10maiormenos1 > 0");
    }
    if($situacaoCritico){
        array_push($whereOr, "numamaior10.numatrasomaior10 > 0");
    }

    if(count($whereOr) > 0){
        array_push($where, "(" . implode(" OR ", $whereOr) . ")");
    }


    $sql = "SELECT
                   con.*,
                   TO_CHAR(con.condatainicio, 'DD/MM/YYYY') AS condatainicio,
                   TO_CHAR(con.condatatermino, 'DD/MM/YYYY') AS condatatermino,
                   usu.*,
                   itr.*,
                   stp.*,
                   ati.*,
                   CASE
                       WHEN numtotalf.numtotalfases = numcc.numconcluidacancelada          THEN 'Concluída'
                       WHEN numamaior10.numatrasomaior10 > 0                               THEN 'Crítico'
                       WHEN numamenor10.numatrasomenor10maiormenos1 > 0                    THEN 'Merece Atenção'
                       WHEN numamenormenos1.numatrasomenormenos1 > 0                       THEN 'Estável'
                       ELSE '<center>N/A</center>'
                   END AS situacao

               FROM
                    sisplan.contratacao con
               INNER JOIN
                    seguranca.usuario usu ON usu.usucpf = con.usucpf
               INNER JOIN
                    pde.atividade ati ON ati.atiid = con.atiid
               INNER JOIN
                    sisplan.instrumento itr ON itr.itrid = con.itrid
               INNER JOIN
                    sisplan.situacaoprocesso stp ON stp.stpid = con.stpid
               LEFT JOIN
                    (SELECT
                        aco2.acoid,
                        aco2.conid,
                        aco2.stpid  AS stpidacompanhamento,
                        stp2.stpdsc AS stpdscacompanhamento
                    FROM
                        sisplan.contratacao con2
                    INNER JOIN
                        sisplan.acompanhamentocontratacao aco2 ON aco2.conid = con2.conid AND acodatacadastro = (SELECT
                                                                                                                  acodatacadastro
                                                                                                              FROM
                                                                                                                  sisplan.acompanhamentocontratacao
                                                                                                              WHERE
                                                                                                                  conid = aco2.conid AND acostatus = 'A'
                                                                                                              ORDER BY
                                                                                                                  acodatacadastro DESC, acoid DESC
                                                                                                              LIMIT 1)
                    INNER JOIN
                        sisplan.situacaoprocesso stp2 ON stp2.stpid = con2.stpid
                    ) aco ON aco.conid = con.conid

                -- NÚMERO TOTAL DE FASES DA CONTRATAÇÃO
                LEFT JOIN
                (SELECT
                    COUNT(1) AS numtotalfases,
                    cof2.conid
                FROM
                    sisplan.contratacaofase cof2
                WHERE
                    cof2.cofstatus = 'A'
                GROUP BY
                    cof2.conid) numtotalf ON numtotalf.conid = con.conid

               -- NÚMERO DE FASES CONCLUÍDAS OU CANCELADAS
               LEFT JOIN
                (SELECT
                    COUNT(1) AS numconcluidacancelada,
                    cof3.acoid,
                    cof33.conid
                FROM
                    sisplan.acompcontratacaofase cof3
                INNER JOIN
                    sisplan.contratacaofase cof33 ON cof3.cofid = cof33.cofid
                WHERE
                    cof3.stfid IN (".SITUACAO_FASE_CONCLUIDA.", ".SITUACAO_FASE_CANCELADA.") -- CONCLUIDA, CANCELADA
                     AND cof33.cofstatus = 'A'
                GROUP BY
                    cof3.acoid, cof33.conid) numcc ON numcc.acoid = aco.acoid AND numcc.conid = con.conid


               -- NÚMERO DE FASES COM MAIS DE 10 DIAS DE ATRASO
               LEFT JOIN
                (SELECT
                    COUNT(1) AS numatrasomaior10,
                    cof4.acoid,
                    cof44.conid
                FROM
                    sisplan.contratacaofase cof44
                LEFT JOIN
                    sisplan.acompcontratacaofase cof4 ON cof4.cofid = cof44.cofid
                WHERE
                    COALESCE(cof4.stfid, -1) NOT IN (".SITUACAO_FASE_CONCLUIDA.", ".SITUACAO_FASE_CANCELADA.") -- CONCLUIDA, CANCELADA
                    AND (date_part('day', now() - cof44.cofdatafim)) >= 10
                    AND cof44.cofstatus = 'A'
                GROUP BY
                    cof44.conid, cof4.acoid) numamaior10 ON CASE WHEN NOT numamaior10.acoid IS NULL THEN numamaior10.acoid = aco.acoid ELSE TRUE END AND numamaior10.conid = con.conid

                -- NÚMERO DE FASES COM MENOS DE 10 DIAS DE ATRASO E ATÉ 1 DIA PARA ATRASAR
               LEFT JOIN
                (SELECT
                    COUNT(1) AS numatrasomenor10maiormenos1,
                    cof5.acoid,
                    cof55.conid
                FROM
                    sisplan.contratacaofase cof55
                LEFT JOIN
                    sisplan.acompcontratacaofase cof5 ON cof5.cofid = cof55.cofid
                WHERE
                    COALESCE(cof5.stfid, -1) NOT IN (".SITUACAO_FASE_CONCLUIDA.", ".SITUACAO_FASE_CANCELADA.") -- CONCLUIDA, CANCELADA
                    AND (date_part('day', now() - cof55.cofdatafim )) < 10
                    AND (date_part('day', now() - cof55.cofdatafim)) >= -1
                    AND cof55.cofstatus = 'A'
                GROUP BY
                    cof55.conid, cof5.acoid) numamenor10 ON CASE WHEN NOT numamenor10.acoid IS NULL THEN numamenor10.acoid = aco.acoid ELSE TRUE END AND numamenor10.conid = con.conid

               -- NÚMERO DE FASES COM ATÉ 2 DIAS ANTES DE ATRASAR
               LEFT JOIN
                (SELECT
                    COUNT(1) AS numatrasomenormenos1,
                    cof6.acoid,
                    cof66.conid
                FROM
                    sisplan.contratacaofase cof66
                LEFT JOIN
                    sisplan.acompcontratacaofase cof6 ON cof6.cofid = cof66.cofid
                WHERE
                    COALESCE(cof6.stfid, -1)  NOT IN (".SITUACAO_FASE_CONCLUIDA.", ".SITUACAO_FASE_CANCELADA.") -- CONCLUIDA, CANCELADA
                    AND (date_part('day', now() - cof66.cofdatafim)) < -1
                    AND cof66.cofstatus = 'A'
                GROUP BY
                    cof66.conid, cof6.acoid) numamenormenos1 ON CASE WHEN NOT numamenormenos1.acoid IS NULL THEN numamenormenos1.acoid = aco.acoid ELSE TRUE END AND numamenormenos1.conid = con.conid


                  WHERE
                     con.constatus = 'A' ".
                    (count($where) ? " AND ". implode(' AND ', $where) : "" );

    return $sql;

}
?>
