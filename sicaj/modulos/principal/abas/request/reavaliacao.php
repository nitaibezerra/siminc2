<?php

/*
 * Recebe e atualiza um pedido de reavaliação.
 */

if ('salvarPedidos' == $_POST['requisicao']) {
    $formInvalid = false;
    $message = '';

    if (equals_cod_sicaj($_POST['pdccodacaojudicial'], $_GET['id'])) {
        $message .= "O código SICAJ: \"{$_POST['pdccodacaojudicial']}\" já existe em nossa base de dados!<br>";
        $_POST['pdccodacaojudicial'] = '';
        $_SESSION['request']['post'] = $_POST;
        $formInvalid = true;
    }

    if ($formInvalid) {
        $_SESSION['flashmessagem'] = array(
            'message' => $message,
            'type' => Simec_Helper_FlashMessage::ERRO
        );

        die("<script type='text/javascript'>
            location.href='{$_SERVER['HTTP_REFERER']}';
        </script>");
    }

    if (!empty($_POST['pdcid'])) {
        atualizarPedido($_POST);
        $_SESSION['flashmessagem'] = array(
            'message' => 'Pedido atualizado com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );
    } else {
        $id = salvarPedido($_POST);
        $_SESSION['flashmessagem'] = array(
            'message' => 'Pedido criado com sucesso!',
            'type' => Simec_Helper_FlashMessage::SUCESSO
        );
    }

    echo "<script type='text/javascript'>
        location.href='{$_SERVER['HTTP_REFERER']}&id={$id}';
    </script>";
}