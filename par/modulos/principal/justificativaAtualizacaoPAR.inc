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

if( $_POST['requisicao'] == 'salvar' ){
	global $b;
	
	$sql = "SELECT inuid FROM par.atualizacaopar WHERE inuid = ".$_SESSION['par']['inuid'];
	$inuid = $db->pegaUm($sql);
	
	if( $inuid ){
		$sql = "UPDATE par.atualizacaopar SET atpjustificativa = '".$_POST['justificativa']."' WHERE inuid = ".$inuid;
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO par.atualizacaopar (inuid, atpjustificativa, atpdata) VALUES (".$_SESSION['par']['inuid'].",'".$_POST['justificativa']."',NOW())";
		$db->executar($sql);
	}
	$db->commit();
	
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='par.php?modulo=principal/dadosUnidadeAtualizacaoPAR&acao=A';
		  </script>";
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
			monta_titulo( 'Justificativa de Atualização do PAR', 'Explique a necessidade de atualização do PAR' );
			
			
			$dados = $db->pegaLinha("SELECT * FROM par.atualizacaopar WHERE inuid = ".$_SESSION['par']['inuid']);
			
			?>
			<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
			<script type="text/javascript">
				function salvar(){
					if($('#justificativa').val() == ''){
						alert('Dê uma justificativa para a atualização do PAR.');
						return false;
					}
					$('#requisicao').val('salvar');
					$('#formulario').submit();
				}
			</script>
			<form name="formulario" id="formulario" action="" method="post">
				<input type="hidden" id="requisicao" name="requisicao">	
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
					<tr>
						<td class="SubTituloDireita" valign="top">Justificativa: </td>
						<td>
							<?
							echo campo_textarea('justificativa','S',$mostra,'Justificativa',100,10,1000,'',0,'',false,null,$dados['atpjustificativa']);
							?>
						</td>
					</tr>
					<?php if( $mostra == 'S' ){ ?>
					<tr>
						<td align="center" colspan="2">
							<input type="button" name="Iniciar Atualização" value="Iniciar Atualização" onclick="javascript:salvar();"/>
						</td>
					</tr>
					<?php } ?>
				</table>
			</form>
		</td>
	</tr>
</table>