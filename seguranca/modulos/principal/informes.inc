<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['op3'] == 'download'){
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
    DownloadArquivoInfo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}



function inserirInfo(){
	global $db;
	
	$sql = " INSERT INTO seguranca.informes
			 (
			 	ifmtitulo, 
			 	ifmtexto,
			 	sisid,
			 	ifmmodal,
			 	ifmdatainicio,
			 	ifmdatafim,
			 	usucpf,
			 	ifmstatus
			 ) VALUES (
			 	'".$_POST['ifmtitulo']."',
			 	'".$_POST['ifmtexto']."',
			 	".$_POST['sisid'].",
			 	'".$_POST['ifmmodal']."',
			 	'".formata_data_sql($_POST['ifmdatainicio']).' '.$_POST['hini']."',
			 	".($_POST['ifmdatafim'] ? "'".formata_data_sql($_POST['ifmdatafim']).' '.$_POST['hfim']."'" : 'null').", 
			 	'".$_SESSION['usucpf']."',
			 	'A'
			 ) RETURNING ifmid";
	$ifmid = $db->pegaUm($sql);
	$db->commit();
	
	return $ifmid;
}


function alterarInfo(){
	global $db;
	
	$sql = "UPDATE 
				seguranca.informes 
			SET 
				ifmtitulo = '".$_POST['ifmtitulo']."',
				ifmtexto = '".$_POST['ifmtexto']."',
				ifmmodal = '".$_POST['ifmmodal']."',
				ifmdatainicio = '".formata_data_sql($_POST['ifmdatainicio']).' '.$_POST['hini']."',
				ifmdatafim = ".($_POST['ifmdatafim'] ? "'".formata_data_sql($_POST['ifmdatafim']).' '.$_POST['hfim']."'" : 'null').",
				sisid = ".$_POST['sisid'].",
				usucpf = '".$_SESSION['usucpf']."'
			WHERE 
				ifmid = '".$_POST['ifmid']."';";

	$db->executar($sql);					
	$db->commit();
}



function deletarInfo(){
	global $db;
	
	$sql = "UPDATE seguranca.informes SET ifmstatus='I' WHERE ifmid=".$_SESSION['ifmid'];
	$db->executar($sql);
	$db->commit();
}

function selecionarInfo(){
	global $db;
	$sql = "SELECT 
				ifmtitulo,
				ifmtexto,
				ifmmodal,
				to_char(ifmdatainicio::timestamp,'DD/MM/YYYY') as ifmdatainicio,	
				to_char(ifmdatafim::timestamp,'DD/MM/YYYY') as ifmdatafim,
				to_char(ifmdatainicio, 'HH24:MI') AS hini,
		 		to_char(ifmdatafim, 'HH24:MI') AS hfim,
				ifmid,
				sisid
			FROM  
				seguranca.informes i 
			WHERE 
				i.ifmid = '".$_SESSION['ifmid']."'";
	return $db->carregar($sql);
}





