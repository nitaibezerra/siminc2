<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript">	
		var id_responsavel = new Array();
	</script>
	<body>
		<input type="hidden" id="tipo" value="<?=$_REQUEST['tipo']; ?>">
		<table width="100%" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Selecionar os responsável da iniciativa</strong></td>
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
							  resid,
							  resdsc,
							  resassunto
							FROM 
							  emenda.responsavel
							WHERE
								resstatus = 'A'
							ORDER BY
								resdsc";
					$dados = $db->carregar($sql);
					
					if($dados) {
						for($i=0; $i<count($dados); $i++) {
							$cor = ($i % 2) ? "#e0e0e0" : "#f4f4f4";
							
							$nome = "responsavel_" . $dados[$i]["resid"];
							$assunto = "responsavel_assunto_" . $dados[$i]["resid"];
							$responsavel = "responsavel_resid_" . $dados[$i]["resid"];
							
							echo "<input type=\"hidden\" name=\"$assunto\" id=\"$assunto\" value=\"".$dados[$i]["resassunto"]."\" />";
							
							echo "<script type=\"text/javascript\"> id_responsavel.push('".$nome."'); </script>
								  <tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
									<td>
										<input type=\"checkbox\" id=\"".$nome."\" value=\"".$dados[$i]["resdsc"]."\" onclick=\"selecionaResponsavel(".$dados[$i]["resid"].", '".$dados[$i]["resdsc"]."', '".$dados[$i]["resassunto"]."');\" />".$dados[$i]["resdsc"]."
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

var tabela 	= window.opener.document.getElementById("tb_responsavel");
var k, id_linha, check;
var id_select = 0;
			
for(k=1; k<tabela.rows.length; k++) {
	id_linha = tabela.rows[k].id;
	id_linha = id_linha.substr(15);
	
	check = document.getElementById("responsavel_"+id_linha);
	check.checked = true;					
}

function selecionaResponsavel(id, nome, assunto) {
	if(document.getElementById("responsavel_" + id).checked == true) {
		if(verificaSelecaoTodos()) document.getElementById("selecionar_todos").checked = true;
		
		var tamanho = tabela.rows.length;
		var linha 	= tabela.insertRow(1);
		
		linha.id	= "tr_responsavel_"+id;
		
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
		
		var colExcluir 		= linha.insertCell(0);
		var colResponsavels = linha.insertCell(1);
		var colAssunto  	= linha.insertCell(2);
		
		colExcluir.style.textAlign 		= "center";
		colResponsavels.style.textAlign = "center";
		colAssunto.style.textAlign 		= "center";
		
		colExcluir.innerHTML	  = "<input type=\"hidden\" name=\"resid[]\" value=\""+id+"\" /> <input type=\"hidden\" name=\"ereid[]\" value=\"\" />" +
		                            "<a href=\"#\" onclick=\"excluirResponsavel(this.parentNode.parentNode.rowIndex);\" ><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\"></a>";
		colResponsavels.innerHTML = nome;
		colAssunto.innerHTML 	  = assunto;
	}
	else {
		if(document.getElementById("selecionar_todos").checked == true) document.getElementById("selecionar_todos").checked = false;
	
		var linha = window.opener.document.getElementById("tr_responsavel_"+id).rowIndex;
		tabela.deleteRow(linha);
	}
	
	if( document.getElementById("tipo").value != 'especificacao' ){	
		if( id_select == '' ){
			id_select = id;
		} else {
			id_select = id_select + ", " + id ;
		}
		window.opener.document.getElementById("tipo_ensino").style.display = 'none';
		window.opener.alteraTipoEnsino(id_select);
	}
}

function selecionaTodos() {
	var i, responsavel, id, nome, assunto;
				
	for(i=0; i<id_responsavel.length; i++) {
		responsavel = document.getElementById(id_responsavel[i]);
		
		if( (document.getElementById("selecionar_todos").checked == true) && (responsavel.checked == false) ) {
			responsavel.checked = true;
			
			id 		= responsavel.id.substr(12);
			nome 	= responsavel.value;
			assunto = document.getElementById("responsavel_assunto_"+id+"").value;
			
			selecionaResponsavel(id, nome, assunto);
		} 
		else if( (document.getElementById("selecionar_todos").checked == false) && (responsavel.checked == true)) {
			responsavel.checked = false;
			
			id 		= responsavel.id.substr(13);
			nome 	= responsavel.value;			
			assunto = document.getElementById("responsavel_assunto_"+id+"").value;
			
			selecionaResponsavel(id, nome, assunto);
		}			
	}
}

function verificaSelecaoTodos() {
	var i, retorno = true;
	
	for(i=0; i<id_responsavel.length; i++) {
		responsavel = document.getElementById(id_responsavel[i]);
		
		if(responsavel.checked == false) retorno = false;
	}
	
	return retorno;
}

</script>