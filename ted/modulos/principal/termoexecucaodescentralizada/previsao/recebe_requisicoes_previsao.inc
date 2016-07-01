<?php

//Captura a requisição com solicitação de exclusão de registro
if (!Ted_Utils_Model::isAjax() && isset($_GET['proid']) && isset($_GET['ted'])) {

    $prevOrcamentaria->carregarPorId($_GET['proid']);
    $prevOrcamentaria->popularDadosObjeto(array(
        'proid' => $_GET['proid'],
        'prostatus' => 'I'
    ));

    if ($prevOrcamentaria->atualizarPrevisaoOrcamentaria()) {
        $message = 'Registro atualizado com sucesso!';
    } else {
        $message = 'Falha ao tentar atualizar o registro, tente novamente.';
    }

    $url = $urlPage.'&ted='.$_GET['ted'];
    Ted_Utils_Model::reloadPage($message, $url);
    die;
}


/**
 * Trata todas as chamadas Ajax
 */
if (Ted_Utils_Model::isAjax()) {

    //Captura requisição de submição do formulário principal da tela
    if (isset($_POST['submit']) && $_POST['submit']) {
        unset($_POST['submit']);

        //Em caso de sucesso persiste os dados no modelo
        if ($previsao->isValid($_POST)) {
            unset($_POST['programaTrabalho']);
            $prevOrcamentaria->gravarPrevisaoOrcamentaria($_POST);
            $previsao->reset();
            Ted_Utils_Model::reloadPage('Registro salvo com sucesso!', $urlPage.'&ted='.$_GET['ted']);
            die;
        } else {
            //Em caso de critica renderiza o form com os erros
            $previsao->populate($_POST);
            echo $previsao->showForm();
            echo '<script type="text/javascript">
                    if ($("#ptrid").val()) {
                        _getProgramaTrabalho('.$_POST['programaTrabalho'].');
                        setTimeout(function() {
                            $("#planoInterno").val('.$_POST['pliid'].');
                        }, 200);
                    }
                  </script>';
        }
        die;
    }

    //Captura requisição para adicionar ou editar registros de previsão orçamentárias
    if (isset($_GET['edit'])) {

        if (is_numeric($_GET['edit'])) {
            $dadosPrevisao = $prevOrcamentaria->get($_GET['edit']);
            $dadosPrevisao['programaTrabalho'] = $dadosPrevisao['ptrid'];

            if (is_array($dadosPrevisao) && count($dadosPrevisao)) {
                $previsao->populate($dadosPrevisao);
                echo $previsao->showForm();
                echo '<script type="text/javascript">
                        _getProgramaTrabalho($("#programaTrabalho").val());
                        setTimeout(function() {
                            $("#planoInterno").val('.$dadosPrevisao['pliid'].');
                            $("#provalor").focus().blur();
                        }, 200);
                    </script>';
            }
            die;

        } elseif (!is_numeric($_GET['edit'])) {
            $previsao->reset();
            $previsao->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());
            echo $previsao->showForm();
            die;
        }
    }

    //Chamada para resetar o formulário de cadastro de nota de crédito
    //Devido ao bug do "remote" do bootstrap (so funciona na primeira chamada
    if (isset($_GET['nc']) && $_GET['nc']) {
        $frmNotaCredito->reset();
        echo $frmNotaCredito->showForm();
        die;
    }

    //Chamada para persistir cadastro de nota de crédito
    if (isset($_POST['nc']) && $_POST['nc']) {
        unset($_POST['nc']);

        if ($frmNotaCredito->isValid($_POST)) {
            $frmNotaCredito->reset();
            Ted_Utils_Model::reloadPage('Nota de rédito salva com sucesso!', $urlPage.'&ted='.$_GET['ted']);
        } else {
            $frmNotaCredito->populate($_POST);
            echo $frmNotaCredito->showForm();
        }
        die;
    }

    //Combo dependente do formulário de previsão orçamentária
    if (isset($_GET['programaTrabalho']) && !is_null($_GET['programaTrabalho'])) {
        $ptrid = (int) $_GET['programaTrabalho'];
        echo json_encode(array(
            'acao' => $prevOrcamentaria->capturaNomeAcaoPtrid($ptrid),
            'descricao' => utf8_encode($prevOrcamentaria->capturaDescricaoAcaoPtrid($ptrid)),
            'planoInterno' => $prevOrcamentaria->capturaListaPlanoInternoPtrid($ptrid)
        ));
        die;
    }

    //Chama formulário de remanejamento de crédito
    if (isset($_GET['retirada']) && is_numeric($_GET['retirada'])) {
        $frmRemanejarCredito->getElement('_proid_')->setValue($_GET['retirada']);
        echo $frmRemanejarCredito->showForm();
        die;
    }

    //Chama log de créditos remanejados
    if (isset($_GET['log']) && is_numeric($_GET['log'])) {
        echo $prevOrcamentaria->showLogTable($_GET['log']);
        die;
    }

    //Chama log de créditos remanejados
    if (isset($_GET['register']) && is_numeric($_GET['register'])) {
        echo $prevOrcamentaria->printLogTable($_GET['register']);
        die;
    }

    //Recebe POST do form de remanejamento de crédito
    if (isset($_POST['submitRemaneja']) && $_POST['submitRemaneja']) {

        if ($frmRemanejarCredito->isValid($_POST)) {
            $frmRemanejarCredito->reset();
        } else {
            $frmRemanejarCredito->populate($_POST);
            echo $frmRemanejarCredito->showForm();
        }
        die;
    }

} // fim da condicao que trata chamadas ajax