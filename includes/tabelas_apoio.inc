<?

include APPRAIZ . 'includes/cabecalho.inc';

if($_REQUEST["submetido"]) {
	$descricao = $_REQUEST["descricao_tabela"];
	
		$sql_tabela = "SELECT 
							a.attnum AS ordem, 
							a.attname AS campo, 
							t.typname AS tipo, 
							a.attlen AS tamanho, 
							a.atttypmod-4 AS tamanhovar, 
							a.attnotnull AS notnull, 
							con.contype AS chave, 
							c.oid AS id_tabela, 
							nf.nspname AS esquema, 
							c.relname AS tabela, 
							d.description AS descricao, 
							nf.nspname AS esquema_ref, 
							clf.relname AS tabela_ref, 
							af.attname AS coluna_ref,
							af2.attname AS coluna_ref_descricao
						
						FROM 
							pg_catalog.pg_class c, 
							pg_catalog.pg_namespace nf, 
							pg_catalog.pg_type t, 
							pg_catalog.pg_attribute a 
						
						LEFT JOIN 
							pg_catalog.pg_description d ON d.objsubid = a.attnum AND a.attrelid = d.objoid 
						
						LEFT JOIN 
							pg_catalog.pg_constraint con ON con.conrelid = a.attrelid AND con.conkey[1] = a.attnum 
						
						LEFT JOIN 
							pg_catalog.pg_class clf ON con.confrelid = clf.oid AND clf.relkind = 'r' 
						
						LEFT JOIN 
							pg_catalog.pg_namespace nfa ON nfa.oid = clf.relnamespace 
						
						LEFT JOIN 
							pg_catalog.pg_attribute af ON af.attrelid = con.confrelid AND af.attnum = con.confkey[1] 
						
						LEFT JOIN 
							pg_catalog.pg_attribute af2 ON af2.attrelid = con.confrelid AND (af2.attname = (substr(af.attname, 0, 4) || 'dsc') OR af2.attname = (substr(af.attname, 0, 4) || 'desc')) 
							
						WHERE 
							nf.nspname = 'obras' AND 
							c.relname = '".$_REQUEST["tabela"]."' AND 
							a.attnum > 0 AND 
							a.attrelid = c.oid AND 
							a.atttypid = t.oid AND 
							nf.oid = c.relnamespace 
						
						ORDER BY 
							a.attnum";
	
	$dados = $db->carregar($sql_tabela);
	
	//
	$parametros = array();
	$cabecalho = "";
	for($i=0; $i<count($dados); $i++) {
		if($dados[$i]["chave"] != 'p') {
			$cabecalho .= '<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>'.$dados[$i]["campo"].'</strong></td>';
		}
		array_push($parametros, $dados[$i]["campo"]);
	}

	//
	$qry = "SELECT
			  ".implode(",", $parametros)."
	 	   FROM
	 		  obras.".$_REQUEST["tabela"];
	
	$valores = $db->carregar($qry);
	
	//
	if($_REQUEST["salvar"]) {
		dbg($_REQUEST,1);
		$excluidos = $_REQUEST["excluidos"];
		
		for($i=0; $i<count($dados); $i++) {
			if($dados[$i]["chave"] == "p") {
				$campoID = $dados[$i]["campo"];
				$esquema = $dados[$i]["esquema"];
				$tabela = $dados[$i]["tabela"];
			}
		}
		
		// Deleta registros se necessário.
		if($_REQUEST["excluidos"]) {
			$sql_delete = "DELETE FROM ".$esquema.".".$tabela." WHERE ".$campoID." in (".$excluidos.")";
			//$db->carregar($sql_delete);
			dbg($sql_delete);
		}
		
		for($i=0; $i<$_REQUEST["contador"]; $i++) {
			if($_REQUEST[$campoID][$i] == "") {
				continue;
			} else if($_REQUEST[$campoID][$i] == "novo") {
				for($i=0; $i<count($dados); $i++) {
					
				}
				$sql_insert = "INSERT INTO ".$esquema.".".$tabela."
									() 
									VALUES 
									()";
			} else {
				$sql_update = "";
			}
			
			echo "&nbsp;&nbsp;&nbsp;[".$i."] >>> ".$_REQUEST[$campoID][$i]."<br />";
		}
		/*$sql = pg_query("SELECT tobaid FROM obras.tipoobra WHERE tobstatus = 'A'");
		while(($dados = pg_fetch_array($sql)) != false) {
			$tobaid = $dados['tobaid'];
			$cont = 0;
			
			for($i=0; $i < count($retornoTabelaID); $i++) {
				if($tobaid == trim($retornoTabelaID[$i]))
					$cont++;
			}
			
			if($cont == 0) {
				$sql_delete = "DELETE FROM obras.tipoobra WHERE tobaid = ".$tobaid;	
				$db->executar($sql_delete);
				$db->commit();
			}
		}
		
		if(count($retornoTabelaID) > 0) {
			// Executa INSERT's e/ou UPDATE's na tabela. 
			for($i=0; $i < count($retornoTabelaID); $i++) {
				if(trim($retornoTabelaID[$i]) == 'xx') {
					$sql = "INSERT INTO 
								obras.tipoobra(tobadesc,tobstatus,tobdtinclusao) 
							VALUES
								('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
					$db->executar($sql);
					$db->commit();
				} else {
					$sql = "UPDATE 
								obras.tipoobra 
							SET 
								tobadesc = '".trim($retornoTabelaDescricao[$i])."'							 
							WHERE 
								tobaid = ".trim($retornoTabelaID[$i]);
					$db->executar($sql);
					$db->commit();
				}
			}
		}
		?>
			<script type="text/javascript">
				alert("Operação realizada com sucesso!");
			</script>
		<?
	*/
		
		/*if($_REQUEST["itcstatus"][1] == NULL)
			dbg("AUHAUIhAUIHUAHAUHUAHA!!!");
			
		if($_REQUEST["itcstatus"][0] == 'on')
			dbg("ONONONONONONONONNONONO!!!");*/
	}
}

