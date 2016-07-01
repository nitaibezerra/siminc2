<?php

$caminho = APPRAIZ."www/Manual_Termo_de_Execucao_Descentralizada_set14.pdf";
if (!is_file($caminho)) {
    echo"<script>
        alert('Arquivo não encontrado na pasta.');
        window.location.href = 'elabrev.php?modulo=principal/termoCooperacao/listaUG&acao=A';
    </script>";
    exit;
}

$finfo = pathinfo($caminho);
header("Content-Type: application/pdf");
header("Content-Length: ".filesize($caminho));
header("Content-Disposition: attachment; filename=".$finfo['basename']);
readfile($caminho);
exit;

?>