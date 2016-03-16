<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);
include "_funcoes_proinfancia.php";

if ($_REQUEST['req'] == 'geraLote') {
    header('content-type: text/html; charset=ISO-8859-1');
    geraLote($_REQUEST);
    die();
}

if ($_REQUEST['req'] == 'listaMunicipiosComLote') {
    header('content-type: text/html; charset=ISO-8859-1');
    listaMunicipiosComLote($_REQUEST);
    die();
}

if ($_REQUEST['req'] == 'confirmaPagamentoMunicipio') {
    header('content-type: text/html; charset=ISO-8859-1');
    confirmaPagamentoMunicipio($_REQUEST);
    die();
}

if ($_REQUEST['req'] == 'confirmaPagamentoLote') {
    header('content-type: text/html; charset=ISO-8859-1');
    confirmaPagamentoLote($_REQUEST);
    die();
}

if ($_REQUEST['req'] == 'imprimirminutaexcel') {
    ob_clean();

    $nomeArquivo = 'minuta_repasse_proinfantil_' . date("Ymd") . '_lote_' . $_REQUEST['lotid'];

    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=" . $nomeArquivo . ".xls");
    header("Content-Disposition: attachment; filename=" . $nomeArquivo . ".xls");
    header("Content-Description: MID Gera excel");

    $sql = "SELECT
				formata_cpf_cnpj(iue.iuecnpj) as cnpj, 
			  	lot.estuf,
			  	mun.mundescricao,
			  	lot.muncod,
			  	lot.crecheparcial,
			  	lot.crecheintegral,
			  	lot.preescolaparcial,
			  	lot.preescolaintegral,
			  	trim(to_char(lot.valorrepasse, '999G999G999G999G999D99')) as valorrepasse
			FROM 
			 	proinfantil.loteminutaproinfantil lot
				inner join territorios.municipio mun on mun.muncod = lot.muncod
				left join par.instrumentounidade iu
	                inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
                on iu.muncod = mun.muncod
			WHERE lot.lotid = {$_REQUEST['lotid']}
			order by
				lot.estuf,
			    mun.mundescricao";

    $cabecalho = array('CNPJ', 'UF', 'Municípo', 'Código IBGE', 'Creche Parcial', 'Creche Integral', 'Pré-Escola Parcial', 'Pré-Escola Integral', 'Valor do Repasse');
    $db->monta_lista_tabulado($sql, $cabecalho, 1000000, 5, 'N', '100%', 'S');
    exit;
}

if ($_REQUEST['req'] == 'imprimirminuta') {
    $sql = "SELECT
			ar.arqnome,
			ar.arqextensao,
			ar.arqtipo
		FROM
			proinfantil.loteproinfancia lp
		    inner join public.arquivo ar on ar.arqid = lp.arqid
		WHERE
			lp.lotid = " . $_REQUEST['lotid'];

    $arquivo = $db->pegaLinha($sql);

    $caminho = APPRAIZ . "arquivos/proinfantil/minutaproinfantil/" . $arquivo['arqnome'] . '.pdf';

    $filename = $arquivo['arqnome'] . '.' . $arquivo['arqextensao'];
    header('Content-type: ' . $arquivo['arqtipo']);
    header('Content-Disposition: attachment; filename=' . $filename);
    readfile($caminho);
    exit();
}
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
?>
<style>
    .div_lote {
        background: #white;
    }
    .div_lote:hover {
        background: #eeeeaa;
    }
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
    <tr>
        <td colspan="2" class="subtituloEsquerda">
            <input type="button" value="Voltar" id="btnVoltar"/>
        </td>
    </tr>
</table>
<br/>
<?php
$perfil = pegaPerfil($_SESSION['usucpf']);

$db->cria_aba($abacod_tela, $url, '');

$titulo_modulo = "Lista de Obras";
monta_titulo($titulo_modulo, 'Selecione os filtros e agrupadores desejados');
echo "<br>";

