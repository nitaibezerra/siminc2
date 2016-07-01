<?php
/********************* INICIANDO TELA *********************/
cte_verificaSessao(); 	// Verifica se usuário logado.
ob_clean(); 			// Limpa o cache.
header('content-type: text/html; charset=iso-8859-1;');

/********************* INCLUDES *********************/
include_once( "par_subacao_funcoes.inc" );
include_once( APPRAIZ."cte/classes/SubacaoIndicador.class.inc" );
include_once( APPRAIZ."cte/classes/ComposicaoPessoa.class.inc" );

/********************* DECLARAÇÃO DE VARIAVEIS E CONSTANTES *********************/

$obSubacao = new SubacaoIndicador( $_REQUEST['sbaid'] );
$itrid = cte_pegarItrid( $_SESSION['inuid'] );                                                                                      

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


$boExibeBotaoIncluir = false;
$perfis = array( CTE_PERFIL_EQUIPE_MUNICIPAL, 
				 CTE_PERFIL_EQUIPE_ESTADUAL_CADASTRO 
				);

if( cte_possuiPerfil( array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_PARECERISTA_FISICO_FINANCEIRO) ) ){
	$boExibeBotaoIncluir = true;
} elseif(cte_possuiPerfil( $perfis ) && $estadoDocumento['esdid'] == CTE_ESTADO_PAR) {
	$boExibeBotaoIncluir = true;
}

if (!$_REQUEST['anoExercicio']){
    $_REQUEST['anoExercicio'] 	= $_REQUEST['parsubacaoanocrt'] ? $_REQUEST['parsubacaoanocrt'] : date('Y');
    $_REQUEST['anoExercicio']   = $_REQUEST['anoExercicio'] > 2011 ? 2011 : $_REQUEST['anoExercicio'];
   	$cosano 					= $_REQUEST['cosano'] ? $_REQUEST['cosano'] : $_REQUEST['anoExercicio'];
}

