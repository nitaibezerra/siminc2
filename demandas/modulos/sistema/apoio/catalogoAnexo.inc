<?php


if($_GET['tutid'] && $_GET['arqid'] && $_GET['op5']){
	session_start();
	$_SESSION['tutid'] = $_GET['tutid'];
	$_SESSION['arqid'] = $_GET['arqid'];
	$_SESSION['op5'] = $_GET['op5'];
	$_SESSION['tipid'] = $_GET['tipid'];
	$_SESSION['ordid'] = $_GET['ordid'];
	$_SESSION['tipo'] = $_GET['tipo'];
	header( "Location: demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A" );
	exit();
}

if($_REQUEST['op5'] == 'download'){

	$param = array();
	$param["arqid"] = $_REQUEST['arqid'];
	DownloadArquivo($param);
	exit;

}

if($_SESSION['tutid'] && $_SESSION['arqid'] && $_SESSION['op5'] && $_SESSION['tipid']){
	if($_SESSION['op5'] == 'delete'){
		deletarAnexo();	
		$tipid = $_SESSION['tipid'];
		$ordid = $_SESSION['ordid'];
		$tipo = $_SESSION['tipo'];
		unset($_SESSION['tutid']);
		unset($_SESSION['arqid']);
		unset($_SESSION['op5']);
		unset($_SESSION['tipid']);
		unset($_SESSION['ordid']);
		unset($_SESSION['tipo']);
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A&tipid={$tipid}&ordid={$ordid}&tipo={$tipo}';
				//history.go(-1);
			 </script>");
	}
	
}

