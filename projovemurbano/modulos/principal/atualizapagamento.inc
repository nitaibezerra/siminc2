<?php
// dl('ZipArchive');
// phpinfo();
// die;
function executarCarga($dados) {
	global $db;
	
	ini_set("memory_limit", "2048M");
	set_time_limit(800000);
	ini_set('max_execution_time',300000);
// 	ini_set('post_max_size', '128M');
// 	ini_set('upload_max_filesize', '128M');
	
// 	if ($_FILES['arquivo']['name']) {
// 		if(!is_dir('../../arquivos/projovemurbano/nis')) {
// 			mkdir(APPRAIZ.'/arquivos/projovemurbano/nis', 0777);
// 		}
		
		
		$pathDir = APPRAIZ . 'arquivos/projovemurbano/nis';
		
		$aArquivos = glob($pathDir."/*");
		
		foreach($aArquivos as $arquivo){
			unlink($arquivo);
		}		
		
		
		$targetFile = $pathDir . '/' . $_FILES['arquivo']['name'];
	
		move_uploaded_file($_FILES['arquivo']['tmp_name'], $targetFile);
	
		$aTiposCompactados = array('application/zip', 'application/x-msdownload');
		
		// Se for ZIP (Única extensão aceita)
		if(in_array($_FILES['arquivo']['type'], $aTiposCompactados)){
			$zip = new ZipArchive;
	
			// Extrai arquivo zip
			$retorno = $zip->open($targetFile);
			
			if($retorno === true){
				
				$zip->extractTo($pathDir.'/');
				$zip->close();
	
				// Apagando arquivo zip
				unlink($targetFile);
			}
		}
		// DÃ¡ um loop nos arquivos da pasta
		$aArquivos = glob($pathDir."/*");
		
		foreach($aArquivos as $arquivo){
			
// 			$file = fopen($arquivo, 'r');
			
			$arrCSV = Array();
			$y = 0;
			
// 			while ($csvarray = file($arquivo)) {
				$csvarray = file($arquivo);
				foreach( $csvarray as $k => $linha){
					if( ($k-14)%18 == 0 ){
						$linha = array_unique(explode(' ', $linha));
						$yy = 0;
						foreach( $linha as $celula){
// 							ver($celula);
							if( strlen(trim($celula)) > 15 ){
								$arrCSV[$y][$yy] = substr ( trim($celula), 53, 11);
								$yy++;
								$arrCSV[$y][$yy] = substr ( trim($celula),45,4);
								$yy++;
							}elseif(strlen(trim($celula)) == 15){
								$arrCSV[$y][$yy] = trim($celula);
								$yy++;
							}
						}
						$y++;
					}
				}
// 			}
			foreach( $arrCSV as  $dadosASalvar ){
// 				ver($dadosASalvar,d);
				
				$nis = (substr($dadosASalvar['2'],0,11));
				if($nis == '' || $dadosASalvar['1'] != '0370'){
					continue;
				}else{
					$sqlchecanis = "SELECT 
										caenispispasep 
									FROM 
										projovemurbano.cadastroestudante 
									WHERE 
										caecpf = '{$dadosASalvar['0']}'";
					$checanis = $db->pegaUm( $sqlchecanis );
					
					if($checanis == ''){
						
						$sqlupdate = "
								UPDATE projovemurbano.cadastroestudante
								SET caenispispasep= $nis
								WHERE caecpf = '{$dadosASalvar['0']}';
								";
						
						$db->executar($sqlupdate );
						$db->commit();
						
					}
				}
			}
		}
		echo "<script>
			alert('Arquivos NIS Carregados.');
	    </script>";			
	}	

if($_REQUEST['requisicao']) {
// 	ver(header('content-type: text/html; charset=ISO-8859-1'),d);
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


checkAno();

include  APPRAIZ . 'includes/cabecalho.inc';
include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
echo '<br>';

$menu = array(0 => array("id" => 1, "descricao" => "Gerar Arquivo CEF", 			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/gerenciarNIS&acao=A"),
				  1 => array("id" => 2, "descricao" => "Carregar Arquivo NIS", 	"link" => "/projovemurbano/projovemurbano.php?modulo=principal/carregarNIS&acao=A")
			  	  );

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Projovem Urbano', 'Carregar NIS. Selecione o arquivo abaixo.');

$bloq = 'S';
$disabled = '';


?>
<script>

function enviarCarga() {
	if(document.getElementById('arquivocsv').value == '') {
		alert('Selecione um arquivo para enviar');
		return false;
	}
	document.getElementById('formulario').submit();
}

</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" target="iframeUpload">
	<input type="hidden" name="requisicao" value="executarCarga">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<b>Carregar arquivos NIS</b>
			</td>
		</tr>
		<tr>
			<td width="40%" class="SubTituloDireita">Arquivo:</td>
			<td>
				<input type="file" name="arquivo" id="arquivocsv">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
				<input type="button" value="Enviar" onclick="enviarCarga();">
			</td>
		</tr>
</table>

<iframe name="iframeUpload" id="iframeresp" style="width:100%;height:800px;border:0px solid #fff;"></iframe>

</form>


