<?php

function montaPopupRequisitos() {
	
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
					   	ace.aceid as id_requisito,
						$descricaoJoin as desc_requisito
					   FROM
						pdeescola.analisecriterioeficacia ace
					   INNER JOIN
						pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
															   AND ace2.aceid = ".$_GET["criterio"]."
					   ORDER BY
						ace.aceid ASC");
	$count = 1;
	while(($reqs = pg_fetch_array($sql)) != false) {
		$cor = "#f4f4f4";
		$count++;
		if($count % 2) { $cor = "#e0e0e0"; }
		
		$checked = "";
		$disabled = "";
		if($reqs["id_requisito"] == $_GET["requisito"])
			$checked = "checked";
					
		if(($reqs["id_requisito"] == $_GET["requisito2"]) || ($reqs["id_requisito"] == $_GET["requisito3"]))
			$disabled = "disabled=\"disabled\"";
			
		echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
				<td>
					<input type=\"radio\" name=\"requisito\" value=\"".$reqs["id_requisito"]."\" ".$checked." ".$disabled." onclick=\"alteraRequisito(".$_GET["criterio"].",".$reqs["id_requisito"].",'".$reqs["desc_requisito"]."',".$_GET["rowindex"].",".$_GET["posicao"].",".$_GET["idtabela"].");\" /> ".$reqs["desc_requisito"]."
				</td>
			  </tr>";
	}
	
	$count++;
	if($count % 2) { $cor = "#e0e0e0"; }
	
	$checked = "";
	if($_GET["requisito"] == 0)
		$checked = "checked";
	
	echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
			<td>
				<input type=\"radio\" name=\"requisito\" value=\"0\" ".$checked." onclick=\"alteraRequisito(".$_GET["criterio"].",0,'Clique para escolher um requisito',".$_GET["rowindex"].",".$_GET["posicao"].",".$_GET["idtabela"].");\" /> Nenhum requisito
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
				Requisitos 
			</label>
		</td>
	</tr>
</table>
<br/>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#cdcdcd">
		<td colspan="2" valign="top">
			<strong>Selecione um requisito</strong>
		</td>
	</tr>
	<?php 
		montaPopupRequisitos();
	?>
	<tr bgcolor="#C0C0C0">
		<td>
			<input type="button" name="ok" value="Ok" onclick="self.close();">
		</td>
	</tr>
</table>
<script type="text/javascript">
	function alteraRequisito(criterio,idNovoRequisito,descNovoRequisito,rowindex,posicao,idTabela) {
		var tabela = window.opener.document.getElementById('tabela_requisitos_' + criterio);
		
		tabela.rows[rowindex].cells[0].innerHTML = 
			"<a style=\"cursor:pointer;\" onclick=\"janelaRequisitos("+criterio+","+idNovoRequisito+",this.parentNode.parentNode.rowIndex,"+idTabela+");\" >"+
			"<img src=\"../imagens/arrow_simples.gif\">"+descNovoRequisito+"</a>";
			
		window.opener.document.getElementById('requisito['+criterio+']['+posicao+']').value = idNovoRequisito;
		
		var tabelaCaracteristicas = window.opener.document.getElementById("tabela_caracteristicas_"+idTabela);
		for(var i=0; i<=2; i++) {
			tabelaCaracteristicas.rows[i].cells[0].innerHTML = 
				"<a style=\"cursor:pointer;\" onclick=\"janelaCaracteristicas("+criterio+","+idNovoRequisito+",0,this.parentNode.parentNode.rowIndex,"+idTabela+");\">" +
					"<img src=\"../imagens/arrow_simples.gif\">" +
					"Clique para escolher uma característica" +
				"</a>";
			
			if(i == 0) {
				var img = "<img src=\"../imagens/seta_baixo.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('baixo','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\"> " +
						  "<img src=\"../imagens/seta_cimad.gif\" style=\"border:0; cursor:pointer;\">";
			} else if(i == 1) {
				var img = "<img src=\"../imagens/seta_baixo.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('baixo','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\"> " +
						  "<img src=\"../imagens/seta_cima.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('cima','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">";
			} else {
				var img = "<img src=\"../imagens/seta_baixod.gif\" style=\"border:0; cursor:pointer;\"> " +
						  "<img src=\"../imagens/seta_cima.gif\" style=\"border:0; cursor:pointer;\" onclick=\"mudaPosicao('cima','caracteristica',this.parentNode.parentNode.parentNode.parentNode.id,this.parentNode.parentNode.rowIndex);\">";
			}
			
			tabelaCaracteristicas.rows[i].cells[1].innerHTML = 
				img +
				"<input type=\"hidden\" id=\"caracteristica["+criterio+"]["+idNovoRequisito+"][" + Number(i+1) + "]\" name=\"caracteristica["+criterio+"]["+idNovoRequisito+"][" + Number(i+1) + "]\" value=\"0\">" +
				"<input type=\"hidden\" value=\"" + Number(i+1) + "\">";
		}
	}
</script>