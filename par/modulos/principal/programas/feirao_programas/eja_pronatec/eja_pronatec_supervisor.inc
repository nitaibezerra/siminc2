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
        $_REQUEST['requisicao']($_REQUEST);
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

    monta_titulo($nomePrograma." - Supervisor de Demandas", $descricao);
    echo '<br>';

    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_supervisor&acao=A');
    
    $dadosEjaPronatec = buscaDadosSupervisoeDemandas ( $_SESSION['par']['adpid'] );
    
    if( $_SESSION['par']['muncod'] != '' ){
        #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
        $textoAviso = 'AVISO: Deverá, obrigatoriamente, ser servidor da Prefeitura Municipal.';
    }else{
        #VARIAVEL DE VERIFICAÇÃO ESTADO OU MUNICIPIO
        $textoAviso = 'AVISO: Deverá, obrigatoriamente, ser servidor da Secretaria Estadual.';
    }
    
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">

    function salvarSupervisorEJA_PRONATEC(){
        var epsnome                 = $('#epsnome');
        var epscpf                  = $('#epscpf');
        var epsmatricula            = $('#epsmatricula');
        var epstelefone             = $('#epstelefone');
        var epscelular              = $('#epscelular');
        var epsemailinstitucional   = $('#epsemailinstitucional');
        var epsemailparticular      = $('#epsemailparticular');
        var epsdtinicio             = $('#epsdtinicio');

        var erro;

        if(!epsnome.val()){
            alert('O campo "Nome" é um campo obrigatório!');
            epsnome.focus();
            erro = 1;
            return false;
        }
        if(!epscpf.val()){
            alert('O campo "CPF" é um campo obrigatório!');
            epscpf.focus();
            erro = 1;
            return false;
        }
        if(!epsmatricula.val()){
            alert('O campo "Matrícula SIAPE" é um campo obrigatório!');
            epsmatricula.focus();
            erro = 1;
            return false;
        }
        if(!epstelefone.val()){
            alert('O campo "Telefone DDD/Número" é um campo obrigatório!');
            epstelefone.focus();
            erro = 1;
            return false;
        }
        if(!epscelular.val()){
            alert('O campo "Celular DDD/Número" é um campo obrigatório!');
            epscelular.focus();
            erro = 1;
            return false;
        }
        if(!epsemailinstitucional.val()){
            alert('O campo "E-mail Insitucional" é um campo obrigatório!');
            epsemailinstitucional.focus();
            erro = 1;
            return false;
        }
        if(!epsemailparticular.val()){
            alert('O campo "E-mail particular" é um campo obrigatório!');
            epsemailparticular.focus();
            erro = 1;
            return false;
        }
        if(!epsdtinicio.val()){
            alert('O campo "Data Inicio" é um campo obrigatório!');
            epsdtinicio.focus();
            erro = 1;
            return false;
        }
        
        if(!erro){
            $('#requisicao').val('salvarSupervisorEJA_PRONATEC');
            $('#formulario').submit();
        }
    }
    
    function verificarCPFValido( epscpf ){
        if(epscpf != ''){
            var valido = validar_cpf( epscpf );
        
            if(!valido){
                alert( "CPF inválido! Favor informar um CPF válido!" );
                $('#epscpf').focus();
                return false;
            }
        }
        return true;
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="epsid" name="epsid" value="<?= $dadosEjaPronatec['epsid']; ?>"/>
    
    <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="SubTituloCentro" colspan="3" style="color: #AE193E;">
                <?=$textoAviso;?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%" > Nome: </td>
            <td width="65%">
               <?php
                    $epsnome = $dadosEjaPronatec['epsnome'];
                    echo campo_texto('epsnome', 'S', $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epsnome"', '', $epsnome, null, '', null);
                ?> 
            </td>
            <td rowspan="12" width="5%" style="margin-right: 10px; vertical-align: central;">
                <?php
                    include_once APPRAIZ . "includes/workflow.php";

                    $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                    wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> CPF: </td>
            <td>
               <?php
                    $epscpf = $dadosEjaPronatec['epscpf'];
                    echo campo_texto('epscpf', 'S', $habilitado['TEXT'], '', 14, 14, '###.###.###-##', '', 'right', '', 0, 'id="epscpf"', '', $epscpf, 'verificarCPFValido(this.value)', '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Matrícula SIAPE: </td>
            <td>
               <?php
                    $epsmatricula = $dadosEjaPronatec['epsmatricula'];
                    echo campo_texto('epsmatricula', 'S', $habilitado['TEXT'], '', 14, 20, '', '', 'right', '', 0, 'id="epsmatricula"', '', $epsmatricula, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> RG: </td>
            <td>
               <?php
                    $epsrg = $dadosEjaPronatec['epsrg'];
                    echo campo_texto('epsrg', 'N', $habilitado['TEXT'], '', 14, 30, '', '', 'right', '', 0, 'id="epsrg"', '', $epsrg, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Órgão Expedidor RG: </td>
            <td>
               <?php
                    $epsrgorgaoexp = $dadosEjaPronatec['epsrgorgaoexp'];
                    echo campo_texto('epsrgorgaoexp', 'N', $habilitado['TEXT'], '', 14, 30, '', '', 'right', '', 0, 'id="epsrgorgaoexp"', '', $epsrgorgaoexp, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF Órgão Expedidor RG: </td>
            <td>
                <?php
                    $estufrgorgaoexp = $dadosEjaPronatec['estufrgorgaoexp'];
                    $sql = "
                         SELECT  estuf as codigo, 
                                 estuf as descricao 
                         FROM territorios.estado 
                         ORDER BY estuf
                     ";
                     $db->monta_combo('estufrgorgaoexp', $sql, 'S', "Selecione...", '', '', '', 160, 'N', 'estufrgorgaoexp', '', $estufrgorgaoexp);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Data Expedição RG: </td>
            <td>
               <?php
                    $epsdtexprg = $dadosEjaPronatec['epsdtexprg'];
                    echo campo_data2('epsdtexprg','N', 'S','','DD/MM/YYYY', $texto_ajuda, '', $epsdtexprg, '', null, 'epsdtexprg' );
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone DDD/Número: </td>
            <td>
               <?php
                    $epstelefone = $dadosEjaPronatec['epstelefone'];
                    echo campo_texto('epstelefone', 'S', $habilitado['TEXT'], '', 14, 14, '##-####-####', '', 'right', '', 0, 'id="epstelefone"', '', $epstelefone, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Celular DDD/Número: </td>
            <td>
               <?php
                    $epscelular = $dadosEjaPronatec['epscelular'];
                    echo campo_texto('epscelular', 'S', $habilitado['TEXT'], '', 14, 14, '##-####-####', '', 'right', '', 0, 'id="epscelular"', '', $epscelular, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> E-mail iInsitucional: </td>
            <td>
               <?php
                    $epsemailinstitucional = $dadosEjaPronatec['epsemailinstitucional'];
                    echo campo_texto('epsemailinstitucional', 'S', $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epsemailinstitucional"', ';', $epsemailinstitucional, null, '', null);
                ?> 
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> E-mail particular: </td>
            <td>
               <?php
                    $epsemailparticular = $dadosEjaPronatec['epsemailparticular'];
                    echo campo_texto('epsemailparticular', 'S', $habilitado['TEXT'], '', 60, 255, '', '', 'left', '', 0, 'id="epsemailparticular"', '', $epsemailparticular, null, '', null);
                ?> 
            </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita">Data Inicio:</td>
            <td width="10%">
                <?php
                    $epsdtinicio = $dadosEjaPronatec['epsdtinicio'];
                    echo campo_data2('epsdtinicio','S','S','','DD/MM/YYYY', $texto_ajuda, '', $epsdtinicio, '', null, 'epsdtinicio' );
                ?>
            </td>
        </tr>
    </table>
    <br>
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="salvar" value="Salvar" onclick="salvarSupervisorEJA_PRONATEC();"/>
                <input type="button" name="cancelar" value="Cancelar" onclick="pesquisarServidor('ver');"/>
            </td>
        </tr>
    </table>
</form>