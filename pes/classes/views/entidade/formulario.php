<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


//echo "<pre>";
//var_dump($this->dataForm);
?>
<form method="POST"  name="formulario" id="form_entidade">
    <input type='hidden' name="action" value="salvar">
    <input type='hidden' name="entcodigo" value="<?php echo $this->dataForm['entcodigo'] ?>">
    <input type='hidden' name="uorcodigo" value="<?php echo $this->dataForm['uorcodigo'] ?>">
    <center>
        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%;">
            <tr>
                <td align='right' class="SubTituloDireita">Código fixo:</td>
                <td><?php echo campo_texto('entcodigofixo', 'S', 'S', '', 10, 100, '', '', '', '', '', 'id="entcodigofixo"', '', $this->dataForm['entcodigofixo']); ?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Status:</td>
                <td>
                    <select name="entativa" class="CampoEstilo" size="1" style="width: 80px">
                        <option value="1"  <?php if ($this->dataForm['entativa'] === '1') echo 'selected="true"' ?>>Ativa</option>
                        <option value="0" <?php if ($this->dataForm['entativa'] === '0') echo 'selected="true"' ?>>Inativa</option>
                    </select>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Nome:</td>
                <td><?php echo campo_texto('entnome', 'S', 'S', '', 49, 100, '', '', '', '', '', 'id="entnome"', '', $this->dataForm['entnome']); ?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">UF:</td>
                <td>
                    <select name="ufecodigo" id="ufecodigo" class="CampoEstilo" style="width: auto">
                        <option value="">Selecione</option>
                        <?php foreach ($this->uf as $value): ?>
                            <option <?php if ($value['ufecodigo'] == $this->dataForm['ufecodigo']) echo 'selected="true"' ?> value="<?php echo $value['ufecodigo'] ?>"><?php echo $value['ufenome'] ?></option>
                        <?php endforeach; ?>
                    </select>
                    <!--<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">-->
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Município:</td>
                <td id="container_municipio">
                    <select name="muncodigo" class="CampoEstilo" style="width: 80px"><option value="">Selecione</option></select>
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Endereço:</td>
                <td><?= campo_texto('entendereco', '', 'S', '', 49, 100, '', '', '', '', '', 'id="entendereco"', '', $this->dataForm['entendereco']); ?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">DDD - Telefone:</td>
                <td>
                    <?= campo_texto('entddd', '', 'S', '', 1, 2, '##', '', '', '', '', '', '', $this->dataForm['entddd']); ?>
                    <?= campo_texto('enttelefone', '', 'S', '', 7, 9, '########', '', '', '', '', 'id="enttelefone"', '', $this->dataForm['enttelefone']); ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Área construída total (m²):</td>
                <td><?= campo_texto('entareaconstruida', 'S', 'S', '', 49, 100, '', '', '', '', '', 'id="entareaconstruida"', '', $this->dataForm['entareaconstruida']); ?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Área externa total (m²):</td>
                <td><?= campo_texto('entareaexterna', 'S', 'S', '', 49, 100, '', '', '', '', '', 'id="entareaexterna"', '', $this->dataForm['entareaexterna']); ?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Número de servidores:</td>
                <td>
                    Ativos <?= campo_texto('entqtdativos', 'S', 'S', '', 5, 100, '', '', '', '', '', 'id="entqtdativos"', '', $this->dataForm['entqtdativos']); ?>
                    Terceirizados <?= campo_texto('entqtdterceirizados', 'S', 'S', '', 5, 100, '', '', '', '', '', 'id="entqtdterceirizados"', '', $this->dataForm['entqtdterceirizados']); ?>
                    Outros <?= campo_texto('entqtdoutros', 'S', 'S', '', 5, 100, '', '', '', '', '', 'id="entqtdterceirizados"', '', $this->dataForm['entqtdoutros']); ?>
                </td>
            </tr>
            <tr bgcolor="#CCCCCC">
                <td></td>
                <td>
                    <input type="button" name="btinserir" value="Salvar" onclick="javascript:salvar();" class="botao">
                    <input type="button" name="btcancela" value="Cancelar" onclick="javascript:cancelar();" class="botao">
                </td>
            </tr>
        </table>
    </center>
    <br><br>
