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
    $table = array ('acaid' => ''
                    , 'acadsc' => ''
                    , 'acadetalhe' => ''
                    , 'tipid' => '4'
                    , 'temid' => ''
                    , 'cocid' => ''
                    , 'acastatus' => "'A'");
    populate($table , $_POST , 'acaid');
    
    if($_POST['acaid']){
        
        $sqlValuesUpdate = array();
        foreach($table as $columnName => $columnValue){
            $sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
        }
        
        $sql = "UPDATE painel.acao SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE acaid= {$table['acaid']} RETURNING acaid";
    } else {
        unset($table['acaid']);
        $sql = "INSERT INTO painel.acao(" . implode( ' , ' , array_keys($table)) .")
                VALUES (" . implode( ' , ' ,array_values($table) ) .")  RETURNING acaid";
    }
    
    $acaid = $db->pegaUm($sql);
    $db->commit();
    
    if($acaid){
        $_SESSION['acaid'] = $acaid;
        echo "<script>alert('Salvo com sucesso!')</script>";
    } else {
        echo "<script>alert('Não foi salvo!')</script>";
    }
}

if($_REQUEST['id']){
    $acaid = $_REQUEST['id'];
    $_SESSION['acaid'] = $acaid;
} else {
    $acaid = $_SESSION['acaid'];
}

include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

// Recuperando os dados.
if ($acaid) {
    
    ////tratando abas que serao ativadas apartir do cadastro de acões
    $abasCanceladas = array();
    //
    if (!$_SESSION['acaid']) {
        $abasCanceladas = array(
            0 => 4919 // 3603 //add value das abas
        );
    }

    $db->cria_aba($abacod_tela , $url , $parametros , $abasCanceladas);
    
    $sbatitulo = $db->pegaLinha("SELECT acadsc FROM painel.acao WHERE acaid = '" . $acaid . "'");

    $sql = "	SELECT sec.secid as ptrid,sec.secdsc as ptres 
								FROM painel.secretaria sec, painel.acaosecretaria ac
								WHERE sec.secid = ac.secid AND ac.acaid=$acaid";
    $desafiosAcao = $db->carregar($sql);

    $sqlUpdate = "	SELECT
                                                acadsc,
                                                tipid,
                                                temid,
                                                acadetalhe,
                                                indidprincipal,
                                                cocid
                                        FROM painel.acao where acaid = $acaid ";
    $arrDados = $db->pegaLinha($sqlUpdate);
    $_SESSION['temid'] = $arrDados[temid];
    $sbatitulo = $valor;
} else {
    $menu = array();
    $menu[0] = array("descricao" => "Pesquisa de ações estratégicas", "link"=> "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/listar_acao&acao=A");
    $menu[1] = array("descricao" => "Cadastro da ação estratégica", "link"=> "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/formulario_acao&acao=A");
    echo montarAbasArray($menu, "/painel/painel.php?modulo=sistema/tabelaapoio/cadastroacoes/formulario_acao&acao=A");
}

monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
    <input type='hidden' name='acaid' value='<?php echo $acaid ?>'>
    <input type='hidden' name = 'action' value='save'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Nome:</td>
            <td>
                <?php echo campo_texto('acadsc' , 'S' , 'S' , '' , 100 , 255 , '' , '' , '' , '' , '' , '' , '' , ($arrDados[acadsc]) ? ($arrDados[acadsc]) : ''); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Detalhe:</td>
            <td>
                <?php echo campo_textarea( 'acadetalhe', 'N', 'S', '', 64, 05, 1000, '', '', '', '', 'id="acadetalhe"', ($arrDados[acadetalhe]) ? ($arrDados[acadetalhe]) : '' ); ?>
            </td>
        </tr>
<!--        <tr>
            <?php
            $sql = "	SELECT 
				        tipnome AS descricao, 
				        tipid AS codigo
				FROM pde.ae_tipo
				ORDER BY tipid asc	 
			  ";
            ?>
            <td align='right' class="SubTituloDireita">Tipo:</td>
            <td><?php // echo $db->monta_combo('tipid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[tipid]) ? ($arrDados[tipid]) : ''); ?></td>
        </tr> -->
        <tr>
            <?php
            $sql = "	SELECT 
			        temdsc AS descricao, 
			        temid AS codigo
				FROM pto.tema
				WHERE temid <> 6
				ORDER BY temid asc	 
			  ";
            ?>
            <td align='right' class="SubTituloDireita">Tema:</td>
            <td><?= $db->monta_combo('temid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[temid]) ? ($arrDados[temid]) : ''); ?></td>
        </tr>
<?php
/*
//RETIRADO A PEDIDO DO MARCOS E MANOELA
        <tr>
            <?php
            $sql = "SELECT 
			        indid || ' - ' || substring(indnome,0,60) AS descricao, 
			        indid AS codigo
				FROM painel.indicador
				WHERE indstatus = 'A'
				ORDER BY indid asc	 
			  ";
            ?>
            <td align='right' class="SubTituloDireita">Indicador:</td>
            <td><?= $db->monta_combo('indidprincipal' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'N' , '' , '' , ($arrDados[indidprincipal]) ? ($arrDados[indidprincipal]) : ''); ?></td>
        </tr>
*/
?>
        <tr>
            <?php
            $sql = "	SELECT 
			        cocnome AS descricao, 
			        cocid AS codigo
				FROM pde.cockpit
				ORDER BY cocid asc	 
			  ";
            ?>
            <td align='right' class="SubTituloDireita">Cockpit:</td>
            <td><?= $db->monta_combo('cocid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'N' , '' , '' , ($arrDados[cocid]) ? ($arrDados[cocid]) : ''); ?></td>
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
        if (document.formulario.acadsc.value == '') {
            alert('O campo "Nome" é de preenchimento obrigatório!');
            document.formulario.acadsc.focus();
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