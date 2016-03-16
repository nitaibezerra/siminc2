<?php
// topo
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/partialsControl/topo.inc';

//caso tenha postado o formulário
if($_SERVER['REQUEST_METHOD']=="POST" && $_POST['requisicao'] == 'salvar'){
			
	//veririca se existe no banco um registro com a combinação
	//catcodclasse+ccbid+icbid, pois não poderá repetir
	$uniqueCatalogo = $oCatalogo->verificaUniqueCatalogo($_POST);
		
	//caso não encontre registro com a descrição informada
	if(!$uniqueCatalogo){
		$resultado = $oCatalogo->salvarCatalogo($_POST);
        if($resultado[0]){
        	direcionar('?modulo=sistema/tabelasdeapoio/catalogo/'.$resultado[2].'&acao=A','Operação realizada com sucesso!');
        }
        else if(!$resultado[0]){
        	direcionar('?modulo=sistema/tabelasdeapoio/catalogo/'.$resultado[2].'&acao=A','Operação falhou!');
        }
	}
	else if($uniqueCatalogo){
		alerta('Já existe registro do Catálogo informado.');
		extract($_POST);
	}
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITAR_CATALOGO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Cat&aacute;logo','');

// view
include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/views/formDefault.inc';
