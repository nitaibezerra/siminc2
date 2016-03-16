<?php 
//Carregar as Funções Gerais
include "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ.  "www/includes/webservice/cpf.php";

if($_REQUEST['requisicao']=='validar_aluno'){
	$db = new cls_banco();
	extract($_POST);
	ob_clean();
	
    $sql = "SELECT			emanomealuno
            FROM		   	avalpronatec.emailalunospronatec
			WHERE			emacpf = '{$cpf}'";
    
    $rs = $db->pegaUm($sql);	
	
    if($rs){
		echo 'S';
    } else {
		echo 'N';
    }
	exit();
}

unset($_SESSION['verificaNome']);
$_SESSION['verificaNome'] = false;
?>

<script type="text/javascript" language="JavaScript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" language="JavaScript" src="../includes/prototype.js"></script>
<script type="text/javascript" language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" language="JavaScript" src="../includes/webservice/cpf.js"></script>
<script type="text/javascript" language="JavaScript">
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
			validarAluno(valor,comp.dados.no_pessoa_rf);
			divCarregado();
		} else {
			jQuery("#div_usuariopronatec").html('<b>CPF Inválido!</b>');
			jQuery("#requisicao").val('');
			jQuery('#avpcpf').val('');			
			jQuery("#avpnome").val('');
			jQuery('#avpcpf').focus();
			divCarregado();
			return false;
		}
	} else {
		alert('O campo "CPF" é obrigatório!');
		jQuery('#avpcpf').focus();
		divCarregado();
		return false;
	}	
}

function validarQuestionario(){
	if(jQuery('#avpcpf').val() == ''){
		alert('O campo "CPF" é obrigatório!');
		jQuery('#avpcpf').focus();
		return false;
	}
	if(jQuery('#txt_captcha').val() == ''){
		alert('O campo "texto da imagem" é obrigatório!');
		jQuery('#txt_captcha').focus();
		return false;
	}
	return true;		
}

function validarAluno(cpf,nome){
	jQuery.ajax({
		url:  'index.php',
		data: { requisicao: 'validar_aluno',cpf: cpf},
		async: false,
		type: 'POST',
		success: function(data){
			if(trim(data)=='S'){
				jQuery("#requisicao").val('exibir_questionario');
				jQuery("#avpnome").val(nome);
				jQuery("#div_usuariopronatec").html('');
				jQuery("#div_responder").show();
			} else {
				jQuery("#div_usuariopronatec").html('<b>Aluno não cadastrado no Pronatec!</b>');
				jQuery("#div_responder").hide();
			}
    	}
	});
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
    <div id="telacentral" style="float:left !important; width:100%; height: 800px;">
		<table bgcolor="#ffffff" cellspacing="1" cellpadding="3" align="center" width="95%" border="0">
			<tr>
				<td colspan="3" height="35"></td>
			</tr>		    
		    <tr>
		        <td align="center">Avaliação dos Cursos PRONATEC</td>
		    </tr>
		</table>
		<form id="formulario" method="post" name="formulario" action="avaliacao_pronatec.php" onsubmit="return validarQuestionario();">
			<input type="hidden" id="requisicao" name="requisicao" value=""/>
			<input type="hidden" id="avpnome" name="avpnome" value=""/>
			<table align="center" bgcolor="#ffffff" cellspacing="0" cellpadding="0" border="0" width="95%" height="225px">
				<tr>
					<td colspan="3" height="35"></td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="10%" align="right">CPF:</td>
					<td width="40%"><?php echo campo_texto('avpcpf', 'S', 'S', 'CPF', '50', '14', '###.###.###-##', '', '', '', '','id="avpcpf"','','',"this.value=mascaraglobal('###.###.###-##',this.value); verificarCPFReceita(this.value);"); ?></td>
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