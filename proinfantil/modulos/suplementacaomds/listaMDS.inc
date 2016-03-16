<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<?php 
$sql = "SELECT		substr(mdsnis,1,11) as mdsnis,
					mdsnome,
					to_char(mdsdatanascimento,'DD/MM/YYYY') as dtnasc,
			       	mdsmae, 
			       	mdspai
		FROM 		proinfantil.mdsmds
		WHERE  		mdsmuncad = '{$_REQUEST['municipio']}'
		ORDER BY 	mdsnome";

$cabecalho = array('NIS', 'Nome', 'Data de Nascimento','Mãe','Pai');
$db->monta_lista($sql, $cabecalho, 100, 10, '', '', '', '');
?>