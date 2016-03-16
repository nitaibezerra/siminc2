<?php
/**
 * $Id: popupptres.inc 87021 2014-09-18 19:12:45Z lindalbertofilho $
 */
/**
 *
 */
require_once APPRAIZ . "www/planacomorc/_funcoes.php";
require_once APPRAIZ . "www/planacomorc/_constantes.php";
require_once APPRAIZ . "www/planacomorc/_funcoespi.php";

if (isAjax() && isset($_POST['prgcod']) && isset($_POST['comboacao'])) {

    $sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
            FROM monitora.acao ac 
            WHERE 
                ac.unicod IS NOT NULL AND 
                ac.prgano = '" . $_SESSION['exercicio'] . "' AND 
                ac.acastatus = 'A' AND 
                ac.acasnrap = false AND
                ac.prgcod = '" . (int) $_POST['prgcod'] . "'
            GROUP BY ac.acacod, ac.acadsc 
            ORDER BY ac.acacod";
    $db->monta_combo('acacod', $sql, 'S', 'Selecione', '', '', '', 400);
    exit;
}


// verificando se é undidade ou unidade gestora
$where .= $_REQUEST['sbaid'] ? "AND dt.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = " . $_REQUEST['sbaid'] . ")" : '';
$where .= $_POST['prgcod'] ? "AND UPPER(dtl.prgcod) LIKE('%" . strtoupper($_POST['prgcod']) . "%')" : '';
$where .= $_POST['acacod'] ? "AND UPPER(dtl.acacod) LIKE('%" . strtoupper($_POST['acacod']) . "%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%" . $_POST['buscalivre'] . "%') OR dtl.ptres ilike '%" . $_POST['buscalivre'] . "%')" : '';

// -- Query utilizada também em: simec/www/planacomorc/_funcoespi.php

/* Parametros para montar a consulta */
$params['where'] = $where;

