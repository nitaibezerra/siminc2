<?php

/**
 * Recebe o request e salva ou atualiza um pedido
 */
if ($_POST['requisicao'] == 'salvarPedidos') {
    $formInvalid = false;
    $message = '';

    if (equals_cod_sicaj($_POST['pdccodacaojudicial'], $_GET['id'])) {
        $message .= "O código SICAJ: \"{$_POST['pdccodacaojudicial']}\" já existe em nossa base de dados!<br>";
        $_POST['pdccodacaojudicial'] = '';
        $_SESSION['request']['post'] = $_POST;
        $formInvalid = true;
    }

    if ('H' == $_POST['pdctipo']) {
        if (!validaData($_POST['pdcdatainicio'])) {
            $_POST['pdcdatainicio'] = '';
            $_SESSION['request']['post'] = $_POST;
            $message .= 'Formato para data inválido, exemplo de formato aceito: "02/12/2014"<br>';
            $formInvalid = true;
        } else {
            $date = formataDataUs($_POST['pdcdatainicio']);
            $date = new DateTime($date);
            $dateNow = new DateTime();
        }

        if ($date > $dateNow) {
            $_POST['pdcdatainicio'] = '';
            $_SESSION['request']['post'] = $_POST;
            $message .= 'O pedido não pode ser cadastrado com data posterior a data atual!<br>';
            $formInvalid = true;
        }
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

/**
 * Verifica se o código sicaj já existe na base de dados
 * @param $pdcid
 * @param $codSicaj
 * @return bool
 */
function equals_cod_sicaj($acaojudicial, $id = null) {
    global $db;

    $strSQL = <<<DML
SELECT pdccodacaojudicial
  FROM sicaj.pedidocdo
    INNER JOIN workflow.documento doc USING(docid)
    WHERE pdccodacaojudicial = '%s'
      AND doc.esdid NOT IN(%d, %d, %d, %d)
DML;

    $strSQL = sprintf($strSQL, (string)$acaojudicial, HOMOLOGADO, PEDIDO_CANCELADO, HOMOLOGACAO_REFEITA, HOMOLOGACAO_ANULADA);

    if (null !== $id) {
        $strSQL.= sprintf(" AND pdcid NOT IN(%d)", (integer) $id);
    }

    return ($db->pegaUm($strSQL)) ? true : false;
}

/**
 * Persiste dados do pedido no banco
 * @param $request
 * @return bool
 */
function salvarPedido($request) {
    global $db;

    $pdcvalorcdo = limpaValorMonetario($request['pdcvalorcdo']);
    $request['pdcvalorexecpassado'] = limpaValorMonetario($request['pdcvalorexecpassado']);
    $request['pdcvalorexecatual'] = $pdcvalorcdo = limpaValorMonetario($request['pdcvalorexecatual']);

    require_once APPRAIZ . 'includes/workflow.php';
    $docid = wf_cadastrarDocumento(WF_TPDID_SICAJ, 'Decisões Judiciais');
    if (!$docid) {
        echo "<script>
            alert('Houver um erro ao tentar criar o documento!'); location.href='sicaj.php?modulo=inicio&acao=C';
        </script>";
    }

    if ('H' == $request['pdctipo']) {
        if (empty($request['pdcvalorexecpassado'])) {
            $request['pdcvalorexecpassado'] = 0;
        }
        if (empty($request['pdcvalorexecatual'])) {
            $request['pdcvalorexecatual'] = 0;
        }

        $sqlInsert = "
        INSERT INTO sicaj.pedidocdo(
          unicod,
          pdcnumprocessojudicial,
          pdccodacaojudicial,
          pdcjuizodacao,
          pdccodobjeto,
          pdcnumbeneficioacao,
          pdcdatainicio,
          pdcdatacadastro,
          pdcobservacao,
          pdcstatus,
          usucpf,
          docid,
          numprocessoadm,
          pdctipo,
          pdcvalorexecpassado,
          pdcvalorexecatual )
        VALUES(
          '{$request['unicod']}',
          '{$request['pdcnumprocessojudicial']}',
          '{$request['pdccodacaojudicial']}',
          '{$request['pdcjuizodacao']}',
          '{$request['pdccodobjeto']}',
          '{$request['pdcnumbeneficioacao']}',
          '". formataDataUs($request['pdcdatainicio']). "',
          'NOW()',
          '{$request['pdcobservacao']}',
          'A',
          '{$request['usucpf']}',
          $docid,
          '{$request['numprocessoadm']}',
          '{$request['pdctipo']}',
          {$request['pdcvalorexecpassado']},
          {$request['pdcvalorexecatual']})
          returning pdcid
    ";
    } else {
        $sqlInsert = <<<DML
INSERT INTO sicaj.pedidocdo(unicod, pdccodacaojudicial, usucpf, docid, pdctipo)
  VALUES('%s', '%s', '%s', %d, '%s')
  RETURNING pdcid
DML;
        $sqlInsert = sprintf(
            $sqlInsert,
            $request['unicod'],
            $request['pdccodacaojudicial'],
            $request['usucpf'],
            $docid,
            $request['pdctipo']
        );
    }

    $result = $db->pegaUm($sqlInsert);
    $db->commit();
    return ($result) ? $result : false;
}

/**
 * Persiste dados do pedido no banco
 * @param $request
 * @return bool
 */
function atualizarPedido($request) {
    global $db;

    $excluirCampos = array('usuemail', 'usufoneddd', 'usufonenum', 'pdcid', 'requisicao', 'usucpf');

    $sqlUpdate = 'UPDATE sicaj.pedidocdo SET ';
    foreach ($request as $k => $v) {
        if (!in_array($k, $excluirCampos)) {

            if ($k=='pdcvalorcdo' || $k == 'pdcvalorexecatual' || $k == 'pdcvalorexecpassado')
                $v = limpaValorMonetario($v);

            $v = trim($v);
            if($k == 'pdcdatainicio'){
                $sqlUpdate .= "{$k}='". formataDataUs($v). "', ";
            } else {
                $sqlUpdate .= "{$k}='{$v}', ";
            }
        }
    }

    $sqlUpdate = substr($sqlUpdate, 0 , -2);
    $sqlUpdate .= " where pdcid = {$request['pdcid']}";

    //ver($sqlUpdate, d);
    updateDadoUsuario($request['usuemail'], $request['usufoneddd'], $request['usufonenum']);
    $db->executar($sqlUpdate);
    $result = $db->commit();
    return ($result) ? true : false;
}

/**
 * Formata o valor monetario no formato do banco (float)
 * @param $valor
 * @return mixed
 */
function limpaValorMonetario($valor) {
    $pdcvalorcdo = str_replace('.', '', $valor);
    $pdcvalorcdo = str_replace(',', '.', $pdcvalorcdo);
    return $pdcvalorcdo;
}

/**
 * Atualiza alguns dados do usuário
 * @param $usuemail
 * @param $usufoneddd
 * @param $usufonenum
 */
function updateDadoUsuario($usuemail, $usufoneddd, $usufonenum) {
    global $db;

    $userSave = "
        select
          usucpf, usunome, usuemail, usufoneddd, usufonenum
        from seguranca.usuario
        where usuemail = '{$usuemail}'
    ";
    $registerSave = $db->pegaLinha($userSave);
    $update = false;

    if ($registerSave['usucpf'] == $_SESSION['usucpf']) {

        $strUpdate = 'UPDATE seguranca.usuario SET ';
        if ($registerSave['usuemail'] != $usuemail) {
            $strUpdate .= "usuemail = '{$usuemail}', ";
            $update = true;
        }
        if ($registerSave['usufoneddd'] != $usufoneddd) {
            $strUpdate .= "usufoneddd = '{$usufoneddd}', ";
            $update = true;
        }
        if ($registerSave['usufonenum'] != $usufonenum) {
            $strUpdate .= "usufonenum = '{$usufonenum}', ";
            $update = true;
        }

        $strUpdate = substr($strUpdate, 0 , -2);
        $strUpdate .= "WHERE usucpf = '{$_SESSION['usucpf']}'";
        if ($update) {
            $db->executar($strUpdate);
            $db->commit();
        }
    }
}
