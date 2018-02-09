<?php
$ptrid = $_REQUEST['ptrid'];
$tipo = $_REQUEST['tipo'];

function cabecalhoPTARelatorioAnalise($ptrid){
	global $db;
	$sql = "SELECT 
			  ent.enbcnpj AS cnpj,
			  ent.enbnome || ' (' || ent.enbid || ')' AS orgao_entidade,
			  ptr.ptrexercicio AS exercicio,
			  mun.mundescricao || ' / ' || est.estuf AS endereco,
			  ptr.ptrid, 
			  ptr.ptrcod, 
			  res.resassunto,
			  (sum(ped.pedvalor) + ptr.ptrvalorproponente) as ptrvalorconcedente,
			  pti.ptivalortotal as valor
			  
			FROM
			  emenda.entidadebeneficiada ent 
			  INNER JOIN emenda.planotrabalho ptr ON (ent.enbid = ptr.enbid) 
			  INNER JOIN territorios.municipio mun ON (mun.muncod = ent.muncod)
			  INNER JOIN territorios.estado est ON (est.estuf = mun.estuf) 
			  INNER JOIN emenda.responsavel res
			  ON (res.resid=ptr.resid)
			  inner join emenda.ptemendadetalheentidade ped
		  					on ped.ptrid = ptr.ptrid
		  			  INNER JOIN 
						(SELECT 
							ptrid,
							sum(ptivalortotal) as ptivalortotal
						 FROM 
						 	emenda.v_ptiniciativa 
						 GROUP 
						 	BY ptrid) pti ON pti.ptrid = ptr.ptrid
			WHERE
			  ptr.ptrid = $ptrid
			  AND ptr.ptrexercicio = ".$_SESSION["exercicio"]."
		   GROUP BY 
		   	  ent.enbcnpj,
			  orgao_entidade,
			  ptr.ptrexercicio,
			  --ent.muncod,
			  mun.mundescricao,
			  est.estuf,
			  ptr.ptrid, 
			  ptr.ptrcod, 
			  res.resassunto,
			  ptr.ptrvalorproponente,
			  pti.ptivalortotal";
	
	return $db->pegaLinha($sql);
}
?>
<html>
<head>
	<style type="">
		@media print {.notprint { display: none } .div_rolagem{display: none} }	
		@media screen {.notscreen { display: none; }
		
		.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
		
		.folha {
	    	page-break-after: always;
		}
		
	</style>		
</head>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body marginwidth="0" marginheight="0" bgcolor="#ffffff" leftmargin="0" topmargin="0">	
	<?php 	
	$sql = "SELECT
				--usu.usunome,
			    ana.anaparecer,
			    to_char(ana.anadataconclusao, 'DD/MM/YYYY HH24:MI:SS') as anadataconclusao,
			    usu.usunome,
			    ana.uniid
			FROM
				emenda.analise ana
			    inner join seguranca.usuario usu
			    	on usu.usucpf = ana.usucpf
			WHERE
				ana.ptrid = $ptrid
			    and ana.anatipo = '$tipo'
			    and ana.anastatus = 'A'
			    --and ana.anaid = $anaid
			order by ana.anadataconclusao desc";

	$arAnalise = $db->carregar($sql);
	$arAnalise = $arAnalise ? $arAnalise : array();

	if($arAnalise){
		echo '<table width="95%" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td style="text-align: left" class="div_rolagem">
						<input type="button" name="imprimir" value="Imprimir Analise" onclick="javascript: window.print();">
					</td>
				</tr>
			</table>';
		
		foreach ($arAnalise as $valor)
		{
			echo "<div style='page-break-after:always;'>";
			montaRelatorioPorAnalise($ptrid, $tipo, $valor);
			echo '</div>';			
		}
		echo '
			<table width="95%" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td style="text-align: center" class="div_rolagem">
						<input type="button" name="imprimir" value="Imprimir Analise" onclick="javascript: window.print();">
					</td>
				</tr>
			</table>';
	} else {
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
		print '</table>';
		echo '<br/>
			<table width="95%" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td style="text-align: center" class="div_rolagem">
						<input type="button" name="imprimir" value="Fechar" onclick="javascript: window.close();">
					</td>
				</tr>
			</table>';
	}
	?>
</body>