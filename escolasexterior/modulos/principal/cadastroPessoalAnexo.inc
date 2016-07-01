<?php


if($_GET['pesid'] && $_GET['arqid'] && $_GET['op5']){
	session_start();
	$_SESSION['pesid'] = $_GET['pesid'];
	$_SESSION['arqid'] = $_GET['arqid'];
	$_SESSION['op5'] = $_GET['op5'];
	$_SESSION['peeid'] = $_GET['peeid'];
	$_SESSION['tipo'] = $_GET['tipo'];
	header( "Location: escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A" );
	exit();
}

if($_REQUEST['op5'] == 'download'){

	$param = array();
	$param["arqid"] = $_REQUEST['arqid'];
	DownloadArquivo($param);
	exit;

}

if($_SESSION['pesid'] && $_SESSION['arqid'] && $_SESSION['op5'] && $_SESSION['peeid']){
	if($_SESSION['op5'] == 'delete'){
		deletarAnexo();	
		$peeid = $_SESSION['peeid'];
		$tipo = $_SESSION['tipo'];
		unset($_SESSION['pesid']);
		unset($_SESSION['arqid']);
		unset($_SESSION['op5']);
		unset($_SESSION['peeid']);
		unset($_SESSION['tipo']);
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A&peeid={$peeid}&tipo={$tipo}';
				//history.go(-1);
			 </script>");
	}
	
}

/*
 * Executa a função que insere o arquivo
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_FILES['anexo']['size']){
 	 $peeid = $_REQUEST['peeid'];
 	 $tipo = $_REQUEST['tipo'];
 	 $dados["arqdescricao"] = $_REQUEST["arqdescricao"];

 	 
	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], $dados, $peeid)){
	 	unset($_FILES['anexo']['size']);
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A&peeid={$peeid}&tipo={$tipo}';
				//history.go(-1);
			 </script>");
	 }else{
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
	
}


monta_titulo( 'Anexo de Documentos Pessoais', '' );
?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

 
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form id="formAnexos" name="formAnexos" method="post" enctype="multipart/form-data" onsubmit="return validaForm();" >

<input type="hidden" name="peeid" value="<?php echo $_REQUEST['peeid']; ?>" />
<input type="hidden" name="tipo" value="<?php echo $_REQUEST['tipo']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="SubTituloDireita" valign="top">Pessoa:</td>
		<td>
			<?
				if($_REQUEST['peeid']){
					echo $db->pegaUm("select peenome from escolasexterior.pessoalescola where peeid = ".$_REQUEST['peeid']);
				}
			?>
		</td>
	</tr>
	<?if($_REQUEST['tipo'] == 'P'){

		$nomepesquisa = $_REQUEST['nomepesquisa']; ?>
		<tr>
			<td class="SubTituloDireita">Título do Arquivo:</td>
			<td><?= campo_texto('nomepesquisa','N', $habil,'',50,140,'','','left','',0, '') ?></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>&nbsp;</td>
			<td>
		    	<input type='submit' class='botao' value='Pesquisar' name='Pesquisar' >
		    	<input type='button' class='botao' value='Voltar' name='voltar' onclick="history.back();" >
			</td>			
		</tr>
	<?}else{?>
		<tr>
			<td align='right' class="SubTituloDireita" valign="top">Título:</td>
			<td><?= campo_texto('arqdescricao','N', $habil,'',100,140,'','','left','',0, '') ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Arquivo:</td>
			<td><input type="file" size="80" name="anexo" <?if($habil=='N') echo "disabled";?>><? echo obrigatorio(); ?></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>&nbsp;</td>
			<td>
		    	<input type='submit' class='botao' value='Salvar' name='Salvar' <?if($habil=='N') echo "disabled";?>>	    	
		    	<input type='button' class='botao' value='Modo Pesquisa' name='modopesquisa' onclick="location.href='escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A&tipo=P&peeid=<?=$_REQUEST['peeid']?>'">
			</td>			
		</tr>
	<?}?>
	<tr>
		<td colspan="2" height="40">&nbsp;</td>
	</tr>
</table>
</form>
<?
	if($_REQUEST['peeid'] || $_REQUEST['nomepesquisa']){
		
		//exibe botao excluir
		$perfil = arrayPerfil();
		if( in_array(PERFIL_SUPER_USUARIO,$perfil) ){
			$btnExcluir = " OR 1=1 ";
		}
	
		//filtros para a pesquisa
		$clausula = "";
	    if($_REQUEST['peeid']) $clausula .= " AND a.peeid = {$_REQUEST['peeid']} ";
		if($_REQUEST['nomepesquisa']) $clausula .= " AND ( upper(p.arqnome) like '%".strtoupper($_REQUEST['nomepesquisa'])."%' OR upper(p.arqdescricao) like '%".strtoupper($_REQUEST['nomepesquisa'])."%' ) ";
		 
		
		$sql = "SELECT
				CASE WHEN p.usucpf = '{$_SESSION['usucpf']}' $btnExcluir THEN 
					'<center><a href=\"escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A&arqid=' || a.arqid || '&op5=download\">
					 	<img border=0 src=\"../imagens/anexo.gif\" title=\"Baixar Arquivo\" />
					 </a> 
					 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A&arqid=' || a.arqid || '&pesid=' || a.pesid || '&peeid=' || a.peeid || '&op5=delete\');\" >
					 	<img border=0 src=\"../imagens/excluir.gif\" title=\"Excluir Arquivo\"/>
					 </a></center>'
				ELSE
					'<center><a href=\"escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A&arqid=' || a.arqid || '&op5=download\">
					 	<img border=0 src=\"../imagens/anexo.gif\" title=\"Baixar Arquivo\" />
					 </a></center>'
				END AS OPCOES,
				to_char(p.arqdata,'DD/MM/YYYY') AS dtinclusao,
				'<a style=\"cursor: pointer; color: blue;\" title=\"Baixar Arquivo\" href=\"escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A&arqid=' || a.arqid || '&op5=download\">' || p.arqdescricao || '</a>' AS arqnome,
				p.arqtamanho || ' kb' AS arqtamanho,
				u.usunome
			
				FROM escolasexterior.pessoalescolaarquivo AS a
			LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
			LEFT JOIN seguranca.usuario AS u ON p.usucpf = u.usucpf
			
			
				WHERE a.pesstatus = 'A' $clausula ";
		//dbg($sql);
		$cabecalho = array( "<center>Opções</center>","Data da Inclusão","Nome do Arquivo","Tamanho","Responsável");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
		
	}
?>
		
			
			
<script type="text/javascript">

function filtraTipo()
{
	document.formAnexos.peeid.value = '';
	document.formAnexos.submit();
}

function validaForm(){
	if(document.formAnexos.tipo.value != 'P'){
		
		if(document.formAnexos.arqdescricao.value == ''){
			alert ('O campo Nome deve ser preenchido.');
			document.formAnexos.arqdescricao.focus();
			return false;
		}
		if(document.formAnexos.anexo.value == ''){
			alert ('Selecione o Arquivo.');
			document.formAnexos.anexo.focus();
			return false;
		}

	}
	/*if(document.formAnexos.arqdescricao.value == ''){
		alert ('O campo Descrição deve ser preenchido.');
		document.formAnexos.arqdescricao.focus();
		return false;
	}*/
}

