<?php
ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';

$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql();
$dados = $db->carregar($sql);


if( $_POST['req'] == 'geraxls' ){
	$arCabecalho = array();
	$colXls = array();
	
	array_push($arCabecalho, 'Nº do Processo SIDOC');
	array_push($colXls, 'prcnumsidoc');
	
	foreach ($_POST['agrupador'] as $agrup){
		if($agrup == 'coodsc'){
			array_push($arCabecalho, 'Unidade');
			array_push($colXls, 'coodsc');
		}
		if($agrup == 'aca.aeddscrealizar'){
			array_push($arCabecalho, 'Ação');
			array_push($colXls, 'aca.aeddscrealizar');
		}
		if($agrup == 'ent.entnome'){
			array_push($arCabecalho, 'Advogado');
			array_push($colXls, 'ent.entnome');
		}
	}
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
	foreach( $dados as $k => $registro ){
		foreach( $colXls as $campo ){
			$arDados[$k][$campo] = $registro[$campo];
		}
	}
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatorioIndicadores".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	die;
}


?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

<?php
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(true);
$r->setMonstrarTolizadorNivel(true);
$r->setBrasao(true);
$r->setEspandir( $_REQUEST['expandir']);
echo $r->getRelatorio();


function monta_sql(){
	extract($_REQUEST);
	$where = array();
	$arrOrdenadores = array();
	if($_REQUEST['ordenador']){
		foreach($_REQUEST['ordenador'] as $ord){
			$arrOrdenadores[] = $ord;
		}
	}
	$arrOrdenador = array("data","entnome","coodsc","aeddscrealizar","prcdesc","prcnomeinteressado");
	foreach($arrOrdenador as $ord){
		if(!in_array($ord,$arrOrdenadores)){
			$arrOrdenadores[] = $ord;
		}
	}
	
	// coordenação
	if( $coonid[0] && $coonid_campo_flag ){
		array_push($where, " co1.coonid " . (!$coonid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coonid ) . "') ");
	}
	
	// advogado
	if( $advid[0] && $advid_campo_flag ){
		array_push($where, " adv.advid " . (!$advid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $advid ) . "') ");
	}
	
	// Ação
	if( $aedid[0] && $aedid_campo_flag ){
		array_push($where, " aca.aedid " . (!$aedid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $aedid ) . "') ");
	}
	
	// período
	if( $htddata_inicio && $htddata_fim ){
		$htddata_inicio = formata_data_sql($htddata_inicio);
		$htddata_fim = formata_data_sql($htddata_fim);
		array_push($where, " his.htddata >= TIMESTAMP '{$htddata_inicio}' AND his.htddata <= TIMESTAMP '{$htddata_fim} 23:59:59' ");
	}
	
	if($_POST['req'] == 'geraxls'){
		$prcnumsidoc = "prc.prcnumsidoc";
		$data = "to_char(his.htddata,'DD/MM/YYYY HH24:MI:SS') as data";
	}else{
		$prcnumsidoc = "prc.prcnumsidoc || '<span style=\"display:none\" >' || his.hstid || '</span>' as prcnumsidoc";
		$data = "'<span style=\"display:none\" >' || his.htddata || '</span>' || to_char(his.htddata,'DD/MM/YYYY HH24:MI:SS') as data";
	}
	
	// monta o sql 
	$sql = "SELECT DISTINCT
				$prcnumsidoc,
				aca.aeddscrealizada as aeddscrealizar,
               coalesce(co1.coodsc,
                   CASE WHEN aca.aedid = ".ACAO_ENCAMINHAR_PARA_ADVOGADO."
                       THEN coalesce(co2.coodsc,'N/A')
                       ELSE 'N/A'
                   END
               ) as coodsc,
				$data,
				to_char(his.htddata,'DD/MM/YYYY') as htddata,
				coalesce(ent.entnome,'N/A') as entnome,
				coalesce(prc.prcdesc,'N/A') as prcdesc,
				coalesce(com.cmddsc,'-') as comentario,
				prc.prcnomeinteressado as prcnomeinteressado,
				1 as qtd
		   FROM
               workflow.historicodocumento his
           LEFT JOIN workflow.comentariodocumento com ON com.hstid = his.hstid
           INNER JOIN workflow.documento doc ON doc.docid = his.docid
           INNER JOIN profeinep.estruturaprocesso est ON est.docid = his.docid
           INNER JOIN profeinep.processoprofeinep prc ON prc.prcid = est.prcid
           LEFT JOIN workflow.acaoestadodoc aca ON aca.aedid = his.aedid
           LEFT JOIN profeinep.historicoadvogados had ON had.hstid = his.hstid
           LEFT JOIN profeinep.advogados adv ON adv.advid = had.advid
           /*LEFT JOIN (SELECT
                           coonid,
                           dtatribuicao,
                           c.prcid
                       FROM
                           profeinep.historicocoordenacoes c
                       INNER JOIN (SELECT
                                   max(hcoid) as hcoid,
                                   h.prcid
                                FROM
                                   profeinep.historicocoordenacoes h
                                INNER JOIN (SELECT
                                           dtatribuicao as dtatribuicao,
                                           prcid
                                        FROM
                                           profeinep.historicoadvogados ) c ON c.prcid = h.prcid AND c.dtatribuicao > h.dtatribuicao
                                GROUP BY
                                   h.prcid
                                   ) cc ON
                       cc.hcoid = c.hcoid AND c.prcid = cc.prcid ) hc2 ON hc2.prcid = prc.prcid*/
           /*LEFT JOIN (SELECT h.coonid,h.prcid
			FROM profeinep.historicocoordenacoes h 
			INNER JOIN (SELECT max(hco.hcoid) as hcoid,hco.prcid,hadv.hadid 
					FROM profeinep.historicocoordenacoes hco, profeinep.historicoadvogados hadv 
					WHERE hco.prcid = hadv.prcid
						AND hco.dtatribuicao < hadv.dtatribuicao 
						--AND hco.prcid = 14327
					GROUP BY hco.prcid,hadv.hadid) h2 ON h.hcoid = h2.hcoid
			)hc2 ON hc2.prcid = prc.prcid*/
           LEFT JOIN profeinep.coordenacao co2 ON co2.coonid = adv.coonid
           LEFT JOIN entidade.entidade ent ON ent.entid = adv.entid
           LEFT JOIN profeinep.historicocoordenacoes hco ON hco.hstid = his.hstid
           LEFT JOIN profeinep.coordenacao co1 ON co1.coonid = hco.coonid
           WHERE
				doc.tpdid = 49
				AND his.htddata > TIMESTAMP '2012-01-01 00:00:00'
				".($where[0] ? ' AND' . implode(' AND ', $where) : '' )." 
			ORDER BY 
				".($arrOrdenadores ? implode(',',$arrOrdenadores) : 'data')." ";
	//dbg($sql,1);
	return $sql;
}
 
