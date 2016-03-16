<?php
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/geral/funcoesPublicacao.php';
$db = new cls_banco();

if( $_REQUEST['requisicao'] == 'salvarscript' ){
	extract($_POST);
	
	$sql = "UPDATE seguranca.publicacaoarquivo SET
				puascript = '$puascript'
			WHERE
				puaid = $puaid";
	
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Operação Realizada com Sucesso!');
			window.location.href = 'solicitarPublicacao.php?aba=script&puaid=".$puaid."';
		</script>";
	exit();
}
if( $_REQUEST['requisicao'] == 'salvar' ){
	extract($_POST);
	
	if( $_POST['puaid'] ){
		$sql = "UPDATE seguranca.publicacaoarquivo SET
					putid = $putid,
					puademanda = '$puademanda',
					puafuncionalidade = '$puafuncionalidade',
					puarastro = '$puarastro',
					puascript = '$puascript',
					pualink = '$pualink',
					puaarquivo = '$puaarquivo',
					puacpfsolicitante = '{$_SESSION['usucpf']}'
				WHERE
					puaid = $puaid";
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO seguranca.publicacaoarquivo(putid, sisid, puademanda, puafuncionalidade, puarastro, pualink, 
					puaarquivo, puacpfsolicitante, puasituacao, puascript) 
				VALUES ($putid, {$_SESSION['sisid']}, '$puademanda', '$puafuncionalidade', '$puarastro', '$pualink', 
					'$puaarquivo', '{$_SESSION['usucpf']}', 'CA', '$puascript') returning puaid";
		$puaid = $db->pegaUm($sql);
	}
	
	$db->commit();
	echo "<script>
			alert('Operação Realizada com Sucesso!');
			window.location.href = 'solicitarPublicacao.php?aba=solicitar&puaid=".$puaid."';
		</script>";
	exit();
}

$abaAtivaGeral = explode('/', $_SERVER['SCRIPT_NAME']);
$abaAtivaGeral = $abaAtivaGeral[2];

echo montaAbaPublicacao($abaAtivaGeral);

$habilita = 'S';
if( $_REQUEST['puaid'] ){
	$sql = "SELECT puaid, putid, sisid, puademanda, puafuncionalidade, puarastro, pualink, puascript, puaarquivo, 
				puacpfsolicitante, puadatasolicitacao, puasituacao, puajutificativa
			FROM seguranca.publicacaoarquivo WHERE puaid = ".$_REQUEST['puaid'];
	$arPublicacao = $db->pegaLinha($sql);
	extract($arPublicacao);
	
	if( $puasituacao == 'CA' ){
		$habilita = 'S';
	} else {
		$habilita = 'S';
	}
}

$abaAtivaGeral = explode('/', $_SERVER['SCRIPT_NAME']);
$abaAtivaGeral = $abaAtivaGeral[2]."?aba=".($_REQUEST['aba'] ? $_REQUEST['aba'] : 'solicitar').($puaid ? "&puaid=".$puaid : '');
if( $puaid ){
	$menuGeral = array (
			0 => array (
					"id" => 0,
					"descricao" => "Solicitação",
					"link" => "solicitarPublicacao.php?aba=solicitar&puaid=".$puaid
			)
	);
} else {
	$menuGeral = array (
			0 => array (
					"id" => 0,
					"descricao" => "Solicitação",
					"link" => "solicitarPublicacao.php?aba=solicitar"
			)	
	);
}

