<?php

// inicializa sistema
require_once 'config.inc';
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/funcoes.inc';

$db = new cls_banco();
$titulo = 'Envio para pagamento';

if ($_POST['requisicao'] == 'envioFormularioPagamento') {

    if ($_POST['tcpnumtransfsiafi'] == '' || $_POST['codncsiafi'] == '') {
        echo "<script>alert('Existem campos obrigatórios não preenchidos.');</script>";
        exit();
    }

    $pros = explode(',', $_POST['celulasOrcamentarias']);

    foreach ($pros as $key => $value) {

        $sql = "select provalor, crdmesliberacao from monitora.previsaoorcamentaria where proid = {$value}";
        $dadosPro = $db->carregar($sql);

        $sql = "
            insert into
                elabrev.previsaoparcela(proid, ppavlrparcela, tcpnumtransfsiafi, ppacancelarnc, ppamesenvio, codncsiafi)
            values ({$value}, '{$dadosPro[0]['provalor']}', {$_POST['tcpnumtransfsiafi']}, false, '{$dadosPro[0]['crdmesliberacao']}', '{$_POST['codncsiafi']}')
        ";
        $db->executar($sql);
        $db->commit();
    }
    echo "<script type='text/javascript'>
                alert('Enviado com sucesso.');
                window.opener.location.reload();
                self.close();
          </script>";
    exit;
}

?>
<html>
	<head>

		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
		<title><?= $titulo ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>

		<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">

		<form name="formularioEnvioPagamento" method="post" action="">
		<input type="hidden" name="requisicao" value=""/>
		<input type="hidden" name="celulasOrcamentarias" value=""/>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" valign="bottom" colspan="2">
					<center><b>Envio para Pagamento</b></center>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width:15%;text-align:right;">Número de Transferência:</td>
				<td style="text-align:left;">
					<?php echo campo_texto('tcpnumtransfsiafi', 'N', 'S', '', 20, 17, '##########', ''); ?>
					<!-- <img border="0" src="../../imagens/obrig.gif" title="Indica campo obrigatório."> --></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width:15%;text-align:right;">Número NC:</td>
				<td style="text-align:left;">
					<?php echo campo_texto('codncsiafi', 'N', 'S', '', 20, 17, '##########', ''); ?>
					<!-- <input name="codncsiafi" class="required" type="text" value=""/> -->
					<img border="0" src="../../imagens/obrig.gif" title="Indica campo obrigatório."></td>
			</tr>
			<tr>
				<td style="width:15%;text-align:right;"></td>
				<td style="text-align:left;"><input type="button" onclick="javascript:enviarFormulario()" value="Enviar"/></td>
			</tr>
		</table>
		</form>

	</body>

	<script>
		// resgata valores da window.opener para preencher dados do lote
		$(document).ready(function(){

			var celulasOrcamentarias = new Array();
			$.each( $(window.opener.document).find('[name="checkEnvio"]'), function(index,value){ 
				if( $(value).is(':checked') == true ){
					celulasOrcamentarias.push( $(value).parent().parent().attr('id').substring(3) );
				}
			});

			$('[name="celulasOrcamentarias"]').val( celulasOrcamentarias.join(',') );
		})

		function enviarFormulario(){
			
			if( $('[name="celulasOrcamentarias"]').val() == '' ){
				alert('Deve haver pelo menos 1 célula orçamentária marcada para envio.');
				self.close();
				return false;
			}

			$.each( $('.required'), function(index,value){
				if( $(value).val() == '' ){
					var label = $(value).parent().parent().find('.SubTituloDireita').html();
					var label = label.substring(0,label.length-1);
					alert(label + ' não pode ficar vazio.');
					return false;
				}
			});

			$('[name="requisicao"]').val( 'envioFormularioPagamento' );

			$('[name="formularioEnvioPagamento"]').submit();

		}
	</script>
</html>
