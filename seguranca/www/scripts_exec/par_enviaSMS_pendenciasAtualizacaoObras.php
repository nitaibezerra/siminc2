<?php
set_time_limit(0);

define('TIPO_PREFEITO', 'P');
define('TIPO_FISCAL', 'F');
define('TIPO_ALERTA_VERMELHO', '60');
define('TIPO_ALERTA_AMARELO', '45');
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$obras = array();

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';


/****************************************
*				PREFEITO				*
****************************************/

$conteudo = "Gestor(a),MEC informa:Há obra(s) no SIMEC desatualizadas há mais de 60 dias.Assistência financeira ao município suspensa até regularização.";

// Pendências do município > 60 dias
$aPendenciasDia = getPendenciasMun(60, TIPO_PREFEITO);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_VERMELHO, $conteudo);

// Pendências da UF > 60 dias
$aPendenciasDia = getPendenciasUF(60, TIPO_PREFEITO);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_VERMELHO, $conteudo);

$conteudo = "Gestor(a),MEC informa:Há obra(s) no SIMEC desatualizadas há mais de 45 dias.Atualize, sob pena de suspensão de assistência financeira.";

// Pendências do município entre 45 e 60 dias
$aPendenciasDia = getPendenciasMun(45, TIPO_PREFEITO);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_AMARELO, $conteudo);

// Pendências da UF entre 45 e 60 dias
$aPendenciasDia = getPendenciasUF(45, TIPO_PREFEITO);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_AMARELO, $conteudo);


/****************************************
*				FISCAL					* 
****************************************/

$conteudo = "MEC informa:Há obra(s) no SIMEC sob sua reponsabilidade desatualizadas há mais de 60 dias.Atualize, sob pena de sanções ao município.";

// Pendências do município > 60 dias
$aPendenciasDia = getPendenciasMun(60, TIPO_FISCAL);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_VERMELHO, $conteudo);

// Pendências da UF > 60 dias
$aPendenciasDia = getPendenciasUF(60, TIPO_FISCAL);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_VERMELHO, $conteudo);

$conteudo = "MEC informa:Há obra(s) no SIMEC sob sua reponsabilidade desatualizadas há mais de 45 dias.Atualize, sob pena de sanções ao município.";

// Pendências do município entre 45 e 60 dias
$aPendenciasDia = getPendenciasMun(45, TIPO_FISCAL);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_AMARELO, $conteudo);

// Pendências da UF entre 45 e 60 dias
$aPendenciasDia = getPendenciasUF(45, TIPO_FISCAL);
// ver(count($aPendenciasDia));
enviarNotificacoes($aPendenciasDia, TIPO_ALERTA_AMARELO, $conteudo);

registraAtividade($obras);

echo "FIM";

