<?php
$enbid = $_REQUEST['enbid'];
$ptrid = $_REQUEST['ptrid'];

$obEntidade = new entidadeFNDE();
$obEntidade->verificaEntidadeBaseFNDE( $enbid, $ptrid );

/*$sql = "SELECT 
		    fuc.funid,
		    ent.entid
		FROM
			entidade.entidade ent
		    inner join entidade.funcaoentidade fue
		    	on fue.entid = ent.entid
		    inner join entidade.funcao fuc
		    	on fuc.funid = fue.funid
		    inner join entidade.funentassoc fea
		    	on fea.fueid = fue.fueid
		WHERE
			fea.entid = ".$entid;

$arDados = $db->pegaLinha( $sql );

if( !empty( $arDados ) ){
	$carid = $db->pegaUm("SELECT carid FROM emenda.cargos WHERE funid = ".$arDados['funid']);
	
	if( !empty( $carid ) ){
		$sql = "UPDATE emenda.planotrabalho SET carid =	".$carid.", entiddirigente = '".$arDados['entid']."' WHERE ptrid = ".$ptrid;
		$db->executar( $sql );
		$db->commit();
	}
}*/

echo "<script type=\"text/javascript\">
		alert('Dados da entidade e do dirigente atualizados com sucesso');
		window.opener.location.reload(); 
		window.close();
	</script>";
exit();	

?>
