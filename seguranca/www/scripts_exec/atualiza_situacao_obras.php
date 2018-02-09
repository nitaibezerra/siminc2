<?php

header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

set_time_limit( 0 );
error_reporting( E_ALL ^ E_NOTICE );

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );
ini_set( 'default_socket_timeout', '99999999' );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento


// carrega as funções gerais
//require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once "/var/www/simec/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/par/_funcoes.php";
require_once APPRAIZ . "www/par/_funcoesPar.php";
require_once APPRAIZ . "www/par/_componentes.php";
require_once APPRAIZ . "www/par/_constantes.php";
require_once APPRAIZ . "www/par/autoload.php";
include_once APPRAIZ . "par/classes/WSSigarp.class.inc";

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 23;
    
ini_set("memory_limit", "5000M");

// abre conexção com o servidor de banco de dados
$db = new cls_banco();

$oWSSigarp = new WSSigarp();

$sql = "SELECT DISTINCT preid FROM par.adesaoobraspac WHERE aopstatus = 'A'";
$obras = $db->carregarColuna($sql);
if( is_array($obras) ){
	foreach( $obras as $obra ){
		$oWSSigarp->consultarSituacaoObra($obra, 1); 
		$sql = "SELECT aoscodsituacao FROM par.adesaoobraspacsituacao WHERE preid = ".$obra." AND aoscodsituacao = 110";
		$situacaoObra = $db->pegaUm($sql);
		if( $situacaoObra ){
			$oWSSigarp->recuperarContrato($obra);
		}
	}
}

// ATUALIZA O WORKFLOW DO OBRAS 2.0
$sql = "SELECT pre.obrid, pre.preid, sts.esdid, aos.aosdtalteracao, pre.preterraplanagem, o.docid, doc.esdid as esdidatual, aos.aosid
		FROM par.adesaoobraspacsituacao aos
		INNER JOIN obras2.sigarpsituacao sts ON sts.stsid = aos.aoscodsituacao
		INNER JOIN obras.preobra pre ON pre.preid = aos.preid
		INNER JOIN obras2.obras o ON o.obrid = pre.obrid AND o.obrstatus = 'A'
		LEFT JOIN workflow.documento doc ON doc.docid = o.docid
		WHERE aoscodsituacao <> 114
		and aosprocessado is false
		ORDER BY pre.preid, aos.aosdtalteracao, sts.stsordem";
$dados = $db->carregar($sql);
if($dados){
	$esdidAnterior = 0;
	$preidAnterior = 0;
	$pulaLinha = 0;
	
	foreach($dados as $dado){
		$obrid = $dado['obrid'];
		$preid = $dado['preid'];
		$esdid = $dado['esdid'];
		$aosdtalteracao = $dado['aosdtalteracao'];
		$preterraplanagem = $dado['preterraplanagem'];
		$docid = $dado['docid'];
		$esdidatual = $dado['esdidatual'];
		$aosid = $dado['aosid'];
		$hstid = '';
		
		if(!$docid){
			$esdidatual=870;
			$docdsc = "Fluxo de obra do módulo Obras II - obrid ".$obrid;
			$sql = "INSERT INTO workflow.documento(tpdid, esdid, docdsc) VALUES (105, ". $esdidatual. ", '". $docdsc ."') RETURNING docid";
			$docid = $db->pegaUm($sql);
			
			$sql = "UPDATE obras2.obras SET docid = ".$docid." WHERE obrid = " .$obrid;
			$db->executar($sql);
		}
		
		if($preid <> $preidAnterior){
			$esdidAnterior = $esdidatual;
			$pulaLinha = 0;
		}else{
			if($esdidAnterior == $esdid){
				$pulaLinha = 1;
			}else{
				$pulaLinha = 0;
			}
		}

		if($pulaLinha==0){
			if($preterraplanagem=='t' && $esdid == 864){
				$esdid = 872;
			}

			if($esdidAnterior <> $esdid){
				$sql = "SELECT aedid
					FROM workflow.acaoestadodoc
					WHERE esdidorigem = " . $esdidAnterior . " AND esdiddestino = " . $esdid;
				$aedid = $db->pegaUm($sql);
				if($aedid){
					$sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, pflcod, htddata)
							VALUES (".$aedid.", ".$docid.", '00000000191', 932, '".$aosdtalteracao."') RETURNING hstid";
					$hstid = $db->pegaUm($sql);
				}else{
					$sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada, aedvisivel)
							VALUES (".$esdidAnterior.", ".$esdid.", 'Enviar para', 'Enviado para', 'f') RETURNING aedid";
					$aedid = $db->pegaUm($sql);
					
					$sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, pflcod, htddata)
							VALUES (".$aedid.", ".$docid.", '00000000191', 932, '".$aosdtalteracao."') RETURNING hstid";
					$hstid = $db->pegaUm($sql);
				}

				if($hstid){
					$sql = "UPDATE workflow.documento SET esdid=".$esdid.", hstid=".$hstid." WHERE docid = " . $docid;
					$db->executar($sql);
				}else{
					$sql = "UPDATE workflow.documento SET esdid=".$esdid." WHERE docid = ". $docid;
					$db->executar($sql);
				}
			}
		}

		$sql = "UPDATE par.adesaoobraspacsituacao SET aosprocessado='t' WHERE aosid = ".$aosid;
		$db->executar($sql);

		$esdidAnterior = $esdid;
		$preidAnterior = $preid;
	}
	$db->commit();
}

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "WS Atualizar Obras a partir do SIGARP";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
$mensagem->Subject = "WS Atualizar Obras a partir do SIGARP (atualiza_situacao_obras.php)";

$mensagem->Body = $corpoemail;
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

echo "fim";

?>