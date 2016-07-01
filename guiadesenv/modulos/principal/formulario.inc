<link href="css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="js/jquery-1.9.1.js"></script>
<script src="js/jquery-ui-1.10.3.custom.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>

<?php
include_once( APPRAIZ . "pes/classes/PesContrato.class.inc" );
include_once( APPRAIZ . "pes/classes/PesCelulaContrato.class.inc" );
include_once( APPRAIZ . "pes/classes/PesContratoNaturezaDespesa.class.inc" );

$entcodigo = $_SESSION['pes']['codigo_entidade'];

$modelContrato = new PesContrato($_REQUEST['concodigo']);
$contratoNaturezaDespesa = new PesContratoNaturezaDespesa($_REQUEST['cndcodigo']);
if ($contratoNaturezaDespesa->cndcodigo) {
    $modelContrato->tidcodigo = $contratoNaturezaDespesa->tidcodigo;
    $modelContrato->contitulo = $contratoNaturezaDespesa->cndtitulo;
    $modelContrato->conobservacao = $contratoNaturezaDespesa->cndobservacao;
}

if ($_POST['gravar']) {

    // Verifica contratos repetidos
    $sql = "select count(*) as qtd from pes.pescontrato
            where tidcodigo = '{$_POST['tidcodigo']}'
            and entcodigo = '$entcodigo'
            and contitulo = '{$_POST['contitulo']}'";

    $sql .= $_POST['concodigo'] ? " and concodigo != '{$_POST['concodigo']}' " : '';

    if ($db->pegaUm($sql)) {
        alertlocation(array(
            'alert' => MSG010,
            'location' => $url . '&tidcodigo=' . $_POST['tidcodigo']
        ));
    }

    // Salva os contratos
    if ($_POST['tidcodigo'] == K_DESPESA_MATERIAL_CONSUMO) {

        $contratoNaturezaDespesa->entcodigo = $entcodigo;
        $contratoNaturezaDespesa->tidcodigo = $_POST['tidcodigo'];
        $contratoNaturezaDespesa->cndtitulo = $_POST['contitulo'];
        $contratoNaturezaDespesa->unicodigo = $_POST['unicodigo'];
        $contratoNaturezaDespesa->natcodigo = $_POST['natcodigo'];
        $contratoNaturezaDespesa->cndobservacao = $_POST['conobservacao'];
        $contratoNaturezaDespesa->cndunidadeoutros = $_POST['cndunidadeoutros'];
        $contratoNaturezaDespesa->natano = date('Y');
        $contratoNaturezaDespesa->cndano = date('Y');

        if ($contratoNaturezaDespesa->cndcodigo) {
            $contratoNaturezaDespesa->cndusucpfalteracao = $_SESSION['usucpforigem'];
            $contratoNaturezaDespesa->cnddataalteracao = date('Y-m-d H:i:s');
        } else {
            $contratoNaturezaDespesa->cndusucpfcriacao = $_SESSION['usucpforigem'];
            $contratoNaturezaDespesa->cnddatacriacao = date('Y-m-d H:i:s');
        }

        $contratoNaturezaDespesa->salvar(null, null, array('cndunidadeoutros', 'cndobservacao'));
        $contratoNaturezaDespesa->Commit();
    } else {
        $modelContrato->popularDadosObjeto();

        // formata_data_sql

        $modelContrato->convalor = $_POST['convalor'] ? real2Db($_POST['convalor']) : null;
        $modelContrato->confimvigencia = $_POST['confimvigencia'] ? formata_data_sql($_POST['confimvigencia']) : null;
        $modelContrato->condatarepactuacao = $_POST['condatarepactuacao'] ? formata_data_sql($_POST['condatarepactuacao']) : null;
        $modelContrato->coniniciovigencia = $_POST['coniniciovigencia'] ? formata_data_sql($_POST['coniniciovigencia']) : null;
        $modelContrato->coniniciovigencia = (!$modelContrato->coniniciovigencia && $_POST['conassinatura']) ? formata_data_sql($_POST['conassinatura']) : $modelContrato->coniniciovigencia;

        $controllerGeral = new Controller_Geral();

        $checkDates = true;

        if ($controllerGeral->getPost('conassinatura') && $controllerGeral->getPost('condatarepactuacao')) {
            $checkDates = $controllerGeral->datesIsValid($controllerGeral->getPost('conassinatura'), $controllerGeral->getPost('condatarepactuacao'));

            if (!$checkDates) {
                echo "<script lang='javascript'>
                    msg($('#condatarepactuacao'), 'O campo [Data de Assinatura] não pode ser superior ao [Data Prevista]!');
                </script>";
            }
        } elseif ($controllerGeral->getPost('coniniciovigencia') && $controllerGeral->getPost('confimvigencia')) {
            $checkDates = $controllerGeral->datesIsValid($controllerGeral->getPost('coniniciovigencia'), $controllerGeral->getPost('confimvigencia'));

            if (!$checkDates) {
                echo "
                <script lang='javascript'>
                    msg($('#condatarepactuacao'), 'O campo [Data de Incío] não pode ser superior ao [Data de Término]!');
                </script>";
            }
        }

        if ($checkDates) {

            if ($modelContrato->concodigo) {
                $modelContrato->conusucpfalteracao = $_SESSION['usucpforigem'];
                $modelContrato->condataalteracao = date('Y-m-d H:i:s');
            } else {
                $modelContrato->entcodigo = $entcodigo;
                $modelContrato->conano = date('Y');
                $modelContrato->conusucpfcriacao = $_SESSION['usucpforigem'];
                $modelContrato->condatacriacao = date('Y-m-d H:i:s');
            }

            $modelContrato->salvar(null, null, array('coniniciovigencia', 'confimvigencia', 'convalor', 'condatarepactuacao', 'conassinatura'));
            $modelContrato->Commit();
        }
    }

    alertlocation(array(
        'alert' => MSG003,
        'location' => $url . '&concodigo=' . $modelContrato->concodigo . '&cndcodigo=' . $contratoNaturezaDespesa->cndcodigo
    ));
}
if ($_POST['gravar_detalhe']) {
    try {

        $sql = "delete from pes.pescelulacontrato
				where concodigo = '{$_POST['concodigo']}'";
        $db->executar($sql);

        if (!empty($_POST['conendereco'])) {
            $sql = "update pes.pescontrato set
		              conendereco = '{$_POST['conendereco']}'
    				where concodigo = '{$_POST['concodigo']}'";
            $db->executar($sql);
        }

        foreach ($_POST['cecvalor'] as $ccdcodigo => $cecvalor) {
            $modelCelulaContrato = new PesCelulaContrato();

            $modelCelulaContrato->cecvalor = $cecvalor ? real2Db($cecvalor) : null;
            $modelCelulaContrato->ccdcodigo = $ccdcodigo;
            $modelCelulaContrato->concodigo = $_POST['concodigo'];
            $modelCelulaContrato->salvar();
        }

        $db->Commit();
        alertlocation(array(
            'alert' => MSG003,
            'location' => "$url&concodigo={$_POST['concodigo']}"
        ));
    } catch (Exception $e) {
        $db->Rollback();
        alertlocation(array(
            'alert' => MSG004,
            'location' => "$url&concodigo={$_POST['concodigo']}"
        ));
    }
}
//    ver($modelContrato->coniniciovigencia, $modelContrato->condatarepactuacao, d);