</form>
<script language="javascript" type="text/javascript">

                        $('#ufecodigo').change(
                                function() {
                                    renderizarCampoMunicipio($('#ufecodigo').val(), null);
                                }
                        );
                            
//                            $(documento).ready(function () {
                        <?php if($this->dataForm['ufecodigo']): ?>
                                renderizarCampoMunicipio($('#ufecodigo').val(), '<?php echo $this->dataForm['muncodigo'] ?>');
                        <?php endif ?>
//                            });
                            
                        /**
                         * Renderiza campo municipio
                         */
                        function renderizarCampoMunicipio(ufecodigo, muncodigo)
                        {
                            $.ajax({
                                type: "POST",
                                url: "<?php echo '/pes/pes.php?modulo=principal/geral/geral&acao=A' ?>",
                                data: {action: "campoMunicipio", ufecodigo: ufecodigo, name: 'muncodigo', muncodigo: muncodigo},
                                success: function(html) {
                                    var containerMunicipio = $('#container_municipio');
                                    containerMunicipio.hide().empty().append(html).fadeIn();
                                }
                            });
                        }

                        /**
                         * 
                         */
                        function salvar()
                        {
                            var entcodigofixo = $('#entcodigofixo');
                            var entnome = $('#entnome');
                            var muncodigo = $('#muncodigo');
                            var entareaconstruida = $('#entareaconstruida');
                            var entareaexterna = $('#entareaexterna');
                            var entqtdativos = $('#entqtdativos');
                            var entqtdterceirizados = $('#entqtdterceirizados');
                            var entqtdterceirizados = $('#entqtdterceirizados');

                            if (entcodigofixo.val() == "") {
                                msg(entcodigofixo, 'O campo "Código fixo" é necessário!');
                                return false;
                            }
                            if (entnome.val() == "") {
                                msg(entnome, 'O campo "Nome" é necessário!');
                                return false;
                            }
                            if (muncodigo.val() == "") {
                                msg(muncodigo, 'O campo "Município" é necessário!');
                                return false;
                            }
                            if (entareaconstruida.val() == "") {
                                msg(entareaconstruida, 'O campo "Área contruída total" é necessário!');
                                return false;
                            }
                            if (entareaexterna.val() == "") {
                                msg(entareaexterna, 'O campo "Área externa total" é necessário!');
                                return false;
                            }
                            if (entqtdativos.val() == "") {
                                msg(entqtdativos, 'O campo "Número de servidores: Ativos" é necessário!');
                                return false;
                            }
                            if (entqtdterceirizados.val() == "") {
                                msg(entqtdterceirizados, 'O campo "Número de servidores: Terceirizados" é necessário!');
                                return false;
                            }
                            if (entqtdterceirizados.val() == "") {
                                msg(entqtdterceirizados, 'O campo "Número de servidores: Outros" é necessário!');
                                return false;
                            }

                            var dataForm = $('#form_entidade').serialize();

                            $.ajax({
                                type: "POST",
                                url: "<?php echo '/pes/pes.php?modulo=principal/entidade/cadastro&acao=A' ?>",
                                data: dataForm,
                                dataType: 'json',
                                success: function(html) {
                                    //                                    html = '<td colspan="2" style="text-align: center;">' + html + "</td>";
                                    if (html['status'] == true) {
                                        alert(html['msg']);
                                        cancelar();
                                        carregarEntidade('<?php echo $this->dataForm['uorcodigo'] ?>');
                                    } else {
                                        var campo = $("#" + html['name']);
                                        alert(html['msg']);
                                        campo.scrollTop(300);
                                        campo.focus();

                                        $('html, body').animate({scrollTop: campo.offset.top - 300}, 500);
                                    }

                                    //Pegar a linha para inserir o conteudo html.
                                    //                                    $('.container_formulario').empty().append(html).fadeIn();
                                }
                            });

                        }

                        /**
                         * Retira o formulario da tela
                         */
                        function cancelar()
                        {
                            $('.container_formulario').fadeOut();
                            //       $('.formulario').empty();
                        }
</script>