<?php
/**
 * Lista de transações de remanejamento de PI.
 * $Id: listartransacoes.inc 75487 2014-02-17 12:30:34Z mariluciaqueiroz $
 */

/**
 * Funções de ajuda ao remanejamento de pi.
 * @see _funcoesremanejavalorpi.php
 */
include_once '_funcoesremanejavalorpi.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";
?>
<!--<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>-->
<!-- Modificado para atender o método $.ajax().done()-->
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="./js/planacomorc.js"></script>
<script>
    
    $(document).ready(function() { });
    
    function verTransacao(rmpid) {
        location.href = '/planacomorc/planacomorc.php?modulo=principal/remanejavalorpi/remaneja&acao=A&rmpid=' + rmpid;
    }

    function excluirTransacao(rmpid) {
        var r = confirm("Você realmente deseja excluir essa Transação ?");
        var x = '';
        if (r === true){
        
            var url = '/planacomorc/planacomorc.php?modulo=principal/remanejavalorpi/listartransacoes&acao=A';
            $.ajax({
              type: "POST",
              url: url,
              data: { requisicao: "excluiTransacaoRmp", rmpid: rmpid }
            }).done(function( msg ) {
                var str = '';
                if(msg === 'true'){
                    str = 'Transação excluída com sucesso.';
                    alert(str);
                    window.location = window.location;
                }else{
                    str = 'A transação não pode ser excluída.';
                    if(msg != 'false'){
                        str += '\n'+msg;
                    }
                    alert(str);                    
                }
            });
        }        
    }

</script>
<?php monta_titulo("Plano Interno", 'Transações de Remanejamento de PI (por PO)'); ?>
<br />
<?php
/* Seleciona o tipo de transação a ser filtrado, default 'S' (Solicitações) */
$tipoTransacao = 'S';
if ($_REQUEST['tipoTransacao']) {
    $tipoTransacao = $_REQUEST['tipoTransacao'];
}
if ($_REQUEST['buscalivre']) {
    $buscaLivreTmp = removeAcentos(str_replace("-"," ",$_REQUEST['buscalivre']));
    $whereBuscaLivre = "AND( ";
    $whereBuscaLivre .= " public.removeacento(sba.sbacod) ilike '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(ptr.ptres) ilike '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(rmp.funcprogramatica) ilike '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(usu.usunome) ilike '%" . $buscaLivreTmp . "%'";
    $whereBuscaLivre .= " OR public.removeacento(sba.sbatitulo) ilike '%". $buscaLivreTmp. "%'";
    $whereBuscaLivre .= ")";
}
if ($tipoTransacao != 'E') {
    $colunaTransacaoEfetivada = ",CASE WHEN rmp.rmpstprocessado THEN 'SIM' ELSE 'NÃO' END as processado";
}

// -- Filtrando solicitações por PERFIL, conforme unidades sob responsabilidade do GO
$perfis = pegaPerfilGeral();
$filtroUOdoRemanejamento = '';

// -- UOs associadas ao perfil do usuário
$uos = pegaUOsPerfil(PFL_GESTAO_ORCAMENTARIA);

if ('S' == $tipoTransacao) {
    // -- Apenas o GO de uma das UOs da subação pode homologar uma subação,
    // -- a menos que uma das UOs seja 26101, nesse caso, só ela homologa.
    if (in_array(PFL_GESTAO_ORCAMENTARIA, $perfis) && !$_SESSION['superuser']) {
        if (in_array('26101', $uos)) {
            $filtroUOdoRemanejamento = <<<PARCIAL_DML
    AND EXISTS (SELECT 1
                  FROM monitora.pi_subacaounidade sbu
                    left join public.unidadegestora ung USING(ungcod)
                  WHERE sbu.sbaid = sba.sbaid
                    AND COALESCE(sbu.unicod, ung.unicod)::numeric IN(%s))
PARCIAL_DML;
        } else {
            $filtroUOdoRemanejamento = <<<PARCIAL_DML
    AND EXISTS (SELECT 1
                  FROM monitora.pi_subacaounidade sbu
                    left join public.unidadegestora ung USING(ungcod)
                  WHERE sbu.sbaid = sba.sbaid
                    AND COALESCE(sbu.unicod, ung.unicod)::numeric IN(%s))
    AND NOT EXISTS (SELECT 1
                      FROM monitora.pi_subacaounidade sbu
                        left join public.unidadegestora ung USING(ungcod)
                      WHERE sbu.sbaid = sba.sbaid
                        AND COALESCE(sbu.unicod, ung.unicod) = '26101')
PARCIAL_DML;
        }
        $filtroUOdoRemanejamento = sprintf($filtroUOdoRemanejamento, implode(', ', $uos));
    }
} elseif ('H' == $tipoTransacao) {
    // -- Apenas a UO dona do PTRES é quem pode visualizá-lo e o evetivar.
    // -- Como o PTRES tem apenas uma UO, não é necessário fazer tratamento
    // -- de conjuntos como na homologação do remanejamento do PI
    if (in_array(PFL_GESTAO_ORCAMENTARIA, $perfis) && !$_SESSION['superuser']) {
        $filtroUOdoRemanejamento = <<<PARCIAL_DML
    AND ptr.unicod::numeric IN(%s)
PARCIAL_DML;
        $filtroUOdoRemanejamento = sprintf($filtroUOdoRemanejamento, implode(', ', $uos));
    }
} else {
    // -- Todas as transações do tipo E são listadas sem restrições
}

