<table class="tabela_filha">
<?php foreach($this->lista as $entidade): ?>
<tr class="linha_listagem">
    <!-- Ação-->
    <td class="td_acao" style="text-align:center;  width: 86px;">
        <img onclick="formularioEntidadeEditar('<?php echo $entidade['entcodigo'] ?>')" style="cursor:pointer;" align="absmiddle" title="Editar Entidade" style="border: 0;" src="../imagens/alterar.gif">
        <!--<img onclick="formularioEntidadeInserir('<?php echo $this->uorcodigo ?>')" border="0" align="absmiddle" style="cursor:pointer;" title="Cadastrar Subtividade" src="../imagens/gif_inclui.gif">--> 
        <img align="absmiddle" src="../imagens/excluir_01.gif" title="Excluir Entidade (Não pode excluir esta Entidade)">
    </td>
    <!-- Titulo-->
    <td class="td_nome" style="font-weight: bold;">
        <div scroll="no" style=" overflow:hidden; width:600px; height:13px;">
            &nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;
            &nbsp;
            <img align="absmiddle" src="../imagens/seta_filho.gif">
            &nbsp;&nbsp;
            <!-- <img src="../imagens/star.gif" border="0" align="top" title="Estratégica"/> --> 
            <!--<span style="margin: 0 5px 0 0; "><?php echo $n ?></span>-->
            <!--<a style="" onmouseout="" onmouseover="window.SuperTitleOn(this, '&lt;b&gt;' + this.innerHTML + '&lt;/b&gt;')" href="?modulo=principal/atividade_/subatividades&amp;acao=A&amp;atiid=71628" id="link71628">-->
            <?php echo $entidade['entnome'] ?>       
            <!--</a>-->
        </div>
    </td>
</tr>
<?php endforeach; ?>
</table>

<script lang="javascript">

   /**
    * Formulario para nova entidade
    */
   function formularioEntidadeEditar(entcodigo)
   {
       $.ajax({
            type: "POST",
            url: "<?php echo '/pes/pes.php?modulo=principal/entidade/cadastro&acao=A' ?>",
            data: {action: 'formulario', entcodigo: entcodigo},
            success: function(html) {
                html = '<td colspan="2" style="text-align: center;">' + html + "</td>";
//                $('#form_entidade').scroll();
                //Pegar a linha para inserir o conteudo html.
                $('.container_formulario').empty().append(html).fadeIn();
            }
        });
   }
</script>