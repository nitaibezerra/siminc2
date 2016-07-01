<?php
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/geral/funcoesPublicacao.php';
$db = new cls_banco();

if( $_REQUEST['requisicao'] == 'copiarArquivos' ){
	
	$puaid = $_POST['puaid'];
	
	$sql = "SELECT puaarquivo FROM seguranca.publicacaoarquivo WHERE puaid = $puaid";
	$puaarquivo = $db->pegaUm($sql);
	
	$arrArquivo = explode('<br />', $puaarquivo);
	
	$sql = "SELECT purpastaorigem, purpastadestino FROM seguranca.publicacaoresponsavel WHERE usucpf = '{$_SESSION['usucpf']}'";
	$pubResp = $db->pegaLinha($sql);
	
	$pastaOrigem = str_ireplace("\\", '/', $pubResp['purpastaorigem']);
	$pastaDestino = str_ireplace("\\", '/', $pubResp['purpastadestino']);
	
	$arrNaoPubl = array();
	foreach ($arrArquivo as $arquivo) {
		$arquivo = str_ireplace("\\", '/', strip_tags($arquivo));
		
		$arqOrigem 	= $pastaOrigem.$arquivo;
		$arqDestino = $pastaDestino.$arquivo;
		
		$retorno = copiarArquivos($arqOrigem, $arqDestino);
		if( !$retorno ){
			array_push($arrNaoPubl, $arquivo);
		}
	}
	
	echo '1';
	exit();
}
if( $_REQUEST['requisicao'] == 'salvarPublicacao' ){
	header('content-type: text/html; charset=ISO-8859-1');
	ver($_POST,d);
	extract($_POST);
	if( is_array($_POST['purid']) && $_POST['purid'][0] ){
		$db->executar("UPDATE seguranca.publicacaoresparquivo SET prastatus = 'I' WHERE puaid = $puaid");
		foreach ($_POST['purid'] as $purid) {
			$sql = "INSERT INTO seguranca.publicacaoresparquivo(puaid, purid) 
					VALUES ($puaid, $purid)";
			$db->executar($sql);
		}
		$sql = "UPDATE seguranca.publicacaoarquivo SET puasituacao = 'AP', puadatasolicitacao = now() WHERE puaid = $puaid";
		$db->executar($sql);
		echo $db->commit();
	}
	exit();
}

