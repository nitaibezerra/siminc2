<?
include APPRAIZ ."includes/workflow.php";


// validando o ID da ordem de serviço
if(!$_REQUEST['odsid']) {
	die("<script>alert('OS não reconhecida');window.close();</script>");
}

if($_POST['obsdsc']){

	$sql = "INSERT INTO fabrica.observacoes (
				usucpf,
				odsid,
				obsdsc,
				obsstatus,
				obsdata,
				obstp
			) VALUES (
				'".$_SESSION['usucpf']."',
				'".$_POST['odsid']."',
				'".$_POST['obsdsc']."',
				'A',
				now(),
				'O'
			);";
	$db->executar($sql);
	$db->commit();

	echo "<script>
				alert('Operação realizada com sucesso!');
				location.href = window.location;
		   </script>";
	exit;
}




//permissao para edição da tela
 $habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">

function salvarObs(){
	if(document.formulario_.obsdsc.value.length == 0){
		alert("Preencha o campo Observação");
		document.formulario_.obsdsc.focus();
		return false;
	}

	document.formulario_.submit();
}

</script>

<?

$sql = "SELECT odsid, docid
		FROM fabrica.ordemservico os
		WHERE odsid='".$_REQUEST['odsid']."'";

// buscando dados da OS
$oss = $db->pegaLinha($sql);

// validando se a ordem de serviço existe
if(!$oss) {
	die("<script>alert('OS não existe');window.close();</script>");
}

$link = "<span style='cursor:pointer; color: #0066CC;' onclick=window.location.href='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=".$oss['odsid']."'>".$oss['odsid']."</span>";

$estado_wf = wf_pegarEstadoAtual($oss['docid']);
$titulo_modulo = "OS Nº {$link}";


echo montarAbasArray(carregarMenuOS());

monta_titulo($titulo_modulo, $estado_wf['esddsc']);

?>
<body>

<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<input type="hidden" name="odsid" value="<?=$oss['odsid']?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td width="35%" class="SubTituloDireita">Observação:</td>
		<td>
			<?php $obsdsc = $oss['obsdsc'];?>
			<? echo campo_textarea( 'obsdsc', 'S',  $habil, '', '70', '4', '4000'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td class="SubTituloEsquerda"><input type="button" name="btn" value="Salvar" onclick="salvarObs();"></td>
	</tr>
</table>
</form>

<?php

$sql = "SELECT
		 to_char(o.obsdata, 'DD/MM/YYYY HH24:MI:SS') AS data,
		 o.obsdsc,
		 CASE WHEN o.obslog = 'A' THEN 'SISTEMA FABRICA'
		 ELSE u.usunome
		 END AS nome

		FROM
		 fabrica.observacoes AS o
		 LEFT JOIN seguranca.usuario AS u ON u.usucpf = o.usucpf
	    WHERE
	     o.odsid = '{$oss['odsid']}' AND
	     o.obsstatus = 'A' AND
	     o.obstp = 'O'
	    ORDER BY
	     o.obsid DESC";
$cabecalho = array("Postado","Observação","Autor");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '', '', '100%', 'center');
?>

</body>
</html>