echo "<br>";
monta_titulo( 'SIMEC - Tabelas de Apoio', $_SESSION["sisdsc"] );

?>

<script type="text/javascript">
	var contador = <?= count($valores) ?>;
	var excluidos = new Array();
	
	var campoColuna = new Array();
	var tipoColuna = new Array();
	var chaveColuna = new Array();
	var tamanhovarColuna = new Array();
	var tamanhonumColuna = new Array();
	
	var campoColunaRef = new Array();
	var campoColunaRefDescricao = new Array();
	var esquemaRef = new Array();
	var tabelaRef = new Array();
	
	<? 
		for($i=0; $i<count($dados); $i++) {
			echo "campoColuna[".$i."] = '".$dados[$i]["campo"]."'; ";
			echo "tipoColuna[".$i."] = '".$dados[$i]["tipo"]."'; ";
			echo "chaveColuna[".$i."] = '".$dados[$i]["chave"]."'; ";
			echo "tamanhovarColuna[".$i."] = '".$dados[$i]["tamanhovar"]."'; ";
			echo "tamanhonumColuna[".$i."] = '".$dados[$i]["tamanho"]."'; ";
			
			echo "campoColunaRef[".$i."] = '".$dados[$i]["coluna_ref"]."'; ";
			echo "campoColunaRefDescricao[".$i."] = '".$dados[$i]["coluna_ref_descricao"]."'; ";
			echo "esquemaRef[".$i."] = '".$dados[$i]["esquema_ref"]."'; ";
			echo "tabelaRef[".$i."] = '".$dados[$i]["tabela_ref"]."'; ";
		}
	?>
</script>