extract($_REQUEST);

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();
echo '<br />';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');

if ($controllerEntidade->permission() == 3)
    $save = 'N';
else
    $save = 'S';
?>
<style type="text/css">
    .campo_especifico{
        display: none;
    }
</style>

<?php if ($save == 'S'): ?>
    <form action="" method="post" name="formulario_gravar" id="formulario_gravar">
<?php endif ?>
    <input type="hidden" name="concodigo" value="<?= $modelContrato->concodigo ?>"/>
    <input type="hidden" name="cndcodigo" value="<?= $contratoNaturezaDespesa->cndcodigo ?>"/>
    <input type="hidden" name="gravar" value="1"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Despesa:</td>
            <td>
<?php
if (($modelContrato->concodigo || $contratoNaturezaDespesa->cndcodigo) && $modelContrato->tidcodigo) {
    $sql = "select tidnome
						   from  pes.pestipodespesa
						   where tidcodigo = {$modelContrato->tidcodigo}";
    echo $db->pegaUm($sql);
    echo '<input type="hidden" name="tidcodigo" id="tidcodigo" value="' . $modelContrato->tidcodigo . '" />';
} else {
    $sql = "select tidnome as descricao, tidcodigo as codigo
						   from  pes.pestipodespesa
                           where tidcodigo not in (" . K_DESPESA_DIARIAS . ", " . K_DESPESA_PASSAGENS . ", " . K_DESPESA_COLETA_SELETIVA . ", " . K_DESPESA_GENERICA . ")
                           order by tidordem desc, descricao";
    echo $db->monta_combo("tidcodigo", $sql, $save, "Selecione...", "", "", "", "200", "S", "tidcodigo", "", $modelContrato->tidcodigo);
}
?>
            </td>
        </tr>
        <tr class="campo_material_consumo campo_especifico">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Subelemento:</td>
            <td>
