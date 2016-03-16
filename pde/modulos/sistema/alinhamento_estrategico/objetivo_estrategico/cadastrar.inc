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
    $table = array ('obeid' => ''
                    , 'temid' => ''
                    , 'obenome' => '');
    populate($table , $_POST , 'obeid');
    if($_POST['obeid']){
        
        $sqlValuesUpdate = array();
        foreach($table as $columnName => $columnValue){
            $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
        }
        
        $sql = "UPDATE pde.ae_objetivoestrategico SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE obeid = {$table['obeid']} RETURNING obeid";
    } else {
        unset($table['obeid']);
        $sql = "INSERT INTO pde.ae_objetivoestrategico(" . implode( ' , ' , array_keys($table)) .")
                VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING obeid";
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

    //tratando abas que serao ativadas apartir do cadastro de acões
    $abasCanceladas = array();
    $db->cria_aba($abacod_tela , $url , $parametros , $abasCanceladas);
    
    $sql = "SELECT * FROM pde.ae_objetivoestrategico where obeid = $id ";
    $arrDados = $db->pegaLinha($sql);
    $_SESSION['temid'] = $arrDados[temid];
} else {
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisar objetivo estratégico", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/pesquisar&acao=A");
    $menu[1] = array("descricao" => "Cadastrar objetivo estratégico", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/cadastrar&acao=A");
    echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/cadastrar&acao=A");
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
    <input type='hidden' name='obeid' value='<?php echo $id ?>'>
    <input type='hidden' name = 'action' value='save'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Objetivo estratégico:</td>
            <td>
                <?php echo campo_texto('obenome' , 'S' , 'S' , '' , 80 , 500 , '' , '' , '' , '' , '' , '' , '' , ($arrDados[obenome]) ? ($arrDados[obenome]) : ''); ?>
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
        if (document.formulario.obenome.value == '') {
            alert('O campo "Objetivo estratégico" é de preenchimento obrigatório!');
            document.formulario.obenome.focus();
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