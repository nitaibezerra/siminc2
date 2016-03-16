<table class="tabela_filha">
<?php foreach($this->lista as $entidade): ?>
    <tr class="linha_listagem">
        <!-- Titulo-->
        <td class="td_nome" style="font-weight: bold;">
            <div scroll="no" style=" overflow:hidden; width:600px; height:13px;">
                &nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;
                &nbsp;
                <img align="absmiddle" src="../imagens/seta_filho.gif">
                &nbsp;&nbsp;
                <a href="#" onclick="javascript:selecionarEntidade('<?php echo $entidade['entcodigo'] ?>');"> <?php echo $entidade['entnome'] ?></a>
            </div>
        </td>
    </tr>
<?php endforeach; ?>
</table>

<script lang="javascript">

   /**
    * Seleciona a entidade para marcar como ultimo acesso.
    *
    * @param {integer} entcodigo
    * @returns void
    *
    * @since 07/06/2013
    * @author Ruy Junior Ferreira <ruy.silva@mec.gov.br>
    */
   function selecionarEntidade(entcodigo)
   {
       url = window.location.href;

       $.ajax({
            type: "POST",
            url: url,
            data: { controller : 'Usuario' , action: 'selecionarEntidade', entcodigo: entcodigo},
            success: function(html) {
                window.location.reload();
            }
        });
   }
</script>