if( $_REQUEST['requisicao'] == 'script' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$puaid = $_REQUEST['puaid'];
	
	$puascript = $db->pegaUm("select puascript from seguranca.publicacaoarquivo where puaid = $puaid"); 
	
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
		<tr>
			<th colspan="4" style="text-align: center;">Script</th>
		</tr>
		<tr>
			<td><?=$puascript?></td>
		</tr>
	</table>
	<?
	exit();
}
if( $_REQUEST['requisicao'] == 'publicar' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$puaid = $_REQUEST['puaid'];
	
	?>
	<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen"> 
	<form action="" method="post" id="formpublicar" name="formpublicar">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
		<tr>
			<th colspan="4" style="text-align: center;">Publicação</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Versão SVN:</td>
			<td><? echo campo_texto( 'puaversaosvn', 'S', 'S', '', 55, 500, '[#]', '','','','','id="puaversaosvn"'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Justificaitva:</td>
			<td><? echo campo_textarea('puajutificativa', 'S', 'S', 'Justificaitva', 75, 5, 4000, '', '', '', '', 'Justificaitva');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Situação:</td>
			<td><?
				$arSit = array(
							array('codigo' => 'CA', 'descricao' => 'Cadastramento'),
							array('codigo' => 'AP', 'descricao' => 'Aguardando Publicação'),
							array('codigo' => 'EP', 'descricao' => 'Em Publicação'),
							array('codigo' => 'PU', 'descricao' => 'Publicado'),
							array('codigo' => 'NO', 'descricao' => 'Não Publicado')
							);
			
			echo $db->monta_combo("puasituacao", $arSit, 'S','-- Selecione a Situação --','', '', '',350,'S','puasituacao');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita"></td>
			<td><input type="button" name="btnCopiar" id="btnCopiar" style="cursor: pointer;" onclick="copiarArquivos(<?=$puaid; ?>);" value="Copiar Arquvos alterados em desenvolvimento para produção"></td>
		</tr>
	</table>
	</form>
	<?
	exit();
}


$abaAtivaGeral = explode('/', $_SERVER['SCRIPT_NAME']);
$abaAtivaGeral = $abaAtivaGeral[2];

echo montaAbaPublicacao($abaAtivaGeral);
?>
<head>
<META http-equiv="Pragma" content="no-cache" />
<title>SIMEC - Publicação</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<style type="">
	.ui-dialog-titlebar{
    	text-align: center;
    }
</style>
<script type="text/javascript">
	window.focus();	
</script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0"	marginheight="0" bgcolor="#ffffff">
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Lista de Solicitações</label></td>
    	</tr>
    	</tbody>
    </table>	
    <?
    	$sql = "SELECT 
    				(case when puascript is not null then
    					'<center>
		    				<img src=\"/imagens/send.png\" border=0 title=\"Publicar\" style=\"cursor:pointer;\" onclick=\"enviarPublicacao('|| pa.puaid ||');\">&nbsp;
		    				<img src=\"/imagens/aceite_os_2.png\" border=0 title=\"Rodar Script\" style=\"cursor:pointer;\" onclick=\"scriptBanco('|| pa.puaid ||');\">&nbsp;
		    			</center>'
    				else
					  '<center>
		    				<img src=\"/imagens/send.png\" border=0 title=\"Publicar\" style=\"cursor:pointer;\" onclick=\"enviarPublicacao('|| pa.puaid ||');\">&nbsp;
		    			</center>'
		    		 end) as acao,
					  pt.putdescricao,
					  usu.usunome,
					  pa.puaarquivo,
					  to_char(pa.puadatasolicitacao, 'DD/MM/YYYY HH24:MI:SS') as data,
					  (case when pa.puasituacao = 'CA' then 'Em Cadastramento'
					  	when pa.puasituacao = 'AP' then 'Aguardando Publicação'
					  	when pa.puasituacao = 'EP' then 'Em Publicação'
					  	when pa.puasituacao = 'PU' then 'Publicado'
					  	when pa.puasituacao = 'NO' then 'Não Publicado'
					  else '-' end) as pendencia,
					  pa.puajutificativa
				FROM 
				  	seguranca.publicacaoarquivo pa
					inner join seguranca.publicacaotipo pt on pt.putid = pa.putid and pt.putstatus = 'A'
					inner join seguranca.usuario usu on usu.usucpf = pa.puacpfsolicitante
					inner join seguranca.publicacaoresparquivo pra on pra.puaid = pa.puaid
                    inner join seguranca.publicacaoresponsavel pr on pr.purid = pra.purid
				WHERE
					pa.puasituacao = 'AP'
					and pr.usucpf = '{$_SESSION['usucpf']}'";
    	
    	$cabecalho = array( "Ação", "Descrição", "Solicitante", "Arquivo", "Data Solicitação", "Situação", "Justificativa");
    	$db->monta_lista_simples($sql, $cabecalho, 20, 10, 'N', '98%', 'N', false, false, false, true);
    ?>
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="2" align="center">
			<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="window.close();">
		</td>
	</tr>
</table>
<div id="dialog_recurso" title="Publicar Arquivos" style="display: none;" >
	<div style="padding:5px;text-align:justify;" id="mostraRetornoRecurso"></div>
</div>
<script type="text/javascript">

function enviarPublicacao(puaid){
	jQuery.ajax({
		type: "POST",
		url: "publicarArquivos.php",
		data: "requisicao=publicar&puaid="+puaid,
		async: false,
		success: function(msg){
			jQuery("#mostraRetornoRecurso").css('height', '260px');
			
			jQuery( "#dialog_recurso" ).show();
			jQuery( "#mostraRetornoRecurso" ).html(msg);
			jQuery( '#dialog_recurso' ).dialog({
					resizable: false,
					width: 600,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Salvar': function() {
							
							if( $('[name="puaversaosvn"]').val() == '' ){
								alert('O campo "Versão SVN" é obrigatório.');
								$('[name="puaversaosvn"]').focus();
								return false;
							}else if( $('[name="puajutificativa"]').val() == '' ){
								alert('O campo "Justificaitva" é obrigatório.');
								$('[name="puajutificativa"]').focus();
								return false;
							} else if( $('[name="puasituacao"]').val() == '' ){
								alert('O campo "Situação" é obrigatório.');
								$('[name="puasituacao"]').focus();
								return false;
							} else {
								solicitarPublicacao(puaid);
								jQuery( this ).dialog( 'close' );
							}
						},
						'Cancelar': function() {
							jQuery( this ).dialog( 'close' );
						}
					}
			});
		}
	});
}

function scriptBanco(puaid){
	jQuery.ajax({
		type: "POST",
		url: "publicarArquivos.php",
		data: "requisicao=script&puaid="+puaid,
		async: false,
		success: function(msg){
			jQuery("#mostraRetornoRecurso").css('height', '500px'); 
			jQuery("#mostraRetornoRecurso").css('overflow', 'auto'); 
			
			jQuery( "#dialog_recurso" ).show();
			jQuery( "#mostraRetornoRecurso" ).html(msg);
			jQuery( '#dialog_recurso' ).dialog({
					resizable: false,
					width: 1000,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {						
						'Fechar': function() {
							jQuery( this ).dialog( 'close' );
						}
					}
			});
		}
	});
}

function solicitarPublicacao( puaid ){
	var dados = jQuery('#formpublicar').serialize();
	
	jQuery.ajax({
		type: "POST",
		url: "publicarArquivos.php",
		data: "requisicao=salvarPublicacao&puaid="+puaid+"&"+dados,
		async: false,
		success: function(msg){
			
			if(msg == 1){
				alert('Operação Realizada com Sucesso!');
			} else {
				alert('Falha na Operação!');
			}
			window.location.href = window.location;
		}
	});
}

function copiarArquivos(puaid){
	jQuery.ajax({
		type: "POST",
		url: "publicarArquivos.php",
		data: "requisicao=copiarArquivos&puaid="+puaid,
		async: false,
		success: function(msg){
			//jQuery('#debug').html(msg);
			if(msg == 1){
				alert('Arquivos copiados com Sucesso!');
			} else {
				alert('Falha na Operação!');
			}
		}
	});
}
</script>
<div id="debug"></div>
</body>