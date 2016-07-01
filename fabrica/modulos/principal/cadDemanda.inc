<?
include_once APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

$titulo_modulo = "Cadastrar Solicitação";
monta_titulo($titulo_modulo, 'Requisitante');

//pega perfis dos usuarios
$perfis = arrayPerfil();

//habilita pagina
$habil = 'S';
?>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (BOX) -->
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script language="javascript" type="text/javascript">
messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function checarDatas(){
	<?php
		if(!in_array(PERFIL_SUPER_USUARIO, $perfis) && !in_array(PERFIL_GESTOR_CONTRATO, $perfis)){
	?>

		<? if(!$_REQUEST['scsid']) { ?>
		var data_1 = document.getElementById('scsprevatendimento').value;
		var data_2 = '<? echo date("d/m/Y"); ?>';

		if(data_1){

			var compara01 = parseInt(data_1.split("/")[2].toString() + data_1.split("/")[1].toString() + data_1.split("/")[0].toString());
			var compara02 = parseInt(data_2.split("/")[2].toString() + data_2.split("/")[1].toString() + data_2.split("/")[0].toString());

			if (compara01 >= compara02) {
				return true;
			} else {
				return false;
			}

		}else{
			return true;
		}
		<? } else { ?>

		return true;

		<? } ?>

<?php }else{ ?>
	return true;
<?php } ?>
}
</script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<?
// verificando se editar ou insere nova solicitação
if($_REQUEST['scsid']) {

	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid'];

	$sql = "SELECT s.*, u.usunome as usunomerequisitante FROM fabrica.solicitacaoservico s
			LEFT JOIN seguranca.usuario u ON u.usucpf=s.usucpfrequisitante
			WHERE scsid='".$_REQUEST['scsid']."'";

	$solicitacaoservico  = $db->pegaLinha($sql);
	$estado_wf = wf_pegarEstadoAtual($solicitacaoservico['docid']);

	if($estado_wf['esdid'] != WF_ESTADO_ELABORACAO) {
		ob_clean();
		header("location:fabrica.php?modulo=principal/listarSolicitacoes&acao=A");
		exit;
	}

	$usucpfrequisitante  = $solicitacaoservico['usucpfrequisitante'];
	$usunomerequisitante = $solicitacaoservico['usunomerequisitante'];
	$scsnecessidade      = $solicitacaoservico['scsnecessidade'];
	$scsjustificativa    = $solicitacaoservico['scsjustificativa'];
	$prg                 = $solicitacaoservico['prgid']."_".$solicitacaoservico['prgcod']."_".$solicitacaoservico['prgano'];
	$scsprevatendimento  = $solicitacaoservico['scsprevatendimento'];
	$requisicao = "atualizarSolicitacaoServico";


$sql = "SELECT usucpforigem FROM fabrica.solicitacaoservico WHERE scsid=".$solicitacaoservico['scsid'];
$usucpforigem = $db->pegaUm($sql);

	//segurança pagina
	if($solicitacaoservico['scsid']){
		if(!in_array(PERFIL_SUPER_USUARIO,$perfis)){
			if(!verificaPermissaoEdicao() && $_SESSION['usucpf'] != $solicitacaoservico['usucpfrequisitante'] && $solicitacaoservico["usucpforigem"]!= $usucpforigem){
				$habil = 'N';
			}
		}
	}

} else {
	$requisicao = "inserirSolicitacaoServico";
}
?>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<input type="hidden" name="scsid" value="<? echo $solicitacaoservico['scsid']; ?>">
<input type="hidden" name="usucpforigem" value="<? echo $_SESSION['usucpf']; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Requisitante:</td>
		<td>
		<input type="hidden" name="usucpfrequisitante" id="usucpfrequisitante" value="<? if(!$usucpfrequisitante) $usucpfrequisitante = $_SESSION['usucpf']; echo $usucpfrequisitante; ?>">
		<?
		if(!$usunomerequisitante) $usunomerequisitante = $_SESSION['usunome'];
		echo campo_texto('usunomerequisitante', 'S', 'N', 'Requisitante', 50, 150, '', '', '', '', 0, 'id="usunomerequisitante"' );

		$perfis = arrayPerfil();
		if( in_array(PERFIL_SUPER_USUARIO, $perfis) ||
			in_array(PERFIL_FISCAL_CONTRATO, $perfis) ||
			in_array(PERFIL_GESTOR_CONTRATO, $perfis) ||
            in_array(PERFIL_PREPOSTO, $perfis) ||
            in_array(PERFIL_GERENTE_PROJETO, $perfis) ||
			in_array(PERFIL_REQUISITANTE, $perfis)  )
		{
			echo "<input type=\"button\" name=\"buscarusuario\" value=\"Outro\" onclick=\"buscarUsuario();\">";
		}
		?>
		</td>
		<?
		if($solicitacaoservico['scsid']) {
		?>
		<td rowspan="6" width="5%" valign="top">
		<?
		$dados_wf = array('scsid' => $solicitacaoservico['scsid']);
		wf_desenhaBarraNavegacao(pegarDocidSolicitacaoServico($dados_wf), $dados_wf);
		?>
		</td>
		<?
		}
		?>
	</tr>
	<tr>
		<td class="SubTituloDireita">Unidade do requisitante:</td>
		<td id="unidadedorequisitante"><? pegarUnidadeUsuario(array("usucpf" => $usucpfrequisitante)); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Solicitação:</td>
		<td><? echo campo_textarea( 'scsnecessidade', 'S', $habil, '', '70', '4', '1000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Justificativa:</td>
		<td><? echo campo_textarea( 'scsjustificativa', 'S', $habil, '', '70', '4', '1000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sistema:</td>
		<td><?

                $sidid = '';
                if($_REQUEST['scsid']){
                        $sql = "SELECT sidid FROM fabrica.solicitacaoservico WHERE scsid=".$_SESSION['fabrica_var']['scsid'];
                        $sidid = $db->pegaUm($sql);
                }
					$sql = "select
							sidid as codigo,
							sidabrev ||' - '|| siddescricao as descricao
						from
							demandas.sistemadetalhe
						where
							sidstatus = 'A'
						order by
							2";
				$db->monta_combo("sidid",$sql,$habil,"Selecione...","","","","","S","sidid","",$sidid);
		?></td>
	</tr>
	<!--<tr>
		<td class="SubTituloDireita">Programa de governo:</td>
		<td>
		<?
		//$sql = "SELECT prgid||'_'||prgcod||'_'||prgano as codigo, prgcod || ' - ' || prgdsc as descricao FROM monitora.programa WHERE prgano='".$_SESSION['exercicio']."'";
		//$db->monta_combo('prg', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'prg');
		?>
		</td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita">Expectativa de atendimento:</td>
		<td><? echo campo_data2('scsprevatendimento','S', $habil, 'Expectativa de atendimento', 'S' ); ?></td>
	</tr>
	<?

	if (($requisicao == "inserirSolicitacaoServico") || ($requisicao == "atualizarSolicitacaoServico")) :
	?>
	<tr>
		<td class="SubTituloCentro" colspan="2">Anexo (opcional)</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Anexar um arquivo:</td>
		<td><input type="file" name="arquivo" id="arquivo"></td>
	</tr>
	<?
	endif;
	?>
	<tr>
		<td class="SubTituloDireita" colspan="2">
		<input type="hidden" name="tasid" id="tasid" value="<? echo TPANEXO_SOLICITACAO_OUTROS; ?>">
		<?php if($habil == 'S'){?>
		<input type="button" name="inserirservico" value="Salvar" onclick="if(checarDatas()){submeterSolicitacaoServico();} else {alert('A data de Expectativa de atendimento não pode ser retroativa');}">
		<?php }?>
		<input type="button" name="voltar" value="Voltar" onclick="history.back(-1);">
		</td>
	</tr>
</table>
</form>
