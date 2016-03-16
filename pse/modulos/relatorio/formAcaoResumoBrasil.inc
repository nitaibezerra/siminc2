<?php

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Monitoramento de Ações - Resumo Brasil', '&nbsp;' );


$sql = "select 'Componente I', 
				acaodescricao, 
				qtdeescola, 
				moetotalpactuado as avaliacoespactuadas, 
				sum(avleducandoavaliado) as avaliacoesrealizadas, 
				(case when moetotalpactuado > 0 then
					(COALESCE(sum(avleducandoavaliado),0) * 100) / moetotalpactuado
				      else
					0
				end) as porcentagemrealizadas,
				sum(avlidentifica1) as problemasidentificados,
				(case when COALESCE(sum(avleducandoavaliado),0) > 0 then
					(COALESCE(sum(avlidentifica1),0) * 100) / COALESCE(sum(avleducandoavaliado),0)
				      else
					0
				end) as porcentagemproblemas
			from pse.avaliacao av
			inner join pse.acao ac on ac.acaoid=av.acaoid
			inner join (select acaoid, count(distinct entid) as qtdeescola, sum(moetotalpactuado) as moetotalpactuado
			from pse.monitoraescola where moeidentificasse='N' and acaoid in (1,2,3,4,5,6,7,8)
			group by acaoid) as  me on me.acaoid = ac.acaoid
			group by 1,acaodescricao,3, 4
			
			union all
			
			select 'Componente II', 
				acaodescricao, 
				qtdeescola, 
				moetotalpactuado as avaliacoespactuadas, 
				sum(avcquantidade) as avaliacoesrealizadas, 
				(case when moetotalpactuado > 0 then
					(COALESCE(sum(avcquantidade),0) * 100) / moetotalpactuado
				      else
					0
				end) as porcentagemrealizadas,
				0 as problemasidentificados,
				0 porcentagemproblemas 
			from pse.avaliacomp2 av
			inner join pse.acao ac on ac.acaoid=av.acaoid
			inner join (select acaoid,  count(distinct entid) as qtdeescola, sum(moetotalpactuado) as moetotalpactuado
			from pse.monitoraescola where moeidentificasse='N' and acaoid in (9,10,11,12,13,14)
			group by acaoid) as  me on me.acaoid = ac.acaoid
			group by 1,acaodescricao,3, 4
			
			order by 1,2";
$dados = $db->carregar($sql);

foreach($dados as $i => $dado){
	$dados[$i]['qtdeescola'] = "<div align='right' style='color:#0066cc;'>".formata_numero($dados[$i]['qtdeescola'],0,'.','.')."</div>";
	$dados[$i]['avaliacoespactuadas'] = "<div align='right' style='color:#0066cc;'>".formata_numero($dados[$i]['avaliacoespactuadas'],0,'.','.')."</div>";  
	$dados[$i]['avaliacoesrealizadas'] = "<div align='right' style='color:#0066cc;'>".formata_numero($dados[$i]['avaliacoesrealizadas'],0,'.','.')."</div>";
	$dados[$i]['problemasidentificados'] = "<div align='right' style='color:#0066cc;'>".formata_numero($dados[$i]['problemasidentificados'],0,'.','.')."</div>";
}


$cabecalho = array("Componente","Ação","Qtd. Escolas","Qtd. Pactuadas","Qtd. Realizadas","(%) Porcentagem Realizadas","Qtd. Problemas Identificados","(%) Porcentagem Problemas Identificados");
$db->monta_lista_array($dados,$cabecalho,50,10,"N","CENTER");

?>
