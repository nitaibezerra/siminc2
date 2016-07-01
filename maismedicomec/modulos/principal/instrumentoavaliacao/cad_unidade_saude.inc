<?PHP
    $tusid = $_REQUEST['tusid'];

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    //include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Instrumento de Avaliação - Medicina em IES Privada', '<b>Cadastro da Unidade de Saúde</b>');
    echo "<br>";

?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">
    $(document).ready(function(){   
        
        jQuery('#salvarUnidadeServico').click(function(){
            var tusid   = $('#tusid');
            var ussdsc  = $('#ussdsc');
            var usscnes = $('#usscnes');

            var erro;

            if(!tusid.val()){
                alert('O campo "Tipo de Unidade de Serviço" é um campo obrigatório!');
                tusid.focus();
                erro = 1;
                return false;
            }
            if(!ussdsc.val()){
                alert('O campo "Nome da Unidade de Saúde" é um campo obrigatório!');
                ussdsc.focus();
                erro = 1;
                return false;
            }
            if(!usscnes.val()){
                alert('O campo "CNES nº" é um campo obrigatório!');
                usscnes.focus();
                erro = 1;
                return false;
            }

            if(!erro){
                $('#requisicao').val('salvarUnidadeServico');
                $('#formulario').submit();
            }
        });
    });

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class ="SubTituloDireita">Tipo de Unidade de Serviço:</td>
            <td id="td_combo_municipio">
                <?PHP
                    $sql = "
                        SELECT  tusid AS codigo,
                                tusdsc AS descricao
                        FROM maismedicomec.tipounidadeservico
                        WHERE tusid = {$tusid}
                        ORDER BY descricao
                    ";
                    $db->monta_combo("tusid", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'tusid', false, $tusid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%">Nome:</td>
            <td>
                <?= campo_texto('ussdsc', 'N', 'S', '', 33, 100, '', '', '', '', 0, 'id="ussdsc"', '', $ussdsc, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%">CNES nº:</td>
            <td>
                <?= campo_texto('usscnes', 'N', 'S', '', 33, 10, '##########', '', '', '', 0, 'id="usscnes"', '', $usscnes, null, '', null); ?>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                <input type="button" id="salvarUnidadeServico" name="salvarUnidadeServico" value="Salvar" />
                
                <input type="button" id="cancelar" name="cancelar" value="Cancelar" onclick="cancelar('C');"/>
            </td>
        </tr>
    </table>
</form>