<html>
	<head>
		<title>SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript">	
		var id_especificacoes = new Array();
	</script>
	<body>
		<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data">
		<table width="100%" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td colspan="2" align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
						onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Selecionar as especificações da iniciativa</strong></td>
				</tr>
			</thead>
			<tr>
				<td class="SubTituloDireita" style="width:39.5%;"><b>Especificação:</b></td>
				<td><?
				$espnome = $_REQUEST['espnome'];
				echo campo_texto( 'espnome', 'S', 'S', 'Especificação', 40, 250, '', '','','','','id="espnome"');?></td>
			</tr>
			<tr bgcolor="#c0c0c0">
				<td colspan="2" align="center">
					<input type="button" value="Pesquisar" name="btnPesquisa" id="btnPesquisa" onclick="pesquisaEspecificacao();"/>
					<input type="button" value="Todos" name="btnTodos" id="btnTodos" onclick="mostrarTodos();"/>
				</td>
			</tr>
			<tbody>
				<tr bgcolor="#ffffff" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#ffffff';">
					<td colspan="2">
						<input type="checkbox" id="selecionar_todos" onclick="selecionaTodos();" /><strong>Selecionar Todos</strong>
					</td>
				</tr>
				<?
					$where = ($_REQUEST["espnome"] != "") ? " AND removeacento(e.espnome) ilike removeacento('%".$_REQUEST["espnome"]."%')" : "";
				
					$sql = "SELECT
								e.espid,
								e.espnome,
								CASE WHEN e.espkit = true THEN e.espunidademedida || ' (kit)' ELSE e.espunidademedida END AS unidade
							FROM
								emenda.especificacao e
								inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
							WHERE
								e.espstatus = 'A'
								".$where."
							ORDER BY
								e.espnome";
	
					$dados = $db->carregar($sql);
					
					if($dados) {
						for($i=0; $i<count($dados); $i++) {
							$cor = ($i % 2) ? "#e0e0e0" : "#f4f4f4";
							
							$nome = "especificacao_" . $dados[$i]["espid"];
							
							echo "<script type=\"text/javascript\"> id_especificacoes.push('".$nome."'); </script>
								  <tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
									<td colspan=\"2\">
										<input type=\"checkbox\" id=\"".$nome."\" name=\"".$dados[$i]["unidade"]."\" value=\"".$dados[$i]["espnome"]."\" onclick=\"selecionaEspecificacao(".$dados[$i]["espid"].", '".$dados[$i]["espnome"]."', '".$dados[$i]["unidade"]."');\" />".$dados[$i]["espnome"]."
									</td>
								  </tr>";
						}
					}
				?>
				<tr bgcolor="#c0c0c0" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#c0c0c0';">
					<td align="center" colspan="2">
						<input type="button" value="Fechar" onclick="javascript:self.close();" />
					</td>
				</tr>
			</tbody>
		</table>
		</form>
	</body>
</html>

<script type="text/javascript">

function pesquisaEspecificacao(){
	if( document.getElementById('espnome').value == '' ){
		alert('Informe o nome da especificação para realizar a consulta!');
		document.getElementById('espnome').focus();
		return false;
	}
	document.getElementById('formulario').submit();
}

function mostrarTodos(){
	window.location.href = window.location;
}

var tabela 	= window.opener.document.getElementById("tb_especificacoes");

var k, id_linha, check;
			
for(k=1; k<tabela.rows.length; k++) {
	id_linha = tabela.rows[k].id;
	id_linha = id_linha.substr(17);
	
	check = document.getElementById("especificacao_"+id_linha);
	check.checked = true;
}

function selecionaEspecificacao(id, nome, unidade) {
	if(document.getElementById("especificacao_" + id).checked == true) {
		if(verificaSelecaoTodos()) document.getElementById("selecionar_todos").checked = true;
		
		var tamanho = tabela.rows.length;
		var linha 	= tabela.insertRow(1);
		
		linha.id	= "tr_especificacao_"+id;
		
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
		var colEspecificacao 	= linha.insertCell(1);
		var colUnidade 			= linha.insertCell(2);
		
		colExcluir.style.textAlign 			= "center";
		colEspecificacao.style.textAlign 	= "center";
		colUnidade.style.textAlign 			= "center";
		
		colExcluir.innerHTML 		= "<input type=\"hidden\" name=\"espid[]\" value=\"" + id + "\" />" +
		                              "<a href=\"#\" onclick=\"excluirEspecificacao(this.parentNode.parentNode.rowIndex);\" ><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\"></a>";
		colEspecificacao.innerHTML 	= nome;
		colUnidade.innerHTML 		= unidade;
	}
	else {
		if(document.getElementById("selecionar_todos").checked == true) document.getElementById("selecionar_todos").checked = false;
	
		var linha = window.opener.document.getElementById("tr_especificacao_"+id).rowIndex;
		tabela.deleteRow(linha);
	}
}

function selecionaTodos() {
	var i, especificacoes, id, nome, unidade;
				
	for(i=0; i<id_especificacoes.length; i++) {
		especificacoes = document.getElementById(id_especificacoes[i]);
		
		if( (document.getElementById("selecionar_todos").checked == true) && (especificacoes.checked == false) ) {
			especificacoes.checked = true;
			
			id 		= especificacoes.id.substr(14);
			nome 	= especificacoes.value;
			unidade = especificacoes.name;
			
			selecionaEspecificacao(id, nome, unidade);
		} 
		else if( (document.getElementById("selecionar_todos").checked == false) && (especificacoes.checked == true)) {
			especificacoes.checked = false;
			
			id 		= especificacoes.id.substr(14);
			nome 	= especificacoes.value;
			unidade = especificacoes.name;
			
			selecionaEspecificacao(id, nome, unidade);
		}		
	}
}

function verificaSelecaoTodos() {
	var i, retorno = true;
	
	for(i=0; i<id_especificacoes.length; i++) {
		especificacoes = document.getElementById(id_especificacoes[i]);
		
		if(especificacoes.checked == false) retorno = false;
	}
	
	return retorno;
}

</script>