if($_REQUEST['sbaidpai'] != 0){
	$_REQUEST['parsubacaoanocrt']= $_REQUEST['anoconvenio'];
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
		$anoAtualTrava 	= date('Y') > 2011 ? 2011 : date('Y');
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
    if($estadoDocumento['esdid'] == CTE_ESTADO_PAR){
    	$editavel = !$parecer;
    }
    
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
								in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) || 
								in_array(CTE_PERFIL_EQUIPE_TECNICA_FORMACAO_ESCOLA, $perfis ))){
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

function pegaDadosProLetramento2010(){
	
	global $db;
	
	$sql = "SELECT DISTINCT
					si.sbaid,
					m.muncod, 
					mundescricao,
					si.qtditensoriginal 
				FROM cte.instrumentounidade i
				INNER JOIN territorios.municipio 	m  ON  i.muncod = m.muncod
				INNER JOIN cte.pontuacao 			p  ON  i.inuid = p.inuid
				INNER JOIN cte.acaoindicador		ai ON  p.ptoid = ai.ptoid
				INNER JOIN cte.subacaoindicador		si ON si.aciid = ai.aciid
				INNER JOIN cte.proletramento2010	pl ON pl.muncod = m.muncod
				WHERE i.inuid 	= {$_SESSION['inuid']}
				AND si.prgid 	= 656
				AND si.sbaid 	= {$_REQUEST['sbaid']}
				AND si.sbadsc NOT ILIKE '%Solicitar um kit contendo um livro de Matemática%'";				
				
	return $db->pegaLinha($sql);	
}

/*
 * LIBERA EDIÇÃO PARA EQUIPE MUNICIPAL CONFORME FORMAÇÃO PELA ESCOLA
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
				left join cte.criterio ct 			on ct.crtid = pt.crtid
				--inner join cte.subacaoparecertecnico spt	on spt.sbaid = s.sbaid
			where s.sbaid in ('".$_REQUEST['sbaid']."')
			and ppsid in ( 1232, 1237, 1242, 1247, 1252, 1257, 1262, 1267, 1272, 1277, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291 )		
			order by s.sbaid desc";
	
	$rsFormacaoEscola = $db->carregar($sql);
	$perfisFormacaoEscola = array(CTE_PERFIL_EQUIPE_TECNICA_FORMACAO_ESCOLA);
	
	if($rsFormacaoEscola[0]['ppsid'] > 1 &&	cte_possuiPerfil($perfisFormacaoEscola) ) {	
		
		define( 'CTE_PRM_EDITAR_SUBACAO', 1);
			
	} else {		
				
		define( 'CTE_PRM_EDITAR_SUBACAO' , cte_podeEditarParecer( $_SESSION['inuid'] ) || $boDa );
		
	}
	
} else {
	
	define( 'CTE_PRM_EDITAR_SUBACAO' , cte_podeEditarParecer( $_SESSION['inuid'] ) || $boDa );
}

//define( 'CTE_PRM_EDITAR_SUBACAO' , cte_podeEditarParecer( $_SESSION['inuid'] ) || $boDa );

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
																					  CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
																					  CTE_PERFIL_EQUIPE_TECNICA_FORMACAO_ESCOLA)));
																					  
define('CTE_PODE_EDITAR_PARECER', CTE_PRM_EMITIR_PARECER || cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                      CTE_PERFIL_EQUIPE_TECNICA,
                                                                                      CTE_PERFIL_EQUIPE_ESCOLA_ATIVA,
                                                                                      CTE_PERFIL_ALTA_GESTAO,
                                                                                      CTE_PERFIL_EQUIPE_TECNICA_FORMACAO_ESCOLA
                                                                                    )));		

if(CTE_PODE_EDITAR_PARECER){
	$disableParecer = 'S';
}else{
	$disableParecer = 'N';
}
							  
define('CTE_PRM_VISUALIZAR_PI', CTE_PRM_EDITAR_SUBACAO && cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                      CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_PARECERISTA_FISICO_FINANCEIRO  )));

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
	
	/************************************************************	
	* 	CRIA O NOVO ADITIVO
	* 	(FUNÇÃO AJAX)
	* 	REQUEST				: novoAditivo
	*   CHAMADA JAVASCRIPT	: function adicionarAditivo(ano)
	* 	DESCRIÇÃO: 
	*   Insere na tabela de subaçãoIndicador a subação e 
	*   da update na mesma colocando o sbaidpai
	*
	*************************************************************/
	if($_REQUEST['req'] == 'novoAditivo'){ // Se for aditivo insere na tabela subacaoindicador a subação que e aditivo.
		$sbaidAdtivo 	= $obSubacao->insereAditivoSubacao($_REQUEST['sbaid'], $_REQUEST['ano']);
		$numAditivo 	= $obSubacao->recuperaUltimoNumAditivo ($_REQUEST['sbaid'], $_REQUEST['ano']);
		$obSubacao->atualizaUltimoNumAditivo($sbaidAdtivo, $numAditivo, $_REQUEST['ano']);
		importarDadosSubAcao( $_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['ano'] , $sbaidAdtivo ); //**@@&&
		
		$_REQUEST['sbaidpai'] 		= $sbaidAdtivo;
		$_REQUEST['anoconvenio'] 	= $anoConvenio;
	   	echo $sbaidAdtivo;
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
	
	if($admTemp == 1){// **@@&& Se for aditivo - reformulação libera para edição
		$boInsereItensEBeneficiarios = true;
	}
	
/************************************************************************
*								SALVAR DADOS							*
************************************************************************/

if ($_REQUEST['formularioSumit'] == '1' && CTE_PRM_EDITAR_SUBACAO) {
	// Exclusão
    if( $_REQUEST['vlrProximaSubacao'] == '-1' && !CTE_NOVA_SUBACAO &&  CTE_PRM_EXCLUIR_SUBACAO ){
    	
		if( cte_podeEditarParecer( $_SESSION['inuid'] ) ){
	    	// Exclusão efetuada com sucesso
	    	if( excluirSubacao( $_REQUEST['sbaid'] ) ){
		        echo '<script type="text/javascript">'
		            .'window.opener.location.reload();'
		            .'alert("Sub-ação excluída com sucesso.");'
		            .'window.opener.focus();'
		            .'window.close();'
		            .'</script>';
	    	}
	    	// Erro na exclusão
	    	else{
				echo '<script type="text/javascript">'
		        	.'alert("Operação não realizada!\nFoi dado o parecer técnico, gerado convênio ou a subação está sendo monitorada.");'
		            .'window.location.href = "?modulo=principal/par_subacao&acao=A&sbaid=' . $_REQUEST['sbaid'] . '";'
		            .'</script>';
	    	}
		}
		else{
			echo '<script type="text/javascript">'
	        	.'alert("Operação não realizada!\nNão foi possível excluir a sub-ação");'
	            .'window.location.href = "?modulo=principal/par_subacao&acao=A&sbaid=' . $_REQUEST['sbaid'] . '";'
	            .'</script>';
		}
        die();
    }
    // Inserção
    $anoExercicio = $_REQUEST['anoExercicio'];

    if (trim($_REQUEST['aciid']) == '' || $_REQUEST['aciid'] == '0') {
        echo 'alert("Não foi possível capturar o indicador da subação.");';
        die();
    }
    
    // Inserindo nova subação
    if( CTE_NOVA_SUBACAO ){

    	$arDados = $_REQUEST;
    	$arDados['usucpf'] = $_SESSION['usucpf'];

        $_REQUEST['sbaid'] = inserirNovaSubacao( $arDados );
        echo 'window.location.href="' , $_SERVER['REQUEST_URI'] , '&sbaid=' , $_REQUEST['sbaid'] , '";';
    }
    else {
        /*!@
         * Validacao de dados
         */
        $ppsindcobuni = $db->pegaUm('SELECT ppsindcobuni
                                     FROM cte.subacaoindicador sba
                                     	INNER JOIN cte.proposicaosubacao pps ON sba.ppsid = pps.ppsid
                                     WHERE sba.sbaid = ' . $_REQUEST['sbaid']);        
        
        if( $ppsindcobuni == 'f' ){

        	$err = verificarErros(); 
        	
        	if( sizeof($err) > 0 && $_REQUEST['validacao'] != 'false' ){
				echo 'var err=[];'
					,'err.push(\'' , implode('\');err.push(\'', $err) , '\');'
					,'alert("Atenção!\nForam encontrados erros ao salvar os dados da subação:\n\n" + err.join("\n") + "\n\nOs dados NÃO foram salvos.\nPor favor corrija os erros e clique em GRAVAR para continuar.");';
		
				die();
			}
        	
        } // fim de ---> if ($ppsindcobuni == 'f')

        gravarSubAcao( $anoExercicio );

        if( !$_REQUEST['vlrProximaSubacao'] )
            $_REQUEST['vlrProximaSubacao'] = $_REQUEST['sbaid'];
            // Verifica se e Aditivo ou subação normal //
            if($_REQUEST['sbaidpai'] && $_REQUEST['ad'] && $_REQUEST['anoconvenio']){
            	echo 'alert("Dados salvos com sucesso.");'
	            ,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['vlrProximaSubacao']
	            ,'&sbaidpai=' , $_REQUEST['sbaidpai']
	            ,'&ad=' , $_REQUEST['ad']
	            ,'&anoconvenio=' , $_REQUEST['anoconvenio'] , '";';
            }else{
            	echo 'alert("Dados salvos com sucesso.");'
	            ,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['vlrProximaSubacao']
	            ,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
            }
            
			
    }

    die();
} // fim de if( $_REQUEST['formularioSumit'] == '1' && CTE_PRM_EDITAR_SUBACAO )

//!@------------------------------------------------------------- FUNCOES AJAX

/************************************************************************
*		   LISTAGEM, EXCULSÃO DE ITENS-INDICADOR E TOTALIZADORES		*
************************************************************************/
$sql="select pssano as sbcano from cte.projetosapesubacao where sbaid = ".trim( $_REQUEST['sbaid'] );

if( array_key_exists('req', $_REQUEST ) ) {

    if (trim($_REQUEST['sbaid']) == '') {
        exit;
    }
	
    // Limpar SubAção
    if( $_REQUEST['req'] == 'limparSubacao' ){
        
    	if( limparSubAcao() ){
			echo 'alert("Dados excluídos com sucesso.");'
                ,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
                ,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
        } 
		else{
			echo 'alert("Já foi dados parecer para esta subação. Os dados NÃO foram apagados.");'
				,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
				,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
		}	
    	
    }
    // Plano Interno
    elseif( $_REQUEST['req'] == 'prgplanointerno' ){
        /*
    	echo $db->pegaUm('SELECT prgplanointerno
                          FROM cte.programa
                          WHERE prgid = ' . (integer) $_REQUEST['prgplanointerno']);
		*/
        echo $db->pegaUm('SELECT sptplanointerno FROM cte.subacaoparecertecnico WHERE sbaid = '.$_REQUEST['sbaid']);
    }
	
    // Verificar Inserção Dados SubAção
    elseif( $_REQUEST['req'] == 'verificarInsercaoDadosSubacao' ){
        
            $sql = ' 
            		SELECT count(sba.sbaid)
            		FROM cte.subacaoindicador sba
	            		INNER JOIN cte.subacaobeneficiario ben ON sba.sbaid = ben.sbaid AND (vlrurbano > 0 OR vlrrural > 0)
	            		INNER JOIN cte.composicaosubacao cos ON sba.sbaid = cos.sbaid   AND (cosqtd    > 0) ';
            $sql .= $_REQUEST['sbaporescola'] == 't' ? '   INNER JOIN cte.qtdfisicoano qfa ON sba.sbaid = qfa.sbaid AND (qfaqtd    > 0) ' : " ";
			$sql .= ' WHERE sba.sbaid = ' . $_REQUEST['sbaid'];

        echo '<script>_totalizar(' , $db->pegaUm($sql) , ');</script>';

    }
    // Carregar Itens de Composição
    elseif( $_REQUEST['req'] == 'carregarItensComposicao' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

        if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica  ){
        	$sql = array();
	        $sql = montarSQLItensComposicaoCPF();
	        $db->monta_lista_simples( $sql, array('Ações', 'CPF', 'Nome', 'Vínculo', 'Formação'), 1000, 1000, 'N', '100%','N' );
        }
        else{
	        $sql = montarSQLItensComposicao();
	        $db->monta_lista_simples( $sql, array('Ações', 'Identificação do Item', 'Un. de Medida', 'Qtd', 'Valor Unitário',
	                                              'Total' ), 1000, 1000, 'N', '100%','N' );
        }
		
        $totalItensC = count($sql)-1;              
        echo '<input type=hidden name="totalitensc" id="totalitensc" value="'.$totalItensC.'" />';        
        echo '<input type=hidden name="anoRef" id="anoRef" value="'.$_REQUEST['cosano'].'" />';
        
    }
    // Remover Itens de Composição
    elseif( $_POST['req'] == 'removerItemComposicao' && (isset($_POST['cosid']) || isset($_POST['cspid'])) ){
    	if( in_array( $undid, $arUnidades ) && $boExecucaoTecnica ){
	        $obComposicaoPessoa = new ComposicaoPessoa();
			$obComposicaoPessoa->excluir($_POST['cspid']);
			$obComposicaoPessoa->commit();
    	} else {
	    	$sql = ' 
	        		DELETE FROM cte.escolacomposicaosubacao
	        		WHERE cosid = ' . (integer) $_POST['cosid'] . ';
	
	        		DELETE FROM cte.composicaosubacao
	        		WHERE cosid = ' . (integer) $_POST['cosid'];
	        $db->executar($sql);
        	$db->commit();
    	}

    }
    // Carregar Beneficiários
    elseif ($_REQUEST['req'] == 'carregarBeneficiarios' && $_REQUEST['sbaid'] != '' && $_REQUEST['sabano'] != '' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

        $sql = montarSQLBeneficiarios();
        $db->monta_lista_simples($sql, array('Ações', 'Beneficiário', 'Zona Urbana', 'Zona Rural',
                                             'Total'), 1000, 1000, 'S', '100%','N');

    } 
    // Remove Beneficiários
    elseif( $_REQUEST['req'] == 'removerBeneficiario' && $_REQUEST['benid']  != '' && $_REQUEST['sabano'] != '' ){
        $sql = '
		        DELETE FROM cte.subacaobeneficiario
		        WHERE benid  	   = ' . (integer) $_REQUEST['benid' ] .'
				AND sabano = ' . (integer) $_REQUEST['sabano'] .'
				AND sbaid  = ' . (integer) $_REQUEST['sbaid'];

        $db->executar($sql);
        $db->commit();

    }
    // Carregar Escolas
    elseif( $_REQUEST['req'] == 'carregarEscolas' && $_REQUEST['sbaid']  != '' && $_REQUEST['qfaano'] != '' ){
    	
        header('Content-type: text/html;charset=ISO-8859-1');

		$arEscolas = montarSQLEscolas();		
        $resp = $db->monta_lista_simples($arEscolas, array('Ações', 'Código INEP', 'Escola', 'Quantidade'), 
        									 	   5000, 5000, 'N', '100%','N');
        									 	   
    }
    // Remover Escolas
    elseif( $_REQUEST['req']   == 'removerEscola' && $_REQUEST['qfaid'] != '' ){
    	
        $sql = '
        		DELETE FROM cte.escolacomposicaosubacao
        		WHERE qfaid = ' . (integer) $_REQUEST['qfaid'] . ';

        		DELETE FROM cte.qtdfisicoano
        		WHERE qfaid = ' . (integer) $_REQUEST['qfaid'];

        $db->executar($sql);
        $db->commit();

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
		            coalesce(spt.ssuid, 0)              as ssuid
		        FROM cte.subacaoparecertecnico spt
		        	INNER JOIN cte.subacaoindicador sba on spt.sbaid = sba.sbaid
		        WHERE spt.sbaid  = ' . $_REQUEST['sbaid'] . '
				AND spt.sptano = ' . $_REQUEST['sbtano'];

        $res = $db->pegaLinha($sql);

        header('content-type: text/xml; charset=ISO-8859-1');
        echo '<?xml version="1.0" encoding="ISO-8859-1"?>' , "\n"
            ,'<response>'
            ,'<sptparecer><![CDATA[', $res['sptparecer']         , ']]></sptparecer>'
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
    // Importar Dados de um ano para outro     
    elseif ($_REQUEST['req'] == 'importarDados') {
		
		$sql="select pssano as sbcano from cte.projetosapesubacao where sbaid = ".trim( $_REQUEST['sbaid'] );
		$res = $db->carregar( $sql );
		$podeImportarDados = true;
		if(!$admTemp || !$adm ){
			if(is_array($res)){
				foreach($res as $dadosConveniada){
					if($dadosConveniada["sbcano"] == $_REQUEST['anoExercicio']){
						$podeImportarDados = false;
					}
				}
			}
		}

		
		if( $podeImportarDados ){
	    	if( importarDadosSubAcao( $_REQUEST['sbaid'], $_REQUEST['anoExercicio'], $_REQUEST['csAno'] ) ){
				echo 'alert("Dados importados com sucesso.");'
	                ,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
	                ,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';    			
	    	}
			else{
				echo 'alert("Não foi possível importar os dados.");'
					,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
					,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
			}
		
		}
    	else{
			echo 'alert("Não foi possível importar os dados, pois a subação neste ano já foi conveniada!!!");'
				,'window.location.href="/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
				,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
		}
	}    
    // Carregar Totalizadores
    elseif( $_REQUEST['req'] == 'carregarTotalizadores' ){

        echo '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">'
            ,'<tr>'
            ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td>';

        if ($_REQUEST['sbaporescola'] != 't') {
            $sql = '
            SELECT
                coalesce(sum(ano2007),0) AS a2007,
                coalesce(sum(ano2008),0) AS a2008,
                coalesce(sum(ano2009),0) AS a2009,
                coalesce(sum(ano2010),0) AS a2010,
                coalesce(sum(ano2011),0) AS a2011,
                coalesce(sum(sptunt) ,0) AS total
            FROM (
                SELECT
                CASE WHEN sptano = 2007 THEN coalesce(sptunt,0) END AS ano2007,
                CASE WHEN sptano = 2008 THEN coalesce(sptunt,0) END AS ano2008,
                CASE WHEN sptano = 2009 THEN coalesce(sptunt,0) END AS ano2009,
                CASE WHEN sptano = 2010 THEN coalesce(sptunt,0) END AS ano2010,
                CASE WHEN sptano = 2011 THEN coalesce(sptunt,0) END AS ano2011,
            sptunt
            FROM cte.subacaoparecertecnico where sbaid = ' . $_REQUEST['sbaid'] . ') AS valores';

            $res = (array) $db->carregar($sql);

            $db->monta_lista_simples($sql, array('2007', '2008', '2009', '2010', '2011',
                                                 'Total'), 1000, 1000, 'N', '100%','N');

            echo '</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';
        }

        if ($_REQUEST['sbaporescola'] == 't') {
            // TOTALIZACAO POR ESCOLA
            $sql = '
		            SELECT DISTINCT ent.entcodent, ent.entnome, sum(qfa.qfaqtd) as qtdtotal, sum(tot.cosvlruni * ecs.ecsqtd) as vlrtotal
		            FROM cte.qtdfisicoano qfa
		            	LEFT JOIN cte.escolacomposicaosubacao ecs on qfa.qfaid = ecs.qfaid
			            LEFT JOIN cte.composicaosubacao cos on cos.cosid = ecs.cosid
			            INNER JOIN entidade.entidade ent on qfa.entid = ent.entid
			            LEFT JOIN (
			            			SELECT DISTINCT cos.cosid, cos.cosvlruni
		                 			FROM cte.escolacomposicaosubacao ecs
		                 				INNER JOIN cte.composicaosubacao cos on cos.cosid = ecs.cosid
		                 			GROUP BY cos.cosid, cos.cosvlruni
		                 		   ) as tot on tot.cosid = ecs.cosid
		            WHERE qfa.sbaid = ' . $_REQUEST['sbaid'] . ' and  qfa.qfaano not in (0) 
		            GROUP BY ent.entnome, ent.entcodent
		            ORDER BY ent.entnome, ent.entcodent';

            $db->monta_lista_simples($sql, array( 'Código INEP', 'Escola', 'Quantidade', 'Total (R$)'), 1000, 1000, 'S', '100%','N');

            echo '</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';

        }

        $sql = '
        SELECT
            cos.cosdsc
            ,coalesce(cos.cosqtd, 0) as cosqtd
            ,coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0) as cosvlrtotal
        FROM
            cte.composicaosubacao cos
        WHERE
            cos.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Identificação do Item',
                                             'Qtd',
                                             //'Valor Unitário',
                                             'Total'), 1000, 1000, 'S', '100%');

        echo '</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td style="font-weight:bold;text-align:center">Totalizador Por Beneficiários</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td>';

        $sql = '
        SELECT
            be.bendsc,
            sb.vlrurbano as vlrurbano,
            sb.vlrrural  as vlrrural,
            sb.vlrurbano + sb.vlrrural as sabvlrtotal
        FROM
            cte.subacaobeneficiario sb
        INNER JOIN
            cte.beneficiario be ON sb.benid = be.benid
        WHERE
            sb.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Beneficiário', 'Zona Urbana', 'Zona Rural', 'Total'), 1000, 1000, 'S', '100%');
        echo '</td>'
            ,'</tr>'
            ,'</table>';
    } // Fim de elseif( $_REQUEST['req'] == 'carregarTotalizadores' )

    die();
} // Fim de if( array_key_exists('req', $_REQUEST ) )

