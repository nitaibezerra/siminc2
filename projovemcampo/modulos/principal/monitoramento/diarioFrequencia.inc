<?php
$perfis = pegaPerfilGeral();
?>

<style type="text/css" >
    #dialogAjax{
        background-color:#ffffff;
        position:absolute;
        color:#000033;
        top:50%;
        left:40%;
        border:2px solid #cccccc;
        width:20%;
        font-size:12px;
        padding: 20px;
        line-height: 20px;
        display: none;
    }
</style>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<form name="frmDiarioFrequencia" id="frmDiarioFrequencia" method="post" action="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
            <td class="SubtituloDireita" width="25%">Escola</td>
            <td id="container-escola" >
                <?php
                $escolas =  pegarEscolas(true);
                if($escolas == false) {
                    echo 'Não foram encontradas escolas';
                } else {
                    $db->monta_combo('entid', $escolas, 'S', 'Selecione', '', '', '', '', 'S', 'entid');
                }
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">Turma</td>
            <td id="container-turma">
                <?php
                $db->monta_combo('turid', array(), 'S', 'Selecione', '', '', '', '', 'S', 'turid', '');
                ?>
            </td>
		</tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Período</td>
			<td id="container-periodo">
                <?php
				$db->monta_combo('perid', array(), 'S', 'Selecione', '', '', '', '', 'S', 'perid');
                ?>
            </td>
		</tr>
		<tr>
            <td class="SubtituloDireita" width="25%">&nbsp;</td>
			<td>
                <input type="button" name="btnVisualizarDiario" id="btnVisualizarDiario" value="Visualizar Diário" />
                &nbsp;
                &nbsp;
                <?php if(in_array( PFL_DIRETOR_ESCOLA, $perfis )||in_array( PFL_SUPER_USUARIO, $perfis )||in_array( PFL_ADMINISTRADOR, $perfis )) { ?>
                <input type="button" name="btnGerarDiario" id="btnGerarDiario" value="Gerar Diário" />
                <?php }?>
                
            </td>
		</tr>
        <tr>
            <td colspan="2">
                <div id="container-diario"></div>
            </td>
        </tr>
    </table>
</form>

<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../projovemcampo/js/diario_frequencia.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>

<script type="text/javascript" language="javascript" >
    $(document).ready(function(){
        
        $("#frmDiarioFrequencia").validate({
            //Define as regras dos campos
            rules:{
                    entid : {required: true},
                    turid : {required: true},
                    perid : {required: true},
                  
              
            },
            //Define as mensagesn de alerta
            messages:{
                    entid : 'Campo Obrigatório',
                    turid : 'Campo Obrigatório',
                    perid : 'Campo Obrigatório'
            }
        });
    
        DiarioFrequencia.init();
    });
    
    

</script>