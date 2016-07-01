<?php 

if ($_REQUEST['friid']) {
    
    $sql = "select * 
                from 
                    parindigena.itemmonitoramento as im 
            inner join parindigena.formacaoinicial as f 
                on f.friid = im.friid 
            where 
                f.friid= {$_REQUEST['friid']}
            and im.itmstatus = 'A'";

    $rsDadosForm       = $db->carregar( $sql );
    $estuf 		 	   = $rsDadosForm[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosForm[0]['estuf']; 
    $tipo 			   = 1;  
    $apmid 			   = $_REQUEST['friid'];
    $tipoId			   = 'friid';
     
} else if ($_REQUEST['frcid']) {
	
    $sql = "SELECT
                *  
            FROM
                parindigena.formacaocontinuada AS fi
            INNER JOIN parindigena.itemmonitoramento AS m ON m.frcid = fi.frcid
            WHERE
                fi.frcid = {$_REQUEST['frcid']}
                and m.itmstatus = 'A'";               

    $rsDadosForm 	   = $db->carregar( $sql );
    $estuf 		 	   = $rsDadosForm[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosForm[0]['estuf'];    
    $tipo 			   = 2;
    $apmid 			   = $_REQUEST['frcid'];
    $tipoId			   = 'frcid';
           
} else if ($_REQUEST['madid']) {
    
    $sql = "select * 
            from 
                parindigena.itemmonitoramento as im 
            inner join parindigena.materialdidatico as m on m.madid = im.madid 
            where m.madid = {$_REQUEST['madid']}
            and im.itmstatus = 'A'";
   
    $rsDadosForm = $db->carregar( $sql );
    
    $itmid 			   = $rsDadosForm[0]['itmid'];
    $estuf 		 	   = $rsDadosForm[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosForm[0]['estuf'];  
    $tipo 			   = 3;
    $apmid 			   = $_REQUEST['madid'];
    $tipoId			   = 'madid';   

}

if(!$_REQUEST['req']){
	
	// Cabeçalho - Abas
	if ($_REQUEST['friid']){
		$res = array(  array (  "descricao" => "Formação inicial",
								    "id" 		=> "0",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
						  		  )
							);
							
		$perfis = arrayPerfil();
		 		  
		if( in_array( PARIND_ADMINISTRADOR, $perfis ) || in_array( PARIND_SUPER_USUARIO, $perfis ) ){	
			array_push($res,
						array ("descricao" => "Etapas",
								    "id"        => "0",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoInicial&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
								   ),
						array ("descricao" => "Restrição",
								    "id"        => "1",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
								   ),
			   			array ("descricao" => "Documentos",
								    "id"		=> "2",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
						  		   ),
						array ("descricao" => "Vinculação a subações do PAR Genêrico",
								    "id"		=> "3",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
						  		   ),
						array ("descricao" => "Subações do PAR - Plano de Metas",
								    "id"		=> "4",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
						  		   )
				  );
		}else{
			array_push($res,
						array ("descricao" => "Etapas",
								    "id"        => "0",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoInicial&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
								   ),
						array ("descricao" => "Restrição",
								    "id"        => "1",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
								   ),
			   			array ("descricao" => "Documentos",
								    "id"		=> "2",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
						  		   ),
						array ("descricao" => "Subações do PAR - Plano de Metas",
								    "id"		=> "3",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
						  		   )
				  );
		}
		
		echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}");
				  
	} else if ($_REQUEST['madid']) {
		
		$res = array( 0 => array (  "descricao" => "Identificação do material",
									    "id" 		=> "0",
									    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadMaterial&acao=I&madid={$rsDadosForm[0]['madid']}"
							  		  )
								);
		
		array_push($res,
						array ("descricao" => "Etapas",
								    "id"        => "1",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaCadMaterial&acao=A&madid={$rsDadosForm[0]['madid']}"
								   ),
						array ("descricao" => "Restrição",
								    "id"        => "2",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&madid={$rsDadosForm[0]['madid']}"
								   ),
			   			array ("descricao" => "Documentos",
								    "id"		=> "3",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&madid={$rsDadosForm[0]['madid']}"
						  		   ),
						array ("descricao" => "Vinculação a subações do PAR Genêrico",
					    			"id"		=> "4",
					    			"link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&madid={$rsDadosForm[0]['madid']}&estuf={$_REQUEST['estuf']}"
			  		   				),
						array ("descricao" => "Subações do PAR - Plano de Metas",
								    "id"		=> "5",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosForm[0]['madid']}&estuf={$_REQUEST['estuf']}"
						  		   )
				  );
				  
		echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&madid={$rsDadosForm[0]['madid']}&estuf={$_REQUEST['estuf']}");
				  
	} else if ($_REQUEST['frcid']) {
		
		$res = array( 0 => array (  "descricao" => "Formação Continuada",
									    "id" 		=> "0",
									    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormContinuada&acao=I&frcid={$rsDadosForm[0]['frcid']}"
							  		  )
								);
		
		array_push($res,
						array ("descricao" => "Etapas",
								    "id"        => "1",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoContinuada&acao=A&frcid={$rsDadosForm[0]['frcid']}"
								   ),
						array ("descricao" => "Restrição",
								    "id"        => "2",
								    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&frcid={$rsDadosForm[0]['frcid']}"
								   ),
			   			array ("descricao" => "Documentos",
								    "id"		=> "3",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&frcid={$rsDadosForm[0]['frcid']}"
						  		   ),
						array ("descricao" => "Vinculação a subações do PAR Genêrico",
								    "id"		=> "4",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
			  		   			),
						array ("descricao" => "Subações do PAR - Plano de Metas",
								    "id"		=> "5",
								    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}"
						  		   )
				  );
				  
		echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&frcid={$rsDadosForm[0]['frcid']}&estuf={$_REQUEST['estuf']}");
	 
	}
}

$sql = "select a.sbaid, si.sbadsc from parindigena.associativaplanodemetasprogramas a 
		inner join cte.subacaoindicador si on si.sbaid = a.sbaid
		where appid = ".$apmid;
$subacoesVinculadas = $db->carregar($sql);
$cont = 0;
if(!$_REQUEST['sbaid']){
	if(is_array($subacoesVinculadas)){
		foreach ($subacoesVinculadas as $subacaoParInd){                                                                           
			$_REQUEST['sbaid'] = $subacaoParInd['sbaid'];
		}
	}
}





/********************* INCLUDES *********************/
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'www/cte/_funcoes.php';
include APPRAIZ . 'www/cte/_componentes.php';
include_once( APPRAIZ."cte/classes/SubacaoIndicador.class.inc" );
include_once( APPRAIZ."cte/classes/ComposicaoPessoa.class.inc" );


/********************* DECLARAÇÃO DE VARIAVEIS E CONSTANTES *********************/

$obSubacao = new SubacaoIndicador( $_REQUEST['sbaid'] ); 
define( 'CTE_NOVA_SUBACAO', trim($_REQUEST['sbaid']) == '');

$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$editavel           = true;
$undid 				= $obSubacao->recuperaUndid($_REQUEST['sbaid']);
$anoAtualTrava 		= 0;
$anoParecer2007 	= 0;
$anoParecer2008 	= 0;
$anoParecer2009 	= 0;
$anoParecer2010 	= 0;
$anoParecer2011 	= 0;
$anoConvenio 		= 0000;
$frmFinanceira 		= $_REQUEST['sbaid'];
$boEscolaAtiva 		= false;
$boProLetramento	= false;
$super 				= $db->testa_superuser() ? 1 : 2;
$adm 				= cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_SUPER_USUARIO)) ? 1 : 2;
$admTemp 				= cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRATOR_TEMP)) ? 1 : 2; // **@@&& -- retirar apos reformulação
$arUnidades 		= cte_recuperarArUnidadesExigemCPF();                                                                                 
							 
if (!$_REQUEST['anoExercicio']){
    $_REQUEST['anoExercicio'] 	= $_REQUEST['parsubacaoanocrt'] ? $_REQUEST['parsubacaoanocrt'] : date('Y');
   	$cosano 					= $_REQUEST['cosano'] ? $_REQUEST['cosano'] : $_REQUEST['anoExercicio'];
}


/********************* QUANDO NÃO FOR CADASTRO DE NOVA SUBAÇÃO. *********************/
if (!CTE_NOVA_SUBACAO) { 
	/**************** Verifica se a subção foi conveniada com o FNDE Se foi bloqueia o ano que foi conveniada ***************/
	$subConveniada 	= subacaoConveniada();
	$anosDoConvenio = array();
	if(is_array($subConveniada)){
		foreach( $subConveniada as $anoSubacoes ){
			$anosDoConvenio[] 		= $anoSubacoes["sbcano"];
			$ultimoAnoConveniado 	= $anoSubacoes["sbcano"];
		}
	}
	$anoConvenio = ($ultimoAnoConveniado == NULL) ? 0000 : $ultimoAnoConveniado;
	/**************** FIM Verifica se a subção foi conveniada com o FNDE ***************/
	
	/*******************************
	* 	REGRA DE NEGÓCIO DO PAR:  TRAVA ABA ANO SE O ANO FOR MENOR QUE O ANO CORRENTE.
	* 	
	* 	CHAMADA JAVASCRIPT	: function travaAnosAnteriores(cosano); (no arquivo: www/includes/par_subacao.js)
	* 	DESCRIÇÃO:
	*	Se estiver em uma aba onde o ano e menor que o ano atual, e a forma de execução da subação
	* 	for assistencia financeira, transferencia voluntaria ou assistencia tecnica com complementação financeira,
	*   a aba dos anos anteriores são travadas.
	*******************************/
	if($frmFinanceira){
		$anoAtualTrava 	= date('Y');
	}
	
	/*******************************
	* 	REGRA DE NEGÓCIO DO PAR:  TRAVA ABA ANO EM ELABORAÇÃO DO PAR E VALIDAÇÃO DO MUNICÍPIO.
	* 
	* 	DESCRIÇÃO:
	*	Se estiver em Elaboração do PAR ou em  Validação do Município e se o usuário estiver em alguma aba onde no ano tenha 
	*   o status da subação diferente de vazio Bloqueia a aba do ano da subação.
	*******************************/
	$sql =		"SELECT sptano 
				FROM cte.subacaoparecertecnico 
				WHERE sbaid = '".$_REQUEST['sbaid']."' 
				AND ssuid is not null";
	$anosParecerDados 	= $db->carregar($sql);
	$docid 				= cte_pegarDocid( $_SESSION['inuid'] );
	$documento 			= wf_pegarEstadoAtual( $docid );
	$total 				= count($anosParecerDados); // Anos que tem o parecer Técnico dado.
	
	for($i = 0; $i < $total; $i++){
		switch($anosParecerDados[$i]["sptano"]) {
			case '2007':
				$anoParecer2007 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2008':
				$anoParecer2008 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2009':
				$anoParecer2009 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2010':
				$anoParecer2010 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2011':
				$anoParecer2011 = $anosParecerDados[$i]["sptano"];
				break; 
		}
	}
	/****** FIM TRAVA ABA ANO EM ELABORAÇÃO DO PAR E VALIDAÇÃO DO MUNICÍPIO. ******/
	
	$parecer = existeParecerPorSubacao( $_REQUEST['sbaid'] );
   
    
    $arPpsid 		 = array( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047, 1064, 1065, 278, 272, 273, 274, 275, 276, 277, 1066 );
	$ppsid 			 = $obSubacao->recuperaPpsid($_REQUEST["sbaid"]);
	$boEscolaAtiva	 = ( in_array( $ppsid, $arPpsid ) && cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL );
	
    $arPpsidProLetramento = array( 1057, 1058, 1059, 1060, 1061, 1049, 1051, 1053, 1055, 1056, 904, 666, 1052, 1050, 1054, 348, 1062, 1063 );
	$boProLetramento = ( in_array( $ppsid, $arPpsidProLetramento ) && cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL );
}

