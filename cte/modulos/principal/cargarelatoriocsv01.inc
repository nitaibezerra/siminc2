<?
if($_REQUEST['requisicao'] == 'limparEstado') {
	
	if($_REQUEST['estuf']) {
		$sql = "DELETE FROM cte.relatoriogeralfinanceiro WHERE rgfuf='".$_REQUEST['estuf']."'";
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Dados removidos com sucesso');
			window.location='cte.php?modulo=principal/cargarelatoriocsv01&acao=A';
		 </script>");
	
}

if($_REQUEST['requisicao'] == 'limparCarga') {
	
	if($_REQUEST['datetime']) {
		$sql = "DELETE FROM cte.relatoriogeralfinanceiro WHERE rgfdatains='".$_REQUEST['datetime']."'";
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Dados removidos com sucesso');
			window.location='cte.php?modulo=principal/cargarelatoriocsv01&acao=A';
		 </script>");
	
}

if($_REQUEST['requisicao'] == 'enviarArquivo') {
	
	$dados = file($_FILES['arquivo']['tmp_name']);
	
	$data_ins = date("Y-m-d h:i:s");

	if($dados) {
		foreach($dados as $numline => $line) {
			
			$columns = explode(";", $line);
			
			// testando para ver se o arquivo possui cabeçalho (1º linha)
			if($numline == 0 && $columns[0] == 'UF') {
				// não fazer nada
			} else {
				
				unset($estuf,$tipo_gen_inf,$inuid,$dt_vigencia,$num_processo,$num_convenio,$ano_convenio,$sbaid,$ano_subacao,$vlr_conveniado,$vlr_empenhado,$vlr_pago);
				
				/* validando UF */
				$estuf = $db->pegaUm("SELECT estuf FROM territorios.estado WHERE estuf='".trim($columns[0])."'");
				if(!$estuf) $_ERROS[$numline][0] = "UF inválida";
				/* FIM - validando UF */
				
				/* validando se GENÉRICA OU INFRA */
				switch(trim($columns[1])) {
					case 'GENÉRICO':
						$tipo_gen_inf = "G";
						break;
					case 'INFRA':
						$tipo_gen_inf = "I";
						break;
					default:
						$_ERROS[$numline][1] = "Não se encontra no padrão GENÉRICA OU INFRA";
				}
				/* FIM - validando se GENÉRICA OU INFRA */
				
				/* validando se PAR OU BRASILPRO */
				if($estuf) {
					switch(trim($columns[2])) {
						case 'PAR':
							$sql = "SELECT e.inuid FROM cte.instrumentounidade e 
									WHERE e.estuf='".$estuf."' AND e.itrid='1'";
							
							$inuid = $db->pegaUm($sql);
							
							break;
						case 'BRASIL PRO':
							$sql = "SELECT e.inuid FROM cte.instrumentounidade e 
									WHERE e.estuf='".$estuf."' AND e.itrid='3'";
							
							$inuid = $db->pegaUm($sql);
							
							break;
						default:
							$_ERROS[$numline][2] = "Não se encontra no padrão PAR OU BRASIL PRO";
					}
				}
				/* FIM - validando se PAR OU BRASILPRO*/
				
				/* validando VIGÊNCIA */
				if(trim($columns[3])) {
					$vigencia = explode("/",$columns[3]);
					if(is_numeric($vigencia[2]) && is_numeric($vigencia[1]) && is_numeric($vigencia[0])) {
						$dt_vigencia = sprintf("%04d-%02d-%02d", $vigencia[2], $vigencia[1], $vigencia[0]);
					} else {
						$_ERROS[$numline][3] = "Data não se encontra no formato dd/mm/YYYY";
					}
				} else {
					$_ERROS[$numline][3] = "Vigência não pode estar em branco";
				}
				/* FIM - validando VIGÊNCIA */

				/* validando PROCESSO */
				if($columns[4]) {
					$num_processo = str_replace(array(".","/","-"),"", $columns[4]);
					if(!is_numeric($num_processo)) {
						$_ERROS[$numline][4] = "Nº Processo não deve conter apenas números";
					}
				} else {
					$_ERROS[$numline][4] = "Nº Processo não pode estar em branco";
				}
				/* FIM - validando PROCESSO */
				
				/* validando CONVENIO */
				if($columns[5]) {
					$convenio = explode("/",$columns[5]);
					$num_convenio = $convenio[0];
					if(!is_numeric($num_convenio)) {
						$_ERROS[$numline][5] = "Nº Convênio não se encontra no formato 999999/0000";
					}
				} else {
					$_ERROS[$numline][5] = "Nº Convênio não pode estar em branco";
				}
				/* FIM - validando CONVENIO */
				
				/* validando ANO CONVENIO */
				if($columns[6]) {
					$ano_convenio = trim($columns[6]);
					if(strlen($ano_convenio) != 4) {
						$_ERROS[$numline][6] = "Ano do convênio deve conter 4 caracteres (Ex. 2009)";
					}
				} else {
					$_ERROS[$numline][6] = "Ano do convênio não pode estar em branco";
				}
				/* FIM - validando ANO CONVENIO */

				/* validando SUBACAO */
				if(is_numeric($columns[8])) {
					$sbaid = $db->pegaUm("SELECT sbaid FROM cte.subacaoindicador WHERE sbaid='".$columns[8]."'");
					if(!$sbaid) {
						$_ERROS[$numline][8] = "ID Subação não encontrada no BD";
					}
				} else {
					$_ERROS[$numline][8] = "ID Subação não pode ser em branco/ não ser númerica";
				}
				/* FIM - validando SUBACAO */

				/* validando ANO SUBACAO */
				if($columns[9]) {
					$ano_subacao = trim($columns[9]);
					if(strlen($ano_convenio) != 4) {
						$_ERROS[$numline][9] = "Ano da subacao deve conter 4 caracteres (Ex. 2009)";
					}
				} else {
					$_ERROS[$numline][9] = "Ano da subação não pode estar em branco";
				}
				/* FIM - validando ANO SUBACAO */

				/* validando VALOR CONVENIADO */
				if(trim($columns[10])) {
					$vlr_conveniado = str_replace(array(".",","),array("","."), trim($columns[10]));
					if(!is_numeric($vlr_conveniado)) {
						$_ERROS[$numline][10] = "Valor conveniado deve estar no padrão brasileiro (Ex. 100.456.876,34)";
					}
				} else {
					$_ERROS[$numline][10] = "Valor conveniado não pode estar em branco";
				}
				/* FIM - validando VALOR CONVENIADO */

				/* validando VALOR EMPENHADO */
				if(trim($columns[11])) {
					$vlr_empenhado = str_replace(array(".",","),array("","."), trim($columns[11]));
					if(!is_numeric($vlr_empenhado)) {
						$_ERROS[$numline][11] = "Valor empenhado deve estar no padrão brasileiro (Ex. 100.456.876,34)";
					}
				} else {
					$_ERROS[$numline][11] = "Valor empenhado não pode estar em branco";
				}
				/* FIM - validando VALOR EMPENHADO */

				/* validando VALOR PAGO */
				if(trim($columns[12])) {
					$vlr_pago = str_replace(array(".",","),array("","."), trim($columns[12]));
					if(!is_numeric($vlr_pago)) {
						$_ERROS[$numline][12] = "Valor pago deve estar no padrão brasileiro (Ex. 100.456.876,34)";
					}
				} else {
					$_ERROS[$numline][12] = "Valor pago não pode estar em branco";
				}
				/* FIM - validando VALOR PAGO */
				
				/* validando OUTROS */
				if(trim($columns[13])) {
					$outros = "1";
				} else {
					$outros = "0";
				}
				/* validando OUTROS */
				
	    		$sqls[] = "INSERT INTO cte.relatoriogeralfinanceiro(
	            		  inuid, rgfuf, rgftipo, rgfnumprocessosape, rgfnumconvenio, 
	            		  rgfanoconvenio, rgfvigenciaconvenio, sbaid, rgfsbaano, rgfvalorconvenioconcedente, 
	            		  rgfvalorempenhado, rgfvalorpago, outros, rgfdatains )
	    				  VALUES ('".$inuid."', '".$estuf."', '".$tipo_gen_inf."', '".$num_processo."', '".$num_convenio."', 
	    				  '".$ano_convenio."', '".$dt_vigencia."', '".$sbaid."', '".$ano_subacao."', '".$vlr_conveniado."', 
	    				  '".$vlr_empenhado."', '".$vlr_pago."', '".$outros."', '".$data_ins."');";
				
				
			}
		}
		
	   	if(count($_ERROS) > 0) {
	    	$HTML .= "<span style='font-family:Arial,verdana;font-size:8pt;'>Foram encontrados erros em <b>".count($_ERROS)."</b> linhas</span><br>";
	    	foreach($_ERROS as $linnum => $erroLin) {
	    		foreach($erroLin as $colnum => $erroCol) {
    				$HTML .= "<span style='font-family:Arial,verdana;font-size:8pt;color:red;'>Linha #".($linnum+1)." Coluna #".($colnum+1)." : ".$erroCol."</span><br>";
	    		}
	    	}
    	} else {
    		$HTML .= "<span style='font-family:Arial,verdana;font-size:8pt;'>Foram processados <b>".count($dados)."</b> linhas</span><br>";
    		if($sqls) {
    			$db->executar(implode("",$sqls));
    			$db->commit();
    		}
    		
    	}
    	
    	echo $HTML;
	}
	
	exit;
}


include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
cte_montaTitulo( 'Carga de dados Financeiros', '&nbsp;' );

?>
<script>
function validarEnvioCarga() {

	if(document.getElementById('arquivocsv').value == '') {
		alert('Selecione um arquivo para enviar');
		return false;
	}
	
	document.getElementById('formulario').submit();
	
}

function limparPorEstado(estuf) {
	if(estuf) {
		var conf = confirm("Deseja realmente excluir os dados do estado: "+estuf);
		if(conf) {
			window.location='cte.php?modulo=principal/cargarelatoriocsv01&acao=A&requisicao=limparEstado&estuf='+estuf;
		}
	}
}

function limparPorCarga(datetime) {
	if(datetime) {
		var conf = confirm("Deseja realmente excluir os dados desta carga");
		if(conf) {
			window.location='cte.php?modulo=principal/cargarelatoriocsv01&acao=A&requisicao=limparCarga&datetime='+datetime;
		}
	}
}


</script>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" target="iframeUpload">
<input type="hidden" name="requisicao" value="enviarArquivo">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Agendamento</td>
</tr>
<tr>
	<td class="SubTituloDireita">Manual para carga :</td>
	<td><b><a style="cursor:pointer;" onclick="verdicionariocarga();">CLIQUE AQUI</a></b></td>
</tr>
<tr>
	<td class="SubTituloDireita">Arquivo :</td>
	<td><input type="file" name="arquivo" id="arquivocsv"> <input type="button" value="Enviar" onclick="validarEnvioCarga();"></td>
</tr>
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td class="SubTituloCentro">Log do upload</td></tr>
<tr><td>
<iframe name="iframeUpload" id="iframeresp" style="width:100%;height:250px;border:0px solid #fff;"></iframe>
</td></tr>
<tr><td>
<table class="listagem" width="100%">
<tr>
<td class="SubTituloDireita">Limpar por estado:</td>
<td><? 
$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado";
$db->monta_combo('estuf', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'estuf');
?> <input type="button" name="botao_lestado" value="Ok" onclick="limparPorEstado(document.getElementById('estuf').value);"></td>
</tr>
<tr>
<td class="SubTituloDireita">Limpar por carga:</td>
<td><? 
$sql = "SELECT rgfdatains as codigo, to_char(rgfdatains, 'dd/mm/YYYY HH24:MI:ss') as descricao FROM cte.relatoriogeralfinanceiro WHERE rgfdatains IS NOT NULL GROUP BY rgfdatains";
$db->monta_combo('rgfdatains', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'rgfdatains');
?> <input type="button" name="botao_lestado" value="Ok" onclick="limparPorCarga(document.getElementById('rgfdatains').value);"></td>
</tr>
</table>
</td></tr>
</table>