<?php

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include  	 APPRAIZ . "includes/cabecalho.inc";

header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre


$qrpid = pegaQrpid( $_SESSION['usucpf'], 45 );
//$qrpid = 50;

?>
<script language="JavaScript"></script>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Questionário</legend>
			<?php
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25 ) );
			?>
		</fieldset>
	</tr>
</table>
