<?php

/**** INCLUDES ****/

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
include_once APPRAIZ . 'www/par/_funcoesPar.php';

/**** DECLARAÇÃO DE VARIAVEIS ****/
session_start();
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db 				= new cls_banco();
$wsusuario 			= 'USAP_WS_SIGARP';
$wssenha			= '03422625';
$data_created 		= date("c");
$ano				= $_REQUEST['ano'];
$sistema			= $_REQUEST['sistema'];
$qtdregistro		= $_REQUEST['qtdregistro'];

$dataInicio = date("d/m/Y h:i:s");

if( $sistema == 'PAR' ){
	$sql = "select
			    processo, codigo, sistema, prpdataconsultasigef
			from(
				SELECT distinct
			    	prpid codigo, prpnumeroprocesso as processo, 'PAR' as sistema, prpstatus as status, coalesce(p.prpdatapagamentosigef, '1900-01-01') as prpdataconsultasigef
			    FROM par.processopar p
                	inner join par.empenho e on e.empnumeroprocesso = p.prpnumeroprocesso and e.empstatus = 'A'
                    inner join par.pagamento pag on pag.empid = e.empid and pag.pagstatus = 'A'
			    where prpstatus = 'A'
			) as foo
			where 
				substring(processo, 12, 4) = '$ano'
				and cast(to_char(coalesce(prpdataconsultasigef, '1900-01-01'), 'YYYY-MM-DD') as date) <> cast(to_char(now(), 'YYYY-MM-DD') as date)
			order by prpdataconsultasigef asc";
} elseif( $sistema == 'OBRAS' ){
	$sql = "select
			    processo, codigo, sistema, prodataconsultasigef
			from(
				SELECT distinct
			    	proid as codigo, pronumeroprocesso as processo, 'OBRAS' as sistema, prostatus as status, coalesce(p.prodatapagamentosigef, '1900-01-01') as prodataconsultasigef
			  	FROM 
			      	par.processoobraspar p
			      	inner join par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empstatus = 'A'
                    inner join par.pagamento pag on pag.empid = e.empid and pag.pagstatus = 'A'
			    where prostatus = 'A'
			) as foo 
			where 
            	substring(processo, 12, 4) = '$ano'
                and cast(to_char(coalesce(prodataconsultasigef, '1900-01-01'), 'YYYY-MM-DD') as date) <> cast(to_char(now(), 'YYYY-MM-DD') as date)
			order by prodataconsultasigef asc";
} elseif( $sistema == 'PAC' ){
	$sql = "select
			    processo, codigo, sistema, prodataconsultasigef
			from(
				SELECT distinct
			    	proid as codigo, pronumeroprocesso as processo, 'PAC' as sistema, prostatus as status, coalesce(p.prodatapagamentosigef, '1900-01-01') as prodataconsultasigef
			  	FROM 
			      	par.processoobra p
			      	inner join par.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empstatus = 'A'
                    inner join par.pagamento pag on pag.empid = e.empid and pag.pagstatus = 'A'
			    where prostatus = 'A'
			) as foo 
			where 
				substring(processo, 12, 4) = '$ano'
				and cast(to_char(coalesce(prodataconsultasigef, '1900-01-01'), 'YYYY-MM-DD') as date) <> cast(to_char(now(), 'YYYY-MM-DD') as date)
			order by prodataconsultasigef asc";
}

$arrProcesso = $db->carregar($sql);
$arrProcesso = $arrProcesso ? $arrProcesso : array();

$strProcesso = '';
$totAtualizado = 0;
foreach($arrProcesso as $dadosProcesso){
	
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
	//ver($nu_processo, $arrRetorno,d);
	if( !empty($arrRetorno[0]['nu_seq_mov_pag']) ) {
		insereCargaPagamentoSIGEF($arrRetorno, $arrParam);
		$totAtualizado++;
		
		if( $sistema == 'PAR' ){
			$sql = "UPDATE par.processopar SET prpdatapagamentosigef = now() WHERE prpid = $codigo";
		} elseif( $sistema == 'OBRAS' ){
			$sql = "UPDATE par.processoobraspar SET prodatapagamentosigef = now() WHERE proid = $codigo";
		} elseif( $sistema == 'PAC' ){
			$sql = "UPDATE par.processoobra SET prodatapagamentosigef = now() WHERE proid = $codigo";
		}
		$db->executar($sql);
		$db->commit();
	}
}
$dataFim  = date("d/m/Y h:i:s");
$intervalos = intervaloEntreDatas($dataInicio, $dataFim);

$html = "<span style='color: red;'><b>Detalhes da Execução - Histórico SIGEF:</b><br/><br/>
									<b>Rotina de Carga de Pagamentos ".$sistema." do SIGEF ".$dataInicio." a ".$dataFim.",<br>realizada com sucesso em ".$intervalos.", foi atualizado ".$totAtualizado." pagamentos.</b></span>";
$assunto  = SIGLA_SISTEMA. " - Carga Pagamento ".$sistema."/".$ano." do SIGEF ";

enviar_email(array('nome'=>SIGLA_SISTEMA. ' - CARGA PROCESSO SIGEF - '.$ano, 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $html );
$db->close();
?>