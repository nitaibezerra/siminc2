<?php
function checklist_monta_coluna_relatorio(){
	
	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "qtdexecucao_sim",
					   		   "label" 	  => "Qtd. Execução - Sim",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdexecucao_nao",
					   		   "label" 	  => "Qtd. Execução - Não",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdvalidacao_sim",
					   		   "label" 	  => "Qtd. Validação - Sim",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdvalidacao_nao",
					   		   "label" 	  => "Qtd. Validação - Não",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdcertificacao_sim",
					   		   "label" 	  => "Qtd. Certificação - Sim",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdcertificacao_nao",
					   		   "label" 	  => "Qtd. Certificação - Não",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "soma_total",
					   		   "label" 	  => "Total",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );

	return $coluna;
	
}


function checklist_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupadores'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("qtdexecucao_sim","qtdexecucao_nao","qtdvalidacao_sim","qtdvalidacao_nao","qtdcertificacao_sim","qtdcertificacao_nao","soma_total")
				);
				
	foreach ( $agrupador as $val ) {
		
		switch( $val ){
			case 'atividadedescricao':
				array_push( $agp['agrupador'], array("campo" 	  => "atividadedescricao",
								   		   			 "label" 	  => "Descrição da atividade") );
			break;
			
			case 'itemdescricao':
				array_push( $agp['agrupador'], array("campo" 	  => "itemdescricao",
								   		   			 "label" 	  => "Descrição do item") );
			break;
			
			case 'itemprazo':
				array_push( $agp['agrupador'], array("campo" 	  => "itemprazo",
								   		   		     "label" 	  => "Prazo do item") );
			break;
			
			case 'itemcritico':
				array_push( $agp['agrupador'], array("campo" 	  => "itemcritico",
								   		   			 "label" 	  => "Crítico") );
			break;
			
			case 'execucao_agrupador':
				array_push( $agp['agrupador'], array("campo" 	  => "execucao_agrupador",
								   		   		     "label" 	  => "Execução") );
			break;
			
			case 'executores':
				array_push( $agp['agrupador'], array("campo" 	  => "executores",
								   		   			 "label" 	  => "Executor(es)") );
			break;
			
			case 'validacao_agrupador':
				array_push( $agp['agrupador'], array("campo" 	  => "validacao_agrupador",
								   		   			 "label" 	  => "Validação") );
			break;
			
			case 'validadores':
				array_push( $agp['agrupador'], array("campo" 	  => "validadores",
								   		   			 "label" 	  => "Validador(es)") );
			break;
			
			case 'ceritificacao_agrupador':
				array_push( $agp['agrupador'], array("campo" 	  => "ceritificacao_agrupador",
								   		   		     "label" 	  => "Certificação") );
			break;
						
			case 'certificadores':
				array_push( $agp['agrupador'], array("campo" 	  => "certificadores",
								   		   		     "label" 	  => "Certificador(es)") );
			break;	

			case 'etapas':
				array_push( $agp['agrupador'], array("campo" 	  => "etapas",
								   		   		     "label" 	  => "Etapas") );
			break;	
			
			case 'processos':
				array_push( $agp['agrupador'], array("campo" 	  => "processos",
								   		   		     "label" 	  => "Processos") );
			break;
			
			case 'subprocessos':
				array_push( $agp['agrupador'], array("campo" 	  => "subprocessos",
								   		   		     "label" 	  => "Sub-processos") );
			break;
			
			case 'atividades':
				array_push( $agp['agrupador'], array("campo" 	  => "atividadedescricao",
								   		   		     "label" 	  => "Atividades") );
			break;
			
			case 'pessoas':
				array_push( $agp['agrupador'], array("campo" 	  => "executores",
								   		   			 "label" 	  => "Executor(es)") );
				array_push( $agp['agrupador'], array("campo" 	  => "validadores",
								   		   			 "label" 	  => "Validador(es)") );
				array_push( $agp['agrupador'], array("campo" 	  => "certificadores",
								   		   		     "label" 	  => "Certificador(es)") );
			break;

		}		
	}
	
	return $agp;
	
}