<?php
if ($contratoNaturezaDespesa->cndcodigo && $contratoNaturezaDespesa->natcodigo) {
    $sql = "select natcodigo || ' - ' || natdescricao as descricao
						   from  pes.pesnaturezadespesa
						   where natcodigo = '{$contratoNaturezaDespesa->natcodigo}'";
    echo $db->pegaUm($sql);
    echo '<input type="hidden" name="natcodigo" id="natcodigo" value="' . $contratoNaturezaDespesa->natcodigo . '" />';
} else {
    $sql = "select natcodigo || ' - ' || natdescricao as descricao, natcodigo as codigo
					       from pes.pesnaturezadespesa
                           where natcodigopai = '33903000'
                           order by natdescricao";
    echo $db->monta_combo("natcodigo", $sql, $save, "Selecione...", "", "", "", "200", "S", "natcodigo", "", $contratoNaturezaDespesa->natcodigo);
}
?>
            </td>
        </tr>
        <tr class="campo_material_consumo campo_especifico">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade de Medida:</td>
            <td>
<?php
if ($contratoNaturezaDespesa->cndcodigo && $contratoNaturezaDespesa->unicodigo) {
    $sql = "select unititulo
						   from  pes.pesunidademedida
						   where unicodigo = '{$contratoNaturezaDespesa->unicodigo}'";
    echo $db->pegaUm($sql);
    echo '<input type="hidden" name="unicodigo" id="unicodigo" value="' . $contratoNaturezaDespesa->unicodigo . '" />';
} else {
    $sql = "select unititulo as descricao, unicodigo as codigo
					       from pes.pesunidademedida
                           order by descricao";
    echo $db->monta_combo("unicodigo", $sql, $save, "Selecione...", "", "", "", "200", "S", "unicodigo", "", $contratoNaturezaDespesa->unicodigo);
}
?>
            </td>
        </tr>
        <tr id="unidade-outros">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição da Unidade de Medida:</td>
            <td>
<?php
if ($contratoNaturezaDespesa->cndcodigo && $contratoNaturezaDespesa->cndunidadeoutros) {
    echo $contratoNaturezaDespesa->cndunidadeoutros;
    echo '<input type="hidden" name="cndunidadeoutros" id="cndunidadeoutros" value="' . $contratoNaturezaDespesa->cndunidadeoutros . '" />';
} else {
    echo campo_texto('cndunidadeoutros', 'S', $save, 'Descrição da Unidade de Medida', 50, 30, '', '', '', '', '', 'id="cndunidadeoutros"', '', $contratoNaturezaDespesa->cndunidadeoutros);
}
?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Contrato:</td>
            <td><?= campo_texto('contitulo', 'S', $save, 'Contrato', 50, 100, '', '', '', '', '', 'id="contitulo"', '', $modelContrato->contitulo) ?></td>
        </tr>
        <tr class="campo_vigencia campo_especifico">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Início:</td>
            <td>
