<?php
verificaSessao('obra');
$_SESSION['obras2']['sueid'] = '';

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];
$empreendimento = new Empreendimento( $empid );

$smiid = ($_REQUEST['smiid'] != '') ? $_REQUEST['smiid'] : $_SESSION['obras2']['smiid'];
$_SESSION['obras2']['smiid'] = ($_REQUEST['smiid'] != '') ? $_REQUEST['smiid'] : $_SESSION['obras2']['smiid'];

if (empty($smiid)) {
    die("<script>
        alert('Faltam parametros para acessar a tela!'); 
        window.location = '?modulo=principal/listaSupervisaoMI&acao=A';
     </script>");
}

$supervisaoMi = new SupervisaoMi($smiid);
extract($supervisaoMi->getDados());
$docid = pegaDocidSupervisaoMI($supervisaoMi->smiid);
$tecnologiaMi   = new TecnologiaMi();
$tecnologiaMi->carregaPorObrid($obrid);
$endereco       = new Endereco($endid ? $endid : $empreendimento->endid);

$somenteLeitura = 'S';
$habilitado     = true;

$obra = new Obras($obrid);
$empreendimento = new Empreendimento();
$arrEmp = $empreendimento->dadosResumo($obra->empid);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
<table class="tabela" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%" >
    <tr>
        <td colspan="2">
            <?php echo monta_cabecalho_relatorio(100); ?>
        </td>
    </tr>
    <tr>
        <td colspan="2">
            <!-- Dados da Medição -->
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%"> 
                <tr>
                    <td class="SubTituloCentro" coslpan="2">Dados da Medição</td>
                </tr>
            </table>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">  
                <tr>
                    <td class="SubTituloDireita" width="20%">Empresa</td>
                    <td>
                        <?php
                        $empresami = new EmpresaMi();
                        echo $empresami->pegaEmpresaMI($endereco->estuf);
                        ?>
                    </td>
                    <td rowspan="10" align="right" valign="top" width="1">&nbsp;</td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Data da Supervisão</td>
                    <td><?php echo $smidtsupervisao ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Nome do Responsável</td>
                    <td>
                        <?php
                        $vistoriador        = new Entidade( $entidvistoriador );
                        $entnomevistoriador = $vistoriador->entnome;
                        //$entidvistoriador   = $vistoriador->getPrimaryKey();
                        $entidvistoriador   = $vistoriador->entid;
                        ?>
                        <span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
                        <input type="hidden" name="entidvistoriador" id="entidvistoriador" value="<?php echo $entidvistoriador; ?>">
                        <?php if ($somenteLeitura == 'S' && $entnomevistoriador == '') { ?>
                        <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
                        <?php } ?>
                        <?php print obrigatorio(); ?>
                    </td>
                </tr>
                <tr id="tr_cargo_vistoriador" style="display: <?=($entidvistoriador ? '' : 'none' ) ?>">
                    <td class="SubTituloDireita">Cargo do Vistoriador</td>
                    <td><?= $smicargorepresentante ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Situação da Obra</td>
                    <td>
                        <?php
                        $situacaoObra = new SituacaoObra();
                        $sql = $situacaoObra->listaCombo();
                        $db->monta_combo("sobid", $sql, $somenteLeitura, "Selecione...", "ctrlSituacaoObra", '', '', '', 'S', 'sobid');
                        ?>
                    </td>
                </tr>
                <tr id="situacaoObraSubPergunta" style="display: <?php echo ( $sobid == SITUACAO_OBRA_EM_EXECUCAO ? '' : 'none' ) ?>;">
                    <td class="SubTituloDireita">Em conformidade com o Projeto Básico aprovado e contrato?</td>
                    <td>
                        <input type="radio" name="smiacordo" id="smiacordo_s" value="s" <?=($smiacordo == 's' ? 'checked="checked"' : '') ?>>
                        <label for="smiacordo_s">Sim</label>
                        &nbsp;&nbsp;
                        <input type="radio" name="smiacordo" id="smiacordo_n" value="n" <?=($smiacordo == 'n' ? 'checked="checked"' : '') ?>>
                        <label for="smiacordo_n">Não</label>
                    </td>
                </tr>
            </table>
            
            <!-- Local da Obra -->
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%"> 
                <tr>
                    <td class="SubTituloCentro" coslpan="2">Local da Obra</td>
                </tr>
            </table>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
                <?php 
                $campoEndHabil = 'N';
                $campoEndClass = 'disabled';
                ?>
                <tr>
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                        <label>No cadastro da obra o endereço está correto?</label>
                    </td>
                    <td colspan="1">
                        <input type="radio" onclick="verificaEnd('S')" name="smiendcorreto" id="smiendcorreto_s" value="s" <?=($smiendcorreto == 's' || empty($smiendcorreto) ? 'checked="checked"' : '') ?>>
                        <label for="smiendcorreto_s">Sim</label>
                        &nbsp;&nbsp;
                        <input type="radio" onclick="verificaEnd('N')" name="smiendcorreto" id="smiendcorreto_n" value="n" <?=($smiendcorreto == 'n' ? 'checked="checked"' : '') ?>>
                        <label for="smiendcorreto_n">Não</label>
                        <?=obrigatorio(); ?>
                    </td>
                </tr>
                <tr>
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                        <label>CEP</label>
                    </td>
                    <td colspan="1">
                        <input type="hidden" name="emp_endid" id="emp_endid" value="<?=$empreendimento->endid ?>">
                        <input type="hidden" name="endid" id="endid" value="<?=$endereco->endid ?>">
                        <input type="text" name="endereco[endcep]" 
                                   onkeyup="this.value=mascaraglobal('##.###-###', this.value);" 
                                   onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo " id="endcep" 
                                   value="<?=$endereco->endcep ?>" size="13" maxlength="10" readonly="readonly"
                        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                    </td>
                </tr>
                <tr id="escolha_logradouro_id" style="display:none">
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>
                    <td colspan="1">
                        <input type="text" name="endlog" class="CampoEstilo" id="endlog" value="<?=$endereco->endlog ?>" size="48" />
                    </td>
                </tr>
                <tr>
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                        <label id="lbLogadouro"> Logradouro </label>
                    </td>
                    <td colspan="1">
                        <input type="text" name="endereco[endlog]" class="CampoEstilo" id="endlogradouro" value="<?=$endereco->endlog ?>" size="65" readonly="readonly" />
                    </td>
                </tr>
                <tr>
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                        <label>Número</label>
                    </td>
                    <td colspan="2">
                        <input type="text" name="endereco[endnum]" class="CampoEstilo" id="endnum" value="<?=$endereco->endnum ?>" size="13" maxlength="8" readonly="readonly" />
                    </td>
                </tr>
                <tr>								
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                        <label>Complemento</label>
                    </td>
                    <td colspan="2">
                        <input type="text" name="endereco[endcom]" class="CampoEstilo " id="endcom" value="<?=$endereco->endcom ?>" size="65" maxlength="100" readonly="readonly" />
                    </td>
                </tr>
                <tr> 
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                        <label>Bairro</label>
                    </td>
                    <td colspan="2">
                        <input type="text" name="endereco[endbai]" class="CampoEstilo " id="endbai" value="<?=$endereco->endbai ?>" size="20" readonly="readonly" />
                    </td>
                </tr>
                <tr>
                    <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                        <label>Município/UF </label>
                    </td>
                    <td colspan="2">
                        <input type="text" name="endereco[mundescricao]"  class="CampoEstilo" id="mundescricao" value="<?=$arrEmp['mundescricao']?>" size="20" readonly="readonly" />
                        <input type="text" name="endereco[estuf]" class="CampoEstilo" id="estuf" value="<?=$arrEmp['estuf'] ?>" style="width: 5ex; padding-left: 2px" readonly="readonly" />
                    </td>
                </tr>
            </table>
            
            <!-- Cronograma -->
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%"> 
                <tr>
                    <td class="SubTituloCentro" coslpan="2">Cronograma</td>
                </tr>
            </table>
            <?php include(APPRAIZ . 'obras2/modulos/principal/cadSupervisaoMI_cronograma.inc'); ?>
            
            <!-- Fotos -->
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
                <tr>
                    <td class="SubTituloCentro" coslpan="2">Fotos</td>
                </tr>
            </table>
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
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="width:100%">
                <tr>
                    <td>
                        <table class="tabela" bgcolor="white" cellspacing="1" cellpadding="3" align="center" border="0" style="width:100%">
                            <tbody>
                                <tr>
                                    <td class="SubTituloCentro" valign="top">Relatório Técnico do Acompanhamento</td>
                                </tr>
                                <tr>
                                    <td>
                                    <?php
                                    $desc_relatorio = "No relatório deverão constar informações pertinentes a execução da obra, tais como:<br/><br/>&nbsp;&nbsp;&nbsp;<b>a.</b> O nome do Responsável Técnico da obra e o número da ART de execução.<br/>&nbsp;&nbsp;&nbsp;<b>b.</b> A existência ou não de todos os documentos necessários para a execução da obra, tais como: projetos, especificações, memoriais, ART´s, licenças, alvarás, caderno de encargos, diários de obras (preenchido e atualizado), dentre outros.<br/>&nbsp;&nbsp;&nbsp;<b>c.</b> Informações sobre o canteiro de obra (existência, organização e segurança do Trabalho).<br/>&nbsp;&nbsp;&nbsp;<b>d.</b> Atrasos na execução da obra e a necessidade ou não de aditivos.<br/>&nbsp;&nbsp;&nbsp;<b>e.</b> Comentário sobre a situação geral da obra e/ou de cada serviço da obra.<br/>&nbsp;&nbsp;&nbsp;<b>f.</b> Outros apontamentos.";
                                    echo $vistoriaEstrutura['supobs'];
                                    ?>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td>
                    <strong>Fotos do Acompanhamento</strong>
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
                    </td>
                </tr>
            </table>
        </td>
    </tr>
</table>

<style type="text/css">
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
</style>

<script type="text/javascript">
$(function(){
    $("input[type='text'], input[type='radio'], select, textarea").attr("disabled", true);
});
</script>