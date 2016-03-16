<?php 
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$memorandoRepositorio = new MemorandoRepositorio();
$tipoMemorando = $_POST['tipoMemorando'];
if ($tipoMemorando == "") {
	$memorandos = $memorandoRepositorio->recupereTodos();
} else {
	$memorandos = $memorandoRepositorio->recuperePorTipoMemorando($tipoMemorando);
}

if ($memorandos!=null){
	foreach($memorandos as $memorando) {
		$idMemorando = $memorando->getId();
		$atributoHtml = "";
		if ($memorando->getStatusMemorando()==StatusMemorando::MEMORANDO_NAO_IMPRESSO){
			$atributoHtml.="
				<a href=\"?modulo=sistema/geral/memorando/formulario&acao=A&memo=$idMemorando\">
					<img class=\"botao-editar botoes-tabela\" title=\"Editar Memorando\" src=\"/imagens/editar_nome.gif\"/>
				</a>
				<img id=\"$idMemorando\" 
					class=\"botao-excluir-memorando botoes-tabela\" 
					title=\"Excluir Memorando\" src=\"/imagens/excluir.gif\"/>";
		}
		$atributoHtml .= "
				<a href=\"?modulo=sistema/geral/memorando/visualizar&acao=A&memo=$idMemorando\">
					<img id=\"visualizar-$idMemorando\"
						class=\"botao-visualizar botoes-tabela\" title=\"Visualizar Memorando\" src=\"../imagens/consultar.gif\"/>
				</a>";
		$m = array($atributoHtml,
			utf8_encode($memorando->getStatusMemorando()=="NIMP" ? "Não emitido" : "Emitido"),
			utf8_encode($memorando->getNumeroMemorando()),
			utf8_encode($memorando->getFiscal()->getNome()),
			utf8_encode($memorando->getDataMemorando()->format('d/m/Y'))
		);
		$dados[] = $m;
	}
} else {
	$dados = array();
}
print simec_json_encode($dados);