if( $_REQUEST["parecerPadrao"] && $_REQUEST["sbaid"] != ''){ // Carrega o Parecer Padrão
	
	$sql = "SELECT ppsparecerpadrao FROM cte.proposicaosubacao ps
				INNER JOIN cte.subacaoindicador sba on sba.ppsid = ps.ppsid 
			WHERE sbaid = ". $_REQUEST["sbaid"];
	
  	echo $db->pegaUm( $sql );
	
	die();
}

function sql2html($coluna) {
    if (strtolower($coluna) == 't')
        return true;

    if (strtolower($coluna) == 'f')
        return false;

    if (strtolower($coluna) == 'null' || $coluna == null)
        return '';

    return $coluna;
}

include APPRAIZ . "includes/registraracesso.php";

$podeExcluirSubacao = cte_podeEditarSubacao($_SESSION["inuid"]);
$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$capturaTipo        = cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";

//---------------------------------------------- SUBAÇÕES - PROXIMA E ANTERIOR
$sql = '
    select
        sai.sbaid,
        dim.dimcod,
        ard.ardcod,
        ind.indcod,
        ind.indid
    from cte.subacaoindicador sai
        inner join cte.acaoindicador aci on
            aci.aciid = sai.aciid
        inner join cte.pontuacao pto on
            pto.ptoid = aci.ptoid
        inner join cte.indicador ind on
            ind.indid = pto.indid
        inner join cte.areadimensao ard on
            ard.ardid = ind.ardid
        inner join cte.dimensao dim on
            dim.dimid = ard.dimid
    where
        pto.inuid = ' . $_SESSION['inuid'] . ' and
        pto.ptostatus = \'A\'
    order by
        dim.dimcod,
        ard.ardcod,
        ind.indcod,
        sai.sbaordem,
        sai.sbadsc';


$subAcoes      = (array) $db->carregar($sql);
$sbaidAnterior = false;
$sbaidProximo  = false;

