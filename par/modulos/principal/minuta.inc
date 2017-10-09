<?php

if($_REQUEST['alterarminuta']){
    header('content-type: text/html; charset=ISO-8859-1');
    alterarDadosMinuta($_POST);
    exit;
}

if($_REQUEST['geraTermoReprogramacao'] == 1 ){ 
    unset($_SESSION['par_minuta']['iueid']);
} 

if( $_POST['requisicao'] == 'email' ){
    if( !empty($_GET['dopid']) ){
        $arConvenio = $db->pegaLinha( "SELECT 
                                            empanoconvenio, empnumeroconvenio 
                                        FROM par.empenho e
                                        INNER JOIN par.processopar p on p.prpnumeroprocesso = e.empnumeroprocesso 
                                        WHERE p.prpid = ".$_GET['prpid']." and empstatus = 'A'");
        $strAssunto = 'Publicação - SIMEC Convênio '.$arConvenio['empnumeroconvenio'].'/'.$arConvenio['empanoconvenio'];		
        //$strMensagem = $db->pegaUm("SELECT doptexto FROM par.vm_documentopar_ativos WHERE dopid = {$_GET['dopid']}");		
                
        $strMensagem = pegaTermoCompromissoArquivo( $_GET['dopid'], '');
        
        $remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
        $strMensagem = html_entity_decode($strMensagem);
        if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
            $retorno = true;
        } elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
            $strEmailTo = array($_SESSION['email_sistema']);
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);		
        } else {
            $strEmailTo = 'dicom_publicacao@fnde.gov.br';
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }
        if( $retorno ){
            echo "<script>alert('E-mail enviado com sucesso!');
                    window.location.href = window.location;
              </script>";
        } else {
            echo "<script>alert('Não foi possível enviar o e-mail!');
                    window.location.href = window.location;
              </script>";
        }
    } else {
        echo "<script>alert('Não foi possível enviar o e-mail, é necessário cadastrar uma publicação!');
                window.location.href = window.location;
            </script>";
    }
    exit();
}

if($_GET['imprimeWord']){
    header("Content-type: application/vnd.ms-word");
    header("Content-type: application/force-download");
    header("Content-Disposition: attachment; filename=MinutaConvenioDou.doc");
    header("Pragma: no-cache");

	$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );
	
    echo "<html>".html_entity_decode($doptexto)."</html>";
    exit;
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if($_REQUEST['pegaDataFinal']){
	header('content-type: text/html; charset=ISO-8859-1');
	if( !empty($_POST['dopdiasvigencia']) )
		getDataFinalConvenio($_POST['dopdiasvigencia'], $_POST['dopdatainicio']);
	else{
		$dopdatainicio = formata_data_sql($_POST['dopdatainicio']);
		$dopdatafim = formata_data_sql($_POST['dopdatafim']);
		$dias = dateDiff1(strtotime($dopdatainicio),strtotime($dopdatafim));
 		echo $dias;
	}		
	exit;
}

if( $_REQUEST['dopid'] ){
	
	global $db;
	
	// Reprogramação de Subação
	$sql = "SELECT dopidoriginal FROM par.reprogramacao WHERE dopidreprogramado = ".$_REQUEST['dopid']." AND repstatus = 'A'";
	$testaReprogramacao = $db->pegaUm($sql);

	// Reprogramação de Prazo
	$sql = "SELECT dopid FROM par.documentoparreprogramacao WHERE dopidreprogramado = ".$_REQUEST['dopid']." AND dprstatus = 'P'";
	$testaReprogramacaoPrazo = $db->pegaUm($sql);
	
}

function getDataFinalConvenio($dias, $data){
	$arData = explode('/', $data);

	$dia = $arData[0] - 1;
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dias, 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	echo $dataFormatada;
}

function dateDiff1($from,$to){
	$diff = $to - $from;
	$info = array();
	if($diff>86400){
		$info['d'] = ($diff - ($diff%86400))/86400;
		$diff = $diff%86400;
	}
	$f = '';
	foreach($info as $k=>$v){
		if($v>0) $f .= "$v";
	}
	return ($f == 24) ? "1" : $f;
}
$prpid = $_GET['prpid'];
$dopid = $_GET['dopid'];


if( empty($prpid) ){
	echo "<script>
                    alert('Dados Insuficientes.');
                    window.close();
                    history.back(-1);
              </script>";
	die;
}

/* Modificado dia 12/05/2014
 * A pedido de: Thiago
* Programador: Eduardo Dunice Neto
* */
if( $_POST['requisicao'] == 'finalizarReprogramacaoPrazo' ){
	global $db;
	
	$sql = "SELECT
			  prpid, dopstatus, dopdiasvigencia, to_char(dopdatainicio, 'DD/MM/YYYY HH24:MI:SS') as dopdatainicio, to_char(dopdatafim, 'DD/MM/YYYY HH24:MI:SS') as dopdatafim,
			  mdoid, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
			  dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
			  dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
			  dopreformulacao, dopidpai, dopdatainiciovigencia
			FROM 
			  par.documentopar WHERE dopid = {$_POST['dopid']} and dopstatus = 'A'";
			  
	$arrDadosDoc = $db->pegaLinha( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
	
	$sql = "UPDATE par.documentoparreprogramacao SET dprstatus = 'A' WHERE dopidreprogramado = ".$_POST['dopid']." AND dprstatus = 'P'";
	$db->executar($sql);
	$db->commit();
	
//	gerarRelatorioEscolaItenPorQuantidade($subacoes, $dopid);
	
	enviaEmailNovoTermo($_REQUEST['dopid'], 'reprogramação');

	echo "
		<script>
			alert( 'Reformulação Finalizada.' );
			window.location.href = 'par.php?modulo=principal/minuta&acao=A&prpid={$arrDadosDoc['prpid']}&dopid={$_POST['dopid']}';
		</script>";
	die();
}
/* FIM - Modificado dia 12/05/2014
 * A pedido de: Thiago
* Programador: Eduardo Dunice Neto
* */

if( $_POST['requisicao'] == 'salvar' ){    
	global $db;
        $strUpdate = '';
        if ($_POST['iueid'] != '') {
            $_SESSION['par_minuta']['iueid'] = $_POST['iueid'];
            $strUpdate = ", iueid = {$_SESSION['par_minuta']['iueid']}";
        }
        
	$texto = $_POST['texto'];
	if( $texto ){
		if( strpos($texto, '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
			$texto = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $texto );
		} else {
			$texto = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $texto );
		}
	}
	$doptexto = $texto ? "'".simec_htmlspecialchars($texto, ENT_QUOTES, 'ISO-8859-1')."'" : 'null';
	
    /* $sql = "UPDATE par.documentopar SET doptexto = ".$doptexto.", dopdataalteracao = now(), usucpfalteracao = '{$_SESSION['usucpf']}' $strUpdate WHERE dopid = ".$_REQUEST['dopid'];
	$db->executar($sql);
	$db->commit(); */
	
	$arrSub = is_array($_POST['chk']) ? $_POST['chk'] : Array();
    
    gerarRelatorioEscolaItenPorQuantidade($arrSub, $_REQUEST['dopid']);
	
	$sqlValidado 		= "select dopusucpfvalidacaogestor, dopreformulacao  from par.documentopar  where dopid = {$_REQUEST['dopid']}";
	$result				= $db->pegalinha($sqlValidado);
	$cpfGestorValidador = $result['dopusucpfvalidacaogestor'];
	$reformulacao 		= $result['dopreformulacao'];
	
	if( (! $cpfGestorValidador) && ( ! $reformulacao) )
	{
		enviaEmailNovoTermo($_REQUEST['dopid']);
	}
	
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.close(); 
		</script>";
	exit();

}

