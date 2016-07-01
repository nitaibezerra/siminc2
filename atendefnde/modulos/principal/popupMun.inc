<?php 

function montaPopup($listauf = null){
	if($listauf){
		$where = "WHERE estuf in ('".str_replace(",","','",$listauf)."')";
	}
	$sql = pg_query("SELECT muncod, estuf||' - '||mundescricao as descricao FROM territorios.municipio $where order by 2");
	$count = "1";
	while (($dados = pg_fetch_array($sql)) != false){
		$muncod = $dados['muncod'];
		$descricao = $dados['descricao'];
		$cor = "#f4f4f4";
		$count++;
		$nome = "muncod_".$muncod;
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		echo "
			<script type=\"text/javascript\"> id_etapas.push('$nome'); </script>
			<tr bgcolor=\"$cor\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td $title>
				<input id=\"".$nome."\" name=\"".$descricao."\" type=\"checkbox\" value=\"" . $muncod . "\" onclick=\"marcaItem('".$descricao."', ".$muncod.");\">" . $descricao . "
				</td>
			</tr>
		";	
	};
}

?>
<html>
	<head>
		<title>Inserir Municípios</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript">	
		var id_etapas = new Array();
		
		function selecionaTodos() {
			var i, etapa, descricao, id, nome;
						
			for(i=0; i<id_etapas.length; i++) {
				etapa = document.getElementById(id_etapas[i]);
								
				if((document.getElementById("selecionar_todos").checked == true)&&(etapa.checked == false)) {
					etapa.checked = true;
					descricao = etapa.name;
					id = etapa.value;
					nome = etapa.id;
										
					marcaItem(descricao, id);
				} else if((document.getElementById("selecionar_todos").checked == false)&&(etapa.checked == true)) {
					etapa.checked = false;
					descricao = etapa.name;
					id = etapa.value;
					nome = etapa.id;
								
					marcaItem(descricao, id);
				}			
			}
		}
		
		function marcaItem(descricao, id) {				
			var tabela = window.opener.document.getElementById("tabela_mun");
			var nome = "muncod_"+id;
			
			//alert(descricao+' - '+id+' - '+nome+' - '+podeExcluir);
			//alert(tabela.rows.length);
			if(tabela.rows.length > 2) {
				//window.opener.document.getElementById("desce_dis").src = '/imagens/seta_baixo.gif';
				//window.opener.document.getElementById("desce_dis").style.cursor = 'pointer';
				//window.opener.document.getElementById("desce_dis").id = 'sobe_dis';
			}
			
			if(document.getElementById(nome).checked == true) {				
				var tamanho = tabela.rows.length;
							
				if(tamanho == 1) {			
					var linha = tabela.insertRow(tamanho);
								
					linha.style.backgroundColor = "#f4f4f4";
				} 
				else {
					var linha = tabela.insertRow(Number(tamanho)-1);
					
					if(tabela.rows[tamanho-2].style.backgroundColor == "rgb(224, 224, 224)") {
						linha.style.backgroundColor = "#f4f4f4";					
					} else {
						linha.style.backgroundColor = "#e0e0e0";					
					}
				}
												
				linha.id = "linha_"+id;
				
									
				var colAcao 	 = linha.insertCell(0);
				var colDescricao = linha.insertCell(1);
				
				colAcao.style.textAlign 	 = "center";

				colAcao.innerHTML 	   = "<span onclick='excluiItem("+id+");'><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
				colDescricao.innerHTML = descricao + "<input type='hidden' name='muncod[]' id='muncod' value='"+id+"'>";
														
			}
			else {	
//				if(podeExcluir) {					
	//				document.getElementById(nome).checked = true;
		//			alert("Existe supervisão cadastrada para esta etapa.");					
			//	}
				//else {
					//alert(id);
					var linha = window.opener.document.getElementById("linha_"+id).rowIndex;
					//alert(linha);
					tabela.deleteRow(linha);

					/*
					if(tabela.rows.length == 4) {
						tabela.deleteRow(3);
						tabela.deleteRow(2);
						tabela.deleteRow(1);
					}
					*/
				//}
			}
		}
		
				
	</script>
	<body>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="100%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Inserir Municípios 
					</label>
				</td>
			</tr>
		</table>
		
		<br/>
		
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<strong>Selecione o(s) Município(s)</strong>
				</td>
			</tr>
			<tr>
				<td>
					<input type="checkbox" value="todos" name="selecionar_todos" id="selecionar_todos" onclick="selecionaTodos();"> <strong>Selecionar Todos</strong>
				</td>
			</tr>
			<?php 
			
				montaPopup($_REQUEST['listauf']);
			?>
			<tr bgcolor="#C0C0C0">
				<td>
					<input type="button" name="ok" value="Ok" onclick="self.close();">
				</td>
			</tr>
		</table>
		
		<script type="text/javascript">
			var tabela = window.opener.document.getElementById("tabela_mun");
			var i, id_linha, check;	
			
			for(i=1; i<tabela.rows.length; i++) {
				id_linha = tabela.rows[i].id;
				id_linha = id_linha.substr(6);
				
				if(document.getElementById("muncod_"+id_linha)) {
					check = document.getElementById("muncod_"+id_linha);
					check.checked = true;
				}					
			}
		</script>
		
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>  
	</body>
</html>