while (list($k, $subacao) = each($subAcoes)) {
    if ($subacao["sbaid"] == (integer) $_REQUEST['sbaid']) {

        if (array_key_exists($k - 1, $subAcoes)) {
            $sbaidAnterior = $subAcoes[$k - 1]["sbaid"];
        }

        if (array_key_exists($k + 1, $subAcoes)) {
            $sbaidProximo  = $subAcoes[$k + 1]["sbaid"];
        }

        break;
    }
}

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
	    
		<div id="container">
			<?php cte_montaTitulo( $titulo_modulo, '' ); ?>
	
			<!-- LANÇAMENTO DE DADOS -->
	
			<?php if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO) { ?>
				<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<colgroup><col/>
					</colgroup>
					<tbody>
						<tr>
							<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
								<img align=top" src="/imagens/atencao.png" /> <p>Para elaborar o Plano de Ações Articuladas é necessário finalizar o <a href="?modulo=principal/estrutura_avaliacao&acao=A" title="Exibir diagnóstico" onclick="return redirecionarFaseDiagnostico();">diagnóstico</a>.</p>
							</td>
						</tr>
					</tbody>
				</table>
	
				<script type="text/javascript">
				<!--
					function redirecionarFaseDiagnostico(){
						window.opener.location.href = '/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A';
						window.opener.focus();
						window.close();
					}
							
					document.getElementById('loader-container').style.display = 'none';
				-->
				</script>
			<?php die();
			} // Fim de if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO) ?>
	
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
						<tr id="buttonsContainer">
							<td style="text-align : left;">
								<input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidAnterior;     ?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
							</td>
							<td style="text-align : right;">
								<input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidProximo; ?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo"        <?php echo !$sbaidProximo ? 'disabled="disabled"' : ''; ?>/>
							</td>
						</tr>
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
							 <!-- 
							<tr id="tr_categoriaDespesa">
								<td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
								<td><?php echo $sbacategoria == 3 ? 'Custeio' : 'Capital'; ?></td>
							</tr>
							 -->
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
						} // Fim de if( !CTE_NOVA_SUBACAO ){
						else {
	
	
							//------------------------------------------------------- CONSTRUÇÃO DE COMBOS
							$select_formaAtendimento = $db->monta_combo('foaid',
																		'select foaid as codigo, foadsc as descricao from cte.formaatendimento order by foadsc',
																		'S', 'Escolha...', '', '', '', '', 'N', 'foaid', true);
								
							$select_programa         = $db->monta_combo('prgid',
																		'select prgid as codigo, substr(prgdsc, 1, 100) as descricao from cte.programa order by descricao',
																		'S', 'Selecione', 'exibirPlanoInterno', '', '', '', 'S', 'prgid', true);
								
							$select_unidadeMedida    = $db->monta_combo('undid',
																		'SELECT DISTINCT uni.undid as codigo, uni.unddsc as descricao FROM cte.unidademedida uni LEFT JOIN cte.indicadorunidademedida iun ON uni.undid = iun.undid WHERE undtipo = \'' . $capturaTipo . '\' ORDER BY descricao',
																		'S', 'Escolha a unidade de medida', '', '','','','S', 'undid', true);
								
							$sql = '
								    SELECT frmid as codigo, frmdsc as descricao
								    FROM cte.formaexecucao
								    WHERE frmtipo = \'' . $capturaTipo . '\'
									AND frmbrasilpro = false
								    ORDER BY descricao';
								
							$select_formaExecucao = $db->monta_combo('frmid', $sql, 'S', 'Selecione', 'regraFormaExec', '','','','S', 'frmid', true); ?>

							<tr style="background-color: #cccccc;">
								<td align="left" colspan="2">
									<strong>Dados da Subação</strong>
								</td>
							</tr>
							<tr id="tr_categoriaDespesa">
								<td colspan="2">
									<a href="#" onclick="windowOpen('cte.php?modulo=principal/proposta/proposta_subacao&acao=A&aciid=<?php echo $aciid ?>', 'janelaProposta', 'width=400,height=300,scrollbars=yes'); return false;">
										Clique neste link para abrir opções de proposta de Subação
									</a>
								</td>
							</tr>
							<tr id="tr_categoriaDespesa">
								<td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
								<td>
									<select name="sbacategoria" class="CampoEstilo" id="sbacategoria">
										<option value="">Escolha...</option>
										<option value="3" <?php echo $sbacategoria == 3 ? 'selected="selected"' : '' ?>>Custeio</option>
										<option value="4" <?php echo $sbacategoria == 4 ? 'selected="selected"' : '' ?>>Capital</option>
									</select>
								</td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Forma de atendimento:</td>
								<td><?php echo $select_formaAtendimento; ?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Descrição da Subação:</td>
								<td>
									<?php echo campo_textarea('sbadsc', "S", 'S', '', 70, 3, 2000 ); ?>
									<input type="hidden" name="sbatexto" id="sbatexto" value="">
								</td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
								<td><?php echo campo_textarea('sbastgmpl', 'S', 'S', '', 70, 3, 2000 ); ?></td>
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
								<td><?php echo campo_texto('sbapcr', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="sbapcr"');?></td>
							</tr>
							<tr>
								<td align='right' class="SubTituloDireita">Cronograma:</td>
								<td>
									<input id="sbaporescola0" type="radio" name="sbaporescola" value="false" checked="checked" /><label for="sbaporescola0">Global</label>
									<input id="sbaporescola1" type="radio" name="sbaporescola" value="true"  /><label for="sbaporescola1">Por Escola</label>
								</td>
							</tr>
	
						<?php
						} // Fim de else
	
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
								global $db;
						     	if($_REQUEST['sbaid']){
							  		$sql = "SELECT
									            ssudescricao
									        FROM cte.subacaoparecertecnico spt
									        	INNER JOIN cte.subacaoindicador sba ON spt.sbaid = sba.sbaid
									        	INNER JOIN cte.statussubacao    ss  ON spt.ssuid = ss.ssuid 
									        WHERE spt.sbaid  = ".$_REQUEST['sbaid'] ."
											AND spt.sptano = ".$i;
							  		$status = $db->pegaUm($sql);
						     	}
						  		if($status == ''){
						  			$status = '---';
						  		}
						        echo '<td id="aba_' , $i , '" style="width: ' , $tamanhoCelula , '%;"><a onclick="return selecionarAba(' , $i , ', true);" href="' , $_SERVER['REQUEST_URI'] , '">' ,$escreve.$i , '<br>',$status,'</a></td>' , "\n";
					 		}// foreach ($_cosano as $ano)
					 		if(count($_cosano) != 1){
					 		?>
							<td id="aba_0000" style="width: <?php echo $tamanhoCelula; ?>%;"><a onclick="return selecionarAba('0000', true)" href="<?php echo $_SERVER['REQUEST_URI']; ?>">Totalizadores<br> - </a></td>
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
										<tr id="divAditivo<?php echo $i; ?>" style="display:none;">
											<td style="background-color: #ddd; height:28px; padding: 5px 5px 5px 5px; " >
												<?php if($boExibeBotaoIncluir): ?>
													<span style="cursor:pointer;" onclick="return adicionarAditivo(<?php echo $i; ?>);"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Inserir Aditivo </span>
												<?php endif;?>	
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
															<td style="background-color: #fff"><?php echo campo_texto('sptunt_' . $i, "S", "S", 'Quantitativos', 10, 25 , '', '', 'left', '', 0, 'id="sptunt_' . $i . '"');?></td>
														</tr>
													<?php } // Fim de if (!$sbaporescola) ?>
													<tr>
														<td class="SubTituloDireita">Cronograma Físico:</td>
														<td style="background-color: #fff; padding-left: 2px">
															<select name="sptinicio_<?php echo $i; ?>" id="sptinicio_<?php echo $i; ?>">
																<option value="0">Selecione</option>
																<option value="1"  <?php echo $sptinicio == '1'  ? 'selected="selected"' : '' ?>>Janeiro</option>
																<option value="2"  <?php echo $sptinicio == '2'  ? 'selected="selected"' : '' ?>>Fevereiro</option>
																<option value="3"  <?php echo $sptinicio == '3'  ? 'selected="selected"' : '' ?>>Março</option>
																<option value="4"  <?php echo $sptinicio == '4'  ? 'selected="selected"' : '' ?>>Abril</option>
																<option value="5"  <?php echo $sptinicio == '5'  ? 'selected="selected"' : '' ?>>Maio</option>
																<option value="6"  <?php echo $sptinicio == '6'  ? 'selected="selected"' : '' ?>>Junho</option>
																<option value="7"  <?php echo $sptinicio == '7'  ? 'selected="selected"' : '' ?>>Julho</option>
																<option value="8"  <?php echo $sptinicio == '8'  ? 'selected="selected"' : '' ?>>Agosto</option>
																<option value="9"  <?php echo $sptinicio == '9'  ? 'selected="selected"' : '' ?>>Setembro</option>
																<option value="10" <?php echo $sptinicio == '10' ? 'selected="selected"' : '' ?>>Outubro</option>
																<option value="11" <?php echo $sptinicio == '11' ? 'selected="selected"' : '' ?>>Novembro</option>
																<option value="12" <?php echo $sptinicio == '12' ? 'selected="selected"' : '' ?>>Dezembro</option>
															</select>
															a
															<select name="sptfim_<?php echo $i; ?>" id="sptfim_<?php echo $i; ?>">
																<option value="0">Selecione</option>
																<option value="1"  <?php echo $sptfim == '1'  ? 'selected="selected"' : '' ?>>Janeiro</option>
																<option value="2"  <?php echo $sptfim == '2'  ? 'selected="selected"' : '' ?>>Fevereiro</option>
																<option value="3"  <?php echo $sptfim == '3'  ? 'selected="selected"' : '' ?>>Março</option>
																<option value="4"  <?php echo $sptfim == '4'  ? 'selected="selected"' : '' ?>>Abril</option>
																<option value="5"  <?php echo $sptfim == '5'  ? 'selected="selected"' : '' ?>>Maio</option>
																<option value="6"  <?php echo $sptfim == '6'  ? 'selected="selected"' : '' ?>>Junho</option>
																<option value="7"  <?php echo $sptfim == '7'  ? 'selected="selected"' : '' ?>>Julho</option>
																<option value="8"  <?php echo $sptfim == '8'  ? 'selected="selected"' : '' ?>>Agosto</option>
																<option value="9"  <?php echo $sptfim == '9'  ? 'selected="selected"' : '' ?>>Setembro</option>
																<option value="10" <?php echo $sptfim == '10' ? 'selected="selected"' : '' ?>>Outubro</option>
																<option value="11" <?php echo $sptfim == '11' ? 'selected="selected"' : '' ?>>Novembro</option>
																<option value="12" <?php echo $sptfim == '12' ? 'selected="selected"' : '' ?>>Dezembro</option>
															</select>
		
															<?php
															/* TODO Estudar solução posteriormente.
															if( !CTE_NOVA_SUBACAO ){
																
																if( $_REQUEST["sbaid"] ){
																	$sql = "select frmid from cte.subacaoindicador where sbaid = ". $_REQUEST["sbaid"];
																	$frmid = $db->pegaUm( $sql );
																	
																	if( cte_possuiFormaExecucaoTecnica( $frmid ) ){ ?>
																		<label for="<?php echo 'sptanoterminocurso_' . $i?>">Ano término:</label>
																		<?php echo campo_texto('sptanoterminocurso_' . $i, "N", "S", 'Quantitativos', 10, 25 , '####', '', 'left', '', 0, 'id="sptanoterminocurso_' . $i . '"');
																	}
																}
															}  // Fim de if( !CTE_NOVA_SUBACAO ) 
															*/
															?>
															
															<label for="<?php echo 'sptanoterminocurso_' . $i?>">Ano término:</label>
															<?php echo campo_texto('sptanoterminocurso_' . $i, "N", "S", 'Quantitativos', 10, 25 , '####', '', 'left', '', 0, 'id="sptanoterminocurso_' . $i . '"'); ?>
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
													<div style="padding: 5px 0px 0px 5px;">
														<span id="adEscolas" name="adEscolas" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarEscolas(<?php echo $i; ?>);"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Escolas</span>
														<!--<span id="totalizadorEscolas<?php echo $i; ?>">0</span>-->
													</div><br />
												</td>
											</tr>
										<?php } // if ($sbaporescola) ?>
										
										<?php if( $boInsereItensEBeneficiarios ){ ?>
											<tr>
												<td>
													<div class="SubtituloEsquerda" style="padding: 5px; text-align: center"><?=ucwords($verificaCPF);?></div>
													<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="itensComposicaoSubAcao<?php echo $i; ?>"></div>
													<div id="adItensComposicao" name="adItensComposicao" style="display:inline; padding: 5px 0px 0px 5px">
															<span style="cursor:pointer;display:block;float:left" onclick="return adicionarItensComposicao();"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir <?=$verificaCPF;?></span>	
														<!--<span id="totalizadorItensComposicaoSubAcao<?php echo $i; ?>">0</span>-->
													</div><br />
												</td>
											</tr>
											<tr id="beneficiarioSubAcao<?php echo $i; ?>">
												<td>
													<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Beneficiários</div>
													<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="beneficiariosSubAcao<?php echo $i; ?>"></div>
													<div id="adBeneficiarios" name="adBeneficiarios" style="padding: 5px 0px 0px 5px">
															<span style="cursor:pointer;display:block;float:left" onclick="return adicionarBeneficiarios();"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Beneficiários</span>
														<!--<span id="totalizadorBeneficiarios<?php echo $i; ?>">0</span>-->
													</div><br />
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
														<td>
															<?php 
															echo campo_textarea('sptuntdsc_'  . $i, 'N', 'S' , '', 100, 10, null );
															
															if($boProLetramento2010){
										
																desabilitarCampoObs($i);
															} 
															?>
														</td>
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
																<tr>
																	<td align="right" class="SubTituloDireita">&nbsp;</td>
																	<td>							  	
																		<input type="checkbox" onclick="carregarParecerPadrao();" name="parecerPadrao" id="parecerPadrao_<?php echo $i ?>" />
																		<label for="parecerPadrao_<?php echo $i ?>">Adicionar Parecer Padrão</label>
																	</td>
																</tr>
															<?php }
														} // Fim de if($_REQUEST["sbaid"] != '') ?>
														
														<tr>
															<td align="right" class="SubTituloDireita">Parecer:</td>
															<td><?php echo campo_textarea('sptparecer_' . $i, 'N', $disableParecer, '', 100, 12, 0 ); ?></td>
														</tr>
														
														<tr>
															<td align="right" class="SubTituloDireita">Status Subação:</td>
															<td>
																<?php
																$db->monta_combo(	'ssuid_' . $i,
																					"select ssuid as codigo, ssudescricao as descricao from cte.statussubacao WHERE ssutipostatus = 'E'",
																					$disableParecer, 'Selecione', '', '', '', '', '', 'ssuid_' . $i ); ?>
															</td>
														</tr>
		
													<?php } // Fim de if (CTE_PRM_VISUALIZAR_PARECER) ?>
												</table><br />
											</td>
										</tr>
										
										<?php 
										if(!$admTemp){
										if (!CTE_PRM_EMITIR_PARECER) {
											echo '<script type="text/javascript">$("sptparecer_' , $i , '","ssuid_' , $i , '").each(function(e){if(e)e.disable()});</script>';
										} } ?>
										
									</table>
								</td>
							</tr>
						<?php } // foreach ($_cosano as $ano) ?>
	
						<tr class="container" style="display: none; height: auto" id="abaCosano0000">
							<td align="left" colspan="<?php echo $colspan; ?>">
								<div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="totalizadoresSubacao"></div>
							</td>
						</tr>
						<tr>
							<td align="center" colspan="<?php echo $colspan; ?>">
								<table width="100%">
									<tr>
										<td colspan="3" style="font-weight:bold;color:red;text-align : center; display:none" id="errorMessageContainer">
											Documentos em análise financeira devem conter Escolas, Itens de Composição e Beneficiários cadastrados.
										</td>
									</tr>
									<?php 
									/************************************************************	
									* 	BOTÕES DO RODAPÉ
									* 	DESCRIÇÃO: 
									*   Se existe sbaid pai então mostra BTNS do Aditivo.
									*************************************************************/
									if( $_REQUEST['sbaidpai'] ){ 
									?>
									<tr id="buttonsContainer">
											<td style="width: 25%;text-align : left;"><input type="hidden" name="vlrProximaSubacao" value="" id="vlrProximaSubacao" />
											</td>
											<td style="text-align : center;">
												<input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar" value="Gravar" id="btnSalvar" <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?> />
												<br />
												<input onclick="return limparSubacao('<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnLimparSubacao" value="Limpar Subação" id="btnLimparSubacao" />
												<input onclick="return submeterFrmParSubacao(true,  -1);" type="button" name="btnExcluir" value="Excluir" id="btnExcluir" <?php echo !CTE_PRM_EXCLUIR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
												<input onclick="return fecharJanela();" type="button" name="btnCancelar"       value="Fechar"            id="btnCancelar" />
											</td>
											<td style="width: 25%;text-align : right;">
												<input onclick="return voltarSubacaoPai('<?php echo $_REQUEST['sbaidpai']; ?>');" type="button" name="btnVoltar" value="Voltar para Subação" id="btnVoltar"/>
												<div id="divImportardados"></div>
											</td>
										</tr>
										
									<?php }else{ // Está na subação padrão. ?>
									<tr id="buttonsContainer">
										<td style="width: 25%;text-align : left;"><input type="hidden" name="vlrProximaSubacao" value="" id="vlrProximaSubacao" />
											<input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidAnterior;     ?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
										</td>
										<td style="text-align : center;">
											
											<input onclick="return submeterFrmParSubacao(true,  '<?php echo $sbaidAnterior;     ?>');" type="button" name="btnSalvarAnterior" value="Gravar e Anterior" id="btnSalvarAnterior" <?php echo !$sbaidAnterior || !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
											<input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar"         value="Gravar"            id="btnSalvar"         <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?> />
											<input onclick="return submeterFrmParSubacao(true,  '<?php echo $sbaidProximo;      ?>');" type="button" name="btnSalvarProximo"  value="Gravar e Próxima"  id="btnSalvarProximo"  <?php echo !$sbaidProximo  || !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
											<br />
											<div id="divImportardados"></div>
											<?php if( CTE_PRM_EDITAR_SUBACAO ){ ?>
												<input onclick="javscript: toggleDiv( 'divImportardados' );" type="button" name="btnImportarDados" value="Importar Dados" id="btnImportarDados" />
											<?php } ?>
											<input onclick="return limparSubacao('<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnLimparSubacao" value="Limpar Subação" id="btnLimparSubacao" />
											<input onclick="return submeterFrmParSubacao(true,  -1);" type="button" name="btnExcluir" value="Excluir" id="btnExcluir" <?php echo !CTE_PRM_EXCLUIR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
											<input onclick="return fecharJanela();" type="button" name="btnCancelar"       value="Fechar"            id="btnCancelar" />
										</td>
										<td style="width: 25%;text-align : right;">
											<input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidProximo; ?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo"        <?php echo !$sbaidProximo ? 'disabled="disabled"' : ''; ?>/>
										</td>

									<?php } ?>
								</table>
							</td>
						</tr>
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
	    var anoExercicio            = '<?php echo ($_REQUEST['parsubacaoanocrt'])?$_REQUEST['parsubacaoanocrt']:2011;  ?>';
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
	    var anoConvenio 	= '<?php echo $anoConvenio ?>';

	    var novaSub 		= <?php echo $novaSub = CTE_NOVA_SUBACAO ? 'true' : 'false'; ?>;
	    
	    var reqt 		= '<?php echo $reqt; ?>';
	    var porEscola 	= '<?php echo $sbaporescola ? 't' : 'f'; ?>';
	    var subacao		= <?php echo $_REQUEST['sbaid'] ? $_REQUEST['sbaid'] : 0;  ?>;
	    var eAditivo	= <?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>;
	    var PIVisivel	= <?php echo CTE_PRM_VISUALIZAR_PI ? CTE_PRM_VISUALIZAR_PI : 0; ?>;

	    /**
	     * FIM
	     * @public
	     * @global
	     */
	    