/********************* DECLARAÇÃO DE CONSTANTES *********************/

$da = true;
$perfis = cte_arrayPerfil();
if( ( count($perfis == 1)) && ( in_array(CTE_PERFIL_CONSULTA_GERAL, $perfis ) 	|| 
								in_array(CTE_PERFIL_CONSULTA_ESTADUAL, $perfis ) || 
								in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) )){
	//não altera
	$da= false;
}


if($_REQUEST['sbaidpai']){ // **@@&& Se for aditivo - reformulação libera para edição
	$boDa = true;
}else{
	
	$boDa = $da && ( $boEscolaAtiva || $boProLetramento ); 
}
if($admTemp == 1){// **@@&& Se for aditivo - reformulação libera para edição
	$boDa = true;
}
/*
 * 
 */
if($_REQUEST['sbaid']){
	
	$sql = "select 
				s.sbaid, 
				a.ptoid, 
				ct.indid, 
				ppsid 
			from cte.subacaoindicador s
				inner join cte.acaoindicador a 			on s.aciid = a.aciid
				inner join cte.pontuacao pt 			on a.ptoid = pt.ptoid
				inner join cte.criterio ct 			on ct.crtid = pt.crtid
				--inner join cte.subacaoparecertecnico spt	on spt.sbaid = s.sbaid
			where s.sbaid in ('".$_REQUEST['sbaid']."')
			and ppsid in ( 1232, 1237, 1242, 1247, 1252, 1257, 1262, 1267, 1272, 1277, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291 )		
			order by s.sbaid desc";
	$rsFormacaoEscola = $db->carregar($sql);
	if( $rsFormacaoEscola[0]['ppsid'] > 1 &&  
		cte_possuiPerfil(array(CTE_PERFIL_EQUIPE_MUNICIPAL)) ||
		cte_possuiPerfil(array(CTE_PERFIL_EQUIPE_ESTADUAL_CADASTRO))) {	
		
		define( 'CTE_PRM_EDITAR_SUBACAO', 1);	
	} else {
		define( 'CTE_PRM_EDITAR_SUBACAO' , cte_podeEditarParecer( $_SESSION['inuid'] ) || $boDa );
	}
}else{
	define( 'CTE_PRM_EDITAR_SUBACAO' , cte_podeEditarParecer( $_SESSION['inuid'] ) || $boDa );
}


define( 'CTE_PRM_EXCLUIR_SUBACAO', CTE_PRM_EDITAR_SUBACAO);
define( 'CTE_PRM_EMITIR_PARECER' , CTE_PRM_EDITAR_SUBACAO && ($db->testa_superuser() ||
                                                            cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                   CTE_PERFIL_EQUIPE_TECNICA,
                                                                                   CTE_PERFIL_EQUIPE_ESCOLA_ATIVA,
                                                                                   CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN))));
                                                   
define('CTE_PRM_VISUALIZAR_PARECER', CTE_PRM_EMITIR_PARECER || cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                      CTE_PERFIL_EQUIPE_TECNICA,
                                                                                      CTE_PERFIL_EQUIPE_ESCOLA_ATIVA,
                                                                                      CTE_PERFIL_ALTA_GESTAO,
                                                                                      CTE_PERFIL_EQUIPE_LOCAL,
																					  CTE_PERFIL_EQUIPE_LOCAL_APROVACAO)));
																					  
define('CTE_PODE_EDITAR_PARECER', CTE_PRM_EMITIR_PARECER || cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                      CTE_PERFIL_EQUIPE_TECNICA,
                                                                                      CTE_PERFIL_EQUIPE_ESCOLA_ATIVA,
                                                                                      CTE_PERFIL_ALTA_GESTAO
                                                                                    )));		

if(CTE_PODE_EDITAR_PARECER){
	$disableParecer = 'S';
}else{
	$disableParecer = 'N';
}
							  
define('CTE_PRM_VISUALIZAR_PI', CTE_PRM_EDITAR_SUBACAO && cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                      CTE_PERFIL_ADMINISTRADOR )));