if($_POST['ifmtitulo'] && !$_POST['ifmid']){
	$ifmid = inserirInfo();
	
	if($_FILES['arquivo']['size'] > 0 && $ifmid){
		$dados["arqdescricao"] = "Arquivo Informe - ifmid: ".$ifmid;
		$retorno = EnviarArquivo($_FILES['arquivo'], $dados, $ifmid);
		if($retorno == false){
			die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");
		}
	}
	 
	unset($_SESSION['ifmid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='seguranca.php?modulo=principal/informes&acao=A'</script>";
	exit();
	
}

if($_POST['ifmtitulo'] && $_POST['ifmid']){
	alterarInfo();
	
	$ifmid = $_POST['ifmid'];
	
	if($_FILES['arquivo']['size'] > 0 && $ifmid){
		$dados["arqdescricao"] = "Arquivo Informe - ifmid: ".$ifmid;
		$retorno = EnviarArquivo($_FILES['arquivo'], $dados, $ifmid);
		if($retorno == false){
			die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");
		}
	}
	
	unset($_SESSION['ifmid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='seguranca.php?modulo=principal/informes&acao=A'</script>";
	exit();
}

if($_GET['ifmid'] && $_GET['op3']){
	session_start();
	$_SESSION['ifmid'] = $_GET['ifmid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: seguranca.php?modulo=principal/informes&acao=A" );
	exit();
}

if($_SESSION['ifmid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarInfo();	
		unset($_SESSION['ifmid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='seguranca.php?modulo=principal/informes&acao=A'</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarInfo();	
		unset($_SESSION['ifmid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Cadastro de Informe', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script src="../includes/calendario.js"></script>

<form id="formulario" name="formulario" method="post" enctype="multipart/form-data" onsubmit="return validaForm();" >

<input type="hidden" name="ifmid" value="<? echo $dados[0]['ifmid']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Título:</td>
		<td>
			<? $ifmtitulo = $dados[0]['ifmtitulo'];  ?>
			<?= campo_texto( 'ifmtitulo', 'S', 'S', '', 40, 150, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Texto:</td>
		<td>
			<? $ifmtexto = $dados[0]['ifmtexto'];?>
			<?= campo_textarea('ifmtexto', 'S', 'S', '', 75, 5, 2500); ?>
			[LINK] = Clique aqui (link para baixar o arquivo)
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Arquivo:</td>
		<td>
			<input type="file" name="arquivo">
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Sistema:</td>
		<td>
			<?
			$sisid = $dados[0]['sisid'];
			$sql = "SELECT 
					 sisid AS codigo,
					 sisid ||' - '|| upper(sisabrev) ||' - '||  sisdsc AS descricao
					FROM 
					 seguranca.sistema
					WHERE
					 sisstatus = 'A'
					ORDER BY 
					 1";
			$db->monta_combo( 'sisid', $sql, 'S', '-- Informe o Sistema --','','','','','S');
			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data Início:</td>
		<td>
			<? $ifmdatainicio = $dados[0]['ifmdatainicio'];  ?>
			<? $hini = $dados[0]['hini'];  ?>
			<?= campo_data( 'ifmdatainicio', 'S', 'S', '', '' );?>
			&nbsp;
			Hora:<?= campo_texto('hini','S', 'S','',5,5,'##:##','','left','',0, 'id="hini"','','','') ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data Fim:</td>
		<td>
			<? $ifmdatafim = $dados[0]['ifmdatafim'];  ?>
			<? $hfim = $dados[0]['hfim'];  ?>
			<?= campo_data( 'ifmdatafim', 'N', 'S', '', '' );?>
			&nbsp;
			Hora:<?= campo_texto('hfim','N', 'S','',5,5,'##:##','','left','',0, 'id="hfim"','','','') ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Modal:</td>
		<td>
			<? $ifmmodal = $dados[0]['ifmmodal'];?>
			<input type="radio" name="ifmmodal" value="t" <?if($ifmmodal == 't'){echo 'checked';}?>> SIM
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="ifmmodal" value="f" <?if($ifmmodal == '' || $ifmmodal == 'f'){echo 'checked';}?>> NÃO
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		<?php
		if (isset($dados)){
			echo '<input type="button" class="botao" name="btn_novo" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		</td>
	</tr>
</table>
</form>

<?php
	$sql = "SELECT
				'<a href=\"seguranca.php?modulo=principal/informes&acao=A&ifmid=' || i.ifmid || '&op3=update\">
				   <img border=0 src=\"../imagens/alterar.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'seguranca.php?modulo=principal/informes&acao=A&ifmid=' || i.ifmid || '&op3=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' ||
				 case when i.arqid is not null then
				 		'<a href=\"seguranca.php?modulo=principal/informes&acao=A&arqid=' || i.arqid || '&op3=download\">
						 	<img border=0 src=\"../imagens/anexo.gif\" />
						 </a>'
					  else
						 	''
				 end as acao, 
				i.ifmtitulo as nucontrato,
				--replace(replace(ifmtexto, '<', '&lt;'), '>', '&gt;') as texto,
				'<textarea rows=3 cols=80>'||ifmtexto||'</textarea>' as texto,
				s.sisid ||' - '|| upper(s.sisabrev) ||' - '||  s.sisdsc as sistema,
				to_char(i.ifmdatainicio::timestamp,'DD/MM/YYYY HH24:MI') as dtini,	
				to_char(i.ifmdatafim::timestamp,'DD/MM/YYYY HH24:MI') as dtfim,
				case when ifmmodal = true then
						'SIM'
					 else
					 	'NÃO'
				end as modal,
				u.usunome
			FROM 
				seguranca.informes i
			INNER JOIN seguranca.sistema s ON s.sisid = i.sisid
			INNER JOIN seguranca.usuario u ON u.usucpf = i.usucpf
			WHERE
				ifmstatus = 'A'
		  	ORDER BY 
				 i.ifmdatainicio";
	
	$cabecalho = array( "Ação","Título","Texto","Sistema","Data Início","Data Fim","Modal","Responsável");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
?>


<script type="text/javascript">
function validaForm(){
	
	if(document.formulario.ifmtitulo.value == ''){
		alert ('O campo Título deve ser preenchido.');
		document.formulario.ifmtitulo.focus();
		return false;
	}
	if(document.formulario.ifmtexto.value == ''){
		alert ('O campo Texto deve ser preenchido.');
		document.formulario.ifmtexto.focus();
		return false;
	}
	if(document.formulario.sisid.value == ''){
		alert ('O campo Sistema deve ser preenchido.');
		document.formulario.sisid.focus();
		return false;
	}
	if(document.formulario.ifmdatainicio.value == ''){
		alert ('O campo Data Início deve ser preenchido.');
		document.formulario.ifmdatainicio.focus();
		return false;
	}
	if(document.formulario.hini.value == ''){
		alert ('O campo Hora Início deve ser preenchido.');
		document.formulario.hini.focus();
		return false;
	}

	if(!validarRegHora()){
		return false;
	}
	
}

function validarRegHora(){
	hrs_ini = document.getElementById('hini').value.substr(0,2);
	min_ini = document.getElementById('hini').value.substr(3,5);
	hrs_fim = document.getElementById('hfim').value.substr(0,2);
	min_fim = document.getElementById('hfim').value.substr(3,5);

	if(document.getElementById('hini').value.length < 5){
		document.getElementById('hini').focus();	
		alert("Formato Incorreto.");
		return false;
	}
	
	if(document.getElementById('hfim').value){
		if(document.getElementById('hfim').value.length < 5){
			document.getElementById('hfim').focus();	
			alert("Formato Incorreto.");
			return false;	
		}		
	}
	
	if(document.getElementById('hini').value == ""){
		alert("Informe a hora de ínicio.");
		document.getElementById('hini').focus();
		return false;	
	} else {
		if ((hrs_ini < 00 ) || (hrs_ini > 23) || ( min_ini < 00) ||( min_ini > 59)){ 
			alert("Hora de ínicio inválida.");
			document.getElementById('hini').focus();
			return false;	
		} 
	}
	
	if(document.getElementById('hfim').value){
		if ((hrs_fim < 00 ) || (hrs_fim > 23) || ( min_fim < 00) ||( min_fim > 59)){ 
			alert("Hora de fim inválida.");
			document.getElementById('hfim').focus();	
			return false;	
		} 
	}
	/*
	if(document.getElementById('hini').value && document.getElementById('hfim').value){
		if((hrs_ini >= hrs_fim) && (min_ini > min_fim)){
			alert("Hora de Ínicio maior que Hora Fim.");
			document.getElementById('hfim').focus();	
			return false;	
		}
	}
	*/
	return true;
}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>

<?

######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$ifmid){
	global $db;
	
	if (!$arquivo || !$ifmid)
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
	
	//atualiza o registro na tabela seguranca.informes
	$sql = "UPDATE seguranca.informes SET arqid = $arqid WHERE ifmid = {$ifmid}";
	$db->executar($sql);
	
	
	if(!is_dir('../../arquivos/informes/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/informes/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/informes/'. floor($arqid/1000) .'/'. $arqid;
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


function DownloadArquivoInfo($arqid){
	global $db;
	
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
		$arquivo = $db->carregar($sql);
		if($arquivo){
			$arquivo = current($arquivo);
		}else{
			$erro = "Arquivo não encontrado.";
			$getErroSimec();
			return false;
		}
		$caminho = APPRAIZ."arquivos/informes/".floor($arqid/1000).'/'.$arqid;
		if(!Files::Download($caminho)){
			$getErroSimec();
			return false;
		}
	    $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
	    header( 'Content-type: '. $arquivo['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();		   
	}

?>