<?

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

if($db->testa_superuser()) {
	die("<script>window.location='smfc.php?modulo=principal/mec/meccadastro&acao=A';</script>");	
}

?>
