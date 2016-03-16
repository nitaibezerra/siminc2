<?php

$controllerGeral = new Controller_Geral();

$action = (isset($_POST['action'])) ? $_POST['action'] : null;
switch ($action):
    case 'campoMunicipio':
        echo $controllerGeral->campoMunicipioAction();
        break;
endswitch;
?>
