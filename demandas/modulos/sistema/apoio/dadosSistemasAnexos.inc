<?php


if($_GET['imgid'] && $_GET['arqid'] && $_GET['op5']){
	session_start();
	$_SESSION['imgid'] = $_GET['imgid'];
	$_SESSION['arqid'] = $_GET['arqid'];
	$_SESSION['op5'] = $_GET['op5'];
	header( "Location: demandas.php?modulo=sistema/apoio/dadosSistemasAnexos&acao=A" );
	exit();
}

if($_REQUEST['op5'] == 'download'){

	$param = array();
	$param["arqid"] = $_REQUEST['arqid'];
	DownloadArquivo($param);
	exit;

}

if($_SESSION['imgid'] && $_SESSION['arqid'] && $_SESSION['op5']){
	if($_SESSION['op5'] == 'delete'){
		deletarAnexo();	
		unset($_SESSION['imgid']);
		unset($_SESSION['arqid']);
		unset($_SESSION['op5']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		//header( "Location: demandas.php?modulo=principal/equipeSistema&acao=A" );
	}
	
}

/*
 * Executa a função que insere o arquivo
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_FILES['anexo']['size']){
 	 $sidid = $_SESSION['sidid'];
 	 $dados["arqdescricao"] = $_REQUEST["arqdescricao"];
	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], $dados, $sidid)){
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='demandas.php?modulo=sistema/apoio/dadosSistemasAnexos&acao=A';
			 </script>");
	 }else{
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
	
}


include APPRAIZ ."includes/cabecalho.inc";

print '<br>';
$db->cria_aba( $abacod_tela, $url, '');

monta_titulo( 'Anexos do Sistema', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<?=cabecalhoSistemas(); ?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formAnexos" name="formAnexos" method="post" enctype="multipart/form-data" onsubmit="return validaForm();" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="sidid" value="<?php echo $_SESSION['sidid']; ?>" />
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>&nbsp;</td>		
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" >Tipo anexo:</td>
		<td>
			<select name="imgtipo">
				<option value="">Selecione...</option>
				<option value="L">Logomarca</option>
				<option value="T">Tela inicial</option>
			</select>
			<?php echo obrigatorio();?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" name="anexo" <?if($habil=='N') echo "disabled";?>><? echo obrigatorio(); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" >Descrição:</td>
		<td><?= campo_textarea( 'arqdescricao', 'N', $habil, '', 60, 2, 250 ); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='submit' class='botao' value='Salvar' name='cadastrar' <?if($habil=='N') echo "disabled";?>>	    	
		</td>			
	</tr>
	<tr>
		<td colspan="2" height="40">&nbsp;</td>
	</tr>
</table>

<?

	$sql = "SELECT
			CASE WHEN p.usucpf = '{$_SESSION['usucpf']}' THEN 
				'<a href=\"demandas.php?modulo=sistema/apoio/dadosSistemasAnexos&acao=A&arqid=' || a.arqid || '&op5=download\">
				 	<img border=0 src=\"../imagens/anexo.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/dadosSistemasAnexos&acao=A&arqid=' || a.arqid || '&imgid=' || a.imgid || '&op5=delete\');\" >
				 	<img border=0 src=\"../imagens/excluir.gif\" />
				 </a>'
			ELSE
				'<a href=\"demandas.php?modulo=sistema/apoio/dadosSistemasAnexos&acao=A&arqid=' || a.arqid || '&op5=download\">
				 	<img border=0 src=\"../imagens/anexo.gif\" />
				 </a>'
			END AS OPCOES,
			to_char(p.arqdata,'DD/MM/YYYY') AS dtinclusao,
			'<a style=\"cursor: pointer; color: blue;\" href=\"demandas.php?modulo=sistema/apoio/dadosSistemasAnexos&acao=A&arqid=' || a.arqid || '&op5=download\">' || p.arqnome || '</a>' AS arqnome,
			p.arqtamanho || ' kb' AS arqtamanho,
			p.arqdescricao,
			u.usunome
		
			FROM demandas.imagens AS a
		LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
		LEFT JOIN seguranca.usuario AS u ON p.usucpf = u.usucpf
		
			WHERE a.sidid = {$_SESSION['sidid']} AND a.imgstatus = 'A'";
	$cabecalho = array( "Opções","Data da Inclusão","Nome do Arquivo","Tamanho","Descrição do Arquivo","Responsável");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>
</form>		
			
			
		
			
			
<script type="text/javascript">

function validaForm(){

	if(document.formAnexos.imgtipo.value == ''){
		alert ('O campo Tipo Anexo deve ser preenchido.');
		document.formAnexos.imgtipo.focus();
		return false;
	}

	if(document.formAnexos.anexo.value == ''){
		alert ('Selecione o Arquivo.');
		document.formAnexos.anexo.focus();
		return false;
	}
}

function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este anexo?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>
<?php 
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$sidid){
	global $db;
	
	if (!$arquivo || !$sidid)
		return false;
		
	// obtém o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'".$dados["arqdescricao"]."',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y/m/d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $_SESSION["sisid"] ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela demandas.imagens
	$sql = "INSERT INTO demandas.imagens 
			(
				sidid,
				arqid,
				imgtipo,
				imgstatus
			)VALUES(
			    ". $sidid .",
				". $arqid .",
				'".$_REQUEST['imgtipo']."',
				'A'
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/demandas/sistemas/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/demandas/sistemas/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/demandas/sistemas/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				return false;
			}
	}
	
	
	$db->commit();
	return true;

}

######## Fim funções de UPLOAD ########
function deletarAnexo(){
	global $db;
	$sql = "UPDATE public.arquivo SET
				arqstatus = 0 
			WHERE arqid = {$_SESSION['arqid']}";
	$db->executar($sql);
	$sql = "UPDATE demandas.imagens SET
				imgstatus = 'I' 
			WHERE imgid = {$_SESSION['imgid']}";
	$db->executar($sql);
	$db->commit();
	//header( "Location: demandas.php?modulo=sistema/apoio/dadosSistemasAnexos&acao=A" );
	//exit();
}

function DownloadArquivo($param){
		global $db;
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
		$arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/demandas/sistemas/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
		if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}
?>
