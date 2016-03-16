<?php
include APPRAIZ . 'includes/cabecalho.inc';  
echo "<br>";
monta_titulo(str_replace("...","",$_SESSION['mnudsc']),'&nbsp'); 

$sql = "select * from ( 
			select
				'<a target=\"_blank\"  href=\"maismedicos.php?modulo=principal/maisMedicos&acao=A&uniid=' || uni.uniid || '\" >' || uni.unisigla || ' - ' || uni.uninome || '</a>' as nome,
				mun.estuf,
				mun.mundescricao,
				count(distinct (tut.tutid)) as qtde_tut,
				count(distinct (sup.tutid)) as qtde_sup,
				count(distinct (tut.tutid)) + count(distinct (sup.tutid)) as total
			from
				maismedicos.universidade uni
			inner join
				territorios.municipio mun ON mun.muncod = uni.muncod 
			left join
				maismedicos.tutor tut ON uni.uniid = tut.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true
			left join
				maismedicos.tutor sup ON uni.uniid = sup.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true
			where
				uni.unistatus = 'A'
			group by
				uni.uniid,uni.unisigla,uni.uninome,mun.mundescricao,mun.muncod,mun.estuf
			order by
				nome ) as tbl where total > 0";

$arrCab = array("Instituição","UF","Município","Qtde. Tutores","Qtde. Supervisores","Qtde.Total");
//$db->monta_lista($sql, $arrCab, 100, 10, "S", "center", "");
$db->monta_lista_simples($sql,$arrCab,1000,1000,'S','','N', true);