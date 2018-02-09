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

ini_set( "memory_limit", "2048M" );
// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql ="SELECT preid FROM obras.preobra WHERE prestatus = 'A'";
$obras = $db->carregar($sql);


foreach($obras as $obra)
{
	$preid = $obra['preid'];
	if($preid)
	{
		$sqlValor = "
				SELECT
					CASE WHEN (pp.ptocategoria IS NOT NULL ) AND ( pp.tpoid in (104,105) )
					THEN 
					     sum(coalesce(itc2.itcvalorunitario, 0)*itc2.itcquantidade)
					ELSE 
					     sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)
					END as vlr
					  FROM obras.preobra po
					
					  INNER JOIN obras.pretipoobra pp on po.ptoid = pp.ptoid
					
					  LEFT JOIN obras.preitenscomposicaomi      itc2 ON po.ptoid   = itc2.ptoid AND itc2.itcquantidade > 0 AND po.preid = itc2.preid
					  LEFT JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itc.itcquantidade > 0   AND itc.ptoid not in (43,42, 44, 45)
					  LEFT JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid   = ppo.itcid AND ppo.preid = po.preid
					  INNER JOIN obras.pretipoobra              tpo ON tpo.ptoid   = po.ptoid
					
						 where po.preid = {$preid}
							
				  GROUP BY po.preid, po.predescricao, tpo.ptopercentualempenho, po.prevalorobra,pp.ptocategoria, pp.tpoid";
		
		$valor = $db->pegaUm($sqlValor);
		
		if( $valor )
		{
			$prevalorobra = $db->pegaUm("select prevalorobra from obras.preobra  where preid ={$preid}");
			
			if( $valor != $prevalorobra )
			{
				
				if($db->executar("update obras.preobra set prevalorobra = {$valor} WHERE preid = {$preid}"))  
				{
					$db->commit();
				}
					
			}
			
		}
		
	}
}



exit();
?>
