<?php
ini_set("memory_limit", "2048M");

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}


$isXls = ($_POST['req'] == 2) ? true :false;


if($isXls)
{
	$file_name="RelatorioGeralEiManutencao". date('d_m_Y_G_i_s') .".xls";
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Content-Type: application/force-download");
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Disposition: attachment;filename=".$file_name);
	header("Content-Transfer-Encoding: binary ");
}

if( ! $isXls )
{
	
	?>
	<html>
		<head>
			<script type="text/javascript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
}
//include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();


/*
$dados = $db->carregar($sql);

$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao($true ? true : false);
$r->setTotNivel(true);
$r->setEspandir($_POST['expandir']);

if( $_POST['req'] == 2 ){
	ob_clean();
	$nomeDoArquivoXls="Relatório_Geral_".$prgid."_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
} else {
	echo $r->getRelatorio();
}
*/

function monta_sql(){
	
	$agrupador = $_POST['agrupador'];
	
	
	extract($_POST);
	$arWhere = Array("obr.obrstatus = 'A'", "ent.entstatus = 'A'", " obr.obrpercentultvistoria >= 90", " pf.prfid = 41 ", "esd1.esdid IN (690, 693)", "obr.obridpai IS NULL", "tpl.tpoid in (16,9,10)" );
	
	// UF
	if($estuf[0] && $estuf_campo_flag ){
		$arWhere[] = " ede.estuf ". (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $estuf )."' ) ";
	}
	
	// Municipio
	if( $muncod[0] && $muncod_campo_flag ){
		$arWhere[] = " mun.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $muncod )."' ) ";
	}

	//Tipologia
	if($tpoid[0]){
		$arWhere[] = " tpl.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpoid ) . "')";
	}	
	
	// Situação do Plano
	if( $esdid[0] ){
		$arWhere[] = " esd.esdid ". (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $esdid )."' ) ";
	}

	//Situação da Obra
	if($stoid[0] ){
		$arWhere[] = " esd1.esdid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $stoid ) . "')";
	}	
		
	// Lote
	if( $lotid ){
		$innerLote = "INNER JOIN proinfantil.loteproinfancia lot ON lot.lotid = pin.lotid";
		$arWhere[] = " lot.lotid = $lotid ";
// 		$arWhere[] = " lot.lotid ". (!$lotid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $lotid )."' ) ";
	}

	//Ano de Inicio do atendimento
	if($anoini){
		$arWhere[] = "to_char(to_date(res.resdsc,'DD/MM/YYYY'),'YYYY') = '{$anoini}'";
	}
	
	//Ano do Pagamento
	if($anopagamento){
		$arWhere[] 			= "pag.anopagamento = '{$anopagamento}'";
		$join_pagamento 	= "LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = ".AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO." 
									ORDER BY		
										hst.htddata DESC
									) as pag ON pag.pinid = pin.pinid";	
	} else {
		$join_pagamento = "";
	}
	
	//Monta Colunas lentas
	if( in_array('total_repassado',$_REQUEST['coluna']) ){
		$campo_total_repassado = "CASE
									WHEN qtd_pagamento = 1 THEN vlr.valorintcre+vlr.valorintpre+vlr.valorparcre+vlr.valorparpre
									WHEN qtd_pagamento = 2 THEN vlr.valorintcre+vlr.valorintpre+vlr.valorparcre+vlr.valorparpre+
												    			vlr.valorintcre2+vlr.valorintpre2+vlr.valorparcre2+vlr.valorparpre2
									ELSE 0
								  END as total_repassado";
		$inner_total_repassado = "LEFT  JOIN
									(
									SELECT
										count(hst.hstid) as qtd_pagamento,
										doc.docid
									FROM
										workflow.historicodocumento hst
									INNER JOIN workflow.documento doc ON doc.docid = hst.docid AND tpdid = ".WF_PROINFANTIL."
									WHERE
										aedid = ".AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO."
									GROUP BY
										doc.docid
									) as qpg ON qpg.docid = pin.docid";
		
	}
	
	$isInnerFormulario = false;
	$isInnerPrefeito = false;
	
	$arrayCamposVal = array(
		'mncintcre', 
		'mncintpre',
		'mncparcre',
		'mncparpre',
		'totalintcre', 
		'totalintpre',
		'totalparcre',
		'totalparpre',
		'valorintcre',
		'valorintcre2',
		'valorintpre',
		'valorintpre2',
		'valorparcre',
		'valorparcre2',
		'valorparpre',
		'valorparpre2',
		'data_inicio_atendimento',
		'valor_analise1',
		'valor_analise2',
		'total_repassado',
		'total_repasse'
	);
	
	$arrayCamposValSql = array(
			'mncintcre' 				=> array('label' => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Creche", 'sql' => 'vlr.mncintcre'),
			'mncintpre' 				=> array('label' => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Pré-Escola", 'sql' => 'vlr.mncintpre'),
			'mncparcre' 				=> array('label' => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Creche", 'sql' => 'vlr.mncparcre'),
			'mncparpre' 				=> array('label' => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Pré-Escola", 'sql' => 'vlr.mncparpre'),
			'totalintcre' 				=> array('label' => "Total de matrículas na escola – Tempo Integral / Creche", 'sql' => 'vlr.totalintcre'),
			'totalintpre' 				=> array('label' => "Total de matrículas na escola – Tempo Integral / Pré-Escola", 'sql' => 'vlr.totalintpre'),
			'totalparcre' 				=> array('label' => "Total de matrículas na escola – Tempo Parcial / Creche", 'sql' => 'vlr.totalparcre'),
			'totalparpre' 				=> array('label' => "Total de matrículas na escola – Tempo Parcial / Pré-Escola", 'sql' => 'vlr.totalparpre'),
			'valorintcre' 				=> array('label' => "Valor de repasse – Tempo Integral / Creche - Análise 1", 'sql' => 'vlr.valorintcre'),
			'valorintcre2' 				=> array('label' => "Valor de repasse – Tempo Integral / Creche - Análise 2", 'sql' => 'vlr.valorintcre2'),
			'valorintpre' 				=> array('label' => "Valor de repasse  – Tempo Integral / Pré-Escola - Análise 1", 'sql' => 'vlr.valorintpre'),
			'valorintpre2' 				=> array('label' => "Valor de repasse  – Tempo Integral / Pré-Escola - Análise 2", 'sql' => 'vlr.valorintpre2'),
			'valorparcre' 				=> array('label' => "Valor de repasse  – Tempo Parcial / Creche - Análise 1", 'sql' => 'vlr.valorparcre'),
			'valorparcre2' 				=> array('label' => "Valor de repasse  – Tempo Parcial / Creche - Análise 2", 'sql' => 'vlr.valorparcre2'),
			'valorparpre' 				=> array('label' => "Valor de repasse – Tempo Parcial / Pré-Escola - Análise 1", 'sql' => 'vlr.valorparpre'),
			'valorparpre2' 				=> array('label' => "Valor de repasse – Tempo Parcial / Pré-Escola - Análise 2", 'sql' => 'vlr.valorparpre2'),
			'data_inicio_atendimento' 	=> array('label' => "data_inicio_atendimento", 'sql' => "to_char(to_date(res.resdsc,'DD/MM/YYYY'),'DD/MM/YYYY') as data_inicio_atendimento"),
			'valor_analise1' 			=> array('label' => "valor_analise1", 'sql' => 'vlr.valorintcre as valor_analise1'),
			'valor_analise2' 			=> array('label' => "valor_analise2", 'sql' => 'vlr.valorintcre2 as valor_analise2'),
			'total_repassado' 			=> array('label' => "Valor Repassado", 'sql' => "{$campo_total_repassado}"),
			'total_repasse'				=> array('label' => "Valor Total de Repasse", 'sql' => 'vlr.valorintcre+vlr.valorintpre+vlr.valorparcre+vlr.valorparpre+
							   				vlr.valorintcre2+vlr.valorintpre2+vlr.valorparcre2+vlr.valorparpre2 as total_repasse')
	);
	
	
	
	$arrCamposPrefeito = 
		array (
			'prefeito', 
			'email_prefeito', 
			'telefone_prefeito'
		);


	
	$arrCamposPrefeitoSql = 
		array (
			'prefeito' 			=> array('label' => "Nome Prefeito", 'sql' => "pre.entnome as prefeito"), 
			'email_prefeito' 	=> array('label' => "E-mail Prefeito", 'sql' =>  "pre.entemail as email_prefeito"), 
			'telefone_prefeito' =>array('label' => "Telefone Prefeito", 'sql' =>  "'(' || pre.entnumdddcomercial || ')' || pre.entnumcomercial as telefone_prefeito")
		);
	
	$arrDemaisCamposSql = 
		
		array (
				'qtd_obra' 			=>array('label' => "Qtd. de Obras", 'sql' => "1 as qtd_obra"),
				'cnpj_prefeitura' 	=>array('label' => "CNPJ", 'sql' => "ent.entnumcpfcnpj as cnpj_prefeitura"),
				'obra' 				=>array('label' => "Nome da Obra", 'sql' => "obr.obrnome as obra"),
				'tipologia_obra' 	=>array('label' => "Tipologia", 'sql' => "tpl.tpodsc as tipologia_obra"),
				'percentual_obra' 	=>array('label' => "Percentual Executado (%)", 'sql' => "obr.obrpercentultvistoria as percentual_obra"),
				'estado_obra' 		=>array('label' => "Situação da Obra", 'sql' => "esd1.esddsc as estado_obra"),
				'municipio' 		=>array('label' => "Município", 'sql' => "mun.mundescricao as municipio"),
				'codigo_municipio' 	=>array('label' => "Código do Município", 'sql' => "mun.muncod as codigo_municipio"),
				'estado' 			=>array('label' => "Estado", 'sql' => "ede.estuf as estado"),
				'id_obra' 			=>array('label' => "ID da Obra", 'sql' => "obr.obrid as id_obra"),
				'estado_plano'		=>array('label' => "Situação do Plano", 'sql' => "esd.esddsc as estado_plano")
		);
	
	if( $_POST['anoini'] ) 
	{
		$isInnerFormulario = true;
	}
	
	
	$cabecalho = array();
	
	$arrayCamposSemGroupBy = array(
			'mncintcre',
			'mncintpre',
			'mncparcre',
			'mncparpre',
			'totalintcre',
			'totalintpre',
			'totalparcre',
			'totalparpre',
			'valorintcre',
			'valorintcre2',
			'valorintpre',
			'valorintpre2',
			'valorparcre',
			'valorparcre2',
			'valorparpre',
			'valorparpre2',
			'data_inicio_atendimento',
			'valor_analise1',
			'valor_analise2',
			'total_repassado',
			'total_repasse',
			'qtd_obra',
			'obra',
			'tipologia_obra',
			'percentual_obra',
			'estado_obra',
			'id_obra',
			'estado_plano'
	
	);
	
	
	
	$isGroupByDadosUnidade = true;
	foreach( $_POST['coluna']  as $k => $coluna )
	{
		
		if(in_array($coluna, $arrayCamposSemGroupBy))
		{
			$isGroupByDadosUnidade = false;
		}
		
		if( array_key_exists(  $coluna ,  $arrayCamposValSql ))
		{
			$arrSqlCampos[] .= " {$arrayCamposValSql[$coluna]['sql']}
			";
			$cabecalho[] = $arrayCamposValSql[$coluna]['label'];
		}
		if(array_key_exists(  $coluna ,  $arrCamposPrefeitoSql ))
		{
			$arrSqlCampos[] .= " {$arrCamposPrefeitoSql[$coluna]['sql']}
			";	
			$cabecalho[] = $arrCamposPrefeitoSql[$coluna]['label'];
		}
		if(	array_key_exists( $coluna ,  $arrDemaisCamposSql ))
		{
			$arrSqlCampos[] .= " {$arrDemaisCamposSql[$coluna]['sql']}
			";	
			$cabecalho[] = $arrDemaisCamposSql[$coluna]['label'];
		}
	}
	
	
	foreach( $arrayCamposVal as $k => $v )
	{
		if ( in_array($v , $_POST['coluna']) )
		{
			$isInnerFormulario = true;
			
		}
	}
	
	foreach( $arrCamposPrefeito as $k => $v )
	{
		if ( in_array($v , $_POST['coluna']) )
		{
			$isInnerPrefeito = true;
			
		}
	}
	
	
	foreach( $arrDemaisCamposSql as $campo => $value)
	{
		if ( in_array($campo , $_POST['coluna']) )
		{
			$isInnerPrefeito = true;
			
		}
	}
	
	
	$agrupadores = ( is_array($_REQUEST['agrupador']) ) ? $_REQUEST['agrupador'] : Array();
	

	foreach ($agrupadores  as $k => $v )
	{
			if( ! in_array( $v , $_POST['coluna'] ) )
			{
				
				$arrSqlCampos[] .= " {$arrDemaisCamposSql[$v]['sql']}
				";
				$cabecalho[] = $arrDemaisCamposSql[$v]['label'];
			}	
			
			if(in_array($v, $arrayCamposSemGroupBy))
			{
				$isGroupByDadosUnidade = false;
			}
	}
	

	$total = count($arrSqlCampos);
	$i = 0;
	
	$sqlCampos = '';
	
	foreach($arrSqlCampos as $k => $v)
	{
		$i++;
		if( $total == $i )
		{
			$sqlCampos .= "$v";
		}
		else
		{
			$sqlCampos .= "$v,";
		}
	}
	
	$sql = "SELECT 
			{$sqlCampos}";
				
			$sql .= " FROM
				obras2.obras obr
			
			
			LEFT JOIN proinfantil.proinfantil  pin  ON obr.obrid = pin.obrid 
			INNER JOIN obras2.empreendimento e ON e.empid =  obr.empid
			INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'
			INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
			INNER JOIN workflow.documento d1 ON d1.docid = obr.docid
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			LEFT JOIN workflow.documento doc ON doc.docid = pin.docid AND doc.tpdid = ".WF_PROINFANTIL."
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			$inner_total_repassado
			INNER JOIN obras2.tipoobra AS tp ON obr.tobid = tp.tobid
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entid
			$innerLote
			$join_pagamento
			";
			
			
			if( $isInnerPrefeito )
			{
				$sql .= "
					LEFT JOIN entidade.funentassoc fea ON fea.entid = obr.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.fueid = fea.fueid AND fen.funid=2 
					LEFT JOIN entidade.entidade pre ON pre.entid = fen.entid 
				";
			}
			$sql .= "
				
				INNER JOIN obras2.tipologiaobra AS tpl ON obr.tpoid = tpl.tpoid AND tpl.tpostatus = 'A' ";
			
			if( $isInnerFormulario ) 
			{$sql .= "
			LEFT  JOIN 
				(
				SELECT    		
					pin.pinid, 
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse
						ELSE 0
					END) as valorintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse 
						ELSE 0
					END) as valorintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparpre,	    
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorintcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorintpre2,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorparcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorparpre2		    
				FROM
					proinfantil.mdsalunoatendidopbf  mat 
				INNER JOIN proinfantil.valoraluno 	val ON val.timid = mat.timid and vaastatus = 'A'
				INNER JOIN proinfantil.proinfantil 	pin ON pin.pinid = mat.pinid
				INNER JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
				INNER JOIN questionario.resposta 	res ON res.qrpid = que.qrpid
				WHERE
					res.perid = 1587 AND to_date(res.resdsc,'DD/MM/YYYY') BETWEEN vaadatainicial AND vaadatafinal
				GROUP BY
					pin.pinid
				) as 				vlr ON vlr.pinid = pin.pinid
			LEFT JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
			LEFT JOIN questionario.resposta 		res ON res.qrpid = que.qrpid AND res.perid = 1587
			
			";	
			}									 
			$sql .= " WHERE 	   
				" . ( $arWhere[0] ? implode(' AND ', $arWhere) : '' ); 
				
			if($isGroupByDadosUnidade)
			{
				$sql .= " 
				GROUP BY  
				".implode(',', $_REQUEST['coluna']);
			}
			
			$sql .= "
			ORDER BY   
			   	".implode(',', $_REQUEST['agrupador']);	
			

global $db;
//die($sql);
$db->monta_lista($sql,$cabecalho,500000,5,'N','90%','N');

die();
// 	ver($sql,d);

}
/*
function monta_agp(){
	
	global $db;
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
		"agrupador" => array(),
		"agrupadoColuna" => array(
									"id_plano",
									"id_obra",
									"codigo_municipio",
									"obra",
									"percentual_obra",
									"estado_plano",
									"estado_obra",
									"tipologia_obra",
									"data_inicio_atendimento",
									"qtd_obra",
									"cnpj_prefeitura",
									"prefeito",
									"email_prefeito",
									"telefone_prefeito",
									"mncintcre",
									"mncintpre",
									"mncparcre",
									"mncparpre",	
									"totalintcre",
									"totalintpre",
									"totalparcre",
									"totalparpre",
									"valorintcre",
									"valorintpre",
									"valorparcre",
									"valorparpre",
									"valor_analise1",
									"valor_analise2",
									"total_repassado",
									"total_repasse"
	));
	
	if($agrupador){
		foreach ($agrupador as $val): 
			switch ($val) {
				case 'estado':
					array_push($agp['agrupador'], array(
											"campo" => "estado",
									  		"label" => "Estado"));				
			    	continue;
			        break;				
				case 'municipio':
					array_push($agp['agrupador'], array(
											"campo" => "municipio",
									  		"label" => "Município"));				
			    	continue;
			        break;		 
				case 'obra':
					array_push($agp['agrupador'], array(
											"campo" => "obra",
									  		"label" => "Nome da Obra"));				
			    	continue;
			        break;		 
			    case 'tipologia_obra':
					array_push($agp['agrupador'], array(
											"campo" => "tipologia_obra",
									  		"label" => "Tipologia"));				
					continue;
			        break;    
			    case 'id_obra':
					array_push($agp['agrupador'], array(
											"campo" => "id_obra",
									  		"label" => "ID da Obra"));				
					continue;
			        break;    			        
			}
		endforeach;
	}
	return $agp;
}*/
/*
function monta_coluna(){
	$colunas = $_REQUEST['coluna'];
	$coluna = array();
	if($colunas){
		foreach ($colunas as $val): 
			switch ($val) {
			    case 'id_obra':
					array_push($coluna, array(
										"campo" => "id_obra",
								  		"label" => "ID da Obra",
										"type"  => "string"));				
					continue;
			        break;
			    case 'qtd_obra':
					array_push($coluna, array(
										"campo" => "qtd_obra",
								  		"label" => "Qtd. de Obras",
				  						"type"  => "numeric"));					
					continue;
				    break;			        
				case 'estado':
					array_push($coluna, array(
										"campo" => "estado",
								  		"label" => "Estado"));				
					continue;
			        break;
			    case 'municipio':
					array_push($coluna, array(
										"campo" => "municipio",
								  		"label" => "Município"));				
					continue;
				    break;
				case 'cnpj_prefeitura':
					array_push($coluna, array(
										"campo" => "cnpj_prefeitura",
								  		"label" => "CNPJ",
										"type" => "string"));				
					continue;
				    break;
				case 'codigo_municipio':
					array_push($coluna, array(
										"campo" => "codigo_municipio",
								  		"label" => "Código do Município",
										"type"  => "string"));				
					continue;
			        break;
			    case 'prefeito':
					array_push($coluna, array(
										"campo" => "prefeito",
								  		"label" => "Nome Prefeito"));				
					continue;
				    break;
				case 'email_prefeito':
					array_push($coluna, array(
										"campo" => "email_prefeito",
								  		"label" => "E-mail Prefeito"));				
					continue;
			        break;
			    case 'telefone_prefeito':
					array_push($coluna, array(
										"campo" => "telefone_prefeito",
								  		"label" => "Telefone Prefeito"));				
					continue;
			        break;				        				        
			    case 'mncintcre':
					array_push($coluna, array(
										"campo" => "mncintcre",
								  		"label" => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Creche",
										"type"  => "numeric"));				
					continue;
			        break;
			    case 'mncintpre':
					array_push($coluna, array(
										"campo" => "mncintpre",
								  		"label" => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Pré-Escola",
										"type"  => "numeric"));				
					continue;
			        break;
			    case 'mncparcre':
					array_push($coluna, array(
										"campo" => "mncparcre",
										"label" => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Creche",
										"type"  => "numeric"));				
					continue;
			        break;
			    case 'mncparpre':
					array_push($coluna, array(
										"campo" => "mncparpre",
								  		"label" => "Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Pré-Escola",
										"type"  => "numeric"));				
					continue;
			        break;
			    case 'totalintcre':
					array_push($coluna, array(
										"campo" => "totalintcre",
								  		"label" => "Total de matrículas na escola – Tempo Integral / Creche",
										"type"  => "numeric"));				
					continue;
			        break;
			    case 'totalintpre':
					array_push($coluna, array(
										"campo" => "totalintpre",
								  		"label" => "Total de matrículas na escola – Tempo Integral / Pré-Escola",
										"type"  => "numeric"));				
					continue;
			        break;
			    case 'totalparcre':
					array_push($coluna, array(
										"campo" => "totalparcre",
								  		"label" => "Total de matrículas na escola – Tempo Parcial / Creche",
										"type"  => "numeric"));				
					continue;
			        break;
			    case 'totalparpre':
					array_push($coluna, array(
										"campo" => "totalparpre",
								  		"label" => "Total de matrículas na escola – Tempo Parcial / Pré-Escola"	,
										"type"  => "numeric"));				
					continue;
				    break;

			    case 'valorintcre':
					array_push($coluna, array(
										"campo" => "valorintcre",
								  		"label" => "Valor de repasse – Tempo Integral / Creche"	,
										"type"  => "float"));				
						continue;
				        break;				    
				    
			    case 'valorintpre':
					array_push($coluna, array(
										"campo" => "valorintpre",
								  		"label" => "Valor de repasse  – Tempo Integral / Pré-Escola",
										"type"  => "float"));				
						continue;
				        break;				    
				    
			    case 'valorparcre':
					array_push($coluna, array(
										"campo" => "valorparcre",
								  		"label" => "Valor de repasse  – Tempo Parcial / Creche",
										"type"  => "float"));				
						continue;
				        break;				    
			    case 'valorparpre':
					array_push($coluna, array(
										"campo" => "valorparpre",
								  		"label" => "Valor de repasse – Tempo Parcial / Pré-Escola",
										"type"  => "float"));				
						continue;
				        break;		
				case 'obra':
					array_push($coluna, array(
										"campo" => "obra",
								  		"label" => "Nome da Obra"));					
					continue;
				    break;					        
			    case 'data_inicio_atendimento':
					array_push($coluna, array(
										"campo" => "data_inicio_atendimento",
								  		"label" => "Data de início do atendimento"));					
					continue;
				    break;					        
			    case 'tipologia_obra':
					array_push($coluna, array(
										"campo" => "tipologia_obra",
								  		"label" => "Tipologia"));					
					continue;
				    break;					        
			    case 'percentual_obra':
					array_push($coluna, array(
									"campo" => "percentual_obra",
							  		"label" => "Percentual Executado (%)",
									"type"  => "string"));				
					continue;
			        break;		
			   case 'estado_obra':
					array_push($coluna, array(
									"campo" => "estado_obra",
							  		"label" => "Situação da Obra"));				
					continue;
			        break;     				        
			   case 'estado_plano':
					array_push($coluna, array(
									"campo" => "estado_plano",
							  		"label" => "Situação do Plano"));				
					continue;
			        break;
			        
		      case 'valor_analise1':
					array_push($coluna, array(
									"campo" => "valor_analise1",
							  		"label" => "Valor da Análise 1"));				
					continue;
			        break;
			   case 'valor_analise2':
					array_push($coluna, array(
									"campo" => "valor_analise2",
							  		"label" => "Valor da Análise 2"));				
					continue;
			        break;
			              			        
			   case 'total_repassado':
					array_push($coluna, array(
									"campo" => "total_repassado",
							  		"label" => "Valor Repassado"));				
					continue;
			        break;     
				case 'total_repasse':
					array_push($coluna, array(
									"campo" => "total_repasse",
							  		"label" => "Valor Total de Repasse",
			  						"type"  => "float"));				
					continue;
			        break;    				        
				}
			endforeach;
		}
	return $coluna;			  	
}
*/
?>
</body>