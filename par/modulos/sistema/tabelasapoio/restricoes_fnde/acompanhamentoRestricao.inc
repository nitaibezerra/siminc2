<?php
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

#Abas
$listaMenus = buscarAbasRestricoes();
echo montarAbasArray($listaMenus, $_SERVER['REQUEST_URI']);

# Excluir restricao FNDE
if ($_REQUEST["submeter"] == 'excluir') {
    $restricaoEntidade = new RestricaoEntidade();
    $restricaoEntidade->delete($_REQUEST["renid"]);
    echo "
        <script>
            alert('Município Excluído da Restrição FNDE com sucesso!');
            window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/acompanhamentoRestricao&acao=A';
        </script>";
}

#Montar Titulo
monta_titulo('Acompanhamento de Restrições', '');
?>
<form id="formulario" name="formulario" method="post" action="par.php?modulo=sistema/tabelasapoio/restricoes_fnde/acompanhamentoRestricao&acao=A">
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
            <td class="SubTituloDireita" valign="top"><b>Restrições por:</b></td>
            <td>
                <input type="radio" id="esferaTermo" value='T' name="esfera" class="link" >
                <label for="esferaTermo" class="link">Termo</label>
                <input type="radio" id="esferaMunicipio" value='M' name="esfera" class="link" >
                <label for="esferaMunicipio" class="link">Município</label>
                <input type="radio" id="esferaEstado" value='E' name="esfera" class="link" >
                <label for="esferaEstado" class="link">Estado</label>
            </td>
        </tr>
        <tr id="trUf" style="display:none">
            <td class="SubTituloDireita">UF:</td>
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

                combo_popup("estuf", $sqlBuscaEstados, "Lista de Estados", "400x400", 0, array(), "", "S", false, false, 5, 400, null, null, false, null, $arrEstados);
                ?>
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
            <td class="SubTituloDireita" valign="top"><b>Estado</b></td>
            <td>
                <?php
                $sql = "
                    SELECT  
                        esdid AS codigo,
                        esddsc AS descricao 
                    FROM 
                        workflow.estadodocumento 
                    WHERE esdstatus = 'A'
                    AND tpdid = ".TPDID_RESTRICAO_PAR;
                        
                $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '', '', '', '', 'esdid', '', $tipoRestricaoID, null, null, ' link ');
                ?>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="submit" id="btnPesquisar" name="btnPesquisar" value="Pesquisar" />
                &nbsp;
                <input type="button" id="btnCancel" value="Limpar Filtros" />
                &nbsp;
                <input type="submit" id="btnGerarExcel" name="btnGerarExcel" value="Gerar Excel" />
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
    CASE WHEN re.inuid IS NULL THEN
    '<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'
    WHEN uf.estuf IS NOT NULL THEN
    '<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid ||',\'' || uf.estuf || '\')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'
    ELSE
    '<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid || ',' || mun.muncod ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'
    END
    ||
    CASE WHEN resstatus = 'A' THEN
    '<a href=\"#\" onclick=\"alterarRestricaoFNDE(' || re.resid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
    ||
    '<a href=\"#\" onclick=\"excluirRestricaoEntidade(' || re.renid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
    ELSE
        ''
    END
";

# Filtros da pesquisa
$where = array();

# Filtro Codigo
if ($_REQUEST['resid'] && !empty($_REQUEST['resid'])) {
    $where[] = " re.resid = {$_REQUEST['resid']} ";
}

# Filtro Numero do Termo
if ($_REQUEST['dopid'] && !empty($_REQUEST['dopid'])) {
    $where[] = " re.prpid = (select prpid from par.vm_documentopar_ativos where dopnumerodocumento = {$_REQUEST['dopid']}) 
        OR re.proidpar = (select proid from par.vm_documentopar_ativos where dopnumerodocumento ={$_REQUEST['dopid']} )
        OR re.proidpac = (select proid from par.termocompromissopac where terstatus = 'A' and terid = {$_REQUEST['dopid']})";
}

# Tipo de restricao
if ($_REQUEST['tprid'] && !empty($_REQUEST['tprid'])) {
    $where[] = " res.tprid = {$_REQUEST['tprid']} ";
}

# Estado
if ($_REQUEST['esdid'] && !empty($_REQUEST['esdid'])) {
    $where[] = " esd.esdid = {$_REQUEST['esdid']} ";
}

# Superado
if ($_REQUEST['ressuperado'] && !empty($_REQUEST['ressuperado'])) {
    $where[] = " res.ressuperado = '{$_REQUEST['ressuperado']}' ";
}

#PArametro enviado pela Url da lista de municipio ou estado
if (!empty($_REQUEST['inuid']) && $_REQUEST['inuid']) {
    $where[] = " i.inuid = '{$_REQUEST['inuid']}' ";
}

