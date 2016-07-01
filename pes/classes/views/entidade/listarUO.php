<table class="tabela_filha">
    <?php foreach ($this->lista as $uo): ?>
        <tr class="linha_listagem">
            <!-- Ação-->
            <td class="td_acao" nowrap="" style="text-align:center; width: 86px;">
                <!--            <a href="?modulo=principal/atividade_/atividade&amp;acao=A&amp;atiid=71628">
                                <img align="absmiddle" title="Informações Gerais" style="border: 0;" src="../imagens/alterar.gif">
                            </a>
                            <img border="0" align="absmiddle" style="cursor:pointer;" title="Cadastrar Subtividade" onclick="cadastrar_atividade(71628)" src="../imagens/gif_inclui.gif"> 
                            <img align="absmiddle" src="../imagens/excluir_01.gif">-->
                <img onclick="formularioEntidadeInserir('<?php echo $uo['uorcodigo'] ?>')" border="0" align="absmiddle" style="cursor:pointer;" title="Inserir nova Entidade nesta UO" src="../imagens/gif_inclui.gif">
            </td>
            <!-- Titulo-->
            <td class="td_nome" style="padding-left: 0px; font-weight:bold; width: 100%;">
                <div scroll="no" style=" overflow:hidden; width:600px; height:13px;">
                    &nbsp;&nbsp;&nbsp;
                    <img align="absmiddle" src="../imagens/seta_filho.gif">
                    <img id="uo_img_<?php echo $uo['uorcodigo'] ?>" style="cursor:pointer;" onclick="javascript:carregarEntidade('<?php echo $uo['uorcodigo'] ?>');" src="../imagens/mais.gif"> 
                    &nbsp;
                    <?php echo $uo['uornome'] ?>        
                </div>
            </td>
        </tr>
        <tr id="uo_lista_<?php echo $uo['uorcodigo'] ?>" style="display: none;">
        </tr>
        <script lang="javascript">
            $(document).ready(function(){
                setTimeout('carregarEntidade("<?php echo $uo['uorcodigo'] ?>")', 50)
            }
        );
        </script>
    <?php endforeach; ?>
</table>
<script lang="javascript">

                /**
                 * Comment
                 */
                function formularioEntidadeInserir(uorcodigo)
                {
                    $.ajax({
                        type: "POST",
                        url: "<?php echo '/pes/pes.php?modulo=principal/entidade/cadastro&acao=A' ?>",
                        data: {action: 'formulario', uorcodigo: uorcodigo},
                        success: function(html) {
                            html = '<td colspan="2" style="text-align: center;">' + html + "</td>";

                            //Pegar a linha para inserir o conteudo html.
                            $('.container_formulario').empty().append(html).fadeIn();
                        }
                    });
                }

                /**
                 * javascript
                 */
                function carregarEntidade(uorcodigo)
                {
                    // Alterando a imagem de mais para menos
                    var objImg = $('#uo_img_' + uorcodigo);
                    objImg.attr('src', '../imagens/menos.gif');

                    //Alterando onclick da imagem
                    objImg.attr('onClick', 'javascript:esconderEntidade("' + uorcodigo + '")');

                    $.ajax({
                        type: "POST",
                        url: "<?php echo '/pes/pes.php?modulo=principal/entidade/cadastro&acao=A' ?>",
                        data: {action: 'listarEntidade', uorcodigo: uorcodigo},
                        success: function(html) {

                            html = '<td colspan="2" style=" padding: 0px;">' + html + "</td>";

                            //Pegar a linha para inserir o conteudo html.
                            objLista = $('#uo_lista_' + uorcodigo);
                            objLista.empty().append(html).fadeIn();
                        }
                    });
                }

                /**
                 * 
                 */
                function esconderEntidade(uorcodigo)
                {
                    // Alterando a imagem de mais para menos
                    var objImg = $('#uo_img_' + uorcodigo);
                    objImg.attr('src', '../imagens/mais.gif');

                    //Alterando onclick da imagem
                    objImg.attr('onClick', 'javascript:carregarEntidade("' + uorcodigo + '")');

                    //Pegar Tr orgao
                    objTrOrgao = $('#uo_lista_' + uorcodigo).fadeOut();
                }
</script>
