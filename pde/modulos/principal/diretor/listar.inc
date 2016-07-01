<?php
global $db;

include_once( APPRAIZ . "pde/classes/Enemdiretor.class.inc" );

$modelDiretor = new EnemDiretor($_REQUEST['ediid']);

if ($_REQUEST['excluir']) {
    $sql = "delete from pde.enemservidor where ediid = '{$modelDiretor->ediid}'";
    $modelDiretor->carregar($sql);

    $modelDiretor->excluir();
    $modelDiretor->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location = 'enem.php?modulo=principal/diretor/listar&acao=A';
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
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">CPF do Diretor:</td>
            <td><?php echo campo_texto('usucpf','','','',16,14,'###.###.###-##','', '', '', '', '', '', '', "this.value=mascaraglobal('###.###.###-##',this.value);");?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Nome do Diretor:</td>
            <td><?php echo campo_texto('usunome', 'N', 'S', 'Nome', 30, 100, '', '', '', '', '', 'id="usunome"'); ?></td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Descrição:</td>
            <td><?php echo campo_texto('usudescricao', 'N', 'S', 'Nome', 50, 30, '', '', '', '', '', 'id="usudescricao"'); ?></td>
        </tr>
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
                <input type="button" name="botao_enviar" id="botao_enviar" value="Buscar" />
                <input type="button" name="botao_novo" id="botao_novo" value="Cadastrar Novo" />
            </td>
        </tr>
    </table>
</form>

<?php
$where = array();
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
            '<a style=\"margin: 0 -5px 0 5px;\" href=\"enem.php?modulo=principal/diretor/formulario&acao=A&ediid=' || ediid || '\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\"></a>'
            || ' <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"javascript:excluir('|| ediid || ');\" ><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>'
            ediid,
            u.usunome,
            u.usucpf,
            usudescricao
        from pde.enemdiretor d
            inner join seguranca.usuario u on u.usucpf = d.usucpf ";

        $sql .= count($where) ? ' where ' . implode(' AND ', $where) : '';
        $sql .= " order by u.usunome ";

$cabecalho = array("Ação", "Nome", "CPF", "Descrição");
$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '');
?>

<script type="text/javascript">
    jQuery(function() {
        jQuery('#botao_enviar').click(function() {
            jQuery('#formulario').submit();
        });

        jQuery('#botao_novo').click(function() {
            window.location = 'enem.php?modulo=principal/diretor/formulario&acao=A';
        });
    });

    function excluir(id)
    {
        if (confirm('Deseja excluir este registro?')) {
            window.location.href = "enem.php?modulo=principal/diretor/listar&acao=A&excluir=1&ediid=" + id
        }
    }
</script>