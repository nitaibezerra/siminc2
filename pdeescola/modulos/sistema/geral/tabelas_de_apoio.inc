<?php
include APPRAIZ . 'includes/cabecalho.inc';
include 'funcoes_tabelas_de_apoio.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "PDE Escola";
monta_titulo( $titulo_modulo, 'Tabelas de Apoio' );
$codigoTabela = "";

if(isset($_REQUEST["tabelas"]) || isset($_REQUEST["nomeTabelaDados"])) {
	if(isset($_REQUEST["nomeTabelaDados"])) {
		$tabela 				= $_REQUEST["nomeTabelaDados"];
		$descricao 				= $_REQUEST["descricaoTabelaDados"];
		$retornoTabelaID 		= $_REQUEST["retornoTabelaDadosID"];
		$retornoTabelaDescricao = $_REQUEST["retornoTabelaDadosDescricao"];

		$retornoTabelaID = explode(';', $retornoTabelaID);
		$retornoTabelaDescricao = explode(';', $retornoTabelaDescricao);

		if($tabela == "tiponivelmodalidadeensino")
			insereDadosTipoNivelModalidaDeEnsino($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "tipodependencia")
			insereDadosTipoDependencia($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "seriecicloescolar")
			insereDadosSerieCicloEscolar($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "tipodisciplina")
			insereDadosTipoDiciplina($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "faixaetaria")
			insereDadosFaixaetaria($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "tipoturno")
			insereDadosTipoTurno($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "cargofuncao")
			insereDadosCargoFuncao($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "tipoesfera")
			insereDadosTipoEsfera($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "fonterecurso")
			insereDadosFonteRecurso($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "conceitoavaliacao")
			insereDadosConceitoAvaliacao($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "tipolocalizacao")
			insereDadosTipoLocalizacao($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "fasepde")
			insereDadosFasePDE($retornoTabelaID, $retornoTabelaDescricao);
	    if($tabela == "tipoprograma")
			insereDadosTipoPrograma($retornoTabelaID, $retornoTabelaDescricao);
		if($tabela == "tipocategoria")
			insereDadosTipoCategoria($retornoTabelaID, $retornoTabelaDescricao);	
			
		echo "<script type=\"text/javascript\">alert('Dados gravados com sucesso.');</script>";
	}
	else if(isset($_REQUEST["tabelas"])) {
		$tabela = $_REQUEST["tabelas"];
		$descricao = $_REQUEST["descricao_tabela"];
	}
	$sql = retornaSelectMontaTabela($tabela);

	$count = 0;
	while(($dados = pg_fetch_array($sql)) != false) {
		$count++;
		if($count % 2)
			$cor = "#f4f4f4";
		else
			$cor = "#e0e0e0";
		if(podeExcluir($tabela, $dados['codigo']))
			$excluir = "excluiItem(this.parentNode.parentNode.rowIndex);";
		else
			$excluir = "alert('Este registro não pode ser excluído pois está sendo usado em outra tabela.');";
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
	
	function insereItem() {
	   if(ItemEmAlteracao == 0) {
		  var descricao = document.getElementById('descricaoNovoItem');
		  document.getElementById('editar').value = 'novo';
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
			
			descricao.value = "";
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
	
	function alteraItem(linha) {
	  if(ItemEmAlteracao == 0) {
		var tabela = document.getElementById("tabelaDados");		
		var cels = tabela.rows[linha].cells;
		document.getElementById('editar').value = 'editar';
		var descItem = cels[1].innerHTML;
		cels[1].innerHTML = "&nbsp;&nbsp;&nbsp;<input type='text' id='ok_" + linha + "' size='60' value='" + descItem + "'>&nbsp;<input type='button' value='OK' onclick='concluiAlteracaoItem(" + linha + ");'>";
		
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
		document.getElementById('editar').value = 'exclui';	
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
	 var descricao = document.getElementById('descricaoNovoItem').value;
	 var editar = document.getElementById('editar').value;
	 if(descricao == '' && editar ==''){
	 	alert('Obrigatorio preencher o campo de descrição');
	 	return false;
	 }else if(descricao !=''){
	 	alert('Clique no ícone de "inclui um novo item" antes de salvar o novo item.');
	 	return false;
	 }
		if(ItemEmAlteracao == 0) {
			var tabela = document.getElementById("tabelaDados");
			var cels, sub, valor, retornoID = '', retornoDesc = '', retornoDescServicos = '';
			
			for(var i=2; i < ((tabela.rows.length) - 1); i++) {
				valor = '';
				cels = tabela.rows[i].cells;
				
				sub = cels[0].innerHTML;				
				sub = sub.substr(sub.search(/valoritem_/), 13);
				
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
					retornoDesc = cels[1].innerHTML;
				}
				else {
					retornoID = retornoID + ";" + valor;
					retornoDesc = retornoDesc + ";" + cels[1].innerHTML;
				}
			}
			document.getElementById('retornoTabelaDadosID').value = retornoID;
			document.getElementById('retornoTabelaDadosDescricao').value = retornoDesc;
			document.getElementById('formTabelasApoioDados').submit();
		}
		else {
			alert("Você deve concluir a alteração do item.");
	  		document.getElementById(idItemEmAlteracao).focus();
		}
	}
</script>
<br>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    
<form method="post" name="formTabelasApoio" id="formTabelasApoio">
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
<tr>
<td class='subTituloDireita'>		
	<strong>Tabela:</strong>
</td>
<td>
	<select id="tabelas"  class='CampoEstilo' name="tabelas" onChange="submeteTabelasApoio();">
		<option value="">-- Selecione uma Tabela --</option>
		<option value="tiponivelmodalidadeensino">Tipo nivel modalidade de ensino</option>
		<option value="tipodependencia">Tipo de Dependência</option>
		<option value="seriecicloescolar">Serie Ciclo Escolar</option>
		<option value="tipodisciplina">Tipo Disciplina</option>
		<option value="faixaetaria">Faixaetaria</option>
		<option value="tipoturno">Tipo Turno</option>
		<option value="cargofuncao">Cargo Função</option>
		<option value="tipoesfera">Tipo Esfera</option>
		<option value="fonterecurso">Fonte Recurso</option>
		<option value="conceitoavaliacao">Conceito Avaliacao</option>
		<option value="tipolocalizacao">Tipo Localizacao</option>
		<option value="fasepde">Fase PDE</option>
		<option value="tipoprograma">Tipo de Programa</option>
		<option value="tipocategoria">Tipo de Categoria</option>
	</select>
	</td>
</tr>
<!-- 
	<tr>
		<td class='subTituloDireita'>
			
		</td>
		<td>
			<input type="button" name="submit" id="submit" value ="Selecione">
		</td>	
	</tr>
 -->
	<input type="hidden" id="descricao_tabela" name="descricao_tabela">
</form>
<br><br>
<?php if(isset($_REQUEST["tabelas"]) || isset($_REQUEST["nomeTabelaDados"])) { ?>
<form method="post" name="formTabelasApoioDados" id="formTabelasApoioDados">
<input type="hidden" value="<?= $tabela ?>" id="nomeTabelaDados" name="nomeTabelaDados">
<input type="hidden" value="<?= $descricao ?>" id="descricaoTabelaDados" name="descricaoTabelaDados">
<input type="hidden" id="retornoTabelaDadosDescricao" name="retornoTabelaDadosDescricao">
<input type="hidden" id="retornoTabelaDadosID" name="retornoTabelaDadosID">
<input type="hidden" id="editar" name="editar">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td>
		<strong>Tabela de apoio:</strong> <?= $descricao ?>
	</td>
</tr>
<tr>
	<td>
		<table id="tabelaDados" width="90%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					<td width="95%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
				</tr>
				<tr>
					<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" onclick="insereItem();" title="Inclui um novo item"></td>
					<td width="95%" valign="top" align="left" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">&nbsp;&nbsp;&nbsp;<input type="text" size="60" id="descricaoNovoItem"></td>
				</tr>
			</thead>
			<?= $codigoTabela ?>
			<tfoot>
				<tr>	
					<td align="center" colspan="2" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<input type="button" value="Salvar" style="cursor: pointer" onclick="submeteTabelaDados();">
						&nbsp;
						<input type="button" value="Fechar" onclick="javascript: history.go(-1);" style="cursor: pointer">
					</td>
				</tr>
			</tfoot>
		</table>
	</td>
</tr>
</table>
</form>
<? }?>