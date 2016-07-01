<?php

include_once '../../cproc/classes/ModeloCproc.class.inc';
include_once '../../cproc/classes/DisponibilidadeDeVistas.class.inc';

if( !verificaExistenciaPrcid() ){
	echo "
		<script>
			alert('Sessão perdeu processo.');
			window.close();
		</script>";
	exit;	
}
$processo = exibirProcesso($prcid);

/*
 * Executa a função que insere o arquivo
 * Retorna mensagem de sucesso e redireciona a página
 */
// ver($_FILES,d);
if ($_FILES['anexo']['size']){
 	 $dados["dpvid"] = $_SESSION['dpvid'];
 	 $dados["arqdescricao"] = $_REQUEST["arqdescricao"];

 	 
	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], $dados)){
	 	unset($_FILES['anexo']['size']);
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='cproc.php?modulo=principal/disponibilidadedevistasdocumentosanexados&acao=A&dpvid=".$_GET['dpvid']."';
			 </script>");
	 }else{
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
	
}

if( $_GET['excluir'] ){
	if( deletarAnexo( $_GET['dpaid'] ) )
		echo "<script>
			alert('Anexo excluído com sucesso!');
			window.history.back();
		</script>";
	else
		echo "<script>
			alert('erro ao excluir anexo.');
			window.history.back();
		</script>";
	exit;
}

monta_titulo( 'Anexo de Documentos de Vistas Disponibilizadas', '' );

?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

 
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">

<div>
	<form method="post" name="formDisponibilidadeDeVistasArquivo" enctype="multipart/form-data">
	<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
	<input name="dpvid_campo" id="dpvid_campo" type="hidden" value="<?=($DisponibilidadeDeVistas->dpvid)?$DisponibilidadeDeVistas->dpvid:''?>"/>
	<input name="prcid_campo" id="prcid_campo" type="hidden" value="<?=$processo['prcid']?>"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="subtituloDireita">Nº do Processo:</td>
            <td>
                <?php
                    echo campo_texto('prcnumsidoc_campo', 'S', 'N', 'Nº Controle SIMEC', '51', '25', '####.######/####-##', '', '', '', '', 'id="prcid_campo"', '', $processo['prcnumsidoc'], '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Mantenedora:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  co_mantenedora AS codigo,
                                no_razao_social AS descricao
                        FROM emec.mantenedora
                        ORDER BY no_razao_social
                    ";
                    $db->monta_combo('co_mantenedora_campo', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_mantenedora'],'Tipo de Processo');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Mantida:</td>
            <td>
                <?php
                   	$sql = "
                        SELECT  co_ies AS codigo,
                                no_ies AS descricao
                        FROM emec.ies
                        ORDER BY no_ies
                    ";
                    $db->monta_combo('co_ies_campo', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_ies'],'Tipo de Processo');
                ?>
            </td>
        </tr>
	<tr>
		<td align='right' class="SubTituloDireita" valign="top">Título:</td>
		<td><?= campo_texto('arqdescricao','N', $habil,'',60,140,'','','left','',0, '') ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" size="80" name="anexo" /><?php echo obrigatorio(); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan="2">
	    	<input type='button' class='botao' value='Salvar' name='Salvar' onclick="validaForm()" />
		</td>			
	</tr>
	</table>
	</form>
<?php

	$sql = " 
		select 
			'<a style=\"cursor:pointer\" title=\"Download\" onclick=\"editar( ' || da.dpaid || ' )\"><img src=\"/imagens/pnld/seta_download.gif\"/></a>
			<a style=\"cursor:pointer\" title=\"Excluir arquivo\" onclick=\"excluir( ' || da.dpaid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
			a.arqdata,
			a.arqnome,
			a.arqtamanho,
			d.dpvservidorresponsavelcharacter
		from cproc.disponibilidadevistasarquivos da
		join public.arquivo a on a.arqid = da.arqid
		join cproc.disponibilidadevistas d on d.dpvid = da.dpvid
		where 
			dpvastatus = 'A' and
			da.dpvid = ".$_GET['dpvid']." 
	";
	$alinhamento = array('left','left','left','left');
	$larguras = array('10%','35%','20%','35%');
	$cabecalho = array( "Ações","Data de Inclusão","Nome do Arquivo","Tamanho","Servidor Responsável");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', ' ','listaDeArquivos', $larguras, $alinhamento );
	
?>
		
			
			
<script type="text/javascript">

function validaForm(){
	if(document.formDisponibilidadeDeVistasArquivo.arqdescricao.value == ''){
		alert ('O campo Nome deve ser preenchido.');
		document.formDisponibilidadeDeVistasArquivo.arqdescricao.focus();
		return false;
	}
	if(document.formDisponibilidadeDeVistasArquivo.anexo.value == ''){
		alert ('Selecione o Arquivo.');
		document.formDisponibilidadeDeVistasArquivo.anexo.focus();
		return false;
	}

	document.formDisponibilidadeDeVistasArquivo.submit();
}

function excluir( chave ) {
	
	var questao = confirm("Deseja realmente excluir este anexo?")
	if (questao){
		window.location = 'cproc.php?modulo=principal/disponibilidadedevistasdocumentosanexados&acao=A&excluir=1&dpaid='+chave+'&dpvid=<?=$_GET['dpvid']?>';
	}

}
</script>
</body>
<?php 
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null){
	global $db;
	
	
	if (!$arquivo)
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

	
	//Insere o registro na tabela cproc.pessoalescolaarquivo
	$sql = "INSERT INTO cproc.disponibilidadevistasarquivos 
			(
				dpvid,
				arqid
			)VALUES(
			    ". $_GET['dpvid'] .",
				". $arqid ."
			);";
	$db->executar($sql);

	if(!is_dir('../../arquivos/cproc')) {
		$pasta = APPRAIZ.'arquivos/cproc';
		mkdir($pasta, 0777);
	}
	
	if(!is_dir('../../arquivos/cproc/'.floor($arqid/1000))) {
		$pasta = APPRAIZ.'arquivos/cproc/'.floor($arqid/1000);
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
			WHERE arqid = ( select arqid from cproc.disponibilidadevistasarquivos where dpaid = {$_GET['dpaid']} )";
	// exit($sql);
	$db->executar($sql);
	$sql = "UPDATE cproc.disponibilidadevistasarquivos SET
				dpvastatus = 'I' 
			WHERE dpaid = {$_GET['dpaid']} ";
	$db->executar($sql);
	return $db->commit();
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



