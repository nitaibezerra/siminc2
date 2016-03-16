<?
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
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao($true ? true : false);
$r->setTotNivel(true);
$r->setEspandir($_POST['expandir']);
//$r->setBrasao(true);
if( $_POST['req'] == 2 ){
	ob_clean();
	$nomeDoArquivoXls="Relatório_Decreto_7446_de_1º_de_março_de_2011_-_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
}else{
	echo $r->getRelatorio();
}

function monta_sql(){
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if ($f_tipoensino){
		if($f_tipoensino == '1') $f_tipoensino = '12'; //Educação Superior
		if($f_tipoensino == '2') $f_tipoensino = '11'; //Educação Profissional
		if($f_tipoensino == '3') $f_tipoensino = '16'; //Hospitais
		$where[] = " fue.funid = '$f_tipoensino'";
	}
	
	$instituicao   = !is_array($instituicao)   ? explode(',', $instituicao)   : $instituicao;
	$tiposolicitacao    = !is_array($tiposolicitacao)    ? explode(',', $tiposolicitacao)    : $tiposolicitacao;

	
	if( $instituicao[0] && ( $instituicao_campo_flag || $instituicao_campo_flag == '1' )){
		$where[] = " ent.entid " . (( $instituicao_campo_excludente == null || $instituicao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $instituicao ) . "') ";
	}
		
	if( $tiposolicitacao[0] && ( $tiposolicitacao_campo_flag || $tiposolicitacao_campo_flag == '1' )){
		$where[] = " tpa.tpaid " . (($tiposolicitacao_campo_excludente == null || $tiposolicitacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tiposolicitacao ) . "') ";
	}
		
	if( $gndcod[0] && ( $gndcod_campo_flag || $gndcod_campo_flag == '1' )){
		$where[] = " sld.gndcod " . (($gndcod_campo_excludente == null || $gndcod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $gndcod ) . "') ";
	}
	
	if( $momento[0] && ( $momento_campo_flag || $momento_campo_flag == '1' )){
		$where[] = " sld.slfid " . (($momento_campo_excludente == null || $momento_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $momento ) . "') ";
	}
		
	if( $valorMin ){
		$where[] = " sld.sldvlrestimado >= " . str_replace(Array('.',','),Array('','.'),$valorMin);
	}
		
	if( $valorMax ){
		$where[] = " sld.sldvlrestimado <= " . str_replace(Array('.',','),Array('','.'),$valorMax);
	}
		
	if( $valorOp && $operador ){
		$where[] = " sld.sldvlrestimado". $operador . str_replace(Array('.',','),Array('','.'),$valorOp);
	}
	
	
	$agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador; 
	/*
	//if (strpos($agrupador, "instituicao") !== false){
		$select[] = "COALESCE(instituicao , 'Não informado') as instituicao";
		$select[] = "idinstituicao as idinstituicao";		
	//}

	//if (strpos($agrupador, "campus") !== false){
		$select[] = "COALESCE(campus, 'Não informado') as campus";
		$select[] = "idcampus as idcampus";	
	//}	

	//if (strpos($agrupador, "classe") !== false){
		$select[] = "COALESCE(classe, 'Não informado') as classe";
		$select[] = "idclasse as idclasse";	
	//}	

	//if (strpos($agrupador, "tipoensino") !== false || !$agrupador){
		$select[] = "tipoensino as tipoensino";	
	//}	
	
	//if (strpos($agrupador, "programa") !== false){
		$select[] = "programa as programa";
		$select[] = "idprograma as idprograma";	
	//}	

	//if (strpos($agrupador, "ano") !== false){
		$select[] = "ano";
	//}	
	
	//if (strpos($agrupador, "portaria") !== false){
		$select[] = "'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria";
	//}	
*/
	$agrupador = $_REQUEST['agrupador'];
	$agrupador[array_search('detalhessolicitacao',$agrupador)] = 'objeto';
	//SQL NOVO
	$sql = "SELECT 
				sldid as id,
				CASE WHEN ent.entsig <> '' THEN
					ent.entsig || ' - '
				     ELSE
					''
				END
				|| COALESCE(ent.entnome , 'Não informado') as instituicao ,
				ent.entid as idinstituicao,
				tpa.tpadsc as tiposolicitacao,
				tpa.tpaid as idtiposolicitacao, 
				sld.sldid || ' - ' || sld.sldobjeto as objeto, 
				sld.sldjustificativa as justificativa, 
				sld.sldvlrestimado as valor,
				CASE fue.funid 
					WHEN 11 THEN 'Educação Profissional'
					WHEN 12 THEN 'Educação Superior'
					WHEN 16 THEN 'Hospitais'
					ELSE 'Não informado'
				END  as tipoensino,
				'1' as qtde,
				gnd.gndcod || ' - ' || gnd.gnddsc as gnd,
				slf.slfdsc as momento
			FROM 
				academico.solicitacaodecreto sld
			INNER JOIN entidade.entidade 	     ent ON sld.entid = ent.entid
			INNER JOIN entidade.funcaoentidade   fue ON fue.entid = ent.entid and fuestatus='A'
			INNER JOIN academico.tiposolicitacao tpa ON tpa.tpaid = sld.tpaid
			LEFT JOIN public.gnd 				 gnd ON gnd.gndcod = sld.gndcod
			LEFT JOIN academico.sldfase			 slf ON slf.slfid = sld.slfid
			WHERE 
				entstatus = 'A' 
				AND sld.sldstatus = 'A' 
				AND fue.funid in (11,12,16)
				" . (is_array($where) ? " 
				AND ".implode(' AND ', $where) : '') . "
			ORDER BY 
				" . (is_array($agrupador) ? implode(' , ', $agrupador)."," : '') . "
				ent.entsig, 
				ent.entnome, 
				ent.entnome ";

	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	/*
	if (!$agrupador){
		$agrupador = array(
							'instituicao',
							'tiposolicitacao',
							'objeto',
							'justificativa',
							'valor'
						 );
	}elseif ( !is_array($agrupador) ){
		$agrupador = explode(",", $agrupador);
	}	
	*/
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"instituicao",	
									   		"tiposolicitacao", 
 									   		"objeto",
 									   		"justificativa",
 									   		"valor",
 									   		"tipoensino",
											"qtde",
											"momento"
										  )	  
				);
	
	//$entrou = 0;				
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'instituicao':
				//$entrou=1;
		    	array_push($agp['agrupador'], array(
													"campo" => "instituicao",
											  		"label" => "Instituição")										
									   				);
   								
		    	continue;
		        break;
			case 'detalhessolicitacao':
				//if($entrou==0){
					array_push($agp['agrupador'], array(
													"campo" => "objeto",
											  		"label" => "Objeto")										
									   				);				
				//}
		    	continue;
		        break;		        		    	
			case 'tiposolicitacao':
				array_push($agp['agrupador'], array(
													"campo" => "tiposolicitacao",
											  		"label" => "Tipo de Solicitação")										
									   				);					
		    	continue;
		        break;
		    case 'tipoensino':
				array_push($agp['agrupador'], array(
													"campo" => "tipoensino",
											  		"label" => "Tipo de Ensino")										
									   				);					
		    	continue;
		        break;		        	
		    case 'gnd':
				array_push($agp['agrupador'], array(
													"campo" => "gnd",
											  		"label" => "Código GND")										
									   				);					
		    	continue;
		        break;
			case 'momento':
				array_push($agp['agrupador'], array(
													"campo" => "momento",
											  		"label" => "Momento")										
									   				);					
		    	continue;
		        break;	
		}
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	
	$agrupador = $_REQUEST['agrupador'];
	
	if(in_array('detalhessolicitacao',$agrupador)){
	
	$coluna    = array(
						array(
							  "campo" => "justificativa",
					   		  "label" => "Justificativa"
						),
						array(
							  "campo" => "qtde",
					   		  "label" => "Quantidade",
							  "type"  => "numeric"
						),						
						array(
							  "campo" => "valor",
					   		  "label" => "Valor"
						)
					);
					
	}else{
					
	$coluna    = array(
						array(
							  "campo" => "qtde",
					   		  "label" => "Quantidade",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "valor",
					   		  "label" => "Valor"
						)						
					);
					
	}				
					
	return $coluna;			  	
}
?>
</body>