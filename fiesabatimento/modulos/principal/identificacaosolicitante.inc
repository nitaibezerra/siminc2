<?
/*
if( !in_array($_SESSION['usucpf'], $cpf_liberados) ){
	echo '<script>
 			location.href = "fiesabatimento.php?modulo=inicio&acao=C";
 		  </script>';
	die;
}
*/

require_once APPRAIZ . 'includes/SSD.php';
if( $_GET['ok'] == 1 ) {
	$ssd = new Essencial_Adapter_SSD;
	$resposta = $ssd->solicitarAcesso(51);
	echo "<script>window.location = '{$resposta}'</script>";
}

// if(!$_SESSION['fiesabatimento_var']['cpfusuario']) die("<script>alert('Professor não identificado');window.location='login_ssd.php';</script>");
// if( ($_SERVER['HTTP_HOST'] == 'simec-local' ||
// 		$_SERVER['HTTP_HOST'] == 'simec-d.mec.gov.br' ||
// 		$_SERVER['HTTP_HOST'] == 'simec-d') && $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
// 	if( $_SESSION['usucpf'] == '' ){
// 		$_SESSION['fiesabatimento_var']['cpfusuario'] = $_REQUEST['cpf'];
// 	}
// }

// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = '';
// $_SESSION['fiesabatimento_var']['cpfusuario'] = ''; // CPF com dados em branco


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$dadosUsu = pegarInfoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);

//trata cpf
/*
if( strlen($_SESSION['fiesabatimento_var']['cpfusuario']) < 11){
	$_SESSION['fiesabatimento_var']['cpfusuario'] = str_pad($_SESSION['fiesabatimento_var']['cpfusuario'], 11, "0", STR_PAD_LEFT);
}
*/

$sql = "SELECT
			idoid
		FROM
			fiesabatimento.identificacaodocente
		WHERE
			idocpf = '".$_SESSION['fiesabatimento_var']['cpfusuario']."'
			AND idostatus = 'A'";

$idoid = $db->pegaUm($sql);

if( $idoid == '' ){
	$onclick2 = "alert('Solicitante não possui o benefício para solicitar o cancelamento do mesmo.');";
?>
	<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
	<script>
	$(document).ready(function(){
		$('[title="Cancelar Abatimento"]').attr('href',' ');
		$('[title="Cancelar Abatimento"]').click(function(){<?=$onclick2 ?>});
	});
	</script>
<?php 
}

$onclick = "window.location='fiesabatimento.php?modulo=principal/identificacaoabatimento&acao=A';";

if( is_array($dadosUsu) ){
	//foreach($dadosUsu as $dado){
		//if( $dado == '' ){
		if ((trim($dadosUsu[nu_cpf])== false) || (trim($dadosUsu[nu_rg])== false)  || 
	 	 (trim($dadosUsu[nu_cep])== false) || (trim($dadosUsu[ds_numero])== false) ||
	 	 (trim($dadosUsu[ds_bairro])== false) || (trim($dadosUsu[nu_ddd])== false)    ||	    
	 	 (trim($dadosUsu[no_pessoa])== false) || (trim($dadosUsu[ds_logradouro])== false) || 
	 	 (trim($dadosUsu[nu_telefone])== false) || (trim($dadosUsu[dt_nascimento])== false) ||
	 	 (trim($dadosUsu[no_municipio_acento])== false)   || (trim($dadosUsu[ds_contato_eletronico])== false)
	 	 ){
			$onclick = "alert('Dados de identificação não preenchidos. Efetue a correção na Plataforma Freire.');";
?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
$(document).ready(function(){
	$('[title="Solicitação de abatimento"]').attr('href',' ');
	$('[title="Solicitação de abatimento"]').click(function(){<?=$onclick ?>});
	$('[title="Cancelar Abatimento"]').attr('href',' ');
	$('[title="Cancelar Abatimento"]').click(function(){<?=$onclick ?>});
});
</script>
<?php 
		//break;
		}
	//}
}

if( $idoid && !verificaSolicitacaoAtiva( $idoid ) ){
	atualizaInformações($idoid);
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Orientações :</b></td>
		<td>
			<?php if( !verificaSolicitacaoAtiva( $idoid ) ){ ?>
			Confira as informações e, se necessário, faça a(s) correção(ões) na Plataforma Freire, 
			<!-- 
			<a href="fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A&ok=1">clicando aqui</a>
			 -->
			 <a href="<?=LINK_FREIRE ?>" target="_blank">clicando aqui</a>
			<?php }?>
		</td>
	</tr>
	<tr>
		<td colspan="2"><? htmlDadosUsuario($_SESSION['fiesabatimento_var']['cpfusuario']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="confirmar" value="Próximo" onclick="<?=$onclick ?>"></td>
	</tr>
</table>