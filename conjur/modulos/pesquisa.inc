<?php
ini_set('memory_limit', '128M');

if ($_REQUEST['evento'] == 'E') {

    $prcid = (string) $_REQUEST['prcid'];

    $sql_D = "UPDATE conjur.processoconjur SET prcstatus = 'I' where prcid = " . $prcid;
    $db->executar($sql_D);
    $db->commit();

    echo "<script>
		alert('Dados salvos com sucesso.');
		window.location = '?modulo=inicio&acao=C';
	  </script>";
    exit;
}

if ($_REQUEST['req'] == 'ACsidoc') {
    ob_clean();
    $q = strtolower($_GET["q"]);
    if ($q != "") {
        $sql = "SELECT prcnumsidoc FROM conjur.processoconjur where prcnumsidoc like ('%$q%')";
    } else {
        $sql = "SELECT prcnumsidoc FROM conjur.processoconjur";
    }
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    if ($q === '')
        return;

    foreach ($arDados as $key => $value) {
        //if (strpos(strtolower($key), $q) !== false) {
        if (trim($value['prcnumsidoc'])) {
            $prcnumsidoc = trim($value['prcnumsidoc']);
            echo "{$prcnumsidoc}|{$prcnumsidoc}\n";
        }
    }

    die;
}

if ($_REQUEST['req'] == 'ACprcnumeroprocjudicial') {
    ob_clean();
    $sql = "SELECT prcnumeroprocjudicial FROM conjur.processoconjur WHERE prcstatus = 'A'";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prcnumeroprocjudicial']), $q) !== false) {
            $prcnumeroprocjudicial = $value['prcnumeroprocjudicial'];
            echo "{$prcnumeroprocjudicial}|{$prcnumeroprocjudicial}\n";
        }
    }

    die;
}

if ($_REQUEST['req'] == 'ACprcnumeroprocjudantigo') {
    ob_clean();
    $sql = "SELECT prcnumeroprocjudantigo FROM conjur.processoconjur WHERE prcstatus = 'A'";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prcnumeroprocjudantigo']), $q) !== false) {
            $prcnumeroprocjudantigo = $value['prcnumeroprocjudantigo'];
            echo "{$prcnumeroprocjudantigo}|{$prcnumeroprocjudantigo}\n";
        }
    }

    die;
}

if ($_REQUEST['req'] == 'ACprcnomeinteressado') {
    ob_clean();

    $encerrado = ( $_REQUEST['encerrados'] == 'sim' ) ? 'AND doc.esdid = 80' : 'AND doc.esdid <> 80';
    $prioritarios = ( $_REQUEST['prioritarios'] == 'sim' ) ? 'AND prc.prcprioritario = true' : '';

    $sql = "
        SELECT  prc.prcnomeinteressado 
	FROM conjur.processoconjur prc
        JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
        JOIN workflow.documento doc ON doc.docid = esp.docid
        WHERE prc.prcstatus = 'A' {$encerrado} {$prioritarios}
    ";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prcnomeinteressado']), $q) !== false) {
            $prcnomeinteressado = $value['prcnomeinteressado'];
            echo "{$prcnomeinteressado}|{$prcnomeinteressado}\n";
        }
    }
    die;
}

if ($_REQUEST['req'] == 'ACtasdsc') {
    ob_clean();
    $sql = "SELECT tasdsc FROM conjur.tipoassunto WHERE tasstatus = 'A' ORDER BY tasdsc";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['tasdsc']), $q) !== false) {
            $tasdsc = $value['tasdsc'];
            echo "{$tasdsc}|{$tasdsc}\n";
        }
    }
    die;
}

if ($_REQUEST['req'] == 'ACanxdesc') {
    ob_clean();

    $encerrado = ( $_REQUEST['encerrados'] == 'sim' ) ? 'AND doc.esdid = 80' : 'AND doc.esdid <> 80';
    $prioritarios = ( $_REQUEST['prioritarios'] == 'sim' ) ? 'AND prc.prcprioritario = true' : '';

    $sql = "
        SELECT  anx.anxdesc 
	FROM conjur.anexos anx
	JOIN conjur.processoconjur prc ON prc.prcid = anx.prcid
	JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
	JOIN workflow.documento doc ON doc.docid = esp.docid
	WHERE anx.anxstatus = 'A' {$encerrado} {$prioritarios} 
        ORDER BY anx.anxdesc
    ";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['anxdesc']), $q) !== false) {
            $anxdesc = $value['anxdesc'];
            echo "{$anxdesc}|{$anxdesc}\n";
        }
    }
    die;
}

if ($_REQUEST['req'] == 'ACprodsc') {
    ob_clean();
    $sql = "SELECT prodsc FROM conjur.procedencia WHERE unpid = '" . $_REQUEST['unpid'] . "' AND prodtstatus = 'A' ORDER BY	prodsc";
    $arDados = $db->carregar($sql);
    $arDados = ($arDados) ? $arDados : array();

    $q = strtolower($_GET["q"]);
    if ($q === '')
        return;
    foreach ($arDados as $key => $value) {
        if (strpos(strtolower($value['prodsc']), $q) !== false) {
            $prodsc = $value['prodsc'];
            echo "{$prodsc}|{$prodsc}\n";
        }
    }
    die;
}

print '<br/>';

// carregando o menu
montaAbaInicio("conjur.php?modulo=inicio&acao=C");

