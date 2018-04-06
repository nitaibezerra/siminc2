<?php
// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

include_once APPRAIZ . 'www/geral/funcoesPublicacao.php';

$db = new cls_banco();

if( $_REQUEST['requisicao'] == 'solicitar' ){
	header('content-type: text/html; charset=ISO-8859-1');
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
if( $_REQUEST['requisicao'] == 'publicar' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT 
				purid as codigo,
			  	u.usunome||' - '||pr.puremail as descricao
			FROM 
			  	seguranca.publicacaoresponsavel pr
			    inner join seguranca.usuario u on u.usucpf = pr.usucpf
			WHERE
				purstatus = 'A'";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	?>
	<form action="" method="post" id="formpublicar" name="formpublicar">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
		<tr>
			<th colspan="4" style="text-align: center;">Responsável pela Publicação</th>
		</tr>
		<?
		foreach ($arrDados as $v) {
			echo '<tr>
					<td><input type="checkbox" name="purid[]" id="purid" value="'.$v['codigo'].'">&nbsp;'.$v['descricao'].'</td>
				</tr>';
		}
		 ?>
	</table>
	</form>
	<?
	exit();
}

if( $_REQUEST['requisicao'] == 'excluir' ){
	
	$sql = "DELETE FROM seguranca.publicacaoarquivo WHERE puaid = ".$_REQUEST['puaid'];
	$db->executar( $sql );
	
	if( $db->commit() ){
		echo "<script>
				alert('Registro excluido com sucesso!');
				window.location.href = 'listaPublicarArquivos.php';
			</script>";
		exit();
	} else {
		echo "<script>
				alert('Falha na operação!');
				window.location.href = 'listaPublicarArquivos.php';
			</script>";
		exit();
	}
}

$abaAtivaGeral = explode('/', $_SERVER['SCRIPT_NAME']);
$abaAtivaGeral = $abaAtivaGeral[2];

echo montaAbaPublicacao($abaAtivaGeral);

$abaAtivaGeral = explode('/', $_SERVER['SCRIPT_NAME']);
$abaAtivaGeral = $abaAtivaGeral[2]."?aba=".($_REQUEST['aba'] ? $_REQUEST['aba'] : 'pendente');

$menuGeral = array (
		0 => array (
				"id" => 0,
				"descricao" => "Pendentes",
				"link" => "listaPublicarArquivos.php?aba=pendente"
		),
		1 => array (
				"id" => 1,
				"descricao" => "Publicados",
				"link" => "listaPublicarArquivos.php?aba=publicado"
		)	
);
echo montarAbasArray ( $menuGeral, $abaAtivaGeral );
?>
<head>
<META http-equiv="Pragma" content="no-cache" />
<title>SIMEC - Publicação</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
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
<?
	if( $_REQUEST['aba'] != 'publicado' ){
?>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Lista de Solicitações Pendentes</label></td>
    	</tr>
    	</tbody>
    </table>	
    <?
    	$sql = "SELECT 
    				(case when pa.puasituacao = 'CA' then
					  '<center>
		    				<img src=\"/imagens/send.png\" border=0 title=\"Enviar para Publicação\" style=\"cursor:pointer;\" onclick=\"enviarPublicacao('|| pa.puaid ||');\">&nbsp;
		    				<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"alterarPublicacao('|| pa.puaid ||');\">&nbsp;
		    				<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirPublicacao('|| pa.puaid ||');\">&nbsp;
		    			</center>'
		    		else 
		    			'<center>
		    				<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"alterarPublicacao('|| pa.puaid ||');\">&nbsp;
		    				<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\"\">&nbsp;
		    			</center>'
		    		end) as acao,
					  pt.putdescricao,
					  pa.puaarquivo,
					  pa.puademanda,
					  to_char(pa.puadatasolicitacao, 'DD/MM/YYYY HH24:MI:SS') as data,
					  (case when pa.puasituacao = 'CA' then 'Em Cadastramento'
					  	when pa.puasituacao = 'AP' then 'Aguardando Publicação'
					  	when pa.puasituacao = 'EP' then 'Em Publicação'
					  	when pa.puasituacao = 'PU' then 'Publicado'
					  	when pa.puasituacao = 'NO' then 'Não Publicado'
					  else '-' end) as pendencia
				FROM 
				  	seguranca.publicacaoarquivo pa
					inner join seguranca.publicacaotipo pt on pt.putid = pa.putid and pt.putstatus = 'A'
				WHERE
					pa.puacpfsolicitante = '{$_SESSION['usucpf']}'
					and pa.puasituacao != 'PU'";
    	/*$arrdados = $db->carregar($sql);
    	
    	$ararquivo = explode('<br />', $arrdados[0]['puaarquivo']);
    	
    	ver(simec_htmlentities(strip_tags($ararquivo[0])), simec_htmlentities($arrdados[0]['puaarquivo'] ), simec_htmlentities(strip_tags($arrdados[0]['puaarquivo']) ),d);*/
    	$cabecalho = array( "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", "Tipo", "Arquivo", "Descrição", "Data Solicitação", "Situação");
    	$db->monta_lista_simples($sql, $cabecalho, 20, 10, 'N', '98%', 'N', false, false, false, true);
	}else{
    ?>
    <table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Lista de Solicitações Publicadas</label></td>
    	</tr>
    	</tbody>
    </table>
    <?
	$sql = "SELECT 
    				(case when pa.puasituacao = 'CA' then
					  '<center>
		    				<img src=\"/imagens/send.png\" border=0 title=\"Enviar para Publicação\" style=\"cursor:pointer;\" onclick=\"enviarPublicacao('|| pa.puaid ||');\">&nbsp;
		    				<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"alterarPublicacao('|| pa.puaid ||');\">&nbsp;
		    				<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirPublicacao('|| pa.puaid ||');\">&nbsp;
		    			</center>'
		    		else 
		    			'<center>
		    				<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"alterarPublicacao('|| pa.puaid ||');\">&nbsp;
		    				<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\"\">&nbsp;
		    			</center>'
		    		end) as acao,
					  pt.putdescricao,
					  pa.puaarquivo,
					  pa.puademanda,
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
				WHERE
					pa.puacpfsolicitante = '{$_SESSION['usucpf']}'
					and pa.puasituacao = 'PU'";
    	
    	$cabecalho = array( "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", "Tipo", "Arquivo", "Descrição", "Data Solicitação", "Situação", "Justificativa");
    	$db->monta_lista_simples($sql, $cabecalho, 20, 10, 'N', '98%', 'N', false, false, false, true);

	} ?>
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="2" align="center">
			<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="window.close();">
		</td>
	</tr>
</table>
<div id="dialog_recurso" title="Enviar Solicitação para publicar arquivos" style="display: none" >
	<div style="padding:5px;text-align:justify;" id="mostraRetornoRecurso"></div>
</div>
<script type="text/javascript">

function enviarPublicacao(puaid){
	jQuery.ajax({
		type: "POST",
		url: "listaPublicarArquivos.php",
		data: "requisicao=publicar&puaid="+puaid,
		async: false,
		success: function(msg){
			jQuery( "#dialog_recurso" ).show();
			jQuery( "#mostraRetornoRecurso" ).html(msg);
			jQuery( '#dialog_recurso' ).dialog({
					resizable: false,
					width: 600,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Salvar': function() {
							if(jQuery('#purid:checked').length == 0){
								alert('Selecione um responsável pela publicação.');
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

function solicitarPublicacao( puaid ){
	var dados = jQuery('#formpublicar').serialize();
	
	jQuery.ajax({
		type: "POST",
		url: "listaPublicarArquivos.php",
		data: "requisicao=solicitar&puaid="+puaid+"&"+dados,
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

function excluirPublicacao(puaid){
	if( confirm('Tem certeza que deseja excluir esta Publicação?') ){
		window.location.href = 'listaPublicarArquivos.php?requisicao=excluir&puaid='+puaid;
	}
}
function alterarPublicacao(puaid){
	window.location.href = 'solicitarPublicacao.php?puaid='+puaid;
}
</script>
</body>
