<?
require_once APPRAIZ . "monitora/www/planotrabalhoUG/_constantes.php";
require_once APPRAIZ . "monitora/www/planotrabalhoUG/_funcoes.php";

$_SESSION['monitora_var']['boMostraAbaListaUnidadeUG'] = true;

$filtro[] = "ung.ungstatus = 'A'";

$stWhere = "";
if(!$db->testa_superuser()){
	$arUngcodTemp = perfil_unidade();
	$arUngcodTemp = ($arUngcodTemp) ? $arUngcodTemp : array();
	if($arUngcodTemp[0]){
		$arUngcod = array();
		foreach($arUngcodTemp as $uniid){
			if($uniid['ungcod']){
				array_push($arUngcod,$uniid['ungcod']);								
			}
		}
		if($arUngcod && $arUngcod[0]){
			$ungcodTemp = "'" . implode("', '",$arUngcod) . "'";
			if( $ungcodTemp ){
				$stWhere = " AND ung.ungcod in(" . $ungcodTemp . ") ";
			}
		}
	}
}

$sql_lista = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&ungcod='||ung.ungcod||'\';\"></center>' as acoes, 
		CASE WHEN (ung.unicod = '26101') THEN ung.ungcod ELSE ung.unicod END AS codigo, ung.ungdsc AS descricao 
		FROM public.unidadegestora ung ".(($filtro)?"WHERE ".implode(" AND ", $filtro):"")." 
		$stWhere
		ORDER BY ung.ungcod";
		
$sql_lista2 = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&ungcod='||ung.ungcod||'\';\"></center>' as acoes, 
		ung.ungcod AS codigo, ung.ungdsc AS descricao 
		FROM public.unidadegestora ung ".(($filtro)?"WHERE ".implode(" AND ", $filtro):"")." 
		$stWhere
		ORDER BY ung.ungcod";
$arDados = $db->carregar($sql_lista2);
if(count($arDados) == 1){
	$ungcod = $arDados[0]['codigo'];
	$_SESSION['monitora_var']['boMostraAbaListaUnidadeUG'] = false;
	echo "<script>window.location='monitora.php?modulo=principal/planotrabalhoUG/subatividadesUG&acao=A&boUmaUninade=1&ungcod=$ungcod'</script>";
	exit;
}
		

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

monta_titulo( 'Gerenciar', 'Gerenciamento nas Unidades Diretas' );

$cabecalho = array("&nbsp", "Código", "Unidade");
$db->monta_lista($sql_lista,$cabecalho,100,5,'N','center',$par2);

?>
