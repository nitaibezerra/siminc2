    <tr>
        <td align="left" colspan="2"><b>Dados da Damandas</b></td>
    </tr>
    <tr>
        <td class="SubTituloDireita" >Título:</td>
        <td>
            <?PHP
                $tartitulo = $obTarefa->tartitulo;
                echo campo_texto('tartitulo', 'S', $permissao_formulario, 'Título', 60, 255, '', '', '', '', '', 'id="tartitulo"');
                $barras = array('\\', '\/', '\\', '\/');
                $tartitulo = str_replace($barras, "/", $obTarefa)
            ?> 
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" >Modalidade:</td>
        <td>
            <?php
                $sql = "select tmdid, tmddescricao from gestaodocumentos.tipomodalidade";
                $arTipoModalidade = $db->carregar($sql);
                $arTipoModalidade = ( is_array($arTipoModalidade) ) ? $arTipoModalidade : array();
                
                foreach ($arTipoModalidade as $modalidade) {
                    if ($obTarefa->tmdid == $modalidade['tmdid']) {
                        $ckecked = "checked=\"checked\"";
                    }
                    echo "<input type=\"radio\" $ckecked id=\"{$modalidade['tmddescricao']}\" title=\"Modalidade\" name=\"tmdid\" value=\"{$modalidade['tmdid']}\" align=\"bottom\"><label for=\"{$modalidade['tmddescricao']}\">{$modalidade['tmddescricao']}</label>";
                    $ckecked = "";
                }
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" >Tema:</td>
        <td>
            <?PHP
                $tartema = $obTarefa->tartema;
                echo campo_texto('tartema', 'S', $permissao_formulario, 'Tema', 60, 50, '', '', '', '', '', 'id="tartema"'); 
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Descrição:</td>
        <td><? $tardsc = $obTarefa->tardsc; ?>
            <?= campo_textarea('tardsc', 'N', 'S', 'Descrição ', 65, 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição'); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" align="right">Data do Recebimento: </td>
        <td>  
            <?PHP
                $tardatarecebimento = $obTarefa->tardatarecebimento;
                echo campo_data('tardatarecebimento', 'S', 'S', 'Data do Recebimento', 'S'); 
            ?>
        </td> 
    </tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Instituição Avaliada:</td>
        <td align="left">
        </td>
    </tr>