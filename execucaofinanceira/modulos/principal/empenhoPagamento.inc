<?php
// ------------- INCLUDES ------------- //
//include APPRAIZ . "execucaofinanceira/classes/processos.class.inc";
//include APPRAIZ . "execucaofinanceira/classes/empenhos.class.inc";

// ------------- DECLARAÇÃO DE VARIAVEIS ------------- //
$empid	 	= $_REQUEST['empid'] 	? $_REQUEST['empid'] 	: 0;
if( $_REQUEST['proid'] ){
	$processo 	= $_REQUEST['proid'];
	$_SESSION['execucaofinanceira']['proid'] = $_REQUEST['proid']; 
} else {
	$processo = $_SESSION['execucaofinanceira']['proid'];
}
if( empty($processo) ) die("<script> alert('Erro ao tentar identificar o processo!'); window.location.href = 'execucaofinanceira.php?modulo=principal/listaDeProcessos&acao=A';</script>");

$obProcesso = new Processo();
$obProcesso->carregarPorId($processo);
$obEmpenho = new Empenho();

// ------------- REQUISIÇÕES AJAX E FUNÇÕES ------------- //
if($_REQUEST['requisicao'] == 'carregarListaEmpenhosRealizados') {

	$obProcesso->controleDeTela('Empenho');
	$obEmpenho->carregarListaEmpenhosRealizados($obProcesso);
	exit();
}

if($_REQUEST['requisicao'] == 'listaPassivelEmpenho') {
	$obProcesso->controleDeTela('Empenho');
	$obEmpenho->listaDadosPassiveisDeEmpenho($obProcesso);
	exit();
}

if( $_POST['requisicao'] == 'mostraInformacoesEmpenho' ){
	header('content-type: text/html; charset=ISO-8859-1');
	$obProcesso->controleDeTela('Empenho');
	$obEmpenho->mostraInformacoesEmpenho( $_POST['empid'], $obProcesso );
	exit();
}

if( $_POST['requisicao'] == 'solicitarEmpenho' ){	
	$obProcesso->controleDeTela('Empenho');
	$obEmpenho->solicitarEmpenhos( $_POST, $obProcesso );
	exit();
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, $parametros);

$valorEmpenhado = $db->pegaUm("SELECT sum(empvalorempenhado) FROM execucaofinanceira.empenho where proid = $processo");
$valorProjeto = $db->pegaUm("SELECT sum(pc.prcvalortotal) FROM execucaofinanceira.processocomposicao pc where proid = $processo");

?>
<?php monta_titulo( $titulo_modulo, '' ); ?>
<? $obProcesso->montaCabecalhoProcesso(); ?>
<style>	
	.popup_alerta
	{
		width:<?php echo $largura ?>;
		height:<?php echo $altura ?>;
		position:absolute;
		z-index:0;
		top:50%;
		left:50%;
		margin-top:-<?php echo $altura/2 ?>;
		margin-left:-<?php echo $largura/2 ?>;
		border:solid 2px black;
		background-color:#FFFFFF;
		display:none
	}
</style>
<style>
	label { cursor: pointer; }
</style>
<script language="javascript" type="text/javascript" src="js/modal.popup.js"></script>
<link rel='stylesheet' type='text/css' href='../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css'/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<div class="demo">
	<div id="dialog-confirm" title="Resposta da solicitação de Empenho:"></div>
</div>
<form name="formempenho" id="formempenho" method="post" action="" >
	<input type="hidden" id="empid" name="empid" value="<?php echo $empid; ?>">
	<input type="hidden" id="processo" name="processo" value="<?php echo $processo; ?>">
	<input type="hidden" id="sbdidH" name="sbdidH" value="">
	<input type="hidden" id="valorempenhado" name="valorempenhado" value="<?=$valorEmpenhado ?>">
	<input type="hidden" id="valorprojeto" name="valorprojeto" value="<?=$valorProjeto ?>">
	<table id="total" align="center" border="0" width="95%" class="tabela" cellpadding="1" cellspacing="1">
		<tr>
			<td><br>
				<table id="tb_empenho" align="center" border="0" style="width: 100%" class="tabela" cellpadding="3" cellspacing="2">
					<tr>
						<th colspan="2">Empenhos</th>
					</tr>
					<tr>
						<td style="text-align: center;"><input type="button" name="btnNovoEmpenho" id="btnNovoEmpenho" value="Solicitar Novo Empenho" <?=verificaPermissaoPerfil(); ?>></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td style="height: 5px"></td>
		</tr>
		<tr>
			<td>
				<table id="tb_pagamento" align="center" border="0" style="width: 100%" class="tabela" cellpadding="3" cellspacing="2">
					<tr>
						<th colspan="2">Pagamento</th>
					</tr>
					<tr>
						<td colspan="2">
							<div id="divListaEmpenho">Carregando...</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
</form>
<?php
$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "270px";
$altura 	= "150px";
$id 		= "div_auth";
?>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php
				$wsusuario = $usuario; 
				echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'", '', WEB_SERVICE_SIOP_USUARIO) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?php echo WEB_SERVICE_SIOP_SENHA; ?>" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" name="btn_enviar" id="btn_enviar" value="ok" />
				<input type="button" name="btn_cancelar" id="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
<script type="text/javascript">

    $(document).ready(function() {
    	var processo = $("#processo").val();
		carregarListaEmpenhosRealizados(processo);
        
        $('#btnNovoEmpenho').click(function(){
        	window.open("execucaofinanceira.php?modulo=principal/executarEmpenho&acao=A","Executar Empenho",'scrollbars=yes,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
        });
        		
        $('#btnPagamento').click(function(){
        	window.open("execucaofinanceira.php?modulo=principal/executarPagamento&acao=A","Executar Empenho",'scrollbars=yes,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
        });
        var valorProjeto = $('#valorprojeto').val();
        var valorEmpenhado = $('#valorempenhado').val();
        
        if( (valorEmpenhado ==  valorProjeto) && valorProjeto != ''){
        	$('#btnNovoEmpenho').attr('disabled', true);
        }
	});
	
	function carregarDadosEmpenho(idImg, empid){
		var img 	 = $('#'+idImg );
		var tr_nome = 'listaDadosEmpenho_'+ empid;
		var td_nome  = 'trV_'+ empid;
		
		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ''){
			$('#'+td_nome).html('Carregando...');
			img.attr('src', '../imagens/menos.gif');
			mostraInformacoesEmpenho(empid, td_nome);
		}
		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
			$('#'+tr_nome).css('display', '');
			img.attr('src', '../imagens/menos.gif');
		} else {
			$('#'+tr_nome).css('display', 'none');
			img.attr('src', '/imagens/mais.gif');
		}
	}
	
	function mostraInformacoesEmpenho(empid, td_nome){
		$.ajax({
	   		type: "POST",
	   		url: "execucaofinanceira.php?modulo=principal/empenhoPagamento&acao=A",
	   		data: "requisicao=mostraInformacoesEmpenho&empid="+empid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   		}
		});
	}
	
	function carregarListaEmpenhosRealizados(processo) {
		$.ajax({
	   		type: "POST",
	   		url: "execucaofinanceira.php?modulo=principal/empenhoPagamento&acao=A",
	   		data: "requisicao=carregarListaEmpenhosRealizados&processo="+processo,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('divListaEmpenho').innerHTML = msg;
	   		}
		});
	}
 </script>