if( $_REQUEST['dopid'] && !isset($_REQUEST['requisicao']) ){
	$sql = "SELECT dp.dopid, dp.prpid, dp.iueid, dp.dopstatus, dp.dopdiasvigencia, dp.dopdatainicio, dp.dopdatafim, 
	  			dp.mdoid, dp.dopdatainclusao, dp.usucpfinclusao, dp.dopdataalteracao, dp.usucpfalteracao, md.tpdcod, odp.omjid, dp.dopjustificativa,
	  			dp.dopnumportaria, dp.doppaginadou, dp.dopdatapublicacao, dpv.dpvcpf, dp.arqidtexto
			FROM par.documentopar dp 
        	LEFT JOIN par.modelosdocumentos 	md  ON md.mdoid = dp.mdoid
            LEFT JOIN par.objetodocumentopar 	odp ON odp.dopid = dp.dopid
			LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid AND dpvstatus = 'A'
			WHERE dp.dopid = ".$_REQUEST['dopid'];
	
	$arMinuta = $db->pegaLinha( $sql );
	$arMinuta = $arMinuta ? $arMinuta : array();
	
	extract($arMinuta);
}

$boObjeto = $omjid ? true : false;
$boDatas = ($dopdiasvigencia) ? true : false;
$boJustificativa = $dopjustificativa ? true : false;

$mdoid = $_POST['mdoid'] 	? $_POST['mdoid'] 		: $mdoid;
$dopid = $_REQUEST['dopid'] ? $_REQUEST['dopid'] 	: $dopid;
$tpdcod = $_POST['tpdcod'] 	? $_POST['tpdcod'] 		: $tpdcod;


if( !empty($mdoid) ){
	$iutid = $db->pegaUm("select iutid from par.modelosdocumentos where mdoid = $mdoid");
}

unset($_SESSION['par']['dopid']);

if($_POST['mdoid'] != "" ) {	
	
	$sql = "SELECT mdoconteudo FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_POST['mdoid'];
	$mdoconteudo = $db->pegaUm($sql);
	
	if(strpos($mdoconteudo, '#Justificativa#')) $boJustificativa = true;
	if(strpos($mdoconteudo, '#Objeto_Convenio#')) $boObjeto = true;
	if(strpos($mdoconteudo, '#Numero_Dias#') || strpos($mdoconteudo, '#Data_Fim_Vigencia') || strpos($mdoconteudo, '#Numero_Dias_Prorrogados#')
		|| strpos($mdoconteudo, '#Nova_Vigencia_Inicio#') || strpos($mdoconteudo, '#Nova_Vigencia_Fim#') || strpos($mdoconteudo, '#Data_Celebracao#')) $boDatas = true;

	if($mdoconteudo && $_POST['requisicao'] == 'geradocumento'){
		
                $_SESSION['par_minuta']['iueid'] = $_POST['iueid'];
		if( !empty($_POST['chk'][0]) || ($_POST['tpdcod'] <> '102' && $_POST['tpdcod'] <> '21') ){
			if( !$_POST['reprogramacao'] ){ 
				$dopid = salvarDadosMinuta($_POST);
			}
			if( $dopid ){
				
 				/* Modificado dia 12/05/2014
				 * A pedido de: Thiago
				* Programador: Eduardo Dunice Neto
				* */
				$_POST['dopdatainiciovigencia'] = pegaDataInicioDocumentoPar( $dopid );
				$_SESSION['par']['cronogramainicial'] = $_POST['dopdatainiciovigencia'];
				
				$_POST['dopdatafimvigencia'] = pegaDataFimDocumentoPar( $dopid );
				$_SESSION['par']['cronogramaFinal'] = $_POST['dopdatafimvigencia'];
				
				if( $_POST['reprogramacao'] == 'P' ){
					
					$sql = "SELECT to_char(dprdataprazoaprovado, 'MM/YYYY') as data FROM par.documentoparreprogramacao WHERE dopid = $dopid AND dprstatus = 'P'";
					
					$_POST['dopdatafimvigencia'] = $db->pegaUm($sql);
					$_SESSION['par']['cronogramaFinal'] = $_POST['dopdatafimvigencia'];
				}
				/* FIM - Modificado dia 12/05/2014
				 * A pedido de: Thiago
				* Programador: Eduardo Dunice Neto
				* */
				
				if( $_POST['mes_fim'] && $_POST['ano_fim'] ){
					$_POST['dopdatafimvigencia'] = $_POST['mes_fim'] . "/" . $_POST['ano_fim'];
					$_SESSION['par']['cronogramaFinal'] = $_POST['mes_fim'] . "/" . $_POST['ano_fim'];
				}

				$_POST['dopid'] = $dopid;
				$_SESSION['par']['dopid'] = $dopid;
// 				ver($_POST, d);
				$doptexto = alteraMacrosMinuta($mdoconteudo, $prpid, $_POST);

				if($_SESSION['par']['totalVLR']){
					$_POST['dopvalor'] = $_SESSION['par']['totalVLR'];
					unset($_SESSION['par']['totalVLR']);
				}
				
				/* Modificado dia 12/05/2014
				 * A pedido de: Thiago
				* Programador: Eduardo Dunice Neto
				* */
				if( !$_POST['reprogramacao'] ){
					salvarMinuta($_POST, $doptexto, $dopid);
				}
				if( $_POST['reprogramacao'] ){
					enviaEmailNovoTermo($dopid, 'reformulacao');
				}
				/* FIM - Modificado dia 12/05/2014
				 * A pedido de: Thiago
				* Programador: Eduardo Dunice Neto
				* */
			} else {
				echo "<script>
					alert('Falha na operação!');
				</script>";
			}
		} else {
			echo "<script>
					alert('Selecione as subações para adicionar ao documento!');
					window.location.href = window.location; 
				</script>";
			exit();
		}
	}
}

