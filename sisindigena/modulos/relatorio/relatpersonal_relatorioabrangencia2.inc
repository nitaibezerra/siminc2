<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de abrangência Geral (Saberes Indígenas na Escola)</b></p>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda">TERRITÓRIOS ETNOEDUCACIONAIS
	</td>
</tr>
<tr>
	<td><? 
	$sql = "SELECT distinct li.laadsc 
			FROM sisindigena.listaabrangenciaacaonucleo l 
			INNER JOIN sisindigena.listaabrangenciaacao li ON li.laaid = l.laaid 
			WHERE li.laatipo='etnoeducacionais'";
	
	$cabecalho = array("TERRITÓRIOS ETNOEDUCACIONAIS");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
 ?></td>
</tr>

<tr>
	<td class="SubTituloEsquerda">POVOS
	</td>
</tr>
<tr>
	<td><? 
	$sql = "SELECT distinct li.laadsc 
			FROM sisindigena.listaabrangenciaacaonucleo l 
			INNER JOIN sisindigena.listaabrangenciaacao li ON li.laaid = l.laaid 
			WHERE li.laatipo='povos'";
	
	$cabecalho = array("POVOS");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
 ?></td>
</tr>

<tr>
	<td class="SubTituloEsquerda">ALDEIAS
	</td>
</tr>
<tr>
	<td><? 
	$sql = "SELECT distinct lanaldeiadsc as lanaldeiadsc FROM sisindigena.listaabrangenciaacaonucleo WHERE lanaldeiadsc IS NOT NULL";
	$aldeias = $db->carregar($sql);
	
	if($aldeias[0]) {
		foreach($aldeias as $key => $ald) {
			$aldeias[$key]['lanaldeiadsc'] = utf8_decode($ald['lanaldeiadsc']);
		}
	}
	
	$cabecalho = array("ALDEIAS");
	$db->monta_lista_simples($aldeias,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
 ?></td>
</tr>

<tr>
	<td class="SubTituloEsquerda">LINGUAS
	</td>
</tr>
<tr>
	<td><? 
	$sql = "SELECT distinct li.laadsc 
			FROM sisindigena.listaabrangenciaacaonucleo l 
			INNER JOIN sisindigena.listaabrangenciaacao li ON li.laaid = l.laaid 
			WHERE li.laatipo='lingua'";
	
	$cabecalho = array("LINGUAS");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
 ?></td>
</tr>

<tr>
	<td class="SubTituloEsquerda">ESCOLAS
	</td>
</tr>
<tr>
	<td><? 
	$sql = "SELECT lanescoladsc FROM sisindigena.listaabrangenciaacaonucleo WHERE lanescoladsc IS NOT NULL GROUP BY lanescoladsc ORDER BY lanescoladsc";
	$escolas = $db->carregar($sql);
	
	if($escolas[0]) {
		foreach($escolas as $key => $esc) {
			$escolas[$key]['lanescoladsc'] = utf8_decode($esc['lanescoladsc']);
		}
	}
	
	$cabecalho = array("ESCOLAS");
	$db->monta_lista_simples($escolas,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
 ?></td>
</tr>

</table>