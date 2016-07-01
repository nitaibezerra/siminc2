<?

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

if($_REQUEST["selecionado"]) {
	
	$qry_combo = "SELECT
    				".$_REQUEST["coluna"]." as codigo,
    				".$_REQUEST["colunadesc"]." as descricao
    			FROM
    				".$_REQUEST["esquema"].".".$_REQUEST["tabela"];
	
    $dados_combo = $db->carregar($qry_combo);

    $vSelect = '';
  	$vSelect .= '<select id="'.$_REQUEST["id"].'" name="'.$_REQUEST["name"].'" class="campoEstilo">';
  	
  	for($k=0; $k<count($dados_combo); $k++) {
  		$selected = ($dados_combo[$k]["codigo"] == $_REQUEST["selecionado"]) ? "selected=\"selected\"" : "";
  		$vSelect .= '<option value="'.$dados_combo[$k]["codigo"].'" '.$selected.'>'.$dados_combo[$k]["descricao"].'</option>';
  	}
  	
  	$vSelect .= '</select>';
  	echo $vSelect;
    exit;
}

?>