<script src="../includes/prototype.js" type="text/javascript"></script>
<script type="text/javascript" src="../includes/calendario.js"></script>
<form method="post" name="formulario" id="formulario">
<input type="hidden" id="submetido" name="submetido" value="1">
<input type="hidden" id="salvar" name="salvar" value="0">
<input type="hidden" id="contador" name="contador" value="">
<input type="hidden" id="excluidos" name="excluidos" value="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Selecione a tabela:</td>
			<td>
				<select class="CampoEstilo" id="tabela" name="tabela" onChange="submeteTabelasApoio('exibir');">
					<option value="">-- Selecione uma Tabela --</option>
					<option value="itenscomposicao" <? if($_REQUEST["tabela"]=="itenscomposicao") { echo "selected='selected'"; } ?>>Etapas da Obra</option>
					<option value="tiporestricao" <? if($_REQUEST["tabela"]=="tiporestricao") { echo "selected='selected'"; } ?>>Tipo de Restrição</option>
					<option value="tiporespcontato" <? if($_REQUEST["tabela"]=="tiporespcontato") { echo "selected='selected'"; } ?>>Tipo de Responsável</option>
					<option value="tipoobra" <? if($_REQUEST["tabela"]=="tipoobra") { echo "selected='selected'"; } ?>>Tipo de Obra</option>
					<option value="unidademedida" <? if($_REQUEST["tabela"]=="unidademedida") { echo "selected='selected'"; } ?>>Unidade de Medida</option>
					<option value="tipoarquivo" <? if($_REQUEST["tabela"]=="tipoarquivo") { echo "selected='selected'"; } ?>>Tipo de Arquivo</option>
					<option value="itensdetalhamento" <? if($_REQUEST["tabela"]=="itensdetalhamento") { echo "selected='selected'"; } ?>>Módulo de Ampliação</option>
					<option value="situacaoobra" <? if($_REQUEST["tabela"]=="situacaoobra") { echo "selected='selected'"; } ?>>Situação da Obra</option>
					<option value="desempenhoconstrutora" <? if($_REQUEST["tabela"]=="desempenhoconstrutora") { echo "selected='selected'"; } ?>>Desempenho da Construtora</option>
					<option value="qualidadeobra" <? if($_REQUEST["tabela"]=="qualidadeobra") { echo "selected='selected'"; } ?>>Qualidade da Obra</option>
					<option value="programafonte" <? if($_REQUEST["tabela"]=="programafonte") { echo "selected='selected'"; } ?>>Programa Fonte</option>
					<option value="tipologiaobra" <? if($_REQUEST["tabela"]=="tipologiaobra") { echo "selected='selected'"; } ?>>Tipologia da Obra</option>
					<option value="programatipologia" <? if($_REQUEST["tabela"]=="programatipologia") { echo "selected='selected'"; } ?>>Programa / Tipologia</option>
				</select>
				<input type="hidden" id="descricao_tabela" name="descricao_tabela" value="<?= $descricao ?>">
			</td>
		</tr>
	</table>
	
