<?php
function checklist_monta_coluna_relatorio(){
	
	$coluna = array();

	array_push( $coluna, array("campo" 	  => "atividadedescricao",
					   		   "label" 	  => "Descrição da atividade",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "itemprazo",
					   		   "label" 	  => "Prazo do item",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "execucao",
					   		   "label" 	  => "Execução",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "executores",
					   		   "label" 	  => "Executor(es)",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "validacao",
					   		   "label" 	  => "Validação",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "validadores",
					   		   "label" 	  => "Validador(es)",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "certificacao",
					   		   "label" 	  => "Certificação",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "certificadores",
					   		   "label" 	  => "Certificador(es)",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	
	return $coluna;
	
}


function checklist_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['popupAgrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("itemprazo","execucao","executores","atividadedescricao",
										  "validacao","validadores","certificacao","certificadores")
				);
				
	switch( $_REQUEST['popupAgrupador'] ){
		
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

	}	
	
	array_push( $agp['agrupador'], array("campo" 	  => "itemdescricao",
		 					   		     "label" 	  => "Descrição do item") );
	
	
	return $agp;
	
}


function checklist_monta_sql_relatorio(){
	
	$where = array();
	
	extract($_REQUEST);
	
	if($_REQUEST['agrupCampo']=='ati2.atiid'){
		$filtroPopup = "(ati2.atiid = ".$_REQUEST['agrupValor']." OR 
						ati3.atiid = ".$_REQUEST['agrupValor']." OR 
						ati4.atiid = ".$_REQUEST['agrupValor']." OR 
						ati5.atiid = ".$_REQUEST['agrupValor']." OR 
						ati6.atiid = ".$_REQUEST['agrupValor'].")";
	}else{
		$filtroPopup = $_REQUEST['agrupCampo']." = ".$_REQUEST['agrupValor'];
	}

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

	if($data_inicio != "" && $data_fim != "") {
		array_push($where, " ( icl.iclprazo >= '".formata_data_sql($data_inicio)."' AND icl.iclprazo <= '".formata_data_sql($data_fim)."' ) ");
	} elseif($data_inicio != "") {
		array_push($where, " icl.iclprazo >= '".formata_data_sql($data_inicio)."'");
	} elseif($data_fim != "") {
		array_push($where, " icl.iclprazo <= '".formata_data_sql($data_fim)."'");
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
	} elseif($possuiexecucao == "nao") {
	if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val1.vldid IS NULL AND et1.etcid IS NOT NULL $wh1");
		}else{
		    array_push($str, "val1.vldid IS NULL ");
		}
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
	} elseif($possuivalidacao == "nao") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val2.vldid IS NULL AND et2.etcid IS NOT NULL $wh2");
		}else{
		    array_push($str, "val2.vldid IS NULL");
		}
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
	} elseif($possuicertificacao == "nao") {
		if( ($executores[0] && $executores_campo_flag) || 
		    ($validadores[0] && $validadores_campo_flag) || 
		    ($certificadores[0] && $certificadores_campo_flag) || 
		    ($pessoas[0] && $pessoas_campo_flag) ||
		    (in_array('pessoas', $agrupador)) ||
		    (in_array('executores', $agrupador)) ||
		    (in_array('validadores', $agrupador)) ||
		    (in_array('certificadores', $agrupador)) ) {
			array_push($str, "val3.vldid IS NULL AND et3.etcid IS NOT NULL $wh3");
		}else{
		    array_push($str, "val3.vldid IS NULL");
		}
	}
	
	if($str[0]!=''){
		array_push($where, "( (".implode(') AND (',$str).") )");
	}
	
	if($prazovencido == "sim") {
		array_push($where, " icl.iclprazo < NOW()");
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
				icl.iclid || ' - <input type=\"hidden\" id=\"'||icl.iclid||'\"/>' ||icl.icldsc as itemdescricao,
				ati._atinumero ||' - <input type=\"hidden\" id=\"'||ati.atiid||'\"/>'|| ati.atidescricao as atividadedescricao,
				ati._atinumero ||' - <input type=\"hidden\" id=\"'||ati.atiid||'\"/>'|| ati.atidescricao as atividades,
				to_char(icl.iclprazo,'dd/mm/YYYY') as itemprazo,
				
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
		        
				CASE WHEN ati2.atitipoenem = 'S' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
					 WHEN ati3.atitipoenem = 'S' THEN ati3._atinumero ||' - '|| ati3.atidescricao 
					 WHEN ati4.atitipoenem = 'S' THEN ati4._atinumero ||' - '|| ati4.atidescricao
					 WHEN ati5.atitipoenem = 'S' THEN ati5._atinumero ||' - '|| ati5.atidescricao
					 WHEN ati6.atitipoenem = 'S' THEN ati6._atinumero ||' - '|| ati6.atidescricao
					ELSE 'Não possuem sub-processos'
			    END as subprocessos,
				CASE WHEN ati2.atitipoenem = 'P' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
					 WHEN ati3.atitipoenem = 'P' THEN ati3._atinumero ||' - '|| ati3.atidescricao
					 WHEN ati4.atitipoenem = 'P' THEN ati4._atinumero ||' - '|| ati4.atidescricao
					 WHEN ati5.atitipoenem = 'P' THEN ati5._atinumero ||' - '|| ati5.atidescricao
					 WHEN ati6.atitipoenem = 'P' THEN ati6._atinumero ||' - '|| ati6.atidescricao
					 ELSE 'Não existe'
				END as processos,
				CASE 
					 WHEN ati2.atitipoenem = 'E' THEN ati2._atinumero ||' - '|| ati2.atidescricao
					 WHEN ati3.atitipoenem = 'E' THEN ati3._atinumero ||' - '|| ati3.atidescricao
					 WHEN ati4.atitipoenem = 'E' THEN ati4._atinumero ||' - '|| ati4.atidescricao
					 WHEN ati5.atitipoenem = 'E' THEN ati5._atinumero ||' - '|| ati5.atidescricao
					 WHEN ati6.atitipoenem = 'E' THEN ati6._atinumero ||' - '|| ati6.atidescricao
					 ELSE 'Não existe'
				END as etapas,
		     	CASE WHEN icl.iclcritico=TRUE THEN 'Sim' ELSE 'Não' END as itemcritico,
		     	
		     	CASE WHEN val1.vldid IS NULL 	 AND val2.vldid IS NULL    AND val3.vldid IS NULL AND ch1.entid IS NOT NULL       THEN 'Não'
		          	 WHEN val1.vldid IS NOT NULL AND et1.etcid IS NOT NULL AND et1.etcopcaoevidencia=TRUE  THEN 'Sim<br> Com evidências ('||et1.etcevidencia||')'
		          	 WHEN val1.vldid IS NOT NULL AND et1.etcid IS NOT NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim<br> Sem evidências'
		     	END as execucao,
		     	CASE WHEN val2.vldid IS NULL 	 AND ch2.entid IS NOT NULL THEN 'Não'
		          	 WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND et2.etcid IS NOT NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim<br> Com evidências ('||coalesce(et2.etcevidencia,'-')||')'
		          	 WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND et2.etcid IS NOT NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim<br> Sem evidências'
		     	END as validacao,
		     	CASE WHEN val3.vldid IS NULL 	 AND ch3.entid IS NOT NULL THEN 'Não'
		         	 WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcid IS NOT NULL AND et3.etcopcaoevidencia=TRUE THEN 'Sim<br> Com evidências ('||et3.etcevidencia||')'
		          	 WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcid IS NOT NULL AND et3.etcopcaoevidencia=FALSE THEN 'Sim<br> Sem evidências'
		     	END as certificacao,
		     	
		     	CASE WHEN val1.vldid IS NULL 	 AND val2.vldid IS NULL AND val3.vldid IS NULL THEN 'Não'
		          	 WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
		         	 WHEN val1.vldid IS NOT NULL AND val2.vldid IS NULL AND val3.vldid IS NULL AND et1.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
		     	END as execucao_agrupador,
		     	CASE WHEN en1.entnome IS NULL THEN 'Sem executor(es)' ELSE en1.entnome || ' ' || case when trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) END 
		     	END as executores,
		
		     	CASE WHEN val2.vldid IS NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL THEN 'Não'
		          	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
		          	WHEN val2.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val3.vldid IS NULL AND et2.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
		     	END as validacao_agrupador,
		     	CASE WHEN en2.entnome IS NULL THEN 'Sem validador(es)' ELSE en2.entnome || ' ' || case when trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) END 
		     	END as validadores,
		     	
		     	CASE WHEN val3.vldid IS NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL THEN 'Não'
		          	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=TRUE THEN 'Sim - Com evidências'
		          	WHEN val3.vldid IS NOT NULL AND val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND et3.etcopcaoevidencia=FALSE THEN 'Sim - Sem evidências'
		     	END as certificacao_agrupador,
		    	CASE WHEN en3.entnome IS NULL
		        	THEN 'Sem certificador(es)'
		        	ELSE coalesce(en3.entnome,' ') || ' ' || case when trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) END 
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
			WHERE
				".$filtroPopup." AND 
			".(($where)?implode(" AND ",$where):"1=1")." 
			".(($_REQUEST['agrupadores'])?"ORDER BY ".implode(",",$agrupadores):"");
//ver($sql,1);
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
$rel->setTolizadorLinha(false);
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