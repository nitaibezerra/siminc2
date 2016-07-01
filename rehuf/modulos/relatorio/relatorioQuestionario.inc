<html>
<head>
	<title><?=$db->pegaUm("SELECT ent.entnome || ' - ' || ena.entsig || ' ( ' || mundescricao || '/' || ende.estuf || ' )'  FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
				LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
				LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
				WHERE ent.entid = '". $_SESSION['rehuf_var']['entid'] ."' ORDER BY ent.entnome") ?>
	</title>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script type="text/javascript"></script>
	<style type="">
		@media print {.notprint { display: none } .div_rolagem{display: none} }	
		@media screen {.notscreen { display: none; }
		
		.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
		
	</style>
</head>
<body>
<?php

ini_set("memory_limit","500M");
set_time_limit(0);

include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionarioResposta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QImpressao.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";


header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

$usucpf = $_SESSION['usucpf'];
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
echo '<br>';
?>
<table cellSpacing='1' cellPadding='3' align='center'>
	<tr>
		<td style="text-align: center;" class="div_rolagem">
			<input type="button" name="imprimir" value="Imprimir" onclick="javascript: window.print();">
		</td>
	</tr>
</table>
<table>
	<tr>
		<td>
			<?php 
				$qrpid = $_REQUEST['qrpid'];
				$obImprime = new QImpressao( array('tema' => 5, 'cabecalho' => 'N') );
				echo $obImprime->montaArvore();
			?>
		</td>
	</tr>
</table>
</body>
</html>