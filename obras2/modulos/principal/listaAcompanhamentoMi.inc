<?php
switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
    case 'delete':

        $amiid = $_GET['amiid'];
        $acompanhamento = new Acompanhamento_Mi($amiid);

        $empresaMI = new EmpresaMi();
        if($empresaMI->verificaAcesso($acompanhamento->obrid)){
            $acompanhamento->amistatus = 'I';
            $acompanhamento->salvar();
            $acompanhamento->commit();
            $retorno = array(
                'exclusao' => 'ok',
                'amiid' => $amiid
            );
        } else {
            $retorno = array(
                'exclusao' => 'erro',
                'amiid' => $amiid
            );
        }

        die(simec_json_encode($retorno));
}


extract($_POST);
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
monta_titulo('Acompanhamento MI', 'Filtre a obra');
?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">
    function carregarMunicipio(estuf) {

        var td = $("#td_municipio")
            , url = location.href;

        if (!estuf) {
            td.find("select option:first").attr("selected", true);
            td.find("select").attr("selected", true).attr("disabled", true);
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

        $("input[name='btn_salvar']").on("click", function(){
            $("#frmAcompanhamentoMI").submit();
        });

        $("input[name='btn_export_xls']").on("click", function(){
            $("#xls").val(1);
            $("#frmAcompanhamentoMI").submit();
        });
    })
</script>

<form method="post" name="formListaObra" id="formListaObra">
    <input type="hidden" name="req" id="req" value="">
</form>