function getPendenciasMun($prazo = '60', $tipo = TIPO_PREFEITO)
{
    $db = new cls_banco();

    $wherePrazo = $prazo == '60' ? ' (o.diasultimaalteracao > 60) ' : ' (o.diasultimaalteracao between 46 and 60) ';

	if ($tipo == TIPO_PREFEITO) {
		$from = "FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade     			fen  ON fen.entid  = ent.entid AND fen.funid = 2
					INNER JOIN entidade.funentassoc     			fea  ON fea.fueid  = fen.fueid
					INNER JOIN entidade.entidade         			ent2 ON ent2.entid = fea.entid
					INNER JOIN entidade.funcaoentidade     			fen2 ON fen2.entid = ent2.entid AND fen2.funid = 1
					INNER JOIN entidade.endereco         			end2 ON end2.entid = ent2.entid
					INNER JOIN territorios.municipio 				mun  ON mun.muncod = end2.muncod
					INNER JOIN par.instrumentounidade 				iu   ON iu.muncod  = mun.muncod
					INNER JOIN tmp_obras_situacao_municipal 	o    ON o.muncod   = iu.muncod AND iu.itrid <> 1 AND (iu.estuf IS NULL OR iu.estuf != 'DF')";
		
		$where = " AND ent2.entstatus='A' ";
	} else {
		$from = "FROM   tmp_obras_situacao_municipal o
					inner join par.instrumentounidade iu ON o.muncod   = iu.muncod AND iu.itrid <> 1 AND (iu.estuf IS NULL OR iu.estuf != 'DF')
					inner join obras2.usuarioresponsabilidade ur on ur.empid = o.empid
					inner join entidade.entidade ent on ent.entnumcpfcnpj = ur.usucpf";
		
		$where = "	AND ur.rpustatus = 'A'
					and ur.pflcod = 948";
	}
	
	$sql = "

            WITH tmp_obras_situacao_municipal AS (
                SELECT
                    *
                FROM   obras2.vm_obras_situacao_municipal o
                WHERE
                    o.situacaoobra in (690, 691, 763, 689)
				    AND ( (o.situacaoobra IN (690, 691) AND $wherePrazo) OR (o.situacaoobra IN (763, 689) AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540) AND (o.diasprimeiropagamento is not null AND o.preid IS NOT NULL) )

            )

            SELECT DISTINCT
				ent.entnumdddcelular as ddd,
				ent.entnumcelular as celular,
				o.obrid
			$from
			where 
				ent.entstatus='A' 
				-- AND ent2.entstatus='A'
				AND ent.entnumdddcelular IS NOT NULL
				AND ent.entnumcelular IS NOT NULL
				$where
	";
 	return $db->carregar($sql);
}

function getPendenciasUF($prazo = '60', $tipo = TIPO_PREFEITO)
{
	$db = new cls_banco();

	$wherePrazo = $prazo == '60' ? ' (o.diasultimaalteracao > 60) ' : ' (o.diasultimaalteracao between 46 and 60) ';
	
	if ($tipo == TIPO_PREFEITO) {
		$from = "FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade 				fen  ON fen.entid = ent.entid AND fen.funid = 25
					INNER JOIN entidade.funentassoc 				fea  ON fea.fueid = fen.fueid
					INNER JOIN entidade.entidade 					ent2 ON ent2.entid = fea.entid
					INNER JOIN entidade.funcaoentidade 				fen2 ON fen2.entid = ent2.entid and fen2.funid = 6
					INNER JOIN entidade.endereco 					end2 on end2.entid = ent2.entid
					INNER JOIN par.instrumentounidade 				iu   ON iu.estuf = end2.estuf
					INNER JOIN territorios.estado 					est  on est.estuf = iu.estuf
					LEFT  JOIN territorios.municipio 				mun  on mun.muncod = est.muncodcapital
					INNER JOIN tmp_obras_situacao_estadual 	o    ON o.inuid = iu.inuid AND ( iu.itrid = 1 OR iu.estuf = 'DF')";
	
		$where = " AND ent2.entstatus='A' ";
	} else {
		$from = "FROM   tmp_obras_situacao_estadual o
					inner join par.instrumentounidade iu ON o.inuid = iu.inuid AND ( iu.itrid = 1 OR iu.estuf = 'DF')
					inner join obras2.usuarioresponsabilidade ur on ur.empid = o.empid
					inner join entidade.entidade ent on ent.entnumcpfcnpj = ur.usucpf";
	
		$where = "	AND ur.rpustatus = 'A'
					and ur.pflcod = 948";
	}	
	
	$sql = "

            WITH tmp_obras_situacao_estadual AS (
                SELECT
                    *
                FROM obras2.vm_obras_situacao_estadual 	o
                WHERE
                    o.situacaoobra in (690, 691, 763, 689)
				    AND ( (o.situacaoobra IN (690, 691) AND $wherePrazo) OR (o.situacaoobra IN (763, 689) AND coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540)  AND (o.diasprimeiropagamento is not null AND o.preid IS NOT NULL) )
            )

            SELECT DISTINCT
				ent.entnumdddcelular as ddd,
				ent.entnumcelular as celular,
				o.obrid
			$from
			where 
				ent.entstatus='A' 
				AND ent.entnumdddcelular IS NOT NULL
				AND ent.entnumcelular IS NOT NULL
				$where";
	return $db->carregar($sql);
}

