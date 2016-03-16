<?php
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
*/

if($db->testa_superuser()){
	header("location:pdeinterativo2013.php?modulo=principal/principal&acao=A");
	exit;	
} else {
	$arrPerfil = pegaPerfilGeral();
	if(in_array(PDEESC_PERFIL_DIRETOR, $arrPerfil)) {
		$_SESSION['pdeinterativo2013_vars']['usucpfdiretor'] = $_SESSION['usucpf'];
		$pflcod = PDEESC_PERFIL_DIRETOR;
		$sql = "SELECT ende.muncod FROM pdeinterativo2013.usuarioresponsabilidade urs
			    LEFT JOIN entidade.endereco ende ON ende.entid = urs.entid
			    WHERE usucpf='".$_SESSION['usucpf']."' AND ende.muncod IS NOT NULL  and pflcod = $pflcod  and rpustatus = 'A' order by rpuid desc";
		$muncod = $db->pegaUm($sql);
		if(!$muncod) {
			echo "<script>alert('Usuário não possui escola vinculada!');</script>";
		}else{
			header("location:pdeinterativo2013.php?modulo=principal/principalDiretor&acao=A");
			exit;
		}
	}else{
		header("location:pdeinterativo2013.php?modulo=principal/principal&acao=A");
		exit;
	}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

?>
<br>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  <td><h1>Bem-vindo</h1></td>
	</tr>
</table>
<?php } ?>
