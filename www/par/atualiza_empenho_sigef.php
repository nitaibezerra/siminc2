<?
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}
 
 session_start();
 
 $Tinicio = getmicrotime();
 
 $dataIni = date("d/m/Y h:i:s");

if(!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

/* $arrProcesso = "'23400009625201271', '23400010786201216', '23400011498201271', '23400011847201254', '23400001632201225', '23400001637201258'";
$arrProcesso = "'23400001637201258'";

$sql = "select
			processo,
		    cnpj,
		    programa,
		    sistema
		from(
		  SELECT distinct
		      p.prpnumeroprocesso as processo,
		      p.prpcnpj as cnpj,
		      'CM' as programa,
		      'PAR' as sistema
		  FROM 
		      par.processopar p
		  WHERE 
		      p.prpcnpj is not null
		      and p.prpstatus = 'A'
		  
		  union all
		  
		  SELECT distinct
		      p.pronumeroprocesso as processo,
		      p.procnpj as cnpj,
		      'CM' as programa,
		      'ObrasPAR' as sistema
		  FROM 
		      par.processoobraspar p
		  WHERE 
		      p.procnpj <> ''
		      
		  union all
		  
		  SELECT distinct
		      p.pronumeroprocesso as processo,
		      p.procnpj as cnpj,
		      'CN' as programa,
		      'PAC' as sistema
		  FROM 
		      par.processoobra p
		  WHERE 
		      p.procnpj is not null
		) as foo		 
		where processo in ('23400001695201281')";

$arrProcesso = $db->carregar($sql);
$arrProcesso = $arrProcesso ? $arrProcesso : array(); */

$wsusuario 	= 'juliov';
$wssenha	= '1segredo';

$wsusuario 	= 'USAP_WS_SIGARP';
$wssenha	= '03422625';
$data_created = date("c");

//$db->executar("delete from par.empenhodivergentesigef");
$arrProcesso = array(array(
					'cnpj' => '',
					'processo' => '23400016037200999',
					'programa' => '03',
					'sistema' => '2',
				));
foreach ($arrProcesso as $v) {
	$co_cnpj 			= $v['cnpj'];
	$nu_processo 		= $v['processo'];
	$co_programa_fnde 	= $v['programa'];	
	$sistema 			= $v['sistema'];	

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
			<co_cnpj>$co_cnpj</co_cnpj>
			<nu_processo>$nu_processo</nu_processo>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<efetivados>S</efetivados>
			<rownum>0</rownum>
			<numero_de_linhas>200</numero_de_linhas>
		</params>
	</body>
</request>
XML;

	/*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
	} else {*/

		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
	//}
	//$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';

	$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoNE') )
			->execute();

	$xmlRetorno = $xml;
    $xml = simplexml_load_string( stripslashes($xml));
	ver( simec_htmlentities($arqXml), $xml, d );
	
	if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
		$arrXML = $xml->body->children();
		
		$boEmpenho = false;
		$arrSeqNE = array();
		foreach ($arrXML as $xml) {
			$nu_seq_ne 						= trim((string)$xml->nu_seq_ne);
			$nu_empenho_original 			= trim((string)$xml->nu_empenho_original);
			$nu_seq_doc_siafi_original 		= trim((string)$xml->nu_seq_doc_siafi_original);
			$an_exercicio_original 			= trim((string)$xml->an_exercicio_original);
			$situacao_documento 			= trim((string)$xml->situacao_documento);
			$data_documento 				= trim((string)$xml->data_documento);			
			$valor_ne 						= trim((string)$xml->valor_ne);
			$processo 						= trim((string)$xml->processo);
			$nu_cnpj 						= trim((string)$xml->nu_cnpj);
			$numero_documento 				= trim((string)$xml->numero_documento);
			$ds_problema 					= trim((string)$xml->ds_problema);
			$co_especie_empenho 			= trim((string)$xml->co_especie_empenho);
			$valor_total_empenhado 			= trim((string)$xml->valor_total_empenhado);
			$valor_saldo_pagamento 			= trim((string)$xml->valor_saldo_pagamento);			
			$co_programa_fnde 				= trim((string)$xml->co_programa_fnde);
			$ano_convenio_original 			= trim((string)$xml->ano_convenio_original);			
			$nu_convenio_original 			= trim((string)$xml->nu_convenio_original);
			$unidade_gestora_responsavel	= trim((string)$xml->unidade_gestora_responsavel);
			$co_diretoria 					= trim((string)$xml->co_diretoria);
			
			//array_push($arrSeqNE, $nu_seq_ne);
			
			/*$valor_ne = str_replace(".","", $valor_ne);
			$valor_ne = str_replace(",",".", $valor_ne);*/
			$arrPrograma = explode('-', $co_programa_fnde);
			
			$empnumerooriginal 		= substr($numero_documento, 6);
			$empanooriginal 		= substr($numero_documento, 0, 4);
			
			$situacaoSIGEF = $situacao_documento;
			
			$data_documento 		= ($data_documento 			? "'".$data_documento."'" 			: 'null');			
			$empnumerooriginal 		= ($empnumerooriginal 		? "'".$empnumerooriginal."'" 		: 'null');
			$empanooriginal 		= ($empanooriginal 			? "'".$empanooriginal."'" 			: 'null');
			$nu_convenio_original 	= ($nu_convenio_original 	? "'".$nu_convenio_original."'" 	: 'null');
			$ano_convenio_original 	= ($ano_convenio_original 	? "'".$ano_convenio_original."'" 	: 'null');
			$situacao_documento 	= ($situacao_documento 		? "'".$situacao_documento."'" 		: 'null');
			$valor_total_empenhado 	= ($valor_total_empenhado 	? "'".$valor_total_empenhado."'" 	: 'null');
			$valor_saldo_pagamento 	= ($valor_saldo_pagamento 	? "'".$valor_saldo_pagamento."'" 	: 'null');
			$nu_empenho_original 	= ($nu_empenho_original 	? "'".$nu_empenho_original."'"		: 'null');
			
			$sql = "select case when e.empnumero is not null then e.empnumero else e.empprotocolo end 
					from par.empenho e where e.empnumeroprocesso = '$processo' and empcnpj = '{$nu_cnpj}' and empstatus = 'A'";
			$arrNumEmpenho = $db->carregarColuna($sql);
			
			//$db->executar("update par.processopar set enviadosigef = 'S' where prpnumeroprocesso = '$processo'");
			
			/*if( $co_especie_empenho == '03'  ){ #CANCELAMENTO
				$sql = "update par.empenhosubacao set eobstatus = 'I' where empid in (
							select empid from par.empenho e 
							where e.empnumeroprocesso = '$processo' and empnumero = '{$numero_documento}'
							)";
				$db->executar($sql);
			}*/
			
			if($arrNumEmpenho){
				
				if( in_array( $nu_seq_ne, $arrNumEmpenho ) || in_array( $numero_documento, $arrNumEmpenho ) ){
					
					if( in_array( $nu_seq_ne, $arrNumEmpenho ) ) $filtro = " and empprotocolo = '{$nu_seq_ne}'";
					if( in_array( $numero_documento, $arrNumEmpenho ) ) $filtro = " and empnumero = '{$numero_documento}'";
					
					$sql = "SELECT e.empcodigoespecie, e.empsituacao, e.empprotocolo, e.empnumero, vve.vrlempenhocancelado as empvalorempenho 
							FROM par.empenho e
								inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
							WHERE e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' and e.empnumeroprocesso = '{$processo}' $filtro";
					$arAtualiza = $db->pegaLinha($sql);
					
					if( $arAtualiza['empcodigoespecie'] != $co_especie_empenho || $arAtualiza['empsituacao'] != $situacaoSIGEF 
							|| $arAtualiza['empprotocolo'] != $nu_seq_ne || $arAtualiza['empnumero'] != $numero_documento || $arAtualiza['empvalorempenho'] != $valor_ne){
						
						$sql = "UPDATE par.empenho SET 
								  empcnpj 					= '{$nu_cnpj}',
								  empnumerooriginal 		= {$empnumerooriginal},
								  empanooriginal 			= {$empanooriginal},
								  empcodigoespecie 			= '{$co_especie_empenho}',
								  empanoconvenio 			= {$ano_convenio_original},
								  empnumeroconvenio 		= {$nu_convenio_original},
								  empunidgestoraeminente 	= '{$unidade_gestora_responsavel}',
								  empprogramafnde 			= '".trim($arrPrograma[0])."',
								  empsituacao 				= {$situacao_documento},
								  nu_seq_ne 				= '{$nu_seq_ne}',
								  empnumero 				= '{$numero_documento}',
								  empvalorempenho 			= '{$valor_ne}',
								  ds_problema 				= '{$ds_problema}',
								  valor_total_empenhado 	= {$valor_total_empenhado},
								  valor_saldo_pagamento 	= {$valor_saldo_pagamento},
								  empdata 					= {$data_documento},
								  co_diretoria 				= '{$co_diretoria}',
								  empnumerooriginalpai 		= {$nu_empenho_original},
								  nu_seq_doc_siafi_original	= '{$nu_seq_doc_siafi_original}',
								  empatualizadosigef 		= 'S'
							WHERE 
	  							empnumeroprocesso = '{$processo}'
	  							$filtro";
						$db->executar($sql);
						//ver($sql);
					}
				} else {
					if( $co_especie_empenho == '03'  ){
						$sql = "SELECT e.empid, e.empcnpj, e.empnumerooriginal, e.empanooriginal, e.empnumeroprocesso, e.empcodigoespecie, e.empcodigopi, e.empcodigoesfera, e.empcodigoptres, e.empfonterecurso,
			  						e.empcodigonatdespesa, e.empcentrogestaosolic, e.empanoconvenio, e.empnumeroconvenio, e.empcodigoobs, e.empcodigotipo, e.empdescricao, e.empgestaoeminente, e.empunidgestoraeminente,
			  						e.empprogramafnde, e.empnumerosistema, e.empsituacao, e.usucpf, e.empprotocolo, e.empnumero, vve.vrlempenhocancelado as empvalorempenho, e.ds_problema, e.valor_total_empenhado, valor_saldo_pagamento,
			  						e.empdata, e.tp_especializacao, e.co_diretoria, e.empnumerooriginalpai, e.nu_seq_doc_siafi_original
			  					FROM par.empenho e
			  						inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid 
								where e.empnumeroprocesso = '$processo' and e.empcnpj = '{$nu_cnpj}' and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'";
						
						$arrEmpenho = $db->pegaLinha( $sql );
						
						$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empanoconvenio, empnumeroconvenio,
									empunidgestoraeminente, empprogramafnde, empsituacao, usucpf, empprotocolo, empnumero, empvalorempenho, ds_problema, valor_total_empenhado,
		  							valor_saldo_pagamento, empdata, co_diretoria, empnumerooriginalpai, nu_seq_doc_siafi_original, empcodigopi, empcodigoptres, empfonterecurso, 
		  							empcodigonatdespesa, empcodigoesfera, empcentrogestaosolic, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empnumerosistema, empatualizadosigef) 
								VALUES ('{$nu_cnpj}', {$empnumerooriginal}, {$empanooriginal}, '{$processo}', '{$co_especie_empenho}', {$ano_convenio_original}, {$nu_convenio_original},
									'{$unidade_gestora_responsavel}', '".trim($arrPrograma[0])."', {$situacao_documento}, '', '{$nu_seq_ne}', '{$numero_documento}', 
									'{$valor_ne}', '{$ds_problema}', {$valor_total_empenhado}, {$valor_saldo_pagamento}, {$data_documento}, '{$co_diretoria}', {$nu_empenho_original}, '{$nu_seq_doc_siafi_original}', 
									'{$arrEmpenho['empcodigopi']}', '{$arrEmpenho['empcodigoptres']}', '{$arrEmpenho['empfonterecurso']}', '{$arrEmpenho['empcodigonatdespesa']}', 
									'{$arrEmpenho['empcodigoesfera']}', '{$arrEmpenho['empcentrogestaosolic']}', '{$arrEmpenho['empcodigoobs']}', '{$arrEmpenho['empcodigotipo']}',
									'{$arrEmpenho['empdescricao']}', '{$arrEmpenho['empgestaoeminente']}', '{$arrEmpenho['empnumerosistema']}', 'S') returning empid";
						$empid = $db->pegaUm($sql);
						//ver($sql);
						$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento)
				    			VALUES ('', $empid, NOW(), $situacao_documento, '{$co_especie_empenho}', '{$ds_problema}', {$valor_total_empenhado}, {$valor_saldo_pagamento})";
						$db->executar($sql);
					} else {
						$sql = "INSERT INTO par.empenhodivergentesigef(nu_seq_ne, nu_empenho_original, nu_seq_doc_siafi_original, an_exercicio_original, situacao_documento, data_documento,
		  							valor_ne, processo, nu_cnpj, numero_documento, ds_problema, co_especie_empenho, valor_total_empenhado, valor_saldo_pagamento, co_programa_fnde,
		  							ano_convenio_original, nu_convenio_original, unidade_gestora_responsavel, co_diretoria, sistema) 
								VALUES ('{$nu_seq_ne}', {$nu_empenho_original}, '{$nu_seq_doc_siafi_original}', '{$an_exercicio_original}', {$situacao_documento}, {$data_documento},
		  							'{$valor_ne}', '{$processo}', '{$nu_cnpj}', '{$numero_documento}', '{$ds_problema}', '{$co_especie_empenho}', {$valor_total_empenhado}, {$valor_saldo_pagamento}, '{$co_programa_fnde}',
		  							{$ano_convenio_original}, {$nu_convenio_original}, '{$unidade_gestora_responsavel}', '{$co_diretoria}', '{$sistema}')";
						$db->executar($sql);
						//ver($sql);
					}
				}
			} else {
				$sql = "INSERT INTO par.empenhodivergentesigef(nu_seq_ne, nu_empenho_original, nu_seq_doc_siafi_original, an_exercicio_original, situacao_documento, data_documento,
  							valor_ne, processo, nu_cnpj, numero_documento, ds_problema, co_especie_empenho, valor_total_empenhado, valor_saldo_pagamento, co_programa_fnde,
  							ano_convenio_original, nu_convenio_original, unidade_gestora_responsavel, co_diretoria, sistema) 
						VALUES ('{$nu_seq_ne}', {$nu_empenho_original}, '{$nu_seq_doc_siafi_original}', '{$an_exercicio_original}', {$situacao_documento}, {$data_documento},
  							'{$valor_ne}', '{$processo}', '{$nu_cnpj}', '{$numero_documento}', '{$ds_problema}', '{$co_especie_empenho}', {$valor_total_empenhado}, {$valor_saldo_pagamento}, '{$co_programa_fnde}',
  							{$ano_convenio_original}, {$nu_convenio_original}, '{$unidade_gestora_responsavel}', '{$co_diretoria}', '{$sistema}')";
				$db->executar($sql);
				//ver($sql);
			}
		}
		if( $arrSeqNE[0] ) {
			/*$sql = "update par.empenho set empstatus = 'I' 
					where empid in ( select 
										e.empid
									from par.empenho e 
									where 
										e.empprotocolo not in ('".implode("', '", $arrSeqNE)."')
									    and e.empnumeroprocesso = '$processo'
									    and e.empcnpj = '$nu_cnpj'
									    and e.empsituacao = 'CANCELADO'
									)";
			$db->executar($sql);*/
		}
		$db->commit();
	}
}
//die('foi');
$Tfinal= getmicrotime() - $Tinicio;

die("<p>A atualização dos Empenhos foram realizados com sucesso! Data Inicio: ".$dataIni.", Data Fim: ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>");

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SCRIPT AUTOMATICO";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "thiago.barbosa@mec.gov.br", "Thiago Barbosa" );
//$mensagem->AddAddress( "wesley.silva@mec.gov.br", "Wesley Romualdo" );
$mensagem->Subject = "Atualização do PAR - Empenho do SIGEF";
$mensagem->Body = "A atualização dos Empenhos foram realizados com sucesso! Data Inicio: ".$dataIni.", Data Fim: ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>";
$mensagem->IsHTML( true );
$mensagem->Send();

?>