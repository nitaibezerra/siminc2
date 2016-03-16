<?php 
ini_set("memory_limit","20000M");

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

$sql = "select 
			c.cenid, c.entid, pa1.conid as conid1, pa2.conid as conid2, c.muncod
		from entidade.entidade ent
		inner join entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid='4'
		left join pse.planoacaoc1 pa1 ON ent.entid = pa1.entid
		left join pse.planoacaoc2 pa2 ON ent.entid = pa2.entid
		inner join pse.censopse c ON c.entid = ent.entid";

$dados = $db->carregar($sql);
$arrmuncod = array();
$arrConid = array();
$executarSQL = '';
if(is_array($dados)){
	foreach($dados as $valor ){
		//if($valor['valorfinal'] == 0){
			$executarSQL .= "DELETE FROM pse.censopse  WHERE cenid =  ".$valor['cenid']."; 
							DELETE FROM pse.planoacaoc1 WHERE entid = {$valor['entid']}; 
							DELETE FROM pse.planoacaoc2 WHERE entid = {$valor['entid']}; "; 
		//}else{
		//	$executarSQL .= "UPDATE pse.censopse SET  cenquanteducando = ".$valor['valorfinal']." WHERE cenid =  ".$valor['cenid']."; DELETE FROM pse.planoacaoc1 WHERE entid = {$valor['entid']}; DELETE FROM pse.planoacaoc2 WHERE entid = {$valor['entid']}; "; 
		//}
		if( $valor['conid1'] ){
			if( !in_array($valor['conid1'], $arrConid) ){
				$arrConid[] = $valor['conid1'];
			} 
		}
		if( $valor['conid2'] ){
			if( !in_array($valor['conid2'], $arrConid) ){
				$arrConid[] = $valor['conid2'];
			} 
		}
		if( !in_array("'".$valor['muncod']."'", $arrmuncod) ){
			$arrmuncod[] = "'".$valor['muncod']."'";
		}
	}
}

$listaMuncod = implode($arrmuncod,",");

if( is_array($listaMuncod) ){
	$sql = "select c.entid
			from entidade.entidade ent
			inner join entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid='7'
			inner join entidade.endereco ende  on ende.entid = ent.entid 
			inner join territorios.municipio mun on mun.muncod = ende.muncod
			inner join pse.contratualizacao c on c.entid = ent.entid 
			where ende.muncod in ({$listaMuncod})";
	
	$arrEntid = $db->carregar($sql);
}

if( is_array($arrConid) ){
	foreach($arrConid as $dado){
		$executarSQL .= "UPDATE pse.contratualizacao SET  arqid = null,
												conquantesfmun = 0 ,
												conquantescolapub = 0, 
												conquanteducando = 0 ,
												conesfatuarapse = 0 ,
												concoberturacomp1 = 0,
												concoberturacomp2 = 0, 
												conpercentualcomp1 = 0 ,
												conpercentualcomp2 = 0 
				WHERE conid =  ".$dado."; "; 
		
	}
}

if( is_array($arrEntid) ){
	foreach($arrEntid as $dado){
		$executarSQL .= "UPDATE pse.contratualizacao SET  arqid = null,
												conquantesfmun = 0 ,
												conquantescolapub = 0, 
												conquanteducando = 0 ,
												conesfatuarapse = 0 ,
												concoberturacomp1 = 0,
												concoberturacomp2 = 0, 
												conpercentualcomp1 = 0 ,
												conpercentualcomp2 = 0 
				WHERE entid =  ".$dado['entid']."; "; 
		
	}
}

//dbg($executarSQL,1);

if($db->executar($executarSQL)){
	echo "sucesso 2";
	$db->commit();
}else{
	echo "erro ao executar no banco 2";
}

die();




?>
