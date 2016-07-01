<?php
    include_once '_funcoes_maismedicos.php';

    include_once APPRAIZ . "includes/workflow.php";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


    if($_REQUEST['requisicao']=='excluir'){
        excluirDoc( $_REQUEST['arquivo'] );
        exibirListaDoc('R');
        die();
    }

    if($_REQUEST['requisicao'] == 'salvarRecursoMaisMedicos'){
        salvarRecursoMaisMedicos($_POST, $_FILES);
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicosRecursoMunicipio&acao=A');
    monta_titulo('Mais Médicos - Recurso Município', recuperaMunicipioEstado());
    $_SESSION['maismedicos'] = true;

    $rqmid = verificaExisteQuestionario();

    if ($rqmid == '') {
        $db->sucesso('principal/programas/feirao_programas/maisMedicosAcoesMunicipio', '', 'Não é possivel realizar o Recurso, necessario preencheer as Ações do Município.');
    } else {
        $dadosRecurso = buscaRecudoMaisMedicos($rqmid);
    }

    if ($_SESSION['par']['rqmid']) {
        $docid = pgMaisMedicosCriarDocumento($_SESSION['par']['rqmid']);
    }

    $perfis = pegaArrayPerfil($_SESSION['usucpf']);

    if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfis) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfis) ){
        #HABILITA PARECER.
        $habilita_par = 'S';
        $habilita_radio = '';
        $habilita_par_anexo  = '';
        $habilita_par_salvar = 'onclick="salvarRecursoMaisMedicos(\'P\');"';

        #DESABILITA RECURSO.
        $habilita_rec = 'N';
        $habilita_rec_anexo  = 'disabled="disabled"';
        $habilita_rec_salvar = 'disabled="disabled"';

    }else{
        #HABILITA RECURSO.
        $habilita_rec = 'S';
        $habilita_rec_anexo  = '';
        $habilita_rec_salvar = 'onclick="salvarRecursoMaisMedicos(\'R\');"';

        #DESABILITA PARECER.
        $habilita_par = 'N';
        $habilita_radio = 'disabled="disabled"';
        $habilita_par_anexo  = 'disabled="disabled"';
        $habilita_par_salvar = 'disabled="disabled"';
    }
    
    if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfis) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfis)){
        
           #DESABILITA RECURSO.
        $habilita_rec = 'N';
        $habilita_rec_anexo  = 'disabled="disabled"';
        $habilita_rec_salvar = 'disabled="disabled"';
        
        $habilita_par = 'N';
        $habilita_radio = 'disabled="disabled"';
        $habilita_par_anexo  = 'disabled="disabled"';
        $habilita_par_salvar = 'disabled="disabled"';
        
    }

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">


    function excluirDoc(arqid){
        if(confirm("Deseja realmente excluir o Arquivo?")){
            $.ajax({
                url: 'par.php?modulo=principal/programas/feirao_programas/maisMedicosRecursoMunicipio&acao=A',
                data: {
                    requisicao: 'excluir',
                    arquivo: arqid
                },
                async: false,
                success: function(data){
                    alert('Arquivo excluído com sucesso!');
                    $("#td_lista_arquivos_anexos").html(data);
                    location.reload (true);
                }
            });
        }
    }

    function imprimeRecursoMunicipio(){
        window.open("par.php?modulo=principal/programas/feirao_programas/maisMedicosRecursoMunicipioImpressao&acao=A","Recurso Município",'scrollbars=yes,height=660,width=830,status=no,toolbar=no,menubar=no,location=no');
    }

    function salvarRecursoMaisMedicos(tipo) {
        var rcmtextmunicipio = $('#rcmtextmunicipio');
        var rcmparecermec = $('#rcmparecermec');
        var rcmsituacao = $('input:radio[name=rcmsituacao]:checked');

        var erro;

        if (tipo == 'R') {
            if (!rcmtextmunicipio.val()) {
                alert('O campo "Descrição do Recurso do Município" é um campo obrigatório!');
                rcmtextmunicipio.focus();
                erro = 1;
                return false;
            }
        } else {
            if (!rcmparecermec.val()) {
                alert('O campo "Descrição do Parecer do MEC" é um campo obrigatório!');
                rcmparecermec.focus();
                erro = 1;
                return false;
            }
            if (!rcmsituacao.val()) {
                alert('O campo "Favorável/Não Favorável" é um campo obrigatório!');
                rcmsituacao.focus();
                erro = 1;
                return false;
            }
        }

        if (!erro) {
            $('#tipo_parecer').val(tipo);
            $('#requisicao').val('salvarRecursoMaisMedicos');
            $('#formulario').submit();
        }
    }

