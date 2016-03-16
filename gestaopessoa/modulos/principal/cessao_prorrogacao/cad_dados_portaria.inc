<?php
    $cprid = $_SESSION['gestao']['cprid'];
    
    if($cprid != ''){
        $sql = "
            SELECT arqid FROM gestaopessoa.docportaria WHERE cprid = {$cprid} AND dpcstatus = 'A';
        ";
        $existeArqid = $db->pegaUm($sql);
    }
    
    if( $cprid != '' ){
        $dadosPort  = buscarDadosPortaria( $cprid );
        $dpcid      = $dadosPort['dpcid'];
        
        $habilita   = 'S';
        $hab_botao  = 'S';
        $disabled   = '';
        
        $hab_cabecalho = 'S';
    }else{
        $habilita   = 'N';
        $disabled   = 'disabled="disabled"';
        $hab_botao  = 'N';
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
    monta_titulo('Getão de Pessoas', '<b>CESSÃO - Cadastro de Portaria</b>');
    
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
    $url            = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_portaria&acao=A';
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
        $('#anexarPortaria').click(function(){
            var arquivo = $('#arquivo');
            var dpcnumero       = $('#dpcnumero');
            var dpcdsc          = $('#dpcdsc');
            var dpcdtportaria   = $('#dpcdtportaria');             
            var erro;
             
            if(!arquivo.val()){
                alert('O campo "Arquivo" é um campo obrigatório!');
                arquivo.focus();
                erro = 1;
                return false;
            }                        
            if(!dpcnumero.val()){
                alert('O campo "Número da Portaria" é um campo obrigatório!');
                arquivo.focus();
                erro = 1;
                return false;
            }
            if(!dpcdsc.val()){
                alert('O campo "Descrição" é um campo obrigatório!');
                aqctipodoc.focus();
                erro = 1;
                return false;
            }
            if(!dpcdtportaria.val()){
                alert('O campo "Data da Portaria" é um campo obrigatório!');
                aqcdsc.focus();
                erro = 1;
                return false;
            }       
            if(!erro){
                $('#requisicao').val('anexarPortaria');
                $('#formulario').submit();
            }
        });
        
        $('#atualizarPortaria').click(function(){
            var dpcnumero       = $('#dpcnumero');
            var dpcdsc          = $('#dpcdsc');
            var dpcdtportaria   = $('#dpcdtportaria');             
            var erro;
             
            if(!dpcnumero.val()){
                alert('O campo "Número da Portaria" é um campo obrigatório!');
                arquivo.focus();
                erro = 1;
                return false;
            }
            if(!dpcdsc.val()){
                alert('O campo "Descrição" é um campo obrigatório!');
                aqctipodoc.focus();
                erro = 1;
                return false;
            }
            if(!dpcdtportaria.val()){
                alert('O campo "Data da Portaria" é um campo obrigatório!');
                aqcdsc.focus();
                erro = 1;
                return false;
            }       
            if(!erro){
                $('#requisicao').val('atualizarPortaria');
                $('#formulario').submit();
            }
        });
    });

    function excluirPortariaAnexo(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirPortariaAnexo');
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
<input type="hidden" id="dpcid" name="dpcid" value="<?= $dpcid; ?>"/>
<input type="hidden" id="arqid" name="arqid" value=""/>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
    <tr>
        <td class ="SubTituloDireita" width="25%">Número de Portaria:</td>
        <td colspan="2" width="70%">
            <?php
            $dpcnumero = $dadosPort['dpcnumero'];
            echo campo_texto('dpcnumero', 'S', $habilita, '', 43, 10, '', '', '', '', 0, 'id="dpcnumero"', '', $dpcnumero, null, null);
            ?>
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
        <td class ="SubTituloDireita">Descrição:</td>
        <td colspan="2">
            <?php
                $dpcdsc = $dadosPort['dpcdsc'];
                echo campo_textarea('dpcdsc', 'S', $habilita, '', 65, 2, '100', '', 0, '', false, null, $dpcdsc); 
            ?>
        </td>
    </tr>
    <tr>
        <td class ="SubTituloDireita">Data da Portaria:</td>
        <td colspan="2">
            <?php
                $dpcdtportaria = $dadosPort['dpcdtportaria'];
                echo campo_data2('dpcdtportaria', 'S', $habilita, '', 'DD/MM/YYYY', '', '', $dpcdtportaria, '', '', 'dpcdtportaria');
            ?>
        </td>
    </tr>
    <?php if($existeArqid == ''){ ?>
    <tr>
        <td class="SubTituloDireita" width="30%">Arquivo:</td>
        <td>
            <input type="file" name="arquivo" id="arquivo" <?= $disabled ?>/>
        </td>
    </tr>
    
    <?php } if( $hab_botao == 'S' ){ ?>
    <tr>
        <td class="SubTituloCentro" colspan="3">
            <?php if($existeArqid != ''){ ?>
            <input type="button" id="atualizarPortaria" name="atualizarPortaria" value=" Salvar "/>
            <?php } ?>
            <?php if($existeArqid == ''){ ?>
                <input type="button" name="anexarPortaria" id="anexarPortaria" value=" Anexar " />
            <?php } ?>
            <input type="button" name="voltar" id="valtar" value="Voltar" onclick="voltarListaPessoa();"/>
        </td>
    </tr>
    <? } ?>
    </table>

<?php 

    if( $hab_botao == 'S' ){

    $acao = "
        <img border=\"0\" onclick=\"excluirPortariaAnexo('|| an.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
        &nbsp;&nbsp;
        <a title=\"Download\" href=\"gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_portaria&acao=A&request=download&arqid=' || an.arqid || '\">
            <img border=\"0\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
        </a>
    ";

    $down = "<a title=\"Download\" href=\"gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_documentos&acao=A&request=download&arqid=' || an.arqid || '\">' || an.dpcdsc || '</a>";

    $sql = "
        SELECT  '{$acao}' as acao,
                '{$down}' as descricao,
                arq.arqnome||'.'||arq.arqextensao as arqnome,
                to_char(dpcdtinclusao, 'DD/MM/YYYY') as dpcdtinclusao
        FROM gestaopessoa.docportaria an
        JOIN public.arquivo arq on arq.arqid = an.arqid
        WHERE dpcstatus = 'A' AND cprid = {$cprid}
    ";

    $cabecalho = Array("Ação", "Descrição", "Nome do arquivo", "Data da Inclusão");
    $whidth = Array('20%', '60%', '20%');
    //$align  = Array('left', 'left', 'center');	
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');

    }
?>

