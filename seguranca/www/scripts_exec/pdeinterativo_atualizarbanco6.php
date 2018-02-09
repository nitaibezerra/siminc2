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

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select count(pe.pesid), p.pdeid, max(pe.pesid) as pesid_atual from pdeinterativo.pessoa pe
inner join pdeinterativo.pessoatipoperfil p on pe.pesid = p.pesid
where pesstatus='A' and pflcod=544 and p.tpeid=2 and pdeid is not null group by pdeid having count(pe.pesid)>1";
$pessoa = $db->carregar($sql);

if($pessoa[0]) {
	foreach($pessoa as $pe) {
		
		$pesids_antigo = $db->carregarColuna("select distinct pe.pesid from pdeinterativo.pessoa pe inner join pdeinterativo.pessoatipoperfil p on pe.pesid = p.pesid where pe.pesid!='".$pe['pesid_atual']."' and p.pdeid='".$pe['pdeid']."' and pe.pesstatus='A' and pe.pflcod=544 and p.tpeid=2");
		
		echo "select distinct pe.pesid from pdeinterativo.pessoa pe inner join pdeinterativo.pessoatipoperfil p on pe.pesid = p.pesid where pe.pesid!='".$pe['pesid_atual']."' and p.pdeid='".$pe['pdeid']."' and pe.pesstatus='A'";
		
		echo "<pre>";
		print_r($pesids_antigo);
		
		$db->executar("update pdeinterativo.pessoa set pflcod=NULL where pesid in('".implode("','",$pesids_antigo)."')");
		echo "update pdeinterativo.pessoa set pflcod=NULL where pesid in('".implode("','",$pesids_antigo)."')<br>";
		$db->executar("delete from pdeinterativo.pessoatipoperfil where tpeid=2 and pesid in('".implode("','",$pesids_antigo)."')");
		echo "delete from pdeinterativo.pessoatipoperfil where tpeid=2 and pesid in('".implode("','",$pesids_antigo)."')<br>";
		
		$db->commit();
	}
}

echo "fim";
?>