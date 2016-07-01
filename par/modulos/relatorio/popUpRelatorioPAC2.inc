<?php
set_time_limit(0);
ini_set("memory_limit", "1024M");
montaBotaoForm();
//include APPRAIZ. 'includes/classes/relatorio.class.inc';

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

function dadosPagamento($dados) {
	global $db;
	if($dados['preid']){
		$sql = "SELECT e.empnumero, pa.pagparcela, po.pobpercentualpag, po.pobvalorpagamento FROM par.processoobra p
				INNER JOIN par.empenho e ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso) and empstatus = 'A'
				INNER JOIN par.pagamento pa ON pa.empid = e.empid
				INNER JOIN par.pagamentoobra po ON po.pagid = pa.pagid
				WHERE p.prostatus = 'A' and po.preid = {$dados['preid']} AND pa.pagstatus = 'A' ORDER BY pa.pagparcela ";
		
		$lista = $db->carregar($sql);
		if(is_array($lista)){
		$html = '';
		$totalParcela = 0;
		$totalPercentual = 0;
		$totalValor = 0;
		
		 $html .= "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		 $html .= "<tr>		<td class=SubTituloCentro>Nº Empenho</td>
			 				<td class=SubTituloCentro>Nº Parcela</td>
			 				<td class=SubTituloCentro>%</td>
			 				<td class=SubTituloCentro>Valor Pago</td>
			 			</tr>";
		 foreach($lista as $dado){
			 $html .= "<tr>	<td class=SubTituloCentro>".$dado['empnumero']."</td>
			 				<td class=SubTituloCentro>".$dado['pagparcela']."</td>
			 				<td class=SubTituloCentro>".$dado['pobpercentualpag']."</td>
			 				<td class=SubTituloCentro>".number_format($dado['pobvalorpagamento'], 2, ',', '.')."</td>
			 			</tr>";
			 $totalParcela = $totalParcela + 1;
			 $totalPercentual = $totalPercentual + $dado['pobpercentualpag'];
			 $totalValor = $totalValor + $dado['pobvalorpagamento'];
		 }
		 $html .= "<tr>	
		 					<td class=SubTituloCentro>Total:</td>
			 				<td class=SubTituloCentro>".$totalParcela."</td>
			 				<td class=SubTituloCentro>".$totalPercentual."</td>
			 				<td class=SubTituloCentro>".number_format($totalValor, 2, ',', '.')."</td>
			 			</tr>
		 		
		 
		 		</table>";
		return $html;
		}else{
			return 'Obra sem pagamento';
		}
	}else{
		return 'Obra sem pagamento';
	}
	//$cabecalho = array("&nbsp;","&nbsp;","Parcela","Mês","Ano","Valor(R$)","Usuário criação","Situação pagamento","Exercício");
	
	//$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

}


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
	//$id[] = $c['dadospagamento'];
}

$arSql   = monta_sql($coluna, $id);
sort($arSql['sql']);

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	sort($arSql['excel']);
	$db->sql_to_excel($arSql['excel'], 'relatorioPAC2', $cabecalho, $formato);
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

