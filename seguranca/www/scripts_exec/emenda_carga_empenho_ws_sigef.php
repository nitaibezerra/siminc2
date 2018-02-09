<?php

/**** INCLUDES ****/

ini_set("memory_limit", "3024M");
set_time_limit(0);

$_REQUEST['baselogin'] = "simec_espelho_producao";

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
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
$data 				= new Data();

$dataInicio = date("d/m/Y h:i:s");

$sql = "select distinct
			p.ptrid,
		    p.ptrcod,
			p.ptrnumprocessoempenho
		from
			emenda.planotrabalho p
		where
			p.ptrnumprocessoempenho is not null
		    and p.ptrstatus = 'A'
			and p.ptridpai is null
		order by p.ptrid";

$arrProcessos = $db->carregar($sql);
$arrProcessos = $arrProcessos ? $arrProcessos : array();

$db->executar("delete from emenda.empenho_carga_sigef");

$total = 0;
if(is_array($arrProcessos) ){
	foreach($arrProcessos as $v){
		
		$nu_processo = $v["ptrnumprocessoempenho"];
		
		$arrParam = array(
				'wsusuario'		=> $wsusuario,
				'wssenha' 		=> $wssenha,
				'ptrid' 		=> $v["ptrid"],
				'ptrcod' 		=> $v['ptrcod'],
				'nu_processo' 	=> $nu_processo,
				'method' 		=> 'historicoempenho',
		);
		
		$arrRetorno = montaXMLHistoricoProcessoSIGEF( $arrParam );
		$arrRetorno = $arrRetorno ? $arrRetorno : array();
		if( !empty($arrRetorno[0]['numero_do_processo']) ) {
			insereCargaEmpenhoEmendaSIGEF($arrRetorno, $arrParam);
			$total++;
		}
	}
}
$dataFim  = date("d/m/Y h:i:s");
$intervalos = intervaloEntreDatas($dataInicio, $dataFim);

$html = "<span style='color: red;'><b>Detalhes da Execução - Histórico SIGEF:</b><br/><br/>
									<b>Rotina de Empenho EMENDAS SIGEF ".$dataInicio." a ".$dataFim." realizada com sucesso em ".$intervalos."</b>Foram atualizados ".$total."/".sizeof($arrProcessos)." Processos</span>";

