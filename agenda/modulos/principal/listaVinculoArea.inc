<?php
$agdid = $_SESSION['agenda']['agdid'];
$eveid = $_SESSION['agenda']['eveid'];

/*
 * Valida edição do evento
 */
$habil  = 'S';
$userResp = new UsuarioResponsabilidade();
$arAgdid  = $userResp->pegaRespAgenda();
if ( !verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) && !in_array($agdid, $arAgdid) ){
	$evento  = new Evento();
	$arAevid = $userResp->pegaRespArea();
	$arEveid = $evento->pegaEventoPorArea( $arAevid, $agdid);
	$arEveid = ( $arEveid ? $arEveid : array(0) );

	if ( !in_array($eveid, $arEveid) ){
		die('<script>
				alert(\'O usuário não possui permissão de edição deste evento!\');
				location.href=\'?modulo=principal/cadastroEvento&acao=E\';
			 </script>');		
	}
	$habil = 'N';		
}


switch ( $_REQUEST['op'] ){
	case 'excluir':
		if ( $_REQUEST['evaid'] ){
			$msg 	= 'Operação realizada com sucesso!';
			$eventoArea = new EventoArea( $_REQUEST['evaid'] );
			$eventoArea->popularDadosObjeto( array('evastatus' => 'I') )
				   	   ->salvar();
			$db->commit();
		}else{
			$msg = 'Falha na execução da operação!';
		}	
			
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=principal/listaVinculoArea&acao=A\';
			 </script>');
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$arAba = abaEdicaoEvento();
echo montarAbasArray($arAba, "?modulo=principal/listaVinculoArea&acao=A");
monta_titulo( 'Lista de Áreas Vinculadas', 'Lista das áreas que estão vinculadas ao evento');

 if ( $_SESSION['agenda']['eveid'] ){
	 
	$evento = new Evento();
	$dados = $evento->carregaTodosDados( $_SESSION['agenda']['eveid']);

	?>
	<table class="tabela" align="center">
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Código</td>
			<td> <?php echo $dados['eveid']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Evento</td>
			<td> <?php echo $dados['tpedsc']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Data de Início</td>
			<td> <?php echo $dados['evedtinicio']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Solicitante</td>
			<td> <?php echo $dados['sltdsc']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Entidade</td>
			<td> <?php echo $dados['etadsc']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Assunto</td>
			<td> <?php echo $dados['eveassunto']; ?></td>
		</tr>
	</table>
<?php }  


$eventoArea = new EventoArea();
$sql 		= $eventoArea->listaSQL( array('eveid' => $eveid), $habil );

$cabecalho = array("Ação", "Área", "Situação do Vínculo");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<form method="post" action="" name="formListaVinculo" id="formListaVinculo">
<input type="hidden" name="op" id="op" value="">
<input type="hidden" name="evaid" id="evaid" value="">



<?php 
if ( $habil == 'S' ):
?>
<table class="tabela" align="center">
	<tr bgcolor="#CCCCCC">
	   <td>
	   	<input type="button" name="btalterar" value="Vincular Nova Área" onclick="location.href='?modulo=principal/vinculoArea&acao=A';" class="botao">
	   </td>
	</tr> 	
</table>
<?php 
endif;
?>
</form>
<script>
function alterarVinculo( evaid ){
	$('#evaid').val( evaid );
	$('#formListaVinculo').attr('action', '?modulo=principal/vinculoArea&acao=E')
					 	  .submit();
}

function excluirVinculo( evaid ){
	if ( confirm('Deseja apagar esse vínculo de área?') ){
		$('[name=op]').val('excluir');
		$('[name=evaid]').val( evaid );
		$('[name=formListaVinculo]').submit();
	}	
}
</script>