function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array("agrupador" => array(),
				 "agrupadoColuna" => array( "aeddscrealizar",
											"coodsc",
											"entnome",
											"prcdesc",
											"prcnomeinteressado",
											"qtd",
											"prcnumsidoc",
											"data",
											"comentario",
											"htddata" ) );
	
	foreach ($agrupador as $val){ 
		switch ($val) {
		    case 'htddata':
				array_push($agp['agrupador'], array("campo" => "htddata",
											 		"label" => "Data") );					
		    	continue;			
		        break;	
		    case 'aeddscrealizar':
				array_push($agp['agrupador'], array("campo" => "aeddscrealizar",
											 		"label" => "Ação") );					
		    	continue;			
		        break;	
		    case 'coodsc':
				array_push($agp['agrupador'], array("campo" => "coodsc",
													"label" => "Unidade") );	
				continue;
				break;
		    case 'entnome':
				array_push($agp['agrupador'], array("campo" => "entnome",
													"label" => "Advogado") );	
				continue;
				break;	 
		}
	};
	array_push($agp['agrupador'], array("campo" => "prcnumsidoc",
										"label" => "Nº do Processo SIDOC") );	
	return $agp;
}

function monta_coluna(){
	
	$coluna = array();
		
	foreach ($_REQUEST['colunas'] as $val){ 
		switch ($val) {
		    case 'htddata':
				array_push($coluna, array("campo" => "data",
											 		"label" => "Data") );					
		    	continue;			
		        break;	
		     case 'descproc':
				array_push($coluna, array("campo" => "prcdesc",
											 		"label" => "Descrição do Processo") );					
		    	continue;			
		        break;	
		    case 'aeddscrealizar':
				array_push($coluna, array("campo" => "aeddscrealizar",
											 		"label" => "Ação") );					
		    	continue;			
		        break;	
		    case 'entnome':
				array_push($coluna, array("campo" => "entnome",
													"label" => "Advogado") );	
				continue;
				break;
			case 'prcnomeinteressado':
				array_push($coluna, array("campo" => "prcnomeinteressado",
													"label" => "Interessado") );	
				continue;
				break;	
			case 'unidadefisica':
				array_push($coluna, array("campo" => "coodsc",
													"label" => "Unidade Física") );	
				continue;
				break;	
			case 'comentario':
				array_push($coluna, array("campo" => "comentario",
													"label" => "Comentário") );	
				continue;
				break;	 
		}
	};
					  	
	return $coluna;			  	
}
?>
</body>
</html>