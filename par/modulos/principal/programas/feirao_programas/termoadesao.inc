<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

<script>
    jQuery.noConflict();
</script>

<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<style>
    <!--
    .ui-dialog-titlebar-close{ display: none; }
    -->
</style>

<?php
    header('Content-type: text/html; charset="iso-8859-1"', true);
    header("Cache-Control: no-store, no-cache, must-revalidate"); // HTTP/1.1
    header("Cache-Control: post-check=0, pre-check=0", false);
    header("Pragma: no-cache"); // HTTP/1.0 Canhe Livre

    unset($_SESSION['par']['prgid']);
    unset($_SESSION['par']['pfaano']);
    unset($_SESSION['par']['adpid']);

    //recupera session
    $_SESSION['par']['pfaid'] = $_REQUEST['pfaid'] ? $_REQUEST['pfaid'] : $_SESSION['par']['pfaid'];

    if ($_SESSION['par']['pfaid'] && $_SESSION['par']['inuid']) {

        $_SESSION['par']['prgid'] = $db->pegaUm("select prgid from par.pfadesao where pfaid = " . $_SESSION['par']['pfaid']);
        $_SESSION['par']['pfaano'] = $db->pegaUm("select pfaano from par.pfadesao where pfaid = " . $_SESSION['par']['pfaid']);
        $_SESSION['par']['adpid'] = $db->pegaUm("select max(adpid) from par.pfadesaoprograma where pfaid = " . $_SESSION['par']['pfaid'] . " and inuid = " . $_SESSION['par']['inuid']);

    } else {

        echo '<script type="text/javascript">
                    alert("Sessão Expirou.\nFavor selecione o programa novamente!");
                    window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
              </script>';
        die;

    }

    //fim recupera session
    //instancia quetionario
    if ($tapinstrucao == 'Q') {
        include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
        if ($_POST['salvar_questionario'] && $_POST['identExterno']) {
            $obMonta = new CampoExternoControle();
            $obMonta->salvar();
        }
    }

    if ($_SESSION['par']['prgid'] == '228' && !$_SESSION['maismedicos']) {
        header("Location:par.php?modulo=principal/programas/feirao_programas/maisMedicosCondicoesPart&acao=A");
        exit();
    }


    $aderiu = verificaAdesaoEJA();
    if ($_SESSION['par']['prgid'] == PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA && $_SESSION['continuaAdesao'] != 'S' && $aderiu != 'S') {
        header("Location:par.php?modulo=principal/programas/feirao_programas/eja/eja_informativo_adesao&acao=A");
        exit();
    }

    $aderiu_pronatec = verificaAdesaoEJA_PRONATEC();
    if( $_SESSION['par']['prgid'] == PROG_PAR_EJA_PRONATEC && $_SESSION['continuaAdesaoPronatec'] != 'S' && $aderiu_pronatec != 'S' ) {
        header("Location:par.php?modulo=principal/programas/feirao_programas/eja_pronatec/eja_pronatec_info_adesao&acao=A");
        exit();
    }

    $aderiu_maismedico = verificaAdesaoMaisMedicoNovo();
    if( $_SESSION['par']['prgid'] == PROG_PAR_MAIS_MEDICO_NOVO_2015 ){
        header("Location:par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/condicoes_participacao&acao=A");
        exit();
    }

    include_once "termoprofuncionario.inc";

   	$aderiu = testaAdesaoProFuncionario();

    /*
     * PNAIC
     * Não permite novas adesões
     * Criado em 03/09/2012
     * Solicitado pelo Analista Wallace
     */

    if ($_SESSION['par']['prgid'] == '157') {

        if ($_SESSION['par']['muncod']) {
            $stWhere = " where inu.muncod = '{$_SESSION['par']['muncod']}'";
        } else {
            $stWhere = " where inu.estuf = '{$_SESSION['par']['estuf']}'";
        }

        $sql = "select
                        adp.*,
                        doc.esdid,
                        cur.pcuid
                from
                        par.pfadesaoprograma adp
                inner join
                        par.pftermoadesaoprograma tap on tap.tapid = adp.tapid
                                and adp.tapid in (13,14)
                inner join
                        par.instrumentounidade inu on inu.inuid = adp.inuid
                left join
                        workflow.documento doc on doc.docid = adp.docid
                left join
                        par.pfcursista cur on cur.adpid = adp.adpid
                {$stWhere}";

        $rsAdesaoPnaic = $db->pegaLinha($sql);

        $finalizado = false;
        if (date('Y-m-d') >= '2012-11-06' && date('Y-m-d') <= '2012-11-30') {
            $finalizado = false;
        }


        // regra passada pela Manuelita - 27/12/12
        // esses municipios estão liberado
        // $list_muncod = array('1303700','5206602','5219738','2105609','2108454','2111201','2111706','3100906','3126950','3131158','3132800','3140530','3143708','3156502','3168101','5005004','5007505','1500305','1504422','1505494','2504405','2504850','2513604','2514453','2611101','2403509','2405306','2408953','1400282','4300901','4316303','4318614','4323754','4211256','4215059','2801009','2805208','3506607','3508900','3509403','3510153','3511201','3511607','3515301','3523701','3524303','3526506','3529658','3548203','3548302','3548807','3554706','3555356','1703842','1718451','1720853');
        // if(in_array($_SESSION['par']['muncod'], $list_muncod)) $finalizado = false;

        if ((!empty($rsAdesaoPnaic['adpresposta']) && !empty($rsAdesaoPnaic['adprespostapacto']) && !empty($rsAdesaoPnaic['pcuid']) && $rsAdesaoPnaic['esdid'] != 483)) {

            $msgPnaic = "A sua Secretaria de Educação concluiu a adesão ao Pacto Nacional pela Alfabetização na Idade Certa e às Ações do Pacto.";
        } else if ($finalizado) {
            $msgPnaic = "O prazo para realizar a adesão ao Pacto Nacional pela Alfabetização na Idade Certa finalizou no dia 30/11/2012.</a>.";
            if ($_SESSION['par']['muncod']) {
                $sql = "select muncod from par.municipioremanescente where muncod = '" . $_SESSION['par']['muncod'] . "'";
                $existe = $db->pegaUm($sql);

                if ($existe) {
                    unset($msgPnaic);
                }
            }
        }
    }


    $perfil = pegaArrayPerfil($_SESSION['usucpf']);

    if (( in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ) || ( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ) ) {
        unset($msgPnaic);
    }

    if (isset($msgPnaic)) {
        echo "<script type=\"text/javascript\">
                jQuery('input, select, textarea').remove();
                jQuery(function(){
                        jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                        jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                        jQuery( '#dialog-content' ).html('{$msgPnaic}');
                        jQuery( '#dialog-confirm' ).dialog({
                                resizable: false,
                                width: 550,
                                modal: true,
                                buttons: {
                                    'Ok': function() {
                                        jQuery( this ).dialog( 'close' );
                                        document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
                                    }
                                }
                        });
                });
            </script>";
    }