if (in_array(PERFIL_ANALISTA, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || $db->testa_superuser()) {
    $abasPar = array(
        0 => array("descricao" => "Lotes",
            "link" => "proinfantil.php?modulo=principal/pagamentoLote&acao=A"),
        1 => array("descricao" => "Criar Lote",
            "link" => "proinfantil.php?modulo=principal/pagamentoLote&acao=A&criar=1")
    );
} else {
    $abasPar = array(
        0 => array("descricao" => "Lotes",
            "link" => "proinfantil.php?modulo=principal/pagamentoLote&acao=A")
    );
}
echo montarAbasArray($abasPar, $_SESSION['favurl']);
if ($_POST) {
    extract($_POST);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>

    $(function() {

        if ($.browser.msie) {
            $('.div_tramita').css('width', '200px');
            $('.div_minuta').css('width', '200px');
            $('.div_excel').css('width', '200px');
        }

        function aguarda(val) {
            jQuery('#pesquisar').attr('disabled', val);
            jQuery('#limpar').attr('disabled', val);
            if (val) {
                jQuery('#aguardando').show();
            } else {
                jQuery('#aguardando').hide();
            }
        }
        $('#btnPesquisar').click(function() {
            $('#formularioFiltro').submit();
        });
        $('#btnLimpar').click(function() {
            window.location = window.location;
        });
        $('#btnVoltar').click(function() {
            window.location = 'proinfantil.php?modulo=inicio&acao=C';
        });
        $('#btnGerar').click(function() {
            if ($('[name="lotnumportaria"]').val() == '') {
                alert('Informe o Número da Portaria');
                $('[name="lotnumportaria"]').focus();
                return false;
            }
            if ($('[name="lotdataportaria"]').val() == '') {
                alert('Informe a Data da Portaria');
                $('[name="lotdataportaria"]').focus();
                return false;
            }

            if ($('[name="pinid[]"]:checked').length > 0) {
                $('#req').val('geraLote');
                $('#btnGerar').attr('disabled', true);
                $('[name="formulario"]').submit();
            } else {
                alert('Escolha pelo menos um município.');
                return false;
            }
        });

        /*$('#btnImprimir').click(function(){
         
         var texto = tinyMCE.get('texto').getContent();
         var horizontal = 850;	
         var vertical   = 650;
         
         document.getElementById('imitexto').value = texto;
         
         var res_ver = screen.height;
         var res_hor = screen.width;
         
         var pos_ver_fin = (res_ver - vertical)/2;
         var pos_hor_fin = (res_hor - horizontal)/2;
         
         var form = document.getElementById('formulario');
         
         form.action = 'proinfantil.php?modulo=principal/popupImprimirMinuta&acao=A';
         form.target = 'imprimir';
         var janela = window.open('proinfantil.php?modulo=principal/popupImprimirMinuta&acao=A','imprimir',"width="+horizontal+",height="+vertical+",top="+pos_ver_fin+",left="+pos_hor_fin+",status=yes");
         janela.focus();
         form.submit();
         });*/

        $('[name="pinid[]"]').click(function() {
            if ($('[name="pinid[]"]').length == $('[name="pinid[]"]::checked').length) {
                $('.todos').attr('checked', true);
            } else {
                $('.todos').attr('checked', false);
            }
        });
        $('.todos').click(function() {
            if ($('[name="pinid[]"]').length != $('[name="pinid[]"]::checked').length) {
                $('[name="pinid[]"]').attr('checked', true);
            } else {
                $('[name="pinid[]"]').attr('checked', false);
            }
        });
        $('.tramitaDocid').live('click',function(){
            var intChecked = 0;
            $('[name="docid[]"]').each(function() {
                if ($(this).attr('checked') === true && !$(this).attr('disabled')) {
                    intChecked += 1;
                }
            });
            
            if( intChecked > 0 ){
                $('#req').val('confirmaPagamentoMunicipio');
                $('.tramitaDocid').attr('disabled', true);
                $('[name="formulario"]').submit();
            }else{
                alert('Escolha pelo menos um Município.');
                return false;
            }
	});
        $('.div_tramita').click(function() {
            if ($('[name="docid[]"]:checked').length > 0) {
                $('#lotid').val($(this).attr('id'));
                $('#req').val('confirmaPagamentoLote');
                $('[name="formulario"]').submit();
            } else {
                alert('Selecione um município.');
            }
        });
        $('.div_minuta').click(function() {
            $('#lotid').val($(this).attr('id'));
            $('#req').val('imprimirminuta');
            $('[name="formulario"]').submit();
        });

        $('.div_excel').click(function() {
            $('#lotid').val($(this).attr('id'));
            $('#req').val('imprimirminutaexcel');
            $('[name="formulario"]').submit();
        });

        $('.div_lote').click(function() {
            aguarda(true);
            var lotid = $(this).attr('id');
            console.log(lotid);
            if ($('#td_' + lotid).html() != '') {
                if ($('#td_' + lotid).css('display') == 'none') {
                    $('#td_' + lotid).show();
                    $('#imgmais' + lotid).hide();
                    $('#imgmenos' + lotid).show();
                } else {
                    $('#td_' + lotid).hide();
                    $('#imgmais' + lotid).show();
                    $('#imgmenos' + lotid).hide();
                }
                aguarda(false);
            } else {
                $.ajax({
                    type: "POST",
                    url: 'proinfantil.php?modulo=principal/pagamentoLote&acao=A',
                    data: "req=listaMunicipiosComLote&lotid=" + lotid,
                    async: false,
                    success: function(msg) {
                        $('#imgmais' + lotid).hide();
                        $('#imgmenos' + lotid).show();

                        jQuery('#td_' + lotid).css('height', '400px');
                        jQuery('#td_' + lotid).css('overflow', 'scroll');
                        jQuery('#td_' + lotid).html(msg);
                        aguarda(false);
                    }
                });
            }
            if ($.browser.msie)
                $('.tramitaDocid').css('width', '250px');
        });
        aguarda(false);
    });

</script>
<center>
    <div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
        <div style="margin-top:250px; align:center;">
            <img border="0" title="Aguardando" src="../imagens/carregando.gif">
            Carregando...
        </div>
    </div>
</center>
<form method="post" name="formularioFiltro" id="formularioFiltro" action="">

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
        <?php
        if (!$_REQUEST['criar']) {
            ?>
            <tr>
                <td class="subtituloDireita">Lote</td>
                <td>
                    <?php
                    $sql = "SELECT 
							lotid as codigo,
							lotdsc as descricao
						FROM
							proinfantil.loteproinfancia
						ORDER BY 
							lotid desc";
                    $db->monta_combo('lotid', $sql, 'S', 'Selecione...', '', '', '');
                    ?>
                </td>
            </tr>
            <?php
        } else {
            ?>
            <tr>
                <td class="subtituloDireita">UF</td>
                <td>
                    <?php
                    $sql = "SELECT 
							estuf as codigo,
							estdescricao as descricao
						FROM
							territorios.estado
						ORDER BY 
							estuf";
                    $db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Município</td>
                <td>
                    <?php echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); ?>
                </td>
            </tr>
            <?php
        }
        ?>
        <tr>
            <td class="subtituloDireita">&nbsp;</td>
            <td class="subtituloEsquerda">
                <input type="button" value="Pesquisar" id="btnPesquisar" />
                <input type="button" value="Limpar" id="btnLimpar" />
            </td>
        </tr>
    </table>
</form>

<form method="post" name="formulario" id="formulario" action="">
    <input type="hidden" name="req" id="req" value=""/>
    <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
        <?php
        if ($_REQUEST['criar']) {
            ?>
            <tr>
                <td class="subtitulodireita" width="40%"><b>Número da Portaria:</b></td>
                <td><?php echo campo_texto('lotnumportaria', 'S', 'S', '', 15, '', '', ''); ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita"><b>Data:</b></td>
                <td><?php echo campo_data2('lotdataportaria', 'S', 'S', '', 'S', '', '', $lotdataportaria); ?></td>
            </tr>
            <tr>
                <td colspan="2">
            <center>
                <div style="overflow:auto;height:400px;width:100%;background-color:white;">
                    <?php
                    print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%">';
                    print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Lista de obras para gerar lotes</label></td></tr><tr>';
                    print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >' . $linha2 . '</td></tr></table>';
                    listaMunicipiosSemLote($_POST);
                    ?>
                </div>
            </center>
            </td>
            </tr>
            <?php
            if (in_array(PERFIL_ANALISTA, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || $db->testa_superuser()) {
                ?>
                <tr>
                    <td colspan="2">
                        <table class="tabela" align="center" style="width: 100%"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
                            <tr>
                                <td class="subtituloEsquerda">
                            <center>
                                <input type="button" value="Gerar Lote" id="btnGerar" />
                            </center>
                    </td>
                </tr>
            </table>
        </td>
        </tr>
        <?php
    }
} else {
    if ($_REQUEST['lotid']){
        $filtro = " and lot.lotid = {$_REQUEST['lotid']}";
    }

    $sql = "SELECT DISTINCT
                lot.lotid, lot.lotdsc, lot.lotnumportaria, TO_CHAR(lotdtcriacao, 'YYYY') as ano
            FROM 
                proinfantil.loteproinfancia lot
            INNER JOIN proinfantil.proinfantil 	dcm ON dcm.lotid = lot.lotid
            LEFT  JOIN workflow.documento		doc ON doc.docid = dcm.docid AND doc.esdid = " . WF_PROINFANTIL_ANALISADO . "
            LEFT  JOIN workflow.estadodocumento	esd ON esd.esdid = doc.esdid
            WHERE 
                lotstatus = 'A'
                $filtro
            ORDER BY 1";
    $lotes = $db->carregar($sql);
    ?>
    <tr>
        <td colspan="2">
            <input type="hidden" name="lotid" id="lotid" value=""/>
            <table class="tabela" align="center" border="0" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">&nbsp;</td>
                    <td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Descrição</b></td>
                    <td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Portaria</b></td>
                    <td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Ano</b></td>
                    <td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><b>Ações</b></td>
                </tr>
                <?php
                if ($lotes[0] != '') {
                    foreach ($lotes as $lote) {
                        $key % 2 ? $cor = "#FFFFFF" : $cor = "";
                        ?>
                        <tr bgcolor="<?= $cor; ?>" id="tr_<?= $key ?>" onmouseout="this.bgColor = '<?= $cor; ?>';" onmouseover="this.bgColor = '#ffffcc';">
                            <td>
                                <div id="<?= $lote['lotid']; ?>" class="div_lote" style="float:left;cursor:pointer;width:100%;">
                                    <img src="../imagens/mais.gif" style="cursor:pointer; padding-left: 5px; padding-top: 5px;" id="imgmais<?= $lote['lotid']; ?>" >
                                    <img src="../imagens/menos.gif" style="cursor:pointer; display: none; padding-left: 5px; padding-top: 5px;" id="imgmenos<?= $lote['lotid']; ?>" >
                                </div>
                            </td>
                            <td width="55%"><b><?= $lote['lotdsc'] ?></b></td>
                            <td><?= $lote['lotnumportaria']; ?></td>
                            <td><?= $lote['ano']; ?></td>
                            <td align="center">
                                <?php
                                $sql = "SELECT DISTINCT
                                    doc.esdid
                                    FROM 
                                            proinfantil.loteproinfancia lot
                                            INNER JOIN proinfantil.proinfantil 	dcm ON dcm.lotid = lot.lotid
                                            LEFT  JOIN workflow.documento		doc ON doc.docid = dcm.docid AND doc.esdid = " . WF_PROINFANTIL_ENVIADO_PARA_PAGAMENTO . "
                                            LEFT  JOIN workflow.estadodocumento	esd ON esd.esdid = doc.esdid
                                    WHERE 
                                            lotstatus = 'A' AND lot.lotid = " . $lote['lotid'] . "
                                    ORDER BY
                                            1";
                                $testaLote = $db->pegaUm($sql);

                                if ($testaLote != '' && ( in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || $db->testa_superuser() )) {
                                    ?>
                                    <input type="button" value="Confirmar Pagamento do Lote" class="div_tramita" id="<?= $lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
                                    <input type="button" value="Imprimir Minuta Portaria" class="div_minuta" id="<?= $lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
                                    <input type="button" value="Imprimir Minuta Portaria em Excel" class="div_excel" id="<?= $lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
                                <?php } elseif (in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) || $db->testa_superuser()) {
                                    ?>	
                                    <input type="button" value="Confirmar Pagamento do Lote" disabled="disabled" style="cursor:pointer; width:160px; white-space: normal;"/>
                                    <input type="button" value="Imprimir Minuta Portaria" class="div_minuta" id="<?= $lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
                                    <input type="button" value="Imprimir Minuta Portaria em Excel" class="div_excel" id="<?= $lote['lotid'] ?>" style="cursor:pointer; width:160px; white-space: normal;"/>
                                    <? }
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="6"><div id="td_<?=$lote['lotid'] ?>"></div></td>
                         </tr>
                        <?php
                    }
                }
                ?>
            </table>
        </td></tr>
    <?php
}
?>
</table>
</form>