<?php
include APPRAIZ ."includes/workflow.php";

monta_titulo("Altera&ccedil;&atilde;o de Previs&atilde;o de T&eacute;rmino", "");


$oHistoricoPrevisaoTermino = new HistoricoPrevisaoTermino();

if($_SERVER['REQUEST_METHOD'] == "POST"){

	if ($_POST['tipoDocumentoPrevisaoTermino']=='SS'){
		$scsid = $_POST['scsid'];
		$sql = "select ansprevinicio from fabrica.analisesolicitacao where scsid = $scsid";
		
		$dataPrevInicio  = new DateTime($db->pegaUm($sql));
		$dataPrevTerminoInformada = DateTimeUtil::retiraMascaraRetornandoObjetoDateTime($_POST['prevtermino']);
		
		if ($dataPrevTerminoInformada < $dataPrevInicio){
			print "<script type='text/javascript'>alert('Data de término informada não poderá ser menor que a data de previsão de início da análise da solicitação.');</script>";
			extract($_POST);
			$prevtermino = $dataPrevTerminoInformada->format("Y-m-d");
		} else {
		    if($oHistoricoPrevisaoTermino->salvar($_POST)){
		        echo "<script type='text/javascript'>
		                alert('Salvo com sucesso.');
		                window.close();
		                window.opener.location.reload();
		             </script>";
		        
		    }else{
		        extract($_POST);
		        $prevtermino = $dataPrevTerminoInformada->format("Y-m-d");
		    }
		}
	}
	
	if ($_POST['tipoDocumentoPrevisaoTermino']=='OSitem1'){
		$odsid = $_POST['odsid'];
		$sql = "select odsdtprevinicio from fabrica.ordemservico where odsid = $odsid";
		
		$dataPrevInicio  = new DateTime($db->pegaUm($sql));
		$dataPrevTerminoInformada = DateTimeUtil::retiraMascaraRetornandoObjetoDateTime($_POST['prevtermino']);
		
		if ($dataPrevTerminoInformada < $dataPrevInicio){
			print "<script type='text/javascript'>alert('Data de término informada não poderá ser menor que a data de previsão de início desta ordem de servico');</script>";
			extract($_POST);
			$prevtermino = $dataPrevTerminoInformada->format("Y-m-d");
		} else {
			if($oHistoricoPrevisaoTermino->salvar($_POST)){
		        echo "<script type='text/javascript'>
		                alert('Salvo com sucesso.');
		                window.close();
		                window.opener.location.reload();
		             </script>";
		        
		    }else{
		        extract($_POST);
		        $prevtermino = $dataPrevTerminoInformada->format("Y-m-d");
		    }
		}
	}
	
	if ($_POST['tipoDocumentoPrevisaoTermino']=='OSitem2'){
		$odsid = $_POST['odsid'];
		$sql = "select odsdtprevinicio from fabrica.ordemservico where odsid = $odsid";
		
		$dataPrevInicio  = new DateTime($db->pegaUm($sql));
		$dataPrevTerminoInformada = DateTimeUtil::retiraMascaraRetornandoObjetoDateTime($_POST['prevtermino']);
		
		
		if ($dataPrevTerminoInformada < $dataPrevInicio){
			print "<script type='text/javascript'>alert('Data de término informada não poderá ser menor que a data de previsão de início desta ordem de servico');</script>";
			extract($_POST);
			$prevtermino = $dataPrevTerminoInformada->format("Y-m-d");
		} else {
		    $retorno = $oHistoricoPrevisaoTermino->alterarPrevisaoTerminoEmpresaItem2( $_POST ); 
			if( $retorno !== false ){
		        echo "<script type='text/javascript'>
		                alert('Salvo com sucesso.');
		                window.opener.location.reload();
		                window.close();
		             </script>";
		        
		    }else{
		        extract($_POST);
				$prevtermino = $dataPrevTerminoInformada->format("Y-m-d");
		    }
		}
	}
}

?>

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />

<form id="formularioAlterarPrevisaoTermino" name="formulario" action="" method="post" enctype="multipart/form-data" >
    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita" style="width:200px;"> Previs&atilde;o de T&eacute;rmino: </td>
            <td> <?php echo campo_data2('prevtermino', 'S', 'S', 'Previsão de Término', 'S'); ?> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width:200px;"> Justificativa: </td>
            <td> <?php echo campo_textarea('obsdsc', 'S', 'S', $label, 70, 8, 6000) ?> </td>
        </tr>
        <tr>
            <th colspan="2" style="text-align: center;">
                <input type="hidden" value="<?php echo $_GET['odsid']; ?>" name="odsid" id="odsid" />
                <input type="hidden" value="<?php echo $_GET['scsid']; ?>" name="scsid" id="scsid" />
                <input type="hidden" value="<?php echo $_GET['alteracao']; ?>" name="tipoDocumentoPrevisaoTermino" id="tipoDocumentoPrevisaoTermino">
                <input type="submit" value="Salvar" name="btnSalvar" id="btnSalvar" />
                <input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="window.close();" />
            </th>
        </tr>
    </table>
</form>
<?php

// titulo da listagem
monta_titulo("Histórico", "");

// retorna todos os dados para a primeira aba
if ($gridMontada == ""){
	if ($_GET['alteracao']=='SS'){
//		var_dump($oHistoricoPrevisaoTermino->montaGridSS($_GET['odsid'], $_GET['scsid']));
		print $oHistoricoPrevisaoTermino->montaGridSS($_GET['odsid'], $_GET['scsid']);
	} else {
//		var_dump($oHistoricoPrevisaoTermino->montaGridOS($_GET['odsid'], $_GET['scsid']));
		print $oHistoricoPrevisaoTermino->montaGridOS($_GET['odsid'], $_GET['scsid']);
	}
}

?>
<script type="text/javascript">
	jQuery("#btnSalvar").click(function(event){
		event.preventDefault();

        if(jQuery.trim(jQuery('#prevtermino').val()) == ''){
            alert('Informe a Previsão de Término');
            jQuery('#odsdtprevtermino').focus();
        } else if(jQuery.trim(jQuery('#obsdsc').val()) == ''){
            alert('Informe uma justificativa para a alteração.');
            jQuery('#obsdsc').focus();
        } else {
        	jQuery('#formularioAlterarPrevisaoTermino').submit();
        }
	});
</script>