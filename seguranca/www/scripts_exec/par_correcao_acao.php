<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
error_reporting(-1);
// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = "select count(*) as c, ppaid, a.ptoid, p.crtid, p.inuid, max(a.aciid) as aciids from par.acao a 
		inner join par.pontuacao p on p.ptoid = a.ptoid 
		inner join par.instrumentounidade u on u.inuid = p.inuid 
		inner join workflow.documento d on d.docid = u.docid 
		where a.acistatus='A' and p.ptostatus='A' and d.esdid in(313,314) group by ppaid, a.ptoid, p.crtid, p.inuid having count(*)>1 
		order by p.inuid, ppaid, a.ptoid, p.crtid";

$acao = $db->carregar($sql);

if($acao[0]) {
	foreach($acao as $aca) {
		$sql = "select s.sbaid from par.subacao s inner join par.empenhosubacao e on e.sbaid = s.sbaid where aciid in(select aciid from par.acao where ppaid=".$aca['ppaid']." and ptoid=".$aca['ptoid']."  and aciid!=".$aca['aciids'].")";
		$sbaid = $db->pegaUm($sql);
		if(!$sbaid) {
			$db->executar("delete from par.subacaodetalhe where sbaid in (select sbaid from par.subacao where aciid in(select aciid from par.acao where ppaid=".$aca['ppaid']." and ptoid=".$aca['ptoid']."  and aciid!=".$aca['aciids']."))");
			$db->executar("delete from par.subacao where aciid in(select aciid from par.acao where ppaid=".$aca['ppaid']." and ptoid=".$aca['ptoid']."  and aciid!=".$aca['aciids'].")");
			$db->executar("delete from par.acao where aciid in(select aciid from par.acao where ppaid=".$aca['ppaid']." and ptoid=".$aca['ptoid']."  and aciid!=".$aca['aciids'].")");
		}
	}
}
$db->commit();
echo "fim";
?>