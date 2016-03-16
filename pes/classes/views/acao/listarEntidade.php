<table class="tabela_filha">
<?php foreach($this->lista as $entidade): ?>
    <tr class="linha_listagem">
        <!-- Ação-->
        <td class="td_acao" style="text-align:center;  width: 86px;">
            <img onclick="formularioAcaoInserir('<?php echo $entidade['entcodigo'] ?>')" border="0" align="absmiddle" style="cursor:pointer;" title="Inserir nova acao" src="../imagens/gif_inclui.gif">
        </td>
        <!-- Titulo-->
        <td class="td_nome" style="font-weight: bold;">
            <div scroll="no" style=" overflow:hidden; width:600px; height:13px;">
                &nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;
                &nbsp;
                <img align="absmiddle" src="../imagens/seta_filho.gif">
                &nbsp;&nbsp;
                <a href="#" onclick="javascript:listarAcao('<?php echo $entidade['entcodigo'] ?>');"> <?php echo $entidade['entnome'] ?></a>
            </div>
        </td>
    </tr>
<?php endforeach; ?>
</table>

<script lang="javascript">

   /**
    * 
    */
   function formularioAcaoInserir(entcodigo)
   {
       $.ajax({
            type: "POST",
            url: "<?php echo '/pes/pes.php?modulo=principal/planoacao/cadastro&acao=A' ?>",
            data: {action: 'formulario', entcodigo: entcodigo},
            success: function(html) {
                html = '<td colspan="2" style="text-align: center;">' + html + "</td>";
//                $('#form_entidade').scroll();
                //Pegar a linha para inserir o conteudo html.
                $('.container_formulario').empty().append(html).fadeIn();
            }
        });
   }

   /**
    * Formulario para nova entidade
    */
   function listarAcao(entcodigo)
   {
       var tidcodigo = $('#tidcodigo');
       
       if( tidcodigo.val() == '' ){
           msg(tidcodigo, 'Por favor, selecione uma despesa!');
           return false;
       }
       
       $.ajax({
            type: "POST",
            url: "<?php echo '/pes/pes.php?modulo=principal/planoacao/cadastro&acao=A' ?>",
            data: {action: 'listarAcao', entcodigo: entcodigo, tidcodigo : tidcodigo.val()},
            success: function(html) {
                html = '<td colspan="2" style="text-align: center;">' + html + "</td>";
                
                //Pegar a linha para inserir o conteudo html.
                $('.container_lista_acao').empty().append(html).fadeIn();
            }
        });
   }
</script>