$titulo = "Listar Processos";
monta_titulo($titulo, ' &nbsp;');
?>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="./js/conjur.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script language="javascript" type="text/javascript">

    jQuery.noConflict();

    function submeteFormPesquisa(tipo) {
        document.getElementById("tipopesquisa").value = tipo;
        document.getElementById("formulario").submit();
    }
    function validaForm(button) {
        button.disabled = true;

        var numdoc = document.getElementById('nudnumero').value;
        var numdocano = document.getElementById('nudano');

        document.getElementById('evento').value = 'pesquisa';

        if (numdoc != '' && numdocano.value == '') {
            alert('Ano obrigat�rio para filtro por numero de documento.');
            numdocano.focus();
            button.disabled = false;
            return false;
        }

        document.getElementById('formulario').submit();
    }
    function alteraCampoCpfCnpj(tipo) {
        var cpf = document.getElementById('cpf');
        var cnpj = document.getElementById('cnpj');

        if (tipo == 'cpf') {
            cnpj.value = "";
            cnpj.style.display = "none";
            cpf.style.display = "inline";
        } else {
            cpf.value = "";
            cpf.style.display = "none";
            cnpj.style.display = "inline";
        }
    }

    function conjurJs_digitoVerificador(campoNumeroProcessoSidoc) {
        conferirDigitoVerificadorModulo11(campoNumeroProcessoSidoc);
    }

    function exibirAdvogado(cooid){
        if (cooid == "<?php echo EST_AGUARDATRIBUICAO ?>") {
            document.getElementById('opc_com_sem_adv').style.display = "";
        } else {
            document.getElementById('opc_com_sem_adv').style.display = "none";
            document.getElementById('rdo_com_adv').checked = false;
            document.getElementById('rdo_sem_adv').checked = false;
        }
    }

    function exibeConsultaLocalizacao(){
        if (jQuery('#consultas_localizacao').css('display') == 'inline')
        {
            jQuery('#consultas_localizacao').hide();
            jQuery('#mais_consulta_localizacao').attr('src', '../imagens/mais.gif');
        }
        else
        {
            jQuery('#consultas_localizacao').show();
            jQuery('#mais_consulta_localizacao').attr('src', '../imagens/menos.gif');
        }
    }

    function consultarLocalizacao(cooid){
        if (cooid == 0)
            jQuery('#cooid').val('');
        else
            jQuery('#cooid').val(cooid);

        jQuery('#formulario').submit();
    }

    jQuery(document).ready(function() {

        var url = "conjur.php?modulo=inicio&acao=C&req=ACprodsc&unpid=" + jQuery('#unpid').val();

        jQuery('#prodsc').removeClass('normal ac_input');
        jQuery('#prodsc').addClass('disabled ac_input');

        jQuery('#prcnumsidoc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACsidoc", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prcnomeinteressado').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprcnomeinteressado&encerrados=" + jQuery('#encerrados').val() + "&prioritarios=" + jQuery('#prioritarios').val(), {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#tasdsc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACtasdsc", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prodsc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprodsc", {
            extraParams:
                    {
                        unpid: function() {
                            return jQuery("#unpid").val();
                        }
                    },
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prcnumeroprocjudicial').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprcnumeroprocjudicial", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#prcnumeroprocjudantigo').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACprcnumeroprocjudantigo", {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#anxdesc').autocomplete("conjur.php?modulo=inicio&acao=C&req=ACanxdesc&encerrados=" + jQuery('#encerrados').val() + "&prioritarios=" + jQuery('#prioritarios').val(), {
            cacheLength: 10,
            width: 440,
            scrollHeight: 220,
            selectFirst: true,
            autoFill: true
        });

        jQuery('#unpid').change(function() {
//		url = "conjur.php?modulo=inicio&acao=C&req=ACprodsc&unpid="+jQuery(this).val();
//		alert(url);
            if (jQuery(this).val() != '')
            {
                if (jQuery('#prodsc').hasClass('disabled'))
                {
                    jQuery('#prodsc').removeClass('disabled ac_input');
                    jQuery('#prodsc').addClass('normal ac_input');
                }
            }
            else
            {
                jQuery('#prodsc').removeClass('normal ac_input');
                jQuery('#prodsc').addClass('disabled ac_input');

            }
        });

        jQuery('[name=tprid]').click(function()
        {
            // judicial
            if (jQuery(this).val() == '2')
            {
                jQuery('.tr_judicial').show();
            }
            else
            {
                jQuery('.tr_judicial').hide();
            }
        });

        jQuery('#tpdid').change(function()
        {
            if (jQuery(this).val() != '')
            {
                jQuery('.tr_documento').show();
            }
            else
            {
                jQuery('.tr_documento').hide();
            }
        });

        jQuery('#unpid').change(function()
        {
            if (jQuery(this).val() != '')
            {
                jQuery('.tr_procedencia').show();
            }
            else
            {
                jQuery('.tr_procedencia').hide();
            }
        });

        jQuery('#pesquisa_avancada').click(function()
        {
            if (jQuery('.tr_pesq_avancada').css('display') == 'none')
            {
                jQuery('.tr_pesq_avancada').show();

                <?php if ($_REQUEST['esdid'] == EST_AGUARDATRIBUICAO): ?>
                    jQuery('#opc_com_sem_adv').show();
                <?php endif; ?>

                jQuery(this).html('[-] Menos Op��es de Busca');
            }
            else
            {
                jQuery('.tr_pesq_avancada').hide();

                <?php if ($_REQUEST['esdid'] == EST_AGUARDATRIBUICAO): ?>
                    jQuery('#opc_com_sem_adv').hide();
                <?php endif; ?>

                jQuery(this).html('[+] Mais Op��es de Busca');
            }
        });

        var arRequest = <?= simec_json_encode($_POST) ?>;

        jQuery('#enviar').click(function() {
            window.open('conjur.php?modulo=principal/popupRelPesquisaInicio&acao=A&' + jQuery.param(arRequest),
                    'modelo',
                    "height=600,width=600,scrollbars=yes,top=50,left=200");
        });

        function limparCampos() {
            jQuery('[name="tprid"]').each(function() {
                jQuery(this).attr('checked', false);
            });
            jQuery('#prcnumsidoc').val('');
            jQuery('#tpdid').val('');
            jQuery('#anxdesc').val('');
            jQuery('#prcnomeinteressado').val('');
            jQuery('#prcdtentradaini').val('');
            jQuery('#prcdtentradafim').val('');
            jQuery('#prcdtultimaini').val('');
            jQuery('#prcdtultimafim').val('');
            jQuery('#advid').val('');
            jQuery('#unpid').val('');
            jQuery('#tasdsc').val('');
            jQuery('#expressaochave').val('');
            jQuery('#tacid').val('');
            jQuery('[name="encerrados"]').each(function() {
                jQuery(this).attr('checked', false);
            });
            jQuery('[name="prioritarios"]').each(function() {
                jQuery(this).attr('checked', false);
            });
        }
        ;

        jQuery('.busca_avancada').click(function() {
            jQuery('#limpar').click();
            jQuery('#div_lista').html('');
            if (jQuery('#busca_avancada').css('display') == 'none') {
                jQuery('#busca_avancada').show();
//			jQuery('#div_busca_avan�ada').show();
                jQuery('#div_busca_simples').hide();
//			jQuery('#div_botao_busca_simples').hide();
                jQuery('#busca_simples').val('');
            } else {
                jQuery('#busca_avancada').hide();
//			jQuery('#div_busca_avan�ada').hide();
                jQuery('#div_busca_simples').show();
//			jQuery('#div_botao_busca_simples').show();
                limparCampos();
            }
        });

        jQuery('#limpar').click(function() {
            jQuery('[type="checkbox"]').each(function() {
                jQuery(this).attr('checked', false);
            });
            jQuery('[type="radio"]').each(function() {
                jQuery(this).attr('checked', false);
            });
            jQuery('[type="text"]').each(function() {
                jQuery(this).val('');
            });
            jQuery('select').each(function() {
                jQuery(this).val('');
            });
        });

        jQuery('[name="vertodos"]').click(function() {
            jQuery('#todos').val('true');
            jQuery('#origem').val('avancado');
            validaForm(jQuery(this));
        });

        jQuery('#pesquisar_simplificado').click(function() {
            if (jQuery('#busca_simples').val() != '') {
                jQuery('#origem').val('simplificado');
                validaForm(jQuery(this));
            }
        });

        jQuery('#visualizar').click(function() {
            jQuery('#origem').val('avancado');
            validaForm(jQuery(this));
        });
    });

    function marcarPorName(name) {
        var check = false;
        jQuery('[name="' + name + '"]').each(function() {
            if (jQuery(this).attr('checked') == false) {
                check = true;
            }
        });
        jQuery('[name="' + name + '"]').each(function() {
            jQuery(this).attr('checked', check)
        });
    }

    function numProcesso(obj) {
        var prcnumsidoc = obj.value;

        if (prcnumsidoc == ''){
            return;
        }


        if (!conferirDigitoVerificadorModulo11SemMensagem(obj)){
            if (prcnumsidoc.length != 9){
                alert('N�mero inv�lido');
                obj.focus();
                return false;
            }else{
                var ano = prcnumsidoc.substring(0, 4);
                var hoje = new Date();

                if (ano < 1900 || ano > hoje.getFullYear()){
                    alert('n�mero inv�lido');
                    obj.focus();
                    return false;
                }else{
                    //� um n�mero emec v�lido
                    return true;
                }
            }
        }else{
            //� um numero sidoc v�lido
            return true;
        }
    }