<?php
$coniniciovigencia = $modelContrato->coniniciovigencia ? formata_data($modelContrato->coniniciovigencia) : "";
//                    echo campo_data2("coniniciovigencia","N","S","","","","",$coniniciovigencia);
echo campo_texto('coniniciovigencia', 'N', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="coniniciovigencia"', '', $coniniciovigencia);
?>
            </td>
        </tr>
        <tr class="campo_vigencia campo_especifico">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Término:</td>
            <td>
<?php
$confimvigencia = $modelContrato->confimvigencia ? formata_data($modelContrato->confimvigencia) : "";
//                    echo campo_data2("confimvigencia","N","S","","","","",$confimvigencia);
echo campo_texto('confimvigencia', 'N', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="confimvigencia"', '', $confimvigencia);
?>
            </td>
        </tr>
        <tr class="campo_vigencia campo_especifico">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Valor Total do Contrato:</td>
            <td>
<?php $convalor = $modelContrato->convalor ? number_format($modelContrato->convalor, 2, ',', '.') : ''; ?>
                <input type="text" class="CampoEstilo" name="convalor" value="<?php echo $convalor; ?>" onKeyUp="this.value = mascaraglobal('[.###],##', this.value)" onFocus="MouseClick(this);
                                        this.select();" />
            </td>
        </tr>
        <tr class="campo_assinatura campo_especifico">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Assinatura do Contrato:</td>
            <td>
<?php
$coniniciovigencia = $modelContrato->coniniciovigencia ? formata_data($modelContrato->coniniciovigencia) : "";
//                    echo campo_data2("conassinatura","N","S","","","","",$coniniciovigencia);
echo campo_texto('conassinatura', 'N', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="conassinatura"', '', $coniniciovigencia);
?>
            </td>
        </tr>
        <tr class="campo_assinatura campo_especifico">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Prevista para a próxima repactuação ou contrato:</td>
            <td>
<?php
$condatarepactuacao = $modelContrato->condatarepactuacao ? formata_data($modelContrato->condatarepactuacao) : "";
//                    echo campo_data2("condatarepactuacao","N","S","","","","",$condatarepactuacao);
echo campo_texto('condatarepactuacao', 'N', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="condatarepactuacao"', '', $condatarepactuacao);
?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Observação:</td>
            <td><?= campo_textarea('conobservacao', 'N', $save, '', 70, 3, 5000, null, null, null, null, null, $modelContrato->conobservacao); ?></td>
        </tr>
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
<?php if ($save == 'S'): ?>
                    <input type="button" name="botao_gravar" id="botao_gravar" value="Gravar" />
                <?php endif ?>
                <?php
                if ($modelContrato->concodigo || $contratoNaturezaDespesa->cndcodigo) {
                    $modelContrato->concodigo = $modelContrato->concodigo ? $modelContrato->concodigo : $contratoNaturezaDespesa->cndcodigo;
                    ?>
                    <input type="button" name="botao_acompanhar" id="botao_acompanhar" value="Acompanhar" href="pes.php?modulo=principal/acompanhamento/despesas&acao=A&concodigo=<?php echo $modelContrato->concodigo; ?>&tidcodigo=<?php echo $modelContrato->tidcodigo; ?>&natcodigo=<?php echo $contratoNaturezaDespesa->natcodigo; ?>" />
                <?php } ?>
            </td>
        </tr>
    </table>
<?php if ($save == 'S'): ?>
    </form>
    <?php endif ?>

<?php if ($modelContrato->tidcodigo && $modelContrato->concodigo) { ?>

    <?php
    // Anos a exibir
    // Configurações por tipo de despesa
    $sql = "select * from pes.pesconfigcontratodespesa ccd
					inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
					inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
					inner join pes.pesgrupolinhacontrato glc on glc.glccodigo = lco.glccodigo
				where ccdtipoconfig = 'CC'
				and tidcodigo = {$modelContrato->tidcodigo}
				order by glcordem, lcoordem, ccototaliza, ccoordem";

    $configuracao = $db->carregar($sql);
    $grupos = array();
    $aConfigs = array();
    if ($configuracao) {
        foreach ($configuracao as $config) {
            $grupos[$config['glcnome']][$config['ccocodigo']] = $config['cconome'];
            $aConfigs[$config['glcnome']][$config['lconome']][$config['ccocodigo']] = $config;
        }
    }

    // Dados de valores do contrato gravados no banco
    $sql = "select * from pes.pesCelulaContrato cea
					inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
				where concodigo = {$modelContrato->concodigo}
				and ccdtipoconfig = 'CC'";

    $celulaContrato = $db->carregar($sql);

    $aCelulaContrato = array();
    foreach ((array) $celulaContrato as $dado) {
        $aCelulaContrato[$dado['ccdcodigo']] = $dado['cecvalor'];
    }
    ?>

    <?php if ($aConfigs) { ?>
        <form action="" name="formulario_detalhe"  id="formulario_detalhe" method="post">

            <div class="botao_gravar">
                <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar Detalhes" />
            </div>

            <input type="hidden" name="gravar_detalhe" value="1"/>
            <input type="hidden" name="concodigo" id="concodigo" value="<?php echo $modelContrato->concodigo; ?>"/>

        <?php foreach ($grupos as $grupo => $aColuna) { ?>
                <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 20px;">
                    <thead>
                        <tr align="center">
                            <td width="300px" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                <strong><?php echo $grupo; ?></strong>
                            </td>
            <?php foreach ($aColuna as $cconome) { ?>
                                <td valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                    <strong><?php echo $cconome; ?></strong>
                                </td>
                            <?php } ?>
                        </tr>
                    </thead>

                    <tbody>
            <?php
//						ver($aConfigs, d);
            $count = 0;
            foreach ((array) $aConfigs[$grupo] as $linha => $aLinhas) {
                $count++;
                $complemento = ($count % 2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
                ?>
                            <tr align="center" <?php echo $complemento; ?>>
                                <td align="left" title="Ação" class="colunaDestaque">
                                    <strong><?php echo $linha; ?></strong>
                                </td>
                <?php foreach ($grupos[$grupo] as $coluna => $aColuna) { ?>
                                    <td title="">
                    <?php
                    if ($aLinhas[$coluna]['ccdcodigo'] == 1084659 || $aLinhas[$coluna]['ccdcodigo'] == 1094959) {
                        $conendereco = $modelContrato->conendereco;
                        ?>
                                            <input size="50" type="text" <?php echo $aLinhas[$coluna]['ccdcodigo'] ? '' : 'disabled="disabled"'; ?> class="CampoEstilo <?php echo $aLinhas[$coluna]['ccdcodigo'] ? '' : 'disabled'; ?>" name="conendereco" value="<?php echo $conendereco; ?>" />
                                        <?php
                                        } else {
                                            $cecvalor = $aCelulaContrato[$aLinhas[$coluna]['ccdcodigo']];

                                            $casasDecimais = 'IN' == $aLinhas[$coluna]['ccdtipovalor'] ? 0 : 2;
                                            $cecvalor = $cecvalor ? number_format($cecvalor, $casasDecimais, ',', '.') : '';

                                            $sLinha = $aLinhas[$coluna]['glccodigo'] . '_' . $aLinhas[$coluna]['lcocodigo'];
                                            $sColuna = $aLinhas[$coluna]['glccodigo'] . '_' . $aLinhas[$coluna]['ccocodigo'];
                                            ?>
                                            <input style="text-align: right;" type="text" <?php echo $aLinhas[$coluna]['ccdcodigo'] ? '' : 'disabled="disabled"'; ?> class="CampoEstilo soma soma_coluna_<?php echo $sColuna; ?> soma_linha_<?php echo $sLinha; ?> <?php echo $aLinhas[$coluna]['ccdcodigo'] ? '' : 'disabled'; ?>" coluna="<?php echo $sColuna; ?>" linha="<?php echo $sLinha; ?>" name="cecvalor[<?php echo $aLinhas[$coluna]['ccdcodigo']; ?>]" value="<?php echo $cecvalor; ?>" onKeyUp="<?php echo 'IN' == $aLinhas[$coluna]['ccdtipovalor'] ? "this.value=mascaraglobal('[.###]', this.value)" : "this.value=mascaraglobal('[.###],##', this.value)" ?>" onFocus="MouseClick(this);
                                                                this.select();" />
                                        <?php } ?>
                                    </td>
                                <?php } ?>
                            </tr>
                        <?php } ?>
                        <?php /* ?>
                          <tr class="colunaDestaque">
                          <td><strong>TOTAL</strong></td>
                          <?php foreach ($anos as $ano) { ?>
                          <?php foreach ($configs as $ccocodigo => $config) {
                          $coluna = $ano . '_' . $ccocodigo; ?>
                          <td style="text-align: center"><strong><span id="span_<?php echo $coluna; ?>"></span></strong></td>
                          <?php } ?>
                          <?php }
                          </tr>
                         */ ?>
                    </tbody>
                </table>
            <?php } ?>

            <div class="botao_gravar">
                <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar Detalhes" />
            </div>
        </form>
    <?php }
}
?>

<script type="text/javascript">
    jQuery(function() {

        $('#coniniciovigencia').change(
                function() {
                    console.info($(this).val());
                    $('#conassinatura').attr('value', $(this).val());
                }
        );

        $('#conassinatura').change(
                function() {
                    console.info($(this).val());
                    $('#coniniciovigencia').attr('value', $(this).val());
                }
        );

        $.datepicker.regional[ 'pt-BR' ];
        $("#conassinatura").datepicker();
        $("#condatarepactuacao").datepicker();
        $("#coniniciovigencia").datepicker();
        $("#confimvigencia").datepicker();

        jQuery('#botao_gravar').click(function() {

            if (!jQuery('#tidcodigo').val()) {
                alert('O campo [Despesa] é obrigatório.');
                jQuery('#tidcodigo').focus();
                return false;
            }

            if (jQuery('#tidcodigo').val() == <?php echo K_DESPESA_MATERIAL_CONSUMO ?> && !jQuery('#natcodigo').val()) {
                alert('O campo [Subelemento] é obrigatório.');
                jQuery('#natcodigo').focus();
                return false;
            }

            if (jQuery('#tidcodigo').val() == <?php echo K_DESPESA_MATERIAL_CONSUMO ?> && !jQuery('#unicodigo').val()) {
                alert('O campo [Unidade de Medida] é obrigatório.');
                jQuery('#unicodigo').focus();
                return false;
            }

            if (!jQuery('#contitulo').val()) {
                alert('O campo [Contrato] é obrigatório.');
                jQuery('#contitulo').focus();
                return false;
            }

            if ($("#conassinatura").val() != '') {
                if (!isValidDate($("#conassinatura"))) {
                    return false;
                }
            }
            if ($("#condatarepactuacao").val() != '') {
                if (!isValidDate($("#condatarepactuacao"))) {
                    return false;
                }
            }
            if ($("#coniniciovigencia").val() != '') {
                if (!isValidDate($("#coniniciovigencia"))) {
                    return false;
                }
            }
            if ($("#confimvigencia").val() != '') {
                if (!isValidDate($("#confimvigencia"))) {
                    return false;
                }
            }





            jQuery('#formulario_gravar').submit();
        });

        jQuery('.botao_gravar_detalhes').click(function() {
            jQuery('#formulario_detalhe').submit();
        });

        jQuery('#botao_acompanhar').click(function() {
            window.location.href = jQuery(this).attr('href');
        });

        jQuery('#unicodigo, #tidcodigo').change(function() {
            // Se a unidade de medida for "Outro", deve-se informar a descrição
            if ('<?php echo K_UNIDADE_MEDIDA_OUTROS; ?>' == jQuery('#unicodigo').val() && '<?php echo K_DESPESA_MATERIAL_CONSUMO ?>' == jQuery('#tidcodigo').val()) {
                jQuery('#unidade-outros').show();
            } else {
                jQuery('#unidade-outros').hide();
            }
        }).change();

        jQuery('#tidcodigo').change(function() {
            if ($(this).val() == '<?php echo K_DESPESA_LIMPEZA ?>' || $(this).val() == '<?php echo K_DESPESA_VIGILANCIA ?>') {
                $('.campo_vigencia').show();
                
                $('.campo_assinatura').hide();
                $("#conassinatura").val('');
                $("#condatarepactuacao").val('');
                
                $('.campo_material_consumo').hide();
            } else {
                if ($(this).val() == '<?php echo K_DESPESA_APOIO_ADM ?>' || $(this).val() == '<?php echo K_DESPESA_LOCACAO_IMOVEIS ?>' || $(this).val() == '<?php echo K_DESPESA_MANUTENCAO_BENS ?>') {
                    $('.campo_vigencia').hide();
                    $("#confimvigencia").val('');
                    $("#coniniciovigencia").val('');
                    
                    $('.campo_assinatura').show();
                    $('.campo_material_consumo').hide();
                    
                } else {
                    if ($(this).val() == '<?php echo K_DESPESA_MATERIAL_CONSUMO ?>') {
                        $('.campo_vigencia').hide();
                        $("#confimvigencia").val('');
                        $("#coniniciovigencia").val('');
                        $('.campo_assinatura').hide();
                        $("#conassinatura").val('');
                        $("#condatarepactuacao").val('');
                        
                        $('.campo_material_consumo').show();
                        
                    } else {
                        $('.campo_especifico').hide();
                        $("#conassinatura").val('');
                        $("#condatarepactuacao").val('');
                    }
                }
            }
            
        }).change();
    });
</script>
