<?php

/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(300);
/* FIM configurações */



switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('carregarestruturavalores'=>true,
									 'inserirvaloresindicadores'=>true,
									 'atualizarvaloresindicadores'=>true,
									 'removervaloresindicadores'=>true
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

$titulo_modulo = "REHUF - Reestruturação dos Hospitais Universitários Federais";

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<?

switch($_REQUEST['pagina']) {
	case 'editar':
		if($_REQUEST['invid']) {
			$indicadorvalor = $db->pegaLinha("SELECT * FROM rehuf.indicadorvalor WHERE invid = '". $_REQUEST['invid'] ."'");
			if($indicadorvalor) {
				$invdsc = $indicadorvalor['invdsc']; 
				$invselect = $indicadorvalor['invselect'];
				$dimid = $indicadorvalor['dimid'];
				/*
				 * QUEBRANDO A SELECT e mapeando os dados
				 */
				$grupo = array();
				$dcol = filtrarValoresSelectColuna($invselect);
				if($dcol['grupo']) {
					$grupo = $grupo+$dcol['grupo'];
					$colid = $dcol['coluna'];
				}
				$dlin = filtrarValoresSelectLinha($invselect);
				if($dlin['grupo']) {
					$grupo = $grupo+$dlin['grupo'];
					$linid = $dlin['linha'];
				}
				$dper = filtrarValoresSelectPeriodo($invselect);
				if($dper['grupo']) {
					$grupo = $grupo+$dper['grupo'];
					$perid = $dper['periodo'];
				}
				
				$dopc = filtrarValoresSelectOpcao($invselect);
				if($dopc['opcao']) {
					$opcid = $dopc['opcao'];
				}
				/*
				 * FIM
				 * QUEBRANDO A SELECT e mapeando os dados
				 */
				$tabelas = $db->carregar("SELECT * FROM rehuf.tabela tab
										  LEFT JOIN rehuf.dimensao dim ON dim.dimid = tab.dimid 
										  ORDER BY dim.dimid, tab.tabtdsc");
				if($tabelas) {
					// listando todas as tabelas do sistema
					foreach($tabelas as $tabela) {
						$marcado_listatabelas = false;					
						if($grupo) {
							foreach($grupo as $grp) {
								if($tabela['tabtid'] == $grp['tabtid']) $marcado_listatabelas = true;
							}
						}
						$listatabelas .= "<option value='".$tabela['tabtid']."' ".(($marcado_listatabelas)?"selected":"").">Dimensão: ".$tabela['dimdsc']." | ".$tabela['tabtdsc']."</option>";
						// se tiver alguma tabela marcada, carregar os grupos das tabelas marcadas
						if($marcado_listatabelas) {
							$gruposgrupo = $db->carregar("SELECT * FROM rehuf.grupoitem git WHERE tabtid='".$tabela['tabtid']."' ORDER BY gitordem");
							if($gruposgrupo) {
								foreach($gruposgrupo as $grop) {
									$marcado_listagrupos = false;
									if($grupo) {
										foreach($grupo as $grp) {
											if($grop['gitid'] == $grp['gitid']) $marcado_listagrupos = true;
										}
									}
									$listagrupos .= "<option value='".$grop['gitid']."' ".(($marcado_listagrupos)?"selected":"").">Tabela: ".$tabela['tabtdsc']." | ".$grop['gitdsc']."</option>";
								}
							}
						}
					}
					// imprindo os campos em variaveis
					$listagrupos = "<select size='7' style='width:650;' name='grupos[]' onclick=\"carregarestrutura('');\" multiple>".$listagrupos."</select>";
					$listatabelas = "<select size='7' style='width:650;' multiple onclick=\"carregarestrutura('tabelas');\" name='tabelas[]'>".$listatabelas."</select>";
				}
				if($grupo) {
					$mostrarlistacolunas = false;
					$mostrarlistalinhas = false;
					foreach($grupo as $grp) {
						$linhasgrupo = $db->carregar("SELECT * FROM rehuf.linha lin
													  LEFT JOIN rehuf.agrupamento agp ON agp.agpid = lin.agpid 
													  LEFT JOIN rehuf.opcoes opc ON opc.opcid = lin.opcid WHERE lin.gitid='".$grp['gitid']."'");
						if($linhasgrupo[0]) {
							foreach($linhasgrupo as $linha) {
								$marcado_listalinhas = false;
								if($linid && !$linha['opcid']) {
									foreach($linid as $lin) {
										if($linha['linid'] == $lin) $marcado_listalinhas = true;
									}
								}
								
								if(!$linha['opcid']) $listalinhas .= "<option value='".$linha['linid']."' ".(($marcado_listalinhas)?"selected":"").">Grupo: ".$grp['gitdsc']." | ".(($linha['agpdsc'])?", Agrupador: ".$linha['agpdsc']." | ":"").(($linha['lindsc'])?$linha['lindsc']:"")."</option>";
								$marcado_listalinhasdin = false;
								if($opcid) {
									foreach($opcid as $opc) {
										if($linha['opcid'] == $opc) $marcado_listalinhasdin = true;
									}
								}
								if($linha['opcid'])$listalinhasdinbk[$linha['opcid']] = "<option value='".$linha['opcid']."' ".(($marcado_listalinhasdin)?"selected":"").">Grupo: ".$grp['gitdsc']." | ".(($linha['opcdsc'])?$linha['opcdsc']:"")."</option>";
							}
							if($listalinhasdinbk) $listalinhasdin = implode("", $listalinhasdinbk);
						}
						$colunasgrupo = $db->carregar("SELECT * FROM rehuf.coluna col 
													   LEFT JOIN rehuf.agrupamento agp ON agp.agpid = col.agpid 
													   WHERE col.gitid='".$grp['gitid']."' 
													   ORDER BY agpordem, colordem");
						if($colunasgrupo && $grp['tpgidcoluna']!=TPG_CFIXAS_PA) {
							$mostrarlistacolunas=true;
							foreach($colunasgrupo as $coluna) {
								if($colid) {
									$marcado_listacolunas = false;
									foreach($colid as $col) {
										if($coluna['colid'] == $col) $marcado_listacolunas = true;
									}
								}
								$listacolunas .= "<option value='".$coluna['colid']."' ".(($marcado_listacolunas)?"selected":"").">Grupo: ".$grp['gitdsc']." | ".(($coluna['agpdsc'])?", Agrupador: ".$coluna['agpdsc']." | ":"").$coluna['coldsc']."</option>";
							}
						}
						
						$periodosgrupo = $db->carregar("SELECT * FROM rehuf.periodogrupoitem per 
													   WHERE per.gitid='".$grp['gitid']."' 
													   ORDER BY perid");
						if($periodosgrupo) {
							$mostrarlistaperiodos=true;
							foreach($periodosgrupo as $periodo) {
								if($perid) {
									$marcado_listaperiodos = false;
									foreach($perid as $per) {
										if($periodo['perid'] == $per) $marcado_listaperiodos = true;
									}
								}
								$listaperiodos .= "<option value='".$periodo['perid']."' ".(($marcado_listaperiodos)?"selected":"").">Ano: ".$periodo['perano']." | ".$periodo['perdsc']."</option>";
							}
						}
						
					}
					$listalinhasdin = "<select size='7' style='width:650;' name='opcid[]' onclick=\"atualizarselect();\" multiple ".((!$mostrarlistacolunas)?"disabled":"").">".$listalinhasdin."</select>";
					$listacolunas   = "<select size='7' style='width:650;' name='colunas[]' onclick=\"atualizarselect();\" multiple ".((!$mostrarlistacolunas)?"disabled":"").">".$listacolunas."</select>";
					$listalinhas    = "<select size='7' style='width:650;' name='linhas[]' onclick=\"atualizarselect();\" multiple>".$listalinhas."</select>";
					$listaperiodos  = "<select size='7' style='width:650;' name='periodos[]' onclick=\"atualizarselect();\" multiple>".$listaperiodos."</select>";
				}
				
				$requisicao = 'atualizarvaloresindicadores';
				$dscacao = 'Editar indicador';
			} else {
				echo "<script>
						alert('Valor do indicador não encontrado');
						window.location='?modulo=indicadores/gerenciarestruturavalores&acao=A';
					  </script>";
				exit;
			}
		} else {
			$requisicao = 'inserirvaloresindicadores';
			$dscacao = 'Inserir valores';
			$tabelas = $db->carregar("SELECT * FROM rehuf.tabela tab 
									  LEFT JOIN rehuf.dimensao dim ON tab.dimid = dim.dimid 
									  ORDER BY dim.dimid, tab.tabtdsc");
			if($tabelas[0]) {
				foreach($tabelas as $tabela) {
					$listatabelas .= "<option value='".$tabela['tabtid']."'>Dimensão: ".$tabela['dimdsc']." | ".$tabela['tabtdsc']."</option>";
				}
				$listatabelas = "<select size='7' style='width:650;' multiple onclick=\"carregarestrutura('tabelas');\" name='tabelas[]'>".$listatabelas."</select>";
			}
			$listagrupos = "<select size='7' style='width:650;' name='grupos[]' onclick=\"carregarestrutura('');\" multiple disabled></select>";
			$listalinhasdin = "<select size='7' style='width:650;' name='opcid[]' onclick=\"atualizarselect();\" multiple disabled>".$listalinhasdin."</select>";
			$listacolunas   = "<select size='7' style='width:650;' name='colunas[]' onclick=\"atualizarselect();\" multiple disabled>".$listacolunas."</select>";
			$listalinhas    = "<select size='7' style='width:650;' name='linhas[]' onclick=\"atualizarselect();\" multiple disabled>".$listalinhas."</select>";
			$listaperiodos  = "<select size='7' style='width:650;' name='periodos[]' onclick=\"atualizarselect();\" multiple>".$listaperiodos."</select>";
		}
		
		monta_titulo( $titulo_modulo, $dscacao );
?>
		<script>
			function validarFormularioValores(frm) {
				if(frm.elements['invdsc'].value == "") {
					alert("'Nome do valor' é obrigatório");
					return false;
				}
				if(frm.elements['invselect'].value == "NA") {
					alert("Selecione os dados para gerar um valor válido");
					return false;
				}
				if(frm.elements['dimid'].value == "") {
					alert("'Dimensão' é obrigatório");
					return false;
				}

				return true;
			}
		</script>
		<form name='formulario' action='?modulo=indicadores/gerenciarestruturavalores&acao=A' method='post' onsubmit='return validarFormularioValores(this);'>
		<?
		if($_REQUEST['invid']) {
			echo "<input type='hidden' name='invid' value='". $_REQUEST['invid'] ."'>";
		}
		?>
		<input type='hidden' name='invselect' id='invselect' value="<? echo $invselect; ?>">
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita">Nome do valor :</td>
				<td><? echo campo_texto('invdsc', "S", "S", "Nome do valor indicador", 67, 150, "", "", '', '', 0, '' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Consulta :</td>
				<td id="tdinvselect"><? echo (($invselect)?$invselect:"NA"); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Dimensão :</td>
				<td><? $db->monta_combo("dimid", "SELECT dimid as codigo, dimdsc as descricao FROM rehuf.dimensao", "S", "Selecione...", '', '', '', '', 'S', 'dimid'); ?></td>
			</tr>

			<tr>
				<td class="SubTituloDireita">Tabelas :</td>
				<td><? echo $listatabelas; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Grupos :</td>
				<td id="tdgrupos"><? echo $listagrupos; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Linhas :</td>
				<td id="tdlinhas"><? echo $listalinhas; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Linhas Dinâmicas:</td>
				<td id="tdlinhasdin"><? echo $listalinhasdin; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Colunas :</td>
				<td id="tdcolunas"><? echo $listacolunas; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Períodos :</td>
				<td id="tdperiodos"><? echo $listaperiodos; ?></td>
			</tr>

			<tr bgcolor="#C0C0C0">
				<td colspan='2' align='center'>
					<table cellSpacing="0" cellPadding="0" align="center" width="100%">
					<tr>
					<td align="center">
					<input type='submit' value="Gravar"> 
					<input type='button' value="Voltar" onclick="window.location='?modulo=indicadores/gerenciarestruturavalores&acao=A';">
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
		$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=indicadores/gerenciarestruturavalores&acao=A&pagina=editar&invid='||idv.invid||'\'\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=indicadores/gerenciarestruturavalores&acao=A&requisicao=removervaloresindicadores&invid='||invid||'\',\'Deseja realmente excluir o valor?\');\">' as opcoes,
					   idv.invid, 
					   idv.invdsc, 
					   dim.dimdsc,
					   idv.invselect 
				FROM rehuf.indicadorvalor idv 
				LEFT JOIN rehuf.dimensao dim ON dim.dimid = idv.dimid 
				ORDER BY dim.dimdsc ASC, idv.invid DESC";
		
		$cabecalho = array("Ações", "ID", "Descrição do valor", "Dimensão", "SQL");
		$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">
				<tr bgcolor=\"#C0C0C0\">
				<td colspan='2' align='center'>
					<table cellSpacing=\"0\" cellPadding=\"0\" align=\"center\" width=\"100%\">
					<tr>
					<td align=\"right\">
					<input type='button' value=\"Inserir indicador\" onclick=\"window.location='?modulo=indicadores/gerenciarestruturavalores&acao=A&pagina=editar';\">
					</td>
					</tr>
					</table>
				</td>
				</tr>
			</table>";
		
?>

<?
}
?>