/*********************************************************************************************
*
*  	REGRA DE NEGÓCIO DO PAR:  ADITIVOS. | FUNÇÕES AJAX
*
**********************************************************************************************/
	/************************************************************	
	* 	CARREGAR ADITIVOS
	* 	(FUNÇÃO AJAX)
	* 	REQUEST				: carregaAditivos
	*   CHAMADA JAVASCRIPT	: function carregarAditivos(cosano)
	* 	DESCRIÇÃO: 
	*   Carrega lista de Aditivos da subação por aba.
	*************************************************************/
	if($_REQUEST['req'] == 'carregaAditivos'){//lista de Aditivos
			header('Content-type: text/html;charset=ISO-8859-1');
			if(!$_REQUEST['sbaid']){
				$nrSbaid = 0;
			}else{
				$nrSbaid = $_REQUEST['sbaid'];
			}
			
			$sql=	'SELECT distinct
			\'<center><img onclick="return irAditivo(\' || si.sbaid || \',\' || si.sbaanoaditivo || \')" src="/imagens/consultar.gif" style="border:none" title="Editar aditivo." alt="Ir para o aditivo" /></center>\' as sbaid,
			 \'Aditivo \' || si.sbaseqaditivo as texto,  
			 to_char(si.sbadata, \'dd/mm/yyyy\') AS data, 
			 prsnumeroprocesso
				FROM cte.subacaoindicador si
				LEFT JOIN cte.projetosapesubacao sc on sc.sbaid = si.sbaid
				LEFT JOIN cte.projetosape c on sc.prsid = c.prsid
				WHERE sbaidpai = '.$nrSbaid .'  
				AND sbaanoaditivo = '.$cosano;
		//echo $sql;
		$existeListaAditivo = $db->carregar($sql); // Existe lista de Aditivos.
		$anoAba = $_REQUEST['cosano'] ? $_REQUEST['cosano'] : 0000; // Ano da aba atual.
		
		if(is_array($anosDoConvenio)){
			foreach($anosDoConvenio as $dados){
				//if($dados == $anoAba && !$_REQUEST['ad'] ){ //**@@&& ALTERAR DEPOIS PARA ESTE QUANDO ACABAR A REFORMULAÇÃO
				if($dados >= $anoAba && !$_REQUEST['ad'] ){ 
					if( ($anoConvenio != 0000) || ($existeListaAditivo != false) ){//Se existe aditivo 0000 quer dizer que não tem aditivo.
						if($existeListaAditivo != false){
							$listaAditivos = '<div style="border-bottom: 2px white solid;"><b><a href="" onclick="extratoSubAcacao();" title="Extrato completo da Subação + seus Aditivos." >Relatório completo</a></b></div>';
						}
						$listaAditivos .= '<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Lista de Aditivos</div>'; 
						echo $listaAditivos;
						$db->monta_lista_simples( $sql, array('Ações','Convênios', 'Data de criação', 'Número de Processo '), 1000, 1000, 'N', '100%','N' );
					}
				}			
			}
		}	
			
		die();
	}
	
	/************************************************************	
	* 	VOLTA PARA SUAÇÃO PAI
	* 	(FUNÇÃO AJAX)
	* 	REQUEST				: voltarsubacaooriginal
	*   CHAMADA JAVASCRIPT	: function voltarSubacaoPai(sbaidPai)
	* 	DESCRIÇÃO: 
	*   Volta para subação pai (Estando na subação aditivo)
	*************************************************************/
	if($_REQUEST['req'] == 'voltarsubacaooriginal'){
		$sbaidOrigianl = $_REQUEST['sbaidpai'];
		$_REQUEST['sbaidpai'] = NULL;
		$_REQUEST['anoconvenio'] = NULL;
		echo $sbaidOrigianl;
		die();
	}

	//FIM ADITIVOS.

	
	/*******************************
	* 	REGRA DE NEGÓCIO DO PAR:  BLOQUEAR CADASTRO DE ITENS DE COMPOSIÇÃO E BENEFICIÁRIOS
	* 	
	*	As subações que possuem Formas de Execução de Assistência Técninca, 
	*	Subações que possuem Unidades de Medida que não exigem CPF (Ex: Professores Cursistas)
	* 	não podem inserir Itens de Composição e Beneficiários.
	*******************************/	
	
	$boUnidadeExigeCPF = in_array( $obSubacao->undid, $arUnidades );
	$boExecucaoTecnica = cte_possuiFormaExecucaoTecnica( $obSubacao->frmid );
	
	$verificaCPF = $boUnidadeExigeCPF ? "dados dos cursistas" : "itens de composição";	

	$boInsereItensEBeneficiarios = ( !$boUnidadeExigeCPF && $boExecucaoTecnica ) ? false : true;


//!@------------------------------------------------------------- FUNCOES AJAX

/************************************************************************
*		   LISTAGEM, EXCULSÃO DE ITENS-INDICADOR E TOTALIZADORES		*
************************************************************************/
$sql="select pssano as sbcano from cte.projetosapesubacao where sbaid = ".trim( $_REQUEST['sbaid'] );

if( array_key_exists('req', $_REQUEST ) ) {

    if (trim($_REQUEST['sbaid']) == '') {
        exit;
    }

    // Plano Interno
    if( $_REQUEST['req'] == 'prgplanointerno' ){
        echo $db->pegaUm('SELECT sptplanointerno FROM cte.subacaoparecertecnico WHERE sbaid = '.$_REQUEST['sbaid']);
    }
    // Carregar Itens de Composição
    elseif( $_REQUEST['req'] == 'carregarItensComposicao' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

        if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica  ){
        	$sql = array();
	        $sql = montarSQLItensComposicaoCPF();
	        $db->monta_lista_simples( $sql, array( 'CPF', 'Nome', 'Vínculo', 'Formação'), 1000, 1000, 'N', '100%','N' );
        }
        else{
       
	        $sql = montarSQLItensComposicao();
	        $db->monta_lista_simples( $sql, array( 'Identificação do Item', 'Un. de Medida', 'Qtd', 'Valor Unitário',
	                                              'Total' ), 1000, 1000, 'N', '100%','N' );
        }
		
        $totalItensC = count($sql);
        echo '<input type=hidden name="totalitensc" id="totalitensc" value="'.$totalItensC.'" />';
        echo '<input type=hidden name="anoRef" id="anoRef" value="'.$_REQUEST['cosano'].'" />';
        
    }
    // Carregar Beneficiários
    elseif ($_REQUEST['req'] == 'carregarBeneficiarios' && $_REQUEST['sbaid'] != '' && $_REQUEST['sabano'] != '' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

        $sql = montarSQLBeneficiarios();
        $db->monta_lista_simples($sql, array( 'Beneficiário', 'Zona Urbana', 'Zona Rural',
                                             'Total'), 1000, 1000, 'S', '100%','N');

    } 
    // Carregar Escolas
    elseif( $_REQUEST['req'] == 'carregarEscolas' && $_REQUEST['sbaid']  != '' && $_REQUEST['qfaano'] != '' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

		$arEscolas = montarSQLEscolas();		
        $resp = $db->monta_lista_simples($arEscolas, array('Ações', 'Código INEP', 'Escola', 'Quantidade'), 
        									 	   5000, 5000, 'N', '100%','N');
        									 	   
    }
    // Carregar dados do Parecer
    elseif( $_REQUEST['req'] == 'carregarDadosParecer' ){
    	
        $sql = '
		        SELECT
		            coalesce(spt.sptparecer, \'\')      as sptparecer,
		            coalesce(spt.sptunt, 0)             as sptunt,
		            coalesce(spt.sptuntdsc, \'\')       as sptuntdsc,
		            coalesce(spt.sptano, 0)             as sptano,
		            coalesce(spt.sptinicio, 0)          as sptinicio,
		            coalesce(spt.sptfim, 0)             as sptfim,
		            coalesce(spt.sptanoterminocurso, 0) as sptanoterminocurso,
		            coalesce(spt.sptplanointerno, \'\') as plinumplanointerno,
		            coalesce(spt.sptnumprocesso, \'\') as sptnumprocesso,
		            coalesce(spta.ssudescricao, \'\')              as ssudescricao,
		            coalesce(spt.ssuid, 0)              as ssuid
		        FROM cte.subacaoparecertecnico spt
		        	INNER JOIN cte.subacaoindicador sba on spt.sbaid = sba.sbaid
		        	left join cte.statussubacao spta on spta.ssuid = spt.ssuid 
		        WHERE spt.sbaid  = ' . $_REQUEST['sbaid'] . '
				AND spt.sptano = ' . $_REQUEST['sbtano'];

        $res = $db->pegaLinha($sql);

        header('content-type: text/xml; charset=ISO-8859-1');
        echo '<?xml version="1.0" encoding="ISO-8859-1"?>' , "\n"
            ,'<response>'
            ,'<sptparecer><![CDATA[', $res['sptparecer']         , ']]></sptparecer>'
            ,'<ssudescricao><![CDATA[', $res['ssudescricao']         , ']]></ssudescricao>'
            ,'<sptunt>'             , $res['sptunt']             , '</sptunt>'
            ,'<sptuntdsc><![CDATA[' , $res['sptuntdsc']          , ']]></sptuntdsc>'
            ,'<sptano>'             , $res['sptano']             , '</sptano>'
            ,'<sptinicio>'          , $res['sptinicio']          , '</sptinicio>'
            ,'<sptfim>'             , $res['sptfim']             , '</sptfim>'
            ,'<sptanoterminocurso>' , $res['sptanoterminocurso'] == 0 ? '' : $res['sptanoterminocurso'] , '</sptanoterminocurso>'
            ,'<plinumplanointerno>'	, $res['plinumplanointerno']    , '</plinumplanointerno>'
            ,'<sptnumprocesso>'		, $res['sptnumprocesso']    , '</sptnumprocesso>'
            ,'<ssuid>'              , $res['ssuid']              , '</ssuid>'
            ,'</response>';

    }
    die();
} // Fim de if( array_key_exists('req', $_REQUEST ) )

if( $_REQUEST["parecerPadrao"] && $_REQUEST["sbaid"] != ''){ // Carrega o Parecer Padrão
	
	$sql = "SELECT ppsparecerpadrao FROM cte.proposicaosubacao ps
				INNER JOIN cte.subacaoindicador sba on sba.ppsid = ps.ppsid 
			WHERE sbaid = ". $_REQUEST["sbaid"];
	
  	echo $db->pegaUm( $sql );
	
	die();
}



include APPRAIZ . "includes/registraracesso.php";

$podeExcluirSubacao = cte_podeEditarSubacao($_SESSION["inuid"]);
$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$capturaTipo        = cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

//-------------------------------------------------- SUBAÇÕES - CARGA DE DADOS
$sql = '
select
    s.sbaid,
    s.aciid,
    s.undid,
    s.frmid,
    s.sbadsc,
    s.sbastgmpl,
    s.sbaprm,
    s.sbapcr,
    s.sbadata,
    s.usucpf,
    s.sbaparecer,
    s.psuid,
    s.ssuid,
    s.foaid,
    s.prgid,
    s.sbaporescola,
    s.ppsid,
    s.sbaordem,
    s.sbaobjetivo,
    s.sbatexto,
    s.sbacategoria
from
    cte.subacaoindicador s
left join
    cte.programa p on p.prgid = s.prgid
where
    s.sbaid = ' . (integer) $_REQUEST['sbaid'];

$sai = array_map('sql2html', (array) $db->pegaLinha($sql));
extract($sai);


$sbaporescola            = $sbaporescola == 't';
$existemEscolasAtendidas = $db->pegaUm('SELECT count(*) FROM cte.qtdfisicoano WHERE sbaid = ' . (integer) $_REQUEST['sbaid']) > 0;

$sql = '
select
    dim.dimid,
    dim.dimcod,
    dim.dimdsc,
    are.ardid,
    are.ardcod,
    are.arddsc,
    ind.indid,
    ind.indcod,
    ind.inddsc,
    aci.acidsc,
    aci.acilocalizador,
    pto.ptoid
from
    cte.acaoindicador aci
inner join
    cte.pontuacao pto on pto.ptoid = aci.ptoid and pto.ptostatus = \'A\'
inner join
    cte.indicador ind on ind.indid = pto.indid
inner join
    cte.areadimensao are on are.ardid = ind.ardid
inner join
    cte.dimensao dim on dim.dimid = are.dimid
where
    aci.aciid = ' . (trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid']);

$aci = (array) $db->pegaLinha($sql);
extract($aci);

$_SESSION['indid'] = $indid;
$aciid             = trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid'];

if($_REQUEST['sbaidpai']){ // Se o usuario estiver em um Aditivo. (Então tem SbaidPai)
	$anoAditivo =  $_REQUEST['anoconvenio'];
	$_cosano = cte_recuperArArAno($anoAditivo); // Mostra aba apenas do ano do aditivo.
}else{
	$_cosano = cte_recuperArArAno(); // mostra todos os anos.
}
?>

<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR - Plano de Metas - Subação<?php echo " - " , $titulo; ?></title>
	
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <link rel="stylesheet" type="text/css" href="../cte/includes/par_subacao.css"/>
	    
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	   
	    <script type="text/javascript">
	    function getElementText(tag, parentElem, index)
	    {
	        if (!index) {
	            index=0;
	        }
	
	        var result = parentElem.responseXML.getElementsByTagName(tag)[index];
	
	        if (result) {
	            if (result.childNodes.length == 0) {
	                return '';
	            } else if (result.childNodes.length > 1) {
	                return result.childNodes[1].nodeValue;
	            } else {
	                return result.firstChild.nodeValue;
	            }
	        } else {
	            return '';
	        }
	    }
	    </script>
	</head>
	<body>
		<div id="loader-container">
	    	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
	    </div>
	    <div>
	    <table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 5px;">
	    <tr style="background-color: #cccccc;">
   		<td>Lista de Subações vinculadas ao Programa.</td>
   		</tr>
   	<tr>
   		<td>
	    <?php 
	    if(is_array($subacoesVinculadas)){
	    foreach ($subacoesVinculadas as $subacaoParInd){                                                                           
			if(!$_REQUEST['req']){
?>	
<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 5px;">
   	<tr style="background-color: #cccccc;">
   		<td>
		<a href="parindigena.php?modulo=principal/subacoesPAR&acao=A&<?php echo $tipoId."=".$rsDadosForm[0][$tipoId];?>&estuf=<?php echo $_REQUEST['estuf'];?>&sbaid=<?php echo $subacaoParInd['sbaid'];?>" ><?php echo $subacaoParInd['sbadsc']; ?></a>
		</td>
	</tr>
</table>
<?php 
			}
		}
	   }else{
?>
	   	<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 5px;">
   	<tr style="background-color: #cccccc;">
   		<td>
		Não existem subações de referência ao PAR - Plano de Metas.
		</td>
	</tr>
</table>
<script type="text/javascript">
$('loader-container').hide();
</script>
<?php
	
die(); 
	   }
?>
</td></tr></table>
	    </div>
		<div id="container">
			<?php cte_montaTitulo( $titulo_modulo, '' ); ?>
	
			<form method="post" name="frmParSubacao" id="frmParSubacao" action="<?php echo $_SERVER['REQUEST_URI']; ?>">
				<input type="hidden" name="formularioSumit" value="1"/>
				<input type="hidden" name="prgidteste" id="prgidteste" value="<?php echo $prgid ?>"/>
				<input type="hidden" name="ppsid" id="ppsid" value="<?php echo $ppsid ?>"/>
				<input type="hidden" name="aciid" id="aciid" value="<?php echo $aciid ?>"/>
				<input type="hidden" name="sbaid" id="sbaid" value="<?php echo $sbaid ?>"/>
				<input type="hidden" name="sbaordem" id="sbaordem" value="<?php echo $sbaordem ?>"/>
				<input type="hidden" name="anoExercicio" id="anoExercicio" value="0"/>
				<input type="hidden" name="sbaobjetivo" id="sbaobjetivo" value="<?php echo $sbaobjetivo ?>"/>
				<input type="hidden" name="sbaporescola" id="sbaporescola" value="<?php echo $sbaporescola ? 'true' : 'false' ?>"/>
	
				<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
					<colgroup>
						<col width="25%" />
						<col width="75%" />
					</colgroup>
	
					<tbody>
						<tr>
							<td class="SubTituloDireita">Dimensão:</td>
							<td><?php echo $aci['dimcod'] , '. ' , $aci['dimdsc'] ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Área:</td>
							<td><?php echo $aci['dimcod'] , '.' , $aci['ardcod'] , '. ' , $aci['arddsc']; ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Indicador:</td>
							<td>
								<?php echo $aci['dimcod'] , '.' , $aci['ardcod'] , '.' , $aci['indcod']   , '. ' , $aci['inddsc']; ?>
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Demanda:</td>
							<td><?php echo $aci['acilocalizador'] == "E" ? "Rede Estadual" : "Rede Municipal"; ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Ação:</td>
							<td><?php echo $aci['acidsc']; ?></td>
						</tr>
	
						<?php

						if( !CTE_NOVA_SUBACAO ){
	
							$select_formaAtendimento = $db->pegaUm('SELECT foadsc FROM cte.formaatendimento WHERE foaid = ' . (integer) $foaid);
							$select_programa         = $db->pegaUm('SELECT prgdsc FROM cte.programa WHERE prgid = '         . (integer) $prgid);
							$select_unidadeMedida    = $db->pegaUm('SELECT unddsc FROM cte.unidademedida WHERE undid = '    . (integer) $undid);
								
							$select_formaExecucao    = $db->pegaUm('SELECT frmdsc FROM cte.formaexecucao WHERE frmbrasilpro = false and  frmid = '    . (integer) $frmid);
						 ?>
																	
							<tr style="background-color: #cccccc;">
								<td align="left" colspan="2">
									<strong>Dados da Subação</strong>
								</td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Forma de atendimento:</td>
								<td><?php echo $select_formaAtendimento; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Descrição da Subação:</td>
								<td><?php echo $aci['dimcod'].".".$aci['ardcod'].".".$aci['indcod']." - $sbaordem ".$sbadsc; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
								<td><?php echo $sbastgmpl; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Programa:</td>
								<td><?php echo $select_programa; ?></td>
							</tr>
							<tr>
								<td align="right" class="SubTituloDireita">Unidade de Medida:</td>
								<td><?php echo $select_unidadeMedida; ?></td>
							</tr>
							<tr>
								<td align="right" class="SubTituloDireita">Forma de Execução:</td>
								<td><?php echo $select_formaExecucao ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
								<td><?php echo $sbapcr; ?></td>
								</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Cronograma:</td>
								<td><?php echo !$sbaporescola ? 'Global' : 'Por Escola'; ?></td>
							</tr>
							<tr>
								<td align="right" class="SubTituloDireita">&nbsp;</td>
								<td id="TdEditarSubacao"  bgcolor="#dddddd"><input type="button" name="BtnEditarSubacao" id="BtnEditarSubacao" value="Editar" onclick="popupEditarSubacao();" /></td>
							</tr>
					
						<?php
						} // Fim de !CTE_NOVA_SUBACAO 
	
						$colspan       = sizeof($_cosano) + 1;
						$tamanhoCelula = round(100 / $colspan);
						// Mostrar apenas 1 aba do Aditivo
						if(count($_cosano) == 1){
							$colspan = 4;
						}
						?>
	          		</tbody>
	        	</table>
	
				<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" style="display:none" id="alertaPermitirCadastroItensSubacao">
					<tbody>
						<tr style="background-color: #cccccc;">
							<td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
								<strong>Finalize o cadastro da subação para que escolas, itens, beneficiários e parecer possam ser registrados.</strong>
							</td>
						</tr>
						<tr>
							<td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
								<input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar" value="Gravar" id="btnSalvar" <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
								<input onclick="return fecharJanela();" type="button" name="btnCancelar" value="Fechar" id="btnCancelar" />
							</td>
						</tr>
					</tbody>
				</table>
				<center>
				<?//=$stAlertConcluido;?>
				</center>
				<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano">
					<tbody>
						<tr style="background-color: #cccccc;">
							<td align="left" colspan="<?php echo $colspan; ?>" style="padding: 5px; font-size: 110%">
								<strong>Detalhamento dos Itens que Compõem a Especificação - <span id="spanAno"></span></strong>
							</td>
						</tr>
				
						<tr id="abasSelecaoCosano">
							<?php 
							
							foreach( $_cosano as $ano ){
						        $i = $ano;
						        if( $_REQUEST['ad'] == 1){ // se for aditivo escreve na aba de ano a var '$escreve'.
						        	$tamanhoCelula = 45;
						        	$sql="	select sbaseqaditivo 
						        			from cte.subacaoindicador 
						        			where sbaid = ".$_REQUEST['sbaid']." and sbaanoaditivo = ".$ano;
						        	$numAditivo = $db->pegaUm($sql);
						        	$escreve = " Aditivo ".$numAditivo." / ";
	
						        }
						        echo '<td id="aba_' , $i , '" style="width: ' , $tamanhoCelula , '%;"><a onclick="return selecionarAba(' , $i , ', true);" href="' , $_SERVER['REQUEST_URI'] , '">' ,$escreve.$i , '</a></td>' , "\n";
					 		}// foreach ($_cosano as $ano)
					 		if(count($_cosano) != 1){
					 		?>
							<?php }else{ ?>
							<td width="90%" style="background: none; border:none;"></td>
							<td width="90%" style="background: none; border:none;"></td>
							<td width="90%" style="background: none; border:none;"></td>
							<?php } ?>
						</tr>
	
						<?php foreach( $_cosano as $ano ){
							$i = $ano; 
							?>
								
							<tr class="container" style="display: none; height: auto; vertical-align: top" id="abaCosano<?php echo $i; ?>">
								<td align="center" colspan="<?php echo $colspan; ?>">
									<table cellpadding="0" cellspacing="0" width="100%">
										<tr id="areaListaAditivo<?=$i; ?>" > <!-- Lista de Aditivo -->
											<td id="divListaAditivo<?php echo $i; ?>" colspan="3" style="padding:5 5 5 5;">
											</td>
										</tr>
										<?php if($prgid && CTE_PRM_VISUALIZAR_PI){ ?>
										<tr>
											<td class="SubtituloEsquerda" style="padding: 5px; text-align: center" class="SubTituloDireita">
												Plano Interno
											</td>
										</tr>
										<tr>
											<td class="SubtituloEsquerda" style="padding: 5px; text-align: center" >
												<table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
														<tr>
															<td class="SubTituloDireita"  style="padding-left: 42px;" >Plano Interno:</td>
															<td style="background-color: #fff">
																<?php
																// RECUPERA OS PLANOS INTERNOS
																$sqlPlanos = "	SELECT distinct plinumplanointerno as codigo, plinumplanointerno as descricao 
																				FROM  cte.programa p
																				INNER JOIN cte.planointerno pi ON pi.prgid = p.prgid
																				WHERE p.prgid = ".$prgid." AND pliano = ".$i." 
																				ORDER BY plinumplanointerno";
																$planos = $db->carregar($sqlPlanos);
																if($planos){
																	$totalPlanos = count($planos);
																	echo '<input type="hidden" name="semplanointerno'.$i.'" id="semplanointerno'.$i.'" value="true"/>';
																	if($totalPlanos > 1){
																		$db->monta_combo(	'plinumplanointerno_' . $i,
																							$sqlPlanos,
																							'S', 'Selecione', '', '', '', '', '', 'plinumplanointerno_' . $i );
																	}else{
																		$db->monta_combo(	'plinumplanointerno_' . $i,
																							$sqlPlanos,
																							'S', '', '', '', '', '', '', 'plinumplanointerno_' . $i );
																	}
																}else{
																	$db->monta_combo(	'plinumplanointerno_' . $i,
																							$sqlPlanos,
																							'N', 'Não existe plano Interno cadastrado para este programa neste ano.', '', '', '', '', '', 'plinumplanointerno_' . $i );	
																	echo '<input type="hidden" name="semplanointerno'.$i.'" id="semplanointerno'.$i.'" value="false"/>';
																}
																?>							
							
														</tr>
												</table>				
											</td>
										</tr>
										<tr>
											<td class="SubtituloEsquerda" style="padding: 5px; text-align: center" >
												<table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
														<tr>
															<td class="SubTituloDireita" >Número de Processo:</td>
															<td style="background-color: #fff">
																<?php
																// RECUPERA OS NÚMEROS DE PROCESSO DO MUNICÍPIO / ESTADO
																$sqlNumProc = "	SELECT DISTINCT cvrnumprocesso as codigo, cvrnumprocesso as descricao FROM cte.convenioretorno 
																				WHERE inuid = ".$_SESSION['inuid']." ORDER BY cvrnumprocesso";
																$numProcessos = $db->carregar($sqlNumProc);
																if($numProcessos){
																	$totalNumProcessos = count($numProcessos);
																	echo '<input type="hidden" name="semnumprocesso'.$i.'" id="semnumprocesso'.$i.'" value="true"/>';
																	$db->monta_combo(	'cvrnumprocesso_' . $i,
																						$sqlNumProc,
																						'S', 'Novo Convênio', '', '', '', '', '', 'cvrnumprocesso_' . $i );
																}else{
																	$db->monta_combo(	'cvrnumprocesso_' . $i,
																							$sqlNumProc,
																							'N', 'Não existe Número de processo para esta Entidade', '', '', '', '', '', 'cvrnumprocesso_' . $i );	
																	echo '<input type="hidden" name="semnumprocesso'.$i.'" id="semnumprocesso'.$i.'" value="false"/>';
																}
																?>							
							
														</tr>
												</table>				
											</td>
										</tr>
										<?php } ?>
										<tr>
											<td style="background-color: #ddd">
												<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Quantidades e Cronograma Físico</div>
												<table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
													<?php if (!$sbaporescola) { ?>
														<tr id="cronogramaGlobal<?php echo $i; ?>">
															<td class="SubTituloDireita">Quantidades:</td>
															<td style="background-color: #fff"><div id="sptunt_<?php echo $i; ?>"></div></td>
														</tr>
													<?php } // Fim de if (!$sbaporescola) ?>
													<tr>
														<td class="SubTituloDireita">Cronograma Físico:</td>
														<td style="background-color: #fff; padding-left: 2px">
															<div id="cronograma_<?php echo $i; ?>"></div>
														</td>
													</tr>
												</table>
												<br />
											</td>
										</tr>
		
										<?php if ($sbaporescola) { ?>
											<tr id="cronogramaPorEscola<?php echo $i; ?>">
												<td>
													<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Escolas</div>
													<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="escolasSubAcao<?php echo $i; ?>"></div>
												</td>
											</tr>
										<?php } // if ($sbaporescola) ?>
										
										<?php if( $boInsereItensEBeneficiarios ){ ?>
											<tr>
												<td>
													<div class="SubtituloEsquerda" style="padding: 5px; text-align: center"><?=ucwords($verificaCPF);?></div>
													<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="itensComposicaoSubAcao<?php echo $i; ?>"></div>
												</td>
											</tr>
											<tr id="beneficiarioSubAcao<?php echo $i; ?>">
												<td>
													<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Beneficiários</div>
													<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="beneficiariosSubAcao<?php echo $i; ?>"></div>
												</td>
											</tr>
										<?php } ?>
		
										<tr>
											<td>
												<table id="parecerEquipeTecnica<?php echo $i; ?>" border="0" align="center" cellpadding="0" cellspacing="0" width="100%">
													<colgroup>
														<col width="25%" />
														<col width="75%" />
													</colgroup>
													<tr>
														<td align="right" class="SubTituloDireita">Detalhamento/Comentário:</td>
														<td><?php echo campo_textarea('sptuntdsc_'  . $i, 'N', 'N' , '', 100, 10, null ); ?></td>
													</tr>
		
													<?php if (CTE_PRM_VISUALIZAR_PARECER) { ?>
														<tr>
															<td colspan="2" style="text-align: center;font-weight:bold">Parecer Equipe Técnica</td>
														</tr>
														
														<?php if($_REQUEST["sbaid"] != ''){
															$sql = "SELECT ppsparecerpadrao 
																	FROM cte.proposicaosubacao ps
																		INNER JOIN cte.subacaoindicador sba on sba.ppsid = ps.ppsid 
																	WHERE sbaid = ". $_REQUEST["sbaid"];
																					
															$res = $db->pegaUm( $sql );
																				  	
															if( $res ){ ?>
																
															<?php }
														} // Fim de if($_REQUEST["sbaid"] != '') ?>
														
														<tr>
															<td align="right" class="SubTituloDireita">Parecer:</td>
															<td><?php echo campo_textarea('sptparecer_' . $i, 'N', 'N', '', 100, 12, 0 ); ?></td>
														</tr>
														
														<tr>
															<td align="right" class="SubTituloDireita">Status Subação:</td>
															<td>
																<div id="ssuid_<?php echo $i; ?>"></div>
															</td>
														</tr>
		
													<?php } // Fim de if (CTE_PRM_VISUALIZAR_PARECER) ?>
												</table><br />
											</td>
										</tr>
										
										<?php 
										if (!CTE_PRM_EMITIR_PARECER) {
											echo '<script type="text/javascript">$("sptparecer_' , $i , '","ssuid_' , $i , '").each(function(e){if(e)e.disable()});</script>';
										} ?>
										
									</table>
								</td>
							</tr>
						<?php } // foreach ($_cosano as $ano) ?>
						
					</tbody>
				</table>
			</form>
		</div>
	</body>

	<?php
	if (function_exists('uniqid'))
    	$reqt = md5(rand() . time() . uniqid());
	else
    	$reqt = md5(rand() . time() . rand());

	if ($undid) {
		$sql = 'SELECT il.labid
	    		FROM cte.laboratorio l
	    			INNER JOIN cte.itenslaboratorio il ON l.labid = il.labid
	    		WHERE l.undid = ' . $undid;

	    if( ( $labid = $db->pegaUm( $sql ) ) === false )
	        $labid = 'null';
	} 
	else {
	    $labid = 'null';
	}
	?>
	
	<script type="text/javascript">
		// Variaveis public & global //
  		var IExplorer               = !!document.all;
	    var safari                  = navigator.userAgent.indexOf( 'Safari' ) > 0;
	    var gecko                   = navigator.product == 'Gecko';
	    var _displayBlock           = IExplorer ? 'block' : 'table-row';
	
	    var TAB_TOTALIZADORES       = '0000';
	    var existemEscolasAtendidas = <?php echo $sbaporescola && $existemEscolasAtendidas ? 'true' : 'false' ?>;
	    var anoExercicio            = '<?php echo ($_REQUEST['parsubacaoanocrt'])?$_REQUEST['parsubacaoanocrt']:date('Y');  ?>';
	    var permitirCadastroSubacao = <?php echo trim($_REQUEST['sbaid']) != '' ? 'true' : 'false'; ?>;
	
	    var itemModificado          = false;
	
	    var totalizadorItensComposicao = 0;
	    var totalizadorBeneficiarios   = 0;
	    var totalizadorEscolas         = 0;
	    var labid                      = <?php echo $labid; ?>;
	    var total                      = 0;
	    
	    var anoAtualTrava 	= <?php echo $anoAtualTrava;?>;
	    var anoParecer2007 	= <?php echo $anoParecer2007;?>;
	    var anoParecer2008 	= <?php echo $anoParecer2008;?>;
	    var anoParecer2009 	= <?php echo $anoParecer2009;?>;
	    var anoParecer2010 	= <?php echo $anoParecer2010;?>;
	    var anoParecer2011 	= <?php echo $anoParecer2011;?>;
	    var anoConvenio 	= <?=$anoConvenio;?>;
	    var novaSub 		= <?php echo $novaSub = CTE_NOVA_SUBACAO ? 'true' : 'false'; ?>;
	    
	    var reqt 		= '<?php echo $reqt; ?>';
	    var porEscola 	= '<?php echo $sbaporescola ? 't' : 'f'; ?>';
	    var subacao		= <?php echo $_REQUEST['sbaid'] ? $_REQUEST['sbaid'] : 0;  ?>;
	    var eAditivo	= <?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>;
	    var PIVisivel	= <?php echo CTE_PRM_VISUALIZAR_PI ? CTE_PRM_VISUALIZAR_PI : 0; ?>;


	     function extratoSubAcacao()
    {
        return windowOpen('/cte/cte.php?modulo=principal/relatorioParSubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>',
                          'extratoSubacao',
                          'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }
	    
	     function carregarAditivos(cosano)
	    {	
	        return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&reqt='+ reqt +'&req=carregaAditivos&cosano=' + cosano,
	                                    asynchronous: false,
	                                    onComplete: function(res)
	                                    {	
	                                      // Controle do que deve ser travado de acordo com a situação da subação.
	                                      
	                                        $('divListaAditivo' + anoExercicio).innerHTML = res.responseText; // Mostra lista de Aditivos
	                                        $('loader-container').hide();
	                                    }
	                                });
	    }
		<?php
		//------------------------- FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA

		if ($sbaporescola) {
    		
    	?>
			    



    /**
     * 
     */
    function carregarEscolas()
    {
		$('escolasSubAcao' + anoExercicio).style.height = "auto";
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&reqt=<?php echo $reqt; ?>&req=carregarEscolas&qfaano=' + anoExercicio,
                                    onComplete: function(res)
                                    {
                                        $('escolasSubAcao' + anoExercicio).innerHTML = res.responseText;
                                        
                                        
                                        var totalEscolas = $('somaTotalEscolas'+ anoExercicio ).innerHTML;
                                        
                                        if( totalEscolas > 20 ){
	                                        $('escolasSubAcao' + anoExercicio).style.height = "500px";
                                        }
                                        else{
	                                        $('escolasSubAcao' + anoExercicio).style.height = "auto";
                                        }
                                        $('loader-container').hide();
                                    }
                                });
    }


    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    {	
        if (itemModificado) {
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {validacao: 'false'},
                onComplete: function()
                {
                    $('loader-container').hide();
                }
            });
        } else {
            itemModificado = false;
        }

        $('loader-container').show();
        var celula;
        var tabela;
        var totalizadores = cosano == '0000';

        // Última selecionada
        
        $('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';

        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
            $('loader-container').hide();
        } else {
<?php
// Desabilita o campo Detalhamento/Comentarios para formas de execução
// forem "EXECUTADA PELO MUNICIPIO" ou "ASSISTENCIA TECNICA DO MEC"
if ($frmid == 4 || $frmid == 10) {
    echo 'var sptuntdsc = $(\'sptuntdsc_\' + anoExercicio);'
        ,'if (sptuntdsc) sptuntdsc.up(2).hide();';
}
?>

            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarEscolas();
                carregarDadosParecer();
            
                carregarAditivos(cosano);

                if( ( '<?php echo $boEscolaAtiva ?>' || '<?php echo $boProLetramento ?>' ) && cosano < 2010 ){
                	if(<?=$adm;?> != 1){
                		bloquearDados( cosano );
                	}
                }                
                else{
	                if( ( '<?php echo $boEscolaAtiva ?>' || '<?php echo $boProLetramento ?>' ) && cosano > 2009  ){
	                	var dadosForms 	= $('frmParSubacao').elements;
						var tamanho 	= $('frmParSubacao').elements.length;
	                	for( i=0; i<tamanho; i++ ){
							if(dadosForms[i].type == "button"){
								if( dadosForms[i].id != "btnAnterior" && dadosForms[i].id != "btnProximo" && dadosForms[i].id != "btnVoltar" ){
									dadosForms[i].disabled = "";
								}
							}
						}
	                }
                }
                
            }

            $('spanAno').innerHTML = cosano;
        }

        return false;
    }
