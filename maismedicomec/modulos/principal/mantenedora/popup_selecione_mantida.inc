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

        function salvarMantida( iesid, iesdsc, iessigla, iescep){
            var cep = iescep.substring(0,5) +'-'+ iescep.substring(5,8);
            var funcao = window.parent.opener;

            var confirma = confirm("Tem certeza que deseja atribuir a Mantenedora selecionada?");
            if( confirma ){
                if(iesid != ''){
                
                	var mtdimovelsit_p = opener.document.getElementById("mtdimovelsit_p").checked;
                	var mtdimovelsit_a = opener.document.getElementById("mtdimovelsit_a").checked;
                	var mtdimovelsit_O = opener.document.getElementById("mtdimovelsit_O").checked;
                	var mnmprioridade = opener.document.getElementById("mnmprioridade").value;
                	var mantida_muncod = opener.document.getElementById("mantida_muncod").value;
                
                    funcao.resetFromulario();
                    
                    $(opener.document.getElementById("mtdid")).attr('readonly', true);

                    $(opener.document.getElementById("mtdid")).val(iesid);
                    $(opener.document.getElementById("mtddsc")).val(iesdsc);
                    $(opener.document.getElementById("mntsigla")).val(iessigla);
                    $(opener.document.getElementById("mtdcep")).val(cep);
                    
                    funcao.buscarEndereceCEP( cep );
                    
                    
                    opener.document.getElementById("mtdimovelsit_p").checked = mtdimovelsit_p;
                	opener.document.getElementById("mtdimovelsit_a").checked = mtdimovelsit_a;
                	opener.document.getElementById("mtdimovelsit_O").checked = mtdimovelsit_O;
                	opener.document.getElementById("mnmprioridade").value = mnmprioridade;
                	opener.document.getElementById("mantida_muncod").value = mantida_muncod;
                   
                }
            }
            window.close();
        }

    </script>

    <form name="formulario" id="formulario" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td class = "subtitulodireita" colspan="2" style="padding: 5px; text-align: center; font-size: 14px;"> Busca de Mantida </td>
            </tr>
            <tr>
                <td class = "subtitulodireita" width="25%">Código:</td>
                <td>
                    <?= campo_texto('mntid', 'N', 'S', 'Código', 57, 10, '##########', '', '', '', '', 'id="mntid"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_nome">
                <td class = "subtitulodireita"> Instituição: </td>
                <td>
                    <?= campo_texto('mntdsc', 'N', 'S', 'Mantenedora', 57, 50, '', '', '', '', '', 'id="mntdsc"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_sigla">
                <td class = "subtitulodireita">Sigla:</td>
                <td>
                    <?= campo_texto('mntsigla', 'N', 'S', 'Sigla', 57, 20, '', '', '', '', '', 'id="mntsigla"', '', ''); ?>
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
    $mntid = $_SESSION['maismedicomec']['mntid'];

    if( $_POST['mntid'] != '' ){
        $mntid = $_POST['mntid'];
        $where[] = " i.iesid = {$mntid} ";
    }
    if( $_POST['mntdsc'] != '' ) {
        $mntdsc = removeAcentos( $_POST['mntdsc'] );
        $where[] = " public.removeacento(i.iesdsc) ILIKE ('%{$mntdsc}%') ";
    }
	if( $_POST['mntsigla'] != '' ) {
        $mntsigla = removeAcentos( $_POST['mntsigla'] );
        $where[] = " public.removeacento(i.iessigla) ILIKE ('%{$mntsigla}%') ";
    }
    if( $_POST['mntcnpj'] != '' ) {
        $mntcnpj = str_replace('.', '', str_replace('/','', str_replace('-', '', $_POST['mntcnpj'])));
        $where[] = " i.iescnpj = '{$mntcnpj}' ";
    }

    if( $where != '' ){
        $WHERE = 'AND' . implode('AND', $where);
    }

    $acao = "<input type=\"radio\" name=\"iesid\" id=\"iesid_'||iesid||'\" value=\"'||iesid||'\" onclick=\"salvarMantida( '|| i.iesid ||',\''|| i.iesdsc ||'\',\''|| i.iessigla ||'\',\''|| i.iescep ||'\' );\">";
    $sql = "
        SELECT  '{$acao}' as acao,
                i.iesid,
                i.iesdsc,
                i.iessigla,
                trim( replace( to_char( cast(i.iescnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) ) AS iescnpj

        FROM gestaodocumentos.instituicaoensino AS i

        WHERE mntid = {$_SESSION['maismedicomec']['mntid']} {$WHERE}

        ORDER BY i.iesdsc
    ";
    $cabecalho = array( "&nbsp", "Código", "Instituição", "Sigla", "CNPJ" );
    $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
    $tamanho = Array('3%', '4%', '25%', '10%', '10%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
?>