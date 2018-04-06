<?php

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
if ( !$_SESSION['usucpf'] ) {
    header( "Location: ../login.php" );
    exit();
}

$prefixoClasse = $_GET['prefix'] ? $_GET['prefix'] : '';
$extensao = ($_GET['extension'] ? $_GET['extension'] : '.inc');

$gerador = new Gerador();
$gerador->stSchema = $schema;
$gerador->stTabela = $tables;

$gerador->gerarArquivos($prefixoClasse, $extensao);