</script>
<form name="formulario" id="formulario" method="post">
    <input type="hidden" name="requisicao" value="pesquisarprojeto">  
    <input type="hidden" name="evento" id="evento" value=""> 
    <input type="hidden" name="origem" id="origem" value="">  
    <input type="hidden" name="todos" id="todos" value=""> 
    <input type="hidden" name="prcid" id="prcid" value="">

    <table align="center" border="0" cellpadding="0" cellspacing="0" class="tabela">
        <tr id="div_busca_simples" <?= ($_REQUEST['origem'] == 'avancado' ? 'style="display:none;"' : ( ($_REQUEST['acao'] == 'C' || $_REQUEST['acao'] == 'A') && $_REQUEST['origem'] == '' ? 'style="display:none;"' : '' )); ?>>
            <td colspan="2">
                <table class="listagem" align="center" border="0" cellpadding="5" cellspacing="1" width="100%" style="background-color: white;">
                    <tr>
                        <td>
                            <div>
                                <div style="margin-left:30px">
                                    <center>
                                        <?= campo_texto('busca_simples', 'N', 'S', '', 30, 255, '', '', 'left;font-size:12pt', '', 0, 'id="busca_simples"'); ?>
                                        <img border="0" align="absmiddle" id="pesquisar_simplificado" width="60px" title="Visualizar" style="cursor:pointer" src="../imagens/gadget_busca.png">
                                    </center>
                                </div>
                                <br>
                                <center>
                                    <u><i><a class="busca_avancada" style="cursor:pointer">Busca Avan�ada</a></i></u>
                                </center>
                            </div>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr id="busca_avancada" <?= ($_REQUEST['origem'] == 'simplificado' ? 'style="display:none;"' : ( ($_REQUEST['acao'] == 'S') && $_REQUEST['origem'] == '' ? 'style="display:none;"' : '' )); ?>>
            <td style="background-color: #e9e9e9; color: #404040; width: 600px;">

                <table class="tabela" align="center" border="0" cellpadding="5"
                       cellspacing="1" style="width: 500px;">
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 150px;">Tipo de Processo:</td>
                        <td>
                            <?
                            $tprid = $_REQUEST['tprid'];
                            $sqlTipoProcesso = "
                                SELECT  tprid as codigo, 
                                        tprdsc as descricao
				FROM conjur.tipoprocesso 
                                WHERE tprstatus = 'A'
                                ORDER BY tprdsc
                            ";
                            $dadosTipoProcesso = $db->carregar($sqlTipoProcesso);

                            foreach ($dadosTipoProcesso as $tipoProcesso) {
                                $checked = ($tipoProcesso['codigo'] == $tprid) ? 'checked="checked"' : '';
                                echo '<input type="radio" name="tprid" value="' . $tipoProcesso['codigo'] . '" ' . $checked . ' /> ' . $tipoProcesso['descricao'];
                            }

                            // judicial
                            $displayLinhasJ = ($tprid && $tprid == '2') ? 'table-row' : 'none';
                            ?>
                        </td>
                    </tr>
                    <tr class="tr_judicial" style="display:<?= $displayLinhasJ ?>;">
                        <td class="SubTituloDireita">Numera��o �nica Judicial :</td>
                        <td>
                            <? echo campo_texto('prcnumeroprocjudicial', 'N', 'S', '', 30, 25, '#######-##.####.#.##.####', '', 'right', '', 0, ' style="text-align:right;" id="prcnumeroprocjudicial" '); ?>
                        </td>
                    </tr>
                    <tr class="tr_judicial" style="display:<?= $displayLinhasJ ?>;">
                        <td class="SubTituloDireita">Numera��o Judicial Antiga :</td>
                        <td>
                            <? echo campo_texto('prcnumeroprocjudantigo', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, ' style="text-align:right;" id="prcnumeroprocjudantigo" '); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">N�mero do Processo SIDOC/EMEC :</td>
                        <td>
                            <?
                                $pcjnumerosidoc = $_REQUEST['prcnumsidoc'];
                                echo campo_texto('prcnumsidoc', 'N', 'S', '', 35, 25, '#################', '', 'left', '', 0, 'id="prcnumsidoc"', '', '', 'numProcesso(this)');
                            ?>
                        </td>
                    </tr>
                    <tr style="visibility: visible;">
                        <td class="SubTituloDireita">Tipo de documento :</td>
                        <td>
                            <?php
                                $tpdid = $_REQUEST['tpdid'];
                                $sql = "
                                    SELECT  tpdid as codigo,
                                            tpddsc as descricao
                                    FROM conjur.tipodocumento
                                    WHERE tpdstatus = 'A'
                                    ORDER BY tpddsc ASC
                                ";
                                $db->monta_combo('tpdid', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'tpdid');

                                $displayLinhaDoc = ($tpdid) ? 'table-row' : 'none';
                            ?>
                        </td>
                    </tr>
                    <tr class="tr_documento" style="display:<?= $displayLinhaDoc ?>;">
                        <td class="SubTituloDireita">Numero do documento :</td>
                        <td>
                            <?
                                echo campo_texto('nudnumero', 'N', 'S', '', 14, 10, '###############', '', 'right', '', 0, ' id="nudnumero" style="text-align:right;" ', '', $_REQUEST['nudnumero']);
                                echo '&nbsp;&nbsp;&nbsp';
                                $nudano = $_REQUEST['nudano'];
                                $sql = "
                                    SELECT  DISTINCT nudano as codigo,
                                            nudano as descricao
                                    FROM conjur.numeracaodocumento
                                    WHERE nudstatus = 'A'
                                    ORDER BY nudano
                                ";
                                $db->monta_combo('nudano', $sql, 'S', 'Todos', '', '', '', '', '', 'nudano');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Ementa:</td>
                        <td>
                            <?
                                $anxdesc = $_REQUEST['anxdesc'];
                                echo campo_texto('anxdesc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="anxdesc"');
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">Interessado :</td>
                        <td>
                            <?
                                $prcnomeinteressado = $_REQUEST['prcnomeinteressado'];
                                echo campo_texto('prcnomeinteressado', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="prcnomeinteressado"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de Entrada do Processo :</td>
                        <td>
                            <?
                                echo "In�cio " . campo_data('prcdtentradaini', 'N', 'S', '', 'S');
                                echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
                                echo "Fim " . campo_data('prcdtentradafim', 'N', 'S', '', 'S');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data da �ltima Tramita��o:</td>
                        <td>
                            <?
                                echo "In�cio " . campo_data('prcdtultimaini', 'N', 'S', '', 'S');
                                echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
                                echo "Fim " . campo_data('prcdtultimafim', 'N', 'S', '', 'S');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Advogado(s) :</td>
                        <td>
                            <?
                                $advid = $_REQUEST['advid'];
                                $sqlAdvogado = "
                                    SELECT  adv.advid as codigo, ent.entnome as descricao 
                                    FROM conjur.advogados adv 
                                    LEFT JOIN entidade.entidade ent ON ent.entid = adv.entid 
                                    WHERE adv.advstatus='A'	
                                    ORDER BY ent.entnome
                                ";
                                $db->monta_combo('advid', $sqlAdvogado, 'S', 'Selecione...', '', '', '', 150, 'N', 'advid');
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">Tipo de Proced�ncia :</td>
                        <td>
                            <?
                                $unpid = $_REQUEST['unpid'];
                                $sqlTipoProcedencia = "
                                    SELECT  unpid as codigo,
                                            unpdsc as descricao
                                    FROM conjur.unidadeprocedencia
                                    WHERE undpstatus = 'A'
                                    ORDER BY unpdsc
                                ";
                                $db->monta_combo('unpid', $sqlTipoProcedencia, 'S', 'Selecione...', '', '', '', 300, 'N', 'unpid');
                                $displayLinhaProc = ($unpid) ? 'table-row' : 'none';
                            ?>
                        </td>
                    </tr>
                    <tr class="tr_procedencia" style="display:<?= $displayLinhaProc ?>">
                        <td align='right' class="SubTituloDireita">Proced�ncia:</td>
                        <td>
                            <?php
                                $prodsc = $_REQUEST['prodsc'];
                                echo campo_texto('prodsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="prodsc"');
                            ?> 
                        </td>
                    </tr>
                    <?php
                        if ($_REQUEST['tasdsc'] || $_REQUEST['tacid'] || $_POST['com_sem_adv'] || $_REQUEST['expressaochave']) {
                            $textoPesqAv = "[-] Menos Op��es de Busca";
                            $displayLinhasPesqAv = "table-row";
                        } else {
                            $textoPesqAv = "[+] Mais Op��es de Busca";
                            $displayLinhasPesqAv = "none";
                        }
                    ?>
                    <tr>
                        <td class="SubTituloDireita"></td>
                        <td class="SubTituloEsquerda">
                            <a id="pesquisa_avancada" style="cursor: pointer;"><?= $textoPesqAv ?></a>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita"></td>
                        <td class="SubTituloEsquerda">
                            <a class="busca_avancada" style="cursor:pointer">Busca Simplificada</a>
                        </td>
                    </tr>

                    <tr class="tr_pesq_avancada" style="display:<?= $displayLinhasPesqAv ?>">
                        <td class="SubTituloDireita">Tema :</td>
                        <td>
                        <?
                            $tasdsc = $_REQUEST['tasdsc'];
                            echo campo_texto('tasdsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="tasdsc"');
                        ?>
                        </td>
                    </tr>
                    <tr id="opc_com_sem_adv" style="display: <?php echo ($_REQUEST['esdid'] == EST_AGUARDATRIBUICAO && $displayLinhasPesqAv == "table-row") ? "table-row" : "none" ?>" >
                        <td class="SubTituloDireita">Advogado Atribu�do:</td>
                        <td>
                            <input type="radio" name="com_sem_adv" value="1" <?php echo ($_POST['com_sem_adv'] && $_POST['com_sem_adv'] == "1") ? "checked='checked'" : "" ?> id="rdo_com_adv" /> Sim 
                            <input type="radio" name="com_sem_adv" value="0" <?php echo ($_POST['com_sem_adv'] && $_POST['com_sem_adv'] == "0") ? "checked='checked'" : "" ?> id="rdo_sem_adv" /> N�o
                        </td>
                    </tr>
                    <tr class="tr_pesq_avancada" style="display:<?= $displayLinhasPesqAv ?>">
                        <td class="SubTituloDireita">Express�o Chave:</td>
                        <td>
                            <?
                                $expressaochave = $_REQUEST['expressaochave'];
                                echo campo_texto('expressaochave', 'N', 'S', '', 50, 60, '', '', 'left', '', 0, 'id="expressaochave"');
                            ?>
                        </td>
                    </tr>
                    <tr class="tr_pesq_avancada" style="display:<?= $displayLinhasPesqAv ?>">
                        <td class="SubTituloDireita">Tipo de A��o:</td>
                        <td>
                            <?php
                                $tacid = $_REQUEST['tacid'];
                                $sqlTipoAcao = "
                                    SELECT  tacid as codigo,
                                            tacdsc as descricao
                                    FROM conjur.tipoacao
                                    WHERE tacstatus = 'A'
                                    ORDER BY	tacdsc
                                 ";
                                $db->monta_combo('tacid', $sqlTipoAcao, 'S', 'Selecione...', '', '', '', 300, 'N', 'tacid');
                            ?>
                        </td>
                    </tr>
                    <tr class="tr_pesq_avancada" style="display:<?= $displayLinhasPesqAv ?>">
                        <td class="SubTituloDireita">Exibir Processos Encerrados:</td>
                        <td>
                            <input type="radio" name="encerrados" id="encerrados" value="sim" <?php if ($_REQUEST['encerrados'] == 'sim') { echo 'checked=\"checked\"'; } ?> />Sim 
                            <input type="radio" name="encerrados" id="encerrados" value="nao" <?php if (!$_REQUEST['encerrados'] || $_REQUEST['encerrados'] == 'nao') { echo 'checked=\"checked\"'; } ?> />N�o
                        </td>
                    </tr>
                    <tr class="tr_pesq_avancada" style="display:<?= $displayLinhasPesqAv ?>">
                        <td class="SubTituloDireita">Exibir Somente Relevantes:</td>
                        <td>
                            <input type="radio" name="prioritarios" id="prioritarios" value="sim" <?php if ($_REQUEST['prioritarios'] == 'sim') { echo 'checked=\"checked\"'; } ?> /> Sim 
                            <input type="radio" name="prioritarios" id="prioritarios" value="nao" <?php if (!$_REQUEST['prioritarios'] || $_REQUEST['prioritarios'] == 'nao') { echo 'checked=\"checked\"';} ?> />N�o
                        </td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2" align="center">
                            <input type="button" id="visualizar" name="visualizar" value="Visualizar" style="cursor: pointer;float:left;" > 
                            <input type="button" id="limpar" value="Limpar" style="float:left;" />
                            <input type="button" name="vertodos" value="Ver Pendentes" style="cursor: pointer;float:right;" >
                            <input type="button" id="enviar" style="float:right;" value="Imprimir Lista" />
                        </td>
                    </tr>
                </table>
                <?
                    $filtroprocesso[] = " prcstatus = 'A' ";
                    $filtro_estado = " ";

                    if ($_REQUEST['busca_simples']) {
                        $filtroprocesso[] = "
                            ( UPPER(removeacento(prcnumsidoc)) ilike UPPER(removeacento('%" . $_REQUEST['busca_simples'] . "%')) OR 
                              UPPER(removeacento(prc.prcnumeroprocjudicial)) ilike UPPER(removeacento('%" . $_REQUEST['busca_simples'] . "%')) OR 
                              UPPER(removeacento(prcnomeinteressado)) ilike UPPER(removeacento('%" . $_REQUEST['busca_simples'] . "%')) OR 
                              UPPER(removeacento(anxdesc)) ilike UPPER(removeacento('%" . $_REQUEST['busca_simples'] . "%')) )
                        ";
                    } else {

                        if ($_REQUEST['tprid']) {
                            $filtroprocesso[] = "tprid='" . $_REQUEST['tprid'] . "'";
                        }
                        if ($_REQUEST['prcnumsidoc']) {
                            $filtroprocesso[] = "prcnumsidoc='" . eregi_replace('([^0-9])', '', $_REQUEST['prcnumsidoc']) . "'";
                        }
                        if ($_REQUEST['prcnomeinteressado']) {
                            $filtroprocesso[] = "UPPER(removeacento(prcnomeinteressado)) ilike(UPPER(removeacento('%" . $_REQUEST['prcnomeinteressado'] . "%')))";
                        }

                        if ($_REQUEST['prcdtentradaini']) {
                            $dataformatadainicio = formata_data_sql($_REQUEST['prcdtentradaini']);

                            $filtroprocesso[] = "prcdtentrada >= '" . $dataformatadainicio . "'";
                        }
                        if ($_REQUEST['prcdtentradafim']) {
                            $dataformatadafim = formata_data_sql($_REQUEST['prcdtentradafim']);

                            $filtroprocesso[] = "prcdtentrada <= '" . $dataformatadafim . "'";
                        }
                        if ($_REQUEST['prcdtultimaini']) {
                            $dataultimainicio = formata_data_sql($_REQUEST['prcdtultimaini']);

                            $filtroprocesso[] = "TO_CHAR(htddata, 'YYYY-MM-DD') >= '" . $dataultimainicio . "'";
                        }
                        if ($_REQUEST['prcdtultimafim']) {
                            $dataultimafim = formata_data_sql($_REQUEST['prcdtultimafim']);

                            $filtroprocesso[] = "TO_CHAR(htddata, 'YYYY-MM-DD') <= '" . $dataultimafim . "'";
                        }
                        if ($_REQUEST['advid']) {
                            $filtroprocesso[] = "had.advid = '" . $_REQUEST['advid'] . "'";
                        }

                        if ($_REQUEST['anxdesc']) {
                            $filtroprocesso[] = "UPPER(removeacento(anx.anxdesc)) ilike UPPER(removeacento('%" . $_REQUEST['anxdesc'] . "%'))";
                        }

                        if (strlen($_REQUEST['proid']) != 0) {
                            $code = explode("&", $_REQUEST['proid']);
                            $filtroprocesso[] = "pro.proid='" . $code[0] . "' ";
                        }
                        if ($_REQUEST['tasid']) {
                            $filtroprocesso[] = "tasid='" . $_REQUEST['tasid'] . "'";
                        }
                        if (is_array($_REQUEST['cooid'])) {
                            $filtroprocesso[] = "cooid in (" . implode(',', $_REQUEST['cooid']) . ")";
                        }

                        if (is_array($_REQUEST['esdid'])) {
                            $filtroprocesso[] = "esd.esdid in (" . implode(',', $_REQUEST['esdid']) . ")";
                        } elseif (  $_REQUEST['esdid'] && $_REQUEST['esdid'] != 'Array') {
                            $filtroprocesso[] = "esd.esdid = " . $_REQUEST['esdid'];
                        }elseif ($_REQUEST['todos'] || $_REQUEST['evento'] == '') {
                            $filtroprocesso[] = "esd.esdid in (374,382,381,378,380)";
                        }   
                        
                        if ($_REQUEST['expressaochave']) {
                            $filtroprocesso[] = "UPPER(removeacento(excdsc)) ilike (UPPER(removeacento('%" . $_REQUEST['expressaochave'] . "%')))";
                        }
                        if ($_REQUEST['tacid']) {
                            $filtroprocesso[] = "tac.tacid = " . $_REQUEST['tacid'];
                        }
                        if ($_REQUEST['prcnumeroprocjudicial']) {
                            $filtroprocesso[] = "prc.prcnumeroprocjudicial='" . $_REQUEST['prcnumeroprocjudicial'] . "'";
                        }

                        if ($_POST['com_sem_adv'] == "1") {
                            $filtroprocesso[] = "had.advid is not null";
                        }
                        if ($_POST['com_sem_adv'] == "0") {
                            $filtroprocesso[] = "had.advid is null";
                        }

                        if ($_REQUEST['esdid'] == false && $qualquer_situacao) {
                            $filtroprocesso[] = " (esd.esdid not in (30,99," . EST_ANEXADO . ") )";
                        }

                        if ($_REQUEST['encerrados'] != 'sim' && $_REQUEST['esdid'] != CONJUR_PROCESSO_ENCERRADO && $qualquer_situacao) {
                            $filtroprocesso[] = " (esd.esdid <> " . CONJUR_PROCESSO_ENCERRADO . ")";
                        }

                        if ($_REQUEST['prioritarios'] == 'sim') {
                            $filtroprocesso[] = " (prc.prcprioritario = true )";
                        }

                        if ($_REQUEST['prcnumeroprocjudantigo'] && $qualquer_situacao == false) {
                            $filtroprocesso[] = " (prc.prcnumeroprocjudantigo='" . $_REQUEST['prcnumeroprocjudantigo'] . "') ";
                        }

                        if ($_REQUEST['tpdid']) {
                            $filtroprocesso[] = " (nud.tpdid = " . $_REQUEST['tpdid'] . " )";
                        }

                        if ($_REQUEST['nudnumero']) {
                            $filtroprocesso[] = " (to_char(nud.nudnumero,'9999999999') LIKE '%" . $_REQUEST['nudnumero'] . "%' AND nud.nudano = " . $_REQUEST['nudano'] . ")";
                        } elseif ($_REQUEST['nudano']) {
                            $filtroprocesso[] = " (nud.nudano = " . $_REQUEST['nudano'] . ")";
                        }
                    }

                    $condicoes = ( (count($filtroprocesso) > 1) ? implode(" AND ", $filtroprocesso) : " prcstatus = 'A' " );

                    $qualquer_situacao = $_REQUEST['prcnumsidoc'] == false && $_REQUEST['prcnumeroprocjudicial'] == false && $_REQUEST['prcnumeroprocjudantigo'] == false;

                    $cabecalho = array('A��es', 'N� do Processo SIDOC', 'Interessado', 'Data de Entrada', 'Situa��o CONJUR');

                if ($_REQUEST['ordemlistadir'] == '') {
                    $sql = "
                        SELECT  prcnumsidoc,
				prcnomeinteressado,
                                /*
				CASE WHEN wd.htddata is not null 
                                    THEN '<input type=\"hidden\" value=\"1'|| to_char(wd.htddata, 'YYYYMMDD') ||'\"/>'||to_char(wd.htddata, 'DD/MM/YYYY') 			
                                    ELSE '<input type=\"hidden\" value=\"1'|| to_char(prcdtentrada, 'YYYYMMDD') ||'\"/>'||to_char(prcdtentrada, 'DD/MM/YYYY') 
				END as dataultimocadastro,
                                */

                                '<span style=\"color: black;\"><input type=\"hidden\" value=\"'|| to_char(prcdtentrada, 'YYYYMMDD') ||'\"/>'||to_char(prcdtentrada, 'DD/MM/YYYY')||'</span>' AS dataultimocadastro,

                                CASE WHEN esd.esdid NOT IN (" . WF_ARQUIVADO . "," . WF_ANEXADO . "," . WF_ENCERRADO . ") AND (current_date - INTERVAL '15 days' > wd.htddata AND tpr.tipid = 2 ) 
                                    THEN '<input type=\"hidden\" value=\"'|| to_char(wd.htddata, 'YYYYMMDD') ||'\"/><font color=red title=\"Passaram 15 dias desde o �ltimo tramite.\">' || to_char(wd.htddata, 'DD/MM/YYYY') || '</font>' 
                                    ELSE 
                                        CASE WHEN esd.esdid NOT IN (" . WF_ARQUIVADO . "," . WF_ANEXADO . "," . WF_ENCERRADO . ") AND (current_date - INTERVAL '5 days' > wd.htddata AND tpr.tipid = 1 ) 
                                            THEN '<input type=\"hidden\" value=\"'|| to_char(wd.htddata, 'YYYYMMDD') ||'\"/><font color=red title=\"Passaram 5 dias desde o �ltimo tramite.\">' || to_char(wd.htddata, 'DD/MM/YYYY') || '</font>' 
                                            ELSE '<input type=\"hidden\" value=\"'|| to_char(wd.htddata, 'YYYYMMDD') ||'\"/>'||to_char(wd.htddata, 'DD/MM/YYYY') 
                                        END 
                                END as dttramite,

                                to_char(had.haddtprazoadv, 'DD/MM/YYYY') as prcdtprazoadv,
                                tpr.tipdsc as prioridade,
                                prc.prcprioritario as prioritario,
                                
			        CASE WHEN ( esp.espnumdiasrespexterna IS NOT NULL ) 
                                    THEN 
                                        CASE WHEN ( ( current_date - esp.espnumdiasrespexterna )  > prcdtentrada ) 
                                            THEN 
                                                CASE WHEN esd.esdid = 376 
                                                    THEN '<div style=\"display:none\">ordenadata'|| to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'YYYYMMDD' ) ||'</div><font color=red title=\"Situa��o em atraso.\">' || esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ') </font>'
                                                    ELSE esd.esddsc
                                                END
                                            ELSE 
                                                CASE WHEN esd.esdid = 376
                                                    THEN esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ')'
                                                    ELSE esd.esddsc
                                                END 
                                        END
                                    WHEN ( espdtrespexterna IS NOT NULL ) 
                                        THEN 
                                        CASE WHEN ( current_date > espdtrespexterna ) 
                                            THEN 
                                                CASE WHEN esd.esdid = 376 
                                                    THEN '<div style=\"display:none\">ordenadata'|| to_char(espdtrespexterna, 'YYYYMMDD' ) ||'</div><font color=red title=\"Situa��o em atraso.\">' || esd.esddsc || '<br> (' || to_char(espdtrespexterna, 'DD/MM/YYYY' ) || ') </font>'
                                                    ELSE esd.esddsc
                                                END
                                            ELSE 
                                                CASE WHEN esd.esdid = 376
                                                    THEN esd.esddsc || '<br> (' || to_char(espdtrespexterna, 'DD/MM/YYYY' ) || ')'
                                                    ELSE esd.esddsc
                                                END 
                                        END
                                    ELSE esd.esddsc 
                                END as esddsc,
                                
                                prc.prcid,
                                coo.coodsc, 
                                prc.usucpf 
		    	FROM conjur.processoconjur prc 
                        
		    	LEFT JOIN (
                            SELECT  hadid, 
                                    adv.advid, 
                                    prcid, 
                                    dtatribuicao, 
                                    coonid,
                                    haddtprazoadv
                            FROM conjur.historicoadvogados had
                            INNER JOIN conjur.advogados adv ON adv.advid = had.advid
                            
                            WHERE hadid IN ( SELECT foo.hadid FROM ( SELECT max(hadid) as hadid, prcid FROM conjur.historicoadvogados GROUP BY prcid ) foo )
                            
                        ) had ON had.prcid = prc.prcid AND had.coonid = prc.cooid
                                        
		    	LEFT JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
		    	LEFT JOIN conjur.coordenacao coo ON coo.coonid = prc.cooid
		    	LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
		    	LEFT JOIN workflow.historicodocumento wd  ON wd.hstid = doc.hstid 
		    	
                        LEFT JOIN workflow.acaoestadodoc ac  ON ac.aedid = wd.aedid AND ac.aedid in (56, 57, 213, 338, 627, 1050, 1051, 1055)
                        LEFT JOIN workflow.estadodocumento ed  ON ed.esdid = ac.esdidorigem
                        LEFT JOIN seguranca.usuario us  ON us.usucpf = wd.usucpf
                        LEFT JOIN workflow.comentariodocumento cd  ON cd.hstid = wd.hstid

		    	LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		    	LEFT JOIN conjur.expressaochave exp ON exp.prcid = prc.prcid  
		    	LEFT JOIN conjur.procedencia pro ON pro.proid = prc.proid
	        	LEFT JOIN conjur.tipoprioridade tpr ON tpr.tipid = prc.tipid 
		        LEFT JOIN conjur.anexos anx ON anx.prcid = prc.prcid 
		        LEFT JOIN conjur.numeracaodocumento nud ON nud.nudid = anx.nudid
		        LEFT JOIN conjur.tipoacao tac ON tac.tacid = prc.tacid
                        
		    	WHERE {$condicoes}
                            
                        GROUP BY espdtrespexterna,prc.prcid, prcnumsidoc, prcnomeinteressado, prcdtentrada,tpr.tipdsc, prc.prcprioritario, esd.esddsc, prc.usucpf,coo.coodsc , esd.esdid, tpr.tipid, dataultimocadastro, esp.espnumdiasrespexterna, wd.htddata, had.haddtprazoadv
                        
	    		ORDER BY prioridade DESC, dataultimocadastro DESC, dttramite, had.haddtprazoadv DESC
                    ";
                } else {
                    $sql = "
                        SELECT  prcnumsidoc,
				prcnomeinteressado,
                                
				CASE WHEN wd.htddata is not null 
                                    THEN 
                                        CASE WHEN esd.esdid NOT IN (" . WF_ARQUIVADO . "," . WF_ANEXADO . "," . WF_ENCERRADO . ") AND (current_date - INTERVAL '15 days' > wd.htddata AND tpr.tipid = 2 ) 
                                            THEN '<input type=\"hidden\" value=\"'|| to_char(wd.htddata, 'YYYYMMDD') ||'\"/><font color=red title=\"Passaram 15 dias desde o �ltimo tramite.\">' || to_char(wd.htddata, 'DD/MM/YYYY') || '</font>' 
                                            ELSE 
                                                CASE WHEN esd.esdid NOT IN (" . WF_ARQUIVADO . "," . WF_ANEXADO . "," . WF_ENCERRADO . ") AND (current_date - INTERVAL '5 days' > wd.htddata AND tpr.tipid = 1 ) 
                                                    THEN '<input type=\"hidden\" value=\"'|| to_char(wd.htddata, 'YYYYMMDD') ||'\"/><font color=red title=\"Passaram 5 dias desde o �ltimo tramite.\">' || to_char(wd.htddata, 'DD/MM/YYYY') || '</font>' 
                                                    ELSE '<input type=\"hidden\" value=\"'|| to_char(wd.htddata, 'YYYYMMDD') ||'\"/>'||to_char(wd.htddata, 'DD/MM/YYYY') 
                                                END 
                                        END				
                                    ELSE 
                                        CASE WHEN esd.esdid NOT IN (" . WF_ARQUIVADO . "," . WF_ANEXADO . "," . WF_ENCERRADO . ") AND (current_date - INTERVAL '15 days' > prcdtentrada AND tpr.tipid = 2 ) 
                                            THEN '<input type=\"hidden\" value=\"'|| to_char(prcdtentrada, 'YYYYMMDD') ||'\"/><font color=red title=\"Passaram 15 dias desde a entrada.\">' || to_char(prcdtentrada, 'DD/MM/YYYY') || '</font>' 
                                            ELSE 
                                                CASE WHEN esd.esdid NOT IN (" . WF_ARQUIVADO . "," . WF_ANEXADO . "," . WF_ENCERRADO . ") AND (current_date - INTERVAL '5 days' > prcdtentrada AND tpr.tipid = 1 ) 
                                                    THEN '<input type=\"hidden\" value=\"'|| to_char(prcdtentrada, 'YYYYMMDD') ||'\"/><font color=red title=\"Passaram 5 dias desde a entrada.\">' || to_char(prcdtentrada, 'DD/MM/YYYY') || '</font>' 
                                                    ELSE '<input type=\"hidden\" value=\"'|| to_char(prcdtentrada, 'YYYYMMDD') ||'\"/>'||to_char(prcdtentrada, 'DD/MM/YYYY')
                                                END 
                                        END
                                END as dataultimocadastro,
                                
				CASE WHEN wd.htddata is not null 
                                    THEN '<input type=\"hidden\" value=\"'|| to_char(wd.htddata,'YYYYMMDD') ||'\" />'||to_char(wd.htddata, 'DD/MM/YYYY')
                                    ELSE '-' 
				END as dttramite,

				to_char(had.haddtprazoadv, 'DD/MM/YYYY') as prcdtprazoadv,
				tpr.tipdsc as prioridade,
				prc.prcprioritario as prioritario,
                                
			        CASE WHEN ( esp.espnumdiasrespexterna IS NOT NULL ) 
                                    THEN 
                                        CASE WHEN ( ( current_date - esp.espnumdiasrespexterna )  > prcdtentrada ) 
                                            THEN 
                                                CASE WHEN esd.esdid = 376 
                                                    THEN '<font color=red title=\"Situa��o em atraso.\">' || esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ') </font>'
                                                    ELSE esd.esddsc 
						END
                                            ELSE
                                                CASE WHEN esd.esdid = 376
                                                    THEN esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ')'
                                                    ELSE esd.esddsc
						END 
					END
                                    WHEN ( espdtrespexterna IS NOT NULL ) 
                                        THEN 
                                            CASE WHEN ( current_date > espdtrespexterna ) 
                                                THEN 
                                                    CASE WHEN esd.esdid = 376 
                                                        THEN '<font color=red title=\"Situa��o em atraso.\">' || esd.esddsc || '<br> (' || to_char(espdtrespexterna, 'DD/MM/YYYY' ) || ') </font>'
                                                        ELSE esd.esddsc 
                                                    END
						ELSE 
                                                    CASE WHEN esd.esdid = 376
                                                        THEN esd.esddsc || '<br> (' || to_char(espdtrespexterna, 'DD/MM/YYYY' ) || ')'
							ELSE esd.esddsc
                                                    END 
                                            END
                                        ELSE esd.esddsc 
                                END as esddsc,
                                
                                prc.prcid,
                                coo.coodsc, 
                                prc.usucpf 
                                
		    	FROM conjur.processoconjur prc 
                        
		    	LEFT JOIN (
                            SELECT  hadid, 
                                    adv.advid, 
                                    prcid, 
                                    dtatribuicao, 
                                    coonid,
                                    haddtprazoadv
                            FROM conjur.historicoadvogados had
                            INNER JOIN conjur.advogados adv ON adv.advid = had.advid
                            
                            WHERE hadid IN (SELECT foo.hadid FROM ( SELECT max(hadid) as hadid, prcid FROM conjur.historicoadvogados GROUP BY prcid) foo )
                            
                        ) had ON had.prcid = prc.prcid AND had.coonid = prc.cooid
                        
		    	LEFT JOIN conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
		    	LEFT JOIN conjur.coordenacao coo ON coo.coonid = prc.cooid
		    	LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
		    	LEFT JOIN workflow.historicodocumento wd ON wd.hstid = doc.hstid 

                        LEFT JOIN workflow.acaoestadodoc ac ON ac.aedid = wd.aedid AND ac.aedid in (56, 57, 213, 338, 627, 1050, 1051, 1055)
                        LEFT JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
                        LEFT JOIN seguranca.usuario us  ON us.usucpf = wd.usucpf
                        LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = wd.hstid

		    	LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		    	LEFT JOIN conjur.expressaochave exp ON exp.prcid = prc.prcid  
		    	LEFT JOIN conjur.procedencia pro ON pro.proid = prc.proid
	        	LEFT JOIN conjur.tipoprioridade tpr ON tpr.tipid = prc.tipid 
		        LEFT JOIN conjur.anexos anx ON anx.prcid = prc.prcid 
		        LEFT JOIN conjur.numeracaodocumento nud ON nud.nudid = anx.nudid
		        LEFT JOIN conjur.tipoacao tac ON tac.tacid = prc.tacid
                        
		    	WHERE {$condicoes}
                            
			GROUP BY espdtrespexterna,prc.prcid, prcnumsidoc, prcnomeinteressado, prcdtentrada,tpr.tipdsc, prc.prcprioritario, esd.esddsc, prc.usucpf,coo.coodsc , esd.esdid, tpr.tipid, dataultimocadastro, esp.espnumdiasrespexterna, wd.htddata, had.haddtprazoadv
                        
	    		ORDER BY prioridade DESC, dataultimocadastro DESC, dttramite, had.haddtprazoadv DESC
                    ";
                }

            ?>

            </td>
            <td style="background-color: #e9e9e9; color: #404040;" valign="top">
                <input type="hidden" name="esdid" id="esdid" value="<?= $_REQUEST['esdid'] ?>" />
                <div style="float: center; position: relative; width: 300px;">
                    <fieldset style="background-color: #ffffff;">
                        <legend> 
                            <img id="mais_consulta_situacao" style="cursor: pointer;" onclick="exibeConsultaSituacao();" src="../imagens/menos.gif" /> 
                            <b>Consultas por Situa��o</b> - ( <a style="cursor:pointer;font-weight:bold;" onclick="marcarPorName('esdid[]');" title="Listar todas as situa��es">marcar todas</a> )
                        </legend>
                        <table id="consultas_situacao" width="100%" style="display: inline;">
                            <?php
                                $sqlSituacaoAndamento = "
                                    SELECT  esdid as codigo, 
                                            esddsc as descricao 
                                    FROM workflow.estadodocumento 
                                    WHERE tpdid='" . NOVO_WORKFLOW . "' AND esdid not in(" . WF_DESARQUIVADO . ")
                                    ORDER  BY esddsc
                                ";
                                $dadosSituacao = $db->carregar($sqlSituacaoAndamento);

                                $situacao = $_REQUEST['todos'] != '' || $_REQUEST['evento'] == '' ? Array(374, 382, 381, 378, 380) : ( is_array($_REQUEST['esdid']) ? $_REQUEST['esdid'] : Array($_REQUEST['esdid']) );
                                for ($i = 0; $i < count($dadosSituacao); $i++):
                            ?>
                            <?php if ($i % 2 == 0): ?>
                                <tr>
                                    <td>
                                        <input type="checkbox" <?= ((!in_array($dadosSituacao[$i]['codigo'], $situacao)) && count($situacao) > 0 ? "" : "checked") ?> value="<?= $dadosSituacao[$i]['codigo'] ?>" name="esdid[]">
                                    </td>
                                    <td valign="bottom">
                                        <?= $dadosSituacao[$i]['descricao'] ?>
                                    </td>
                                        <?php if (!is_null($dadosSituacao[$i + 1]['descricao'])): ?>
                                    <td>
                                        <input type="checkbox" <?= ((!in_array($dadosSituacao[$i + 1]['codigo'], $situacao)) && count($situacao) > 0 ? "" : "checked") ?> value="<?= $dadosSituacao[$i + 1]['codigo'] ?>" name="esdid[]">
                                    </td>
                                    <td valign="bottom">
                                        <?= $dadosSituacao[$i + 1]['descricao'] ?>
                                    </td>
                            <?php else: ?>
                                <td></td>
                                <td valign="bottom"></td>
                            <?php endif; ?>
                                </tr>
                            <?php endif; ?>
                            <?php endfor; ?>
                        </table>
                    </fieldset>
                </div>
                <br>
                <input type="hidden" name="cooid" id="cooid" value="<?= $_REQUEST['cooid'] ?>" />
                <div style="float: center; position: relative; width: 300px;">
                    <fieldset style="background-color: #ffffff;">
                        <legend> 
                            <img id="mais_consulta_localizacao" style="cursor: pointer;" onclick="exibeConsultaLocalizacao();" src="../imagens/menos.gif" /> 
                            <b>Localiza��o na CONJUR</b> - (<a style="cursor:pointer;font-weight:bold" onclick="marcarPorName('cooid[]')" title="Listar todas as localiza��es">marcar todas</a>)
                        </legend>
                        <table id="consultas_localizacao" width="100%" style="display: inline;">
                            <?php
                                $sqlCoordenacao = "SELECT   coonid as codigo, coodsc as descricao
                                                                                                       FROM     conjur.coordenacao
                                                                                                       ORDER BY	coodsc";
                                $dadosLocalizacao = $db->carregar($sqlCoordenacao);

                                $_REQUEST['cooid'] = is_array($_REQUEST['cooid']) ? $_REQUEST['cooid'] : Array();

                                for ($i = 0; $i < count($dadosLocalizacao); $i++):
                            ?>
                            <?php if ($i % 2 == 0): ?>
                                    <tr>
                                        <td>
                                            <input type="checkbox" <?= ((!in_array($dadosLocalizacao[$i]['codigo'], $_REQUEST['cooid'])) && count($_REQUEST['cooid']) > 0 ? "" : "checked") ?> value="<?= $dadosLocalizacao[$i]['codigo'] ?>" name="cooid[]">
                                        </td>
                                        <td valign="bottom">
                                            <?= $dadosLocalizacao[$i]['descricao'] ?> 
                                        </td>
                                            <?php if (!is_null($dadosLocalizacao[$i + 1]['descricao'])): ?>
                                        <td>
                                            <input type="checkbox" <?= ((!in_array($dadosLocalizacao[$i + 1]['codigo'], $_REQUEST['cooid'])) && count($_REQUEST['cooid']) > 0 ? "" : "checked") ?> value="<?= $dadosLocalizacao[$i + 1]['codigo'] ?>" name="cooid[]">
                                        </td>
                                        <td valign="bottom">
                                            <?= $dadosLocalizacao[$i + 1]['descricao'] ?>
                                        </td>
                            <?php else: ?>
                                        <td></td>
                                        <td valign="bottom"></td>
                            <?php endif; ?>
                                    </tr>
                            <?php endif; ?>
                            <?php endfor; ?>
                        </table>
                    </fieldset>
                </div>
            </td>
        </tr>
    </table>
    <br>
    <table class="tabela" align="center" border="0" cellpadding="5"
           cellspacing="1">
        <tr>
            <td bgcolor="#CCCCCC" align="left">&nbsp; 
            <? if ($permissoes['gravar']) { ?>
                    <input type="button" name="cadastrarProcesso" value="Cadastrar Processo" style="cursor: pointer;" onclick="this.disabled = true; window.location.href = '?modulo=principal/cadastrarprocesso&acao=A';">
            <? } ?>
            </td>
        </tr>
    </table>
