<?php
/**** INCLUDES ****/

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";

/**** DECLARAÇÃO DE VARIAVEIS ****/
session_start();

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '';
$_SESSION['usucpf'] 		= '';

$db = new cls_banco();

$sql = "select distinct v.emecod, v.emeano, a.autemail from 
			emenda.v_emendadetalheentidade v 
		    inner join emenda.autor a on a.autid = v.autid
		where v.emeano = '2015'
		    and v.entid is not null
		    and a.autemail is not null";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
	$conteudo = '<p><b>Senhor(a) parlamentar,</b></p>
		a indicação da emenda <b>'.$v['emecod'].'/'.$v['emeano'].'</b> foi validada no SIOP.<br>
		O próximo passo é o preenchimento, até <b>09/07/'.date(Y).'</b> no SIMEC/Emendas da iniciativa, dos dados do responsável pela elaboração do PTA e, quando se tratar de prefeitura e secretaria estadual, da vinculação da subação.<br>
		Qualquer dúvida, tratar com a ASPAR do MEC (2022-7899/7896/7894)';
	
	$remetente = array('nome' => 'SIMEC - MÓDULO EMENDAS', 'email' => 'noreply@simec.gov.br');
	$email = $v['autemail'];
	
	if( !empty($email) ){
		enviar_email($remetente, array($email), 'SIMEC - EMENDAS', $conteudo, $cc, null);
	}
}