<?PHP

    $usunome= $_SESSION['usunome'];
    $cprid  = $_SESSION['gestao']['cprid'];
    
    $perm = permissaoBotaoSalvarParecer();
        
    if($cprid != ''){
        $docid = buscarDocidCessaoProrrogacao( $cprid );
        if($docid != ''){
            $estadoAtual = pegaEstadoAtualWorkflowCessao($docid); 
        }
    }
    
    if($cprid != ''){
        $sql = "
            SELECT arqid FROM gestaopessoa.docportaria WHERE cprid = {$cprid} AND dpcstatus = 'A';
        ";
        $existeArqid = $db->pegaUm($sql);
    }
    
    if( $cprid != '' ){
        $dadosParecer  = buscarDadosParecer( $cprid );

        $nome_sitmovimentacao  = $dadosParecer['nome_sitmovimentacao'] == '' ? $_SESSION['usunome'] : $dadosParecer['nome_sitmovimentacao']; 
        $nome_sitcap           = $dadosParecer['nome_sitcap'] == '' ? $_SESSION['usunome'] : $dadosParecer['nome_sitcap']; 
        $nome_sitcggp          = $dadosParecer['nome_sitcggp']  == '' ? $_SESSION['usunome'] : $dadosParecer['nome_sitcggp']; 
        $nome_sitadvogado      = $dadosParecer['nome_sitadvogado']  == '' ? $_SESSION['usunome'] : $dadosParecer['nome_sitadvogado']; 
        $nome_sitcoordenador   = $dadosParecer['nome_sitcoordenador']  == '' ? $_SESSION['usunome'] : $dadosParecer['nome_sitcoordenador']; 
        
        $usucpfsitmovimentacao  = $dadosParecer['usucpfsitmovimentacao'] == '' ? $_SESSION['usucpf'] : $dadosParecer['usucpfsitmovimentacao']; 
        $usucpfsitcap           = $dadosParecer['usucpfsitcap'] == '' ? $_SESSION['usucpf'] : $dadosParecer['usucpfsitcap']; 
        $usucpfsitcggp          = $dadosParecer['usucpfsitcggp']  == '' ? $_SESSION['usucpf'] : $dadosParecer['usucpfsitcggp']; 
        $usucpfsitadvogado      = $dadosParecer['usucpfsitadvogado']  == '' ? $_SESSION['usucpf'] : $dadosParecer['usucpfsitadvogado']; 
        $usucpfsitcoordenador   = $dadosParecer['usucpfsitcoordenador']  == '' ? $_SESSION['usucpf'] : $dadosParecer['usucpfsitcoordenador']; 
        
        if($dadosParecer['cprsitmovimento'] == 't'){
            $checkSim_mov = 'checked = "checked"';
        }elseif($dadosParecer['cprsitmovimento'] == 'f'){
            $checkNao_mov = 'checked = "checked"';
        }
        
        if($dadosParecer['cprsitcap'] == 't'){
            $checkSim_cap = 'checked = "checked"';
        }elseif($dadosParecer['cprsitcap'] == 'f'){
            $checkNao_cap = 'checked = "checked"';
        }
        
        if($dadosParecer['cprsitcggp'] == 't'){
            $checkSim_cggp = 'checked = "checked"';
        }elseif($dadosParecer['cprsitcggp'] == 'f'){
            $checkNao_cggp = 'checked = "checked"';
        }
        
        if($dadosParecer['cprsitadvogado'] == 't'){
            $checkSim_adv = 'checked = "checked"';
        }elseif($dadosParecer['cprsitadvogado'] == 'f'){
            $checkNao_adv = 'checked = "checked"';
        }
        
        if($dadosParecer['cprsitcoordenacao'] == 't'){
            $checkSim_coo = 'checked = "checked"';
        }elseif($dadosParecer['cprsitcoordenacao'] == 'f'){
            $checkNao_coo = 'checked = "checked"';
        }
        
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
    $url            = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_parecer&acao=A';
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

<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>

<script type="text/javascript">
    
    $(document).ready(function(){
        $('#salvarParecer').click(function(){
            var prcdsc = $('#prcdsc');           
            var erro;

            if(!prcdsc.val()){
                alert('O campo "Descrição do Parecer" é um campo obrigatório!');
                arquivo.focus();
                erro = 1;
                return false;
            }
            
            if(!erro){
                $('#requisicao').val('salvarParecer');
                $('#formulario').submit();
            }
        });
    });
    
    function atualizaComboAdvogados( coonid ){ 
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboAdvogados&coonid="+coonid,
            async: false,
            success: function(resp){
                $('#comboAdvogados').html(resp);
            }
        });
    }
    
    function editarParecer( prcid ){      
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=editarParecer&prcid="+prcid,
            asynchronous: false,
            success: function(resp){
                var dados = $.parseJSON(resp);
                if(dados.permissao == 'N'){
                    alert('Usuário sem permissão para fazer Alteração no parecer. Apenas o usuário que inseriu o parecer pode altera-lo.');
                    $('#cprid').val('');
                    $('#usunome').val('');
                    $('#prcsitworkflow').val('');
                    $('#prcdsc').val('');
                }else{
                    $('#prcid').val(dados.prcid);
                    $('#cprid').val(dados.cprid);
                    $('#usunome').val(dados.usunome);
                    $('#prcsitworkflow').val(dados.prcsitworkflow);
                    $('#prcdsc').val(dados.prcdsc);
                }
            }
        }); 
        
    }

    function excluirPortariaAnexo(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirPortariaAnexo');
            $('#formulario').submit();
        }
    }

    function salvarParecerUni( tipo ){
        var cprsitmovimento     = $('input:radio[name=cprsitmovimento]:checked');
        var cprsitcap           = $('input:radio[name=cprsitcap]:checked');
        var cprsitcggp          = $('input:radio[name=cprsitcggp]:checked');
        var cprsitcoordenacao   = $('input:radio[name=cprsitcoordenacao]:checked');
        var cprsitadvogado      = $('input:radio[name=cprsitadvogado]:checked');
        
        var coonid = $('#coonid');
        var advid = $('#advid');
        
        var erro;

        if( !cprsitmovimento.val() && tipo == 'M' ){
            alert('O campo "Movimentação" é um campo obrigatório!');
            cprsitmovimento.focus();
            erro = 1;
            return false;
        }
        
        if( !cprsitcap.val() && tipo == 'C' ){
            alert('O campo "CAP" é um campo obrigatório!');
            cprsitcap.focus();
            erro = 1;
            return false;
        }

        if( !cprsitcggp.val() && tipo == 'G' ){
            alert('O campo "CGGP" é um campo obrigatório!');
            cprsitcggp.focus();
            erro = 1;
            return false;
        }
        
        if(!coonid.val() && tipo == 'V' ){
            alert('O campo "Selecionar Coordenação" é um campo obrigatório!');
            coonid.focus();
            erro = 1;
            return false;
        }
        
        if(!advid.val() && tipo == 'V' ){
            alert('O campo "Selecionar Advogado" é um campo obrigatório!');
            advid.focus();
            erro = 1;
            return false;
        }
        
        if(!cprsitcoordenacao.val() && tipo == 'O' ){
            alert('O campo "Coordenador" é um campo obrigatório!');
            cprsitcoordenacao.focus();
            erro = 1;
            return false;
        }

        if(!cprsitadvogado.val() && tipo == 'A' ){
            alert('O campo "Advogado" é um campo obrigatório!');
            cprsitadvogado.focus();
            erro = 1;
            return false;
        }
   
        if(!erro){
            $('#requisicao').val('salvarParecerUni');
            $('#tipo').val( tipo );
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
<input type="hidden" id="prcid" name="prcid" value="<?= $prcid; ?>"/>
<input type="hidden" id="tipo" name="tipo" value=""/>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
    <tr>
        <td class ="SubTituloDireita" width="30%">Responsável pelo Parecer:</td>
        <td colspan="4" width="40%">
            <?php
                echo campo_texto('usunome', 'S', 'N', '', 45, 10, '', '', '', '', 0, 'id="usunome"', '', $usunome, null, null);
            ?>
        </td>
        <!-- COLUNA DO WORKFLOW -->
        <td rowspan="17" align="right" width="30%">
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
        <td class ="SubTituloDireita">Situação Atual:</td>
        <td colspan="4">
            <?php
            $prcsitworkflow = $estadoAtual['esddsc'];
            echo campo_texto('prcsitworkflow', 'S', 'N', '', 45, 10, '', '', '', '', 0, 'id="prcsitworkflow"', '', $prcsitworkflow, null, null);
            ?>
        </td>
    </tr>
    
    <tr>
        <td colspan="5" class ="SubTituloCentro" style="text-align: left;">CGGP/CAP</td>
    </tr>

    <tr>
        <td rowspan="4" class="SubTituloDireita">Parecer CGGP/CAP</td>
        <th width="15%">Unidade</th>
        <th width="30%">Situação</th>
        <th width="3%">Ação</th>
        <th width="52%">Responsável</th>
    </tr>
    <tr>
        <td>Movimentação</td>
        <td>
            <input type="radio" name="cprsitmovimento" id="cprsitmovimento_s" value="S" <?=$checkSim_mov;?>> Favorável 
            <input type="radio" name="cprsitmovimento" id="cprsitmovimento_n" value="N" <?=$checkNao_mov;?>> Não Favorável
        </td>
        <td>
            <input type="button" value="OK" name="salvarMov" id="salvarMov" <?=$perm['mov'];?>>
        </td>
        <td align="center">
            <?=$nome_sitmovimentacao;?> <input type="hidden" name="usucpfsitmovimentacao" id="usucpfsitmovimentacao" value="<?=$usucpfsitmovimentacao;?>">
        </td>
    </tr>
    <tr>
        <td>CAP</td>
        <td>
            <input type="radio" name="cprsitcap" id="cprsitcap_s" value="S" <?=$checkSim_cap;?>> Favorável
            <input type="radio" name="cprsitcap" id="cprsitcap_n" value="N" <?=$checkNao_cap;?>> Não Favorável
        </td>
        <td>
            <input type="button" value="OK" name="salvarCap" id="salvarCap" <?=$perm['cap'];?>>
        </td>
         <td align="center">            
            <?=$nome_sitcap;?> <input type="hidden" name="usucpfsitcap" id="usucpfsitcap" value="<?=$usucpfsitcap;?>">
        </td>
    </tr>
    <tr>
        <td>CGGP</td>
        <td>
            <input type="radio" name="cprsitcggp" id="cprsitcggp_s" value="S" <?=$checkSim_cggp;?>> Favorável
            <input type="radio" name="cprsitcggp" id="cprsitcggp_n" value="N" <?=$checkNao_cggp;?>> Não Favorável
        </td>
        <td>
            <input type="button" value="OK" name="salvarCggp" id="salvarCggp" <?=$perm['cggp'];?>>
        </td>
         <td align="center">
            <?=$nome_sitcggp?> <input type="hidden" name="usucpfsitcggp" id="usucpfsitcggp" value="<?=$usucpfsitcggp;?>">
        </td>
    </tr>
    
    <!-- PARA QUIE ESSE BLOCO VOLTE A FUCIONAR NORMALMENTE TANTO O LAYOUT QUANTO A PARTE DE PERSISTÊNCIA E A REGRAS, BASTA DESCOMENTAR ESSE BLOCO.
    <tr>
        <td colspan="5" class ="SubTituloCentro" style="text-align: left;">CONJUR</td>
    </tr>
    <tr>
        <td rowspan="7" class="SubTituloDireita">Parecer CONJUR</td>
        <th>Unidade</th>
        <th>&nbsp;</th>
        <th>Ação</th>
        <th>&nbsp;</th>
    </tr>
    <tr>
        <td>Coordenação</td>
        <td>
            <?php
                $coonid = $dadosParecer['coonid'];
                $sql = "
                    SELECT  coonid AS codigo, 
                            coodsc AS descricao
                    FROM conjur.coordenacao
                ";
                $db->monta_combo("coonid", $sql, 'S', 'Selecione...', 'atualizaComboAdvogados', '', '', '370', 'S', 'coonid', false, $coonid, null);
            ?>
        </td>
        <td rowspan="2">
            <input type="button" value="OK" name="salvarCoor" id="salvarCoorAdv" <?=$perm['adCoo'];?>>
        </td>
        <td rowspan="2">&nbsp;</td>
    </tr>
    <tr>
        <td>Advogado</td>
        <td id="comboAdvogados">                
            <?php
                $advid = $dadosParecer['advid'];
                $sql = "
                    SELECT  DISTINCT a.advid  AS codigo,
                            e.entnome AS descricao
                    FROM conjur.coordenacao c
                    JOIN conjur.advogadosxcoordenacao ac ON ac.coonid = c.coonid
                    JOIN conjur.advogados a ON a.advid = c.advid
                    JOIN entidade.entidade e ON e.entid = a.entid
                    WHERE a.advstatus = 'A'
                ";
                $db->monta_combo("advid", $sql, 'S', 'Selecione...', '', '', '', '370', 'S', 'advid', false, $advid, null);
            ?>
        </td>
    <tr>
        <td colspan="4" class="SubTituloCentro">Deferimento</td>
    </tr>
    <tr>
        <th>Unidade</th>
        <th>Situação</th>
        <th>Ação</th>
        <th>Responsável</th>
    </tr>
    <tr>
        <td>Coodernador</td>
        <td>
            <input type="radio" name="cprsitcoordenacao" id="cprsitcoordenacao_s" value="S" <?=$checkSim_coo;?>> Favorável
            <input type="radio" name="cprsitcoordenacao" id="cprsitcoordenacao_n" value="N" <?=$checkNao_coo;?>> Não Favorável
        </td>
        <td>
            <input type="button" value="OK" name="salvarCoor" id="salvarCoor" <?=$perm['coo'];?>>
        </td>
         <td align="center">          
            <?=$nome_sitcoordenador;?> <input type="hidden" name="usucpfsitcoordenador" id="usucpfsitcoordenador" value="<?=$usucpfsitcoordenador;?>">
        </td>
    </tr>
    
    <tr>
        <td>Advogado</td>
        <td>
            <input type="radio" name="cprsitadvogado" id="cprsitadvogado_s" value="S" <?=$checkSim_adv;?>> Favorável
            <input type="radio" name="cprsitadvogado" id="cprsitadvogado_n" value="N" <?=$checkNao_adv;?>> Não Favorável
        </td>
        <td>
            <input type="button" value="OK" name="salvarAdv" id="salvarAdv" <?=$perm['adv'];?>>
        </td>
         <td align="center"> 
             <?=$nome_sitadvogado;?><input type="hidden" name="usucpfsitadvogado" id="usucpfsitadvogado" value="<?=$usucpfsitadvogado;?>">
        </td>
    </tr>
    -->
    <tr>
        <td colspan="5" class ="SubTituloCentro" style="text-align: left;">Descrição do Parecer</td>
    </tr>
    
    <tr>
        <td class ="SubTituloDireita">Descrição:</td>
        <td colspan="4">
            <?php
                $prcdsc = $dadosParecer['prcdsc'];
                echo campo_textarea('prcdsc', 'S', $habilita, '', 150, 10, '1000', '', 0, '', false, null); 
            ?>
        </td>
    </tr>


    
    <?php if( $hab_botao == 'S' ){ ?>
    <tr>
        <td class="SubTituloCentro" colspan="6">
            <input type="button" id="salvarParecer" name="salvarParecer" value=" Salvar "/>
            <input type="button" name="voltar" id="valtar" value="Voltar" onclick="voltarListaPessoa();"/>
        </td>
    </tr>
    <? } ?>
    </table>

<?php 

    if( $hab_botao == 'S' ){
        
        $usucpf = $_SESSION['usucpf'];
                
        $acao = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarParecer('||prcid||');\" title=\"Editar Servidor\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletarParecer('||prcid||')\" title=\"Editar Servidor\" >
            </center>
        ";
        
        $sem = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar_01.gif\" title=\"Sem Permissão\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Sem Permissão\" >
            </center>
        ";

        $sql = "
            SELECT  CASE WHEN pc.usucpf = '{$usucpf}'
                        THEN '{$acao}' 
                        ELSE '{$sem}'
                    END as acao,
                    pc.prcdsc,
                    pc.prcsitworkflow,
                    pc.usucpf,
                    u.usunome,
                    to_char(pc.prcdtinclusao, 'DD/MM/YYYY') as prcdtinclusao
            FROM gestaopessoa.parecerchecklist pc
            JOIN seguranca.usuario AS u ON u.usucpf = pc.usucpf
            WHERE prcstatus = 'A' AND cprid = {$cprid}
            ORDER BY  prcid
        ";
        $cabecalho = Array("Ação", "Descrição", "Situação Atual", "CPF", "Responsável", "Data da Inclusão");
        $alinhamento = Array('center', 'left', 'left', 'right', 'left', 'right');
        $tamanho = Array('4%', '61%', '15%', '10%', '10%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

    }
?>

