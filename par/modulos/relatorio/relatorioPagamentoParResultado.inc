<?
// Codificação do relatório
header( 'Content-Type: text/html; charset=iso-8859-1' );
// Objeto da classe do relatório 
$obRelatorio = new relatorioPagamentoParResultado();
// Verifica se é um relatório p o browser ou excel
$isXls = ($_REQUEST['requisicao'] == 'relatorio_xls') ? true :false;

// Classe responsável por montar o resultado do relatorio
class relatorioPagamentoParResultado
{
	
	// Método construtor
	function __construct()
	{
		// Instancia a classe do BD
		$this->db = new cls_banco();

	}
	public function getArrayRel($post)
	{
		$strWhere 	= '';
		$preid 		= $post['preid'];
		$arrMuncod 	= $post['muncod'];
		$empnumero 	= $post['empnumero'];
		$usunome 	= $post['usunome'];
		$pagDataDe 	= $post['pagdatapagamento_de'];
		$pagDataAte	= $post['pagdatapagamento_ate'];
		$uf			= $post['uf'];
		$nprocesso	= $post['nprocesso'];
		$situacao	= $post['situacao'];
		$tipoObra	= $post['tipo_obra'];
		
		if( $preid )
		{
			$strWhere .= " AND
							CAST(obr.obrid AS char(50)) ilike '%{$preid}%' " ;
		}
		
		if( (is_array($arrMuncod)) && (count($arrMuncod)) && ($arrMuncod[0]) )
		{
		
			foreach( $arrMuncod as $k => $v )
			{
				$arrMuncod[$k] = "'{$v}'";
			}
			
			$strReturn = implode(",", $arrMuncod);
		
			$strWhere .= " AND
							m.muncod in ( $strReturn ) ";
		}
		
		
		if( $empnumero )
		{
			$strWhere .= "AND 
							emp.empnumero ilike '%{$empnumero}%' ";
		}
		
		
		if( $usunome )
		{
            $usunomeTmp = removeAcentos(str_replace("-"," ",$usunome));
			$strWhere .= "	AND 
							UPPER(public.removeacento(usu.usunome)) ILIKE '%{$usunomeTmp}%' ";
		}
		
		if( $pagDataDe )
		{
			$data		= formata_data_sql($_POST['pagdatapagamento_de']);
			$strWhere 	.= " AND
							pa.pagdatapagamento >= '{$data}' ";
		}
		if( $pagDataAte )
		{
			$data		= formata_data_sql($_POST['pagdatapagamento_ate']);
			$strWhere 	.= " AND
							pa.pagdatapagamento <= '{$data}' ";
		}
		
		if( (is_array($uf)) && (count($uf)) && ($uf[0]) )
		{
			
			foreach( $uf as $k => $v )
			{
				$uf[$k] = "'{$v}'";
			}
			
			$strReturnUF = implode(",", $uf);
			$strWhere .= " AND 
	
						CASE WHEN p.estuf IS NOT NULL THEN 
						
							p.estuf in ( {$strReturnUF} )
						ELSE 
							m.estuf in ( {$strReturnUF} )
						END ";
		}
		
		if( $nprocesso )
		{
			$nprocesso = str_replace( '.' , '', $nprocesso);
			$nprocesso = str_replace( '/' , '', $nprocesso);
			$nprocesso = str_replace( '-' , '', $nprocesso);
			$strWhere .= " AND
							p.pronumeroprocesso ilike '%$nprocesso%' ";
		}
		
		if( $situacao )
		{
			$strWhere .=  "AND
			
							pa.pagsituacaopagamento = '{$situacao}' ";
		}
		
		if( $tipoObra )
		{
			$strWhereTipoObra =  " where \"Programa\" = '$tipoObra' ";
		}
		
		/* Alterado dia 29/10/2014 por Eduardo Dunice a pedido
		 * da Demanda PAR 1265.
		 * */ 
		$sqlEmpenhoObras = " 'R$ '||to_char( ( SELECT saldo FROM par.v_dadosfinanceiroporobra veo WHERE veo.preid = pre.preid ),'999G999G990D99') as \"Valor total empenhado da obra\", -- valor total empenhado;";
		/* FIM - Alterado dia 29/10/2014 por Eduardo Dunice a pedido da Demanda PAR 1265. */
			     		
		$sql = "SELECT * 
				FROM (
					SELECT DISTINCT 
						'PAR' as \"Programa\",
						obr.obrid as \"ID da obra\",-- id da obra;
						pre.predescricao\"Descrição da Obra\", -- Descrição da Obra;
						pto.ptodescricao \"Tipo da Obra\", -- tipo da obra
						m.mundescricao as \"Município\", -- município; 
						CASE WHEN p.estuf IS NOT NULL THEN 
							p.estuf 
						ELSE 
							m.estuf
						END as \"UF\", -- uf; 
						emp.empnumero as \"Número empenho\", -- n empenho;

			       		{$sqlEmpenhoObras}

			       		'R$ '||to_char(coalesce((SELECT SUM(popvalorpagamento) as perc_pag FROM par.pagamentoobrapar pao WHERE pao.pagid = pa.pagid AND pao.preid = poc.preid ),0),'999G999G990D99') as \"Valor da Parcela\",
						coalesce(to_char((CASE WHEN pre.prevalorobra > 0 THEN
							ROUND( 
							(
								(
				                SELECT SUM(popvalorpagamento) 
				                FROM par.pagamentoobrapar p2 
				                INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid 
				                WHERE p2.preid = pre.preid AND pag2.pagid = pa.pagid AND pag2.pagstatus = 'A'  
								) 
							/ pre.prevalorobra  
							)*100) 
							ELSE 0
						END),'990D99'),'0,00')||' %' as \"Percentual pagamento\", -- % pagamento; 
						usu.usunome as \"Nome do usuário\",
							
						pa.pagparcela as \"Parcela\",
						to_char(pa.pagdatapagamento, 'DD/MM/YYYY') as \"Data pagamento\",
						pa.pagsituacaopagamento as situacao,
						'R$ '||to_char(coalesce(TRUNC (pre.prevalorobra, 2),0),'999G999G990D99') as \"Valor da Obra\", 
						'R$ '||to_char(coalesce((
							SELECT 
								COALESCE(sum(po.popvalorpagamento),0.00) 
							FROM 
								par.pagamento pg 
							INNER JOIN par.empenho em ON em.empid=pg.empid and empstatus = 'A' 
							INNER JOIN par.pagamentoobrapar po ON po.pagid = pg.pagid
							WHERE 
								em.empnumeroprocesso = p.pronumeroprocesso AND
								po.preid = pre.preid
								AND pg.pagstatus='A' AND pg.pagsituacaopagamento not ilike '%CANCELADO%'
						),0),'999G999G990D99') as \"Valor total pago da obra\", -- valor total pago da obra; 
						coalesce(to_char((CASE WHEN pre.prevalorobra > 0 THEN
							ROUND( 
							(
								(
				                SELECT SUM(popvalorpagamento) 
				                FROM par.pagamentoobrapar p2 
				                INNER JOIN par.pagamento pag2 ON pag2.pagid = p2.pagid 
				                WHERE p2.preid = pre.preid AND pag2.pagstatus = 'A'  
								) 
							/ pre.prevalorobra  
							)*100) 
							ELSE 0
						END),'990D99'),'0,00')||' %' as \"Percentual total pago da obra\",  -- % total pago da obra;
										
						CASE WHEN (p.muncod is null or p.muncod = '') THEN 
							'Estadual' 
						ELSE 
							'Municipal' 
						END 
						as \"Esfera\",
						to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as  \"Número processo\", 
						replace( obr.obrpercentultvistoria::text, '.',',') || '%' as \"% executado instituição\",
						emp.empfonterecurso as \"Fonte do Recurso\"
					FROM 
						par.pagamentoobrapar pob
					INNER JOIN par.processoobrasparcomposicao 	poc ON poc.preid = pob.preid and poc.pocstatus = 'A'
					INNER JOIN par.processoobraspar 			p   ON poc.proid = p.proid and p.prostatus = 'A'
					INNER JOIN par.pagamento   					pa  ON pob.pagid = pa.pagid AND pa.pagstatus = 'A'
					INNER JOIN seguranca.usuario 				usu ON usu.usucpf = pa.usucpf
					INNER JOIN par.empenho			 			emp ON p.pronumeroprocesso = emp.empnumeroprocesso and empstatus = 'A' AND pa.empid = emp.empid
					INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid
					INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid
					LEFT  JOIN obras2.obras 					obr ON obr.preid = pre.preid AND (obr.obridpai IS NULL) --obr.obrid = pre.obrid_1
					LEFT  JOIN obras2.validacao 				val ON val.obrid = obr.obrid
					LEFT  JOIN territorios.municipio 			m   ON m.muncod = pre.muncod
					LEFT  JOIN par.documentopar 				dp  ON dp.proid = p.proid AND dopstatus = 'A'
					LEFT  JOIN territorios.estado 				e   ON e.estuf = p.estuf
					LEFT  JOIN territorios.muntipomunicipio 	mt  ON mt.muncod = m.muncod
					LEFT  JOIN territorios.tipomunicipio 		tm  ON tm.tpmid = mt.tpmid
					LEFT  JOIN par.instrumentounidadeentidade 	iue ON iue.inuid = p.inuid and iue.iuestatus = 'A' and iue.iuedefault = true
					LEFT  JOIN par.instrumentounidade 			iu  ON iu.inuid = p.inuid
					LEFT  JOIN workflow.documento 				d   ON d.docid = iu.docid AND d.tpdid = 44
					LEFT  JOIN workflow.estadodocumento 		ed  ON ed.esdid = d.esdid
					WHERE 
						(pa.pagsituacaopagamento IS NULL OR pa.pagsituacaopagamento not ilike '%CANCELADO%')";
		if($strWhere != ''){
        	$sql .= $strWhere; 
        }			
		$sql .="UNION ALL
				SELECT DISTINCT 
					'PAC' as \"Programa\",
					obr.obrid as \"ID da obra\",-- id da obra;
					pre.predescricao\"Descrição da Obra\", -- Descrição da Obra;
					pto.ptodescricao \"Tipo da Obra\", -- tipo da obra
					m.mundescricao as \"Município\", -- município; 
					CASE WHEN p.estuf IS NOT NULL THEN 
						p.estuf 
					ELSE 
						m.estuf
					END as \"UF\", -- uf; 
					emp.empnumero as \"Número empenho\", -- n empenho;

					'R$ '||to_char(coalesce((
                    	SELECT
                        	COALESCE(sum(eo.eobvalorempenho - coalesce(vrlcancelado,0)),0.00)
                        FROM
                        	par.empenho em
                        	INNER JOIN par.empenhoobra eo ON eo.empid = em.empid and eobstatus = 'A' and empstatus = 'A'
							left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
														where empcodigoespecie in ('03', '13', '04')
														group by 
															empnumeroprocesso,
															empcodigoespecie,
															empidpai) as ep on ep.empidpai = em.empid
                        WHERE
                        	em.empnumeroprocesso = p.pronumeroprocesso 
                            AND eo.preid = pre.preid
                            
					),0),'999G999G990D99') as \"Valor total empenhado da obra\", -- valor total empenhado;

					'R$ '||to_char(coalesce((SELECT SUM(pobvalorpagamento) as perc_pag FROM par.pagamentoobra pao WHERE pao.pagid = pa.pagid AND pao.preid = poc.preid ),0),'999G999G990D99') as \"Valor da Parcela\",
					coalesce(to_char((SELECT COALESCE((pao.pobvalorpagamento*100)/pre.prevalorobra,0.00) as perc_pag 
							FROM par.pagamentoobra pao 
							WHERE pao.pagid = pa.pagid AND pa.pagsituacaopagamento not ilike '%CANCELADO%' AND pao.preid = poc.preid ),'990D99'),'0,00')||' %' as \"Percentual pagamento\", -- % pagamento; 
					usu.usunome as \"Nome do usuário\",
							
					pa.pagparcela as \"Parcela\",
					to_char(pa.pagdatapagamento, 'DD/MM/YYYY') as \"Data pagamento\",
					pa.pagsituacaopagamento as situacao,
					'R$ '||to_char(coalesce(TRUNC (pre.prevalorobra, 2),0),'999G999G990D99') as \"Valor da Obra\", -- valor da obra; 
					'R$ '||to_char(coalesce((
						SELECT 
							COALESCE(sum(po.pobvalorpagamento),0.00) 
						FROM 
							par.pagamento pg 
						INNER JOIN par.empenho em ON em.empid=pg.empid and empstatus = 'A' 
						INNER JOIN par.pagamentoobra po ON po.pagid = pg.pagid
						WHERE 
							em.empnumeroprocesso = p.pronumeroprocesso AND
							po.preid = pre.preid
							AND pg.pagstatus='A' AND pg.pagsituacaopagamento not ilike '%CANCELADO%'
					),0),'999G999G990D99') as \"Valor total pago da obra\", -- valor total pago da obra; 
					coalesce(to_char( 
						(SELECT ROUND(SUM( pao.pobvalorpagamento * 100 ) / p.prevalorobra) 
						FROM par.pagamentoobra pao 
						INNER JOIN par.pagamento pag1 ON pag1.pagid = pao.pagid AND pag1.pagsituacaopagamento not ilike '%CANCELADO%' AND pag1.pagstatus = 'A' 
						INNER JOIN obras.preobra p ON p.preid = pao.preid 
						WHERE pao.preid = pre.preid
						GROUP BY p.prevalorobra ),'990D99'
					),'0,00')||' %' as \"Percentual total pago da obra\",  -- % total pago da obra;
										
					CASE WHEN (p.muncod is null or p.muncod = '') THEN 
						'Estadual' 
					ELSE 
						'Municipal' 
					END as \"Esfera\",
					to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as  \"Número processo\", 
					replace( obr.obrpercentultvistoria::text, '.',',') || '%' as \"% executado instituição\",
					emp.empfonterecurso as \"Fonte do Recurso\"
				FROM 
					par.pagamentoobra pob
				INNER JOIN par.processoobraspaccomposicao 	poc ON poc.preid = pob.preid and poc.pocstatus = 'A'
				INNER JOIN par.processoobra 				p   ON poc.proid = p.proid  and p.prostatus = 'A'
				INNER JOIN par.pagamento                    pa  ON pob.pagid = pa.pagid AND pa.pagstatus = 'A'
				INNER JOIN seguranca.usuario 				usu ON usu.usucpf = pa.usucpf
				INNER JOIN par.empenho			 			emp ON p.pronumeroprocesso = emp.empnumeroprocesso AND empstatus = 'A' AND pa.empid = emp.empid 
				INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid
				INNER JOIN obras.pretipoobra 				pto on pto.ptoid = pre.ptoid
				LEFT  JOIN obras2.obras 					obr ON obr.preid = pre.preid AND (obr.obridpai IS NULL) --obr.obrid = pre.obrid_1
				LEFT  JOIN obras2.validacao 				val ON val.obrid = obr.obrid
				LEFT  JOIN territorios.municipio 			m   ON m.muncod = pre.muncod
				LEFT  JOIN par.termocompromissopac 			tc  ON tc.proid = p.proid AND terstatus = 'A'
				LEFT  JOIN territorios.estado 				e   ON e.estuf = p.estuf
				LEFT  JOIN territorios.muntipomunicipio 	mt  ON mt.muncod = m.muncod
				LEFT  JOIN territorios.tipomunicipio 		tm  ON tm.tpmid = mt.tpmid
				WHERE 
					(pa.pagsituacaopagamento IS NULL OR pa.pagsituacaopagamento not ilike '%CANCELADO%')";
		if($strWhere != '')
        {
        	$sql .= $strWhere; 
        }			
		$sql .= ") as foo ";
								
		if( $tipoObra )
		{
			$sql .=	$strWhereTipoObra ;
		}
								
		$sql .= "ORDER BY \"Programa\", \"UF\", \"Município\" "; 
// ver($sql,d);
		return $this->dados = $this->db->carregar( $sql );
		
	}
}

