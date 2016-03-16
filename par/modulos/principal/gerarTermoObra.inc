<?php
include '_funcoes_termo.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

if ($_REQUEST['proid']) {
    $_SESSION['par_var']['proid'] = $_REQUEST['proid'];
}

if ($_REQUEST['proid']) {
    $processo = $db->pegaUm("select pronumeroprocesso from par.processoobra where proid = {$_REQUEST['proid']}");
    verificaEmpenhoSigef($processo);
}

// Monta tarja vermelha com aviso de pendência de obras
montarAvisoCabecalho(null, null, null, null, $_REQUEST['proid']);
?>
<html>
    <head>
        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        <script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
        <script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
        <script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
        <link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
        <style type="text/css">
            #listaRestricoes thead tr td
            {
                background-color: #F7F7F7;
                font-weight: bold;
                font-size: 12px;
                font-family: arial;
            }

        </style>
    </head>
    <body>
        <?php
        monta_titulo($titulo_modulo, '&nbsp;');
        ?>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script type="text/javascript" src="./js/par.js"></script>
        <? $tipoObra = cabecalhoSolicitacaoTermo(); ?>
        <?php
        $muncod = $_REQUEST['muncod'];
        $arrDados = $db->pegaLinha("SELECT 
								inuid 
							FROM 
								par.instrumentounidade where muncod = '{$muncod}'");

        $inuId = $arrDados['inuid'];

        $today = date('Y-m-d');
        if ($inuId) {
            $sqlRestricoes = "SELECT 
			res.resdescricao,
			tpr.tprdescricao as tipo_restricao,
			CASE WHEN res.restemporestricao THEN
				to_char(resdatainicio, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_inicio,
			
			CASE WHEN res.restemporestricao THEN
				to_char(resdatafim, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_fim
		FROM
			par.restricaofnde res
		INNER JOIN
			par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
		INNER JOIN
			par.instrumentounidaderestricaofnde iur ON res.resid = iur.resid
		WHERE
			res.resstatus = 'A'
		AND
			iur.inuid = {$inuId}
		AND
			CASE WHEN res.restemporestricao THEN
				CASE
					WHEN '{$today}' between resdatainicio and resdatafim THEN
						true
					ELSE
						false
					END
			ELSE
				true
			END
		";

            $arrRestricoes = $db->carregar($sqlRestricoes);
            if (count($arrRestricoes) && is_array($arrRestricoes)) {
                ?>
                <table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 95%;" cellpadding="3" cellspacing="1">
                    <thead>
                        <tr>
                            <td  colspan="4"> Restrições do município </td>
                        </tr>

                    </thead>
                    <tbody>

                        <?php
                        foreach ($arrRestricoes as $kRes => $vRes) {
                            echo "<tr style=\"color:red;\">
                                            <td style=\"width:40%;\" > {$vRes['resdescricao']} </td>
                                    </tr>";
                        }
                        ?>
                    </tbody>
                </table>
                <br>
                <?php
            }
        }
        ?>
        <script>
            function excluirAnexoTermo(terid) {
                var conf = confirm('Deseja excluir o anexo?');
                if (conf) {
                window.location = 'par.php?modulo=principal/gerarTermoObra&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo = principal / gerarTermoObra & acao = A & muncod = '.$_REQUEST['muncod']); ?>&requisicao=excluiranexotermo&terid=' + terid;
                }
            }

            function enviarAnexo(terid) {
                displayMessage('par.php?modulo=principal/gerarTermoObra&acao=A&requisicao=telaanexo&urlgo=<? echo md5_encrypt('par.php?modulo = principal / gerarTermoObra & acao = A & muncod = '.$_REQUEST['muncod']); ?>&terid=' + terid);
            }

            $(document).ready(function () {
                <?php if ($_REQUEST['muncod']) { ?>
                    carregarListaPreObraTermo(<?php echo $_REQUEST['muncod'] ?>, '<?php echo $tipoObra ?>', false);
                    carregarListaTermos('<?php echo $_REQUEST['muncod'] ?>', '<?php echo $_SESSION['par_var']['proid'] ?>', false);
                <?php } elseif ($_REQUEST['estuf']) { ?>
                    carregarListaPreObraTermo(false, '<?= $tipoObra ?>', '<?= $_REQUEST['estuf'] ?>');
                    carregarListaTermos(false, '<?= $_SESSION['par_var']['proid'] ?>', '<?= $_REQUEST['estuf'] ?>');
                <?php } ?>

                $('.aditivoTermo').live('click', function () {
                    var variaveis = $(this).attr('id').split('_');
                    var proid = variaveis[0];
                    var protipo = variaveis[1];
                    var muncod = variaveis[2];
                    var estuf = variaveis[3];
                    var terid = variaveis[4];
                    if (muncod == 'null' || muncod == '') {
                        muncod = '';
                    } else {
                        muncod = '&muncod=' + muncod;
                    }

                    $.ajax({
                        type: "POST",
                        url: "par.php?modulo=principal/termoPac&acao=A",
                        data: "requisicao=formTermoAditivo&tipo=simples&proid=" + proid + "&muncod=" + muncod + "&estuf=" + estuf + "&terid=" + terid,
                        async: false,
                        success: function (msg) {
                            $("#dialogAditivo").html(msg);
                            $("#dialogAditivo").dialog({
                                resizable: true,
                                width: 400,
                                modal: true,
                                buttons: {
                                    "Sim": function () {

                                        var arPreid = '';
                                        $('[name="preids[]"]').each(function (i, obj) {
                                            if (arPreid == '') {
                                                arPreid = $(obj).val();
                                            } else {
                                                arPreid += "," + $(obj).val();
                                            }
                                        });
                                        if (arPreid == '') {
                                            alert('Nenhuma obra carregada');
                                            $("#dialogAditivo").html('')
                                            return false;
                                        }

                                        window.open('par.php?modulo=principal/modeloTermoAditivoObra&acao=A&termoaditivo=true&proid=' + proid + '&arPreid=' + arPreid + muncod + '&estuf=' + estuf + '&tipoobra=' + protipo,
                                                'modelo',
                                                "height=600,width=400,scrollbars=yes,top=0,left=0");
                                        $(this).dialog("close");
                                    },
                                    "Não": function () {
                                        $(this).dialog("close");
                                    }

                                }
                            });
                        }
                    });
                });
                $('.regerarTermo').live('click', function () {

                    var variaveis = $(this).attr('id').split('_');
                    var proid = variaveis[0];
                    var protipo = variaveis[1];
                    var muncod = variaveis[2];
                    var estuf = variaveis[3];
                    var terid = variaveis[4];
                    if (muncod == 'null' || muncod == '') {
                        muncod = '';
                    } else {
                        muncod = '&muncod=' + muncod;
                    }

                    $.ajax({
                        type: "POST",
                        url: "par.php?modulo=principal/termoPac&acao=A",
                        data: "requisicao=formRegeraTermo&tipo=simples&proid=" + proid + "&muncod=" + muncod + "&estuf=" + estuf + "&terid=" + terid,
                        async: false,
                        success: function (msg) {
                            $("#dialog").html(msg);
                            $("#dialog").dialog({
                                resizable: true,
                                width: 400,
                                modal: true,
                                buttons: {
                                    "Sim": function () {

                                        var arPreid = '';
                                        $('[name="preids[]"]').each(function (i, obj) {
                                            if (arPreid == '') {
                                                arPreid = $(obj).val();
                                            } else {
                                                arPreid += "," + $(obj).val();
                                            }
                                        });
                                        if (arPreid == '') {
                                            alert('Nenhuma obra carregada');
                                            $("#dialog").html('')
                                            return false;
                                        }

                                        window.open('par.php?modulo=principal/modeloTermoObra&acao=A&regerar=true&proid=' + proid + '&arPreid=' + arPreid + muncod + '&estuf=' + estuf + '&tipoobra=' + protipo,
                                                'modelo',
                                                "height=600,width=400,scrollbars=yes,top=0,left=0");
                                        $(this).dialog("close");
                                    },
                                    "Não": function () {
                                        $(this).dialog("close");
                                    }

                                }
                            });
                        }
                    });
                });
            });
        </script>

        <div id="dialog" title="Regerar Termo" style="display:none;"></div>
        <div id="dialogAditivo" title="Gerar Termo Aditivo" style="display:none;"></div>

        <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td id="listapreobra">Carregando...</td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    <?php
                    $terid = $db->pegaUm("SELECT terid FROM par.termocompromissopac WHERE proid='".$_SESSION['par_var']['proid']."' AND terstatus='A'");
                    // se não tiver termo, exibir o botão de gerar
                    if(!$terid) :
                    ?>
                    <form name="formulario" id="formulario">
                        <input type="button" name="solicitar" value="Liberar Termo para Assinatura" onclick="gerarTermo('<?php echo $_REQUEST['muncod'] ?>', '<?php echo $tipoObra ?>', '<?php echo $_SESSION['par_var']['proid'] ?>')" />
                    </form>
                    <?php
                    endif;
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloCentro">Lista de termos</td>
            </tr>
            <tr>
                <td id="listatermo">Carregando...</td>
            </tr>
        </table>
        <script>
            messageObj = new DHTML_modalMessage(); // We only create one object of this class
            messageObj.setShadowOffset(5); // Large shadow

            function displayMessage(url) {
                messageObj.setSource(url);
                messageObj.setCssClassMessageBox(false);
                messageObj.setSize(690, 400);
                messageObj.setShadowDivVisible(true); // Enable shadow for these boxes
                messageObj.display();
            }
            function displayStaticMessage(messageContent, cssClass) {
                messageObj.setHtmlContent(messageContent);
                messageObj.setSize(600, 150);
                messageObj.setCssClassMessageBox(cssClass);
                messageObj.setSource(false); // no html source since we want to use a static message here.
                messageObj.setShadowDivVisible(false); // Disable shadow for these boxes	
                messageObj.display();
            }
            function closeMessage() {
                messageObj.close();
            }
        </script>