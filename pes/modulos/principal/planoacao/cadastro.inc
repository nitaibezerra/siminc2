<?php
$controllerAcao = new Controller_Acao();
$action = (isset( $_REQUEST['action'] )) ? $_REQUEST['action'] : null;
switch ($action) :
    case 'salvar':
        echo $controllerAcao->salvarAction();
        exit;
        break;
    case 'formulario':
        echo $controllerAcao->formularioAction();
        exit;
        break;
    case 'listarAcao':
        echo $controllerAcao->listarAcaoAction();
        exit;
        break;
    case 'alterarSituacaoAcao':
        echo $controllerAcao->alterarSituacaoAcaoAction();
        exit;
        break;
    case 'excluir':
        echo $controllerAcao->excluirAction();
        exit;
        break;
    case 'sugestao':
        echo $controllerAcao->listarSugestoesAction();
        exit;
        break;
    default:

        global $db;

        $controllerGeral = new Controller_Geral();

        //Chamada de programa
        include APPRAIZ . "includes/cabecalho.inc";

        // Exibindo barra com os dados recursivos da entidade.
        $controllerEntidade = new Controller_Usuario();
        echo $controllerEntidade->barraEntidadeUsuarioAction();
        print '<br/>';
        ?>
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2" style="text-align: center;" bgcolor="#e7e7e7">
            <tr>
                <td>
                    <h1>Plano de Ações</h1>
                    <p class="aviso">Selecione uma despesa para realizar a pesquisa.</p>
                </td>
            </tr>
            <tr>
                <td class="container_form">
                    <?php echo $controllerAcao->formularioAction(); ?>
                </td>
            </tr>
            <tr class="container_list" bgcolor="#e7e7e7" >
            </tr>
        </table>
        <?php
        break;
endswitch;
?>
