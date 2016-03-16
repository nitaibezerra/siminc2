<?php
 
header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";

?>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>

<?

$entid = academico_existecampus( $_SESSION['academico']['entidcampus'] );
$_SESSION['sig_var']['iscampus'] = 'sim';	
$param = "&entidcampus=".$_SESSION["academico"]["entidcampus"];

$_SESSION['sig_var']['entid'] = $entid;
$_SESSION['sig_var']['orgid'] = $_SESSION['academico']['orgid'];

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

// Monta as abas
if( $_SESSION['academico']['entidadenivel'] == 'campus' ){
	if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57277' : '57136'; // Dados Desenv
	}else{
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57268' : '57136';
	}
}else{
	if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57276' : '57135'; //Dados Desenv
	}else{
		$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57267' : '57135';
	}
}

$db->cria_aba($abacod_tela,$url,$parametros);
academico_monta_cabecalho_sig($entid, 'Ações Educacionais');

$qrpid = pegaQrpid( $entid, QUEID_ACADEMICO );

?>

<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Questionário</legend>
			<?php
				$habilitado = 'S';
				
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'relatorio' => 'modulo=relatorioQuestionario&acao=A', 'habilitado' => $habilitado  ) );
			?>
		</fieldset>
	</tr>
</table>