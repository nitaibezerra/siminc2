<?
$sql = "SELECT *
			FROM 
			  rehuf.item i 
			  inner join rehuf.itemgrupo ig 
			  	on (i.iteid = ig.iteid) 
			  inner join rehuf.grupoitens g
			  	on (ig.gruid = g.gruid)
			  inner join rehuf.itemgrupopregao igp
    			on (ig.itgid = igp.itgid) 
    		  left join rehuf.itemlote il 
    		  	on (il.itlid = i.itlid)
			WHERE i.itestatus = 'A'
			AND g.grustatus = 'A'
			AND ig.itgstatus = 'A'
			AND igp.igpstatus = 'A'
			AND igp.preid='".$_REQUEST['preid']."' 
			ORDER BY i.itlid, i.itedescricao, g.gruid, i.itecatmat, i.iteapresentacao";
$itens = $db->carregar($sql);

$sql = "SELECT ent.entnome, ena.entsig, ent.entid FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
		LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
		LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
		WHERE fen.funid IN ('". HOSPITALUNIV ."','".HOSPITALFEDE."') AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
		ORDER BY ena.entsig";

$entidades = $db->carregar($sql);

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<table class="listagem" style="width:100%" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
<td class="SubTituloEsquerda"><? echo strtoupper($db->pegaUm("SELECT preobjeto FROM rehuf.pregao WHERE preid='".$_REQUEST['preid']."'")); ?></td>
</tr>
</table>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
<td rowspan="2" class="SubTituloCentro">DESCRIÇÃO</td>
<td rowspan="2" class="SubTituloCentro">APRESENTAÇÃO</td>
<td rowspan="2" class="SubTituloCentro">CATMAT</td>
<td rowspan="2" class="SubTituloCentro">COD SUS</td>
<td rowspan="2" class="SubTituloCentro">LOTE</td>
<?
if($entidades[0]) {
	foreach($entidades as $entidade) {
		$resumoent = explode(" ", $entidade['entnome']);
		unset($hospabrv);
		for($i=0;$i<count($resumoent);$i++) {
			 $hospabrv .= strtoupper(substr(trim($resumoent[$i]),0,1));
		}
		echo "<td colspan='2' class='SubTituloCentro' title='".$entidade['entnome']."'>".(($entidade['entsig'])?$entidade['entsig']."-":"").$hospabrv."</td>";
		
	}
}
?>
<td colspan="2" class="SubTituloCentro">TOTAL</td>
</tr>
<tr>
<?
if($entidades[0]) {
	foreach($entidades as $entidade) {
		echo "<td class='SubTituloCentro' style='font-size:9px;' nowrap>QTD Mensal</td>";
		echo "<td class='SubTituloCentro' style='font-size:9px;' nowrap>QTD Anual</td>";
	}
}
?>
	<td class="SubTituloCentro" style="font-size:9px;" nowrap>QTD Mensal</td>
	<td class="SubTituloCentro" style="font-size:9px;" nowrap>QTD Anual</td>
</tr>
<?
if($itens[0]) {
	foreach($itens as $i => $itm) {
		if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
?>
		<tr bgcolor="<? echo $marcado; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<? echo $marcado; ?>';">
			<td><? echo (($itm['itedescricao'])?$itm['itedescricao']:"-"); ?></td>
			<td><? echo (($itm['iteapresentacao'])?$itm['iteapresentacao']:"-"); ?></td>
			<td><? echo (($itm['itecatmat'])?$itm['itecatmat']:"-"); ?></td>
			<td><? echo (($itm['itecodsus'])?$itm['itecodsus']:"-"); ?></td>
			<td><? echo (($itm['itldesc'])?$itm['itlabrev']:"-"); ?></td>
<?
			$totalizador=0;
			foreach($entidades as $entidade) {
				$sql = "SELECT ippquantidadeanual FROM rehuf.itenspregaopreenchido ipp 
						LEFT JOIN rehuf.huparticipante hup ON hup.hupid=ipp.hupid 
						WHERE igpid='".$itm['igpid']."' AND entid='".$entidade['entid']."' AND preid='".$_REQUEST['preid']."'";
				$ippquantidadeanual = $db->pegaUm($sql);
				$totalizador += $ippquantidadeanual; 
				echo "<td style='text-align:center'>".(($ippquantidadeanual)?round($ippquantidadeanual/12):"")."</td>";
				echo "<td style='text-align:center'>".$ippquantidadeanual."</td>";
			}
?>
	<td style="text-align:center"><strong><? echo round($totalizador/12); ?></strong></td>
	<td style="text-align:center"><strong><? echo $totalizador; ?></strong></td>
	</tr>
<?
	}
} else {
?>
<tr>
	<td class="SubTituloEsquerda" colspan="<? echo ((count($entidades)*2)+7); ?>">Não existem itens cadastrados</td>
</tr>
<?
}
?>