<?php
    # Inicializa sistema
//    require_once "config.inc";

    $requisicao = $_REQUEST['requisicao'];
    switch ($requisicao) {
        case 'carregarListaTipoDeTermo':
            carregarListaTipoDeTermo();
            die;
        break;
        case 'listarMunicipioTermo':
            $listaFiltro = $_REQUEST;
            listarMunicipioTermo($listaFiltro);
            die;
        break;
        case 'listarTermo':
            $listaFiltro = $_REQUEST;
            listarTermo($listaFiltro);
            die;
        break;
    }
    
    $db = new cls_banco();
    $sqlEstado = "
        SELECT
            e.estuf as codigo,
            e.estdescricao as descricao
        FROM
            territorios.estado e
        ORDER BY
            e.estdescricao ASC
    ";
    $listaEstado = $db->carregar( $sqlEstado );
?>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <table class="tabela" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <th colspan="2">Dados da Demanda</th>
        </tr>
        <tr>
            <td style="width: 100%;" valign="top">
                <form id="formTermo" name="formTermo" method="POST" action="">
                    <table class="tabela" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
                        <tr class="trRestricaoTermo">
                            <td class="SubTituloDireita" valign="top"><b>Tipo de documento</b></td>
                            <td>
                                <?php
                                    $sql = "
                                        SELECT DISTINCT
                                            99999 as codigo,
                                            'Termo de Compromisso - PAC' AS descricao
                                        UNION
                                        SELECT DISTINCT
                                            td.tpdcod AS codigo,
                                            td.tpddsc AS descricao
                                        FROM public.tipodocumento td
                                        WHERE
                                            tpdcod IN(".
                                                TIPO_DE_DOCUMENTO_TERMO_ADITIVO.",".
                                                TIPO_DE_DOCUMENTO_TERMO_DE_COMPROMISSO.",".
                                                TIPO_DE_DOCUMENTO_TERMO_REFORMULADO.
                                            ")
                                        ORDER BY
                                            descricao ASC
                                    ";
                                    $db->monta_combo("tpdcod", $sql, 'S', 'Selecione...', '', '', 'Tipo de documento', '', '', 'tpdcod', '', $tpdcod, '', '', ' link ');
                                ?>
                                &nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                            </td>
                        </tr>
                        <tr class="trRestricaoTermo" id="tipo_termo">
                            <td class="SubTituloDireita" valign="top"><b>Tipo de termo</b></td>
                            <td>
                                <div id="divTipoTermo">
                                    <?php
                                        $db->monta_combo("mdoid", array(), 'S', 'Selecione...', '', '', 'Tipo de termo', '', '', 'mdoid', '', $mdoid, '', '', ' link ');
                                    ?>
                                    &nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                                </div>
                            </td>
                        </tr>
                        <tr class="trRestricaoTermo">
                            <td class="SubTituloDireita" valign="top"><b>UF</b></td>
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
                                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                            </td>
                        </tr>
                        <tr class="trRestricaoTermo">
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
                                            <option value="">Duplo clique para selecionar da lista</option>
                                    </select>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td align="center" bgcolor="#c0c0c0" colspan="2">
                                <input type="submit" id="btnPesquisarTermo" value="Pesquisar" />
                            </td>
                        </tr>
                    </table>
                </form>
            </td>
        </tr>
        <tr>
            <td style="width: 100%;" valign="top" id="tdListaMunicipioTermo"></td>
        </tr>
    </table>

<script>
    $(document).ready(function() {
        
        $('#tpdcod').change(function(){
            var tpdcod = $(this).val();
                
            $("#tipo_termo").show();

            if(tpdcod == 99999) {
                $("#tipo_termo").hide();
            } else {
                carregarListaTipoDeTermo(tpdcod);
            }

        });

        $('#formTermo').submit(function(){
            $('#estuf option').attr('selected', 'selected');
            $.ajax({
                type: 'POST',
                url: window.location.href,
                data: 'requisicao=listarMunicipioTermo&'+$('#formTermo').serialize(),
                async: false,
                success: function(resultado){
                    $('#tdListaMunicipioTermo').empty();
                    $('#tdListaMunicipioTermo').html(resultado);
                }
            });
            return false;
        });
        
        jQuery('input.listaTermo').live('change', function(){
            
            var dopid = jQuery(this).val();
            // verifica se a caixa do popup está marcada
            if(jQuery(this).attr('checked') ==  true) {
                // Verifica se caixa da janela de cadastro de restrições ainda não foi criada
                if(opener.document.getElementById('listaTermo_'+dopid) == null){
                    // Insere os dadados no formulario de cadastro de restrições
                    jQuery(this).attr('checked','true');
                    opener.document.getElementById('tbodyTermo').innerHTML += '<tr id="tr_termo_'+dopid+'" >'+jQuery(this).closest('tr').html()+'</tr>';
                    opener.document.getElementById('listaTermo_'+dopid).setAttribute('checked','true');
                }
            } else {
                opener.document.getElementById('tr_termo_'+dopid).remove();
            }
            
        });

    });
    
    /**
     * Carrega lista de tipos de termo de acordo com o tipo de documento selecionado
     * 
     * @param integer tpdcod
     * @returns VOID
     */
    function carregarListaTipoDeTermo(tpdcod){
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: "requisicao=carregarListaTipoDeTermo&tpdcod="+tpdcod,
            async: false,
            success: function(resultado){
                $('#divTipoTermo').empty();
                $('#divTipoTermo').html(resultado);
                divCarregado();
            }
        });
    }
    
    /**
     * Abre janela popup para selecionar municipios para pesquisa do termo
     * 
     * @returns VOID
     */
    function abrirPopupListaMunicipio() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formTermo', 'municipios', 'width=400,height=400,scrollbars=1');
    }
    
    /**
     * Lista os termos do municipio
     * 
     * @param {type} idImg
     * @param {type} muncod
     * @returns VOID
     */
    function carregarListaTermo(idImg, muncod){
        var img = jQuery('#'+idImg);
        var tr_nome = 'listaTermo_'+ muncod;
        var td_nome = 'trV_'+ muncod;

        if(jQuery('#'+tr_nome).css('display') == 'none'){
            img.attr('src', '../imagens/menos.gif');
            jQuery('#'+tr_nome).show();
            if(jQuery('#'+td_nome).html() == ''){
                jQuery.ajax({
                    type: 'POST',
                    url: 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/selecionarTermo&acao=A& =listarTermo&muncod='+muncod,
                    data: 'requisicao=listarTermo&'+jQuery('#formTermo').serialize(),
                    async: false,
                    success: function(resultadoHtml){
                        jQuery('#'+td_nome).html(resultadoHtml);
                    }
                });
            }
        } else {
            img.attr('src', '../imagens/mais.gif');
            jQuery('#'+tr_nome).hide();
        }
    }

    function consultarTermo(terid) {
    
    window.open('par.php?modulo=principal/gerarTermoObra&acao=A&requisicao=download&O='+terid, 
                'modelo', 
                "height=600,width=400,scrollbars=yes,top=0,left=0" );
    }

    function consultarTermoPorDopid(dopid) {
    
    window.open('par.php?modulo=principal/documentoPar&acao=A&req=formVizualizaDocumento&dopid='+dopid, 
                'modelo', 
                "height=600,width=400,scrollbars=yes,top=0,left=0" );
    }
</script>
    