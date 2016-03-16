<?php
include_once APPRAIZ . 'includes/workflow.php';

monta_titulo( "Envio para Diligência", 'Preencha a quantidade de dias' );

$docid  = prePegarDocid($_REQUEST['preid']);
$esdid  = prePegarEstadoAtual($docid);

if($_POST['dia']){
	
	global $db;


	if( (in_array( $esdid, array(WF_TIPO_EM_CORRECAO) )) ){
		
		$sql = "SELECT max(dioid) FROM par.diligenciaobra WHERE preid = {$_REQUEST['preid']} and dioativo = true";

		$dioid = $db->pegaUm($sql);

		$sql = "UPDATE par.diligenciaobra SET dioqtddia = {$_POST['dia']} WHERE dioid = $dioid;";
		$db->executar( $sql );
	} else {
		$sql = "UPDATE par.diligenciaobra SET dioativo = false WHERE preid = ".$_POST['preid'];
		$db->executar( $sql );

		$sql = "INSERT INTO par.diligenciaobra(diotipo, dioqtddia, preid, dioativo, diodata)
	    		VALUES ('".$_POST['tipo']."', ".$_POST['dia'].", ".$_POST['preid'].", 't', now())";
		$db->executar( $sql );
		
		$esdiddestino = WF_TIPO_EM_CORRECAO;		
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
		$aedid = $db->pegaUm( $sql );	
		$arDados = array( 'preid' => $_REQUEST['preid'], 'qrpid' => $_REQUEST['qrpid']);
		$cmddsc = $_POST['cmddsc'];
		
		wf_alterarEstado( $docid, $aedid, $cmddsc, $arDados );	
	}
	$db->commit();

	echo '<script>alert("Operação realizada com sucesso!"); parent.window.opener.location.href = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid='.$_POST['preid'].'";</script>';
	die();
}

?>
<title>Dias de Diligência</title>
<script type="text/javascript">

	function gravarDia(){
		formulario = document.formulario;

		if( formulario.dia.value == '' ){
			alert( "Você precisa preencher a quantidade de dias!" );
			return false
		}
		<?php if( !in_array( $esdid, array(WF_TIPO_EM_CORRECAO) ) ){?>
		if( formulario.cmddsc.value == '' ){
			alert( "Você precisa preencher a Justificativa!" );
			return false
		} 
		<?php } ?>
		else {
			if(confirm( "Você tem certeza que é(são) "+formulario.dia.value+" dia(s)" )){
				formulario.submit();
			} else {
				return false;
			}
		}
	}

</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" id="preid" name="preid" value="<?=$_REQUEST['preid'] ?>">
	<input type="hidden" id="tipo" name="tipo" value="<?=$_REQUEST['tipo'] ?>">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Quantidade de Dias:</td>
			<td>
				<?php
				$sql = "SELECT dioqtddia FROM par.diligenciaobra WHERE dioativo = 't' AND preid = ".$_REQUEST['preid'];
				$dia = $db->pegaUm( $sql );
				echo campo_texto('dia','N','S','',20,60,'[#]','','','','','dia', '', '', "this.value=mascaraglobal('[#]',this.value);") ?>
			</td>
		</tr>
		<?php if( !in_array( $esdid, array(WF_TIPO_EM_CORRECAO) ) ){?>
		<tr>
			<td class="SubTituloDireita">Justificativa:</td>
			<td>
				<?php
				$sql = "SELECT
							cd.cmddsc
						FROM
							obras.preobra po
			                inner join workflow.comentariodocumento cd on cd.docid = po.docid
                            --inner join workflow.documento d on d.docid = cd.docid and d.esdid = 516
			                inner join par.diligenciaobra o on o.preid = po.preid and o.dioativo = true
						WHERE
						 	po.preid = {$_REQUEST['preid']}
			            order by cd.cmdid desc";
				$cmddsc = $db->pegaUm( $sql );
				echo campo_textarea('cmddsc', 'S', 'S', 'Justificativa', 60, 5, 4000, '', '', '', '', 'Justificativa'); ?>
			</td>
		</tr>
		<?php } ?>
		<tr>
			<td align="center" colspan="2">
				<input type="button" name="Salvar" value="Salvar" onclick="javascript:gravarDia();"/>
			</td>
		</tr>
	</table>
</form>