<?
if($_REQUEST["submetido"]) {
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3"	align="center">
	<tr>
		<td><?= $descricao ?></td>
	</tr>
	<tr>
		<td>
			<table id="tabelaDados" width="90%" align="center" border="0" cellspacing="2" cellpadding="5" class="listagem">
				<thead>
					<tr>
						<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						<?= $cabecalho ?>
					</tr>
				</thead>
				<tbody>
					<?
						echo '<tr style="background-color: #C0C0C0;">';
						for($i=0; $i<count($dados); $i++) {
							if($dados[$i]["chave"] == 'p') {
						  		echo '<td align="center">
									  	<img src="/imagens/gif_inclui.gif" style="cursor:pointer;" onclick="incluirItem();" title="Inclui um novo item">
									  	<input type="hidden" id="novo_'.$dados[$i]["campo"].'" name="novo_'.$dados[$i]["campo"].'">
									  </td>';
						    }
						    else if($dados[$i]["chave"] == 'f') {
						    	$qry_fk = "SELECT
						    					".$dados[$i]["coluna_ref"]." as codigo,
						    					".$dados[$i]["coluna_ref_descricao"]." as descricao
						    				FROM
						    					".$dados[$i]["esquema_ref"].".".$dados[$i]["tabela_ref"];
						    	
						    	$dados_fk = $db->carregar($qry_fk);
						    	
						  		echo '<td align="center">';
						  		echo '<select id="novo_'.$dados[$i]["campo"].'" name="novo_'.$dados[$i]["campo"].'" class="campoEstilo">';
						  		for($k=0; $k<count($dados_fk); $k++) {
						  			echo '<option value="'.$dados_fk[$k]["codigo"].'">'.$dados_fk[$k]["descricao"].'</option>';
						  		}
						  		echo '</select>';
						  		echo '</td>';
						    }
							else {
								if($dados[$i]["tipo"] == 'timestamp') {
									echo '<td align="center">'.campo_data('novo_'.$dados[$i]["campo"] , 'N', 'S', '', 'S' ).'</td>';
								}
							   	else if($dados[$i]["tipo"] == 'varchar') {
							   		if($dados[$i]["tamanhovar"] > 100)
										echo '<td align="left"><textarea id="novo_'.$dados[$i]["campo"].'" name="novo_'.$dados[$i]["campo"].'" cols="50" rows="3"></textarea></td>';
									else
										echo '<td align="left"><input id="novo_'.$dados[$i]["campo"].'" name="novo_'.$dados[$i]["campo"].'" class="normal" type="text" size="60" maxlentgh="'.$dados[$i]["tamanhovar"].'" /></td>';
								}
								else if($dados[$i]["tipo"] == 'bpchar') {
									echo '<td align="center"><input id="novo_'.$dados[$i]["campo"].'" name="novo_'.$dados[$i]["campo"].'" type="checkbox"></td>';	
								}
								else if($dados[$i]["tipo"] == 'int4') {
									echo '<td align="center"><input id="novo_'.$dados[$i]["campo"].'" name="novo_'.$dados[$i]["campo"].'" class="normal" type="text" size="5" maxlentgh="'.$dados[$i]["tamanho"].'" /></td>';
								}
								else {
									echo '<td align="center">&nbsp;</td>';
								}
							}
						}
						echo '</tr>';
						
						
						$count = 0;
						for($j=0; $j<count($valores); $j++) {
							$count++;
							$cor = ($count % 2) ? "#f4f4f4" : "#e0e0e0";
							
							echo '<tr style="background-color: '.$cor.';">';
							
							for($i=0; $i<count($dados); $i++) {
							    if($dados[$i]["chave"] == 'p') {
							  		echo '<td align="center">
										  	<img src="/imagens/excluir.gif" style="cursor:pointer;" onclick="excluirItem(this.parentNode.parentNode.rowIndex,'.$valores[$j][$dados[$i]["campo"]].');" title="Exclui o item">
										  	<input type="hidden" id="'.$dados[$i]["campo"].'_'.$j.'" name="'.$dados[$i]["campo"].'['.$j.']" value="'.$valores[$j][$dados[$i]["campo"]].'">
										  </td>';
							    }
							    else if($dados[$i]["chave"] == 'f') {
							    	$qry_fk = "SELECT
							    					".$dados[$i]["coluna_ref"]." as codigo,
							    					".$dados[$i]["coluna_ref_descricao"]." as descricao
							    				FROM
							    					".$dados[$i]["esquema_ref"].".".$dados[$i]["tabela_ref"];
							    	
							    	$dados_fk = $db->carregar($qry_fk);
							    	
							  		echo '<td align="center">';
							  		echo '<select id="'.$dados[$i]["campo"].'_'.$j.'" name="'.$dados[$i]["campo"].'['.$j.']" class="campoEstilo">';
							  		for($k=0; $k<count($dados_fk); $k++) {
							  			$selected = ($valores[$j][$dados[$i]["campo"]] == $dados_fk[$k]["codigo"]) ? "selected=\"selected\"" : "";
							  			echo '<option value="'.$dados_fk[$k]["codigo"].'" '.$selected.'>'.$dados_fk[$k]["descricao"].'</option>';
							  		}
							  		echo '</select>';
							  		echo '</td>';
							    }
								else {
									if($dados[$i]["tipo"] == 'timestamp') {
										$valor = $dados[$i]["campo"].'['.$j.']';
										$$valor = $valores[$j][$dados[$i]["campo"]];
										echo '<td align="center">'.campo_data($dados[$i]["campo"].'['.$j.']' , 'N', 'S', '', 'S' ).'</td>';
									}
								   	else if($dados[$i]["tipo"] == 'varchar') {
								   		if($dados[$i]["tamanhovar"] > 100)
											echo '<td align="left"><textarea id="'.$dados[$i]["campo"].'_'.$j.'" name="'.$dados[$i]["campo"].'['.$j.']" cols="50" rows="3">'.$valores[$j][$dados[$i]["campo"]].'</textarea></td>';
										else
											echo '<td align="left"><input id="'.$dados[$i]["campo"].'_'.$j.'" name="'.$dados[$i]["campo"].'['.$j.']" class=\'normal\' type=\'text\' size=\'60\' maxlentgh=\''.$dados[$i]["tamanhovar"].'\' value=\''.$valores[$j][$dados[$i]["campo"]].'\' /></td>';
									}
									else if($dados[$i]["tipo"] == 'bpchar') {
										$checked = ($valores[$j][$dados[$i]["campo"]] == "A") ? "checked=checked" : "";
										echo '<td align="center"><input id="'.$dados[$i]["campo"].'_'.$j.'" name="'.$dados[$i]["campo"].'['.$j.']" type=\'checkbox\' '.$checked.'></td>';
									}
									else if($dados[$i]["tipo"] == 'int4') {
										echo '<td align="center"><input id="'.$dados[$i]["campo"].'_'.$j.'" name="'.$dados[$i]["campo"].'['.$j.']" class=\'normal\' type=\'text\' size=\'5\' maxlentgh=\''.$dados[$i]["tamanho"].'\' value=\''.$valores[$j][$dados[$i]["campo"]].'\' /></td>';
									}
									else {
										echo '<td align="center">&nbsp;</td>';
									}
								}
							}
							
							echo '</tr>';
						}
					?>
				</tbody>
				<tfoot>
					<tr bgcolor="#C0C0C0">
						<td colspan="<?= count($dados) ?>">
							<div style="float: left;">
								<input type="button" value="Salvar" style="cursor: pointer" onclick="submeteTabelasApoio('salvar');">
								<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
							</div>
						</td>
					</tr>
				</tfoot>
			</table>
		</td>
	</tr>
</table>
<?
}
?>
</form>