</script>

<script type="text/javascript" src="../cte/includes/par_subacao.js"></script>

<script type="text/javascript">

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
	                                        <?php if (!CTE_NOVA_SUBACAO) { ?>
	                                       
	        										var anoConvenioTemp = '<?php  echo $anoConvenio ? $anoConvenio : $_REQUEST['anoconvenio'];?>';
	           										var boExibeBotaoIncluir = '<?php echo $boExibeBotaoIncluir;?>';
	           										
	                                        		if( (<?=$admTemp; ?> == 2) && (<?=$super;?> == 2)  ){ // **@@&& retirar depois o if apos reformulação - se não for administrator temporario trava.
	           											if( ( <?= CTE_ESTADO_PAR?> == <?=$documento["esdid"]; ?> ) && <?php echo (integer) !cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ?>   ){ // excessao dos perfils CTE_PERFIL_EQUIPE_MUNICIPAL, CTE_PERFIL_EQUIPE_ESTADUAL_CADASTRO, CTE_PERFIL_PARECERISTA_FISICO_FINANCEIRO no final do arquivo
															if(anoConvenioTemp == cosano && boExibeBotaoIncluir ){
																if(<?=$anoConvenio; ?>){
																	$('divAditivo' + cosano).style.display="table-row";
																}
															} else {
																travaAbaAnoJaAnalisada(cosano);
															}
	           	 										}else if(<?=$anoConvenio; ?>){
		           	 										if(anoConvenioTemp == cosano && boExibeBotaoIncluir ){
		           	 											if(<?=$anoConvenio; ?>){
																	$('divAditivo' + cosano).style.display="table-row";
		           	 											}
															} else {
               													travaAbaAnoSeAditivada(cosano);
															}
	               										} 
		               									if(<?=$anoConvenio; ?> != cosano &&  !(<?=$super;?> != 1 || <?=$adm;?> != 1 ) ){
		               											travaAnosAnteriores(cosano);
		               									}
	               									}else{
	               										if(<?=$anoConvenio; ?> >= cosano ){ // **@@&& retirar depois o if apos reformulação se for 2007 - 2008 - 2009 mostra add aditivo 
	               												$('divAditivo' + cosano).style.display="table-row";//**@@&& Mostra botão de Adicionar Aditivo.  retirar apos a reformulação
	               										}
	               									}
	               									
            									<? } ?>
	                                        $('divListaAditivo' + anoExercicio).innerHTML = res.responseText; // Mostra lista de Aditivos
	                                        $('loader-container').hide();


