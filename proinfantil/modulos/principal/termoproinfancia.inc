<?php
include_once "_funcoes_proinfancia.php";

function formArquivaProInfancia(){
	ob_clean();
?>
<form id="formArquiva" name="formArquiva" method="POST">
	<input type="hidden" id="req2" name="req" value=""/>
	<b>Justificativa:</b>
	<?php
	echo campo_textarea(
		"cmddsc",		// nome do campo
		"S",			// obrigatorio
		"S",			// habilitado
		"Comentário",	// label
		61,				// colunas
		15,				// linhas
		6000			// quantidade maximo de caracteres
	)
	?>
</form>
<?
}

if($_REQUEST['req']) {
	$_REQUEST['req']($_REQUEST);
	exit;
}

$adesao = verificarAdesaoProInfancia($_SESSION['proinfantil']['pinid']);
$habilitado = verificaLiberacao();

if($habilitado == 'S'){
	$disabled = "";	
} else {
	$disabled = "disabled";
}

if(trim($adesao) == '' || trim($adesao) == 'N' || trim($adesao) == 'A' ) { ?>
<html>
    <head>
        <style type="text/css">
            @page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
            .divT_1{ margin-left: 2.5598in; text-align: center; }
            .P_1{ font-family: Arial; font-size: 10pt; margin-left: 0.2in; margin-right: 0in; text-align: justify ! important; clear: both; }
            .P_2{ font-family: Arial; font-size: 8pt; float: left; margin-left: 2in; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
            .P_3{ font-family: Arial; font-size: 10pt; margin-left: 1in; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
            .P_4{ font-family: Arial; font-size: 10pt; line-width:50px;float: left; text-align: left; width="100px"}
            .Data{ font-family: Arial; font-size: 10pt; margin-left: 1in; margin-right: 0in; text-align: right; }
            .T{ font-family: Arial; font-size: 10pt; font-weight: bold; text-align: center; }
            .T_1{ font-family: Arial; font-size: 10pt; font-weight: bold; margin-left: 0.2in; text-align: center; }
            .T_11{ font-family: Arial; font-size: 10pt; font-weight: bold; margin-left: 0.2in; text-align: center; }
            .N{ font-weight: bold; }
            .I{ font-style: italic; }
            .S{ text-decoration: underline; }
    </style>
	</head>
<body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>

$(document).ready(function(){

	$('.btnArquivar').click(function(){
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=formArquivaProInfancia",
	   		async: false,
	   		success: function(msg){
	   			$( "#dialog" ).html(msg);
				$( "#dialog" ).dialog({
					resizable: true,
					height:400,
					width:600,
					modal: true,
					buttons: {
						"Arquivar": function() {
							arquivar();
							$( this ).dialog( "close" );
						},
						"Fechar": function() {
							$( this ).dialog( "close" );
						}

					}
				});
	   		}
		});
	});
});

function validaForm( resposta ){
	var form = document.getElementById('form');
	document.getElementById('resposta').value = resposta;
	document.getElementById('req').value = 'respondeTermoProInfancia';
	form.submit();
}

function cancela(){
	var form = document.getElementById('form');
	document.getElementById('req').value = 'cancelaTermoProInfancia';
	form.submit();
}

function continuar(){
	var form = document.getElementById('form');
	document.getElementById('continua').value = 'S';
	form.submit();
}

function arquivar(){
	var form = document.getElementById('formArquiva');
	document.getElementById('req2').value = 'arquivarProInfancia';
	form.submit();
}
</script>

<form id="form" name="form" method="POST">
<input type="hidden" id="resposta" name="resposta" value=""/>
<input type="hidden" id="continua" name="continua" value=""/>
<input type="hidden" id="req" name="req" value=""/>
<input type="hidden" id="tapid" name="tapid" value="<?php echo $dados['tapid']; ?>"/>
<table bgcolor="#f5f5f5" align="center" class="tabela">
	<tr>
		<td width="90%" align="center">
			<br />
			<div style="overflow: auto; height:400px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="center" >
				<div style="width: 95%; magin-top: 10px;">
					<br />
					<?php if(trim($adesao) == 'A' ){ ?>
					<div>
				            <p class="T_1">OBRA ARQUIVADA.</p>
				            <p class="P_1">
				            <?php 
				            $sql = "SELECT 		prapareceraprovacao, praid
						            FROM 		proinfantil.proinfanciaanalise
						            WHERE		pinid = {$pinid} AND praarquivada IS TRUE AND prastatus = 'A'
				            		ORDER BY 	praid DESC
				            		LIMIT 1";
				            
				            $justificativa = $db->pegaUm($sql);
				            echo $justificativa;
				            ?>
				            </p>
					</div>
					<?php }else{?>
					<div>
				            <p class="T_1">DECLARAÇÃO PARA RECEBIMENTO DE RECURSOS DE CUSTEIO PARA EDUCAÇÃO INFANTIL.</p>
				            <p class="P_1">
				                Declaro, como representante do Poder Executivo do município de <?=$_SESSION['proinfantil']['descricao'] ?>/<?=$_SESSION['proinfantil']['estuf'] ?>, que as informações prestadas no Simec – Módulo E. I. Manutenção são fidedignas, responsabilizo-me pela exatidão delas e afirmo que:
				            </p>
				            <p class="P_1">
				                <br />
				                1 – essas informações referem-se somente a crianças que estão sendo atendidas em unidade de educação infantil construída com recursos do Governo Federal (Proinfância); e
				            </p>
				            <p class="P_1">
				                <br />
				                2 – o estabelecimento informado está em pleno funcionamento; e
				            </p>
				            <p class="P_1">
				                <br />
				                3 – as crianças informadas não são computadas para efeito de recebimento dos recursos do Fundeb; e
				            </p>
				            <p class="P_1">
				                <br />
				                4 – o estabelecimento não está cadastrado no Educacenso ou, ainda que cadastrado, não teve essas crianças computadas para efeitos de recebimento de recursos do Fundeb; e
				            </p>
				            <p class="P_1">
				                <br />
				                5 – todas as crianças serão cadastradas no próximo Educacenso/Inep.
				            </p>
					</div>
					<?php }?>
				</div>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center">
			<div style="float: center; width: 70%; ">
            	<?php 
            	if( empty($_SESSION['proinfantil']['popup']) ){
            		if(trim($adesao) != 'S' && trim($adesao) != 'A' ){ ?>
					<input id="aceito" type="button" name="aceito" value="Aceito" onclick="validaForm('S');" <?php echo $disabled; ?>/>
                <?php } ?>
                <?php if(trim($adesao) == 'S' ){ ?>
                    <input id="cancelar" type="button" name="cancelar" value="Cancelar Adesão" onclick="cancela();" <?php echo $disabled; ?>/>
                    <input id="continua" type="button" name="continua" value="Continua" onclick="continuar();" <?php echo $disabled; ?>/>
                <?php } ?>
                <?php if(trim($adesao) != 'S' && trim($adesao) != 'A'){ ?>
				   <input id="naceito" 		type="button" name="naceito" 	value="Não Aceito" 	onclick="validaForm('N');" 	<?php echo $disabled; ?>/>
                <?php } ?>
             	<?php if(trim($adesao) != 'A' ){ ?>
			   		<input class="btnArquivar" 	type="button" value="Arquivar" 	<?php echo $disabled; ?>/>
			   	<?php } ?>
                	<input id="sair" type="button" name="sair" value="Sair" onclick="window.location.href = 'proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A'" />
				<? } else { ?>
					<input id="sair" type="button" name="sair" value="Sair" onclick="window.close();" />
				<?} ?>
			</div>
		</td>
	</tr>
</table>
</form>
<?php } ?>
<div id="dialog" title="Arquivar" style="display:none;">
</div>
</body>
</html>

