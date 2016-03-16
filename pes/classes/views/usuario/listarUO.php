<table class="tabela_filha">
    <?php foreach ($this->lista as $uo): ?>
        <tr class="linha_listagem">
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
//                setTimeout('carregarEntidade("<?php echo $uo['uorcodigo'] ?>")', 50)
            }
        );
        </script>
    <?php endforeach; ?>
</table>
<script lang="javascript">

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
            url: window.location.href,
            data: { controller : 'Usuario' , action: 'listarEntidade', uorcodigo: uorcodigo},
            success: function(html) {

                html = '<td colspan="2" style=" padding: 0px;">' + html + "</td>";

                //Pegar a linha para inserir o conteudo html.
                objLista = $('#uo_lista_' + uorcodigo);
                objLista.empty().append(html).fadeIn();
            }
        });
    }

    /**
     * Carrega listagem com as unidades orcamentarias do orgao.
     *
     * @name modalAjax
     * @param {integer} orgcodigo - Codigo do orgao.
     *
     * @since 07/06/2013
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
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