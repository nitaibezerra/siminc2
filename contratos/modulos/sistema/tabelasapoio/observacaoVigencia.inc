<?php
$schema = "contratos"; //informa o schema
$table = "ctobsvigencia"; //informa a tabela
$showStatus = false; //informa a tabela
$hideWarning = true; //esconde os alertas, erros e informaчѕes de pk, fk, mapeamento de tabelas, etc.
$arrPermission = array(
		"inserir", //Permite inserir registros na tabela
		"alterar", //Permite alterar registros da tabela
		"excluir", //Permite excluir registros da tabela
		//								"comentar_tabela", //Permite adicionar / editar comentсrios sobre a tabela
//								"comentar_coluna", //Permite adicionar / editar comentсrios sobre a coluna da tabela
//								"campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
//								"campo_lista", //Permite definir quais os campos aparecerуo na lista dos registros da tabela
//								"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
);
//Regras para antes de salvar
function antesSalvar()
{
	global $db;
	$retorno = false;
	$sql = "SELECT count(*) FROM contratos.ctobsvigencia WHERE obvdsc='".$_POST["obvdsc"]."'";
	$result = $db->pegaUm($sql);
	//Nуo podem existir itens com o mesmo nome
	if($result > 0){ 
		return "Jс existe um registro com essa descriчуo."; //Retorna uma mensagem para o usuсrio
		$retorno = false;
	}else{
		$retorno = true;
	}
	//O sistema nуo deve permitir alterar uma observaчуo de vigъncia que jс esteja vinculada a um contrato.
	if ($_POST["obvid"]){
		$sql = "SELECT count(*) FROM contratos.ctcontrato WHERE obvid='".$_POST["obvid"]."'";
		$existe = $db->pegaUm($sql);
		if($existe>0){
			return "Nуo щ permitido alterar uma observaчуo de vigъncia que jс esteja vinculada a um contrato";
		}else{
			$retorno = true;
		}	
	}
	return $retorno;
}

//Regras para antes de salvar
function antesExcluir()
{
	global $db;
	//O sistema nуo deve permitir excluir uma observaчуo de vigъncia que jс esteja vinculada a um contrato.
	if ($_POST["obvid"]){
		$sql = "SELECT count(*) FROM contratos.ctcontrato WHERE obvid='".$_POST["obvid"]."'";
		$existe = $db->pegaUm($sql);
		if($existe>0){
			return "Nуo щ permitido excluir uma observaчуo de vigъncia que jс esteja vinculada a um contrato";
		}else{
			return true;
		}
	}
	return true;

}
include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>