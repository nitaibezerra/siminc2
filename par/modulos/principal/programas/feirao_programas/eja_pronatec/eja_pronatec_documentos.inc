<?php
    include_once '_funcoes_eja_pronatec.php';

    if( !$_SESSION['par']['inuid'] ){
    	echo "
            <script>
                alert('Sessão expirada.');
                window.location.href = 'par.php?modulo=inicio&acao=C';
            </script>";
    	die();
    }

    if( !$_SESSION['par']['pfaid'] ){
    	echo "
            <script>
                alert('Sessão expirada.');
                window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
            </script>";
    	die();
    }

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }

    if( $_SESSION['par']['adpid'] == ''){
        $_SESSION['par']['adpid'] = $db->pegaUm( "SELECT MAX(adpid) FROM par.pfadesaoprograma WHERE pfaid = {$_SESSION['par']['pfaid']} AND inuid = {$_SESSION['par']['inuid']}" );
    }

    if( $_SESSION['par']['adpid'] == '' || $_SESSION['par']['prgid'] == '' ){
        $db->sucesso( 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa', '', 'Ocorreu um ploblema. Selecione o programa novamente!' );
        die();
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br>';
    $sql = " SELECT prgdsc FROM par.programa WHERE prgid = ".$_SESSION['par']['prgid'];
    $nomePrograma = $db->pegaUm($sql);

    #PEGA, AVERIGUA O MUNICÍPIO OU ESTADO SELECIONADO OU LOGADO PELO USUÁRIO. É USADO PARA CRIAR O TITULO.
    if($_SESSION['par']['muncod']){
        $sql = "SELECT mundescricao, estuf FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'";
        $rsMunicipio = $db->pegaLinha($sql);
        $descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
    }else{
        $sql = "SELECT estdescricao, estuf FROM territorios.estado WHERE estuf = '{$_SESSION['par']['estuf']}'";
        $rsEstado = $db->pegaLinha($sql);
        $descricao = $rsEstado['estdescricao'].' - '.$rsEstado['estuf'];
    }

    monta_titulo($nomePrograma." - Anexar Documentos", $descricao);
    echo '<br>';

    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_documentos&acao=A');

    $dadosQuestPronatec = buscaDadosQuestPronatec ( $_SESSION['par']['inuid'] );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    $(document).ready(function(){
        $('#anexarDocumentos').click(function(){
            var arquivo     = $('#arquivo');
            var erro;
            
            if(!arquivo.val()){
                alert('O campo "Arquivo" é um campo obrigatório!');
                arquivo.focus();
                erro = 1;
                return false;
            }
            
            if(!erro){
                $('#requisicao').val('anexarDocumentos');
                $('#formulario').submit();
            }
        });
        
    });

    function donwloadDocAnexo(arqid){
        $('#arqid').val(arqid);
        $('#requisicao').val('donwloadDocAnexo');
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
    <input type="hidden" id="arqid" name="arqid" value=""/>
    <input type="hidden" id="tpaid" name="tpaid" value="26"/>


    <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td>
                <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
                    <tr>
                        <td class="SubTituloDireita" width="25%">Arquivo:</td>
                        <td colspan="2">
                            <input type="file" name="arquivo" id="arquivo" <?=$disabled?>/>
                        </td>
                    </tr>
                    <tr> 
                        <td class ="SubTituloDireita">Tipo de documento:</td>
                        <td colspan="2">
                            <?php
                                $sql = "
                                    SELECT  tpaid AS codigo, 
                                            tpadsc AS descricao
                                    FROM par.tipoarquivo
                                    WHERE tpaid = 26
                                ";
                                $db->monta_combo("tpaid_dsc", $sql, 'S', '', '', '', '', 400, 'S', 'tpaid_dsc', false, $tpaid_dsc, null);
                            ?>
                        </td>
                    </tr>
                </table>
                <br>
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
                    <tr>
                        <td class="SubTituloCentro" colspan="2" > 
                            <input type="button" name="anexarDocumentos" id="anexarDocumentos" value="Anexar" style="font-weight:bold; width:100px; height: 20px;">
                        </td>
                    </tr>
                </table>
            </td>
            <td width="5%" style="cursor: pointer; margin-left: 25px;">
                <?php
                    include_once APPRAIZ . "includes/workflow.php";

                    $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                    wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                ?>
            </td>
        </tr>
    </table>
    <br>
</form>
    
<?PHP  
    $acaoP = "
        <img border=\"0\" onclick=\"excluirDocAnexo('|| a.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
        <img border=\"0\" onclick=\"donwloadDocAnexo('|| a.arqid ||');\" style=\"cursor: pointer; margin-left:5px;\" align=\"absmiddle\" src=\"../imagens/clipe.gif\" />
    ";
    $acaoS = "
        <img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" />
        <img border=\"0\" onclick=\"donwloadDocAnexo('|| a.arqid ||');\" style=\"cursor: pointer; margin-left:5px;\" align=\"absmiddle\" src=\"../imagens/clipe.gif\" />
    ";

    $down   = "<a title=\"Download\" href=\"#\" onclick=\"donwloadDocAnexo('|| a.arqid ||');\">' || 'Termo EJA - PRONATEC' || '</a>";

    $adpid  = $_SESSION['par']['adpid'];

    $sql = "
        SELECT  CASE WHEN a.usucpf = '{$_SESSION['usucpf']}'
                    THEN '{$acaoP}' 
                    ELSE '{$acaoS}'
                END as acao,
                '{$down}' as descricao,
                tpadsc,
                arq.arqnome||'.'||arq.arqextensao,
                to_char(apedtinclusao, 'DD/MM/YYYY') as apedtinclusao
        FROM eja.arquivoeja a
        JOIN par.tipoarquivo ta on ta.tpaid = a.tpaid
        JOIN public.arquivo arq on arq.arqid = a.arqid
        WHERE apestatus = 'A' AND adpid = {$adpid}
    ";

    $cabecalho = Array("Ação", "Descrição",  "Tipo de Documento", "Nome do arquivo", "Data da Inclusão");
    //$whidth = Array('20%', '60%', '20%');
    $alinhamento  = Array('center', 'left', 'left', 'left', 'left');	
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $alinhamento, '');

?>
