<?php 
include_once APPRAIZ . "includes/workflow.php";

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}

if($_REQUEST['sifid']){
	$docid = criarDocumento($_REQUEST['sifid']);
} ?>

<?php monta_titulo('Workflow Curso',$cies); ?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td align="center">
			<?php if($docid){ 		
				wf_desenhaBarraNavegacao( $docid , array('sifid' => $_REQUEST['sifid']), ''); 
			} ?>
		</td>
	</tr>			
</table>	

<script language="javascript" type="text/javascript">
	window.opener.location.reload();
</script>