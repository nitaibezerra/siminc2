<?php

$prefixoClasse = $_GET['prefix'] ? $_GET['prefix'] : '';
$extensao = ($_GET['extension'] ? $_GET['extension'] : '.inc');

$gerador = new Gerador();
$gerador->stSchema = $schema;
$gerador->stTabela = $tables;

$gerador->gerarArquivos($prefixoClasse, $extensao);