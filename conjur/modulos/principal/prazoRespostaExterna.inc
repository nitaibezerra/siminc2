<?php 

include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

$ProcessoConjur = new ProcessoConjur();

if( $_POST['req'] == 'atualizarPrazo' ){

    global $db;
    // ver($_POST,d);
    if(strlen($_REQUEST['dtprazo']) == 10){
        $dt = explode('/',$_REQUEST['dtprazo']);
        $dt = $dt[2]."/".$dt[1]."/".$dt[0];

        $sql = "UPDATE conjur.estruturaprocesso SET 
                        espdtrespexterna = '{$dt}'
                WHERE
                        prcid = {$_SESSION['conjur_var']['prcid']} ";

        $db->executar($sql);
        $db->commit();
        // registra responsavel
        $ProcessoConjur->aplicaResponsavelPorInformativos( $_SESSION['conjur_var']['prcid'], $_POST['responsavel'] );
        direcionar('?modulo=principal/editarprocesso&acao=A&prcid='.$_SESSION['conjur_var']['prcid'],'Prazo definido.');
    }else{
        echo "<script>
                        alert('Preencha a data no campo Prazo corretamente!');
              </script>";	
    }
}

 ?>

<br></br>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<form name="formulario" id="formulario" method="post" action="">
<input type="hidden" name="requisicao" value="atualizarprocessoconjur">
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1" bgcolor="#f5f5f5">
	<tr>
		<td colspan="2" class="SubTituloCentro"><b>Cadastro de Prazo para Resposta Externa</b></td>
	</tr>

	<!-- DATA PARA PRAZO -->
	<tr>
		<td class="SubTituloDireita" widht="190px">
			Prazo:
		</td>
		<td>
			<?=campo_data2('dtprazo', 'N', 'S', '', 'DD/MM/YYYY') ?>
		</td>
	</tr>

	<!-- DATA PARA PRAZO -->
	<tr>
		<td class="SubTituloDireita" widht="190px">
			Responsável:
		</td>
		<td>
			<select multiple="multiple" name="responsavel[]" onDblClick="abrepopupResponsaveis();" id="responsavel" style="width:400px;" >
    	        <option value="">Duplo clique para selecionar da lista</option>
            </select>
		</td>
	</tr>

	<tr bgcolor="#DCDCDC">
		<td></td>
		<td>
			<input type="button" value="Definir Prazo" style="cursor:pointer" onclick="validaForm();">
			<input type="button" value="Voltar" style="cursor:pointer" onclick="history.back(-1);">
			<input type="hidden" id="req" name="req" value=""></input>						
		</td>
	</tr>
</table>
</form>
<script>
function validaForm(){

	if( $('#dtprazo').val() == '' ){
		alert('Campo de data é Obrigatório!');
		$('#prazo').focus();
		return false;
	}

	if( $('#responsavel').val() == '' ){
		alert('Campo de responsável é Obrigatório!');
		$('#responsavel').focus();
		return false;
	}

	$('#req').val('atualizarPrazo');

	$('#formulario').submit();
}


/**
 * Abre popup com responsaveis para selecionar
 */
function abrepopupResponsaveis(){
	window.open('/conjur/prazoRespostaExternaResponsaveis.php','Advogados','width=513,height=500,scrollbars=1');
}


</script>