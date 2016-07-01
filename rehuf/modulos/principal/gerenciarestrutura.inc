<?php
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('buscardadostabela'=>true,
									 'buscargrupoopcoes'=>true,
									 'removergrupoitem'=>true,
									 'atualizargrupoitem'=>true,
									 'inserirgrupoitem'=>true,
									 'atualizartabela'=>true,
									 'inserirtabela'=>true,
									 'ordenargrupoitem'=>true,
									 'ordenaragrupamento'=>true,
									 'ordenarcoluna'=>true,
									 'ordenarlinha'=>true,
									 'verificarremocaoperiodo'=>true,
									 'removerperiodogrupoitem'=>true
									 );
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

$titulo_modulo = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
					<tr>
						<td class='SubTituloCentro' colspan='2'>Gerenciando estrutura da tabela</td>
					</tr>
					".(($_REQUEST['tabtid'])?"<tr><td class='SubTituloDireita' width='30%'>Nome da tabela :</td><td><strong>". $db->pegaUm("SELECT tabtdsc FROM rehuf.tabela WHERE tabtid='".$_REQUEST['tabtid']."'") ."</strong></td></tr>":"")."
					".(($_REQUEST['gitid'])?"<tr><td class='SubTituloDireita' width='30%'>Nome do grupo :</td><td><strong>". $db->pegaUm("SELECT gitdsc FROM rehuf.grupoitem WHERE gitid='".$_REQUEST['gitid']."'") ."</strong></td></tr>":"")."
				  </table>";

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<style type="text/css">
.SubTituloColuna {
	background-color: #dcdcdc;
	text-align: center; 
}
</style>
<?

