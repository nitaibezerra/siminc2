<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de Bolsistas sem tramitação de pagamento</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$sql = "SELECT foo.estuf, foo.mundescricao, SUM(foo.prof) as nprof, SUM(foo.orie) as norie, SUM(foo.cool) as ncool FROM (
		SELECT m.estuf, 
			   m.mundescricao, 
			   CASE WHEN t.pflcod=849 THEN 1 ELSE 0 END as prof, 
			   CASE WHEN t.pflcod=827 THEN 1 ELSE 0 END as orie,
			   CASE WHEN t.pflcod=826 THEN 1 ELSE 0 END as cool
		FROM sispacto.identificacaousuario i 
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
		INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
		WHERE iusstatus='A' AND CASE WHEN t.pflcod=827 THEN i.iusformacaoinicialorientador=true ELSE true END
		) foo 
		GROUP BY foo.estuf, foo.mundescricao 
		ORDER BY foo.estuf, foo.mundescricao";

echo "<p><b>Rede municipal</b></p>";

$cabecalho = array("UF","Município","Nº Professores Alfabetizadores","Nº de Orientadores de Estudo","Nº Coordenadores locais");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);

$sql = "SELECT foo.estuf, foo.mundescricao, SUM(foo.prof) as nprof, SUM(foo.orie) as norie, SUM(foo.cool) as ncool FROM (
		SELECT CASE WHEN i.muncodatuacao IS NOT NULL THEN m.estuf
			    WHEN i.muncod        IS NOT NULL THEN m2.estuf 
			    ELSE 'Não cadastrado' END as estuf, 
		       CASE WHEN i.muncodatuacao IS NOT NULL THEN m.mundescricao
			    WHEN i.muncod        IS NOT NULL THEN m2.mundescricao 
			    ELSE 'Não cadastrado' END as mundescricao, 
		       CASE WHEN t.pflcod=849 THEN 1 ELSE 0 END as prof, 
		       CASE WHEN t.pflcod=827 THEN 1 ELSE 0 END as orie,
		       CASE WHEN t.pflcod=826 THEN 1 ELSE 0 END as cool
		FROM sispacto.identificacaousuario i 
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
		INNER JOIN territorios.estado e ON e.estuf = p.estuf 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncod
		WHERE iusstatus='A' AND CASE WHEN t.pflcod=827 THEN i.iusformacaoinicialorientador=true ELSE true END
		) foo 
		GROUP BY foo.estuf, foo.mundescricao 
		ORDER BY foo.estuf, foo.mundescricao";

echo "<p><b>Rede estadual</b></p>";

$cabecalho = array("UF","Município","Nº Professores Alfabetizadores","Nº de Orientadores de Estudo","Nº Coordenadores locais");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);


$sql = "SELECT foo.unisigla, foo.uninome, SUM(foo.coog) as ncoog, SUM(foo.cooa) as ncooa, SUM(foo.supe) as nsupe, SUM(foo.form) as nform FROM (
		SELECT un.unisigla, 
		       un.uninome, 
		       CASE WHEN t.pflcod=832 THEN 1 ELSE 0 END as coog,
		       CASE WHEN t.pflcod=846 THEN 1 ELSE 0 END as cooa,
		       CASE WHEN t.pflcod=847 THEN 1 ELSE 0 END as supe,
		       CASE WHEN t.pflcod=848 THEN 1 ELSE 0 END as form
		       
		FROM sispacto.identificacaousuario i 
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid  
		INNER JOIN sispacto.universidade un ON un.uniid = u.uniid 
		WHERE iusstatus='A' AND t.pflcod IN(832,846,847,848)
		) foo 
		GROUP BY foo.unisigla, foo.uninome
		ORDER BY foo.unisigla, foo.uninome";

echo "<p><b>Universidades</b></p>";

$cabecalho = array("Sigla","IES","Nº Coordenadores Gerais","Nº Coordenadores Adjuntos","Nº Supervisores","Nº Formadores");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);

?>