function enviarNotificacoes($pendencias, $tipoAlerta, $conteudo)
{
	$db = new cls_banco();

    $aCelularEnvio = array();
		$aCelularEnvio = array(
			'556191434894', // Daniel
			'556193348906', // Andre
//			'556199440090', // Marcos
//			'556192191325', // Raquel
// 			'556181054537', // Orion
// 			'556181184192', // Dudu
		);
	foreach ($pendencias as $contato) {

        $GLOBALS['obras'][$contato['obrid']][$tipoAlerta][] = array('conteudo' => $conteudo);
		$aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['ddd'] . $contato['celular']);
	}
	
// 	ver($aCelularEnvio);
	
	$aCelularEnvio = array_unique($aCelularEnvio);
	
	// 		$conteudo = str_replace(
	// 			array('{PROJETO}', '{ITEM}', '{RESPONSAVEL}', '{TELEFONE}', ';'),
	// 			array($contato['atiapelido'], $contato['indapelido'], $contato['nome'], $contato['celular_executor'], '.'),
	// 			$conteudo
	// 		);
	
	// Se tiver mais que 150 caracteres, corta o nome do responsável
	// 		if(strlen($conteudo) > 150){
	// 			$conteudo = str_replace($pendencia['nome'], substr($pendencia['nome'], 0, 20), $conteudo);
	// 		}
	
	
	// Envia SMS
	$humanMultipleSend = new HumanMultipleSend("mec.gway", "WWh4O8xZOo");
	$tipo = HumanMultipleSend::TYPE_A;
	$tipo = HumanMultipleSend::TYPE_E;
	
	$data = date('Y-m-d') . ' 06:30';
	$schedule = date('d/m/Y H:i:s', strtotime($data));
	
	$aMensagem = array();
	foreach ($aCelularEnvio as $celular) {
        $celular = str_replace(array('(', ')', '-', ' ', '_', '/', '.'), array(''), $celular);
		$sql = "INSERT INTO seguranca.sms(
		sisid, smscelular, smsmensagem, smsdataenvio)
		VALUES ('147', '$celular', '$conteudo', '$data')
		returning smsid;
		";
		$id = $db->pegaUm($sql);
		$db->commit();
	
		$aMensagem[] = "{$celular};{$conteudo};$id;;$schedule";
	}
	$msg_list  = implode("\n", $aMensagem);
	
// 	ver($msg_list, d);
	
	$responses = $humanMultipleSend->sendMultipleList($tipo, $msg_list);

	//	foreach ($responses as $response) {
	//		echo $response->getCode();
	//		echo $response->getMessage();
	//	}


// 	ver($msg_list, $responses);	
	
}

function registraAtividade($obras){

    foreach($obras as $obrid => $tipos){

        $messages = (isset($tipos[TIPO_ALERTA_AMARELO])) ? $tipos[TIPO_ALERTA_AMARELO] : $tipos[TIPO_ALERTA_VERMELHO];
        $atraso = (isset($tipos[TIPO_ALERTA_AMARELO])) ? 45 : 60;

        $regAtividade = new RegistroAtividade();
        $arDado = array();

        $arDado['obrid'] = $obrid;
        $arDado['rgaautomatica'] = true;
        $arDado['rgadscsimplificada'] = 'SMS enviado (Obra a '.$atraso.' dias sem atualização)';
        $arDado['rgadsccompleta'] = 'SMS enviado (Obra a '.$atraso.' dias sem atualização) para: Gestores e Fiscais';

        $arCamposNulo = array();

        $arCamposNulo[] = 'arqid';
        $arCamposNulo[] = 'arqid';

        $rga = $regAtividade->popularDadosObjeto( $arDado )
            ->salvar(true, true, $arCamposNulo);
        $regAtividade->commit();
    }
}
?>
