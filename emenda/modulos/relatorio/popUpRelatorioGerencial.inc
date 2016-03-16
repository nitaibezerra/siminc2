<?php
set_time_limit(0);
ini_set("memory_limit", "1024M");
montaBotaoForm();

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	 <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if($_POST['limpaSession']){
	unset($_SESSION['emenda']['post']);
	$_POST['limpaSession'] = "";
}

if( empty($_SESSION['emenda']['post']) ){
	$_SESSION['emenda']['post'] = $_POST;
}
$col = monta_coluna();
$coluna = array();
$cabecalho = array();
$id = array();

foreach ($col as $c) {
	if( $c['campo'] ){
		$coluna[] = $c['campo'];
	}
	$cabecalho[] = $c['label'];
	
	if( strstr( strtoupper($c['label']), strtoupper('valor') ) ) {
		$formato[] = "n";
	} else {
		$formato[] = "";
	}
	$id[] = $c['id'];
}

$arSql   = monta_sql($coluna, $id);
sort($arSql['sql']);

if( in_array( 'valor_pagamento', $id ) && in_array( 'partido', $id ) && sizeof($id) == 2 ){
	$cabecalho = array('Partido', 'Pagamento Solicitado', 'Pagamento Efetivado');
}

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	sort($arSql['excel']);
	$db->sql_to_excel($arSql['excel'], 'relEmendasGerencial', $cabecalho, $formato);
	exit;
	$exporta = "false";
}

$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				"</td>" .
			  "</tr>
			  </table>";

echo $cabecalhoBrasao;
$db->monta_lista_array( $arSql['sql'], $cabecalho, 100000, 1, 'N', 'center', '');

montaBotaoForm();

