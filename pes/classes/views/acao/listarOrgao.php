<style>
    .tabela_filha{
        border-collapse: collapse;
        border: 0px solid red; padding: 0px; margin: 0px;
        width: 100%;
    }
    .td_acao {
        /*border: 2px solid blue;*/ 
        border-right: 2px solid #e0e0e0;
        border-bottom: 2px solid #e0e0e0;
        padding: 0px; 
        margin: 0px;
    }

    .td_nome {
        /*border: 2px solid blue;*/ 
        border-right: 0px solid #e0e0e0;
        border-bottom: 2px solid #e0e0e0;
        padding: 0px; 
        margin: 0px;
    }

    /*.tabela_filha tbody {border: 1px solid blue; padding: 0px; margin: 0px;}*/
    .tabela_filha tr {
        border: 0px solid red; 
        padding: 0px;
        margin: 0px;
        width: 100%;
        height: 20px;
        background-color: #fafafa;
    }

    .linha_listagem{
        background-color: #fafafa;
    }

    .linha_listagem:hover{
        background-color: #ffffcc;
    }
    
    a {
        color: #000;
    }
    
    a:hover{
        color: #000;
        text-decoration:none; 
    }
</style>
<table cellpadding="3" style="width:100%;">
    <!--<colgroup><col width="80"><col width="50"><col><col width="80"><col width="70"><col width="70"><col width="50"></colgroup>-->
    <thead>
        <tr style="background-color: #e0e0e0">
            <td style="font-weight:bold; text-align:center; width: 80px;" >Ação</td>
            <td style="font-weight:bold; text-align:center;">Nome</td>
        </tr>
    </thead>
    <tbody>
        <?php $n = 0 ?>
        <?php if ($this->listaOrgao): ?>
            <?php foreach ($this->listaOrgao as $orgao): ?>
                <?php $n++ ?>
                <tr class="linha_listagem">
                    <!-- Ação-->
                    <td nowrap="" style="text-align:center; width: 80px;">
                    </td>
                    <!-- Titulo-->
                    <td style="padding-left: 0px; font-weight:bold;">
                        <div scroll="no" style=" overflow:hidden; width:600px; height:13px;">
                            &nbsp;&nbsp;
                            <img id="org_img_<?php echo $orgao['orgcodigo'] ?>" style="cursor:pointer;" onclick="javascript:carregarUnidadeOrcamentaria( '<?php echo $orgao['orgcodigo'] ?>' );" src="../imagens/mais.gif"> 
                            <!--<span style="margin: 0 5px 0 0; "><?php echo $n ?></span>-->
                            &nbsp;
                            <?php echo $orgao['orgnome'] ?>        
                        </div>
                    </td>
                </tr>
                <tr id="org_lista_<?php echo $orgao['orgcodigo'] ?>" style="background-color: #e0e0e0; display: none;" >
                </tr>
        </table>
        </td>
        </tr>
        <script lang="javascript">
            $(document).ready(function(){
                setTimeout('carregarUnidadeOrcamentaria("<?php echo $orgao['orgcodigo'] ?>")', 50)
            }
        );
        </script>
    <?php endforeach ?>
<?php endif ?>
</tbody>
<script lang="javascript" contentType="text/html; charset=UTF-8">

    /**
     * javascript
     */
    function carregarUnidadeOrcamentaria( orgcodigo )
    {
        // Alterando a imagem de mais para menos
        var objImg = $( '#org_img_' + orgcodigo );
        objImg.attr( 'src', '../imagens/menos.gif' );

        //Alterando onclick da imagem
        objImg.attr( 'onClick', 'javascript:esconderUnidadeOrcamentaria("' + orgcodigo + '")' );

        $.ajax( {
            type: "POST",
            url: "<?php echo '/pes/pes.php?modulo=principal/planoacao/cadastro&acao=A' ?>",
            data: { action: 'listarUO', orgcodigo: orgcodigo },
            success: function( html ) {

                html = '<td colspan="2" style=" padding: 0px;">' + html + "</td>";

                //Pegar Tr orgao
                objListaOrgao = $( '#org_lista_' + orgcodigo );
                objListaOrgao.empty().append( html ).fadeIn();
            }
        } );
    }

    /**
     * 
     */
    function esconderUnidadeOrcamentaria( orgcodigo )
    {
        // Alterando a imagem de mais para menos
        var objImg = $( '#org_img_' + orgcodigo );
        objImg.attr( 'src', '../imagens/mais.gif' );

        //Alterando onclick da imagem
        objImg.attr( 'onClick', 'javascript:carregarUnidadeOrcamentaria("' + orgcodigo + '")' );

        //Pegar Tr orgao
        objTrOrgao = $( '#org_lista_' + orgcodigo ).fadeOut();
    }
</script>
</table>
