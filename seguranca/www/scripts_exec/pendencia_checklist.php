<?php

set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

if(date('l')!='Saturday' && date('l')!='Sunday'){
	if(date('l')=='Monday'){
		// Pendências do dia
		$aPendenciasDia = getPendencias('hoje');
		$conteudo = 'Deverá ser informado, no Projeto "{PROJETO}", o item "{ITEM}". Responsável: {RESPONSAVEL} ({TELEFONE})';
		enviarNotificacoes($aPendenciasDia, $conteudo);
	}

	// Pendências daqui a 3 dias
	$aPendencias3Dias = getPendencias('3dias');
	$conteudo = 'Em 3 dias deverá ser informado, no Projeto "{PROJETO}", o item "{ITEM}". Responsável: {RESPONSAVEL} ({TELEFONE})';
	enviarNotificacoes($aPendencias3Dias, $conteudo);
}
echo "FIM";

function getPendencias($prazo = 'hoje')
{
	$db = new cls_banco();

	$sql = "SELECT distinct
				a1.atiid, a1._atinumero as _atinumero1, a1.atidescricao as projeto, a1.atiapelido,
				a2.atiid, a2._atinumero as _atinumero2, a2.atidescricao,
				a3.atiid, a3._atinumero as _atinumero3, a3.atidescricao as subatividade,
				ind.indapelido, dmiid, i.metid,
				mic.micid, mic.indid,
				mnm.mnmid, mnm.mnmdsc, mnm.micid, mnm.metid,
				i.dmiid, i.dmivalor, i.dmiqtde, i.dmidtcoleta, i.dmidataexecucao, i.dmidatameta,
				to_char(i.dmidtcoleta, 'DD/MM/YYYY') as data_coleta,
				to_char(i.dmidataexecucao, 'DD/MM/YYYY') as data_execucao,
				to_char(i.dmidatameta, 'DD/MM/YYYY') as data_meta,
				ent.entid, ent.entnome, ent.entemail,
				ent.entnumdddcelular,
				ent.entnumcelular,
				rc.dddcelular, rc.celular,
				'0' || ent.entnumdddcelular || ' ' ||ent.entnumcelular as celular_executor
			FROM pde.atividade a1
				INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
				INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
				INNER JOIN pde.monitoraitemchecklist mic on mic.atiid = a3.atiid AND mic.micstatus = 'A'
				INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
				INNER JOIN painel.indicador ind on mic.indid = ind.indid
				INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
				INNER JOIN workflow.documento doc ON doc.docid = i.docid
				INNER JOIN pde.monitorametaentidade me on me.mnmid = mnm.mnmid
				inner join entidade.entidade ent ON ent.entid = me.entid
                left join pde.notificacaometaresponsavel nmr
                    inner join pde.responsavelcelular rc on rc.usucpf = nmr.usucpf
                on nmr.micid = mic.micid
			WHERE doc.esdid = 443
			-- and a1.atiid = 134153
			and mic.micenviasms    = 't'
			and coalesce(ent.entnumdddcelular, '0') != '0'
			and coalesce(ent.entnumcelular,    '0') != '0'
			AND a1.atistatus = 'A'";

	$sql .= $prazo == 'hoje' ?
			" and i.dmidatameta <= CURRENT_DATE " : // Data Atual
			" and to_char(i.dmidatameta, 'YYYYMMDD') = to_char((now() + interval '3 day'), 'YYYYMMDD') "; // 3 Dias antes

	$sql .= "order by a1.atidescricao, a2._atinumero, a3._atinumero, mic.indid";

//	ver($sql, d);

	return $db->carregar($sql);
}

function enviarNotificacoes($pendencias, $conteudo)
{
	$db = new cls_banco();

	$aPendencias = array();
	if($pendencias){
		foreach ($pendencias as $pendencia) {
			$aPendencias[$pendencia['dmiid']][] = $pendencia;
		}
	}

	foreach ($aPendencias as $dmiid => $aContatosDia) {
    	$aCelularEnvio = array();

		$aCelularEnvio = array(
//			'556191434894', // Daniel
//			'556199440090', // Marcos
//			'556192191325', // Raquel
			'556181054537', // Orion
		);

		foreach ($aContatosDia as $contato) {
			$aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['entnumdddcelular'] . $contato['entnumcelular']);
			$aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['dddcelular'] . $contato['celular']);
		}

		$aCelularEnvio = array_unique($aCelularEnvio);

		$conteudo = str_replace(
			array('{PROJETO}', '{ITEM}', '{RESPONSAVEL}', '{TELEFONE}', ';'),
			array($contato['atiapelido'], $contato['indapelido'], $contato['entnome'], $contato['celular_executor'], '.'),
			$conteudo
		);

		// Se tiver mais que 150 caracteres, corta o nome do responsável
		if(strlen($conteudo) > 150){
			$conteudo = str_replace($pendencia['entnome'], substr($pendencia['entnome'], 0, 20), $conteudo);
		}


		// Envia SMS
		$humanMultipleSend = new HumanMultipleSend("mec.gway", "WWh4O8xZOo");
		$tipo = HumanMultipleSend::TYPE_A;
		$tipo = HumanMultipleSend::TYPE_E;

		$data = date('Y-m-d') . ' 10:00:00';
		$schedule = date('d/m/Y H:i:s', strtotime($data));

		$aMensagem = array();
		foreach ($aCelularEnvio as $celular) {
			$sql = "INSERT INTO seguranca.sms(
		            sisid, smscelular, smsmensagem, smsdataenvio)
				    VALUES ('132', '$celular', '$conteudo', '$data')
				    returning smsid;
			";
			$id = $db->pegaUm($sql);
			$db->commit();

			$aMensagem[] = "{$celular};{$conteudo};$id;;$schedule";
		}
		$msg_list  = implode("\n", $aMensagem);

//		ver($msg_list, d);

		$responses = $humanMultipleSend->sendMultipleList($tipo, $msg_list);

	//	foreach ($responses as $response) {
	//		echo $response->getCode();
	//		echo $response->getMessage();
	//	}


		ver($msg_list, $responses);
	}
}
?>