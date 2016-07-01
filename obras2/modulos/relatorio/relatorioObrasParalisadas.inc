<?php

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}
if (count($_POST) && array_key_exists('obrid', $_POST)) {
    $where = array();
    $innerWhere = array();
    if (!empty($_POST['obrid'])) {
        $obridTmp = removeAcentos(str_replace("-"," ",(trim($_POST['obrid']))));
        $where[] = "( (UPPER(public.removeacento(foo.obrnome) ) ) ILIKE ('%" . $obridTmp . "%') OR
            foo.obrid::CHARACTER VARYING ILIKE '%" . $_POST['obrid'] . "%') ";
    }

    if (!empty($_POST['tobid'])) {
        array_push($where, " foo.tobid = '{$_POST['tobid']}' ");
    }
    
    if (!empty($_POST['estuf'])) {
        array_push($where, " foo.estuf = '{$_POST['estuf']}' ");
    }
    
    if (!empty($_POST['tplid'])) {
        $where[] = " foo.tplid = '{$_POST['tplid']}' ";
    }

    // Programa
    if( $_POST['prfid'][0] && $_POST['prfid_campo_flag'] ){
        if ( !$_POST['prfid_campo_excludente'] ){
            array_push($innerWhere, " pf.prfid  IN (" . implode( ',', $_POST['prfid'] ) . ") ");
        }else{
            array_push($innerWhere, " ( pf.prfid  NOT IN (" . implode( ',', $_POST['prfid'] ) . ") OR pf.prfid is null ) ");
        }

    }
    // Fonte
    if( $_POST['tooid'][0] && $_POST['tooid_campo_flag'] ){
        if ( !$_POST['tooid_campo_excludente'] ){
            array_push($innerWhere, " too.tooid  IN (" . implode( ',', $_POST['tooid'] ) . ") ");
        }else{
            array_push($innerWhere, " ( too.tooid  NOT IN (" . implode( ',', $_POST['tooid'] ) . ") OR too.tooid IS NULL ) ");
        }

    }

    if (!empty($_POST['muncod'])) {
        $muncod  = (array) $_POST['muncod'];
        $where[] = "foo.muncod IN('" . implode("', '", $muncod) . "')";
    }

    $whereString = implode(' AND ', $where);
    $_where = count($where) ? " WHERE {$whereString}" : '';

    $whereInnerString = implode(' AND ', $innerWhere);
    $_innerwhere = count($innerWhere) ? " AND {$whereInnerString}" : '';
}

$urlbase = 'obras2.php?modulo=relatorio/relatorioObrasParalisadas&acao=A';

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo('Relatório - Obras Paralisadas', '');

