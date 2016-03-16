<?php
unset($_SESSION['rehuf_var']['entid']);

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo(str_replace("...","",$_SESSION['mnudsc']), '&nbsp;' );

$sql = "SELECT 
			ent.entid,
			ent.entnome,
			ena.entsig,
			count(hit.hitid) as qtde
		FROM 
			entidade.entidade ent 
		LEFT JOIN 
			entidade.funcaoentidade fen ON fen.entid = ent.entid 
		LEFT JOIN 
			entidade.funcao fun ON fun.funid = fen.funid 
		LEFT JOIN 
			entidade.funentassoc fue ON fue.fueid = fen.fueid 
		LEFT JOIN 
			entidade.entidade ena ON ena.entid = fue.entid 
		LEFT JOIN 
			entidade.endereco ende ON ende.entid = ent.entid 
		LEFT JOIN 
			territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
		LEFT JOIN 
			rehuf.estruturaunidade esu ON esu.entid = ent.entid
		LEFT JOIN
			rehuf.hospitalitemhomologado hit on hit.entid = ent.entid and ( (hitvalorunitario is not null and hitcnpjfornecedor is not null and tpaid is not null) or (hitobs is not null) ) 
		WHERE 
			fen.funid IN ('16','93') AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) and ent.tpcid = 2
		group by
			ent.entid,
			ent.entnome,
			ena.entsig
		ORDER BY qtde desc,ena.entsig";
//dbg($sql);
$arrHospitais = $db->carregar($sql);

$sql = "select preid,preobjeto from rehuf.pregaohomologado where prestatus = 'A' order by preid";
$arrPregoes = $db->carregar($sql);

$sql = "select
			ent.entid,
			pre.preid,
			count(hit.hitid) as itens_preenchidos
		from
			rehuf.pregaohomologado pre
		inner join
			rehuf.itemhomologado ite ON ite.preid = pre.preid and ite.itestatus = 'A'
		inner join
			rehuf.hospitalitemhomologado hit on hit.iteid = ite.iteid
		inner join
			entidade.entidade ent ON ent.entid = hit.entid
		and
			( (hitvalorunitario is not null and hitcnpjfornecedor is not null and tpaid is not null) or (hitobs is not null) )
		where
			prestatus = 'A'
		group by
			pre.preid,ent.entid
		order by
			ent.entid";
$arrItensPorHospital = $db->carregar($sql);
foreach($arrItensPorHospital as $i){
	$arrDados[$i['entid']][$i['preid']] = $i;
}

$sql = "select
			pre.preid,
			count(ite.iteid) as itens
		from
			rehuf.pregaohomologado pre
		inner join
			rehuf.itemhomologado ite ON ite.preid = pre.preid and ite.itestatus = 'A'
		where
			prestatus = 'A'
		group by
			pre.preid";
$arrItens = $db->carregar($sql);
foreach($arrItens as $i){
	$arrItensPorPregao[$i['preid']] = $i;
}
?>
<style>
	.link{cursor:pointer}
	.center{text-align:center}
	.bold{font-weight:bold}
	.blue{color:blue}
	.right{text-align:right}
</style>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
	<tr bgcolor="#c5c5c5">
		<td width="250px" class="bold center" >Hospital</td>
		<?php foreach($arrPregoes as $pre): ?>
			<td class="bold center" ><?php echo $pre['preobjeto']?></td>
		<?php endforeach;?>
		<td width="150px" class="bold center" >Média</td>
	</tr>
	<?php foreach($arrHospitais as $cor => $hos): ?>
		<tr bgcolor="<?php echo $cor%2 ? "#FFFFFF" : "" ?>" >
			<td class="" ><a target="_blank" href="rehuf.php?modulo=pregao/listaPregaoHomologado&acao=A&entid=<?php echo $hos['entid'] ?>" ><?php echo $hos['entsig'] ? $hos['entsig']." - " : "" ?><?php echo $hos['entnome'] ?></a></td>
			<?php $itens = 0; $itens_homologados = 0;?>
			<?php foreach($arrPregoes as $pre): ?>
				<td class="blue right" >
					<?php 
						$itens+=$arrDados[$hos['entid']][$pre['preid']]['itens_preenchidos']; 
						$itens_homologados+=$arrItensPorPregao[$pre['preid']]['itens']; 
					?>
					<span style="white-space:nowrap;"><?php echo number_format($arrDados[$hos['entid']][$pre['preid']]['itens_preenchidos'],0,',','.'); ?> de 
					<?php echo number_format($arrItensPorPregao[$pre['preid']]['itens'],0,',','.'); ?> </span> <br/>
					(<?php echo number_format(($arrDados[$hos['entid']][$pre['preid']]['itens_preenchidos']/$arrItensPorPregao[$pre['preid']]['itens'])*100,2,',','.'); ?>%)
				</td>
			<?php endforeach;?>
			<td class="blue right bold" >
				<span style="white-space:nowrap;"><?php echo number_format($itens,0,',','.'); ?> de 
				<?php echo number_format($itens_homologados,0,',','.'); ?></span> <br/>
				(<?php echo number_format(($itens/($itens_homologados ? $itens_homologados : 1))*100,2,',','.'); ?>%)
			</td>
		</tr>
	<?php endforeach;?>
	<tr bgcolor="#c5c5c5">
		<td colspan="<?php echo count($arrPregoes)+2 ?>" class="bold" >Total de Registros: <?php echo $cor+1 ?></td>
	</tr>
</table>