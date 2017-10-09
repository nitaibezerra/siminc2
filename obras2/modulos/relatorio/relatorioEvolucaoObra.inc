<?php
$dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/evolucaomi';
if($_GET['download']){
    if(file_exists($dir_relatorios . '/' . $_GET['download'])) {
        header("Content-Type: application/vnd.ms-excel; charset=ISO-8859-1");
        header("Content-Disposition: inline; filename=\"" . $_GET['download'] . "\"");
        echo file_get_contents($dir_relatorios . '/' . $_GET['download']);
    }
    exit;
}

function monta_sql() {
    global $db;

    extract($_REQUEST);

    $where = array();
    
    if (!empty($obrid)) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrid))));
        $where[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                    o.obrid::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
    }

    if (!empty($estuf)) {
        if (is_array($estuf)) {
            foreach ($estuf as $k => $v) {
                $estuf[$k] = "'{$v}'";
            }
        } else {
            $estuf = array("'{$estuf}'");
        }
        foreach ($estuf as $k => $v) {
            if (trim($v) == '' || $v == "''" || empty($v)) {
                unset($estuf[$k]);
            }
        }
        if (!empty($estuf)) {
            $where[] = " mun.estuf IN ( " . implode(",", $estuf) . " ) ";
        }
    }

    if (!empty($muncod)) {
        if (is_array($muncod)) {
            foreach ($muncod as $k => $v) {
                $muncod[$k] = "'{$v}'";
            }
        } else {
            $muncod = array("'{$muncod}'");
        }
        foreach ($muncod as $k => $v) {
            if (trim($v) == '' || $v == "''" || empty($v)) {
                unset($muncod[$k]);
            }
        }
        if (!empty($muncod)) {
            $where[] = " mun.muncod IN ( " . implode(",", $muncod) . " ) ";
        }
    }

    if (!empty($esdid_obr) && is_array($esdid_obr) && $esdid_obr[0] != '') {
        $where[] = " ed.esdid IN ( " . implode(",", $esdid_obr) . " ) ";
    }

    if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
        $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
    }

    if ( !empty($tooid) && count($tooid) > 0 && $tooid[0] !== ''){
        $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
    }
    
    switch ($vistoria) {
        case 'sim' : 
            $where[] = " o.obrdtvistoria IS NOT NULL ";
            break;
        case 'nao' : $where[] = " o.obrdtvistoria IS NULL ";
            break;
    }

    $strWhere = (!empty($where)) ? ' AND '.implode(' AND ', $where) : '';

    $sql = "SELECT 
                    o.obrid,
                    o.obrnome,
                    pf.prfdesc as programa,
                    too.toodescricao as fonte,
                    ed.esddsc as situacao,
                    mun.estuf,
                    mun.mundescricao,


                    DATE_PART('days', (SELECT s.supdata
					FROM obras2.obras o1
						    JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
						    WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
						    ORDER BY supdata DESC LIMIT 1)
							-
					(SELECT s.supdata
					FROM obras2.obras o1
						    JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
						    WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
						    ORDER BY supdata DESC OFFSET 2 LIMIT 1) ) as days_ult_vist,



                    (SELECT (SELECT
                                     CASE
                                         WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                         ELSE 0::numeric
                                     END AS total
                                      FROM obras2.itenscomposicaoobra i
                                        INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                      WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC LIMIT 1)
                                        -
                                (SELECT (SELECT
                                         CASE
                                             WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                             ELSE 0::numeric
                                         END AS total
                                          FROM obras2.itenscomposicaoobra i
                                          INNER JOIN obras2.cronograma cro ON cro.croid = i.croid  AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                            LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                          WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 2 LIMIT 1) as percent_ult_vist,

                    DATE_PART('days', (SELECT s.supdata

                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 3 LIMIT 1)
                                        -
                                (SELECT s.supdata
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 5 LIMIT 1) ) as days_pen_vist,



                    (SELECT (SELECT
                                     CASE
                                         WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                         ELSE 0::numeric
                                     END AS total
                                      FROM obras2.itenscomposicaoobra i
                                      INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus = 'A'
                                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                      WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 3 LIMIT 1)
                                        -
                                (SELECT (SELECT
                                         CASE
                                             WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                             ELSE 0::numeric
                                         END AS total
                                          FROM obras2.itenscomposicaoobra i
                                          INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                            LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                          WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 5 LIMIT 1) as percent_pen_vist,



                  DATE_PART('days', (SELECT s.supdata
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 6 LIMIT 1)
                                        -
                                (SELECT s.supdata
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 8 LIMIT 1) ) as days_ant_vist,
                  (SELECT (SELECT
                                     CASE
                                         WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                         ELSE 0::numeric
                                     END AS total
                                      FROM obras2.itenscomposicaoobra i
                                      INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                      WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 6 LIMIT 1)
                                        -
                                (SELECT (SELECT
                                         CASE
                                             WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                             ELSE 0::numeric
                                         END AS total
                                          FROM obras2.itenscomposicaoobra i
                                          INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                            LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                          WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)
                                FROM obras2.obras o1
                                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                                        ORDER BY supdata DESC OFFSET 8 LIMIT 1) as percent_ant_vist,


                    (SELECT ARRAY_TO_STRING(ARRAY(
                    SELECT 
                            TO_CHAR(s.supdata, 'DD/MM/YYYY')::text || ';' ||
                            (SELECT
                                 CASE
                                     WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                     ELSE 0::numeric
                                 END AS total
                                  FROM obras2.itenscomposicaoobra i
                                  INNER JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid
                                    LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                  WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o1.obrid AND i.obrid = cro.obrid)::text

                    FROM obras2.obras o1
                    JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                    WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                    ORDER BY supdata DESC), '@')) as vistorias

            FROM obras2.obras o
            JOIN obras2.empreendimento          ep ON ep.empid   = o.empid
            LEFT JOIN workflow.documento       doc ON doc.docid  = o.docid
            LEFT JOIN workflow.estadodocumento 	ed ON ed.esdid   = doc.esdid
            LEFT JOIN obras2.tipoorigemobra    too ON too.tooid  = o.tooid
            LEFT JOIN obras2.programafonte      pf ON pf.prfid   = ep.prfid
            LEFT JOIN entidade.endereco        edo ON edo.endid  = o.endid
            LEFT JOIN territorios.municipio    mun ON mun.muncod = edo.muncod
            WHERE o.obridpai IS NULL 
              AND o.obrstatus = 'A'
              {$strWhere}  ";

    return $sql;
}

// exibe consulta
if ($_REQUEST['tiporelatorio']) {
    
    $sql = monta_sql();

    $cabecalho = array("ID", "Nome", "Programa", "Fonte","Situação","UF", "Município", "Qtd Dias entre as 3 últimas vistorias", "% de avanço de obra nas 3 últimas vistorias", "Qtd Dias entre as 3 penúltimas vistorias", "% de avanço de obra nas 3 penúltimas vistorias", "Qtd Dias entre as 3 anti-penúltimas vistorias", "% de avanço de obra nas 3 anti-penúltimas vistorias",  "Vistorias");
    $obras = $db->carregar($sql);
    
    switch ($_REQUEST['tiporelatorio']) {
        case 'xls':
            $cabecalho = array("ID", "Nome", "Programa", "Fonte","Situação","UF", "Município", "Vistorias");
            array_pop($cabecalho);
            $maxVst = 0;
            foreach ($obras as $key => $obra) {
                $data = $obras[$key]['vistorias'];
                $vistorias = str_replace('@', ';', $obras[$key]['vistorias']);
                unset($obras[$key]['vistorias']);
                $arV = explode(';', $vistorias);
                $maxVst = (count($arV) > $maxVst) ? count($arV) : $maxVst;

                $qtd = 0;
				$vistorias = explode('@', $data);
				foreach($vistorias as $v){
					$qtd++;
                    $d = explode(';',$v);
					$obras[$key][] = $d[0];
                    $obras[$key][] = $d[1];
					
                    switch ($qtd){
						case 3:
							$obras[$key][] = $obras[$key]["days_ult_vist"];
							$obras[$key][] = $obras[$key]["percent_ult_vist"];
							break;
						case 6:
							$obras[$key][] = $obras[$key]["days_pen_vist"];
							$obras[$key][] = $obras[$key]["percent_pen_vist"];
							break;
						case 9:
							$obras[$key][] = $obras[$key]["days_ant_vist"];
							$obras[$key][] = $obras[$key]["percent_ant_vist"];
							break;
					}
                }
				unset($obras[$key]["days_ant_vist"]);
				unset($obras[$key]["percent_ant_vist"]);
				unset($obras[$key]["days_ult_vist"]);
				unset($obras[$key]["percent_ult_vist"]);
				unset($obras[$key]["days_pen_vist"]);
				unset($obras[$key]["percent_pen_vist"]);
            }
						
            for ($x = 1; $x <= $maxVst; $x++){
                if(($x % 2) != 0)
                    $cabecalho[] = 'Dt. Vistoria';
                else
                    $cabecalho[] = '% Vistoria';
					
				switch ($x){
						case 6:
							$cabecalho[] = "Qtd Dias entre as 3 últimas vistorias";
							$cabecalho[] = "% de avanço de obra nas 3 últimas vistorias";
							break;
						case 12:
							$cabecalho[] = "Qtd Dias entre as 3 penúltimas vistorias";
							$cabecalho[] = "% de avanço de obra nas 3 penúltimas vistorias";
							break;
						case 18:
							$cabecalho[] = "Qtd Dias entre as 3 anti-penúltimas vistorias";
							$cabecalho[] ="% de avanço de obra nas 3 anti-penúltimas vistorias";
							break;
						}
            }
			
            $db->sql_to_xml_excel($obras, 'rel_obrasmi', $cabecalho);
            die();
            break;
        case 'html':
            foreach ($obras as $key => $value) {
                if(!empty($value['vistorias'])){
                    $vistorias = explode('@', $value['vistorias']);
                    $obras[$key]['vistorias'] = '';
                    foreach ($vistorias as $k => $v) {
                        $vist = explode(';', $v);
                        $str_vistoria = 'Data vistoria: '.$vist[0].' - % Vistoria: '.$vist[1].'<br>';
                        $obras[$key]['vistorias'] .= $str_vistoria;
                    }
                }else{
                    $obras[$key]['vistorias'] = 'Sem vistoria';
                    
                }
            }
            
            echo '<html>
                    <head>
                        <title> <?php echo NOME_SISTEMA; ?> </title>
                        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                        <body>
                            <center>
                                    <!--  Cabeçalho Brasão -->
                                    '.monta_cabecalho_relatorio('95').' 
                            </center>
                ';
            $db->monta_lista($obras, $cabecalho, 30, 50, 'N', 'center', 'N', 'N');
            echo '</body>';
            die();
            break;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
monta_titulo("Relatório de Evolução da Obra", 'Selecione os filtros');
?>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<script type="text/javascript">


    function exibeRelatorioGeral(tipo) {
        var formulario = document.formulario;
        selectAllOptions(document.getElementById('prfid'));
        selectAllOptions(document.getElementById('tooid'));
        selectAllOptions(document.getElementById('estuf'));
        selectAllOptions(document.getElementById('muncod'));
        selectAllOptions(document.getElementById('esdid_obr'));

        formulario.action = 'obras2.php?modulo=relatorio/relatorioEvolucaoObra&acao=A&tiporelatorio=' + tipo;
        window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        formulario.target = 'relatorio';
        formulario.submit();
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

//-->
</script>

<form action="" method="post" name="formulario" id="filtro"> 

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" style="width: 490px;">Nome da Obra/ID:</td>
            <td>
                <?php echo campo_texto('obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        <tr>
                <?php
                $sql_obr = "SELECT esdid as codigo, esddsc as descricao 
                            FROM workflow.estadodocumento 
                            WHERE tpdid='" . TPDID_OBJETO . "' 
                              AND esdstatus='A' 
                            ORDER BY esdordem";
                $stSqlCarregados = '';
                $arr_obr_wf = array();
                if (!empty($_POST['esdid_obr']) && is_array($_POST['esdid_obr']) && $_POST['esdid_obr'][0] != '') {
                    foreach ($_POST['esdid_obr'] as $key => $value) {
                        $arr_obr_wf[$key] = "'" . $value . "'";
                    }
                    $str_colecao = (!empty($arr_obr_wf)) ? " AND esdid IN (" . implode(',', $arr_obr_wf) . ") " : '';
                    $stSqlCarregados = "SELECT esdid as codigo, esddsc as descricao 
                                            FROM workflow.estadodocumento 
                                            WHERE tpdid='" . TPDID_OBJETO . "' 
                                              AND esdstatus='A' 
                                            {$str_colecao}
                                            ORDER BY
                                                    esdid";
                }
                mostrarComboPopup('Situação da Obra:', 'esdid_obr', $sql_obr, $stSqlCarregados, 'Selecione a(s) Situação(ões)', null, '', false);
                ?>
        </tr>
        <tr id="idUF">
        <?php
        #UF
        $ufSql = " SELECT 	estuf as codigo, estdescricao as descricao
                               FROM territorios.estado est
                               ORDER BY estdescricao
                             ";
        $stSqlCarregados = '';
        $arr_uf = array();
        if ($_POST['estuf'][0] != '') {
            foreach ($_POST['estuf'] as $key => $value) {
                $arr_uf[$key] = "'" . $value . "'";
            }
            $stSqlCarregados = "SELECT
                                                    estuf as codigo, estdescricao as descricao
                                            FROM territorios.estado est
                                            WHERE 
                                                    estuf IN (" . implode(',', $arr_uf) . ")
                                            ORDER BY
                                                    2";
        }
        mostrarComboPopup('UF:', 'estuf', $ufSql, $stSqlCarregados, 'Selecione o(s) Estado(s)', null, 'ajaxEstado', false);
        ?>
        </tr>
        <tr id="idMunicipio">
            <?php
            #Municipio
            $munSql = " SELECT muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                FROM territorios.municipio 
                                ORDER BY
                                    mundescricao";
            $stSqlCarregados = '';
            $arr_muncod = array();
            if (is_array($_POST['muncod'])) {
                foreach ($_POST['muncod'] as $key => $value) {
                    $arr_muncod[$key] = "'" . $value . "'";
                }
            }

            $where_mun = (!empty($_POST['muncod'])) ? " muncod IN (" . implode(',', $arr_muncod) . ") " : '';
            $where_uf = (!empty($arr_uf)) ? " estuf IN (" . implode(',', $arr_uf) . ") " : '';

            $where = '';
            if (trim($where_mun) != '') {
                $where .= $where_mun;
            }

            if (trim($where_uf) != '' && trim($where) !== '') {
                $where .= ' AND ' . $where_uf;
            } elseif (trim($where_uf) !== '') {
                $where .= $where_uf;
            }

            if (trim($where) !== '') {
                $stSqlCarregados = "SELECT
                                                    muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                            FROM territorios.municipio
                                            WHERE 
                                                    {$where}
                                            ORDER BY
                                                    mundescricao";
            }
            mostrarComboPopup('Município:', 'muncod', $munSql, $stSqlCarregados, 'Selecione os municípios', null, '', false);
            ?>
        </tr>
            <?php
            // Programa
            $stSql = "SELECT
                    prfid AS codigo,
                    prfdesc AS descricao
                 FROM 
                    obras2.programafonte
                 ORDER BY
                    prfdesc ";
            if (!empty($_POST['prfid'][0])) {
                $stSqlSelecionados = "SELECT
                        prfid AS codigo,
                        prfdesc AS descricao
                     FROM 
                        obras2.programafonte
                     WHERE prfid IN (" . implode(', ', $_POST['prfid']) . ")
                     ORDER BY
                        prfdesc ";
            }
            mostrarComboPopup('Programa', 'prfid', $stSql, $stSqlSelecionados, 'Selecione o(s) Programa(s)');
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

            if (!empty($_POST['tooid'][0])) {
                $sql_carregados = "SELECT
                        tooid AS codigo,
                        toodescricao AS descricao
                     FROM 
                        obras2.tipoorigemobra
                     WHERE
                        toostatus = 'A'
                        AND tooid IN (" . implode(', ', $_POST['tooid']) . ")
                     ORDER BY
                        toodescricao ";
            }
            mostrarComboPopup('Fonte', 'tooid', $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)');
            ?>     
        <tr>
            <td class="SubTituloDireita">Possui vistoria?</td>
            <td>
                <input type='radio' id='vistoria' name='vistoria' value='sim'   /> Sim
                <input type='radio' id='vistoria' name='vistoria' value='nao'   /> Não
                <input type='radio' id='vistoria' name='vistoria' value='todos' checked="checked" /> Todas
            </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/> 
                <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
                <?
                    $dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/evolucaomi';
                    $scanned_directory = array_diff(scandir($dir_relatorios), array('..', '.', '.svn'));

                    $file = array_pop($scanned_directory);
                    if($file):
                ?>
                    <input type="button" value="Donwload Relatório Compilado" onclick="downloadUltimoArquivo('<?=$file?>');" style="cursor: pointer;"/>
                <? endif; ?>
            </td>
        </tr>
    </table>
    
</form>

<script type="text/javascript">
    function downloadUltimoArquivo(file){
        window.open( window.location + '&download=' + file );
    }
</script>