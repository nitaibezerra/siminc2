<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Cache-Control" content="no-cache">

<?php

if (isset($_REQUEST['form']) == true && !$_REQUEST['carregar']) {
    switch ($_REQUEST['pesquisa']) {
        case '1':
            include "relatorio_matriculas_resultado.inc";
            exit;
        case '2':
            include "relatorio_matriculas_resultado.inc";
            exit;
    }
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br>";
$db->cria_aba($abacod_tela, $url, '');
$titulo_modulo = "Relatório Quantitativo de Matriculas";
monta_titulo($titulo_modulo, 'Selecione os filtros desejados');
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

    function exibeRelatorioProcesso(tipoXLS) {
        document.getElementById('req').value = tipoXLS;
        var formulario = document.formulario;

        selectAllOptions(document.getElementById('estuf'));
        selectAllOptions(document.getElementById('polid'));
        selectAllOptions(document.getElementById('nucid'));

        if (document.getElementById('req').value != 'geraxls') {
            formulario.target = 'matricula';
            var janela = window.open('?modulo=relatorio/relatorio_matriculas_resultado&acao=A', 'matricula', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.submit();
            janela.focus();
        } else {
            formulario.target = '';
            formulario.submit();
        }
    }

    /* Função para substituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }



    /**
     * Alterar visibilidade de um bloco.
     * 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    /**
     * Alterar visibilidade de um campo.
     * 
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
</script>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript">

    $(document).ready(function()
    {
//	jQuery('[name="status"]').click(function(){
//		if( jQuery(this).val() == 'I' ){
//			jQuery('#tr_justificativa').show();
//			jQuery('#tr_caejustificativainativacao').show();
//		}else{
//			jQuery('#caejustificativainativacao').val('');
//			jQuery('#tr_caejustificativainativacao').hide();
//			jQuery('#tr_justificativa').hide();
//			jQuery('[name="justificativaD"]').attr('checked',false);
//			jQuery('[name="justificativaF"]').attr('checked',false);
//			jQuery('[name="justificativaO"]').attr('checked',false);
//		}
//	});
    });

</script>

<form action="" method="post" name="formulario" id="filtro">

    <input type="hidden" name="form" value="1"/>
    <input type="hidden" id="req" name="req" value=""/>	
    <input type="hidden" name="pesquisa" value="1"/>
    <input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
    <input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
    <input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
    <input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<?php

$arrVisivel = array("descricao");
$arrOrdem = array("descricao");

// Estado
$stSql = " SELECT
							estuf AS codigo,
							estuf AS descricao
						FROM 
							territorios.estado";
mostrarComboPopup('Estado:', 'estuf', $stSql, '', 'Selecione o(s) Estado(s)', null, null, null, null, $arrVisivel, $arrOrdem);
?>
        <tr>
            <td class="SubTituloDireita">Município:</td>
            <td><? echo campo_texto('mundescricao', "N", "S", "", 37, 255, "", "", '', '', 0, 'id="mundescricao"'); ?></td>
        </tr>
<?php

// Escola
$sql = "SELECT
			ent.entid as codigo,
			ent.entnome as descricao
		FROM entidade.entidade ent
		INNER JOIN projovemcampo.turma tur ON tur.entid = ent.entid";
// VER($sql);
mostrarComboPopup('Escola:', 'entid', $sql, $stSqlCarregados, 'Selecione o(s) Nucleo(s)', null, null, null, null, $arrVisivel, $arrOrdem);
?>
        <tr>
            <td class="SubTituloDireita">Esfera:</td>
<?
$arExp = array(
    array("codigo" => 'T',
        "descricao" => 'Todos'),
    array("codigo" => 'M',
        "descricao" => 'Municipal'),
    array("codigo" => 'E',
        "descricao" => 'Estadual')
);
?>
            <td><?= $db->monta_combo("esfera", $arExp, 'S', '', '', '', '', '', 'N', 'esfera', '', '', 'Esfera'); ?></td>
        </tr>
        
        <tr>
            <td class="SubTituloDireita">Expandir Relatório:</td>
<?
$arExp = array(
    array("codigo" => 'true',
        "descricao" => 'Sim'),
    array("codigo" => '',
        "descricao" => 'Não')
);
?>
            <td><?= $db->monta_combo("expandir", $arExp, 'S', '', '', '', '', '', 'N', 'expandir', '', '', 'Expandir'); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Status:</td>
            <td>
                <input type="radio" name="status" value="A" <?= ($_POST['status'] == 'A' ? 'checked' : '') ?>/> Ativo
                <input type="radio" name="status" value="I" <?= ($_POST['status'] == 'I' ? 'checked' : '') ?>/> Inativo - outros
                <input type="radio" name="status" value="D" <?= ($_POST['status'] == 'D' ? 'checked' : '') ?>/> Inativo - desistencia
                <input type="radio" name="status" value=" " <?= ($_POST['status'] == ' ' ? 'checked' : '') ?>/> Todos
            </td>
        </tr>
<!--		<tr id="tr_justificativa" style="display:none">-->
<!--			<td class="SubTituloDireita">Tipos:</td>-->
<!--			<td>-->
<!--				<input type="checkbox" name="justificativaF" value="F" <?= ($_POST['justificativaF'] != '' ? 'checked' : '') ?>/> Frequência-->
<!--				<input type="checkbox" name="justificativaO" value="O" <?= ($_POST['justificativaO'] != '' ? 'checked' : '') ?>/> Outros-->
        <!--			</td>-->
        <!--		</tr>-->
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                <input type="button" value="Visualizar"    onclick="exibeRelatorioProcesso('');" style="cursor: pointer;"/>
                <input type="button" value="VisualizarXLS" onclick="exibeRelatorioProcesso('geraxls');">
            </td>
        </tr>
    </table>
</form>