<form name="frmAcompanhamentoMI" id="frmAcompanhamentoMI" method="post">
    <input type="hidden" name="xls" id="xls" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="35%">Nome da Obra/ID</td>
            <td>
                <?php echo campo_texto('obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="35%">Empresa</td>
            <td>
                <?php
                $valEmi = (!empty($_POST['emiid'])) ? $_POST['emiid'] : '';
                $empresami = new EmpresaMi();
                $sql = $empresami->listaCombo(false);
                //                                $sql = $empresami->listaComboComEstuf();
                $db->monta_combo("emiid", $sql, 'S', "Selecione...", "", '', '', '', 'N', $valEmi);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">UF</td>
            <td>
                <?php
                $uf = new Estado();
                $db->monta_combo('estuf', $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Município</td>
            <td id="td_municipio">
                <?php
                if ($estuf) {
                    $municipio = new Municipio();
                    $dado = $municipio->listaCombo(array('estuf' => $estuf));
                    $habMun = 'S';
                } else {
                    $dado = array();
                    $habMun = 'N';
                }
                $habMun = ($disable == 'N' ? $disable : $habMun);
                echo $db->monta_combo('muncod', $dado, $habMun, 'Selecione...', '', '', '', 200, 'N', 'muncod');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Execuçao:</td>
            <td>
                <input type="radio" name="amiobraexecucao" id="" value="S" <?= ( $_POST["amiobraexecucao"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="amiobraexecucao" id="" value="N" <?= ( $_POST["amiobraexecucao"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                <input type="radio" name="amiobraexecucao" id="" value="C"  <?= ( $_POST["amiobraexecucao"] == "" ? "checked='checked'" : "" ) ?> /> Concluída
                <input type="radio" name="amiobraexecucao" id="" value=""  <?= ( $_POST["amiobraexecucao"] == "" ? "checked='checked'" : "" ) ?> /> Todas
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
            <td>
                <input type="radio" name="foto" id="" value="S" <?= ( $_POST["foto"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="foto" id="" value="N" <?= ( $_POST["foto"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                <input type="radio" name="foto" id="" value=""  <?= ( $_POST["foto"] == "" ? "checked='checked'" : "" ) ?> /> Todas
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Possui anexos:</td>
            <td>
                <input type="radio" name="anexo" id="" value="S" <?= ( $_POST["anexo"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="anexo" id="" value="N" <?= ( $_POST["anexo"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                <input type="radio" name="anexo" id="" value=""  <?= ( $_POST["anexo"] == "" ? "checked='checked'" : "" ) ?> /> Todas
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Problema sinalizado:</td>
            <td>
                <input type="radio" name="amiproblema" id="" value="t" <?= ( $_POST["amiproblema"] == "t" ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="amiproblema" id="" value="f" <?= ( $_POST["amiproblema"] == "f" ? "checked='checked'" : "" ) ?>/> Não
                <input type="radio" name="amiproblema" id="" value=""  <?= ( $_POST["amiproblema"] == "" ? "checked='checked'" : "" ) ?> /> Todas
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Liberada para edição:</td>
            <td>
                <input type="radio" name="amiedicao" id="" value="t" <?= ( $_POST["amiedicao"] == "t" ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="amiedicao" id="" value="f" <?= ( $_POST["amiedicao"] == "f" ? "checked='checked'" : "" ) ?>/> Não
                <input type="radio" name="amiedicao" id="" value=""  <?= ( $_POST["amiedicao"] == "" ? "checked='checked'" : "" ) ?> /> Todas
            </td>
        </tr>

        <tr>
            <td bgcolor="#c0c0c0" colspan="2" align="center">&nbsp;</td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <input type="submit" value="Filtrar" name="btn_salvar">&nbsp;&nbsp;
                <input type="submit" value="Exportar XLS" name="btn_export_xls">
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0" colspan="2" align="center">&nbsp;</td>
        </tr>
    </table>
</form>
<?php

$param = $_POST;

if ($param['xls']) {
} else {


    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))){
        $excluir = "<img
                            align=\"absmiddle\"
                            src=\"/imagens/excluir.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: excluirAcompanhamento('|| ami.amiid ||');\"
                            title=\"Alterar Obra\" />";
    }

    $acao = "
               ' <center><div style=\"width:100px\">
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/consultar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: verAcompanhamento('|| ami.amiid ||');\"
                            title=\"Alterar Obra\" />'
                    || CASE WHEN ami.amiedicao = 't' THEN
                        ' <img
                                align=\"absmiddle\"
                                src=\"/imagens/editar_nome.gif\"
                                style=\"cursor: pointer\"
                                onclick=\"javascript: alterarAcompanhamento('|| ami.amiid ||');\"
                                title=\"Alterar Obra\" /> '
                      ELSE ' '
                    END ||
                    '$excluir
                </div></center>'

     as acao, ";
}


$where = array();

if (possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_ADMINISTRATIVO))) {
    $where[] = "
                    emi.emiid IN (SELECT urs.emiid FROM obras2.usuarioresponsabilidade urs
                                        WHERE  urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'
                                        GROUP BY urs.emiid)
                   ";
}


if($param['emiid']){
    $where['emiid'] = "emi.emiid IN ({$param['emiid']})";
}
if($param['estuf']){
    $where['estuf'] = "mun.estuf IN ('{$param['estuf']}')";
}
if($param['muncod']){
    $where['muncod'] = "mun.muncod IN ('{$param['muncod']}')";
}
if($param['amiobraexecucao']){
    $where['amiobraexecucao'] = "ami.amiobraexecucao IN ('{$param['amiobraexecucao']}')";
}
if($param['foto']){
    $c = ($param['foto'] == 'S') ? '>' : '<=';
    $where['foto'] = "
        (SELECT COUNT(*)
            FROM obras2.acompanhamento_mi_arquivos am
            JOIN arquivo a ON a.arqid = am.arqid
            WHERE am.amiid = ami.amiid AND am.amastatus = 'A' AND (a.arqtipo ILIKE '%jpg%' OR a.arqtipo ILIKE '%png%' OR a.arqtipo ILIKE '%bmp%' OR a.arqtipo ILIKE '%image%')) $c 0
    ";
}
if($param['anexo']){
    $c = ($param['anexo'] == 'S') ? '>' : '<=';
    $where['anexo'] = "
         (SELECT COUNT(*) FROM obras2.acompanhamento_mi_arquivos am WHERE am.amiid = ami.amiid AND am.amastatus = 'A') $c 0
    ";
}
if($param['amiproblema']){
    $where['amiproblema'] = "ami.amiproblema IN ('{$param['amiproblema']}')";
}
if($param['amiedicao']){
    $where['amiedicao'] = "ami.amiedicao IN ('{$param['amiedicao']}')";
}

if ($param['obrid']) {
    $obrbuscatextoTemp = removeAcentos(str_replace("-"," ",(trim($param['obrid']) )));

    if(!strpos ($obrbuscatextoTemp, ',')){
        $where['obrnome'] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $param['obrid'] . "%') ) ";
    } else {
        $campos = explode(',', $obrbuscatextoTemp);
        $w = array();
        foreach ($campos as $c){
            $c = trim($c);
            $w[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
        }

        $w = '(' . implode('OR', $w) . ')';
        $where['obrnome'] = $w;
    }

}

$sql = "

    SELECT
        $acao
        o.obrid,
        o.obrnome,
        est.estuf,
        mun.mundescricao,
        emi.emidsc,
        u.usunome,
        TO_CHAR(ami.amidataacompanhamento, 'DD/MM/YYYY'),
        (
            SELECT CASE WHEN COUNT(*) > 0 THEN 'SIM' ELSE 'NÃO' END
            FROM obras2.acompanhamento_mi_arquivos am
            JOIN arquivo a ON a.arqid = am.arqid
            WHERE am.amiid = ami.amiid AND am.amastatus = 'A' AND (a.arqtipo ILIKE '%jpg%' OR a.arqtipo ILIKE '%png%' OR a.arqtipo ILIKE '%bmp%' OR a.arqtipo ILIKE '%image%')
        ) as foto,
        (SELECT CASE WHEN COUNT(*) > 0 THEN 'SIM' ELSE 'NÃO' END FROM obras2.acompanhamento_mi_arquivos am WHERE am.amiid = ami.amiid AND am.amastatus = 'A') as anexo,
        CASE WHEN amiobraexecucao = 'S'::bpchar THEN 'SIM' WHEN  amiobraexecucao = 'N'::bpchar THEN 'NÃO' WHEN amiobraexecucao = 'C'::bpchar THEN 'CONCLUÍDO' END as execucao,
        amipercexecucao,
        CASE WHEN amiproblema = 't' THEN 'SIM' ELSE 'NÃO' END as amiproblema

    FROM obras2.acompanhamento_mi ami
    JOIN obras2.obras o ON o.obrid = ami.obrid AND o.obridpai IS NULL AND o.obrstatus = 'A'
    LEFT JOIN entidade.endereco ed ON ed.endid = o.endid
    LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
    LEFT JOIN territorios.estado est ON est.estuf = mun.estuf
    LEFT JOIN obras2.empresami_uf euf ON euf.estuf = mun.estuf AND euf.eufstatus = 'A'
    LEFT JOIN obras2.empresami emi ON emi.emiid = euf.emiid AND emi.emistatus = 'A'
    LEFT JOIN seguranca.usuario u ON u.usucpf = ami.amiusuario
    WHERE amistatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "

     ORDER BY ami.amidataacompanhamento

";

if($_POST['xls']){
    $cabecalho = array('ID', 'Nome', 'UF', 'Municipio', 'Empresa', 'Quem Fez', 'Quando Fez', 'Possui Foto', 'Possui anexos', 'Execução', 'Percentual', 'Problema');
    $db->sql_to_xml_excel($sql, 'relatorioAcompanhamentoMI', $cabecalho, '');
} else {
    $cabecalho = array('Ação', 'ID', 'Nome', 'UF', 'Municipio', 'Empresa', 'Quem Fez', 'Quando Fez', 'Possui Foto', 'Possui anexos', 'Execução', 'Percentual', 'Problema');
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', 'formulario', '', '', null, $_param);
}


?>

<script type="text/javascript">
    function verAcompanhamento(amiid)
    {
        document.location.href = "/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&amiid=" + amiid;
    }
    function alterarAcompanhamento(amiid)
    {
        document.location.href = "/obras2/obras2.php?modulo=principal/cadAcompanhamentoMi&acao=A&amiid=" + amiid;
    }
    function excluirAcompanhamento(amiid)
    {
        if (window.confirm("Realmente deseja excluir o acompanhamento?")) {
            $.getJSON('/obras2/obras2.php?modulo=principal/listaAcompanhamentoMi&acao=A&ajax=delete&amiid=' + amiid, function(data){
                console.info(data);
                if(data.exclusao == 'ok'){
                    alert('Acompanhamento exluído com sucesso.');
                    $('#frmAcompanhamentoMI').submit();
                } else {
                    alert('Erro ao tentar excluir o acompanhamento.');
                }
            });
        }

    }
</script>