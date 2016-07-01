<?PHP
    include_once APPRAIZ . "includes/classes/dateTime.inc";

    if ($_REQUEST['mostra']) {

        $sql = "
            SELECT  pi.plicod as codigo,
                    pi.plicod || ' - ' || ug.ungabrev as descricao,
                    ug.ungabrev
            FROM monitora.pi_planointerno pi
            INNER JOIN monitora.pi_subacao s ON s.sbaid = pi.sbaid
            INNER JOIN monitora.pi_subacaounidade sau ON sau.sbaid = s.sbaid
            INNER JOIN public.unidadegestora ug ON ug.ungcod = sau.ungcod
            INNER JOIN evento.unidadeparceira up ON up.ungcod = ug.ungcod
            WHERE pi.plistatus = 'A'
            AND pi.pliano = '" . $_REQUEST['anopi'] . "'
            --AND ug.ungcod IN (select ungcod from evento.unidadeparceira where eveid)
            ORDER BY ug.ungabrev, pi.plicod
        ";

        $evenumeropi = $_REQUEST['nrpi'];
        $db->monta_combo('evenumeropi', $sql, 'S', 'Selecione o PI...', '', '', '', '', 'N', 'evenumeropi');
        exit();
    }

    $perfis = arrayPerfil();

    if ($_POST['action'] == 'grava') {
        if ($_SESSION['evento']['eveid']) {
            salvar($_SESSION['evento']['eveid']);
        }
    }

    if ($_SESSION['evento']['eveid']) {
        $sql = "
            SELECT  ev.evetitulo,
                    ev.ungcod,
                    ev.tpeid,
                    ev.evedatainicio,
                    ev.evedatafim,
                    ev.eveemail,
                    ev.everespnome,
                    ev.everesptelefone,
                    ev.evenumeropi,
                    ev.evenumeroprocesso,
                    ev.evecustoprevisto,
                    ev.evepublicoestimado,
                    ev.evequantidadedias,
                    ev.muncod,
                    ev.estuf,
                    ev.sevid,
                    ev.eveqtdpassagemaerea,
                    u.ungdsc,
                    us.usunome,
                    ev.docid,
                    ev.eveurgente,
                    aval.aevid,
                    ev.endid,
                    to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao,
                    ev.eveanopi
            FROM evento.evento AS ev
            LEFT JOIN  evento.tipoevento AS    te ON te.tpeid  = ev.tpeid
            LEFT JOIN  public.unidadegestora AS u ON ev.ungcod = u.ungcod
            LEFT JOIN  seguranca.usuario AS    us ON us.usucpf = ev.usucpf
            LEFT JOIN  evento.avaliacaoevento aval ON aval.eveid = ev.eveid
            WHERE ev.eveid = {$_SESSION['evento']['eveid']}
        ";
        $rsDadosEvento = $db->carregar($sql);
    }


    function salvar( $id = null ){
        global $db;

        if ($_POST['adreverendo']) {
            $adreferendum = "t";
            $eveurgente = "t";
        } else {
            $adreferendum = "f";
            $eveurgente = "f";
        }

        if ($_SESSION['evento']['eveid']) {
            $sql = "
                UPDATE evento.evento
                    SET evenumeropi         = '{$_POST['evenumeropi']}',
                        --evenumeroprocesso = ".( strlen($_POST['evenumeroprocesso']) != 20 ? "null" : "'" . $_POST['evenumeroprocesso'] . "'" ).",
                        eveurgente          = '{$eveurgente}',
                        sevid               = 2,
                        eveanopi            = '{$_POST['eveanopi']}'
                WHERE eveid = {$_SESSION['evento']['eveid']}
            ";

            if ($db->executar($sql)) {
                $db->commit();
                echo"<script>alert('Dados gravados com sucesso.');window.location.href = 'evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A';</script>";
            }
        }
        return true;
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';

    $titulo_modulo = "Eventos";
    monta_titulo($titulo_modulo, 'Estrutura Orçamentária.');

    $res = array(
        0 => array("descricao" => "Lista",
            "id" => "4",
            "link" => "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
        ),
        1 => array("descricao" => "Informações Básicas",
            "id" => "4",
            "link" => "/evento/evento.php?modulo=principal/cadEvento&acao=A"
        )
    );

    if (( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '')) {
        array_push($res, array("descricao" => "Documentos Anexos",
            "id" => "3",
            "link" => "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
            ), array("descricao" => "Infraestrutura",
            "id" => "2",
            "link" => "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
            )
        );

        $pflcod = pegaPerfil($_SESSION['usucpf']);
    }

    if ($_SESSION['evento']['eveid']) {
        array_push($res, array("descricao" => "Estrutura Orçamentária",
            "id" => "6",
            "link" => "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
            )
        );
    }

    if (mostraAbaDocOS($_SESSION['evento']['eveid'])) {
        array_push($res, array("descricao" => "Ordem de Serviço",
            "id" => "7",
            "link" => "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
            )
        );
    }

    if (mostraAbaDocPagamento($_SESSION['evento']['eveid'])) {
        array_push($res, array("descricao" => "Documento de Pagamento",
            "id" => "5",
            "link" => "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
            )
        );
    }

    if (( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '')) {
        array_push($res, array("descricao" => "Avaliação",
            "id" => "0",
            "link" => "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
            )
        );
    }

    if ($_SESSION['evento']['eveid']) {
        echo'<br><br>';
        echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A");
    }

    headEvento($rsDadosEvento[0]['evetitulo'], $rsDadosEvento[0]['everespnome'], $rsDadosEvento[0]['ungdsc'], $rsDadosEvento[0]['ungcod'], $rsDadosEvento[0]['eveurgente'], $rsDadosEvento[0]['evedatainclusao'], false);

    $docid = evtCriarDoc($_SESSION['evento']['eveid']);
    $esdid = verificaEstadoDocumento($docid);

    $ativo = 'N';
    if ($esdid == EM_CADASTRAMENTO_WF) {
        $ativo = 'S';
    }
?>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>

    <script type="text/javascript">

        function alertaPI() {
            alert("Escolha um ano para listar os PIs.");
            document.getElementById('anoPI').focus();
            return false;
        }

        function pegaNumerosPIs(nrpi) {
            var ano = document.formulario.eveanopi.value;

            if (ano == null) {
                ano = $('eveanopi').value;
            }

            if (ano == '' || ano == null) {
                alert("Selecione um ano do PI!");
                $('eveanopi').focus();
                return false;
            }

            var tdPI = document.getElementById('comboPI');

            new Ajax.Request('evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A', {
                method: 'post',
                parameters: '&mostra=true&anopi=' + ano + '&nrpi=' + nrpi,
                onComplete: function(res){
                    tdPI.innerHTML = res.responseText;
                }
            });
        }

        function validaForm(tipo){
            var evenumeropi = document.getElementById('evenumeropi').value;

            if (tipo == 'grava'){
                document.getElementById('action').value = tipo;
                document.formulario.submit();
            }

            if (tipo == 'cancel'){
                window.location.href = "evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A";
            }
        }


        function setPassagem(value){
            var lb_sim = document.getElementById('lb_sim');
            if(value == 't'){
                var content = " - Quantidade: <input type='text' value='' size='4' onkeyup=\"this.value=mascaraglobal('#####',this.value);\" name='eveqtdpassagemaerea' id='eveqtdpassagemaerea'><img src='../imagens/obrig.gif' title='Indica campo obrigatório.' border='0'>";
                lb_sim.innerHTML = content;
                top.temPassagem = 's';
            }else{
                lb_sim.innerHTML = '';
                top.temPassagem = 'n';
            }
        }

        function abreMapa() {
            var graulatitude = window.document.getElementById("graulatitude").value;
            var minlatitude = window.document.getElementById("minlatitude").value;
            var seglatitude = window.document.getElementById("seglatitude").value;
            var pololatitude = window.document.getElementById("pololatitude").value;

            var graulongitude = window.document.getElementById("graulongitude").value;
            var minlongitude = window.document.getElementById("minlongitude").value;
            var seglongitude = window.document.getElementById("seglongitude").value;

            var latitude = (((Number(seglatitude) / 60) + Number(minlatitude)) / 60) + Number(graulatitude);
            var longitude = (((Number(seglongitude) / 60) + Number(minlongitude)) / 60) + Number(graulongitude);
            var eveid = document.getElementById("eveid").value;
            var janela = window.open('evento.php?modulo=principal/mapaEvento&acao=A&longitude=' + longitude + '&latitude=' + latitude + '&polo=' + pololatitude + '&eveid' + eveid, 'mapa', 'height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

        }

        function validaMascaraProcesso(valor){
            if (valor.length != 20 && valor.search("/") != 12 && valor.search("/") != 17){
                alert('Número de Processo inválido!');
            }
        }

    </script>

    <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
        <input type="hidden" name="action" value="0" id="action">
        <input type="hidden" name="eveid" value=",<?= $_SESSION['evento']['eveid']; ?>" id="eveid">

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
            <tr>
                <td vAlign="top">
                    <table bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" width="100%">
                        <tr>
                            <td class ="SubTituloDireita">Ano do PI: </td>
                            <td>
                                <?PHP
                                    $eveanopi = $rsDadosEvento[0]["eveanopi"];
                                    if (!$eveanopi) {
                                        $eveanopi = date('Y');
                                    }
                                    $arAnos = array(
                                        array("codigo" => "2009", "descricao" => "2009"),
                                        array("codigo" => "2010", "descricao" => "2010"),
                                        array("codigo" => "2011", "descricao" => "2011"),
                                        array("codigo" => "2012", "descricao" => "2012")
                                    );

                                    $db->monta_combo('eveanopi', $arAnos, $ativo, 'Selecione o Ano...', 'pegaNumerosPIs', '', '', '200', 'N', 'eveanopi');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class ="SubTituloDireita" align="right">Nº PI: </td>
                            <td id="comboPI">
                                <?PHP
                                    $evenumeropi = $rsDadosEvento[0]["evenumeropi"];
                                    if($eveanopi){
                                        $sql = "
                                            SELECT  DISTINCT pi.plicod as codigo,
                                                    pi.plicod || ' - ' || ug.ungabrev as descricao,
                                                    ug.ungabrev
                                            FROM monitora.pi_planointerno pi

                                            INNER JOIN monitora.pi_subacao s ON s.sbaid = pi.sbaid
                                            INNER JOIN monitora.pi_subacaounidade sau ON sau.sbaid = s.sbaid
                                            INNER JOIN public.unidadegestora ug ON ug.ungcod = sau.ungcod
                                            INNER JOIN evento.unidadeparceira up ON up.ungcod = ug.ungcod

                                            WHERE pi.plistatus = 'A' AND pi.pliano = '{$eveanopi}' --AND ug.ungcod IN ({$ungcods})

                                            ORDER BY ug.ungabrev, pi.plicod
                                        ";
                                    }
                                    $db->monta_combo('evenumeropi', $sql, $ativo, 'Selecione o PI...', 'pegaNumerosPIs', '', '', '200', 'N', 'evenumeropi');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class ="SubTituloDireita" align="right">AD Referendum: </td>
                            <td>
                                <?PHP
                                    if ($rsDadosEvento[0]['eveurgente'] == 't') {
                                        $checked = "checked = checked;";
                                    } else {
                                        $checked = "";
                                    }

                                    if (in_array(EVENTO_PERFIL_SEC_EXECUTIVA_EVENTOS, $perfis) || in_array(EVENTO_PERFIL_SUPER_USUARIO, $perfis)) {
                                        $stDisable = "";
                                    } else {
                                        $stDisable = "disabled";
                                    }
                                ?>
                                <input type="checkbox" <?= $checked; ?> name="adreverendo" id="adreverendo" <?= $stDisable; ?>>
                            </td>
                        </tr>
                        <!-- -->
                        <tr>
                            <td colspan="5" class ="SubTituloCentro" style="text-align: left;">Observações</td>
                        </tr>
                        <tr>
                            <!-- <td class ="SubTituloDireita" width="25%">Descrição:</td> -->
                            <td colspan="2">
                                <?php
                                    $pejresposta = $dadosParecer['pejresposta'];
                                    echo campo_textarea('pejresposta', 'N', $habil['descricao'], '', 228, 10, 1000, '', 0, '', false, null);
                                ?>
                            </td>
                        </tr>
                        <!-- -->
                        <tr>
                            <td colspan="2" class ="SubTituloCentro">
                                <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');" <?php echo $ativo == 'N' ? 'disabled="disabled"' : '' ?>>
                                <input type="button" name="btnCancel" value="Cancelar" id="btnCancel" onclick="validaForm('cancel');" <?php echo $ativo == 'N' ? 'disabled="disabled"' : '' ?>>
                            </td>
                        </tr>
                    </table>
                </td>
                <td align="center" width="3%">
                    <?php wf_desenhaBarraNavegacao($docid, array('' => '')); ?>
                </td>
        </table>
    </form>