//    $sql = "
//            SELECT  max(adpid),
//                    adpresposta
//            FROM par.pfadesaoprograma
//            WHERE adpid = {$_SESSION['par']['adpid']}
//            GROUP BY adpid, adpresposta
//            ORDER BY adpid DESC
//            LIMIT 1
//        ";
//    ver($sql, $_SESSION['par']['adpid'], d);
//


if ($aderiu == 'S') {

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    //$db->cria_aba( $abacod_tela, "par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A", "", array() );
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A');

    //profuncionario
    if ($tapinstrucao == 'Q') {

        include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
        include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
        include_once APPRAIZ . "par/classes/controle/CampoExternoControle.class.inc";
        require_once APPRAIZ . "www/includes/webservice/cpf.php";

        monta_titulo($titulo_modulo, '
            Antes de responder às questões, passe o mouse em cima do símbolo <img src="/imagens/ajuda.gif" style="border:0;"> para obter as informações adicionais de cada pergunta.<br>
            Respondido<img src="/imagens/check_p.gif" style="border:0;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            Não Respondido<img src="/imagens/atencao.png" style="border:0;">'
        );

        $inuid = $_SESSION['par']['inuid'];

        $sql = "SELECT itrid FROM par.instrumentounidade WHERE inuid = {$inuid}";
        $tipo = $db->pegaUm($sql);

        $questionario = $tipo == 1 ? QUESTIONARIOPROEST : QUESTIONARIOPROMUN;

        $qrpid = pegaQrpidProFunc($questionario, $inuid, $_SESSION['par']['adpid']);

        $sql = "select
                    itptitulo
                from questionario.resposta res
                inner join questionario.itempergunta itp on itp.itpid = res.itpid
                where res.qrpid = {$qrpid}
                and itp.itpid = " . RESPOSTA1_BLOQUEIA_PROFUN . "
                order by res.resid desc";

        $boBloqueia = $db->pegaUm($sql);


        //pega estado atual
        $docid = pgCriarDocumento($_SESSION['par']['adpid']);
        $esdid = prePegarEstadoAtual($docid);

        $bloqueiaBotoes = false;

        if ($esdid != WF_TIPO_PROFUNC_CADASTRAMENTO && $esdid) {
            $bloqueiaBotoes = true;
        }


        #NOVA REGRA ADICIONA - SE O PROGRAMA ESTIVER COM A DATA DE ADESAO DISPONIVEL NÃO ENTRA E OS BOTÕES E CAMPOS CONTINUAM HABILITADOS.
        $habilitado = 'S';

        $programa = pegarProgramaDisponivel();
        $perfil = pegaArrayPerfil($_SESSION['usucpf']);

        $programa = $programa ? $programa : array();

        if ($_SESSION['par']['prgid'] == PROG_PAR_PROFUNCIONARIO &&
            !( in_array(PROGRAMA_PROFUNCIONARIO, $programa)) &&
            !( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) )
        ) {
            $habilitado = 'N';
        } else {
            $habilitado = 'S';
        }
        ?>
        <script type="text/javascript">

            jQuery(document).ready(function() {

                jQuery('input.classcpf').live('change', function() {
                    if (!validar_cpf(jQuery(this).val())) {
                        alert("CPF inválido!\nFavor informar um cpf válido!");
                        jQuery(this).val('');
                        return false;
                    }
                    var comp = new dCPF();
                    var td = jQuery(this).parent('td').next();
                    var nome = td.find('input');
                    var label = td.find('label');

                    comp.buscarDados(jQuery(this).val());
                    if (comp.dados.no_pessoa_rf != '') {
                        nome.val(comp.dados.no_pessoa_rf);
                        label.html(comp.dados.no_pessoa_rf);
                        nome.attr("readonly", "readonly");
                    }

                });

                var boArvore, stPergunta, stParams, boParams, arParams, params, perid;

        <?php if ($_SESSION['par']['itrid'] == 1): ?>
                    stPergunta = '   3 - Durante o período de implementação do Profuncionário no ...';
        <?php else: ?>
                    stPergunta = '   3 - Selecione a habilitação e, em seguida, preencha a quanti...';
        <?php endif; ?>

                jQuery().ajaxComplete(function(event, request, settings) {
                    boArvore = settings.url.search("dtree");
                    stParams = settings.data;
                    if (typeof (stParams) == 'string') {
                        boParams = stParams.search("perid");
                        if (boParams > 0) {
                            arParams = stParams.split('&');
                            $.each(arParams, function() {
                                params = this.split('=');
                                if (params[0] == 'perid') {
                                    if (params[1] > 0) {
                                        perid = params[1];
                                    }
                                }
                            });
                        }
                    }

                    if (boArvore > 0) {
                        var qrpid = "<?= $qrpid ?>";
                        jQuery.ajax({
                            type: "POST",
                            url: 'ajax.php',
                            data: 'requisicao=questionarioProfuncionario&qrpid=' + qrpid,
                            success: function(html) {
                                idPergunta3IE = bloqueiaP3IE();
                                if (html.substr(0, 1) == 'N') {
                                    if (perid == '1558' || perid == '1559' || perid == '1567' || perid == '1720' || perid == '1729') {
                                        jQuery('#proximo').attr('disabled', true);
                                        jQuery('#salvar_proximo').attr('disabled', true);
                                    }
                                    jQuery('.dTreeNode:contains("' + stPergunta + '")').hide();
                                    if (idPergunta3IE) {
                                        document.getElementById('iarvore' + idPergunta3IE).style.display = 'none';
                                        document.getElementById('sarvore' + idPergunta3IE).style.display = 'none';
                                    }
                                } else {
                                    jQuery('.dTreeNode:contains("' + stPergunta + '")').show();
                                    if (idPergunta3IE) {
                                        document.getElementById('iarvore' + idPergunta3IE).style.display = '';
                                        document.getElementById('sarvore' + idPergunta3IE).style.display = '';
                                    }
                                }

        <?php if ($bloqueiaBotoes) { ?>
                                    jQuery('input:button,:radio,:checkbox,:text').attr('disabled', true);
        <?php } ?>


                            }
                        });
                    }
                });

                jQuery('input[type=radio]').live('click', function() {
                    idPergunta3IE = bloqueiaP3IE();
                    if (this.value == '3162' || this.value == '3012' || this.value == '3026' || this.value == '3197') {
                        jQuery('.dTreeNode:contains("' + stPergunta + '")').hide();
                        if (idPergunta3IE) {
                            document.getElementById('iarvore' + idPergunta3IE).style.display = 'none';
                            document.getElementById('sarvore' + idPergunta3IE).style.display = 'none';
                        }
                    }
                    if (this.value == '3005' || this.value == '3161' || this.value == '3181' || this.value == '3019') {
                        jQuery('.dTreeNode:contains("' + stPergunta + '")').show();
                        if (idPergunta3IE) {
                            document.getElementById('iarvore' + idPergunta3IE).style.display = '';
                            document.getElementById('sarvore' + idPergunta3IE).style.display = '';
                        }
                    }

                    var qrpid = "<?= $qrpid ?>";
                    jQuery.ajax({
                        type: "POST",
                        url: 'ajax.php',
                        data: 'requisicao=questionarioProfuncionario&qrpid=' + qrpid,
                        success: function(html) {
                            idPergunta3IE = bloqueiaP3IE();
                            if (html.substr(0, 1) == 'N') {
                                if (perid == '1558' || perid == '1559') {
                                    jQuery('#proximo').attr('disabled', true);
                                    jQuery('#salvar_proximo').attr('disabled', true);
                                    document.getElementById('proximo').disabled = true;
                                    document.getElementById('salvar_proximo').disabled = true;
                                }
                            }
                        }
                    });

        <?php if ($bloqueiaBotoes) { ?>
                        jQuery('input:button,:radio,:checkbox,:text').attr('disabled', true);
        <?php } ?>

                });

        <?php if ($boBloqueia == 'Não.'): ?>
                    jQuery('.dTreeNode:contains("' + stPergunta + '")').hide();
                    idPergunta3IE = bloqueiaP3IE();
                    if (idPergunta3IE) {
                        document.getElementById('iarvore' + idPergunta3IE).style.display = 'none';
                        document.getElementById('sarvore' + idPergunta3IE).style.display = 'none';
                    }
        <?php endif; ?>

        <?php if ($bloqueiaBotoes) { ?>
                    jQuery('input:button,:radio,:checkbox,:text').attr('disabled', true);
        <?php } ?>

            });

            function bloqueiaP3IE() {
                //recupera id pergunta3 para browser ie
                var t = document.getElementsByTagName("a");
                var idPergunta3IE = "";
                for (i = 0; i < t.length; i++) {
                    if (t[i].id.substr(0, 7) == 'sarvore') {
                        if (t[i].innerHTML.indexOf('3 - ') == 1) {
                            idPergunta3IE = t[i].id.substr(7);
                            break;
                        }
                    }
                }
                return idPergunta3IE;

                //fim recupera id pergunta3 para browser ie
            }


            function popupSelecionarMunicipios(perid, qrpid) {
                return windowOpen('/par/par.php?modulo=principal/selecionaMunicipios&acao=A&qrpid=' + qrpid + '&perid=' + perid,
                        'selecionaMunicipios', 'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
            }

            function isArray(o) {
                return(typeof (o.length) == "undefined") ? false : true;
            }

            function geraTabela(valor) {

                var html = '';

                var arr = new Array();

                j = $("#quantidadeBanco").val();

                if (j) {
                    for (i = 0; i < j; i++) {
                        arr[i] = new Array();
                        arr[i]["cpf"] = $("#cpf_" + i).val();
                        arr[i]["nome"] = $("#nome_" + i).val();
                        arr[i]["email"] = $("#email_" + i).val();
                        arr[i]["ano"] = $("#ano_" + i).val();
                        arr[i]["emexercicio"] = $("#emexercicio_" + i).val();
                    }
                }

                if (valor > 0) {
                    html += "<tr>";
                    html += "<td bgcolor='#A8A8A8'><b>CPF</b></td>";
                    html += "<td bgcolor='#A8A8A8' width='25%'><b>Nome</b></td>";
                    html += "<td bgcolor='#A8A8A8'><b>E-mail</b></td>";
                    html += "<td bgcolor='#A8A8A8'><b>Ano de Formação</b></td>";
                    if (<?php echo $questionario; ?> == <?php echo QUESTIONARIOPROMUN; ?>) {
                        var txtQueid = 'municipal';
                    } else {
                        var txtQueid = 'estadual';
                    }
                    html += "<td bgcolor='#A8A8A8'><b>Em exercício na rede<br>" + txtQueid + " de ensino?</b></td>";
                    html += "</tr>";
                    //getElementById('tabpro').style.display = "block";
                    $("#tabpro").css('display', 'block');

                    for (i = 0; i < valor; i++) {
                        if (i < j) {
                            cpf = arr[i]['cpf'];
                            nome = arr[i]['nome'];
                            email = arr[i]['email'];
                            ano = arr[i]['ano'];
                            emexercicio = arr[i]['emexercicio'];
                        } else {
                            cpf = "";
                            nome = "";
                            email = "";
                            ano = "";
                            emexercicio = "";
                        }
                        html += "<tr>";
                        html += "<td><input type='text' name='cpf[]' class='normal classcpf' title='CPF' onkeyup='this.value=mascaraglobal(\"###.###.###-##\", this.value);' class='CampoEstilo' id='cpf' value='" + cpf + "' style='width: 100%' maxlength='14' /></td>";
                        html += "<td><input type='hidden' name='nome[]' title='NOME' id='nome' value='" + nome + "' style='width: 100%' maxlength='50' /><label>" + nome + "</label></td>";
                        html += "<td><input type='text' name='email[]' title='EMAIL' id='email' value='" + email + "' style='width: 100%' maxlength='30' /></td>";
                        html += "<td><select name='ano[]'  style='width: 100%'>";
                        html += "<option value='0'>...</option>";
                        html += "<option " + (ano == 2005 ? 'selected' : '') + " value='2005'>2005</option>";
                        html += "<option " + (ano == 2006 ? 'selected' : '') + " value='2006'>2006</option>";
                        html += "<option " + (ano == 2007 ? 'selected' : '') + " value='2007'>2007</option>";
                        html += "<option " + (ano == 2008 ? 'selected' : '') + " value='2008'>2008</option>";
                        html += "<option " + (ano == 2009 ? 'selected' : '') + " value='2009'>2009</option>";
                        html += "<option " + (ano == 2010 ? 'selected' : '') + " value='2010'>2010</option>";
                        html += "</select></td>";
                        html += "<td><select name='emexercicio[]' style='width: 100%'>";
                        html += "<option value='0'>...</option>";
                        html += "<option " + (emexercicio == 's' ? 'selected' : '') + " value='s'>Sim</option>";
                        html += "<option " + (emexercicio == 'n' ? 'selected' : '') + " value='n'>Não</option>";
                        html += "</select></td>";
                        html += "</tr>";
                    }
                    $("#tabpro").html(html);
                }

                //	alert(html);
                return true;
            }

        </script>

        <center>
            <table bgcolor="#f5f5f5" align="center" class="tabela" >
                <tr>
                    <td width="90%" >
                        <fieldset style="width: 97%; background: #fff;"  >
                            <legend>Questionário</legend>
                            <?php
                                $tela = new Tela(array("qrpid" => $qrpid, 'tamDivPx' => 355, 'descricao' => 'N', 'habilitado' => $habilitado));
                            ?>
                        </fieldset>
                    </td>
                    <td align="center" valign="top">
                        <?
                            $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                            wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                        ?>
                    </td>
                </tr>
                </td>
            </table>
        </center>

        <?php  //direciona para o codigo da pergunta
            if ($_REQUEST['perguntaid']) {
        ?>
                <script type="text/javascript">
                    quest.atualizaTela(<?= $_REQUEST['perguntaid'] ?>);
                </script>
        <?php }

    } else {

        if ($_SESSION['par']['prgid'] == 153) { //153 = FNDE - Programa Caminho da Escola - Ônibus Escolar Acessível
            monta_titulo($titulo_modulo, '');
        ?>
            <center>
            <table bgcolor="#f5f5f5" align="center" class="tabela" >
                <tr>
                    <td width="90%" valign="top" >
                        <fieldset style="width: 97%; background: #fff;">
                            <legend>Instruções</legend>
                            <?php
                                echo $tapinstrucao;
                            ?>
                        </fieldset>
                        <center>
                            <p><input type="button" value="Sair" onclick="javascript:document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" /></p>
                        </center>
                    </td>
                    <td align="center" valign="top">
                        <?php
                            $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                            wf_desenhaBarraNavegacao(pgTransporteCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                        ?>
                    </td>
                </tr>
                </td>
            </center>
    <?php
        } elseif ($_SESSION['par']['prgid'] == 183) { //183 = Programa Água na Escola
            echo "<script type=\"text/javascript\">
                        jQuery(function(){
                            jQuery( '#dialog-confirm' ).attr({'title':'AVISO'});
                            jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                            jQuery( '#dialog-content' ).html('As escolas serão atendidadas considerando o limite orçamentário do exercício. As demais escolas, que aderiram, incluiram as fotos e enviarem para análise, serão atendidas nos próximos exercícios.<br><br> Atenciosamente,<br>Equipe PDDE Água na Escola.');
                            jQuery( '#dialog-confirm' ).dialog({
                                resizable: false,
                                width: 550,
                                modal: true,
                                buttons: {
                                    'Ok': function() {
                                        jQuery( this ).dialog( 'close' );
                                    }
                                }
                            });
                        });
	    	  </script>";
    ?>
            <center>
                <table bgcolor="#f5f5f5" align="center" class="tabela" >
                    <tr>
                        <td width="90%" valign="top" >
                            <fieldset style="width: 97%; background: #fff;"  >
                                <legend>Instruções</legend>
                                <?php
                                    echo $tapinstrucao;
                                ?>
                            </fieldset>
                            <center>
                                <p><input type="button" value="Sair" onclick="javascript:document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" /></p>
                            </center>
                        </td>
                        <td align="center" valign="top">
                            <?php
                                $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                                wf_desenhaBarraNavegacao(pgAguaEscolaCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                            ?>
                        </td>
                    </tr>
                    </td>
            </center>

    <?php
        } elseif ($_SESSION['par']['prgid'] == 219) { //214 = Programa Saúde na Escola
            if (in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
                in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
                in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO, $perfil) ||
                in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE, $perfil)) {


                //verifica se existe secretario estadual de educação
                if (in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO, $perfil)) {
                    $sql = "
                        select usunome
                        from seguranca.usuario u
                        inner join par.usuarioresponsabilidade ur
                                on ur.usucpf = u.usucpf
                                and ur.rpustatus = 'A'
                                and ur.pflcod = " . PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO . "
                                and ur.estuf = '" . $_SESSION['par']['estuf'] . "'
                                and ur.usucpf = '" . $_SESSION['usucpf'] . "'
                        inner join seguranca.usuario_sistema us
                                on us.usucpf = ur.usucpf
                                and us.suscod='A'
                                and us.sisid = " . SISID_PAR;
                    $nomeSecEduc = $db->pegaUm($sql);

                    if (!$nomeSecEduc) {
                        echo "
                            <script type=\"text/javascript\">
                                jQuery(function(){
                                    jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                                    jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                                    jQuery( '#dialog-content' ).html('Não Existe UF vincalada para seu perfil de Secretário Estadual de Educação. Por favor entre em contato com o Gestor do Sistema.');
                                    jQuery( '#dialog-confirm' ).dialog({
                                        resizable: false,
                                        width: 550,
                                        modal: true,
                                        buttons: {
                                            'Ok': function() {
                                                jQuery( this ).dialog( 'close' );
                                                document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
                                            }
                                        }
                                    });
                                });
                            </script>";
                        exit;
                    }
                }

                //verifica se existe secretario estadual de saude
                if (in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE, $perfil)) {
                    $sql = "
                        select usunome
                        from seguranca.usuario u
                        inner join par.usuarioresponsabilidade ur
                                on ur.usucpf = u.usucpf
                                and ur.rpustatus = 'A'
                                and ur.pflcod = " . PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE . "
                                and ur.estuf = '" . $_SESSION['par']['estuf'] . "'
                                and ur.usucpf = '" . $_SESSION['usucpf'] . "'
                        inner join seguranca.usuario_sistema us
                                on us.usucpf = ur.usucpf
                                and us.suscod='A'
                                and us.sisid = " . SISID_PAR;
                    $nomeSecSaude = $db->pegaUm($sql);

                    if (!$nomeSecSaude) {
                        echo "
                            <script type=\"text/javascript\">
                                jQuery(function(){
                                    jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                                    jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                                    jQuery( '#dialog-content' ).html('Não Existe UF vincalada para seu perfil de Secretário Estadual de Saúde. Por favor entre em contato com o Gestor do Sistema.');
                                    jQuery( '#dialog-confirm' ).dialog({
                                        resizable: false,
                                        width: 550,
                                        modal: true,
                                        buttons: {
                                            'Ok': function() {
                                                jQuery( this ).dialog( 'close' );
                                                document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
                                            }
                                        }
                                    });
                                });
                        </script>";
                        exit;
                    }
                }

                //faz adesão estadual
                $sql = "select pfpid, usucpfseceducacao, usucpfsecsaude from par.pfpse where adpid = " . $_SESSION['par']['adpid'];
                //dbg($sql,1);
                $dadospse = $db->pegaLinha($sql);

                $pfpid = $dadospse['pfpid'];
                $usucpfseceducacao = $dadospse['usucpfseceducacao'];
                $usucpfsecsaude = $dadospse['usucpfsecsaude'];

                if (!$pfpid) {
                    $sql = "
                        INSERT INTO par.pfpse(
                            adpid,
                            pfpdatainclusao,
                            estuf
                        )VALUES(
                            {$_SESSION['par']['adpid']},
                            now(),
                            '{$_SESSION['par']['estuf']}'
                        ) returning pfpid";
                    $pfpid = $db->pegaUm($sql);
                }

                if (in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO, $perfil)) {
                    if (!$usucpfseceducacao) {
                        $usucpfseceducacao = $_SESSION['usucpf'];
                        $sql = "UPDATE par.pfpse SET usucpfseceducacao='" . $usucpfseceducacao . "' WHERE pfpid = $pfpid";
                        $db->executar($sql);
                    }
                }

                if (in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE, $perfil)) {
                    if (!$usucpfsecsaude) {
                        $usucpfsecsaude = $_SESSION['usucpf'];
                        $sql = "UPDATE par.pfpse SET usucpfsecsaude='" . $usucpfsecsaude . "' WHERE pfpid = $pfpid";
                        $db->executar($sql);
                    }
                }

                $db->commit();

                if (!$usucpfseceducacao || !$usucpfsecsaude) {
                    unset($aderiu);

                    $sql = "UPDATE par.pfadesaoprograma SET adpresposta=null WHERE adpid=" . $_SESSION['par']['adpid'];
                    $db->executar($sql);
                    $db->commit();

                    if (!$usucpfseceducacao) {
                        $msgpse = "Adesão em Processo.<br><br>O Secretário(a) Estadual da Educação ainda não aderiu ao programa. Aguarde a adesão do mesmo para fazer a impressão do Termo de Compromisso.";
                    }

                    if (!$usucpfsecsaude) {
                        $msgpse = "Adesão em Processo.<br><br>O Secretário(a) Estadual de Saúde ainda não aderiu ao programa. Aguarde a adesão do mesmo para fazer a impressão do Termo de Compromisso.";
                    }
                }

                if ($usucpfseceducacao && $usucpfsecsaude) {
                    $msgpse = "Adesão Concluída.<br><br>Imprima abaixo o Termo de Compromisso.";
                }
            } else {
                echo "
                    <script type=\"text/javascript\">
                        jQuery(function(){
                            jQuery( '#dialog-confirm' ).attr({'title':'Alerta'});
                            jQuery( '#dialog:ui-dialog' ).dialog( 'destroy' );
                            jQuery( '#dialog-content' ).html('Você não tem permissão a este programa');
                            jQuery( '#dialog-confirm' ).dialog({
                                resizable: false,
                                width: 550,
                                modal: true,
                                buttons: {
                                    'Ok': function() {
                                            jQuery( this ).dialog( 'close' );
                                            document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
                                    }
                                }
                            });
                        });
                    </script>";
                exit;
            }
    ?>
    <script>
        function PrintElementID(id, pg) {
            var oPrint, oJan;
            oPrint = window.document.getElementById(id).innerHTML;
            oJan = window.open(pg);
            oJan.document.write(oPrint);
            oJan.history.go();
            oJan.window.print();
        }
    </script>

    <center>
        <table bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td width="90%" valign="top" >
                    <fieldset style="width: 97%; background: #fff;"  >
                        <legend>Instruções</legend>
                        <center>
                            <b>
                            <?php
                                echo $msgpse;
                            ?>
                            </b>
                        </center>
                        <br>
                        <table width="50%" class="listagem" cellspacing="3" cellpadding="3" bgcolor="#f5f5f5" align="center" >
                            <tr>
                                <td width="20%" class="SubTituloCentro">Secretário(a)</td>
                                <td class="SubTituloCentro">Nome</td>
                                <td width="20%" class="SubTituloCentro">Aderiu?</td>
                            </tr>
                            <tr>
                                <td align="center">EDUCAÇÃO</td>
                                <td nowrap="nowrap">
                                    <?php
                                        $sql = "
                                            select usunome
                                            from seguranca.usuario u
                                            inner join par.usuarioresponsabilidade ur
                                                    on ur.usucpf = u.usucpf
                                                    and ur.rpustatus = 'A'
                                                    and ur.pflcod = " . PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO . "
                                                    and ur.estuf = '" . $_SESSION['par']['estuf'] . "'
                                            inner join seguranca.usuario_sistema us
                                                    on us.usucpf = ur.usucpf
                                                    and us.suscod='A'
                                                    and us.sisid = " . SISID_PAR;
                                        $nomeSecEduc = $db->pegaUm($sql);
                                        if ($nomeSecEduc) {
                                            echo $nomeSecEduc;
                                        } else {
                                            echo '<center><font color=red>Não existe Secretário Estadual de Educação para este Estado.</font></center>';
                                        }
                                    ?>
                                </td>
                                <td align="center">
                                    <?php
                                        if ($usucpfseceducacao) {
                                            echo '<font color=blue>SIM</font>';
                                        } else {
                                            echo '<font color=red>NÃO</font>';
                                        }
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td align="center">SAÚDE</td>
                                <td nowrap="nowrap">
                                    <?php
                                        $sql = "
                                            select usunome
                                            from seguranca.usuario u
                                            inner join par.usuarioresponsabilidade ur
                                                    on ur.usucpf = u.usucpf
                                                    and ur.rpustatus = 'A'
                                                    and ur.pflcod = " . PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE . "
                                                    and ur.estuf = '" . $_SESSION['par']['estuf'] . "'
                                            inner join seguranca.usuario_sistema us
                                                    on us.usucpf = ur.usucpf
                                                    and us.suscod='A'
                                                    and us.sisid = " . SISID_PAR;
                                        $nomeSecSaude = $db->pegaUm($sql);
                                        if ($nomeSecSaude) {
                                            echo $nomeSecSaude;
                                        } else {
                                            echo '<center><font color=red>Não existe Secretário Estadual de Saúde para este Estado.</font></center>';
                                        }
                                    ?>
                                </td>
                                <td align="center">
                                    <?php
                                    if ($usucpfsecsaude) {
                                        echo '<font color=blue>SIM</font>';
                                    } else {
                                        echo '<font color=red>NÃO</font>';
                                    }
                                    ?>
                                </td>
                            </tr>
                        </table>
                        <br><br>

                <?php if ($usucpfseceducacao && $usucpfsecsaude) { ?>
                        <center>
                        <div id="div_termo_pse" class="div_termo_pse" style="overflow: auto; height:150px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="center" >
                            <center>
                                <?php
                                    $taptermo = preencheCamposTexto($taptermo);
                                    $taptermo = str_replace("Secretário de Educação e de Saúde", "Secretário de Educação, $nomeSecEduc, inscrito no CPF sob o nº " . formatar_cpf($usucpfseceducacao) . " e de Saúde, $nomeSecSaude, inscrito no CPF sob o nº " . formatar_cpf($usucpfsecsaude), $taptermo);
                                    echo $taptermo;
                                ?>
                                <br>
                                <table width="100%">
                                    <tr>
                                        <td align="center">________________________________________________<br> <?= $nomeSecEduc ?> <br> Secretário Estadual de Educação </td>

                                        <td align="center"> ________________________________________________<br> <?= $nomeSecSaude ?> <br> Secretário Estadual de Saúde </td>
                                    </tr>
                                </table>
                            </center>
                        </div>
                        <center>
                        <br>
                        <center>
                            <input type="button" value="Imprimir" onclick="PrintElementID('div_termo_pse', 'par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=<?php echo $_SESSION['par']['pfaid']; ?>')"/>
                        </center>
                <?php } ?>

                    </fieldset>
                    <center>
                        <p><input type="button" value="Sair" onclick="javascript:document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" /></p>
                    </center>
                </td>
                <td align="center" valign="top">
                    <?php
                        $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                        wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                    ?>
                </td>
            </tr>
            </td>
        </center>

    <?PHP
        } else {
            monta_titulo($titulo_modulo, '');
    ?>
        <script>
            jQuery(function() {
                jQuery('#link_termo_pactuacao').click(function() {
                    jQuery('.div_termo_pactuacao, .img_termo_pactuacao_maismenos').toggle();
                });

                jQuery('#link_termo_adesao').click(function() {
                    jQuery('.div_termo_adesao, .img_termo_adesao_maismenos').toggle();
                });
            });

            function PrintElementID(id, pg) {
                var oPrint, oJan;
                oPrint = window.document.getElementById(id).innerHTML;
                oJan = window.open(pg);
                oJan.document.write(oPrint);
                oJan.history.go();
                oJan.window.print();
                //oPrint.print();
            }
        </script>

        <center>
        <table bgcolor="#f5f5f5" align="center" class="tabela listagem" >
            <tr>
                <td width="90%" valign="top" >
                <br>
                <?php if (!empty($tappacto)): ?>
                <fieldset style="width: 97%; background: #fff; margin-bottom:35px;"  >
                    <legend>
                        <img src="../imagens/mais.gif" class="img_termo_pactuacao_maismenos" />
                        <img src="../imagens/menos.gif" class="img_termo_pactuacao_maismenos" style="display:none;"/>
                        <a href="javascript:void(0)" id="link_termo_pactuacao">Adesão ao Pacto</a>
                    </legend>
                    <div id="div_termo_pactuacao" class="div_termo_pactuacao" style="overflow: auto; margin-bottom:15px; display: none;">
                        <center>
                            <?php
                                $tappacto = preencheCamposTexto($tappacto);
                                echo $tappacto;
                            ?>
                        </center>
                    </div>
                    <center>
                        <input type="button" class="div_termo_pactuacao" style="display:none;" value="Imprimir" onclick="PrintElementID('div_termo_pactuacao', 'par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=<?php echo $_SESSION['par']['pfaid']; ?>')"/>
                    </center>
                </fieldset>
            <?php endif; ?>

            <?php
                #PROGRMA FINANCIAMENTO EJA
                $docid = pgCriarDocumento($_SESSION['par']['adpid']);
                $estado = pgPegarEstadoAtual($docid);

                if ($_SESSION['par']['prgid'] == PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA ){

                    include_once '_funcoes_eja.php';

                    if( !empty($taptermo) ) {
            ?>
                        <fieldset style="width: 97%; background: #fff; margin-bottom:35px;"  >
                            <legend>
                                <a href="javascript:void(0)" id="link_termo_adesao">
                                    <img src="../imagens/mais.gif" class="img_termo_adesao_maismenos" style="display:none;"/>
                                    <img src="../imagens/menos.gif" class="img_termo_adesao_maismenos"/>

                                    <?php
                                        $titulo_modulo = 'Adesão ao Programa';
                                        if ($taptituloadesao) {
                                            $titulo_modulo = $taptituloadesao;
                                        }
                                        echo $titulo_modulo;
                                    ?>

                                </a>
                            </legend>
                            <div id="div_termo_adesao" class="div_termo_adesao" style="overflow: auto; margin-bottom:15px;">
                                <center>
                                    <?php
                                        $taptermo = preencheCamposTexto($taptermo);
                                        echo $taptermo;

                                        dadosImpressaoTermoAdsao();
                                    ?>
                                </center>
                            </div>
                            <center>
                            <?php if( $estado == WF_EJA_APROVADO ){ ?>
                                    <input type="button" class="div_termo_adesao" value="Imprimir" onclick="PrintElementID('div_termo_adesao', 'par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=<?php echo $_SESSION['par']['pfaid']; ?>')"/>
                            <?php } ?>
                            </center>
                        </fieldset>
            <?php
                    }
                }

                #PROGRAMA EJA PRONATEC
                $docid = pgCriarDocumento($_SESSION['par']['adpid']);
                $estado = pgPegarEstadoAtual($docid);

                if (in_array($_SESSION['par']['prgid'], array( PROG_PAR_EJA_PRONATEC )) ){

                    include_once '_funcoes_eja_pronatec.php';
                    if( !empty($taptermo) ) {
            ?>
                        <fieldset style="width: 98%; background: #fff; margin-left: 25px; margin-bottom:55px;"  >
                            <legend>
                                <a href="javascript:void(0)" id="link_termo_adesao">
                                    <img src="../imagens/mais.gif" class="img_termo_adesao_maismenos" style="display:none;"/>
                                    <img src="../imagens/menos.gif" class="img_termo_adesao_maismenos"/>

                                    <?php
                                        $titulo_modulo = 'Adesão ao Programa';
                                        if ($taptituloadesao) {
                                            $titulo_modulo = $taptituloadesao;
                                        }
                                        echo $titulo_modulo;
                                    ?>
                                </a>
                            </legend>
                            <div id="div_termo_adesao" class="div_termo_adesao" style="overflow: auto; margin-bottom:15px;">
                                <center>
                                    <?php
                                        $taptermo = preencheCamposTexto($taptermo);
                                        echo $taptermo;

                                        if( $estado != WF_EJA_PRONATEC_EM_PREENCHIMENTO_UNIDADE && $estado != WF_EJA_PRONATEC_CANCELADO ){
                                            echo dadosImpressaoTermoAdsaoPronatec();
                                        }
                                    ?>
                                </center>
                            </div>
                            <center>
                            <?php if( $estado == WF_EJA_APRONATEC_PROVADO ){ ?>
                                    <input type="button" class="div_termo_adesao" value="Imprimir" onclick="PrintElementID('div_termo_adesao', 'par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=<?php echo $_SESSION['par']['pfaid']; ?>')"/>
                            <?php } ?>
                            </center>
                        </fieldset>
            <?php
                    }
                }
            ?>

            <?php

            ?>

            <?php

            ?>

            <?php if (!empty($taptermo) && ( $_SESSION['par']['prgid'] != PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA && $_SESSION['par']['prgid'] != PROG_PAR_EJA_PRONATEC ) ): ?>
                <fieldset style="width: 97%; background: #fff; margin-bottom:35px;"  >
                    <legend>
                        <a href="javascript:void(0)" id="link_termo_adesao">
                            <img src="../imagens/mais.gif" class="img_termo_adesao_maismenos" style="display:none;"/>
                            <img src="../imagens/menos.gif" class="img_termo_adesao_maismenos"/>

                            <?php
                                $titulo_modulo = 'Adesão ao Programa';
                                if ($taptituloadesao) {
                                    $titulo_modulo = $taptituloadesao;
                                }
                                echo $titulo_modulo;
                            ?>
                        </a>
                    </legend>
                    <div id="div_termo_adesao" class="div_termo_adesao" style="overflow: auto; margin-bottom:15px; display: none;">
                        <center>
                            <?php
                                $taptermo = preencheCamposTexto($taptermo);
                                echo $taptermo;
                            ?>
                        </center>
                    </div>

                    <?php if($_SESSION['par']['prgid'] == PROG_PAR_ADESAO_CONSELHO_ESCOLAR): ?>
                        <fieldset style="width: 97%; background: #fff;">
                            <legend>Instruções</legend>
                                <p style="text-align: center;">Instru&ccedil;&otilde;es</p>
                                <p style="text-align:justify;">
                                    &nbsp;&nbsp;&nbsp;&nbsp; Nesta oportunidade, a Secretaria de Educa&ccedil;&atilde;o que aceitou o Termo de  Ades&atilde;o &agrave;
                                    oferta do Curso de Forma&ccedil;&atilde;o para Conselheiros Escolares dever&aacute; indicar  um servidor para ser o respons&aacute;vel
                                    pela interlocu&ccedil;&atilde;o com a Coordena&ccedil;&atilde;o Estadual  do Programa, clicando na Aba INTERLOCUTOR, localizada na
                                    parte superior  esquerda desta tela.
                                </p>
                                <p style="text-align:justify;">
                                    &nbsp;&nbsp;&nbsp;&nbsp; Os contatos com a Secretaria de Educa&ccedil;&atilde;o que aderiu a este Termo ser&atilde;o  realizados pela
                                    Coordena&ccedil;&atilde;o Estadual que &eacute; a respons&aacute;vel por planejar e executar  a oferta do Curso no &acirc;mbito dessa
                                    unidade da federa&ccedil;&atilde;o, de acordo com as  orienta&ccedil;&otilde;es emanadas do Programa Nacional de Fortalecimento dos Conselhos
                                    Escolares.
                                </p>
                                <br />
                                <br />
                                <br />
                        </fieldset>
                    <?php endif; ?>

                    <center>
                        <input type="button" style="display:none;" class="div_termo_adesao" value="Imprimir" onclick="PrintElementID('div_termo_adesao', 'par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=<?php echo $_SESSION['par']['pfaid']; ?>')"/>
                    </center>
                </fieldset>
            <?php if ($_SESSION['par']['prgid'] == 218){ ver('ops', d);?>
                    <script>
                        jQuery('.div_termo_adesao, .img_termo_adesao_maismenos').toggle();
                    </script>
            <?php } ?>
            <?php endif; ?>

            <?php if (!empty($tapinstrucao)): ?>
                    <fieldset style="width: 97%; background: #fff;">
                        <legend>Instruções</legend>
                        <?php echo $tapinstrucao; ?>
                    </fieldset>
            <?php
                endif;

                if ( $_SESSION['par']['prgid'] != PROG_PAR_PROGRAMA_FINANCIAMENTO_EJA && $_SESSION['par']['prgid'] != PROG_PAR_EJA_PRONATEC){
            ?>
                <center>
                    <p><input type="button" value="Sair" onclick="javascript:document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';" /></p>
                </center>

            <?php } ?>

            </td>
            <td align="center" valign="top">
                <?php
                    $dados_wf = array('adpid' => $_SESSION['par']['adpid']);
                    wf_desenhaBarraNavegacao(pgCriarDocumento($_SESSION['par']['adpid']), $dados_wf);
                ?>
            </td>
        </tr>
    </td>
    </center>
<?php
        }
    }
}
?>

<div id="dialog-confirm" title="Confirmação" style="display:none;">
    <p>
        <span class="ui-icon ui-icon-alert" style="float:left; margin:0 7px 5px 0;"></span>
        <div id="dialog-content"></div>
    </p>
</div>