if( $mdoconteudo && $_POST['requisicao'] == 'regerarDocumento' ){
	
	global $db;
	
	ini_set("memory_limit","2500M");
	set_time_limit(0);
        
	$sql = "SELECT
				iueid, prpid, dopstatus, dopdiasvigencia, to_char(dopdatainicio, 'DD/MM/YYYY HH24:MI:SS') as dopdatainicio, to_char(dopdatafim, 'DD/MM/YYYY HH24:MI:SS') as dopdatafim,
				dp.mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
				dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
				dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
				dopreformulacao, dopidpai, tpdcod, dopdatainiciovigencia, '".$_REQUEST['dopdatafimvigencia']."' as dopdatafimvigencia
			FROM
				par.documentopar dp
				INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
			WHERE dp.dopid = {$_REQUEST['dopid']}";

	$arrDadosDoc = $db->pegaLinha( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
	
	
// ver($arrDadosDoc);
	$sql = "SELECT  
				s.sbadsc as finalidade,
				to_char(now(), 'MM/YYYY') AS cronogramainicial,
				'".$_REQUEST['dopdatafimvigencia']."' AS cronogramafinal
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A' and empstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = ".$arrDadosDoc['prpid'];

	$arCronograma = $db->pegaLinha( $sql );
	
	/* Modificado dia 12/05/2014
	 * A pedido de: Thiago
	* Programador: Eduardo Dunice Neto
	* */
	$arrDadosDoc['dopdatainiciovigencia'] = pegaDataInicioDocumentoPar( $_REQUEST['dopid'] );
	
	
	/* Modificado dia 13/01/2014
	 * A pedido de: Isabel
	* Programador: Eduardo Dunice Neto
	* */
	$tipoDocumento = $_POST['mdoid'] ? $_POST['mdoid'] : $arrDadosDoc['modelo'];

	$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$tipoDocumento;
	$arrModelo = $db->pegaLinha($sql);

	$mdoconteudo = $arrModelo['mdoconteudo'];
	$tpdcod = $arrModelo['tpdcod'];

	$sql = "SELECT sbdid as chk FROM par.termocomposicao WHERE dopid = ".$_REQUEST['dopid'];
	$subacoes = $db->carregarColuna($sql);
	
        $_SESSION['par_minuta']['iueid'] = $_POST['iueid'];
	$postInterno = array('mdoid' => $arrDadosDoc['modelo'], 'tpdcod' => $tpdcod, 'dopid' => $_REQUEST['dopid'], 'chk' => $_POST['chk']);
        $post = array_merge($postInterno, $_POST);

	$_SESSION['par']['cronogramainicial'] 	= $arrDadosDoc['dopdatainiciovigencia'];
	$_SESSION['par']['cronogramaFinal'] 	= $arrDadosDoc['dopdatafimvigencia'];
	
	if( $_POST['mes_fim'] && $_POST['ano_fim'] ){
		$_SESSION['par']['cronogramaFinal'] = $_POST['mes_fim'] . "/" . $_POST['ano_fim'];
	}
	
	if( $_POST['tipoRepro'] == 2 ){ // Prazo
		$sql = "SELECT TO_CHAR( dprdataprazoaprovado, 'mm' ) || '/' || TO_CHAR( dprdataprazoaprovado, 'YYYY' ) as crono FROM par.documentoparreprogramacao WHERE dopid = ".$_REQUEST['dopid']." AND dprstatus = 'P' AND dprvalidacao = 't'";
		$crono = $db->pegaUm($sql);
		if( $crono ){
			$_SESSION['par']['cronogramaFinal'] = $crono;
		} 
	}	
	$doptexto = alteraMacrosMinuta($mdoconteudo, $arrDadosDoc['prpid'], $post);

	$arrDadosDoc['mdoid'] = $tipoDocumento;
	
	if($post['chk'] && $post['reprogramacao'] == 'S')
	{
		$sqlValor = "select sum(sbdvaloraprovado) from par.subacaodetalhe where sbdid in (".implode($post['chk'], ', ')." )" ;
		$dopValor = $db->pegaUm($sqlValor);

		if($dopValor){
			$arrDadosDoc['dopvalor'] = $dopValor;

		}
	}
	
	$dopidNovo = salvarDadosMinuta($arrDadosDoc, $doptexto, $_REQUEST['dopid']);
	
	if( $_POST['tipoRepro'] == 1 ){
		$sql = "UPDATE par.reprogramacao SET dopidreprogramado = ".$dopidNovo." WHERE dopidoriginal = ".$_REQUEST['dopid']." AND repstatus = 'A'";
		$db->executar($sql);
		$db->commit();
	} else {
		$sql = "UPDATE par.documentoparreprogramacao SET dopidreprogramado = ".$dopidNovo." WHERE dopid = ".$_REQUEST['dopid']." AND dprstatus = 'P' AND dprvalidacao = 't'";
		$db->executar($sql);
		$db->commit();
	}
	
	
	if( $_REQUEST['origem'] == 2 ){
		echo "
			<script>
				window.location.href = 'par.php?modulo=principal/minuta&acao=A&dopid=".$dopidNovo."&prpid=".$arrDadosDoc['prpid']."';
			</script>";
	} else {
		echo "
			<script>
				window.opener.location.href = 'par.php?modulo=principal/documentoPar&acao=A&popup=true';
				window.close();
			</script>";
	}
}

/* Verifica se, em caso de reformulação, está aguardando geração de termo Demanda PAR 348 */
if( $_REQUEST['dopid'] != '' ){
	
	$sql = "SELECT DISTINCT
				TRUE
			FROM 
				par.processopar prp
			INNER JOIN par.vm_documentopar_ativos 			dop ON dop.prpid = prp.prpid
			INNER JOIN par.modelosdocumentos  				mdo ON mdo.mdoid = dop.mdoid
			INNER JOIN par.documentoparreprogramacaosubacao	dpr ON dpr.dopid = dop.dopid
			WHERE
				dopreformulacao IS TRUE 
				AND dpr.dpsstatus = 'A'
				AND prp.prpid = {$_REQUEST['prpid']}
				AND dop.dopid = {$_REQUEST['dopid']}";
	
	$emReformulacao = $db->pegaUm($sql);
	
	$sql = "SELECT TRUE FROM par.documentoparreprogramacao WHERE dopid = ".$_REQUEST['dopid']." AND dprstatus = 'P' AND dprdataprazoaprovado IS NULL";
	
	$emReformulacaoPrazo = $db->pegaUm($sql);
}

if( $emReformulacao == 't' || $emReformulacaoPrazo == 't' ){
	
	?>
	<div style="position:absolute;width:95%;">
		<div style="position:relative;float:right;">
			<img src="../imagens/par/carimbo-reformulacao.png" border="0">
		</div>
	</div>
	<?php 
	$sql = "SELECT DISTINCT
				TRUE
			FROM 
				par.processopar prp
			INNER JOIN par.vm_documentopar_ativos 			dop ON dop.prpid = prp.prpid
			INNER JOIN par.modelosdocumentos  				mdo ON mdo.mdoid = dop.mdoid
			INNER JOIN par.documentoparreprogramacaosubacao	dpr ON dpr.dopid = dop.dopid
			WHERE
				dopreformulacao IS TRUE 
				AND (
					SELECT count(sd2.sbdid) 
					FROM par.processoparcomposicao ppc 
					LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20,21) 
					WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid
				) = 0 
				AND dpr.dpsstatus = 'A'
				AND prp.prpid = {$_REQUEST['prpid']}
				AND dop.dopid = {$_REQUEST['dopid']}";
	
	$aguardandoGeracaoTermo = $db->pegaUm($sql);
	
	$sql = "SELECT TRUE FROM par.documentoparreprogramacao WHERE dopid = ".$_REQUEST['dopid']." AND dprstatus = 'P' AND dprdataprazoaprovado IS NOT NULL";
	
	$aguardandoGeracaoTermoPrazo = $db->pegaUm($sql);
	
	if( $aguardandoGeracaoTermo != 't' || $aguardandoGeracaoTermoPrazo != 't' ){
		$disabled = 'N';
		$readonly = 'readonly="readonly"';
	}
}

$perfil = pegaPerfilGeral();
//regras de acesso passada por Thiago em 24/05/2012
/* Regra atualizada dia 29/05/2014
 * Analista: Thiago
 * Programador: Eduardo
 * Documento validado deve apenas ser vizualizado.
 * */
