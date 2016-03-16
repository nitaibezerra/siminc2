<?php

//verifica session
verificaSessao();

$perfil = arrayPerfil();

//verifica permissão
 if( in_array(PERFIL_CADASTRADOR_INEP,$perfil) || in_array(PERFIL_CONSULTA,$perfil) || in_array(PERFIL_INTERESSADO,$perfil) || in_array(PERFIL_VALIDADOR, $perfil) ) {
$vPermissao = 'N';
 }

$vpflcod = arrayPerfil();
if( in_array(PERFIL_SUPER_USUARIO, $vpflcod) || in_array(PERFIL_CADASTRADOR, $perfil) ){
	$vPermissao = 'S';
}


if($_GET['tdeid'] && $_GET['arqid'] && $_GET['op5']){
	session_start();
	$_SESSION['tdeid'] = $_GET['tdeid'];
	$_SESSION['arqid'] = $_GET['arqid'];
	$_SESSION['op5'] = $_GET['op5'];
	$_SESSION['tpdid'] = $_GET['tpdid'];
	$_SESSION['tipo'] = $_GET['tipo'];
	header( "Location: escolasexterior.php?modulo=principal/cadastroDocumento&acao=A" );
	exit();
}

if($_REQUEST['op5'] == 'download'){

	$param = array();
	$param["arqid"] = $_REQUEST['arqid'];
	DownloadArquivo($param);
	exit;

}

if($_SESSION['tdeid'] && $_SESSION['arqid'] && $_SESSION['op5'] && $_SESSION['tpdid']){
	if($_SESSION['op5'] == 'delete'){
		deletarAnexo();	
		$tpdid = $_SESSION['tpdid'];
		$tipo = $_SESSION['tipo'];
		unset($_SESSION['tdeid']);
		unset($_SESSION['arqid']);
		unset($_SESSION['op5']);
		unset($_SESSION['tpdid']);
		unset($_SESSION['tipo']);
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='escolasexterior.php?modulo=principal/cadastroDocumento&acao=A';
				//history.go(-1);
			 </script>");
	}
	
}

