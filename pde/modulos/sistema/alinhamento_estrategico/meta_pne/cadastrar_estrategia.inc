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

if($_REQUEST['id']){
    $id = $_REQUEST['id'];
    $_SESSION['id'] = $id;
} else {
    $id = $_SESSION['id'];
}

//Salvando ou editando os dados.
if($_REQUEST['action']){
    if($_REQUEST['action'] == 'save'){

        // Definindo os campos da tabela e populando os valores.
        $table = array ('estid' => ''
                        , 'metid' => $id
                        , 'estnome' => ''
                        , 'estprazo' => ''
                        , 'sitid' => ''
                        , 'estordem' => '');
        populate($table , $_POST , 'estid');

        if($_POST['estid']){
            $sqlValuesUpdate = array();
            foreach($table as $columnName => $columnValue){
                $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
            }

            $sql = "UPDATE pde.ae_estrategia SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE estid = {$table['estid']} RETURNING estid";
        } else {
            unset($table['estid']);
            $sql = "INSERT INTO pde.ae_estrategia(" . implode( ' , ' , array_keys($table)) .")
                    VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING estid";
        }

        $idEstrategia = $db->pegaUm($sql);
        $db->commit();

        if($idEstrategia){
    //        $_SESSION['idEstrategia'] = $idEstrategia;
            echo "<script>alert('Salvo com sucesso!')</script>";
            $_POST['estnome'] = '';
            $_REQUEST['estnome'] = '';
            $_REQUEST['estid'] = '';
            $_REQUEST['estprazo'] = '';
            $_REQUEST['sitid'] = '';
            $_REQUEST['estordem'] = '';
        } else {
            echo "<script>alert('Não foi salvo!')</script>";
        }
    } if( $_REQUEST['action'] == 'deletar' && isset($_REQUEST['estid']) && !empty($_REQUEST['estid']) ){
        $sql = "DELETE FROM pde.ae_estrategia WHERE estid = {$_REQUEST['estid']}";
        if($db->executar($sql)){
            $db->commit();
            echo "<script>
                    alert('Excluído com sucesso!');		  ;
                  </script>";
        } else {
            echo "<script>
                    alert('Não pode excluir!');		  ;
                  </script>";
        }
        
        $_REQUEST['estid'] = '';
    }
}

include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

// Recuperando os dados.
$abasCanceladas = array();
$db->cria_aba($abacod_tela , $url , $parametros , $abasCanceladas);

if($_REQUEST['estid']){
    $sql = "SELECT * FROM pde.ae_estrategia where estid = {$_REQUEST['estid']} ";
    $arrDados = $db->pegaLinha($sql);
} else {
    $sql = "SELECT MAX(estordem) + 1 AS estordem FROM pde.ae_estrategia WHERE metid = $id";
    $arrDados = $db->pegaLinha($sql);
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<form method="POST"  name="formulario" action="estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar_estrategia&acao=A">
    <input type='hidden' name = 'action' value='save'>
    <input type='hidden' name = 'estid' value='<?php echo $arrDados['estid'] ?>'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Ordem:</td>
            <td>
                <?php echo campo_texto('estordem','S','S','','5','5','#####','','','','','','',($arrDados[estordem]) ? ($arrDados[estordem]) : '') ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Nome:</td>
			<td>
                <?php echo campo_textarea( 'estnome', 'S', 'S', '', 64, 5, 1000, '', '', '', '', 'id="estnome"', ($arrDados[estnome]) ? ($arrDados[estnome]) : ''); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Prazo:</td>
            <td><?= campo_data('estprazo', 'S', 'S', '', 'S','','',($arrDados[estprazo]) ? ($arrDados[estprazo]) : ''); ?></td>
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
        <tr bgcolor="#cccccc">
            <td></td>
            <td>
                <?php if($_REQUEST['estid']): ?>
                <input type="button" class="botao" name="btp" value="Cancelar" onclick="cancelar()">
                <?php else: ?>
                <input type="button" class="botao" name="btp" value="Pesquisar" onclick="pesquisar();">
                <?php endif ?>
                <input type="button" class="botao" name="btg" value="<?php echo ($_REQUEST['estid'])? 'Editar' : 'Gravar' ?> " onclick="submeter();">
        </tr>
    </table>
</form>

<?php 


if(!$_REQUEST['estid']){
    
if(!empty($_POST['estnome'])){
    $where = " WHERE metid = {$id} AND upper(estnome) like upper('%$_POST[estnome]%') ";
} else {
    $where = "WHERE metid = {$id}";
}
 
$sql = "SELECT '<center>
                                        <a style=\"cursor:pointer;\" onclick=\"editar(\''||ae.estid||'\');\">
                                                <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                                        </a>
                                        <a  style=\"cursor:pointer;\" onclick=\"deletar(\''||ae.estid||'\');\">
                                                <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                                        </a>
                                </center>' as acao,
                        ae.estordem,
                        ae.estnome,
                        to_char(ae.estprazo,'DD/MM/YYYY') AS prazo,
                        sit.sitdsc
                        FROM pde.ae_estrategia ae
                        INNER JOIN pde.ae_situacao sit ON sit.sitid = ae.sitid
                        $where
                        ORDER BY ae.estordem";

$cabecalho = array("Ação", "Ordem", "Título", "Prazo", "Situação");
$db->monta_lista($sql , $cabecalho,100,5,'N','95%','');
}

?>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css"/>
<script src="./js/planacomorc.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
    function submeter() {
        if (document.formulario.estordem.value == '') {
            alert('O campo "Ordem" é de preenchimento obrigatório!');
            document.formulario.estordem.focus();
            return false;
        }else if (document.formulario.estnome.value == '') {
            alert('O campo "Nome" é de preenchimento obrigatório!');
            document.formulario.estnome.focus();
            return false;
        }else if (document.formulario.estprazo.value == '') {
                alert('O campo "Prazo" é de preenchimento obrigatório!');
                document.formulario.estprazo.focus();
                return false;
        }else if (document.formulario.sitid.value == '') {
            alert('O campo "Situação" é de preenchimento obrigatório!');
            document.formulario.sitid.focus();
            return false;
        } else {
            document.formulario.action.value = 'save';
            document.formulario.submit();
        }
    }
    
    /**
     * pesquisar
     */
    function pesquisar() {
        document.formulario.action.value = '';
        document.formulario.submit();
    }
    
    function cancelar() {
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar_estrategia&acao=A&';
    }
    function editar(id) {
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar_estrategia&acao=A&estid='+id;
    
    }
    function deletar(id) {
        if(confirm("Você realmente deseja excluir este item?")) {
            window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar_estrategia&acao=A&action=deletar&estid='+id;
        }
    }
</script>