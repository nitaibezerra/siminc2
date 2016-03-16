<?php
ob_start();
$controllerExecucaoFatorAvaliado = new Controller_ExecucaoFatorAvaliado();
$controllerExecucaoFatorAvaliado->downloadAction();
ob_end_flush();
exit();