$assunto  = SIGLA_SISTEMA. " - Carga de Empenho EMENDAS SIGEF";
enviar_email(array('nome'=>SIGLA_SISTEMA. ' - Histórico Emendas', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $html );

$db->close();

function insereCargaEmpenhoEmendaSIGEF( $arrRetorno, $arrParam ){
	global $db;
	
	$nu_processo = $arrParam["nu_processo"];
	$ptrid  	 = $arrParam["ptrid"];
	$ptrcod 	 = $arrParam["ptrcod"];
	
	/* $pedid	 	 = $arrParam["pedid"];
	$tpeid	 	 = $arrParam["tpeid"];
	$exfid	 	 = $arrParam["exfid"]; */
	
	foreach($arrRetorno as $chaves => $dados ){
		
		$dados['data_do_empenho'] = ($dados['data_do_empenho'] == '--' ? '' : $dados['data_do_empenho']);
		
		$cnpj 						= trim((string)$dados['cnpj']);
		$programa_fnde 				= trim((string)$dados['programa_fnde']);
		$unidade_gestora 			= trim((string)$dados['unidade_gestora']);
		$numero_da_proposta_siconv 	= trim((string)$dados['numero_da_proposta_siconv']);
		$numero_da_ne 				= trim((string)$dados['numero_da_ne']);
		$numero_de_vinculacao_ne 	= trim((string)$dados['numero_de_vinculacao_ne']);
		$valor_da_ne 				= trim((string)$dados['valor_da_ne']);
		$numero_sequencial_da_ne 	= trim((string)$dados['numero_sequencial_da_ne']);
		$nu_seq_mov_ne 				= trim((string)$dados['nu_seq_mov_ne']);
		$data_do_empenho 			= trim((string)$dados['data_do_empenho']);
		$cpf 						= trim((string)$dados['cpf']);
		$nu_id_sistema 				= trim((string)$dados['nu_id_sistema']);
		$descricao_do_empenho 		= trim((string)$dados['descricao_do_empenho']);
		$ano_do_empenho 			= trim((string)$dados['ano_do_empenho']);
		$centro_de_gestao 			= trim((string)$dados['centro_de_gestao']);		
		$natureza_de_despesa 		= trim((string)$dados['natureza_de_despesa']);
		$fonte_de_recurso 			= trim((string)$dados['fonte_de_recurso']);
		$ptres 						= trim((string)$dados['ptres']);
		$esfera 					= trim((string)$dados['esfera']);
		$pi 						= trim((string)$dados['pi']);
		$cod_especie 				= trim((string)$dados['cod_especie']);
		$numero_do_processo 		= trim((string)$dados['numero_do_processo']);
		$situacao_do_empenho 		= trim((string)$dados['situacao_do_empenho']);
		
		$teeid = 'null';
		if($cod_especie == '01'){
			$teeid = 1;
		}elseif($cod_especie == '02'){
			$teeid = 2;
		}elseif($cod_especie == '03' || $cod_especie == '13'){
			$teeid = 3;
		}elseif($cod_especie == '04'){
			$teeid = 4;
		}
		
		$numero_de_vinculacao_ne 	= $numero_de_vinculacao_ne ? "'".$numero_de_vinculacao_ne."'" : 'NULL';
		$nu_seq_mov_ne 				= $nu_seq_mov_ne ? $nu_seq_mov_ne : 'NULL';				
		$exfid 						= $exfid ? $exfid : 'NULL';				
		$pedid 						= $pedid ? $pedid : 'NULL';				
		$tpeid 						= $tpeid ? $tpeid : 'NULL';				
		$nu_id_sistema 				= $nu_id_sistema ? "'".$nu_id_sistema."'" : 'NULL';
		$data_do_empenho 			= $data_do_empenho ? "'".$data_do_empenho."'" : 'NULL';
		 
		$numero_da_ne 				= $numero_da_ne ? "'".$numero_da_ne."'" : 'NULL';
		$valor_da_ne 				= $valor_da_ne ? "'".$valor_da_ne."'" : 'NULL';
		$numero_sequencial_da_ne 	= $numero_sequencial_da_ne ? "'".$numero_sequencial_da_ne."'" : 'NULL';
		
		$sql = "INSERT INTO emenda.empenho_carga_sigef(ptrid, teeid, cnpj, programa_fnde, unidade_gestora, numero_da_proposta_siconv, numero_da_ne, numero_de_vinculacao_ne,
  					valor_da_ne, numero_sequencial_da_ne, nu_seq_mov_ne, data_do_empenho, cpf, nu_id_sistema, descricao_do_empenho, ano_do_empenho, centro_de_gestao,
  					natureza_de_despesa, fonte_de_recurso, ptres, esfera, pi, cod_especie, numero_do_processo, situacao_do_empenho)
  				VALUES ($ptrid, $teeid, '{$cnpj}', '{$programa_fnde}', '{$unidade_gestora}', '{$numero_da_proposta_siconv}', {$numero_da_ne}, {$numero_de_vinculacao_ne},
  					{$valor_da_ne}, {$numero_sequencial_da_ne}, {$nu_seq_mov_ne}, {$data_do_empenho}, '{$cpf}', {$nu_id_sistema}, '{$descricao_do_empenho}', '{$ano_do_empenho}', '{$centro_de_gestao}',
  					'{$natureza_de_despesa}', '{$fonte_de_recurso}', '{$ptres}', '{$esfera}', '{$pi}', '{$cod_especie}', '{$numero_do_processo}', '{$situacao_do_empenho}')";
			
		$db->executar($sql);
		
		$db->commit();
	}
	return true;
}
?>