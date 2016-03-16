<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();

$sql = "
		SELECT
		 count(ct.crsid) AS tot_curso, 
		 e.entnome,
		 e.entid
		FROM  
		 cte.instrumentounidadeescola iue
		 JOIN entidade.entidade e ON e.entid = iue.entid
		 JOIN entidade.endereco ende ON ende.entid = e.entid 
		 JOIN territorios.municipio mu ON mu.muncod = ende.muncod AND mu.estuf = ende.estuf
		 JOIN territorios.estado ON estado.estuf = mu.estuf
		 JOIN territorios.regiao ON regiao.regcod = estado.regcod
		 JOIN territorios.pais ON pais.paiid = regiao.paiid
		 JOIN territorios.mesoregiao mes ON mes.estuf = estado.estuf AND mes.mescod = mu.mescod
		 LEFT JOIN entidade.entidadedetalhe edd ON e.entid = edd.entid AND edd.entcodent = e.entcodent
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
		 mu.muncod = '".$_REQUEST[muncod]."'
		GROUP BY
		 e.entnome,
		 e.entid
		ORDER BY
		 tot_curso DESC, e.entnome";

$dado = $db->carregar($sql);

$table = "
<table width='100%' cellpadding='1' cellspacing='1'>
<!--	<tr style='background-color: #ccc; font-weight: bold;'>
		<td width='84%' height='20;'>
			Escola
		</td>
		<td width='8%'>
			&nbsp;
		</td>
		<td width='8%'>
			Qtd. de Cursos
		</td>
	</tr> -->";
$i = 0;
if ($dado && is_array($dado)):
	foreach ($dado as $dado):
		
		$img = $dado[tot_curso] ? '<img src="../imagens/mais.gif" align="absmiddle" onclick="detalharEscola( '.$dado[entid].','.($_REQUEST[prof]+1).' );" id="img_'.$dado[entid].'" onmouseover="this.style.cursor = \'pointer\';"/>': '';
		
		$table .= "<tr style='background-color: ".(($i%2)?'#DFDFDF':'#F9F9F9').";'>
					<td width='505' style='padding-left:" . (int)($_REQUEST[prof]+1) * 20 ."px;'><img src='../imagens/seta_filho.gif' align='absmiddle'/> {$img} <b>{$dado[entnome]}</b></td>
					<td width='75'>&nbsp;</td>
					<td width='75' align='right'>{$dado[tot_curso]}</td>
				   </tr>		
		";
		$i++;
		$table .= $dado[tot_curso] ? '<tr style="display:none;" id="tr_'.$dado[entid].'"><td colspan="14" id="td_'.$dado[entid].'">... carregando</td></tr>' : '';
	endforeach;
else:
	$table .= "<tr>
				  <td style='color:red;' align='center'>Nenhum registro foi encontrado.</td>	
			   </tr>";
endif;
$table .= "</table>";

echo $table;
?>