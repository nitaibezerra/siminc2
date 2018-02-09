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
include_once APPRAIZ . "includes/classes/Sms.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 147;

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "
	select distinct pro.proid from par.termocompromissopac tc
	INNER JOIN par.processoobra pro ON pro.proid = tc.proid and pro.prostatus = 'A' 
	INNER JOIN par.processoobraspaccomposicao poc ON poc.proid = pro.proid and poc.pocstatus = 'A'
	INNER JOIN obras.preobra pre ON poc.preid = pre.preid
	INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
	where  
	tc.terstatus = 'A' 
	AND
		pto.ptocategoria is not null
	-- Validado
	AND 
		(tc.usucpfassinatura IS NOT NULL OR tc.terassinado = 't')
	AND pro.proid NOT IN
		( SELECT distinct proid from par.processoobraspaccomposicao poc1
		INNER JOIN par.enviomiitemperdido eip ON poc1.preid = eip.preid 
			where poc1.pocstatus = 'A'
		)
	AND
		pre.preid not in(
			select distinct preid from par.adesaoobraspac 
		)

";
$result = $db->carregar($sql);

$result = ($result) ? $result : Array();
foreach($result as $k => $v)
{
	$db->executar("SELECT par.ativaenviosigarpmi({$v['proid']})"); 
}

//$db->commit();

$sql2 = "	select distinct pcp.terid, pcp.proid from par.termocompromissopac pcp 
			WHERE pcp.proid in 
			(
				select distinct poc.proid from par.processoobraspaccomposicao poc
				where 
				poc.pocstatus = 'A' and
				poc.preid in 
				(
					select distinct preid from par.enviomiitemperdido
				)
			) order by pcp.proid
 ";
$result2 = $db->carregar($sql2);
 
include_once(APPRAIZ."par/classes/WSSigarp.class.inc");

$oWSSigarp = new WSSigarp();


$result2 = ($result2) ? $result2 : Array();
$i = 0;

foreach($result2 as $k => $v)
{
	
	//@todo chama rotina
	//$db->executar("SELECT par.ativaenviosigarpmi({$v['prpid']})");
	if( ($v['terid']) && ($v['proid']) )
	{
		$i++;
		
		
		$id = $v['terid'];
		$proid = $v['proid'];
		print_r($proid);die('asdf');
		//$oWSSigarp->solicitarManualmenteItemObra($id, $proid, 'pac');
		
	}
	else 
	{
		///@todo erro
	}
	 
}
print_r($result);
exit();
?>