/*
 * Executa a função que insere o arquivo
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_FILES['anexo']['size']){
 	 $tipid = $_REQUEST['tipid'];
 	 $ordid = $_REQUEST['ordid'];
 	 $tipo = $_REQUEST['tipo'];
 	 $dados["arqdescricao"] = $_REQUEST["arqdescricao"];

 	 
	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], $dados, $tipid)){
	 	unset($_FILES['anexo']['size']);
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A&tipid={$tipid}&ordid={$ordid}&tipo={$tipo}';
				//history.go(-1);
			 </script>");
	 }else{
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
	
}


monta_titulo( 'Anexo de Tutoriais de Atendimento', '' );
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

<input type="hidden" name="tipo" value="<?php echo $_REQUEST['tipo']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita">Origem:</td>
		<td>
			<?php
				$ordid = $_REQUEST['ordid']; 
			
				$sql = "SELECT
						 ordid AS codigo,
						 orddescricao AS descricao
						FROM
						 demandas.origemdemanda WHERE ordstatus = 'A' 
						ORDER BY
						 orddescricao;";
				$db->monta_combo("ordid",$sql,'S',"-- Informe a Origem da demanda --",'filtraTipo','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo Serviço:</td>
				<td>
			<?php
				$tipid = $_REQUEST['tipid']; 
				
				if($ordid){
					$sql = "SELECT
							 tipid AS codigo,
							 tipnome AS descricao
							FROM
							 demandas.tiposervico
							 WHERE tipstatus = 'A' 
							 AND ordid = {$ordid}
							ORDER BY
							 tipnome;";
				}
				else{
					$sql = "SELECT
							 tipid AS codigo,
							 tipnome AS descricao
							FROM
							 demandas.tiposervico
							 WHERE tipstatus = 'A' 
							 AND ordid = 999
							ORDER BY
							 tipnome;";
				}
				//dbg($sql,1);
				$db->monta_combo("tipid",$sql,'S',"-- Informe o Tipo Serviço--",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	
	<?if($_REQUEST['tipo'] == 'P'){?>
		<tr>
			<td class="SubTituloDireita">Nome / Descrição:</td>
			<td><?= campo_texto('nomepesquisa','N', $habil,'',50,140,'','','left','',0, '') ?></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>&nbsp;</td>
			<td>
		    	<input type='submit' class='botao' value='Pesquisar' name='Pesquisar' >
			</td>			
		</tr>
	<?}else{?>
		<tr>
			<td class="SubTituloDireita">Arquivo:</td>
			<td><input type="file" size="80" name="anexo" <?if($habil=='N') echo "disabled";?>><? echo obrigatorio(); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" valign="top">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'N', $habil, '', 100, 10, '' ); ?></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>&nbsp;</td>
			<td>
		    	<input type='submit' class='botao' value='Salvar' name='Salvar' <?if($habil=='N') echo "disabled";?>>	    	
		    	<input type='button' class='botao' value='Modo Pesquisa' name='modopesquisa' onclick="location.href='demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A&tipo=P&tipid=<?=$_REQUEST['tipid']?>&ordid=<?=$_REQUEST['ordid']?>'">
			</td>			
		</tr>
	<?}?>
	<tr>
		<td colspan="2" height="40">&nbsp;</td>
	</tr>
</table>
</form>
<?
	if($_REQUEST['ordid'] || $_REQUEST['tipid'] || $_REQUEST['nomepesquisa']){
		
		//exibe botao excluir
		$perfil = arrayPerfil();
		if( in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfil) || in_array(DEMANDA_PERFIL_ADMINISTRADOR,$perfil) ){
			$btnExcluir = " OR 1=1 ";
		}
	
		//filtros para a pesquisa
		$clausula = "";
		if($_REQUEST['ordid']) $clausula .= " AND o.ordid = {$_REQUEST['ordid']} ";
	    if($_REQUEST['tipid']) $clausula .= " AND a.tipid = {$_REQUEST['tipid']} ";
		if($_REQUEST['nomepesquisa']) $clausula .= " AND ( upper(p.arqnome) like '%".strtoupper($_REQUEST['nomepesquisa'])."%' OR upper(a.tutdesc) like '%".strtoupper($_REQUEST['nomepesquisa'])."%' ) ";
		 
		
		$sql = "SELECT
				CASE WHEN p.usucpf = '{$_SESSION['usucpf']}' $btnExcluir THEN 
					'<a href=\"demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A&arqid=' || a.arqid || '&op5=download\">
					 	<img border=0 src=\"../imagens/anexo.gif\" />
					 </a> 
					 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A&arqid=' || a.arqid || '&tutid=' || a.tutid || '&tipid=' || a.tipid || '&op5=delete\');\" >
					 	<img border=0 src=\"../imagens/excluir.gif\" />
					 </a>'
				ELSE
					'<a href=\"demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A&arqid=' || a.arqid || '&op5=download\">
					 	<img border=0 src=\"../imagens/anexo.gif\" />
					 </a>'
				END AS OPCOES,
				to_char(a.tutdtinclusao,'DD/MM/YYYY') AS dtinclusao,
				'<a style=\"cursor: pointer; color: blue;\" href=\"demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A&arqid=' || a.arqid || '&op5=download\">' || p.arqnome || '</a>' AS arqnome,
				o.orddescricao as origem, 
				t.tipnome as tipo,
				p.arqtamanho || ' kb' AS arqtamanho,
				a.tutdesc,
				u.usunome
			
				FROM demandas.tutoriais AS a
			LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
			LEFT JOIN seguranca.usuario AS u ON p.usucpf = u.usucpf
			LEFT JOIN demandas.tiposervico t ON t.tipid = a.tipid
			LEFT JOIN demandas.origemdemanda o ON o.ordid = t.ordid
			
				WHERE a.tutstatus = 'A' $clausula ";
		//dbg($sql);
		$cabecalho = array( "Opções","Data da Inclusão","Nome do Arquivo","Origem","Tipo Serviço","Tamanho","Descrição do Arquivo","Responsável");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
		
	}
?>
		
			
			
<script type="text/javascript">

function filtraTipo()
{
	document.formAnexos.tipid.value = '';
	document.formAnexos.submit();
}

function validaForm(){
	if(document.formAnexos.tipo.value != 'P'){
		
		if(document.formAnexos.ordid.value == ''){
			alert ('Selecione a Origem.');
			document.formAnexos.ordid.focus();
			return false;
		}
		if(document.formAnexos.tipid.value == ''){
			alert ('Selecione o Tipo Serviço.');
			document.formAnexos.tipid.focus();
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
	var questao = confirm("Tem Certeza?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>
<?php 
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$tipid){
	global $db;
	
	
	if (!$arquivo || !$tipid)
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

	
	//Insere o registro na tabela demandas.tutoriais
	$sql = "INSERT INTO demandas.tutoriais 
			(
				tipid,
				arqid,
				tutdesc,
				tutdtinclusao,
				tutstatus
			)VALUES(
			    ". $tipid .",
				". $arqid .",
				'".$dados["arqdescricao"]."',
				now(),
				'A'
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/demandas/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/demandas/'.floor($arqid/1000), 0777);
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
	$sql = "UPDATE demandas.tutoriais SET
				tutstatus = 'I' 
			WHERE tutid = {$_SESSION['tutid']}";
	$db->executar($sql);
	$db->commit();
	//header( "Location: demandas.php?modulo=sistema/apoio/catalogoAnexo&acao=A" );
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