</form>

<br>

<div id="div_lista">
    <?php
        if ($_REQUEST['acao'] != 'S' || $_REQUEST['evento'] == 'pesquisa') {
            $dados = $db->carregar($sql);

            if ($dados) {
                foreach ($dados as $chave => $val) {
                    $acao = "<img src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" border=\"0\" align=\"absmiddle\" title=\"Alterar\" onclick=\"window.location='?modulo=principal/editarprocesso&acao=A&prcid=" . $val['prcid'] . "'\"/>";
                    if ((($db->testa_superuser()) || $_SESSION['conjur']['perfilid'] == PRF_ADMINISTRADOR)) {
                        $acao .= "  <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" align=\"absmiddle\" title=\"Excluir.\" onclick=\"apagarProcesso(" . $val['prcid'] . ");\"/>";
                    } else {
                        $acao .= "  <img src=\"/imagens/excluir_01.gif\" style=\"cursor:pointer\"  border=\"0\" align=\"absmiddle\" title=\"Sem autoriza��o para excluir.\" />";
                    }

                    if ($val['prioritario'] == 't') {
                        $prioritario = "<img src=\"/imagens/check_p.gif\" style=\"cursor:pointer\"  border=\"0\" align=\"absmiddle\" title=\"Priorit�rio.\" />";
                    } else {
                        $prioritario = " ";
                    }

                    $dados_array[$chave] = array(
                        "acao"          => $acao,
                        "num_processo"  => $val['prcnumsidoc'] . " ",
                        "interessado"   => $val['prcnomeinteressado'] . " ",
                        "prioridade"    => $val['prioridade'],
                        "prioritario"   => $prioritario,
                        "data"          => $val['dataultimocadastro'],
                        "dttramite"     => $val['dttramite'],
                        "prcdtprazoadv" => $val['prcdtprazoadv'],
                        "coord"         => $val['coodsc'],
                        "situacao"      => $val['esddsc']
                    );
                }

                $arrayDeTiposParaOrdenacao = array();
                $arrayDeTiposParaOrdenacao[] = array("num_processo" => "integer");

                $cabecalho = array("A��es", "N� do Processo SIDOC", "Interessado", "Prioridade", "Relevante", "Data de Entrada", "Data da �ltima Tramita��o", "Prazo Advogado", "Unidade", "Situa��o CONJUR");
                $db->monta_lista_array($dados_array, $cabecalho, 50, 20, '', '100%', '', $arrayDeTiposParaOrdenacao);
            } else {
                $db->monta_lista_array("", $cabecalho, 50, 20, '', '100%', '');
            }
        }
    ?>
</div>