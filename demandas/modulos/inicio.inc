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
//include  APPRAIZ."includes/cabecalho.inc";

//session avaliacao
if($_SESSION['dmdidavaliacao']){
	$_SESSION['dmdid'] = $_SESSION['dmdidavaliacao'];
	$_SESSION['dmdidavaliacao'] = "";
	die('<script>
		location.href = "?modulo=principal/avaliacao&acao=A";
		</script>');
}


$perfilc = arrayPerfil();

//verifica se tem acesso a celula gabinete
if ( !in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfilc) ){
	if ( in_array(DEMANDA_PERFIL_GESTOR_EQUIPE, $perfilc) || in_array(DEMANDA_PERFIL_EQUIPE, $perfilc) ) {
		
		$total = $db->pegaUm("SELECT count(celid) as total from demandas.usuarioresponsabilidade where rpustatus='A' and celid=49 and usucpf = '".$_SESSION['usucpf']."'");
		if($total>0){
			die('<script>
					location.href = "popPainelGerenciaGabinete.php";
				</script>');
		}
	}
}

//econde o campo origem da demanda	
if (count($perfilc) == 1 && in_array(DEMANDA_PERFIL_DEMANDANTE, $perfilc)){
	die('<script>
		//window.open( "../geral/popup_mensagem.php", "mensagens", "width=780,height=400,scrollbars=yes,menubar=no,toolbar=no,statusbar=no" );
		//w = window.open( "?modulo=principal/popMensagem&acao=C", "relatorio", "width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1" ); 
		//w.focus();
		//location.href = "?modulo=principal/cadDemanda&acao=A";
		location.href = "?modulo=principal/painelDemandante&acao=A";
		</script>');
}
else{
	die('<script>
		location.href = "?modulo=principal/lista&acao=A";
		</script>');
}
?>

