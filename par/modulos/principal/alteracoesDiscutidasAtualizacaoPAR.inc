<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if( !$_SESSION['par']['inuid'] ){
	echo '<script>
				alert("Faltam dados da sessão!");
				window.location="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
			</script>';
	die;
}

$mnuid = verificaAbasAtualizacao( $_SESSION['par']['inuid'] );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
$mostra = 'N';

if(	in_array(PAR_PERFIL_PREFEITO,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrayPerfil) ||
	in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || 
	in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)
){
	$mostra = "S";
}

//$db->cria_aba( $abacod_tela, $url, '' );
echo cabecalho();
echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">';
				echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);
			echo '</a>';

$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
monta_titulo( 'Atualização do PAR', '' );

if( $_POST['requisicao'] == 'salvar' ){
	global $b;
	
	$sql = "UPDATE par.atualizacaopar SET atpalteracoesdiscutidas = 't' WHERE inuid = ".$_SESSION['par']['inuid'];
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='par.php?modulo=principal/idebAtualizacaoPAR&acao=A';
		  </script>";
		die;
}

$dados = $db->pegaLinha("SELECT * FROM par.atualizacaopar WHERE inuid = ".$_SESSION['par']['inuid']);

$habilitaBotao = $dados['atpalteracoesdiscutidas'] ? 'disabled="disabled"' : '';

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function salvar(){
		if( confirm('Tem certeza que todas as alterações foram discutidas\ncom a equipe local e validadas pelo Comitê Local?') ){
			$('#requisicao').val('salvar');
			$('#formulario').submit();
		} else {
			return false;
		}
	}
</script>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" id="requisicao" name="requisicao">	
	<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td>
			<?php
				if( $dados['atpalteracoesdiscutidas'] ){
					echo '<BR><CENTER><h1>Todas as alterações foram validadas pela Equipe Local e pelo Comitê Local</h1></CENTER>';
				} else {
					if( $mostra == 'S' ){
						echo '<BR><CENTER><h1>Confirme se todas as alterações foram validadas pela Equipe Local e pelo Comitê Local</h1></CENTER>';
					} else {
						echo '<BR><CENTER><h1>Aguardando confirmação se todas as alterações foram validadas pela Equipe Local e pelo Comitê Local</h1></CENTER>';
					}
				}
			?>
		</td>
	</tr>
	<tr>
		<td align="center" colspan="2">
			<?php if( !$habilitaBotao && $mostra == 'S'){ ?>
				<input <?=$habilitaBotao ?> type="button" name="Confirmar" value="Confirmar" onclick="javascript:salvar();"/>
			<?php } ?>
		</td>
	</tr>
	</table>
</form>
		</td>
	</tr>
</table>