echo montarAbasArray ( $menuGeral, $abaAtivaGeral );
?>
<html>
<head>
	<META http-equiv="Pragma" content="no-cache" />
	<title>SIMEC - Publicação</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
	<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">        
    <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
     <script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
	<script type="text/javascript">
		window.focus();
	</script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0"	marginheight="0" bgcolor="#ffffff">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Solicitar Publicação</label></td>
    	</tr>
    	</tbody>
    </table>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="aba_atual" id="aba_atual" value="<?=$_REQUEST['aba']; ?>">
	<input type="hidden" name="puaid" id="puaid" value="<?=$puaid; ?>">
	<input type="hidden" name="texto" id="texto" value="<?=simec_htmlentities( str_ireplace( '\"', '"', $puascript) ); ?>">
	<input type="hidden" name="arquivo" id="arquivo" value="<?=simec_htmlentities( str_ireplace( '\"', '"', $puaarquivo) ); ?>">

	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="55%">
		<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="subtitulodireita" width="25%">Tipo da Demanda:</td>
				<td width="75%"><?
				$sql = "SELECT 
						 	putid as codigo,
						  	putdescricao as descricao
						FROM 
						  	seguranca.publicacaotipo
						WHERE
							putstatus = 'A'
						order by putdescricao";
				echo $db->monta_combo("putid", $sql, $habilita,'-- Selecione o Tipo --','', '', '',350,'S','putid');?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Demanda:</td>
				<td><? echo campo_textarea('puademanda', 'S', $habilita, 'Demanda', 98, 2, 4000, '', '', '', '', 'Demanda');?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Funcionalidade:</td>
				<td><?
					$puafuncionalidade = ($puafuncionalidade ? $puafuncionalidade : 'Não Aplica');
					echo campo_textarea('puafuncionalidade', 'S', $habilita, 'Funcionalidade', 98, 2, 4000, '', '', '', '', 'Funcionalidade');?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Rastro:</td>
				<td><?
					$puarastro = ($puarastro ? $puarastro : 'Não Aplica');
					echo campo_texto( 'puarastro', 'S', $habilita, '', 90, 500, '', '','','','','id="puarastro"'); ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Link:</td>
				<td><?
					$pualink = ($pualink ? $pualink : 'Não Aplica');
					echo campo_texto( 'pualink', 'S', $habilita, '', 90, 1000, '', '','','','','id="pualink"'); ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Arquivos para publicação:</td>
				<td><span style="color: red; font-size: 12px;">Ex.: emenda/modulos/principal/nome_do_arquivo.inc ou <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;emenda\modulos\principal\nome_do_arquivo.inc</span>
				<textarea id="puaarquivo" name="puaarquivo" rows="6" cols="68" class="emendatinymce obrigatorio normal"></textarea></td>
			</tr>
		</table>
		</td>
		<td>
			<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="subtitulocentro">Script:</td>
				</tr>
				<tr>
					<td><textarea id="puascript" name="puascript" rows="20" cols="132" style="width:100%" class="emendatinymce obrigatorio normal"></textarea></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" align="center">
			<input type="button" value="Salvar" name="btnSalvar" <?=($habilita=='N' ? 'disabled="disabled"' : ''); ?> id="btnSalvar" style="cursor: pointer;" onclick="salvarSolicitacao();">
			<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="window.close();">
		</td>
	</tr>
</table>
</form>
</body>
<script type="text/javascript">
function salvarSolicitacao(){
	selectAllOptions( document.getElementById( 'purid' ) );
	
	if( $('[name="putid"]').val() == '' ){
		alert('O campo "Tipo da Demanda" é obrigatório.');
		$('[name="putid"]').focus();
		return false;
	}
	
	if( $('[name="puademanda"]').val() == '' ){
		alert('O campo "Demanda" é obrigatório.');
		$('[name="puademanda"]').focus();
		return false;
	}
	
	var ed = tinyMCE.get('puaarquivo').getContent();
	if( ed == "" ){
		alert('O campo "Arquivos para publicação" é obrigatório.');
		return false;
	}
	
	$('#requisicao').val('salvar');
	$('#formulario').submit();
}

document.getElementById('puascript').value = document.getElementById('texto').value;
document.getElementById('puaarquivo').value = document.getElementById('arquivo').value;

tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "emendatinymce",
        plugins : "table,inlinepopups",

        // Theme options
        theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,code",
        theme_advanced_buttons2 : "",
        theme_advanced_buttons3 : "",
        theme_advanced_buttons4 : "",
        theme_advanced_toolbar_location : "top",
        theme_advanced_toolbar_align : "left",
        theme_advanced_resizing : true,

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});

</script>
</html>