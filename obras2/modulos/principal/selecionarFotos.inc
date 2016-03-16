<?php
/**
 * Arquivo que seleciona as fotos da galeria para o Extrato da obra
 * 
 * @author Fernando Araújo Bagno da SIlva
 * @since 11/02/2010
 * 
 */
monta_titulo('Galeria de Fotos', '');
?>
<html>
    <head>
        <title>SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação</title>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
        <script type="text/javascript">

            function salvaFotosSelecionadas() {

                var objForm = document.getElementsByName("fotos[]");
                var ids = [];
                var total = 0;
                var i;

                for (i = 0; i < objForm.length; i++) {

                    if (objForm[i].checked) {
                        ids.push(objForm[i].id);
                        total += 1;
                    }
                }
                if(total == 0){
                    alert("Selecione uma foto");
                }else{
                    window.opener.document.getElementById("numfotos").value = total;
                    window.opener.document.getElementById("fotoselecionada").value = ids.join(",");
                    self.close();
                }
            }

        </script>
        <script>jQuery.noConflict();</script>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <script type="text/javascript" src="../includes/entidades.js"></script>
        <script type="text/javascript" src="/includes/estouvivo.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    </head>
    <body>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

<?php
$strSQL = "SELECT 
    arq.arqnome, 
    arq.arqid, 
    arq.arqextensao, 
    arq.arqtipo, 
    arq.arqdescricao
FROM 
    public.arquivo arq
INNER JOIN 
    obras2.obras_arquivos oa ON (oa.arqid = arq.arqid)
WHERE 
    oa.obrid_1 = %d AND
    oa.oarstatus = 'A' AND
    (
        arq.arqtipo = 'image/jpeg' OR 
        arq.arqtipo = 'image/gif' OR 
        arq.arqtipo = 'image/png'
    ) 
ORDER BY 
    arq.arqid";
$stmt = sprintf($strSQL, (int) $_SESSION['obras2']['obrid']);
$fotosObras = $db->carregar($stmt);

if ($fotosObras) {

    echo '<tr>
        <td class="subtitulocentro">Selecionar</td>
        <td class="subtitulocentro">Foto</td>
     <tr>';

    for ($i = 0; $i < count($fotosObras); $i++) {

        $cor = ($i%2) ? '#f4f4f4' : '#E0E0E0';

        echo "<tr bgcolor='{$cor}'>
            <td align='center'>
              <input type='checkbox' name='fotos[]' id='{$fotosObras[$i]['arqid']}'/>
            </td>
            <td>
              <img 
              src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid={$fotosObras[$i]['arqid']}' 
              hspace='3'
              vspace='3'
              style='width:100px;
              height:100px; '
              />
              <br/>{$fotosObras[$i]['arqdescricao']}
            </td>
          <tr>";
    }
} else {
    echo '<tr><td align="center" style="color:#ee0000"> Não existem fotos cadastradas para esta obra. </td></tr>';
}
?>

            <tr bgcolor="#D0D0D0">
                <td colspan="2">
                    <input type="button" value="OK" onclick="salvaFotosSelecionadas();" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </body>
</html>