<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();

$sql = "
		SELECT
			crstitulo
		FROM  
		 cte.instrumentounidadeescola iue
		 JOIN entidade.entidade e ON e.entid = iue.entid
		 JOIN entidade.endereco ende ON ende.entid = e.entid 
		 JOIN territorios.municipio mu ON mu.muncod = ende.muncod AND mu.estuf = ende.estuf 
		 JOIN territorios.estado ON estado.estuf = mu.estuf
		 JOIN territorios.regiao ON regiao.regcod = estado.regcod
		 JOIN territorios.pais ON pais.paiid = regiao.paiid
		 JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
		 LEFT JOIN entidade.entidadedetalhe edd ON e.entid = edd.entid AND  e.entcodent = edd.entcodent 
			AND (
				entdreg_medio_prof = '1' OR 
				entdreg_medio_medio = '1' OR 
				entdreg_medio_normal = '1' OR
				entdreg_medio_integrado = '1' 
			    )
		 LEFT JOIN cte.conteudoppp cpp ON cpp.entid = e.entid
		 LEFT JOIN cte.conteudopppcursotecnico cont ON cpp.cppid = cont.cppid
		 LEFT JOIN cte.cursotecnico ct ON cont.crsid =  ct.crsid
		 ".str_replace("\'","'",$_REQUEST[where])." AND
		 e.entid = ".$_REQUEST[entid]." AND crstitulo IS NOT NULL";
$dado = $db->carregar($sql);

$table = "<table width='100%' cellpadding='1' cellspacing='1'>";
$i = 0;
if (!empty($dado) && is_array($dado)):
	foreach ($dado as $dado):
		
		$table .= "<tr style='background-color: ".(($i%2)?'#F9F9F9':'#DFDFDF').";'>
					<td style='padding-left:" . (int)($_REQUEST[prof]+1) * 20 ."px;'><img src='../imagens/seta_filho.gif' align='absmiddle'/> <b>{$dado[crstitulo]}</b></td>
				   </tr>";
		$i++;
	endforeach;
else:
	$table .= "<tr>
				  <td style='color:red;' align='center'>Nenhum registro foi encontrado.</td>	
			   </tr>";
endif;
$table .= "</table>";

die($table);
?>