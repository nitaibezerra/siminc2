<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

//recuperando entid 
if(!$_REQUEST['entid']){
	$entid = $_SESSION['entid'];
}
if($_REQUEST['entid']){
	$entid = $_REQUEST['entid'];
	$_SESSION['entid'] = $entid; 
}

if(!isset($_SESSION['entid']) || $_SESSION['entid'] == null){
	
	alert("Este relatório deve ser acessado pelo módulo PDE Escola!");
	echo "<script>document.location.href = 'pdeescola.php?modulo=inicio&acao=C';</script>";	
	exit();
}

// Tratamento para exibir alert qdo perfil não está atribuído a nenhuma escola
if (!isset($_SESSION['entid'])){
	echo('<script>
		    alert(\'Usuário não possui uma escola atribuída.\');
		    history.go(-1);
	      </script>');
    exit();
}

// Tratamento Exercicio base de comparacao - exibir dados do ano de exercicio ate ano vigente.
if($_SESSION["exercicio"] != $_SESSION['exercicio_atual']) {
				echo "<script language=\javascript\" type=\"text/javascript\">
						alert('Nenhum dado para este ano Exercício Base de Comparação.');
						window.history.go(-1);
 					</script>";
				exit;	
}
include  APPRAIZ."includes/cabecalho.inc";
include_once APPRAIZ . 'includes/workflow.php';

unset($_SESSION['pdeid']);
criarDocumento( $entid );
$docid 			   = pegarDocid( $entid );
$_SESSION['pdeid'] = pegarPdeid($entid);

echo '<BR>';

$db->cria_aba($abacod_tela,$url,$parametros);

?>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<?=cabecalhoPDE(); ?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<?=montaTreeReport();?>
			</td>
			<td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
			<?php 
				// Barra de estado atual e ações e Historico
				wf_desenhaBarraNavegacao( $docid , array( 'unicod' => '' ) );			
			?>		
			</td>	
		</tr>
	</tbody>
</table>	