#Filtro Esfera
if (isset($_REQUEST['esfera'])) {
    if ($_REQUEST['esfera'] == 'M') {
        $where[] = " i.muncod IN( '" . join("', '", $_REQUEST['listaMunicipio']) . "' ) ";
    }else if($_REQUEST['esfera'] == 'E') {
        $where[] = " i.estuf IN( '" . join("', '", $_REQUEST['estuf']) . "' ) ";
    }else{
        $where[] = " re.prpid IS NOT NULL or re.proidpar IS NOT NULL or re.proidpac IS NOT NULL ";
    }
}

# Trata botao pesquisar ou gerar
$acaoPesquisar = "'<center>'||" . $acao . "||'</center>' AS acao,";
if (isset($_REQUEST['btnGerarExcel'])) {
    $acaoPesquisar = "";
}

$sql = "
    SELECT
        $acaoPesquisar
        res.resid, 
        res.resdescricao, 
        CASE WHEN re.inuid IS NULL THEN 
            CASE WHEN re.prpid IS NOT NULL THEN 
		( SELECT
		CASE WHEN iu.estuf = 'DF' THEN uf.estuf WHEN iu.itrid = 1 THEN uf.estuf ELSE mun.estuf END
			FROM par.processopar pp
		     INNER JOIN par.instrumentounidade iu ON pp.inuid = iu.inuid
		     LEFT JOIN territorios.estado uf ON iu.estuf = uf.estuf
		     LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
		     LEFT JOIN par.vm_documentopar_ativos dp ON dp.prpid = pp.prpid
		WHERE pp.prpid = re.prpid
		)

	    WHEN re.proidpar IS NOT NULL THEN
		( SELECT
                CASE WHEN iu.estuf = 'DF' THEN uf.estuf WHEN iu.itrid = 1 THEN uf.estuf ELSE mun.estuf END
			FROM par.processoobraspar pp
		     INNER JOIN par.instrumentounidade iu ON pp.inuid = iu.inuid
		     LEFT JOIN territorios.estado uf ON iu.estuf = uf.estuf
		     LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
		     LEFT JOIN par.vm_documentopar_ativos dp ON dp.proid = pp.proid 
		WHERE pp.proid = re.proidpar
		)
	    ELSE 
		(SELECT
		CASE WHEN iu.estuf = 'DF' THEN uf.estuf WHEN iu.itrid = 1 THEN uf.estuf ELSE mun.estuf END
			FROM par.processoobra po
			INNER JOIN par.instrumentounidade iu ON iu.muncod = po.muncod OR iu.estuf = po.estuf
			LEFT JOIN par.termocompromissopac tcp ON tcp.proid = po.proid AND tcp.terstatus = 'A'
			LEFT JOIN territorios.estado uf ON iu.estuf = uf.estuf
			LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
			WHERE po.proid = re.proidpac
		) 
		END
        WHEN i.estuf IS	NOT NULL THEN 
            uf.estuf 
        ELSE 
            mun.estuf 
        END AS uf,
        CASE WHEN re.inuid IS NULL THEN 
		CASE WHEN re.prpid IS NOT NULL THEN 
		( SELECT
		CASE WHEN iu.estuf = 'DF' THEN mun.mundescricao WHEN iu.itrid = 1 THEN '-' ELSE mun.mundescricao END
			FROM par.processopar pp
		     INNER JOIN par.instrumentounidade iu ON pp.inuid = iu.inuid
		     LEFT JOIN territorios.estado uf ON iu.estuf = uf.estuf
		     LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
		     LEFT JOIN par.vm_documentopar_ativos dp ON dp.prpid = pp.prpid 
		WHERE pp.prpid = re.prpid
		)

		WHEN re.proidpar IS NOT NULL THEN
		( SELECT
                CASE WHEN iu.estuf = 'DF' THEN mun.mundescricao WHEN iu.itrid = 1 THEN '-' ELSE mun.mundescricao END
			FROM par.processoobraspar pp
		     INNER JOIN par.instrumentounidade iu ON pp.inuid = iu.inuid
		     LEFT JOIN territorios.estado uf ON iu.estuf = uf.estuf
		     LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
		     LEFT JOIN par.vm_documentopar_ativos dp ON dp.proid = pp.proid
		WHERE pp.proid = re.proidpar
		)
		ELSE 
		(SELECT
		CASE WHEN iu.estuf = 'DF' THEN mun.mundescricao WHEN iu.itrid = 1 THEN '-' ELSE mun.mundescricao END
			FROM par.processoobra po
			INNER JOIN par.instrumentounidade iu ON iu.muncod = po.muncod OR iu.estuf = po.estuf
			LEFT JOIN par.termocompromissopac tcp ON tcp.proid = po.proid AND tcp.terstatus = 'A'
			LEFT JOIN territorios.estado uf ON iu.estuf = uf.estuf
			LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
			WHERE po.proid = re.proidpac
		) 
		END
             
        WHEN i.estuf IS	NOT NULL THEN 
            '-' 
        ELSE 
            mun.mundescricao
        END AS municipio,
        esd.esddsc,
        tpr.tprdescricao,
        CASE WHEN re.inuid IS NULL THEN 
            'FNDE' 
        WHEN i.estuf IS	NOT NULL THEN 
            'Estadual' 
        ELSE 
            'Municipal' 
        END AS tratativa  
    FROM par.restricaoentidade re 
    INNER JOIN par.restricaofnde res ON res.resid = re.resid 
    INNER JOIN par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
    LEFT JOIN par.instrumentounidade i ON re.inuid = i.inuid
    LEFT JOIN territorios.municipio mun ON mun.muncod = i.muncod
    LEFT JOIN territorios.estado uf ON uf.estuf = i.estuf
    LEFT JOIN workflow.documento doc ON doc.docid = re.docid 
    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        " . (!empty($where) ? ' WHERE ' : '') . "
        " . join(' AND ', $where) . "
    ORDER BY
        res.resid ASC
