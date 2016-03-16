<?php

if($_REQUEST['id']){
    $id = $_REQUEST['id'];
    $_SESSION['id'] = $id;
} else {
    $id = $_SESSION['id'];
}

if(isset($_SESSION['desid'])){
    unset($_SESSION['desid']);
}

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
if($_REQUEST['action']){
    if($_REQUEST['action'] == 'save'){

        // Definindo os campos da tabela e populando os valores.
        $table = array ('desid' => ''
                        , 'opeid' => $id
                        , 'desnome' => '');
        populate($table , $_POST , 'desid');

        if($_POST['desid']){
            $sqlValuesUpdate = array();
            foreach($table as $columnName => $columnValue){
                $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
            }

            $sql = "UPDATE pde.ae_desafio SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE desid = {$table['desid']} RETURNING desid";
        } else {
            unset($table['desid']);
            $sql = "INSERT INTO pde.ae_desafio(" . implode( ' , ' , array_keys($table)) .")
                    VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING desid";
        }
        
        $idMetaPPA = $db->pegaUm($sql);
        $db->commit();

        if($idMetaPPA){
            echo "<script>alert('Salvo com sucesso!')</script>";
            $_POST['desnome'] = '';
            $_REQUEST['desnome'] = '';
            $_REQUEST['desid'] = '';
        } else {
            echo "<script>alert('Não foi salvo!')</script>";
        }
    } if( $_REQUEST['action'] == 'deletar' && isset($_REQUEST['desid']) && !empty($_REQUEST['desid']) ){
		//Verifica a existência de vinculos
		$sql = "SELECT SUM(total) as total
			FROM(
				SELECT count(0) as total FROM pde.ae_desafioxmetapne WHERE desid = {$_REQUEST['desid']}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_desafioxmetappa WHERE desid = {$_REQUEST['desid']}
			) AS foo";
		$valor = $db->pegaum($sql);
		if ($valor > 0){
			echo "<script>
			alert('O desafio não pode ser excluído, pois existem metas vinculadas!');		  
			</script>";
		} else {
			$sql = "DELETE FROM pde.ae_desafio WHERE desid = {$_REQUEST['desid']}";
			$db->executar($sql);
			$db->commit();
			echo "<script>
			alert('Desafio excluído com sucesso!');
			</script>";
		}
        $_REQUEST['desid'] = '';
    }
}

include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

if($_REQUEST['desid']){
    $sql = "SELECT * FROM pde.ae_desafio where desid = {$_REQUEST['desid']} ";
    $arrDados = $db->pegaLinha($sql);
    $_SESSION['desid'] = $_REQUEST['desid'];
} else {
    $arrDados = array();
}

if($_SESSION['desid']){
    //tratando abas que serao ativadas apartir do cadastro de acões
    $abasCanceladas = array();
    $db->cria_aba($abacod_tela , $url , $parametros , $abasCanceladas);
    
} else {
    // Recuperando os dados.
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisar objetivo PE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/pesquisar&acao=A");
    $menu[1] = array("descricao" => "Cadastrar objetivo PE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar&acao=A");
    $menu[2] = array("descricao" => "Cadastrar desafio", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar_desafio&acao=A");
    echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar_desafio&acao=A");
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar_desafio&acao=A">
    <input type='hidden' name = 'action' value='save'>
    <input type='hidden' name = 'desid' value='<?php echo $arrDados['desid'] ?>'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Nome:</td>
            <td>
                <?php echo campo_texto('desnome' , 'S' , 'S' , '' , 80 , '500' , '' , '' , '' , '' , '' , '' , '' , ($arrDados[desnome]) ? ($arrDados[desnome]) : $_REQUEST[desnome]); ?>
            </td>
        </tr>
        <tr bgcolor="#cccccc">
            <td></td>
            <td>
                <?php if($_REQUEST['desid']): ?>
                <input type="button" class="botao" name="btp" value="Cancelar" onclick="cancelar()">
                <?php else: ?>
                <input type="button" class="botao" name="btp" value="Pesquisar" onclick="pesquisar();">
                <?php endif ?>
                <input type="button" class="botao" name="btg" value="<?php echo ($_REQUEST['desid'])? 'Editar' : 'Gravar' ?> " onclick="submeter();">
        </tr>
    </table>
</form>

<?php 

    if(!$_REQUEST['desid']){
        if(!empty($_POST['desnome'])){
            $where = " WHERE opeid = {$id} AND upper(desnome) like upper('%$_POST[desnome]%') ";
        } else {
            $where = "WHERE opeid = {$id}";
        }

        $sql = "SELECT '<center>
                                                <a style=\"cursor:pointer;\" onclick=\"editar(\''||desid||'\');\">
                                                        <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                                                </a>
                                                <a  style=\"cursor:pointer;\" onclick=\"deletar(\''||desid||'\');\">
                                                        <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                                                </a>
                                        </center>' as acao, 
                                desnome
                                FROM pde.ae_desafio
                                $where
                                ORDER BY desnome";

        $cabecalho = array("Ação","Título");
        $db->monta_lista($sql , $cabecalho,100,5,'N','95%','');
    }
?>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css"/>
<script src="./js/planacomorc.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
    function submeter() {
        if (document.formulario.desnome.value == '') {
            alert('O campo "Nome" é de preenchimento obrigatório!');
            document.formulario.desnome.focus();
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
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar_desafio&acao=A&';
    }
    function editar(id) {
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar_desafio&acao=A&desid='+id;
    
    }
    function deletar(id) {
        if(confirm("Você realmente deseja excluir este item?")) {
            window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar_desafio&acao=A&action=deletar&desid='+id;
        }
    }
</script>
