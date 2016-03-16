<?
// Recupera o perfil do usuário no Mais Educação.
$usuPerfil = arrayPerfil();

if($_POST){
	
	$sql = "INSERT INTO pdeescola.meadesao(
            maddata, madanoreferencia, madadesao, estuf, muncod, usucpf)
    		VALUES ('".date("Y-m-d")."',
    				'".$_SESSION["exercicio"]."',
    				'S',
    				".($_POST["estuf"] ? "'".$_POST["estuf"]."'" : "null").",
    				".($_POST["muncod"] ? "'".$_POST["muncod"]."'" : "null").", 
    				'".$_SESSION["usucpf"]."')
    		";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			location.href='pdeescola.php?modulo=merelatorio/relatorio_plano_atendimento&acao=A';
		  </script>";
	exit;
	
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Termo de Adesão', 'Plano de Atendimento Geral Consolidado - ' . $_SESSION["exercicio"] );




if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
	echo "<script>
			location.href='pdeescola.php?modulo=merelatorio/relatorio_plano_atendimento&acao=A';
		  </script>";
	exit;
}
else if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT muncod FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO;
	$muncod = $db->pegaUm($sql);
	
	$sql = "select madid from pdeescola.meadesao where muncod = '".$muncod."'";
	$madid = $db->pegaUm($sql);
	
	if($madid){
		echo "<script>
				location.href='pdeescola.php?modulo=merelatorio/relatorio_plano_atendimento&acao=A';
			  </script>";
		exit;
	}
	
}
else if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT estuf FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO;
	$estuf = $db->pegaUm($sql);
	
	$sql = "select madid from pdeescola.meadesao where estuf = '".$estuf."'";
	$madid = $db->pegaUm($sql);
	
	if($madid){
		echo "<script>
				location.href='pdeescola.php?modulo=merelatorio/relatorio_plano_atendimento&acao=A';
			  </script>";
		exit;
	}
	
}
else {
	echo "<script>
			alert('Não há nenhum Munícipio ou Estado associado ao seu perfil.');
			history.back(-1);
		  </script>";
	exit;
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>

<form name="formulario" id="formulario" method="post" action="">

<input type="hidden" name="muncod" value="<?=$muncod?>">
<input type="hidden" name="estuf" value="<?=$estuf?>">

<table cellSpacing="0" cellPadding="3"	align="center" width="50%">
	<tr><td>&nbsp;</td></tr>
	<tr>
		<td style="font-size: 14px">
			
			<center>
			<img src="../../imagens/brasao.gif" width="80" height="80">
			
			
			<b>
							<br>MINISTÉRIO DA EDUCAÇÃO
				<br>SECRETARIA DE EDUCAÇÃO BÁSICA
				<br>DIRETORIA DE CURRÍCULOS E EDUCAÇÃO INTEGRAL
				
				<br><br>
				
				TERMO DE COMPROMISSO AO PROGRAMA MAIS EDUCAÇÃO
				<br><br>				
			</b>
			
			</center>
			
			
			A Entidade Executora (EEx) firma o compromisso de acatar, cumprir e fazer cumprir as disposições da Resolução do Conselho Deliberativo do FNDE, que dispõe, no corrente exercício, dos processos de adesão e habilitação, formas de execução e prestação de contas, referente ao Programa Dinheiro Direto na Escola/Educação Integral (PDDE/Integral).
			<br><br>
			Este termo acatado formaliza a adesão ao Programa Mais Educação das escolas relacionadas no Plano Geral Consolidado de sua rede de ensino. 
			
			
		</td>
	</tr>
</table>

<br><br>

<table bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3" align="center" width="100%">
	<tr>
		<td align="center" >
			<input type="button" id="btn_aceito" onclick="submeteFiltro();" value="ACEITO">
			&nbsp;&nbsp;
			<input type="button" onclick="history.back();" value="NÃO ACEITO">
		</td>
	</tr>
</table>


</form>


<script type="text/javascript">

function submeteFiltro() {
	var form 	= document.getElementById("formulario");
	var button	= document.getElementById("btn_aceito");
	
	button.disabled = true;

	form.submit();
}

</script>