//	                                        var cosanoTemp = '<?php echo $cosano;?>';
//	                                        var anoConvenioTemp = '<?php echo $anoConvenio;?>';
//	                                        var boExibeBotaoIncluir = '<?php echo $boExibeBotaoIncluir;?>';
//
//	                                        if(anoConvenioTemp == cosano && boExibeBotaoIncluir ){
//	                                        	$('divAditivo' + cosano).style.display="table-row";
//	                                        }
	                                        
	                                    }
	                                });
	    }
		<?php
		//------------------------- FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA

		if ($sbaporescola) {
    		if (CTE_PRM_EDITAR_SUBACAO) { 
    	?>
			    
			    function removerEscola(qfaid)
			    {
			        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
			            return false;
			        }
			        return new Ajax.Request(window.location.href,
			                                   {
			                                       method: 'post',
			                                       parameters: '&reqt=<?php echo $reqt; ?>&req=removerEscola&qfaid=' + qfaid,
			                                       onComplete: function(res)
			                                       {
			                                           carregarEscolas();
			                                       }
			                                   });
			        return false;
			    }
			
			<?php
			} // Fim de if (CTE_PRM_EDITAR_SUBACAO) 
    		else {
				echo 'function removerEscola() {return false;}';
			} ?>


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
                carregarDivImportacao( cosano );
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

<? } else { ?>

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
                carregarDivImportacao( cosano );
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
    
<?php } ?>

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
    
    function calculaValorTotalGeral(){
    
    	cosid = document.getElementsByName('cosid[]');    	
    	for(x=0;x<cosid.length;x++){
    	
    		nrcosid = cosid[x].value;
    		total += string2float($('cosvlrtotal' + nrcosid).value);
    	} 	        
        
        $('totalGeral').innerHTML = float2string(total);
        total = 0;
    
    }

	function calcularValorTotal(cosid)
    {
        
        var vlrtotal 	= 	string2float($F('cosqtd'    + cosid)) *
                       		string2float($F('cosvlruni' + cosid)) ;
        var vlrtotal2 	= 	string2float($F('cosqtd'    + cosid)) * 
        					string2float($F('cosvlruni' + cosid)) ;
        vlrtotal2		= 	parseFloat(vlrtotal2).toFixed(6);
        
        cosvlruni 		= 	string2float($('cosvlruni' + cosid).value);
        Fcosvlruni		= 	string2float($('Fcosvlruni' + cosid).value).toFixed(2);

        $('cosvlrtotal' + cosid).value = float2string(vlrtotal);                
        $('Fcosvlrtotal' + cosid).value = vlrtotal2.replace(".",",");
        
        if(cosvlruni != Fcosvlruni){        
        	$('Fcosvlruni' + cosid).value = $('cosvlruni' + cosid).value;
        }
        
        calculaValorTotalGeral();
                
    }
    
    function calcularValorUnitario(cosid)
    {
        var vlrunitario 	= 	string2float($F('cosvlrtotal' + cosid)) /
                       			string2float($F('cosqtd'    + cosid));
        var vlrunitario2 	= 	string2float($F('cosvlrtotal' + cosid)) / 
        						string2float($F('cosqtd'    + cosid));
        vlrunitario2		= 	parseFloat(vlrunitario2).toFixed(6);
        
        cosvlrtotal 		= 	string2float($('cosvlrtotal' + cosid).value);
        Fcosvlrtotal 		= 	string2float($('Fcosvlrtotal' + cosid).value).toFixed(2);        
                       		
        $('cosvlruni' + cosid).value = float2string(vlrunitario);                		
		$('Fcosvlruni' + cosid).value = vlrunitario2.replace(".",",");
		
		if(cosvlrtotal != Fcosvlrtotal){
			$('Fcosvlrtotal' + cosid).value = $('cosvlrtotal' + cosid).value;
		}
		
		calculaValorTotalGeral();
    }
    
    function string2float(str)
    {
        if (str.indexOf(',') == -1)
            return parseFloat(str);
        else
            return parseFloat(str.replace(/\./g, '').replace(/,/g, '.'));
    }
    
    function float2string(num)
    {
        return mascaraglobal('###.###.###.###.###,##', (parseFloat(num).toFixed(2)) + '');
    }    

