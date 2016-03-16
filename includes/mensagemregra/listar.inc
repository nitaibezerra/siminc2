<?php
global $db;

include_once( APPRAIZ . "seguranca/classes/MensagemRegra.class.inc" );
include_once( APPRAIZ . "seguranca/classes/MensagemCampo.class.inc" );

$modelMensagemRegra = new MensagemRegra($_REQUEST['mreid']);

if ($_REQUEST['excluir']) {
    $modelMensagemCampo = new MensagemCampo();
    $modelMensagemCampo->excluirVarios("mreid = '{$modelMensagemRegra->mreid}'");

    $modelMensagemRegra->excluir();
    $modelMensagemRegra->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = '?modulo=sistema/mensagemregra/listar&acao=A';
		 </script>");
}

//$controllerGeral = new Controller_Geral();
?>

<link href="css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="js/jquery-1.9.1.js"></script>
<script src="js/jquery-ui-1.10.3.custom.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>
<?php
extract($_REQUEST);

include APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');
?>

<form action="" method="post" name="formulario" id="formulario">
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td style="text-align: center;">
                <input type="button" name="botao_novo" id="botao_novo" value="Cadastrar Novo" />
            </td>
        </tr>
    </table>
</form>

<?php
$where = array(0 => ' r.sisid = ' . $_SESSION['sisid']);
if ($_POST['usucpf']) {
    $where[] = " u.usucpf like ('%" . str_replace(array('.', ',', '-', ' '), '', $_POST['usucpf']) . "%') ";
}
if ($_POST['usunome']) {
    $where[] = " u.usunome ilike ('%{$_POST['usunome']}%') ";
}
if ($_POST['usudescricao']) {
    $where[] = " d.usudescricao ilike ('%{$_POST['usudescricao']}%') ";
}

$sql = "select
            '<a style=\"margin: 0 -5px 0 5px;\" href=\"?modulo=sistema/mensagemregra/formulario&acao=A&mreid=' || mreid || '\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\"></a>'
            || ' <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"javascript:excluir('|| mreid || ');\" ><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>' as acao,
            mredescricao,
            case
                when mretipo = 'A' then 'Solicitação de acesso'
                when mretipo = 'W' then 'Workflow'
                else mretipo
            end as mretipo,
            case
                when mrestatus = 'A' then 'Ativo'
                when mrestatus = 'I' then 'Inativo'
                else mrestatus
            end as mrestatus
        from seguranca.mensagemregra r
        ";
//ver($sql, d);
        $sql .= count($where) ? ' where ' . implode(' AND ', $where) : '';
//        $sql .= " order by u.usunome ";

$cabecalho = array("Ação", "Descrição", "Tipo", "Status");
$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '');
?>

<script type="text/javascript">
    jQuery(function() {
        jQuery('#botao_enviar').click(function() {
            jQuery('#formulario').submit();
        });

        jQuery('#botao_novo').click(function() {
            window.location = '?modulo=sistema/mensagemregra/formulario&acao=A';
        });
    });

    function excluir(id)
    {
        if (confirm('Deseja excluir este registro?')) {
            window.location.href = "?modulo=sistema/mensagemregra/listar&acao=A&excluir=1&mreid=" + id
        }
    }
</script>