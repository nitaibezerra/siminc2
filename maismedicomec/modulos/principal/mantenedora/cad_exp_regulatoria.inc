<?PHP

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_SESSION['maismedicomec']['mntid'] == ''){
        $db->sucesso("principal/mantenedora/cad_mantenedora", '', "Sessão expirou. Tente novamente!");
    }
    
    verificaMantenedora();
    
    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Experiência Regulatória' );

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

?>

<?php $mntid = $_SESSION['maismedicomec']['mntid'];
$sql = "select * from maismedicomec.mantenedora where mntid = ".$mntid;
$dados_mantenedora = $db->carregar($sql);

?>
<table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
            <td><?php echo $mntid ?></td>
        </tr>
        
          <tr>
            <td class ="SubTituloDireita" width="35%">Razão social</td>
            <td><?php echo $dados_mantenedora[0][mntrazaosocial];?></td>
        </tr>
</table>

<?


/* * * Array com os itens da aba de primeiros passos ** */
$menu = array(
    0 => array("id" => 1, "descricao" => "Mantenedora", "link" => "/maismedicomec/maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantenedora"),
    1 => array("id" => 2, "descricao" => "Mantida Indicada", "link" => "/maismedicomec/maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantida")
);

/* * * Verifica qual aba está ativa ** */
switch ($_GET['aba']) {
    case 'mantida':
        $abaAtiva = "/maismedicomec/maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantida";
        $pagAtiva = "cad_exp_regulatoria2.inc";
        break;
    default:
        $abaAtiva = "/maismedicomec/maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantenedora";
        $pagAtiva = "cad_exp_regulatoria1.inc";
        break;
}

/* * * Monta o segundo conjunto de abas ** */
echo montarAbasArray($menu, $abaAtiva);

/* * * Monta a página do segundo conjunto de abas ** */
include_once $pagAtiva;

?>



