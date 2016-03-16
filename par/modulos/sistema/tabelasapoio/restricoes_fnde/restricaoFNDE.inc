<?php
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

$dopid = $_REQUEST['dopid'];
$tprid = $_REQUEST['tprid'];

# Abas de restricoes
$listaMenus = buscarAbasRestricoes();
echo montarAbasArray($listaMenus, $_SERVER['REQUEST_URI']);

# Excluir restricao FNDE
if ($_REQUEST["submeter"] == 'excluir') {
    $sql = " UPDATE par.restricaofnde SET resstatus = 'I' WHERE resid = " . $_REQUEST['resid'];
    $db->executar($sql);
    $db->commit();
    echo "
        <script>
            alert('Restrição FNDE desativada com sucesso!');
            window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/restricaoFNDE&acao=A';
        </script>";
}

#Montar Titulo
monta_titulo('Lista de Restrições FNDE', '');
?>
<form id="formulario" name="formulario" method="post" action="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Código:</b></td>
            <td>
                <?php
                $resid = $_REQUEST['resid'];
                echo campo_texto('resid', 'N', 'S', '[#]', 20, 50, '', '', 'left', '', 0, '', '', $resid);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Número do Termo:</b></td>
            <td>
                <?php
                $dopid = $_REQUEST['dopid'];
                echo campo_texto('dopid', 'N', 'S', '[#]', 20, 50, '', '', 'left', '', 0, '', '', $dopid);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Tipo da restrição FNDE</b></td>
            <td>
                <?php
                $sql = "
                        SELECT
                            tpr.tprid AS codigo,
                            tpr.tprdescricao AS descricao
                        FROM 
                            par.tiporestricaofnde tpr
                        WHERE
                            tpr.tprstatus = 'A'
                        ORDER BY 
                            tpr.tprdescricao asc";
                $db->monta_combo("tprid", $sql, 'S', 'Selecione...', '', '', '', '', '', 'tprid', '', $tipoRestricaoID, null, null, ' link ');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Descrição:</b></td>
            <td>
                <?php
                $resdescricao = $_REQUEST['resdescricao'];
                echo campo_texto('resdescricao', 'N', 'S', '', 100, 100, '', '', 'left', '', 0, '', '', $resdescricao);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Esfera:</b></td>
            <td>
                <?php
                if (isset($_REQUEST['esfera'])) {
                    $esfera = $_REQUEST['esfera'];
                    $esfera1 = '';
                    $esfera2 = '';
                    $esfera3 = '';
                    if ($esfera == 'M') {
                        $esfera1 = " checked='checked'";
                    } else if($esfera == 'E') {
                        $esfera2 = " checked='checked'";
                    } else {
                        $esfera3 = " checked='checked'";
                    }
                } else {
                    $esfera3 = " checked='checked'";
                }
                ?>
                <input type="radio" id="esferaMunicipal" value='M' name="esfera" <?php echo $esfera1; ?> class="link" >
                <label for="esferaMunicipal" class="link">Municípal</label>
                <input type="radio" id="esferaEstadual" value='E' name="esfera" <?php echo $esfera2; ?> class="link" >
                <label for="esferaEstadual" class="link">Estadual</label>
                <input type="radio" id="todos" value='T' name="esfera" <?php echo $esfera3; ?> class="link" >
                <label for="todos" class="link">Todos</label>
            </td>
        </tr>
        <tr id="trMunicipio" style="display:none">
            <td class="SubTituloDireita" valign="top"><b>Municípios</b></td>
            <td>
               <div onmouseover="return escape('Municípios');">
                    <select
                        multiple="multiple"
                        size="5"
                        name="listaMunicipio[]"
                        id="listaMunicipio"
                        ondblclick="abrirPopupListaMunicipio();"
                        class="CampoEstilo link"
                        onkeydown="javascript:combo_popup_remove_selecionados(event, 'listaMunicipio');"
                        style="width:400px;" >
                            <?php if (!empty($restricao['listaMunicipio'])): ?>
                                <?php foreach ($restricao['listaMunicipio'] as $municipio): ?>
                                <option value="<?php echo $municipio['muncod']; ?>" selected="selected"><?php echo $municipio['descricao']; ?></option>
                            <?php endforeach; ?>
                        <?php else: ?>
                            <option value="">Duplo clique para selecionar da lista</option>
                        <?php endif; ?>
                    </select>
                </div>
            </td>
        </tr>
        <tr id="trUf" style="display:none">
            <td class="SubTituloDireita" valign="top">UF:</td>
            <td>
                <?php 
                    $sqlBuscaEstados = "
                        SELECT 
                            estuf AS codigo,
                            estdescricao AS descricao 
                        FROM 
                            territorios.estado
                        ORDER BY 
                            estuf;
                    ";

                    combo_popup("estuf", $sqlBuscaEstados, "Lista de Estados", "400x400", 0, array(), "", "S", false, false, 5, 400);
                    ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Prazo da Providência:</b></td>
            <td>
                <?php
                if (isset($_REQUEST['restemporestricao'])) {
                    $restemporestricao = $_REQUEST['restemporestricao'];
                    $restemporestricao1 = '';
                    $restemporestricao2 = '';
                    $restemporestricao3 = '';
                    if ($restemporestricao == '1') {
                        $restemporestricao1 = " checked='checked' ";
                    } elseif ($restemporestricao == '0') {
                        $restemporestricao2 = " checked='checked' ";
                    } else {
                        $restemporestricao3 = " checked='checked' ";
                    }
                } else {
                    $restemporestricao3 = " checked='checked' ";
                }
                ?>
                <input type="radio" id="restemporestricao_sim" value='1' name="restemporestricao" <?php echo $restemporestricao1; ?> class="link" >
                <label for="restemporestricao_sim" class="link">Sim</label>

                <input type="radio" id="restemporestricao_nao" value='0' name="restemporestricao" <?php echo $restemporestricao2; ?> class="link" >
                <label for="restemporestricao_nao" class="link">Não</label>

                <input type="radio" id="restemporestricao_todos" value='2' name="restemporestricao" <?php echo $restemporestricao3; ?> class="link" >
                <label for="restemporestricao_todos" class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top"><b>Status Restrição:</b></td>
            <td>
                <?php
                if (isset($_REQUEST['resstatus'])) {
                    $resstatus = $_REQUEST['resstatus'];
                    $resstatus1 = '';
                    $resstatus2 = '';
                    $resstatus3 = '';
                    if ($resstatus == 'A') {
                        $resstatus1 = " checked='checked' ";
                    } elseif ($resstatus == 'I') {
                        $resstatus2 = " checked='checked' ";
                    } else {
                        $resstatus3 = " checked='checked' ";
                    }
                } else {
                    $resstatus1 = " checked='checked' ";
                }
                ?>
                <input type="radio" id="resstatus_A" value='A' name="resstatus" <?php echo $resstatus1; ?> class="link" >
                <label for="resstatus_A" class="link">Ativo</label>

                <input type="radio" id="resstatus_I" value='I' name="resstatus" <?php echo $resstatus2; ?> class="link" >
                <label for="resstatus_I" class="link">Inativo</label>

                <input type="radio" id="resstatus_T" value='T' name="resstatus" <?php echo $resstatus3; ?> class="link" >
                <label for="resstatus_T" class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="submit" id="btnPesquisar" value="Pesquisar" />
                &nbsp;
                <input type="reset" id="btnCancel" value="Limpar Filtros" />
                &nbsp;
                <input type="button" id="btnNovo" value="Novo" />
            </td>
        </tr>
    </table>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
    <tr>
        <td align="center" colspan="2"><b>Lista de restrições do FNDE</b></td>
    </tr>
