<?php 
ini_set("memory_limit","20000M");

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

$sql = "select 
			c.muncod, 
			count(c.entid) as quantidadeescolas, 
			sum(c.cenquanteducando) as quantidadeestudantes
		from pse.censopse c
		group by c.muncod";

$dados = $db->carregar($sql);

$executarSQL = '';
$muncod = '';
$n = 0;

if(is_array($dados)){
	foreach($dados as $dado ){
		$sql = "select c.entid
				from entidade.entidade ent
				inner join entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid='7'
				inner join entidade.endereco ende  on ende.entid = ent.entid 
				inner join territorios.municipio mun on mun.muncod = ende.muncod
				inner join pse.contratualizacao c on c.entid = ent.entid 
				where ende.muncod in ('{$dado['muncod']}')";
		
		$entid = $db->pegaUm( $sql );

		if( $entid ){
			$executarSQL .= "UPDATE pse.contratualizacao SET  arqid = null,
								conquantescolapub = {$dado['quantidadeescolas']}, 
								conquanteducando = {$dado['quantidadeestudantes']} ,
								conesfatuarapse = 0 ,
								concoberturacomp1 = 0,
								concoberturacomp2 = 0, 
								conpercentualcomp1 = 0 ,
								conpercentualcomp2 = 0 
							WHERE entid =  ".$entid."; "; 
		}
	}
}

if($db->executar($executarSQL)){
	echo "sucesso 3";
	$db->commit();
}else{
	echo "erro ao executar no banco 3";
}

die();




?>
