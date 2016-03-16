<?php
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
require_once APPRAIZ . "includes/classes/AbaAjax.class.inc";
header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

if (!$_SESSION['entid']){
	die('<script>
			history.go(-1);
		 </script>');
}

$entid = $_SESSION['entid'];
$_SESSION['pdeid'] = pegarPdeid($entid);
$pdeid = $_SESSION['pdeid'];

if($_GET['acao'] == 'A'){
	$qrpid = pegaQrpid( $pdeid, QUESTIONARIO_I );
}elseif($_GET['acao'] == 'B'){
	$qrpid = pegaQrpid( $pdeid, QUESTIONARIO_II );
}elseif($_GET['acao'] == 'C'){
	$qrpid = pegaQrpid( $pdeid, QUESTIONARIO_III );
} 


include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
echo montarAbasArray(carregaAbas(), "/pdeescola/pdeescola.php?modulo=principal/questionario1&acao=A");

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Questionário';
monta_titulo($titulo_modulo,'Respondido<img src="/imagens/check_p.gif" style="border:0;">
							 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						     Não Respondido<img src="/imagens/atencao.png" style="border:0;">');

echo cabecalhoPDE();
?>
<center>
<fieldset style="width: 94%; background: #fff;"  >
<legend>Questionários</legend>
<?php 

$menu = array(

			0 => array("id" => 0, "descricao" => "Folha de Rosto", "link" => "/pdeescola/pdeescola.php?modulo=principal/questionario1&acao=A"),
			1 => array("id" => 1, "descricao" => "Questionário do Diretor", "link" => "/pdeescola/pdeescola.php?modulo=principal/questionario1&acao=B"),
			2 => array("id" => 2, "descricao" => "Folha de Resposta do Diretor", "link" => "/pdeescola/pdeescola.php?modulo=principal/questionario1&acao=C"),
			);
echo montarAbasArray($menu, "/pdeescola/pdeescola.php?modulo=principal/questionario1&acao=".$_GET['acao']);

$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25) );

?>
</fieldset>
</center>