//------------------------ ! Fim das FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA
<?
} else {

?>
    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    { 
        if (itemModificado) {
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {validacao: 'false'},
                onComplete: function()
                {
                    $('loader-container').hide();
                }
            });
        } else {
            itemModificado = false;
        }

        $('loader-container').show();
        var celula;
        var tabela;
        var totalizadores = cosano == '0000';

        // Última selecionada
       
        $('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.color              = '#333';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';
        $('aba_'      + cosano)      .firstChild.style.color              = '#333';
		
        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
            $('loader-container').hide();
        } else {
<?php
// Desabilita o campo Detalhamento/Comentarios para formas de execução
// forem "EXECUTADA PELO MUNICIPIO" ou "ASSISTENCIA TECNICA DO MEC"
if ($frmid == 4 || $frmid == 10) {
    echo 'var sptuntdsc = $(\'sptuntdsc_\' + anoExercicio);'
        ,'if (sptuntdsc) sptuntdsc.up(2).hide();';
}
?>
            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarDadosParecer();
               
                carregarAditivos(cosano);
                
                if( ( '<?php echo $boEscolaAtiva ?>' || '<?php echo $boProLetramento ?>' ) && cosano < 2010 ){
                	if(<?=$adm;?> != 1){
                		bloquearDados( cosano );
                	}
                }
                else{
	                if( ( '<?php echo $boEscolaAtiva ?>' || '<?php echo $boProLetramento ?>' ) && cosano > 2009  ){
	                	var dadosForms 	= $('frmParSubacao').elements;
						var tamanho 	= $('frmParSubacao').elements.length;
	                	for( i=0; i<tamanho; i++ ){
							if(dadosForms[i].type == "button"){
								if( dadosForms[i].id != "btnAnterior" && dadosForms[i].id != "btnProximo" && dadosForms[i].id != "btnVoltar" ){
									dadosForms[i].disabled = "";
								}
							}
						}
	                }
                }                
                
            }

            $('spanAno').innerHTML = cosano;
        }

        return false;
    }
