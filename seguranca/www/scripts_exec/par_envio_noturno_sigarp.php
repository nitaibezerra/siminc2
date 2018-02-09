<?php

set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

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
include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' || $_SESSION['baselogin'] == 'simec_espelho_producao' ){
	define("USUARIO_SIGARP", 'MECVICTOR');
	define("SENHA_SIGARP", '27672463');
} else {
	define("USUARIO_SIGARP", 'USAP_WS_SIGARP');
//	define("SENHA_SIGARP", '97635212');
	define("SENHA_SIGARP", '03422625');
}

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 147;

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

function pegaCnpj($inuid, $prpid){
	global $db;
	$cnpj = $db->pegaUm("SELECT prpcnpj FROM par.processopar WHERE prpstatus = 'A' and prpid = ".$prpid);
	if( $cnpj ){
		return $cnpj;
	} else {
		return $db->pegaUm("SELECT iue.iuecnpj 
		                     FROM par.instrumentounidade iu
		                     inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		                     WHERE
		                     	iu.inuid = {$inuid}
		                     	and iue.iuestatus = 'A'
		                        and iue.iuedefault = true");
	}
}


$sql = "

SELECT 
	distinct dop.prpid from par.documentopar dop
	INNER JOIN  par.documentoparvalidacao dpv on dpv.dopid = dop.dopid
	INNER JOIN par.termocomposicao tc on tc.dopid = dop.dopid
	INNER JOIN par.subacaodetalhe sbd on sbd.sbdid = tc.sbdid
	inner JOIN par.subacao sba on sba.sbaid = sbd.sbaid
	INNER JOIN par.subacaoitenscomposicao ico on sbd.sbaid = ico.sbaid and ico.icoano = sbd.sbdano and icostatus = 'A'
	INNER JOIN par.propostaitemcomposicao pic on ico.picid = pic.picid
	where dpvstatus = 'A'
	and dopstatus = 'A'
	and sbastatus = 'A'
	and pic.idsigarp is not null
	and (dop.prpid, sbd.sbaid, sbd.sbdano) not in
	( 
		Select distinct epi.prpid, epi.sbaid, epi.epiano from par.empenhopregaoitensenviados epi 
	)
	and (dop.prpid, sbd.sbdid) not in (
		Select distinct prpid, sbdid from par.empenhopregaoitemperdido  
	)
	and dop.dopid not in (
		select distinct dopid from par.documentoparreprogramacao where dprstatus != 'A' 
	)
	and dop.dopid not in (
		select distinct dopid from par.documentoparreprogramacaosubacao dpr
		INNER JOIN par.reprogramacao rep ON dpr.repid = rep.repid
		where repdtfim is null	
	)


";
$result = $db->carregar($sql);

$result = ($result) ? $result : Array();
foreach($result as $k => $v)
{
	$db->executar("SELECT par.ativaenviosigarp({$v['prpid']})"); 
}
$db->commit();

$sql3 = "	select distinct prpid from par.empenhopregaoitemperdido order by prpid desc limit 300 		
 ";
$result3 = $db->carregar($sql3);
$result3 = ($result3 )? $result3 : Array();
foreach($result3 as $k => $v){
	$arrPrpIds[] = $v['prpid'];
}
$strPrpIds =  implode(", ", $arrPrpIds);


$sql2 = "	SELECT distinct ip.prpid, inuid from par.empenhopregaoitemperdido ip
			INNER JOIN par.processopar prp on ip.prpid = prp.prpid and prp.prpstatus = 'A'
			INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
			INNER JOIN par.termocomposicao tc on tc.dopid = dop.dopid
			INNER JOIN par.subacaodetalhe sbd on sbd.sbdid = tc.sbdid
			INNER JOIN par.subacaoitenscomposicao ico on sbd.sbaid = ico.sbaid and ico.icoano = sbd.sbdano and icostatus = 'A'
			INNER JOIN par.propostaitemcomposicao pic on ico.picid = pic.picid
			inner JOIN par.subacao sba on sba.sbaid = sbd.sbaid
			where
			ip.prpid in(99999999999999, {$strPrpIds}
				 )
			and pic.idsigarp is not null
			limit 300
 ";

$result2 = $db->carregar($sql2);
 
include_once(APPRAIZ."par/classes/WSSigarp.class.inc");

$oWSSigarp = new WSSigarp();

$result2 = ($result2) ? $result2 : Array();
$i = 0;
if(count($result2) == 0 )
{
	die('Sem itens para envio');
}
foreach($result2 as $k => $v)
{
	
	if( ($v['prpid']) && ($v['inuid']) )
	{
		$i++;
		$dados = array(
			'prpid' => $v['prpid'],
			'inuid' => $v['inuid']
		);
		
		$oWSSigarp->aderirPregaoRotinaNoturna($dados, 1, 'processo');
		
	}
	else 
	{
		///@todo erro
	}
	 
}
print_r($result2);
exit();
?>
