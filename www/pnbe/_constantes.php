<?php

define('PNBE_MODULE_TITLE', 'PNBE - PROGRAMA NACIONAL DA BIBLIOTECA NA ESCOLA');
define('ADICIONA_TRIAGEM', 1);
define('TRIAGEM_SELECIONADA', 2);
define('REMOVE_TRIAGEM', 3);
define('OBRAS_SELECIONADAS_PREANALISE', 4);
define('OBRAS_EXCLUÍDAS_PREANALISE', 5);
define('UPLOAD_VALID_EXENSION', 'pdf');
define('DS', DIRECTORY_SEPARATOR);
define('OBRAS_NAO_AVALIADAS', 1);

if( $_SESSION['sisbaselogin'] == 'simec_desenvolvimento'){
    //PERFIS
    define('SUPER_USUARIO', 945);
    define('AVALIADOR', 950);
    define('EDITORA', 951);
    define('CONSULTA', 952);
    define('GESTOR', 949);
} else {
    //PERFIS
    define('SUPER_USUARIO', 1061);
    define('AVALIADOR', 1059);
    define('EDITORA', 1060);
    define('CONSULTA', 1062);
    define('GESTOR', 1058);
}