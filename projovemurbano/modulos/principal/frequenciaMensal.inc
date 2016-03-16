<?php
// Retorna informações do Polo
$polomunicipio = buscaPolos( $_SESSION['projovemurbano']['pjuid'] );
?>
<link rel="stylesheet" type="text/css" href="./css/geral.css" />
<form name="frmDiarioFrequenciaMensal" id="frmDiarioFrequenciaMensal" method="get">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
            <td class="SubtituloDireita" width="25%">N&uacute;cleo</td>
            <td id="container-nucleo" >
                <?php
                $nucleos = pegarNucleos( $polomunicipio['pmupossuipolo'] );
                if($nucleos == false) {
                    echo 'Não foram encontrados núcleos';
                } else {
                    $db->monta_combo('nucid', $nucleos, 'S', 'Selecione', '', '', '', '', 'S', 'nucid');
                }
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">Turma</td>
            <td id="container-turma">
                <?php
                    $db->monta_combo('turid', array(), 'S', 'Selecione um núcleo', '', '', '', '', 'S', 'turid', '');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%"></td>
            <td id="container-diario">
                <?php
                    $db->monta_combo('perid', array(), 'S', 'Selecione uma turma', '', '', '', '', 'S', 'perid', '');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">&nbsp;</td>
	    <td>
                <input type="button" name="btnVisualizarDiario" id="btnVisualizarDiario" value="Visualizar Diário" />
                <span id="msg"></span>
            </td>
		</tr>
        <tr>
            <td colspan="2">
                <div id="container-diario-frequencia-mensal"></div>
            </td>
        </tr>
    </table>
    
</form>

<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../projovemurbano/js/frequencia_mensal.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" language="javascript" >
    $(document).ready(function(){

        $("#frmDiarioFrequenciaMensal").validate({

            //Define as regras dos campos
            rules:{
                    perid : {required: true},
                    turid : {required: true},
                    nucid : {required: true}
            },
            //Define as mensagens de alerta
            messages:{
                    perid : 'Campo Obrigatório',
                    turid : 'Campo Obrigatório',
                    nucid : 'Campo Obrigatório'
            }
        });

        DiarioFrequenciaMensal.init();
    });
</script>