</table>

<?php
$acao = "
        '<img onclick=\"listaEntidade('|| res.resid ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'
        ||
        CASE WHEN resstatus = 'A' THEN
        '<a href=\"#\" onclick=\"alterarRestricaoFNDE(' || res.resid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
        ||
        '<a href=\"#\" onclick=\"excluirRestricaoFNDE(' || res.resid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
        ELSE
            ''
        END
    ";

# Filtros da pesquisa
$where = array();

# Filtro Codigo
if ($_REQUEST['resid'] && !empty($_REQUEST['resid'])) {
    $where[] = " res.resid = {$_REQUEST['resid']} ";
}

# Filtro Numero do Termo
if ($_REQUEST['dopid'] && !empty($_REQUEST['dopid'])) {
    $where[] = " re.prpid = (select prpid from par.vm_documentopar_ativos where dopnumerodocumento = {$_REQUEST['dopid']}) 
        OR re.proidpar = (select proid from par.vm_documentopar_ativos where dopnumerodocumento ={$_REQUEST['dopid']} )
        OR re.proidpac = (select proid from par.termocompromissopac where terstatus = 'A' and terid = {$_REQUEST['dopid']})";
}

# Filtro Tipo de Restricaok
if ($_REQUEST['tprid'] && !empty($_REQUEST['tprid'])) {
    $where[] = " res.tprid = {$_REQUEST['tprid']} ";
}

