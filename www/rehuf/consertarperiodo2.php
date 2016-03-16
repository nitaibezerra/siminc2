<?
set_time_limit(30000);
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$db = new cls_banco();

$sql = "select * from carga.conteudoitem where perid='".$_REQUEST['perid']."' and esuid='".$_REQUEST['esuid']."'";

$__dados = $db->carregar($sql);

if($__dados[0]) {
	foreach($__dados as $d) {
		$dadosest[$d['linid'].'_'.$d['perid']][] = $d; 
	}
}

if($dadosest) {
	foreach($dadosest as $key => $dd) {
		$ind = explode("_", $key);
		$sql = "select * from rehuf.linha where linid='".$ind[0]."'";
		$linha = $db->pegaLinha($sql);
		
		$ordem[$linha['gitid']]++;
		
		$sql = "INSERT INTO rehuf.linha(esuid, gitid, agpid, opcid, lindsc, linobs, linordem, linpermiteobs, linano, perid)
    			VALUES ('".$linha['esuid']."', '".$linha['gitid']."', ".(($linha['agpid'])?"'".$linha['agpid']."'":"NULL").", ".(($linha['opcid'])?"'".$linha['opcid']."'":"NULL").", 
    			'".$linha['lindsc']."', ".(($linha['linobs'])?"'".$linha['linobs']."'":"NULL").", 
    			'".$ordem[$linha['gitid']]."', ".(($linha['linpermiteobs'])?"'".$linha['linpermiteobs']."'":"NULL").", ".(($linha['linano'])?"'".$linha['linano']."'":"NULL").", '".$ind[1]."') RETURNING linid;";
		$nlinha = $db->pegaUm($sql);
		
		$nlinhas++;
		
		if($dd) {
			foreach($dd as $v) {
				$sql = "INSERT INTO rehuf.conteudoitem(ctivalor, ctiexercicio, esuid, ctistatus, linid, colid, opcid, perid)
				    	VALUES ('".$v['ctivalor']."', '".$v['ctiexercicio']."', '".$v['esuid']."', 'A', '".$nlinha."', '".$v['colid']."', ".(($v['opcid'])?"'".$v['opcid']."'":"NULL").", '".$v['perid']."');";
				$db->executar($sql);
				$nregs++;
			}
			$db->commit();
		}
		
	}
}
echo "Foram criadas ".$nlinhas." linhas e ".$nregs." registros<br>";



?>