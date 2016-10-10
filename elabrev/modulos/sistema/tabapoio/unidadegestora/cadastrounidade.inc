<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Cadastro de Unidade Gestora', 'Tabela de Apoio');

function cleanUpCPF($cpf) {
    return str_replace(array('.', '-'), '', $cpf);
}

// alterar os dados
if ($_GET['execucao'] == 'alterar' && isset($_GET['ungcod'])) {
    // recupera os dados para colocar nos inputs
    $sql = "
	SELECT
	uni.unicod,
	uni.unidsc,
	ung.ungdsc,
	ung.ungcod,
	ung.ungabrev,
	ung.gescod,
	rpl.ug,
	rpl.cpf,
	rpl.nome,
	rpl.email
	FROM public.unidadegestora ung
	LEFT JOIN elabrev.representantelegal rpl ON ung.ungcod = rpl.ug
	LEFT JOIN public.unidade uni ON uni.unicod = ung.unicod
	WHERE  ung.ungcod = '{$_REQUEST['ungcod']}'";
    $rs = $db->pegaLinha($sql);

    if ($rs)
        extract($rs);

    $cpf = $cpf ? formatar_cpf($cpf) : '';
}

// salvar os dados (Salvar uma alteração)
if ($_REQUEST ['req'] == 'salvar') {

    if ($_POST) extract($_POST);

    $cpf = cleanUpCPF($cpf);
    $cpfanterior = $cpfanterior ? str_replace(array('.', '-'), '', $cpfanterior) : '';

    // recupera os dados atuais no banco para verificar se houve alteração nos dados
    $sqlG = "select ungcod, ungdsc, ungabrev, unicod, gescod from public.unidadegestora where ungcod = '{$_GET['ungcod']}'";
    $unidadegestora = $db->pegaLinha($sqlG);

    // caso haja alteração no ungcod ou ungdsc ou ungabrev será executado um update no banco
    if ($unidadegestora) {

        $ungcodB = $unidadegestora['ungcod'];
        $ungdscB = $unidadegestora['ungdesc'];
        $ungabrevB = $unidadegestora['ungabrev'];
        $unicodB = $unidadegestora['unicod'];
        $gescodB = $unidadegestora['gescod'];

        // monta os valores a serem alterados na query
        if ($ungcodB != $ungcod) {
            $set .= "ungcod = '".trim($ungcod)."',";
        }
        if ($ungdscB != $ungdsc) {
            $set .= "ungdsc	= '".trim($ungdsc)."',";
        }
        if ($ungabrevB != $ungabrev) {
            $set .= "ungabrev = '".trim($ungabrev)."',";
        }
        if ($unicodB != $unicod) {
            $set .= "unicod = '".trim($unicod)."',";
        }

        if ($gescod != $gescodB) {
            $set .= "gescod = '".trim($gescod)."',";
        }

        $setA = substr($set, 0, - 1);
        if (strlen($setA) > 0) {
            $sqlUnidadeGestora = "update public.unidadegestora set {$setA} where ungcod = '".trim($_GET['ungcod'])."'";
            $db->executar($sqlUnidadeGestora);
        }

        // recupera os dados atuais no banco para verificar se houve alteração nos dados
        $sql = "select cpf, nome, email from elabrev.representantelegal where ug = '{$_GET['ungcod']}'";
        $representanteLegal = $db->pegaLinha($sql);

        // caso haja alteração no cpf ou nome ou email será executado um update no banco
        if ($representanteLegal) {

            $cpfB = $representanteLegal['cpf'];
            $nomeB = $representanteLegal['nome'];
            $emailB = $representanteLegal['email'];

            // monta os valores a serem alterados na query
            if ($cpfB != $cpf) {
                $sett .= "cpf = '{$cpf}',";
            }
            if ($nomeB != $nome) {
                $sett .= "nome = '{$nome}',";
            }
            if ($emailB != $email) {
                $sett .= "email 	= '{$email}',";
            }
            $setR = substr($sett, 0, - 1);
            if (strlen($setR) > 0) {
                $sqlRepresentanteLegal = "update elabrev.representantelegal set {$setR} where ug = '{$ungcod}'";
                $db->executar($sqlRepresentanteLegal);
            }
        } else {
            // Caso seja um novo Representante, Insere.
            $sqlRepresentanteLegal = "
                INSERT INTO elabrev.representantelegal (cpf, nome, email, ug)
                VALUES('{$cpf}', '{$nome}', '{$email}', '{$ungcod}') ";
            $db->executar($sqlRepresentanteLegal);
        }

        if ($db->commit()) {
            $db->sucesso('sistema/tabapoio/unidadegestora/cadastrounidade', '&cpf=' . $cpf . '&ug=' . $ungcod);
        } else {
            $db->insucesso('Falha na operação!', '&cpf=' . $cpfanterior . '&ug=' . $ug, 'sistema/tabapoio/unidadegestora/cadastrounidade');
        }

    }
}