<script type="text/javascript">

function submeteTabelasApoio(tipo) {
	var select = document.getElementById('tabela');
	
	if(tipo == 'salvar') {
		// Grava no hidden o ID de todos os itens que foram excluídos.
		document.getElementById('excluidos').value = excluidos.join(",");
		
		document.getElementById('salvar').value = "1";
		document.getElementById('contador').value = contador;
		document.getElementById('formulario').submit();	
	}
	else {
		if(select.value != "") {
			document.getElementById('descricao_tabela').value = select.options[select.selectedIndex].innerHTML;
			document.getElementById('formulario').submit();
		}
	}
}

function incluirItem() {
	var tabela = document.getElementById("tabelaDados");
	var linha = tabela.insertRow(2);
	
	if(tabela.rows[3].style.backgroundColor == "rgb(224, 224, 224)") {
		linha.style.backgroundColor = "#f4f4f4";					
	} else {
		linha.style.backgroundColor = "#e0e0e0";					
	}
					
	for(var i=0; i<campoColuna.length; i++) {
		campoNovo = document.getElementById("novo_" + campoColuna[i]);
		
		celula = linha.insertCell(i);
		celula.align = 'center';
		
		if(chaveColuna[i] == 'p') {
			celula.innerHTML = "<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirItem(this.parentNode.parentNode.rowIndex,'novo');\" title=\"Exclui o item\">" +
							   "<input type=\"hidden\" id=\"" + campoColuna[i] + "_" + contador + "\" name=\"" + campoColuna[i] + "[" + contador + "]\" value=\"novo\">";
		}
		else if(chaveColuna[i] == 'f') {
			carregaComboFK(celula, '../geral/ajax/requisicao_ajax_tabelas_apoio.php', 'selecionado='+campoNovo.value+'&coluna='+campoColunaRef[i]+'&colunadesc='+campoColunaRefDescricao[i]+'&esquema='+esquemaRef[i]+'&tabela='+tabelaRef[i]+'&id=' + campoColuna[i] + "_" + contador + '&name='+ campoColuna[i] + '[' + contador + ']');
		}
		else {
			if(tipoColuna[i] == 'varchar') {
				celula.align = 'left';
				
				if(tamanhovarColuna[i] > 100) {
					celula.innerHTML =  "<textarea id=\"" + campoColuna[i] + "_" + contador + "\" name=\"" + campoColuna[i] + "[" + contador + "]\" cols=\"50\" rows=\"3\">" + campoNovo.value + "</textarea>";
				}
				else {
					celula.innerHTML = "<input id=\"" + campoColuna[i] + "_" + contador + "\" name=\"" + campoColuna[i] + "[" + contador + "]\" class=\"normal\" type=\"text\" size=\"60\" maxlentgh=\"" + tamanhovarColuna[i] + "\" value=\"" + campoNovo.value + "\" />";	
				}
				campoNovo.value = "";
			}
			else if(tipoColuna[i] == 'bpchar') {
				if(campoNovo.checked == true) {
					celula.innerHTML = "<input id=\"" + campoColuna[i] + "_" + contador + "\" name=\"" + campoColuna[i] + "[" + contador + "]\" type=\"checkbox\" checked=\"checked\" />";
					campoNovo.checked = false;
				} else {
					celula.innerHTML = "<input id=\"" + campoColuna[i] + "_" + contador + "\" name=\"" + campoColuna[i] + "[" + contador + "]\" type=\"checkbox\" />";
				}
			}
			else if(tipoColuna[i] == 'int4') {
				celula.innerHTML = "<input id=\"" + campoColuna[i] + "_" + contador + "\" name=\"" + campoColuna[i] + "[" + contador + "]\" class=\"normal\" type=\"text\" size=\"5\" maxlentgh=\"" + tamanhonumColuna[i] + "\" value=\"" + campoNovo.value + "\" />";
				campoNovo.value = "";
			}
			else if(tipoColuna[i] == 'timestamp') {
				celula.innerHTML = "<input type=\"text\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onchange=\"\" class=\"normal\" onkeyup=\"this.value=mascaraglobal('##/##/####',this.value);\" value=\"" + campoNovo.value + "\" maxlength=\"10\" style=\"text-align: right;\" size=\"12\" id=\"" + campoColuna[i] + "_" + contador + "\" name=\"" + campoColuna[i] + "[" + contador + "]\" /> " +
								   "<a href=\"javascript:show_calendar('formulario." + campoColuna[i] + "[" + contador + "]');\">" +
								   "<img width=\"16\" height=\"15\" border=\"0\" align=\"absmiddle\" alt=\"\" src=\"../imagens/calendario.gif\" />" +
								   "</a>";
				campoNovo.value = "";
			}
			else {
				celula.innerHTML = "&nbsp";
			}
		}
	}
	contador++;
}

function excluirItem(index,idExcluido) {
	var tabela = document.getElementById("tabelaDados");
	
	if(idExcluido != 'novo') { excluidos.push(idExcluido); }
	tabela.deleteRow(index);
}

/*** FUNÇÃO AJAX ***/
function carregaComboFK(cel,url,data) {
	var aj = new Ajax.Request(  
	url, {  
	 method:'post',   
	 parameters: data,   
	 onComplete: function(r)
		 {
			cel.innerHTML = r.responseText;
		 }
	});
}

</script>