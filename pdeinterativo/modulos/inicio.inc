<?
if($db->testa_superuser()) {
	
	header("location:pdeinterativo.php?modulo=principal/principal&acao=A");
	exit;
	
} else {
	
	$arrPerfil = pegaPerfilGeral();
	
	if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_MUNICIPAL, $arrPerfil) ||
	   in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL, $arrPerfil) || 
	   in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL, $arrPerfil) || in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL, $arrPerfil)) {
		header("location:pdeinterativo.php?modulo=principal/principal&acao=A");
		exit;
	}
	
	if(in_array(PDEESC_PERFIL_DIRETOR, $arrPerfil)) {
		$_SESSION['pdeinterativo_vars']['usucpfdiretor'] = $_SESSION['usucpf'];
		$pflcod = PDEESC_PERFIL_DIRETOR;
		$muncod = $db->pegaUm("SELECT ende.muncod FROM pdeinterativo.usuarioresponsabilidade urs
							   LEFT JOIN entidade.endereco ende ON ende.entid = urs.entid
							   WHERE usucpf='".$_SESSION['usucpf']."' AND ende.muncod IS NOT NULL  and pflcod = $pflcod  and rpustatus = 'A' order by rpuid desc");
		if(!$muncod) {
			echo "<script>alert('Usuário não possui escola vinculada!');history.back(-1)</script>";
		}else{
			header("location:pdeinterativo.php?modulo=principal/principalDiretor&acao=A");
		}
		exit;
	}


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
include  APPRAIZ."includes/cabecalho.inc";

?>
<br>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  <td><center><h1>Bem-vindo</h1></center></td>
	</tr>
	<tr>
		<td bgcolor="#E8E8E8">
			<div align="center">
				<font color="#36648B" >
					<b>Bem vindo ao PDE Interativo!</b>
				</font>
			</div>	
		</td>
	</tr>

</table>
<?php } ?>