$strSQL = "SELECT * FROM (SELECT
                o.obrid,
                o.obrnome,
                pf.prfdesc,
                too.toodescricao,
                /*
                CASE WHEN oc.covid is not null 
                        THEN oc.covnumero || ' ' 
                        ELSE o.numconvenio || ' ' 
                END as nr_convenio,
                */
                dco.dcoconvenio as nr_convenio,
                o.obrnumprocessoconv,
                esd.esddsc,
                (SELECT
                 hist.hprobs
                 FROM
                   obras2.historicoparalisacao hist
                 WHERE hist.obrid = o.obrid AND hist.hprdtstatus = 'A'
                 ORDER BY hist.hprid DESC
                 LIMIT 1) hprobs,
                (SELECT
                 tpl.tpldsc
                 FROM
                   obras2.historicoparalisacao hist
                   JOIN obras2.tipoparalisacao tpl ON tpl.tplid = hist.tplid
                 WHERE hist.obrid = o.obrid AND hist.hprdtstatus = 'A'
                 ORDER BY hist.hprid DESC
                 LIMIT 1) tpldsc,
                 (SELECT
                 tpl.tplid
                 FROM
                   obras2.historicoparalisacao hist
                   JOIN obras2.tipoparalisacao tpl ON tpl.tplid = hist.tplid
                 WHERE hist.obrid = o.obrid AND hist.hprdtstatus = 'A'
                 ORDER BY hist.hprid DESC
                 LIMIT 1) tplid,
                 o.tobid,
                 edo.estuf,
                 mun.mundescricao,

                 (SELECT TO_CHAR(s.supdata, 'DD/MM/YYY') as supdata
                   FROM obras2.supervisao s
                  WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar
                  ORDER BY s.supdata DESC LIMIT 1) as dtultvistoria,

                  (SELECT ( SELECT
                                  CASE
                                  WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                  ELSE 0::numeric
                                  END AS total
                                    FROM obras2.itenscomposicaoobra i
                                    INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                      LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                    WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o.obrid AND i.obrid = cro.obrid) AS percentual
                   FROM obras2.supervisao s
                  WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar
                  ORDER BY s.supdata DESC LIMIT 1) as percentultvistoria,

                  (SELECT TO_CHAR(s.supdata,'DD/MM/YYYY') FROM obras2.supervisaoempresa e
                    JOIN obras2.supervisao s ON s.sueid = e.sueid
                    WHERE s.obrid = o.obrid AND s.supstatus = 'A'::bpchar
                    ORDER BY s.supdata LIMIT 1) dtultsup,

                    (SELECT ( SELECT
                                  CASE
                                  WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                  ELSE 0::numeric
                                  END AS total
                                    FROM obras2.itenscomposicaoobra i
                                    INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                      LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                    WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o.obrid AND i.obrid = cro.obrid) AS percentual FROM obras2.supervisaoempresa e
                    JOIN obras2.supervisao s ON s.sueid = e.sueid
                    WHERE s.obrid = o.obrid AND s.supstatus = 'A'::bpchar
                    ORDER BY s.supdata LIMIT 1) percentultsup,

                  (SELECT s.supobs
                   FROM obras2.supervisao s
                  WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar
                  ORDER BY s.supdata DESC LIMIT 1) as obsultsup


          FROM obras2.obras o
                JOIN workflow.documento         doc ON doc.docid   = o.docid
                JOIN workflow.estadodocumento   esd ON esd.esdid   = doc.esdid
                JOIN obras2.empreendimento      emp ON emp.empid   = o.empid
                LEFT JOIN obras2.tipoorigemobra too ON too.tooid   = o.tooid
                LEFT JOIN obras2.programafonte   pf ON pf.prfid    = emp.prfid
                LEFT JOIN entidade.endereco     edo ON (edo.endid  = o.endid)
                LEFT JOIN territorios.municipio      mun ON (mun.muncod = edo.muncod)
                
                LEFT JOIN painel.dadosconvenios        dco ON dco.dcoprocesso = Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')
                /*
                LEFT JOIN obras2.formarepasserecursos      frr ON o.obrid    = frr.obrid    -- Forma de repasse de recurso
                LEFT JOIN obras2.tipoformarepasserecursos tfrr ON tfrr.frpid = frr.frpid    -- Tipo da Forma de repasse de recurso
                LEFT JOIN obras2.conveniosobra              oc ON oc.covid   = frr.covid    -- Convenio da obra - Verificar se é isso mesmo
                */

          WHERE 
            o.obrstatus = 'A' AND 
            o.obridpai IS null AND 
            doc.esdid IN (".ESDID_OBJ_PARALISADO.")
            $_innerwhere
            ORDER BY o.obrnome) foo {$_where}
            ";

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
    $(".row-header").hover(function(){
        $(this).css("background-color", "#c0c0c0");
    }, function(){
        $(this).css("background-color", "");
    });
    
    $("input[name='btn_salvar']").on("click", function(){
        $("#xls").val("");
    });
    
    $("input[name='btn_export_xls']").on("click", function(){
        if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }
        $("#xls").val(1);
        $("#frmOrdemOSMI").submit();
    });

    $("#frmOrdemOSMI").submit(function(){
        if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }
    });
});
</script>
<form name="frmOrdemOSMI" id="frmOrdemOSMI" method="post">
    <input type="hidden" name="xls" id="xls" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="35%">Nome da Obra/ID</td>
            <td>
                <?php echo campo_texto('obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo da Obra</td>
            <td>
                <?php
                    $tipoObra = new TipoObra();
                    $db->monta_combo('tobid', $tipoObra->listaCombo(), 'S', 'Todos', '', '', '', 200, 'N', 'tobid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo de paralisação</td>
            <td>
                <?php 
                    $tipo = "SELECT tplid AS codigo, tpldsc AS descricao FROM obras2.tipoparalisacao WHERE tplstatus = 'A'";
                    $db->monta_combo('tplid', $tipo, 'S', 'Todos', '', '', '', 200, 'N', 'tplid');
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

        <?

        // Programa
        $stSql = "SELECT
				prfid AS codigo,
				prfdesc AS descricao
			 FROM
				obras2.programafonte
			 ORDER BY
				prfdesc ";
        $sql_carregados = '';
        if($_POST['prfid_campo_flag'] && $_POST['prfid'][0]){
            $sql_carregados = "	SELECT
				prfid AS codigo,
				prfdesc AS descricao
			 FROM
				obras2.programafonte
				WHERE prfid IN (" . implode( ',', $_POST['prfid'] ) . ")
			 ORDER BY
				prfdesc";
        }
        mostrarComboPopup( 'Programa', 'prfid',  $stSql, $sql_carregados, 'Selecione o(s) Programa(s)' );

        // Fonte
        $stSql = "SELECT
				tooid AS codigo,
				toodescricao AS descricao
			 FROM
				obras2.tipoorigemobra
			WHERE
				toostatus = 'A'
			 ORDER BY
				toodescricao ";
        $sql_carregados = '';
        if($_POST['tooid_campo_flag'] && $_POST['tooid'][0]){
            $sql_carregados = "	SELECT
							tooid AS codigo,
							toodescricao AS descricao
						 FROM
							obras2.tipoorigemobra
						WHERE
							toostatus = 'A'
						AND
							tooid IN (" . implode( ',', $_POST['tooid'] ) . ")
						 ORDER BY
							toodescricao ";
        }


        mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)' );
        ?>

        <tr>
            <td bgcolor="#c0c0c0" colspan="2" align="center">&nbsp;</td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <input type="submit" value="Filtrar" name="btn_salvar"/>&nbsp;&nbsp;
                <input type="submit" value="Gerar Excel" name="btn_export_xls"/>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0" colspan="2" align="center">&nbsp;</td>
        </tr>
    </table>
</form>
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
<?php

$supervisao = new Supervisao();

if ($_POST['xls']) {
    $arquivo = "relatorioObrasParalisadas_".date("Ymdhis");
    //$cabecalho = array('Id Obra', 'Nome da Obra', 'Fonte da Obra', 'Origem da Obra', 'Convênio' , 'Processo', 'Status', 'Observações', 'Tipo Paralisação', 'Data de início da paralisação');
    $cabecalho = array('ID Obra', 'Nome da Obra', 'Programa', 'Fonte', 'Convênio' , 'Processo', 'Situação', 'Observações', 'Tipo Paralisação', 'UF', 'Município', 'Dt Últ Vistoria', '% Últ. Vistoria','Dt Últ Supervisão', '% Últ. Supervisão', 'Laudo Técnico', 'Data Última Paralisação');

    $dados = $db->carregar($strSQL);
    if ($dados) {
        foreach ($dados as $k => &$row) {
            unset($row['tplid']);
            unset($row['tobid']);
            unset($row['muncod']);
            $row['dtUltimaParalisacao'] = $supervisao->getUltimaParalicaoObra($row['obrid']);
        }
        $db->sql_to_xml_excel($dados,$arquivo,$cabecalho);
    }

}

$perpage = 50;
$total_reg = $db->pegaUm("SELECT COUNT(1) FROM ({$strSQL}) rs",0,null);
$pages = 5;
$numero = ($_REQUEST['numero']=='') ? 1 : intval($_REQUEST['numero']);
if (!$_REQUEST['ordemlista']) $_REQUEST['ordemlista'] = 'obrnome';

$cabecalho = array('Id Obra', 'Nome da Obra', 'UF', 'Município', 'Programa', 'Fonte', 'Convênio' , 'Processo', 'Status', 'Observações','Dt Ult Vistoria', '% Últ. Vistoria','Dt Ult Supervisão', '% Últ. Supervisão', 'Tipo Paralisação', 'Laudo Técnico', 'Data Últ. Paralisação');
$order = array('obrid', 'obrnome', 'prfdesc', 'toodescricao', 'nr_convenio','obrnumprocessoconv', 'esddsc', 'hprobs', 'tpldsc', 'dtinclusao');
$strSQL = "{$strSQL} ORDER BY foo.{$_REQUEST['ordemlista']} {$_REQUEST['ordemlistadir']} LIMIT {$perpage} OFFSET ".($numero-1);
$dados = $db->carregar($strSQL);
echo '<form name="formlista" method="POST">
        <input type="Hidden" name="numero"        value="" />
        <input type="Hidden" name="ordemlista"    value="'.$_REQUEST['ordemlista'].'"/>
        <input type="Hidden" name="ordemlistadir" value="'.$_REQUEST['ordemlistadir'].'"/>
        <input type="Hidden" name="obrid"         value="'.$_REQUEST['obrid'].'" />
        <input type="Hidden" name="tobid"         value="'.$_REQUEST['tobid'].'" />
        <input type="Hidden" name="tplid"         value="'.$_REQUEST['tplid'].'" />
        <input type="Hidden" name="estuf"         value="'.$_REQUEST['estuf'].'" />
        <input type="Hidden" name="muncod"        value="'.$_REQUEST['muncod'].'" />
    </form>';

if (!$_REQUEST['ordemlistadir']) {
    $orderlistadir = 'DESC';
} else {
    $orderlistadir = ($_REQUEST['ordemlistadir'] === 'ASC') ? 'DESC' : 'ASC';
}

echo '<table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0"><thead><tr>';
foreach ($cabecalho as $i => $item) {
    $onclick = ($i<count($cabecalho)-1) ? 'onclick="ordena(\''.$order[$i].'\', \''.$orderlistadir.'\');"' : '';
    echo '<td class="row-header" '.$onclick.' valign="top"><strong>'.$item.'</strong></td>';
}
echo '</tr></thead>';

echo '<tbody>';

if (!is_bool($dados) && count($dados) > 0) {
    foreach ($dados as $k => $row) {
        $class = ($k%2)==0 ? '' : 'row-odd';
        echo '<tr class="'.$class.'">';
        echo '<td width="2%">'.$row['obrid'].'</td>';
        echo '<td width="10%">'.$row['obrnome'].'</td>';
        echo '<td width="5%">'.$row['estuf'].'</td>';
        echo '<td width="5%">'.$row['mundescricao'].'</td>';
        echo '<td width="5%">'.$row['prfdesc'].'</td>';
        echo '<td width="5%">'.$row['toodescricao'].'</td>';        
        echo '<td width="5%">'.$row['nr_convenio'].'</td>';
        echo '<td width="5%">'.$row['obrnumprocessoconv'].'</td>';
        echo '<td width="5%">'.$row['esddsc'].'</td>';
        echo '<td width="15%">'.$row['hprobs'].'</td>';
        echo '<td width="6%">'.$row['dtultvistoria'].'</td>';
        echo '<td width="6%">'.$row['percentultvistoria'].'</td>';
        echo '<td width="6%">'.$row['dtultsup'].'</td>';
        echo '<td width="6%">'.$row['percentultsup'].'</td>';
        echo '<td width="6%">'.$row['tpldsc'].'</td>';
        echo '<td width="6%">'.$row['obsultsup'].'</td>';
        echo '<td width="3%">'.$supervisao->getUltimaParalicaoObra($row['obrid']).'</td>';
        echo '</tr>';
    }
} else {
    echo '<td colspan="8" align="center"><strong>Nenhum registro encontrado!</strong></td>';
}
echo '</tbody>';

echo '</table>';
echo '<table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0"><tr><td><strong>Total de registro</strong>: '.$total_reg.'</td></tr></table>';
include APPRAIZ."includes/paginacao.inc";
echo '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
?>
<script type="text/javascript">

    /**
     * Alterar visibilidade de um bloco.
     *
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco( bloco )
    {

        if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }

        var div_on = document.getElementById( bloco + '_div_filtros_on' );
        var div_off = document.getElementById( bloco + '_div_filtros_off' );
        var img = document.getElementById( bloco + '_img' );
        var input = document.getElementById( bloco + '_flag' );
        if ( div_on.style.display == 'none' )
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
    function onOffCampo( campo )
    {
        if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }

        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' )
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