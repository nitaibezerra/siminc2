<?php
//pega o parâmetro que indidica se a consulta será de entrada ou saída de equipamento
$tipo = $_REQUEST['tipo'];
?>

<html>
<head></head>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript">
//caso digite enter(13), chama função buscarEquipamento
function leituraCodigoBarras(e) {
    var code = e.keyCode ? e.keyCode : e.which;
        if(e.which==17 || e.which==74){
            e.preventDefault();
        }

    if(code == 13 && $('eqmnumetiqueta').getValue() != ''){
        executarScriptPai("buscarEquipamento('" + $('eqmnumetiqueta').getValue() + "','" + $('tipo').getValue() + "');");
        self.close();
    }
}
</script>


<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<body>
<form name="formularioSelecao" id="formularioSelecao" method="post" onsubmit='return false'>
    <table  class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
      <tr>
        <td align='right' class="SubTituloDireita" width='25%'>Etiqueta:</td>
        <td><?=campo_texto('eqmnumetiqueta','','','',30,50,'','','left','',0,'id="eqmnumetiqueta"','leituraCodigoBarras(event);');?></td>
      </tr>
      <tr>
        <td align='center' class="SubTituloEsquerda" colspan='2'>Passe o scanner no código de barras</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloCentroAzul" colspan='2'><img src='../imagens/barcode.png' /></td>
      </tr>
      <tr bgcolor="#C0C0C0">
        <td colspan='2' style="text-align:center;">
        <input type="hidden" name="requisicao" value="salvar" />
        </td>
      </tr>
    </table>
    <input type='hidden' id='tipo' name='tipo' value='<?=$tipo?>'>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("eqmnumetiqueta").focus();
      });
</script>
</body>
</html>