<?php

} // if ($sbaporescola) {

?>


    /**
     * 
     */
    function verificarModificaoItensComposicao(el)
    {
        if (!itemModificado)
            itemModificado = el.value != el.defaultValue;
    }


    /**
     * 
     */
    function regraFormaExec()
    {
        return true;
    }

    function exibirPlanoInterno(value)
    {
        new Ajax.Request(window.location.href,
                         {
                             parameters: '&reqt=<?php echo $reqt; ?>&req=prgplanointerno&prgplanointerno=' + value,
                             onComplete: function(e)
                             {
                                 $('prgplanointerno').value = e.responseText;
                             }
                         });
    }

    /**
     * 
     */
    function calcularValorTotal(cosid)
    {
        var vlrtotal = string2float($F('cosqtd'    + cosid)) *
                       string2float($F('cosvlruni' + cosid)) ;

        $('cosvlrtotal' + cosid).innerHTML = float2string(vlrtotal);
    }

    /**
     * 
     */
    function string2float(str)
    {
        if (str.indexOf(',') == -1)
            return parseFloat(str);
        else
            return parseFloat(str.replace(/\./g, '').replace(/,/g, '.'));
    }

    /**
     * 
     */
    function float2string(num)
    {
        return mascaraglobal('###.###.###.###.###,##', (parseFloat(num).toFixed(2)) + '');
    }


    /**
     * @private
     */
    //this._closeWindows = false;

