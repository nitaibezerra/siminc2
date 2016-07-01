<?php
    $cprid = $_SESSION['gestao']['cprid'];  
    
    if( $cprid != ''){
        $habilita = 'S';
        $disabled = '';
        $hab_botao= 'S';
        $hab_cabecalho = 'S';
    }else{
        $habilita = 'N';
        $disabled = 'disabled="disabled"';
        $hab_botao= 'N';
    }
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }    
    
    if ($_REQUEST['request'] == 'download') {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        
        if ($_REQUEST['arqid']){
            $file = new FilesSimec("arqcessao", $campos, "gestaopessoa");
            $file->getDownloadArquivo($_REQUEST['arqid']);
        }
    }    
   
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Getão de Pessoas', '<b>CESSÃO - Cadastro de Documentos</b>');
    
    if( $_SESSION['gestao']['Aba'] == 'CESSAO' ){
        $id_aba = ABA_CAD_CESSAO_SERVIDOR;
    }elseif( $_SESSION['gestao']['Aba'] == 'PRORROGACAO' ){
        $id_aba = ABA_CAD_PRORROGACAO_SERVIDOR;
    }elseif( $_SESSION['gestao']['Aba'] == 'CONSULTA' ){
        $id_aba = ABA_CAD_CONSULTA_SERVIDOR;
    }elseif( $_SESSION['gestao']['Aba'] == 'RETIFICACAO' ){
        $id_aba = ABA_CAD_REITERACAO_SERVIDOR;
    }
    
    $abacod_tela    = $id_aba;
    $url            = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_documentos&acao=A';
    $parametros     = '&cprid='.$cprid;

    if($hab_cabecalho  == 'S'){
        cabecalhoAbas($cprid);
    }else{
        echo "<br>";
    }

    $db->cria_aba($abacod_tela, $url, $parametros);
?>
<!--
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
-->
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    $(document).ready(function(){
        $('#anexarDocumentos').click(function(){
            var arquivo     = $('#arquivo');
            var aqctipodoc  = $('#aqctipodoc');
            var aqcdsc      = $('#aqcdsc');            
            var erro;
            
            if(!arquivo.val()){
                alert('O campo "Arquivo" é um campo obrigatório!');
                arquivo.focus();
                erro = 1;
                return false;
            }
            if(!aqctipodoc.val()){
                alert('O campo "Tipo de Documento" é um campo obrigatório!');
                aqctipodoc.focus();
                erro = 1;
                return false;
            }
            if(!aqcdsc.val()){
                alert('O campo "Descrição" é um campo obrigatório!');
                aqcdsc.focus();
                erro = 1;
                return false;
            }
            
            if(!erro){
                $('#requisicao').val('anexarDocumentos');
                $('#formulario').submit();
            }
        });
        
        $('#anexarContinuar').click(function(){
            var arquivo     = $('#arquivo');
            var aqctipodoc  = $('#aqctipodoc');
            var aqcdsc      = $('#aqcdsc');            
            var erro;
            
            if(!arquivo.val()){
                alert('O campo "Arquivo" é um campo obrigatório!');
                arquivo.focus();
                erro = 1;
                return false;
            }
            if(!aqctipodoc.val()){
                alert('O campo "Tipo de Documento" é um campo obrigatório!');
                aqctipodoc.focus();
                erro = 1;
                return false;
            }
            if(!aqcdsc.val()){
                alert('O campo "Descrição" é um campo obrigatório!');
                aqcdsc.focus();
                erro = 1;
                return false;
            }
            
            if(!erro){
                $('#continuar').val('S');
                $('#requisicao').val('anexarDocumentos');
                $('#formulario').submit();
            }
        });
    });

    function excluirDocAnexo(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirDocAnexo');
            $('#formulario').submit();
        }
    }

    function voltarListaPessoa(){
        window.location.href = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_grid_cessao&acao=A';
    }
    
</script>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    
<input type="hidden" id="requisicao" name="requisicao" value=""/>
<input type="hidden" id="aba" name="aba" value="<?= $_POST['aba']; ?>"/>
<input type="hidden" id="cprid" name="cprid" value="<?= $cprid; ?>"/>
<input type="hidden" id="arqid" name="arqid" value=""/>
<input type="hidden" id="continuar" name="continuar" value=""/>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
    <tr>
        <td class="SubTituloDireita" width="25%">Arquivo:</td>
        <td colspan="2">
            <input type="file" name="arquivo" id="arquivo" <?=$disabled?>/>
        </td>
        <!-- COLUNA DO WORKFLOW -->
        <td rowspan="4" align="right">
            <?php
                require_once APPRAIZ . "includes/workflow.php";

                if($cprid != ''){
                    $docid = buscarDocidCessaoProrrogacao( $cprid );

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
                    SELECT  tpdid AS codigo, 
                            tpddsc AS descricao
                    FROM gestaopessoa.tipodocumento
                    WHERE tpdstatus = 'A'
                ";
                $db->monta_combo("aqctipodoc", $sql, $habilita, 'Selecione...', '', '', '', 280, 'S', 'aqctipodoc', false, $aqctipodoc, null);
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Descrição:</td>
        <td colspan="2">
            <?php 
                echo campo_textarea('aqcdsc', 'S', $habilita, '', 56, 5, 40, '', 0, '', false, null, $aqcdsc); 
            ?>
        </td>
    </tr>
    <?php if( $hab_botao == 'S' ){ ?>
    <tr>
        <td class="SubTituloCentro" colspan="2" > 
            <input type="button" name="anexarDocumentos" id="anexarDocumentos" value="Anexar" />
            <input type="button" name="voltar" id="valtar" value="Voltar" onclick="voltarListaPessoa();"/>
            <!--
            <input type="button" name="anexarContinuar" id="anexarContinuar" value="Anexar e Continuar" />
            -->
        </td>
    </tr>
    
    <?  
        $acao = "
            <img border=\"0\" onclick=\"excluirDocAnexo('|| anx.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
            &nbsp;&nbsp;
            <a title=\"Download\" href=\"gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_documentos&acao=A&request=download&arqid=' || anx.arqid || '\">
                <img border=\"0\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
            </a>
        ";
        
        $down = "<a title=\"Download\" href=\"gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_documentos&acao=A&request=download&arqid=' || anx.arqid || '\">' || anx.aqcdsc || '</a>";
        
        $sql = "
            SELECT  '{$acao}' as acao,
                    '{$down}' as descricao,
                    tpddsc,
                    arq.arqnome||'.'||arq.arqextensao,
                    to_char(aqcdtinclusao, 'DD/MM/YYYY') as aqcdtinclusao
            FROM gestaopessoa.arqcessao anx
            JOIN gestaopessoa.tipodocumento td on td.tpdid = anx.tpdid
            JOIN public.arquivo arq on arq.arqid = anx.arqid
            WHERE aqcstatus = 'A' AND cprid = {$cprid}
        ";
            
        $cabecalho = Array("Ação", "Descrição",  "Tipo de Documento", "Nome do arquivo", "Data da Inclusão");
        //$whidth = Array('20%', '60%', '20%');
        //$align  = Array('left', 'left', 'center');	
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
    
        }
    ?>
</table> 
</form>