</script>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
    <tr class="subtituloEsquerda">
        <td height="10" width="45%">
            <input type="button" value="Voltar" onclick="window.location.href = '?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" id="btnVoltar" />
        </td>
    </tr>
</table>

<br>

<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <input type="hidden" id="tipo_parecer" name="tipo_parecer" value=""/>

    <input type="hidden" id="rqmid" name="rqmid" value="<?= $rqmid; ?>"/>
    <input type="hidden" id="rcmid" name="rcmid" value="<?= $dadosRecurso['rcmid'] ?>"/>

    <input type="hidden" id="usucpfmunicipio" name="usucpfmunicipio" value="<?= $_SESSION['usucpf']; ?>"/>
    <input type="hidden" id="usucpfmec" name="usucpfmec" value="<?= $_SESSION['usucpf']; ?>"/>

    <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td>
                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <? if( in_array(PAR_PERFIL_PREFEITO, $perfis) || $dadosRecurso['usucpfmunicipio'] != '' ){ ?>
                        <tr>
                            <td bgcolor="#CCCCCC" colspan="2"> <b>Dados do Responsável pelo Recurso do Município</b> </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" width="25%"> Nome: </td>
                            <td><?= $dadosRecurso['nome_municipio'] ? $dadosRecurso['nome_municipio'] : $_SESSION['usunome']; ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita"> CPF: </td>
                            <td><?= $dadosRecurso['usucpfmunicipio'] ? $dadosRecurso['usucpfmunicipio'] : $_SESSION['usucpf']; ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Data de inserção do Recurso:</td>
                            <td> <?= $dadosRecurso['rcmdtinclusaomunicipio'] ? $dadosRecurso['rcmdtinclusaomunicipio'] : gmdate('d/m/Y'); ?> </td>
                        </tr>
                    <? } ?>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Descrição do Recurso do Município</b></td>
                    </tr>
                    <tr>
                        <td colspan="2"><?php echo campo_textarea('rcmtextmunicipio', 'S', $habilita_rec, '', '150', '10', '10000', '', 0, '', false, NULL, $dadosRecurso['rcmtextmunicipio'], '99%'); ?></td>
                    </tr>
                    <tr>
                        <td class="subtituloCentro" colspan="2">&nbsp;</td>
                    </tr>
                    <tr width="25%">
                        <td class="subtituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arquivo" <?=$habilita_rec_anexo;?> >
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." >

                            <input type="hidden" id="tpaid" name="tpaid" value="21"/>
                            <input type="hidden" id="arqdescricao" name="arqdescricao" value="Arquivo Recurso Município"/>

                        </td>
                    </tr>
                    <?PHP
                        $data_hoje = strtotime( gmdate('Y-m-dTh:i:s') );
                        $data_fech = strtotime( DATA_FECHAMENTO_RECURSO );

                        if( ($data_hoje <= $data_fech) ){
                    ?>
                    <tr>
                        <td colspan="2" class="SubTituloCentro">
                            <input type="button" value="Salvar" id="salvar" <?=$habilita_rec_salvar;?> />
                            <input type="button" value="Voltar"  id="btnVoltar" <?=$habilita_rec_salvar;?> onclick="window.location.href = '?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';"/>
                        </td>
                    </tr>
                    <?  }else{ ?>
                    <tr>
                        <td colspan="2" class="SubTituloCentro">
                            <b style="color:red;">NÃO É POSSÍVEL INSERIR O RECURSO DO MUNICÍPIO, O PRAZO FINAL FOI "10/12/2013"</b>
                        </td>
                    </tr>
                    <?  } ?>
                </table>
                <br>
                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td colspan="2" id="td_lista_arquivos_anexos">
                            <?php exibirListaDoc('R'); ?>
                        </td>
                    </tr>
                </table>
                <br>
                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <? if( $habilita_par == 'S' ){ ?>
                        <tr>
                            <td bgcolor="#CCCCCC" colspan="2"> <b>Dados do Responsável pelo Parecer do MEC</b> </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" width="25%"> Nome: </td>
                            <td><?= $dadosRecurso['nome_mec'] ? $dadosRecurso['nome_mec'] : $_SESSION['usunome']; ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita"> CPF: </td>
                            <td><?= $dadosRecurso['usucpfmec'] ? $dadosRecurso['usucpfmec'] : $_SESSION['usucpf']; ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Data de inserção do Parecer:</td>
                            <td> <?= $dadosRecurso['rcmdtinclusaomec'] ? $dadosRecurso['rcmdtinclusaomec'] : gmdate('d/m/Y'); ?> </td>
                        </tr>
                    <? } ?>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Descrição do Parecer do MEC</b></td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <?php
                                echo campo_textarea('rcmparecermec', 'S', $habilita_par, '', '150', '10', '10000', '', 0, '', false, NULL, $dadosRecurso['rcmparecermec'], '99%');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <input type="radio" name="rcmsituacao" id="rcmsituacao_s" <? echo $habilita_radio; if ($dadosRecurso['rcmsituacao'] == 't') echo 'checked = checked'; ?> value="S"> <b> Recurso Deferido </b>
                            <br>
                            <input type="radio" name="rcmsituacao" id="rcmsituacao_n" <? echo $habilita_radio; if ($dadosRecurso['rcmsituacao'] == 'f') echo 'checked = checked'; ?> value="N"> <b> Recurso Indeferido </b>
                        </td>
                    </tr>
                    <tr width="25%">
                        <td class="subtituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arquivo" <?=$habilita_par_anexo;?> >
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." >

                            <input type="hidden" id="tpaid" name="tpaid" value="22"/>
                            <input type="hidden" id="arqdescricao" name="arqdescricao" value="Parecer Recurso MEC"/>

                        </td>
                    </tr>
                </table>
                <br>
                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr>
                        <td colspan="2" id="td_lista_arquivos_anexos">
                            <?php exibirListaDoc('M'); ?>
                        </td>
                    </tr>
                </table>
                <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <br>
                    <tr>
                        <td colspan="2" class="SubTituloCentro">
                            <input type="button" value="Salvar" id="salvar" <?=$habilita_par_salvar;?>/>
                            <input type="button" value="Voltar" id="btnVoltar" <?=$habilita_par_salvar;?> onclick="window.location.href = '?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';"/>

                            <? if( in_array(PAR_PERFIL_ADMINISTRADOR, $perfis) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfis) || in_array(PAR_PERFIL_PREFEITO, $perfis) ){ ?>
                                <input type="button" value="Imprimir" id="imprimir" name="imprimir" onclick="imprimeRecursoMunicipio();"/>
                            <? } ?>
                        </td>
                    </tr>
                </table>
            </td>
            <td width="5%" align="center" valign="top" class ="SubTituloCentro">
                <?PHP
                    if ($docid) {
                        wf_desenhaBarraNavegacao($docid, array('rqmid' => $_SESSION['par']['rqmid']), '');
                    }
                ?>
            </td>
    </table>
</form>