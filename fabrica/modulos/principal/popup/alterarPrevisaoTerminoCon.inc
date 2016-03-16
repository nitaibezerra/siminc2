<?php

require_once APPRAIZ . "fabrica/classes/HistoricoPrevisaoTermino.class.inc";

monta_titulo("Altera&ccedil;&atilde;o de Previs&atilde;o de T&eacute;rmino", "");


$oHistoricoPrevisaoTermino = new HistoricoPrevisaoTermino();


if(isset($_POST['btnSalvar'])){

    if($oHistoricoPrevisaoTermino->salvar($_POST)){

        echo "<script type='text/javascript'>
                alert('Salvo com sucesso.');
                window.close();
                window.opener.location.reload();
             </script>";
        
    }else{

        extract($_POST);

    }

}


?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<script type="text/javascript">

    function validaFormulario(){

        var resultado = false;
        $$('input[type=submit]').invoke('disable');

        if($('prevtermino').getValue().trim() == ''){

            alert('Informe a Previsão de Término');
            $('odsdtprevtermino').focus();

        }else if($('obsdsc').getValue().trim() == ''){
            
            alert('Informe uma justificativa para a alteração.');
            $('obsdsc').focus();
            
        }else{

            resultado = true;

        }

        $$('input[type=submit]').invoke('enable');
        return resultado;

    }


</script>

<form id="formulario" name="formulario" action="" method="post" onSubmit="return validaFormulario()" enctype="multipart/form-data" >
    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr style="display: none">
            <td class="SubTituloDireita" style="width:200px;"> Previs&atilde;o de T&eacute;rmino: </td>
            <td> <?php echo campo_data2('prevtermino', 'S', 'S', 'Previsão de Término', 'S'); ?> </td>
        </tr>
        <tr style="display: none">
            <td class="SubTituloDireita" style="width:200px;"> Justificativa: </td>
            <td> <?php echo campo_textarea('obsdsc', 'S', 'S', $label, 70, 8, 6000) ?> </td>
        </tr>
        <tr style="display: none">
            <th colspan="2" style="text-align: center;">
                <input type="hidden" value="<?php echo $_GET['odsid']; ?>" name="odsid" id="odsid" />
                <input type="hidden" value="<?php echo $_GET['scsid']; ?>" name="scsid" id="scsid" />
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
$oHistoricoPrevisaoTermino->montaGrid($_GET['odsid'], $_GET['scsid']);


?>