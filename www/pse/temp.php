<?php 
ini_set("memory_limit","20000M");

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

$sql = "select 
			foo.cenid,
			foo.entid,
			cen.cenquanteducando, 
			foo.qtdcreche,
			(cen.cenquanteducando - foo.qtdcreche) as valorfinal,
			foo.muncod
		from (
			select  cenid,
					e.entid,
				count(distinct m.fk_cod_aluno) as qtdcreche ,
				c.muncod
			from pse.censopse c 
			inner join entidade.entidade e on e.entid = c.entid
			INNER JOIN educacenso_2010.tab_matricula m on e.entcodent::character varying(8) = m.fk_cod_entidade::character varying(8)
			where  -- c.muncod in ( '3138682')
			 -- and 
			m.fk_cod_etapa_ensino = 1
			group by cenid, e.entid, c.muncod 
		) as foo 
		INNER JOIN pse.censopse cen on cen.cenid = foo.cenid
		-- where cen.muncod  in ('3138682')
		order by cen.muncod  ";

$dados = $db->carregar($sql);
$arrmuncod = array();
$executarSQL = '';
if(is_array($dados)){
	foreach($dados as $valor ){
		if($valor['valorfinal'] == 0){
			$executarSQL .= "DELETE FROM pse.censopse  WHERE cenid =  ".$valor['cenid']."; DELETE FROM pse.planoacaoc1 WHERE entid = {$valor['entid']}; DELETE FROM pse.planoacaoc2 WHERE entid = {$valor['entid']}; "; 
		}else{
			$executarSQL .= "UPDATE pse.censopse SET  cenquanteducando = ".$valor['valorfinal']." WHERE cenid =  ".$valor['cenid']."; DELETE FROM pse.planoacaoc1 WHERE entid = {$valor['entid']}; DELETE FROM pse.planoacaoc2 WHERE entid = {$valor['entid']}; "; 
		}
		if(!in_array("'".$valor['muncod']."'", $arrmuncod)){
			$arrmuncod[] = "'".$valor['muncod']."'";
		}
	}
}

$listaMuncod = implode($arrmuncod,",");

$sql = "select c.entid
		from entidade.entidade ent
		inner join entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid='7'
		inner join entidade.endereco ende  on ende.entid = ent.entid 
		inner join territorios.municipio mun on mun.muncod = ende.muncod
		inner join pse.contratualizacao c on c.entid = ent.entid 
		where ende.muncod in ({$listaMuncod})";
$arrConid = $db->carregar($sql);

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
			WHERE entid =  ".$dado['entid']."; "; 
	
}


//dbg($executarSQL,1);

if($db->executar($executarSQL)){
	echo "sucesso";
	$db->commit();
}else{
	echo "erro ao executar no banco";
}

die();




?>
