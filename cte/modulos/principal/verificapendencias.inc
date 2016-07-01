<?php

$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$estadoDocumento    = $estadoDocumento['esdid'];

if ($estadoDocumento != CTE_ESTADO_DIAGNOSTICO ||
    $estadoDocumento != CTE_ESTADO_PAR)
{
    $errossub = verifica_erros_preenchimento($_SESSION['inuid']);

    if (!cte_exibeErrosSubAcao($errossub)) {
        echo '<script type="text/javascript">'
            ,'alert("Não há pendências nas subações.");'
            ,'window.opener.location.reload();'
            ,'window.close();'
            ,'</script>';
    }
}