function checklist_monta_sql_relatorio(){
	
	$where = array();
	
	extract($_REQUEST);
	
	//$etapa
	if( $etapa[0] && $etapa_campo_flag ){
		array_push($where, "( ( ati3.atitipoenem = 'E' AND ati3.atiid " . (!$etapa_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $etapa ) . "') ) OR
							  ( ati4.atitipoenem = 'E' AND ati4.atiid " . (!$etapa_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $etapa ) . "') ) )");
	}
	
	//$processo
	if( $processo[0] && $processo_campo_flag ){
		array_push($where, "( ( ati2.atitipoenem = 'P' AND ati2.atiid " . (!$processo_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $processo ) . "') ) OR
							  ( ati3.atitipoenem = 'P' AND ati3.atiid " . (!$processo_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $processo ) . "') ) )");
	}
	
	//$subprocesso
	if( $subprocesso[0] && $subprocesso_campo_flag ){
		array_push($where, " ati2.atiid " . (!$subprocesso_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $subprocesso ) . "') ");
	}
	
	// $atividade
	if( $atividade[0] && $atividade_campo_flag ){
		array_push($where, " ati.atiid " . (!$atividade_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $atividade ) . "') ");
	}
	
	// $executores
	if( $executores[0] && $executores_campo_flag ){
		array_push($where, " en1.entid " . (!$executores_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $executores ) . "') ");
	}
	
	// $validadores
	if( $validadores[0] && $validadores_campo_flag ){
		array_push($where, " en2.entid " . (!$validadores_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $validadores ) . "') ");
	}
	
	// $certificadores
	if( $certificadores[0] && $certificadores_campo_flag ){
		array_push($where, " en3.entid " . (!$certificadores_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $certificadores ) . "') ");
	}

	$wh1 = "";	
	$wh2 = "";	
	$wh3 = "";	
	
	// $pessoas
	if( $pessoas[0] && $pessoas_campo_flag ){
		array_push($where, "( en1.entid " . (!$pessoas_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pessoas ) . "')
							  OR en2.entid " . (!$pessoas_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pessoas ) . "')
							  OR en3.entid " . (!$pessoas_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pessoas ) . "') )");
		$wh1 = " AND ch1.entid " . (!$pessoas_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pessoas ) . "')";
		$wh2 = " AND ch2.entid " . (!$pessoas_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pessoas ) . "')";
		$wh3 = " AND ch3.entid " . (!$pessoas_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pessoas ) . "')";
	}

	if($data_inicio_exec != "" && $data_fim_exec != "") {
		array_push($where, " ( icl.iclprazoexec >= '".formata_data_sql($data_inicio_exec)."' AND icl.iclprazoexec <= '".formata_data_sql($data_fim_exec)."' ) ");
	} elseif($data_inicio_exec != "") {
		array_push($where, " icl.iclprazoexec >= '".formata_data_sql($data_inicio_exec)."'");
	} elseif($data_fim_exec != "") {
		array_push($where, " icl.iclprazoexec <= '".formata_data_sql($data_fim_exec)."'");
	}

	if($data_inicio_val != "" && $data_fim_val != "") {
		array_push($where, " ( icl.iclprazoval >= '".formata_data_sql($data_inicio_val)."' AND icl.iclprazoval <= '".formata_data_sql($data_fim_val)."' ) ");
	} elseif($data_inicio_val != "") {
		array_push($where, " icl.iclprazoval >= '".formata_data_sql($data_inicio_val)."'");
	} elseif($data_fim_val != "") {
		array_push($where, " icl.iclprazoval <= '".formata_data_sql($data_fim_val)."'");
	}

	if($data_inicio_cert != "" && $data_fim_cert != "") {
		array_push($where, " ( icl.iclprazocert >= '".formata_data_sql($data_inicio_cert)."' AND icl.iclprazocert <= '".formata_data_sql($data_fim_cert)."' ) ");
	} elseif($data_inicio != "") {
		array_push($where, " icl.iclprazocert >= '".formata_data_sql($data_inicio_cert)."'");
	} elseif($data_fim != "") {
		array_push($where, " icl.iclprazocert <= '".formata_data_sql($data_fim_cert)."'");
	}

	if($data_inicio != "" && $data_fim != "") {
		array_push($where, "( en3.entid IS NOT NULL AND ( icl.iclprazo >= '".formata_data_sql($data_inicio)."' AND icl.iclprazo <= '".formata_data_sql($data_fim)."' ) )
							OR ( en3.entid IS NULL AND en2.entid IS NOT NULL AND ( icl.iclprazo >= '".formata_data_sql($data_inicio)."' AND icl.iclprazo <= '".formata_data_sql($data_fim)."' ) )
							OR ( en3.entid IS NULL AND en2.entid IS NULL AND en1.entid IS NOT NULL AND ( icl.iclprazo >= '".formata_data_sql($data_inicio)."' AND icl.iclprazo <= '".formata_data_sql($data_fim)."' ) )");
	} elseif($data_inicio != "") {
		array_push($where, "( en3.entid IS NOT NULL AND (icl.iclprazo >= '".formata_data_sql($data_inicio)."') )
							OR ( en3.entid IS NULL AND en2.entid IS NOT NULL AND (icl.iclprazo >= '".formata_data_sql($data_inicio)."') )
							OR ( en3.entid IS NULL AND en2.entid IS NULL AND en1.entid IS NOT NULL AND (icl.iclprazo >= '".formata_data_sql($data_inicio)."') )");
	} elseif($data_fim != "") {
		array_push($where, "( en3.entid IS NOT NULL AND ( icl.iclprazo <= '".formata_data_sql($data_fim)."' ) )
							OR ( en3.entid IS NULL AND en2.entid IS NOT NULL AND ( icl.iclprazo <= '".formata_data_sql($data_fim)."' ) )
							OR ( en3.entid IS NULL AND en2.entid IS NULL AND en1.entid IS NOT NULL AND ( icl.iclprazo <= '".formata_data_sql($data_fim)."' ) )");
	}
	
	$agrupador = $_REQUEST['agrupadores'];
	$str = Array();

	if($possuiexecucao == "sim") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val1.vldid IS NOT NULL AND et1.etcid IS NOT NULL $wh1");
		}else{
		    array_push($str, "val1.vldid IS NOT NULL");
		}
		$nao1 = 'AND 1=0';
		$sim1 = 'AND 1=1';
	} elseif($possuiexecucao == "nao") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val1.vldid IS NULL 	 AND val2.vldid IS NULL    AND val3.vldid IS NULL AND ch1.entid IS NOT NULL AND et1.etcid IS NOT NULL $wh1");
		}else{
		    array_push($str, "val1.vldid IS NULL 	 AND val2.vldid IS NULL    AND val3.vldid IS NULL AND ch1.entid IS NOT NULL");
		}
		$nao1 = 'AND 1=1';
		$sim1 = 'AND 1=0';
	}

	if($possuivalidacao == "sim") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val2.vldid IS NOT NULL AND et2.etcid IS NOT NULL $wh2");
		}else{
		    array_push($str, "val2.vldid IS NOT NULL");
		}
		$nao2 = 'AND 1=0';
		$sim2 = 'AND 1=1';
	} elseif($possuivalidacao == "nao") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val2.vldid IS NULL 	 AND ch2.entid IS NOT NULL AND et2.etcid IS NOT NULL $wh2");
		}else{
		    array_push($str, "val2.vldid IS NULL 	 AND ch2.entid IS NOT NULL");
		}
		$nao2 = 'AND 1=1';
		$sim2 = 'AND 1=0';
	}
	
	if($possuicertificacao == "sim") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val3.vldid IS NOT NULL AND et3.etcid IS NOT NULL $wh3");
		}else{
		    array_push($str, "val3.vldid IS NOT NULL ");
		}
		$nao3 = 'AND 1=0';
		$sim3 = 'AND 1=1';
	} elseif($possuicertificacao == "nao") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val3.vldid IS NULL 	 AND ch3.entid IS NOT NULL AND et3.etcid IS NOT NULL $wh3");
		}else{
		    array_push($str, "val3.vldid IS NULL 	 AND ch3.entid IS NOT NULL");
		}
		$nao3 = 'AND 1=1';
		$sim3 = 'AND 1=0';
	}
	
	if($str[0]!=''){
//		if( ($executores[0] && $executores_campo_flag) || 
//		    ($validadores[0] && $validadores_campo_flag) || 
//		    ($certificadores[0] && $certificadores_campo_flag) || 
//		    ($pessoas[0] && $pessoas_campo_flag) ) {
//		    	
//			array_push($where, "( (".implode(' OR ',$str).") AND (".implode(' AND ',$str2).") )");
//		}else{
			
			array_push($where, "( (".implode(') OR (',$str).") )");
//		}
	}
	
	if($prazovencido == "sim") {
		array_push($where, "( en3.entid IS NOT NULL AND ( icl.iclprazo < NOW() ) ) OR
							( en3.entid IS NULL AND en2.entid IS NOT NULL AND ( icl.iclprazo < NOW() ) ) OR
							( en3.entid IS NULL AND en2.entid IS NULL AND en1.entid IS NOT NULL AND ( icl.iclprazo < NOW() ) )");
	}
	
	$agrupadores = Array();
	if($_REQUEST['agrupadores']){
		if(in_array('pessoas',$_REQUEST['agrupadores'])){
			foreach($_REQUEST['agrupadores'] as $arg){
				if($arg != 'pessoas'){
					array_push($agrupadores,$arg);
				}
			}
			array_push($agrupadores,'executores');
			array_push($agrupadores,'validadores');
			array_push($agrupadores,'certificadores');
		}else{
			$agrupadores = $_REQUEST['agrupadores'];
		}
	}
	
if (in_array('subprocessos', $agrupador)) {
	array_push($where, " ( ati2.atitipoenem = 'S' or ati3.atitipoenem = 'S' or ati4.atitipoenem = 'S' or ati5.atitipoenem = 'S' or 
	ati6.atitipoenem = 'S' ) ");
	}

	// monta o sql 
	$sql = "SELECT DISTINCT
				'<input input=\"hidden\" value=\"icl.iclid\"/><div onclick=\"popupDetalhe( \'icl.iclid\', '|| icl.iclid ||', \'itemdescricao\' )\">'|| icl.iclid || ' - ' ||icl.icldsc||'<div>' as itemdescricao,
				'<div onclick=\"popupDetalhe( \'icl.iclprazo\', '||icl.iclprazo||', \'itemprazo\' )\">'||to_char(icl.iclprazo,'dd/mm/YYYY')||'</div>' as itemprazo,
				CASE WHEN icl.iclcritico=TRUE 
					THEN '<div onclick=\"popupDetalhe( \'icl.iclcritico\', \'TRUE\', \'itemcritico\' )\">Sim</div>' 
					ELSE '<div onclick=\"popupDetalhe( \'icl.iclcritico\', \'FALSE\', \'itemcritico\' )\">Não</div>' 
				END as itemcritico,
				'<div onclick=\"popupDetalhe( \'ati.atiid\', '|| ati.atiid ||', \'atividades\' )\">'||ati._atinumero ||' - '|| ati.atidescricao||'<div>' as atividades,
				'<div onclick=\"popupDetalhe( \'ati.atiid\', '|| ati.atiid ||', \'atividadedescricao\' )\">'||ati._atinumero ||' - '|| ati.atidescricao||'</div>' as atividadedescricao,
				
				CASE WHEN val1.vldsituacao = TRUE THEN ' Execução validada. '|| CASE WHEN val1.vldobservacao !='' THEN 'Observação:'||val1.vldobservacao ELSE '' END
		          	WHEN val1.vldsituacao = FALSE THEN ' Execução invalidada. '|| CASE WHEN val1.vldobservacao !='' THEN 'Observação:'||val1.vldobservacao ELSE '' END
		          	ELSE ' Execução não realizada. ' 
		        END as executado,
				CASE WHEN val2.vldsituacao = TRUE THEN ' Validação validada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
		          	WHEN val2.vldsituacao = FALSE THEN ' Validação invalidada. '|| CASE WHEN val2.vldobservacao !='' THEN 'Observação:'||val2.vldobservacao ELSE '' END
		          	ELSE ' Validação não realizada. ' 
		        END as validado,
		     	CASE WHEN val3.vldsituacao = TRUE THEN ' Certificação validada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
					WHEN val3.vldsituacao = FALSE THEN ' Certificação invalidada. '|| CASE WHEN val3.vldobservacao !='' THEN 'Observação:'||val3.vldobservacao ELSE '' END
		          	ELSE ' Certificação não realizada. ' 
		        END as certificado,
				
				CASE WHEN (val1.vldid IS NOT NULL AND val1.vldsituacao = TRUE) AND val2.vldid IS NULL AND val3.vldid IS NULL $wh1 $sim1 THEN 1 ELSE 0 END as qtdexecucao_sim,
				CASE WHEN (val1.vldid IS NULL OR val1.vldsituacao = FALSE)     AND val2.vldid IS NULL AND val3.vldid IS NULL $wh1 $nao1 THEN 1 ELSE 0 END as qtdexecucao_nao,
				
				CASE WHEN (val2.vldid IS NOT NULL AND val2.vldsituacao = TRUE)  AND val1.vldid IS NOT NULL AND val3.vldid IS NULL $wh2 $sim2 THEN 1 ELSE 0 END as qtdvalidacao_sim,
				CASE WHEN (val2.vldid IS NOT NULL AND val1.vldsituacao = FALSE) AND val1.vldid IS NOT NULL AND val3.vldid IS NULL $wh2 $nao2 THEN 1 ELSE 0 END as qtdvalidacao_nao,
				
				CASE WHEN (val3.vldid IS NOT NULL AND val3.vldsituacao = TRUE)  AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL $wh3 $sim3 THEN 1 ELSE 0 END as qtdcertificacao_sim,
				CASE WHEN (val3.vldid IS NOT NULL AND val3.vldsituacao = FALSE) AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL $wh3 $nao3 THEN 1 ELSE 0 END as qtdcertificacao_nao,

				CASE WHEN (val1.vldid IS NOT NULL AND val1.vldsituacao = TRUE)  AND val2.vldid IS NULL AND val3.vldid IS NULL $wh1 $sim1 THEN 1 ELSE 0 END +
				CASE WHEN (val1.vldid IS NULL OR val1.vldsituacao = FALSE)      AND val2.vldid IS NULL AND val3.vldid IS NULL $wh1 $nao1 THEN 1 ELSE 0 END +
				CASE WHEN (val2.vldid IS NOT NULL AND val2.vldsituacao = TRUE)  AND val1.vldid IS NOT NULL AND val3.vldid IS NULL $wh2 $sim2 THEN 1 ELSE 0 END +
				CASE WHEN (val2.vldid IS NOT NULL AND val1.vldsituacao = FALSE) AND val1.vldid IS NOT NULL AND val3.vldid IS NULL $wh2 $nao2 THEN 1 ELSE 0 END +
				CASE WHEN (val3.vldid IS NOT NULL AND val3.vldsituacao = TRUE)  AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL $wh3 $sim3 THEN 1 ELSE 0 END +
				CASE WHEN (val3.vldid IS NOT NULL AND val3.vldsituacao = FALSE) AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL $wh3 $nao3 THEN 1 ELSE 0 END as soma_total,


				CASE WHEN ati2.atitipoenem = 'S' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati2.atiid ||', \'sub-processos\' )\">'||ati2._atinumero ||' - '|| ati2.atidescricao||'</div>' 
					 WHEN ati3.atitipoenem = 'S' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati3.atiid ||', \'sub-processos\' )\">'||ati3._atinumero ||' - '|| ati3.atidescricao||'</div>' 
					 WHEN ati4.atitipoenem = 'S' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati4.atiid ||', \'sub-processos\' )\">'||ati4._atinumero ||' - '|| ati4.atidescricao||'</div>'
					 WHEN ati5.atitipoenem = 'S' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati5.atiid ||', \'sub-processos\' )\">'||ati5._atinumero ||' - '|| ati5.atidescricao||'</div>'
					 WHEN ati6.atitipoenem = 'S' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati6.atiid ||', \'sub-processos\' )\">'||ati6._atinumero ||' - '|| ati6.atidescricao||'</div>'
					ELSE 'Não possuem sub-processos'
			    END as subprocessos,
				CASE WHEN ati2.atitipoenem = 'P' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati2.atiid ||', \'processos\' )\">'||ati2._atinumero ||' - '|| ati2.atidescricao||'</div>' 
					 WHEN ati3.atitipoenem = 'P' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati3.atiid ||', \'processos\' )\">'||ati3._atinumero ||' - '|| ati3.atidescricao||'</div>'
					 WHEN ati4.atitipoenem = 'P' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati4.atiid ||', \'processos\' )\">'||ati4._atinumero ||' - '|| ati4.atidescricao||'</div>'
					 WHEN ati5.atitipoenem = 'P' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati5.atiid ||', \'processos\' )\">'||ati5._atinumero ||' - '|| ati5.atidescricao||'</div>'
					 WHEN ati6.atitipoenem = 'P' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati6.atiid ||', \'processos\' )\">'||ati6._atinumero ||' - '|| ati6.atidescricao||'</div>'
					 ELSE 'Não existe'
				END as processos,
				CASE 
					 WHEN ati2.atitipoenem = 'E' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati2.atiid ||', \'etapas\' )\">'||ati2._atinumero ||' - '|| ati2.atidescricao||'</div>'
					 WHEN ati3.atitipoenem = 'E' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati3.atiid ||', \'etapas\' )\">'||ati3._atinumero ||' - '|| ati3.atidescricao||'</div>'
					 WHEN ati4.atitipoenem = 'E' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati4.atiid ||', \'etapas\' )\">'||ati4._atinumero ||' - '|| ati4.atidescricao||'</div>'
					 WHEN ati5.atitipoenem = 'E' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati5.atiid ||', \'etapas\' )\">'||ati5._atinumero ||' - '|| ati5.atidescricao||'</div>'
					 WHEN ati6.atitipoenem = 'E' THEN '<div onclick=\"popupDetalhe( \'ati2.atiid\', '|| ati6.atiid ||', \'etapas\' )\">'||ati6._atinumero ||' - '|| ati6.atidescricao||'</div>'
					 ELSE 'Não existe'
				END as etapas,
				
				CASE WHEN val1.vldid IS NULL 
						THEN '<div onclick=\"popupDetalhe( \'val1.vldid\', \'NULL\', \'execucao\' )\">Não</div>'
		          	WHEN val1.vldid IS NOT NULL AND et1.etcopcaoevidencia=TRUE 
		          		THEN '<div onclick=\"popupDetalhe( \'et1.etcopcaoevidencia\', \'TRUE\', \'execucao\' )\">Sim Com evidências ('||et1.etcevidencia||')</div>'
		          	WHEN val1.vldid IS NOT NULL AND et1.etcopcaoevidencia=FALSE 
		          		THEN '<div onclick=\"popupDetalhe( \'et1.etcopcaoevidencia\', \'FALSE\', \'execucao\' )\">Sim Sem evidências</div>'
		     	END as execucao,
		     	CASE WHEN val1.vldid IS NULL THEN 'Não'
		          	WHEN val1.vldid IS NOT NULL AND et1.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
		         	WHEN val1.vldid IS NOT NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
		     	END as execucao_agrupador,
		     	CASE WHEN en1.entnome IS NULL 
		     		THEN '<div onclick=\"popupDetalhe( \'en1.entid\', \'NULL\', \'executores\' )\">Sem executor(es)</div>' 
		     		ELSE '<div onclick=\"popupDetalhe( \'en1.entid\', '|| en1.entid ||', \'executores\' )\">'||en1.entnome || ' ' || case when trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) END ||'</div>' 
		     	END as executores,
		
		     	CASE WHEN val2.vldid IS NULL 
		     			THEN '<div onclick=\"popupDetalhe( \'val2.vldid\', \'NULL\', \'validacao\' )\">Não</div>'
		          	WHEN val2.vldid IS NOT NULL AND et2.etcopcaoevidencia=TRUE 
		          		THEN '<div onclick=\"popupDetalhe( \'et2.etcopcaoevidencia\', \'TRUE\', \'validacao\' )\">Sim Com evidências ('||et2.etcevidencia||')</div>'
		          	WHEN val2.vldid IS NOT NULL AND et2.etcopcaoevidencia=FALSE 
		          		THEN '<div onclick=\"popupDetalhe( \'et2.etcopcaoevidencia\', \'FALSE\', \'validacao\' )\">Sim Sem evidências</div>'
		     	END as validacao,
		     	CASE WHEN val2.vldid IS NULL THEN 'Não'
		          	WHEN val2.vldid IS NOT NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
		          	WHEN val2.vldid IS NOT NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
		     	END as validacao_agrupador,
		     	CASE WHEN en2.entnome IS NULL 
		     		THEN '<div onclick=\"popupDetalhe( \'en2.entid\', \'NULL\', \'validadores\' )\">Sem validador(es)</div>' 
		     		ELSE '<div onclick=\"popupDetalhe( \'en2.entid\', '|| en2.entid ||', \'validadores\' )\">'||en2.entnome || ' ' || case when trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) END ||'</div>'
		     	END as validadores,
		     	
		     	CASE WHEN val3.vldid IS NULL 
		     			THEN '<div onclick=\"popupDetalhe( \'val3.vldid\', \'NULL\', \'certificacao\' )\">Não</div>'
		         	WHEN val3.vldid IS NOT NULL AND et3.etcopcaoevidencia=TRUE 
		         		THEN '<div onclick=\"popupDetalhe( \'et3.etcopcaoevidencia\', \'TRUE\', \'certificacao\' )\">Sim Com evidências ('||et3.etcevidencia||')</div>'
		          	WHEN val3.vldid IS NOT NULL AND et3.etcopcaoevidencia=FALSE 
		          		THEN '<div onclick=\"popupDetalhe( \'et3.etcopcaoevidencia\', \'FALSE\', \'certificacao\' )\">Sim Sem evidências</div>'
		     	END as certificacao,
		     	CASE WHEN val3.vldid IS NULL THEN 'Não'
		          	WHEN val3.vldid IS NOT NULL AND et3.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
		          	WHEN val3.vldid IS NOT NULL AND et3.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
		     	END as certificacao_agrupador,
		    	CASE WHEN en3.entnome IS NULL
		        	THEN '<div onclick=\"popupDetalhe( \'en3.entid\', \'NULL\', \'certificadores\' )\">Sem certificador(es)</div>'
		        	ELSE '<div onclick=\"popupDetalhe( \'en3.entid\', '|| en3.entid ||', \'certificadores\' )\">'||coalesce(en3.entnome,' ') || ' ' || case when trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) END ||'</div>' 
		    	END as certificadores
				
			FROM 
				pde.itemchecklist icl
			INNER JOIN 
				pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A'
			LEFT JOIN 
				pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
			LEFT JOIN 
				pde.atividade ati3 ON ati3.atiid = ati2.atiidpai AND ati3.atistatus = 'A'
			LEFT JOIN 
				pde.atividade ati4 ON ati4.atiid = ati3.atiidpai AND ati4.atistatus = 'A'
			LEFT JOIN 
				pde.atividade ati5 ON ati5.atiid = ati4.atiidpai AND ati5.atistatus = 'A'
			LEFT JOIN 
				pde.atividade ati6 ON ati6.atiid = ati5.atiidpai AND ati6.atistatus = 'A'
			LEFT JOIN 
				pde.etapascontrole et1 ON et1.iclid = icl.iclid AND et1.tpvid = 1 
			LEFT JOIN 
				pde.validacao val1 ON val1.iclid = icl.iclid AND val1.tpvid = 1 
			LEFT JOIN 
				pde.checklistentidade ch1 ON ch1.iclid = icl.iclid AND ch1.tpvid = 1
			LEFT JOIN 
				entidade.entidade en1 ON en1.entid = ch1.entid AND en1.entstatus = 'A'
			LEFT JOIN 
				pde.etapascontrole et2 ON et2.iclid = icl.iclid AND et2.tpvid = 2 
			LEFT JOIN 
				pde.validacao val2 ON val2.iclid = icl.iclid AND val2.tpvid = 2 
			LEFT JOIN 
				pde.checklistentidade ch2 ON ch2.iclid = icl.iclid AND ch2.tpvid = 2
			LEFT JOIN 
				entidade.entidade en2 ON en2.entid = ch2.entid AND en2.entstatus = 'A'
			LEFT JOIN 
				pde.etapascontrole et3 ON et3.iclid = icl.iclid AND et3.tpvid = 3
			LEFT JOIN 
				pde.validacao val3 ON val3.iclid = icl.iclid AND val3.tpvid = 3 
			LEFT JOIN 
				pde.checklistentidade ch3 ON ch3.iclid = icl.iclid AND ch3.tpvid = 3
			LEFT JOIN 
				entidade.entidade en3 ON en3.entid = ch3.entid AND en3.entstatus = 'A'	
			".(($where)?"WHERE ".implode(" AND ",$where):"")." 
			".(($order)?"ORDER BY ".implode(",",$order):"");
//	ver($sql,d);
	return $sql;
	
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = checklist_monta_sql_relatorio(); //dbg($sql,1);
$agrupador = checklist_monta_agp_relatorio();
$coluna    = checklist_monta_coluna_relatorio();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setTotNivel(true);
$rel->setEspandir($_REQUEST['arvore_']);
$rel->setMonstrarTolizadorNivel(true);

// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == '2' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery.timers.js"></script>
<script>
var arRequest = <?=simec_json_encode($_REQUEST) ?>;
var arPost = <?=simec_json_encode($_POST) ?>;
jQuery.noConflict();

function popupDetalhe( campo, valor, agrupador ){
	window.open('pacto.php?modulo=relatorio_pacto/popup_checklist_resultado&acao=A&form=true&enviar=true&'+jQuery.param(arPost)+'&agrupCampo='+campo+'&agrupValor='+valor+'&popupAgrupador='+agrupador, 
			   'modelo', 
			   "height=600,width=800,scrollbars=yes,top=50,left=200" );
}

</script>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>