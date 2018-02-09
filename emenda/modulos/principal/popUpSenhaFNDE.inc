<?php 
#Desenvolvimento
//$usuario = 'MECTIAGOT'; 
/* $usuario = 'MECTHIAGOBARBOSA'; 
$senha   = '45389408';  */

//PTA: 4867

#Produção
/*$usuario = 'MECPRISCILA'; 
$senha   = '74496385';*/

#usuario siconv

/* $usuario = '73282081120';
$senha = 'Simec01#'; */

/*$usuario = 'dorides';
$senha = 'paulos04';*/

/*$usuario = 'rmedeiro';
$senha = 'R5659532';*/

if( $_SESSION['usucpf'] == '05646593638' ){
	$usuario = 'USAP_WS_SIGARP';
	$senha = '28241625';
}

if( $_REQUEST['arparcela'] ){
	$arParcela = str_replace( '\"', '"', $_REQUEST['arparcela']);
	$arParcela = urlencode($arParcela);
}

?>
<html>
<head>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
</head>
<body>
<br />
<div id="divFormulario">
<form name="formulario" id="formulario" method="post">
	<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
	<input type="hidden" name="ptrid" id="ptrid" value="<?php echo $_REQUEST['ptrid'];?>" />
	<input type="hidden" name="arparcela" id="arparcela" value="<?php echo $arParcela;?>" />
	<input type="hidden" name="exfid" id="exfid" value="<?php echo ($_REQUEST['exfidexecucao'] ? $_REQUEST['exfidexecucao'] : $_REQUEST['exfid']);?>" />
	<input type="hidden" name="orbid" id="orbid" value="<?php echo $_REQUEST['orbid'];?>" />
	<input type="hidden" name="especie_empenho" id="especie_empenho" value="<?php echo $_REQUEST['especie_empenho'];?>" />
	<input type="hidden" name="especie_pagamento" id="especie_pagamento" value="<?php echo $_REQUEST['especie_pagamento'];?>" />
	<input type="hidden" name="novoValor" id="novoValor" value="<?php echo $_REQUEST['novoValor'];?>" />
	<input type="hidden" name="boAnulacaoTotal" id="boAnulacaoTotal" value="<?php echo $_REQUEST['boAnulacaoTotal'];?>" />
	<input type="hidden" name="orbnumsolicitacao" id="orbnumsolicitacao" value="<?php echo $_REQUEST['orbnumsolicitacao'];?>" />
	<input type="hidden" name="solicitarconta" id="solicitarconta" value="<?php echo $_REQUEST['solicitarconta'];?>" />
	<input type="hidden" name="verificadoSiafi" id="verificadoSiafi" value="<?php echo $_REQUEST['verificadoSiafi'];?>" />
	<input type="hidden" name="verificadoCadin" id="verificadoCadin" value="<?php echo $_REQUEST['verificadoCadin'];?>" />
	<table id="tb_tabela" class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2"><b>
			<?if( $_REQUEST['especie_empenho'] == 'enviarproposta' ){ ?>
				Informações de Usuário do SICONV
			<?}else{ ?>
				Informações de Usuário do FNDE
			<?} ?></b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Usuário:</b></td>
			<td>
				<input type="text" class="normal" name="usuario" id="usuario" value="<?php echo $usuario;?>" size="40" maxlength="40" title="Usuário">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Senha:</b></td>
			<td>
				<input type="password" class="normal" name="senha" id="senha" value="<?php echo $senha;?>" size="40" maxlength="40" title="Senha">
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: center">
				<?php
				if( $_REQUEST['solicitacao'] == 'pagamento' ){
					if( $_REQUEST['especie_pagamento'] == 'anular' ){?>
						<input type="button" name="btnSalvar1" value="Cancelar Pagamento" id="btnSalvar1" onclick="solicitarPagamentoPTA('<?php echo $_REQUEST['ptrid'] ?>');">
					<?}else{ ?>
						<input type="button" name="btnSalvar1" value="<?php echo ($_REQUEST['especie_pagamento'] == 'consultaPagamento') ? 'Consultar Pagamento' : 'Solicitar Pagamento'; ?>" id="btnSalvar1" onclick="solicitarPagamentoPTA('<?php echo $_REQUEST['ptrid'] ?>');">
					<?} ?>
					<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="fechar('p');">
			  <?} else if( $_REQUEST['especie_empenho'] == 'cancelarConvenio' ){?>
			  		<input type="hidden" name="arpmcid" id="arpmcid" value="<?php echo $_REQUEST['pmcid'];?>" />
					<input type="button" name="btnSalvar1" value="Cancelar Convênio" id="btnSalvar1" onclick="cancelarConvenio();">
					<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="fechar('cc');" style="cursor: pointer;">
			  <?} else if( $_REQUEST['especie_consulta'] == 'consultarandamentocc' ){?>
			  		<input type="button" name="btnSalvar1" value="<?='Consultar Conta Corrente'; ?>" id="btnSalvar1" onclick="consultaContaCorrente('<?php echo $_REQUEST['ptrid'] ?>');">
					<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="fechar('<?=$_REQUEST['tipo']; ?>');" style="cursor: pointer;">
			  <?} else if( $_REQUEST['especie_consulta'] == 'solicitarcontacorrente' ){?>
			  		<input type="button" name="btnSalvar1" value="<?='Solicitar Conta Corrente'; ?>" id="btnSalvar1" onclick="solicitarContaCorrente('<?php echo $_REQUEST['ptrid'] ?>');">
					<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="fechar('<?=$_REQUEST['tipo']; ?>');" style="cursor: pointer;">
			  <?} else if( $_REQUEST['especie_empenho'] == 'enviarproposta' ){?>
			  		<input type="button" name="btnSalvar1" value="Enviar Proposta" id="btnSalvar1" onclick="enviarPropostaSiconv('<?php echo $_REQUEST['ptrid'] ?>');">
					<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="fechar('sv');" style="cursor: pointer;">
			  <?} else if( $_REQUEST['especie_empenho'] == 'original_siconv' ){?>
			  		<input type="button" name="btnSalvar1" value="Solicitar Empenho SICONV" id="btnSalvar1" onclick="solicitarEmpenhoSiconv('<?php echo $_REQUEST['ptrid'] ?>');">
			  		<input type="button" name="btnConvenio" value="Convênio SICONV" id="btnConvenio" onclick="solicitarConvenioSiconv('<?php echo $_REQUEST['ptrid'] ?>');">
					<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="fechar('e');" style="cursor: pointer;">
			  <?} else {?>
					<input type="button" name="btnSalvar1" value="<?php echo ($_REQUEST['especie_empenho'] == 'consultaEmpenho') ? 'Consultar Empenho' : 'Solicitar Empenho'; ?>" id="btnSalvar1" onclick="solicitarEmpenho('<?php echo $_REQUEST['ptrid'] ?>');">
					<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="fechar('e');" style="cursor: pointer;">
			  <?}?>
			</td>
		</tr>
		<tr><td colspan="2" id="divDebug2"></td></tr>
	</table>
