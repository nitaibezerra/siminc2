<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */
//Chamada de programa

ob_clean();

$perfis = pegaPerfilGeral();

if(in_array(PFL_PROFESSOR,$perfis)) {
// 	$teste = testaProfessor();
// 	if($teste['erro']!=''){
// 		echo "
// 			<script>
// 				alert('".$teste['erro']."');
// 				window.location = 'http://".URL_SISTEMA."/fiesabatimento/login_ssd.php';
// 			</script>
// 		";
// 		die(); 
// 	}
	
	
	die("<script>window.location='fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A';</script>");
	header("location: fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A");
	exit;
	
	
}

if(in_array(PFL_SECRETARIO_ESTADUAL,$perfis) || in_array(PFL_SECRETARIO_MUNICIPAL,$perfis)) {
	
	
	die("<script>window.location='fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';</script>");
	header("location: fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A");
	exit;
	
		
	/*
	die("<script>window.location='fiesabatimento.php?modulo=principal/listasolicitacaoabatimentoconcedida&acao=A';</script>");
	header("location: fiesabatimento.php?modulo=principal/listasolicitacaoabatimentoconcedida&acao=A");
	exit;
	*/
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
?>
<form name="formulario_aprovacao" id="formulario_aprovacao" action="" method="post" >
	<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
		<tr bgcolor="#e7e7e7">
		  <td>
		  	<h1>Bem-vindo</h1>
		  	 
		  	<h3>
		  	<center>
		  		<!--
		  		<font color=red>Sistema temporariamente indisponível para manutenção.</font> 
		  		 
		  		<br>
		  		<font color=blue>Previsão de retorno: 31.10.2014</font>
		  		 -->
		  	</center>
		  	</h3>
		  	<br>
		  	<br><br>
		  	 
		  </td>
		</tr>
	</table>
</form>
