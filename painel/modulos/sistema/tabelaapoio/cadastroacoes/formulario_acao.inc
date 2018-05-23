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
	, 'mapid' => ''
	, 'dirid' => ''
	, 'acaorcamento' => ''
	, 'acastatus' => "'A'"
	, 'acaorcamentoatual' => ''
	, 'acadescontigenciamento' => ''
	, 'acarecomposicao' => ''
	, 'acacredito' => '');
	populate($table , $_POST , 'acaid');

	if($_POST['acaid']){

		$sqlValuesUpdate = array();
		foreach($table as $columnName => $columnValue){

			$orcamentos_vals = array(
				'acaorcamento',
				'acaorcamentoatual',
				'acadescontigenciamento',
				'acarecomposicao',
				'acacredito'
			);

			$columnValue = in_array($columnName, $orcamentos_vals) ? str_replace(['.', ','], ['', '.'], $columnValue) : $columnValue;

			$sqlValuesUpdate[] = "{$columnName} = {$columnValue}";
		}

		$sql = "UPDATE painel.acao SET " . implode( ' , ' , $sqlValuesUpdate) ." WHERE acaid= {$table['acaid']} RETURNING acaid";
	} else {
		unset($table['acaid']);

		$orcamentos_vals = array(
			'acaorcamento',
			'acaorcamentoatual',
			'acadescontigenciamento',
			'acarecomposicao',
			'acacredito'
		);

		foreach ($orcamentos_vals as $vals){
			$table[$vals] = str_replace(['.', ','], ['', '.'], $table[$vals]);
		}
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
                                                cocid,
                                                mapid,
                                                dirid,
                                                acaorcamento,
                                                acaorcamentoatual,
                                                acadescontigenciamento,
                                                acarecomposicao,
                                                acacredito
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
				<?php echo campo_textarea( 'acadetalhe', 'N', 'S', '', 64, 05, 100000, '', '', '', '', 'id="acadetalhe"', ($arrDados[acadetalhe]) ? ($arrDados[acadetalhe]) : '' ); ?>
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
				ORDER BY temdsc asc
			  ";
			?>
            <td align='right' class="SubTituloDireita">Eixo:</td>
            <td><?= $db->monta_combo('temid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[temid]) ? ($arrDados[temid]) : ''); ?></td>
        </tr>
        <tr>
			<?php
			$sql = "	SELECT
			        dirdsc AS descricao,
			        dirid AS codigo
				FROM painel.diretriz
				ORDER BY dirdsc asc
			  ";
			?>
            <td align='right' class="SubTituloDireita">Diretriz:</td>
            <td><?= $db->monta_combo('dirid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[dirid]) ? ($arrDados[dirid]) : ''); ?></td>
        </tr>
        <tr>
			<?php
			$sql = "SELECT
			        mapdsc AS descricao,
			        mapid AS codigo
				FROM mapa.mapa
				ORDER BY mapdsc asc
			  ";
			?>
            <td align='right' class="SubTituloDireita">Fonte do Recurso:</td>
            <td><?= $db->monta_combo('mapid' , $sql , 'S' , 'Selecione...' , '' , '' , '' , '' , 'S' , '' , '' , ($arrDados[mapid]) ? ($arrDados[mapid]) : ''); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Orçamento da ação:</td>
            <td><?=campo_texto('acaorcamento', "N", "S", "Orçamento (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="acaorcamento"', '', mascaraglobal($arrDados[acaorcamento],"###.###.###,##") ); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Orçamento atual:</td>
            <td><?=campo_texto('acaorcamentoatual', "N", "S", "Orçamento atual (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="acaorcamentoatual"', '', mascaraglobal($arrDados[acaorcamentoatual],"###.###.###,##") ); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Descontigenciamento:</td>
            <td><?=campo_texto('acadescontigenciamento', "N", "S", "Descontigenciamento (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="acadescontigenciamento"', '', mascaraglobal($arrDados[acadescontigenciamento],"###.###.###,##") ); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Recomposição PLOA:</td>
            <td><?=campo_texto('acarecomposicao', "N", "S", "Recomposição PLOA (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="acarecomposicao"', '', mascaraglobal($arrDados[acarecomposicao],"###.###.###,##") ); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Crédito:</td>
            <td><?=campo_texto('acacredito', "N", "S", "Crédito (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="acacredito"', '', mascaraglobal($arrDados[acacredito],"###.###.###,##") ); ?></td>
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