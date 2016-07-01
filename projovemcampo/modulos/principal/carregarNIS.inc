<?php
// dl('ZipArchive');
// phpinfo();
// die;

// ini_set('memory_limit','4096M');
// ini_set('max_execution_time', 3000);

include_once APPRAIZ . 'includes/workflow.php';

$perfis = pegaPerfilGeral();
function atualizarCadastro($dados){
	global $db;
	$sql = "SELECT
				estid
			FROM	
				projovemcampo.estudante
			WHERE
				estid not in(
								SELECT DISTINCT
									estid
								FROM	
									projovemcampo.historicocadastro
								)
			Limit 2000";
	$estudantes = $db->carregarColuna($sql);
	
	
	foreach($estudantes as $estudante){
		
		$dadosIniciais = array();
		$sql = "SELECT
					est.estid,
					MIN(dia.perid),
					MIN(per.perdtinicio) as data,
					usucpf,
					secocpf
				FROM 
					projovemcampo.estudante est
				LEFT JOIN projovemcampo.lancamentodiario lnd ON lnd.estid = est.estid
				INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
				LEFT JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
				LEFT JOIN projovemcampo.periodo per ON per.perid = dia.perid
				INNER JOIN projovemcampo.usuarioresponsabilidade usu ON usu.entid = tur.entid
				INNER JOIN projovemcampo.secretaria sec ON sec.secaid = tur.secaid
				INNER JOIN projovemcampo.secretario seco On seco.secoid = sec.secoid
				WHERE
					est.estid = {$estudante}
				GROUP BY
					usucpf,
					secocpf,
					est.estid";
		
		$dadosIniciais = $db->pegaLinha($sql);
		if($dadosIniciais['secocpf']||$dadosIniciais['usucpf']){
			
			$estid		= $estudante;
			$usucpf 	= $dadosIniciais['usucpf']?$dadosIniciais['usucpf']:$dadosIniciais['secocpf'];
			$data 		= $dadosIniciais['data']?$dadosIniciais['data']:"now()";
			$hictipo	= 'A';
			$hicacao	= "I";
			$sqlInsert .= "
						Insert Into projovemcampo.historicocadastro(estid,usucpf,hicdataacao,hictipo,hicacao)
						Values($estid,'$usucpf','$data','$hictipo','$hicacao');
			";
		}
	}
	if( $sqlInsert != '' ){
		$db->executar($sqlInsert);
		$db->commit();
	}
	echo "<script>
			alert('Atualização concluida!');
		</script>";

}
function gerarDocPagamento(){
	global $db;
	
	$sql = "SELECT
				lndid
			FROM 
				projovemcampo.lancamentodiario
			WHERE
				docid is null
			";
		
	$registros = $db->carregarColuna( $sql );
// 	ver($registros,d);
	foreach($registros as $lndid){
		
		$docid = wf_cadastrarDocumento( WORKFLOW_TIPODOCUMENTO_PAGAMENTO , 'Fluxo Pagamento Projovem Campo' );
		
		$sqlUpdate .= "UPDATE projovemcampo.lancamentodiario
						SET docid = {$docid}
						WHERE
							lndid = $lndid;";
	}
	
	if($sqlUpdate!=''){
		$db->executar ( $sqlUpdate );
	}
	
	$db->commit ();
	echo "<script>
			alert('Documentos Gerados.');
	    </script>";
}
function executarCarga($dados) {
	global $db;
	
	ini_set("memory_limit", "2048M");
	set_time_limit(800000);
	ini_set('max_execution_time',300000);
// 	ini_set('post_max_size', '128M');
// 	ini_set('upload_max_filesize', '128M');
	
// 	if ($_FILES['arquivo']['name']) {
// 		if(!is_dir('../../arquivos/projovemcampo/nis')) {
// 			mkdir(APPRAIZ.'/arquivos/projovemcampo/nis', 0777);
// 		}
		
		
		$pathDir = APPRAIZ . 'arquivos/projovemurbano/nis';
		
		$aArquivos = glob($pathDir."/*");
		
		foreach($aArquivos as $arquivo){
			unlink($arquivo);
		}		
		
		
		$targetFile = $pathDir . '/' . $_FILES['arquivo']['name'];
	
		move_uploaded_file($_FILES['arquivo']['tmp_name'], $targetFile);
	
		$aTiposCompactados = array('application/zip', 'application/x-msdownload', 'application/x-zip-compressed');
		
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
// 				phpinfo();
				foreach( $csvarray as $k => $linha){
// 					if( ($k-14)%18 == 0 ){
					
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
// 					}
				}
// 			}
				
			foreach( $arrCSV as  $dadosASalvar ){
				
				$checanis = '';

				$nis = (substr($dadosASalvar['2'],0,11));
				
// 				if($dadosASalvar['0']=='09074343473'){
// 					ver($nis,d);
// 				}		
				if($nis == '' || $dadosASalvar['1'] != '0370'){
					continue;
				}else{
					$sqlchecanis = "SELECT 
										estnis 
									FROM 
										projovemcampo.estudante 
									WHERE 
										estcpf = '{$dadosASalvar['0']}'";
					$checanis = $db->pegaUm( $sqlchecanis );
					
					if(($checanis == '' )|| ($checanis == NULL)||($checanis == '           ')||($checanis == 0)){
						
						$sqlupdate = "
								UPDATE projovemcampo.estudante
								SET estnis= $nis
								WHERE estcpf = '{$dadosASalvar['0']}';
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
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

$menu = array(0 => array("id" => 1, "descricao" => "Gerar Arquivo CEF", 			"link" => "/projovemcampo/projovemcampo.php?modulo=principal/gerenciarNIS&acao=A"),
				  1 => array("id" => 2, "descricao" => "Carregar Arquivo NIS", 	"link" => "/projovemcampo/projovemcampo.php?modulo=principal/carregarNIS&acao=A")
			  	  );

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

monta_titulo('Projovem campo', 'Carregar NIS. Selecione o arquivo abaixo.');

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
function atualizarCadastro() {
	document.getElementById('requisicao').value='atualizarCadastro';
	document.getElementById('formulario').submit();
}
function gerarDocPAg() {
	document.getElementById('requisicao').value='gerarDocPagamento';
	document.getElementById('formulario').submit();
}
</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" target="iframeUpload">
	<input type="hidden" name="requisicao" value="executarCarga" id="requisicao">

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
		<?if($db->testa_superuser()){?>
	    	<tr>
	    		<td class="SubTituloCentro" colspan="3"><input type="button" value="Atualizar Cadastro" onclick="atualizarCadastro();"></td>
	    	</tr>
	    	<tr>
	    		<td class="SubTituloCentro" colspan="3"><input type="button" value="Gerar Doc PAgamento" onclick="gerarDocPAg();"></td>
	    	</tr>
	    <?}?>
</table>

<iframe name="iframeUpload" id="iframeresp" style="width:100%;height:800px;border:0px solid #fff;"></iframe>

</form>