echo '<script>
		$(\'loader-container\').hide();
	  </script>';

function montaBotaoForm(){
	echo '	<html>
			<head>
				<style>
				
				#loader-container,
				#LOADER-CONTAINER{
				    background: transparent;
				    position: absolute;
				    width: 100%;
				    text-align: center;
				    z-index: 8000;
				    height: 100%;
				}
				
				
				#loader {
				    background-color: #fff;
				    color: #000033;
				    width: 300px;
				    border: 2px solid #cccccc;
				    font-size: 12px;
				    padding: 25px;
				    font-weight: bold;
				    margin: 150px auto;
				}
				</style>
				</head>
			<script type="text/javascript" src="/includes/prototype.js"></script>
			<body>
			<style type="">
				@media print {.notprint { display: none } .div_rolagem{display: none} }	
				@media screen {.notscreen { display: none; }
				
				.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
				
			</style>
			<div id="loader-container" style="display: none">
		   		<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
			</div>
			<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
					</td>
				</tr>
			</table>
			</form>
			</body>
			<script type="text/javascript">				
				function exportarExcel(){
					document.getElementById(\'exporta\').value = "true";
					document.getElementById(\'formulario\').submit();
				}
				$(\'loader-container\').show();
			</script>
			</html>';
}

function monta_sql($cols, $id){
	global $db;
	$arFiltros = array();
	$carregaCPFresp = false;
	extract($_SESSION['emenda']['post']);
	$group = "ptr.ptrid";
	foreach($coluna as $colu) {
		if( $colu != 'valor_empenhado' && $colu != 'valor_pagamento' ){
			$group.= ", ".$colu;
		}elseif( $colu == 'valor_pago_efetivado' ){
			$group .= ", ob.spgcodigo";
		}
	}

	$estuf		 = !is_array($estuf) ? explode(',', $estuf) : $estuf;
	
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
		$where[] = " enb.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	
	if($numsiafi){
		$where[] = "ptmi.pmcnumconveniosiafi = '".$pmcnumconveniosiafi."'";
		$arFiltros[] = 'numsiafi';
	}	

	if($numfnde){
		$where[] = "ptr.ptrnumconvenio = '".$ptrnumconvenio."'";
		$arFiltros[] = 'numfnde';
	}	

	if($ptrcod){
		$where[] = "ptr.ptrcod = '".$ptrcod."'";
	}	
	
	//$where[] = "eme.emeano = ".$_SESSION['exercicio'];
	
	$order[] = 'ptr.ptrcod';

	#verifica a diferença entre o array de ordenação com o de colunas
	#caso exista diferença, deleta os valores de ordenação que estão diferente do de colunas
	$arDif = array_diff( $order, $cols );
	$arDif = ( $arDif ? $arDif : array() );
	$arDifKey = array_keys( $arDif );

	if( $arDifKey ){
		foreach ($arDifKey as $v) {
			unset( $order[$v] );
		}		
	}
		
	if( !empty($geradoconvenio) ){
		if($geradoconvenio == 'S'){
			$where[] = "ptr.ptrnumconvenio is not null";
		}else{
			$where[] = "ptr.ptrnumconvenio is null";
		}
		$arFiltros[] = 'ptrnumconvenio';
	}
	if( !empty($empenhado) ){
		if($empenhado == 'S'){
			$where[] = " exf.semid = 4 ";
		}else{
			$where[] = " exf.semid is null OR exf.semid <> 4 ";
		}
		$arFiltros[] = 'empenhado';
	}
	if( !empty($pagamento) ){
		if($pagamento == 'S'){
			$where[] = " (ob.orbvalorparcela) is not null ";
		}else{
			$where[] = " (ob.orbvalorparcela) is null ";
		}
		$arFiltros[] = 'pagamento';
	}
	if( !empty($pagamentoE) ){
		if($pagamentoE == 'S'){
			$where[] = " ( SELECT count(exfid) FROM emenda.ordembancaria ob2 WHERE ob2.spgcodigo = '2' AND ob2.exfid = ob.exfid ) > 0";
		}else{
			$where[] = " ( SELECT count(exfid) FROM emenda.ordembancaria ob2 WHERE ob2.spgcodigo = '2' AND ob2.exfid = ob.exfid ) = 0";
		}
		$arFiltros[] = 'pagamento';
	}

/*
	$arColunas = array_merge( $cols, $id );
	$arJoinColunas = montaJoinColunas( $arColunas, $arFiltros );
	$arJoinColunas['colunas'] = array_merge( $arJoinColunas['colunas'], $cols );
	$stColunas = (is_array($arJoinColunas['colunas']) && $arJoinColunas['colunas'][0]) ? ', '.implode(', ', $arJoinColunas['colunas']) : "*";
*/
	$stColunas = implode(', ', $cols);
	//ver($cols, sizeof($cols),d);
	if($stColunas && sizeof($cols) > 2 ) {
		$sql = "SELECT DISTINCT ptr.ptrid, 
					$stColunas
				FROM
					emenda.planotrabalho ptr
				LEFT JOIN 
					(SELECT 
						ptrid,
						sum(ptivalortotal) as ptivalortotal
					 FROM emenda.v_ptiniciativa 
					 GROUP BY ptrid) pti ON pti.ptrid = ptr.ptrid
				LEFT JOIN
					emenda.execucaofinanceira exf 
					INNER JOIN
						emenda.ordembancaria ob ON ob.exfid = exf.exfid
				ON exf.ptrid = ptr.ptrid AND exf.exfstatus = 'A'
				LEFT JOIN 
					emenda.ptminutaconvenio ptmi ON ptmi.ptrid = ptr.ptrid AND ptmi.pmcstatus = 'A'
				INNER JOIN
					emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid and enb.enbstatus = 'A'
				WHERE
   					ptr.ptrstatus = 'A'
   					and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
   					".(($where)? ' and '.implode(" AND ", $where):"")."
   				GROUP BY
   					".$group."
   				".($id ? "ORDER BY ptr.ptrid " : '');

//ver($sql);
		$arDados = $db->carregar( $sql );
		
	} else {
		if( in_array( 'valor_pagamento', $id ) && in_array( 'partido', $id ) ){
			$sql = "select parsigla||' - '||partido as partido, sum(valor_pagamento) as valor_pagamento, sum(valor_pago_efetivado) as valor_pago_efetivado from (
					    SELECT DISTINCT ptr.ptrid,  sum(COALESCE(ob.orbvalorparcela,0)) as valor_pagamento,
					        CASE WHEN ob.spgcodigo = '2' THEN sum(COALESCE(ob.orbvalorparcela,0)) ELSE 0 END as valor_pago_efetivado,
					        p.parnome as partido, p.parsigla
					    FROM
					        emenda.planotrabalho ptr
					        INNER JOIN emenda.execucaofinanceira exf ON exf.ptrid = ptr.ptrid AND exf.exfstatus = 'A'
					        INNER JOIN emenda.ordembancaria ob ON ob.exfid = exf.exfid
					        inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
					        inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid and ve.edestatus = 'A'
					        inner join emenda.autor a on a.autid = ve.autid and a.autstatus = 'A'
					        inner join emenda.partido p on p.parid = a.parid and p.parstatus = 'A'
					    WHERE
					        ptr.ptrstatus = 'A'
					        and ptr.ptrexercicio = '{$_SESSION['exercicio']}'
					       					
					    GROUP BY
					        p.parnome, p.parsigla, ptr.ptrid, ob.spgcodigo
					    ORDER BY 
					        partido
					) as foo
					group by 
						parsigla, partido
					order by partido";
					
			$arRegistro = $db->carregar( $sql );
			$arRegistro = $arRegistro ? $arRegistro : array();
			//ver($arRegistro,d);
			/*$html = '<table id="table" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
							<tr>
								<th>Valor Pago Solicitado</th>
								<th>Valor Pago Efetivado</th>
							</tr>';
			$arrPartido = array();
			foreach ($arRegistro as $key => $pag) {
				ver( $arRegistro,d );
				if( $key == 'partido' ){
					$arrPartido[$key] = $pag['partido'];
				}
				$html .= '<tr>
							<td align="right">'.number_format($pag['valor_pagamento'],2,',','.').'</td>
							<td align="right">'.number_format($pag['valor_pago_efetivado'],2,',','.').'</td>
						</tr>';
			}
			$html .= '</table>';*/
			
			/*$htmlPagamento 	= $html;
			$pagamentoExcel	= implode(', ', $excP);*/
					
			return array("sql" => $arRegistro, "excel" => $arRegistro);
		} else {
			die("<script>alert('Colunas não selecionadas');</script>");
		}
	}	
	$arRegistro = array();
	$arExcel	= array();
	if( $arDados ){
		$arDadosArray = array();
		foreach ($arDados as $key => $value) {
			
			if( in_array( 'numempenho', $id ) ){
				$html = '';
				$empenhoExcel = '';
				$sql = "select DISTINCT exfnumempenhooriginal, case when ex.semid is not null then se.semdesc else '-' end as situacao 
						from emenda.execucaofinanceira ex 
						INNER JOIN emenda.situacaoempenho se on se.semid = ex.semid and se.semstatus = 'A'
						INNER JOIN emenda.ordembancaria ob ON ob.exfid = ex.exfid
						where ex.ptrid = ".$value['ptrid'];
				$arEmpenho = $db->carregar( $sql );
				$arEmpenho = $arEmpenho ? $arEmpenho : array();								            
				$html = '<table id="table" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
							<tr>
								<th>Nº Empenho</th>
								<th> - Situação</th>
							</tr>';
				if( $arEmpenho ){
					$exc = array();
					foreach ($arEmpenho as $empenho) {
						$html .= '<tr>
									<td>'.$empenho['exfnumempenhooriginal'].'</td>
									<td> - '.$empenho['situacao'].'</td>
								</tr>';
						$exc[] = $empenho['exfnumempenhooriginal'];
					}
					$html .= '</table>';
					
					$htmlEmpenho = $html;
					$empenhoExcel = implode(', ', $exc);
				} else {
					$htmlEmpenho = '-';					
					$empenhoExcel = '-';
				}
			}

			if( in_array( 'valor_pagamento', $id ) ){
				$pagamentoExcel = '';
				$valorS = 0;
				$valorE = 0;
				$sql = "SELECT numempenho, sum(valor_pago_solicitado) as valor_pago_solicitado, sum(valor_pago_efetivado) as valor_pago_efetivado
						FROM (
						SELECT DISTINCT 
							exfnumempenhooriginal as numempenho,
							sum(COALESCE(ob.orbvalorparcela,0)) as valor_pago_solicitado, 
							CASE WHEN ob.spgcodigo = '2' THEN sum(COALESCE(ob.orbvalorparcela,0)) ELSE 0 END as valor_pago_efetivado
						FROM
							emenda.execucaofinanceira ex 
						INNER JOIN emenda.ordembancaria ob ON ob.exfid = ex.exfid
						WHERE
							ex.ptrid = ".$value['ptrid']." AND
							ex.exfstatus = 'A'
						GROUP BY
							ex.ptrid, numempenho, ob.spgcodigo
						) as foo
						GROUP BY
							numempenho";
				$arEmpenho = $db->carregar( $sql );
				$arEmpenho = $arEmpenho ? $arEmpenho : array();								            
				$html = '<table id="table" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
							<tr>
								<th>Nº Empenho</th>
								<th> - Valor Pago Solicitado</th>
								<th> - Valor Pago Efetivado</th>
							</tr>';
				if( $arEmpenho ){
					$excP = array();
					foreach ($arEmpenho as $empenho) {
						$html .= '<tr>
									<td>'.$empenho['numempenho'].'</td>
									<td align="right">'.number_format($empenho['valor_pago_solicitado'],2,',','.').'</td>
									<td align="right">'.number_format($empenho['valor_pago_efetivado'],2,',','.').'</td>
								</tr>';
						$excP[] = 'Solicitado: ' . $empenho['valor_pago_solicitado'] . ' - Efetivado: ' . $empenho['valor_pago_efetivado'];
						$valorS = $valorS + $empenho['valor_pago_solicitado'];
						$valorE = $valorE + $empenho['valor_pago_efetivado'];
					}
					$html .= '<tr>
									<td><b>Total:</b></td>
									<td align="right">'.number_format($valorS,2,',','.').'</th>
									<td align="right">'.number_format($valorE,2,',','.').'</th>
								</tr>
							</table>';
					
					$htmlPagamento 	= $html;
					$pagamentoExcel	= implode(', ', $excP);
				} else {
					$htmlPagamento  = '-';					
					$pagamentoExcel = '-';
					$psExcel 	    = '-';
					$peExcel 	    = '-';
				}
			}
			$arrPartidoH = '-';
			$arrPartidoE = '-';
			if( in_array( 'partido', $id ) ){
				$sql = "select p.parsigla||' - '|| p.parnome as partido from emenda.ptemendadetalheentidade pte
							inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid and ve.edestatus = 'A'
						    inner join emenda.autor a on a.autid = ve.autid and a.autstatus = 'A'
						    inner join emenda.partido p on p.parid = a.parid and p.parstatus = 'A'
						 where pte.ptrid = {$value['ptrid']}";
						 
				$arrPartido = $db->carregarColuna( $sql );
				$arrPartido = $arrPartido ? $arrPartido : array();
				$arrPartidoH = implode( '<br>', $arrPartido );
				$arrPartidoE = implode( ', ', $arrPartido );
			}

			$xx += isset($xx) ? 1 : 0;
			$boRegistro = false;
			
			foreach ($id as $key => $c) {
				if( $c == 'numprocesso' && $value[$c] != '-' ) {
					$arRegistro[$xx][$c] = formataProcesso( $value[$c] );
					$arExcel[$xx][$c] = formataProcesso( $value[$c] );
				} else {
					$arRegistro[$xx][$c] = $value[$c];
					$arExcel[$xx][$c] = $value[$c];
				}
				if( !empty($value[$c]) ){
					$boRegistro = true;
				}
			}
			
			if( in_array('numempenho', $id) && !empty($htmlEmpenho) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['numempenho'] = $htmlEmpenho;
				$arExcel[$xx]['numempenho'] = $empenhoExcel;
				$htmlEmpenho = '';
			}

			if( in_array('valor_pagamento', $id) && !empty($htmlPagamento) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['valor_pagamento'] = $htmlPagamento;
				$arExcel[$xx]['valor_pagamento'] = $pagamentoExcel;
				$htmlPagamento = '';
			}
			
			if( in_array('partido', $id) && !empty($arrPartido) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['partido'] = $arrPartidoH;
				$arExcel[$xx]['partido'] = $arrPartidoE;
				$htmlPagamento = '';
			}
			
			if( !$boRegistro ){
				unset( $arRegistro[$xx] );
				sort($arRegistro);
			}
		}// fim do foreach
	}

	return array("sql" => $arRegistro, 
				 "excel" => $arExcel);
}

function formataProcesso( $numero ){
	if( !empty( $numero ) ) $processo = substr($numero,0,5) . ".".substr($numero,5,6)."/".substr($numero,11,4) . "-".substr($numero,15,2);
	return $processo;
}

function monta_coluna(){

	$cols = $_SESSION['emenda']['post']['coluna'] ? $_SESSION['emenda']['post']['coluna'] : array();
	
	$coluna = array("coluna" => array() );

	if( is_array($cols) ){
		for($i=0; $i<sizeof($cols); $i++){
			
			switch ($cols[$i]) {
				case 'estado':
					array_push($coluna['coluna'], array(
														"campo" => "enb.estuf as estado",
														"id" => "estado",
												  		"label" => "UF da Entidade")										
										   				);
				break;
				case 'ptrcod':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptrcod",
														"id" => "ptrcod",
												  		"label" => "PTA")
										   				);
				break;
				case 'ptrid':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptrid",
														"id" => "ptrid",
												  		"label" => "PTRID")
										   				);
				break;
				case 'numprocesso':
					array_push($coluna['coluna'], array(
														"campo" => "COALESCE(ptr.ptrnumprocessoempenho,'-') as numprocesso",
														"id" => "numprocesso",
												  		"label" => "Nº do Processo")
										   				);
				break;
				case 'numempenho':
					array_push($coluna['coluna'], array(
														"campo" => "'' as numempenho",
														"id" => "numempenho",
												  		"label" => "Nº do Empenho")
										   				);
				break;
				case 'mundescricao':
					array_push($coluna['coluna'], array(
														"campo" => "(select mun.mundescricao from territorios.municipio mun WHERE mun.muncod = enb.muncod) as mundescricao",
														"id" => "mundescricao",
												  		"label" => "Município da Entidade")
										   				);
				break;
				case 'numsiafi':
					array_push($coluna['coluna'], array(
														"campo" => "COALESCE(ptmi.pmcnumconveniosiafi,'-') as numsiafi",
														"id" => "numsiafi",
												  		"label" => "Nº do Convênio SIAFI")										
										   				);
				break;
				case 'numfnde':
					array_push($coluna['coluna'], array(
														"campo" => "COALESCE(ptr.ptrnumconvenio,'-') as numfnde",
														"id" => "numfnde",
												  		"label" => "Nº do Convênio FNDE")
										   				);
				break;
				case 'valor_do_projeto':
					array_push($coluna['coluna'], array(
														"campo" => "COALESCE(pti.ptivalortotal,0) as valor_do_projeto",
														"id" => "valor_do_projeto",
												  		"label" => "Valor do Projeto")
										   				);
				break;
				case 'valor_empenhado':
					array_push($coluna['coluna'], array(
														"campo" => "sum(COALESCE(exf.exfvalor,0)) as valor_empenhado",
														"id" => "valor_empenhado",
												  		"label" => "Valor Empenhado")
										   				);
				break;
				case 'valor_pagamento':
					array_push($coluna['coluna'], array(
														"campo" => "sum(COALESCE(ob.orbvalorparcela,0)) as valor_pagamento",
														"id" => "valor_pagamento",
												  		"label" => "Detalhamento do Pagamento")
										   				);
				break;
				case 'partido':
					array_push($coluna['coluna'], array(
														"campo" => "'' as partido",
														"id" => "partido",
												  		"label" => "Partido")
										   				);
				break;
				/*
				case 'valor_pago_solicitado':
					array_push($coluna['coluna'], array(
														"campo" => "sum(COALESCE(ob.orbnumparcela*ob.orbvalorparcela,0)) as valor_pago_solicitado",
														"id" => "valor_pago_solicitado",
												  		"label" => "Valor Pago Solicitado")
										   				);
				break;
				case 'valor_pago_efetivado':
					array_push($coluna['coluna'], array(
														"campo" => "CASE WHEN ob.spgcodigo = '2' THEN sum(COALESCE(ob.orbnumparcela*ob.orbvalorparcela,0)) ELSE 0 END as valor_pago_efetivado",
														"id" => "valor_pago_efetivado",
												  		"label" => "Valor Pago Efetivado")
										   				);
				break;
				*/
			}		
		}
	}
	return $coluna['coluna'];			  	
}
?>