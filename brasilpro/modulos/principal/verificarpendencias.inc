<?php

$errossub = verifica_erros_preenchimento($_SESSION['inuid']);

if (!cte_exibeErrosSubAcao($errossub)) {
	echo '<script type="text/javascript">'
		,'alert("Não há pendências nas subações.");'
		,'window.opener.location.reload();'
		,'window.close();'
		,'</script>';
}
