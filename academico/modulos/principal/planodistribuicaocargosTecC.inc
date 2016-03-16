<?

$tabela = "tabela_tecc";
$clsid = 4;

montaTabelaPlanoDistribuicao();

$tpetipo = $tpetipo = $_SESSION['academico']['tpetipo'];

if($tpetipo != ACA_TPEDITAL_PUBLICACAO){
	
	echo("<br/>
	<div style=\"text-align:right;\">
		&nbsp;
	</div>");
}else{
	$btinserir = $habilidado ? "<a href=\"javascript:void(0);\" onclick=\"listarCargos('$tabela',$clsid);\"><img src=\"/imagens/gif_inclui.gif\" style=\"cursor:pointer;\" border=\"0\" title=\"Inserir Cargos\">&nbsp;&nbsp;Inserir Cargos</a>"
							 : "";
	
	echo("<br/>
	<div style=\"text-align:right;\">
		&nbsp;&nbsp;" . $btinserir . "
	</div>");
}

?>
