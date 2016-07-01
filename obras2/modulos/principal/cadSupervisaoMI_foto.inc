<?php
$supervisao = new Supervisao();
$supid = $supervisao->pegaSupidByObraAndSmiid($obrid, $supervisaoMi->smiid);
if(!$supid) {
    if (empty($_SESSION['obras2']['smiid'])) {
        echo 'Preencher primeiramente o cronograma.';
        exit;
    }
    $smiid = $_SESSION['obras2']['smiid'];
    $supervisao->smiid = $_SESSION['obras2']['smiid'];
    $supervisao->obrid = $obrid;
    $supid             = $supervisao->salvar();
}
$supervisao->carregarPorId($supid);
$vistoriaEstrutura = $supervisao->getDados();
$_SESSION['obras2']['supid'] = $supid;
?>

<table class="tabela" bgcolor="white" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
    <tbody>
    <tr bgcolor="#DCDCDC">
        <td width="100%" align="center" colspan="2">
            <label class="TituloTela" style="color:#000000;">Cadastro de Fotos da Supervisão</label>
        </td>
    </tr>
    </tbody>
</table>
<table class="tabela" bgcolor="white" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
<tbody>
<tr>
    <!--<td class="SubTituloDireita">Observação da Vistoria</td>-->
    <td class="SubTituloDireita" valign="top">Relatório Técnico do Acompanhamento</td>
    <td colspan="2">
        <?php
        $supobs = $vistoriaEstrutura['supobs'];
        $desc_relatorio = "No relatório deverão constar informações pertinentes a execução da obra, tais como:<br/><br/>&nbsp;&nbsp;&nbsp;<b>a.</b> O nome do Responsável Técnico da obra e o número da ART de execução.<br/>&nbsp;&nbsp;&nbsp;<b>b.</b> A existência ou não de todos os documentos necessários para a execução da obra, tais como: projetos, especificações, memoriais, ART´s, licenças, alvarás, caderno de encargos, diários de obras (preenchido e atualizado), dentre outros.<br/>&nbsp;&nbsp;&nbsp;<b>c.</b> Informações sobre o canteiro de obra (existência, organização e segurança do Trabalho).<br/>&nbsp;&nbsp;&nbsp;<b>d.</b> Atrasos na execução da obra e a necessidade ou não de aditivos.<br/>&nbsp;&nbsp;&nbsp;<b>e.</b> Comentário sobre a situação geral da obra e/ou de cada serviço da obra.<br/>&nbsp;&nbsp;&nbsp;<b>f.</b> Outros apontamentos.";
        ?>
        <?php echo  campo_textarea('supobs', 'S', $somenteLeitura, '', '100', '8', '5000', '', '', $desc_relatorio); ?>
    </td>
</tr>
<?php
if ($supid) {
    $sql = "SELECT
                    fot.*, arq.arqdescricao
            FROM
                    obras2.fotos AS fot
            LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
            WHERE
                    obrid =" . $obrid . " AND
                    supid=" . $supid . "
            ORDER BY fotordem;";
    $fotos = ($db->carregar($sql));
}
?>
<tr id="tr10">
    <td colspan="2" align="center">
    </td>
</tr>

<tr id="">
    <td colspan="2" align="center">

