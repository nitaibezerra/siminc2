<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";
		
if (isset ( $_POST ["theme_simec"] )) {
	$theme = $_POST ["theme_simec"];
	setcookie ( "theme_simec", $_POST ["theme_simec"], time () + 60 * 60 * 24 * 30, "/" );
} else {
	if (isset ( $_COOKIE ["theme_simec"] )) {
		$theme = $_COOKIE ["theme_simec"];
	}
}

/**
 * Sistema Integrado de Monitoramento do Ministério da Educação
 * Setor responsvel: SPO/MEC
 * Desenvolvedor: Desenvolvedores Simec
 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Solicitação de cadastro de contas de usuário.
 * Data de criação:
 * Última modificação: 30/08/2006
 */

// carrega as bibliotecas internas do sistema
require_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . 'www/validacaodocumento/gerarchave.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
$mensagens = array();

if (! $theme) {
	$theme = $_SESSION ['theme_temp'];
}

// Carregar informações do documento
if ($_REQUEST['chave']) {
	$sql = sprintf("select vld.vldid, vld.vldchave, sis.sisabrev, usu.usunome, TO_CHAR(dataultimaatualizacao, 'DD/MM/YYYY') as dataultimaatualizacao
					  from public.validacaodocumento vld
			    inner join seguranca.sistema sis on vld.sisid = sis.sisid
			    inner join seguranca.usuario usu on usu.usucpf  = vld.usucpf
	 		    where vldchave = '%s'", stripslashes($_REQUEST['chave']));

	$documento = $db->pegaLinha($sql);
	
	if (!$documento)
		$_SESSION ['MSG_AVISO'][] = 'Documento não validado ou a chave digitada esta incorreta';
}

// Download do arquivo
if ($_REQUEST['vldid']) {
	baixarDocumentoValidado($_REQUEST['vldid']);
}

?>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<meta name="viewport" content="width=device-width, initial-scale=1.0" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />

<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

<!-- Styles Boostrap -->
<link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
<link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
<link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">

<!-- Custom Style -->
<link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
<link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">
<link href="estrutura/temas/default/css/estilon.css" rel="stylesheet">

<!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
<!--[if lt IE 9]>
            <script src="estrutura/js/html5shiv.js"></script>
        <![endif]-->
<!--[if IE]>
            <link href="estrutura/temas/default/css/styleie.css" rel="stylesheet">
        <![endif]-->

<!-- Boostrap Scripts -->
<script src="library/jquery/jquery-1.10.2.js"></script>
<script src="library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
<script src="library/chosen-1.0.0/chosen.jquery.min.js"></script>
<script src="library/bootstrap-switch/js/bootstrap-switch.min.js"></script>

<!-- Custom Scripts -->
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript">
$(function(){
    $('.chosen-select').chosen();
});

function ImprimeStatus(texto){
    document.formul.numCaracteres.value = texto
}

function validar_formulario()
{
	var validacao = true;
    var mensagem  = '';

    if (document.formulario.chave.value == '' || document.formulario.chave.value.length < 19) {
    	mensagem += '\nChave informada não é válida.';
        validacao = false;
	}

    if ( !validacao ) {
    	alert( mensagem );
	}else{
    	document.formulario.submit();
	}
}
</script>

<style type="text/css">
a.chosen-single { height: 30px !important; padding-top: 2px !important; }
.switch-left, .switch-right { padding: 3px 10px !important }
td b, td p, td li { margin: 10px; }
table tr td { padding: 3px; }
#barra-brasil .brasil-flag { height: 100% !important; }
</style>

</head>

<body>
	<div id="barra-brasil" style="background: #7F7F7F; height: 20px; padding: 0 0 0 10px; display: block;">
		<ul id="menu-barra-temp" style="list-style: none;">
			<li style="display: inline; float: left; padding-right: 10px; margin-right: 10px; border-right: 1px solid #EDEDED">
				<a href="http://brasil.gov.br" style="font-family: sans, sans-serif; text-decoration: none; color: white;">Portal do Governo Brasileiro</a>
			</li>
			<li>
				<a style="font-family: sans, sans-serif; text-decoration: none; color: white;" href="http://epwg.governoeletronico.gov.br/barra/atualize.html">Atualize sua Barra de Governo</a>
			</li>
		</ul>
	</div>

	<br>

	<div class="row">
		<div class="col-md-12">
			<img src="estrutura/temas/default/img/logo-simec.png">
		</div>
		<!-- / .col-md-12 -->
	</div>
        <?php $mensagens = implode ( '<br/>', ( array ) $_SESSION ['MSG_AVISO'] ); $_SESSION ['MSG_AVISO'] = null; ?>
        <div class="row">
		<div class="col-md-12">
			<form method="post" name="formulario" id="formulario"
				onsubmit="return false;">
				<table width="100%" cellpadding="0" cellspacing="0" id="main">
					<tr>
						<td colspan="2" width="100%" valign="top">
							<!-- Lista de Módulos-->
							<table width="98%" border="0" cellpadding="0" cellspacing="0"
								class="tabela_modulos">
								<tr>
									<td class="td_bg">&nbsp;Validação de documentos - <small>Preencha os Dados Abaixo e clique no botão: "Confirmar"</small></td>
								</tr>
								<tr>
									<td align="center">
				                      	<?php if( strlen($mensagens) > 5 ) : ?>
				                    	<div class="error_msg"><? echo (($mensagens)?$mensagens:""); ?></div>
				                    	<?php endif; ?>
                    				</td>
								</tr>
								<tr>
									<td valign="middle" class="td_table_inicio">
										<table width="95%">
											<?php if (!$documento) : ?>
											<tr>
												<td width="110" style="font-weight: bold;" align='right'>Chave:</td>
												<td><input id="chave" type="text" name="chave"
													value=<? print '"'.$chave.'"'; ?> class="login_input"
													onkeyup="this.value=this.value.toUpperCase();"
													maxlength="19" /> <img border='0'
													src='../imagens/obrig.gif'
													title='Indica campo obrigatório.'></td>
											</tr>
											<tr>
												<td>&nbsp;</td>
												<td>
													<a class="botao2" href="javascript:validar_formulario()">Confirmar</a>
													<a class="botao1" href="./login.php">Voltar</a>
												</td>
											</tr>
											<tr>
												<td colspan="2">&nbsp;</td>
											</tr>
											<?php else: ?>
											<input id="vldid" type="hidden" name="vldid" value="<?php echo $documento['vldid']; ?>">
											<input id="chave" type="hidden" name="chave" value="<?php echo $documento['vldchave']; ?>">
											<tr>
												<td colspan="2" style="font-weight: bold;">
													&#10003; Documento válido segue informações sobre o mesmo:
												</td>
											</tr>
											<tr>
												<td colspan="2">&nbsp;</td>
											</tr>
											<tr>
												<td width="80"><b style="font-weight: bold;">Usuário :</b></td>
												<td><?php echo $documento['usunome']; ?></td>
											</tr>
											<tr>
												<td><b style="font-weight: bold;">Modulo :</b></td>
												<td><?php echo $documento['sisabrev']; ?></td>
											</tr>
											<tr>
												<td><b style="font-weight: bold;">Data :</b></td>
												<td><?php echo $documento['dataultimaatualizacao']; ?></td>
											</tr>
											<tr>
												<td>&nbsp;</td>
												<td>
													<a class="botao2" href="javascript:validar_formulario()">Download</a>
													<a class="botao1" href="./valida_documento.php">Voltar</a>
												</td>
											</tr>
											<tr>
												<td colspan="2">&nbsp;</td>
											</tr>
											<?php endif; ?>
										</table>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr>
						<td colspan="2" class="rodape"> Data do Sistema: <? echo date("d/m/Y - H:i:s") ?></td>
					</tr>
				</table>
			</form>

		</div>
	</div>

	<!-- Fim barra governo -->
	<script src="//static00.mec.gov.br/barragoverno/barra.js"
		type="text/javascript"></script>

</body>
</html>