# Filtro Descricao
if ($_REQUEST['resdescricao'] && !empty($_REQUEST['resdescricao'])) {
    $where[] = " res.resdescricao ILIKE '%" . $_REQUEST['resdescricao'] . "%' ";
}

#Filtro Prazo Providencia
if (isset($_REQUEST['restemporestricao']) && $_REQUEST['restemporestricao'] != '2') {
    $where[] = " res.restemporestricao = '{$_REQUEST['restemporestricao']}' ";
}

#Filtro Esfera
if (isset($_REQUEST['esfera']) && $_REQUEST['esfera'] != 'T') {
    if ($_REQUEST['esfera'] == 'M') {
        $where[] = " i.muncod IN( '" . join("', '", $_REQUEST['muncod']) . "' ) ";
    }else {
        $where[] = " i.estuf IN( '" . join("', '", $_REQUEST['estuf']) . "' ) ";
    }
}

$sql = "
     SELECT DISTINCT
        '<center>'||" . $acao . "||'</center>' AS acao,
        res.resid, 
        res.resdescricao, 
        tpr.tprdescricao AS tipo, 
        CASE WHEN re.inuid IS NULL THEN 
            'FNDE' 
        WHEN i.estuf IS NOT NULL THEN 
            'Estadual' 
        ELSE 
            'Municipal' 
        END AS tratativa 
    FROM par.restricaofnde res 
    INNER JOIN par.tiporestricaofnde tpr ON res.tprid = tpr.tprid 
    LEFT JOIN par.restricaoentidade re ON(res.resid = re.resid) 
    LEFT JOIN par.instrumentounidade i ON(re.inuid = i.inuid) 
        " . (!empty($where) ? ' WHERE ' : '') . "
        " . join(' AND ', $where) . "
    ORDER BY 
    res.resid ASC ";
$cabecalho = array("ações", "Código", "Descrição", "Tipo", "Tratativa");
$db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', 'N');

if ($edit) {
    if ($boolTempoRestricao) {
        echo "<script>changeTempoRestricao();</script>";
    }
}
?>

<!--@TODO VErificar chamadas de Jquery-->
<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>


<script>
    $(document).ready(function() {
        // Selecionar Estados
        $( "#estuf" ).bind( "change blur focusout mouseover focusin", function() {
            $('#estuf option').attr('selected', 'selected');
        });        
        
        // Habilita somente quando restricao por = Termo
        $('input[name=esfera]').click(function() {
            if ($(this).val() === 'M') {
                ocultarCampos();
                $('#trMunicipio').show();
            } else if($(this).val() === 'E') {
                ocultarCampos();
                $('#trUf').show();
            } else {
                ocultarCampos();
            }
        });

        // Somente numeros
        $("input[name=resid]").bind("keyup blur focus", function(e) {
            e.preventDefault();
            var expre = /[^0-9]/g;
            if ($(this).val().match(expre))
                $(this).val($(this).val().replace(expre, ''));
        });

        $('#btnCancel').click(function() {
            window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/restricaoFNDE&acao=A';
        });

        $('#btnNovo').click(function() {
            window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A';
        });
    });

    function ocultarCampos() {
        $('#trMunicipio').hide();
        $('#trUf').hide();
    }

    function alterarRestricaoFNDE(resid)
    {
        window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A&resid=' + resid;
    }

    function excluirRestricaoFNDE(resid)
    {
        if (confirm('Deseja realmente desativar esta restrição?'))
        {
            window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/restricaoFNDE&acao=A&resid='
                    + resid + '&submeter=excluir';
        }
    }

    function listaEntidade(resid, docid, muncod)
    {
        var title = 'Restrição detalhada #'+resid;
        var data = {action: 'visualizar', resid: resid, docid: docid, muncod: muncod};
        var url = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/popupEntidade&acao=A';

        dialogAjax(title, data, url, null, false, true, true, true, false, 900, 400);
    }

    /**
     * Abre janela popup para selecionar municipios
     * 
     * @returns VOID
     */
    function abrirPopupListaMunicipio() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }

    function consultarTermoPorDopid(dopid) {
    window.open('par.php?modulo=principal/documentoPar&acao=A&req=formVizualizaDocumento&dopid='+dopid, 
                'modelo', 
                "height=600,width=400,scrollbars=yes,top=0,left=0" );
    }
</script>