function montaJoinColunas( $arColunas, $arFiltros ){
	$arJoin = array();
	$arCols = array();
	
	//foreach( $arColunas as $stColuna ){
		/*if( in_array( 'responsavel', $arColunas ) || in_array( 'responsavel', $arFiltros ) ){
			(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "left join emenda.responsavel res on res.resid = eme.resid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
		}*/
		/*if( in_array( 'tpeid', $arColunas ) || in_array( 'tpeid', $arFiltros ) ){
			(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "left join emenda.responsavel res on res.resid = eme.resid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			$join = "left join emenda.tipoensino tpe on tpe.resid = res.resid and tpe.tpestatus = 'A'";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			
		} */
		/*if( in_array( 'entidade', $arColunas) || in_array( 'valorentidade', $arColunas) || in_array( 'usucpf', $arColunas) || in_array( 'entidade', $arFiltros) || 
				in_array( 'estufentidade', $arFiltros) || in_array( 'mundescricao', $arColunas) ){
			( in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			$join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
					 left join territorios.municipio mun on mun.muncod = ent.muncod";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			
		}*/
	/*ver($arColunas,d);
		if( in_array( 'emeano', $arColunas ) || in_array( 'autnome', $arColunas ) ){
			//(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
				    left join emenda.autor aut 
				    	inner join emenda.tipoautor tpa on tpa.tpaid = aut.tpaid
				    on aut.autid = vede.autid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
		}*/
		$arJoinPTA = array();		
		if ( in_array( 'ptrcod', $arColunas) || in_array( 'ptivalortotal', $arColunas) || in_array( 'pmcnumconveniosiafi', $arColunas) || in_array( 'ptrnumconvenio', $arColunas) /*|| 
					in_array( 'esddsc', $arColunas)  || in_array( 'exfvalor', $arColunas) || 
					in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'esddsc', $arFiltros) || in_array( 'ininome', $arColunas) ||
					in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) || in_array( 'ptrnumconvenio', $arFiltros) ||
					in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas ) || in_array( 'iniid', $arFiltros ) || 
					in_array( 'unisigla', $arColunas ) || in_array( 'ptrnumprocessoempenho', $arFiltros ) || in_array( 'ptrnumprocessoempenho', $arColunas ) 
					|| in_array( 'pedvalor', $arFiltros )*/ ){
			
			(in_array( "(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcodexer", $arCols ) ? '' : $arCols[] = "(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcodexer");
			//(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			//(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
			//(in_array( 'ptr.ptrid', $arCols ) ? '' : $arCols[] = 'ptr.ptrid');
			
			/*if( in_array( 'ininome', $arColunas) || in_array( 'iniid', $arFiltros ) ){
				$join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.iniciativa ini on ini.iniid = pti.iniid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);				
			}*/
			
			/*if( in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) ){				
				$join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.iniciativaespecificacao ine on ine.iceid = ptie.iceid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.especificacao esp on esp.espid = ine.espid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}*/
			
			/*if( in_array( 'pmcnumconveniosiafi', $arColunas) || in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'datareferencia', $arColunas ) ){
				$join = "left join emenda.ptminutaconvenio ptmi on ptmi.ptrid = ptr.ptrid and ptmi.pmcstatus = 'A'
						 left join emenda.ptpublicacao pt on pt.pmcid = ptmi.pmcid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			
			if( in_array( 'esddsc', $arColunas) || in_array( 'esddsc', $arFiltros) ){
				$join = "left join workflow.documento doc on ptr.docid = doc.docid 
						  left join workflow.estadodocumento esd on esd.esdid = doc.esdid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}*/
			
			/*
			if( in_array( 'pmcnumconveniosiafi', $arColunas) ){
				$join = "left join emenda.dadosconveniosiafi dcs on substring(dcs.it_nu_original from 1 for 6) = ptr.ptrnumconvenio";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			*/
			
			if( in_array( 'empenhado', $arFiltros) || in_array( 'pagamento', $arFiltros) ){
				$join = "left join emenda.execucaofinanceira ef on ptr.ptrid = ef.ptrid and pte.pedid = ef.pedid and ef.exfstatus = 'A'
						left join emenda.ordembancaria ord on ord.exfid = ef.exfid ";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			if( in_array( 'ptivalortotal', $arColunas) ){
				$join = "left join
							(SELECT ptrid, sum(ptivalortotal) as ptivalortotal
							 FROM emenda.v_ptiniciativa 
							 GROUP BY ptrid) vpti ON vpti.ptrid = ptr.ptrid";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			/*if( in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas) ){
				$join = "left join emenda.ptvigencia ptv ON ptv.ptrid = ptr.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}*/
			
			/*$join = "left join emenda.ptemendadetalheentidade pede
					  	inner join emenda.planotrabalho ptr on ptr.ptrid = pede.ptrid and ptr.ptrstatus = 'A' and ptr.ptrexercicio = ".$_SESSION['exercicio']."
					  	".implode(' ', $arJoinPTA)."
					  on ede.edeid = pede.edeid";
			
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			$join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
					 left join territorios.municipio mun on mun.muncod = ent.muncod";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);*/
			
			//ver($arColunas,$arJoin,d);
		}
		/*if( in_array( 'obcdsc', $arColunas) ){
			(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
		}
		if( in_array( 'iniid', $arColunas) || in_array( 'iniid', $arFiltros) ){
			(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
			
			if( in_array( 'iniid', $arColunas) ){
				$join = "left join emenda.iniciativadetalheentidade ide
			            	inner join emenda.iniciativa ini1 on ini1.iniid = ide.iniid
			            on ide.edeid = ede.edeid";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			
		}*/
	//}	
	return array("colunas" => $arCols,
				 "joins" => $arJoin);
}

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
	extract($_SESSION['emenda']['post']);
		
	$arColunas = array_merge( $cols, $id );
	$arJoinColunas = montaJoinColunas( $arColunas, $arFiltros );
	$arJoinColunas['colunas'] = array_merge( $arJoinColunas['colunas'], $cols );	
	$stColunas = (is_array($arJoinColunas['colunas']) && $arJoinColunas['colunas'][0]) ? ', '.implode(', ', $arJoinColunas['colunas']) : "*";

	if($stColunas) {
//		$sql = "SELECT DISTINCT 'Municipal' as esfera, pre.preid 
//					$stColunas
//				FROM  par.processoobra pro
//				INNER JOIN par.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso
//				INNER JOIN par.empenhoobra eo ON eo.empid = emp.empid and eobstatus = 'A'   
//				LEFT JOIN par.pagamento pag on pag.empid = emp.empid
//				LEFT JOIN obras.preobra pre ON pre.preid = eo.preid
//				LEFT JOIN obr as.o brainfraestrutura obr on obr.obrid = pre.obrid
//				inner JOIN territorios.municipio mun ON pre.muncod = mun.muncod
//				LEFT JOIN territorios.estado est ON est.estuf = pro.estuf
//				WHERE pro.estuf IS NULL
//				union all
//				SELECT DISTINCT 'Estadual' as esfera, pre.preid 
//					$stColunas
//				FROM  par.processoobra pro
//				INNER JOIN par.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso
//				INNER JOIN par.empenhoobra eo ON eo.empid = emp.empid and eobstatus = 'A'   
//				LEFT JOIN par.pagamento pag on pag.empid = emp.empid
//				LEFT JOIN obras.preobra pre ON pre.preid = eo.preid
//				LEFT JOIN obr as.o brainfraestrutura obr on obr.obrid = pre.obrid
//				left JOIN territorios.municipio mun ON pre.muncod = mun.muncod
//				LEFT JOIN territorios.estado est ON est.estuf = pro.estuf
//				WHERE pro.estuf IS not NULL";
		$sql = "SELECT DISTINCT 'Municipal' as esfera, pre.preid 
					$stColunas
				FROM  par.processoobra pro
				INNER JOIN par.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso and empstatus = 'A'
				INNER JOIN par.empenhoobra eo ON eo.empid = emp.empid and eobstatus = 'A'   
				LEFT JOIN par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A'
				LEFT JOIN obras.preobra pre ON pre.preid = eo.preid
				LEFT JOIN obras2.obras obr on obr.obrid = pre.obrid
				inner JOIN territorios.municipio mun ON pre.muncod = mun.muncod
				LEFT JOIN territorios.estado est ON est.estuf = pro.estuf
				WHERE pro.estuf IS NULL
				and pro.prostatus = 'A'
				union all
				SELECT DISTINCT 'Estadual' as esfera, pre.preid 
					$stColunas
				FROM  par.processoobra pro
				INNER JOIN par.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso and empstatus = 'A'
				INNER JOIN par.empenhoobra eo ON eo.empid = emp.empid and eobstatus = 'A'   
				LEFT JOIN par.pagamento pag on pag.empid = emp.empid AND pag.pagstatus = 'A'
				LEFT JOIN obras.preobra pre ON pre.preid = eo.preid
				LEFT JOIN obras2.obras obr on obr.obrid = pre.obrid
				left JOIN territorios.municipio mun ON pre.muncod = mun.muncod
				LEFT JOIN territorios.estado est ON est.estuf = pro.estuf
				WHERE pro.estuf IS not NULL
				and pro.prostatus = 'A'
				";
		
		//ver($sql);	    
		$arDados = $db->carregar( $sql );
	} else {
		die("<script>alert('Colunas não selecionadas');</script>");
	}	
	$arRegistro = array();
	$arExcel	= array();
	if( $arDados ){
		$arDadosArray = array();
		
		foreach ($arDados as $key => $value) {
		
			$xx += isset($xx) ? 1 : 0;
			$boRegistro = false;
			
			foreach ($id as $key => $c) {
				if( $c == 'cnpj' ){ 
					$arRegistro[$xx][$c] = formatar_cpf_cnpj( $value[$c] );
					$arExcel[$xx][$c] = formatar_cpf_cnpj( $value[$c] );
				} elseif( $c == 'dadospagamento') {
					$dadosPagamento = dadosPagamento($value);
					$arRegistro[$xx][$c] = $dadosPagamento;
					$arExcel[$xx][$c] = $dadosPagamento;
				}else {
					$arRegistro[$xx][$c] = $value[$c];
					$arExcel[$xx][$c] = $value[$c];
				}
				if( !empty($value[$c]) ){
					$boRegistro = true;
				}
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
			
//		}	
//		foreach ($cols as $val) {
			switch ($cols[$i]) {
				case 'predescricao':
					array_push($coluna['coluna'], array(
														"campo" => "pre.predescricao",
														"id" => "predescricao",
												  		"label" => "Nome da obra")									
										   				);
				break;
				case 'estuf':
					array_push($coluna['coluna'], array(
														"campo" => "mun.estuf",
														"id" => "estuf",
												  		"label" => "Estado")
										   				);
				break;
				case 'muncod':
					array_push($coluna['coluna'], array(
														"campo" => "mun.mundescricao as muncod",
														"id" => "muncod",
												  		"label" => "Município")										
										   				);
				break;
				case 'empnumero':
					array_push($coluna['coluna'], array(
														"campo" => "emp.empnumero",
														"id" => "empnumero",
												  		"label" => "Número do Empenho")										
										   				);
				break;
				case 'empvalorempenho':
					array_push($coluna['coluna'], array(
														"campo" => "(SELECT COALESCE(sum(vve.vrlempenhocancelado),0.00) FROM par.empenho ep 
																			inner join par.v_vrlempenhocancelado vve on vve.empid = ep.empid and empstatus = 'A'
																		WHERE ep.empnumeroprocesso=pro.pronumeroprocesso) as empvalorempenho",
														"id" => "empvalorempenho",
												  		"label" => "Valor do Empenho")										
										   				);
				break;
				case 'pagdatapagamento':
					array_push($coluna['coluna'], array(
														"campo" => "to_char(pag.pagdatapagamento, 'DD/MM/YYYY') as pagdatapagamento",
														"id" => "pagdatapagamento",
												  		"label" => "Data do Pagamento")										
										   				);
				break;
				case 'pagvalorparcela':
					array_push($coluna['coluna'], array(
														"campo" => "(SELECT COALESCE(sum(pg.pagvalorparcela),0.00) FROM par.pagamento pg 
																      INNER JOIN par.empenho em ON em.empid=pg.empid and empstatus = 'A' 
																      WHERE em.empnumeroprocesso=pro.pronumeroprocesso AND pg.pagstatus='A') as pagvalorparcela",
														"id" => "pagvalorparcela",
												  		"label" => "Valor do Pagamento")										
										   				);
				break;
				case 'esfera':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "esfera",
												  		"label" => "Esfera")										
										   				);
				break;
				case 'dadospagamento':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "dadospagamento",
												  		"label" => "Detalhe Pagamento")										
										   				);
				break;
				case 'obrid':
					array_push($coluna['coluna'], array(
														"campo" => "obr.obrid",
														"id" => "obrid",
												  		"label" => "ID da Obra")										
										   				);
				break;
				case 'obrpercexec':
					array_push($coluna['coluna'], array(
														"campo" => "obr.obrpercexec",
														"id" => "obrpercexec",
												  		"label" => "% Executado")										
										   				);
				break;
			}		
		}
	}
	return $coluna['coluna'];			  	
}
?>