/*
 * Executa a função que insere o arquivo
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_FILES['anexo']['size']){
 	 $tpdid = $_REQUEST['tpdid'];
 	 $tipo = $_REQUEST['tipo'];
 	 $dados["eexid"] = $_SESSION['eexid'];
 	 $dados["arqdescricao"] = $_REQUEST["arqdescricao"];

 	 
	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], $dados, $tpdid)){
	 	unset($_FILES['anexo']['size']);
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href='escolasexterior.php?modulo=principal/cadastroDocumento&acao=A';
				//history.go(-1);
			 </script>");
	 }else{
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
	
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo = "ESCOLAS NO EXTERIOR";

$dsctitulo = 'CADASTRO DE DOCUMENTOS<BR><img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';


montaAbasEscolasExterior();

monta_titulo( $titulo, $dsctitulo );

cabecalhoEE($_SESSION['peeid']);

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
		<td class="SubTituloDireita" width="32%">Tipo de Documento:</td>
		<td>
			<?php
				$tpdid = $_REQUEST['tpdid']; 
				
				$whereExtra = "";
				
//				if( in_array(PERFIL_CADASTRADOR,$perfil) 
//					&& !in_array(PERFIL_ADMINISTRADOR,$perfil) 
//					&& !in_array(PERFIL_SUPER_USUARIO,$perfil) )
//					$whereExtra .= " AND tpdid < 8 ";
//				if( in_array(PERFIL_CADASTRADOR,$perfil) 
//					&& !in_array(PERFIL_SUPER_USUARIO,$perfil) 
//					&& !in_array(PERFIL_ADMINISTRADOR,$perfil) )
//					$whereExtra .= " AND tpdid = ".TPD_PARECER_CNE." and = ".TPD_HOMOLOGAÇÃO." ";
//				if( !in_array(PERFIL_PARECERISTA_CNE,$perfil) 
//					&& !in_array(PERFIL_ADMINISTRADOR,$perfil) 
//					&& !in_array(PERFIL_SUPER_USUARIO,$perfil) )
//					$whereExtra .= " AND tpdid != ".TPD_PARECER_CNE." ";
//				if( 
//					!in_array(PERFIL_APROVADOR_SEB,$perfil) 
//					&& !in_array(PERFIL_PARECERISTA_CNE,$perfil) 
//					&& !in_array(PERFIL_ADMINISTRADOR,$perfil) 
//					&& !in_array(PERFIL_SUPER_USUARIO,$perfil) )
//					$whereExtra .= " AND tpdid != ".TPD_NOTA_TECNICA." AND tpdid != ".TPD_DESPACHO." ";
                                if( in_array(PERFIL_PARECERISTA_CONJUR,$perfil)) {
                                    $whereExtra .= " AND tpdid IN(' ".TPD_PARECER_CONJUR."', '".TPD_OUTROS."') ";
                                }
                                if( in_array(PERFIL_PARECERISTA_CNE, $perfil) ) {
									$whereExtra .= " AND tpdid IN(' ".TPD_PARECER_CNE."','".TPD_HOMOLOGACAO."', '".TPD_OUTROS."') ";
								}
                                if( in_array(PERFIL_HOMOLOGADOR,$perfil)) {
                                    $whereExtra .= " AND tpdid IN(' ".TPD_DESPACHO."','".TPD_HOMOLOGACAO."', '".TPD_OUTROS."') ";
                                }
                                if( in_array(PERFIL_APROVADOR_SEB,$perfil)) {
                                    $whereExtra .= " AND tpdid IN(' ".TPD_NOTA_TECNICA."', '".TPD_OUTROS."') ";
                                }
                                if( in_array(PERFIL_CADASTRADOR,$perfil) || in_array(PERFIL_VALIDADOR,$perfil)) {
                                    $whereExtra .= " AND tpdid IN(' ".TPD_COMPROVACAO."', '".TPD_DESCRICAO."','".TPD_ORGANIZACAO."','".TPD_PROPOSTA."','".TPD_REGIMENTO."', '".TPD_OUTROS."') ";
                                }

				$sql = "SELECT
							tpdid AS codigo,
							tpddsc AS descricao
						FROM
							escolasexterior.tipodocumento
						WHERE 
							tpdstatus = 'A' 
							{$whereExtra}
						ORDER BY
						 	tpddsc;";
				//ver($sql,d);
				
				$db->monta_combo("tpdid",$sql,$vPermissao,"-- Selecione o Tipo de Documento --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
		<td align="right" rowspan="5">
			
				<?php
					if($_SESSION['eexid']){
						$docid = criaDocumento( $_SESSION['eexid'] );
						// ver($docid,$_SESSION['sisdiretorio']);
						if($docid){
							wf_desenhaBarraNavegacao( $docid , array("eexid" => $_SESSION['eexid']) );
						} 
					}
				?>
			
		</td>
	</tr>
	
	<?if($_REQUEST['tipo'] == 'P'){?>
		<tr>
			<td class="SubTituloDireita">Nome / Descrição:</td>
			<td><?= campo_texto('nomepesquisa','N', $vPermissao,'',50,140,'','','left','',0, '') ?></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>&nbsp;</td>
			<td >
		    	<input type='submit' class='botao' value='Pesquisar' name='Pesquisar' >
		    	<input type='button' class='botao' value='Voltar' name='voltar' onclick="history.back();" >
			</td>			
		</tr>
	<?}else{?>
		<tr>
			<td class="SubTituloDireita">Arquivo:</td>
			<td><input type="file" size="80" name="anexo" <?if($vPermissao=='N') echo "disabled";?>><? echo obrigatorio(); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" valign="top">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', $vPermissao, '', 100, 10, '1000' ); ?></td>
		</tr>
		<tr style="background: rgb(238, 238, 238)">
			<td>&nbsp;</td>
			<td >
		    	<input type='submit' class='botao' value='Salvar' name='Salvar' <?if($vPermissao=='N') echo "disabled";?>>	    	
		    	<input type='button' class='botao' value='Modo Pesquisa' name='modopesquisa' onclick="location.href='escolasexterior.php?modulo=principal/cadastroDocumento&acao=A&tipo=P&tpdid=<?=$_REQUEST['tpdid']?>'">
			</td>			
		</tr>
	<?}?>
	<tr>
		<td colspan="3" height="40">&nbsp;</td>
	</tr>
</table>
</form>
<?
	//if($_REQUEST['tpdid'] || $_REQUEST['nomepesquisa']){
		
		//exibe botao excluir
		if( in_array(PERFIL_SUPER_USUARIO,$perfil) ){
			$btnExcluir = " OR 1=1 ";
		}
	
		//filtros para a pesquisa
		$clausula = "";
	    if($_REQUEST['tpdid']) $clausula .= " AND a.tpdid = {$_REQUEST['tpdid']} ";
	    
	    // Estas regras limitam a apresentação da lista de documento para os tipos referentes ao perfil.
	    /*if( in_array(PERFIL_PARECERISTA_CONJUR,$perfil)) {
	    	$clausula .= " AND a.tpdid IN(' ".TPD_PARECER_CONJUR."') ";
	    }
	    if( in_array(PERFIL_PARECERISTA_CNE, $perfil) ) {
	    	$clausula .= " AND a.tpdid IN(' ".TPD_PARECER_CNE."','".TPD_HOMOLOGACAO."') ";
	    }
	    if( in_array(PERFIL_HOMOLOGADOR,$perfil)) {
	    	$clausula .= " AND a.tpdid IN(' ".TPD_DESPACHO."','".TPD_HOMOLOGACAO."') ";
	    }
	    if( in_array(PERFIL_APROVADOR_SEB,$perfil)) {
	    	$clausula .= " AND a.tpdid IN(' ".TPD_NOTA_TECNICA."') ";
	    }
	    if( in_array(PERFIL_CADASTRADOR,$perfil) || in_array(PERFIL_VALIDADOR,$perfil)) {
	    	$clausula .= " AND a.tpdid IN(' ".TPD_COMPROVACAO."', '".TPD_DESCRICAO."','".TPD_ORGANIZACAO."','".TPD_PROPOSTA."','".TPD_REGIMENTO."') ";
	    }*/
	    
		if($_REQUEST['nomepesquisa']) $clausula .= " AND ( upper(p.arqnome) like '%".strtoupper($_REQUEST['nomepesquisa'])."%' OR upper(a.tdedsc) like '%".strtoupper($_REQUEST['nomepesquisa'])."%' ) ";
		 
		
		$sql = "
			SELECT
				CASE WHEN p.usucpf = '{$_SESSION['usucpf']}' $btnExcluir THEN 
					'<center><a href=\"escolasexterior.php?modulo=principal/cadastroDocumento&acao=A&arqid=' || a.arqid || '&op5=download\">
					 	<img border=0 src=\"../imagens/anexo.gif\" title=\"Baixar Arquivo\" />
					 </a> 
					 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'escolasexterior.php?modulo=principal/cadastroDocumento&acao=A&arqid=' || a.arqid || '&tdeid=' || a.tdeid || '&tpdid=' || a.tpdid || '&op5=delete\');\" >
					 	<img border=0 src=\"../imagens/excluir.gif\" title=\"Excluir Arquivo\" />
					 </a></center>'
				ELSE
					'<center><a href=\"escolasexterior.php?modulo=principal/cadastroDocumento&acao=A&arqid=' || a.arqid || '&op5=download\">
					 	<img border=0 src=\"../imagens/anexo.gif\" title=\"Baixar Arquivo\" />
					 </a></center>'
				END AS OPCOES,
				t.tpddsc as tipo,
				'<a style=\"cursor: pointer; color: blue;\" title=\"Baixar Arquivo\" href=\"escolasexterior.php?modulo=principal/cadastroDocumento&acao=A&arqid=' || a.arqid || '&op5=download\">' || p.arqnome || '</a>' AS arqnome,
				a.tdedsc,
				p.arqtamanho || ' kb' AS arqtamanho,
				to_char(p.arqdata,'DD/MM/YYYY') ||' '|| p.arqhora AS dtinclusao,
				u.usunome
			
			FROM escolasexterior.tpdocescolaext AS a

			LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
			LEFT JOIN seguranca.usuario AS u ON p.usucpf = u.usucpf
			LEFT JOIN escolasexterior.tipodocumento t ON t.tpdid = a.tpdid
			
			WHERE 
				a.tdestatus = 'A' 
				AND a.eexid = '{$_SESSION['eexid']}'
				$clausula 
			order by p.arqdata, p.arqhora";
		//dbg($sql);
		$cabecalho = array( "<center>Opções</center>","Tipo de Documento","Nome do Arquivo","Descrição","Tamanho","Data da Inclusão","Responsável");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
		
	//}
