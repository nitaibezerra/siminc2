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


if(checkPerfil(array(PERFIL_ADMINISTRADOR, PERFIL_SUPER_USUARIO))){	
	
	header("Location: em.php?modulo=painel&acao=A");
	
}elseif(checkPerfil(PERFIL_ANALISTA_SEDUC)){
	
	$sql = "SELECT ur.estuf FROM em.usuarioresponsabilidade ur	 		
			WHERE usucpf = '{$_SESSION['usucpf']}'
			AND rpustatus = 'A' 
			AND pflcod = ".PERFIL_ANALISTA_SEDUC;
	
	$rsResponsabilidade = $db->pegaLinha($sql);
	
	if($rsResponsabilidade){
		
		$_SESSION['estuf'] = $rsResponsabilidade['estuf'];		 
		
		header("Location: em.php?modulo=lista&acao=E");
			
	}
			
}elseif(checkPerfil(PERFIL_CADASTRADOR)){
	
	
	$sql = "
		SELECT ur.entid,
		       ei.emiid,
		       eno.estuf
		FROM em.usuarioresponsabilidade ur
	 		LEFT JOIN em.emiensinomedioinovador ei  ON ei.entid  = ur.entid
	 		LEFT JOIN entidade.endereco         eno ON eno.entid = ur.entid
		WHERE ur.usucpf    = '{$_SESSION['usucpf']}'
		AND   ur.rpustatus = 'A' 
		AND   ur.pflcod    = ".PERFIL_CADASTRADOR."
		AND   eno.tpeid    = 1
	";
	
	$rsResponsabilidade = $db->pegaLinha($sql);
	
	if($rsResponsabilidade){
		
		$_SESSION['entid'] = $rsResponsabilidade['entid'];
		$_SESSION['emiid'] = $rsResponsabilidade['emiid']; 
		$_SESSION['estuf'] = $rsResponsabilidade['estuf'];
		
		header("Location: em.php?modulo=principal/prcEscola&acao=A");
			
	}
	
}elseif(checkPerfil(PERFIL_CONSULTA)){
	
	$sql = "SELECT ur.estuf FROM em.usuarioresponsabilidade ur	 		
			WHERE usucpf = '{$_SESSION['usucpf']}'
			AND rpustatus = 'A' 
			AND pflcod = ".PERFIL_CONSULTA;
	
	$rsResponsabilidade = $db->pegaLinha($sql);
	
	if($rsResponsabilidade){
		
		$_SESSION['estuf'] = $rsResponsabilidade['estuf'];		 
		
		header("Location: em.php?modulo=lista&acao=E");
			
	}
	
}elseif(checkPerfil(PERFIL_SEC_ESTADUAL)){
	
	$sql = "SELECT estuf FROM em.usuarioresponsabilidade ur	 		
			WHERE usucpf = '{$_SESSION['usucpf']}'
			AND rpustatus = 'A' 
			AND pflcod = ".PERFIL_SEC_ESTADUAL;
	
	$rsResponsabilidade = $db->pegaLinha($sql);
	
	if($rsResponsabilidade){
		
		header("Location: em.php?modulo=lista&acao=E");
			
	}	
}

include  APPRAIZ."includes/cabecalho.inc";
alert('Usuário sem resposabilidade!');

?>
<br>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  <td><h1>Bem-vindo</h1></td>
	</tr>
</table>
