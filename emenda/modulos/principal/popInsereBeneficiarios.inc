<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript">	
		var id_beneficiarios = new Array();
	</script>
	<body>
		<table width="100%" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Selecionar os beneficiários da iniciativa</strong></td>
				</tr>
			</thead>
			<tbody>
				<tr bgcolor="#ffffff" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#ffffff';">
					<td>
						<input type="checkbox" id="selecionar_todos" onclick="selecionaTodos();" /><strong>Selecionar Todos</strong>
					</td>
				</tr>
				<?
					$sql = "SELECT
								ben.benid,
								ben.bennome
							FROM
								emenda.beneficiario ben
							WHERE
								ben.benstatus = 'A'
							ORDER BY
								ben.bennome";
					$dados = $db->carregar($sql);
					
					if($dados) {
						for($i=0; $i<count($dados); $i++) {
							$cor = ($i % 2) ? "#e0e0e0" : "#f4f4f4";
							
							$nome = "beneficiario_" . $dados[$i]["benid"];
							
							echo "<script type=\"text/javascript\"> id_beneficiarios.push('".$nome."'); </script>
								  <tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
									<td>
										<input type=\"checkbox\" id=\"".$nome."\" value=\"".$dados[$i]["bennome"]."\" onclick=\"selecionaBeneficiario(".$dados[$i]["benid"].", '".$dados[$i]["bennome"]."');\" />".$dados[$i]["bennome"]."
									</td>
								  </tr>";
						}
					}
				?>
				<tr bgcolor="#c0c0c0" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#c0c0c0';">
					<td align="center">
						<input type="button" value="Fechar" onclick="javascript:self.close();" />
					</td>
				</tr>
			</tbody>
		</table>
	</body>
</html>

<script type="text/javascript">

var tabela 	= window.opener.document.getElementById("tb_beneficiarios");

var k, id_linha, check;
			
for(k=1; k<tabela.rows.length; k++) {
	id_linha = tabela.rows[k].id;
	id_linha = id_linha.substr(16);
	
	check = document.getElementById("beneficiario_"+id_linha);
	check.checked = true;					
}

function selecionaBeneficiario(id, nome) {
	if(document.getElementById("beneficiario_" + id).checked == true) {
		if(verificaSelecaoTodos()) document.getElementById("selecionar_todos").checked = true;
		
		var tamanho = tabela.rows.length;
		var linha 	= tabela.insertRow(1);
		
		linha.id	= "tr_beneficiario_"+id;
		
		if(tamanho == 1) {
			linha.style.backgroundColor = "#f4f4f4";
		} 
		else {
			if(tabela.rows[2].style.backgroundColor == "rgb(224, 224, 224)" || tabela.rows[2].style.backgroundColor == "#e0e0e0") {
				linha.style.backgroundColor = "#f4f4f4";					
			} else {
				linha.style.backgroundColor = "#e0e0e0";					
			}
		}
		
		var colExcluir 			= linha.insertCell(0);
		var colBeneficiarios 	= linha.insertCell(1);
		var colObrigatorio	 	= linha.insertCell(2);
		
		colExcluir.style.textAlign 			= "center";
		colBeneficiarios.style.textAlign 	= "center";
		colObrigatorio.style.textAlign 		= "center";
		
		colExcluir.innerHTML 		= "<input type=\"hidden\" name=\"benid[]\" value=\"" + id + "\" />" +
		                              "<a href=\"#\" onclick=\"excluirBeneficiario(this.parentNode.parentNode.rowIndex);\" ><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\"></a>";
		colBeneficiarios.innerHTML 	= nome;
		colObrigatorio.innerHTML 	= "<input type=\"checkbox\" id=\"icbobrigatorio\" name=\"icbobrigatorio[" + id + "]\" value=\"t\"/>";
	}
	else {
		if(document.getElementById("selecionar_todos").checked == true) document.getElementById("selecionar_todos").checked = false;
	
		var linha = window.opener.document.getElementById("tr_beneficiario_"+id).rowIndex;
		tabela.deleteRow(linha);
	}
}

function selecionaTodos() {
	var i, beneficiarios, id, nome;
				
	for(i=0; i<id_beneficiarios.length; i++) {
		beneficiarios = document.getElementById(id_beneficiarios[i]);
						
		if( (document.getElementById("selecionar_todos").checked == true) && (beneficiarios.checked == false) ) {
			beneficiarios.checked = true;
			
			id 		= beneficiarios.id.substr(13);
			nome 	= beneficiarios.value;
			
			selecionaBeneficiario(id, nome);
		} 
		else if( (document.getElementById("selecionar_todos").checked == false) && (beneficiarios.checked == true)) {
			beneficiarios.checked = false;
			
			id 		= beneficiarios.id.substr(13);
			nome 	= beneficiarios.value;
			
			selecionaBeneficiario(id, nome);
		}			
	}
}

function verificaSelecaoTodos() {
	var i, retorno = true;
	
	for(i=0; i<id_beneficiarios.length; i++) {
		beneficiarios = document.getElementById(id_beneficiarios[i]);
		
		if(beneficiarios.checked == false) retorno = false;
	}
	
	return retorno;
}

</script>