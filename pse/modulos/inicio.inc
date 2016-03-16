<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>

var countModal = 1;

function montaShowModal() {
	var campoTextArea = ''+
		'<p align=\'justify\'>Informamos que os cadastros Secretaria, Escola e Unidade Local Integrada estão abertos para preenchimento com os dados do PSE relativos ao ano/exercício de 2010.'+
		'<br /><br />Ao acessar o SIMEC/PSE, verifique na tela do monitor, na parte de cima à direita, que o sistema disponibiliza o ano/exercício do programa. Para acessar outros anos/exercícios do programa, basta trocar o ano antes de visualizar as informações.'+
		'<br /><br />Atenção! As Secretarias e Escolas dos Municípios da Portaria nº 2.931 de 4 de dezembro de 2008, terão até o dia 31 de dezembro de 2010, para registrar as informações relativas ao ano/exercício 2009.'+
		'<br /><br />Contamos com seu compromisso!'+
		'<br /><br />Gratos,'+
		'<br /><br />Equipe PSE.';
	var alertaDisplay = '<div class="titulo_box" >Prezado usuário do SIMEC/PSE, <br/ >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(520,300);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
	<?php
	if($db->testa_superuser()){?>
		window.location.href='pse.php?modulo=principal/listaMunicipios&acao=A';
<?php	}else{
		if($pflcod == SUPER_USUARIO || $pflcod == MEC || $pflcod == CONSULTA || $pflcod == PARCEIRO){?>
			window.location.href='pse.php?modulo=principal/listaMunicipios&acao=A';
<?php	} elseif ($pflcod == SECRETARIA_ESTADUAL){?>
			window.location.href='pse.php?modulo=principal/listaMunicipiosTermo&acao=A';
<?php	} elseif ($pflcod == SECRETARIA_MUNICIPAL){?>
			window.location.href='pse.php?modulo=principal/listaMunicipiosTermo&acao=A';
<?php	} else {?>
			window.location.href='pse.php?modulo=principal/ListarEscolas&acao=A';
<?php	}
	}
	?>
	
}

function redirecionaPDF(tipo){
	window.location.href = 'monitora.php?modulo=inicio&acao=C&tipoUnidade='+tipo+'&boImprimeManualPDF=1';
}

function verificaSeMostraShowModal(){
	var mostraShowModal = '<?php echo $mostraShowModal; ?>';
	if(mostraShowModal){
		montaShowModal();	
	}
}

</script>
<!--<body onload="montaShowModal()">-->
<body>
<?
 
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

$usucpf = $_SESSION['usucpf'];
$pflcod = pegaPerfil($usucpf);

$_SESSION['mensagem'] = true; //comentar pra voltar a mensagem!

unset($_SESSION['pse']);

if( $_SESSION['mensagem'] == true ){
	
	$pfls = arrayPerfil();
	
	if ( in_array(SUPER_USUARIO, $pfls) || in_array(EDUCADOR_ESCOLA, $pfls) ||
		 in_array(MEC, $pfls) || in_array(SECRETARIA_ESTADUAL, $pfls) || 
		 in_array(SECRETARIA_MUNICIPAL, $pfls) || in_array(CONSULTA, $pfls)  ) {

		 echo "<script>window.location.href='pse.php?modulo=principal/listaComp2&acao=A';</script>";
		die();
				
	}
	
	/*
	if($_SESSION["sisbaselogin"] == 'simec_desenvolvimento'){
		echo "<script>window.location.href='pse.php?modulo=principal/listaComp2&acao=A';</script>";
		//echo "<script>window.location.href='pse.php?modulo=principal/listaAbas&acao=A';</script>";
	}else{
		
		
		
		echo "<script>window.location.href='pse.php?modulo=principal/ListarEscolasMonitoramento&acao=A';</script>";
	}
	*/
	/*
	if($db->testa_superuser()){
		//echo "<script>window.location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';</script>";
		echo "<script>window.location.href='pse.php?modulo=principal/ListarEscolasMonitoramento&acao=A';</script>";
	}else{
		if($pflcod == MEC){
			//echo "<script>window.location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';</script>";
			echo "<script>window.location.href='pse.php?modulo=principal/ListarEscolasMonitoramento&acao=A';</script>";
		} elseif ($pflcod == SECRETARIA_ESTADUAL){
			//echo "<script>window.location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';</script>";
			echo "<script>window.location.href='pse.php?modulo=principal/ListarEscolasMonitoramento&acao=A';</script>";
		} elseif ($pflcod == SECRETARIA_MUNICIPAL){
			echo "<script>window.location.href='pse.php?modulo=principal/ListarEscolasMonitoramento&acao=A';</script>";
		} else {
			echo "<script>window.location.href='pse.php?modulo=principal/ListarEscolasMonitoramento&acao=A';</script>";	
		}
	}
	*/
}

$_SESSION['mensagem'] = true;
?>
<br>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" >
			<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">PSE - PROGRAMA SAÚDE NA ESCOLA</label></td></tr>
			<tr><td bgcolor="#e9e9e9" align="center">&nbsp;<br>
						<table width="50%">
							<tr>
								<td>
									<b>Bem vindo ao PSE - PROGRAMA SAÚDE NA ESCOLA!</b>
									<!-- 
									<b>Atenção  Senhore(a)s gestore(a)s!</b>
									<br><br>O Sistema de Monitoramento do PSE está sendo atualizado de acordo com o novo modelo de gestão  intersetorial do Programa que considera a necessidade de pactuações das metas das ações de prevenção, promoção e atenção à saúde a serem implantadas/implementadas pelos Municípios, de modo a possibilitar as ações de saúde nas escolas. 
									<br><br>Assim:
									<br>Fica definido na forma do <b><u>Anexo II</u> da PORTARIA INTERMINISTERIAL Nº 1.910, DE 8 DE AGOSTO DE 2011</b>, os Municípios aptos a assinarem o Termo de Compromisso Municipal do Programa Saúde na Escola (PSE), exercício 2011/2012, que deverá ser realizada a contar da data de publicação desta Portaria até o <b>dia 30 de setembro de 2011</b>.
									<br><br><b>Para municípios que aderiram ao PSE em portarias anteriores</b> <br> Fica definido no <b><u>Anexo III</u> da PORTARIA INTERMINISTERIAL Nº 1.910, DE 8 DE AGOSTO DE 2011</b>, os Municípios aptos a assinarem o Termo de Compromisso Municipal do PSE, exercício 2011/2012, que deverá ser realizada no período do dia 30 de setembro a 30 de novembro de 2011.
									<br><br>
									 -->
								</td>
							</tr>
						</table>
				</td>
			</tr>
		</table>
</body>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />