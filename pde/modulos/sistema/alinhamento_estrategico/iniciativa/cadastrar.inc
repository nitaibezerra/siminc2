<?php
function populate(&$table , $dataForm)
{
    foreach($table as $columnName => $columnValue){
        if(isset($dataForm[$columnName])){
            if(!empty($dataForm[$columnName])){
                $table[$columnName] =  "'" . addslashes($dataForm[$columnName]) . "'";
            } else {
                $table[$columnName] =  "NULL";
            }
        }
    }
}

//Salvando ou editando os dados.
if($_POST['action'] == 'save'){
    
    // Definindo os campos da tabela e populando os valores.
    $table = array ('iniid' => ''
                    , 'temid' => ''
                    , 'ininome' => '');
    populate($table , $_POST , 'iniid');
    if($_POST['iniid']){
        
        $sqlValuesUpdate = array();
        foreach($table as $columnName => $columnValue){
            $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
        }
        
        $sql = "UPDATE pde.ae_iniciativa SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE iniid = {$table['iniid']} RETURNING iniid";
    } else {
        unset($table['iniid']);
        $sql = "INSERT INTO pde.ae_iniciativa(" . implode( ' , ' , array_keys($table)) .")
                VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING iniid";
    }
    
    $id = $db->pegaUm($sql);
    $db->commit();
    
    if($id){
        $_SESSION['id'] = $id;
        echo "<script>alert('Salvo com sucesso!')</script>";
    } else {
        echo "<script>alert('Não foi salvo!')</script>";
    }
}

if($_REQUEST['id']){
    $id = $_REQUEST['id'];
    $_SESSION['id'] = $id;
} else {
    $id = $_SESSION['id'];
}

include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

// Recuperando os dados.
if ($id) {
    
    // Recuperando os dados.
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisar iniciativa", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/iniciativa/pesquisar&acao=A");
    $menu[1] = array("descricao" => "Cadastrar iniciativa", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/iniciativa/cadastrar&acao=A");
    echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/iniciativa/cadastrar&acao=A");
    
    $sql = "SELECT * FROM pde.ae_iniciativa where iniid = $id ";
    $_SESSION['temid'] = $arrDados[temid];
    $arrDados = $db->pegaLinha($sql);
} else {
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisar iniciativa", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/iniciativa/pesquisar&acao=A");
    $menu[1] = array("descricao" => "Cadastrar iniciativa", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/iniciativa/cadastrar&acao=A");
    echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/iniciativa/cadastrar&acao=A");
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
    <input type='hidden' name='iniid' value='<?php echo $id ?>'>
    <input type='hidden' name = 'action' value='save'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Iniciativa:</td>
            <td>
                <?php echo campo_texto('ininome' , 'S' , 'S' , '' , 80 , 500 , '' , '' , '' , '' , '' , '' , '' , ($arrDados[ininome]) ? ($arrDados[ininome]) : ''); ?>
            </td>
        </tr>
        <tr>
            <?php
            $sql = "	SELECT
			        temdsc AS descricao,
			        temid AS codigo
				FROM pto.tema
				ORDER BY temid asc
			  ";
            ?>
            <td align='right' class="SubTituloDireita">Tema:</td>
            <td><?= $db->monta_combo('temid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[temid]) ? ($arrDados[temid]) : ''); ?></td>
        </tr>
        <tr bgcolor="#cccccc">
            <td></td>
            <td>
                <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter();">
            </td>
        </tr>
    </table>
</form>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css"/>
<script src="./js/planacomorc.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
    function submeter() {
        if (document.formulario.ininome.value == '') {
            alert('O campo "Iniciativa" é de preenchimento obrigatório!');
            document.formulario.ininome.focus();
            return false;
        } else if(document.formulario.temid.value == '') {
            alert('O campo "Tema" é de preenchimento obrigatório!');
            document.formulario.temid.focus();
            return false;
        } else {
            document.formulario.submit();
        }
    }
</script>