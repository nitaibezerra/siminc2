<?php
    header('content-type: text/html; charset=ISO-8859-1');
?>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css"></link>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
    
    <script type="text/javascript">
        
        function pesquisarMantenedora( param ){
            if(trim(param) == 'fil'){
                $('#formulario').submit();
            }else{
                $('#formulario').submit();
            }
        }
        
        function fecharPopup(){
            window.close();
        }
        
        function salvarMantenedora( mntid, mntcnpj, mntdsc){
            var confirma = confirm("Tem certeza que deseja atribuir a Mantenedora selecionada?");
            if( confirma ){
                if(mntid != ''){
                    $(opener.document.getElementById("mntid")).val(mntid);
                    $(opener.document.getElementById("mntcnpj")).val(mntcnpj);
                    $(opener.document.getElementById("mntrazaosocial")).val(mntdsc);
                }
            }
            window.close();
        }
    
    </script>        
        
    <form name="formulario" id="formulario" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td class = "subtitulodireita" colspan="2" style="padding: 5px; text-align: center; font-size: 14px;"> Busca de Mantenedora </td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="25%">Código:</td>
                <td>
                    <?= campo_texto('mntid', 'N', 'S', 'Código', 57, 10, '##########', '', '', '', '', 'id="mntid"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_nome">
                <td class = "subtitulodireita"> Mantenedora: </td>
                <td>
                    <?= campo_texto('mntdsc', 'N', 'S', 'Mantenedora', 57, 50, '', '', '', '', '', 'id="mntdsc"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_cnpj">
                <td class = "subtitulodireita">CNPJ:</td>
                <td>
                    <?= campo_texto('mntcnpj', 'N', 'S', 'CNPJ', 57, 20, '##.###.###/####-##', '', '', '', '', 'id="mntcnpj"', '', ''); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="pesquisar" value="Pesquisar" id="pesquisar" onclick="pesquisarMantenedora('fil');">
                    <input type="button" name="mostar" value="Mostar Tudo" id="mostar" onclick="pesquisarMantenedora('all');">
                    <input type="button" name="fechar" value="Cancelar" id="fechar" onclick="fecharPopup();">
                </td>
            </tr>
        </table>
    </form>
    
<?PHP
    if( $_POST['mntid'] != '' ){
        $mntid = $_POST['mntid'];
        $where[] = " m.mntid = {$mntid} ";
    }
    if( $_POST['mntdsc'] != '' ) {
        $mntdsc = removeAcentos( $_POST['mntdsc'] );
        $where[] = " public.removeacento(m.mntdsc) ILIKE ('%{$mntdsc}%') ";
    }
    if( $_POST['mntcnpj'] != '' ) {
        $mntcnpj = str_replace('.', '', str_replace('/','', str_replace('-', '', $_POST['mntcnpj'])));
        $where[] = " m.mntcnpj = '{$mntcnpj}' ";
    }

    if( $where != '' ){
        $WHERE = 'WHERE' . implode('AND', $where);
    }

    $acao = "<input type=\"radio\" name=\"mntid\" id=\"mntid_'||mntid||'\" value=\"'||mntid||'\" onclick=\"salvarMantenedora( '|| mntid ||',\''|| replace( to_char( cast(m.mntcnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) ||'\',\''|| m.mntdsc ||'\' );\">";
    $sql = "
        SELECT  '{$acao}' as acao,
                m.mntid,
                trim( replace( to_char( cast(m.mntcnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) ) AS mntcnpj,
                m.mntdsc

        FROM gestaodocumentos.mantenedoras AS m

        {$WHERE}

        ORDER BY m.mntdsc
    ";
    $cabecalho = array( "&nbsp", "Código", "CNPJ", "Mantenedora" );
    $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
    $tamanho = Array('4%', '3%', '4%', '45%', '40%', '4%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
?>