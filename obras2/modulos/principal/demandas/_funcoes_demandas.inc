<?php

/**
 * ARQUIVO DE COMPATIBILIDADE COM AS DIFERENCAS DO DEMANDAS DO PAR
 * MANTER AS NOMECLATURAS PARA FACILITAR FUTURAS ATUALIZAÇÕES
 */


define('WF_TPDID_DEMANDA', 147);

/*
 * INICIO de estados do documento da demanda
 * _____________________________________________________________________________
 */

define("PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO", 933);# Envia para AGUARDANDO_ATENDIMENTO
define("PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO", 934); # Envia para ANALISE_DE_ATENDIMENTO
define("PAR_WF_DEMANDA_ESTADO_EM_ANALISE_DE_ATENDIMENTO", 935); # Envia para ATENDIMENTO ou EM_DILIGENCIA
define("PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA", 936); # Volta para ANALISE_DE_ATENDIMENTO
define("PAR_WF_DEMANDA_ESTADO_EM_ATENDIMENTO", 937); # Envia para EM_DILIGENCIA ou EM_PAUSA ou AGUARDANDO_VALIDACAO
define("PAR_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO", 938); # Envia para ESTADO_CONCLUIDO
define("PAR_WF_DEMANDA_ESTADO_CONCLUIDO", 939); # Nao pode mais fazer nada.
define("PAR_WF_DEMANDA_ESTADO_EM_PAUSA", 940);# Volta para EM_ATENDIMENTO

define("PAR_PERFIL_DEMANDANTE_PAR", 1342);
define("PAR_PERFIL_SUPER_USUARIO", 932);
define("PAR_PERFIL_GESTOR_MEC", 944);

function pegaArrayPerfil($usucpf)
{
    global $db;

    $sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";

    $pflcod = $db->carregar($sql);

    if (is_array($pflcod)) {
        foreach ($pflcod as $dados) {
            $arPflcod[] = $dados['pflcod'];
        }
    }
    return (($arPflcod) ? $arPflcod : array());
}

function atualizaPrioridadeDemanda( $dmdprioridade, $dmtid, $tiraPrioridade = false ){

    global $db;

    if( $tiraPrioridade ){
        $operador = '-';
        $operadorWhere = ">";
    }else{
        $operador = '+';
        $operadorWhere = ">=";
    }

    $arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
        PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
        PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

    $sql = "SELECT
				dmdid,
				esdid,
				dmdprioridade
			FROM
				obras2.demandas dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmdstatus = 'A'
				AND dmdprioridade IS NOT NULL
				AND dmdprioridade $operadorWhere $dmdprioridade
				--AND dmtid = $dmtid
			ORDER BY
				dmdprioridade";

    $demandas = $db->carregar($sql);

    if( is_array($demandas) ){
        $sql = "";
        foreach( $demandas as $k => $demanda ){
            if( !in_array($demanda['esdid'], $arrEstados) ){
                continue;
            }
            $interacao = ajustaProxDemandaAtendimento( $demandas, $k+1 );
            $sql .= "UPDATE obras2.demandas SET
						dmdprioridade = dmdprioridade $operador $interacao
					WHERE
						dmdid = {$demanda['dmdid']};";
        }
        if( $sql != '' ){
            $db->executar($sql);
            $db->commit();
        }
    }

}

function buscaDadosSolicitanteDemandas($usucpf, $retorno = null)
{
    global $db;

    $sqlSolicitante = "
        SELECT
            usucpf,
            usunome,
            usuemail
        FROM
            seguranca.usuario
        WHERE
            usucpf = '{$usucpf}'
    ";

    $dadosSolicitante = $db->pegaLinha($sqlSolicitante);

    if(!is_null($retorno)) {
        return $dadosSolicitante[$retorno];
    } else {
        return $dadosSolicitante;
    }

}

function tiraFuroOrdem($dmtid)
{
    global $db;

    $sql = "SELECT
				dmdid
			FROM
				obras2.demandas
			WHERE
				dmdprioridade IS NOT NULL
				AND dmdstatus = 'A'
				--AND dmtid = $dmtid
			ORDER BY
				dmdprioridade,
				dmdultimaalteracao DESC";
    $dmdids = $db->carregarColuna($sql);

    if (is_array($dmdids) && !empty($dmdids)) {
        $sql = "";
        foreach ($dmdids as $k => $dmdid) {
            $sql .= "UPDATE obras2.demandas SET dmdprioridade = " . ($k + 1) . " WHERE dmdid = $dmdid;";
        }
        $db->executar($sql);
        $db->commit();
    }
}


function pegaPrioridadeMaxima(){

    global $db;

    $sql = "
	    SELECT
            MAX(dmdprioridade) as prioridade
        FROM
            obras2.demandas
        WHERE
            dmdstatus = 'A';
    ";

    $prioridade = $db->pegaUm($sql);



    if( $prioridade == '' ){
        $prioridade = 0;
    }
    echo $prioridade;
    return $prioridade;
}


function pegaDocidDemanda( $dmdid ){
    global $db;

    require_once APPRAIZ . 'includes/workflow.php';

    // descrição do documento
    $docdsc = "Fluxo de demandas do PAR - dmdid " . $dmdid;

    // cria documento do WORKFLOW
    $docid = wf_cadastrarDocumento( WF_TPDID_DEMANDA, $docdsc );

    // atualiza o DOCID na OBRA
    $sql = "UPDATE obras2.demandas SET
				docid = $docid
			WHERE
				dmdid = $dmdid";
    $db->executar($sql);
    $db->commit();

    return $docid;
}

