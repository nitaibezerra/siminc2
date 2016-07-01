<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select ptt.ptrid from emenda.ptminreformulacao pt 
		inner join emenda.planotrabalho ptt on ptt.ptrid = pt.ptrid 
		where ptridpai is not null 
		order by pt.ptrid asc";

$arrPrtid = $db->carregar($sql);
$arrPrtidPai = array();

if($arrPrtid[0]){
	foreach($arrPrtid as $ptrid){
		$sql = "delete from emenda.ptescolasbeneficiadas  where ptrid = ".$ptrid['ptrid'];
		$db->executar($sql);
		
		$sql = "SELECT ptridpai FROM emenda.planotrabalho WHERE ptrid = ".$ptrid['ptrid'];
		$PrtidPai = $db->pegaUm($sql);
		
		$sql = "SELECT ".$ptrid['ptrid']." as ptrid, e.entid, e.esbquantidadealunos
				FROM emenda.planotrabalho p
				INNER JOIN emenda.ptescolasbeneficiadas e ON e.ptrid = p.ptrid
				WHERE p.ptrid = ".$PrtidPai;
		$escolas = $db->carregar($sql);	
		if($escolas[0]){
			foreach($escolas as $escola){
				$sql = "INSERT INTO emenda.ptescolasbeneficiadas (ptrid, entid, esbquantidadealunos) VALUES (".$escola['ptrid'].",".$escola['entid'].",".$escola['esbquantidadealunos'].")";
				$db->executar($sql);
			}
		}
		echo "Foi inserido escola para o PTRID = ".$ptrid['ptrid'];
		echo "<br>";
	}
}
$db->commit();
die();
?>