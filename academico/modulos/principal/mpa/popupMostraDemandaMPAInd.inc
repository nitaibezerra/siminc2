<?php

    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        die();
    }

    $idaid  = $_REQUEST['idaid'];
    $view   = $_REQUEST['view'];
    
    if($view == 'S'){
        $habilita = 'N';
        $disabled = 'disabled="disabled"';
    }else{
        $habilita = 'S';
        $disabled = '';        
    }
    
    if($idaid != ''){
        $sql = "
            SELECT  imiid,
                    imiiddsc
            FROM academico.medidasindicadores
            WHERE idaid = {$idaid}
        ";
        $dados = $db->pegaLinha($sql);
        $imiid = $dados['imiid'];
    }
    
    #SALVA DADOS DA INDICADORES ACADÊMICOS - MEDIDAS SANEADORAS: NAS RESPECTIVAS TABELAS - academico.medidasindicadores
    function salvarMedidasSaneadoras() {
        global $db;

        #DADOS MEDIDA INDICADORES.
        $imiid      = $_POST['imiid'];
        $idaid      = $_POST['idaid'];
        $imiiddsc   = $_POST['imiiddsc'];
       
        if (idaid != '' && $imiid != '') {
            $sql = "
                UPDATE academico.medidasindicadores 
                    SET imiiddsc    = '{$imiiddsc}'                        
		WHERE imiid = {$imiid} RETURNING idaid;
            ";
        } else {
            $sql = "
                INSERT INTO academico.medidasindicadores( 
                                idaid,
                                imiiddsc, 
                                imiidtinclusao, 
                                imiistatus 
                        ) VALUES (
                                {$idaid}, 
                                '{$imiiddsc}', 
                                'now()', 
                                'A' 
                        )RETURNING idaid;
            ";
        }
        $dados = $db->pegaUm($sql);

        if( $dados > 0 ){
            $db->commit();
            $db->sucesso( 'principal/mpa/popupMostraDemandaMPAInd', '&idaid='.$dados );
        }else{
            $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/mpa/popupMostraDemandaMPAInd&acao=A');
        }
    }
    
    #LISTAGEM NÃO ESTA SENDO USADA: FOI PEDIDO PARA RETIRAR NO DIA: 26/06/2013. PARA TAL, FOI COMENTADO A CHAMADA DA FUNÇÃO.
    function montaListaSugestao(){
        global $db;
        
        header("Content-Type: text/html; charset=ISO-8859-1");
        
        $dinid = $dados['dinid'];
        
        $sql = "
            SELECT  tmsid as codigo,
                    tmsdsc as descricao
            FROM academico.tipomedidasan
            ORDER BY 1;
        ";
        $dados =$db->carregar($sql);
        
        if($dados){
            $titulo .= '<table style="width: 100%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">';
            $titulo .= '<tr><th colspan="2">SUGESTÃO</th></tr>';
            $titulo .= '<tr><th>Cód.</th><th>Descrição</th></tr>';
            $x = 0;						
            foreach($dados as $dado){
                    if( ($x % 2) == 0 ){
                            $cor = "#FFFAFA";
                    }else{
                            $cor = "#D3D3D3";
                    }
                    $titulo .= '<tr style="background-color: '.$cor.';">
                                    <td>'.$dado['codigo'].'</td>
                                    <td>'.$dado['descricao'].'</td>
                                </tr>';
                    $x = $x + 1;
            }
            $titulo .= '</table>';
        }else{
            $titulo = 'Não há informações!';
        }
        echo $titulo;            
    }

    monta_titulo('Indicadores Acadêmicos', 'Medidas Saneadoras');
    echo '<br>';
?>
    
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" ></link>

<script language="JavaScript" type="text/javascript" src="../../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>

<script>
    
    <?PHP if($view != 'S'){ ?>
      /*  $(window).unload(
            function() { 
                this.opener.location.reload(); 
            }
        ); */
    <?PHP } ?>

    function salvarMedidasSaneadoras(){
        var imiiddsc = $('#imiiddsc');
        
        var erro;
        /*
        if(!imiiddsc.val()){
            alert('O campo "Descrição" é um campo obrigatório!');
            imiiddsc.focus();
            erro = 1;
            return false;
        }
       */
        if(!erro){
            $('#requisicao').val('salvarMedidasSaneadoras');
            $('#formulario').submit();
        }
    }
    
    function anexarArquivos(){
        var arquivo         = $('#arquivo');
        var arqdescricao    = $('#arqdescricao');
        var tpaid_arquivo   = $('#tpaid_arquivo');
        
        var erro;

        if(!arquivo.val()){
            alert('É necessario selecionar um arquivo!');
            arquivo.focus();
            erro = 1;
            return false;
        }
        if(!arqdescricao.val()){
            alert('O campo "Descrição do arquivo" é um campo obrigatório!');
            arqdescricao.focus();
            erro = 1;
            return false;
        }
        if(!tpaid_arquivo.val()){
            alert('O campo "Tipo do arquivo" é um campo obrigatório!');
            tpaid_arquivo.focus();
            erro = 1;
            return false;
        }
	
        if(!erro){
            $('#requisicao').val('salvarArquivoMedidas');
            $('#formulario').submit();
        }
    }
        
    function excluirArquivoMedidas(arqid){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $('#requisicao').val('excluirArquivoMedidas');
            $('#arqid').val( arqid );
            $('#formulario').submit();
        }
    }

    function downloadArquivoMedidas( arqid ){
        $('#requisicao').val('downloadArquivoMedidas');
        $('#arqid').val( arqid );
        $('#formulario').submit();
    }

    tinyMCE.init({
        disabled : "disabled",
        elements : "imiiddsc",
        editor_selector : "mceEditor",
	mode : "textareas",
        theme : "advanced",
        plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
        /*theme_advanced_buttons1_add_before : "save,newdocument,separator",*/
        theme_advanced_buttons1_add : "fontselect,fontsizeselect",
        theme_advanced_buttons2_add : "separator,insertdate,inserttime,preview,zoom,separator,forecolor,backcolor",
        theme_advanced_buttons2_add_before: "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator",
        theme_advanced_buttons3_add_before : "tablecontrols,separator",
        /*theme_advanced_buttons3_add : "emotions,iespell,flash,advhr,separator,print,separator,ltr,rtl,separator,fullscreen",*/
        theme_advanced_toolbar_location : "top",
        theme_advanced_toolbar_align : "left",
        theme_advanced_path_location : "bottom",
        plugin_insertdate_dateFormat : "%d-%m-%Y",
        plugin_insertdate_timeFormat : "%H:%M:%S",
        extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
        external_link_list_url : "example_link_list.js",
        external_image_list_url : "example_image_list.js",
        flash_external_list_url : "example_flash_list.js",
        /*file_browser_callback : "fileBrowserCallBack",*/
        language : "pt_br",
	entity_encoding : "raw",
	width : "100%"
    });
    
