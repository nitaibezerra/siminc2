<?php
# Codificacao do relatorio
header('Content-Type: text/html; charset=iso-8859-1');
# Verifica se e um relatorio p o browser ou excel
$isXls = ($_POST['xls'] ) ? true : false;

function monta_sql() {

    	extract($_POST);
    	
    	$where = Array( "1=1" );
    	$whereInnerPag = Array();
    	
    	if( $tipo_obra != '' ){
    		$where[] = " pre.tooid $tipo_obra 1 ";
    	}
    	
    	if( $preid ){
    		$where[] = " pre.preid = $preid ";
    	}
    	
    	$whereInnerPag = Array(' 1=1 ');
    	if( $pagdatapagamento_de != '' ){
    		$whereInnerPag[] = " pag.pagdatapagamento::date >= '".formata_data_sql($pagdatapagamento_de)."'::date ";
    	}
    	
    	if( $pagdatapagamento_ate != '' ){
    		$whereInnerPag[] = " pag.pagdatapagamento::date <= '".formata_data_sql($pagdatapagamento_ate)."'::date ";
    	}
    	
    	if( $empnumero != '' ){
	    	$innerEmpenhoPAR = "INNER JOIN par.empenhoobrapar		eob ON eob.preid = pre.preid
							 	INNER JOIN par.empenho 				emp ON emp.empid = eob.empid AND emp.empstatus = 'A' AND emp.empsituacao <> 'CANCELADO' AND emp.empnumero = '$empnumero'";
	    	$innerEmpenhoPAC = "INNER JOIN par.empenhoobra 			eob ON eob.preid = pre.preid
							 	INNER JOIN par.empenho 				emp ON emp.empid = eob.empid AND emp.empstatus = 'A' AND emp.empsituacao <> 'CANCELADO' AND emp.empnumero = '$empnumero'";
    	}
    	
    	if( $nprocesso != '' ){
    		$where[] = " pro.pronumeroprocesso = '".str_replace(Array(".","/","-"),"",$nprocesso)."' ";
    	}
    	
    	if( $situacao != '' ){
    		$whereInnerPag[] = " pag.pagsituacaopagamento = '{$situacao}' ";
    	}
    	
    	if ($usunome) {
    		$usunomeTmp = removeAcentos(str_replace("-", " ", $usunome));
    		$whereUsuario = " AND UPPER(public.removeacento(usu.usunome)) ILIKE '%{$usunomeTmp}%' ";
    	}
    	
    	if( $pagdatapagamento_de != '' || $pagdatapagamento_ate != '' || $usunome != '' || $situacao != '' ){
	    	$innerPagPAC = "INNER JOIN par.pagamentoobra	pob ON pob.preid = pre.preid
							INNER JOIN par.pagamento		pag ON pag.pagid = pob.pagid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento <> 'CANCELADO' AND ".implode(' AND ', $whereInnerPag )."
							INNER JOIN seguranca.usuario 	usu ON usu.usucpf = pag.usucpf";
	    	
	    	$innerPagPAR = "INNER JOIN par.pagamentoobrapar	pob ON pob.preid = pre.preid
							INNER JOIN par.pagamento		pag ON pag.pagid = pob.pagid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento <> 'CANCELADO' AND ".implode(' AND ', $whereInnerPag )."
							INNER JOIN seguranca.usuario 	usu ON usu.usucpf = pag.usucpf $whereUsuario";
    	}
    	
    	if( $mundescricao != '' ){
    		$where[] = " UPPER(public.removeacento(mun.mundescricao)) ILIKE '%{$mundescricao}%'";
    	}
    	
    	if( $uf[0] != '' ){
    		$where[] = " pre.estuf in ('".implode("','",$uf)."')";
    	}
    	
        $sql = "SELECT DISTINCT
					to_char(pro.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as \"Processo\",
					'PAR' as \"Programa\",
					'<input type=hidden />'||pre.preid as \"ID da Pre-Obra\",
					'<input type=hidden />'||obr.obrid as \"ID da Obra\",
					pto.ptodescricao as \"Tipo da Obra\",
					pre.predescricao as \"Descrição da Obra\",
					esd.esddsc as \"Situação da Obra\",
					obr.obrpercentultvistoria as \"Percentual de Execução da Obra\",
					CASE 
						WHEN pre.preesfera = 'M' THEN 'Municipal'
						WHEN pre.preesfera = 'E' THEN 'Estadual'
						ELSE 'Outros'
					END as \"Esfera\",
					mun.mundescricao as \"Municipio\",
					mun.estuf as \"UF\",
					pre.prevalorobra as \"Valor da Obra\",
					vem.saldo as \"Valor Empenhado\",
					(
					SELECT 
						SUM(po.popvalorpagamento) 
					FROM 
						par.pagamentoobrapar po
					INNER JOIN par.pagamento pg ON pg.pagid = po.pagid AND pg.pagstatus = 'A' AND pg.pagsituacaopagamento ilike '%EFETIVADO%'
					WHERE
						po.preid = pre.preid
					) as \"Valor Pagamento Efetivado\",
					CASE WHEN pre.prevalorobra > 0
						THEN ((SELECT 
								SUM(po2.popvalorpagamento) 
							FROM 
								par.pagamentoobrapar po2
							INNER JOIN par.pagamento pg2 ON pg2.pagid = po2.pagid AND pg2.pagstatus = 'A' AND pg2.pagsituacaopagamento ilike '%EFETIVADO%'
							WHERE
								po2.preid = pre.preid
							)/pre.prevalorobra)*100
						ELSE 0
					END as \"Percentual Pagamento Efetivado\",
					(SELECT 
						COALESCE(SUM(po3.popvalorpagamento),0.00) as pagvalorparcela
					FROM par.pagamento pg3
					INNER JOIN par.pagamentoobrapar po3 ON po3.pagid = pg3.pagid
					WHERE 
						pg3.pagstatus = 'A' 
						AND po3.preid = pre.preid
						AND trim(pg3.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
					) as \"Valor Pagamento Solicitado\",
					CASE WHEN pre.prevalorobra > 0
						THEN ((SELECT 
									COALESCE(SUM(po4.popvalorpagamento),0.00) as pagvalorparcela
								FROM par.pagamento pg4
								INNER JOIN par.pagamentoobrapar po4 ON po4.pagid = pg4.pagid
								WHERE 
									pg4.pagstatus = 'A' 
									AND po4.preid = pre.preid
									AND trim(pg4.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
								)/pre.prevalorobra)*100
						ELSE 0 
					END as \"Percentual Pagamento Solicitado\",
					(SELECT 
						to_char(MIN(pg5.pagdatapagamento),'DD/MM/YYYY') as dt
					FROM par.pagamento pg5
					INNER JOIN par.pagamentoobrapar po5 ON po5.pagid = pg5.pagid
					WHERE 
						pg5.pagstatus = 'A' 
						AND po5.preid = pre.preid
						AND trim(pg5.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
					) as \"Data Pagamento Solicitado\",
					(SELECT 
						to_char(MAX(pg6.pagdatapagamento),'DD/MM/YYYY') as dt
					FROM par.pagamento pg6
					INNER JOIN par.pagamentoobrapar po6 ON po6.pagid = pg6.pagid
					WHERE 
						pg6.pagstatus = 'A' 
						AND po6.preid = pre.preid
						AND trim(pg6.pagsituacaopagamento) IN ('EFETIVADO', '2 - EFETIVADO')
					) as \"Data Pagamento Validado\",
					array_to_string( 
						Array(SELECT
							ne
						FROM
							par.v_saldo_obra_por_empenho 
						WHERE preid = pre.preid), ', ') as \"Numero do Empenho\",
					'<input type=hidden />'||
					array_to_string( 
						Array(SELECT
							empfonterecurso
						FROM
							par.v_saldo_obra_por_empenho voe
						INNER JOIN par.empenho emp ON emp.empid = voe.empid
						WHERE preid = pre.preid), ', ') as \"Fonte do Recurso\",
					par.retornasaldoprocesso(pro.pronumeroprocesso) AS \"Saldo Conta\"
				FROM
					par.processoobraspar pro
				INNER JOIN par.processoobrasparcomposicao 	poc ON pro.proid = poc.proid AND poc.pocstatus = 'A' AND pro.prostatus = 'A'
				INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.prestatus = 'A' AND pre.preidpai IS NULL
				INNER JOIN obras.pretipoobra				pto ON pto.ptoid = pre.ptoid
				$innerEmpenhoPAR
				$innerPagPAR
				INNER JOIN obras2.obras						obr ON obr.preid = pre.preid AND obr.obrstatus = 'A' AND obr.obridpai IS NULL
				INNER JOIN workflow.documento 				doc ON doc.docid = obr.docid AND tpdid = 105
				INNER JOIN workflow.estadodocumento 		esd ON esd.esdid = doc.esdid
				INNER JOIN territorios.municipio 			mun ON mun.muncod = pre.muncod
				INNER JOIN par.v_saldo_empenho_por_obra 	vem ON vem.preid = pre.preid
				WHERE
					".implode(' AND ', $where)."
        		UNION ALL
        		SELECT DISTINCT
					to_char(pro.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as \"Processo\",
					'PAR' as \"Programa\",
					'<input type=hidden />'||pre.preid as \"ID da Pre-Obra\",
					'<input type=hidden />'||obr.obrid as \"ID da Obra\",
					pto.ptodescricao as \"Tipo da Obra\",
					pre.predescricao as \"Descrição da Obra\",
					esd.esddsc as \"Situação da Obra\",
					obr.obrpercentultvistoria as \"Percentual de Execução da Obra\",
					CASE 
						WHEN pre.preesfera = 'M' THEN 'Municipal'
						WHEN pre.preesfera = 'E' THEN 'Estadual'
						ELSE 'Outros'
					END as \"Esfera\",
					mun.mundescricao as \"Municipio\",
					mun.estuf as \"UF\",
					pre.prevalorobra as \"Valor da Obra\",
					vem.saldo as \"Valor Empenhado\",
					(
					SELECT 
						SUM(po.pobvalorpagamento) 
					FROM 
						par.pagamentoobra po
					INNER JOIN par.pagamento pg ON pg.pagid = po.pagid AND pg.pagstatus = 'A' AND pg.pagsituacaopagamento ilike '%EFETIVADO%'
					WHERE
						po.preid = pre.preid
					) as \"Valor Pagamento Efetivado\",
					CASE WHEN pre.prevalorobra > 0 
						THEN ((
								SELECT 
									SUM(po2.pobvalorpagamento) 
								FROM 
									par.pagamentoobra po2
								INNER JOIN par.pagamento pg2 ON pg2.pagid = po2.pagid AND pg2.pagstatus = 'A' AND pg2.pagsituacaopagamento ilike '%EFETIVADO%'
								WHERE
									po2.preid = pre.preid
								)/pre.prevalorobra)*100
						ELSE 0
					END as \"Percentual Pagamento Efetivado\",
					(SELECT 
						COALESCE(SUM(po3.pobvalorpagamento),0.00) as pagvalorparcela
					FROM par.pagamento pg3
					INNER JOIN par.pagamentoobra po3 ON po3.pagid = pg3.pagid
					WHERE 
						pg3.pagstatus = 'A' 
						AND po3.preid = pre.preid
						AND trim(pg3.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
					) as \"Valor Pagamento Solicitado\",
					CASE WHEN pre.prevalorobra > 0 
						THEN ((SELECT 
									COALESCE(SUM(po4.pobvalorpagamento),0.00) as pagvalorparcela
								FROM par.pagamento pg4
								INNER JOIN par.pagamentoobra po4 ON po4.pagid = pg4.pagid
								WHERE 
									pg4.pagstatus = 'A' 
									AND po4.preid = pre.preid
									AND trim(pg4.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
								)/pre.prevalorobra)*100
						ELSE 0
					END as \"Percentual Pagamento Solicitado\",
					(SELECT 
						to_char(MIN(pg5.pagdatapagamento),'DD/MM/YYYY') as dt
					FROM par.pagamento pg5
					INNER JOIN par.pagamentoobra po5 ON po5.pagid = pg5.pagid
					WHERE 
						pg5.pagstatus = 'A' 
						AND po5.preid = pre.preid
						AND trim(pg5.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
					) as \"Data Pagamento Solicitado\",
					(SELECT 
						to_char(MAX(pg6.pagdatapagamento),'DD/MM/YYYY') as dt
					FROM par.pagamento pg6
					INNER JOIN par.pagamentoobra po6 ON po6.pagid = pg6.pagid
					WHERE 
						pg6.pagstatus = 'A' 
						AND po6.preid = pre.preid
						AND trim(pg6.pagsituacaopagamento) IN ('EFETIVADO', '2 - EFETIVADO')
					) as \"Data Pagamento Validado\",
					array_to_string( 
						Array(SELECT
							ne
						FROM
							par.v_saldo_obra_por_empenho 
						WHERE preid = pre.preid), ', ') as \"Numero do Empenho\",
					'<input type=hidden />'||
					array_to_string( 
						Array(SELECT
							empfonterecurso
						FROM
							par.v_saldo_obra_por_empenho voe
						INNER JOIN par.empenho emp ON emp.empid = voe.empid
						WHERE preid = pre.preid), ', ') as \"Fonte do Recurso\",
					par.retornasaldoprocesso(pro.pronumeroprocesso) AS \"Saldo Conta\"
				FROM
					par.processoobra pro
				INNER JOIN par.processoobraspaccomposicao 	poc ON pro.proid = poc.proid AND poc.pocstatus = 'A' AND pro.prostatus = 'A'
				INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.prestatus = 'A' AND pre.preidpai IS NULL
				INNER JOIN obras.pretipoobra				pto ON pto.ptoid = pre.ptoid
				$innerEmpenhoPAC
				$innerPagPAC
				INNER JOIN obras2.obras						obr ON obr.preid = pre.preid AND obr.obrstatus = 'A' AND obr.obridpai IS NULL
				INNER JOIN workflow.documento 				doc ON doc.docid = obr.docid AND tpdid = 105
				INNER JOIN workflow.estadodocumento 		esd ON esd.esdid = doc.esdid
				INNER JOIN territorios.municipio 			mun ON mun.muncod = pre.muncod
				INNER JOIN par.v_saldo_empenho_por_obra 	vem ON vem.preid = pre.preid
				WHERE
					".implode(' AND ', $where)."";

	return $sql;
}

$sql = monta_sql();
$dados = $db->carregar($sql);
?>
<html>
    <head>
<?php
if (count($dados) && is_array($dados)) {
	if (!$isXls) {
    ?>
            <script type="text/javascript" src="../includes/funcoes.js"></script>
            <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
            <link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
            <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
    <?php
	}else{
        $file_name = "RelatorioAnaliticoPagamentoObrasPar.xls";
        header("Pragma: public");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Content-Type: application/force-download");
        header("Content-Type: application/octet-stream");
        header("Content-Type: application/download");
        header("Content-Disposition: attachment;filename=" . $file_name);
        header("Content-Transfer-Encoding: binary ");
	}
?>
    </head>
    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php

    // Monta o cabeçalho com o Brasao
    $cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
    $cabecalhoBrasao .= "<tr> <td colspan=\"100\">".monta_cabecalho_relatorio('100')."</td> </tr> </table>";

    // Caso não seja excel monta cabeçalho e título						
    if (!$isXls) {
        echo $cabecalhoBrasao;
        monta_titulo('Relatório Analítico Pagamento Obras', '');
    }
    
    // Monta tabela que irá exibir os dados
    echo '<table class="table table-bordered table-hover table-striped table-condensed" align="center" style="width: 98%" >';
    // Cabeçalho para o arquivo excel
    if ($isXls) {
        echo ' <tr style="background-color: #dcdcdc;"> <td colspan="' . count($dados[0]) . '" > Relatório Pagamento Obras</td></tr>';
    }

    // Começa linha com os nomes das colunas
    echo '	<tr >';
    foreach ($dados[0] as $colKey => $colVal) {
        echo "		<td style=\"background-color: #e9e9e9;\" align=\'center\'  > <b> {$colKey} </b> </td>";
    }
    echo '</tr>';

    // Lista as informações do relatório
    foreach ($dados as $linhaKey => $linhaVal) {
        echo '<tr>';
        foreach ($linhaVal as $colKey => $colVal) {
            $cor = '';
            if ( is_numeric($colVal) ) {
                $cor = 'style="color:blue"';
                $colVal = number_format( $colVal, '2', ',', '.' );
            }
            if (!$colVal) {
                $colVal = '-';
            }
            echo " <td align=\'center\' width=\'50%\' $cor > <b> {$colVal} </b> </td>";
        }
    }
    // Finaliza o relatório
    echo '</table>';
} else {
    // Exibe mensagem de que não existem registros.
    die('<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">' .
            '<TR style="background:#FFF; color:red;">' .
            '<TD colspan="10" align="center">' .
            'Não foram encontrados registros.' .
            '</TD>' .
            '</TR>' .
            '</table>');
}

if ($isXls) {
    // Finaliza processo
    die('<script>window.close();</script>');
}
?>

    </body>