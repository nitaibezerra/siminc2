<?

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
include APPRAIZ . 'includes/Agrupador.php';

?>

<script type="text/javascript">
	function submete() {
		var selectEstado = document.getElementById('estado');
			
		if(selectEstado.value == '') {
			if(confirm('Deseja efetuar a atualização em todos os estados?')) {				
				selectEstado.options[selectEstado.selectedIndex].value = 'Todos';
				document.getElementById('formAtualizaBandaLarga').submit();
			}
		}
		else {
			document.getElementById('formAtualizaBandaLarga').submit();
		}
	}
</script>
<form id="formAtualizaBandaLarga" action="" method="post">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr align="center">
		<td>Selecione o(s) estado(s) que deseja atualizar:
			&nbsp;&nbsp;
			<?php
				$sql = "select estuf as codigo , estdescricao as descricao from territorios.estado order by estdescricao";
				$db->monta_combo( "estado", $sql, 'S', 'Todos os estados', '', '', '', '', '', "estado" );
			?>			
		</td>
	<tr>
	<tr align="center">
		<td>
			<input type="button" value="Atualizar dados" onclick="submete();">
		</td>
	</tr>
	<tr><td>&nbsp;</td></tr>
<?

if(isset($_REQUEST['estado'])) {
	$totalInserts = 0;
	
	include APPRAIZ . 'includes/nusoap/lib/nusoap.php';
	
	$wsdl = "http://10.1.3.113/ws/escolasbandalarga.wsdl";

	$configuracao = array(
		"encoding" => "ISO-8859-1",
		"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
		"trace" => true
	);

	$conexao = new SoapClient( $wsdl, $configuracao );
	
	set_time_limit(0);
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
		  	<th>Estado</th><th>Quantidade de Registros (Web Service)</th><th>Updates Realizados</th><th>Inserts Não Executados</th><th>Erros</th>";
	
	if($_REQUEST['estado'] == "Todos") {
		ini_set( "memory_limit", "2048M" );
		
		$sql = pg_query("select estuf as codigo from territorios.estado order by estdescricao");
						
		while (($dados = pg_fetch_array($sql)) != false) {
			$registrosWebService = 0;
			$updatesRealizados = 0;
			$insertsNaoExecutados = 0;
			$erros = 0;
										
			$cod_uf = $dados["codigo"];
			$cod_mun = "";
						
			$parametros = array("CodigoUF"=>$cod_uf, "CodigoMunicipio"=>$cod_mun);
						
			//$objRetorno = $conexao->call("getEscola", $parametros);
			$objRetorno = $conexao->__call("getEscola", $parametros);
			$registrosWebService = count($objRetorno);
			
			for($i=0; $i<$registrosWebService; $i++) {
				$sql_objeto = "select bdlid from cte.bandalarga b
							   inner join entidade.entidade e on
							   				e.entcodent = '".$objRetorno[$i]['co_inep']."' 
							   				and e.entid = b.entid
							   where b.estuf = '".$objRetorno[$i]['co_uf']."' and b.muncod = '".$objRetorno[$i]['co_municipio']."'";
							
				$bdlid = $db->pegaUm($sql_objeto);
				if($bdlid) {
					$sql_objeto = "update cte.bandalarga set 
										bdllab = '".($objRetorno[$i]['st_laboratorio'] == 'S' ? 'T' : 'F')."', 
										bdllabmec = '".($objRetorno[$i]['st_laboratorio_mec'] == 'S' ? 'T' : 'F')."',
										bdlalunos = ".$objRetorno[$i]['nu_alunos'].",
										bdlprofessor = ".$objRetorno[$i]['nu_professores'].",
										tpaid = ".$objRetorno[$i]['co_prioridade'].",
										bdlbanda = '".($objRetorno[$i]['co_situacao_banda_larga'] == 'S' ? 'T' : 'F')."'
									where bdlid = ".$bdlid;
					$db->executar($sql_objeto);
					$retorno = $db->commit();
					
					if($retorno)
						$updatesRealizados++;
					else
						$erros++;			
				}
				else {
					$insertsNaoExecutados++;			
				}			
			}
			
			echo "<tr align=\"center\">
					<td>".$cod_uf."</td>
					<td>".$registrosWebService."</td>
					<td>".$updatesRealizados."</td>
					<td>".$insertsNaoExecutados."</td>
					<td>".$erros."</td>
				  </tr>";
			
			$totalInserts += $insertsNaoExecutados;
		}
	}
	else {
		$registrosWebService = 0;
		$updatesRealizados = 0;
		$insertsNaoExecutados = 0;
		$erros = 0;
		$inserts = "";
		
		$cod_uf = $_REQUEST['estado'];
		$cod_mun = "";
		
		$parametros = array("CodigoUF"=>$cod_uf, "CodigoMunicipio"=>$cod_mun);				
		
		//$objRetorno = $conexao->call("getEscola", $parametros);
		$objRetorno = $conexao->__call("getEscola", $parametros);
		
		$registrosWebService = count($objRetorno);
		
		for($i=0; $i<$registrosWebService; $i++) {
			$sql_objeto = "select bdlid from cte.bandalarga b
						   inner join entidade.entidade e on
						   				e.entcodent = '".$objRetorno[$i]['co_inep']."' 
						   				and e.entid = b.entid
						   where b.estuf = '".$objRetorno[$i]['co_uf']."' and b.muncod = '".$objRetorno[$i]['co_municipio']."'";
			
			$bdlid = $db->pegaUm($sql_objeto);
			if(isset($bdlid) && ($bdlid != "")) {
				$sql_objeto = "update cte.bandalarga set 
									bdllab = '".($objRetorno[$i]['st_laboratorio'] == 'S' ? 'T' : 'F')."', 
									bdllabmec = '".($objRetorno[$i]['st_laboratorio_mec'] == 'S' ? 'T' : 'F')."',
									bdlalunos = ".$objRetorno[$i]['nu_alunos'].",
									bdlprofessor = ".$objRetorno[$i]['nu_professores'].",
									tpaid = ".$objRetorno[$i]['co_prioridade'].",
									bdlbanda = '".($objRetorno[$i]['co_situacao_banda_larga'] == 'S' ? 'T' : 'F')."'
								where bdlid = ".$bdlid;
				$db->executar($sql_objeto);
				$retorno = $db->commit();
				
				if($retorno)
					$updatesRealizados++;
				else
					$erros++;			
			}
			else {
				$insertsNaoExecutados++;
								
				$entid = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entcodent = '".$objRetorno[$i]['co_inep']."'");				
				$entid = ($entid == "" ? "NULL" : $entid);
				
				$inserts .= "INSERT INTO cte.bandalarga(estuf,muncod,entid,tpaid,bdllab,bdllabmec,bdlbanda,bdlalunos,bdlprofessor) VALUES('".$objRetorno[$i]['co_uf']."','".$objRetorno[$i]['co_municipio']."',".$entid.",".$objRetorno[$i]['co_prioridade'].",'".($objRetorno[$i]['st_laboratorio'] == 'S' ? 'T' : 'F')."','".($objRetorno[$i]['st_laboratorio_mec'] == 'S' ? 'T' : 'F')."','".($objRetorno[$i]['co_situacao_banda_larga'] == 'S' ? 'T' : 'F')."',".$objRetorno[$i]['nu_alunos'].",".$objRetorno[$i]['nu_professores'].")\r\n\r\n";
			}
		}
		
		echo "<tr align=\"center\">
				<td>".$cod_uf."</td>
				<td>".$registrosWebService."</td>
				<td>".$updatesRealizados."</td>
				<td>".$insertsNaoExecutados."</td>
				<td>".$erros."</td>
			  </tr>";
		
		$totalInserts += $insertsNaoExecutados;
	}

	echo "</table>";
	
	if($totalInserts > 0) {
		$arquivo = fopen(APPRAIZ . "www/cte/inserts_banda_larga.txt", "w");
		$texto = $inserts;

		fwrite($arquivo, $texto);
		fclose($arquivo);
		
		echo "<br />";

		echo "<center><a href=\"download.php?file=inserts_banda_larga\">Baixar arquivo de INSERT's</a></center>";
	}
}

?>
</table>
</form>