function exclusao(url) {
	if ('<?=$habil?>' == 'N'){
		alert('Seu perfil não lhe permite executar esta operação!');
		return;
	}	
	var questao = confirm("Deseja realmente excluir este anexo?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>
<?php 
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$peeid){
	global $db;
	
	
	if (!$arquivo || !$peeid)
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
	//dbg($sql,1);
	$arqid = $db->pegaUm($sql);

	
	//Insere o registro na tabela escolasexterior.pessoalescolaarquivo
	$sql = "INSERT INTO escolasexterior.pessoalescolaarquivo 
			(
				peeid,
				arqid,
				pesstatus
			)VALUES(
			    ". $peeid .",
				". $arqid .",
				'A'
			);";
	$db->executar($sql);

	if(!is_dir('../../arquivos/escolasexterior')) {
		$pasta = APPRAIZ.'arquivos/escolasexterior';
		mkdir($pasta, 0777);
	}
	
	if(!is_dir('../../arquivos/escolasexterior/'.floor($arqid/1000))) {
		$pasta = APPRAIZ.'arquivos/escolasexterior/'.floor($arqid/1000);
		mkdir($pasta, 0777);
	}
		
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
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
	$sql = "UPDATE escolasexterior.pessoalescolaarquivo SET
				pesstatus = 'I' 
			WHERE pesid = {$_SESSION['pesid']}";
	$db->executar($sql);
	$db->commit();
	//header( "Location: escolasexterior.php?modulo=principal/cadastroPessoalAnexo&acao=A" );
	//exit();
}

function DownloadArquivo($param){
		global $db;
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
		$arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
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

</html>



