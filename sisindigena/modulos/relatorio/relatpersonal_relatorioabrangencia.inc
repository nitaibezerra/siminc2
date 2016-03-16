<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de abrangência nos Municípios (Saberes Indígenas na Escola)</b></p>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda">Municípios Atendidos</td>
</tr>
<tr>
	<td><? 
	$sql = "select m.estuf, m.mundescricao, count(*) as num from sisindigena.identificacaousuario i 
			inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
			inner join territorios.municipio m on m.muncod = i.muncodatuacao 
			where t.pflcod in(1027,1029) 
			GROUP BY m.estuf, m.mundescricao
			order by m.estuf, m.mundescricao";
	
	$cabecalho = array("UF","Município","Qtd de cursistas");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
 ?></td>
</tr>



</table>