$params['obrigatorio'] = $_REQUEST['obrigatorio'];
if($_REQUEST['tipo'] == 'pi'){    
$params['SELECT'] = <<<SQL
SELECT '<input type="checkbox" id="chk_'||dtl.ptres||'" onclick="resultado(0,this,'''||dtl.ptrid||''','''||dtl.ptres||''')">' AS checkbox,
       '<div class="linkSubacao" onclick="detalhePtres('''|| dtl.ptrid ||''')">'|| dtl.ptres ||'</div>' AS sbacod,
       trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
       uni.unidsc,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) AS dotacaoinicial,
       COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
       -- dotacaoinicial - det_subacao
       (COALESCE(SUM(dtl.ptrdotacao), 0.00) - 
           COALESCE(SUM(dt.valor), 0.00)) AS nao_det_subacao,
       --COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
       -- det_subacao - det_pi
       --(COALESCE(SUM(dt.valor), 0.00) - COALESCE(SUM(dt2.valorpi), 0.00)) AS nao_det_pi,
       COALESCE((pemp.total), 0.00) AS empenhado,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado
SQL;
}else{
$params['SELECT'] = <<<SQL
SELECT '<input type="checkbox" id="chk_'||dtl.ptres||'" onclick="resultado(1,this,'''||dtl.ptrid||''','''||dtl.ptres||''')">' AS checkbox,
       '<div class="linkSubacao" onclick="detalhePtres('''|| dtl.ptrid ||''')">'|| dtl.ptres ||'</div>' AS sbacod,
       trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
       uni.unidsc,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) AS dotacaoinicial,
       COALESCE(SUM(dt.valor), 0.00) AS det_subacao,
       -- dotacaoinicial - det_subacao
       (COALESCE(SUM(dtl.ptrdotacao), 0.00) - 
           COALESCE(SUM(dt.valor), 0.00)) AS nao_det_subacao,
       COALESCE(SUM(dt2.valorpi), 0.00) AS det_pi,
       -- det_subacao - det_pi
       (COALESCE(SUM(dt.valor), 0.00) - COALESCE(SUM(dt2.valorpi), 0.00)) AS nao_det_pi,
       COALESCE((pemp.total), 0.00) AS empenhado,
       COALESCE(SUM(dtl.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado
SQL;
}
$sql_lista = retornaConsultaPTRES($params);
#ver($sql_lista);
?>
<html>
    <head>
        <script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
        <link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
        <script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
        <style>
            .linkSubacao{font-weight:bold;color:#00529b;cursor:pointer}
            *{font-family:Arial,verdana;font-size:11px}
            .SubTituloDireita{width:300px;font-weight:bold;text-align:right;vertical-align:top}
        </style>
         <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
        <div id="dialog" title="Dados do PTRES">

    </div>

    <body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
        <br />
        <?php monta_titulo('Seleção de PTRES', ''); ?>
        <form action="" method="post" name="formulario">
            <input type="hidden" name="obrigatorio" id="obrigatorio" value="<?php echo $_REQUEST['obrigatorio']; ?>" />
            <table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="SubTituloDireita" align="right">Programa:</td>
                    <td>
                        <?
                        $sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
			FROM monitora.programa p
			WHERE p.prgano = '" . $_SESSION['exercicio'] . "' ORDER BY prgcod";
                        $db->monta_combo('prgcod', $sql, 'S', 'Selecione', 'carregaAcao', '', '', 400);
                        ?>
                    </td>
                </tr>	
                <tr>
                    <td class="SubTituloDireita" align="right">Ação:</td>
                    <td class="comboAcao">
                        <?
                        $sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
 			FROM monitora.acao ac 
 			WHERE ac.unicod IS NOT NULL AND ac.prgano = '" . $_SESSION['exercicio'] . "' AND ac.acastatus = 'A' AND ac.acasnrap = false  
 			GROUP BY ac.acacod, ac.acadsc 
 			ORDER BY ac.acacod";
                        $db->monta_combo('acacod', $sql, 'S', 'Selecione', '', '', '', 400);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" align="right">Buscar:</td>
                    <td>
                        <? echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, ''); ?>
                    </td>
                </tr>
                <tr style="background-color: #cccccc">
                    <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
                    <td>
                        <input type="submit" name="botao" value="Pesquisar"/>
                        <input type="button" name="close" value="Fechar" onclick="window.close();">	
                    </td>
                </tr>		
            </table>

        </form>
        <br>
        <?php
        if($_REQUEST['tipo'] == 'pi'){    
        $cabecalho = array(
            " ",
            "PTRES",
            "Ação",
            "Unidade Orçamentária",
            "Dotação&nbsp;Atual&nbsp;(R$)",
            array('label' => 'Subação (R$)', 'colunas' => array('Detalhado', 'Não&nbsp;Detalhado')),
            array('label' => 'Empenho (R$)', 'colunas' => array('Empenhado', 'Não&nbsp;Empenhado'))
        );
        }else{
            $cabecalho = array(
            " ",
            "PTRES",
            "Ação",
            "Unidade Orçamentária",
            "Dotação&nbsp;Atual&nbsp;(R$)",
            array('label' => 'Subação (R$)', 'colunas' => array('Detalhado', 'Não&nbsp;Detalhado')),
            array('label' => 'PI (R$)', 'colunas' => array('Detalhado', 'Não&nbsp;Detalhado')),
            array('label' => 'Empenho (R$)', 'colunas' => array('Empenhado', 'Não&nbsp;Empenhado'))
        );
        }
        $db->monta_lista($sql_lista, $cabecalho, 60, 20, '', '', '');
        ?>
        <script type="text/javascript">

            function carregaAcao(prgcod) {
                jQuery.ajax({
                    url: 'monitora.php?modulo=principal/planotrabalhoUG/listarProgramaUG&acao=A'
                    , type: 'POST'
                    , data: {prgcod: prgcod, comboacao: true}
                    , success: function(data) {
                        $(".comboAcao").html(data);
                    }
                });
            }

            /* CARREGANDO OS DADOS DE PTRES */
            var tabelaorigem = window.opener.document.getElementById('orcamento');
            for (i = 2; i < tabelaorigem.rows.length - 2; i++) {
                if (document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML)) {
                    document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML).checked = true;
                }
            }
            /* FIM CARREGANDO OS DADOS DE PTRES */

            function resultado(pi,dados, acaid, ptres)
            {
                if (!ptres) {
                    alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
                    return false;
                }

                if (dados.checked) {
                    var linhaTbl = dados.parentNode.parentNode;
                    var tabelaorigem = window.opener.document.getElementById('orcamento');
                    if (eval(tabelaorigem.rows.length % 2)) {
                        var cor = "";
                    } else {
                        var cor = "#DCDCDC";
                    }

                    // -- Criando a nova linha na tabela da página de origem
                    var linha = tabelaorigem.insertRow(3);
                    linha.id = "ptres_" + ptres;
                    linha.style.backgroundColor = cor;
                    linha.style.height = '30px';

                    // -- O número da primeira linha da tabela onde serão inseridos os novos dados
                    var linha = 3;

                    // setando o ptres
                    var celPTRES = tabelaorigem.rows[linha].insertCell(0);
                    celPTRES.style.textAlign = "center";
                    celPTRES.innerHTML = ptres;

                    var celAcao = tabelaorigem.rows[linha].insertCell(1);
                    celAcao.style.textAlign = "center";
                    celAcao.innerHTML = linhaTbl.cells[2].innerHTML + "<input type='hidden' name='acaid[" + ptres + "]' value='" + acaid + "'>";

                    var celDotAtual = tabelaorigem.rows[linha].insertCell(2);
                    celDotAtual.style.textAlign = "right";
                    celDotAtual.style.color = linhaTbl.cells[4].style.color;
                    celDotAtual.innerHTML = linhaTbl.cells[4].innerHTML;

                    var celSubDet = tabelaorigem.rows[linha].insertCell(3);
                    celSubDet.style.color = linhaTbl.cells[5].style.color;
                    celSubDet.style.textAlign = "right";
                    celSubDet.innerHTML = linhaTbl.cells[5].innerHTML;

                    var celSubNaoDet = tabelaorigem.rows[linha].insertCell(4);
                    celSubNaoDet.style.color = linhaTbl.cells[6].style.color;
                    celSubNaoDet.style.textAlign = "right";
                    celSubNaoDet.innerHTML = linhaTbl.cells[6].innerHTML;
                    //        celSubNaoDet.innerHTML = "<a href=javascript:detfin('"+ptres+"')>"+linhaTbl.cells[6].innerHTML+"</a>";

                    if(pi == '0' ){
                      var celPIDet = tabelaorigem.rows[linha].insertCell(5);
                    celPIDet.style.color = linhaTbl.cells[7].style.color;
                    celPIDet.style.textAlign = "right";
                    celPIDet.innerHTML = linhaTbl.cells[7].innerHTML + "<input type='hidden' name='plivalor[" + ptres + "][" + acaid + "]' size='28'"
                            + "data-ptres='" + ptres + "' value ='0.00' />";
                    
                    }else if(pi == '1'){
                        
                    var celPIDet = tabelaorigem.rows[linha].insertCell(5);
                    celPIDet.style.color = linhaTbl.cells[7].style.color;
                    celPIDet.style.textAlign = "right";
                    celPIDet.innerHTML = linhaTbl.cells[7].innerHTML;
                        
                    var celPINaoDet = tabelaorigem.rows[linha].insertCell(6);
                    celPINaoDet.style.color = linhaTbl.cells[8].style.color;
                    celPINaoDet.style.textAlign = "right";
                    celPINaoDet.innerHTML = linhaTbl.cells[8].innerHTML;

                    var celEmpenhado = tabelaorigem.rows[linha].insertCell(7);
                    celEmpenhado.style.color = linhaTbl.cells[9].style.color;
                    celEmpenhado.style.textAlign = "right";
                    celEmpenhado.innerHTML = linhaTbl.cells[9].innerHTML;

                    var celNaoEmpenhado = tabelaorigem.rows[linha].insertCell(8);
                    celNaoEmpenhado.style.color = linhaTbl.cells[10].style.color;
                    celNaoEmpenhado.style.textAlign = "right";
                    celNaoEmpenhado.innerHTML = linhaTbl.cells[10].innerHTML;

                    var celVal = tabelaorigem.rows[linha].insertCell(9);
                    celVal.style.textAlign = "center";
                    celVal.innerHTML = '<input type="text" name="plivalor[' + ptres + '][' + acaid + ']" size="28" '
                            + 'onKeyUp="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);calculovalorPI()" '
                            + 'onBlur="return verificaDisponivel(this,\'' + ptres + '\',\'0\')" '
                            + 'data-ptres="' + ptres + '" '
                            + 'class="normal somar" onmouseover="MouseOver(this)" onfocus="MouseClick(this);this.select()" '
                            + 'onmouseout="MouseOut(this)" onblur="MouseBlur(this)" style="text-align:right;width:20ex" />';
                }
                } else {
                    var tabelaorigem = window.opener.document.getElementById('orcamento');
                    
                    tabelaorigem.deleteRow(window.opener.document.getElementById('ptres_' + ptres).rowIndex);
                    window.opener.calculovalorPI();
                    //jogando ids desmarcados para verificar se existem no banco, se sim deletar vinculo.
                    //window.opener.ptresApagar(acaid);
                }

            }
            
            function detalhePtres(ptrid) {
                url = 'planacomorc.php?modulo=principal/ptres/detalheptresmodal&acao=A&ptrid=' + ptrid;
                if ($('#obrigatorio').val() == 'n') {
                    url = 'planacomorc.php?modulo=principal/unidade/detalheptresmodal&acao=A&ptrid=' + ptrid;
                }
                jQuery.ajax({
                    url:url,
                            async: false,
                    success: function(html) {
                        jQuery("#dialog").html(html);
                        jQuery("#dialog").dialog({
                            modal: true,
                            width: 900,
                            height: 400
                        });
                    }
                });
            }
        </script>
    <div id="dialog" />
    </body>
</html>

