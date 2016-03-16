<?php

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório de Adesão - Escola Ativa", "" );

$muncods = cte_pegarMunicipiosPermitidos();

if( is_array($muncods) ){
	$sql = "SELECT
				mun.estuf as uf,
				mun.muncod || ' - ' || mun.mundescricao as municipio,
				ent.entnome as escola
			FROM
				cte.escolaativaescolas ea
			INNER JOIN
				entidade.entidade     ent ON ent.entid = ea.entid
			INNER JOIN
				entidade.endereco     ende ON ende.entid = ent.entid
			INNER JOIN
				territorios.municipio mun ON mun.muncod = ende.muncod
			WHERE
				mun.muncod::integer in (".implode($muncods,",").")
			ORDER BY
				uf,municipio,escola";
}else{
	$sql = "SELECT
				'' as uf,
				'' as municipio,
				'<label style=\"color:red\">Sem município atribuido para o seu perfil.</label>' as escola ";
}

$cabecalho = array('UF','Município','Escola');

?>
<form action="" method="post" name="relatorio">
	<?php $db->monta_lista($sql,$cabecalho,500,20,'N','','N',"relatorio"); ?>
</form>