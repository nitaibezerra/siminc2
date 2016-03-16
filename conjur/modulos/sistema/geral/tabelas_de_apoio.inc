<?php

include APPRAIZ . 'includes/cabecalho.inc';
include 'funcoes_tabelas_de_apoio.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "CONJUR";
monta_titulo( $titulo_modulo, 'Tabelas de Apoio' );

$codigoTabela = "";

if(isset($_REQUEST["tabelas"]) || isset($_REQUEST["nomeTabelaDados"])) {
	$tabela = $_REQUEST["nomeTabelaDados"];
	//dump($_REQUEST["enviado"]).die;
	if(isset($_REQUEST["nomeTabelaDados"])) {	
		$descricao = $_REQUEST["descricaoTabelaDados"];	
		//primeiro a ser enviado para as funções de inserção	
		$retornoTabelaID = $_REQUEST["retornoTabelaDadosID"];
		//primeiro a ser enviado para as funções de inserção
		$retornoTabelaDescricao = $_REQUEST["retornoTabelaDadosDescricao"];
		$retornoTabelaID = explode(';', $retornoTabelaID);
		$retornoTabelaDescricao = explode(';', $retornoTabelaDescricao);
		
		if($tabela == "tipoassunto")
			insereDadosTipoAssunto($retornoTabelaID, $retornoTabelaDescricao);
		
		if($tabela == "procedencia"){
			//segundo campo a ser enviado para as funções de inserção
			$retornoTabelaDescricaoServicos = $_REQUEST["retornoTabelaDadosDescricaoServicos"];
			$retornoTabelaDescricaoServicos = explode(';', $retornoTabelaDescricaoServicos);
			insereDadosProcedencia($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos);			
		}
		echo("<script type=\"text/javascript\">
				alert('Dados gravados com sucesso.');	
				location.href = window.location;
			</script>");		
		$_REQUEST["enviado"] = '0';
	}
	else if(isset($_REQUEST["tabelas"])) {
		$tabela = $_REQUEST["tabelas"];
		$descricao = $_REQUEST["descricao_tabela"];
		echo("<script type=\"text/javascript\">				
				//submeteTabelas();
				//location.href = window.location+'&descricaoTabelaDados=".$_REQUEST[$tabela]."';			
			</script>");		
	}	
	$resultado = retornaSelectMontaTabela($tabela);	
	$count = 0;
	while($dados = pg_fetch_array($resultado)) {
		$count++;
		if($count % 2)
			$cor = "#f4f4f4";
		else
			$cor = "#e0e0e0";
			
		if(podeExcluir($tabela, $dados['codigo']))
			$excluir = "excluiItem(this.parentNode.parentNode.rowIndex);";
		else
			$excluir = "alert('Este registro não pode ser excluído pois está sendo usado em outra tabela.');";
		

			if ($tabela == "procedencia"){	

				$sql = "SELECT 
						unpid as codigo,
						unpdsc as descricao 
					FROM 
						conjur.unidadeprocedencia
					WHERE
						undpstatus = 'A'
					ORDER BY unpid
					";	
				$hidden_class = "";
				$dados_class =  $db->carregar($sql);
				
				foreach ($dados_class as $dado_class) {
					$hidden_class .=  $dado_class['codigo'].";".$dado_class['descricao']."|";
				}
				echo("<input type='hidden' id='hidden_class' name='hidden_class' value='".$hidden_class."'>");				
				$aux = "";
				foreach ( $dados_class as $dado_class)	{					
					$aux .="<option value='".$dado_class['codigo']."' ". ($dados['idunidade'] == $dado_class['codigo'] ? 'selected=selected' : '').">".$dado_class['descricao']."</option>";					
				}					
				
				$codigoTabela .= "<tr style=\"background-color:".$cor."\">
									<td align=\"center\">
										<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraItem(this.parentNode.parentNode.rowIndex);\" title=\"Altera o item\">
										<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
										<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
									</td>
									<td align=\"left\">".$dados['descricao']."</td>
									<td align=\"center\">
										<select class='CampoEstilo' id='descricaoServicos_".$dados['codigo']."' name='descricaoServicos'>
											".$aux."
										</select>
									</td>
								 </tr>";
										
			}else{
				$codigoTabela .= "<tr style=\"background-color:".$cor."\">
									<td align=\"center\">
										<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraItem(this.parentNode.parentNode.rowIndex);\" title=\"Altera o item\">
										<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
										<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
									</td>
									<td align=\"left\">".$dados['descricao']."</td>
								 </tr>";			
		}
	}
}

?>

<script type="text/javascript">
	var ItemEmAlteracao = 0;
	var idItemEmAlteracao = '';
	var limite = 200;
	
	function submeteTabelasApoio() {
		var select = document.getElementById('tabelas');
				
		if(select.value != "") {
			document.getElementById('descricao_tabela').value = select.options[select.selectedIndex].innerHTML;
			document.getElementById('formTabelasApoio').submit();
		}
	}

	function submeteTabelas() {
		var select = document.getElementById('nomeTabelaDados');
				
		if(select.value != "") {
			document.getElementById('descricao_tabela').value = select.options[select.selectedIndex].innerHTML;
			document.getElementById('formTabelasApoio').submit();
		}
	}
	
	function insereItem() {
	   if(ItemEmAlteracao == 0) {
		   
		  var descricao = document.getElementById('descricaoNovoItem');		  
		  
		  if(descricao.value != "") {		
			var tabela = document.getElementById('tabelaDados');
			
			var linha = tabela.insertRow(2);
			
			if(tabela.rows[3].style.backgroundColor == "rgb(224, 224, 224)") {
				linha.style.backgroundColor = "#f4f4f4";					
			} else {
				linha.style.backgroundColor = "#e0e0e0";					
			}
			
			var colAcao = linha.insertCell(0);
			var colDescricao = linha.insertCell(1);
			
			colAcao.style.textAlign = "center";
			colDescricao.style.textAlign = "left";
							
			colAcao.innerHTML = "<img src='/imagens/alterar.gif' style='cursor:pointer;' onclick='alteraItem(this.parentNode.parentNode.rowIndex);' title='Altera o item'>&nbsp;<img src='/imagens/excluir.gif' style='cursor:pointer;' onclick='excluiItem(this.parentNode.parentNode.rowIndex);' title='Exclui o item'><input type='hidden' id='item' name='item' value='valoritem_xx'>";								
			colDescricao.innerHTML = descricao.value;
			
			<? if($tabela == "itenscomposicao") { ?>
			var colDescricaoServicos = linha.insertCell(2);
			colDescricaoServicos.style.textAlign = "center";
			colDescricaoServicos.innerHTML = "<textarea class='txareanormal' cols='70' rows='2' name='descricaoServicos' id='descricaoServicos'></textarea>";
			<? } ?>
			
			<? if($tabela == "programafonte") {?>

			var orgaoNovoItem = document.getElementById('orgaoNovoItem');			
			var colDescricaoServicos = linha.insertCell(2);
			colDescricaoServicos.style.textAlign = "center";		
			colDescricaoServicos.innerHTML =   '<select class="CampoEstilo" id="descricaoServicos" name="descricaoServicos">' 
											 + '<option value="1" '+( orgaoNovoItem.value == 1 ? 'selected="selected"' : '')+' >Educação Superior</option>'
											 + '<option value="2" '+( orgaoNovoItem.value == 2 ? 'selected="selected"' : '')+' >Educação Técnica</option>'
											 + '<option value="3" '+( orgaoNovoItem.value == 3 ? 'selected="selected"' : '')+' >Educação Básica</option>'
											 + '</select>';
			<? } ?>

			<? if($tabela == "procedencia") {?>

			var orgaoNovoItem = document.getElementById('classeNovoItem');			
			var colDescricaoServicos = linha.insertCell(2);
			var registro = document.getElementById('hidden_class').value;	
			var select = '';		
			registro = registro.split('|');

			colDescricaoServicos.style.textAlign = "center";	
			select =   '<select class="CampoEstilo" id="descricaoServicos" name="descricaoServicos">';
				
			for ( i = 0; i < (registro.length - 1); i++){

				var dado = registro[i].split(';');				
				
				select = select + '<option value="'+dado[0]+'" '+( orgaoNovoItem.value == dado[0] ? 'selected="selected"' : '')+' >'+dado[1]+'</option>';
			}

			select = select +  '</select>';
			
			colDescricaoServicos.innerHTML = select;
			<? } ?>
						
			descricao.value = "";
			orgaoNovoItem.value = 1;
		  }
		  else {
		  	alert("A 'Descrição' deve ser informada.");
		  }
	   }
	   else {
	   	 alert("Você deve concluir a alteração do item.");
	   	 document.getElementById(idItemEmAlteracao).focus();
	   }
	}

	function insereItemProgramaTipologia(){		 	
			 	
			var tabela = document.getElementById('tabelaDados');
			
			var linha = tabela.insertRow(2);
			
			if(tabela.rows[3].style.backgroundColor == "rgb(224, 224, 224)") {
				linha.style.backgroundColor = "#f4f4f4";					
			} else {
				linha.style.backgroundColor = "#e0e0e0";					
			}
			
			var colAcao = linha.insertCell(0);
						
			colAcao.style.textAlign = "center";										
			colAcao.innerHTML = "<img src='/imagens/excluir.gif' style='cursor:pointer;' onclick='excluiItem(this.parentNode.parentNode.rowIndex);' title='Exclui o item'><input type='hidden' id='item' name='item' value='valoritem_xx'>";								
								
			var colPrograma = linha.insertCell(1);
			var colTipologia = linha.insertCell(2);

			colPrograma.style.textAlign = "center";
			colTipologia.style.textAlign = "center";				
			
			var programaNovoItem = document.getElementById('programaNovoItem');
			var tipologiaNovoItem = document.getElementById('tipologiaNovoItem');				
			
			var registroPrograma = document.getElementById('hidden_programa').value;
			var registroTipologia = document.getElementById('hidden_tipologia').value;				
				
			var selectPrograma = '';
			var selectTipologia = '';	
			registroPrograma = registroPrograma.split('|');
			registroTipologia = registroTipologia.split('|');			
			
			selectPrograma =   '<select class="CampoEstilo" id="programaNovoItem" name="programaNovoItem">';
			for ( i = 0; i < (registroPrograma.length - 1); i++){

				var dado = registroPrograma[i].split(';');	
				
				selectPrograma = selectPrograma + '<option value="'+dado[0]+'" '+( programaNovoItem.value == dado[0] ? 'selected="selected"' : '')+' >'+dado[1]+'</option>';
			}

			selectPrograma = selectPrograma +  '</select>';

			selectTipologia =  '<select class="CampoEstilo" id="tipologiaNovoItem" name="tipologiaNovoItem">';
			for ( i = 0; i < (registroTipologia.length - 1); i++){

				var dado = registroTipologia[i].split(';');				
				
				selectTipologia = selectTipologia + '<option value="'+dado[0]+'" '+( tipologiaNovoItem.value == dado[0] ? 'selected="selected"' : '')+' >'+dado[1]+'</option>';
			}

			selectTipologia = selectTipologia +  '</select>';
			
			colPrograma.innerHTML = selectPrograma;
			colTipologia .innerHTML = selectTipologia;
								
			programaNovoItem.value = 1;
			tipologiaNovoItem.value = 1;
			
	}
	
	function alteraItem(linha) {
	  if(ItemEmAlteracao == 0) {
		var tabela = document.getElementById("tabelaDados");		
		var cels = tabela.rows[linha].cells;
		
		var descItem = cels[1].innerHTML;
		cels[1].innerHTML = "&nbsp;&nbsp;&nbsp;<input class='normal' type='text' id='ok_" + linha + "' size='60' value='" + descItem + "'>&nbsp;<input type='button' value='OK' onclick='concluiAlteracaoItem(" + linha + ");'>";
		
		idItemEmAlteracao = 'ok_' + linha;
		ItemEmAlteracao++;
	  }
	  else {
	  	alert("Você deve concluir a alteração do item.");
	  	document.getElementById(idItemEmAlteracao).focus();
	  }
	}
	
	function concluiAlteracaoItem(linha) {
		var tabela = document.getElementById("tabelaDados");		
		var cels = tabela.rows[linha].cells;
		var valor = cels[1].getElementsByTagName("input")[0].value;
		
		if(valor == "") {
			alert("A 'Descrição' deve ser informada.");
			document.getElementById("ok_"+linha).focus();
		}
		else {
			cels[1].innerHTML = valor;
			ItemEmAlteracao--;
			idItemEmAlteracao = '';
		}
	}
	
	function excluiItem(linha) {		
		if(ItemEmAlteracao == 0) {
			var tabela = document.getElementById("tabelaDados");
			tabela.deleteRow(linha);
		}
		else {
			alert("Você deve concluir a alteração do item.");
	  		document.getElementById(idItemEmAlteracao).focus();
		}
	}
	
	function submeteTabelaDados() {
		if(ItemEmAlteracao == 0) {
			var tabela = document.getElementById("tabelaDados");
			var cels, sub, valor, retornoID = '', retornoDesc = '', retornoDescServicos = '';
			
			for(var i=2; i < ((tabela.rows.length) - 1); i++) {
				valor = '';
				cels = tabela.rows[i].cells;
				
				sub = cels[0].innerHTML;				
				sub = sub.substr(sub.search(/valoritem_/), 13);
				//itens novos
				if((sub.charAt(10) == 'x') && (sub.charAt(11) == 'x')) {
					valor = 'xx';
				}
				else {
					for(var z=0; z < sub.length; z++) {
						if(!isNaN(sub.charAt(z)))
							valor = valor + sub.charAt(z);
					}
				}				
				if(i == 2) {
					retornoID = valor;
					//primeiro campo a ser enviado para as funções de inserção
					retornoDesc = cels[1].innerHTML;				
					<? if( $tabela == "procedencia") { ?>
					//segundo campo a ser enviado para as funções de inserção
					retornoDescServicos = cels[2].getElementsByTagName("select")[0].value;
					<? } ?>
				}
				else {
					retornoID = retornoID + ";" + valor;
					//primeiro campo a ser enviado para as funções de inserção
					retornoDesc = retornoDesc + ";" + cels[1].innerHTML;
				
					<? if( $tabela == "procedencia") { ?>
					//segundo campo a ser enviado para as funções de inserção
					retornoDescServicos = retornoDescServicos + ";" + cels[2].getElementsByTagName("select")[0].value;
					<? } ?>
				}
			}			
			document.getElementById('retornoTabelaDadosID').value = retornoID;
			document.getElementById('retornoTabelaDadosDescricao').value = retornoDesc;
			<? if( $tabela == "procedencia") { ?>
			document.getElementById('retornoTabelaDadosDescricaoServicos').value = retornoDescServicos;
			<? } ?>
			//document.formTabelasApoio.enviado.value = '1';
			//document.formTabelasApoioDados.enviado.value = '1';
			document.getElementById('formTabelasApoioDados').submit();
		}
		else {
			alert("Você deve concluir a alteração do item.");
	  		document.getElementById(idItemEmAlteracao).focus();
		}
	}
	function submeteProgramaTipologia() {
		
		var tabela = document.getElementById("tabelaDados");
		var cels, sub, valor, dados = '';
		
		for(var i=2; i < ((tabela.rows.length) - 1); i++) {
			valor = '';
			cels = tabela.rows[i].cells;
			var nome = cels[1].getElementsByTagName("select")[0].name;		
			//itens novos			
			if(nome == 'programaNovoItem') {
				valor = 'n';
			}
			else {
				nome = nome.substr(13);
				valor = nome;
			}					
			dados = dados + valor;
			dados = dados + ";" + cels[1].getElementsByTagName("select")[0].value;
			dados = dados + ";" + cels[2].getElementsByTagName("select")[0].value + "|";		
			
		}		
		document.getElementById('dados').value = dados;	
		//document.formTabelasApoio.enviado.value = '1';
		//document.formTabelasApoioDados.enviado.value = '1';
		document.getElementById('formTabelasApoioDados').submit();		
	}	
</script>
<form method="post" name="formTabelasApoio" id="formTabelasApoio">
<input type="hidden" id="enviado" name="enviado" value="0">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Selecione a tabela:</td>
			<td>
				<select class="CampoEstilo" id="tabelas" name="tabelas" onChange="submeteTabelasApoio();">
					<option value="">-- Selecione uma Tabela --</option>
					<option value="tipoassunto">Tema</option>
					<option value="procedencia">Procedência</option>					
				</select>
				<input type="hidden" id="descricao_tabela" name="descricao_tabela">
			</td>
		</tr>
	</table>
</form>

<?php if(isset($_REQUEST["tabelas"]) || isset($_REQUEST["nomeTabelaDados"])) { ?>
	<form method="post" name="formTabelasApoioDados" id="formTabelasApoioDados">
		<input type="hidden" value="<?= $tabela ?>" id="nomeTabelaDados" name="nomeTabelaDados">
		<input type="hidden" value="<?= $descricao ?>" id="descricaoTabelaDados" name="descricaoTabelaDados">
		<input type="hidden" id="retornoTabelaDadosDescricao" name="retornoTabelaDadosDescricao">
		<input type="hidden" id="retornoTabelaDadosID" name="retornoTabelaDadosID">
		<input type="hidden" id="dados" name="dados">
		<input type="hidden" id="enviado" name="enviado" value="0">
		
		<? if( $tabela == "procedencia" ) { ?>
			<input type="hidden" id="retornoTabelaDadosDescricaoServicos" name="retornoTabelaDadosDescricaoServicos">
		<? } ?>
	
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td><?=$descricao ?></td>
			</tr>
			<tr>
				<td>
					<table id="tabelaDados" width="90%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
						<thead>															
							<? if($tabela == "procedencia") { ?>
							<tr>
								<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
								<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
								<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade</strong></td>
							</tr>
							<tr>
								<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" onclick="insereItem();" title="Inclui um novo item"></td>
								<td width="45%" valign="top" align="left" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">&nbsp;&nbsp;&nbsp;<input type="text" size="60" id="descricaoNovoItem" class="normal"></td>
								<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">								
									<select class='CampoEstilo' id='classeNovoItem' name='classeNovoItem'>
										<?php 
										$sql = "SELECT 
													unpid as codigo,
													unpdsc as descricao 
												FROM 
													conjur.unidadeprocedencia
												WHERE
													undpstatus = 'A'
													";											
										$dados_class =  $db->carregar($sql);
										foreach ( $dados_class as $dado_class)	{					
											echo("<option value='".$dado_class['codigo']."' ". ($dados['idunidade'] == $dado_class['codigo'] ? 'selected=selected' : '').">".$dado_class['descricao']."</option>");					
										}			
										?>	
									</select>
								</td>
							</tr>
							<? } else { ?>
							<tr>
								<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
								<td width="95%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
							</tr>
							<tr>
								<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" onclick="insereItem();" title="Inclui um novo item"></td>
								<td width="95%" valign="top" align="left" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">&nbsp;&nbsp;&nbsp;<input type="text" size="60" id="descricaoNovoItem" class="normal"></td>
							</tr>
							<? } ?>								
						</thead>
						<?= $codigoTabela ?>
						<tr bgcolor="#C0C0C0">
							<td></td>
							<td colspan="2">
								<div style="float: left;">
								<?php
								$submeter = ""; 
								if($tabela == "programatipologia"){
									$submeter = "onclick='submeteProgramaTipologia();'"; 
								}else{
									$submeter = "onclick='submeteTabelaDados();'"; 
								}
								?>
									<input type="button" value="Salvar" style="cursor: pointer" <?=$submeter; ?>">
									<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
								</div>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
	</form>
<? } ?>