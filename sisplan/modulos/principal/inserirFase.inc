<?php
function montaPopup(){
    $fase   = new FaseContratacao();
    $fases  = $fase->recuperarTodos('facid, facnome, facdescricao', array("facstatus = 'A'"), 'facordem');
    $html   = "";
    $script = "";

    if(is_array( $fases )){

        $html .= " <table  width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" id=\"tabelaFases\"> ";
        $html .= " <tr bgcolor=\"#cdcdcd\"> ";
        $html .= " <td colspan=\"2\" > ";
        $html .= " <input type=\"checkbox\" value=\"todos\" name=\"selecionar_todos\" id=\"selecionar_todos\" style=\"cursor:pointer;\"> <label for=\"selecionar_todos\" style=\"cursor:pointer;\"><strong>Selecionar Todos</strong><label> ";
        $html .= " </td> ";
        $html .= " </tr> ";

        foreach ($fases as $key => $dados){
            $facid        = $dados['facid'];
            $facnome      = $dados['facnome'];
            $facdescricao = $dados['facdescricao'];
            $cor          = "#f4f4f4";
            $nome         = "fase_".$facid;

            if ($key % 2){
                $cor = "#e0e0e0";
            }

            if (trim($facdescricao)!=''){
              $title = "onmouseover=\"return escape('".addslashes($facdescricao)."');\"";
            }else{
              $title = "";
            }

            $html .= " <tr bgcolor=\"$cor\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\"> ";
            $html .= " <td colspan=\"2\" $title> ";
            $html .= " <input id=\"".$nome."\" name=\"".$facnome."\" type=\"checkbox\" value=\"" . $facid . "\" style='cursor:pointer;'>" . "<label for='{$nome}'  style='cursor:pointer;'>".$facnome."</label>";
            $html .= " </td> ";
            $html .= " </tr> ";
            $script .= "$('#{$nome}').change(function(){ marcaItem('".addslashes($facnome)."', ".$facid.", '".$nome."'); });";
        };

        $html .= " <tr bgcolor=\"#C0C0C0\"> ";
        $html .= " <td  colspan=\"2\" > ";
        $html .= " <input type=\"button\" name=\"ok\" value=\"Ok\" onclick=\"self.close();\"> ";
        $html .= " </td> ";
        $html .= " </tr> ";
        $html .= " </table> ";
    }else{
        $html = "<h3><center>Não existem fases cadastradas.</center></h3>";
    }

    return array('html' => $html, 'script' => $script);
}

$retorno = montaPopup();
$script  = $retorno['script'];
$html    = $retorno['html'];
?>
<html>
    <head>
        <title>Inserir Fase</title>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
    </head>
    <script type="text/javascript">

        $(function(){
            $('#selecionar_todos').change(function(){
                selecionaTodos();
            });

            <?= $script; ?>

            $('[id^=facid_]', opener.document).each(function(item, elem){
                $('#fase_'+elem.value).attr('checked','checked');
            });
            verificaTodos();
        });

        var id_fases = new Array();

        function selecionaTodos() {
            if($('#selecionar_todos:checked').length > 0){
                $('input[type=checkbox]:not(:checked)').attr('checked','checked').change();
            }else{
                $('input[type=checkbox]:not(#selecionar_todos)').attr('checked','').change();
            }
        }

        function marcaItem(descricao, id, nome) {
            if(document.getElementById(nome).checked == false) {
                window.opener.removeTrById(id);
            }else{
                window.opener.adicionaLinha( id, descricao );
            }

            verificaTodos();
        }

        function verificaTodos(){
            if($('input[type=checkbox]:not(#selecionar_todos)').length
            == $('input[type=checkbox]:not(#selecionar_todos):checked').length ){
                $('#selecionar_todos').attr('checked','checked');
            }else{
                $('#selecionar_todos').attr('checked','');
            }
        }

    </script>
    <body>
        <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
            <tr>
                <td width="100%" align="center">
                    <label class="TituloTela" style="color: #000000;">
                        Adicionar Fases
                    </label>
                </td>
            </tr>
        </table>
        <?= $html; ?>
        <script type="text/javascript">
           $('[name*=facid]', window.opener.document).each(function (i, obj){
                 var facid = $(obj).val();
                 $('#fase_' + facid).attr('checked', true);
               });

           function enviaform(){
                 document.formulario.submit();
           }

        </script>

    <script type="text/javascript" src="../includes/remedial.js"></script>
    <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
    </body>
</html>