<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<style type="text/css">
.stylecolunas {
	background-color: #dcdcdc;
	text-align: center;
	font-weight: bold;
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
			  

$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, ende.endcep, 
								ende.endlog, ende.endcom, ende.endnum, ende.endbai, SUBSTR(ent.entnumcpfcnpj,1,2)||'.'||SUBSTR(ent.entnumcpfcnpj,3,3)||'.'||SUBSTR(ent.entnumcpfcnpj,6,3)||'/'||SUBSTR(ent.entnumcpfcnpj,9,4)||'-'||SUBSTR(ent.entnumcpfcnpj,13,2) as entnumcpfcnpj, ent.entrazaosocial  
						 FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");


echo "<h1>Relatório de dados dos Hospitais</h1>";
echo "<table width='100%' class='tabela'>";
echo "<tr>";
echo "<td rowspan='2' class='SubTituloCentro'>IFES</td>";
echo "<td rowspan='2' class='SubTituloCentro'>Hospital</td>";
echo "<td rowspan='2' class='SubTituloCentro'>CNPJ</td>";
echo "<td rowspan='2' class='SubTituloCentro'>Razão social</td>";
echo "<td rowspan='2' class='SubTituloCentro'>CEP</td>";
echo "<td rowspan='2' class='SubTituloCentro'>Logradouro</td>";
echo "<tr>";

foreach($esuids as $esuid) {
	echo "<tr>";
	echo "<td>".$esuid['entsig']."</td>";
	echo "<td nowrap >".$esuid['entnome']."</td>";
	echo "<td>".$esuid['entnumcpfcnpj']."</td>";
	echo "<td>".$esuid['entrazaosocial']."</td>";
	echo "<td>".$esuid['endcep']."</td>";
	echo "<td>".$esuid['estuf'].", ".$esuid['mundescricao'].", ".$esuid['endbai'].",  ".$esuid['endlog'].(($esuid['endcom'])?", ".$esuid['endcom']:"").", Número ".$esuid['endnum']."</td>";
	echo "</tr>";
}
echo "</table>";



?>