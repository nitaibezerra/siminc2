<?php
include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

#Monta abas de restricoes
$listaMenus = buscarAbasRestricoes();
echo montarAbasArray($listaMenus, $_SERVER['REQUEST_URI']);

#Montar Titulo
monta_titulo('Tipo de Restriчѕes FNDE', '');

$titulo = "Tipo Restricao FNDE"; //Monta o tэtulo da pсgina
$schema = "par"; //informa o schema
$table = "tiporestricaofnde"; //informa a tabela
$arrOrdemColunas = array("tprdescricao"); //Ordena a exibiчуo das colunas na tela, pode ser agrupado.
$arrPermission = array(
    "inserir", //Permite inserir registros na tabela
    "alterar", //Permite alterar registros da tabela
    "excluir"  //Permite excluir registros da tabela
        // "comentar_tabela", //Permite adicionar / editar comentсrios sobre a tabela
        // "comentar_coluna", //Permite adicionar / editar comentсrios sobre a coluna da tabela
        // "campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
        //"campo_lista", //Permite definir quais os campos aparecerуo na lista dos registros da tabela,
        //"campo_tabela",
        //"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
);
$esconderCabecalho = TRUE;
include APPRAIZ . '/seguranca/modulos/sistema/tabelasApoio.inc';
?>