<?php
if (!CTE_PRM_EDITAR_SUBACAO) {

?>
    function submeterFrmParSubacao(submit, vlrProximaSubacao)
    {	
        if (vlrProximaSubacao != '') {
            window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' + vlrProximaSubacao + '&bloqueado=1';
        } else {
            window.opener.location.reload();
            window.opener.focus();
            window.close();
        }
    }
    // 2083
    Form.disable('frmParSubacao');

    var btnCancelar = $('btnCancelar');
    var btnProximo  = $('btnProximo');
    var btnAnterior = $('btnAnterior');

    if (btnCancelar)
        btnCancelar.removeAttribute('disabled');

    if (btnProximo)
        btnProximo .removeAttribute('disabled');

    if (btnAnterior)
        btnAnterior.removeAttribute('disabled');

    //---------------------------------------------------------------- POP-UPS
    function popupSelecionarEscolas(qfaano)
    {
        return false;
    }

    function popupEditarSubacao()
    {
        return false;
    }

    function popupSelecionarEscolasItemComposicao(input, cosid)
    {
        return false;
    }

    function adicionarItensComposicao()
    {
        return false;
    }

    function adicionarBeneficiarios()
    {
        return false;
    }

    function removerItemComposicao(cosid)
    {
        return false;
    }

    function removerBeneficiario(benid)
    {
        return false;
    }
<?php
} else {
?>

    var submeter = false;
    //---------------------------------------------------------------- POP-UPS
    /**
     * 
     */
    function adicionarItensComposicao()
    {
        if (itemModificado)
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {validacao: 'false'},
                onComplete: function()
                {
                    $('loader-container').hide();
                }
            });

        return windowOpen('/cte/cte.php?modulo=principal/cadastraritenssubacao&acao=A&undid=<?php echo $undid ?>&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosano=' + anoExercicio +'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>+ '&ad='+<?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>,
                          'adicionarItensComposicao',
                          'height=400,width=990,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }


    /**
     * 
     */
    function adicionarBeneficiarios()
    {
        return windowOpen('/cte/cte.php?modulo=principal/cadastrarbeneficiariossubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&sabano=' + anoExercicio+'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>+ '&ad='+<?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>,
                          'adicionarBeneficiarios',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function removerItemComposicao(cosid, cspid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }
		
        return new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&reqt=<?php echo $reqt; ?>&req=removerItemComposicao&cosid=' + cosid +'&cspid='+cspid,
                                       onComplete: function(res)
                                       {
                                           //$('divTeste').update(res.responseText);
                                           carregarItensComposicao();
                                           var arCPF = document.getElementsByName( 'cpf[]' );
		                                   for( o=0; o<arCPF.length; o++ ){
		                                      var cpf = arCPF[o].innerHTML;
		                                   	  arCPF[o].innerHTML = mascaraglobal( '###.###.###-##', cpf );
		                                   }
                                       }
                                   });

        return false;
    }


    /**
     * 
     */
    function removerBeneficiario(benid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        var req = new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&reqt=<?php echo $reqt; ?>&req=removerBeneficiario&benid=' + benid + '&sabano=' + anoExercicio,
                                       onComplete: function(res)
                                       {
                                           carregarBeneficiarios();
                                       }
                                   });

        return false;
    }

    /**
     * 
     */
    function popupSelecionarEscolas(qfaano)
    {
        return windowOpen( '/cte/cte.php?modulo=principal/cadastrarescolassubacao&acao=A&tipo=cadastrarEscolas&sbaid=<?php echo $_REQUEST['sbaid']; ?>&qfaano=' + qfaano,
                           'popupSelecionarEscolas',
                           'height=400, width=800, status=yes, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=yes' );
        return false;
    }

    /**
     * 
     */
    function popupEditarSubacao()
    {
        return windowOpen('/cte/cte.php?modulo=principal/popupeditarsubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>',
                          'editarSubacao',
                          'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function popupSelecionarEscolasItemComposicao(input, cosid)
    {
        return windowOpen('/cte/cte.php?modulo=principal/escolasescolhasubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosid='+cosid+'&qfaano='+anoExercicio +'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>,
                          'escolherEscolasItemComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
    }
    
    function submeterFrmParSubacao(submit, vlrProximaSubacao)
    {
        $('anoExercicio').value = anoExercicio;
        
        var totalItens = document.getElementById('totalitensc');
        var anoRef = document.getElementById('anoRef');

		        
        
        <?php 
        	if($boProLetramento2010) {
        		
        		$qtdProLetramento2010 = pegaDadosProLetramento2010();
        		$qtdProLetramento2010 = $qtdProLetramento2010['qtditensoriginal'];       		 
        ?>
        
        if(anoRef){
        
        	var qtdPrevistaPro = document.getElementById('sptunt_'+anoRef.value);
        	var qtdProLetramento2010 = <?=$qtdProLetramento2010;?>;
        	
        	if(totalItens && qtdProLetramento2010){
        
		       	if(totalItens != '' && qtdProLetramento2010 != ''){
		       	
		       		if(totalItens.value > qtdProLetramento2010){
		       		
		        		alert('A quantidade de itens atual não pode ser \n maior que a quantidade de itens inicial?')		        		
		        		return false;
		        	}
		       	}
        	}
        }
        	
        <?php } ?>
        
        if(anoRef){
        
        	if(anoRef.value != ''){
        	
        		var qtdPrevista = document.getElementById('sptunt_'+anoRef.value);
        		
        		if(totalItens && qtdPrevista){
        
		        	if(totalItens != '' && qtdPrevista != ''){
		        	
			        	if(totalItens.value > qtdPrevista.value){
			        		if(!confirm('A quantidade de itens preenchida maior que a prevista!\nDeseja continuar?')){
			        		qtdPrevista.focus();
			        		return false;
			        		}
			        	}
		        	}
		        
		        }
        	}
        }        

		// Exclusão de sub-ação
        if (vlrProximaSubacao == '-1' && permitirCadastroSubacao) {
            if (confirm('Deseja realmente excluir a subação?')) {
                $('vlrProximaSubacao').value = vlrProximaSubacao;
                $('frmParSubacao').submit();
                return true;
            } else {
                return false;
            }
        }

		// Gravar sub-ação
        if (submit) {
            var sbapcr = $('sbapcr');

			<?php if (CTE_NOVA_SUBACAO) {
                echo 'while (sbapcr.disabled)';
            } ?>
            Form.enable("frmParSubacao");
			var prgidteste         = $('prgidteste');
            var sptunt             = $('sptunt_'             + anoExercicio);
            var sptinicio          = $('sptinicio_'          + anoExercicio);
            var sptfim             = $('sptfim_'             + anoExercicio);
            var sptanoterminocurso = $('sptanoterminocurso_' + anoExercicio);
            var numprocesso         = $('cvrnumprocesso_'	 + anoExercicio);
            /*
            var numprocesso         = $('semnumprocesso'	 + anoExercicio);
            
            if(numprocesso){
            	var numprocessoSelecionado = $('cvrnumprocesso_' + anoExercicio);  
            	if(!numprocessoSelecionado.value){
            		alert(  "Não foi possível gravar essa subação.\n"+
							"Selecionar um número de processo.\n");
					return false;	
            	} 
            }
            */

			if( (prgidteste.value && PIVisivel != 0 )  ){
				var plinumplanointerno = $('plinumplanointerno_' + anoExercicio);      
				if(!plinumplanointerno.value && $('semplanointerno'+ anoExercicio).value == 'true'){
					alert(  "Não foi possível gravar essa subação.\n"+
							"Selecionar um plano interno\n");
					return false;	
				}
			}
			
			// Verificando se cronograma físico e quantidade estão preenchidos.			
			if( vlrProximaSubacao ){
				<?php
				if( $_REQUEST['sbaid'] ){
									
					$sql = "select sbaporescola from cte.subacaoindicador where sbaid = ". $_REQUEST['sbaid'];
					$sbaPorEscola = $db->pegaUm($sql); ?>
				
					sbaporescola = '<?php echo $sbaPorEscola ?>';
					
					<?php
					if( $estadoDocumento["esdid"] == CTE_ESTADO_PAR ){ ?> 
						
						var boErro = false;
						var complementoQtd = "";
						var complementoDatas = "";
						
						if( sbaporescola == 'f' ) {
							var bosptunt = sptunt.value == "" || sptunt.value == 0 ? false : true;
							
							if( !bosptunt ){
								boErro = true;
								var complementoQtd = "- Quantidades;\n"; 
							}
						}
							
						var bosptinicio = sptinicio.value == "" || sptinicio.value == 0 ? false : true;
						var bosptfim = sptfim.value == "" || sptfim.value == 0 ? false : true;
						
						if( !bosptinicio || !bosptfim ){
								boErro = true;
								var complementoDatas = "- Cronograma Físico (início e fim)."; 
						}
						
						if( boErro ){
							alert(  "Não foi possível gravar essa subação.\n"+
									"Favor verificar se todos os campos listados abaixo estão preenchidos:\n"+
									complementoQtd +
									complementoDatas );
							return false;						
						}
					
					<?php } // Fim de if( $estadoDocumento["esdid"] == CTE_ESTADO_PAR )
				} // Fim de if( $_REQUEST['sbaid'] ) ?>
			} // Fim de if( vlrProximaSubacao ){

            if (vlrProximaSubacao != '' && sptanoterminocurso) {
                if (sptanoterminocurso.value == '' && (Number(sptinicio.value) > Number(sptfim.value))) {
                    alert('O mês fim para o campo Cronograma Físico deve ser maior ou igual o mês inicial ou o ano de término deve ser preenchido.');
                    return false;
                } else if (sptanoterminocurso.value != '' && Number(sptanoterminocurso.value) <= anoExercicio) {
                    alert('O ano de término do cronograma deve ser maior do que o ano do lançamento do exercício.');
                    return false;
                }
            }

            if (vlrProximaSubacao != '') {
                $('vlrProximaSubacao').value = vlrProximaSubacao;
            }

            var errors    = new Array();

            var sbadsc    = $('sbadsc');
            var sbastgmpl = $('sbastgmpl');
            var prgid     = $('prgid');
            var undid     = $('undid');
            var frmid     = $('frmid');

            if (sbadsc    && sbadsc.value == '')
                errors.push('Descrição');

            if (sbastgmpl && sbastgmpl.value == '')
                errors.push('Estratégia de Implementação');

            if (prgid     && (prgid.value == '0' || prgid.value == ''))
                errors.push('Programa');

            if (undid     && (undid.value == '0' || undid.value == ''))
                errors.push('Unidade de Medida');

            if (frmid     && (frmid.value == '0' || frmid.value == ''))
                errors.push('Forma de Execução');
			
            if (errors.length > 0) {
                if (errors.length == 1)
                    alert('O campo ' + errors.join('') + ' é obrigatório!');
                else
                    alert('Os campos abaixo são obrigatórios:\n - '
                         +errors.join('\n - ')
                         +'\n\nPor favor corrija para continuar!');

                return false;
            } 
            else {
				<?php if (in_array($estadoDocumento['esdid'], array(30,31,26))) { ?>
	                if (vlrProximaSubacao != '' && total <= 0) {
	                    alert('Em fase de análise financeira, a sub-ação deve conter Escolas, Itens de Composição e Beneficiários viculados.');
	                    return false;
	                }
				<?php 
				} // if ($estadoDocumento['esdid'] == 59)@1823 ?>

                var submeter = true;
                var inputs   = $('frmParSubacao').getInputs('text');

				<?php if ($labid == 'null' && ($frmid != 4 && $frmid != 10)) { ?>
	                for (var i = 0; i < inputs.length; i++) {
	                    if (/cosvlruni/.test(inputs[i].getAttribute('name')) ||
	                        /cosqtd/   .test(inputs[i].getAttribute('name'))) {
	                        if (string2float(inputs[i].value) <= 0) {
	                            inputs[i].setStyle({backgroundColor: '#ffd'});
	                            submeter = false;
	                        } else {
	                            inputs[i].setStyle({backgroundColor: '#fff'});
	                        }
	                    }
	                }
				<?php
				} ?>
                if (submeter) {
                    if (vlrProximaSubacao != '-1' && vlrProximaSubacao != '') {
                        if (confirm('Atenção!\nOs dados serão gravados somente para o ano ' + anoExercicio + '.\n\nDeseja continuar?')) {
                            $('loader-container').show();
                            $('frmParSubacao').request(
                            {
                                encoding: 'ISO-8859-1',
                                parameters: {
                                    anoExercicio: anoExercicio
                                },
                                onComplete: function(response)
                                {
                                    eval(response.responseText);
                                    $('loader-container').hide();
                                }
                            });
                        }
                    } 
                    else {
                        $('loader-container').show();
                        $('frmParSubacao').request(
                        {
                            encoding: 'ISO-8859-1',
                            parameters: {
                                anoExercicio: anoExercicio,
                                validacao: 'false'
                            },
                            onComplete: function(response)
                            {
                                eval(response.responseText);
                                $('loader-container').hide();
                            }
                        });
                    }
                    return false;
                } 
                else {
                    alert('O dados para Quantidade e Valor Unitário dos Itens de Composição devem ser preenchidos.');
                    return false;
                }
            }
        } 
        else {
            if (vlrProximaSubacao != '') {
                window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=' + vlrProximaSubacao;
            } 
            else {
                window.opener.location.reload();
                window.opener.focus();
                window.close();
            }
        }
    }

<?php

} // if (!CTE_PRM_EDITAR_SUBACAO) {

if (trim($_REQUEST['sbaid']) != '')
	echo '        selecionarAba(anoExercicio, true);';
else {
    echo '        $("alertaPermitirCadastroItensSubacao").show();
        $("tabAbasSelecaoCosano").hide();
        $("btnExcluir").setAttribute("disabled", "disabled");';
}

?>

    //$('loader-container').hide();

<?php

if (CTE_NOVA_SUBACAO && $capturaTipo == 'M' && !cte_verificaGrandeMunicipio($_SESSION['inuid'])) {

?>
    Form.disable('frmParSubacao');
    var btnCancelar = $('btnCancelar');

    if (btnCancelar)
        btnCancelar.removeAttribute('disabled');

<?php

} // if (CTE_NOVA_SUBACAO && $capturaTipo == 'M')

?>
    function propostaSubAcao(arr)
    {
        $('ppsid').value        = arr['ppsid'];
        $('prgid').value        = arr['prgid'];
        $('frmid').value        = arr['frmid'];
        $('sbadsc').value       = arr['ppsdsc'];
        $('sbaordem').value     = arr['ppsordem'];
        $('sbastgmpl').value    = arr['ppsmetodologia'];
        $('sbacategoria').value = arr['sbacategoria'];
        $('sbatexto').value 	= arr['ppstexto'];
        $('sbaobjetivo').value 	= arr['ppsobjetivo'];
		
        for (var i = 0, length = $('undid').options.length; i < length; i++) {
            if ($('undid').options[i].value == arr['undid'])
                $('undid').selectedIndex = $('undid').options[i].index;
        }

        Form.disable('frmParSubacao');

        var btnSalvar   = $('btnSalvar');
        var btnCancelar = $('btnCancelar');

        if (btnSalvar)
            btnSalvar.removeAttribute('disabled');

        if (btnCancelar)
            btnCancelar.removeAttribute('disabled');

        if (arr['indqtdporescola'] == 'f') {
            $('sbaporescola0').checked = true;
            $('sbaporescola0').enable();
            $('sbaporescola1').enable();
        } else {
            $('sbaporescola1').checked = true;
        }
    }

<?php

if (CTE_NOVA_SUBACAO) {
    echo '$(\'loader-container\').hide();';

} else {
?>

    function limparSubacao(sbaid)
    {
        if (confirm('Atenção!\nTodo o preenchimento do ano ' + anoExercicio + ' da subação será excluído.\n'
                   +'Os dados NÃO poderão ser recuperados posteriormente.\n\nDeseja continuar?'))
        {
            $('loader-container').show();
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {
                    anoExercicio: anoExercicio,
                    req: 'limparSubacao',
                    formularioSumit: 0
                },
                onComplete: function(response)
                {
                    eval(response.responseText);
                    $('loader-container').hide();
                }
            });
        }
    }


<?php } 
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

		function carregarDivImportacao( anoCorrente ){
			
			var obDiv = document.getElementById( "divImportardados" );
			obDiv.innerHTML = '';
			
			var obLabel = document.createElement( "label" );
			obLabel.setAttribute( "for", "csAno" );
			obLabel.innerHTML = "Importar Dados de: ";
			
			var obSelect = document.createElement( "select" );
			obSelect.setAttribute( "id", "csAno" );
			obSelect.setAttribute( "name", "csAno" );
			
			var arAno = new Array();
			arAno[0] = 2007;
			arAno[1] = 2008;
			arAno[2] = 2009;
			arAno[3] = 2010;
			arAno[4] = 2011;
				
			for( i=0; i<arAno.length; i++){
			
				if( arAno[i] == anoCorrente ) continue;
				
				var obOption = document.createElement( "option" );
				obOption.setAttribute( "value", arAno[i] );
				obOption.innerHTML = arAno[i];
				
				obSelect.appendChild( obOption );
			}
			
			var obButton = document.createElement( "input" );
			obButton.setAttribute( "type", "button" );
			obButton.setAttribute( "id", "btnImportar" );
			obButton.setAttribute( "name", "btnImportar" );
			obButton.setAttribute( "value", "Importar" );
			
			if(window.addEventListener){ // Mozilla, Netscape, Firefox
				obButton.setAttribute( "onclick", "javascript: importarDados();" );
			} 
			else { // IE
				obButton.attachEvent( "onclick", importarDados );
			}
			
			
			
			obDiv.appendChild( obLabel );
			obDiv.appendChild( obSelect );
			obDiv.appendChild( document.createElement( "br" ) );
			obDiv.appendChild( document.createElement( "br" ) );
			obDiv.appendChild( obButton );
			
				
		
		}
		
		function importarDados(){
		
			var csAno = document.getElementById( 'csAno' ).value;
			
			if( confirm( "Esta operação adicionará todos os dados de itens de composição e de escolas (se for o caso) do ano de " + csAno + " para o ano em questão!\n\nDeseja continuar?" ) ){
			
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{
											method: 'post',
											parameters: '&req=importarDados&anoExercicio=' + anoExercicio + '&csAno=' + csAno,
											onComplete: function(res){
												eval( res.responseText );
		                    					$('loader-container').hide();
											}
										});	
			}
		}
		
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
		
	</script>
</html>
<?php 

function desabilitarCampoObs($ano){

	echo "<script>			
			var campo = document.getElementById('sptuntdsc_{$ano}');
			campo.disabled = 'disabled';			
		 </script>";
}

die(); 
?>