";

if (isset($_REQUEST['btnGerarExcel'])) {#Excel
    
    ob_clean();
    
    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=SIMEC_Relat" . date("Ymdhis") . ".xls");
    header("Content-Disposition: attachment; filename=SIMEC_RelatPAR" . date("Ymdhis") . ".xls");
    header("Content-Description: MID Gera excel");
    $cabecalho = array("Código", "Descrição", "UF", "Municipio", "Estado", "Tipo de Restrição", "Tratativa");
    $db->monta_lista_tabulado($sql, $cabecalho, 1000000, 5, 'N', '100%');
    exit;
} else { #Listagem
    $cabecalho = array("ações", "Código", "Descrição", "UF", "Municipio", "Estado", "Tipo de Restrição", "Tratativa");
    $db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', 'N');
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
        
        $('#btnCancelar').click(function() {
            window.location.reload();
        });
        
        // Habilita somente quando restricao por = Termo
        $('input[name=esfera]').click(function() {
            if ($(this).val() === 'T') {
                ocultarCampos();
                $('.trRestricaoTermo').show();
            } else if ($(this).val() === 'M') {
                ocultarCampos();
                $('#trMunicipio').show();
            } else {
                ocultarCampos();
                $('#trUf').show();
            }
        });

        $('#tpdcod').change(function() {
            var tpdcod = $(this).val();

            $("#tipo_termo").show();

            if (tpdcod == 99999) {
                $("#tipo_termo").hide();
            } else {
                carregarListaTipoDeTermo(tpdcod);
            }
        });
    });

    function ocultarCampos() {
        $('.trRestricaoTermo').hide();
        $('#trMunicipio').hide();
        $('#trUf').hide();
    }

    function alterarRestricaoFNDE(resid)
    {
        window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A&resid=' + resid;
    }

    function excluirRestricaoEntidade(renid)
    {
        if (confirm('Deseja realmente Excluir o Municípo para esta Restrição?'))
        {
            window.location.href = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/acompanhamentoRestricao&acao=A&renid='
                    + renid + '&submeter=excluir';
        }
    }

    function listaEntidade(resid, docid, muncod)
    {
        var title = 'Restrição detalhada #'+resid;
        var data = {action: 'visualizar', resid: resid, docid: docid, muncod: muncod, btnVoltar:'N'};
        var url = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/popupEntidade&acao=A';

        dialogAjax(title, data, url, null, false, true, true, true, false, 900, 400);
    }
    
    // Somente numeros
    $("input[name=resid]").bind("keyup blur focus", function(e) {
        e.preventDefault();
        var expre = /[^0-9]/g;
        if ($(this).val().match(expre))
            $(this).val($(this).val().replace(expre, ''));
    });
    
    // Baixar Arquivos
    $(".downloadArquivo").bind("click", function(e) {
        e.preventDefault();
        window.open('par.php?modulo=sistema/tabelasapoio/restricoes_fnde/cadastrarRestricao&acao=A&requisicao=baixarArquivo&arqid=' + $(this).attr('id'), 'Arquivo', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    });

    // Excluir Arquivos
    $('.excluirArquivo').click(function() {
        if (confirm('Deseja excluir este arquivo?')) {
            window.location.href = window.location.href + '&requisicao=excluirArquivo&arqid=' + $(this).attr('id') + '&resid=' + $('#resid').val();
        }
    });
    /**
     * Abre janela popup para selecionar municipios
     * 
     * @returns VOID
     */
    function abrirPopupListaMunicipio() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }

    /**
     * Abre janela popup para selecionar municipios
     * 
     * @returns VOID
     */
    function abrirPopupListaEstado() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }
    
    $('#formulario').submit(function()
        {
            var esfera = $('input[name="esfera"]:checked').val();

            if (esfera == 'M') {
                var municipios = $('#listaMunicipio').val();
                if (municipios == undefined || municipios == null || municipios == '') {
                    alert('Escolha um municipio.');
                    return false;
                }
            } else if (esfera == 'E') {
                $('#estuf option').attr('selected', 'selected');
                var estados = $('#estuf option:selected').val();

                if (estados == undefined || estados == null || estados == '') {
                    alert('Escolha um estado.');
                    return false;
                }
            }
            return true;
        });
</script>
