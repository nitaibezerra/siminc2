<?php

if ($this->view->noContrato) {
    include_once 'formulario.php';
} else {
    include_once 'formulario_item.php';
}