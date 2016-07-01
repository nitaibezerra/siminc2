<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$db = new cls_banco();

$sql = sprintf(
	"select
		ps.ppsdsc,
		c.crtdsc, c.ctrpontuacao,
		i.indcod, i.inddsc,
		ad.ardcod, ad.arddsc,
		d.dimcod, d.dimdsc
	from cte.proposicaosubacao ps
	inner join cte.proposicaoacao pa on
		pa.ppaid = ps.ppaid
	inner join cte.criterio c on
		c.crtid = pa.crtid
	inner join cte.indicador i on
		i.indid = c.indid and
		i.indstatus = 'A'
	inner join cte.areadimensao ad on
		ad.ardid = i.ardid and
		ad.ardstatus = 'A'
	inner join cte.dimensao d on
		d.dimid = ad.dimid and
		d.dimstatus = 'A'
	where
		ps.ppstexto is not null
	order by
		d.dimcod, ad.ardcod, i.indcod, c.crtdsc"
);

$sql = sprintf(
			"select 
			m.muncod as codigoibge, 
			m.estuf as uf,
			m.mundescricao as municipio,
			s.sbaid as codigosubacao, 
			s.sbadsc as descricaosubacao, 
			p.prgplanointerno as pi,
			replace ( trim(d.dimcod||'.'||a.ardcod||'.'||i.indcod||'.'||s.sbaordem||' - '||s.sbauntdsc ) , ' – ' , '' )  as descricao
			from 
			cte.instrumentounidade iu 
			inner join territorios.municipio m ON m.muncod = iu.muncod and m.estuf = iu.mun_estuf 
			inner join cte.convenio c ON c.inuid = iu.inuid 
			inner join cte.subacaoconvenio sc ON sc.cnvid = c.cnvid 
			inner join cte.subacaoindicador s ON s.sbaid = sc.sbaid 
			inner join cte.acaoindicador ai ON ai.aciid = s.aciid 
			inner join cte.pontuacao pt ON pt.ptoid = ai.ptoid
			inner join cte.indicador i ON i.indid = pt.indid
			inner join cte.areadimensao a ON a.ardid = i.ardid
			inner join cte.dimensao d ON d.dimid = a.dimid
			inner join cte.programa p ON p.prgid = s.prgid
			order by m.estuf,
			m.mundescricao ");

$lista = $db->carregar( $sql );
$lista = $lista ? $lista : array();

?>
<style>
	table tbody tr td { vertical-align: top; }
	table thead tr th { background-color: #cccccc; }
</style>
<table>
	<thead>
		<tr>
			<th>IBGE</th>
			<th>UF</th>
			<th>Municipio</th>
			<th>codigosubacao</th>
			<th>descricaosubacao</th>
			<th>PI</th>
			<th>descricao</th>
		</tr>
	</thead>
	<tbody>
		<?php foreach( $lista as $indice => $item ): ?>
		<?php
		$cor = $indice % 2 ? "#eeeeee" : "ffffff";
		?>
		<tr style="background-color: <?= $cor ?>">
			<td><?= $item["codigoibge"] ?> </td>
			<td><?= $item["uf"] ?> </td>
			<td><?= $item["municipio"] ?> </td>
			<td><?= $item["codigosubacao"] ?></td>
			<td><?= $item["descricaosubacao"] ?></td>
			<td><?= $item["pi"] ?></td>
			<td><?= $item["descricao"] ?></td>
		</tr>
		<?php endforeach; ?>
	</tbody>
</table>