<?php
if (!CTE_PRM_EDITAR_SUBACAO) {

?>
   

<?php
} else {

} // if (!CTE_PRM_EDITAR_SUBACAO) {

if (trim($_REQUEST['sbaid']) != '')
	echo '        selecionarAba(anoExercicio, true);';
else {
    echo '        $("alertaPermitirCadastroItensSubacao").show();
        $("tabAbasSelecaoCosano").hide();
        $("btnExcluir").setAttribute("disabled", "disabled");';
}



if (CTE_NOVA_SUBACAO && $capturaTipo == 'M' && !cte_verificaGrandeMunicipio($_SESSION['inuid'])) {

?>
    Form.disable('frmParSubacao');
    var btnCancelar = $('btnCancelar');

    if (btnCancelar)
        btnCancelar.removeAttribute('disabled');

<?php

} // if (CTE_NOVA_SUBACAO && $capturaTipo == 'M')

?>


<?php

if (CTE_NOVA_SUBACAO) {
    echo '$(\'loader-container\').hide();';

}
if($anoConvenio != 0000 or $_REQUEST['sbaidpai'] ){
?> 
	document.getElementById('BtnEditarSubacao').style.display="none";
	<?php if($_REQUEST['ad'] == 1){ 
			$escreve = " Aditivo ".$numAditivo." / ";
	?>
		document.getElementById('TdEditarSubacao').innerHTML="<b style='color:black;' >Criação & Edição do Ativio nº <?=$numAditivo;?> ano "+anoExercicio+"</b>";
	<?php }else{ 		?>
		document.getElementById('TdEditarSubacao').innerHTML="<b style='color:red;' >Está subação foi conveniada.</b>";
	<?php } ?>
	
	
<? } ?>
	
function toggleDiv( idDiv ){

	var obDiv = document.getElementById( idDiv );

	if( obDiv.style.display == 'block' )
		obDiv.style.display = 'none';
	else
		obDiv.style.display = 'block';
	
}


var arCPF = document.getElementsByName( 'cpf[]' );
for( o=0; o<arCPF.length; o++ ){
	var cpf = arCPF[o].innerHTML;
	arCPF[o].innerHTML = mascaraglobal( '###.###.###-##', cpf );
}

///////////////////////////////////////////////////////////////////////

function _totalizar(x){
	total = x;
}

function carregarBeneficiarios()
{
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&req=carregarBeneficiarios&sabano=' + anoExercicio,
                                    asynchronous: false,
                                    onComplete: function(res)
                                    {
                                        $('beneficiariosSubAcao' + anoExercicio).innerHTML = res.responseText;
                                    }
                                });
}

function carregarParecerPadrao(){
	return new Ajax.Request(window.location.href,
    						{
                            	method: 'post',
								parameters: '&parecerPadrao=true&cosano=' + anoExercicio,
								onComplete: function(res) {
									if( $( 'parecerPadrao_'+anoExercicio ).checked ){
										$( 'sptparecer_' + anoExercicio ).value = res.responseText;
										$( 'sptparecer_' + anoExercicio ).disabled = true;
									}
									else{
										$( 'sptparecer_' + anoExercicio ).disabled = false;
										$( 'sptparecer_' + anoExercicio ).value = "";
									}
								}
							});
}

function carregarItensComposicao()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&req=carregarItensComposicao&cosano=' + anoExercicio + '&sbaporescola='+porEscola,
                                asynchronous: false,
                                onComplete: function(res)
                                {	
                                    $('itensComposicaoSubAcao' + anoExercicio).innerHTML = res.responseText;
                                }
                            });
}


function carregarDadosParecer()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&req=carregarDadosParecer&sbtano=' + anoExercicio,
                                asynchronous: false,
                                onComplete: function(res)
                                {
                                    var sptparecer      		= $('sptparecer_' + anoExercicio);
                                    var sptunt          		= $('sptunt_'     + anoExercicio);
                                    var sptuntdsc       		= $('sptuntdsc_'  + anoExercicio);
                                    var sptinicio       		= $('sptinicio_'  + anoExercicio);
                                    var sptfim          		= $('sptfim_'     + anoExercicio);
                                    var ssuid           		= $('ssuid_'      + anoExercicio);
                                    var sptanoterminocurso      = $('sptanoterminocurso_' + anoExercicio);
                                    var ssudescricao			= getElementText('ssudescricao' , res);
                                    var ssuidValue      		= getElementText('ssuid'    , res);
                                    var sptinicioValue  		= getElementText('sptinicio', res);
                                    var sptfimValue     		= getElementText('sptfim'   , res);
                                    var prgidTesteValor =  $('prgidteste');
									
                                    if(prgidTesteValor.value && PIVisivel != 0){
                                     	 var plinumplanointerno 		= $('plinumplanointerno_'     + anoExercicio);
                                     	 var plinumplanointernoValue = getElementText('plinumplanointerno', res);
                                     	 if (plinumplanointerno) {
                                        	for (var i = 0; i < plinumplanointerno.options.length; i++) {
                                            	if (plinumplanointernoValue == plinumplanointerno.options[i].value)
                                               		plinumplanointerno.selectedIndex = plinumplanointerno.options[i].index;
                                        		}
                                    		}
                                    }
 
                                     var cvrnumprocesso 		= $('cvrnumprocesso_'     + anoExercicio);
                                     var cvrnumprocessoValue = getElementText('sptnumprocesso', res);
                                     
                                     if (cvrnumprocesso) {
                                       for (var i = 0; i < cvrnumprocesso.options.length; i++) {
                                           if (cvrnumprocessoValue == cvrnumprocesso.options[i].value){
                                               cvrnumprocesso.selectedIndex = cvrnumprocesso.options[i].index;
                                              }
                                       }
                                    }
								
                                    if (ssuid) { 
                                        if(ssudescricao){
                                        	ssuid.innerHTML = ssudescricao;
                                        }else{
                                        	ssuid.innerHTML = 'Não informado';
                                        }
                                    	
                                    }
                                    
                                    
                                        if(sptanoterminocurso){
                                        	var termino = (getElementText('sptanoterminocurso', res));
                                        }else{
                                        	var termino = ' -- ';
                                        }


										
                                        if(sptinicioValue){
                                        	$('cronograma_'+ anoExercicio).innerHTML =  mes(sptinicioValue) + ' a '+  mes(sptfimValue) + ' <br> Ano de término ' + termino;
                                        }else{
                                        	$('cronograma_'+ anoExercicio).innerHTML = 'Não informado.'
                                        }
                                    	
             						
                                    if (sptunt){
                                        sptunt.innerHTML = getElementText('sptunt', res);
                                      
                                    }
                                    if (sptparecer)
                                        sptparecer.value = getElementText('sptparecer', res);

                                    if (sptuntdsc)
                                        sptuntdsc.value  = getElementText('sptuntdsc', res);
                                }
                            });
}

