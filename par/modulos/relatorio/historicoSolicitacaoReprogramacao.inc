<?php

# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';

# Titulo
monta_titulo('Histórico de Solicitação de Reprogramação', '');

?>
<form id="formulario" name="formulario" method="post" action="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr id="trUf">
            <td class="SubTituloDireita">UF:</td>
            <td>
               <?php
                $obEstadoControle = new EstadoControle();
                $strEstado = $obEstadoControle->carregarUf(TRUE);
                $arrUf = array();
                if (!empty($_REQUEST['estuf'][0])) {
                    foreach ($_REQUEST['estuf'] as $value) {
                        $arrUf[] = array('codigo' => $value, 'descricao' => $value);
                    }
                }
                combo_popup("estuf", $strEstado, "Lista de Estados", "400x400", 0, array(), "", "S", false, false, 5, 400, '', '', '', '', $arrUf);
                ?>
            </td>
        </tr>
        <tr id="trMunicipio">
            <td class="SubTituloDireita"><b>Municípios</b></td>
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
                            <?php
                            if (!empty($_REQUEST['listaMunicipio'][0])) {
                                $obMunicipioControle = new MunicipioControle();
                                foreach ($_REQUEST['listaMunicipio'] as $value) {
                                    $arrMunicipio = $obMunicipioControle->carregarMunicipioPorMuncod($value);
                                    echo "<option value=\"{$arrMunicipio['codigo']}\">{$arrMunicipio['descricao']}</option>";                               
                                }
                            }else{
                                echo "<option value=\"\">Duplo clique para selecionar da lista</option>";
                            }
                        ?>
                    </select>
                </div>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Termo de Compromisso:</b></td>
            <td>
               <?php
                    echo campo_texto( 'dopnumerodocumento', 'N', 'S', '', 20, 50, '[#]', '', '', '', '', '', '', $_REQUEST['dopnumerodocumento']);
		?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Número de Protocolo:</b></td>
            <td>
               <?php
                    echo campo_texto( 'hsrprotocolo', 'N', 'S', '', 20, 50, '[#]', '', '', '', '', '', '', $_REQUEST['hsrprotocolo']);
		?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Tipo de Reprogramação:</b></td>
            <td>
                <input type="radio" value='S' name="hsrtipo" <?php echo ($_REQUEST['hsrtipo'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Subação</label>
                <input type="radio" value='P' name="hsrtipo" <?php echo ($_REQUEST['hsrtipo'] == 'P') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Prazo</label>
                <input type="radio" value='T' name="hsrtipo" <?php echo ($_REQUEST['hsrtipo'] == 'T' || $_REQUEST['hsrtipo'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr class="div_situacao" style="display: <?php echo (!empty($_REQUEST['situacao'])) ? '' : 'none'?>">
            <td class="SubTituloDireita"></td>
            <td>
                <div class="tipo_data tipo_subacao">
                    <input type="radio" value='A' name="situacao" <?php echo ($_REQUEST['situacao'] == 'A') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Aguardando Validação do FNDE</label><br />
                </div>
                <div class="tipo_subacao">
                    <input type="radio" value='B' name="situacao" <?php echo ($_REQUEST['situacao'] == 'B') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Aguardando Conclusão do Município / Estado</label><br />
                </div>
                <div class="tipo_subacao">
                    <input type="radio" value='C' name="situacao" <?php echo ($_REQUEST['situacao'] == 'C') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Aguardando Aprovação do FNDE</label><br />
                </div>
                <div class="tipo_subacao">
                    <input type="radio" value='G' name="situacao" <?php echo ($_REQUEST['situacao'] == 'G') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Aguardando Empenho</label><br />
                </div>
                <div class="tipo_data">
                    <input type="radio" value='B' name="situacao" <?php echo ($_REQUEST['situacao'] == 'B' && $_REQUEST['hsrtipo'] == 'P') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Aguardando Geração de Termo</label><br />
                </div>
                <div class="tipo_subacao">
                    <input type="radio" value='D' name="situacao" <?php echo ($_REQUEST['situacao'] == 'D') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Aguardando Geração de Termo</label><br />
                </div>
                <div class="tipo_subacao">
                    <input type="radio" value='F' name="situacao" <?php echo ($_REQUEST['situacao'] == 'F') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Aguardando Finalização de Reprogramação</label><br />
                </div>
                <div class="tipo_data tipo_subacao">
                    <input type="radio" value='E' name="situacao" <?php echo ($_REQUEST['situacao'] == 'E') ? "checked='checked'" : '' ?> class="link" >
                    <label class="link">Reprogramações Finalizadas</label><br />
                </div>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="button" name="btnPesquisar" id="btnPesquisar" value="Filtrar" />
                <input type="hidden" name="acaoBotao" value="" />
            </td>
        </tr>
    </table>
</form>

<?php
# Filtros da pesquisa
if (isset($_REQUEST['acaoBotao'])) {
    #Instancia de ProcessoPar
    $obHistSolicitacaoRep = new HistoricoSolicitacaoReprogramacaoControle();
    $mixHistSolicitacaoRep = $obHistSolicitacaoRep->relatorioHistoricoSolicitacaoReprogramacao($_REQUEST, TRUE);

    $cabecalho = array("Entidade", "UF", "Termo de Compromisso", "Subação", "Tipo de Solicitação", "Data de Solicitação", "Usuário da Solicitação", "Justificativa Apresentada", "Número do Protocolo", "Situação", "Resultado", "Justificativa", "Data", "Usuário");
    $db->monta_lista($mixHistSolicitacaoRep, $cabecalho, 50, 15, '', 'center', 'N', '', '', '', '', '');
}
?>

<script type="text/javascript">
    jQuery(document).ready(function () {

        jQuery('#btnPesquisar').click(function(){
            var estuf = document.getElementById('estuf');
            selectAllOptions( estuf );
            var muncod = document.getElementById('listaMunicipio');
            selectAllOptions( muncod );
            jQuery('input[name=acaoBotao]').val(jQuery(this).attr('id'));
            jQuery('#formulario').submit();
        });
        
        jQuery('input[name=hsrtipo]').click(function(){
            if (jQuery(this).val() !== 'T') {
                jQuery('.div_situacao').css('display', '');
                if (jQuery(this).val() === 'S') {
                    jQuery('.tipo_data').hide();
                    jQuery('.tipo_subacao').show();
                } else {
                    jQuery('.tipo_subacao').hide();
                    jQuery('.tipo_data').show();
                }
            } else {
                jQuery('.div_situacao').css('display', 'none');
            }
        });
        jQuery('input[name=hsrtipo]:checked').click();
    });

    /**
     * Abre janela popup para selecionar municipios
     * @returns VOID
     */
    function abrirPopupListaMunicipio() {
        window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }
</script>