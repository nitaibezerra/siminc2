<script language="JavaScript" src="../includes/funcoes.js"></script>		 
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php

#Inclui funções do termo de execução descentralizado
require_once '_funcoes_termoCooperacao.php';

#Seta as sessões para navegar no termo
$_SESSION['elabrev']['tcpid'] = $_REQUEST['tcpid'] ? $_REQUEST['tcpid'] : $_SESSION['elabrev']['tcpid'];
$_SESSION['elabrev']['termo']['aba'] = $_REQUEST['aba'] ? $_REQUEST['aba'] : $_SESSION['elabrev']['termo']['aba'];

#Recupera estado atual do documento - integer
if($_SESSION['elabrev']['tcpid'] != ''){
	$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
}
#Recupera perfis do usuario - array
if($_SESSION['usucpf'] != '' && $_SESSION['sisid'] != ''){
	$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
}

#Recupera aba atual
$abaAtual = $_SESSION['elabrev']['termo']['aba'] ? $_SESSION['elabrev']['termo']['aba'] : 'proponente';

#Trata abas
if( $_SESSION['favurl'] != 'elabrev.php?modulo=principal/termoCooperacao/visualizarTermo&acao=A' ){
	if( $_REQUEST['aba'] != '' && $_REQUEST['tcpid'] != '' ){
		echo "<script>
		window.location = 'elabrev.php?modulo=principal/termoCooperacao/visualizarTermo&acao=A&aba=".$_REQUEST['aba']."';
		</script>";
	}if( $_REQUEST['aba'] == 'gerapdf' && $_REQUEST['tcpid'] != '' ){
		echo "<script>
		window.location = 'elabrev.php?modulo=principal/termoCooperacao/visualizarTermo&acao=A&aba=".$_REQUEST['aba']."&requisicao=".$_REQUEST['requisicao']."';
		</script>";
	}
}

#Verifica se é popup para visualizar termo
$boPopup = true;

// Começa tudo bloqueado 
#Outras Abas
$habilita 				= 'N';
$habilita_Und_G 		= 'N';
$habilitaSalvar 		= 'N';
$habilitaInserir 		= 'N';
$habilita_pro_con 		= 'N';
$habilita_parecer		= 'N';
$habilita_cumprimento	= 'N';
#Aba de Previsão orçamentária
$habilita_Plano  		= 'N';
$habilita_Natur  		= 'N';
$habilita_botao  		= 'N';
$habilita_Parc   		= 'N';
$habilita_Acao   		= 'N';
$habilita_Meslib 		= 'N';
#Aba de Anexo
$habilita_botao 		= 'N';

#Faz o include do arquivo referente a aba
if(!empty($abaAtual)){
	include APPRAIZ."elabrev/modulos/principal/termoCooperacao/aba".ucfirst($abaAtual).".inc";
}

?>