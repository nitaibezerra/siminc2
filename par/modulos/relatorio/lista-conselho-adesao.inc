<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<?php include APPRAIZ . "includes/cabecalho.inc"; ?>

<br />

<?php monta_titulo('Relatório Interlocutor Adesão Conselheiro Escolar', ''); ?>

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

$cabecalho = array(
    'Esfera',
    'UF',
    'Código IBGE',
    'Município',
    'CPF',
    'Nome',
    'Email',
    'Telefone',
    'Ramal',
    'Formação',
    'Vinculo',
    'Função Atual',
    'Órgão',
    'Cargo Efetivo',
    'Exercício',
    'Estado'
);

$sql = "
    SELECT DISTINCT
        CASE WHEN p_iu.itrid = 1 THEN
            'Estadual'
        ELSE
            'Municipal'
        END AS esfera,
    	CASE WHEN p_iu.estuf IS NULL THEN
            p_iu.mun_estuf
        ELSE
            p_iu.estuf
        END AS uf,
        t_m.muncod AS cod_ibge,
        t_m.mundescricao AS municipio,
        p_pfc.pcucpf AS cpf,
        p_pfc.pcunome AS nome,
        p_pfc.pcuemail AS email,
        '(' || p_pfc.pcudddnumtelefone || ') ' || p_pfc.pcunumtelefone AS telefone,
        p_pfc.pcuramaltelefone AS ramal,
        p_tfo.tfodsc AS formacao,
        CASE WHEN p_pfc.tvpid = 4 THEN p_pfc.pcuvinculo ELSE p_pfv.tvpdsc END AS vinculo,
        CASE WHEN p_pfc.pffid = 14 then p_pfc.pcufuncao ELSE p_pff.pffdescricao END AS funcao_atual,
        p_pfc.pcuorgao AS orgao,
        CASE WHEN p_pfc.pcucargoefetivo = 't' THEN 'Sim' ELSE 'Não' END AS cargo_efetivo,
        CASE WHEN p_pfc.pcuexerciciosecretariaeducacao = 't' THEN 'Sim' ELSE 'Não' END AS em_exercicio,
        w_ed.esddsc AS estado
    FROM par.instrumentounidade p_iu
        LEFT JOIN territorios.municipio t_m ON t_m.muncod = p_iu.muncod and p_iu.mun_estuf IS NOT NULL
        LEFT join territorios.estado t_e ON p_iu.estuf = t_e.estuf and p_iu.mun_estuf IS NULL
        JOIN par.pfadesaoprograma p_pf ON p_iu.inuid = p_pf.inuid
        JOIN par.pfadesao p_fa ON p_pf.pfaid = p_fa.pfaid
        JOIN par.pfcursista p_pfc ON p_pf.adpid = p_pfc.adpid
        LEFT JOIN public.tipoformacao p_tfo ON p_pfc.tfoid = p_tfo.tfoid
        JOIN public.tipovinculoprofissional p_pfv ON p_pfc.tvpid = p_pfv.tvpid
        JOIN par.pffuncao p_pff ON p_pfc.pffid = p_pff.pffid
        JOIN workflow.documento w_d  ON w_d.docid = p_iu.docid    
        JOIN workflow.estadodocumento w_ed ON w_ed.esdid = w_d.esdid
    WHERE
    	p_fa.prgid = ". PROG_PAR_INSTRUTOR_CONSELHO_ESCOLAR ."
        AND p_iu.inuid NOT IN(
            SELECT
                inuid
            FROM
                par.instrumentounidade
            WHERE (
                muncod IS NULL AND mun_estuf IS NULL) AND (estuf IS NULL)
        )
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
    $db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%','');
    exit;
}

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
        jQuery('#formulario').attr('action', 'par.php?modulo=relatorio/lista-conselho-adesao&acao=A');
        jQuery('#formulario').submit();
    }
    
    function exportarExcel(){
        jQuery('#formulario').attr('action', 'par.php?modulo=relatorio/lista-conselho-adesao&acao=A&exportarExcel=1');
        jQuery('#formulario').submit();
    }
    
    function limparFormulario(){
        window.location = 'par.php?modulo=relatorio/lista-conselho-adesao&acao=A';
    }
</script>