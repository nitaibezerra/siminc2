<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>

<?php

include_once( APPRAIZ. "pes/modulos/principal/acompanhamento/_funcoes_despesas.php" );

$entcodigo = $_SESSION['pes']['codigo_entidade'];
$unicod    = $_SESSION['pes']['codigo_unidadeorcamentaria'];

include APPRAIZ . 'includes/cabecalho.inc';

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

// Tabela
montaTabelaPainelExecucaoResumida();
?>