function extratoEscolas(qfaid)
    {
        return windowOpen('/cte/cte.php?modulo=principal/extratoescolassubacao&acao=A&sbaid='+ subacao +'&qfaid=' + qfaid,
                          'extratoEscolas',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

 
/************** ADITIVOS *********************/

		function irAditivo(sbaidAditivo, ano){
			 window.location.href = '/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&aditivo=1&sbaid='+sbaidAditivo+'&sbaidpai='+subacao+'&anoconvenio='+ano+'&ad=1';
		}
		
		function voltarSubacaoPai(sbaidPai){
			 return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&req=voltarsubacaooriginal',
	                                    onComplete: function(res)
	                                    {
	                                    	subacaoOriginal = res.responseText;
	                                        window.location.href = '/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&sbaid='+subacaoOriginal;
	                                    }
	                                });
		}


		/*******************************
		* 
		*	FUNCTION: travaAnosAnteriores(cosano);
		* 	DATE: 03/12/2008
		* 	DESCRIÇÃO:
		* 	Se estiver em uma aba onde o ano e menor que o ano atual, e a forma de execução da subação
		* 	for assistencia financeira, transferencia voluntaria ou assistencia tecnica com complementação financeira,
		*   a aba dos anos anteriores são travadas.
		* 
		* 	@PARAM cosano - Ano referente a aba a que se encontra.
		* 
		*******************************/
		function travaAnosAnteriores(cosano){
			if(cosano < anoAtualTrava){
				bloquearDados(cosano);
			}else{
				bloquearDados(0);
			}
		
		}
		
		/*******************************
		* 
		*	FUNCTION: travaAbaAnoJaAnalisada(cosano);
		* 	DATE: 02/12/2008
		* 	DESCRIÇÃO:
		* 	Se estiver em Elaboração do PAR ou em  Validação do Município a aba (Ano)
		*   onde o parecer já foi dado trava.
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function travaAbaAnoJaAnalisada(cosano){
		
			if(	anoParecer2007 == cosano || // Elaboração
				anoParecer2008 == cosano || // Elaboração
				anoParecer2009 == cosano || // Elaboração
				anoParecer2010 == cosano || // Elaboração
				anoParecer2011 == cosano 
				)
			{ // Se o ano conveniado for igual ao ano da aba trava.
				bloquearDados(cosano);
			}else{
				bloquearDados(0);
			}
		}
		
		/*******************************
		* 	FUNÇÃO DE ADITIVO
		*	FUNCTION: travaAbaAnoSeAditivada(cosano);
		* 	DATE: 02/12/2008
		* 	DESCRIÇÃO:
		* 	Se a subação foi conveniada trava a aba que foi conveniada 
		*   onde o parecer já foi dado trava.
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function travaAbaAnoSeAditivada(cosano){
			if(	anoConvenio == cosano && !eAditivo ){
				if(anoAtualTrava == cosano){ // se for o ano corrente e foi convenida mostra botão Adicionar Aditivo.
					$('divAditivo' + cosano).style.display="table-row";// Mostra botão de Adicionar Aditivo.
				}
				bloquearDados(cosano);
			}else{
				if(eAditivo){
					bloquearDados(cosano);
				}else{
					bloquearDados(0);
				}
			}
		}
		
				/*******************************
		* 
		*	FUNCTION: bloquearDados(cosano);
		* 	12/11/2008
		* 	DESCRIÇÃO:
		* 	Trava a subação de acordo com o ano em que foi conveniada com o FNDE.
		*   Exemplo: Se a subação foi conveniada o ano de 2008 a aba de 2008 ficará desablilitada,
		*   será possivel apenas visualizar os dados. 	
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function bloquearDados(cosano)
	    { 	
			if(!novaSub){ // Se for nova subação não trava os dados
				var dadosForms 	= $('frmParSubacao').elements;
				var tamanho 	= $('frmParSubacao').elements.length;
				if( cosano != 0 )
				{ // Se o ano conveniado for igual ao ano da aba trava.
					if(cosano == "2007"){
						ind = 0;
					}else if(cosano == "2008"){
						ind = 1;
					}else if(cosano == "2009"){
						ind = 2;
					}
					else if(cosano == "2010"){
						ind = 3;
					}
					else if(cosano == "2011"){
						ind = 4;
					}
					
					if(document.getElementsByName("adItensComposicao")[ind]){
						document.getElementsByName("adItensComposicao")[ind].style.display="none";
					}else if(document.getElementsByName("adItensComposicao")[0]){
						document.getElementsByName("adItensComposicao")[0].style.display="none";
					}
					if(document.getElementsByName("adBeneficiarios")[ind]){
						document.getElementsByName("adBeneficiarios")[ind].style.display="none";
					}else if(document.getElementsByName("adBeneficiarios")[0]){
						document.getElementsByName("adBeneficiarios")[0].style.display="none";
					}
					if(document.getElementsByName("adEscolas")[ind]){
						document.getElementsByName("adEscolas")[ind].style.display="none";
						//alert(document.getElementsByName("adEscolas")[ind].style.display);
					}else if(document.getElementsByName("adEscolas")[0]){
						document.getElementsByName("adEscolas")[0].style.display="none";
					}
					if(document.getElementById('sptunt_'+cosano)){
						document.getElementById('sptunt_'+cosano).disabled="disabled";
					}
					document.getElementById('sptinicio_'+cosano).disabled="disabled";
					document.getElementById('sptfim_'+cosano).disabled="disabled";
					if(document.getElementById('sptanoterminocurso_'+cosano)){
						document.getElementById('sptanoterminocurso_'+cosano).disabled="disabled";
					}
					document.getElementById('sptuntdsc_'+cosano).disabled="disabled";
					
					if($( 'sptparecer_' + cosano )){
						$( 'sptparecer_' + cosano ).disabled = true;
					}
					if($( 'ssuid_' + cosano )){
						$( 'ssuid_' + cosano ).disabled = true;
					}
					// Desabilita btns.
					
					var btnExcluirItensComposicao = document.getElementsByName('removeItens');
					for( cont=0; cont<btnExcluirItensComposicao.length; cont++ ){
						btnExcluirItensComposicao[cont].style.display="none";
					}
					
					var btnExcluirBenficiario = document.getElementsByName('removeBeneficiario');
					for( cont=0; cont<btnExcluirBenficiario.length; cont++ ){
						btnExcluirBenficiario[cont].style.display="none";
					}
					
					var btnExcluirEscolas = document.getElementsByName('removeEscolas');
					for( cont=0; cont<btnExcluirEscolas.length; cont++ ){
						btnExcluirEscolas[cont].style.display="none";
					}
					var inputsItens   = $('frmParSubacao').getInputs('text');
					for (var a = 0; a < inputsItens.length; a++) {
	                    if (/cosvlruni/.test(inputsItens[a].getAttribute('name')) ||
	                        /cosqtd/   .test(inputsItens[a].getAttribute('name'))) {
	                        inputsItens[a].disabled="disabled";   
	                    }
	                }
					for( i=0; i<tamanho; i++ ){
						if(dadosForms[i].type == "button"){
							if( dadosForms[i].id != "btnAnterior" && dadosForms[i].id != "btnProximo" && dadosForms[i].id != "btnVoltar" ){
								dadosForms[i].disabled="disabled";
							}
						}
					}
					// Fim desabilita Btns
				}else{ // Se não foi conveniada não bloqueia (OBS: libera os btns para as outras abas.)
					for( i=0; i<tamanho; i++ ){
						if(dadosForms[i].type == "button"){
							dadosForms[i].disabled="";
						}
					}
				}  
    		}
    		
	    }
	    
		function mes(spt){
			var dataEscrita;
			if(spt == 1){
				dataEscrita = "Janeiro";
			}else if(spt == 2){
				dataEscrita = "Fevereiro";
			}else if(spt == 3){
				dataEscrita = "Março";
			}else if(spt == 4){
				dataEscrita = "Abril";
			}else if(spt == 5){
				dataEscrita = "Maio";
			}else if(spt == 6){
				dataEscrita = "Junho";
			}else if(spt == 7){
				dataEscrita = "Julho";
			}else if(spt == 8){
				dataEscrita = "Agosto";
			}else if(spt == 9){
				dataEscrita = "Setembro";
			}else if(spt == 10){
				dataEscrita = "Outubro";
			}else if(spt == 11){
				dataEscrita = "Novembro";
			}else if(spt == 12){
				dataEscrita = "Dezembro";
			}
			return dataEscrita;
		}
		
	</script>
</html>

<?php 

function sql2html($coluna) {
    if (strtolower($coluna) == 't')
        return true;

    if (strtolower($coluna) == 'f')
        return false;

    if (strtolower($coluna) == 'null' || $coluna == null)
        return '';

    return $coluna;
}
/*
 * Função que monta o SQL para carregar os Beneficiários
 * @return string - SQL dos Beneficiários
 */
function montarSQLBeneficiarios(){
	
	$sql = '
	        SELECT 
	            be.bendsc,
	            sb.vlrurbano as vlrurbano,
	            sb.vlrrural  as vlrrural,
	            sb.vlrurbano + sb.vlrrural as sabvlrtotal
	        FROM cte.subacaobeneficiario sb
	        	INNER JOIN cte.beneficiario be ON sb.benid = be.benid
	        WHERE sb.sbaid  = ' . $_REQUEST['sbaid']   . '
			AND sb.sabano   = ' . $_REQUEST['sabano'];
	
	return $sql;
}

/*
 * Função que monta o SQL para carregar Itens de Composição
 * @return string - SQL dos itens de composição
 */
