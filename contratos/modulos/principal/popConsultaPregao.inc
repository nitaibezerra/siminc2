<?php
global $db;

monta_titulo( 'Pregão', '' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body>
<? 	

	$sql = "
		SELECT 
			'<center>
				<a style=\"cursor:pointer;\" onclick=\"selecionaPregao('||preid||', '||precodpregao||', '||prenumprocesso||')\">
					<img src=\"/imagens/alterar.gif \" border=0 title=\"Seleciona Pregão\">
				</a>
			</center>' as acao,	precodpregao, prenumprocesso
		FROM
			evento.pregaoevento
		ORDER BY preid";

	$cabecalho 	= array("Ação", "Código", "Número do Processo");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');

?>

<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">

function selecionaPregao(id, cod, proc){

	window.parent.opener.location.href = '?modulo=principal/cadUnidadeRecurso&acao=A&id='+id;

	//window.opener.document.getElementById('preid').value = id;
	//window.opener.document.getElementById('prenumprocesso').value = proc;
	//window.opener.document.getElementById('precodpregao').value = cod;
	//window.opener.document.getElementById('lista').style.display = 'block';
	window.close();
	
}

</script>
</body>