function ajustaProxDemandaAtendimento( $demandas, $k ){

    global $db;

    $arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
        PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
        PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

    if( !in_array($demandas[$k]['esdid'], $arrEstados) && $demandas[$k]['esdid'] != '' ){
        return ajustaProxDemandaAtendimento( $demandas, $k+1 )+1;
    }else{
        return '1';
    }
}


function pegaPrioridadeMinimaSemAtendimento(){

    global $db;

    $arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
        PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
        PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

    $sql = "SELECT
				min(dmdprioridade) as prioridade
			FROM
				obras2.demandas dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmtid = {$_REQUEST['dmtid']}
				AND dmdprioridade > {$_REQUEST['prioridadeInvalida']}
				AND dmdstatus = 'A'
				AND esdid IN (".implode(', ',$arrEstados).")";

    $prioridade = $db->pegaUm($sql);

    if( $prioridade == '' ){

        $sql = "SELECT
					max(dmdprioridade)+1 as prioridade
				FROM
					obras2.demandas dmd
				INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
				WHERE
					dmtid = {$_REQUEST['dmtid']}
					AND dmdstatus = 'A'
					AND esdid NOT IN (".implode(', ',$arrEstados).")";

        $prioridade = $db->pegaUm($sql);

        if( $prioridade == '' ){
            $prioridade = 0;
        }
    }
    echo $prioridade;
    return $prioridade;
}

function pegaPrioridadeMinima(){

    global $db;

    $arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
        PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
        PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

    $sql = "SELECT
				min(dmdprioridade) as prioridade
			FROM
				obras2.demandas dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmtid = {$_REQUEST['dmtid']}
				AND dmdstatus = 'A'
				AND esdid IN (".implode(', ',$arrEstados).")";

    $prioridade = $db->pegaUm($sql);

    if( $prioridade == '' ){

        $sql = "SELECT
					max(dmdprioridade)+1 as prioridade
				FROM
					obras2.demandas dmd
				INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
				WHERE
					dmtid = {$_REQUEST['dmtid']}
					AND dmdstatus = 'A'
					AND esdid NOT IN (".implode(', ',$arrEstados).")";

        $prioridade = $db->pegaUm($sql);

        if( $prioridade == '' ){
            $prioridade = 0;
        }
    }
    echo $prioridade;
    return $prioridade;
}

function testaAtendimentoDemandaPorPrioridade(){

    global $db;

    $arrEstados = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO,
        PAR_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO,
        PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

    $sql = "SELECT
				true
			FROM
				obras2.demandas dmd
			INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
			WHERE
				dmtid = {$_REQUEST['dmtid']}
				AND dmdprioridade = {$_REQUEST['dmdprioridade']}
				AND dmdstatus = 'A'
				AND esdid NOT IN (".implode(', ',$arrEstados).")";

    $atendimento = $db->pegaUm($sql);

    echo $atendimento;
    return $atendimento;
}

function atualizaEfeitoPriorizacao($prioridade, $hpdid, $dmdid){

    global $db;

    $sql = "SELECT dmdid FROM obras2.demandas WHERE dmdprioridade >= $prioridade OR dmdid = $dmdid";

    $dmdids = $db->carregarColuna($sql);

    $sql = "";
    if( is_array($dmdids) ){
        foreach( $dmdids as $dmdid ){
            $sql .= "INSERT INTO obras2.demandas_priorizacao_demandas(dmdid, hpdid)
					 VALUES ($dmdid, $hpdid);";
        }
    }
    if( $sql != "" ){
        $db->executar($sql);
        $db->commit();
    }
}

function enviarEmailNotificacao($dados, $dadosSolicitante, $dmdid = '', $arquivo = '')
{

    global $db;

    $tipo = $db->pegaUm("
        SELECT
        	dmtnome
        FROM
        	obras2.demandastipo
        WHERE
            dmtid = {$dados['dmtid']}
    ");

    $origem = $db->pegaUm("
        SELECT
        	dmonome
        FROM
        	obras2.demandas_origem
        WHERE
        	dmoid = {$dados['dmoid']};
    ");

    if($dados['dmdid']) {
        $dmdid = $dados['dmdid'];
    } else if ($dmdid) {
        $dmdid = $dmdid;
    } else {
        $dmdid = '';
    }

    $remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");

    $destinatarios = array($dadosSolicitante['usuemail']);

    $assunto = "[SIMEC-OBRAS@] #{$dmdid} - {$dados['dmdnome']}";

    $mensagem = "
        <p>
            Prezado(a),
        </p>
        <p>
            <b>ID #:</b> {$dmdid}
            <br/><b>Assunto:</b> {$dados['dmdnome']}
            <br/><b>Tipo: </b> {$tipo}
            <br/><b>Origem: </b> {$origem}
            <br/><b>Prioridade: </b> {$dados['dmdprioridade']}
            <br/><b>Urgente: </b> " . (($dados['dmdurgente'] == 'TRUE') ? 'Sim' : 'Não') . "
            <br/><b>Descrição detalhada: </b> {$dados['dmddescricao']}
            <br/><b>Solicitante: </b> " . ucwords(strtolower($dadosSolicitante['usunome'])) . "
            <br/><b>Arquivos: </b>  {$arquivo}
        </p>
        <p>
            Atenciosamente,
            <br/>
            Equipe SIMEC.
        </p>
        <p>
            <small>
                Notificação enviada automaticamente pelo sistema para informar o cadastro de sua solicitação.
                <br/>Por favor, não responda esse e-mail.
            </small>
        </p>
    ";

    return enviar_email($remetente, $destinatarios, $assunto, $mensagem);
}