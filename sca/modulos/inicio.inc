<?php

// definindo redirecionamento
switch ($_SESSION['usunivel']){
    // super usuario
    case 1:
        $link = 'registrarEntradaVisitante';
        break;
    // gerente de area
    case 2:
        $link = 'autorizarAcessoForaHorario';
        break;
    // recepcionista
    case 3:
        $link = 'localTrabalho';
        break;
    // outros
    default:
        $link = 'registrarEntradaVisitante';
}

// redirecionando
header("Location: ?modulo=principal/visitante/$link&acao=A");

?>