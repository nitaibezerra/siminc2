<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/entidadeFNDE.class.inc";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select * from entidade.prefnatualizadas WHERE status='A' and entid=8977 limit 20";

$prefs = $db->carregar($sql);

//8462
$obEntidade = new entidadeFNDE();
$result = $obEntidade->verificaEntidadeBaseFNDE(8462);

exit;

echo "inicio<br/>";

if($prefs[0]) {
	foreach($prefs as $p) {
		$sql = "UPDATE entidade.prefnatualizadas SET status='E' WHERE entid='".$p['entid']."'";
		$db->pegaUm($sql);
		$db->commit();
		$obEntidade = new entidadeFNDE();
		$result = $obEntidade->verificaEntidadeBaseFNDE( $p['entid'] );
		if($result) { 
			echo "Dirigente da '".$p['entnome']."' foi atualizado com sucesso<br/>";
			
			$sql = "UPDATE entidade.prefnatualizadas SET status='I' WHERE entid='".$p['entid']."'";
			$db->pegaUm($sql);
			$db->commit();
			
		}
		else echo "Erro do dirigente da '".$p['entnome']."'<br/>";
		
		
	}
	
	
	
}



echo "fim<br/>";
?>
<!-- 
<meta http-equiv="refresh" content="5;url=http://simec.mec.gov.br/emenda/atualizaprefeito.php">
 -->