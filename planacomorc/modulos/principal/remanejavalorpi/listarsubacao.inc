
<?
include_once '_funcoesremanejavalorpi.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";
?>
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script src="./js/planacomorc.js"></script>
<style>
    .linkSubacao{
        font-weight: bold;
        color: #00529b;
        cursor: pointer;
    }
    .linkDetalhar{
        cursor: pointer;
        font-weight: bold;
        color: #00529b;
        font-size: 12px;
    }
    TD{
        vertical-align: top !important;
    }

</style>
<script>
    function detalheSubacao(sbaid) {
        window.open('planacomorc.php?modulo=principal/remanejavalorpi/detalhesubacao&acao=A&sbaid=' + sbaid, 'Detalhe da Subação', 'STATUS=NO, TOOLBAR=NO, LOCATION=NO, DIRECTORIES=NO, RESISABLE=NO, SCROLLBARS=YES, TOP=10, LEFT=10, WIDTH=800, HEIGHT=500');
    }

    function detalharPTRES(el, sbaid) {
        var novo = '';
        $('#mais_' + sbaid).html('');
        divCarregando();
        jQuery.ajax({
            url: 'planacomorc.php?modulo=principal/remanejavalorpi/listarsubacao&acao=A&requisicao=listaPtresSubacao&sbaid=' + sbaid,
            async: false,
            success: function(html) {
                $(el).parent().parent().after(html);
                $(el).removeAttr('onclick');
                divCarregado();
            }
        });
    }

    function remanejar(ptrid, sbaid) {
        if (!ptrid || !sbaid) {
            alert('Não existe PTRES/Subação. Entre em contato com o administrador do sistema.');
            return false;
        } else {
            location.href = '/planacomorc/planacomorc.php?modulo=principal/remanejavalorpi/remaneja&acao=A&ptrid=' + ptrid + '&sbaid=' + sbaid;
        }
    }
</script>
<?php monta_titulo("Plano Interno", 'Remanejamento de PI (por Subação)'); ?>
<br />
<?
$params['where'] = " ";
if ($_REQUEST['buscalivre']) {
    $params['where'] .=" AND  (";
    $params['where'] .= "sba.sbacod ilike '%" . $_REQUEST['buscalivre'] . "%'";
    $params['where'] .=" OR UPPER( public.removeacento(sba.sbatitulo)) ilike '%" . removeAcentos(str_replace("-"," ",$_REQUEST['buscalivre']) ) . "%'";
    $params['where'] .=")";
}
$params['where'] .= $_REQUEST["ptres"][0] ? " AND ptr.ptrid in ('".implode( "','", $_REQUEST["ptres"] )."') ":"";
$params['SELECT'] = <<<SQL
SELECT 
        '<div class=\"linkSubacao\" onclick=\"detalheSubacao(\''|| sbaid ||'\');\">'|| sbacod ||'</div>'as sbacod,
       '<spam onclick=\"detalharPTRES(this,'''||sbaid||''');\" class=\"linkDetalhar\" ><spam id=mais_'||sbaid||'>[+]</spam> '||sbatitulo||'</spam> ' as sbatitulo,
       dotacao,
       SUM(CAST(detalhado_pi AS NUMERIC)) AS detalhado_pi,
       SUM(empenhado) AS empenhado,
       SUM(dotacao) - SUM(CAST(detalhado_pi AS NUMERIC)) AS saldo_nao_detalhado,
       SUM(dotacao) - SUM(empenhado) AS saldo_nao_empenhado
SQL;
$sql  = retornaConsultaSubacao($params);

$cabecalho = array(
    "Código",
    "Subação",
    "Orçamento Atual (R$)",
    "Detalhado em PI (R$)",
    "Empenhado (R$)",
    "Não detalhado em PI (R$)",
    "Não Empenhado (R$)"
);
?>
<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
    <body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
        
        <form action="" method="post" name="form" onsubmit="divCarregando(); selectAllOptions(form.ptres);">
            <table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
                <tr style="background-color: #cccccc">
                    <td align='right' style="vertical-align:top; width:25%;">
                        Buscar
                    </td>
                    <td>
                        <? echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, ''); ?>
                    </td>
                </tr>		
                <tr>
                    <td class="SubTituloDireita" valign="top">
                        Que contenha o(s) PTRES:
                    </td>
                    <td>
                        <?
                        $obrigatorias = UNIDADES_OBRIGATORIAS;
                        if ($_REQUEST['ptres'][0]) {
                            $ptres = $db->carregar("SELECT
						pt.ptrid as codigo,
						'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| aca.acadsc as descricao
                                            FROM monitora.acao aca 
                                                    inner join monitora.ptres pt on pt.acaid=aca.acaid  
                                            WHERE aca.prgano='" . $_SESSION['exercicio'] . "' and aca.acasnrap = false
                                                AND aca.unicod IN($obrigatorias)
                                                AND pt.ptrid in (" . implode(",", $_REQUEST['ptres']) . ")
                                                AND pt.ptrano='{$_SESSION['exercicio']}'     
                                            GROUP BY codigo , descricao
                                            ORDER BY 
                                            1");
                        }
                        $sqlComboAcoes = "SELECT
						pt.ptrid as codigo,
						'(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod ||' - '|| substring(aca.acadsc  from 0 for 80) || '...' as descricao
                                            FROM monitora.acao aca 
                                                    inner join monitora.ptres pt on pt.acaid=aca.acaid  
                                            WHERE 
                                                aca.prgano='{$_SESSION['exercicio']}'
                                                AND pt.ptrano='{$_SESSION['exercicio']}'
                                                AND aca.acasnrap = false
                                                AND aca.unicod IN($obrigatorias)
                                            GROUP BY codigo , descricao
                                            ORDER BY 1";
                        #    ver($sqlComboAcoes);
                        combo_popup("ptres", $sqlComboAcoes, "Ações", "600x600", 0, array(), "", "S", false, false, 5, 400);
                        ?> 
                </td>
                </tr>
                <tr style="background-color: #cccccc">
                    <td align='right' style="vertical-align:top; width:25%;">
                    </td>
                    <td>
                        <input type="submit" name="botao" value="Pesquisar"/>
                    </td>
                </tr>		
            </table>
        </form>
        <br>
        <?php
        $db->monta_lista($sql, $cabecalho, 100, 20, 'S', 'S', 'S');
        ?>
    </body>
</html>
