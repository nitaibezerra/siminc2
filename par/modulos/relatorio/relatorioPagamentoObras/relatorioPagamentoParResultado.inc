<?php
# Codificacao do relatorio
header('Content-Type: text/html; charset=iso-8859-1');
# Objeto da classe do relatorio 
$obRelatorio = new relatorioPagamentoParResultado();
# Verifica se e um relatorio p o browser ou excel
$isXls = ($_REQUEST['requisicao'] == 'relatorio_xls') ? true : false;

// Classe responsavel por montar o resultado do relatorio
class relatorioPagamentoParResultado {

    # Metodo construtor
    function __construct() {
        # Instancia a classe do BD
        $this->db = new cls_banco();
    }

    public function getArrayRel($post) {
    	
        $strWhere 		= '';
        $preid 			= $post['preid'];
        $arrMuncod 		= $post['muncod'];
        $empnumero 		= $post['empnumero'];
        $usunome 		= $post['usunome'];
        $pagDataDe 		= $post['pagdatapagamento_de'];
        $pagDataAte 	= $post['pagdatapagamento_ate'];
        $uf 			= $post['uf'];
        $nprocesso 		= $post['nprocesso'];
        $situacao 		= $post['situacao'];
        $tipoObra 		= $post['tipo_obra'];
        $tipo_obra_mi 	= $post['tipo_obra_mi'];

        if ($preid) {
            $strWhere .= " AND CAST(o.obrid AS char(50)) ilike '%{$preid}%' ";
        }

        if ((is_array($arrMuncod)) && (count($arrMuncod)) && ($arrMuncod[0])) {

            foreach ($arrMuncod as $k => $v) {
                $arrMuncod[$k] = "'{$v}'";
            }

            $strReturn = implode(",", $arrMuncod);
            $strWhere .= " AND m.muncod in ( $strReturn ) ";
        }

        if ($empnumero) {
            $strWhere .= "AND e.empnumero ilike '%{$empnumero}%' ";
        }

        if ($usunome) {
            $usunomeTmp = removeAcentos(str_replace("-", " ", $usunome));
            $strWhere .= " AND UPPER(public.removeacento(usu.usunome)) ILIKE '%{$usunomeTmp}%' ";
        }

        if ($pagDataDe) {
            $data = formata_data_sql($_POST['pagdatapagamento_de']);
            $strWhere .= " AND pag.pagdatapagamento >= '{$data}' ";
        }
        if ($pagDataAte) {
            $data = formata_data_sql($_POST['pagdatapagamento_ate']);
            $strWhere .= " AND pag.pagdatapagamento <= '{$data}' ";
        }

        if ((is_array($uf)) && (count($uf)) && ($uf[0])) {

            foreach ($uf as $k => $v) {
                $uf[$k] = "'{$v}' ";
            }

            $strReturnUF = implode(",", $uf);
            $strWhere .= "AND 
                            CASE WHEN p.estuf IS NOT NULL THEN 
                                p.estuf in ( {$strReturnUF} )
                            ELSE 
                                m.estuf in ( {$strReturnUF} )
                            END ";
        }

        if ($nprocesso) {
            $nprocesso = str_replace('.', '', $nprocesso);
            $nprocesso = str_replace('/', '', $nprocesso);
            $nprocesso = str_replace('-', '', $nprocesso);
            $strWhere .= " AND p.pronumeroprocesso ilike '%$nprocesso%' ";
        }

        if ($situacao) {
            $strWhere .= "AND pag.pagsituacaopagamento = '{$situacao}' ";
        }
        
        if ( $tipo_obra_mi == 'MI' ) {
            $strWhere .= " AND pre.ptoid IN (43, 42, 44, 45) ";
        }
        if ( $tipo_obra_mi == 'NMI' ) {
            $strWhere .= " AND pre.ptoid NOT IN (43, 42, 44, 45) ";
        }

        if ($tipoObra) {
            $strWhereTipoObra = " where \"Programa\" = '$tipoObra' ";
        }

        $sql = "SELECT * 
				FROM (
					SELECT  DISTINCT
						to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"\/\"0000\"-\"00') as  \"Número processo\",  -- numero do processo
						'PAR' as \"Programa\",
						'<input type=hidden />'||pre.preid as \"ID preobra\",
						'<input type=hidden />'||o.obrid as \"ID da obra\",-- id da obra;
						pto.ptodescricao as \"Tipo da Obra\", -- tipo da obra
						pre.predescricao as \"Descrição da Obra\", -- Descrição da Obra;
						str.strdsc as \"Situação da Obra\",
						CASE WHEN (p.muncod is null or p.muncod = '') 
						    THEN 'Estadual' 
						    ELSE 'Municipal' 
						END as \"Processo Estadual ou Municipal\",
						m.mundescricao as \"Município\", -- município; 
						pre.estuf AS \"UF\", -- uf; 
						coalesce(TRUNC (pre.prevalorobra, 2),0) AS \"Valor da Obra\", -- valor da obra; 
						e.empnumero as \"Número empenho\", 
						'( '||vps.vpsvinculacao||' )' as \"N° da Vinculação\",
						-- SUM(eo.eobvalorempenho) as \"Valor empenhado na NE\",
						( SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE preid = pre.preid GROUP BY preid  ) AS \"Valor total empenhado da obra\",
        		
						-- Valor total Solicitado da obra
						( SELECT SUM(po.popvalorpagamento) 
						    FROM par.pagamento p 
						    INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid 
						    WHERE p.pagstatus = 'A' AND trim(p.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA') AND po.preid = pre.preid 
						) AS \"Valor Total Solicitado da Obra\",
						-- Percentual total pago da obra
						( SELECT 
        						CASE WHEN pe.prevalorobra > 0 
	        					THEN ROUND(SUM( po.popvalorpagamento * 100 ) / pe.prevalorobra)
	        					ELSE 0 END 
						    FROM par.pagamento p 
						    INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid 
						    INNER JOIN obras.preobra pe ON pe.preid = po.preid 
						    WHERE p.pagstatus = 'A' 
						        AND trim(p.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')  
						        AND po.preid = pre.preid 
						    GROUP BY pe.prevalorobra
						) AS \"Percentual Total Solicitado da Obra\",
        		
						-- Valor total pago (efetivado)
						( SELECT SUM(po.popvalorpagamento) 
						    FROM par.pagamento p 
						    INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid 
						    WHERE p.pagstatus = 'A' AND p.pagsituacaopagamento ilike '%EFETIVADO%' AND po.preid = pre.preid 
						) AS \"Valor Total Pago da Obra\",
						-- Percentual total Pago da Obra após efetivação das parcelas
						( SELECT 
        						CASE WHEN pe.prevalorobra > 0 then 
						        ROUND(SUM( po.popvalorpagamento * 100 ) / pe.prevalorobra)
						        ELSE 0 END 
						    FROM par.pagamento p 
						    INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid 
						    INNER JOIN obras.preobra pe ON pe.preid = po.preid 
						    WHERE p.pagstatus = 'A' 
						        AND p.pagsituacaopagamento ilike '%EFETIVADO%'
						        AND po.preid = pre.preid 
						    GROUP BY pe.prevalorobra
						) AS \"Percentual Total Pago da Obra\",
        		
						po.popvalorpagamento AS \"Valor da Parcela Solicitada\",
        		
        				(SELECT 
							to_char(MIN(pg5.pagdatapagamento),'DD/MM/YYYY') as dt
						FROM par.pagamento pg5
						INNER JOIN par.pagamentoobrapar po5 ON po5.pagid = pg5.pagid
						WHERE 
							pg5.pagstatus = 'A' 
							AND po5.preid = pre.preid 
							AND trim(pg5.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
						) as \"Data Pagamento Solicitado\",
        		
						((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as \"% executado instituição\",
						usu.usunome as \"Nome do usuário que fez o pagamento\",
						pag.pagparcela||'&nbsp;' as \"Parcela\",
						pag.pagsituacaopagamento as \"Situação\",
						(1+
						( CASE WHEN (select count(vldid) from obras2.validacao where vldstatushomologacao = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
						( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus25exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
						( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus50exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END ))||' / 4' as \"Execução Física\",
						e.empfonterecurso||'&nbsp;' as \"Fonte do Recurso\",
                		(par.retornasaldoprocesso(p.pronumeroprocesso)) as \"Saldo Conta Corrente\",
        				pag.parnumseqob||'&nbsp;' as \"Sequencial Ordem Bancária\",
        				pag.pagvalorparcela as \"Valor da OB\"
					FROM par.processoobraspar p
					INNER JOIN par.processoobrasparcomposicao pc ON p.proid = pc.proid AND pc.pocstatus = 'A'
					INNER JOIN obras.preobra pre ON pre.preid = pc.preid
					INNER JOIN obras.pretipoobra pto on pto.ptoid = pre.ptoid
					INNER JOIN par.empenho e ON e.empnumeroprocesso = p.pronumeroprocesso AND e.empstatus = 'A'
                    LEFT  JOIN par.vinculacaoptressigef	vps ON vps.vpsptres = e.empcodigoptres 
					INNER JOIN par.empenhoobrapar eo ON eo.empid = e.empid AND eo.preid = pc.preid AND eo.eobstatus = 'A'
					INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A'
					INNER JOIN par.pagamentoobrapar po ON po.pagid = pag.pagid AND po.preid = eo.preid
					INNER JOIN seguranca.usuario usu ON usu.usucpf = pag.usucpf
					LEFT  JOIN obras2.obras o ON o.preid = pre.preid AND o.obridpai IS NULL AND o.obrstatus = 'A'
					LEFT  JOIN territorios.municipio m   ON m.muncod = pre.muncod
					LEFT  JOIN obras2.situacao_registro str ON str.strid = o.strid and str.strstatus = 'A'
					WHERE p.prostatus = 'A'";
        if ($strWhere != '') {
            $sql .= $strWhere;
        }
        $sql .="UNION ALL
        			SELECT DISTINCT
						to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"\/\"0000\"-\"00') as  \"Número processo\",  -- numero do processo
						'PAC' as \"Programa\",
						'<input type=hidden />'||pre.preid as \"ID preobra\",
						'<input type=hidden />'||o.obrid as \"ID da obra\",-- id da obra;
						pto.ptodescricao as \"Tipo da Obra\", -- tipo da obra
						pre.predescricao as \"Descrição da Obra\", -- Descrição da Obra;
						str.strdsc as \"Situação da Obra\",
						CASE WHEN (p.muncod is null or p.muncod = '') 
						    THEN 'Estadual' 
						    ELSE 'Municipal' 
						END as \"Processo Estadual ou Municipal\",
						m.mundescricao as \"Município\", -- município; 
						pre.estuf AS \"UF\", -- uf; 
						coalesce(TRUNC (pre.prevalorobra, 2),0) AS \"Valor da Obra\", -- valor da obra; 
						e.empnumero as \"Número empenho\", 
						'( '||vps.vpsvinculacao||' )' as \"N° da Vinculação\",
						-- SUM(eo.eobvalorempenho) as \"Valor empenhado na NE\",
						( SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE preid = pre.preid GROUP BY preid  ) AS \"Valor total empenhado da obra\",
        		
						-- Valor Total Solicitado da Obra
						( SELECT SUM(po.pobvalorpagamento) 
						    FROM par.pagamento p 
						    INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid 
						    WHERE p.pagstatus = 'A' AND trim(p.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')  AND po.preid = pre.preid 
						) AS \"Valor Total Solicitado da Obra\",	
						-- Percentual total Solicitado da obra
						( 	SELECT 
        						CASE WHEN pe.prevalorobra > 0 then 
						        ROUND(SUM( po.pobvalorpagamento * 100 ) / pe.prevalorobra)
						        ELSE 0 END 
							FROM par.pagamento p 
							INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid 
							INNER JOIN obras.preobra pe ON pe.preid = po.preid 
							WHERE p.pagstatus = 'A' 
							    AND trim(p.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')  
							    AND po.preid = pre.preid 
							GROUP BY pe.prevalorobra
						) AS \"Percentual Total Solicitado da Obra\",	
        		
						-- Percentual Total Pago (Efetivado)
						( 	SELECT SUM(po.pobvalorpagamento) 
						    FROM par.pagamento p 
						    INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid 
						    WHERE p.pagstatus = 'A' AND p.pagsituacaopagamento ILIKE '%EFETIVADO%' AND po.preid = pre.preid 
						) AS \"Valor Total Pago da Obra\",
						-- Percentual total Pago da Obra (efetivado)
						( 	SELECT case when pe.prevalorobra > 0 then 
						        ROUND(SUM( po.pobvalorpagamento * 100 ) / pe.prevalorobra)
						        else 0 end 
						    FROM par.pagamento p 
						    INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid 
						    INNER JOIN obras.preobra pe ON pe.preid = po.preid 
						    WHERE p.pagstatus = 'A' 
								AND p.pagsituacaopagamento ILIKE '%EFETIVADO%'
								AND po.preid = pre.preid 
						    GROUP BY pe.prevalorobra
						) AS \"Percentual Total Pago da Obra\",
						po.pobvalorpagamento AS \"Valor da Parcela Solicitada\",
        		
						(SELECT 
							to_char(MIN(pg5.pagdatapagamento),'DD/MM/YYYY') as dt
						FROM par.pagamento pg5
						INNER JOIN par.pagamentoobra po5 ON po5.pagid = pg5.pagid
						WHERE 
							pg5.pagstatus = 'A' 
							AND po5.preid = pre.preid 
							AND trim(pg5.pagsituacaopagamento) IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA')
						) as \"Data Pagamento Solicitado\",
        		
						((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as \"% executado instituição\",
						usu.usunome as \"Nome do usuário que fez o pagamento\",
						pag.pagparcela||'&nbsp;' as \"Parcela\",
						pag.pagsituacaopagamento as situacao,
						(1+
						( CASE WHEN (select count(vldid) from obras2.validacao where vldstatushomologacao = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
						( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus25exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
						( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus50exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END ))||' / 4' as \"Execução Física\",
						e.empfonterecurso||'&nbsp;' as \"Fonte do Recurso\",
                		(par.retornasaldoprocesso(p.pronumeroprocesso)) as \"Saldo Conta Corrente\",
        				pag.parnumseqob||'&nbsp;' as \"Sequencial Ordem Bancária\",
        				pag.pagvalorparcela as \"Valor da OB\"
					FROM par.processoobra p
					INNER JOIN par.processoobraspaccomposicao pc ON p.proid = pc.proid AND pc.pocstatus = 'A'
					INNER JOIN obras.preobra pre ON pre.preid = pc.preid 
					INNER JOIN obras.pretipoobra pto on pto.ptoid = pre.ptoid
					INNER JOIN par.empenho e ON e.empnumeroprocesso = p.pronumeroprocesso AND e.empstatus = 'A'
                    LEFT  JOIN par.vinculacaoptressigef	vps ON vps.vpsptres = e.empcodigoptres 
					INNER JOIN par.empenhoobra eo ON eo.empid = e.empid AND eo.preid = pc.preid AND eo.eobstatus = 'A'
					INNER JOIN par.pagamento pag ON pag.empid = e.empid AND pag.pagstatus = 'A'
					INNER JOIN par.pagamentoobra po ON po.pagid = pag.pagid AND po.preid = eo.preid
					INNER JOIN seguranca.usuario usu ON usu.usucpf = pag.usucpf
					LEFT JOIN obras2.obras o ON o.preid = pre.preid AND o.obridpai IS NULL AND o.obrstatus = 'A'
					LEFT  JOIN territorios.municipio m ON m.muncod = pre.muncod
					LEFT JOIN obras2.situacao_registro str ON str.strid = o.strid and str.strstatus = 'A'
					WHERE p.prostatus = 'A'";
        if ($strWhere != '') {
            $sql .= $strWhere;
        }
        $sql .= ") as foo ";

        if ($tipoObra) {
            $sql .= $strWhereTipoObra;
        }
        $sql .= "ORDER BY \"Programa\", \"UF\", \"Município\" ";
        return $this->dados = $this->db->carregar($sql);
    }

}

$dados = $obRelatorio->getArrayRel($_POST);
?>
<html>
    <head>

<?php
if (!$isXls) {
    ?>
            <script type="text/javascript" src="../includes/funcoes.js"></script>
            <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
            <link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
            <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
    <?php
}
?>

    </head>
    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
// Faz requisição para o método retornaDados que irá retornar o array com os dados do relatório
//$dados =  $obRelatorio->retornaDados();
// Caso exista dados ele  monta o relatório, senão exibe mensagem de que não existem registros

if (count($dados) && is_array($dados)) {
    // Caso seja uma exportação para excel
    if ($isXls) {

		ob_clean();

		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatorioGerencialPagamentoObrasPar".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
        $cabecalho = Array();
        foreach ($dados[0] as $colKey => $colVal) {
        	$cabecalho[] = $colKey;
        }
		$db->monta_lista_tabulado($dados,$cabecalho,100000000,5,'N','100%',$par2);
		die();

    }

    // Monta o cabeçalho com o Brasao
    $cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
    $cabecalhoBrasao .= "<tr>" .
            "<td colspan=\"100\">" .
            monta_cabecalho_relatorio('100') .
            "</td>" .
            "</tr>
				        </table>";


    // Caso não seja excel monta cabeçalho e título						
    if (!$isXls) {
        echo $cabecalhoBrasao;
        monta_titulo('Relatório Gerencial Pagamento Obras', '');
    }
    // Monta tabela que irá exibir os dados
    echo '<table class="table table-bordered table-hover table-striped table-condensed" align="center" style="width: 98%" >';

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
            if (is_numeric($colVal) ) {
                $cor = 'style="color:blue"';
                $colVal = number_format($colVal, '2', ',', '.');
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
?>
</body>