switch($_REQUEST['visetapa']) {
	

	case 'etapa3':
		$dadostabela = $db->carregar("SELECT * FROM rehuf.tabela tbt
									  LEFT JOIN rehuf.grupoitem git ON git.tabtid = tbt.tabtid 
									  WHERE tbt.tabtid = '". $_REQUEST['tabtid'] ."' AND git.gitid = '". $_REQUEST['gitid'] ."'");
		if($dadostabela) {
			$dadostabela = current($dadostabela);
		} else {
			die("Erro");
		}
		
		monta_titulo( $titulo_modulo, '1.Definindo dados da tabela >> 2.Configurando estrutura da tabela >> <b>3.Criando linha e colunas</b>' );
		
?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class='SubTituloDireita'>Formato da linha :</td>
			<td><? echo $db->pegaUm("SELECT tpgdsc FROM rehuf.tipogrupo WHERE tpgid = '". $dadostabela['tpgidlinha'] ."'"); ?></td>
		</tr>
<?
		switch($dadostabela['tpgidlinha']) {
			case TPG_LDINAM_OPCOES:
?>
			<tr>
				<td class="SubTituloDireita">O usuário poderá criar linhas com as seguintes opções :</td>
				<td id="linha">
				<?
				if($dadostabela['gpoid']) {
					$sql = "SELECT opcid as codigo, opcdsc as descricao FROM rehuf.opcoes WHERE gpoid = '". $dadostabela['gpoid'] ."' ORDER BY opcdsc";
					$db->monta_combo('gpoid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'gpoid');
				} else {
					echo "Opções não gravadas";	
				}
				?>
				
				</td>
			</tr>
<?
			break;
			case TPG_LFIXAS_SN:
?>
			<tr>
				<td class="SubTituloDireita"><img src="../imagens/gif_inclui.gif" onclick="window.open('?modulo=principal/inserirdadostabela&acao=A&op=linha&gitid=<? echo $dadostabela['gitid']; ?>','Incluir linhas','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');"> Linhas:</td>
				<td id="linha"></td>
			</tr>
			<script>
			ajaxatualizar('requisicao=buscardadostabela&tipodado=linha&gitid=<? echo $dadostabela['gitid']; ?>','linha');
			</script>
<?
			break;
			case TPG_LFIXAS_CN:
?>
			<tr>
				<td class="SubTituloDireita"><img src="../imagens/gif_inclui.gif" onclick="window.open('?modulo=principal/inserirdadostabela&acao=A&op=agrupamento&islinha=true&gitid=<? echo $dadostabela['gitid']; ?>','Incluir','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');"> Agrupadores de linha :</td>
				<td id="agrupamentolinha"></td>
			</tr>
			<script>
			ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento&islinha=true&gitid=<? echo $dadostabela['gitid']; ?>','agrupamentolinha');
			</script>
			<tr id="idsubnivellinha" style="display:none">
				<td class="SubTituloDireita"><img src="../imagens/gif_inclui.gif" onclick="window.open('?modulo=principal/inserirdadostabela&acao=A&op=linha&gitid=<? echo $dadostabela['gitid']; ?>&agpid='+document.getElementById('selectagrupamentolinha').options[document.getElementById('selectagrupamentolinha').selectedIndex].value,'Incluir','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');"> Linhas :</td>
				<td id="sublinha"></td>
			</tr>
<?
			break;
		}
?>
		<tr>
			<td class='SubTituloDireita'>Formato da coluna :</td>
			<td><? echo $db->pegaUm("SELECT tpgdsc FROM rehuf.tipogrupo WHERE tpgid = '". $dadostabela['tpgidcoluna'] ."'"); ?></td>
		</tr>
<?
		switch($dadostabela['tpgidcoluna']) {
			
			case TPG_CFIXAS_PA:
				
			for($i = $dadostabela['tabanoini']; $i <= $dadostabela['tabanofim']; $i++) {
				$colid = $db->pegaUm("SELECT colid FROM rehuf.coluna WHERE gitid = '". $dadostabela['gitid'] ."' AND coldsc = '". $i ."' ORDER BY coldsc");
				if(!$colid) {
					$sql = "SELECT colordem AS ordem FROM rehuf.coluna WHERE gitid = '". $dadostabela['gitid'] ."' ORDER BY ordem DESC LIMIT 1";
					$ordem = $db->pegaUm($sql);
					$sql = "INSERT INTO rehuf.coluna(
    	        	 		gitid, agpid, tpiid, coldsc, colobs, colordem)
    				 		VALUES ('". $dadostabela['gitid'] ."', NULL, '". TPIID_PD ."', '".$i."', '', '". (($ordem)?$ordem+1:'1') ."');";
					$db->executar($sql);
				}
			}
			$db->commit();
?>
			<tr>
				<td class="SubTituloDireita">Colunas:</td>
				<td id="coluna"></td>
			</tr>
			<script>
			ajaxatualizar('requisicao=buscardadostabela&tipodado=coluna&is_porano=true&gitid=<? echo $dadostabela['gitid']; ?>','coluna');
			document.getElementById('setasubircoluna').src = '../imagens/seta_cimad.gif';
			document.getElementById('setasubircoluna').onclick = function () {return false;}
			document.getElementById('setadescercoluna').src = '../imagens/seta_baixod.gif';
			document.getElementById('setadescercoluna').onclick = function () {return false;}
			</script>
<?
			break;
			case TPG_CFIXAS_SN:
?>
			<tr>
				<td class="SubTituloDireita"><img src="../imagens/gif_inclui.gif" onclick="window.open('?modulo=principal/inserirdadostabela&acao=A&op=coluna&gitid=<? echo $dadostabela['gitid']; ?>','Incluir','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');"> Colunas:</td>
				<td id="coluna"></td>
			</tr>
			<script>
			ajaxatualizar('requisicao=buscardadostabela&tipodado=coluna&gitid=<? echo $dadostabela['gitid']; ?>','coluna');
			</script>
<?
			break;
			case TPG_CFIXAS_CN:
?>
			<tr>
				<td class="SubTituloDireita"><img src="../imagens/gif_inclui.gif" onclick="window.open('?modulo=principal/inserirdadostabela&acao=A&op=agrupamento&gitid=<? echo $dadostabela['gitid']; ?>','Incluir','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');"> Agrupadores de coluna :</td>
				<td id="agrupamentocoluna"></td>
			</tr>
			<script>
			ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento&gitid=<? echo $dadostabela['gitid']; ?>','agrupamentocoluna');
			</script>
			<tr id="idsubnivelcoluna" style="display:none">
				<td class="SubTituloDireita"><img src="../imagens/gif_inclui.gif" onclick="window.open('?modulo=principal/inserirdadostabela&acao=A&op=coluna&gitid=<? echo $dadostabela['gitid']; ?>&agpid='+document.getElementById('selectagrupamentocoluna').options[document.getElementById('selectagrupamentocoluna').selectedIndex].value,'Incluir','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');"> Colunas :</td>
				<td id="subcoluna"></td>
			</tr>
<?
			break;
		}
?>
	<tr>
		<td colspan='2' align='center'><input type='button' onclick="window.location='?modulo=principal/gerenciarestrutura&acao=A&visetapa=etapa2&tabtid=<? echo $_REQUEST['tabtid']; ?>'" value='Voltar'><input type='button' onclick="window.open('?modulo=principal/verestrutura&acao=A&gitid=<? echo $_REQUEST['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; ?>','Estrutura','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');" value='Ver estrutura'></td>
	</tr>
	</table>
<?		
	break;
	
	
	case 'etapa2':
		if(!$_REQUEST['tabtid']) {
			die("Erro"); 
		} else {
			if($_REQUEST['gitid']) {
				$grupoitem = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE gitid = '". $_REQUEST['gitid'] ."'");
				if($grupoitem) {
					$grupoitem = current($grupoitem);
					$gitdsc = $grupoitem['gitdsc'];
					$tpgidlinha = $grupoitem['tpgidlinha'];
					$tpgidcoluna = $grupoitem['tpgidcoluna'];
					$gitobs = $grupoitem['gitobs'];
					$gpoid = $grupoitem['gpoid'];
					$requisicao = 'atualizargrupoitem';
					$desabilitarmodificarformato = true;
				} else {
					die("Erro");
				}
			} else {
				$requisicao = 'inserirgrupoitem';
			}
		}
		monta_titulo( $titulo_modulo, '1.Definindo dados da tabela >> <b>2.Configurando estrutura da tabela</b> >> 3.Criando linha e colunas' );
?>
		<script>
			function detalheslinha(tpgidlinha) {
				switch(tpgidlinha) {
					case '<? echo TPG_LDINAM_OPCOES; ?>':
						document.getElementById('detalheslinha').style.display = '';
						ajaxatualizar('requisicao=buscargrupoopcoes','comboopcoeslinha');
					break;
					default:
						document.getElementById('detalheslinha').style.display = 'none';
						document.getElementById('comboopcoeslinha').innerHTML = '';
				}
			}
			
			function validarFormularioEtapa2(frm) {
				if(frm.elements['gitdsc'].value == "") {
					alert("'Nome do grupo' é obrigatório");
					return false;
				}
				if(frm.elements['tpgidlinha'].value == "") {
					alert("'Formato da linha' é obrigatório");
					return false;
				}
				if(frm.elements['tpgidcoluna'].value == "") {
					alert("'Formato da coluna' é obrigatório");
					return false;
				}
				
				return true;
			}
			function inserirperiodo(ano) {
				var perdsc = prompt("Digite a descrição do período :");
				if(perdsc) {
					tabela = document.getElementById('periodo'+ano);
					if(tabela.rows[0].id=="naoexistem") {
						tabela.rows[0].cells[0].className = "SubTituloDireita";
						tabela.rows[0].cells[0].innerHTML = "<input type='hidden' name='perdsc[ins]["+ano+"][]' value='"+perdsc+"'><b>1 :</b>";
						tabela.rows[0].id="";
						var coluna1 = tabela.rows[0].insertCell(1);
						coluna1.innerHTML = perdsc;
						var coluna2 = tabela.rows[0].insertCell(2);
						coluna2.innerHTML = "<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarperiodo(this);\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"removerperiodo(this, '');\">";
					} else {
						var linha = tabela.insertRow(tabela.rows.length);
						var coluna0 = linha.insertCell(0);
						coluna0.className = "SubTituloDireita";
						coluna0.innerHTML = "<input type='hidden' name='perdsc[ins]["+ano+"][]' value='"+perdsc+"'><b>"+(linha.rowIndex+1)+" :</b>";
						var coluna1 = linha.insertCell(1);
						coluna1.innerHTML = perdsc;
						var coluna2 = linha.insertCell(2);
						coluna2.innerHTML = "<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarperiodo(this);\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"removerperiodo(this, '');\">";
					}
				}
				
			}
			function alterarperiodo(dado) {
				var linha = dado.parentNode.parentNode;
				var perdsc = prompt("Digite a descrição do período :", linha.cells[1].innerHTML);
				if(perdsc) {
					linha.cells[1].innerHTML = perdsc;
					linha.cells[0].childNodes[0].value = perdsc;
				} else {
					alert('A descrição do período é obrigatória');
					return false;
				}
			}
			function removerperiodo(dado, perid) {
				var nlinha = dado.parentNode.parentNode.rowIndex;
				var tabela = dado.parentNode.parentNode.parentNode;
				if(perid) {
					/* Verifica se não existem registros nesse período */
					 ajaxatualizar('requisicao=verificarremocaoperiodo&perid='+perid,'controleremover');
				} else {
					document.getElementById('controleremover').innerHTML = 'simremover';
				}
				
				switch(document.getElementById('controleremover').innerHTML) {
					case 'naoremover':
						alert("Não foi possível remover este período pois, existem registros gravados.");
						return false;
						break;
					case 'simremover':
						var conf = confirm("Deseja realmente excluir este período ?");
						if(conf) {
							if(perid) {
								/* Verifica se não existem registros nesse período */
								ajaxatualizar('requisicao=removerperiodogrupoitem&perid='+perid,'');
							}
							tabela.deleteRow(nlinha);
							if(tabela.rows.length==0) {
								var linha = tabela.insertRow(0);
								linha.id="naoexistem";
								var coluna = linha.insertCell(0);
								coluna.innerHTML = "Não existem períodos";
							}
						}
				}
			}
		</script>
		<form name='formulario' action='?modulo=principal/gerenciarestrutura&acao=A' method='post' onsubmit='return validarFormularioEtapa2(this);'>
		<?
		if($grupoitem['gitid']) {
			echo "<input type='hidden' name='gitid' value='". $grupoitem['gitid'] ."'>";
		}
		$grupositem = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $_REQUEST['tabtid'] ."' ORDER BY gitordem");
		?>
		<input type='hidden' id='visetapa' name='visetapa' value=''>
		<input type='hidden' name='tabtid' value='<? echo $_REQUEST['tabtid']; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<?
		if($grupositem) {		
		?>
			<tr>
				<td class="SubTituloDireita"><strong>Selecione um grupo para editar :</strong></td>
				<td id="grupoitem"></td>
				<script>
				ajaxatualizar('requisicao=buscardadostabela&tipodado=grupoitem','grupoitem');
				<? if($grupoitem['gitid']) { ?>
				document.getElementById('selectgrupoitem').value = '<? echo $_REQUEST['gitid']; ?>';
				<? } ?>
				</script>
			</tr>
		<?
		}
		?>
			<tr>
				<td class="SubTituloDireita">Nome do grupo :</td>
				<td><? echo campo_texto('gitdsc', "S", "S", "Nome do grupo", 67, 150, "", "", '', '', 0, '' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Formato da linha :</td>
				<td>
				<?
				$sql = "SELECT tpgid AS codigo, tpgdsc AS descricao FROM rehuf.tipogrupo WHERE tpglinha = 'true'";
				$db->monta_combo('tpgidlinha', $sql, (($desabilitarmodificarformato)?'N':'S'), 'Selecione', 'detalheslinha', '', '', '', 'S', 'tpgidlinha');
				?>
				</td>
			</tr>
			<tr style='display:none' id='detalheslinha'>
				<td class='SubTituloDireita'>Opções da linha :</td>
				<td id='comboopcoeslinha'></td>
			</tr>
			<?
			if($gpoid) {
				echo "<script>
						document.getElementById('detalheslinha').style.display = '';
						ajaxatualizar('requisicao=buscargrupoopcoes&gpoid=". $gpoid ."','comboopcoeslinha');
					  </script>";	
			}
			
			?>
			<tr>
				<td class="SubTituloDireita">Deseja ter linha totalizadora ?</td>
				<td><input type='radio' name='gitpossuitotallinha' value='sim' <? echo (($grupoitem['gitpossuitotallinha']=='t')?'checked':''); ?> > Sim <input type='radio' name='gitpossuitotallinha' value='nao' <? echo (($grupoitem['gitpossuitotallinha']=='f' || !$grupoitem['gitpossuitotallinha'])?'checked':''); ?> > Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Formato da coluna :</td>
				<td>
				<?
				$sql = "SELECT tpgid AS codigo, tpgdsc AS descricao FROM rehuf.tipogrupo WHERE tpglinha = 'false'";
				$db->monta_combo('tpgidcoluna', $sql, (($desabilitarmodificarformato)?'N':'S'), 'Selecione', '', '', '', '', 'S', 'tpgidcoluna');
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Deseja ter coluna totalizadora ?</td>
				<td><input type='radio' name='gitpossuitotalcoluna' value='sim' <? echo (($grupoitem['gitpossuitotalcoluna']=='t')?'checked':''); ?> > Sim <input type='radio' name='gitpossuitotalcoluna' value='nao' <? echo (($grupoitem['gitpossuitotalcoluna']=='f' || !$grupoitem['gitpossuitotalcoluna'])?'checked':''); ?> > Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Deseja listar grupo no menu ?</td>
				<td><input type='radio' name='gitvisivel' value='sim' <? echo (($grupoitem['gitvisivel']=='t')?'checked':''); ?> > Sim <input type='radio' name='gitvisivel' value='nao' <? echo (($grupoitem['gitvisivel']=='f' || !$grupoitem['gitvisivel'])?'checked':''); ?> > Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Observações :</td>
				<td><? echo campo_textarea( 'gitobs', 'S', 'S', '', '70', '4', '200'); ?></td>
			</tr>
			<?
			/*
			 * Rotina que implementa a subdivisão dos ano nos grupos
			 */
			$tabela = $db->pegaLinha("SELECT tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid='".$_REQUEST['tabtid']."'");
			?>
			<tr>
				<td class="SubTituloDireita">Dividir por ano :</td>
				<td>
				<table class="tabela">
				<tr>
				<? for($i=$tabela['tabanoini'];$i<=$tabela['tabanofim'];$i++) { ?>
					<td class="SubTituloCentro"><? echo $i; ?></td>
				<? } ?>
				</tr>
				<tr>
				<? for($i=$tabela['tabanoini'];$i<=$tabela['tabanofim'];$i++) { ?>
				<td valign="top">
					<span id="controleremover" style="display:none"></span>
					<table id="periodo<? echo $i; ?>" width="100%">
					<?
					if($grupoitem['gitid']) {
						$periodos = $db->carregar("SELECT * FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' AND perano='".$i."' ORDER BY perid");
						if($periodos[0]) {
							foreach($periodos as $c => $periodo) {
								echo "<tr>
										<td class='SubTituloDireita'><input type='hidden' name='perdsc[atu][".$i."][".$periodo['perid']."]' value='".$periodo['perdsc']."'><b>".($c+1)." :</b></td>
										<td>".$periodo['perdsc']."</td>
										<td><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarperiodo(this);\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"removerperiodo(this, '".$periodo['perid']."');\"></td>
									  </tr>";
							}
						} else {
							echo "<tr id=\"naoexistem\"><td>Não existem períodos</td></tr>";
						}
					} else {
						echo "<tr id=\"naoexistem\"><td>Não existem períodos</td></tr>";
					}
					?>
					</table>
				</td>
				<? } ?>
				</tr>
				<tr>
				<? for($i=$tabela['tabanoini'];$i<=$tabela['tabanofim'];$i++) { ?>
					<td class="SubTituloDireita"><img src="../imagens/gif_inclui.gif" onclick="inserirperiodo('<? echo $i; ?>');"></td>
				<? } ?>
				</tr>
				<?
				if($grupoitem['gitid']) {
				?>
				<tr>
				<? 
				for($i=$tabela['tabanoini'];$i<=$tabela['tabanofim'];$i++) {
					$periodos = $db->carregar("SELECT * FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' AND perano='".$i."' ORDER BY perid");
					if($periodos[0]) { 
						$peracumulado = $db->pegaUm("SELECT pgdid FROM rehuf.periodogrupoitemacumulado WHERE gitid='".$grupoitem['gitid']."' AND perano='".$i."'");
						echo "<td class='SubTituloDireita'>Os valores informados por períodos serão acumulado? <input type='checkbox' name='pgd[".$i."]' ".(($peracumulado)?"checked":"")."></td>";
					} else {
						echo "<td class='SubTituloDireita'>&nbsp;</td>";
					}
				} 
				?>
			  	</tr>
				<?
				}
				?>
				</table>
				</td>

			</tr>
			<tr>
				<td colspan='2' align='center'>
					<table cellSpacing="1" cellPadding="3" align="center" width="100%">
					<tr>
					<td width="15%" align="left">
					<? if($_REQUEST['tabtid']) { ?>
					<input type='button' value='<<' onclick="window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=etapa1&tabtid=<? echo $_REQUEST['tabtid']; ?>';">
					<? } ?>
					</td>
					<td align="center">
					<input type='submit' value="Gravar e anterior" onclick="document.getElementById('visetapa').value='etapa1';"> <?if($grupoitem['gitid']) echo "<input type='button' value='Deletar' onclick=\"window.location='?modulo=principal/gerenciarestrutura&acao=A&requisicao=removergrupoitem&tabtid=".$_REQUEST['tabtid']."&gitid=".$grupoitem['gitid']."'\">"; ?> <input type='submit' value='Gravar' onclick="document.getElementById('visetapa').value='etapa2';"> <input type='button' value='Novo' onclick="window.location='?modulo=principal/gerenciarestrutura&acao=A&tabtid=<? echo $_REQUEST['tabtid']; ?>&visetapa=etapa2';"> <input type='submit' value='Gravar e próximo' onclick="document.getElementById('visetapa').value='etapa3';"> 
					<input type='button' value='Lista tabelas' onclick="window.location='rehuf.php?modulo=principal/listarestrutura&acao=A';">
					</td>
					<td width="15%" align="right">
					<? if($_REQUEST['gitid']) { ?>
					<input type='button' value='>>' onclick="window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=etapa3&tabtid=<? echo $_REQUEST['tabtid']; ?>&gitid=<? echo $_REQUEST['gitid']; ?>';">
					<? } ?>
					</td>
					</tr>
					</table>
				</td>
			</tr>

		</table>
		</form>
<?		
	break;
	default:
		
		foreach($_ANOS as $a) {
			$dadosano[] = array('codigo' => $a, 'descricao' => $a); 
		}
	
		if($_REQUEST['tabtid']) {
			$tabeladados = $db->pegaLinha("SELECT * FROM rehuf.tabela WHERE tabtid = '". $_REQUEST['tabtid'] ."'");
			if($tabeladados) {
				$tabtdsc = $tabeladados['tabtdsc'];
				$tabobs = $tabeladados['tabobs'];
				$tabanoini = $tabeladados['tabanoini'];
				$tabanofim = $tabeladados['tabanofim'];
				$dimid = $tabeladados['dimid'];
				$tabvisivel = $tabeladados['tabvisivel'];
				$requisicao = 'atualizartabela';
			}
			$filtrodadosanoperfil = $db->carregar("SELECT * FROM rehuf.filtrodadosanoperfil fap LEFT JOIN seguranca.perfil pfl ON pfl.pflcod = fap.pflcod WHERE tabtid = '". $_REQUEST['tabtid'] ."'");
			if($filtrodadosanoperfil[0]) {
				/*
				 * Atualmente o sistema filtra 1 periodo de anos para diversos perfis,
				 * porém o modelo este preparado para filtrar 1 periodo de anso para cada perfil
				 */
				$anos = current($filtrodadosanoperfil);
				$filtroanoperfilinicio = $anos['anoini'];
				$filtroanoperfilfim = $anos['anofim'];
				foreach($filtrodadosanoperfil as $perfil) {
					$pflcod[] = array('codigo'=>$perfil['pflcod'],'descricao'=>$perfil['pfldsc']);
				}
			}
			
		} else {
			$requisicao = 'inserirtabela';	
		}
		monta_titulo( $titulo_modulo, '<b>1.Definindo dados da tabela</b> >> 2.Configurando estrutura da tabela >> 3.Criando linha e colunas' );
		
?>
		<script>
		function validarFormularioEtapa1(frm) {
			if(frm.elements['tabtdsc'].value == "") {
				alert("'Nome da tabela' é obrigatório");
				return false;
			}
			if(frm.elements['tabanoini'].value == "") {
				alert("'Período início' é obrigatório");
				return false;
			}
			if(frm.elements['tabanofim'].value == "") {
				alert("'Período fim' é obrigatório");
				return false;
			}
			if(frm.elements['tabanofim'].value < frm.elements['tabanoini'].value) {
				alert("'Período fim' deve ser maior que é 'Período início'");
				return false;
			}
			if(frm.elements['dimid'].value == "") {
				alert("'Dimensão' é obrigatório");
				return false;
			}
			if(frm.elements['filtroanoperfilinicio'].value && frm.elements['filtroanoperfilfim'].value) {
				selectAllOptions(document.getElementById('pflcod'));
			}

			return true;
		} 
		</script>
		<form name='formulario' action='?modulo=principal/gerenciarestrutura&acao=A' method='post' onsubmit="return validarFormularioEtapa1(this);">
		<?
		if($tabeladados['tabtid']) {
			echo "<input type='hidden' name='tabtid' value='". $tabeladados['tabtid'] ."'>";	
		}
		?>
		<input type='hidden' id='visetapa' name='visetapa' value=''>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Nome da tabela :</td>
			<td><? echo campo_texto('tabtdsc', 'S', 'S', 'Nome da tabela', 67, 150, "", "", '', '', 0, '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Observações :</td>
			<td><? echo campo_textarea( 'tabobs', 'N', 'S', '', '70', '4', '200'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período início :</td>
			<td><? $db->monta_combo("tabanoini", $dadosano, "S", "Selecione...", '', '', '', '100', 'S', 'orgid'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período fim :</td>
			<td><? $db->monta_combo("tabanofim", $dadosano, "S", "Selecione...", '', '', '', '100', 'S', 'orgid'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Dimensão :</td>
			<td><? $db->monta_combo("dimid", "SELECT dimid as codigo, dimdsc as descricao FROM rehuf.dimensao", "S", "Selecione...", '', '', '', '', 'S', 'dimid'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Deseja listar tabela no menu?</td>
			<td><input type="radio" name="tabvisivel" value="sim" <? echo (($tabvisivel=="t" || !$tabvisivel)?"checked":""); ?>> Sim <input type="radio" name="tabvisivel" value="nao" <? echo (($tabvisivel=="f")?"checked":""); ?>> Não</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Filtrar dados por perfil :</td>
			<td>
			<table>
			<tr>
			<td align="right">Ano início:</td>
			<td><? $db->monta_combo("filtroanoperfilinicio", $dadosano, "S", "Selecione...", '', '', '', '100', 'N', 'filtroanoperfilinicio'); ?></td>
			<td align="right">Ano fim:</td>
			<td><? $db->monta_combo("filtroanoperfilfim", $dadosano, "S", "Selecione...", '', '', '', '100', 'N', 'filtroanoperfilfim'); ?></td>
			</tr>
			<tr>
			<td align="right">Perfis:</td>
			<td colspan="3"><?
			$sqlPerfis = "SELECT pflcod as codigo, pfldsc as descricao FROM seguranca.perfil 
							 WHERE sisid='".SISID."' AND pflsuperuser=FALSE ORDER BY pfldsc";
			combo_popup("pflcod", $sqlPerfis, "Perfis", "192x400", 0, array(), "", "S", false, false, 5, 270 );				
			?></td>
			</tr>
			</table>
			</td>
		</tr>
		<tr>
			<td colspan='2' align='center'>
				<table cellSpacing="1" cellPadding="3" align="center" width="100%">
				<tr>
				<td width="35%" align="left">&nbsp;</td>
				<td align="center">
				<input type='submit' value='Gravar'  onclick="document.getElementById('visetapa').value='etapa1';"> <input type='submit' value='Gravar e próximo' onclick="document.getElementById('visetapa').value='etapa2';"> <input type='button' value='Lista tabelas' onclick="window.location='rehuf.php?modulo=principal/listarestrutura&acao=A';"></td>
				<td width="35%" align="right">
				<? if($tabeladados['tabtid']) { ?>
				<input type='button' value='>>' onclick="window.location = '?modulo=principal/gerenciarestrutura&acao=A&visetapa=etapa2&tabtid=<? echo $tabeladados['tabtid']; ?>';">
				<? } ?>
				</td>
				</tr>
				</table>
			</td>
		</tr>

</table>
</form>
<?
}
?>