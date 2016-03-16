<?php

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }

    $prgid = $_REQUEST['prgid'];
    
    echo "<br>";
    monta_titulo('Instrumento de Avaliação - Medicina em IES Privada', '<b>Listagem dos Arquivos Anexos</b>');
    echo "<br>";

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_desc_apectos_gerais&acao=A';
    $parametros     = '';

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

    function dowloadDocAnexo( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadDocAnexo');
        $('#formulario').submit();
    }

    function excluirDocAnexo(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirDocAnexo');
            $('#formulario').submit();
        }
    }

</script>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="prgid" name="prgid" value="<?=$prgid;?>"/>
    <input type="hidden" id="arqid" name="arqid" value=""/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class ="SubTituloCentro" colspan="4"> Listagem dos Arquivos Anexos. </td>
        </t>
        <tr>
            <td colspan="2">
                <?PHP
                    $acao = "
                        <img border=\"0\" title=\"Apagar arquivo\" onclick=\"excluirDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
                        <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                    ";

                    $desativado = "
                        <img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" />
                        <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                    ";

                    $down = "<a title=\"Baixar arquivo\" href=\"#\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\">' || arq.arqnome || '</a>";

                    $sql = "
                        SELECT  CASE WHEN anx.usucpf = '{$_SESSION['usucpf']}'
                                    THEN '{$acao}'
                                    ELSE '{$desativado}'
                                END as acao,
                                '{$down}' as descricao,
                                t.tpadsc,
                                arq.arqnome||'.'||arq.arqextensao,
                                su.usunome,
                                to_char(aqrdtinclusao, 'DD/MM/YYYY') as aqddtinclusao
                        FROM maismedicomec.arquivoresposta anx
                        
                        LEFT JOIN maismedicomec.tipoarquivo t on t.tpaid = anx.tpaid
                        
                        JOIN maismedicomec.resposta r ON r.rptid = anx.rptid
                        JOIN public.arquivo arq on arq.arqid = anx.arqid
                        JOIN seguranca.usuario su ON (su.usucpf = anx.usucpf)
                        
                        WHERE anx.arqstatus = 'A' AND etqid = {$_SESSION['maismedicomec']['etqid']} AND qstid = {$_SESSION['maismedicomec']['qstid']} AND r.prgid = {$prgid}
                    ";
                    $cabecalho = Array("Ação", "Descrição",  "Tipo de Documento", "Nome do arquivo", "Usuário Responsável", "Data da Inclusão");
                    //$whidth = Array('20%', '60%', '20%');
                    $align  = Array('center', 'left', 'left', 'left', 'right');

                    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', NULL, $whidth, $align, '');
                ?>
            </td>
        </tr>   
    </table>
</form>