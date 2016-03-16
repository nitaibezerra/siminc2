<?php
    #VERIFICA SE VARIÁVEIS DE SESSÃO FOI INICIADA. cASO NÃO, REDIRECIONA PARA PAGINA INICIAL.
    verificaExisteSessaoMM();
    
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Instrumento de Avaliação',"Serviços de saúde do município, {$_SESSION['maismedicomec']['mundescricao']}. <br> Candidato a sediar curso de medicina em IES privada." );
    echo "<br>";

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/cad_documentos_aval&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    
   function anexarDocumentosAvaliacao(){
        var erro;
        var campos = '';
        
       $.each($("#formulario").find('.obrigatorio'), function(i, v){
            if( $(this).attr('type') != 'radio' ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else{
                var name  = $(this).attr('name');
                var value = $(this).attr('value');
                var radio_box = $('input:radio[name='+name+']:checked');

                if(!radio_box.val()){
                    erro = 1;
                    if(value == 'S'){
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
                }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('anexarDocumentosAvaliacao');
            $('#formulario').submit();
        }
    }

    function dowloadDocAnexo( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadDocAnexo');
        $('#formulario').submit();
    }

    function excluirDocAnexoAvaliacao(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirDocAnexoAvaliacao');
            $('#formulario').submit();
        }
    }
    
</script>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="arqid" name="arqid" value=""/>

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="SubTituloDireita" width="25%">Arquivo:</td>
            <td colspan="2">
                <input type="file" name="arquivo" id="arquivo" class="obrigatorio" title="Arquivo"/>
                <img border="0" style="margin:-16px 0px 8px 312px;" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
            <!-- COLUNA DO WORKFLOW -->
            <td rowspan="4" align="right">
                <?PHP
                    require_once APPRAIZ . "includes/workflow.php";
                    $etqid = $_SESSION['maismedicomec']['etqid'];
                    if($etqid != ''){
                        $docid = buscarDocidAvaliacaoMM( $etqid );

                        if($docid != ''){
                            $dados_wf = array("cprid" => $cprid);
                            wf_desenhaBarraNavegacao($docid, $dados_wf);
                        }
                    }
                ?>
            </td>
            <!-- FIM COLUNA DO WORKFLOW -->
        </tr>
        <tr> 
            <td class ="SubTituloDireita">Tipo de documento:</td>
            <td colspan="2">
                <?php
                    $sql = "
                        SELECT  tpaid AS codigo, 
                                tpadsc AS descricao
                        FROM maismedicomec.tipoarquivo
                        WHERE tpastatus = 'A'
                    ";
                    $db->monta_combo("tpaid", $sql, 'S', 'Selecione...', '', '', '', 280, 'S', 'tpaid', false, $tpaid, 'Tipo de documento');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Descrição:</td>
            <td colspan="2">
                <?php 
                    echo campo_textarea('avadsc', 'S', 'S', '', 56, 5, 40, '', 0, '', false, 'Descrição do documento', $avadsc); 
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="2" > 
                <input type="button" name="anexarDoc" id="anexarDoc" value="Anexar" onclick="anexarDocumentosAvaliacao();"/>
                <!--
                <input type="button" name="anexarContinuar" id="anexarContinuar" value="Anexar e Continuar" />
                -->
            </td>
        </tr>
    </table> 
</form>

<?PHP 
    $qstid = $_SESSION['maismedicomec']['qstid'];
    $etqid = $_SESSION['maismedicomec']['etqid'];
    
    $acao = "<img border=\"0\" title=\"Apagar arquivo\" onclick=\"excluirDocAnexoAvaliacao('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\">&nbsp;<img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\">";

    $desativado = "<img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\">&nbsp;<img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\">";

    $down = "<a title=\"Baixar arquivo\" href=\"#\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\">' || anx.avadsc || '</a>";
   
    $sql = "SELECT  CASE WHEN anx.usucpf = '{$_SESSION['usucpf']}' THEN
                      '{$acao}'
                    ELSE
                      '{$desativado}'
                END as acao,
                '{$down}' as descricao,
                tpadsc,
                arq.arqnome||'.'||arq.arqextensao,
                to_char(avadtinclusao, 'DD/MM/YYYY') as avadtinclusao
        FROM maismedicomec.arquivoavaliacao anx
        JOIN maismedicomec.tipoarquivo td on td.tpaid  = anx.tpaid 
        JOIN public.arquivo arq on arq.arqid = anx.arqid
        WHERE avastatus = 'A' AND qstid = {$qstid} AND etqid = {$etqid}";
    //ver($sql,d);
    $cabecalho = Array("Ação", "Descrição", "Tipo de Documento", "Nome do arquivo", "Data da Inclusão");
    //$whidth = Array('20%', '60%', '20%');
    //$align  = Array('left', 'left', 'center');	
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');


?>

