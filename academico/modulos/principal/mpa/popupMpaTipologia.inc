<?php 
//ver($_GET,d);
function montaPopupEtapas(){
	global $db;
	$sql = "SELECT 
				dintipo as tipo
			FROM
				academico.demandainfra
			WHERE
	 			dinid = {$_REQUEST['dinid']}";
	$te = $db->pegaUm($sql);
	
	$sql = "SELECT
				tpdid as codigo,
				tpddsc as descricao,
				tdpdetalhamento as tooltip
			FROM
				academico.tipologiademanda
			WHERE tpdtipo = '{$te}'
			ORDER BY descricao";
	$sql = pg_query($sql);
	//ver($resultado,d);
	
	$count = "1";
	while (($dados = pg_fetch_array($sql)) != false){
		$itcid = $dados['codigo'];
		$itcdesc = $dados['descricao'];
		$itcdescservico = $dados['tooltip'];
		$cor = "#f4f4f4";
		$count++;
		$nome = "etapa_".$itcid;
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		$title = "onmouseover=\"return escape('$itcdescservico');\"";
		
		echo "
			<script type=\"text/javascript\"> id_etapas.push('$nome'); </script>
			<tr bgcolor=\"$cor\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td $title>
					<input id=\"tpoid\" name=\"tpoid\" type=\"radio\" 
							value=\"" . $itcid . "\"  
							onclick=\"marcaItem('".$itcdesc."', ".$itcid.");\">" . $itcdesc . "
				</td>
			</tr>
		";	
	};
}

?>
<html>
	<head>
		<title>Inserir Serviços</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head> 
	<script type="text/javascript">	
		
		function marcaItem( descricao, id ) {
						
			window.opener.document.getElementById("tpdid").value = id;
			window.opener.document.getElementById("div_tpdid").value = descricao;
			
		}
		
	</script>
	<body>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="100%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Inserir Serviços 
					</label>
				</td>
			</tr>
		</table>
		<br/>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<strong>Selecione a(s) Serviço(s)</strong>
				</td>
			</tr>
			<?php 
				montaPopupEtapas();
			?>
			<tr bgcolor="#C0C0C0">
				<td>
					<input type="button" name="ok" value="Ok" onclick="self.close();">
				</td>
			</tr>
		</table>
		
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
	</body>
</html>