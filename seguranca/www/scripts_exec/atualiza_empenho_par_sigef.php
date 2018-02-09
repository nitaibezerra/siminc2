<?php

/**** INCLUDES ****/

ini_set("memory_limit", "3024M");
set_time_limit(0);

//include_once "config.inc";
include_once "/var/www/simec/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . 'www/par/_funcoesPar.php';

/**** DECLARAÇÃO DE VARIAVEIS ****/
session_start();

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

$db 				= new cls_banco();
$wsusuario 			= 'USAP_WS_SIGARP';
$wssenha			= '03422625';
$data_created 		= date("c");
$data 				= new Data();

$dataInicio = date("d/m/Y h:i:s");

$sql = "select distinct substring(prpnumeroprocesso, 12, 4) from par.processopar order by substring(prpnumeroprocesso, 12, 4)";
$arAno = $db->carregarColuna($sql);
$arAno = $arAno ? $arAno : array();

foreach ($arAno as $ano) {

	$sql = "select
			    processo, codigo, sistema
			from(
				SELECT distinct
			    	prpid codigo, prpnumeroprocesso as processo, 'PAR' as sistema, prpstatus as status
			    FROM par.processopar p
			) as foo
			where 
				substring(processo, 12, 4) = '$ano'
			order by status desc";
	
	$processos = $db->carregar($sql);
	
	if(is_array($processos) ){
		foreach($processos as $dadosProcesso){
			
			$nu_processo = $dadosProcesso["processo"];
			$codigo  	 = $dadosProcesso["codigo"];
			$sistema 	 = $dadosProcesso["sistema"];
			
			$arrParam = array(
					'wsusuario'		=> $wsusuario,
					'wssenha' 		=> $wssenha,
					'sistema' 		=> $sistema,
					'codigo' 		=> $codigo,
					'nu_processo' 	=> $nu_processo,
					'method' 		=> 'historicoempenho',
			);
				
			$arrRetorno = montaXMLHistoricoProcessoSIGEF( $arrParam );
			$arrRetorno = $arrRetorno ? $arrRetorno : array();
			
			if( !empty($arrRetorno[0]['numero_do_processo']) ) {
				$sql = "SELECT processo FROM par.v_saldo_empenho_do_processo WHERE saldo > 0 and processo = '{$nu_processo}'";
				$testeProcesso = $db->pegaUm($sql);
				
				if( $testeProcesso ){
					$db->executar("UPDATE par.processopar SET prpdataconsultasigef = now(), prpstatus = 'A' WHERE prpnumeroprocesso = '{$nu_processo}'");
					$db->commit();
				}
				
				insereCargaEmpenhoSIGEF($arrRetorno, $arrParam);			
			}
		}
	}
}
$dataFim  = date("d/m/Y h:i:s");
$intervalos = intervaloEntreDatas($dataInicio, $dataFim);

$html = "<span style='color: red;'><b>Detalhes da Execução - Histórico SIGEF:</b><br/><br/>
									<b>Rotina de Atualização de Empenho PAR SIGEF ".$dataInicio." a ".$dataFim." realizada com sucesso em ".$intervalos."</b></span>";
$assunto  = SIGLA_SISTEMA. " - Atualiza Empenho PAR SIGEF";
enviar_email(array('nome'=>SIGLA_SISTEMA. ' - Histórico', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $html );
?>