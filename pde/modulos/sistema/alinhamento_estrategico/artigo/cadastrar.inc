<?php

function populate(&$table , $dataForm)
{
    foreach($table as $columnName => $columnValue){
        if(isset($dataForm[$columnName])){
            
            if(empty($dataForm[$columnName])){
                $table[$columnName] =  "NULL";
            } else {
                $table[$columnName] =  "'" . addslashes($dataForm[$columnName]) . "'";
            }
        }
    }
}

//Salvando ou editando os dados.
if($_POST['action'] == 'save'){
    
    // Definindo os campos da tabela e populando os valores.
    $table = array ('artid' => ''
                    , 'artordem' => ''
                    , 'artnome' => ''
                    , 'temid' => ''
                    , 'sitid' => ''
                    , 'artprazo' => '');
    populate($table , $_POST , 'artid');
    
    if($_POST['artid']){
        
        $sqlValuesUpdate = array();
        foreach($table as $columnName => $columnValue){
            $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
        }
        
        $sql = "UPDATE pde.ae_artigo SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE artid = {$table['artid']} RETURNING artid";
    } else {
        unset($table['artid']);
        $sql = "INSERT INTO pde.ae_artigo(" . implode( ' , ' , array_keys($table)) .")
                VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING artid";
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
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisar artigo", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/pesquisar&acao=A");
    $menu[1] = array("descricao" => "Cadastrar artigo", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/cadastrar&acao=A");
    echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/cadastrar&acao=A");
    
    $sql = "SELECT * FROM pde.ae_artigo where artid = $id ";
    $arrDados = $db->pegaLinha($sql);
    $_SESSION['temid'] = $arrDados[temid];
} else {
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisar artigo", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/pesquisar&acao=A");
    $menu[1] = array("descricao" => "Cadastrar artigo", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/cadastrar&acao=A");
    echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/cadastrar&acao=A");
    $sql = "SELECT MAX(artordem) + 1 AS artordem FROM pde.ae_artigo";
    $arrDados = $db->pegaLinha($sql);
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>

<form method="POST"  name="formulario">
    <input type='hidden' name='artid' value='<?php echo $id ?>'>
    <input type='hidden' name = 'action' value='save'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Ordem:</td>
			<td>
                <?php echo campo_texto('artordem','S','S','','5','5','#####','','','','','','',($arrDados[artordem]) ? ($arrDados[artordem]) : '') ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Artigo:</td>
			<td>
                <?php echo campo_textarea( 'artnome', 'S', 'S', '', 64, 5, 500, '', '', '', '', 'id="artnome"', ($arrDados[artnome]) ? ($arrDados[artnome]) : '' ); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Prazo:</td>
            <td><?= campo_data('artprazo', 'S', 'S', '', 'S','','',($arrDados[artprazo]) ? ($arrDados[artprazo]) : ''); ?></td>
        </tr>
        <tr>
            <?php
            $sql = "SELECT
			        sitdsc AS descricao,
			        sitid AS codigo
				FROM pde.ae_situacao
				ORDER BY sitid asc
			  ";
            ?>
            <td align='right' class="SubTituloDireita">Situação:</td>
            <td><?= $db->monta_combo('sitid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[sitid]) ? ($arrDados[sitid]) : ''); ?></td>
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
        </tr>
    </table>
</form>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css"/>
<script src="./js/planacomorc.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
    function submeter() {
        if (document.formulario.artordem.value == '') {
            alert('O campo "Ordem" é de preenchimento obrigatório!');
            document.formulario.artordem.focus();
            return false;
        } else if (document.formulario.artnome.value == '') {
            alert('O campo "Artigo" é de preenchimento obrigatório!');
            document.formulario.artnome.focus();
            return false;
        } else if (document.formulario.artprazo.value == '') {
            alert('O campo "Prazo" é de preenchimento obrigatório!');
            document.formulario.artprazo.focus();
            return false;
        }else if (document.formulario.sitid.value == '') {
            alert('O campo "Situação" é de preenchimento obrigatório!');
            document.formulario.sitid.focus();
            return false;
        } else if(document.formulario.temid.value == '') {
            alert('O campo "Tema" é de preenchimento obrigatório!');
            document.formulario.temid.focus();
            return false;
        } else {
            document.formulario.submit();
        }
    }

    function resultado(ptrid) {
        var tabela = document.getElementById('orcamento');
        tabela.deleteRow(window.document.getElementById('ptrid_' + ptrid).rowIndex);
    }
</script>