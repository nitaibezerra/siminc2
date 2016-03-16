<?php
function __autoload($class)
{
    $path = APPRAIZ;
    $ds = DIRECTORY_SEPARATOR;
    $componentes = explode('_', $class);
    $arquivo = array_pop($componentes);
    $componentes = array_map('strtolower', $componentes);

    switch ($arquivo) {
        case 'Modelo':
            require_once("{$path}{$ds}includes{$ds}classes{$ds}Modelo.class.inc");
            return;
        case 'FilesSimec':
            require_once("{$path}{$ds}includes{$ds}classes{$ds}fileSimec.class.inc");
            return;
        case 'Grafico':
            require_once("{$path}{$ds}includes{$ds}library{$ds}simec{$ds}Grafico.php");
            return;
        case 'FlashMessage':
            require_once("{$path}{$ds}includes{$ds}library{$ds}simec{$ds}Helper{$ds}FlashMessage.php");
            return;
        case 'DML':
            require_once "{$path}{$ds}includes{$ds}library{$ds}simec{$ds}DB{$ds}{$arquivo}.php";
            return;
    }

    $modulo = array_shift($componentes);
    
    switch ($modulo) {
        case 'simec':
            $path .= "includes{$ds}library{$ds}simec{$ds}";
            break;
        default:
            $path .= "{$modulo}{$ds}classes{$ds}";
    }

    foreach ($componentes as $_path) {
        $path .= "{$_path}{$ds}";
    }

    $path .= $arquivo;

    if (is_file("{$path}.class.inc")) {
        $path .= '.class.inc';
    } elseif (is_file("{$path}.inc")) {
        $path .= '.inc';
    } elseif (is_file("{$path}.php")) {
        $path .= '.php';
    } else {
        $path = '';
    }

    if (!empty($path)) {
        require_once($path);
    }
}