function montarSQLItensComposicao(){
	global $db;
	$sql = '
	        SELECT count(sba.undid)
	        FROM cte.subacaoindicador sba
	        	INNER JOIN cte.unidademedida und ON sba.undid  = und.undid
	        	INNER JOIN cte.laboratorio lab ON und.undid = lab.undid
	        WHERE sbaid = ' . $_REQUEST['sbaid'];

	if( !CTE_PRM_EDITAR_SUBACAO || $db->pegaUm($sql) == 1 ){
		$readonly = ' readonly="readonly"';
		$remover  = "alert(\\'Não é possível excluir itens de composição de laboratórios.\\');return false;";
	} 
	else{
		$readonly = '';
		$remover  = 'removerItemComposicao';
	}

	if( $_REQUEST['sbaporescola'] == 't' ){
		if( $readonly == '' ){
			$sql = '
                	SELECT 
                		cos.cosdsc
                    	,und.undddsc
	                    ,
						 CASE 
							WHEN substring( trim(to_char(coalesce(ecs.total, 0), \'9999999990D99\')), position(\',\' in trim(to_char(coalesce(ecs.total, 0), \'9999999990D99\'))) +1, 2 ) = \'00\'
								THEN trim(to_char(coalesce(ecs.total, 0), \'999999999\'))
								ELSE trim(to_char(coalesce(ecs.total, 0), \'9999999990D99\'))
						 END AS cosqtd
                    	,trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) AS cosvlruni
                    	,trim(to_char(coalesce(ecs.total, 0) * coalesce(cos.cosvlruni, 0), \'0D99\'))  AS vlrtotal
					FROM cte.composicaosubacao cos
						LEFT JOIN (SELECT 
										cosid, 
										SUM(ecsqtd) AS total 
									FROM cte.escolacomposicaosubacao 
									GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                      AND cos.cosano = ' . $_REQUEST['cosano'] . '
                	ORDER BY cos.cosdsc';

			$sql = $db->carregar($sql);

			$dat = $db->carregar('
									SELECT
										\'\' AS cosdsc,
										\'\' AS undddsc,
										\'\' AS cosqtd,
										\'\' AS cosvlruni,
										trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'99999999.99\')) AS vlrtotal
			                		FROM cte.composicaosubacao cos
			                			LEFT JOIN (SELECT 
			                							cosid, 
			                							SUM(ecsqtd) AS total 
			                						FROM cte.escolacomposicaosubacao 
			                						GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
			                			INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
			                		WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
			                    	  AND cos.cosano = ' . $_REQUEST['cosano']);

			$sql[] = $dat[0];
			
		}
		else {
			$sql = '
                	SELECT
                    	 cos.cosdsc
                    	,und.undddsc
	                    ,
						 CASE 
							WHEN substring( trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D99\')), position(\',\' in trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D99\'))) +1, 2 ) = \'00\'
								THEN trim(to_char(coalesce(cos.cosqtd, 0), \'999999999\'))
							    ELSE trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D99\'))
						 END AS cosqtd
                    	, trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) AS cosvlruni
                    	, \'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' AS vlrtotal
                	FROM cte.composicaosubacao cos
                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                    AND cos.cosano = ' . $_REQUEST['cosano'] . '
                	ORDER BY cos.cosdsc';

			$sql   = $db->carregar($sql);
			$dat   = $db->carregar('
									SELECT
										\'\' AS cosdsc,
										\'\' AS undddsc,
										\'\' AS cosqtd,
										\'\' AS cosvlruni,
										trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'999999990D99\')) as vlrtotal
				                	FROM cte.composicaosubacao cos
				                		LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
				                		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
				                	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
				                    AND cos.cosano = ' . $_REQUEST['cosano']);

			$sql[] = $dat[0];
		}
	} 
	else{
		$sql = '
            	SELECT cos.cosdsc
					,und.undddsc
					,
						case 
							    		when substring( trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D99\')), position(\',\' in trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D99\'))) +1, 2 ) = \'00\'
							    			then trim(to_char(coalesce(cos.cosqtd, 0), \'999999999\'))
							    		else trim(to_char(coalesce(cos.cosqtd, 0), \'9999999990D99\'))
							    	END as cosqtd
					,trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) as cosvlruni
                	,  trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) as vlrtotal
				FROM cte.composicaosubacao cos
            		INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
            	WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                AND cos.cosano = ' . $_REQUEST['cosano'] . '
            	GROUP BY cos.cosdsc ,und.undddsc , cosqtd ,cosvlruni ,cos.cosid
            	ORDER BY cos.cosdsc';

		$sql = $db->carregar($sql);

		$dat = $db->carregar('
								SELECT
									\'\' AS cosdsc,
									\'\' AS undddsc,
									\'\' AS cosqtd,
									\'\' AS cosvlruni,
									trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'99999999.99\')) as vlrtotal
								FROM cte.composicaosubacao cos
									INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
								WHERE cos.sbaid  = ' . $_REQUEST['sbaid']  . '
								AND cos.cosano = ' . $_REQUEST['cosano']);

		$sql[] = $dat[0];
		
	}
	return $sql;
}

	function montarSQLEscolas(){
	
	global $db;
	
	$sql = ' 
			SELECT
	            \'<center><img id="removeEscolas" name="removeEscolas" onclick="return removerEscola(\' || q.qfaid || \')" src="/imagens/excluir.gif" style="border:none" title="Excluir" alt="Excluir item" /> <img onclick="return extratoEscolas(\' || q.qfaid || \')" src="/imagens/consultar.gif" style="border:none" title="Extrato" alt="Extrato item" /></center>\' as qfaid,
	            e.entcodent,
	            e.entnome,
	            sum(coalesce(qfaqtd,0))
        	FROM cte.qtdfisicoano q
        		INNER JOIN entidade.entidade e on q.entid = e.entid
        	WHERE q.qfaano = ' . $_REQUEST['qfaano'] . '
            AND q.sbaid  = ' . $_REQUEST['sbaid']  . '
        	GROUP BY q.qfaid, e.entcodent, e.entnome
        	ORDER BY entnome';
		
		$arEscolas = $db->carregar($sql);	
	
		if( $arEscolas ){
			$soma = 0;
			foreach( $arEscolas as $escola ){
				$soma += isset( $escola["sum"] ) ? $escola["sum"] : 0;
			}
		
	        $arTotais["qfaid"] = "";
	        $arTotais["entcodent"] = ""; 
	        $arTotais["entnome"] = "<b>Total de Escolas: &nbsp;&nbsp;<span id='somaTotalEscolas".$_REQUEST['qfaano']."'>". count($arEscolas)."</span></b>";
	        $arTotais["sum"] = "<div style='text-align: right;' ><b>Total: &nbsp;&nbsp;$soma</b></div>";
	        
	        $arEscolas[] = $arTotais;
		}
		else{
			$arEscolas = $sql;
		}
	
	return $arEscolas; 	
}

	/*******************************
	* 
	*	FUNCTION: subacaoConveniada();
	* 	17/06/2008
	* 	DESCRIÇÃO:
	* 	Verifica se a subação foi gerada convênio com o FNDE
	* 
	* 	@PARAM 	RETORNO : $conveniada - Ano do convênio 
	* 
	*******************************/
	function subacaoConveniada(){
		global $db;
		/*
		$sql = 'select sc.sbcano
			from cte.subacaoconvenio sc
			inner join cte.convenio c on c.cnvid = sc.cnvid
			inner join cte.convenioretorno cr on cr.cnvid = c.cnvid
			where sbaid = '.trim($_REQUEST['sbaid']);
		*/
		$sql="select distinct pssano as sbcano from cte.projetosapesubacao where sbaid = ".trim($_REQUEST['sbaid'])." -- and sbcano = date_part('year', current_date)";
		//$conveniada = $db->pegaUm($sql);	
		$conveniada = $db->carregar($sql);
		return $conveniada;
	}
	
	
	/*
 * Função que monta o SQL para carregar Itens de Composição de professores
 * @return string - SQL dos itens de composição
 */
function montarSQLItensComposicaoCPF(){
	
	global $db;
	
	$sql = '
	        SELECT count(sba.undid)
	        FROM cte.subacaoindicador sba
	        	INNER JOIN cte.unidademedida und ON sba.undid  = und.undid
	        	INNER JOIN cte.laboratorio lab ON und.undid = lab.undid
	        WHERE sbaid = ' . $_REQUEST['sbaid'];

	if( !CTE_PRM_EDITAR_SUBACAO || $db->pegaUm($sql) == 1 ){
		$readonly = ' readonly="readonly"';
		$remover  = "alert(\\'Não é possível excluir itens de composição de laboratórios.\\');return false;";
	} 
	else{
		$readonly = '';
		$remover  = 'removerItemComposicao';
	}
	
	$sql = '
	        SELECT 
	        	\'<label name="cpf[]">\'|| e.entnumcpfcnpj ||\'</label>\' as coscpf,
		        e.entnome,
		        CASE 
		            WHEN tvp.tvpid = 4
		        THEN vpe.tvpdscoutros
		            ELSE tvp.tvpdsc
		        END AS vinculo,
		        CASE 
		            WHEN tf.tfoid = 7 
		        THEN fe.tfodscoutros
		            ELSE tf.tfodsc
		        END AS formacao
		    FROM cte.composicaopessoa cp
		        inner join entidade.entidade e on cp.entid = e.entid
		        left join entidade.formacaoentidade fe on e.entid = fe.entid
		        inner join public.tipoformacao tf on fe.tfoid =  tf.tfoid
		        left join entidade.vinculoprofissionalentidade vpe on e.entid = vpe.entid
		        inner join public.tipovinculoprofissional tvp on vpe.tvpid =  tvp.tvpid
	        
	        WHERE cp.sbaid = ' . $_REQUEST['sbaid'] . '
	        and cp.cspano  = ' . $_REQUEST['cosano'];
	
	//xd($sql);
	$sql = $db->carregar($sql);
	return $sql ? $sql : array(); 
}

/*
 * Função que verifica se a subação tem parecer 
 * @param int $sbaid - Identificação da subação
 * @return int - Retorna 1 ou 0 (se tem ou não tem parecer)   
 */
function existeParecerPorSubacao( $sbaid ){
	
	global $db;
	
	 $sql = '
	    SELECT count(spt.ssuid)
	    FROM cte.subacaoparecertecnico spt
	    	INNER JOIN cte.statussubacao ssu ON ssu.ssuid = spt.ssuid
	    WHERE spt.sbaid = ' . $sbaid . '
 		AND trim(spt.sptparecer) <> \'\'
		AND spt.ssuid IS NOT NULL';

	    return (integer) $db->pegaUm($sql) > 0;
}

die(); ?> 

