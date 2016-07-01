<?php

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "www/pdeinterativo/_funcoesplanoestrategico_1.php";

if ( !$db )
{
	$db = new cls_banco();
}

//$escolas = array(52023494);

$sql = "select pdicodinep from pdeinterativo.pdinterativo p 
inner join workflow.documento d on d.docid=p.docid 
where d.tpdid=43 and esdid=310 and (pdiretornofnde is null or pdiretornofnde=false)";

$escolas = $db->carregarColuna($sql);


for($i=0;$i<count($escolas);$i++){

	$_SESSION["pdeinterativo_vars"]["pdicodinep"] = $escolas[$i]; 
	$teste = validarPdeInterativo();
	
	echo $i .' - '. $_SESSION["pdeinterativo_vars"]["pdicodinep"] .' = '. $teste.'<br>';
		
}

echo '<BR><BR>FIMMMMMMMMMMMMMMMMMMMMMMMMMM';


?>