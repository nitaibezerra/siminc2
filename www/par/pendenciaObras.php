<?php

ini_set("memory_limit", "3048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_funcoes.php';
include_once '_funcoesPar.php';
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";
include_once 'autoload.php';
include_once '_constantes.php';

$db = new cls_banco();

//$preid = $_REQUEST['preid'];

$sql = "SELECT pre.preid, pre.docid 
		FROM 
			obras.preobra pre
		INNER JOIN par.subacaoobra so            ON so.preid = pre.preid 
		INNER JOIN par.subacao s ON s.sbaid = so.sbaid and sbastatus = 'A'
		INNER JOIN workflow.documento d ON d.docid = pre.docid
		INNER JOIN workflow.estadodocumento ed on ed.esdid = d.esdid 
		WHERE sobano in ('2011','2012') AND ed.tpdid=45 AND ed.esdid = 327 AND pre.prestatus = 'A'  AND pre.tooid = 2
	--	 pre.preid in (7721)
		";

$preids = $db->carregar( $sql );
$pendencia = array();

foreach( $preids as $preida ){
	
	if($preida['preid']){
	        $preid 				= $preida['preid'];
	        $docid				= $preida['docid'];
	        $oPreObra 			= new PreObra();
	        
			$oSubacaoControle 	= new SubacaoControle();
			$pacFNDE  			= $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
			$arDados  			= $oSubacaoControle->recuperarPreObra($preid);

			$qrpid 				= pegaQrpidPAC( $preid, 43 );
			$pacDados 			= $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
			$pacFotos 			= $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
			$pacDocumentos 		= $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
			
			if($pacFNDE == 'f'){
				$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
			}
		
			$pacQuestionario 		= $oPreObra->verificaQuestionario($qrpid);
			$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
			$pacCronograma 			= $oPreObra->verificaCronograma($preid);
			
			$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];

						//Caso o ano de cadastramento da subação seja o ano de exercício é obrigatório o preenchimento de tudo.
						if( $dado['sobano'] <= date('Y') ){
							$arPendencias = array('Dados do terreno' 						   => 'Falta o preenchimento dos dados.',
											  'Relatório de vistoria' 					   => 'Falta o preenchimento dos dados do Relatório de Vistoria.',
											  'Cadastro de fotos do terreno' 			   => 'Deve conter no mínimo 3 fotos do terreno.',
											  'Cronograma físico-financeiro' 			   => 'Falta o preenchimento dos dados.',
											  'Documentos anexos' 						   => 'Falta anexar os arquivos.',
											  'Projetos - Tipo A' 						   => 'Falta anexar os arquivos.',
											  'Itens Planilha orçamentária' 			   => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
											  'Planilha orçamentária' 					   => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
											  'Planilha orçamentária quadra com cobertura' => 'O valor {valor} não confere, deve ser menor ou igual a R$ 490.000,00.',
											  'Planilha orçamentária Tipo B 110v' 		   => 'O valor {valor} não confere, deve estar entre R$ 1.100.000,00 e R$ 1.330.000,00.',
											  'Planilha orçamentária Tipo B 220v' 		   => 'O valor {valor} não confere, deve estar entre R$ 1.100.000,00 e R$ 1.330.000,00.',
											  'Planilha orçamentária Tipo C 110v' 		   => 'O valor {valor} não confere, deve estar entre R$ 520.000,00 e R$ 620.000,00.',
											  'Planilha orçamentária Tipo C 220v' 		   => 'O valor {valor} não confere, deve estar entre R$ 520.000,00 e R$ 620.000,00.');
						} else { //Caso os anos sejam diferentes o único preenchimento obrigatório é o do Dados do Terreno.
							$arPendencias = array('Dados do terreno' 						   => 'Falta o preenchimento dos dados.');
						}


        			$sql = "select ptoid from obras.pretipoobra where ptoprojetofnde = 'f' AND ptostatus = 'A'";
        			$arrExcTipoObra = $db->carregarColuna( $sql );
        			//$arrExcTipoObra = array(16, 9, 21, 35, 17, 18, 29, 33, 34, 30);
        			
        			foreach($arPendencias as $k => $v){
        				if(  ( !$pacDados && $k == 'Dados do terreno' ) ||
							 ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) ||
							 ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
							 ( $k == 'Itens Planilha orçamentária' && $boPlanilhaOrcamentaria['faltam'] > 0 && !in_array($pacDados, $arrExcTipoObra) ) ||
							 ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $arDados['ptoprojetofnde'] == 't') ||
							 ( $k == 'Planilha orçamentária Tipo B 110v' && $boPlanilhaOrcamentaria['ptoid'] == 2 && ($boPlanilhaOrcamentaria['valor'] < 1100000 || $boPlanilhaOrcamentaria['valor'] > 1330000) ) ||
							 ( $k == 'Planilha orçamentária Tipo B 220v' && $boPlanilhaOrcamentaria['ptoid'] == 7 && ($boPlanilhaOrcamentaria['valor'] < 1100000 || $boPlanilhaOrcamentaria['valor'] > 1330000) ) ||
							 ( $k == 'Planilha orçamentária Tipo C 110v' && $boPlanilhaOrcamentaria['ptoid'] == 3 && ($boPlanilhaOrcamentaria['valor'] < 520000 || $boPlanilhaOrcamentaria['valor'] > 620000) ) ||
							 ( $k == 'Planilha orçamentária Tipo C 220v' && $boPlanilhaOrcamentaria['ptoid'] == 6 && ($boPlanilhaOrcamentaria['valor'] < 520000 || $boPlanilhaOrcamentaria['valor'] > 620000) ) ||
							 ( $k == 'Planilha orçamentária quadra com cobertura' && $boPlanilhaOrcamentaria['ptoid'] == 5 && $boPlanilhaOrcamentaria['valor'] > 490000 ) ||
							 ( $k == 'Cronograma físico-financeiro' && !$pacCronograma && $arDados['ptoprojetofnde'] == 't' ) ||
							 ( ($pacDocumentosTipoA['arqid'] != $pacDocumentosTipoA['podid'] || !$pacDocumentosTipoA) && $k == 'Projetos - Tipo A' && $arDados['ptoprojetofnde'] == 'f' ) ||
							 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' )
							 ){

								 switch($k){
											case 'Dados do terreno':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "1";
												}
												break;

											case 'Relatório de vistoria':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "2";
												}
												break;

											case 'Cadastro de fotos do terreno':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "3";
												}
												break;

											case 'Itens Planilha orçamentária':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "4";
												}
												break;

											case 'Planilha orçamentária':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "5";
												}
												break;

											case 'Planilha orçamentária Tipo B 110v':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "6";
												}
												break;
											case 'Planilha orçamentária Tipo B 220v':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "7";
												}
												break;

											case 'Planilha orçamentária Tipo C 110v':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "8";
												}
												break;

											case 'Planilha orçamentária Tipo C 220v':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "9";
												}
												break;
												
											case 'Cronograma físico-financeiro':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "10";
												}
												break;
												
											case 'Documentos anexos':
												if(!in_array($docid, $pendencia)){
													$pendencia[] = $docid;
													//echo "11";
												}
												break;
										}

							 }
        				}
        			}else{
        				echo "Preid não encontrado!";
        			}
	}
   // ver($pendencia);
  // die();
    foreach($pendencia as $docid){
	    //$sql = "select docid from obras.preobra where preid = ".$preid;
	   // $docid = $db->pegaUm($sql);
	    
	    $sqlHistorico = "insert into workflow.historicodocumento
													( aedid, docid, usucpf, htddata )
													values (1142 , " . $docid . ", '', now() )
													returning hstid";
	    
	    $db->executar($sqlHistorico);
	    
	    $sql = "update workflow.documento
				set esdid = 326
				where docid = " . $docid;
	    $db->executar($sql);
    }
    $db->commit();
?>