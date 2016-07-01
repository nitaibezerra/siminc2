<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Cadastro de Unidade Gestora', 'Tabela de Apoio');

// cadastrar os dados (Cadastrar um novo registro)
if ($_REQUEST ['req'] == 'cadastrar') {
    if ($_POST) extract($_POST);
    $ungcod = trim($ungcod);

    if ($ungcod) {
        $sql = "select ungcod from contratos.hospitalug where ungcod = '$ungcod'";
        $rs = $db->carregar($sql);
        if ($rs[0]['ungcod']) {
            alert("Unidade Gestora já encontra-se cadastrada ou inexistente por favor digite outro código de Unidade Gestora");
        } else {
            $sql = "insert into contratos.hospitalug (hspid, ungcod) VALUES ('$hspid', '$ungcod')";
            $db->executar($sql);
            $db->commit();
            alert("Unidade Gestora cadastrada com sucesso");
        }
    } else {
        alert("Por favor digite o código da unidade gestora");
    }


}

if ($_GET['excluir']) {
    $sql = "delete from contratos.hospitalug where hugid={$_GET['excluir']}";
    $db->executar($sql);
    $db->commit();
}

if ($_REQUEST['req'] == 'pesquisar') {
    if ($_REQUEST['hspid']) {
        $where = " AND hospitalug.hspid = {$_REQUEST['hspid']}";
    }

    if ($_REQUEST['ungcod']) {
        $where = " AND hospitalug.ungcod = '{$_REQUEST['ungcod']}'";
    }

    if ($_REQUEST['hspid'] && $_REQUEST['ungcod']) {
        $where = " AND hospitalug.ungcod = '{$_REQUEST['ungcod']}' AND hospitalug.hspid = {$_REQUEST['hspid']}";
    }
}

?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript">
        $(function() {
            $('.pesquisar').click(function() {
                $('#req').val('pesquisar');
                $('#formulario').submit();
            });

            $('.cadastrar').click(function() {
                if ($('[name=cpf]').val() == '') {
                    alert('O campo CPF é obrigatório!');
                    $('[name=cpf]').focus();
                    return false;
                }
                if ($('[name=ug]').val() == '') {
                    alert('Não existe nenhuma UG selecionada!');
                    $('[name=ug]').focus();
                    return false;
                }
                prepara_formulario();
                $('#req').val('cadastrar');
                $('#formulario').submit();
            });

        });
    </script>
    <form id="formulario" name="formulario" method="post" action="">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
               align="center">
            <input type="hidden" name="req" id="req">
            <tr>
                <td class="subtituloDireita">Hospital</td>
                <td><?php

                    $sql =  "select hspid as codigo, hspdsc as descricao from contratos.hospital order by hspdsc";

                    $db->monta_combo('hspid', $sql, 'S', 'Selecione um Hospital', null, null, null, '460', 'N', 'hspid', null, '', null, 'id="hspid"', null, (isset($hspid) ? $hspid : null));
                    ?> </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Código da Unidade Gestora</td>
                <td><?php
                    $sql = "select ungcod as codigo, ungdsc as descricao from public.unidadegestora order by ungdsc";
                    //$db->monta_combo('ungcod', $sql, 'S', 'Selecione uma UG', null, null, null, '460', 'N', 'ungcod', null, '', null, 'id="ungcod"', null, (isset($unicod) ? $ungcod : null));

                    echo campo_texto('ungcod', 'S', 'S', 'Código da Unidade Gestora', 10, 20, '', '', 'left', '',  0, 'id="ungcod"' );

                    ?></td>
            </tr>

            <tr>
                <td class="subtituloDireita">&nbsp;</td>
                <td>
                    <input type="button" value="Cadastrar" class="cadastrar" />
                    <input type="button" value="Pesquisar" class="pesquisar" />
                </td>
            </tr>
        </table>
    </form>
<?php

$endereco = "contratos.php?modulo=sistema/tabelasapoio/contratanteUG&acao=A";
$sql = "
SELECT 
'
<a href=\"$endereco&excluir='||hospitalug.hugid||'\"><img src=\"../imagens/excluir.gif\" border=\"0\" id=\"'||unidadegestora.ungcod||'\" class=\"alterar\" style=\"cursor:pointer\" /></a>
' as acao,
  hospital.hspdsc,
  hospital.hspabrev,
  unidadegestora.ungcod,
  unidadegestora.ungdsc
FROM
  contratos.hospital,
  contratos.hospitalug,
  public.unidadegestora
WHERE
  hospitalug.ungcod = unidadegestora.ungcod AND
  hospitalug.hspid = hospital.hspid $where";

$arCabecalho = array(
    'Ação',
    'Hospital',
    'Abreviatura',
    'Código da UG',
    'Unidade Gestora'
);

$db->monta_lista($sql, $arCabecalho, 50, 10, 'N', '', '', '', '');