// cadastrar os dados (Cadastrar um novo registro)
if ($_REQUEST ['req'] == 'cadastrar') {

    if ($_POST) extract($_POST);

    $cpf = cleanUpCPF($cpf);
    $cpfanterior = $cpfanterior ? str_replace(array('.', '-'), '', $cpfanterior) : '';

    // recupera os dados atuais no banco para verificar se houve alteração nos dados
    $sqlG = "select ungcod, ungdsc, ungabrev, gescod from public.unidadegestora where ungcod = '{$_REQUEST['ungcod']}'";
    $unidadegestora = $db->pegaLinha($sqlG);

    if ($unidadegestora) {

        $ungcodB = $unidadegestora['ungcod'];
        $ungdscB = $unidadegestora['ungdesc'];
        $ungabrevB = $unidadegestora['ungabrev'];
        $gescodB = $unidadegestora['gescod'];

        // monta os valores a serem alterados na query
        if ($ungcodB != $ungcod) {
            $set .= "ungcod = '{$ungcod}',";
        }
        if ($ungdscB != $ungdsc) {
            $set .= "ungdsc	= '{$ungdsc}',";
        }
        if ($ungabrevB != $ungabrev) {
            $set .= "ungabrev = '{$ungabrev}',";
        }

        if ($gescod != $gescodB) {
            $set .= "gescod = '{$gescod}',";
        }
        $setA = substr($set, 0, - 1);
        if (strlen($setA) > 0) {
            $sqlUnidadeGestora = "update public.unidadegestora set {$setA} where ungcod = '{$ungcod}'";
            $db->executar($sqlUnidadeGestora);
        }
    } else {
        $sqlUnidadeGestora = "
			insert into public.unidadegestora (ungcod, ungdsc, ungabrev, unicod, gescod)
			values ('{$ungcod}','{$ungdsc}','{$ungabrev}', '{$unicod}', '{$gescod}');
			";
        $db->executar($sqlUnidadeGestora);
    }

    // recupera os dados atuais no banco para verificar se houve alteração nos dados
    $sql = "select cpf, nome, email from elabrev.representantelegal where ug = '{$_REQUEST['ungcod']}'";
    $representanteLegal = $db->pegaLinha($sql);

    if ($representanteLegal) {

        $cpfB = $representanteLegal['cpf'];
        $nomeB = $representanteLegal['nome'];
        $emailB = $representanteLegal['email'];

        // caso haja alteração no cpf ou nome ou email será executado um update no banco
        // monta os valores a serem alterados na query
        if ($cpfB != $cpf) {
            $sett .= "cpf = '{$cpf}',";
        }
        if ($nomeB != $nome) {
            $sett .= "nome = '{$nome}',";
        }
        if ($emailB != $email) {
            $sett .= "email 	= '{$email}',";
        }
        $setR = substr($sett, 0, - 1);
        if (strlen($setR) > 0) {
            $sqlRepresentanteLegal = "update elabrev.representantelegal set {$setR} where ug = '{$ungcod}'";
            $db->executar($sqlRepresentanteLegal);
        }

    } else {
        $sqlRepresentanteLegal = "
			insert into elabrev.representantelegal (cpf, nome, email, ug, status)
			values ('{$cpf}','{$nome}','{$email}','{$ungcod}','A');
		";
        $db->executar($sqlRepresentanteLegal);
    }

    if ($db->commit()) {
        $db->sucesso('sistema/tabapoio/unidadegestora/cadastrounidade', '&cpf=' . $cpf . '&ug=' . $ungcod);
    } else {
        $db->insucesso('Falha na operação!', '&cpf=' . $cpfanterior . '&ug=' . $ug, 'sistema/tabapoio/unidadegestora/cadastrounidade');
    }
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
    $(function() {
        $('.cancelar').click(function() {
            window.location = 'elabrev.php?modulo=sistema/tabapoio/unidadegestora/cadastrounidade&acao=A';
        });

        $('.limpar').click(function() {
            window.location = 'elabrev.php?modulo=sistema/tabapoio/unidadegestora/cadastrounidade&acao=A';
        });
        $('.alterar').click(function() {
            id = this.id.split('_');
            document.location = 'elabrev.php?modulo=sistema/tabapoio/unidadegestora/cadastrounidade&acao=A&execucao=alterar&ungcod=' + id;
        });
        $('.pesquisar').click(function() {
            if ($('#unicod option:selected').val() == '' && $('[name=ungcod]').val() == '' && $('[name=ungabrev]').val() == '' && $('[name=email]').val() == '' && $('[name=ungdsc]').val() == '' && $('[name=cpf]').val() == '' && $('[name=nome]').val() == '') {
                alert('Preencha pelo menos um dos campos da pesquisa!');
                return false;
            }
            $('#formulario').submit();
        });
        $('.salvar').click(function() {
            if ($('[name=cpf]').val() == '') {
                alert('O campo CPF é obrigatório!');
                $('[name=cpf]').focus();
                return false;
            }
            if ($('[name=nome]').val() == '') {
                alert('O campo NOME é obrigatório!');
                $('[name=nome]').focus();
                return false;
            }
            if ($('[name=email]').val() == '') {
                alert('O campo E-MAIL é obrigatório!');
                $('[name=email]').focus();
                return false;
            }
            if ($('[name=ug]').val() == '') {
                alert('Não existe nenhuma UG selecionada!');
                $('[name=ug]').focus();
                return false;
            }
            $('#req').val('salvar');
            $('#formulario').submit();
        });
        $('.cadastrar').click(function() {
            if ($('[name=cpf]').val() == '') {
                alert('O campo CPF é obrigatório!');
                $('[name=cpf]').focus();
                return false;
            }
            if ($('[name=nome]').val() == '') {
                alert('O campo NOME é obrigatório!');
                $('[name=nome]').focus();
                return false;
            }
            if ($('[name=email]').val() == '') {
                alert('O campo E-MAIL é obrigatório!');
                $('[name=email]').focus();
                return false;
            }
            if ($('[name=ug]').val() == '') {
                alert('Não existe nenhuma UG selecionada!');
                $('[name=ug]').focus();
                return false;
            }
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
            <td class="subtituloDireita">Unidade Orçamentaria</td>
            <td><?php
$sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
WHERE uni.unistatus = 'A'
    {$whereUO}
ORDER BY uni.unicod
DML;

$db->monta_combo('unicod', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'N', 'unicod', null, '', null, 'style="width=100%;" id="unicod"', null, (isset($unicod) ? $unicod : null));
?> </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Código da UG</td>
            <td><?php echo campo_texto('ungcod', 'N', 'S', '', 15, '', '', ''); ?></td>
        </tr>
        <tr>
            <td class="subtituloDireita">Nome da UG</td>
            <td><?php echo campo_texto('ungdsc', 'N', 'S', '', 45, '100', '', ''); ?></td>
        </tr>

        <tr>
            <td class="subtituloDireita">Abreviação da UG</td>
            <td><?php echo campo_texto('ungabrev', 'N', 'S', '', 45, '20', '', ''); ?></td>
        </tr>

        <tr>
            <td class="subtituloDireita">CPF do Responsável</td>
            <td><?php echo campo_texto('cpf', 'N', 'S', '', 25, '', '###.###.###-##', ''); ?></td>
        </tr>
        <tr>
            <td class="subtituloDireita">Nome do Responsável</td>
            <td><?php echo campo_texto('nome', 'N', 'S', '', 45, '', '', ''); ?></td>
        </tr>

        <tr>
            <td class="subtituloDireita">Email do Responsável</td>
            <td><?php echo campo_texto('email', 'N', 'S', '', 45, '', '', ''); ?></td>
        </tr>

        <tr>
            <td class="subtituloDireita">Codigo da Gestão</td>
            <td><?php echo campo_texto('gescod', 'N', 'S', '', 15, '5', '', ''); ?></td>
        </tr>

        <tr>
            <td class="subtituloDireita">&nbsp;</td>
            <td>
                <?php if ($ungcod) { ?>
                    <input type="button" value=Salvar class="salvar" />
                    <input type="button" value="Cancelar" class="cancelar" />
                <?php } else { ?>
                    <input type="button" value="Cadastrar" class="cadastrar" />
                    <input type="button" value="Pesquisar" class="pesquisar" />
                    <input type="button" value="Limpar" class="limpar" />
                <?php } ?>
            </td>
        </tr>
    </table>
</form>
<?php

if ($_POST['unicod'])
    $where [] = "ung.unicod = '{$_POST['unicod']}'";

if ($_POST['ungcod'])
    $where [] = "ung.ungcod = '{$_POST['ungcod']}'";

if ($_POST['ungabrev'])
    $where [] = "ung.ungabrev = '{$_POST['ungabrev']}'";

if ($_POST['email'])
    $where [] = "rpl.email = '{$_POST['email']}'";

if ($_POST['ungdsc'])
    $where [] = "(ung.ungdsc ILIKE '%{$_POST['ungdsc']}%' OR removeacento(ung.ungdsc) ILIKE removeacento('%{$_POST['ungdsc']}%'))";

if ($_POST['cpf'])
    $where [] = "rpl.cpf = '" . str_replace(array('.', '-'), '', $_POST['cpf']) . "'";

if ($_POST['nome'])
    $where [] = "(rpl.nome ILIKE '%{$_POST['nome']}%' OR removeacento(rpl.nome) ILIKE removeacento('%{$_POST['nome']}%'))";

$sql = "
SELECT 
	'
	<img src=\"../imagens/alterar.gif\" border=\"0\" id=\"'||ung.ungcod||'\" class=\"alterar\" style=\"cursor:pointer\" />
	' as acao,
	uni.unicod,
	uni.unidsc,
	ung.ungcod,
	ung.ungdsc,
	rpl.cpf,	
	rpl.nome,
	rpl.email
FROM public.unidadegestora ung
LEFT JOIN  elabrev.representantelegal rpl ON ung.ungcod = rpl.ug
LEFT JOIN public.unidade uni ON ung.unicod = uni.unicod
" . ($where ? " WHERE " . implode(" AND ", $where) : "WHERE uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "'") . "
ORDER BY ung.unicod
";
$arCabecalho = array(
    'Ação',
    'Código da UO',
    'Unidade Orçamentária',
    'Código da UG',
    'Unidade Gestora',
    'CPF do Representante Legal',
    'Nome do Representante Legal',
    'E-mail do Representante Legal'
);

$db->monta_lista($sql, $arCabecalho, 50, 10, 'N', '', '', '', '');