if( ( 
		in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
		in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) 
	)/* && 
	(
		$dpvcpf != '' && 
		($aguardandoGeracaoTermo == 't' || $aguardandoGeracaoTermoPrazo == 't') 
	)*/
){
	$disabled = 'S';
	$readonly 	= '';
}else{
	$disabled = 'N';
	$readonly = 'readonly="readonly"';
}
if( in_array(PAR_PERFIL_ADMINISTRADOR_REPROGRAMAÇÃO, $perfil) && 
	( $_REQUEST['geraTermoReprogramacaoPrazo'] == 1 || 
	$_REQUEST['geraTermoReprogramacao'] == 1 ) 
	){
	$disabled = 'S';
	$readonly 	= '';
} 

//if(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
//    in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
//    in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil)
//){
//	$disabled = 'S';
//	$readonly = '';
//}

/* FRIM - Verifica se, em caso de reformulação, está aguardando geração de termo*/

if( $_REQUEST['dopid'] ){
	$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );
}

$doptexto = $doptexto ? $doptexto : "";

$doptexto = str_replace('"', "'", $doptexto);

monta_titulo( $titulo_modulo, '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

$sql = "SELECT 
			p.prpnumeroprocesso,
			iu.muncod, 
		    m.mundescricao||' / '|| m.estuf as municipio,
		    e.estdescricao||' / '||e.estuf as estado
		FROM par.processopar p
		    inner join par.instrumentounidade iu on iu.inuid = p.inuid
		    inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		    left join territorios.municipio m on m.muncod = iu.muncod
		    left join territorios.estado e on e.estuf = iu.estuf
		WHERE
			p.prpid = ".$prpid;

$arTerritorio = $db->pegaLinha($sql);

if( $prpid ){
	$processo = $db->pegaUm("select prpnumeroprocesso from par.processopar where prpid = $prpid");
	verificaEmpenhoSigef( $processo );
}
 
?>
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

#listaRestricoes thead tr td
	{
		background-color: #F7F7F7;
		font-weight: bold;
		font-size: 12px;
		font-family: arial;
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
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../library/jquery/jquery-1.10.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<!-- <script type="text/javascript" src="/includes/prototype.js"></script> -->

<script type="text/javascript">
tinyMCE.init({
	// General options
	mode : "textareas",
	theme : "advanced",
	language: "pt",
	editor_selector : "minutatinymce",
	plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

	// Theme options
	theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
	theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
	theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
	theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	theme_advanced_statusbar_location : "",
	theme_advanced_resizing : true,

	// Example content CSS (should be your site CSS)
	content_css : "css/content.css",

	// Drop lists for link/image/media/template dialogs
	template_external_list_url : "lists/template_list.js",
	external_link_list_url : "lists/link_list.js",
	external_image_list_url : "lists/image_list.js",
	media_external_list_url : "lists/media_list.js",

	// Replace values for the template plugin
	template_replace_values : {
		username : "Some User",
		staffid : "991234"
	}
});

function listarSubacao(sbaid, ano, inuid){
	var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
	janela(local,800,600,"Subação");
}

function fechaMinuta(){
	window.opener.location.href = "par.php?modulo=principal/documentoPar&acao=A&popup=true";
	window.close()
}

function mostraCampos( tpdcod, mdoid, dopid ){
	if( tpdcod ){
		$('#modelotermo').show();
	} else {
		$('#modelotermo').hide();
	}
	
	if( mdoid ){
		$('#documento').show();
		$('#botao').show();
		$('#botaotexto').show();
		if( tpdcod == '100' && dopid != '' ) $('#dou').show();
	} else {
		$('#documento').hide();
		$('#botao').hide();
		$('#botaotexto').hide();
		if( tpdcod == '100' ) $('dou').hide();
	}
}

function carregaTipoDoc(){
	
	$("#mdoid").val("");
	$("#requisicao").val("carregadoc");
	$('#formulario').submit();
}

function carregarDados(){
	$('#formulario').submit();
}

function calculaDataInicioVigencia(){
	var datainicio 	= $('#dopdatainicio').val();
	var datafim 	= $('#dopdatafim').val();
	var diavigencia = $('#dopdiasvigencia').val();
		 
	if( datafim != '' ){
		$.ajax({
		   	type: "POST",
		   	url: window.location,
		   	data: '&pegaDataFinal=true&dopdatafim='+datafim+'&dopdatainicio='+datainicio,
		   	success: function(msg){
			   $('#dopdiasvigencia').val(msg);
		   	}
		 });
	} else if( diavigencia != '' ){
		$.ajax({
		   	type: "POST",
		   	url: window.location,
		   	data: '&pegaDataFinal=true&dopdiasvigencia='+diavigencia+'&dopdatainicio='+datainicio,
		   	success: function(msg){
			   $('#dopdatafim').val(msg);
		   	}
		 });
	}	
}

function calculaDiasVigencia(){
	var datainicio 	= $('#dopdatainicio').val();
	var diavigencia = $('#dopdiasvigencia').val();	 
	
	if( diavigencia != '' && datainicio != '' ){
		$.ajax({
		   	type: "POST",
		   	url: window.location,
		   	data: '&pegaDataFinal=true&dopdiasvigencia='+diavigencia+'&dopdatainicio='+datainicio,
		   	success: function(msg){
			   $('#dopdatafim').val(msg);
		   	}
		 });
	}	
}

function calculaDataFimVigencia(){
	var datainicio 	= $('#dopdatainicio').val();
	var datafim 	= $('#dopdatafim').val();	 
	
	if( datainicio != '' && datafim != '' ){
		$.ajax({
		   	type: "POST",
		   	url: window.location,
		   	data: '&pegaDataFinal=true&dopdatafim='+datafim+'&dopdatainicio='+datainicio,
		   	success: function(msg){
			   $('#dopdiasvigencia').val(msg);
		   	}
		 });
	}	
}

function validaFormulario(){
	
	var checkVazio = true;

	if( $('[name="dopdatainicio"]').length > 0 && $('#tpdcod').val() == 100 ){ //se tipo de documento for DOU
		if( $('#dopdatainicio').val() == "" ){
			alert( 'O campo Data Celebração é de preenchimento obrigatório!' );
			$('#dopdatainicio').focus();
			return false;
		}
		if(!validaData($('#dopdatainicio'))) {
			alert('Data Celebração incorreta!');
			return false;
		}
	}else if( $('[name="dopdiasvigencia"]').length > 0 ){
		if( $('#dopdiasvigencia').val() == "" ){
			alert( 'O campo Dias de Vigência é de preenchimento obrigatório!' );
			$('#dopdiasvigencia').focus();
			return false;
		}
	}else if( $('[name="dopdatafim"]').length > 0 && $('#tpdcod').val() == 100 ){ //se tipo de documento for DOU
		if( $('#dopdatafim').val() == "" ){
			alert( 'O campo Data Final da Vigência é de preenchimento obrigatório!' );
			$('#dopdatafim').focus();
			return false;
		}
		if(!validaData($('#dopdatafim'))) {
			alert('Data Final da Vigência incorreta!');
			return false;
		}
	}
	if( $('[name="dopjustificativa"]').length > 0 ){
		if( $('#dopjustificativa').val() == "" ){
			alert( 'O campo Justificativa é de preenchimento obrigatório!' );
			$('#Justificativa').focus();
			return false;
		}
	}
	selectAllOptions( document.getElementById('objid') );
	if( $('[name="objid[]"]').length >0 ){
		if( $('[name="objid[]"][value!=""]').length < 1 ){
			alert('Objeto é de preenchimento obrigatório');
			return false;
		}
	}
	if( $('[name="chk[]"]:checked').length > 0 ){
		checkVazio = false;
	}
	if( checkVazio && ($('#tpdcod').val() == '102' || $('#tpdcod').val() == '21') ){
		alert('Selecione as subações para adicionar ao documento');
		return false;
	}
	return true;
}

function salvarMinutaTermoAditivo() {

	$('#bt_salvar').attr('disabled', true);
	
    if( $('#tpdcod').val() == '' ){
            alert('Campo obrigatório.');
            $('#tpdcod').focus();
            $('#bt_salvar').attr('disabled', false);
            return false;
    }

    if( $('#mdoid').val() == '' ){
            alert('Campo obrigatório.');
            $('#mdoid').focus();
            $('#bt_salvar').attr('disabled', false);
            return false;
    }
    if( $('#iutid').val() != '' && $('#iueid').val() == '' ){
            alert('Campo obrigatório.');
            $('#iueid').focus();
            $('#bt_salvar').attr('disabled', false);
            return false;
    }
	var texto = tinyMCE.get('texto').getContent();
	
	if( texto != '' ){
		if( validaFormulario() ){
			$('#requisicao').val("salvar");
			$('#formulario').submit();
		}
	} else {
		alert('É necessário carregar o documento antes de salvar.');
		$('#bt_carregar').attr('disabled', false);
		return false;
	}
	$('#bt_salvar').attr('disabled', false);
}

function finalizarReprogramacaoPrazo() {
	$('#bt_carregar').attr('disabled', true);
	if( confirm('Deseja finalizar a Reprogramação de Prazo?') ){
		if( confirm('Você carregou o termo e ele está de acordo?') ){
			var texto = tinyMCE.get('texto').getContent();
			
			if( texto != '' ){
				if( validaFormulario() ){
					$('#requisicao').val("finalizarReprogramacaoPrazo");
					$('#formulario').submit();
				}
			} else {
				alert('É necessário carregar o documento antes de salvar.');
				$('#bt_carregar').attr('disabled', false);
				return false;
			}
		}
	}
	$('#bt_carregar').attr('disabled', false);
}

function regeraTexto(tipo){
	$('#bt_gerar').attr('disabled', true);
	if( validaFormulario() ){
		
		if( $('#tpdcod').val() == '' ){
			alert('Campo obrigatório.');
			$('#tpdcod').focus();
			$('#bt_gerar').attr('disabled', false);
			return false;
		}

		if( $('#mdoid').val() == '' ){
			alert('Campo obrigatório.');
			$('#mdoid').focus();
			$('#bt_gerar').attr('disabled', false);
			return false;
		}
	
		selectAllOptions( document.getElementById( 'objid' ) );
		$('#tipoRepro').val(tipo);
		$('#requisicao').val("regerarDocumento");
		$('#formulario').submit();
	}
	$('#bt_gerar').attr('disabled', false);
}

function carregaTexto( mdoid ) {

	$('#bt_carregar').attr('disabled', true);
	if( validaFormulario() ){
		
		if( $('#tpdcod').val() == '' ){
			alert('Campo obrigatório.');
			$('#tpdcod').focus();
			$('#bt_carregar').attr('disabled', true);
			return false;
		}

		if( $('#mdoid').val() == '' ){
			alert('Campo obrigatório.');
			$('#mdoid').focus();
			$('#bt_carregar').attr('disabled', true);
			return false;
		}
		if( $('#iutid').val() != '' && $('#iueid').val() == '' ){
			alert('Campo obrigatório.');
			$('#iueid').focus();
			$('#bt_carregar').attr('disabled', true);
			return false;
		}
		divCarregando();
		selectAllOptions( document.getElementById( 'objid' ) );
		$('#requisicao').val("geradocumento");
		$('#formulario').submit();
	}
	$('#bt_carregar').attr('disabled', false);
}

function imprimirMinutaDOU(dopid) {
	var janela = window.open( '/par/par.php?modulo=principal/popImprimeMinutaDOU&acao=A&dopid='+dopid+'', 'imprimeminuta', 'width=450,height=400,status=0,menubar=1,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}

function enviarEmailPublicacao(dopid){
	if( confirm('Deseja enviar e-mail para o responsável da publicação?') ){
		$('#requisicao').val("email");
		$('#formulario').submit();
	}
}

function minutaConvenioWord(dopid){
	if(dopid){
		window.location.href='par.php?modulo=principal/minuta&acao=A&dopid='+dopid+'&imprimeWord=1';
	}
}

$(document).ready(function(){

	$('#texto').val($('#doptexto').val());
	
	mostraCampos('<?=$tpdcod; ?>', '<?=$mdoid; ?>', '<?=$dopid ?>');
});
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" name="requisicao" 		id="requisicao" 	value="" />
	<input type="hidden" name="tipoRepro" 		id="tipoRepro" 	    value="" />
	<input type="hidden" name="reprogramacao" 	id="reprogramacao"	value="<?=$_REQUEST['geraTermoReprogramacaoPrazo'] ? 'P' : ($_REQUEST['geraTermoReprogramacao'] ? 'S' : ''); ?>" />
	<input type="hidden" name="prpid" 			id="prpid" 			value="<?=$prpid; ?>" />
	<input type="hidden" name="dopid" 			id="dopid" 			value="<?=$dopid; ?>" />
	<input type="hidden" name="modelo" 			id="modelo" 		value="<?=$mdoid; ?>" />
	<input type="hidden" name="doptexto"		id="doptexto" 		value="<?=$doptexto ?>" />
	<input type="hidden" name="iutid" 			id="iutid" 			value="<?=$iutid; ?>" />
	
<table class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="subtitulodireita" style="width: 50%;"><b>Processo:</b></td>
		<td><?php echo formatarProcesso($arTerritorio['prpnumeroprocesso']); ?></td>
	</tr>
	<?php
	if( !empty($arTerritorio['muncod']) ){ 
	?>
	<tr>
		<td class="subtitulodireita"><b>Município:</b></td>
		<td><?php echo $arTerritorio['municipio']; ?></td>
	</tr>
	<?} else {?>
	<tr>
		<td class="subtitulodireita"><b>Estado:</b></td>
		<td><?php echo $arTerritorio['estado']; ?></td>
	</tr>
	<?}?>
</table>
<table class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
<?php 
				
$sql = "SELECT 
			iu.inuid AS inuid
		FROM
			par.processopar p 
		INNER JOIN par.instrumentounidade iu ON iu.muncod = p.muncod
		WHERE p.prpid  = $prpid";

$arrDados = $db->pegaLinha( $sql );
							
$inuId = $arrDados['inuid'];
if($inuId)
{
	
	$today = date('Y-m-d');
	$sqlRestricoes = 
		"SELECT 
			res.resdescricao,
			tpr.tprdescricao as tipo_restricao,
			CASE WHEN res.restemporestricao THEN
				to_char(resdatainicio, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_inicio,
			
			CASE WHEN res.restemporestricao THEN
				to_char(resdatafim, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_fim
		FROM
			par.restricaofnde res
		INNER JOIN
			par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
		INNER JOIN
			par.instrumentounidaderestricaofnde iur ON res.resid = iur.resid
		WHERE
			res.resstatus = 'A'
		AND
			iur.inuid = {$inuId}
		AND
			CASE WHEN res.restemporestricao THEN
				CASE
					WHEN '{$today}' between resdatainicio and resdatafim THEN
						true
					ELSE
						false
					END
			ELSE
				true
			END
		";
	
	
	$arrRestricoes = $db->carregar($sqlRestricoes);
	if( count($arrRestricoes) && is_array($arrRestricoes) )
	{
?>
<tr>
		<td>
			<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 100%;" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td  colspan="4"> Restrições do município </td>
				</tr>

			</thead>
			<tbody>
			
				<?php
				
					foreach($arrRestricoes as $kRes => $vRes )
					{
						echo "
							<tr style=\"color:red;\">
								<td style=\"width:40%;\" > {$vRes['resdescricao']} </td>
							</tr>
							";	
					}
				?>
			</tbody>
			</table>
			</td>
	
	</tr>
	
<?php
	}
} ?>
			
		<td>
		<table class="tabela" align="center" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none; width: 100%">
			<tr>
				<td class="SubTituloDireita" style="text-align: left;" colspan="3" valign="middle"><b>Documentos</b></td>
			</tr>
			<tr>
				<td class="subtitulodireita" style="width: 19%;"><b>Tipo do Documento:</b></td>
				<td>
					<?php
                        /*
                         * Regra-Se for "inserção" Cadastro de um novo termo aditivo, o formulario não permite mostrar todas
                         * as opções para tipo do documento se o processo já possuir termo de compromisso
                         */
                        $sqlAndWhere = '';
                        if(empty($dopid) || !in_array($tpdcod, array(102,103))){
                            $sqlAndWhere = "
								AND CASE WHEN (
                                    SELECT DISTINCT
                                        COUNT(mo.tpdcod)
                                    FROM
                                        par.documentopar mi
                                        LEFT JOIN par.modelosdocumentos mo  ON mo.mdoid = mi.mdoid
                                    WHERE
                                        mi.prpid = '$prpid'
                                        AND mo.tpdcod IN(102, 103)
                                        AND mi.dopstatus IN ('A')
                                ) > 0 THEN
                                	td.tpdcod NOT IN(102, 103)
                                ELSE
                                	TRUE
                                END";
                        }
                        
						$sql = "
                            SELECT
                                td.tpdcod as codigo,
                                td.tpddsc as descricao
                            FROM
                                public.tipodocumento td
                            WHERE
                                td.tpdstatus = 'A'
                                $sqlAndWhere
                            ORDER BY
                                descricao";
						$db->monta_combo("tpdcod", $sql, $disabled, "Selecione...", 'carregaTipoDoc', '', '', '600', 'N', 'tpdcod');
					?>
				</td>
			</tr>
		    <tr id="modelotermo" style="display: none">
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>Modelo de documento:</b>
		        </td> 
		        <td>
				    <?php 
			    	$sql = "select 
			    				mod.mdoid as codigo, 
			    				mod.mdonome as descricao
								from par.modelosdocumentos mod
					     		where mod.mdostatus = 'A' AND mod.mdovisivel <> 'N'
					     		and mod.tpdcod = {$tpdcod}
			    				and mod.mdonome not ilike '%obra%' order by mod.mdonome asc";
			    	if( $tpdcod )
				    	$db->monta_combo("mdoid", $sql, $disabled, 'Selecione...', 'carregarDados', '', '', '600', '', 'mdoid', '', '', 'Lista de modelo(s) vinculado(s) a termo aditivo' );
			        ?>
		        </td>    
		    </tr>
		    <?php
		    if( !empty($iutid) ){
		    ?>
		    <tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>Assinatura de outra Entidade:</b>
		        </td> 
		        <td>
				    <?php 
			    	$sql = "SELECT iue.iueid as codigo, iue.iuenome as descricao
							FROM par.instrumentounidade iu
								INNER JOIN par.processopar pro ON pro.inuid = iu.inuid AND pro.prpstatus = 'A'
								INNER JOIN par.instrumentounidadeentidade iue ON iu.inuid = iue.inuid AND iue.iuestatus = 'A'
								INNER JOIN par.instrumentounidadeentidadetipo iut ON iut.iutid = iue.iutid
							WHERE
								iut.iutid = $iutid
								AND pro.prpid = $prpid";
				    $db->monta_combo("iueid", $sql, $disabled, 'Selecione...', '', '', '', '600', '', 'iueid', '', $_SESSION['par_minuta']['iueid'], 'Lista de Entidades' );
			        ?>
		        </td>    
		    </tr>
		    <?php
		    } else {
		    	echo '<input type="hidden" name="iueid" id="iueid" value="" />';
		    }
		    if( $boDatas || $boObjeto ){ ?>
		    <tr>
		    	<td class="SubTituloDireita" style="text-align: center;" colspan="2" valign="middle"><b>Dados Adicionais</b></td>
		    </tr>
		    <?php } if( $boDatas ){ ?>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Data Celebração:</b></td>
				<td >
				<?php	
					$dopdatainicio = ($dopdatainicio ? formata_data( $dopdatainicio ) : "");
					echo campo_data2('dopdatainicio', 'N', $disabled,'Data Inicio da Vigência', '', '', '', $dopdatainicio, 'calculaDataInicioVigencia();', '', 'dopdatainicio');
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência:</b></td>
				<td>
				<?php
					echo campo_texto('dopdiasvigencia', 'S', $disabled, '', 5, 10, '[#]', '', '', '', 0, 'id="dopdiasvigencia"','', $dopdiasvigencia, 'calculaDiasVigencia();' );
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Data Final da Vigência:</b></td>
				<td>
				<?php
					$dopdatafim = ($dopdatafim ? formata_data($dopdatafim) : "");
					echo campo_data2('dopdatafim', 'N', $disabled,'Data Final da Vigência', '', '', '', $dopdatafim, 'calculaDataFimVigencia();', '', 'dopdatafim');
				?>
				</td>
			</tr>
			<?php }
			if( $boObjeto ){ ?>
		    <tr>
				<td class="SubTituloDireita" valign="middle"><b>Objeto:</b></td>
				<td >
					<?php
					$sql = "SELECT oc.objid as codigo, oc.objdsc as descricao
							FROM 
								par.objeto oc
							    inner join par.objetodocumentopar omc on omc.objid = oc.objid
							WHERE
								omc.dopid = $dopid
							    and oc.objstatus = 'A'
							    --and oc.objtpobj = 'A'";
					
					if(  !empty($dopid) ) $objid = $db->carregar( $sql );
					
					$sql = "SELECT oc.objid as codigo, oc.objdsc as descricao FROM par.objeto oc
							WHERE  objstatus = 'A'
							--and oc.objtpobj = 'A'
							ORDER BY oc.objdsc";
								
					combo_popup( 'objid', $sql, '', '400x400', 0, array(), '', $disabled, false, false, 7, 400 );
					echo obrigatorio(); 
					?>
				</td>
			</tr>
			<?php } 
			if($boJustificativa){ ?>
		    <tr>
				<td class="SubTituloDireita" valign="middle"><b>Justificativa:</b></td>
				<td><?=campo_textarea('dopjustificativa', 'S', $disabled, 'Justificativa', 80, 5, 4000, '', '', '', '', '');?></td>
			</tr>
			<tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>ptres:</b>
		        </td> 
		        <td>
			    <?php 
			    	$db->monta_combo("mdoid1", array(), $disabled, 'Selecione...', '', '', '', '', '', 'mdoid1', '', '', '' );
		        ?>
		        </td>    
		    </tr>
			<tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>PI:</b>
		        </td> 
		        <td>
			    <?php 
			    	$db->monta_combo("mdoid2", array(), $disabled, 'Selecione...', '', '', '', '', '', 'mdoid2', '', '', '' );
		        ?>
		        </td>    
		    </tr>
			<tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>Natureza de Despesa:</b>
		        </td> 
		        <td>
			    <?php 
			    	$db->monta_combo("mdoid3", array(), $disabled, 'Selecione...', '', '', '', '', '', 'mdoid3', '', '', '' );
		        ?>
		        </td>    
		    </tr>
			<?} ?>
		</table>
	<?php
	if( $_REQUEST['geraTermoReprogramacao'] || $testaReprogramacao ){ 
		
		$sql = "SELECT * FROM par.reprogramacao WHERE dopidoriginal = ".$_REQUEST['dopid']." AND dopidreprogramado IS NULL AND repstatus = 'A'";
		$dados1 = $db->pegaLinha($sql);
		
		if( $dados1['dopidreprogramado'] ){
			echo "
			<script>
				window.location.href = 'par.php?modulo=principal/minuta&acao=A&dopid=".$dados1['dopidreprogramado']."&prpid=".$_REQUEST['prpid']."';
			</script>";
		}
		
		$sql = "SELECT DISTINCT
					(
						CASE WHEN sbdrepassevlrempenhoaprovado > 0 
						THEN 	(
							SELECT DISTINCT
								(
									(par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano)-(sbdrepassevlrcomplementaraprovado+sbdrepassevlrrafaprovado))
									-
									(
									CASE WHEN emr.vrlreforco IS NOT NULL THEN
										sum(ems.eobvalorempenho+emr.vrlreforco)
									ELSE
										sum(ems.eobvalorempenho)
									END
									)
								) <= 0
							FROM
								par.empenhosubacao ems
							INNER join par.empenho emp on emp.empid = ems.empid and empstatus = 'A' and eobstatus = 'A' 
							inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
							left  join par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
							left  join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid and sbdano = sd.sbdano
							left  join (
								select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
								from par.empenho ep
									inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A' and eobstatus = 'A'
									inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
								where ep.empcodigoespecie in ('02')
								group by 
									ep.empnumeroprocesso,
									ep.empcodigoespecie,
									ep.empidpai,
									sd1.sbdid
							) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
							where
								ems.sbaid = s.sbaid  AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
							GROUP BY
								sbd.sbaid, sbd.sbdano, sbd.sbdrepassevlrcomplementaraprovado, sbd.sbdrepassevlrrafaprovado,emr.vrlreforco
							)
						ELSE TRUE 
						END
					)
				FROM 
					par.documentopar dop
				INNER JOIN par.processoparcomposicao 			ppc ON ppc.prpid = dop.prpid and ppc.ppcstatus = 'A'
				INNER JOIN par.subacaodetalhe 					sd  ON sd.sbdid = ppc.sbdid
				INNER JOIN par.subacao 							s   ON s.sbaid = sd.sbaid
				WHERE
					dopid = {$_REQUEST['dopid']}";
		
		$booSubEmpenhadas = $db->carregarColuna($sql);
		
		$carregarTermo = false;
		$gerarTermo = false;
		if( $dados1['repid'] && !in_array('f',$booSubEmpenhadas) ){ // Ainda é o termo original, então ele vai gerar um novo termo.
			$gerarTermo = true;
			$readonly = 'readonly="readonly"'; 
		}
		$booSubAEmpenhar = FALSE;
		if(in_array('f',$booSubEmpenhadas)){
			$booSubAEmpenhar = TRUE;
		}
		
// 		$sql = "SELECT * FROM par.reprogramacao WHERE dopidreprogramado = ".$_REQUEST['dopid']." AND repstatus = 'A'";
// 		$dados2 = $db->pegaLinha($sql);
		
// 		if( $dados2['repid'] ){ // Já é o novo termo, então só pode carregar em cima dele.
// 			$gerarTermo = false;
// 			$carregarTermo = true;
// 			$readonly = ''; 
// 		}
		?>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="border-bottom:none; width: 100%">
		<tr>
			<td class="SubTituloDireita" style="text-align: left;" colspan="3" valign="middle"><b>Dados da Reprogramação</b></td>
		</tr>
			<tr>
				<td class="SubTituloDireita">Data final do Termo:</td>
				<td>
					<?=campo_texto('mes_fim','S','S','',3,2,'##','', 'right', '',0, 'id="mes_fim" class="mes"','', '', '', '' )?> Mês /
					<?=campo_texto('ano_fim','S','S','',5,4,'####','', 'right', '',0, 'id="ano_fim" class="ano"','', '', '', '' )?> Ano
				</td>
			</tr>
		</table>
	<?php
	}
	
	$tipoRepro = 1; // Subação
	
	if( $testaReprogramacaoPrazo || ($_REQUEST['geraTermoReprogramacaoPrazo'] && !($emReformulacao == 't' || $emReformulacaoPrazo == 't')) ){ // Prazo

		$tipoRepro = 2; //Prazo
		
		$sql = "SELECT to_char(dprdataprazoaprovado, 'dd/mm/yyyy') as data FROM par.documentoparreprogramacao WHERE dopidreprogramado = ".$_REQUEST['dopid']." AND dprstatus = 'P'";
		$data = $db->pegaUm($sql);
		
		//********* 
		$sql = "SELECT * FROM par.documentoparreprogramacao WHERE dopid = ".$_REQUEST['dopid']." AND dprstatus = 'P' AND dprvalidacao = 't'";
		$dados3 = $db->pegaLinha($sql);
		
		if( $dados3['dopidreprogramado'] ){
			echo "
			<script>
				window.location.href = 'par.php?modulo=principal/minuta&acao=A&dopid=".$dados3['dopidreprogramado']."&geraTermoReprogramacaoPrazo=1&prpid=".$_REQUEST['prpid']."';
			</script>";
		}
		
		$carregarTermo = false;
		$gerarTermo = false;
		if( $dados3['dprid'] ){ // Ainda é o termo original, então ele vai gerar um novo termo.
			$gerarTermo = true;
			$readonly = 'readonly="readonly"'; 
		}
		
		$sql = "SELECT * FROM par.documentoparreprogramacao WHERE dopidreprogramado = ".$_REQUEST['dopid']." AND dprstatus = 'P'";
		$dados4 = $db->pegaLinha($sql);
		
		if( $dados4['dprid'] ){ // Já é o novo termo, então só pode carregar em cima dele.
			$gerarTermo = false;
			$carregarTermo = true;
		
		
		
		//********* 
	?>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="border-bottom:none; width: 100%">
		<tr>
			<td class="SubTituloDireita" style="text-align: left;" colspan="3" valign="middle"><b>Dados da Reprogramação</b></td>
		</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Data final do Termo:</td>
				<td width="25%">
					<?=$data ?> 
				</td>
				<td>
					<input type="button" id="bt_carregar" value="Finalizar Reprogramação de Prazo" onclick="finalizarReprogramacaoPrazo();" />
				</td>
			</tr>
		</table>
	<?php
		}
	}
	if( $_POST['tpdcod'] == '102' || $tpdcod == '102' || $_POST['tpdcod'] == '21' || $tpdcod == '21' ){ 
	?>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="border-bottom:none; width: 100%">
		<tr>
			<th>Adicionar Subações ao Termo</th>
		</tr>
		<tr>
			<td><?php
			// A pedido do Julio Viana no dia 28/12/2012 (reta final).
			if( $_REQUEST['prpid'] == 3389 ){
				$sql = "SELECT
							case when tc.sbdid is not null then
						    	'<center><a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sbdano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a><input type=checkbox name=chk[] id=chk_'||sd.sbdid||' checked value='||sd.sbdid||'></center>'
						    else
								'<center><a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sbdano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a><input type=checkbox name=chk[] id=chk_'||sd.sbdid||' value='||sd.sbdid||'></center>'
								
						    end as acoes,
						    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
						    sub.sbadsc as subacao,
						    prg.prgdsc as programa,
						    COALESCE(sum(ems.eobvalorempenho),0.00) as valorempenho,
						    sd.sbdano||'&nbsp;' as ano
						FROM
							par.processopar prp
						    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
						    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid 
						    left join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
						    left join par.empenho emp on emp.empid = ems.empid and empstatus = 'A'
						    inner join par.subacao sub on sub.sbaid = sd.sbaid   
						    inner join par.acao a on a.aciid = sub.aciid
						    inner join par.pontuacao pon on pon.ptoid = a.ptoid
						    inner join par.criterio c on c.crtid = pon.crtid
						    inner join par.indicador i on i.indid = c.indid
						    inner join par.area are on are.areid = i.areid
						    inner join par.dimensao d on d.dimid = are.dimid
						    inner join par.programa prg on prg.prgid = sub.prgid
						    left join par.termocomposicao tc on tc.sbdid = sd.sbdid
						WHERE
							prp.prpid = {$_REQUEST['prpid']}
						GROUP BY
							sub.sbadsc, d.dimcod, are.arecod, i.indcod, sub.sbaordem, prg.prgdsc, sd.sbdid, tc.sbdid, sub.sbaid, sd.sbdano, pon.inuid
						ORDER BY d.dimcod, are.arecod, i.indcod, sub.sbaordem";
			} else {
				
				$sql = "SELECT
							case when tc.sbdid is not null then
						    	'<center><a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sbdano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a><input type=checkbox name=chk[] id=chk_'||sd.sbdid||' checked value='||sd.sbdid||'></center>'
						    else
								'<center><a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sbdano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a><input type=checkbox name=chk[] id=chk_'||sd.sbdid||' value='||sd.sbdid||'></center>'
						    end as acoes,
						    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
						    sub.sbadsc as subacao,
						    prg.prgdsc as programa,
						    sum(ems.eobvalorempenho) as valorempenho,
						    sd.sbdano||'&nbsp;' as ano
						FROM
							par.processopar prp
						    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
						    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid 
						    inner join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano
						    inner join par.empenho emp on emp.empid = ems.empid and empstatus = 'A' and eobstatus = 'A'
						    inner join par.subacao sub on sub.sbaid = ems.sbaid
						    inner join par.acao a on a.aciid = sub.aciid
						    inner join par.pontuacao pon on pon.ptoid = a.ptoid
						    inner join par.criterio c on c.crtid = pon.crtid
						    inner join par.indicador i on i.indid = c.indid
						    inner join par.area are on are.areid = i.areid
						    inner join par.dimensao d on d.dimid = are.dimid
						    inner join par.programa prg on prg.prgid = sub.prgid
						    left join par.termocomposicao tc
								inner join par.vm_documentopar_ativos dop on dop.dopid = tc.dopid
							on tc.sbdid = sd.sbdid
						WHERE
							prp.prpid = {$_REQUEST['prpid']}
						    AND emp.empsituacao  != 'CANCELADO'
						    AND ems.eobstatus = 'A'
						GROUP BY
							sub.sbadsc, d.dimcod, are.arecod, i.indcod, sub.sbaordem, prg.prgdsc, sd.sbdid, tc.sbdid, sub.sbaid, sd.sbdano, pon.inuid
						ORDER BY d.dimcod, are.arecod, i.indcod, sub.sbaordem";
			}
//				ver($sql,0);
				$cabecalho = array('&nbsp;&nbsp;Ação&nbsp;&nbsp;', 'Localiza', 'Nome', 'Programa', 'Valor Empenhado', 'Ano');
				if( empty($_REQUEST['prpid']) ) $sql = array();
				$db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'S');
			?></td>
		</tr>
		<?php if($gerarTermo){?>
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align: center;">
						<input type="button" id="bt_gerar" value="Gerar Documento" onclick="regeraTexto(<?=$tipoRepro ?>);" />
				</td>
			</tr>
		<?php }?>
		<?php if($booSubAEmpenhar){?>
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align: center;">
						<label style="color:red"><b>Aguardando Empenho</b></label>
				</td>
			</tr>
		<?php }?>
		</table>
	<?php } ?>
      
        <?php 
        if (!empty($dopid)) {
            $hasProblema = verificaPendenciaSubacaoEscola($dopid);
            echo $hasProblema;
        }
        ?>
                    
                    
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none; width: 100%">
	<tr id="botaotexto" style="display: none;">
		<td align="center" bgcolor="#c0c0c0" colspan="3">
			<?php if($readonly == ''){?>
				<input type="button" id="bt_carregar" value="Carregar Documento" onclick="<?php echo ($hasProblema !='') ? '' : 'carregaTexto();'?>" />
			<?php }?> 
		</td>
	</tr>
	<tr id="documento" style="display: none;">
		<td colspan="3">
			<div>
				<textarea id="texto" name="texto" rows="48" cols="80" style="width:100%" class="minutatinymce"></textarea>
			</div>
		</td>
	</tr>
	<tr id="dou" style="display: none;">
		<td>
		<?php
			echo 'Data de publicação: ';
			echo campo_data2( 'dopdatapublicacao','S', 'S', 'Data do Publicação', 'S', '', '');	
		?>
		</td>
		<td>
		<?php
			echo 'Página da publicação: ';
			echo campo_texto( 'doppaginadou', 'N', 'S', 'Página da publicação', 110, 100, '', '','','','','id="pubpagdou"');
		?>
		</td>
		<td>
		<?php
			echo 'Secão da publicação: ';
			echo campo_texto( 'dopnumportaria', 'N', 'S', 'Portaria da publicação', 40, 50, '', '','','','','id="pubnumportaria"');
		?>
		</td>
	</tr>
	<tr id="botao" style="display: none;">
		<td align="center" bgcolor="#c0c0c0" colspan="3">
		<?php
		if( $dopid && $tpdcod == 100 ){
			echo '<input type="button" value="Imprimir" onclick="imprimirMinutaDOU(\''.$dopid.'\');" />';
			echo '&nbsp;<input type="button" value="Imprime Word" onclick="minutaConvenioWord(\''.$dopid.'\');" />';	
			echo '&nbsp;<input type="button" id="bt_enviaemail" value="Enviar e-mail" onclick="enviarEmailPublicacao(\''.$dopid.'\');" />';
		}
		 ?>
		<?php if($readonly == ''){?>
			<input type="button" id="bt_salvar" value="Salvar" onclick="<?php echo ($hasProblema !='') ? '' : 'salvarMinutaTermoAditivo();'?>" />
		<?php }?>
			&nbsp;
			<input type="button" id="bt_cancelar" value="Fechar" onclick="fechaMinuta();" />
		</td>
	</tr>
	</table></td>
	</tr>
</table>
</form>
<div id="erro"></div>
</body>
<?php  
$_POST['texto'] = '';
unset($_SESSION['par_minuta']['iueid']);
?>