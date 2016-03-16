<?php
#Define o limite de memoria para 1024M
ini_set("memory_limit", "1024M");
set_time_limit(0);

#Monta combo com o tipo de documento de obra
if ($_POST['tipodocumento']) {
    $obModelosDocumentosControle = new ModelosDocumentosControle();
    $sqlModelosDoc = $obModelosDocumentosControle->carregarModeloDocumento($_POST['tipodocumento'], TRUE);
    echo $db->monta_combo("mdoid", $sqlModelosDoc, 'S', 'Selecione', '', '', '', 400, 'N', 'mdoid', '', $_POST['mdoid']);
    exit();
}

#Faz requisicoes
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

#Inclui cabecalho na tela, Monta ABA e TITULO
include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
$tabela_execucao = monta_tabela_execucao(TIPO_OBRAS_PAR);
monta_titulo($titulo_modulo, $tabela_execucao);
?>

<!--Verificar Chamadas de Jquery-->
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/par.js"></script>
<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />
<script type="text/javascript">
    function carregarListaEmpenhoPagamentoObrasPar(pro, obj) {
        var tabela = obj.parentNode.parentNode.parentNode.parentNode.parentNode;
        var linha = obj.parentNode.parentNode.parentNode;

        if (obj.title == 'mais') {
            obj.title = 'menos';
            obj.src = '../imagens/menos.gif';
            var nlinha = tabela.insertRow(linha.rowIndex + 1);
            var col0 = nlinha.insertCell(0);
            col0.innerHTML = "&nbsp;";
            var col1 = nlinha.insertCell(1);
            col1.id = "listapagamentoprocesso_" + pro;
            col1.colSpan = 8;
            carregarListaPagamentoEmpenhoObrasPar('', pro);
        } else {
            obj.title = 'mais';
            obj.src = '../imagens/mais.gif';
            var nlinha = tabela.deleteRow(linha.rowIndex + 1);
        }
    }

    function carregarListaPagamentoEmpenhoObrasPar(empid, processo) {
        $.ajax({
            type: "POST",
            url: "par.php?modulo=principal/listaPagamentoObrasPar&acao=A",
            data: "requisicao=listaPagamentoEmpenhoObrasPar&empid=" + empid + "&processo=" + processo,
            async: false,
            success: function (msg) {
                if (!document.getElementById('listapagamentoprocesso')) {
                    document.getElementById('listapagamentoprocesso_' + processo).innerHTML = msg;
                }
                else {
                    document.getElementById('listapagamentoprocesso').innerHTML = msg;
                }
            }
        });
    }

    function exportarExcel() {
        document.getElementById('acaoBotao').value = 'exportar';
        document.getElementById('formulario').submit();
    }

    $(document).ready(function () {
        if ($('#tpdcod').val() !== '') {
            verificaTipoDocumento($('#tpdcod').val());
        }

        $('[name="perc_n_exec"]').keyup(function () {
            if (parseFloat($(this).val()) > 100) {
                $(this).val('100');
            }
        });
    });

    function verificaTipoDocumento(tipo) {
        
        if (tipo !== '') {
            $.ajax({
                type: "POST",
                url: "par.php?modulo=principal/listaPagamentoObrasPar&acao=A",
                data: "tipodocumento=" + tipo + '&mdoid=<?= $_REQUEST['mdoid'] ?>',
                async: false,
                success: function (msg) {
                    $('#tr_modelodoc').show();
                    $('#div_modelodoc').html(msg);
                }
            });
        } else {
            $('#tr_modelodoc').hide();
        }
    }

    function pesquisarObrasPar() {
        document.getElementById('acaoBotao').value = 'pesquisar';
        selectAllOptions(document.getElementById('tipoobra'));
        selectAllOptions(document.getElementById('grupoMunicipio'));

        if (document.getElementById('termogerado_1').checked === true) {
            if ($('#tpdcod').val() === '') {
                alert('Informe um Tipo de Documento!');
                $('#tpdcod').focus();
                return false;
            }
            if ($('#mdoid').val() === '') {
                alert('Informe um Modelo de Documento!');
                $('#mdoid').focus();
                return false;
            }
        } else {
            if (jQuery('#tpdcod').val() !== '') {
                if (jQuery('#mdoid').val() === '') {
                    alert('Informe um Modelo de Documento!');
                    jQuery('#mdoid').focus();
                    return false;
                }
            }
        }
        document.getElementById('formulario').submit();
    }

    function exibeTR(id, id2) {
        jQuery("#" + id2).hide();
        jQuery("#" + id).show();
    }
