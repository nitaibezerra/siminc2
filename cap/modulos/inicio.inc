<?php
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

// gera erro quando servidor implicar com output já enviado - solução é o js a seguir
// header("location: cap.php?modulo=principal/listaviagem&acao=A");
?>
<script>
	window.location.href = "cap.php?modulo=principal/listaviagem&acao=A";
</script>