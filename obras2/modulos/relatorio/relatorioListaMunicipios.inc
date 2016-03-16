<?php

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
    case 'popup':
        exibirAvisoPendencias(null, $_REQUEST['muncod'], 'N', '', 'M');
        exit;
}
$_where = '';
$_where1 = '';
$_POST["pendencia"] = (empty($_POST["pendencia"])) ? 'todos' : $_POST["pendencia"];
$_POST["pendencia_r"] = (empty($_POST["pendencia_r"])) ? 'todos' : $_POST["pendencia_r"];
$_POST["esfera"] = (empty($_POST["esfera"])) ? 'M' : $_POST["esfera"];
if ($_POST) {

    $where = array();
    $where1 = array();

    if (!empty($_POST['mundesc'])) {
        $mundescTemp = removeAcentos($_POST['mundesc']);
        $where[] = "mun.muncod = '{$_POST['mundesc']}' OR UPPER(public.removeacento(mun.mundescricao)) ILIkE '%{$mundescTemp}%'";
        $where1[] = "o.muncod = '{$_POST['mundesc']}' OR UPPER(public.removeacento(o.mundescricao)) ILIkE '%{$mundescTemp}%'";
    }
    if (!empty($_POST['estuf'])) {
        $where[] = "mun.estuf = '{$_POST['estuf']}'";
        $where1[] = "o.estuf = '{$_POST['estuf']}'";
    }

    if (!empty($_POST['esfera'])) {
        $where1[] = "o.empesfera = '{$_POST['esfera']}'";
    }

    $whereString = implode(' AND ', $where);
    $_where = $whereString;
    $_where = (empty($_where)) ? '' : 'WHERE ' . $_where;

    $whereString = implode(' AND ', $where1);
    $_where1 = $whereString;
    $_where1 = (empty($_where1)) ? '' : 'WHERE ' . $_where1;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo('Relatório - Bloqueio Obras', '');
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<script type="text/javascript">
function carregarMunicipio(estuf) {
    
    var td = $('#td_municipio')
      , url = location.href;
    
    if (!estuf) {
        td.find('select option:first').attr('selected', true);
        td.find('select').attr('selected', true).attr('disabled', true);
        return false;
    }
    
    $.ajax({
        url: url,
        type: "POST",
        data: {ajax: "municipio", estuf: estuf},
        dataType: "html",
        async: false,
        beforeSend: function() {
            divCarregando();
            td.find("select option:first").attr("selected", true);
        },
        error: function() {
            divCarregado();
        },
        success: function(data) {
            td.html(data);
            divCarregado();
        }
    });
}

$(function(){

    $('#frmListaMun').submit(function(e){
          if($('select[name="estuf"]').val() == ''){
              alert('Selecione ao menos um estado.')
              return false;
          }
    });

    $(".row-header").hover(function(){
        $(this).css("background-color", "#c0c0c0");
    }, function(){
        $(this).css("background-color", "");
    });
    
    $("input[name='btn_salvar']").on("click", function(e){
        e.preventDefault();
        $("#xls").val("");
        $("#frmListaMun").submit();
    });
    
    $("input[name='btn_export_xls']").on("click", function(){
        $("#xls").val(1);
        $("#frmListaMun").submit();
    });
});
<?
    extract($_POST);
?>

</script>
<form name="frmListaMun" id="frmListaMun" method="post">
    <input type="hidden" name="xls" id="xls" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloEsquerda" style="width: 20%">Município</td>
            <td class="SubTituloEsquerda" style="width: 20%">UF</td>
            <td class="SubTituloEsquerda">Possui pendência PAR?</td>
            <td class="SubTituloEsquerda">Possui pendência de desembolso?</td>
            <td class="SubTituloEsquerda">Esfera</td>
        </tr>
        <tr>
            <td>
                <?php echo campo_texto('mundesc', 'N', 'S', '', 65, 60, '', '', 'left'); ?>
            </td>
            <td>
                <?php
                    $uf = new Estado();
                    $db->monta_combo('estuf', $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
                ?>
            </td>
            <td>
                <input type='radio' id='foto' name='pendencia' value='sim' <?= ( $_POST["pendencia"] == "sim" ? "checked='checked'" : "" ) ?>/> Sim
                <input type='radio' id='foto' name='pendencia' value='nao' <?= ( $_POST["pendencia"] == "nao" ? "checked='checked'" : "" ) ?>/> Não
                <input type='radio' id='foto' name='pendencia' value='todos' <?= ( $_POST["pendencia"] == "todos" ? "checked='checked'" : "" ) ?>/> Todas
            </td>
            <td>
                <input type='radio' id='pendencia_r' name='pendencia_r' value='sim' <?= ( $_POST["pendencia_r"] == "sim" ? "checked='checked'" : "" ) ?>/> Sim
                <input type='radio' id='pendencia_r' name='pendencia_r' value='nao' <?= ( $_POST["pendencia_r"] == "nao" ? "checked='checked'" : "" ) ?>/> Não
                <input type='radio' id='pendencia_r' name='pendencia_r' value='todos' <?= ( $_POST["pendencia_r"] == "todos" ? "checked='checked'" : "" ) ?>/> Todas
            </td>
            <td>
                <input type='radio' id='' name='esfera' value='M' <?= ( $_POST["esfera"] == "M" ? "checked='checked'" : "" ) ?>/> Municipal
                <input type='radio' id='' name='esfera' value='E' <?= ( $_POST["esfera"] == "E" ? "checked='checked'" : "" ) ?>/> Estadual
            </td>
        </tr>

        <tr>
            <td colspan="2" class="">
                <input type="submit" value="Pesquisar" name="btn_salvar"/>&nbsp;&nbsp;
                <input type="submit" value="Gerar Excel" name="btn_export_xls"/>
            </td>
        </tr>
    </table>
</form>
<?php
$acao = '';
if (!$_POST['xls']) {
    $cod = ($_POST["esfera"] == 'M') ? "muncod=\"'|| p.muncod ||'\"" : "estuf=\"'|| p.estuf ||'\"";
    $cod1 = "muncod=\"'|| p.muncod ||'\" esfera=\"".$_POST["esfera"]."\" estuf=\"'|| p.estuf ||'\"";

    $acao = "
            CASE WHEN p.pendencias > 0 OR p.pendenciasRecursos > 0 THEN
              '<center><img
                    align=\"absmiddle\"
                    src=\"/imagens/alterar.gif\"
                    style=\"cursor: pointer\"
                    class=\"detalhar_pendencias_obras\"
                    $cod
                    title=\"Detalhar\">'
              ELSE
              '<center><img
                    align=\"absmiddle\"
                    src=\"/imagens/alterar_01.gif\"
                    title=\"Detalhar\">'
              END || ' <img
                    align=\"absmiddle\"
                    src=\"/imagens/money_g.gif\"
                    style=\"cursor: pointer\"
                    class=\"controle_financeiro\"
                    $cod1
                    title=\"Ver Relatório de Controle Financeiro\"></center>' as acao,
            ";
}
$sqlPendenciasRecursos = "


SELECT COUNT(foo.obrid) FROM (SELECT m.muncod, so.obrid
	FROM obras2.obras so
	INNER JOIN obras2.empreendimento e ON e.empid = so.empid AND e.empstatus = 'A'
	INNER JOIN workflow.documento d ON d.docid = so.docid
	INNER JOIN  entidade.endereco ed on ed.endid = so.endid
	INNER JOIN  territorios.municipio m on m.muncod = ed.muncod
	JOIN (
			    SELECT
			      max( res_estado ) as res_estado,
			      obrid
			    FROM
			      (
				SELECT DISTINCT
				  CASE
				  WHEN rstdtsuperacao IS NOT NULL THEN 1
				  WHEN rstdtprevisaoregularizacao > now() THEN 2
				  WHEN rstdtprevisaoregularizacao < now() THEN 3
				  ELSE 2
				  END as res_estado,
				  obrid
				FROM
				  obras2.restricao
				WHERE
				  obrid IS NOT NULL AND
				  rstitem = 'R' AND
				  rststatus = 'A'
			      ) as  foo
			    GROUP BY
			      obrid
			  ) res ON res.obrid = so.obrid

                AND e.prfid IN (41)
                AND d.esdid IN (690, 691)
                AND res_estado != 1
                AND m.muncod = mun.muncod
                AND e.empesfera = '{$_POST['esfera']}'
                AND so.obridpai IS NULL
                AND so.obrstatus = 'A'

) as foo GROUP BY foo.muncod


";

$sql = "
    SELECT
          mun.muncod,
          mun.mundescricao,
          mun.estuf,
          (
                SELECT count(o.*)
                FROM obras2.obras o
                JOIN entidade.endereco ed ON ed.endid = o.endid AND ed.endstatus = 'A' AND ed.tpeid = 4
                WHERE o.obrstatus = 'A' AND o.obridpai IS NULL AND ed.muncod = mun.muncod
          ) as obras,
          COUNT(pen.*) as pendencias,
           ( $sqlPendenciasRecursos ) as pendenciasRecursos
          FROM territorios.municipio mun
          LEFT JOIN (
            SELECT o.obrid, o.preid, o.obrnome, o.estuf, o.muncod, o.mundescricao, o.pendencia, o.empesfera
              FROM obras2.vm_pendencia_obras o
              {$_where1}
          ) as pen ON pen.muncod = mun.muncod
    {$_where}
	GROUP BY mun.muncod, mun.mundescricao, pen.estuf, pen.mundescricao, pen.muncod, mun.estuf


    ";

$where = array();
if ($_POST['pendencia'] == 'sim') {
    $where[] = " p.pendencias > 0";
} else if ($_POST['pendencia'] == 'nao') {
    $where[] = " p.pendencias < 1";
}
if ($_POST['pendencia_r'] == 'sim') {
    $where[] = " p.pendenciasRecursos > 0";
} else if ($_POST['pendencia_r'] == 'nao') {
    $where[] = " (p.pendenciasRecursos < 1 OR p.pendenciasRecursos IS NULL)";
}
$where = implode(' AND ', $where);
$where = (empty($where)) ? '' : 'WHERE ' . $where;

$sql = "SELECT {$acao} p.* FROM ($sql) as p {$where}";
//ver(simec_htmlentities($sql), d);
$cabecalho = array('Ação', 'Código', 'Município', 'UF', 'Total de Obras', 'Obras com pendências PAR', 'Obras pendentes de desembolso');
if ($_POST['xls']) {
    $db->sql_to_xml_excel($sql,'relatorio_bloqueio_obras',$cabecalho);
}
if(!empty($_POST['estuf'])){
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N');
}
?>
<style type="text/css">
.row-header {
    border-right: 1px solid #c0c0c0; 
    border-bottom: 1px solid #c0c0c0; 
    border-left: 1px solid #ffffff;
    background-color: #f0f0f0;
    cursor: pointer;
}
.row-odd {
    background-color: #F7F7F7
}
</style>

<div id="popup-municipio"></div>

<script type="text/javascript">
    $(function(){
        $('.controle_financeiro').click(function(e){
            esfera = $(this).attr('esfera').toLowerCase();;
            muncod = $(this).attr('muncod');
            estuf = $(this).attr('estuf');

            $('#formulario').remove();

            var form = $('<form  name="formulario" id="formulario" action="/obras2/obras2.php?modulo=relatorio/relatorioControleFinanceiro&acao=A" method="post">' +
                '<input type="hidden" name="form" value="1" /> ' +
                '<input type="hidden" name="slMunicipio[]" value="'+muncod+'" />' +
                '<input type="hidden" name="slEstado[]" value="'+estuf+'" />' +
                '<input type="hidden" name="esfera" value="'+esfera+'" />' +
                '<input type="hidden" name="tipo_relatorio" value="visual" />' +
                '</form>');
            $('body').append(form);

            var formulario = document.formulario;
            var tipo_relatorio = 'visual';
            var janela = window.open('/obras2/obras2.php?modulo=relatorio/relatorioControleFinanceiro&acao=A', 'relatorio', 'width=1150,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');

            formulario.target = 'relatorio';
            janela.opener.notclose = janela;
            formulario.submit();

            e.preventDefault();
        });
    });
</script>