<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<style type="text/css">
.stylecolunas {
	background-color: #dcdcdc;
	text-align: center; 
}
.stylelinhas {
	background-color: #dcdcdc;
	text-align: right; 
}
.styleconteudo {
	text-align: left;
}
.titulounidade {
	background-color:#CDCDCD;
	text-align:center;
	font-weight: bold;
}
</style>
<?
/*
215;45;;1;"Contratualizado"
216;45;;1;"Apresentado"
217;45;;1;"Aprovado/Pago"
352;45;;1;"Produzido"
*/


$sql = array(// Contratualizado
			 0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid='215' AND linid IN('473','474','479','480','481') AND ctiexercicio='{ano}'",
			 // Aprovado
			 1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid='217' AND linid IN('473','474','479','480','481') AND ctiexercicio='{ano}'",
			 // Produzido
			 2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid='352' AND linid IN('473','474','479','480','481') AND ctiexercicio='{ano}'"
			);
			  
			  

$filtro = (($_REQUEST['entid'][0])?"WHERE ent.entid IN ('".implode("','",$_REQUEST['entid'])."')":"");
$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, esu.esutipo FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");

$_ano = array('2004','2005','2006','2007','2008');

echo "<h1>Relatório de Recursos do SUS</h1>";
foreach($_ano as $ano) {
 	unset($Totcontratualizado,$Totaprovado,$Totproduzido);
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='6'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Contratualizado</td>";
	echo "<td class='SubTituloCentro'>Produzido</td>";
	echo "<td class='SubTituloCentro'>Aprovado</td>";
	echo "<td class='SubTituloCentro'>(Pago-Produzido)</td>";
	echo "</tr>";
	
	foreach($esuids as $esuid) {
		$contratualizado = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[0]));
		$Totcontratualizado += $contratualizado;
		$aprovado = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[1]));
		$Totaprovado += $aprovado;
		$produzido = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[2]));
		$Totproduzido += $produzido;
		
		echo "<tr>";
		echo "<td>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td>".number_format($contratualizado,2,',','.')."</td>";
		echo "<td>".number_format($produzido,2,',','.')."</td>";
		echo "<td>".number_format($aprovado,2,',','.')."</td>";
		echo "<td>".number_format($aprovado-$produzido,2,',','.')."</td>";
		echo "</tr>";
	}
	
	echo "<tr bgcolor=\"#C0C0C0\">";
	echo "<td align='right' colspan='2'><b>TOTAL :</b></td>";
	echo "<td><b>".number_format($Totcontratualizado,2,',','.')."</b></td>";
	echo "<td><b>".number_format($Totaprovado,2,',','.')."</b></td>";
	echo "<td><b>".number_format($Totproduzido,2,',','.')."</b></td>";
	echo "<td><b>".number_format($Totaprovado-$Totproduzido,2,',','.')."</b></td>";
	echo "</tr>";
	

echo "</table>";

}

?>