</form>
</div>
</body>
</html>

<script type="text/javascript">
	function fechar(tipo){
		if( tipo == 'p' ){
			window.location.href = 'emenda.php?modulo=principal/analisepta/pagamento/solicitar&acao=A';
		} else if( tipo == 'cc' ){
			window.location.href = 'emenda.php?modulo=principal/cancelarConvenioRAP&acao=A';
		}else if( tipo == 'sv' ){
			window.location.href = 'emenda.php?modulo=principal/propostaSiconv&acao=A';
		}else {
			window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
		}
		closeMessage();
	}
    function solicitarEmpenho(ptrid){
		if(ptrid){
			var msg = '';
	
			if($('usuario').value == ''){
				msg += "O campo Usuário é obrigatório.\n";
			}
			if($('senha').value == ''){
				msg += "O campo Senha é obrigatório.\n";
			}
	
			if(msg != ''){
				alert(msg);
				return false;
			}
	
			var usuario = $('usuario').value;
			var senha = $('senha').value;
			var bkDiv = $('divFormulario').innerHTML;
			
			$('btnSalvar1').disabled = true;
			var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
			var url = "ajax_emenda.php";
			var pars = 'execucaoPTA=solicitarEmpenho&'+$('formulario').serialize();
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						asynchronous: false,
						onComplete: function(r)
						{
							if(r.responseText=="ok") {
								alert("Operação realizada com sucesso!");
								window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
								closeMessage();
							} else {
								//alert('Erro durante a execução do empenho');
								document.getElementById('divDebug2').innerHTML = r.responseText; 
								//$('#divDebug2').html(r.responseText);
							}
							//alert(r.responseText);
							//$('divDebug').update(r.responseText);
						//	$('divDebug2').update(r.responseText);
							//var erro = trim(r.responseText.substring(0,5));
							//var tamanho = r.responseText.length;
							/*if(erro == 'Erro-' ){
								var resultado = trim(r.responseText.substring(5,tamanho));
								alert(resultado);
							} else if(r.responseText == 'consultaEmpenho'){
								alert('Consulta e alteração da situação do empenho foram efetivadas com sucesso.');
								//window.location.reload();
								//return false;
							} else {
								alert(r.responseText);
								//window.location.reload();
								//return false;
							}*/
							//closeMessage();
							//window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
						}
					});
		} else {
        	//alert('Falha na operação');
        	alert('Plano de Trabalho não existe');
        	closeMessage();
        	return false;
        }
        $('btnSalvar1').disabled = false;
	}
	
    function enviarPropostaSiconv(ptrid){
		if(ptrid){
			var msg = '';
	
			if($('usuario').value == ''){
				msg += "O campo Usuário é obrigatório.\n";
			}
			if($('senha').value == ''){
				msg += "O campo Senha é obrigatório.\n";
			}
	
			if(msg != ''){
				alert(msg);
				return false;
			}
	
			var usuario = $('usuario').value;
			var senha = $('senha').value;
			var bkDiv = $('divFormulario').innerHTML;
			
			$('btnSalvar1').disabled = true;
			var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
			var url = "ajax_emenda.php";
			var pars = 'execucaoPTA=enviarPropostaSiconv&'+$('formulario').serialize();
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						asynchronous: false,
						onComplete: function(r)
						{
							if(r.responseText=="ok") {
								alert("Operação realizada com sucesso!");
								window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
								closeMessage();
							} else {
								document.getElementById('divDebug2').innerHTML = r.responseText;
							}
						}
					});
		} else {
        	alert('Plano de Trabalho não existe');
        	closeMessage();
        	return false;
        }
        $('btnSalvar1').disabled = false;
	}
	
    function solicitarConvenioSiconv(ptrid){
    	var usuario = $('usuario').value;
		var senha = $('senha').value;
		
		$('btnConvenio').disabled = true;
		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		var url = "ajax_emenda.php";
		var pars = 'execucaoPTA=exportaConvenioSiconv&'+$('formulario').serialize();
		var myAjax = new Ajax.Request(
			url, 
			{
				method: 'post', 
				parameters: pars, 
				asynchronous: false,
				onComplete: function(r)
				{
					if(r.responseText=="ok") {
						alert("Operação realizada com sucesso!");
						window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
						closeMessage();
					} else {
						document.getElementById('divDebug2').innerHTML = r.responseText;
					}
				}
			});
		$('btnConvenio').disabled = false;
    }
    function solicitarEmpenhoSiconv(ptrid){
		if(ptrid){
			var msg = '';
	
			if($('usuario').value == ''){
				msg += "O campo Usuário é obrigatório.\n";
			}
			if($('senha').value == ''){
				msg += "O campo Senha é obrigatório.\n";
			}
	
			if(msg != ''){
				alert(msg);
				return false;
			}
	
			var usuario = $('usuario').value;
			var senha = $('senha').value;
			var bkDiv = $('divFormulario').innerHTML;
			
			$('btnSalvar1').disabled = true;
			var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
			var url = "ajax_emenda.php";
			var pars = 'execucaoPTA=solicitarEmpenhoSiconv&'+$('formulario').serialize();
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						asynchronous: false,
						onComplete: function(r)
						{
							if(r.responseText=="ok") {
								alert("Operação realizada com sucesso!");
								window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
								closeMessage();
							} else {
								document.getElementById('divDebug2').innerHTML = r.responseText;
							}
						}
					});
		} else {
        	alert('Plano de Trabalho não existe');
        	closeMessage();
        	return false;
        }
        $('btnSalvar1').disabled = false;
	}
	
    function cancelarConvenio(){
		var msg = '';

		if($('usuario').value == ''){
			msg += "O campo Usuário é obrigatório.\n";
		}
		if($('senha').value == ''){
			msg += "O campo Senha é obrigatório.\n";
		}

		if(msg != ''){
			alert(msg);
			return false;
		}

		var usuario = $('usuario').value;
		var senha = $('senha').value;
		var bkDiv = $('divFormulario').innerHTML;

		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		var url = "ajax_emenda.php";
		var pars = 'execucaoPTA=solicitarEmpenho&'+$('formulario').serialize();
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					onLoading: $('divFormulario').update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
					onComplete: function(r)
					{
						//$('divDebug').update(r.responseText);
						if( Number(r.responseText) ){
							alert('Cancelamento efetivadas com sucesso');
							window.location.reload();
						} else {
							alert(r.responseText);
							window.location.reload();
						}
						closeMessage();
					}
				});
	}
    function consultaContaCorrente( ptrid ){
		var msg = '';

		if($('usuario').value == ''){
			msg += "O campo Usuário é obrigatório.\n";
		}
		if($('senha').value == ''){
			msg += "O campo Senha é obrigatório.\n";
		}

		if(msg != ''){
			alert(msg);
			return false;
		}

		var usuario = $('usuario').value;
		var senha = $('senha').value;
		var bkDiv = $('divFormulario').innerHTML;

		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		var url = "ajax_emenda.php";
		var pars = 'execucaoPTA=ConsultarAndamentoContaCorrente&'+$('formulario').serialize();
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					//onLoading: $('divFormulario').update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
					onComplete: function(r)
					{
						//$('divDebug').update(r.responseText);
						//document.getElementById('divDebug2').innerHTML = r.responseText;
						if( Number(r.responseText) ){
							alert('Cancelamento efetivadas com sucesso');
							window.location.reload();
						} else {
							document.getElementById('divDebug2').innerHTML = r.responseText;
							//window.location.reload();
						}
						//closeMessage();
					}
				});
	}
    function solicitarContaCorrente( ptrid ){
		var msg = '';

		if($('usuario').value == ''){
			msg += "O campo Usuário é obrigatório.\n";
		}
		if($('senha').value == ''){
			msg += "O campo Senha é obrigatório.\n";
		}

		if(msg != ''){
			alert(msg);
			return false;
		}

		var usuario = $('usuario').value;
		var senha = $('senha').value;
		var bkDiv = $('divFormulario').innerHTML;

		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		var url = "ajax_emenda.php";
		var pars = 'execucaoPTA=SolicitarContaCorrente&'+$('formulario').serialize();
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					//onLoading: $('divFormulario').update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
					onComplete: function(r)
					{
						//$('divDebug').update(r.responseText);
						//document.getElementById('divDebug2').innerHTML = r.responseText;
						if( Number(r.responseText) ){
							alert('Cancelamento efetivadas com sucesso');
							window.location.reload();
						} else {
							document.getElementById('divDebug2').innerHTML = r.responseText;
							//window.location.reload();
						}
						//closeMessage();
					}
				});
	}
    function solicitarPagamentoPTA(ptrid){
		if(ptrid){
			var msg = '';
	
			if($('usuario').value == ''){
				msg += "O campo Usuário é obrigatório.\n";
			}
			if($('senha').value == ''){
				msg += "O campo Senha é obrigatório.\n";
			}
	
			if(msg != ''){
				alert(msg);
				return false;
			}
	
			var usuario = $('usuario').value;
			var senha = $('senha').value;
			var bkDiv = $('divFormulario').innerHTML;

			//var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
			var url = "ajax_emenda.php";
			var pars = 'execucaoPTA=solicitarPagamento&'+$('formulario').serialize();
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						asynchronous: false,
						//onLoading: $('divFormulario').update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
						onComplete: function(r)
						{
							/*if(r.responseText=="ok") {
								alert("Operação realizada com sucesso!");
								window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
								closeMessage();
							} else {
								alert('Erro durante a execução do pagamento');
								alert(r.responseText);
							}*/
							document.getElementById('divDebug2').innerHTML = r.responseText;
								
							/*$('divDebug').update(r.responseText);
							closeMessage();
							return;
							var erro = trim(r.responseText.substring(0,5));
							var tamanho = r.responseText.length;
							if(erro == 'Erro-' ){
								var resultado = trim(r.responseText.substring(5,tamanho));
								alert(resultado);
							} else if(r.responseText == 'consultaEmpenho'){
								alert('Consulta e alteração da situação do empenho foram efetivadas com sucesso.');
							} else {
								alert(r.responseText);
							}*/
						}
					});
		} else {
        	//alert('Falha na operação');
        	alert('Plano de Trabalho não existe');
        	closeMessage();
        	return false;
        }
	}
</script>