<input type="hidden" name="supid" value="<?php echo $supid; ?>"/>
<input type="hidden" name="hdn_fotos_galeria" id="hdn_fotos_galeria" value=""/>
<input type="hidden" name="hdn_fotos_supervisao" id="hdn_fotos_supervisao" value=""/>
<?php
if (!$supid) {
    $sql = "SELECT
                arq.arqid
            FROM
                public.arquivo arq
            INNER JOIN
                obras2.arquivosobra oar ON oar.arqid = arq.arqid AND
                						   oar.aqostatus = 'A'
            INNER JOIN
                seguranca.usuario seg ON seg.usucpf = oar.usucpf
            INNER JOIN
                obras2.supervisao supv ON supv.obrid = oar.obrid
            WHERE
                oar.obrid = {$_SESSION['obras2']["obrid"]} AND
                tpaid = " . TIPO_ARQUIVO_FOTO_VISTORIA . "
            AND
                (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
            ORDER BY
                arq.arqid";
} else {
    $sql = "SELECT DISTINCT
                arq.arqid
            FROM
                obras2.fotos fot
            INNER JOIN
                public.arquivo arq ON arq.arqid = fot.arqid
            INNER JOIN
                obras2.arquivosobra oar ON arq.arqid = oar.arqid
            WHERE
                fot.obrid = {$_SESSION['obras2']["obrid"]}
                AND fot.supid = {$supid}
                AND oar.tpaid = " . TIPO_ARQUIVO_FOTO_VISTORIA . "
                AND oar.aqostatus = 'A'
            ORDER BY
                arq.arqid";
}
$arrFotosGaleria = $db->carregar($sql);
// array que armazenará o arqid da Galeria
$arrArqidGaleria = array();
if (is_array($arrFotosGaleria)) {
    foreach ($arrFotosGaleria as $foto) {
        $arrArqidGaleria[] = $foto['arqid'];
    }
}
?>
<table class="tabela" id="tbl_graggable" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td bgcolor="#DCDCDC" colspan="2">
            <b>Fotos do Acompanhamento</b>
        </td>
    </tr>
    <tr>
        <td <?php echo $supid ? "width=\"50%\"" : "colspan='2'" ?> >
            <fieldset class="field_fotos">
                <legend>Fotos do Acompanhamento</legend>
                <ul id="fotos_supervisao" class="div_fotos">
                    <?php if (is_array($fotos)): $arrFotos = $fotos; ?>
                        <?php $n = 1 ?>
                        <?php foreach ($arrFotos as $foto): ?>
                            <?php $pagina = floor($n / 16); ?>

                            <?php
                            $sql = "SELECT arqtipo, arqid  FROM public.arquivo
                                        WHERE arqid = '" . $foto['arqid'] . "'";
                            $dados = $db->pegaLinha($sql);

                            $imgend = APPRAIZ . 'arquivos/obras2/' . floor($foto['arqid'] / 1000) . '/' . $foto['arqid'];

                            if (is_file($imgend)) {
                                $img_max_dimX = 100;
                                $img_max_dimY = 85;

                                $imginfo = getimagesize($imgend);

                                $width = $imginfo[0];
                                $height = $imginfo[1];

                                if (($width > $img_max_dimX) or ($height > $img_max_dimY)) {
                                    if ($width > $height) {
                                        $w = $width * 0.9;
                                        while ($w > $img_max_dimX) {
                                            $w = $w * 0.9;
                                        }
                                        $w = round($w);
                                        $h = ($w * $height) / $width;
                                    } else {
                                        $h = $height * 0.9;
                                        while ($h > $img_max_dimY) {
                                            $h = $h * 0.9;
                                        }
                                        $h = round($h);
                                        $w = ($h * $width) / $height;
                                    }
                                } else {
                                    $w = $width;
                                    $h = $height;
                                }

                                $tamanho = " width=\"$w\" height=\"$h\" ";
                            } else {
                                $tamanho = "";
                            }
                            $eschema = ($foto['obrid_1'] ? 'obras' : 'obras2');
                            ?>
                            <li id="foto_<?php echo $foto['arqid'] ?>"
                                class="nodraggable<?php echo in_array($foto['arqid'], $arrArqidGaleria) ? " f_selected" : "" ?>">
                                <img <?php echo $tamanho; ?> class="img_foto"
                                                             ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>', '<?php echo $pagina ?>')"
                                                             src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?php echo $eschema ?>"/>
                            </li>
                            <?php $n++ ?>
                        <?php endforeach; ?>
                    <?php endif; ?>
                </ul>
            </fieldset>
        </td>
    </tr>
    <tr>
        <td bgcolor="#c0c0c0" colspan="2" align="center">
            <input <?php echo(($habilitado) ? "" : "disabled=\"disabled\""); ?>
                type="button" name="add_fotos"
                onmouseover="return escape('Gerenciamento de fotos do acompanhamento (inserir, alterar e remover).');"
                value="<?php echo($supid ? 'Alterar Fotos' : 'Inserir Fotos') ?>"
                onclick="ImageComponent( <?php if (isset($supid)) {
                    echo "'?funcao=AtualizaFotos&supid=" . $supid . "'";
                } else {
                    echo "' '";
                } ?>);">
            <?php
            if ($supid):
                ?>
                <input <?php echo(($habilitado) ? "" : "disabled=\"disabled\""); ?>
                    type="button"
                    name="btn_salvar_fotos"
                    onmouseover="return escape('Salvar as alterações das fotos de acompanhamento e galeria.');"
                    value="Salvar Fotos"
                    onclick="salvarFotos()">
            <?php
            endif;
            ?>
        </td>
    </tr>
    <tr>
        <td colspan="2">
        </td>
    </tr>