$btn_excluir = '';
if(in_array(PFL_SUPERUSUARIO, pegaPerfilGeral())){
    $btn_excluir = "|| '<a style=\"cursor:pointer\" onclick=\"excluirTransacao(''' || rmp.rmpid || ''');\">'
                    || '<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\"></a>' ";
}

if (!in_array(PFL_GABINETE, $perfis)) {
    $acao = "'<a style=\"cursor:pointer;\" onclick=\"verTransacao(\''||rmp.rmpid||'\');\">
              <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a> &nbsp;' {$btn_excluir}
              AS acao";
} else {
    $acao = "'-' as acao";
    $sqlComplementar = retornaFiltroSubacoesUoUg();
}
$sql_lista = <<<DML
SELECT {$acao},
       ptr.ptres,
       sba.sbacod || ' - ' || sba.sbatitulo,
       rmp.funcprogramatica,
       TO_CHAR(rmp.datatransacao,'dd/mm/yyyy') AS datatransacao,
       usu.usunome,
       rmp.rmpsaldosubtraido,
       rmp.rmpsaldoadicionado
       {$colunaTransacaoEfetivada}
  FROM planacomorc.remanejamentopi rmp
    INNER JOIN monitora.ptres ptr
      ON rmp.ptrid = ptr.ptrid
    INNER JOIN monitora.pi_subacao sba
      ON rmp.sbaid = sba.sbaid
    INNER JOIN seguranca.usuario usu
      ON rmp.usucpf = usu.usucpf
  WHERE rmp.tipotransacao = '{$tipoTransacao}' {$whereBuscaLivre} {$filtroUOdoRemanejamento}
    AND rmp.rmpstprocessado = FALSE
  {$sqlComplementar}
  ORDER BY rmp.rmpstprocessado,
           rmp.datatransacao DESC
DML;

$cabecalho = array(
    "",
    "PTRES",
    "Subação",
    "Funcional Programática",
    "Data da Operação",
    "Usuário",
    "Total Subtraído (R$)",
    "Total Adicionado (R$)"
);
if ($tipoTransacao != 'E') {
    $cabecalho[] = "Processada";
}
?>
<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
    <body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">

        <form action="" method="post" name="formulario">

            <table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="SubTituloDireita" align="right">Pesquisar por:</td>
                    <td>
                        <input type="radio" name="tipoTransacao" value="S" <?php if ($tipoTransacao == 'S') echo 'checked=checked'; ?> /> Para Homologação
                        <input type="radio" name="tipoTransacao" value="H" <?php if ($tipoTransacao == 'H') echo 'checked=checked'; ?> /> Para Aprovação
                        <input type="radio" name="tipoTransacao" value="E" <?php if ($tipoTransacao == 'E') echo 'checked=checked'; ?> /> Aprovado
<!--                    <input type="radio" name="tipoTransacao" value="S" <?php if ($tipoTransacao == 'S') echo 'checked=checked'; ?> /> Solicitação de Remanejamento
                        <input type="radio" name="tipoTransacao" value="H" <?php if ($tipoTransacao == 'H') echo 'checked=checked'; ?> /> Homologação de Remanejamento
                        <input type="radio" name="tipoTransacao" value="E" <?php if ($tipoTransacao == 'E') echo 'checked=checked'; ?> /> Remanejamento Efetivado-->
                    </td>
                </tr>

                <tr >
                    <td class="SubTituloDireita" align="right">Buscar:</td>
                    <td>
                        <?php echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, ''); ?>
                    </td>
                </tr>
                <tr style="background-color: #cccccc">
                    <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
                    <td>
                        <input type="submit" name="botao" value="Pesquisar" />
                    </td>
                </tr>		
            </table>
        </form>
        <br>
        <?php $db->monta_lista($sql_lista, $cabecalho, 60, 20, '', '', ''); ?>
    </body>
</html>