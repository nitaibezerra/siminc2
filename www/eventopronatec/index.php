<?php 
//Carregar as Funções Gerais
include "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ.  "www/includes/webservice/cpf.php";

unset($_SESSION['verificaNome']);
$_SESSION['verificaNome'] = false;
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script language="javascript" type="text/javascript">
jQuery.noConflict();

function verificarCPFReceita(cpf){
	divCarregando();
	if(cpf){		
		var valor = cpf.replace(".", "");
		valor = valor.replace(".", "");
		valor = valor.replace("-", "");
		
		if(validar_cpf(valor)){		
			var comp = new dCPF();
			comp.buscarDados(valor);
			jQuery("#requisicao").val('exibir_questionario');
			jQuery("#evpnome").val(comp.dados.no_pessoa_rf);
			jQuery("#div_usuariopronatec").html('');
			jQuery("#div_responder").show();
			divCarregado();
		} else {
			jQuery("#div_usuariopronatec").html('<b>CPF Inválido!</b>');
			jQuery("#requisicao").val('');
			jQuery('#evpcpf').val('');			
			jQuery("#evpnome").val('');
			jQuery('#evpcpf').focus();
			divCarregado();
			return false;
		}
	} else {
		alert('O campo "CPF" é obrigatório!');
		jQuery('#evpcpf').focus();
		divCarregado();
		return false;
	}	
}

function validarQuestionario(){
	if(jQuery('#evpcpf').val() == ''){
		alert('O campo "CPF" é obrigatório!');
		jQuery('#evpcpf').focus();
		return false;
	}
	if(jQuery('#txt_captcha').val() == ''){
		alert('O campo "texto da imagem" é obrigatório!');
		jQuery('#txt_captcha').focus();
		return false;
	}
	return true;		
}
</script>
<link rel="stylesheet" href="http://spp.mec.gov.br/public/js/libs/jquery-ui/css/custom-theme/jquery-ui-1.8.20.custom.css" media="screen" type="text/css">
<link rel="stylesheet" href="http://pronatec.mec.gov.br/templates/pronatec/barra_governo3/css/barra_do_governo.css" media="all" type="text/css" />
<link rel="stylesheet" href="http://pronatec.mec.gov.br/templates/pronatec/css/template.css" type="text/css"/>
<link rel="alternate stylesheet" href="http://pronatec.mec.gov.br/templates/pronatec/css/altocontraste.css" title="altoContraste" type="text/css" />
<link rel="stylesheet" href="css/style.css" type="text/css"></link>
<link rel="stylesheet" href="css/dtree.css" type="text/css"></link>

<div id="barra-brasil-v3" class="barraGovernoPreto">
	<div id="barra-brasil-v3-marca">Brasil &ndash; Governo Federal &ndash; Minist&eacute;rio da Educa&ccedil;&atilde;o</div>
</div>
<div id="main"> 
    <div id="logomarca">
   		<a href="http://pronatec.mec.gov.br/index.php" title="Pronatec Portal" alt="Pronatec - Programa Nacional de Acesso ao Ensino Técnico e Emprego" tabindex="1" accesskey="1">
    	<img src="http://pronatec.mec.gov.br/templates/pronatec/images/logo.png" alt="Pronatec - Programa Nacional de Acesso ao Ensino Técnico e Emprego" border="0"/></a>
    </div>
    <div id="telacentral" style="float:left !important; width:100%; height: 700px;">
		<table bgcolor="#ffffff" cellspacing="1" cellpadding="3" align="center" width="95%" border="0">
		    <tr>
		    	<td height="25"></td>
		    </tr>		    
		    <tr>
		        <td align="center">Evento Pronatec - INSCRIÇÃO</td>
		    </tr>
		    <tr>
		    	<td height="25"></td>
		    </tr>
		    <tr>
		    	<td>
					<p style="text-align: justify; font-size: 15px; margin-left: 25px;margin-right: 25px;">
					O Evento Pronatec, a ser realizado nos dias 25, 26 e 27 de novembro, tem por objetivo avaliar 
					a execução 2011-2013 e consolidar a pactuação de vagas da Bolsa-Formação 2014.
					</p>
					<p style="text-align: justify; font-size: 15px; margin-left: 25px;margin-right: 25px;">
					Faça a sua inscrição no evento, identificando as atividades das quais participará.
					</p>
					<p style="text-align: justify; font-size: 15px; margin-left: 25px;margin-right: 25px;">
					Informações importantes: 
					</p>
					<p style="text-align: justify; font-size: 15px; margin-left: 25px;margin-right: 25px;">
					•	Preencha suas informações com atenção e salve suas respostas antes de comutar entre uma página e outra – elas não são salvas automaticamente. <br>
					•	Revise as informações ao final do preenchimento do formulário, antes de enviar. <br>
					•	Caso seja necessário atualizar alguma informação em momento posterior, basta inserir o mesmo CPF novamente. <br>
					•	No caso de dúvidas, favor entrar em contato com Francisca – e-mail <?php echo $_SESSION['email_sistema']; ?> ou do telefone (61)2022-8620.
					</p>
					<p style="text-align: justify; font-size: 15px; margin-left: 25px;margin-right: 15px;">
					Obrigada!
					</p>
		    	</td>
		    </tr>
		</table>
		<form id="formulario" method="post" name="formulario" action="evento_pronatec.php" onsubmit="return validarQuestionario();">
			<input type="hidden" id="requisicao" name="requisicao" value=""/>
			<input type="hidden" id="evpnome" name="evpnome" value=""/>
			<table align="center" bgcolor="#ffffff" cellspacing="0" cellpadding="0" border="0" width="95%" height="225px">
				<tr>
					<td colspan="3" height="35"></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="10%" align="right">CPF:</td>
					<td width="40%"><?php echo campo_texto('evpcpf', 'S', 'S', 'CPF', '50', '14', '###.###.###-##', '', '', '', '','id="evpcpf"','','',"this.value=mascaraglobal('###.###.###-##',this.value); verificarCPFReceita(this.value);"); ?></td>
					<td width="50%" id="div_usuariopronatec" align="left"></td>
				</tr>	
				<tr>
					<td align="center" colspan="3">Para prosseguir informe o texto abaixo:</td>
				</tr>
				<tr>
					<td align="center" colspan="3">
						<img src="captcha.php" width="113" height="49">
						&nbsp;&nbsp;&nbsp;
						<input type="text" name="txt_captcha" id="txt_captcha" maxlength="4" size="20"/>
					</td>
				</tr>
				<tr>
					<td id="div_responder" align="center" colspan="3" style="display: none;"><input type="submit" value="Responder Questionário" id="btnResponder"/></td>
				</tr>	
			</table>		
		</form>
		<?php if($_REQUEST['erro']){ ?>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" width="75%">
		    <tr>
		        <td align="center"><b>CAPTCHA Inválido! Tente Novamente!</b></td>
		    </tr>
		</table>
		<?php } ?>
	</div>
	<div id="rodape">© 2012 Ministério da Educação. Todos os direitos reservados.</div>
</div>