</script>
<form method="post" name="formulario" id="formulario">
    <input type="hidden" name="exportarexcel" id="exportarexcel" value="">
    <table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tbody>
            <tr>
                <td class="SubTituloDireita" width="30%">Preid:</td>
                <td><?php echo campo_texto('preid', 'N', 'S', 'Preid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['preid']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Obrid:</td>
                <td><?php echo campo_texto('obrid', 'N', 'S', 'Obrid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['obrid']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Número de Processo:</td>
                <td>
                    <?php
                    $numeroprocesso = simec_htmlentities($_POST['numeroprocesso']);
                    echo campo_texto('numeroprocesso', 'N', 'S', '', 60, 200, '#####.######/####-##', '', '', '', '', '', '', $numeroprocesso);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Município:</td>
                <td>
                    <?php
                    $municipio = simec_htmlentities($_POST['municipio']);
                    echo campo_texto('municipio', 'N', 'S', '', 60, 200, '', '', '', '', '', '', '', $municipio);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Estado:</td>
                <td>
                    <?php
                    $obEstadoControle = new EstadoControle();
                    $sqlEstado = $obEstadoControle->carregarUf(TRUE);
                    $db->monta_combo("estuf", $sqlEstado, 'S', 'Todas as Unidades Federais', '', '', '', '', '', '', '', $_REQUEST['estuf']);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Esfera:</td>
                <td>
                    <?php
                    $arrEsfera = array(
                        array("codigo" => "Municipal", "descricao" => "Municipal"),
                        array("codigo" => "Estadual", "descricao" => "Estadual")
                    );
                    $db->monta_combo("esfera", $arrEsfera, 'S', 'Todas as esferas', '', '', '', '', '', '', '', $_REQUEST['esfera']);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Grupo de Município:</td>
                <td>
                    <?php
                    #TODO VERIFICAR RETORNO DO PREENCHIMENTO
                    $obMunicipioControle = new MunicipioControle();
                    $sqlGrupoMunicipio = $obMunicipioControle->carregarGrupoMunicipio(TRUE);
                    combo_popup('grupoMunicipio', $sqlGrupoMunicipio, 'Selecione...', "400x400", 0, array(), "", "S", false, false, 5, 400, '', '', '', '', $arrGrupoMunicipio);
                    ?>															
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Tipo de Obra:</td>
                <td>
                    <?php
                    #TODO VERIFICAR RETORNO DO PREENCHIMENTO
                    $obPreTipoObraControle = new PreTipoObraControle();
                    $sqlPreTipoObra = $obPreTipoObraControle->carregarPreTipoObraClassificacao(TRUE);
                    combo_popup('tipoobra', $sqlPreTipoObra, 'Lista de Tipo de Obra', "400x400", 0, array(), "", "S", false, false, 5, 400, '', '', '', '', $arrPreTipoObra);
                    ?>															
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Termo:</td>
                <td>
                    <fieldset style="width: 380px"><legend>Filtros Termo:</legend>
                        <input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="termogerado" id="termogerado_1" value="1" >Gerado
                        <input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="termogerado" id="termogerado_2" value="2" >Não Gerado
                        <input <?php echo empty($_REQUEST['termogerado']) ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="" >Todos
                        <table border="0"  align="left" bgcolor="#f5f5f5" cellpadding="0" cellspacing="0" width="380px">
                            <tr>
                                <td class="SubTituloDireita" style="width: 60px;text-align:center;" >Validação:</td>
                                <td>
                                    <table border="0" style="width: 100%; background-color: #f5f5f5">
                                        <tr>
                                            <td class="SubTituloDireita" style="width: 40%;" >Gestor:</td>
                                            <td>
                                                <input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "t" ? "checked='checked'" : "" ?>  type="radio" value="t" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />Sim 
                                                <input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "f" ? "checked='checked'" : "" ?>  type="radio" value="f" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />Não
                                                <input <?php echo empty($_REQUEST['dopusucpfvalidacaogestor']) ? "checked='checked'" : "" ?> type="radio" name="dopusucpfvalidacaogestor" value="" >Todos
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" style="width: 40%;" >De outras Entidades:</td>
                                            <td>
                                                <input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "t" ? "checked='checked'" : "" ?> type="radio" value="t" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde" />Sim
                                                <input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "f" ? "checked='checked'" : "" ?> type="radio" value="f" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde"  />Não
                                                <input <?php echo empty($_REQUEST['dopusucpfvalidacaofnde']) ? "checked='checked'" : "" ?> type="radio" name="dopusucpfvalidacaofnde" value="" >Todos
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                        </table>
                    </fieldset>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" >Tipo de Documento:</td>
                <td colspan="3">
                    <?php
                    $obTipoDocumento = new TipoDocumentoControle();
                    $sqlTipoDocumento = $obTipoDocumento->carregarTipoDocumento(TRUE);
                    $db->monta_combo("tpdcod", $sqlTipoDocumento, 'S', 'Selecione', 'verificaTipoDocumento', '', '', 400, 'N', 'tpdcod', '', $_REQUEST['tpdcod']);
                    ?>
                </td>
            </tr>
            <tr id="tr_modelodoc" style="display: none">
                <td class="SubTituloDireita" >Modelo de Documento:</td>
                <td colspan="3">
                    <div id="div_modelodoc">
                        <?php
                        $db->monta_combo("mdoid", array(), 'S', 'Selecione', '', '', '', 400, 'N', 'mdoid');
                        ?>
                    </div>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Pagamento:</td>
                <td>
                    <input <?php echo $_REQUEST['pagamento'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="1" />Pagamento Solicitado
                    <input <?php echo $_REQUEST['pagamento'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="2" />Pagamento Não Solicitado
                    <input <?php echo $_REQUEST['pagamento'] == "3" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="3" />Pagamento efetivado
                    <input <?php echo $_REQUEST['pagamento'] == "0" || empty($_REQUEST['pagamento']) ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="0" />Todos
                </td>
            </tr>
            <tr>
                <td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')">
                    <b>Possui Obras com:</b>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Ordem de serviço inserida:</td>
                <td>
                    <input type="radio" value="S" id="homologacao" name="homologacao" <?php echo ($_REQUEST["homologacao"] == "S") ? "checked" : ""; ?> /> Sim
                    <input type="radio" value="N" id="homologacao" name="homologacao" <?php echo ($_REQUEST["homologacao"] == "N") ? "checked" : ""; ?> /> Não Validados
                    <input type="radio" value="P" id="homologacao" name="homologacao" <?php echo ($_REQUEST["homologacao"] == "P") ? "checked" : ""; ?> /> Não Analisados
                    <input type="radio" value="" id="homologacao" name="homologacao"  <?php echo (empty($_REQUEST["homologacao"])) ? "checked" : ""; ?> /> Todos
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Execução 25% Validada:</td>
                <td>
                    <input type="radio" value="S" id="execucao25" name="execucao25" <?php echo ($_REQUEST["execucao25"] == "S") ? "checked" : ""; ?> /> Sim
                    <input type="radio" value="N" id="execucao25" name="execucao25" <?php echo ($_REQUEST["execucao25"] == "N") ? "checked" : ""; ?> /> Não Validados
                    <input type="radio" value="P" id="execucao25" name="execucao25" <?php echo ($_REQUEST["execucao25"] == "P") ? "checked" : ""; ?> /> Não Analisados
                    <input type="radio" value="" id="execucao25" name="execucao25" <?php echo (empty($_REQUEST["execucao25"])) ? "checked" : ""; ?> /> Todos
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Execução 50% Validada:</td>
                <td>
                    <input type="radio" value="S" id="execucao50" name="execucao50" <?php echo ($_REQUEST["execucao50"] == "S") ? "checked" : ""; ?> /> Sim
                    <input type="radio" value="N" id="execucao50" name="execucao50" <?php echo ($_REQUEST["execucao50"] == "N") ? "checked" : ""; ?> /> Não Validados
                    <input type="radio" value="P" id="execucao50" name="execucao50" <?php echo ($_REQUEST["execucao50"] == "P") ? "checked" : ""; ?> /> Não Analisados
                    <input type="radio" value="" id="execucao50" name="execucao50" <?php echo (empty($_REQUEST["execucao50"])) ? "checked" : ""; ?> /> Todos
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" >Situação da Obra:</td>
                <td colspan="3">
                    <?php
                    $obEstadoDocumento = new EstadoDocumentoControle();
                    $sqlEstadoDocumento = $obEstadoDocumento->carregarEstadoDocumento(105, TRUE);
                    $db->monta_combo("esdid", $sqlEstadoDocumento, 'S', 'Selecione', '', '', '', '', 'N', 'esdid', '', $_REQUEST['esdid']);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" style="width: 190px;">Obras não paga até:</td>
                <td>
                    <?php
                    echo campo_texto('perc_n_exec', 'N', 'S', '', 3, 3, '[#]', '', 'right', '', 0, '', '', $_REQUEST['perc_n_exec']);
                    ?> %
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Obras MUNICIPAIS com pendências:</td>
                <td>
                    <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
                    <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
                    <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Obras ESTADUAIS com pendências:</td>
                <td>
                    <input <?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1" value="1" >Sim
                    <input <?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2" value="2" >Não
                    <input <?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" value=""   />Todos
                </td>
            </tr>
            <tr>
            	<td class="SubTituloDireita">Possui Solicitação de Desembolso:</td>
                <td>
                	<input <?php echo $_REQUEST['boSolicitacaoDesembolso'] == "S" ? "checked='checked'" : "" ?> type="radio" name="boSolicitacaoDesembolso" id="boSolicitacaoDesembolso_S" value="S" >Sim
                    <input <?php echo $_REQUEST['boSolicitacaoDesembolso'] == "N" ? "checked='checked'" : "" ?> type="radio" name="boSolicitacaoDesembolso" id="boSolicitacaoDesembolso_N" value="N" >Não
                    <input <?php echo empty($_REQUEST['boSolicitacaoDesembolso']) ? "checked='checked'" : "" ?> type="radio" name="boSolicitacaoDesembolso" value=""   />Todos
				</td>
			</tr>
            <?php
            if ($db->testa_superuser()):
                ?>
                <tr>
                    <td class="SubTituloDireita" style="color:red">Possui erro na distribuição do pagamento:</td>
                    <td>
                        <input <?php echo $_REQUEST['pagamentoMaiorQueEmpenho'] == "S" ? "checked='checked'" : "" ?> type="radio" name="pagamentoMaiorQueEmpenho" id="pagamentoMaiorQueEmpenho" value="S" >Sim
                        <input <?php echo empty($_REQUEST['pagamentoMaiorQueEmpenho']) ? "checked='checked'" : "" ?> type="radio" name="pagamentoMaiorQueEmpenho" value=""   />Todos
                    </td>
                </tr>
            <?php endif; ?>
            <tr>
                <td class="SubTituloDireita"></td>
                <td>
                    <input class="botao" type="button" name="pesquisar" id="pesquisar" value="Pesquisar" onclick="pesquisarObrasPar();" />
                    <input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location = 'par.php?modulo=principal/listaPagamentoObrasPar&acao=A';" />
                    <input class="botao" type="button" name="exportar" id="exportar" value="Exportar Excel" onclick="exportarExcel();" />
                    <input type="hidden" name="acaoBotao" id="acaoBotao" value="" />
                </td>
            </tr>
        </tbody>
    </table>
</form>

<?php
# Filtros da pesquisa
if (isset($_REQUEST['acaoBotao'])) {
    #Instancia de ProcessoObrasParControle
    $obProcessoObrasPar = new ProcessoObrasParControle();

    $cabecalho = array("&nbsp;", "&nbsp;", "Nº Processo", "Município", "UF", "Vlr obras no processo", "Valor empenhado(R$)", "Valor pagamento(R$)", "Qtd obras no processo", "Esfera", 'Sistema', 'Entidade Habilitada', 'Situação');
    if ($_REQUEST['acaoBotao'] == 'exportar') {#Excel
		$_REQUEST['colunas'] = Array('saldo_conta_corrente');
	    $mixObra = $obProcessoObrasPar->buscarPagamentoObrasPar($_REQUEST, TRUE);
        $cabecalho = array("Nº Processo", "Município", "UF", "Vlr obras no processo", "Valor empenhado(R$)", "Valor pagamento(R$)", "Qtd obras Empenhadas", "Esfera", 'Sistema', 'Entidade Habilitada', 'Saldo em Conta', 'Situação');
        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatDocente" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_PagamentoObrasPAR" . date("Ymdhis") . ".xls");
        $db->monta_lista_tabulado($mixObra, $cabecalho, 1000000, 5, 'N', '100%');
        exit;
    } else { #Listagem
	    $mixObra = $obProcessoObrasPar->buscarPagamentoObrasPar($_REQUEST, TRUE);
        $db->monta_lista($mixObra, $cabecalho, 25, 10, 'N', 'center', 'N');
    }
}
?>

<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";
?>
<style>
    .popup_alerta{ width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura / 2 ?>;margin-left:-<?php echo $largura / 2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
    <div style="width:100%;text-align:right">
        <img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display = 'none'" />
    </div>
    <div style="padding:5px;text-align:justify;">
        <table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td width="30%" class="SubtituloDireita">Usuário:</td>
                <td><?php echo campo_texto("ws_usuario_consulta", "S", "S", "Usuário", "22", "", "", "", "", "", "", "id='ws_usuario_consulta'") ?></td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Senha:</td>
                <td>
                    <input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);
                            this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_consulta" name="ws_senha_consulta">
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td align="center" bgcolor="#D5D5D5" colspan="2">
                    <input type="hidden" name="ws_empid" id="ws_empid" value="" />
                    <input type="hidden" name="ws_processo" id="ws_processo" value="" />
                    <input type="button" name="btn_enviar" onclick="consultarEmpenhoWS()" value="ok" />
                    <input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display = 'none'" value="cancelar" />
                </td>
            </tr>
        </table>
    </div>
</div>

<?php

function listaPagamentoEmpenhoObrasPar($dados) {
    global $db;

    if ($dados['processo']) {
        $arrProcesso = $db->pegaLinha("SELECT muncod, estuf FROM par.processoobraspar WHERE proid = '" . $dados['processo'] . "'");
        if ($arrProcesso['muncod'])
            $esfera = 'municipal';
        if ($arrProcesso['estuf'])
            $esfera = 'estadual';
    }

    if (!$dados['processo']) {
        die("<p align=center><b>Número do processo não encontrado. Por favor feche a janela e reinicie o procedimento.</b></p>");
    }

    $where[] = "p.proid = '" . $dados['processo'] . "'";
    if ($esfera == 'estadual') {
//		$where[] = "funid = 6";
        //$where[] = "dutid = 9";
    } else {
//		$where[] = "funid = 1";
        //$where[] = "dutid = 6";
    }

    $where[] = "p.prostatus = 'A'";
    $sql = "SELECT distinct
                '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarPagamento(\''||e.empid||'\', this);\">' as mais,
                e.empcnpj,
                en.entnome,
                e.empnumero,
                e.empvalorempenho,
                u.usunome,
                e.empsituacao
            FROM 
                par.processoobraspar p
            INNER JOIN par.empenho e ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso) AND empstatus = 'A'
            LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
            LEFT JOIN par.entidade en ON en.entnumcpfcnpj=e.empcnpj
            " . (($where) ? "WHERE " . implode(" AND ", $where) : "");

    $cabecalho = array("&nbsp;", "CNPJ", "Entidade", "Nº Empenho", "Valor empenho(R$)", "Usuário criação", "Situação empenho");
    $db->monta_lista_simples($sql, $cabecalho, 500, 5, 'N', '100%', $par2);
}
?>