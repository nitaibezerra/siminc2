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
    $table = array ('mpneid' => ''
                    , 'tipid' => '1'
                    , 'temid' => ''
                    , 'mpnenome' => ''
                    , 'mpneordem' => ''
                    , 'mpneprazo' => '');
    populate($table , $_POST , 'mpneid');
    
    if($_POST['mpneid']){
        
        $sqlValuesUpdate = array();
        foreach($table as $columnName => $columnValue){
            $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
        }
        
        $sql = "UPDATE pde.ae_metapne SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE mpneid = {$table['mpneid']} RETURNING mpneid";
    } else {
        unset($table['mpneid']);
        $sql = "INSERT INTO pde.ae_metapne(" . implode( ' , ' , array_keys($table)) .")
                VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING mpneid";
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
    
    $sql = "SELECT * FROM pde.ae_metapne where mpneid = $id ";
    $arrDados = $db->pegaLinha($sql);
    $_SESSION['temid'] = $arrDados[temid];
} else {
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisar meta PNE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/pesquisar&acao=A");
    $menu[1] = array("descricao" => "Cadastrar meta PNE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar&acao=A");
    echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar&acao=A");
    $sql = "SELECT MAX(mpneordem) + 1 AS mpneordem FROM pde.ae_metapne";
    $arrDados = $db->pegaLinha($sql);
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>

<form method="POST"  name="formulario">
    <input type='hidden' name='mpneid' value='<?php echo $id ?>'>
    <input type='hidden' name = 'action' value='save'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Ordem:</td>
			<td>
                <?php echo campo_texto('mpneordem','S','S','','5','5','#####','','','','','','',($arrDados[mpneordem]) ? ($arrDados[mpneordem]) : '') ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Nome:</td>
			<td>
                <?php echo campo_textarea( 'mpnenome', 'S', 'S', '', 64, 5, 1000, '', '', '', '', 'id="mpnenome"', ($arrDados[mpnenome]) ? ($arrDados[mpnenome]) : '' ); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Prazo:</td>
            <td><?= campo_data('mpneprazo', 'S', 'S', '', 'S','','',($arrDados[mpneprazo]) ? ($arrDados[mpneprazo]) : ''); ?></td>
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
        if (document.formulario.mpneordem.value == '') {
            alert('O campo "Ordem" é de preenchimento obrigatório!');
            document.formulario.mpneordem.focus();
            return false;
        } else if (document.formulario.mpnenome.value == '') {
            alert('O campo "Nome" é de preenchimento obrigatório!');
            document.formulario.mpnenome.focus();
            return false;
        } else if (document.formulario.mpneprazo.value == '') {
            alert('O campo "Prazo" é de preenchimento obrigatório!');
            document.formulario.mpneprazo.focus();
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