?>
		
			
			
<script type="text/javascript">

function filtraTipo()
{
	document.formAnexos.tpdid.value = '';
	document.formAnexos.submit();
}

function validaForm(){
	if(document.formAnexos.tipo.value != 'P'){
		
		if(document.formAnexos.tpdid.value == ''){
			alert ('Selecione o Tipo de Documento.');
			document.formAnexos.tpdid.focus();
			return false;
		}
		if(document.formAnexos.anexo.value == ''){
			alert ('Selecione o Arquivo.');
			document.formAnexos.anexo.focus();
			return false;
		}
		if(document.formAnexos.arqdescricao.value == ''){
			alert('Informe a descrição.');
			document.formAnexos.arqdescricao.focus();
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
	if ('<?=$vPermissao?>' == 'N'){
		alert('Seu perfil não lhe permite executar esta operação!');
		return;
	}	
	var questao = confirm("Deseja realmente excluir este documento?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>
<?php 
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$tpdid){
	global $db;
	
	
	if (!$arquivo || !$tpdid)
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

	
	//Insere o registro na tabela escolasexterior.tpdocescolaext
	$sql = "INSERT INTO escolasexterior.tpdocescolaext 
			(
				eexid,
				tpdid,
				arqid,
				tdedsc,
				tdestatus
			)VALUES(
				".$dados["eexid"].",
			    ". $tpdid .",
				". $arqid .",
				'".$dados["arqdescricao"]."',
				'A'
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/escolasexterior/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/escolasexterior/'.floor($arqid/1000), 0777);
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
				arqstatus = 'I' 
			WHERE arqid = {$_SESSION['arqid']}";
	$db->executar($sql);
	$sql = "UPDATE escolasexterior.tpdocescolaext SET
				tdestatus = 'I' 
			WHERE tdeid = {$_SESSION['tdeid']}";
	$db->executar($sql);
	$db->commit();
	//header( "Location: escolasexterior.php?modulo=principal/cadastroDocumento&acao=A" );
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



