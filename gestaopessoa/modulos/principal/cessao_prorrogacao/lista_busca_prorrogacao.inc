<?php

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_GET['cprid'] != ''){
        $cprid = $_GET['cprid'];
 
        $dadosServ = dadosServidor( $cprid );
        
        $abrirGrid = 'S';
        
        $dados['tipo'] = 'ed';
        $dados['orcid'] = $_GET['orcid'];
        $dados['fdpcpf'] = str_replace("-", "", str_replace(".", "", $dadosServ['fdpcpf']) );
    }
    
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Getão de Pessoas', '<b>PRORROGAÇÃO - Litagem das Cessões e Prorrogações / Cadastro de Prorrogação</b>');
    echo "<br>";

?>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    
    function atualizaGridCessao(nu_matricula_siape){ 
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaGridCessao&tipo=at&nu_matricula_siape="+nu_matricula_siape,
            async: false,
            success: function(resp){
                $('#listaCessaoServidor').html(resp);
            }
        });
    }
    
    function atualizaGridProrrogacao(orcid, tpsid){
        var nu_matricula_siape = $('#nu_matricula_siape').val();
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaGridProrrogacao&tipo=at&nu_matricula_siape="+nu_matricula_siape+"&orcid="+orcid,
            async: false,
            success: function(resp){
                $('#orcid').val(orcid);
                $('#tpsid').val(tpsid);
                $('#listaProrrogacaoServidor').html(resp);
            }
        });
    }

    function buscarDadosServidorSIAPE( nu_matricula_siape ){ 
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosServidor&nu_matricula_siape="+nu_matricula_siape,
            //asynchronous: false,
            success: function(resp){
                var dados = $.parseJSON(resp);             
                $('#nu_matricula_siape').val(dados.nu_matricula_siape);
                $('#nu_cpf').val(dados.nu_cpf);
                $('#no_servidor').val(dados.no_servidor);

                atualizaGridCessao( nu_matricula_siape );
            }
        });
    }

    function buscarDadosServidorCPF( nu_cpf ){ 
        nu_cpf = replaceAll( replaceAll( $('#nu_cpf').val(), ".", "" ), "-", "" );
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosServidor&nu_cpf="+nu_cpf,
            success: function(resp){
                var dados = $.parseJSON(resp);             
                $('#nu_matricula_siape').val(dados.nu_matricula_siape);
                $('#nu_cpf').val(dados.nu_cpf);
                $('#no_servidor').val(dados.no_servidor);
                
                $('#listaCessaoServidor').html('');
                $('#listaProrrogacaoServidor').html('');
                var nu_matricula_siape = dados.nu_matricula_siape;
                atualizaGridCessao( nu_matricula_siape );
            }
        });
    }

    function cadastrarNovaProrrogracao(){
        var nu_matricula_siape = $('#nu_matricula_siape').val();
        var tpsid  = $('#tpsid').val();
        var orcid  = $('#orcid').val();
        
        var selec_orcid = $('input:radio[name=selec_orcid[]]:checked');
        
        var erro = 0;

        if(!selec_orcid.val()){
            alert('É necesssario selecionar uma "CESSÃO"!');
            selec_orcid.focus();
            erro = 1;
            return false;
        }
        
        if (erro == 0){
            window.location.href ='gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_prorrogacao&acao=A&cadastro=S&nu_matricula_siape='+nu_matricula_siape+'&orcid='+orcid+"&tpsid="+tpsid;
        }
    }
    
    function deletaDadosProrrogacao(cprid){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=deletaProcessoCessaoProrrogacao&cprid="+cprid,
                asynchronous: false,
                success: function(resp){
                    if( trim(resp) == 'ok' ){
                        alert('Operação Realizada com sucesso!');
                        $('#formulario').submit();
                    }else{
                        alert('Ocorreu um problema, não foi possivél realizar a operação. Tente novamente mais tarde!');
                    }
                }
            });
        }
    }
    
    function editarProcessoProrrogacao(cprid){
        window.location.href ='gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_prorrogacao&acao=A&cprid='+cprid;
    }
    
    function voltarListaPessoa(){
        window.location.href = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_grid_prorrogacao&acao=A';
    }
    
</script>

<form action="" method="POST" id="formulario" name="formulario">
<input type="hidden" id="requisicao" name="requisicao" value=""/>
<input type="hidden" id="cprid" name="cprid" value="<?=$dadosServ['cprid'];?>"/>
<input type="hidden" id="orcid" name="orcid" value=""/>
<input type="hidden" id="tpsid" name="tpsid" value=""/>
<input type="hidden" id="cpf" name="cpf" value=""/>
 
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita">Código SIAPE:</td>
            <td colspan="2">
                <?php
                    $fdpsiape = $dadosServ['nu_matricula_siape'];
                    echo campo_texto('nu_matricula_siape', 'S', 'S', '', 20, 16, '', '', '', '', 0, 'id="nu_matricula_siape"', '', $fdpsiape, 'buscarDadosServidorSIAPE(this.value)', null); 
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="40%">CPF do Servidor:</td>
            <td width="60%">
                <?php
                    $fdpcpf = $dadosServ['nu_cpf'];
                    echo campo_texto('nu_cpf', 'S', 'S', '', 20, 14, '###.###.###-##', '', '', '', 0, 'id="nu_cpf"', '', $nu_cpf, 'buscarDadosServidorCPF(this.value)', null); 
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Nome do Servidor:</td>
            <td colspan="2">
                <?php
                    $fdpnome = $dadosServ['no_servidor'];
                    echo campo_texto('no_servidor', 'N', 'N', '', 35, 16, '', '', '', '', 0, 'id="no_servidor"', '', $no_servidor, null, null); 
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="3">
                <input type="button" id="pesquisarProcesso" name="pesquisar" value="Pesquisar"/>
                <input type="button" id="voltar" name="voltar" value="Voltar" onclick="voltarListaPessoa();"/>
            </td>
        </tr>
    </table>
</form>
<br>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <td class="SubTituloCentro">
            Cessão
        </td>
    </tr>
    <tr>
        <td>
            <div id="listaCessaoServidor" style="width: 100%; height: 135px; overflow: auto;">
                <?php 
                    if( $abrirGrid == 'S' ){
                        atualizaGridCessao( $dados );
                    }else{
                ?>    
                        <span style="color: red;"><center>Não foi informado o CPF do Servidor!</center></span>
                <?  }   ?>       
            </div>
        </td>
    </tr>
</table>
<br>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <td class="SubTituloCentro">
            Prorrogação
        </td>
    </tr>
    <tr>
        <td>
            <div id="listaProrrogacaoServidor" style="width: 100%; height: 135px; overflow: auto;">
                <?php 
                    if( $abrirGrid == 'S' && $dados['orcid'] != '' ){
                        atualizaGridProrrogacao( $dados );
                    }else{
                ?>    
                        <span style="color: red;"><center>Não foi informado o CPF do Servidor!</center></span>
                <?  }   ?>
            </div>
        </td>
    </tr>
</table>
<br>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td style="padding: 8px;">
            <span style="cursor: pointer" onclick="cadastrarNovaProrrogracao();" title="Nova Prorrogação" >
                <img align="absmiddle" src="/imagens/gif_inclui.gif"/> Cadastrar Nova Prorrogação
            </span>
        </td>
    </tr>
</table>