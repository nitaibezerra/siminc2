<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    echo "<br>";
    monta_titulo('Gestão Pessoas - Avaliação de Servidores', 'Justificativa de Alteração');
    echo "<br>";

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">
    
    function salvarHistoricoAlteracaoFeche(){
        var hmemotivo = $('#hmemotivo');
        var tmeid = $('#tmeid');
        var erro;

        if(!tmeid.val()){
            alert('O campo "Motivo" é um campo obrigatório!');
            tmeid.focus();
            erro = 1;
            return false;
        }
        if(!hmemotivo.val()){
            alert('O campo "Justificativa" é um campo obrigatório!');
            hmemotivo.focus();
            erro = 1;
            return false;
        }
       
        if(!erro){
            $('#requisicao').val('salvarHistoricoAlteracaoFeche');
            $('#tipo_acao').val();
            $('#formulario').submit();
        }
    }

    function fecharJanela(){
        window.close();
    }
    
</script>
    

<form action="" method="POST" id="formulario" name="formulario">

<input type="hidden" id="requisicao" name="requisicao" value=""/>
<input type="hidden" id="sercpf" name="sercpf" value="<?= $_GET['sercpf']; ?>"/>
<input type="hidden" id="tipo_acao" name="tipo_acao" value="<?= $_GET['tipo_acao']; ?>"/>
<input type="hidden" id="tipo_serv" name="tipo_serv" value="<?= $_GET['tipo_serv']; ?>"/>
    
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita" width="20%">Responsável:</td>
            <td>
                <?PHP
                    $sernome = $_SESSION['usunome'];
                    echo campo_texto('sernome', 'N', 'N', '', 45, 200, '', '', '', '', 0, 'id="sernome"', '', $sernome, null, null); 
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="20%">Motivo:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  tmeid AS codigo,
                                tmedsc AS descricao
                        FROM gestaopessoa.tipomanutencaoequipe
                        ORDER BY descricao
                    ";
                    $db->monta_combo("tmeid", $sql, 'S', 'Selecione...', '', '', '', '500', 'S', 'tmeid', false, $tmeid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Justificativa:</td>
            <td>
                <?PHP
                    echo campo_textarea('hmemotivo', 'S', 'S', '', 100, 10, '255', '', 0, '', false, null); 
                ?>
            </td>
        </tr>        
        <tr>
            <td class="SubTituloCentro" colspan="2"> 
                <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarHistoricoAlteracaoFeche();"/>
                <input type="button" id="cancelar" name="cancelar" value="Cancelar" onclick="fecharJanela();"/>
            </td>
        </tr>
    </table>
</form>