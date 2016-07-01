<?php

ini_set("memory_limit","200000M");
set_time_limit(0);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

//$sql = "select preid from obr as.ob rainfraestrutura 
//		where preid is not null 
//		and endid is null";
		
$sql = "SELECT 
			preid 
		FROM 
			obras2.obras
		WHERE 
			preid is not null 
			AND endid is null";

$preids = $db->carregarColuna( $sql );
//$count = 0;
if( is_array($preids) ){
	foreach($preids as $preid){
	
		$sql = "SELECT 
					p.predescricao || ' - ' || mun.mundescricao || ' - ' || mun.estuf as nome_obra,
					ent.entid as unidade_implantadora,
					p.precep,
					p.prelogradouro,
					p.precomplemento,
					p.prebairro,
					p.muncod,
					p.estuf,
					p.prenumero,
					p.prelatitude,
					p.prelongitude
				FROM 
					obras.preobra p
				INNER JOIN
					territorios.municipio mun on p.muncod = mun.muncod
				INNER JOIN 
					entidade.endereco ende ON ende.muncod = p.muncod
				INNER JOIN
					entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
				INNER JOIN
					entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid IN (1)
				WHERE 
					p.preid = ".$preid;
		$dadosPreObra = $db->carregar($sql);
		
		/*** Insere novo endereço da obra ***/
		$sql1 = "INSERT INTO 
					entidade.endereco (endcep,
									   endlog,
									   endcom,
									   endbai,
									   muncod,
									   estuf,
									   endnum,
									   medlatitude,
									   medlongitude,
									   endstatus)
				
				VALUES
					( '".$dadosPreObra[0]['precep']."', 
					  '".$dadosPreObra[0]['prelogradouro']."', 
					  '".$dadosPreObra[0]['precomplemento']."', 
					  '".$dadosPreObra[0]['prebairro']."', 
					  '".$dadosPreObra[0]['muncod']."', 
					  '".$dadosPreObra[0]['estuf']."', 
					  '".$dadosPreObra[0]['prenumero']."',  
					  '".$dadosPreObra[0]['prelatitude']."', 
					  '".$dadosPreObra[0]['prelongitude']."', 
					  'A' ) RETURNING endid";
		
		$endid = $db->pegaUm($sql1);
		
//		$sql2 = "UPDATE obr as.ob rainfraestrutura SET endid = ".$endid." WHERE preid = ".$preid;
		$sql2 = "UPDATE obras2.obras SET endid = ".$endid." WHERE preid = ".$preid;
		$db->executar( $sql2 );
		$db->commit();
	}
}

echo "Script executado com sucesso!";
die();



?>