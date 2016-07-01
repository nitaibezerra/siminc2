<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<?php include APPRAIZ . "includes/cabecalho.inc"; ?>

<br />

<?php monta_titulo('Entidades com Pendências no Obras', ''); ?>

<form method="post" name="formulario" id="formulario">
    <table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<!--
        <tr>
            <td class="SubTituloDireita" >Município:</td>
            <td colspan="3">
                <?php
                $municipio = $_POST['municipio'];
                echo campo_texto('municipio', 'N', 'S', '', 50, 200, '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Estado:</td>
            <td colspan="3">
                <?php
                $estuf = $_POST['estuf'];
                $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
                $db->monta_combo("estuf", $sql, 'S', 'Todas as Unidades Federais', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"></td>
            <td colspan="3">
                <input class="botao" type="button" id= "btnPesquisar" value="Pesquisar">
                <input class="botao" type="button" id="btnLimpar" value="Limpar" />
            </td>
        </tr>
-->
        <tr>
            <td colspan="4" style="text-align: center;">
                <input class="botao" type="button" id= "btnExportarExcel" value="Exportar Para Excel">
            </td>
        </tr>
    </table>
</form>

<?php

$where = NULL;

//if ($municipio) {
//    $municipioTemp = removeAcentos($_POST['municipio']);
//    $where .= " AND public.removeacento(mun.mundescricao) ilike '%" . $municipioTemp . "%'";
//}
//if ($estuf) {
//    $where .= " AND (iu.estuf = '" . $estuf . "' OR iu.mun_estuf = '" . $estuf . "')";
//}

$cabecalho = array('UF','Município', 'Processo' );

# Filtro por pendencias
$condicaoProcessoPendencia = buscarSelectProcessoPendencia($pendenciaObras);

# Sql principal do relatorio
$sqlPac = "
    -- CONSULTA PAC
    SELECT DISTINCT
     	m.estuf,
        m.mundescricao,
     	to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as numeroprocesso 
    FROM
        par.processoobra p 
    INNER JOIN territorios.municipio m ON m.muncod=p.muncod 
    WHERE
        $condicaoProcessoPendencia
        AND m.mundescricao IS NOT NULL
        and p.prostatus = 'A' 

    UNION ALL (
        SELECT
        	p.estuf,
        	'<center>-</center>' as mundescricao,
           to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
          
           
        FROM
            par.processoobra p
        WHERE
            $condicaoProcessoPendencia
            and p.prostatus = 'A' 
            AND p.estuf IS NOT NULL AND p.muncod IS NULL
	)
";

$condicaoProcessoParPendencia = buscarSelectProcessoParPendencia($pendenciaObras);
$sqlPar = "
    -- CONSULTA PAR
    SELECT DISTINCT
         CASE WHEN iu.itrid = 1 THEN e.estuf ELSE m.estuf END as uf,
        CASE WHEN iu.itrid = 1 THEN '' ELSE CASE WHEN iu.estuf = 'DF' THEN '' ELSE m.mundescricao END END as descricao, 
        to_char(p.prpnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as numeroprocesso
    FROM
        par.instrumentounidade iu
        INNER JOIN par.processopar p ON p.inuid = iu.inuid and p.prpstatus = 'A'
        LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
        LEFT JOIN territorios.estado e ON e.estuf = iu.estuf
    WHERE
        p.prpstatus = 'A'
        AND $condicaoProcessoParPendencia
";

$sql = "
    $sqlPac
    UNION
    $sqlPar
";
//ver($sql,d);
$exportarExcel = $_REQUEST['exportarExcel'];

if(!empty($exportarExcel)){
    ob_clean();
    header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
    header ( "Pragma: no-cache" );
    header ( "Content-type: application/xls; name=SIMEC_Relatorio_Entidade_Pendencia-".date("Ymdhis").".xls");
    header ( "Content-Disposition: attachment; filename=SIMEC_Relatorio_Entidade_Pendencia-".date("Ymdhis").".xls");
    header ( "Content-Description: MID Gera excel" );
    $db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%',$par2);
    exit;
}

//ver($sql,d);
$db->monta_lista(
    $sql,
    $cabecalho,
    100,
    70,
    'N',
    '95%',
    'N',
    '');
?>

<script>
    jQuery(document).ready(function(){
        jQuery('#btnPesquisar').click(function(){
            submeterFormulario();
        });
        
        jQuery('#btnLimpar').click(function(){
            limparformulario();
        });

        jQuery('#btnExportarExcel').click(function(){
            exportarExcel();
        });
        
    });
    
    function submeterFormulario() {
        jQuery('#formulario').attr('action', 'par.php?modulo=relatorio/lista-entidade-pendencia&acao=A');
        jQuery('#formulario').submit();
    }
    
    function exportarExcel(){
        jQuery('#formulario').attr('action', 'par.php?modulo=relatorio/lista-entidade-pendencia&acao=A&exportarExcel=1');
        jQuery('#formulario').submit();
    }
    
    function limparFormulario(){
        window.location = 'par.php?modulo=relatorio/lista-entidade-pendencia&acao=A';
    }
</script>