</tbody>
</table>
</td>
</tr>
<script type="text/javascript">

    function ImageComponent(params) {
        var a = window.open("plugins/component_foto.php" + params, "inserir_fotos", "scrollbars=yes,height=540,width=630");
        a.focus();
    }

    <?php
    $arPflcod = array(PFLCOD_SUPERVISOR_MEC,
                      PFLCOD_ADMINISTRADOR,
                      PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                      PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                      PFLCOD_SUPERVISOR_UNIDADE,
                      PFLCOD_GESTOR_UNIDADE);
    if (possuiPerfil($arPflcod)) {
        ?>
    jQuery(function () {
        jQuery("#fotos_supervisao").sortable({
            placeholder: "draggable_space",
            connectWith: "#fotos_supervisao",
            cancel: ".nodraggable"
        });
        jQuery("#fotos_galeria").droppable({
            drop: function (event, ui) {
                if (!jQuery("#s_" + ui.draggable.attr("id")).html()) {
                    jQuery("<li id=\"s_" + ui.draggable.attr("id") + "\" class=\"nodraggable\" ><img src=\"../imagens/fechar.jpeg\" title=\"Remover Foto\" class=\"fechar\" onclick=\"removerFotoGaleria('" + ui.draggable.attr("id") + "')\" />" + ui.draggable.html() + "</li>").appendTo(this);
                    jQuery("#s_" + ui.draggable.attr("id") + " .img_foto").attr("style", "margin-top:-15px");
                    jQuery("#" + ui.draggable.attr("id")).attr("class", "draggable f_selected");
                }
            }
        }).sortable({
                cancel: ".nodraggable"
            });
    });
    function removerFotoGaleria(foto) {
        jQuery("#s_" + foto).remove();
        jQuery("#" + foto).attr("class", "draggable");
    }
    <?php } ?>


    function abrirGaleria(arqid, pagina) {
        window.open("../slideshow/slideshow/obras2_galeriaGaleriaFotos.php?&arqid=" + arqid + "&supid=<?php echo $supid ?>", "imagem", "width=850,height=600,resizable=yes");
    }

    function salvarFotos() {
        jQuery("[name=btn_salvar_fotos]").val("Carregando...");
        jQuery("[name=btn_salvar_fotos]").attr("disabled", "disabled");
        var arrFotosSupervisao = jQuery("#fotos_supervisao").sortable("serialize", {key: 'fotosSupervisao[]'});
        var url = window.location + "&ajaxFotosGaleria=true&" + arrFotosSupervisao;
        jQuery.ajax({
            url: url,
            success: function (data) {
                if (data == "true") {
                    alert("Operação realizada com sucesso!");
                    jQuery("[name=btn_salvar_fotos]").val("Salvar Fotos");
                    jQuery("[name=btn_salvar_fotos]").attr("disabled", "");
                } else {
                    alert("Não foi possível realizar a operação!");
                    jQuery("[name=btn_salvar_fotos]").val("Salvar Fotos");
                    jQuery("[name=btn_salvar_fotos]").attr("disabled", "");
                }
            }
        });
    }

</script>


<script type="text/css">
    .div_fotos {
        list-style-type: none;
        margin: 0;
        padding: 0;
        padding-top: 3px
    }

    .div_fotos li {
        font-size: 1.2em;
        height: 110px;
        height: 90px;
        padding: 1px
    }

    html > body .div_fotos li {
        height: 80px;
        line-height: 1.2em;
    }

    .field_fotos {
        padding: 10px;
    }

    .div_fotos {
        height: 300px;
        overflow: auto;
    }

    .draggable {
        width: 110px;
        height: 90px;
        margin: 3px;
        border: solid 1px black;
        float: left;
        cursor: move;
        text-align: center;
        background-color: #FFFFFF
    }

    .nodraggable {
        width: 110px;
        height: 90px;
        margin: 3px;
        border: solid 1px black;
        float: left;
        text-align: center;
        background-color: #FFFFFF
    }

    .draggable_space {
        line-height: 1.2em;
        width: 110px;
        height: 90px;
        margin: 3px;
        float: left;
        cursor: pointer;
        background-color: #CCCCCC
    }

    .f_selected {
        border: solid 1px red;
    }

    .fechar {
        position: relative;
        margin-left: 105px;
        top: -8px;
        cursor: pointer
    }

    .img_foto {
        z-index: 2;
    }

    .img_class {
        margin-top: -15px;
    }
</script>


<script type="text/javascript">
function ctrlObs(qstid, val) {
    if (val == true) {
        jQuery('#div_obs_' + qstid).show();
    } else {
        jQuery('#div_obs_' + qstid).hide()
            .find('textarea')
            .val('');
    }
}

function ctrlImg(qstid, val) {
    if (val == true) {
        jQuery('#div_arq_' + qstid).show();
    } else {
        jQuery('#div_arq_' + qstid).hide()
            .find('input:file')
            .val('');
    }
}

var totalArq = new Array();
function addArquivo(qstid) {
    totalArq[qstid] = (totalArq[qstid] ? totalArq[qstid] + 1 : 1);
    jQuery('<br>').appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
    jQuery('#div_arq_' + qstid).find('[type=file]:eq(0)')
        .clone()
        .val('')
        .attr('name', 'img_' + qstid + '_' + totalArq[qstid])
        .appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
    jQuery('#totImg_' + qstid).val(totalArq[qstid]);
}

</script>