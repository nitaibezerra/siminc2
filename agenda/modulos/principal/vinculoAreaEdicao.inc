<?php 
include APPRAIZ ."includes/workflow.php";

$docid = pegaDocidEventoArea($evaid);

$habilNovoParecer = 'S';
$userResp = new UsuarioResponsabilidade();
$arAgdid  = $userResp->pegaRespAgenda();
if ( !verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) && !in_array($agdid, $arAgdid) ){
	$arAevid = $userResp->pegaRespArea();
	$evento  = new Evento();
	$arEveid = $evento->pegaEventoPorArea( $arAevid, $agdid);
	$arEveid = ( $arEveid ? $arEveid : array(0) );

	if ( !in_array($eveid, $arEveid) ){
		die('<script>
				alert(\'O usuário não possui permissão de acesso ao evento!\');
				location.href=\'?modulo=principal/listaAgenda&acao=A\';
			 </script>');		
	}
	
	$esdid = pegaEstadoEventoArea( $docid );
	if ( !in_array( $esdid, $_ESTADOS_WF_AGENDA_UNIDADE ) ){
		$habilNovoParecer = 'N';		
	}
}

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
<?php } ?>

<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita" width="200">Área:</td>
		<td>
		<?php
		$area = new AreaEnvolvida();
		$dados = $area->listaCombo();
		echo $db->monta_combo("aevid", $dados, 'N','Selecione...','carregaDirigente', '', '',240,'S','aevid');
		?>
		</td>
		<td rowspan="5" width="10" valign="top">
		<?php 
		$dados_wf = array();
		wf_desenhaBarraNavegacao($docid, $dados_wf);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" valign="top" colspan="2">Dirigente(s) Vinculado(s) à Área</td>
	</tr>
	<tr>
		<td valign="top" id="td_lista_dirigente" colspan="2">
			<span>selecione uma área para visualizar seus dirigentes.</span>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" valign="top" colspan="2">
			Histórico de Encaminhamento Evento/Área
			<?php 
			/*
			 * Valida edição do evento
			 */
			if ( $habilNovoParecer == 'S' ):
			?>
			<br>
			<a href="javascript:cadastrarParecer(<?php echo $evaid ?>);" style="float:left; margin-left:40px;">[Cadastrar Nova Informação]</a>
			<?php 
			endif;
			?>
		</td>
	</tr>
	<tr>
		<td valign="top" colspan="2">
			<div style="height: 150px; overflow: auto;">
			<?php
			$histEncaminhamento = new HistEncaminhamento();
			$sql 	= $histEncaminhamento->listaSQL( array('evaid' => $evaid, 'agdid' => $agdid) );
			
			$cabecalho = array("Ação", "Situação da Tramitação", "Informação", "Informação Complementar", "Data/Hora de Inclusão da Informação", "Usuário Responsável pela informação");
			$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
			?>
			</div>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td colspan="2">
	   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">
	   	<input type="button" name="btvoltar" value="Voltar" onclick="location.href='?modulo=principal/listaVinculoArea&acao=A';" class="botao">
	   	<?php 
	   	if ( $habilNovoParecer == 'S' ):
	   	?>
	   	&nbsp;&nbsp;&nbsp;&nbsp;
	   	<input type="button" name="btevento" value="Vincular Nova Área" onclick="location.href='?modulo=principal/vinculoArea&acao=A';" class="botao">
	   	<?php 
	   	endif;
	   	?>
	   </td>
	</tr> 	
</table>