</script>

<form name="formulario" id="formulario" method="POST" enctype="multipart/form-data">
    <input type="hidden" value="" name="requisicao" id="requisicao" />
    <input type="hidden" value="<?=$idaid ?>" name="idaid" id="idaid" />
    <input type="hidden" value="<?=$imiid ?>" name="imiid" id="imiid" />
    
    <input type="hidden" value="" name="arqid" id="arqid" />
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <th>Descrição</th>
        <tr align="center">
            <td>
                <?PHP
                    $texto = "Descreva abaixo texto com a/as medida/s saneadora/s que a Universidade pretende adotar de forma a melhorar o CPC atual. Exemplos de medidas saneadoras: aquisição de acervo bibliográfico, contratação de docentes, aquisição de equipamentos, construção de laboratórios, construção de salas de aula, etc.";
                    if($dados['imiiddsc'] == ''){
                        $imiidsc = "<span style=\"font-family: sans-serif; font-size: 11px; color: silver; \">{$texto}</span>";
                    }else{
                        $imiidsc = $dados['imiiddsc'];
                    }
                ?>
                
                <textarea class="mceEditor" name="imiiddsc" id="imiiddsc" rows="30"><?= $imiidsc; ?></textarea>
            </td>
        </tr>
        <tr>
            <td>
                <? #montaListaSugestao();?>
            </td>
        </tr>        
        <?PHP if( $habilita == 'S'){ ?>
        <tr>
            <td colspan="5" class="subTituloCentro">
                <input class="salvarDadosDemanda" type="button" value="Salvar" onclick="salvarMedidasSaneadoras();" style="cursor:pointer">
                <input type="button" value="Cancelar" style="cursor:pointer">
            </td>
        </tr>
        <?PHP } ?>
    </table>
    
    <br>
<?PHP if($imiid != ''){ ?>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita">Arquivo:</td>
            <td>
                <input type="file" name="arquivo" id="arquivo" <?=$disabled?> >
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Descrição:</td>
            <td>
                <?= campo_textarea('arqdescricao', 'S', $habilita, '', 80, 3, 250, '', 0, '', false, NULL, $arqdescricao) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Tipo de Arquivo:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  tpaid as codigo,
                                tpadsc as descricao
                        FROM academico.tipoarquivo
                    ";
                    $db->monta_combo('tpaid', $sql, $habilita, 'Selecione', $acao, $opc, '', '200', 'S', 'tpaid_arquivo', false, $tpaid, null);
                ?>
            </td>
        </tr>
        <tr>
            <?PHP if( $habilita == 'S'){ ?>
            <td colspan="2" class="subTituloCentro">
                <input class="anexar" onclick="anexarArquivos();" type="button" value="Anexar" style="cursor:pointer">
            </td>
            <?PHP } ?>
        </tr>
    </table>
</form>

<br>

<?PHP
        monta_titulo('Lista de Arquivos','');

        if($habilita == 'S'){
        $acao = "
            <center>
                <img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer\" onclick=\"excluirArquivoMedidas('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">
                &nbsp;&nbsp;
                <img src=\"../imagens/indicador-vermelha.png\" title=\"Baixar Arquivo (Download)\" style=\"cursor: pointer\" onclick=\"downloadArquivoMedidas('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">            
            </center>
        ";
        }else{
        $acao = "
            <center>
                <img src=\"../imagens/excluir_01.gif\" title=\"Excluir\" >
                &nbsp;&nbsp;
                <img src=\"../imagens/indicador-vermelha.png\" title=\"Baixar Arquivo (Download)\" style=\"cursor: pointer\" onclick=\"downloadArquivoMedidas('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">            
            </center>
        ";            
        }

        $sql = "
            SELECT '{$acao}' as acao,
                    ta.tpadsc as tipo_doc,
                    arqdescricao as descricao,
                    arqnome||'.'||arqextensao as arqnome,
                    arqtamanho as tamanho,
                    to_char(arqdata, 'DD/MM/YYYY') as data
                    
            FROM academico.medidasindicadores m
            JOIN academico.arqmedidaindicadores AS a ON a.imiid = m.imiid
            JOIN academico.tipoarquivo AS ta ON ta.tpaid = a.tpaid
            JOIN public.arquivo AS arq ON arq.arqid = a.arqid
            
            WHERE m.imiid  = {$imiid}
        ";
        $cabecalho = array("<center>Ação</center>", "Tido de Documento", "Descrição Arquivo", "Nome Arquivo", "Tamanho (MB)", "Data Inclusão");
        $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
        $tamanho = Array('5%', '10%', '40%', '20%', '10%', '10%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    }
?>