$dados = $obRelatorio->getArrayRel($_POST);
 
?>
<html>
	<head>
		
<?php 
	if( ! $isXls )
{
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

if( count($dados) && is_array($dados) )
{
	// Caso seja uma exportação para excel
	if($isXls)
	{
		$file_name="RelatorioPagamentoObrasPar.xls";
		header("Pragma: public");
		header("Expires: 0");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Content-Type: application/force-download");
		header("Content-Type: application/octet-stream");
		header("Content-Type: application/download");
		header("Content-Disposition: attachment;filename=".$file_name);
		header("Content-Transfer-Encoding: binary ");
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
	if( ! $isXls )
	{
		echo $cabecalhoBrasao;
		monta_titulo('Relatório Pagamento Obras','');
	}
	// Monta tabela que irá exibir os dados
	echo '<table class="table table-bordered table-hover table-striped table-condensed" align="center" style="width: 98%" >';
	// Cabeçalho para o arquivo excel
	if( $isXls )
	{
		echo ' <tr style="background-color: #dcdcdc;"> <td colspan="' . count($dados[0]) . '" > Relatório Pagamento Obras</td></tr>';
	}
	
	// Começa linha com os nomes das colunas
	echo '	<tr >' ;
	foreach($dados[0] as $colKey => $colVal )
	{
		echo "		<td style=\"background-color: #e9e9e9;\" align=\'center\'  > <b> {$colKey} </b> </td>";
	}
	echo '</tr>';
	
	// Lista as informações do relatório
	foreach($dados as $linhaKey => $linhaVal )
	{
		echo '<tr>';
		$arrNumericos = Array('Valor total empenhado da obra','Valor da Parcela','Percentual pagamento','Valor da Obra','Valor total pago da obra','Percentual total pago da obra','% executado instituição');
		foreach( $linhaVal as $colKey => $colVal )
		{
			$cor = '';
			if( in_array($colKey,$arrNumericos) ){
				$cor = 'style="color:blue"';
			}
			if( !$colVal ){ $colVal = '-'; }
			echo " <td align=\'center\' width=\'50%\' $cor > <b> {$colVal} </b> </td>";	
		}
		
	}
	// Finaliza o relatório
	echo '</table>';
	
}
else 
{
	// Exibe mensagem de que não existem registros.
	die( '<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">'.
										'<TR style="background:#FFF; color:red;">'.
											'<TD colspan="10" align="center">'.
												'Não foram encontrados registros.'.
											'</TD>'.
										'</TR>'.
								   '</table>');
}
 
if($isXls)
{
	// Finaliza processo
	die('<script>window.close();</script>');
}

?>

</body>