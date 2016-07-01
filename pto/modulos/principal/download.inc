<?php
ob_start();
$controllerBoletim = new Controller_Boletim();
$controllerBoletim->downloadAction();
ob_end_flush();
exit();