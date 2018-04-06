<?php

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
if ( !$_SESSION['usucpf'] ) {
    header( "Location: ../login.php" );
    exit();
}

header ('Location: /gerador/gerador.php');