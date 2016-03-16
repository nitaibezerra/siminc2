<?php
// monta cabeçalho

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
monta_titulo('Local de Trabalho', 'Informar Local de Trabalho');

if(isset($_POST['gravar'])){
    
    $_SESSION['entrada'] = $_POST['entrada'];
    direcionar("?modulo=principal/visitante/registrarEntradaVisitante&acao=A");

}else{

    if(isset($_POST['entrada'])){
        $entrada = $_POST['entrada'];
    }else{
        $entrada = $_SESSION['entrada'];
    }
}



?>


<form name="formulario" id="formulario" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td colspan="3"><?php echo $_SESSION['usunome']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Entrada:</td>
            <td colspan="3">
                <select style="width:300px;" name="entrada">
                    <option value=""> --Selecione-- </option>
                    <option value="<?php echo ENTRADA_EDIFICIO_SEDE_PRINCIPAL; ?>" <?php echo $entrada == ENTRADA_EDIFICIO_SEDE_PRINCIPAL ? 'selected' : '';?>> Edif&iacute;cio Sede - Principal </option>
                    <option value="<?php echo ENTRADA_EDIFICIO_SEDE_PRIVATIVA; ?>" <?php echo $entrada == ENTRADA_EDIFICIO_SEDE_PRIVATIVA ? 'selected' : '';?>> Edif&iacute;cio Sede - Privativa </option>
                    <option value="<?php echo ENTRADA_ANEXO_1; ?>" <?php echo $entrada == ENTRADA_ANEXO_1 ? 'selected' : '';?>> Anexo I - Principal </option>
                </select>
            </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC" colspan="4" align="center">
                <input type="submit" name="gravar" value="Selecionar" style="cursor:pointer;">
            </td>
        </tr>
    </table>
</form>