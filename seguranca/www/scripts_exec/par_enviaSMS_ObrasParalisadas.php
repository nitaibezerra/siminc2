<?php
set_time_limit(0);

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
include_once APPRAIZ . "includes/classes/Sms.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 147;


/****************************************
*				PREFEITO				*
****************************************/

$conteudo = "Gestor(a), MEC informa: Há obra(s) no SIMEC paralisada(s). Regularize, sob pena de suspensão de assistência financeira.";

// Pendências do município > 60 dias
$aPendenciasDia = getObrasParalisadas();
enviarNotificacoes($aPendenciasDia, $conteudo);

//registraAtividade($obras);

echo "FIM";

function getObrasParalisadas()
{
	$db = new cls_banco();

	$sql = "SELECT DISTINCT
                ent.entnumdddcelular as ddd,
                ent.entnumcelular as celular,
                o.obrid
            FROM entidade.entidade ent
                INNER JOIN entidade.funcaoentidade     			fen  ON fen.entid  = ent.entid AND fen.funid = 2
                INNER JOIN entidade.funentassoc     			fea  ON fea.fueid  = fen.fueid
                INNER JOIN entidade.entidade         			ent2 ON ent2.entid = fea.entid
                INNER JOIN entidade.funcaoentidade     			fen2 ON fen2.entid = ent2.entid AND fen2.funid = 1
                INNER JOIN entidade.endereco         			end2 ON end2.entid = ent2.entid
                INNER JOIN territorios.municipio 			mun  ON mun.muncod = end2.muncod
                INNER JOIN par.instrumentounidade 			iu   ON iu.muncod  = mun.muncod
                INNER JOIN obras2.vm_obras_situacao_municipal 	        o    ON o.muncod   = iu.muncod AND iu.itrid <> 1 AND (iu.estuf IS NULL OR iu.estuf != 'DF')
            WHERE situacaoobra = 691 -- Paralisadas (691)
            and o.diasultimaalteracao > 45
            AND ent.entnumdddcelular IS NOT NULL
            AND ent.entnumcelular IS NOT NULL
            AND ent2.entstatus='A'
            union
            SELECT DISTINCT
                ent.entnumdddcelular as ddd,
                ent.entnumcelular as celular,
                o.obrid
            FROM entidade.entidade ent
                INNER JOIN entidade.funcaoentidade 				fen  ON fen.entid = ent.entid AND fen.funid = 25
                INNER JOIN entidade.funentassoc 				fea  ON fea.fueid = fen.fueid
                INNER JOIN entidade.entidade 					ent2 ON ent2.entid = fea.entid
                INNER JOIN entidade.funcaoentidade 				fen2 ON fen2.entid = ent2.entid and fen2.funid = 6
                INNER JOIN entidade.endereco 					end2 on end2.entid = ent2.entid
                INNER JOIN par.instrumentounidade 				iu   ON iu.estuf = end2.estuf
                INNER JOIN territorios.estado 					est  on est.estuf = iu.estuf
                LEFT  JOIN territorios.municipio 				mun  on mun.muncod = est.muncodcapital
                INNER JOIN obras2.vm_obras_situacao_estadual 	o    ON o.inuid = iu.inuid AND ( iu.itrid = 1 OR iu.estuf = 'DF')
            WHERE situacaoobra = 691 -- Paralisadas (691)
            and o.diasultimaalteracao > 45
            AND ent.entnumdddcelular IS NOT NULL
            AND ent.entnumcelular IS NOT NULL
            AND ent2.entstatus='A'
            ";
 	return $db->carregar($sql);
}

function enviarNotificacoes($pendencias, $conteudo)
{
	$db = new cls_banco();

    $aCelularEnvio = array();
		$aCelularEnvio = array(
			'556191434894', // Daniel
 			'556181054537', // Orion
// 			'556181184192', // Dudu
		);
	foreach ($pendencias as $contato) {

        $GLOBALS['obras'][$contato['obrid']][] = array('conteudo' => $conteudo);
		$aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['ddd'] . $contato['celular']);
	}
	
// 	ver($aCelularEnvio);
	
	$aCelularEnvio = array_unique($aCelularEnvio);
//    ver($aCelularEnvio, d);

    $data = date('Y-m-d') . ' 06:30:00';

    $sms = new Sms();
    $sms->enviarSms($aCelularEnvio, $conteudo, $data);
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