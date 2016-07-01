<?php

if($_REQUEST['id']){
    $id = $_REQUEST['id'];
    $_SESSION['id'] = $id;
} else {
    $id = $_SESSION['id'];
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
        $table = array ('mppaid' => ''
                        , 'objid' => $id
                        , 'mppanome' => ''
                        , 'mppatipo' => '');
        populate($table , $_POST , 'mppaid');

        if($_POST['mppaid']){
            $sqlValuesUpdate = array();
            foreach($table as $columnName => $columnValue){
                $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
            }

            $sql = "UPDATE pde.ae_metappa SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE mppaid = {$table['mppaid']} RETURNING mppaid";
        } else {
            unset($table['mppaid']);
            $sql = "INSERT INTO pde.ae_metappa(" . implode( ' , ' , array_keys($table)) .")
                    VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING mppaid";
        }
        
        $idMetaPPA = $db->pegaUm($sql);
        $db->commit();

        if($idMetaPPA){
            echo "<script>alert('Salvo com sucesso!')</script>";
            $_POST['mppanome'] = '';
            $_REQUEST['mppanome'] = '';
            $_REQUEST['mppaid'] = '';
        } else {
            echo "<script>alert('Não foi salvo!')</script>";
        }
    } if( $_REQUEST['action'] == 'deletar' && isset($_REQUEST['mppaid']) && !empty($_REQUEST['mppaid']) ){
		//Verifica a existência de vinculos
		$sql = "SELECT SUM(total) as total
			FROM(
				SELECT count(0) as total FROM pde.ae_acaoxmetappa WHERE mppaid = {$_REQUEST['mppaid']}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_desafioxmetappa WHERE mppaid = {$_REQUEST['mppaid']}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_metapnexppa WHERE mppaid = {$_REQUEST['mppaid']}
			) AS foo";
		$valor = $db->pegaum($sql);
		if ($valor > 0){
			echo "<script>
			alert('A meta não pode ser excluída, pois existem vinculos!');		  
			</script>";
		} else {
			$sql = "DELETE FROM pde.ae_metappa WHERE mppaid = {$_REQUEST['mppaid']}";
			$db->executar($sql);
			$db->commit();
			echo "<script>
			alert('Meta excluída com sucesso!');
			</script>";
		}

        $_REQUEST['mppaid'] = '';
    }
}

include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

// Recuperando os dados.
$abasCanceladas = array();
$db->cria_aba($abacod_tela , $url , $parametros , $abasCanceladas);

if($_REQUEST['mppaid']){
    $sql = "SELECT * FROM pde.ae_metappa where mppaid = {$_REQUEST['mppaid']} ";
    $arrDados = $db->pegaLinha($sql);
} else {
    $arrDados = array();
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_ppa/cadastrar_meta_ppa&acao=A">
    <input type='hidden' name = 'action' value='save'>
    <input type='hidden' name = 'mppaid' value='<?php echo $arrDados['mppaid'] ?>'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Nome:</td>
			<td>
                <?php echo campo_textarea( 'mppanome', 'S', 'S', '', 64, 5, 1000, '', '', '', '', 'id="mppanome"', ($arrDados[mppanome]) ? ($arrDados[mppanome]) : $_REQUEST[mppanome] ); ?>
            </td>
        </tr>
		<tr>
            <?php
            $sql = "SELECT 'M' AS codigo, 'Metas PPA' AS descricao
					UNION ALL
					SELECT 'I' AS codigo, 'Iniciativas PPA' AS descricao";
            ?>
            <td align='right' class="SubTituloDireita">Tipo:</td>
            <td><?= $db->monta_combo('mppatipo' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[mppatipo]) ? ($arrDados[mppatipo]) : ''); ?></td>
        </tr>
        <tr bgcolor="#cccccc">
            <td></td>
            <td>
                <?php if($_REQUEST['mppaid']): ?>
                <input type="button" class="botao" name="btp" value="Cancelar" onclick="cancelar()">
                <?php else: ?>
                <input type="button" class="botao" name="btp" value="Pesquisar" onclick="pesquisar();">
                <?php endif ?>
                <input type="button" class="botao" name="btg" value="<?php echo ($_REQUEST['mppaid'])? 'Editar' : 'Gravar' ?> " onclick="submeter();">
        </tr>
    </table>
</form>

<?php 
if(!$_REQUEST['mppaid']){
	if(!empty($_POST['mppanome'])){
		$where = " WHERE objid = {$id} AND upper(mppanome) like upper('%$_POST[mppanome]%') ";
	} else {
		$where = "WHERE objid = {$id}";
	}

	$sql = "SELECT '<center>
											<a style=\"cursor:pointer;\" onclick=\"editar(\''||mppaid||'\');\">
													<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
											</a>
											<a  style=\"cursor:pointer;\" onclick=\"deletar(\''||mppaid||'\');\">
													<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
											</a>
									</center>' as acao, 
							mppanome
							FROM pde.ae_metappa
							$where
							ORDER BY mppanome";

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
        if (document.formulario.mppanome.value == '') {
            alert('O campo "Nome" é de preenchimento obrigatório!');
            document.formulario.mppanome.focus();
            return false;
        } else if(document.formulario.mppatipo.value == '') {
            alert('O campo "Tipo" é de preenchimento obrigatório!');
            document.formulario.mppatipo.focus();
            return false;
        } else {
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
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_ppa/cadastrar_meta_ppa&acao=A&';
    }
    function editar(id) {
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_ppa/cadastrar_meta_ppa&acao=A&mppaid='+id;
    
    }
    function deletar(id) {
        if(confirm("Você realmente deseja excluir este item?")) {
            window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_ppa/cadastrar_meta_ppa&acao=A&action=deletar&mppaid='+id;
        }
    }
</script>
