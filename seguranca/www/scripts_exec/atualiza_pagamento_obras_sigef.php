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

/**** DECLARA��O DE VARIAVEIS ****/
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

$sql = "select distinct substring(pronumeroprocesso, 12, 4) from par.processoobraspar order by substring(pronumeroprocesso, 12, 4)";
$arAno = $db->carregarColuna($sql);
$arAno = $arAno ? $arAno : array();

foreach ($arAno as $ano) {

	$sql = "select
			    processo, codigo, sistema
			from(
				SELECT distinct
			    	proid as codigo, pronumeroprocesso as processo, 'ObrasPAR' as sistema, prostatus as status
			  	FROM par.processoobraspar p
			  		inner join par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso
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
					'method' 		=> 'historicopagamento',
			);
			
			$arrRetorno = montaXMLHistoricoProcessoSIGEF( $arrParam );
			$arrRetorno = $arrRetorno ? $arrRetorno : array();
	
			insereCargaPagamentoSIGEF($arrRetorno, $arrParam);
		}
	}
}

$dataFim  = date("d/m/Y h:i:s");
$intervalos = intervaloEntreDatas($dataInicio, $dataFim);

$html = "<span style='color: red;'><b>Detalhes da Execu��o - Hist�rico SIGEF:</b><br/><br/>
									<b>Rotina de Atualiza��o de Pagamento OBRAS PAR SIGEF ".$dataInicio." a ".$dataFim." realizada com sucesso em ".$intervalos."</b></span>";
$assunto  = SIGLA_SISTEMA. " - Atualiza Pagamento OBRAS PAR SIGEF";
enviar_email(array('nome'=>SIGLA_SISTEMA. ' - Hist�rico', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $html );	
?>