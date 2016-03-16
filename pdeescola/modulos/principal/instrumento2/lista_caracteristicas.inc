<?php

function montaPopupCaracteristicas() {
	
	
	$uf = trim( $_SESSION['estado'] );
	
	if( $uf == 'PR'){
		$descricao 		= "acedescricaoPR";
		$descricaoJoin  = "ace.acedescricaoPR";
		$descricaoJoin2 = "ace2.acedescricaoPR";
		$descricaoJoin3 = "ace3.acedescricaoPR";
	}
	else
	{
		$descricao 		= "acedescricao";
		$descricaoJoin  = "ace.acedescricao";
		$descricaoJoin2 = "ace2.acedescricao";
		$descricaoJoin3 = "ace3.acedescricao";
	}
	
	$sql = pg_query("SELECT
				   	ace.aceid as id_caracteristica,
					$descricaoJoin as desc_caracteristica
				   FROM
					pdeescola.analisecriterioeficacia ace
				   INNER JOIN
					pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
														   AND ace2.aceid = ".$_GET["requisito"]."
				   ORDER BY
					ace.aceid ASC");
	$count = 1;
	while(($caracts = pg_fetch_array($sql)) != false) {
		$cor = "#f4f4f4";
		$count++;
		if($count % 2) { $cor = "#e0e0e0"; }
		
		$checked = "";
		$disabled = "";
		if($caracts["id_caracteristica"] == $_GET["caracteristica"])
			$checked = "checked";
		
		if(($caracts["id_caracteristica"] == $_GET["caracteristica2"]) || ($caracts["id_caracteristica"] == $_GET["caracteristica3"]))
			$disabled = "disabled=\"disabled\"";
			
		echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
				<td>
					<input type=\"radio\" name=\"caracteristica\" value=\"".$caracts["id_caracteristica"]."\" ".$checked." ".$disabled." onclick=\"alteraCaracteristica(".$_GET["criterio"].",".$_GET["requisito"].",".$caracts["id_caracteristica"].",'".$caracts["desc_caracteristica"]."',".$_GET["rowindex"].",".$_GET["posicao"].",".$_GET["idtabela"].");\" /> ".$caracts["desc_caracteristica"]."
				</td>
			  </tr>";
	}
	
	$count++;
	if($count % 2) { $cor = "#e0e0e0"; }
	
	$checked = "";
	$disabled = "";
	if($_GET["caracteristica"] == 0)
		$checked = "checked";
		
		
	echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
			<td>
				<input type=\"radio\" name=\"caracteristica\" value=\"0\" ".$checked." onclick=\"alteraCaracteristica(".$_GET["criterio"].",".$_GET["requisito"].",0,'Clique para escolher uma característica',".$_GET["rowindex"].",".$_GET["posicao"].",".$_GET["idtabela"].");\" /> Nenhuma característica
			</td>
		  </tr>";
}

?>

<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top:none; border-bottom:none; width:100%;">
	<tr>
		<td width="100%" align="center">
			<label class="TituloTela" style="color:#000000;"> 
				Características 
			</label>
		</td>
	</tr>
</table>
<br/>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#cdcdcd">
		<td colspan="2" valign="top">
			<strong>Selecione uma característica</strong>
		</td>
	</tr>
	<?php 
		montaPopupCaracteristicas();
	?>
	<tr bgcolor="#C0C0C0">
		<td>
			<input type="button" name="ok" value="Ok" onclick="self.close();">
		</td>
	</tr>
</table>
<script type="text/javascript">
	function alteraCaracteristica(criterio,requisito,idNovaCaracteristica,descNovaCaracteristica,rowindex,posicao,idtabela) {
		var tabela = window.opener.document.getElementById('tabela_caracteristicas_' + idtabela);
		
		tabela.rows[rowindex].cells[0].innerHTML = 
			"<a style=\"cursor:pointer;\" onclick=\"janelaCaracteristicas("+criterio+","+requisito+","+idNovaCaracteristica+",this.parentNode.parentNode.rowIndex,"+idtabela+");\" >"+
			"<img src=\"../imagens/arrow_simples.gif\">"+descNovaCaracteristica+"</a>";
			
		window.opener.document.getElementById('caracteristica['+criterio+']['+requisito+']['+posicao+']').value = idNovaCaracteristica;
	}
</script>