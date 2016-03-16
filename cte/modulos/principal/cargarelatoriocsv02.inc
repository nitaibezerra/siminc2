<?
if($_REQUEST['requisicao'] == 'limparEstado') {
	
	if($_REQUEST['estuf']) {
		$sql = "DELETE FROM 
					cte.dadossubacaotecnica 
				WHERE
					sbaid in (
								SELECT
									s.sbaid
								FROM 
									cte.dimensao d
								INNER JOIN 
									cte.areadimensao     ad  ON ad.dimid = d.dimid
								INNER JOIN 
									cte.indicador            i   ON i.ardid  = ad.ardid
								INNER JOIN 
									cte.criterio               c   ON c.indid  = i.indid
								INNER JOIN 
									cte.pontuacao          p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
								INNER JOIN 
									cte.instrumentounidade iu  ON iu.inuid = p.inuid
								INNER JOIN 
									cte.acaoindicador     a   ON a.ptoid   = p.ptoid
								INNER JOIN 
									cte.subacaoindicador   s   ON s.aciid   = a.aciid
								INNER JOIN 
									cte.dadossubacaotecnica ds ON ds.sbaid = s.sbaid
								WHERE 
									iu.estuf = '".$_REQUEST['estuf']."'
							 )";
		$db->executar($sql);
		$db->commit();
	}
	die("<script>
			alert('Dados removidos com sucesso');
			window.location='cte.php?modulo=principal/cargarelatoriocsv02&acao=A';
		 </script>");
	
}

if($_REQUEST['requisicao'] == 'limparCarga') {
	
	if($_REQUEST['datetime']) {
		$sql = "DELETE FROM cte.dadossubacaotecnica WHERE dstdatains='".$_REQUEST['datetime']."'";
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Dados removidos com sucesso');
			window.location='cte.php?modulo=principal/cargarelatoriocsv02&acao=A';
		 </script>");
	
}

if($_REQUEST['requisicao'] == 'enviarArquivo') {
	
	$dados = file($_FILES['arquivo']['tmp_name']);
	
	$data_ins = date("Y-m-d h:i:s");

	if($dados) {
		foreach($dados as $numline => $line) {
			
			$columns = explode(";", $line);
			
			// testando para ver se o arquivo possui cabeçalho (1º linha)
			if($numline == 0 && $columns[0] == 'ID') {
				// não fazer nada
			} else {

				unset($qtd_painel,$indicador_painel,$qtd_outrafonte,$fonte,$sbaid,$qtd_monitoramento);
				
				/* validando SUBACAO */
				if(is_numeric($columns[0])) {
					$sbaid = $db->pegaUm("SELECT sbaid FROM cte.subacaoindicador WHERE sbaid='".$columns[0]."'");
					if(!$sbaid) {
						$_ERROS[$numline][0] = "ID Subação não encontrada no BD";
					}
				} else {
					$_ERROS[$numline][0] = "ID Subação não pode ser em branco/ não ser númerica";
				}
				/* FIM - validando SUBACAO */
				
				/* validando QTD MONITORAMENTO */
				if(is_numeric(trim($columns[8]))) {
					if(strstr(trim($columns[8]),".") || strstr(trim($columns[8]),",")){
						$_ERROS[$numline][8] = "Quantidade Monitoramento não pode conter '.' ou ','";
					}else{
						$qtd_monitoramento = trim($columns[8]);	
					}
				} else {
					$_ERROS[$numline][8] = "Quantidade Monitoramento não pode estar em branco/ ou ser caracter";
				}
				/* FIM - validando QTD MONITORAMENTO */
				
				/* validando QTD PAINEL */
				if(is_numeric(trim($columns[9]))) {
					$qtd_painel = trim($columns[9]);
				} else {
					$_ERROS[$numline][9] = "Quantidade Painel não pode estar em branco/ ou ser caracter";
				}
				/* FIM - validando QTD PAINEL */
				
				/* validando INDICADOR PAINEL */
				if(trim($columns[10]) || is_numeric($columns[10])) {
					$indicador_painel = trim($columns[10]);
				} else {
					$_ERROS[$numline][10] = "Indicador Painel não pode estar em branco";
				}
				/* FIM - validando INDICADOR PAINEL */
				
				/* validando QTD OUTRA FONTE */
				if(is_numeric(trim($columns[11]))) {
					$qtd_outrafonte = trim($columns[11]);
				} else {
					$_ERROS[$numline][11] = "Quantidade outra fonte não pode estar em branco/ ou ser caracter";
				}
				/* FIM - validando QTD OUTRA FONTE */
				
				/* validando FONTE */
				if($columns[12]) {
					$fonte = $columns[12];
				} else {
					$_ERROS[$numline][12] = "Fonte não pode estar em branco";
				}
				/* FIM - validando FONTE */
				
				$sqls[] = "INSERT INTO cte.dadossubacaotecnica(
				            dstqtdpainel, dstindicadorpainel, dstqtdoutrafonte, dstfonte, 
				            sbaid, dstqtdmonitorada, dstdatains )
						    VALUES ('".$qtd_painel."', '".$indicador_painel."', '".$qtd_outrafonte."', '".$fonte."', 
						    		'".$sbaid."', '".$qtd_monitoramento."', '".$data_ins."');";
				
				
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
cte_montaTitulo( 'Carga de dados Tecnicos', '&nbsp;' );

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
			document.getElementById('botao_luf').value = "Aguarde...";
			document.getElementById('botao_luf').disabled = "disabled";
			window.location='cte.php?modulo=principal/cargarelatoriocsv02&acao=A&requisicao=limparEstado&estuf='+estuf;
		}
	}
}

function limparPorCarga(datetime) {
	if(datetime) {
		var conf = confirm("Deseja realmente excluir os dados desta carga");
		if(conf) {
			document.getElementById('botao_lestado').value = "Aguarde...";
			document.getElementById('botao_lestado').disabled = "disabled";
			window.location='cte.php?modulo=principal/cargarelatoriocsv02&acao=A&requisicao=limparCarga&datetime='+datetime;
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
?> <input type="button" name="botao_luf" id="botao_luf"  value="Ok" onclick="limparPorEstado(document.getElementById('estuf').value);"></td>
</tr>
<tr>
<td class="SubTituloDireita">Limpar por carga:</td>
<td><? 
$sql = "SELECT dstdatains as codigo, to_char(dstdatains, 'dd/mm/YYYY HH24:MI:ss') as descricao FROM cte.dadossubacaotecnica WHERE dstdatains IS NOT NULL GROUP BY dstdatains";
$db->monta_combo('dstdatains', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'dstdatains');
?> <input type="button" name="botao_lestado" name="botao_lestado" value="Ok" onclick="limparPorCarga(document.getElementById('dstdatains').value);"></td>
</tr>
</table>
</td></tr>
</table>